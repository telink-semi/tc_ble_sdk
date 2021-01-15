/********************************************************************************************************
 * @file     app_buffer.h
 *
 * @brief    for TLSR chips
 *
 * @author	 BLE Group
 * @date     June. 2, 2020
 *
 * @par      Copyright (c) Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *			 The information contained herein is confidential and proprietary property of Telink
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in.
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/

#if (FEATURE_TEST_MODE == TEST_EXT_SCAN)

#ifndef APP_BUFFER_H_
#define APP_BUFFER_H_

#include "tl_common.h"

#include "app_config.h"



/********************* ACL connection LinkLayer TX & RX data FIFO allocation, Begin *******************************/

#define ACL_RX_FIFO_SIZE		64
#define ACL_RX_FIFO_NUM			8

#define ACL_TX_FIFO_SIZE		40
#define ACL_TX_FIFO_NUM			8

extern	u8	app_acl_rxfifo[];
extern	u8	app_acl_mstTxfifo[];
extern	u8	app_acl_slvTxfifo[];

/******************** ACL connection LinkLayer TX & RX data FIFO allocation, End ***********************************/



/***************** ACL connection L2CAP layer MTU TX & RX data FIFO allocation, Begin ********************************/

/*Note:
 * MTU Buff size = Extra_Len(10)+ ATT_MTU_MAX(23) then align 4bytes
 */


#define	MTU_M_BUFF_SIZE_MAX			CAL_MTU_BUFF_SIZE(23)

/** if support LE Secure Connections, L2CAP buffer must >= 76.[65+10,align 4bytes] */
#define MTU_S_BUFF_SIZE_MAX			CAL_MTU_BUFF_SIZE(23)


extern	u8 mtu_m_rx_fifo[];

extern	u8 mtu_s_rx_fifo[];
extern	u8 mtu_s_tx_fifo[];
/***************** ACL connection L2CAP layer MTU TX & RX data FIFO allocation, End **********************************/





#endif /* APP_BUFFER_H_ */


#endif // of of (FEATURE_TEST_MODE == TEST_EXT_SCAN)
