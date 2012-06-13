/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2010, Atmel Corporation
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
#ifndef _LIB_SAM3X_
#define _LIB_SAM3X_

/*
 * Peripherals registers definitions
 */
#if defined sam3x8
#elif defined sam3x4
#elif defined sam3x2
#else
    #warning Library does not support the specified chip, specifying sam3x8.
    #define sam3x8
#endif
#include "SAM3XA.h"


/* Define attribute */
#if defined   ( __CC_ARM   ) /* Keil µVision 4 */
    #define WEAK __attribute__ ((weak))
#elif defined ( __ICCARM__ ) /* IAR Ewarm 5.41+ */
    #define WEAK __weak
#elif defined (  __GNUC__  ) /* GCC CS3 2009q3-68 */
    #define WEAK __attribute__ ((weak))
#endif

/* Define NO_INIT attribute */
#if defined   ( __CC_ARM   )
    #define NO_INIT
#elif defined ( __ICCARM__ )
    #define NO_INIT __no_init
#elif defined (  __GNUC__  )
    #define NO_INIT
#endif

/*
 * SAM3X Embedded IP features.
 * Define the feature to use it. Comment it out to not use it.
 */
/* Indicate chip has a nandflash controller (NFC). */
#define CHIP_NAND_CTRL 			1

#define MCI_DMA_ENABLE			0

#define MCI2_INTERFACE			0

#define MCI_BUSY_CHECK_FIX		1
#if 0
#define HSMCI_WRITE_FIFO		1

#define HSMCI_READ_FIFO			1
#endif

/* Indicate chip has a hardware ECC. Note: NFC must be used if using hardware ECC. */
#if defined(CHIP_NAND_CTRL)
#define HARDWARE_ECC
#endif

/*
 * Core
 */
#include "exceptions.h"
//#include "cmsis/core_cm3.h"

/*
 * Peripherals
 */
#include "adc.h"
#include "async.h"
#include "can.h"
#include "chipid.h"
#include "dacc.h"
#include "dmac.h"
#include "eefc.h"
//#include "emac.h"
#include "flashd.h"
#include "hsmci.h"
#include "matrix.h"
#include "pio.h"
#include "pio_it.h"
#include "pmc.h"
#include "pwm.h"
#include "rstc.h"
#include "rtc.h"
#include "rtt.h"
#include "sdramc.h"
#include "smc.h"
#include "spi.h"
#include "ssc.h"
#include "supc.h"
#include "tc.h"
#include "trng.h"
#include "twi.h"
#include "twid.h"
#include "usart.h"
#include "wdt.h"

#include "trace.h"

#endif /* _LIB_SAM3X_ */
