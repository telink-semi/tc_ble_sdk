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
#include "drivers/8278/driver_8278.h"
#include "ext_misc.h"
/*********************************************************** STIMER START********************************************/
_attribute_ram_code_ void systimer_set_irq_capture(unsigned int sys_timer_tick)//8278 for save time,need to place in ram.
{
	if( (unsigned int)( sys_timer_tick  - (reg_system_tick + 7*CLOCK_16M_SYS_TIMER_CLK_1US )) > BIT(30) )//63us - 75us in flash / 3.5us in ram
	{
		reg_system_tick_irq = reg_system_tick + 12*CLOCK_16M_SYS_TIMER_CLK_1US; ///320 = 0x140, so not &0xfffffff8
	}
	else{
		reg_system_tick_irq = sys_timer_tick & 0xfffffff8;
	}
}
/*********************************************************** STIMER END********************************************/


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

/*********************************************************** RANDOM START********************************************/
/**
 * @brief      This function performs to get a serial of random number.
 * @param[in]  len- the length of random number
 * @param[in]  data - the first address of buffer store random number in
 * @return     the result of a serial of random number..
 */
void generateRandomNum(int len, unsigned char *data)
{
	int i;
	unsigned int randNums = 0;
    /* if len is odd */
	for (i=0; i<len; i++ ) {
		if( (i & 3) == 0 ){
			randNums = rand();
		}

		data[i] = randNums & 0xff;
		randNums >>=8;
	}
}
/*********************************************************** RANDOM END********************************************/


/************************************************** ADC calibration START********************************************/
/**
 * @brief      This function is used to calib ADC 1.2V vref.
 * @param[in]  velfrom - the calibration value from flash or otp.
 * @param[in]  addr - the calibration value address of flash or otp.
 * @return 	   1 - the calibration value update, 0 - the calibration value is not update.
 */
unsigned char user_calib_adc_vref(user_calib_from_e velfrom, unsigned int addr)
{
/********************************************************************************************
	There have three kind of calibration value of ADC 1.2V vref in flash,and one calibration value in Efuse.
	Three kind of ADC calibration value in flash are adc_vbat_calib_vref(used for internal voltage sample)
	and two-point calibration adc_gpio_calib_vref_gain(used for gpio voltage sample)
	and adc_gpio_calib_vref_gain(used for gpio voltage sample).The ADC calibration value in Efuse is
	adc_vbat_calib_vref(used for internal voltage sample).
	The calibration value of efuse has a total of 8 bits, all of which are calibration values..
	The priority of adc_gpio_calib_vref_gain is: two-point calib from Flash > one-point calib from Flash > Default(1175mV).
	The priority of adc_vbat_calib_vref is: Flash > Efuse >Default(1175mV).
********************************************************************************************/
	unsigned char adc_vref_calib_value[7] = {0};
	unsigned short gpio_calib_vref_gain = 0;
	signed char gpio_calib_vref_offset = 0;
	unsigned short vbat_calib_vref_gain = 0;
	if(addr == 0)
	{
		/****** If flash check mid fail,use the Efuse vbat calibration value ********/
		vbat_calib_vref_gain = adc_vref_cfg.adc_vref;
		if((vbat_calib_vref_gain >= 1100) && (vbat_calib_vref_gain <= 1300))
		{
			adc_set_vbat_calib_vref(vbat_calib_vref_gain);
			return 1;
		}
	}
	else{
		if(velfrom == USER_CALIB_FROM_FLASH)
		{
			flash_read_page(addr, 7, adc_vref_calib_value);
		}
		/****** Check the two-point gpio calibration value whether is exist ********/
		if((adc_vref_calib_value[4] >= 0) && (adc_vref_calib_value[4] <= 127) && (((adc_vref_calib_value[6] << 8) + adc_vref_calib_value[5]) >= 47) && (((adc_vref_calib_value[6] << 8) + adc_vref_calib_value[5]) <= 300)){
			/****** Method of calculating two-point gpio calibration Flash_gain and Flash_offset value: ********/
			/****** Gain = [(Seven_Byte << 8) + Six_Byte + 1000]mv ********/
			/****** offset = [Five_Byte - 20] mv.********/
			gpio_calib_vref_gain = (adc_vref_calib_value[6] << 8) + adc_vref_calib_value[5] + 1000;
			gpio_calib_vref_offset = adc_vref_calib_value[4] - 20;
			adc_set_gpio_calib_vref(gpio_calib_vref_gain);
			adc_set_gpio_two_point_calib_offset(gpio_calib_vref_offset);
		}
		else{
			/****** If flash do not exist the two-point gpio calibration value,use the one-point gpio calibration value ********/
			/****** Method of calculating one-point gpio calibration Flash_gpio_Vref value: ********/
			/****** Gain = [1175 +First_Byte-255+Second_Byte] mV = [920 + First_Byte + Second_Byte] mV ********/
			gpio_calib_vref_gain = 920 + adc_vref_calib_value[0] + adc_vref_calib_value[1];
			/****** Check the one-point calibration value whether is correct ********/
			if ((gpio_calib_vref_gain >= 1047) && (gpio_calib_vref_gain <= 1300))
			{
				adc_set_gpio_calib_vref(gpio_calib_vref_gain);
			}
		}
		/****** Method of calculating calibration Flash_vbat_Vref value: ********/
		/****** Gain = [1175 +First_Byte-255+Second_Byte] mV = [920 + First_Byte + Second_Byte] mV ********/
		vbat_calib_vref_gain = 920 + adc_vref_calib_value[2] + adc_vref_calib_value[3];
		/****** If flash do not exist the vbat one-point calibration value or the value is incorrect,use the Efuse vbat calibration value ********/
		if ((vbat_calib_vref_gain < 1047) || (vbat_calib_vref_gain > 1300))
		{
			vbat_calib_vref_gain = adc_vref_cfg.adc_vref;
			if((vbat_calib_vref_gain >= 1100) && (vbat_calib_vref_gain <= 1300))
			{
				adc_set_vbat_calib_vref(vbat_calib_vref_gain);
				return 1;
			}
		}
		else
		{
			adc_set_vbat_calib_vref(vbat_calib_vref_gain);
			return 1;
		}
	}
	return 0;
}
/************************************************** ADC calibration END********************************************/
