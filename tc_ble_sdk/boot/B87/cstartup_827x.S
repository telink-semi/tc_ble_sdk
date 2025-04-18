/********************************************************************************************************
 * @file    cstartup_827x.S
 *
 * @brief   This is the boot file for 827x
 *
 * @author  BLE GROUP
 * @date    06,2020
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
@********************************************************************************************************
@                                   SRAM CONFIGURATION
@********************************************************************************************************
#define SRAM_BASE_ADDR		(0x840000)
#define SRAM_48K      		(0xc000)	//48KSRAM
#define SRAM_64K      		(0x10000)	//64KSRAM

#if (MCU_STARTUP_8278)
	#define	SRAM_SIZE		(SRAM_BASE_ADDR + SRAM_64K)
#elif (MCU_STARTUP_8238)
	#define	SRAM_SIZE		(SRAM_BASE_ADDR + SRAM_48K)
#endif

#ifndef SRAM_SIZE
#define	SRAM_SIZE			(SRAM_BASE_ADDR + SRAM_64K)
#endif

@********************************************************************************************************
@                                   BOOT FEATURE CONFIGURATION
@********************************************************************************************************

#define	FLL_STK_EN					0
#define	GPIO_DEBUG_EN				0
#define	STARTUP_WATCHDOG_EN			0

#ifndef __LOAD_RAM_SIZE__
#define	__LOAD_RAM_SIZE__		0xc
#endif


	.code	16

@********************************************************************************************************
@                                           MACROS AND DEFINIITIONS
@********************************************************************************************************

					@ Mode, correspords to bits 0-5 in CPSR
	.equ MODE_BITS,		0x1F	@ Bit mask for mode bits in CPSR
	.equ IRQ_MODE, 		0x12	@ Interrupt Request mode
	.equ SVC_MODE, 		0x13	@ Supervisor mode 

	.equ IRQ_STK_SIZE,	0x200
	.equ __LOAD_RAM, 	__LOAD_RAM_SIZE__
	.equ __SRAM_SIZE,   SRAM_SIZE
@********************************************************************************************************
@                                            TC32 EXCEPTION VECTORS
@********************************************************************************************************

	.section	.vectors,"ax"
	.global		__reset
	.global	 	__irq
	.global 	__start
	.global		__LOAD_RAM
	.global		__SRAM_SIZE
__start:					@ MUST,  referenced by boot.link

	.extern irq_handler

	.extern	 _retention_data_size_div_256_
	.extern  _retention_data_size_align_256_
	.extern  _ictag_start_
	.extern  _ictag_end_

	.org 0x0
	tj	__reset
@	.word	(BUILD_VERSION)
	.org 0x8
	.word	(0x544c4e4b)
	.word	(0x00880000 + _retention_use_size_div_16_)

	.org 0x10
	tj		__irq

	.org 0x18
	.word	(_bin_size_)
@********************************************************************************************************
@                                   LOW-LEVEL INITIALIZATION
@********************************************************************************************************
	.extern  main



	.org 0x20
	.align 4



__reset:

#if STARTUP_WATCHDOG_EN
	@ wd_set_interval_ms()
	tloadr		r1, WD_REG_BASE    @0x800620
	tloadr		r0, WD_REG_BASE+8  @  default 0x7A
	tshftl		r0, r0,#1          @  (WD_REG_BASE+8)<<1
	tstorerb	r0, [r1, #1]       @ *(unsigned char*)(0x800621)=(WD_REG_BASE+8)<<1
	tshftr		r0, r0,#8          @  (WD_REG_BASE+8)>>8
	tstorerb	r0, [r1, #2]       @ *(unsigned char*)(0x800622)=(WD_REG_BASE+8)>>8

	@ wd_start()
	tloadr		r1, WD_REG_BASE    @0x800620
	tmov		r0, #0x40          @ 01000000
	tstorerb	r0, [r1, #0]       @ *(unsigned char*)(0x800620)=0x40,reg_tmr_ctrl8[6]=1
	tloadr		r1, WD_REG_BASE+4  @0x800622
	tloadrb		r2, [r1]
	tmov		r0, #0x80          @ 10000000
	tor			r0, r2
	tstorerb	r0, [r1, #0]       @ reg_wd_ctrl1=reg_wd_ctrl1|0x80

	@ wd_clear() need to be executed periodically in main_loop()
#endif

#if GPIO_DEBUG_EN
	@ add debug, PB4 output 1
	tloadr     	r1, DEBUG_GPIO  @0x80058a  PB oen
	tmov		r0, #239      	@0b 11101111
	tstorerb	r0, [r1, #0]

	tmov		r0, #16			@0b 00010000
	tstorerb	r0, [r1, #1]	@0x80058b  PB output
#endif

SET_BOOT:
	tmov        r2, #20
	tloadrb     r1, [r2]		@read from core_840014

	tmov     	r0, #90    @5A
	tcmp        r0, r1
	tjne		SET_BOOT_END

	tmov        r2, #21
	tloadrb     r1, [r2]		@read from core_840015

	tmov     	r0, #165    @A5
	tcmp        r0, r1
	tjne		SET_BOOT_END

	tmov        r2, #22
	tloadrb     r1, [r2]		@read from core_840016
	tloadrb		r0,	BOOT_SEL_D
	tstorerb	r1, [r0, #0]

	tmov        r2, #23
	tloadrb     r1, [r2]		@read from core_840017
	tstorerb	r1, [r0, #1]

SET_BOOT_END:



@send flash cmd 0xab to wakeup flash;
FLASH_WAKEUP_BEGIN:
	tloadr      r0,FLASH_RECOVER + 0
	tmov		r1,#0
	tstorerb    r1,[r0,#1]
	tmov        r1,#171						@Flash deep cmd: 0xAB
	tstorerb    r1,[r0,#0]
	tmov		r2,#0
	tmov        r3,#6
TNOP:
	tadd        r2,#1
	tcmp        r2,r3
	tjle        TNOP
	tmov		r1,#1
	tstorerb    r1,[r0,#1]
FLASH_WAKEUP_END:

@ efuse load need delay about 18us
	tmov		r2,#0
	tmov        r3,#110
EFUSE_DELAY:
	tadd        r2,#1
	tcmp        r2,r3
	tjle        EFUSE_DELAY
EFUSE_DELAY_END:

@********************************************************************************************************
@                              		 FILL .DATA AND .BSS WITH 0xFF
@********************************************************************************************************
#if FLL_STK_EN

	tloadr	r0, FLL_D				@r0 = 0xffffffff
	tloadr	r1, FLL_D+4				@r1 = _start_data_
	tloadr	r2, FLL_D+8				@r2 = SRAM_SIZE

FLL_STK:
	tcmp	r1, r2
	tjge	FLL_STK_END   @r1>=r2 jump to FLL_STK_END
	tstorer r0, [r1, #0]  @*(unsigned int*)(r1)=0xffffffff
	tadd    r1, #4        @r1 = r1+4
	tj		FLL_STK       @jump to FLL_STK
FLL_STK_END:

#endif

	tloadr	r0, DAT0
	tmcsr	r0			
	tloadr	r0, DAT0 + 8
	tmov	r13, r0  

	tloadr	r0, DAT0 + 4
	tmcsr	r0	
	tloadr	r0, DAT0 + 12
	tmov	r13, r0  
@********************************************************************************************************
@                                    .BSS INITIALIZATION FOR 0
@********************************************************************************************************
	tmov	r0, #0				@r0 = 0
	tloadr	r1, DAT0 + 16		@r1 = _start_bss_
	tloadr	r2, DAT0 + 20		@r2 = _end_bss_

ZERO_BSS_BEGIN:
	tcmp	r1, r2
	tjge	ZERO_BSS_END		@r1>=r2 jump to ZERO_BSS_END
	tstorer	r0, [r1, #0]		@*(unsigned int*)(_start_bss_)=r0=0
	tadd    r1, #4				@r1 = r1 + 4
	tj		ZERO_BSS_BEGIN		@jump to ZERO_BSS_BEGIN
ZERO_BSS_END:

@********************************************************************************************************
@                                    IC TAG INITIALIZATION
@********************************************************************************************************
	tloadr	r1, DAT0 + 28		@r1 = _ictag_start_
	tloadr	r2, DAT0 + 32		@r2 = _ictag_end_

ZERO_TAG_BEGIN:
	tcmp	r1, r2
	tjge	ZERO_TAG_END		@r1>=r2 jump to ZERO_TAG_END
	tstorer	r0, [r1, #0]		@*(unsigned int*)(_ictag_start_)=r0=0
	tadd    r1, #4				@r1 = r1 + 4
	tj		ZERO_TAG_BEGIN		@jump to ZERO_TAG_BEGIN
ZERO_TAG_END:

@********************************************************************************************************
@                                    IC CACHE INITIALIZATION
@********************************************************************************************************
SETIC:
	tloadr     	r1, DAT0 + 24	@ r1 = 0x80060c
	tloadr      r0, DAT0 + 36	@ r0 = _retention_data_size_div_256_
	tstorerb	r0, [r1, #0]	@ *(unsigned int*)(0x80060c) = r0
	tadd    	r0, #1			@ r0 = r0 + 1
	tstorerb	r0, [r1, #1]	@ *(unsigned int*)(0x80060d) = r0

@********************************************************************************************************
@                                    DATA INITIALIZATION
@********************************************************************************************************
	tloadr		r1, DATA_I				@ r1 = _dstored_
	tloadr		r2, DATA_I+4			@ r2 = _start_data_
	tloadr		r3, DATA_I+8			@ r3 = _end_data_
COPY_DATA:
	tcmp		r2, r3
	tjge		COPY_DATA_END			@ r2>=r3 jump to COPY_DATA_END
	tloadr		r0, [r1, #0]			@ r0 = *(unsigned int*)(_dstored_)
	tstorer 	r0, [r2, #0]			@ *(unsigned int*)(_start_data_) = r0
	tadd    	r1, #4					@ r1 = r1 + 4
	tadd		r2, #4					@ r2 = r2 + 4
	tj			COPY_DATA				@ jump to COPY_DATA_BEGIN
COPY_DATA_END:

#if 0
SETSPISPEED:
	tloadr     	r1, DAT0 + 36
	tmov		r0, #0xbb		@0x0b for fast read; 0xbb for dual dat/adr
	tstorerb	r0, [r1, #0]
	tmov		r0, #3			@3 for dual dat/adr
	tstorerb	r0, [r1, #1]
#endif

	tjl	main
END:	tj	END

	.balign	4
DAT0:
	.word	0x12			    @IRQ    @0
	.word	0x13			    @SVC    @4
	.word	(irq_stk + IRQ_STK_SIZE)    @8
	.word	(SRAM_SIZE)		    @12  stack end
	.word	(_start_bss_)               @16
	.word	(_end_bss_)                 @20
	.word	(0x80060c)                  @24
	.word	_ictag_start_               @28		@ IC tag start
	.word	_ictag_end_	            	@32		@ IC tag end
	.word	_retention_data_size_div_256_ @36
	.word	_retention_data_size_align_256_ @40
	.word   (_retention_data_end_)      @44
	.word   (_rstored_)                 @48
	.word	(_retention_use_size_div_16_)     @52
	.word	(_retention_size_)     		@56

DATA_I:
	.word	_dstored_					@0
	.word	_start_data_				@4
	.word	_end_data_					@8

#if FLL_STK_EN
FLL_D:
	.word	0xffffffff					@0
	.word	(_start_data_)				@4
	.word	(SRAM_SIZE)					@8
#endif

#if GPIO_DEBUG_EN
DEBUG_GPIO:
	.word	(0x80058a)                  @  PBx oen
#endif

#if STARTUP_WATCHDOG_EN
WD_REG_BASE:
	.word	(0x800620)                  @0  reg_tmr_ctrl
	.word	(0x800622)                  @4  reg_wd_ctrl1
	.word	(0x7A)                      @8
		@sys clock 32MHz
			@capt tick = 1s, ((32000000 tick)>>18) = 0x7A;	capt tick = 500ms, ((16000000 tick)>>18)=0x3D,
			@capt tick = 2s, ((64000000 tick>>18) = 0xF4;	capt tick = 3s, ((96000000 tick>>18)=0x16E,
			@capt tick = 4s, ((128000000 tick)>>18) = 0x1E8;capt tick = 5s, ((160000000 tick>>18)=0x262,
		@sys clock 48MHz
			@capt tick = 1s, ((48000000 tick)>>18) = 0xB7,	capt tick = 500ms, ((24000000 tick)>>18)=0x5B,
			@capt tick = 2s, ((96000000 tick)>>18) = 0x16E,	capt tick = 3s, ((144000000 tick)>>18)=0x225,
			@capt tick = 4s, ((192000000 tick)>>18) = 0x2DC,capt tick = 5s, ((240000000 tick)>>18)=0x393,
#endif

BOOT_SEL_D:
	.word	(0x80063e)

FLASH_RECOVER:
	.word	(0x80000c)                  @0

	.align 4
__irq:
	tpush    	{r14}
	tpush    	{r0-r7}
	tmrss    	r0
	
	tmov		r1, r8
	tmov		r2, r9
	tmov		r3, r10
	tmov		r4, r11
	tmov		r5, r12
	tpush		{r0-r5}
	
	tjl      	irq_handler

	tpop		{r0-r5}
	tmov		r8, r1
	tmov		r9, r2
	tmov		r10,r3
	tmov		r11,r4
	tmov		r12,r5

	tmssr    	r0
	tpop		{r0-r7}
	treti    	{r15}

ASMEND:

	.section .bss
	.align 4
	.lcomm irq_stk, IRQ_STK_SIZE
	.end
