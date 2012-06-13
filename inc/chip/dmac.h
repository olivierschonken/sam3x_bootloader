/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support  -  ROUSSET  -
 * ----------------------------------------------------------------------------
 * Copyright (c) 2006, Atmel Corporation

 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaiimer below.
 *
 * - Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the disclaimer below in the documentation and/or
 * other materials provided with the distribution.
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

//------------------------------------------------------------------------------
/// \dir
/// !Purpose
///
/// Interface for configuration the %DMA controller(DMAC).
///
/// !Usage
///
/// -# Enable or disable the a DMAC controller with
///    DMA_Enable() and or DMA_Disable().
/// -# Enable or disable %Dma interrupt using DMA_EnableIt()
///    or DMA_DisableIt().
/// -# Get %Dma interrupt status by DMA_GetStatus().
/// -# Enable or disable specified %Dma channel with
///    DMA_EnableChannel() or DMA_DisableChannel().
/// -# Get %Dma channel status by DMA_GetChannelStatus().
/// -# Configure source and/or destination start address with
///    DMA_SetSourceAddr() and/or DMA_SetDestAddr().
/// -# Set %Dma descriptor address using DMA_SetDescriptorAddr().
/// -# Set source transfer buffer size with DMA_SetSourceBufferSize().
/// -# Configure source and/or destination transfer mode with
///    DMA_SetSourceBufferMode() and/or DMA_SetDestBufferMode().
/// -# Configure source and/or destination Picture-In-Picutre
///    mode with DMA_SPIPconfiguration() and/or DMA_DPIPconfiguration().
//------------------------------------------------------------------------------

#ifndef DMA_H
#define DMA_H

//------------------------------------------------------------------------------
//         Definitions
//------------------------------------------------------------------------------
#if defined sam3x2
#elif defined sam3x2c | defined sam3a2
#elif defined sam3x4
#elif defined sam3x4c | defined sam3a4
#elif defined sam3x8
#elif defined sam3x8c | defined sam3a8
#else
  #error Library does not support the specified device.
#endif

typedef enum _DMA_CHANNELS
{
    DMA_CHANNEL_0,
    DMA_CHANNEL_1,
    DMA_CHANNEL_2,
    DMA_CHANNEL_3,
    DMA_CHANNEL_4,
    DMA_CHANNEL_5,
    DMA_CHANNEL_NUM
} DMA_CHANNELS ;

typedef enum _DMA_TRANSFER_MODES
{
    DMA_TRANSFER_SINGLE,
    DMA_TRANSFER_LLI,
    DMA_TRANSFER_RELOAD,
    DMA_TRANSFER_CONTIGUOUS
} DMA_TRANSFER_MODES ;

#define DMA_BTC                  (1 << 0)
#define DMA_CBTC                 (1 << 8)
#define DMA_ERR                  (1 << 16)

#ifdef AT91C_HDMA_SRC_DSCR
    #define AT91C_SRC_DSCR AT91C_HDMA_SRC_DSCR
    #define AT91C_DST_DSCR AT91C_HDMA_DST_DSCR
    #define AT91C_SRC_INCR AT91C_HDMA_SRC_ADDRESS_MODE
    #define AT91C_DST_INCR AT91C_HDMA_DST_ADDRESS_MODE
    #define AT91C_SRC_PER  AT91C_HDMA_SRC_PER
    #define AT91C_DST_PER  AT91C_HDMA_DST_PER
#if defined(AT91C_HDMA_SRC_REP)
    #define AT91C_SRC_REP  AT91C_HDMA_SRC_REP
#endif
#if defined(AT91C_HDMA_DST_REP)
    #define AT91C_DST_REP  AT91C_HDMA_DST_REP
#endif
    #define AT91C_FC       AT91C_HDMA_FC

    #define AT91C_BTC             (0xFF <<  0)
    #define AT91C_CBTC            (0xFF <<  8)
    #define AT91C_ERR             (0xFF << 16)
#endif

//------------------------------------------------------------------------------
//         Exported functions
//------------------------------------------------------------------------------

extern void DMA_Config( Dmac* pDmac, uint32_t dwFlag ) ;

extern void DMA_Enable( Dmac* pDmac ) ;
extern void DMA_Disable( Dmac* pDmac ) ;

extern void DMA_EnableIt( Dmac* pDmac, uint32_t dwFlag ) ;
extern void DMA_DisableIt( Dmac* pDmac, uint32_t dwFlag ) ;

extern uint32_t DMA_GetStatus( Dmac* pDmac ) ;
extern uint32_t DMA_GetInterruptMask( Dmac* pDmac ) ;
extern uint32_t DMA_GetMaskedStatus( Dmac* pDmac ) ;

extern void DMA_EnableChannel( Dmac* pDmac, uint32_t dwChannel ) ;
extern void DMA_EnableChannels( Dmac* pDmac, uint32_t dwChannels ) ;

extern void DMA_DisableChannel( Dmac* pDmac, uint32_t dwChannel ) ;
extern void DMA_DisableChannels( Dmac* pDmac, uint32_t dwChannels ) ;

extern void DMA_KeeponChannel( Dmac* pDmac, uint32_t dwChannel ) ;
extern void DMA_ClearAutoMode( Dmac* pDmac, uint32_t dwChannel ) ;

extern uint32_t DMA_GetChannelStatus( Dmac* pDmac ) ;

extern void DMA_SetSourceAddr( Dmac* pDmac, uint32_t dwChannel, uint32_t dwAddress ) ;
extern void DMA_SetDestinationAddr( Dmac* pDmac, uint32_t dwChannel, uint32_t dwAddress);
extern void DMA_SetDescriptorAddr( Dmac* pDmac, uint32_t dwChannel, uint32_t dwAddress ) ;

extern void DMA_SetSourceBufferSize( Dmac* pDmac, uint32_t dwChannel, uint32_t dwSize, uint32_t dwSourceWidth, uint32_t dwDestWidth, uint32_t dwDone ) ;
extern void DMA_SetSourceBufferMode( Dmac* pDmac, uint32_t dwChannel, uint32_t dwTransferMode, uint32_t dwAddressingType ) ;
extern void DMA_SetDestBufferMode( Dmac* pDmac, uint32_t dwChannel, uint32_t dwTransferMode, uint32_t dwAddressingType ) ;

extern void DMA_SetConfiguration( Dmac* pDmac, uint32_t dwChannel, uint32_t dwValue ) ;

extern void DMA_SetFlowControl( Dmac* pDmac, uint32_t dwChannel, uint32_t dwFlow ) ;


#endif //#ifndef DMA_H
