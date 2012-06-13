
/*---------------------------------------------------------------------------
                                 MODULE INCLUDES
-----------------------------------------------------------------------------*/

#include "Media_Init.h"
#include "board.h"
#include "memories.h"
#include <RawNandFlash.h>
#include <TranslatedNandFlash.h>
#include <ff.h>
#include <stdio.h>
#include "MEDNandFlash.h"
#include "Media.h"
#include "diskio.h"
#include "led.h"
#include "wdt.h"
#include "string.h"
/*---------------------------------------------------------------------------
                             LOCAL DEFINED CONSTANTS
-----------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                                  LOCAL MACROS
-----------------------------------------------------------------------------*/

#define NAND_ESRAM_BUFFER(bufName) bufName __attribute__ ((section (".nandinfo")))
/*---------------------------------------------------------------------------
                                LOCAL DATA TYPES
-----------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                                   LOCAL TABLES
-----------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                              LOCAL FUNCTION PROTOTYPES
-----------------------------------------------------------------------------*/
void FF_ScanDir(char* path);

/*---------------------------------------------------------------------------
                                 GLOBAL VARIABLES
-----------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                                  LOCAL VARIABLES
-----------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                             GLOBAL FUNCTION DEFINITIONS
-----------------------------------------------------------------------------*/

/** Nandflash memory size. */
static unsigned int memSize;
/** Number of blocks in nandflash.*/
static unsigned int numBlocks;
/** Size of one block in the nandflash, in bytes.*/
static unsigned int blockSize;
/** Size of one page in the nandflash, in bytes.*/
static unsigned int pageSize;
/** Number of page per block*/
static unsigned int numPagesPerBlock;


/** Pins used to access to nandflash.*/
static const Pin pPinsNf[] = {PINS_NANDFLASH};
/** Nandflash device structure.*/
#if defined(TRANSLATENANDFLASH)
	static struct TranslatedNandFlash translatedNf;
#endif
/** Address for transferring command bytes to the nandflash.*/
static unsigned int cmdBytesAddr = BOARD_NF_COMMAND_ADDR;
/** Address for transferring address bytes to the nandflash.*/
static unsigned int addrBytesAddr = BOARD_NF_ADDRESS_ADDR;
/** Address for transferring data bytes to the nandflash.*/
static unsigned int dataBytesAddr = BOARD_NF_DATA_ADDR;
/** Nandflash chip enable pin.*/
static const Pin nfCePin = BOARD_NF_CE_PIN;
/** Nandflash ready/busy pin.*/
static const Pin nfRbPin = BOARD_NF_RB_PIN;


///////////////////////////////
/// enable fatfs on nandflash /
///////////////////////////////
FATFS NAND_ESRAM_BUFFER(fs[MAX_LUNS]);
Media NAND_ESRAM_BUFFER(medias[MAX_LUNS]);
/// Number of medias which are effectively used.

/*---------------------------------------------------------------------------
   Function   : FatFs_NandFlash_Init
 -----------------------------------------------------------------------------*/
 /**
 *  @brief 	Init Nandflash for FatFS
 *  @retval Returns 0 if succesful; otherwise, returns error code.
 *  @remarks None
 */
int Medias_Init(void)
{
    /* Temporary buffer */
    unsigned short block, page;
    unsigned int i;
    /* Errors returned by SkipNandFlash functions */
    unsigned char error = 0;
    DIR dirs;

    FRESULT res;
    /* Configure SMC for Nandflash accesses */
#if 1
    BOARD_ConfigureNandFlash(SMC);
	PIO_Configure(pPinsNf, PIO_LISTSIZE(pPinsNf));

	memset(&translatedNf, 0, sizeof(translatedNf));

	if (TranslatedNandFlash_Initialize(&translatedNf,
							   0,
							   cmdBytesAddr,
							   addrBytesAddr,
							   dataBytesAddr,
							   nfCePin,
							   nfRbPin, 0, 2048)) {

	   printf("-E- Device Unknown\n\r");
	   return 0;
	}

	printf("-I- Nandflash driver initialized\n\r");

	/* Get device parameters */
	memSize = NandFlashModel_GetDeviceSizeInBytes(&translatedNf.mapped.managed.ecc.raw.model);
	blockSize = NandFlashModel_GetBlockSizeInBytes(&translatedNf.mapped.managed.ecc.raw.model);
	numBlocks = NandFlashModel_GetDeviceSizeInBlocks(&translatedNf.mapped.managed.ecc.raw.model);
	pageSize = NandFlashModel_GetPageDataSize(&translatedNf.mapped.managed.ecc.raw.model);
	numPagesPerBlock = NandFlashModel_GetBlockSizeInPages(&translatedNf.mapped.managed.ecc.raw.model);

	printf("-I- Size of the whole device in bytes : 0x%x \n\r",memSize);
	printf("-I- Size in bytes of one single block of a device : 0x%x \n\r",blockSize);
	printf("-I- Number of blocks in the entire device : 0x%x \n\r",numBlocks);
	printf("-I- Size of the data area of a page in bytes : 0x%x \n\r",pageSize);
	printf("-I- Number of pages per block : 0x%x \n\r",numPagesPerBlock);
	MEDNandFlash_Initialize(&medias[DRV_NAND] , &translatedNf);
#endif


#if 1
	MEDSdcard_Initialize(&medias[DRV_MMC], 0 ); //TODO: Try to fix MMC SD init

	memset(&fs[DRV_MMC], 0, sizeof(FATFS));
	res = f_mount(DRV_MMC, &fs[DRV_MMC]);
	if( res != FR_OK )
	{
		printf("-E- f_mount pb: 0x%X\n\r", res);
		return 0;
	}


	// Test if the disk is formated
	res = f_opendir (&dirs, MMC_ROOT_DIRECTORY);
	if(res == FR_OK )
	{
		// erase sdcard to re-format it ?
		printf("-I- The disk is already formated.\n\r");

		// Display the file tree
		printf("-I- Display files contained on the SDcard :\n\r");
		//FF_ScanDir(MMC_ROOT_DIRECTORY);

    }

#endif

#if 1
	res = f_mount(DRV_NAND, &fs[DRV_NAND]);
    if( res != FR_OK )
    {
    	printf("-E- f_mount pb: 0x%X\n\r", res);
    	return 0;
    }
#endif
	return 0;
}


//------------------------------------------------------------------------------
/// Scan the directory passed in argument and display all the files and
/// directories contained in it and in its subdirectories
/// (recursive function)
/// \param path  directory path to scan
//------------------------------------------------------------------------------
void FF_ScanDir(char* path)
{
    FILINFO finfo;
    DIR dirs;
    int i;

    if (f_opendir(&dirs, path) == FR_OK) {

        i = strlen(path);
        while ((f_readdir(&dirs, &finfo) == FR_OK) && finfo.fname[0]) {

            if (finfo.fattrib & AM_DIR) {

                sprintf(&path[i], "/%s", &finfo.fname[0]);
                printf("%s\n\r", path);
                FF_ScanDir(path);
                path[i] = 0;
            }
            else {
                printf("%s/%s\n\r", path, &finfo.fname[0]);
            }
        }
    }
}




