/********************************************************************************************************
 * @file	hci_h5.c
 *
 * @brief	This is the source file for BLE SDK
 *
 * @author	BLE GROUP
 * @date	2020.06
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions
 *              in binary form must reproduce the above copyright notice, this list of
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *
 *              3. Neither the name of TELINK, nor the names of its contributors may be
 *              used to endorse or promote products derived from this software without
 *              specific prior written permission.
 *
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or
 *              relating to such deletion(s), modification(s) or alteration(s).
 *
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************************************/
#include "hci_h5.h"
#include "hci_tr_api.h"
#include "hci_slip.h"
#include "hci_tr_def.h"

/*! H5 protocol define configuration. */
static HciH5Config_t hciH5Config = {
	.slidWinSize       = HCI_H5_SLIDING_WIN_SIZE,
	.oofFlowCtrl       = HCI_H5_OOF_FLW_CTRL,
	.dataIntgrtChkType = HCI_H5_DATA_INTEGRITY_LEVEL,
	.version           = HCI_H5_VERSION,
};


/*! H5 transport tx buffer define. */
u8 h5TxBuf[HCI_H5_TX_BUF_SIZE] = {0};

/*! HCI H5 Main Conrol Block. */
typedef struct{
	hci_fifo_t    *pHciRxFifo;
	hci_fifo_t    *pHciTxFifo;
	u8            *pTxBuf;
	hci_fifo_t    *pTxQ;

	HciH5Config_t  config;
	u32            tick;
	u8             linkState;
	u8             rxSlidWinSize; /*!< range 1~7 */
	u8             txSlidWinSize; /*!< range 1~7 */
	u8             txSeq;
	u8             txAck;
	u8             rxAck;
	u8             recvd[2];
}HciH5Cb_t;

static HciH5Cb_t hciH5Cb;

/*! function declaration. */
static void HCI_H5_PakcetHandler(u8 *pPacket, u32 len);

/**
 * @brief : H5 protocol initialization.
 * @param : pHciRxFifo    Pointer point to hci rx FIFO.
 * @param : pHciTxFifo    Pointer point to hci tx FIFO.
 * @return: none.
 */
void HCI_H5_Init(hci_fifo_t *pHciRxFifo, hci_fifo_t *pHciTxFifo)
{
	//hciH5Config = *defConfig;

	hciH5Cb.pHciRxFifo = pHciRxFifo;
	hciH5Cb.pHciTxFifo = pHciTxFifo;
	hciH5Cb.pTxBuf     = h5TxBuf;

	hciH5Cb.linkState     = HCI_H5_LINK_STATE_IDLE;
	hciH5Cb.rxSlidWinSize = 0;
	hciH5Cb.txSlidWinSize = 0;
	hciH5Cb.txSeq = 0;
	hciH5Cb.txAck = 0;
	hciH5Cb.rxAck = 0;
	hciH5Cb.tick = clock_time()|1;

	/* Register Slip handler. */
	HCI_Slip_RegisterPktHandler(HCI_H5_PakcetHandler);

	HCI_SLip_SetFlowCtrlEnable(false);
}

/**
 * @brief : H5 protocol reset.
 * @param : none.
 * @return: none.
 */
void HCI_H5_Reset(void)
{
	hciH5Cb.rxSlidWinSize = 0;
	hciH5Cb.txSlidWinSize = 0;
	hciH5Cb.txSeq = 0;
	hciH5Cb.txAck = 0;
	hciH5Cb.rxAck = 0;
	hciH5Cb.tick = clock_time()|1;

	HCI_SLip_SetFlowCtrlEnable(false);
}

/**
 * @brief : CRC calculate.
 * @param : crcInit    CRC initialization value.
 * @param : pPacket    Pointer point to buffer.
 * @param : len        the length of data.
 * @return: crc value.
 */
u16 HCI_H5_CcittCrc16Calc(u16 crcInit, u8 *pPacket, u32 len)
{
	u16 crc = crcInit;
	u16 crcPoly = 0x8408;

	while(len--)
	{
		crc ^= *pPacket++;
		for(int i=0; i<8; i++)
		{
			if(crc & 1){
				crc = (crc>>1) ^ crcPoly;
			}else{
				crc = (crc>>1);
			}
		}
	}
	return crc;
}

/**
 * @brief : Checksum calculate.
 * @param : p       Pointer point to buffer.
 * @param : len     the length of data.
 * @return: none.
 */
u8 HCI_H5_CheckSumCalc(u8 *p, u32 len)
{
	ASSERT(len == 3, HCI_TR_ERR_H5_HEAD_LEN);

#if 0
	u32 sum = 0;
	while(len--){
		sum += *p++;
	}

	u32 err = (sum >= 255) ? (sum - 255):(255 - sum);
	u16 x = (err>>8);
	if(x == 0) x = 1;

	return ((x<<8) + 255 - sum);
#else
	return ~((p[0] + p[1] + p[2]) & 0xff);
#endif
}

/**
 * @brief : Check checksum.
 * @param : p       Pointer point to buffer.
 * @param : len     The length of data.
 * @return: none.
 */
bool HCI_H5_IsCheckSumValid(u8 *p, u32 len)
{
	ASSERT(len == 4, HCI_TR_ERR_H5_HEAD_LEN);

	u32 sum = 0;
	while(len--){
		sum += *p++;
	}
	return (sum % 256) == 0xFF ? true:false;
}

/**
 * @brief : Pack H5 head.
 * @param : pBuf    Pointer point to buffer
 * @param : pHead   Pointer point to HciH5Head_t .
 * @return: none.
 */
u16 HCI_H5_PackHeader(u8 *pBuf, HciH5Head_t *pHead)
{
	u8 *p = pBuf;

	u8  seqNum     = pHead->seqNum;
	u8  ackNum     = pHead->ackNum;
	u8  crc        = pHead->crc;
	u8  reliable   = pHead->reliable;
	u8  pktType    = pHead->pktType;
	u16 payloadLen = pHead->payloadLen;

	u16 tmp = (seqNum<<0) | (ackNum<<3) | (crc<<6) | (reliable<<7);
	UINT8_TO_BSTREAM(p, tmp);

	tmp = pktType | ((payloadLen<<4) & 0x0fff);
	UINT16_TO_BSTREAM(p, tmp);

	u8 checkSum = HCI_H5_CheckSumCalc(pBuf, 3);
	UINT8_TO_BSTREAM(p, checkSum);

	return HCI_H5_HEAD_LEN;
}

/**
 * @brief : Unpack H5 head.
 * @param : pPacket     Pointer point to buffer
 * @param : hciH5Head   Pointer point to HciH5Head_t .
 * @return: none.
 */
void HCI_H5_UnpackHeader(HciH5Head_t *hciH5Head, u8 *pPacket)
{
	u32 head = 0;
	BSTREAM_TO_UINT32(head, pPacket);

	hciH5Head->seqNum        = H5_HDR_SEQ(head);
	hciH5Head->ackNum        = H5_HDR_ACK(head);
	hciH5Head->crc           = H5_HDR_CRC(head);
	hciH5Head->reliable      = H5_HDR_RELIABLE(head);
	hciH5Head->pktType       = H5_HDR_PKT_TYPE(head);
	hciH5Head->payloadLen    = H5_HDR_LEN(head);
	hciH5Head->headCheckSum  = H5_HDR_CHECKSUM(head);
}

/**
 * @brief : Check reliable packet type.
 * @param : type     H5 protocol packet type.
 * @return: true if reliable or false.
 */
static bool HCI_H5_IsReliablePacket(u8 type)
{
	switch(type)
	{
	case HCI_H5_PKT_TYPE_CMD:
	case HCI_H5_PKT_TYPE_ACL:
	case HCI_H5_PKT_TYPE_EVT:
		return true;
	default:
		return false;
	}
}

/**
 * @brief : Send H5 protocol packet.
 * @param : h5Type     H5 protocol packet type.
 * @param : pPacket    Pointer point to buffer.
 * @param : len        The length of data.
 * @return: true if send successfully or false.
 */
bool HCI_H5_Send(u8 h5Type, u8 *pPacket, u32 len)
{
	HciH5Head_t hciH5Head = {0,0,0,0,0,0,0};
	hciH5Head.ackNum        = hciH5Cb.txAck;
	hciH5Head.pktType       = h5Type;
	hciH5Head.payloadLen    = len;

	if(HCI_H5_IsReliablePacket(h5Type)){
		hciH5Head.reliable = 1;
		hciH5Head.seqNum   = hciH5Cb.txSeq;
		hciH5Cb.txSeq = (hciH5Cb.txSeq + 1) % 8;
	}

	if(h5Type != HCI_H5_PKT_TYPE_ACK && h5Type != HCI_H5_PKT_TYPE_LINK_CTRL){
		hciH5Head.crc = hciH5Cb.config.dataIntgrtChkType;
	}

	u8 *pBuf = hciH5Cb.pTxBuf;
	pBuf += HCI_H5_PackHeader(pBuf, &hciH5Head);

	if(pPacket == NULL || len == 0){

	}
	else
	{
		if(len > HCI_H5_TX_BUF_SIZE-HCI_H5_HEAD_LEN){
			H5_TRACK_ERR("H5 Tx buffer oveflow...\n");
			ASSERT(false, HCI_TR_ERR_H5_TX_BUF_OVFL);
		}
		memcpy(pBuf, pPacket, len);
		pBuf += len;
	}

	if(hciH5Head.crc){
		u16 crc = HCI_H5_CcittCrc16Calc(0xFFFF, hciH5Cb.pTxBuf, HCI_H5_HEAD_LEN + len);
		*pBuf++ = (crc>>8) & 0xFF;
		*pBuf++ =  crc & 0xFF;
	}

	u16 pktLen = HCI_H5_HEAD_LEN + len + (hciH5Head.crc ? HCI_H5_CRC_LEN:0);
    return HCI_Slip_Send(hciH5Cb.pTxBuf, pktLen);
}

/**
 * @brief : Send HCI packet.
 * @param : none.
 * @return: none.
 */
void HCI_H5_SendData(void)
{
	hci_fifo_t *pHciTxFifo = hciH5Cb.pHciTxFifo;
	if(pHciTxFifo->wptr == pHciTxFifo->rptr+hciH5Cb.txSlidWinSize){
		return;//have no data to send.
	}

	u8  r = pHciTxFifo->rptr + hciH5Cb.txSlidWinSize;
	u8 *p = pHciTxFifo->p + (r & pHciTxFifo->mask) * pHciTxFifo->size;
	hciH5Cb.txSlidWinSize++;

	u16 len = 0;
	u8  hciType = 0;
	BSTREAM_TO_UINT16(len, p);
	BSTREAM_TO_UINT8(hciType, p);

	bool res = HCI_H5_Send(hciType, p, len - 1);

	H5_TRACK_INFO("Hci Type: 0x%02X\n H5 Tx data: ", hciType);
	HCI_TRACK_DATA((p-1), len);

	if(res == false){
		hciH5Cb.txSeq = (hciH5Cb.txSeq - 1) & 0x07;
		hciH5Cb.txSlidWinSize--;
	}
	else{
		hciH5Cb.rxSlidWinSize = 0;
	}
}

/**
 * @brief : Send H5 pure ACK packet.
 * @param : none.
 * @return: none.
 */
bool HCI_H5_SendPureAck(void)
{
	return HCI_H5_Send(HCI_H5_PKT_TYPE_ACK, NULL, 0);
}

/**
 * @brief : Send H5 SYNC Message packet.
 * @param : none.
 * @return: none.
 */
void HCI_H5_SendSync(void)
{
	u16 payload = HCI_H5_MSG_SYNC;
	HCI_H5_Send(HCI_H5_PKT_TYPE_LINK_CTRL, (u8*)&payload, 2);
}

/**
 * @brief : Send H5 SYNC_RSP Message packet.
 * @param : none.
 * @return: none.
 */
void HCI_H5_SendSyncRsp(void)
{
	u16 payload = HCI_H5_MSG_SYNC_RSP;
	HCI_H5_Send(HCI_H5_PKT_TYPE_LINK_CTRL, (u8*)&payload, 2);
}

/**
 * @brief : Send H5 CONFIG Message packet.
 * @param : none.
 * @return: none.
 */
void HCI_H5_SendConfig(void)
{
	u8 payload[3]={0};
	u8 *pBuf = payload;

	UINT16_TO_BSTREAM(pBuf, HCI_H5_MSG_CONFIG);

	u8 config = (hciH5Config.slidWinSize) | (hciH5Config.oofFlowCtrl<<3) |
			    (hciH5Config.dataIntgrtChkType<<4) | (hciH5Config.version<<5);
	UINT8_TO_BSTREAM(pBuf, config);

    HCI_H5_Send(HCI_H5_PKT_TYPE_LINK_CTRL, payload, sizeof(payload));
}

/**
 * @brief : Send H5 CONFIG_RSP Message packet.
 * @param : none.
 * @return: none.
 */
void HCI_H5_SendConfigRsp(void)
{
	u8 payload[3] = {0};
	u8 *pBuf = payload;

	UINT16_TO_BSTREAM(pBuf, HCI_H5_MSG_CONFIG_RSP);

	u8 config = (hciH5Config.slidWinSize) | (hciH5Config.oofFlowCtrl<<3) |
			    (hciH5Config.dataIntgrtChkType<<4) | (hciH5Config.version<<5);
	UINT8_TO_BSTREAM(pBuf, config);

	HCI_H5_Send(HCI_H5_PKT_TYPE_LINK_CTRL, payload, sizeof(payload));
}

/**
 * @brief : Decode H5 link control packet.
 * @param : pHciH5Head    Pointer point to H5 head.
 * @param : pPacket       Pointer point to buffer.
 * @param : len           The length of data.
 * @return: none.
 */
void HCI_H5_DecodeLinkPdu(HciH5Head_t *pHciH5Head, u8 *pPacket, u32 len)
{
	HciH5Config_t *pH5Config = &hciH5Cb.config;
	u8 *p = pPacket + HCI_H5_HEAD_LEN;

	u16 msg = 0;
	u8 cfg = 0;
	BSTREAM_TO_UINT16(msg, p);
	BSTREAM_TO_UINT8(cfg, p);

	switch(hciH5Cb.linkState)
	{
	case HCI_H5_LINK_STATE_IDLE:
		if(msg == HCI_H5_MSG_SYNC)//SYNC message.
		{
			HCI_H5_SendSyncRsp();
			H5_TRACK_INFO("Send SYNC_RSP Message...\n");
		}
		else if(msg == HCI_H5_MSG_SYNC_RSP)//SYNC Rsp message.
		{
			hciH5Cb.linkState = HCI_H5_LINK_STATE_INIT;
			H5_TRACK_INFO("Switch to state: INIT...\n");

			HCI_H5_SendConfig();
			H5_TRACK_INFO("Send CONFIG Message...\n");
		}
		else //invalid packet.
		{
			HCI_H5_SendSync();
			H5_TRACK_INFO("Send SYNC Message...\n");
		}
		break;

	case HCI_H5_LINK_STATE_INIT:
		if(msg == HCI_H5_MSG_SYNC)//SYNC message.
		{
			HCI_H5_SendSyncRsp();
			H5_TRACK_INFO("Send SYNC_RSP Message...\n");
		}
		else if(msg == HCI_H5_MSG_CONFIG)
		{
			HCI_H5_SendConfigRsp();
			H5_TRACK_INFO("Send CONFIG_RSP Message...\n");
		}
		else if(msg == HCI_H5_MSG_CONFIG_RSP)
		{
			hciH5Cb.linkState = HCI_H5_LINK_STATE_ACTIVE;
			HCI_H5_Reset();

			/* Set config info. */
			u8 slidSize  = min(hciH5Config.slidWinSize, (cfg&0x07));
			u8 oofFlow   = hciH5Config.oofFlowCtrl & ((cfg>>3)&0x01);
			u8 dataCheck = hciH5Config.dataIntgrtChkType & ((cfg>>4)&0x01);
			u8 version   = min(hciH5Config.version,((cfg>>5)&0x07));

			pH5Config->slidWinSize       = slidSize;
			pH5Config->oofFlowCtrl       = oofFlow;
			pH5Config->dataIntgrtChkType = dataCheck;
			pH5Config->version           = version;

			H5_TRACK_INFO("Switch to state: ACTIVE...\n");
		}
		else
		{
			//spec: All other messages that are received must be ignored.
		}
		break;

	case HCI_H5_LINK_STATE_ACTIVE:
		if(msg == HCI_H5_MSG_CONFIG)
		{
			HCI_H5_SendConfigRsp();
			H5_TRACK_INFO("Send CONFIG_RSP Message...\n");
		}
		else if(msg == HCI_H5_MSG_SYNC)
		{
			hciH5Cb.linkState  = HCI_H5_LINK_STATE_IDLE;
			HCI_H5_Reset();
			H5_TRACK_INFO("Reset to state: IDLE...\n");

			HCI_H5_SendSyncRsp();
			H5_TRACK_INFO("Send SYNC_RSP Message...\n");
		}
		else
		{
		}
		break;

	default:
		H5_TRACK_WRN("Invalid link packet...\n");
		break;
	}
}

/**
 * @brief : Pack H5 packet to HCI controller.
 * @param : hciType       HCI type.
 * @param : pPacket       Pointer point to buffer.
 * @param : len           The length of data.
 * @return: none.
 */
void HCI_H5_PackHciPdu(u8 hciType, u8 *pPayload, u32 len)
{
	hci_fifo_t *pHciRxFifo = hciH5Cb.pHciRxFifo;

	u8 *p = pHciRxFifo->p + (pHciRxFifo->wptr & pHciRxFifo->mask)*pHciRxFifo->size;

	if(len+1 > pHciRxFifo->size){
		H5_TRACK_ERR("HCI Rx buffer overflow...\n");
		ASSERT(FALSE, HCI_TR_ERR_H5_HCI_RX_BUF_OVFL);
	}

	UINT8_TO_BSTREAM(p, hciType);
	memcpy(p, pPayload, len);
	pHciRxFifo->wptr++;

	H5_TRACK_INFO("data to hci buffer:");
	HCI_TRACK_DATA(p-1, len + 1);
}

/**
 * @brief : resend handler.
 * @param : none.
 * @return: none.
 */
void HCI_H5_ReSendCheck(void)
{
	hci_fifo_t *pHciTxFifo = hciH5Cb.pHciTxFifo;

	if(hciH5Cb.rxAck == hciH5Cb.txSeq)
	{
		H5_TRACK_INFO("local packet is received by peer corrently...\n");
		pHciTxFifo->rptr += hciH5Cb.txSlidWinSize;
		hciH5Cb.txSlidWinSize = 0;
	}
	else
	{
		H5_TRACK_INFO("local device need resend packet...\n");
		while(hciH5Cb.rxAck != hciH5Cb.txSeq){
			hciH5Cb.txSeq = (hciH5Cb.txSeq - 1) & 0x07;
			hciH5Cb.txSlidWinSize--;
		}
		pHciTxFifo->rptr += hciH5Cb.txSlidWinSize;
		hciH5Cb.txSlidWinSize = 0;
	}
}

/**
 * @brief : Decode H5 data packet.
 * @param : pHciH5Head    Pointer point to H5 head.
 * @param : pPacket       Pointer point to buffer.
 * @param : len           The length of data.
 * @return: none
 */
void HCI_H5_DecodeDataPdu(HciH5Head_t *pHciH5Head, u8 *pPacket, u32 len)
{
	H5_TRACK_INFO("=== H5 data parse start ===\n");

	HciH5Config_t *pH5Config = &hciH5Cb.config;
	if(!pH5Config->dataIntgrtChkType && pHciH5Head->crc){
		return;//discard
	}

	/* Check CRC */
	if(pHciH5Head->crc)
	{
		u16 pLen = len - HCI_H5_CRC_LEN;
		u16 crc = HCI_H5_CcittCrc16Calc(0x0000, pPacket, pLen);
		if((crc & 0xff) != *(pPacket + pLen+1) || ((crc>>8) & 0xff) != *(pPacket + pLen)){
			H5_TRACK_WRN("CRC error...\n");
			return;//discard
		}
	}

	/* Reliable transport */
	if(pHciH5Head->reliable)
	{
		H5_TRACK_INFO("This is H5 reliable packet...\n");

		if(pHciH5Head->seqNum == hciH5Cb.txAck)
		{
			hciH5Cb.txAck = (hciH5Cb.txAck + 1) % 8;
			hciH5Cb.rxSlidWinSize++;
			if(hciH5Cb.rxSlidWinSize >= pH5Config->slidWinSize){
				HCI_H5_SendPureAck();
				hciH5Cb.rxSlidWinSize = 0;
			}

			/* H5 to HCI */
			HCI_H5_PackHciPdu(pHciH5Head->pktType, pPacket+HCI_H5_HEAD_LEN, pHciH5Head->payloadLen);

			H5_TRACK_INFO("H5 RxSeq Ok...\n");
		}
		else
		{
			H5_TRACK_WRN("H5 RxSeq error, Need peer resend...\n");
		}

		/* Resend check. */
		hciH5Cb.rxAck = pHciH5Head->ackNum;
		HCI_H5_ReSendCheck();
	}
	else/* Unreliable transport. */
	{
		H5_TRACK_INFO("This is H5 unreliable packet...\n");

		hciH5Cb.txAck = (hciH5Cb.txAck + 1) % 8;
		/* H5 to HCI */
		HCI_H5_PackHciPdu(pHciH5Head->pktType, pPacket+HCI_H5_HEAD_LEN, pHciH5Head->payloadLen);
	}

	H5_TRACK_INFO("=== H5 data parse end ===\n");
}

/**
 * @brief : H5  packet handler.
 * @param : pPacket       Pointer point to buffer.
 * @param : len           The length of data.
 * @return: none
 */
void HCI_H5_PakcetHandler(u8 *pPacket, u32 len)
{
	H5_TRACK_INFO("==== H5 Rx Handler Start ====\n");

	/* Check Head Checksum. */
	if(!HCI_H5_IsCheckSumValid(pPacket, HCI_H5_HEAD_LEN)){
		H5_TRACK_WRN("Header checksum error...\n");
		return; //discard
	}

	HciH5Head_t hciH5Head;
	HCI_H5_UnpackHeader(&hciH5Head, pPacket);

	/* Check Payload length. */
	if(!hciH5Head.crc){
		if(HCI_H5_HEAD_LEN + hciH5Head.payloadLen != len){
			H5_TRACK_WRN("Payload length error(NO CRC)...\n");
			return;//discard.
		}
	}
	else{
		if(HCI_H5_HEAD_LEN + hciH5Head.payloadLen + HCI_H5_CRC_LEN != len){
			H5_TRACK_WRN("Payload length error(CRC)...\n");
			return;//discard.
		}
	}

	switch(hciH5Head.pktType)
	{
	case HCI_H5_PKT_TYPE_ACK:
		H5_TRACK_INFO("Pure ACK Packet:");
		HCI_TRACK_DATA(pPacket, len);
		/* Resend check. */
		hciH5Cb.rxAck = hciH5Head.ackNum;
		HCI_H5_ReSendCheck();
		break;

	case HCI_H5_PKT_TYPE_LINK_CTRL:
		H5_TRACK_INFO("Link Control Packet:");
		HCI_TRACK_DATA(pPacket, len);
		HCI_H5_DecodeLinkPdu(&hciH5Head, pPacket, len);
		break;

	case HCI_H5_PKT_TYPE_CMD:
	case HCI_H5_PKT_TYPE_ACL:
	case HCI_H5_PKT_TYPE_SCO:
	case HCI_H5_PKT_TYPE_EVT:
		if(hciH5Cb.linkState != HCI_H5_LINK_STATE_ACTIVE){
			return;
		}
		H5_TRACK_INFO("Data Packet:");
		HCI_TRACK_DATA(pPacket, len);
		HCI_H5_DecodeDataPdu(&hciH5Head, pPacket, len);
		break;

	case HCI_H5_PKT_TYPE_VENDOR:
	default:
		H5_TRACK_WRN("Invalid H5 packet:");
		HCI_TRACK_DATA(pPacket, len);
		break;
	}

	H5_TRACK_INFO("==== H5 Rx Handler End   ====\n\n");
}

/**
 * @brief : H5 protocol main loop.
 * @param : none.
 * @return: none
 */
void HCI_H5_Poll(void)
{
	if(hciH5Cb.linkState == HCI_H5_LINK_STATE_IDLE)
	{
		if(clock_time_exceed(hciH5Cb.tick, 250*1000))
		{
			HCI_H5_SendSync();
			hciH5Cb.tick = clock_time();
		}
	}
	else if(hciH5Cb.linkState == HCI_H5_LINK_STATE_INIT)
	{
		if(clock_time_exceed(hciH5Cb.tick, 250*1000))
		{
			HCI_H5_SendConfig();
		    hciH5Cb.tick = clock_time();
		}
	}
	else if(hciH5Cb.linkState == HCI_H5_LINK_STATE_ACTIVE)
	{
		HCI_H5_SendData();
	}
}


