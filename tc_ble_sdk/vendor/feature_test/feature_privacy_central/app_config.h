/********************************************************************************************************
 * @file    app_config.h
 *
 * @brief   This is the header file for BLE SDK
 *
 * @author  BLE GROUP
 * @date    06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#pragma once

#include "../feature_config.h"

#if (FEATURE_TEST_MODE == TEST_PRIVACY_CENTRAL)


#define ACL_CENTRAL_MAX_NUM							1 // ACL central maximum number
#define ACL_PERIPHR_MAX_NUM							0 // ACL peripheral maximum number




///////////////////////// Feature Configuration////////////////////////////////////////////////
#define ACL_CENTRAL_SMP_ENABLE						1   //1 for smp,  0 no security
#define ACL_CENTRAL_SIMPLE_SDP_ENABLE				0   //simple service discovery for ACL central

#define BLE_APP_PM_ENABLE							0

///////////////////////// ADV Configuration ///////////////////////////////////////////////
#define LEGACY_SCAN_TEST								0	//Legacy Advertising test
#define EXTEND_SCAN_TEST								1   //Extend Advertising test
/* for user to select ADV test mode*/
#define	PRIVACY_TEST_MODE								LEGACY_SCAN_TEST

#define APP_DEFAULT_BUFFER_ACL_OCTETS_MTU_SIZE_MINIMUM		1
#define APP_DEFAULT_HID_BATTERY_OTA_ATTRIBUTE_TABLE			1


///////////////////////// DEBUG  Configuration ////////////////////////////////////////////////
#define DEBUG_GPIO_ENABLE							0
#define UART_PRINT_DEBUG_ENABLE                     1  //printf

#define APP_LOG_EN							1
#define APP_SMP_LOG_EN						0
#define APP_KEY_LOG_EN						1
#define APP_CONTR_EVENT_LOG_EN				1  //controller event log
#define APP_HOST_EVENT_LOG_EN				1  //host event log

#define APP_DEFAULT_HID_BATTERY_OTA_ATTRIBUTE_TABLE			1


/////////////////////// Board Select Configuration ///////////////////////////////
#if (CHIP_TYPE == CHIP_TYPE_825x)
	//Only support BOARD_825X_EVK_C1T139A30
	#define BOARD_SELECT							BOARD_825X_EVK_C1T139A30
#elif (CHIP_TYPE == CHIP_TYPE_827x)
	//Only support BOARD_827X_EVK_C1T197A30
	#define BOARD_SELECT							BOARD_827X_EVK_C1T197A30
#elif (CHIP_TYPE == CHIP_TYPE_TC321X)
	//Only support BOARD_TC321X_EVK_C1T357A20
#define BOARD_SELECT								BOARD_TC321X_EVK_C1T357A20
#endif


///////////////////////// UI Configuration ////////////////////////////////////////////////////
#define UI_LED_ENABLE								1
#define	UI_KEYBOARD_ENABLE							1


/////////////////// Clock  /////////////////////////////////
#define CLOCK_SYS_CLOCK_HZ  							32000000



/////////////////////////////////////// PRINT DEBUG INFO ///////////////////////////////////////
#if (UART_PRINT_DEBUG_ENABLE)
	#define DEBUG_INFO_TX_PIN           		GPIO_PA0
	#define PULL_WAKEUP_SRC_PA0         		PM_PIN_PULLUP_10K
	#define PA0_OUTPUT_ENABLE         			1
	#define PA0_DATA_OUT                     	1 //must
#endif




#include "../../common/default_config.h"


#endif
