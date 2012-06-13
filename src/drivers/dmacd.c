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

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include "board.h"

#include <assert.h>

//------------------------------------------------------------------------------
//         Local types
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// DMA transfer descriptor. Tracks the status and parameters of a transfer
/// on the DMA bus.
//------------------------------------------------------------------------------
typedef struct _DmaTransfer
{
    /// Buffer transfer status.
    volatile uint8_t status;
    /// Transfer buffer size in byte.
    uint32_t bufSize;
    /// Total transfer size to byte.
    volatile uint32_t transferSize;
    /// Optional callback function.
    DmaCallback callback;
} DmaTransfer ;

//------------------------------------------------------------------------------
/// DMAD driver structure. Monitors the status of transfers on all
/// DMA channels.
//------------------------------------------------------------------------------
typedef struct _Dmad
{
    /// List of transfers occuring on each dwChannel.
    DmaTransfer transfers[DMA_CHANNEL_NUM] ;
} Dmad ;

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

/// Global DMA transfer instance.
static Dmad dmad ;

//------------------------------------------------------------------------------
//         Local functions
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// This handler function must be called by the DMAC interrupt service routine.
/// Identifies which event was activated and calls the associated function.
//------------------------------------------------------------------------------
extern void DMAC_IrqHandler( void )
{
    uint32_t dwStatus ;
    uint32_t dwChannel ;
    DmaTransfer *pTransfer ;

    dwStatus = DMA_GetStatus( DMAC ) ;

    // Check if the buffer transfer completed is set.
    if ( dwStatus & (DMAC_EBCISR_BTC0|DMAC_EBCISR_BTC1|DMAC_EBCISR_BTC2|DMAC_EBCISR_BTC3|DMAC_EBCISR_BTC4|DMAC_EBCISR_BTC5) )
    {
        // Scan each dwChannel status.
        for ( dwChannel = 0 ; dwChannel < DMA_CHANNEL_NUM ; dwChannel++ )
        {
            if ( !(dwStatus & (DMAC_EBCISR_BTC0 << dwChannel)) )
            {
                continue ;
            }

            dmad.transfers[dwChannel].transferSize -= dmad.transfers[dwChannel].bufSize ;

            // if next buffer is to be the last buffer in the transfer, then clear the automatic mode bit.
            if ( dmad.transfers[dwChannel].transferSize <= dmad.transfers[dwChannel].bufSize )
            {
                DMA_ClearAutoMode( DMAC, dwChannel ) ;
            }

            // Transfer finished
            if ( dmad.transfers[dwChannel].transferSize == 0 )
            {
                pTransfer = &(dmad.transfers[dwChannel]) ;
                pTransfer->callback() ;
                DMA_DisableIt( DMAC, DMAC_EBCIDR_BTC0 << dwChannel ) ;
                DMA_DisableChannel( DMAC, dwChannel ) ;
            }
            else
            {
                // Write the KEEPON field to clear the STALL states.
                DMA_KeeponChannel( DMAC, dwChannel ) ;
            }
        }
    }
}

//------------------------------------------------------------------------------
/// Initializes the DMA controller.
/// \param dwChannel Particular dwChannel number
/// \param defaultHandler Using the default dmad interrupt handler.
//------------------------------------------------------------------------------
extern void DMAD_Initialize( uint32_t dwChannel, uint32_t defaultHandler )
{
    uint32_t dwStatus ;
    uint32_t dwFlag ;

    /* Enable peripheral clock */
    PMC_EnablePeripheral( ID_DMAC ) ;

    /* Read the dwChannel handler status to ensure the channel is a free channel */
    dwStatus = DMA_GetChannelStatus( DMAC ) ;
    TRACE_INFO( "DMAD_Initialize dwChannel %x  \n\r", dwChannel ) ;
    assert( (dwStatus & (1 << dwChannel)) == 0 ) ;

    /* Clear any pending interrupts on the channel */
    DMA_GetStatus( DMAC ) ;

    /* Disable the channel */
    DMA_DisableChannel( DMAC, dwChannel ) ;

    /* Disable the interrupt */
    dwFlag = 0x3FFFFF ;
    DMA_DisableIt( DMAC, dwFlag ) ;
    /* Enable DMA */
    DMA_Enable( DMAC ) ;

    if ( defaultHandler )
    {
        NVIC_EnableIRQ( DMAC_IRQn ) ;
    }

    // Initialize transfer instance.
    dmad.transfers[dwChannel].transferSize = 0;
}

//------------------------------------------------------------------------------
/// Configure the DMA transfer buffer by giving transfer mode, it could be single
/// buffer or multi-buffer(LLI/auto-reload/contiguous buffers) with or without
/// Picture-In-Picture mode.
/// \param dwChannel Particular dwChannel number.
/// \param sourceTransferMode Source buffer transfer mode.
/// \param destTransferMode Destination buffer transfer mode.
/// \param lli Pointer to a DmaLinkList structure instance.
/// \param pip Pointer to a PictureInPicture structure.
//------------------------------------------------------------------------------
extern uint32_t DMAD_Configure_Buffer( uint32_t dwChannel, uint32_t sourceTransferMode,
                                       uint32_t destTransferMode, DmaLinkList *lli )
{
    DmaTransfer *pTransfer = &(dmad.transfers[dwChannel]) ;

    // Check that no transfer is pending on the dwChannel
    if ( pTransfer-> transferSize > 0 )
    {
        TRACE_ERROR("DAM transfer is already pending\n\r");
        return DMAD_ERROR_BUSY ;
    }

    // Configure source transfer mode.
    DMA_SetSourceBufferMode( DMAC, dwChannel, sourceTransferMode, 0 ) ;

    // Configure destination transfer mode.
    DMA_SetDestBufferMode( DMAC, dwChannel, destTransferMode, 0 ) ;

    if ( lli )
    {
        DMA_SetDescriptorAddr( DMAC, dwChannel, (uint32_t)(&lli[0]) ) ;
    }
    else
    {
        DMA_SetDescriptorAddr( DMAC, dwChannel, 0 ) ;
    }

    return 0 ;
}

//------------------------------------------------------------------------------
/// Configure the DMA transfer control infomation.
/// \param dwChannel Particular dwChannel number.
/// \param bufSize Buffer transfer size in byte.
/// \param sourceWidth Source transfer width.
/// \param destWidth Destination transfer width.
/// \param sourceAddress Destination transfer width.
/// \param destAddress Destination transfer width.
//------------------------------------------------------------------------------
extern uint32_t DMAD_Configure_TransferController( uint32_t dwChannel,
                                      uint32_t bufSize,
                                      uint32_t sourceWidth,
                                      uint32_t destWidth,
                                      uint32_t sourceAddress,
                                      uint32_t destAddress)
{
    DmaTransfer *pTransfer = &(dmad.transfers[dwChannel]);
    // Check that no transfer is pending on the dwChannel
    if (pTransfer-> transferSize > 0 )
    {
        TRACE_ERROR("DAM transfer is already pending\n\r");
        return DMAD_ERROR_BUSY ;
    }
    pTransfer->bufSize = bufSize ;

    // Set up the transfer width and transfer size.
    DMA_SetSourceBufferSize( DMAC, dwChannel, bufSize, sourceWidth, destWidth, 0 ) ;

    if ( sourceAddress )
    {
        // Write the starting source address.
        DMA_SetSourceAddr( DMAC, dwChannel, sourceAddress ) ;
    }

    if ( destAddress )
    {
        // Write the starting destination address.
        DMA_SetDestinationAddr( DMAC, dwChannel, destAddress ) ;
    }
    return 0;
}

//------------------------------------------------------------------------------
/// Starts buffer transfer on the given dwChannel
/// \param dwChannel Particular dwChannel number.
/// \param size Total transfer size in byte.
/// \param callback Optional callback function.
/// \param polling Polling dwChannel status enable.
//------------------------------------------------------------------------------
extern uint32_t DMAD_BufferTransfer( uint32_t dwChannel, uint32_t dwSize,
                                     DmaCallback callback, uint32_t dwPolling )
{
    DmaTransfer *pTransfer = &(dmad.transfers[dwChannel]) ;

    // Check that no transfer is pending on the dwChannel
    if ( pTransfer-> transferSize > 0 )
    {
        TRACE_ERROR( "DAM transfer is already pending\n\r" ) ;

        return DMAD_ERROR_BUSY ;
    }

    pTransfer->status = DMAD_ERROR_BUSY ;
    pTransfer->transferSize = dwSize ;
    pTransfer->callback = callback ;

    if ( dwPolling == 0 )
    {
        DMA_EnableIt( DMAC, DMA_BTC << dwChannel ) ;
    }

    // Enable the dwChannel.
    DMA_EnableChannel( DMAC, dwChannel ) ;

    if ( dwPolling != 0 )
    {
        while ( (DMA_GetChannelStatus( DMAC ) & (DMAC_CHSR_ENA0 << dwChannel)) == (DMAC_CHSR_ENA0 << dwChannel)) ;

        if ( pTransfer->callback )
        {
            pTransfer->callback() ;
        }
        pTransfer->transferSize = 0 ;
        DMA_DisableChannel( DMAC, dwChannel ) ;
    }

    return 0 ;
}

//------------------------------------------------------------------------------
/// Returns 1 if no transfer is currently pending on the given dwChannel;
/// otherwise, returns 0.
/// \param dwChannel  Channel number.
//------------------------------------------------------------------------------
extern uint32_t DMAD_IsFinished( uint32_t dwChannel )
{
    assert( dwChannel <= DMA_CHANNEL_NUM ) ;

    if ( dmad.transfers[dwChannel].transferSize > 0 )
    {
        return 0 ;
    }
    else
    {
        DMA_DisableChannel( DMAC, dwChannel ) ;

        return 1 ;
    }
}
