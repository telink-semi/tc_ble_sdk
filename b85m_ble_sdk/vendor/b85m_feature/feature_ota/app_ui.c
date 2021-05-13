/********************************************************************************************************
 * @file	app_ui.c
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
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"

#include "app_config.h"
#include "app.h"
#include "../default_att.h"
#include "app_ui.h"
#include "app_ota_client.h"

#include "application/keyboard/keyboard.h"
#include "application/usbstd/usbkeycode.h"


#if (FEATURE_TEST_MODE == TEST_OTA)

int	master_pairing_enable = 0;
int master_unpair_enable = 0;

int master_disconnect_connhandle;   //mark the master connection which is in un_pair disconnection flow




int master_auto_connect = 0;
int user_manual_pairing;
















#if (UI_KEYBOARD_ENABLE)

_attribute_ble_data_retention_	int 	key_not_released;


#define CONSUMER_KEY   	   			1
#define KEYBOARD_KEY   	   			2
#define PAIR_UNPAIR_KEY   	   		3

_attribute_ble_data_retention_	u8 		key_type;

/**
 * @brief   Check changed key value.
 * @param   none.
 * @return  none.
 */
void key_change_proc(void)
{

	u8 key0 = kb_event.keycode[0];
//	u8 key1 = kb_event.keycode[1];
//	u8 key_buf[8] = {0,0,0,0,0,0,0,0};

	key_not_released = 1;
	if (kb_event.cnt == 2)   //two key press
	{

	}
	else if(kb_event.cnt == 1)
	{
		static u32 last_singleKey_press_tick;


		if(key0 == BTN_OTA_1 || key0 == BTN_OTA_2)
		{
			#if (BLE_OTA_CLIENT_ENABLE)

				if(blotaClt.ota_test_mode == 0){
					static u32 button_history = 0;
					if(!clock_time_exceed(last_singleKey_press_tick, 2000000)){
						button_history = button_history<<1 | (key0 == BTN_OTA_1 || key0 == BTN_OTA_2);
						if( (button_history & 0x0f) == 0x0f){
							blotaClt.ota_test_mode = 1;  //enter OTA test mode, LED shine
							ota_trigger_tick = clock_time() | 1;
							my_dump_str_data(OTA_DUMP_EN,"OTA test mode", 0, 0);
						}
					}
					else{
						button_history = 0;
					}
				}
				else if(blotaClt.ota_test_mode == 1){
					if(conn_master_num){ //at least one connection exist
						if(key0 == BTN_OTA_1){
							app_key_trigger_ota_start(NEW_FW_ADDR0, conn_dev_list[0].conn_handle);
						}
						else if(key0 == BTN_OTA_2){
							app_key_trigger_ota_start(NEW_FW_ADDR1, conn_dev_list[0].conn_handle);
						}
					}
				}



			#endif
		}
		if(key0 == BTN_PAIR)   //Manual pair triggered by Key Press
		{
			master_pairing_enable = 1;
			my_dump_str_data(APP_DUMP_EN, "UI PAIR begin", 0, 0);
		}
		else if(key0 == BTN_UNPAIR) //Manual un_pair triggered by Key Press
		{
			/*Here is just Telink Demonstration effect. Cause the demo board has limited key to use, only one "un_pair" key is
			 available. When "un_pair" key pressed, we will choose and un_pair one device in connection state */
			if(conn_master_num){ //at least 1 master connection exist

				if(!master_disconnect_connhandle){  //if one master un_pair disconnection flow not finish, here new un_pair not accepted

					/* choose one master connection to disconnect */
					for(int i=0; i < MASTER_MAX_NUM; i++){ //slave index is from 0 to "MASTER_MAX_NUM - 1"
						if(conn_dev_list[i].conn_state){
							master_unpair_enable = conn_dev_list[i].conn_handle;  //mark connHandle on master_unpair_enable
							break;
						}
					}
				}
			}
		}

		last_singleKey_press_tick = clock_time();  //update single key press tick
	}
	else   //kb_event.cnt == 0,  key release
	{
		if(master_pairing_enable){
			master_pairing_enable = 0;
		}

		if(master_unpair_enable){
			master_unpair_enable = 0;
		}
	}
}



_attribute_ble_data_retention_		static u32 keyScanTick = 0;

/**
 * @brief      keyboard task handler
 * @param[in]  e    - event type
 * @param[in]  p    - Pointer point to event parameter.
 * @param[in]  n    - the length of event parameter.
 * @return     none.
 */
void proc_keyboard (u8 e, u8 *p, int n)
{
	if(clock_time_exceed(keyScanTick, 10 * 1000)){  //keyScan interval: 10mS
		keyScanTick = clock_time();
	}
	else{
		return;
	}

	kb_event.keycode[0] = 0;
	int det_key = kb_scan_key (0, 1);

	if (det_key){
		key_change_proc();
	}
}




#endif   //end of UI_KEYBOARD_ENABLE







_attribute_ble_data_retention_	u8 		ota_is_working = 0;


/**
 * @brief      this function is used to register the function for OTA start.
 * @param[in]  none
 * @return     none
 */
void app_enter_ota_mode(void)
{
	ota_is_working = 1;

	#if(UI_LED_ENABLE)  //this is only for debug
		gpio_write(GPIO_LED_BLUE, 1);
	#endif
}






/**
 * @brief       no matter whether the OTA result is successful or fail.
 *              code will run here to tell user the OTA result.
 * @param[in]   result    OTA result:success or fail(different reason)
 * @return      none
 */
void app_ota_result(int result)
{
	#if (1)  //this is only for debug
		if(result == OTA_SUCCESS){  //led for debug: OTA success
			gpio_write(GPIO_LED_GREEN, 1);
		}
		else{  //OTA fail

			gpio_write(GPIO_LED_GREEN, 0);

			#if 0 //this is only for debug,  can not use this in application code
				irq_disable();
				WATCHDOG_DISABLE;

				while(1){
				}
			#endif
		}
	#endif

	gpio_write(GPIO_LED_BLUE, 0);
	gpio_write(GPIO_LED_WHITE, 1);

//	sleep_us(2000000);  //debug

//	BLMS_ERR_DEBUG(1, result);   //debug
}



void app_ota_server_init(void)
{
	/* OTA module initialization must be called after "blc_ota_setFirmwareSizeAndBootAddress"(if used), and before other OTA API.*/
	blc_ota_initOtaServer_module();

	blc_ota_registerOtaStartCmdCb(app_enter_ota_mode);
	blc_ota_registerOtaResultIndicationCb(app_ota_result);
	blc_ota_setOtaProcessTimeout(20);   //OTA process timeout:  20 seconds
	blc_ota_setOtaDataPacketTimeout(3); //OTA data packet interval timeout 3 seconds
}




/**
 * @brief   BLE Unpair handle for master
 * @param   none.
 * @return  none.
 */
void proc_master_role_unpair(void)
{
#if (!BLE_MASTER_SMP_ENABLE)
    if(blm_manPair.manual_pair && clock_time_exceed(blm_manPair.pair_tick, 2000000)){  //@@
    	blm_manPair.manual_pair = 0;
    }
#endif


	//terminate and un_pair process, Telink demonstration effect: triggered by "un_pair" key press
	if(master_unpair_enable){

		dev_char_info_t* dev_char_info = dev_char_info_search_by_connhandle(master_unpair_enable); //connHandle has marked on on master_unpair_enable

		if( dev_char_info ){ //un_pair device in still in connection state

			if(blc_ll_disconnect(master_unpair_enable, HCI_ERR_REMOTE_USER_TERM_CONN) == BLE_SUCCESS){

				master_disconnect_connhandle = master_unpair_enable; //mark conn_handle

				master_unpair_enable = 0;  //every "un_pair" key can only triggers one connection disconnect


				// delete this device information(mac_address and distributed keys...) on FLash
				#if (BLE_MASTER_SMP_ENABLE)
					blc_smp_deleteBondingSlaveInfo_by_PeerMacAddress(dev_char_info->peer_adrType, dev_char_info->peer_addr);
				#else
					user_tbl_slave_mac_delete_by_adr(dev_char_info->peer_adrType, dev_char_info->peer_addr);
				#endif
			}

		}
		else{ //un_pair device can not find in device list, it's not connected now

			master_unpair_enable = 0;  //every "un_pair" key can only triggers one connection disconnect
		}

	}
}








#endif
