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
/// \unit
///
/// !!!Purpose
///
/// The %Dma driver is a high level dma driver which performs DMA device Initializes,
/// transfer mode configuration and dma transfer.
///
/// !!!Usage
///
/// -# Initializes a %Dma controller and dma transfer instance.
///    Initializes dma for specified channel using DMAD_Initialize().
/// -# Configures the %Dma transfer buffer by giving transfer mode, transfer mode
///    for source peripheral and destination peripheral could be single buffer or
///    multi-buffer(LLI/auto-reload/contiguous buffers) with or without
///    Picture-In-Picture mode.
///    DMAD_Configure_Buffer()
/// \code
///   // Configure multi-buffer transfer with source address auto-reloaded and
///      contiguous destination address.
///    DMAD_Configure_Buffer(DMA_CHANNEL_1,
///                          DMA_TRANSFER_RELOAD,
///                          DMA_TRANSFER_CONTIGUOUS,
///                          0,
///                          0);
/// \endcode
/// -# Configures the %Dma characteristics (such as source/destination
///    single transfer width, buffer transfer size and source/destionation
///    start addreass for the device corresponding to the specified channle
///    using DMAD_Configure_TransferController().
/// -# Starts a %Dma transfer using DMAD_BufferTransfer().
///    The transfer is performed using the %Dma channels.
///    -# Initialize the total size to be transfered.
///    -# Initialize the callback function if specified.
///    -# Enable the interrupt for specified %dma channel.
///    -# Enable the specified %dma channel.
///    -# Example for transfering buffer through the %Dma.
/// \code
///     // Start channel 1 transfer. Source image auto-reload 4 times.
///     and transfer to destination continguous.
///     DMAD_BufferTransfer(DMA_CHANNEL_1, bufferSize * 4, TestCallback, 0);
///     while (!DMAD_IsFinished(DMA_CHANNEL_1));
/// \endcode
/// -# The DMAD_Handler() must be called by the DMA Interrupt Service Routine
///    with the corresponding %Dma instance. It is invokes to check for pending
///    interrupts.
///    - Example for initializing %Dma interrupt handler in upper application.
/// \code
///       AIC_ConfigureIT(AT91C_ID_HDMA, 0, DMAD_Handler);
/// \endcode
//------------------------------------------------------------------------------

#ifndef _DMAD_
#define _DMAD_

//------------------------------------------------------------------------------
//         Global definitions
//------------------------------------------------------------------------------
/// DMA driver is currently busy.
#define DMAD_ERROR_BUSY              1
/// Using the default interrupt handler of the DMAD
#define DMAD_USE_DEFAULT_IT          1
#define DMAD_NO_DEFAULT_IT           0

//------------------------------------------------------------------------------
//         Types
//------------------------------------------------------------------------------

/// DMA driver callback function.
typedef void (*DmaCallback)( void ) ;

//------------------------------------------------------------------------------
/// DMA multi buffer transfer Linker List Item structure.
//------------------------------------------------------------------------------
typedef struct _DmaLinkList
{
    /// Source address.
    uint32_t sourceAddress;
    /// Destination address.
    uint32_t destAddress;
    /// Control A value.
    uint32_t controlA;
    /// Control B value.
    uint32_t controlB;
    /// Descriptor Address.
    uint32_t descriptor;
} DmaLinkList ;

//------------------------------------------------------------------------------
//         Global functions
//------------------------------------------------------------------------------
extern void DMAD_Initialize( uint32_t dwChannel, uint32_t defaultHandler ) ;

extern uint32_t DMAD_Configure_Buffer( uint32_t dwChannel, uint32_t sourceTransferMode,
                                       uint32_t destTransferMode, DmaLinkList *lli ) ;

extern uint32_t DMAD_Configure_TransferController( uint32_t dwChannel, uint32_t bufSize,
                                                   uint32_t sourceWidth, uint32_t destWidth,
                                                   uint32_t sourceAddress, uint32_t destAddress ) ;

extern uint32_t DMAD_BufferTransfer( uint32_t dwChannel, uint32_t size, DmaCallback callback, uint32_t polling ) ;

extern uint32_t DMAD_IsFinished( uint32_t dwChannel ) ;


#endif // #ifndef _DMAD_
