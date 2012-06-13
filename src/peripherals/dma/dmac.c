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

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include "chip.h"
#include "dmac.h"

#include <assert.h>

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Configure a DMAC peripheral
//------------------------------------------------------------------------------
extern void DMA_Config( Dmac* pDmac, uint32_t dwFlag )
{
    pDmac->DMAC_GCFG = dwFlag ;
}

//------------------------------------------------------------------------------
/// Enables a DMAC peripheral
//------------------------------------------------------------------------------
extern void DMA_Enable( Dmac* pDmac )
{
    pDmac->DMAC_EN = DMAC_EN_ENABLE ;
}

//------------------------------------------------------------------------------
/// Disables DMAC peripheral
//------------------------------------------------------------------------------
extern void DMA_Disable( Dmac* pDmac )
{
    pDmac->DMAC_EN = ~(uint32_t)DMAC_EN_ENABLE ;
}

//-----------------------------------------------------------------------------
/// Enable DMA interrupt
/// \param flag IT to be enabled
//-----------------------------------------------------------------------------
extern void DMA_EnableIt( Dmac* pDmac, uint32_t dwFlag )
{
    pDmac->DMAC_EBCIER = dwFlag ;
}

//-----------------------------------------------------------------------------
/// Disable DMA interrupt
/// \param flag IT to be enabled
//-----------------------------------------------------------------------------
extern void DMA_DisableIt( Dmac* pDmac, uint32_t dwFlag )
{
    pDmac->DMAC_EBCIDR = dwFlag ;
}

//-----------------------------------------------------------------------------
/// Return DMA Interrupt Status
//-----------------------------------------------------------------------------
extern uint32_t DMA_GetStatus( Dmac* pDmac )
{
    return (pDmac->DMAC_EBCISR) ;
}

//-----------------------------------------------------------------------------
/// Return DMA Interrupt Mask Status
//-----------------------------------------------------------------------------
extern uint32_t DMA_GetInterruptMask( Dmac* pDmac )
{
    return (pDmac->DMAC_EBCIMR) ;
}

//-----------------------------------------------------------------------------
/// Returns the current status register of the given DMA peripheral, but
/// masking interrupt sources which are not currently enabled.
//-----------------------------------------------------------------------------
extern uint32_t DMA_GetMaskedStatus( Dmac* pDmac )
{
    uint32_t dwStatus ;

    dwStatus = pDmac->DMAC_EBCISR ;
    dwStatus &= pDmac->DMAC_EBCIMR ;

    return dwStatus ;
}

//------------------------------------------------------------------------------
/// Enables DMAC dwChannel
/// \param dwChannel Particular dwChannel number.
//------------------------------------------------------------------------------
extern void DMA_EnableChannel( Dmac* pDmac, uint32_t dwChannel )
{
    assert( dwChannel < DMA_CHANNEL_NUM ) ; //, "this dwChannel does not exist" ) ;

    pDmac->DMAC_CHER |= DMAC_CHER_ENA0 << dwChannel ;
}

extern void DMA_EnableChannels( Dmac* pDmac, uint32_t dwChannels )
{
    pDmac->DMAC_CHER = dwChannels ;
}

//------------------------------------------------------------------------------
/// Disables a DMAC dwChannel
/// \param dwChannel Particular dwChannel number.
//------------------------------------------------------------------------------
extern void DMA_DisableChannel( Dmac* pDmac, uint32_t dwChannel )
{
    assert( dwChannel < DMA_CHANNEL_NUM ) ; //, "this dwChannel does not exist");

    pDmac->DMAC_CHDR |= DMAC_CHER_ENA0 << dwChannel ;
}

extern void DMA_DisableChannels( Dmac* pDmac, uint32_t dwChannels )
{
    pDmac->DMAC_CHDR = dwChannels ;
}

//------------------------------------------------------------------------------
/// Resume DMAC dwChannel from an stall state.
/// \param dwChannel Particular dwChannel number.
//------------------------------------------------------------------------------
extern void DMA_KeeponChannel( Dmac* pDmac, uint32_t dwChannel )
{
    assert( dwChannel < DMA_CHANNEL_NUM ) ; //, "this dwChannel does not exist");

    pDmac->DMAC_CHER |= DMAC_CHER_KEEP0 << dwChannel ;
}

//------------------------------------------------------------------------------
/// Clear automatic mode for multi-buffer transfer.
/// \param dwChannel Particular dwChannel number.
//------------------------------------------------------------------------------
extern void DMA_ClearAutoMode( Dmac* pDmac, uint32_t dwChannel )
{
    assert( dwChannel < DMA_CHANNEL_NUM ) ; //, "this dwChannel does not exist");

    pDmac->DMAC_CH_NUM[dwChannel].DMAC_CTRLB &= 0x7FFFFFFF ;
}

//------------------------------------------------------------------------------
/// Return DMAC dwChannel status
//------------------------------------------------------------------------------
extern uint32_t DMA_GetChannelStatus( Dmac* pDmac )
{
   return( pDmac->DMAC_CHSR) ;
}

//-----------------------------------------------------------------------------
/// Set DMA source dwAddress used by a HDMA dwChannel.
/// \param dwChannel Particular dwChannel number.
/// \param sources sources dwAddress.
//-----------------------------------------------------------------------------
extern void DMA_SetSourceAddr( Dmac* pDmac, uint32_t dwChannel, uint32_t dwAddress )
{
    assert( dwChannel < DMA_CHANNEL_NUM ) ; //, "this dwChannel does not exist");

    pDmac->DMAC_CH_NUM[dwChannel].DMAC_SADDR = dwAddress;
}

//-----------------------------------------------------------------------------
/// Set DMA destination dwAddress used by a HDMA dwChannel.
/// \param dwChannel Particular dwChannel number.
/// \param sources destination dwAddress.
//-----------------------------------------------------------------------------
extern void DMA_SetDestinationAddr( Dmac* pDmac, uint32_t dwChannel, uint32_t dwAddress)
{
    assert( dwChannel < DMA_CHANNEL_NUM ) ; //, "this dwChannel does not exist");

    pDmac->DMAC_CH_NUM[dwChannel].DMAC_DADDR = dwAddress;
}

//-----------------------------------------------------------------------------
/// Set DMA descriptor dwAddress used by a HDMA dwChannel.
/// \param dwChannel Particular dwChannel number.
/// \param sources destination dwAddress.
//-----------------------------------------------------------------------------
extern void DMA_SetDescriptorAddr( Dmac* pDmac, uint32_t dwChannel, uint32_t dwAddress )
{
    assert( dwChannel < DMA_CHANNEL_NUM ) ; //, "this dwChannel does not exist");

    pDmac->DMAC_CH_NUM[dwChannel].DMAC_DSCR = dwAddress ;
}


//-----------------------------------------------------------------------------
/// Set DMA control A register used by a HDMA dwChannel.
/// \param dwChannel Particular dwChannel number.
/// \param size Dma transfer size in byte.
/// \param sourceWidth Single transfer width for source.
/// \param destWidth Single transfer width for destination.
/// \param done Transfer done field.
//-----------------------------------------------------------------------------
extern void DMA_SetSourceBufferSize( Dmac* pDmac, uint32_t dwChannel, uint32_t dwSize, uint32_t dwSourceWidth, uint32_t dwDestWidth, uint32_t dwDone )
{
    assert( dwChannel < DMA_CHANNEL_NUM ) ; //, "this dwChannel does not exist");
    assert( dwSourceWidth < 4 ) ; //, "width does not support");
    assert( dwDestWidth < 4 ) ; //, "width does not support");

    pDmac->DMAC_CH_NUM[dwChannel].DMAC_CTRLA = (dwSize | dwSourceWidth << 24 | dwDestWidth << 28 | dwDone << 31 ) ;
}

//-----------------------------------------------------------------------------
/// Set DMA transfer mode for source used by a HDMA dwChannel.
/// \param dwChannel Particular dwChannel number.
/// \param transferMode Transfer buffer mode (single, LLI, reload or contiguous)
/// \param addressingType Type of addrassing mode
///                       0 : incrementing, 1: decrementing, 2: fixed.
//-----------------------------------------------------------------------------
extern void DMA_SetSourceBufferMode( Dmac* pDmac, uint32_t dwChannel, uint32_t dwTransferMode, uint32_t dwAddressingType )
{
    uint32_t dwValue ;

    assert( dwChannel < DMA_CHANNEL_NUM ) ; //, "dwChannel %d does not exist", dwChannel);

    dwValue = pDmac->DMAC_CH_NUM[dwChannel].DMAC_CTRLB ;
    dwValue &= ~ (DMAC_CTRLB_SRC_DSCR | DMAC_CTRLB_SRC_INCR_Msk | DMAC_CTRLB_AUTO ) ;

    switch ( dwTransferMode )
    {
        case DMA_TRANSFER_SINGLE:
             dwValue |= DMAC_CTRLB_SRC_DSCR_FETCH_DISABLE | dwAddressingType << 24 ;
        break ;

        case DMA_TRANSFER_LLI:
             dwValue |= dwAddressingType << 24 ;
        break ;

        case DMA_TRANSFER_RELOAD:
        case DMA_TRANSFER_CONTIGUOUS:
             dwValue |= DMAC_CTRLB_SRC_DSCR_FETCH_DISABLE | dwAddressingType << 24 | DMAC_CTRLB_AUTO_ENABLE ;
        break;
    }
    pDmac->DMAC_CH_NUM[dwChannel].DMAC_CTRLB = dwValue ;

    if ( dwTransferMode == DMA_TRANSFER_RELOAD || dwTransferMode == DMA_TRANSFER_CONTIGUOUS )
    {
        dwValue = pDmac->DMAC_CH_NUM[dwChannel].DMAC_CFG ;
        dwValue &= ~(uint32_t)DMAC_CFG_SRC_REP ;

        // When automatic mode is activated, the source dwAddress and the control register are reloaded from previous transfer.
        if ( dwTransferMode == DMA_TRANSFER_RELOAD )
        {
            dwValue |= DMAC_CFG_SRC_REP_RELOAD_ADDR ;
        }
        pDmac->DMAC_CH_NUM[dwChannel].DMAC_CFG = dwValue ;
    }
    else
    {
        pDmac->DMAC_CH_NUM[dwChannel].DMAC_CFG = 0 ;
    }
}

//-----------------------------------------------------------------------------
/// Set DMA transfer mode for destination used by a HDMA dwChannel.
/// \param dwChannel Particular dwChannel number.
/// \param transferMode Transfer buffer mode (single, LLI, reload or contiguous)
/// \param addressingType Type of addrassing mode
///                       0 : incrementing, 1: decrementing, 2: fixed.
//-----------------------------------------------------------------------------
extern void DMA_SetDestBufferMode( Dmac* pDmac, uint32_t dwChannel, uint32_t dwTransferMode, uint32_t dwAddressingType )
{
    uint32_t dwValue ;

    assert( dwChannel < DMA_CHANNEL_NUM ) ; //, "this dwChannel does not exist");

    dwValue = pDmac->DMAC_CH_NUM[dwChannel].DMAC_CTRLB ;
    dwValue &= ~(uint32_t)(DMAC_CTRLB_DST_DSCR | DMAC_CTRLB_DST_INCR_Msk) ;

    switch ( dwTransferMode )
    {
        case DMA_TRANSFER_SINGLE:
        case DMA_TRANSFER_RELOAD:
        case DMA_TRANSFER_CONTIGUOUS:
             dwValue |= DMAC_CTRLB_DST_DSCR_FETCH_DISABLE | dwAddressingType << 28 ;
        break ;

        case DMA_TRANSFER_LLI:
             dwValue |= dwAddressingType << 28 ;
        break ;
    }
    pDmac->DMAC_CH_NUM[dwChannel].DMAC_CTRLB = dwValue ;

    if ( dwTransferMode == DMA_TRANSFER_RELOAD || dwTransferMode == DMA_TRANSFER_CONTIGUOUS )
    {
        dwValue = pDmac->DMAC_CH_NUM[dwChannel].DMAC_CFG ;
        dwValue &= ~(uint32_t)DMAC_CFG_DST_REP ;

        // When automatic mode is activated, the source dwAddress and the control register are reloaded from previous transfer.
        if ( dwTransferMode == DMA_TRANSFER_RELOAD )
        {
            dwValue |= DMAC_CFG_DST_REP_RELOAD_ADDR ;
        }

        pDmac->DMAC_CH_NUM[dwChannel].DMAC_CFG = dwValue ;
    }
    else
    {
        pDmac->DMAC_CH_NUM[dwChannel].DMAC_CFG = 0 ;
    }
}

//------------------------------------------------------------------------------
/// Set DMA configuration registe used by a HDMA dwChannel.
/// \param dwChannel Particular dwChannel number.
/// \param value configuration value.
//------------------------------------------------------------------------------
extern void DMA_SetConfiguration( Dmac* pDmac, uint32_t dwChannel, uint32_t dwValue )
{
    assert( dwChannel < DMA_CHANNEL_NUM ) ; //, "this dwChannel does not exist");

    pDmac->DMAC_CH_NUM[dwChannel].DMAC_CFG = dwValue ;
}

//-----------------------------------------------------------------------------
/// Set DMA control B register Flow control bit field.
/// \param dwChannel Particular dwChannel number.
/// \param size Dma transfer size in byte.
/// \param sourceWidth Single transfer width for source.
/// \param destWidth Single transfer width for destination.
/// \param done Transfer done field.
//-----------------------------------------------------------------------------
extern void DMA_SetFlowControl( Dmac* pDmac, uint32_t dwChannel, uint32_t dwFlow )
{
    uint32_t dwValue ;

    assert( dwChannel < DMA_CHANNEL_NUM ) ; //, "this dwChannel does not exist");
    assert( dwFlow < 4 ) ; //, "flow control does not support");

    dwValue = pDmac->DMAC_CH_NUM[dwChannel].DMAC_CTRLB ;
    dwValue &= ~(uint32_t)DMAC_CTRLB_FC_Msk ;
    dwValue |= dwFlow << 21 ;
    pDmac->DMAC_CH_NUM[dwChannel].DMAC_CTRLB = dwValue ;
}

