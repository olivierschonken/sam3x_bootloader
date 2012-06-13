/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation
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

#ifndef _CAN_H
#define _CAN_H

//------------------------------------------------------------------------------
//      Definitions
//------------------------------------------------------------------------------
#define CAN_TIMEOUT         100000

#define CAN_STATUS_SUCCESS             0
#define CAN_STATUS_LOCKED              1
#define CAN_STATUS_ABORTED             2
#define CAN_STATUS_RESET               3

#define NUM_MAILBOX_MAX 16

//------------------------------------------------------------------------------
//         Types
//------------------------------------------------------------------------------
typedef struct
{
    volatile uint8_t   state ;
    volatile uint8_t   can_number ;
    volatile uint8_t   mailbox_number ;
    volatile uint8_t   test_can ;
    volatile uint32_t  mode_reg ;
    volatile uint32_t  acceptance_mask_reg ;
    volatile uint32_t  identifier ;
    volatile uint32_t  data_low_reg ;
    volatile uint32_t  data_high_reg ;
    volatile uint32_t  control_reg ;
    volatile uint32_t  mailbox_in_use ;
    volatile int       size ;
} CanTransfer ;

//------------------------------------------------------------------------------
//         Exported functions
//------------------------------------------------------------------------------
extern uint32_t CAN_Init( Can* pCan, uint32_t dwBaudRate, CanTransfer* pTransferRead, CanTransfer* pTransferWrite ) ;

extern void CAN_BasicTestSuite( Can* pCan ) ;
extern void CAN_disable( Can* pCan ) ;


extern void CAN_ResetAllMailbox( CanTransfer* pTransfer ) ;
extern void CAN_TransferReset( CanTransfer* pTransfer ) ;
extern void CAN_InitMailboxRegisters( CanTransfer* pTransfer ) ;

extern uint32_t CAN_Write( Can* pCan, CanTransfer* pTransfer ) ;
extern uint32_t CAN_Read( Can* pCan, CanTransfer* pTransfer ) ;

extern void CAN_BasicTestSuiteWithoutInterrupt( Can* pCan ) ;
extern uint32_t CAN_IsInIdle( Can* pCan, CanTransfer* pTransfer ) ;

#endif // _CAN_H
