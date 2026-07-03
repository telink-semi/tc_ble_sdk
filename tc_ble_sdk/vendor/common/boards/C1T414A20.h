/********************************************************************************************************
 * @file    C1T414A20.h
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
#ifndef C1T414A20_H_
#define C1T414A20_H_

#include "common/config/user_config.h"


#if(BOARD_SELECT == BOARD_TC123X_EVK_C1T414A20)

#if (UI_KEYBOARD_ENABLE)   // if test pure power, kyeScan GPIO setting all disabled
		//---------------  KeyMatrix PB2/PB3/PB4/PB5 -----------------------------
		#define	MATRIX_ROW_PULL					PM_PIN_PULLDOWN_100K
		#define	MATRIX_COL_PULL					PM_PIN_PULLUP_10K

		#define	KB_LINE_HIGH_VALID				0   //drive pin output 0 when keyscan, scanpin read 0 is valid

		#define BTN_PAIR						0x01
		#define BTN_UNPAIR						0x02

		#define	CR_VOL_UP						0xf0  ////
		#define	CR_VOL_DN						0xf1


		/**
		 *  @brief  Normal keyboard map
		 */
		#define		KB_MAP_NORMAL	{	{CR_VOL_UP,	BTN_PAIR} ,	 \
										{CR_VOL_DN,	BTN_UNPAIR},     }



		//////////////////// KEY CONFIG (EVK board) ///////////////////////////
		#define  KB_DRIVE_PINS  		{GPIO_PC1, GPIO_PC0}
		#define  KB_SCAN_PINS   		{GPIO_PA5, GPIO_PA4}

		//drive pin as gpio
		#define	PC1_FUNC				AS_GPIO
		#define	PC0_FUNC				AS_GPIO

		//drive pin need 100K pulldown
		#define	PULL_WAKEUP_SRC_PC1		MATRIX_ROW_PULL
		#define	PULL_WAKEUP_SRC_PC0		MATRIX_ROW_PULL

		//drive pin open input to read gpio wakeup level
		#define PC1_INPUT_ENABLE		1
		#define PC0_INPUT_ENABLE		1

		//scan pin as gpio
		#define	PA5_FUNC				AS_GPIO
		#define	PA4_FUNC				AS_GPIO

		//scan  pin need 10K pullup
		#define	PULL_WAKEUP_SRC_PA5		MATRIX_COL_PULL
		#define	PULL_WAKEUP_SRC_PA4		MATRIX_COL_PULL

		//scan pin open input to read gpio level
		#define PA5_INPUT_ENABLE		1
		#define PA4_INPUT_ENABLE		1
#endif

/**
 *  @brief  LED Configuration
 */
#if (UI_LED_ENABLE)
	/* 825X Dongle and 827X Dongle use same GPIO for LED: PA3/PB1/PA2/PB0/PA4 */
	#define	GPIO_LED_RED			GPIO_PB5
	#define	GPIO_LED_GREEN			GPIO_PB3
	#define	GPIO_LED_BLUE			GPIO_PB4
	#define	GPIO_LED_YELLOW			GPIO_PB6

	#define PB3_FUNC				AS_GPIO
	#define PB4_FUNC				AS_GPIO
	#define PB5_FUNC				AS_GPIO
	#define PB6_FUNC				AS_GPIO

	#define	PB3_OUTPUT_ENABLE		1
	#define	PB4_OUTPUT_ENABLE		1
	#define PB5_OUTPUT_ENABLE		1
	#define	PB6_OUTPUT_ENABLE		1

	#define LED_ON_LEVAL 			1 		//gpio output high voltage to turn on led
#endif

#if(DEBUG_GPIO_ENABLE)
	#define GPIO_CHN0							GPIO_PB3
	#define GPIO_CHN1							GPIO_PB4
	#define GPIO_CHN2							GPIO_PB5
	#define GPIO_CHN3							GPIO_PB6
	#define GPIO_CHN4							GPIO_PB7
	#define GPIO_CHN5							GPIO_PC1
	#define GPIO_CHN6							GPIO_PC0
	#define GPIO_CHN7							GPIO_PA5
	#define GPIO_CHN8							GPIO_PA4
	#define GPIO_CHN9							GPIO_PA2
	#define GPIO_CHN10							GPIO_PA1
	#define GPIO_CHN11							GPIO_PA7
	#define GPIO_CHN12							GPIO_PA6
	#define GPIO_CHN13							GPIO_PB2
	#define GPIO_CHN14							GPIO_PB1
	#define GPIO_CHN15							GPIO_PB0

	#define PB3_OUTPUT_ENABLE					1
	#define PB4_OUTPUT_ENABLE					1
	#define PB5_OUTPUT_ENABLE					1
	#define PB6_OUTPUT_ENABLE					1
	#define PB7_OUTPUT_ENABLE					1
	#define PC1_OUTPUT_ENABLE					1
	#define PC0_OUTPUT_ENABLE					1
	#define PA5_OUTPUT_ENABLE					1
	#define PA4_OUTPUT_ENABLE					1
	#define PA2_OUTPUT_ENABLE					1
	#define PA1_OUTPUT_ENABLE					1
	#define PA7_OUTPUT_ENABLE					1
	#define PA6_OUTPUT_ENABLE					1
	#define PB2_OUTPUT_ENABLE					1
	#define PB1_OUTPUT_ENABLE					1
	#define PB0_OUTPUT_ENABLE					1
#endif  //end of DEBUG_GPIO_ENABLE

/**
 *  @brief  Battery_check Configuration
 */
#if (APP_BATT_CHECK_ENABLE)
	//Telink device: you must choose one gpio with adc function to output high level(voltage will equal to vbat), then use adc to measure high level voltage
	//use PB0 output high level, then adc measure this high level voltage
	#define GPIO_VBAT_DETECT				ADC_GPIO_PB0
#endif

/**
 *  @brief  PRINT DEBUG INFO
 */
#if (UART_PRINT_DEBUG_ENABLE)
    #define DEBUG_INFO_TX_PIN               GPIO_PA0
    #define PULL_WAKEUP_SRC_PA0             PM_PIN_PULLUP_10K
    #define PA0_OUTPUT_ENABLE               1
    #define PA0_DATA_OUT                    1
	#define BIT_INTERVAL					(sys_tick_per_us*1000*1000/PRINT_BAUD_RATE)
#endif


#endif /* BOARD_SELECT == BOARD_TC123X_EVK_C1T414A20 */

#endif /* C1T414A20_H_ */
