#ifndef _BOARD_TOUCHSCREEN_
#define _BOARD_TOUCHSCREEN_

/*
 * TouchScreen
 *
 * - \ref BOARD_TSC_ADS7843
 * - \ref PIN_TCS_IRQ
 * - \ref PIN_TCS_BUSY
 * - \ref BOARD_TSC_SPI_BASE
 * - \ref BOARD_TSC_SPI_ID
 * - \ref BOARD_TSC_SPI_PINS
 * - \ref BOARD_TSC_NPCS
 * - \ref BOARD_TSC_NPCS_PIN
 *
 */

// ----------------------------------------------------------------------------------------------------------
// Touchscreen
// checked - tvd
// ----------------------------------------------------------------------------------------------------------
/** Touchscreen controller IRQ pin definition. */
#define PIN_TSC_IRQ  {PIO_PA31, PIOA, ID_PIOA, PIO_INPUT, PIO_PULLUP}
#define PIN_TSC_IRQ_WUP_ID (1 << 3)
/** Touchscreen controller Busy pin definition. */
#define PIN_TSC_BUSY {PIO_PA30, PIOA, ID_PIOA, PIO_INPUT, PIO_PULLUP}

/** Base address of SPI peripheral connected to the touchscreen controller. */
#define BOARD_TSC_SPI_BASE         SPI0
/** Identifier of SPI peripheral connected to the touchscreen controller. */
#define BOARD_TSC_SPI_ID           ID_SPI0
/** Pins of the SPI peripheral connected to the touchscreen controller. */
#define BOARD_TSC_SPI_PINS         PINS_SPI
/** Chip select connected to the touchscreen controller. */
#define BOARD_TSC_NPCS             0
/** Chip select pin connected to the touchscreen controller. */
#define BOARD_TSC_NPCS_PIN         PIO_PA28A_SPI0_NPCS0

#endif /* _BOARD_TOUCHSCREEN_ */
