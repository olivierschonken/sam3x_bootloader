
/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "board.h"
#include "cmsis/core_cm3.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "ff.h"
#include "diskio.h"
#include "Media_Init.h"
#include "linuxboot.h"

#define SRAM_BASE			0x70000000
#define SRAM_SIZE			0x2000000

#define ZIMAGE_LOAD_ADDR	(SRAM_BASE + 0x008000)
#define RAMDISK_LOAD_ADDR	(SRAM_BASE + 0x800000)
/*---------------------------------------------------------------------------
                              LOCAL FUNCTION DEFINITIONS
-----------------------------------------------------------------------------*/
static void loadLinux(void);
static int load_image(unsigned int dst, const char* FileName);

/*----------------------------------------------------------------------------
 *        Local variables
 *----------------------------------------------------------------------------*/
static unsigned int machine_type = 5000;
/*********** NOTE add rootflags=noatime to stop timestamping of files */
/* Linux commandline parameters */
static const char bootargs[ATAG_CMD_LINE_LEN] =
{"mem=32M init=/bin/sh console=ttyS0,115200 ramdisk_size=16384"};

/**
 *  \brief Configure LEDs
 *
 *  Configures LEDs \#1 and \#2 (cleared by default).
 */
static void _ConfigureLeds( void )
{
    LED_Configure( LED_GREEN ) ;
    LED_Configure( LED_AMBER ) ;
    LED_Configure( LED_BLUE ) ;
    LED_Configure( LED_RED ) ;
}

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

/**
 *  \brief getting-started Application entry point.
 *
 *  \return Unused (ANSI-C compatibility).
 */
extern int main( void )
{
#if defined   ( __CC_ARM   ) /* Keil �Vision 4 */
    /* Disable semihosting */
#    pragma import(__use_no_semihosting_swi)
#endif
	uint8_t result = 0;
	/* Disable watchdog */
    WDT_Disable( WDT ) ;
    TRACE_CONFIGURE(115200, BOARD_MCK);

    printf( "-- %s\n\r", BOARD_NAME ) ;
    printf( "-- Compiled: %s %s --\n\r", __DATE__, __TIME__ ) ;

    /* complete SDRAM configuration.*/
    BOARD_ConfigureSdram() ;

    printf( "Configure TC.\n\r" );
    //_ConfigureTc() ;

    printf( "Configure LED PIOs.\n\r" ) ;
    _ConfigureLeds() ;

    /* Initialize LCD */
	LCD_Initialize() ;

	LCD_SetBacklight(32);

	LCD_On();

	/* Test basic color space translation and LCD_DrawFilledRectangle */
	LCD_SetColor(COLOR_WHITE);
	LCD_DrawFilledRectangle(0, 0, BOARD_LCD_WIDTH-1, BOARD_LCD_HEIGHT-1);

	LCD_SetColor(COLOR_BLACK);
	LCD_DrawFilledRectangle(BOARD_LCD_WIDTH-5, BOARD_LCD_HEIGHT-5, 4, 4);

	LCD_SetColor(COLOR_BLUE);
	LCD_DrawFilledRectangle(8, 8, BOARD_LCD_WIDTH-9, BOARD_LCD_HEIGHT-9);

	LCD_SetColor(COLOR_RED);
	LCD_DrawFilledRectangle(12, 12, BOARD_LCD_WIDTH-13, BOARD_LCD_HEIGHT-13);

	LCD_SetColor(COLOR_GREEN);
	LCD_DrawFilledRectangle(16, 14, BOARD_LCD_WIDTH-17, BOARD_LCD_HEIGHT-17);

	LCD_SetColor(COLOR_RED);

	/* Test horizontal/vertial LCD_drawLine  */
	LCD_DrawLine(0, BOARD_LCD_HEIGHT/2, BOARD_LCD_WIDTH-1, BOARD_LCD_HEIGHT/2);
	LCD_DrawLine(BOARD_LCD_WIDTH/2, 0, BOARD_LCD_WIDTH/2, BOARD_LCD_HEIGHT-1);

	/* Test Bresenham LCD_drawLine  */
	LCD_DrawLine(0, 0, BOARD_LCD_WIDTH-1, BOARD_LCD_HEIGHT-1);
	LCD_DrawLine(0, BOARD_LCD_HEIGHT-1, BOARD_LCD_WIDTH-1, 0);

	/* Test LCD_DrawRectangle */
	LCD_DrawRectangle(BOARD_LCD_WIDTH/4, BOARD_LCD_HEIGHT/4, BOARD_LCD_WIDTH*3/4, BOARD_LCD_HEIGHT*3/4);
	LCD_DrawRectangle(BOARD_LCD_WIDTH*2/3, BOARD_LCD_HEIGHT*2/3, BOARD_LCD_WIDTH/3, BOARD_LCD_HEIGHT/3);

	/* Test LCD_DrawFilledCircle */
	LCD_SetColor(COLOR_BLUE);
	LCD_DrawFilledCircle(BOARD_LCD_WIDTH*3/4, BOARD_LCD_HEIGHT*3/4, BOARD_LCD_WIDTH/4);
	LCD_DrawCircle(BOARD_LCD_WIDTH/4, BOARD_LCD_HEIGHT/4, BOARD_LCD_HEIGHT/4);
	LCD_SetColor(COLOR_YELLOW);
	LCD_DrawFilledCircle(BOARD_LCD_WIDTH/4, BOARD_LCD_HEIGHT*3/4, BOARD_LCD_HEIGHT/4);
	LCD_DrawCircle(BOARD_LCD_WIDTH*3/4, BOARD_LCD_HEIGHT/4, BOARD_LCD_WIDTH/4);

	/* Test LCD_DrawPicture */
	RTT_SetPrescaler(RTT, 32768 );
    result = Medias_Init();
	loadLinux();
}


//------------------------------------------------------------------------------
//         Local functions
//------------------------------------------------------------------------------
static void loadLinux(void)
{

    LINUX_MACHINE_PARMS lparms;

   	const char* kernelFile = MMC_ROOT_DIRECTORY "Image";
   	const char* ramdiskFile = MMC_ROOT_DIRECTORY "ramdisk";
    lparms.kernel_size = load_image(ZIMAGE_LOAD_ADDR, kernelFile);

    lparms.ramdisk_size = load_image(RAMDISK_LOAD_ADDR, ramdiskFile);

    /* Set up the rest of the Linux machine parameters */
    lparms.machine = machine_type;
    lparms.ram_base = SRAM_BASE;
    lparms.ram_size = SRAM_SIZE;
    lparms.kernel_addr = ZIMAGE_LOAD_ADDR;
    lparms.ramdisk_addr = RAMDISK_LOAD_ADDR;
    lparms.bootargs = (char *)bootargs;
    printf("Booting Linux\n\r");
    bootlinux(&lparms);

}

/*---------------------------------------------------------------------------
  Function   : load_uimage
  Purpose    : Loads a binary image from Flash memory into SRAM. The flash
               image is formatted as a u-boot image using u-boot's
               "mkimage" utility.
  Parameters : dst   - Destination SRAM address
               src   - Source Flash memory address
  Returns    : Returns the length of the unformatted image in bytes
  Notes      : None
-----------------------------------------------------------------------------*/
static int load_image(unsigned int dst, const char* FileName)
{
	char text[16];
	uimage_hdr *uip;
	int i, len;
	unsigned char *da = (unsigned char *)dst;
    unsigned int ByteToRead;
    unsigned int curOffset;
    unsigned int ByteRead;
    FIL FileObject;
   	FRESULT res;

	 printf("-I- Open the same file : \"%s\"\n\r", FileName);
  	 res = f_open(&FileObject, FileName, FA_OPEN_EXISTING|FA_READ);
	 if( res != FR_OK ) {
		printf("-E- f_open read pb: 0x%X \n\r", res);
		//return 0;
	 }

	 // Read file
	 printf("-I- Read file\n\r");
	 memset(da, 0, FileObject.fsize);
	 //FileObject.fsize
	 curOffset = 0;
	 while (curOffset < FileObject.fsize)
	 {
		 ByteToRead = 512;
		 res = f_read(&FileObject, da, ByteToRead, &ByteRead);
		 if(res != FR_OK) {
			printf("-E- f_read pb: 0x%X \n\r", res);
			//return 0;
			len = 0;
		 }
		 else
		 {
			 curOffset += ByteRead;
			 da += ByteRead;
		 }
	 }

	 len = FileObject.fsize;

	 // Close the file
	 printf("-I- Close file\n\r");
	 res = f_close(&FileObject);
	 if( res != FR_OK ) {
		printf("-E- f_close pb: 0x%X \n\r", res);
		//return 0;
	 }
	return len;
}



