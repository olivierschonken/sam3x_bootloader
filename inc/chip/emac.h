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

#ifndef _EMAC_
#define _EMAC_

//-----------------------------------------------------------------------------
/// \dir
/// !Purpose
///
///     Definition of methods and structures for using EMAC
///
/// !Usage
///
/// -# Initialize EMAC with EMAC_Init with MAC address.
/// -# Then the caller application need to initialize the PHY driver before further calling EMAC
///      driver.
/// -# Get a packet from network
///      -# Interrupt mode: EMAC_Set_RxCb to register a function to process the frame packet
///      -# Polling mode: EMAC_Poll for a data packet from network
/// -# Send a packet to network with EMAC_Send.
///
/// Please refer to the list of functions in the #Overview# tab of this unit
/// for more detailed information.
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//         Headers
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//         Definitions
//-----------------------------------------------------------------------------

/// Number of buffer for RX, be carreful: MUST be 2^n
#define RX_BUFFERS  16
/// Number of buffer for TX, be carreful: MUST be 2^n
#define TX_BUFFERS   8

/// Buffer Size
#define EMAC_RX_UNITSIZE            128     /// Fixed size for RX buffer
#define EMAC_TX_UNITSIZE            1518    /// Size for ETH frame length

// The MAC can support frame lengths up to 1536 bytes.
#define EMAC_FRAME_LENTGH_MAX       1536


//-----------------------------------------------------------------------------
//         Types
//-----------------------------------------------------------------------------
/// Callback used by send function
typedef void (*EMAC_TxCallback)( uint32_t dwStatus ) ;
typedef void (*EMAC_RxCallback)( uint32_t dwStatus ) ;
typedef void (*EMAC_WakeupCallback)( void ) ;

#ifdef __ICCARM__          // IAR
#pragma pack(4)            // IAR
#define __attribute__(...) // IAR
#endif                     // IAR
/** Describes the type and attribute of Receive Transfer descriptor. */
typedef struct _EmacRxTDescriptor
{
    uint32_t addr;
    uint32_t status;
} __attribute__((packed, aligned(8))) EmacRxTDescriptor, *PEmacRxTDescriptor;

/** Describes the type and attribute of Transmit Transfer descriptor. */
typedef struct _EmacTxTDescriptor
{
    uint32_t addr;
    uint32_t status;
} __attribute__((packed, aligned(8))) EmacTxTDescriptor, *PEmacTxTDescriptor;
#ifdef __ICCARM__          // IAR
#pragma pack()             // IAR
#endif                     // IAR

/**  Descriptors for RX (required aligned by 8) */
typedef struct _RxTd
{
   volatile EmacRxTDescriptor td[RX_BUFFERS];
   EMAC_RxCallback rxCb;  /** Callback function to be invoked once a frame has been received */
   uint16_t idx;
} RxTd ;

/** Descriptors for TX (required aligned by 8) */
typedef struct _TxTd
{
   volatile EmacTxTDescriptor td[TX_BUFFERS] ;
   EMAC_TxCallback txCb[TX_BUFFERS] ;   /** Callback function to be invoked once TD has been processed */
   EMAC_WakeupCallback wakeupCb ;       /**  Callback function to be invoked once several TD have been released */
   uint16_t wakeupThreshold ;           /** Number of free TD before wakeupCb is invoked */
   uint16_t head ;                      /** Circular buffer head pointer incremented by the upper layer (buffer to be sent) */
   uint16_t tail ;                      /** Circular buffer head pointer incremented by the IT handler (buffer sent) */
} TxTd ;


//-----------------------------------------------------------------------------
/// Describes the statistics of the EMAC.
//-----------------------------------------------------------------------------
typedef struct _EmacStats
{
    // TX errors
    uint32_t tx_packets;    /// Total Number of packets sent
    uint32_t tx_comp;       /// Packet complete
    uint32_t tx_errors;     /// TX errors ( Retry Limit Exceed )
    uint32_t collisions;    /// Collision
    uint32_t tx_exausts;    /// Buffer exhausted
    uint32_t tx_underruns;  /// Under Run, not able to read from memory
    // RX errors
    uint32_t rx_packets;    /// Total Number of packets RX
    uint32_t rx_eof;        /// No EOF error
    uint32_t rx_ovrs;       /// Over Run, not able to store to memory
    uint32_t rx_bnas;       /// Buffer is not available

} EmacStats, *PEmacStats ;

//-----------------------------------------------------------------------------
//         PHY Exported functions
//-----------------------------------------------------------------------------
extern uint32_t EMAC_SetMdcClock( Emac* pEmac, uint32_t dwMCk ) ;

extern void EMAC_EnableMdio( Emac* pEmac ) ;
extern void EMAC_DisableMdio( Emac* pEmac ) ;

extern void EMAC_EnableMII( Emac* pEmac ) ;
extern void EMAC_EnableRMII( Emac* pEmac ) ;

extern uint32_t EMAC_ReadPhy( Emac* pEmac, uint32_t dwPhyAddress, uint32_t dwAddress, uint32_t *pdwValue, uint32_t dwRetry ) ;
extern uint32_t EMAC_WritePhy( Emac* pEmac, uint32_t dwPhyAddress, uint32_t dwAddress, uint32_t dwValue, uint32_t dwRetry ) ;

extern void EMAC_SetLinkSpeed( Emac* pEmac, uint32_t dwSpeed, uint32_t dwFullduplex ) ;

//-----------------------------------------------------------------------------
//         EMAC Exported functions
//-----------------------------------------------------------------------------

extern void EMAC_Init( Emac* pEmac, uint32_t dwId, const uint8_t *pucMacAddress, uint32_t dwMode ) ;

#define EMAC_CAF_DISABLE  0
#define EMAC_CAF_ENABLE   1
#define EMAC_NBC_DISABLE  0
#define EMAC_NBC_ENABLE   2

extern void EMAC_Handler( Emac* pEmac ) ;

extern void EMAC_Reset( Emac* pEmac ) ;

extern uint32_t EMAC_Send( Emac* pEmac, void *pvBuffer, uint32_t dwSize, EMAC_TxCallback fEMAC_TxCallback ) ;

/// Return for EMAC_Send function
#define EMAC_TX_OK                     0
#define EMAC_TX_BUFFER_BUSY            1
#define EMAC_TX_INVALID_PACKET         2

extern uint32_t EMAC_Poll( Emac* pEmac, uint8_t *pFrame, uint32_t frameSize, uint32_t *pRcvSize ) ;

/// Return for EMAC_Poll function
#define EMAC_RX_OK                   0
#define EMAC_RX_NO_DATA              1
#define EMAC_RX_FRAME_SIZE_TOO_SMALL 2

extern void EMAC_GetStatistics( Emac* pEmac, EmacStats *pStats, uint32_t dwReset ) ;

extern uint32_t EMAC_TxLoad( Emac* pEmac ) ;

extern void EMAC_Set_RxCb( Emac* pEmac, EMAC_RxCallback pRxCb ) ;
extern void EMAC_Clear_RxCb( Emac* pEmac ) ;
extern char EMAC_Set_TxWakeUpCb( Emac* pEmac, EMAC_WakeupCallback pTxWakeUpCb, uint32_t dwThreshold ) ;
extern void EMAC_Clear_TxWakeUpCb( Emac* pEmac ) ;

#endif // #ifndef _EMAC_

