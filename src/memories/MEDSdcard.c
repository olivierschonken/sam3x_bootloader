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

#include "memories.h"

#include <assert.h>
#include <string.h>

//------------------------------------------------------------------------------
//         Constants
//------------------------------------------------------------------------------

/// Number of SD Slots
#define NUM_SD_SLOTS            1

//------------------------------------------------------------------------------
//         Local variables
//------------------------------------------------------------------------------

/// MCI driver instance.
static Mcid mciDrv[NUM_SD_SLOTS];

/// SDCard driver instance.
static SdCard sdDrv[NUM_SD_SLOTS];

#if MCI_BUSY_CHECK_FIX && defined(PIN_MCI_DAT0)
/// SD DAT0 pin
static const Pin pinSdDAT0 = PIN_MCI_DAT0;
#endif

//------------------------------------------------------------------------------
//      Internal Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// MCI0 interrupt handler. Forwards the event to the MCI driver handler.
//------------------------------------------------------------------------------
void HSMCI_IrqHandler( void )
{
    Sdmmc_Handler( mciDrv ) ;
}

//------------------------------------------------------------------------------
//         Optional: SD card detection
//------------------------------------------------------------------------------

/// SD card detection pin instance.
#if defined(BOARD_SD_PIN_CD)
static const Pin pinCardDetect = BOARD_SD_PIN_CD;
#endif

#if defined(BOARD_SD_MCI1_PIN_CD)
static const Pin pinCardDetect1 = BOARD_SD_MCI1_PIN_CD;
#endif

//------------------------------------------------------------------------------
/// Detect if SD card is connected
//------------------------------------------------------------------------------
static uint8_t CardIsConnected( uint8_t slot )
{
    if (slot == 0)
    {
      #if defined(BOARD_SD_PIN_CD)
        PIO_Configure(&pinCardDetect, 1);
        return PIO_Get(&pinCardDetect) ? 0 : 1;
      #else
        return 1;
      #endif
    }

    if (slot == 1)
    {
      #if defined(BOARD_SD_MCI1_PIN_CD)
        PIO_Configure(&pinCardDetect1, 1);
        return PIO_Get(&pinCardDetect1) ? 0 : 1;
      #else
        return 1;
      #endif
    }

    return 0;
}

//------------------------------------------------------------------------------
//         Optional: Write protection status
//------------------------------------------------------------------------------

/// Write protection status pin instance.
#ifdef BOARD_SD_PIN_WP
static const Pin pinMciWriteProtect = BOARD_SD_PIN_WP;
#endif
#ifdef BOARD_SD_MCI1_PIN_WP
static const Pin pinMciWriteProtect1 = BOARD_SD_MCI1_PIN_WP;
#endif

//------------------------------------------------------------------------------
/// Checks if the device is write protected.
/// \return 1 if protected.
//------------------------------------------------------------------------------
static uint8_t CardIsProtected(uint8_t slot)
{
    if (slot == 0) {
      #ifdef BOARD_SD_PIN_WP
        PIO_Configure(&pinMciWriteProtect, 1);
        return (PIO_Get(&pinMciWriteProtect) != 0);
      #else
        return 0;
      #endif
    }

    if (slot == 1) {
      #ifdef BOARD_SD_MCI1_PIN_WP
        PIO_Configure(&pinMciWriteProtect1, 1);
        return (PIO_Get(&pinMciWriteProtect1) != 0);
      #else
        return 0;
      #endif
    }

    return 0;
}


//------------------------------------------------------------------------------
/// Configure the PIO for SD
//------------------------------------------------------------------------------
static void ConfigurePIO(uint8_t mciID)
{
    #ifdef BOARD_SD_PINS
    const Pin pinSd0[] = {BOARD_SD_PINS};
    #endif

    #ifdef BOARD_SD_MCI1_PINS
    const Pin pinSd1[] = {BOARD_SD_MCI1_PINS};
    #endif

    if(mciID == 0) {
        #ifdef BOARD_SD_PINS
        PIO_Configure(pinSd0, PIO_LISTSIZE(pinSd0));
        #endif
    } else {
        #ifdef BOARD_SD_MCI1_PINS
        PIO_Configure(pinSd1, PIO_LISTSIZE(pinSd1));
        #endif
    }
}



//------------------------------------------------------------------------------
//! \brief  Reads a specified amount of data from a SDCARD memory
//! \param  media    Pointer to a Media instance
//! \param  address  Address of the data to read
//! \param  data     Pointer to the buffer in which to store the retrieved
//!                   data
//! \param  length   Length of the buffer
//! \param  callback Optional pointer to a callback function to invoke when
//!                   the operation is finished
//! \param  argument Optional pointer to an argument for the callback
//! \return Operation result code
//------------------------------------------------------------------------------
static uint8_t MEDSdcard_Read(Media         *media,
                                    uint32_t  address,
                                    void          *data,
                                    uint32_t  length,
                                    MediaCallback callback,
                                    void          *argument)
{
    uint8_t error;

    // Check that the media is ready
    if (media->state != MED_STATE_READY) {

        TRACE_INFO("Media busy\n\r");
        return MED_STATUS_BUSY;
    }

    // Check that the data to read is not too big
    if ((length + address) > media->size) {

        TRACE_WARNING("MEDSdcard_Read: Data too big: %d, %d\n\r",
                      (int)length, (int)address);
        return MED_STATUS_ERROR;
    }

    // Enter Busy state
    media->state = MED_STATE_BUSY;

    error = SD_Read((SdCard*)media->interface, address, data, length, callback, argument );//length,

    // Leave the Busy state
    media->state = MED_STATE_READY;

    // Invoke callback
    if (callback != 0) {

        callback(argument, MED_STATUS_SUCCESS, 0, 0);
    }

    return MED_STATUS_SUCCESS;
}

//------------------------------------------------------------------------------
//! \brief  Writes data on a SDRAM media
//! \param  media    Pointer to a Media instance
//! \param  address  Address at which to write
//! \param  data     Pointer to the data to write
//! \param  length   Size of the data buffer
//! \param  callback Optional pointer to a callback function to invoke when
//!                   the write operation terminates
//! \param  argument Optional argument for the callback function
//! \return Operation result code
//! \see    Media
//! \see    MediaCallback
//------------------------------------------------------------------------------
static uint8_t MEDSdcard_Write(Media         *media,
                                    uint32_t  address,
                                    void          *data,
                                    uint32_t  length,
                                    MediaCallback callback,
                                    void          *argument)
{
    uint8_t error;

    // Check that the media if ready
    if (media->state != MED_STATE_READY) {

        TRACE_WARNING("MEDSdcard_Write: Media is busy\n\r");
        return MED_STATUS_BUSY;
    }

    // Check that the data to write is not too big
    if ((length + address) > media->size) {

        TRACE_WARNING("MEDSdcard_Write: Data too big\n\r");
        return MED_STATUS_ERROR;
    }

    // Put the media in Busy state
    media->state = MED_STATE_BUSY;

    error = SD_Write((SdCard*)media->interface, address, data, length, callback, argument ); // length,

    // Leave the Busy state
    media->state = MED_STATE_READY;

    // Invoke the callback if it exists
    if (callback != 0) {

        callback(argument, MED_STATUS_SUCCESS, 0, 0);
    }

    return MED_STATUS_SUCCESS;
}

//------------------------------------------------------------------------------
//! \brief Callback invoked when SD/MMC transfer done
//------------------------------------------------------------------------------
static void SdMmcCallback( uint8_t status, void *pCommand )
{
	MciCmd       * pCmd = (MciCmd*)pCommand;
    Media       * pMed = pCmd->pArg;
    MEDTransfer * pXfr = &pMed->transfer;

    TRACE_INFO_WP("SDCb ");

    // Error
    if (status == SDMMC_ERROR_BUSY) {
        status = MED_STATUS_BUSY;
    }
    else if (status) {
        status = MED_STATUS_ERROR;
    }

    pMed->state = MED_STATE_READY;
    if (pXfr->callback) {
        pXfr->callback(pXfr->argument,
                       status,
                       pXfr->length * pMed->blockSize,
                       0);
    }
}

//------------------------------------------------------------------------------
//! \brief  Reads a specified amount of data from a SDCARD memory
//! \param  media    Pointer to a Media instance
//! \param  address  Address of the data to read
//! \param  data     Pointer to the buffer in which to store the retrieved
//!                   data
//! \param  length   Length of the buffer
//! \param  callback Optional pointer to a callback function to invoke when
//!                   the operation is finished
//! \param  argument Optional pointer to an argument for the callback
//! \return Operation result code
//------------------------------------------------------------------------------
static uint8_t MEDSdusb_Read(Media         *media,
                                   uint32_t  address,
                                   void          *data,
                                   uint32_t  length,
                                   MediaCallback callback,
                                   void          *argument)
{
    MEDTransfer * pXfr;
    uint8_t error;

    TRACE_INFO_WP("SDuRd(%d,%d) ", (int)address, (int)length);

    // Check that the media is ready
    if (media->state != MED_STATE_READY) {
        TRACE_INFO("MEDSdusb_Read: Busy\n\r");
        return MED_STATUS_BUSY;
    }
    // Check that the data to read is not too big
    if ((length + address) > media->size) {
        TRACE_WARNING("MEDSdusb_Read: Data too big: %d, %d\n\r",
                      (int)length, (int)address);
        return MED_STATUS_ERROR;
    }
    // Enter Busy state
    media->state = MED_STATE_BUSY;

    // Start media transfer
    pXfr = &media->transfer;
    pXfr->data     = data;
    pXfr->address  = address;
    pXfr->length   = length;
    pXfr->callback = callback;
    pXfr->argument = argument;

    error = SD_Read((SdCard*)media->interface,
                     address,
                     data,
                     length,
                     SdMmcCallback,
                     media);

    return (error ? MED_STATUS_ERROR : MED_STATUS_SUCCESS);
}

//------------------------------------------------------------------------------
//! \brief  Writes data on a SDRAM media
//! \param  media    Pointer to a Media instance
//! \param  address  Address at which to write
//! \param  data     Pointer to the data to write
//! \param  length   Size of the data buffer
//! \param  callback Optional pointer to a callback function to invoke when
//!                   the write operation terminates
//! \param  argument Optional argument for the callback function
//! \return Operation result code
//! \see    Media
//! \see    MediaCallback
//------------------------------------------------------------------------------
static uint8_t MEDSdusb_Write(Media         *media,
                                    uint32_t  address,
                                    void          *data,
                                    uint32_t  length,
                                    MediaCallback callback,
                                    void          *argument)
{
    MEDTransfer * pXfr;
    uint8_t error;
    TRACE_INFO_WP("SDuWr(%d,%d) ", (int)address, (int)length);

    // Check that the media if ready
    if (media->state != MED_STATE_READY)
    {
        TRACE_INFO("MEDSdusb_Write: Busy\n\r");
        return MED_STATUS_BUSY;
    }

    // Check that the data to write is not too big
    if ((length + address) > media->size)
    {
        TRACE_WARNING("MEDSdcard_Write: Data too big\n\r");
        return MED_STATUS_ERROR;
    }
    // Put the media in Busy state
    media->state = MED_STATE_BUSY;

    // Start media transfer
    pXfr = &media->transfer;
    pXfr->data = data;
    pXfr->address = address;
    pXfr->length = length;
    pXfr->callback = callback;
    pXfr->argument = argument;

    error = SD_Write((SdCard*)media->interface,
                      address,
                      data,
                      length,
                      SdMmcCallback,
                      media);

    return (error ? MED_STATUS_ERROR : MED_STATUS_SUCCESS);
}

//------------------------------------------------------------------------------
//      Exported Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Detect if there is SD card in socket
//------------------------------------------------------------------------------
uint8_t MEDSdcard_Detect( Media * media, uint8_t mciID )
{
    return CardIsConnected( mciID ) ;
}

//------------------------------------------------------------------------------
/// Initializes a Media instance and the associated physical interface
/// \param  media Pointer to the Media instance to initialize
/// \return 1 if success.
//------------------------------------------------------------------------------
uint8_t MEDSdcard_Initialize(Media *media, uint8_t mciID)
{
    TRACE_INFO("MEDSdcard init\n\r");

    // Initialize SDcard
    //--------------------------------------------------------------------------

    if ( !CardIsConnected( mciID ) )
    {
        return 0;
    }

    // Configure SDcard pins
    ConfigurePIO(mciID);

#if defined(MCI2_INTERFACE)
    DMAD_Initialize(BOARD_MCI_DMA_CHANNEL, DMAD_NO_DEFAULT_IT);
#endif

    // Initialize the MCI driver
    if ( mciID == 0 )
    {
        //IRQ_ConfigureIT(BOARD_SD_MCI_ID,  1, MCI0_IrqHandler);
        MCI_Init(mciDrv, BOARD_SD_MCI_BASE, BOARD_SD_MCI_ID,BOARD_MCK );
        NVIC_EnableIRQ( (IRQn_Type)BOARD_SD_MCI_ID );
        //IRQ_EnableIT(BOARD_SD_MCI_ID);
    }
    else
    {
        #ifdef BOARD_SD_MCI1_ID
        IRQ_ConfigureIT(BOARD_SD_MCI1_ID,  1, MCI0_IrqHandler);
        MCI_Init(mciDrv, BOARD_SD_MCI1_BASE, BOARD_SD_MCI1_ID, BOARD_SD_MCI1_SLOT, BOARD_MCK );
        IRQ_EnableIT(BOARD_SD_MCI1_ID);
        #else
        TRACE_ERROR("SD/MMC card initialization failed (MCI1 not supported)\n\r");
        #endif
    }
#if MCI_BUSY_CHECK_FIX && defined(BOARD_SD_DAT0)
    MCI_SetBusyFix(mciDrv, &pinSdDAT0);
#endif

    // Initialize the SD card driver
    if (SD_Init(sdDrv, (Mcid *)mciDrv))
    {
        TRACE_ERROR("SD/MMC card initialization failed\n\r");
        return 0;
    }
    else
    {
        //SD_DisplayRegisterCSD(&sdDrv);
        TRACE_WARNING("SD/MMC card initialization successful\n\r");
        TRACE_WARNING("Card size: %d MB\n\r", (int)(SD_GetTotalSizeKB(sdDrv)/1024));
    }
#if MCI_BUSY_CHECK_FIX && defined(PIN_MCI_DAT0)
    MCI_SetBusyFix(mciDrv, &pinSdDAT0);
#endif
    MCI_SetSpeed(mciDrv, sdDrv->transSpeed, BOARD_MCK);

    // Initialize media fields
    //--------------------------------------------------------------------------
    media->interface = sdDrv;
    #if !defined(OP_BOOTSTRAP_MCI_on)
    media->write = MEDSdcard_Write;
    #else
    media->write = 0;
    #endif
    media->read = MEDSdcard_Read;
    media->lock = 0;
    media->unlock = 0;
    media->handler = 0;
    media->flush = 0;

    media->blockSize = SD_BLOCK_SIZE;
    media->baseAddress = 0;
    media->size = SD_TOTAL_BLOCK(sdDrv);

    media->mappedRD  = 0;
    media->mappedWR  = 0;
    media->protected = CardIsProtected(mciID);
    media->removable = 1;

    media->state = MED_STATE_READY;

    media->transfer.data = 0;
    media->transfer.address = 0;
    media->transfer.length = 0;
    media->transfer.callback = 0;
    media->transfer.argument = 0;

    return 1;
}

//------------------------------------------------------------------------------
/// Initializes a Media instance and the associated physical interface
/// \param  media Pointer to the Media instance to initialize
/// \return 1 if success.
//------------------------------------------------------------------------------
uint8_t MEDSdusb_Initialize(Media *media, uint8_t mciID)
{
    TRACE_INFO("MEDSdusb init\n\r");

    // Initialize SDcard
    //--------------------------------------------------------------------------

    if (!CardIsConnected(mciID)) return 0;

    // Configure SDcard pins
    ConfigurePIO(mciID);

#if defined(MCI2_INTERFACE)
    DMAD_Initialize(BOARD_MCI_DMA_CHANNEL, DMAD_NO_DEFAULT_IT);
#endif
    // Initialize the MCI driver
    if ( mciID == 0 )
    {
        //IRQ_ConfigureIT(BOARD_SD_MCI_ID,  1, MCI0_IrqHandler);
        MCI_Init(mciDrv, BOARD_SD_MCI_BASE, BOARD_SD_MCI_ID, BOARD_MCK );
        //IRQ_EnableIT(BOARD_SD_MCI_ID);
    }
    else
    {
#ifdef BOARD_SD_MCI1_ID
        IRQ_ConfigureIT(BOARD_SD_MCI1_ID,  1, MCI0_IrqHandler);
        MCI_Init(mciDrv, BOARD_SD_MCI1_BASE, BOARD_SD_MCI1_ID, BOARD_SD_SLOT, BOARD_MCK );
        IRQ_EnableIT(BOARD_SD_MCI1_ID);
#else
        TRACE_ERROR("SD/MMC card initialization failed (MCI1 not supported)\n\r");
#endif
    }
#if MCI_BUSY_CHECK_FIX && defined(BOARD_SD_DAT0)
    MCI_SetBusyFix(mciDrv, &pinSdDAT0);
#endif

    // Initialize the SD card driver
    if (SD_Init(sdDrv, (Mcid *)mciDrv))
    {
        TRACE_ERROR("SD/MMC card initialization failed\n\r");
        return 0;
    }
    else
    {
        TRACE_INFO("SD/MMC card initialization successful\n\r");
        TRACE_INFO("Card size: %d MB\n\r", (int)(SD_GetTotalSizeKB(sdDrv)/1024));
    }
#if MCI_BUSY_CHECK_FIX && defined(PIN_MCI_DAT0)
    MCI_SetBusyFix(mciDrv, &pinSdDAT0);
#endif
    MCI_SetSpeed(mciDrv, sdDrv->transSpeed, BOARD_MCK);

    // Initialize media fields
    //--------------------------------------------------------------------------
    media->interface = sdDrv;
    media->write = MEDSdusb_Write;
    media->read = MEDSdusb_Read;
    media->lock = 0;
    media->unlock = 0;
    media->handler = 0;
    media->flush = 0;

    media->blockSize = SD_BLOCK_SIZE;
    media->baseAddress = 0;
    media->size = SD_TOTAL_BLOCK(sdDrv);

    media->mappedRD  = 0;
    media->mappedWR  = 0;
    media->protected = CardIsProtected(mciID);
    media->removable = 1;

    media->state = MED_STATE_READY;

    media->transfer.data = 0;
    media->transfer.address = 0;
    media->transfer.length = 0;
    media->transfer.callback = 0;
    media->transfer.argument = 0;

    return 1;
}

//------------------------------------------------------------------------------
/// erase all the Sdcard
/// \param  media Pointer to the Media instance to initialize
//------------------------------------------------------------------------------
void MEDSdcard_EraseAll(Media *media)
{
    uint8_t buffer[SD_BLOCK_SIZE];
    uint32_t block;
    uint32_t multiBlock = 1; // change buffer size for multiblocks
    uint8_t error;

    TRACE_INFO("MEDSdcard Erase All ...\n\r");

    // Clear the block buffer
    memset(buffer, 0, media->blockSize * multiBlock);

    for (block=0;
         block < (SD_TOTAL_BLOCK((SdCard*)media->interface)-multiBlock);
         block += multiBlock)
    {
        error = SD_WriteBlocks((SdCard*)media->interface, block, multiBlock, buffer);
        assert( !error ); /* "\n\r-F- Failed to write block (%d) #%u\n\r", error, block */
    }
}

//------------------------------------------------------------------------------
/// erase block
/// \param  media Pointer to the Media instance to initialize
/// \param  block to erase
//------------------------------------------------------------------------------
void MEDSdcard_EraseBlock(Media *media, uint32_t block)
{
    uint8_t buffer[SD_BLOCK_SIZE];
    uint8_t error;

    // Clear the block buffer
    memset(buffer, 0, media->blockSize);

    error = SD_WriteBlock((SdCard*)media->interface, block, 1, buffer);
    assert( !error ) ; /* "\n\r-F- Failed to write block (%d) #%u\n\r", error, block */
}

//------------------------------------------------------------------------------
/// Get driver pointer
//------------------------------------------------------------------------------
SdCard* MEDSdcard_GetDriver( uint32_t slot )
{
    if (slot >= NUM_SD_SLOTS)
    {
        return 0;
    }

    return &sdDrv[slot];
}
