/********************************************************************************************************
 * @file    ext_calibration.c
 *
 * @brief   This is the source file for B85
 *
 * @author  BLE Group
 * @date    May 8,2018
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "ext_calibration.h"
#include "drivers.h"
#include "vendor/common/ble_flash.h"

/**
 * @brief      This function servers to get calibration value from flash.
 * @param[in]  none
 * @return     0 - the calibration value update, 1 - the calibration value is not update.
 */
unsigned char flash_calib_adc_vref(unsigned int addr)
{
    //the data order is gpio_gain, gpio_offset, rsvd, vbat_gain, vbat_offset
    unsigned char  adc_calib_value[5] = {0};
    flash_read_page(addr, 5, adc_calib_value);
    /**
     * The legal range of gain for both gpio and vbat in efuse is [0,250],
     * and the legal range of offset for both gpio and vbat is [-100,100].
     */
    if (adc_calib_value[0] <= 250 && (signed char)adc_calib_value[1] <= 100 && (signed char)adc_calib_value[1] >= -100 && adc_calib_value[3] <= 250 && (signed char)adc_calib_value[4] <= 100 && (signed char)adc_calib_value[4] >= -100) {
        adc_set_gpio_calib_vref(1100 + adc_calib_value[0], (signed char)adc_calib_value[1]);
        adc_set_vbat_calib_vref(1100 + adc_calib_value[3], (signed char)adc_calib_value[4]);
        return 0;
    }
    return 1;
}

/**
 * @brief      This function servers to get calibration value from flash or efuse.
 * @param[in]  addr - the addr of calib value in flash
 * @return     0 - the calibration value update, 1 - the calibration value is not update.
 */
void user_calib_adc_vref(unsigned int addr)
{
    unsigned char ret = 1; // 1 fail 0 success
    if (addr != 0) {
        ret = flash_calib_adc_vref(addr);
    }
    if (ret == 1) {
        efuse_calib_adc_vref();
    }
}

/**
 * @brief		This function serves to calibration the flash voltage(VDD_F),if the flash has the calib_value,we will use it,either will
 * 				trim vdd_f to 1.95V(2b'111 the max) if the flash is zb.
 */
int user_calib_vdd_f(unsigned char * calib_value) {
	//Todo
	return 0;
}









