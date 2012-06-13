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
 * \file
 *
 * Implementation of High Speed MultiMedia Card Interface (HSMCI) controller,
 * not using PDC nor DMA to transfer data.
 *
 */

/*---------------------------------------------------------------------------
 *         Headers
 *---------------------------------------------------------------------------*/
#include "chip.h"

#include <assert.h>


/*---------------------------------------------------------------------------
 *         Local macros
 *---------------------------------------------------------------------------*/
#define DTOR_1MEGA_CYCLES           (HSMCI_DTOR_DTOCYC_Msk | HSMCI_DTOR_DTOMUL_Msk)
/** Reset MCI */
#define MCI_RESET(pMciHw)      (pMciHw->HSMCI_CR = HSMCI_CR_SWRST)

/*---------------------------------------------------------------------------
 *         Exported functions
 *---------------------------------------------------------------------------*/

/**
 * Enable MCI
 * \param pMciHw  Pointer to a MCI peripheral.
 */
void  MCI_Enable(Hsmci *pMciHw)
{
    pMciHw->HSMCI_CR = HSMCI_CR_MCIEN;
}

/**
 * Disable MCI
 * \param pMciHw  Pointer to a MCI peripheral.
 */
void  MCI_Disable(Hsmci *pMciHw)
{
    pMciHw->HSMCI_CR = HSMCI_CR_MCIDIS;
}

/**
 * Initializes a MCI driver instance and the underlying peripheral.
 * \param pMci    Pointer to a MCI driver instance.
 * \param pMciHw  Pointer to a MCI peripheral.
 * \param mciId   MCI peripheral identifier.
 */
void MCI_Init( Mcid *pMci, Hsmci *pMciHw, uint8_t mciId, uint32_t dwMCk )
{
    unsigned short clkDiv;

#if MCI_BUSY_CHECK_FIX
    pMci->pPinDAT0 = 0;
#endif

    /* Initialize the MCI driver structure */
    pMci->pMciHw    = pMciHw;
    pMci->mciId     = mciId;
    pMci->semaphore = 1;
    pMci->pCommand  = NULL;

    /* Enable the MCI peripheral */
    PMC_EnablePeripheral( mciId ) ;

    /* Reset the MCI */
    pMciHw->HSMCI_CR = HSMCI_CR_SWRST;

    /* Disable the MCI */
    pMciHw->HSMCI_CR = HSMCI_CR_MCIDIS | HSMCI_CR_PWSDIS;

    /* Disable all the interrupts */
    pMciHw->HSMCI_IDR = 0xFFFFFFFF;

    /* Set the Data Timeout Register */
    pMciHw->HSMCI_DTOR = DTOR_1MEGA_CYCLES ;
    /* CSTOR ? */
    pMciHw->HSMCI_CSTOR = HSMCI_CSTOR_CSTOCYC_Msk | HSMCI_CSTOR_CSTOMUL_Msk ;

    /* Set the Mode Register: 400KHz for MCK = 48MHz (CLKDIV = 58) */
    clkDiv = (dwMCk / (MCI_INITIAL_SPEED * 2)) - 1;
    pMciHw->HSMCI_MR = (clkDiv | (HSMCI_MR_PWSDIV( 0x07 )) ) ;

    /* Set the SDCard Register 1-bit, slot A */
    pMciHw->HSMCI_SDCR = HSMCI_SDCR_SDCSEL_SLOTA | HSMCI_SDCR_SDCBUS_1 ;

    /* Enable the MCI and the Power Saving */
    pMciHw->HSMCI_CR = HSMCI_CR_MCIEN;

    pMciHw->HSMCI_DMA = (0x0u << 8);


    /* Configure MCI */
    pMciHw->HSMCI_CFG = HSMCI_CFG_FIFOMODE | HSMCI_CFG_FERRCTRL;

    /* Disable the MCI peripheral clock. */
    PMC_DisablePeripheral(mciId);

}

/**
 * Configure the  MCI CLKDIV in the MCI_MR register. The max. for MCI clock is
 * MCK/2 and corresponds to CLKDIV = 0
 * \param pMci  Pointer to the low level MCI driver.
 * \param mciSpeed  MCI clock speed in Hz, 0 will not change current speed.
 * \param mck       MCK to generate MCI Clock, in Hz
 * \return The actual speed used, 0 for fail.
 */
uint32_t MCI_SetSpeed( Mcid* pMci, uint32_t mciSpeed, uint32_t mck )
{
    Hsmci *pMciHw = pMci->pMciHw;
    uint32_t mciMr;
    uint32_t clkdiv;
    uint8_t  mciDis;

    assert(pMci);
    assert(pMciHw);

    PMC_EnablePeripheral(pMci->mciId);
    mciDis = PMC_IsPeriphEnabled(pMci->mciId);

    mciMr = pMciHw->HSMCI_MR & (~(uint32_t)HSMCI_MR_CLKDIV_Msk);
    /* Multimedia Card Interface clock (MCCK or MCI_CK) is Master Clock (MCK)
     * divided by (2*(CLKDIV+1))
     * mciSpeed = MCK / (2*(CLKDIV+1)) */
    if (mciSpeed > 0)
    {
        clkdiv = (mck / (mciSpeed * 2));
		
        if ( clkdiv > 0 )
        {
            clkdiv -= 1;
        }
        assert( (clkdiv & 0xFFFFFF00) == 0 ) ; /* "mciSpeed too small" */
    }
    else
	{
        clkdiv = 0 ;
    }

    /* Actual MCI speed */
    mciSpeed = mck / ((2 * clkdiv) + 2);
    /* Modify MR */
    pMciHw->HSMCI_MR = mciMr | clkdiv;
    if ( mciDis )
    {
        PMC_DisablePeripheral( pMci->mciId ) ;
    }

    return (mciSpeed);
}

/**
 * Configure the MCI_CFG to enable the HS mode
 * \param pMci     Pointer to the low level MCI driver.
 * \param hsEnable 1 to enable, 0 to disable HS mode.
 */
uint8_t MCI_EnableHsMode(Mcid* pMci, uint8_t hsEnable)
{
    Hsmci *pMciHw = pMci->pMciHw;
    uint32_t cfgr;
    uint8_t mciDis;
    uint8_t rc = 0;

    assert(pMci);
    assert(pMci->pMciHw);

    PMC_EnablePeripheral(pMci->mciId);
    mciDis = PMC_IsPeriphEnabled(pMci->mciId);

    cfgr = pMciHw->HSMCI_CFG;
    if (hsEnable == 1)
    {
        cfgr |=  HSMCI_CFG_HSMODE;
    }
    else
    {
        if (hsEnable == 0)
        {
            cfgr &= ~(uint32_t)HSMCI_CFG_HSMODE;
        }
        else
        {
            rc = ((cfgr & HSMCI_CFG_HSMODE) != 0);
        }
    }

    pMciHw->HSMCI_CFG = cfgr;
    if (mciDis)
    {
        PMC_DisablePeripheral(pMci->mciId);
    }

    return rc;
}

/**
 * Configure the  MCI SDCBUS in the MCI_SDCR register. Only two modes available
 *
 * \param pMci  Pointer to the low level MCI driver.
 * \param busWidth  MCI bus width mode. 00: 1-bit, 10: 4-bit, 11: 8-bit.
 */
uint32_t MCI_SetBusWidth(Mcid*pMci, uint32_t newBusWidth)
{
    Hsmci *pMciHw = pMci->pMciHw;
    uint32_t mciSdcr;
    uint32_t busWidth = 0;
    uint8_t mciDis;

    assert(pMci);
    assert(pMci->pMciHw);


    switch (newBusWidth)
    {
    	case 0:
    	{
    		busWidth = HSMCI_SDCR_SDCBUS_1;
    	}
    	break;
    	case 2:
		{
			busWidth = HSMCI_SDCR_SDCBUS_4;
		}
		break;
    	case 3:
		{
			busWidth = HSMCI_SDCR_SDCBUS_8;
		}
		break;

    }


    if( (busWidth != HSMCI_SDCR_SDCBUS_1) && (busWidth != HSMCI_SDCR_SDCBUS_4) && (busWidth != HSMCI_SDCR_SDCBUS_8) )
    {
    	TRACE_DEBUG("BusWidth Error");
        return (uint32_t)-1;
    }

    busWidth &= HSMCI_SDCR_SDCBUS_Msk ;

    PMC_EnablePeripheral(pMci->mciId);
    mciDis = PMC_IsPeriphEnabled(pMci->mciId);

    mciSdcr = (pMciHw->HSMCI_SDCR & ~(uint32_t)(HSMCI_SDCR_SDCBUS_Msk));
    pMciHw->HSMCI_SDCR = mciSdcr | busWidth;

    if (mciDis)
    {
        PMC_DisablePeripheral(pMci->mciId);
    }
    return 0;

}


#if MCI_BUSY_CHECK_FIX
/// Invoked after MCI_Init to enable busy line fix
void MCI_SetBusyFix(Mcid *pMci, const Pin * pDAT0)
{
    pMci->pPinDAT0 = pDAT0;
}
#endif


//------------------------------------------------------------------------------
/// Check NOTBUSY and DTIP bits of status register on the given MCI driver.
/// Return value, 0 for bus ready, 1 for bus busy
/// \param pMci  Pointer to a MCI driver instance.
//------------------------------------------------------------------------------
unsigned char MCI_CheckBusy(Mcid *pMci)
{
	Hsmci *pMciHw = pMci->pMciHw;
    volatile unsigned int status;

    // Enable MCI clock
    PMC_EnablePeripheral(pMci->mciId);

    status = pMciHw->HSMCI_SR;//READ_MCI(pMciHw, MCI_SR);

#if MCI_BUSY_CHECK_FIX
    if ((status & HSMCI_SR_DTIP) == 0) {
        unsigned char isBusy = 1;
        if (pMci->pPinDAT0) {
            Pin pinBsy;
            pinBsy.mask      = pMci->pPinDAT0->mask;
            pinBsy.pio       = pMci->pPinDAT0->pio;
            pinBsy.id        = pMci->pPinDAT0->id;
            pinBsy.type      = PIO_INPUT;
            pinBsy.attribute = PIO_PULLUP;
            PIO_Configure(&pinBsy, 1);
            isBusy = (PIO_Get(&pinBsy) == 0);
            PIO_Configure(pMci->pPinDAT0, 1);
        }
        else {
            isBusy = ((status & HSMCI_SR_NOTBUSY) == 0);
        }
        if (isBusy == 0) {
        	PMC_DisablePeripheral(pMci->mciId);
        }
        return isBusy;
    }

    return 1;
#else

    if(    ((status & HSMCI_SR_NOTBUSY)!=0)
        && ((status & HSMCI_SR_DTIP)==0)
        ) {

        // Disable MCI clock
    	PMC_DisablePeripheral(pMci->mciId);

        return 0;
    }
    else {
        return 1;
    }
#endif
}

