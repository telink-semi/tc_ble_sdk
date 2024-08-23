/********************************************************************************************************
 * @file	ext_misc.c
 *
 * @brief	This is the header file for B91
 *
 * @author	BLE Group
 * @date	2020
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


#include "drivers/8258/driver_8258.h"
#include "ext_misc.h"

/*********************************************************** FLASH START********************************************/
_attribute_data_retention_   _attribute_aligned_(4)	Flash_CapacityDef	flash_capacity;

void flash_set_capacity(Flash_CapacityDef flash_cap)
{
	flash_capacity = flash_cap;
}

Flash_CapacityDef flash_get_capacity(void)
{
	return flash_capacity;
}
/*********************************************************** FLASH END********************************************/


/*********************************************************** ADC calibration start********************************************/

/**
 * @brief      This function is used to calib ADC 1.2V vref.
 * @param[in]  velfrom - the calibration value from flash or otp.
 * @param[in]  addr - the calibration value address of flash or otp.
 * @return 	   1 - the calibration value update, 0 - the calibration value is not update.
 */
unsigned char user_calib_adc_vref(user_calib_from_e velfrom, unsigned int addr)
{
/********************************************************************************************
	There have two kind of calibration value of ADC 1.2V vref in flash,and one calibration value in Efuse.
	Two kind of ADC calibration value in flash are two-point calibration adc_gpio_calib_vref_gain(used for gpio voltage sample)
	and one-point calibration adc_gpio_calib_vref_gain(used for gpio voltage sample).
	The ADC calibration value in Efuse is  adc_gpio_calib_vref_gain(used for gpio voltage sample).
	The efuse calibration value has a total of 8 bits, the value from bit[5] to bit[0] is the calibration value,
	bit[7] and bit[6] represent whether the calibration value is stored.
	The priority of adc_gpio_calib_vref_gain is: two-point calib from Flash > one-point calib from Flash > calib from Efuse > Default(1175mV).
********************************************************************************************/
	unsigned char adc_vref_calib_value[7] = {0};
	unsigned short gpio_calib_vref_gain = 0;
	signed char gpio_calib_vref_offset = 0;
	if(addr == 0)
	{
		/****** If flash check mid fail,use the Efuse gpio calibration value ********/
		gpio_calib_vref_gain = adc_vref_cfg.adc_vref;
		if((gpio_calib_vref_gain>=1100) && (gpio_calib_vref_gain<=1300))
		{
			adc_set_gpio_calib_vref(gpio_calib_vref_gain);
			return 1;
		}
	}
	else
	{
		if(velfrom == USER_CALIB_FROM_FLASH)
		{
			flash_read_page(addr, 7, adc_vref_calib_value);
		}
		/****** Check the two-point gpio calibration value whether is exist ********/
		if((adc_vref_calib_value[4] >= 0) && (adc_vref_calib_value[4] <= 127) && (((adc_vref_calib_value[6] << 8) + adc_vref_calib_value[5]) >= 47) && (((adc_vref_calib_value[6] << 8) + adc_vref_calib_value[5]) <= 300)){
			/****** Method of calculating two-point gpio calibration Flash_gain and Flash_offset value: ********/
			/****** Gain = [(Seven_Byte << 8) + Six_Byte + 1000]mv ********/
			/****** offset = [Five_Byte - 20] mv. ********/
			gpio_calib_vref_gain = (adc_vref_calib_value[6] << 8) + adc_vref_calib_value[5] + 1000;
			gpio_calib_vref_offset = adc_vref_calib_value[4] - 20;
			adc_set_gpio_calib_vref(gpio_calib_vref_gain);
			adc_set_gpio_two_point_calib_offset(gpio_calib_vref_offset);
			return 1;
		}
		else{
			/****** If flash do not exist the two-point gpio calibration value,use the one-point gpio calibration value ********/
			/****** Method of calculating one-point gpio calibration Flash_gpio_Vref value: ********/
			/****** Gain = [1175 +First_Byte-255+Second_Byte] mV = [920 + First_Byte + Second_Byte] mV ********/
			gpio_calib_vref_gain = 920 + adc_vref_calib_value[0] + adc_vref_calib_value[1];
			/****** Check the one-point calibration value whether is correct ********/
			if ((gpio_calib_vref_gain >= 1047) && (gpio_calib_vref_gain <= 1302))
			{
				adc_set_gpio_calib_vref(gpio_calib_vref_gain);
				return 1;
			}
			else
			{
				/****** If flash do not exist the calibration value or the value is incorrect,use the Efuse gpio calibration value ********/
				gpio_calib_vref_gain = adc_vref_cfg.adc_vref;
				if((gpio_calib_vref_gain>=1100) && (gpio_calib_vref_gain<=1300))
				{
					adc_set_gpio_calib_vref(gpio_calib_vref_gain);
					return 1;
				}
			}
		}
	}
	return 0;
}
/*********************************************************** ADC calibration end********************************************/
