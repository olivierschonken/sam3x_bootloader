/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2009, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

/**
 * \page sam3x_ek_board/board_desc SAM3X-EK - Board Description
 *
 * \section Purpose
 *
 * This file is dedicated to description of the SAM3X-EK board.
 *
 * \section Contents
 *
 *  - For SAM3X-EK information, see \subpage sam3x_ek_board/board_info.
 *  - For operating frequency information, see \subpage sam3x_ek_opfreq.
 *  - For using portable PIO definitions, see \subpage sam3x_ek_piodef.
 *  - For on-board memories, see \subpage sam3x_ek_mem.
 *  - Several USB definitions are included here, see \subpage sam3x_ek_usb.
 *  - For External components, see \subpage sam3x_ek_extcomp.
 *  - For Individual chip definition, see \subpage sam3x_ek_chipdef.
 *
 * To get more software details and the full list of parameters related to the
 * SAM3X-EK board configuration, please have a look at the source file:
 * \ref board.h\n
 *
 * \section Usage
 *
 *  - The code for booting the board is provided by board/board_cstartup_xxx.c and
 *    board/board_lowlevel.c.
 *  - For using board PIOs, board characteristics (clock, etc.) and external
 *    components, see board.h.
 *  - For manipulating memories, see board/board_memories.h.
 *
 * This file can be used as a template and modified to fit a custom board, with
 * specific PIOs usage or memory connections.
 */

/**
 *  \file board.h
 *
 *  Definition of SAM3X-EK characteristics, sam3x-dependant PIOs and
 *  external components interfacing.
 */

#ifndef _BOARD_
#define _BOARD_

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "chip.h"

#include "board_lowlevel.h"
#include "board_memories.h"
#include "clock.h"
#include "dmacd.h"
#include "hamming.h"
#include "hx8347.h"
#include "frame_buffer.h"
#include "iso7816_4.h"
#include "lcdd.h"
#include "lcd_color.h"
#include "lcd_draw.h"
#include "lcd_font.h"
#include "lcd_font10x14.h"
#include "lcd_gimp_image.h"
#include "led.h"
#include "math.h"
#include "timetick.h"
#include "uart_console.h"

/**
 * Libc porting layers
 */
#if defined   ( __CC_ARM   ) /* Keil �Vision 4 */
#    include "rand.h"
#elif defined ( __ICCARM__ ) /* IAR Ewarm 5.41+ */
#    include "rand.h"
#elif defined (  __GNUC__  ) /* GCC CS3 2009q3-68/2010q1-188 */
#    include "syscalls.h" /** RedHat Newlib minimal stub */
#endif

/*----------------------------------------------------------------------------
 *        Definitions
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/**
 * \page sam3x_ek_board/board_info "SAM3X-EK - Board informations"
 * This page lists several definition related to the board description.
 *
 * \section Definitions
 * - \ref BOARD_NAME
 */

/** Name of the board */
#define BOARD_NAME "SAM3X-EK"
/** Board definition */
#define sam3xek
/** Family definition (already defined) */
#define sam3x
/** Core definition */
#define cortexm3

#define BOARD_REV_A
//#define BOARD_REV_B

/*----------------------------------------------------------------------------*/
/**
 *  \page sam3x_ek_opfreq "SAM3X-EK - Operating frequencies"
 *  This page lists several definition related to the board operating frequency
 *  (when using the initialization done by board/board_lowlevel.c).
 *
 *  \section Definitions
 *  - \ref BOARD_MAINOSC
 *  - \ref BOARD_MCK
 */

/** Frequency of the board main oscillator */
#define BOARD_MAINOSC           12000000

/** Master clock frequency (when using board/board_lowlevel.c) */
//#define BOARD_MCK               48000000  // for debug
#define BOARD_MCK               84000000

/* SDRAMC clock speed */
#define SDRAMC_CLK 				BOARD_MCK
/*----------------------------------------------------------------------------*/
/**
 * \page sam3x_ek_piodef "SAM3X-EK - PIO definitions"
 * This pages lists all the pio definitions contained in board.h. The constants
 * are named using the following convention: PIN_* for a constant which defines
 * a single Pin instance (but may include several PIOs sharing the same
 * controller), and PINS_* for a list of Pin instances.
 *
 */

#include "board/board_adc.h"

#include "board/board_ebi.h"

#include "board/board_leds.h"

#include "board/board_buttons.h"

#include "board/board_pwm.h"

#include "board/board_spi.h"

#include "board/board_ssc.h"

#include "board/board_pck.h"

#include "board/board_mci.h"

#include "board/board_uart.h"

#include "board/board_usart0.h"

#include "board/board_usart1.h"

#include "board/board_usb.h"

#include "board/board_can.h"

#include "board/board_qtouch.h"

/*----------------------------------------------------------------------------*/
/**
 * \page sam3x_ek_extcomp "SAM3X-EK - External components"
 * This page lists the definitions related to external on-board components
 * located in the board.h file for the SAM3X-EK.
 *
 */

#include "board/board_lcd.h"

#include "board/board_touchscreen.h"

#include "board/board_emac.h"

#include "board/board_sdcard.h"

#include "board/board_smartcard.h"


/*----------------------------------------------------------------------------*/
/**
 * \page sam3x_ek_mem "SAM3X-EK - Memories"
 * This page lists definitions related to internal & external on-board memories.
 *
 *
 */

#include "board/board_nandflash.h"

#include "board/board_sdram.h"

#include "board/board_norflash.h"


/*----------------------------------------------------------------------------*/
/**
 * \page sam3x_ek_chipdef "SAM3X-EK - Individual chip definition"
 * This page lists the definitions related to different chip's definition
 * located in the board.h file for the SAM3X-EK.
 *
 * \section USART
 * - \ref BOARD_PIN_USART_RXD
 * - \ref BOARD_PIN_USART_TXD
 * - \ref BOARD_PIN_USART_CTS
 * - \ref BOARD_PIN_USART_RTS
 * - \ref BOARD_PIN_USART_EN
 * - \ref BOARD_USART_BASE
 * - \ref BOARD_ID_USART
 */

/** MCI interface */
#define BOARD_SD_MCI_ID 			ID_HSMCI

#define BOARD_SD_MCI_BASE           HSMCI

/// Dma channel number
#define BOARD_MCI_DMA_CHANNEL                         0


/** Rtc */
#define BOARD_RTC_ID                ID_RTC

/** TWI ID for EEPROM application to use */
#define BOARD_ID_TWI_EEPROM         ID_TWI1
/** TWI ID for SLAVE application to use */
#define BOARD_ID_TWI_SLAVE          ID_TWI1
/** TWI Base for TWI EEPROM application to use */
#define BOARD_BASE_TWI_EEPROM       TWI1
/** TWI Base for TWI SLAVE application to use */
#define BOARD_BASE_TWI_SLAVE        TWI1
/** TWI pins for EEPROM application to use */
#define BOARD_PINS_TWI_EEPROM       PINS_TWI1
/** TWI pins for TWI SLAVE application to use */
#define BOARD_PINS_TWI_SLAVE        PINS_TWI1

/** USART RX pin for application */
#define BOARD_PIN_USART_RXD        PIN_USART1_RXD
/** USART TX pin for application */
#define BOARD_PIN_USART_TXD        PIN_USART1_TXD
/** USART CTS pin for application */
#define BOARD_PIN_USART_CTS        PIN_USART1_CTS
/** USART RTS pin for application */
#define BOARD_PIN_USART_RTS        PIN_USART1_RTS
/** USART ENABLE pin for application */
#define BOARD_PIN_USART_EN         PIN_USART1_EN
/** USART Base for application */
#define BOARD_USART_BASE           USART1
/** USART ID for application */
#define BOARD_ID_USART             ID_USART1

#endif /* #ifndef _BOARD_ */

