/********************************************************************************************************
 * @file	vcp.h
 *
 * @brief	This is the header file for BLE SDK
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
#ifndef _VCP_H_
#define _VCP_H_


#if (AUDIO_VCP_ENABLE)



#define VCP_OPCODE_RELATIVE_VOLUME_DOWN           0x00
#define VCP_OPCODE_RELATIVE_VOLUME_UP             0x01
#define VCP_OPCODE_UNMUTE_RELATIVE_VOLUME_DOWN    0x02
#define VCP_OPCODE_UNMUTE_RELATIVE_VOLUME_UP      0x03
#define VCP_OPCODE_SET_ABSOLUTE_VOLUME            0x04
#define VCP_OPCODE_UNMUTE                         0x05
#define VCP_OPCODE_MUTE                           0x06

#define VCP_ERRCODE_INVALID_CHANGE_COUNTER        0x80
#define VCP_ERRCODE_OPCODE_NOT_SUPPORTED          0x81


#define VCP_FLAG_NONE             0x0000
#define VCP_FLAG_STATE_CHANGE     0x0001


typedef struct{
	u16 statHandle;
	u16 ctrlHandle;
	u8 *pState;
	u8 *pStateCCC;
}vcp_ctrl_t;


int blc_audioVcpRead(u16 connHandle, void *p);
int blc_audioVcpWrite(u16 connHandle, void *p);



#endif


#endif //_VCP_H_

