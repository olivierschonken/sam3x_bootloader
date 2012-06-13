#ifndef _BOARD_NORFLASH_
#define _BOARD_NORFLASH_

/*
 * \section NorFlash
 * - \ref BOARD_NORFLASH_ADDR
 * - \ref BOARD_NORFLASH_DFT_BUS_SIZE
 *
 */

// ----------------------------------------------------------------------------------------------------------
// NOR FLASH
// ----------------------------------------------------------------------------------------------------------
/** Address for transfering command bytes to the norflash. */
#define BOARD_NORFLASH_ADDR     0x63000000
/** Default NOR bus size after power up reset */
#define BOARD_NORFLASH_DFT_BUS_SIZE 8


#endif /* _BOARD_NORFLASH_ */
