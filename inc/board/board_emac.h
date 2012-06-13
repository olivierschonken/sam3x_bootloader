#ifndef _BOARD_USB_
#define _BOARD_USB_

/*
 * EMAC
 * - BOARD_EMAC_PIN_PWRDN
 * - BOARD_EMAC_MODE_RMII
 * - BOARD_EMAC_PINS
 * - BOARD_EMAC_PIN_TEST
 * - BOARD_EMAC_PIN_RMII
 * - BOARD_EMAC_PINS_PHYAD
 * - BOARD_EMAC_PIN_10BT
 * - BOARD_EMAC_PIN_RPTR
 * - BOARD_EMAC_RST_PINS
 * - BOARD_EMAC_RUN_PINS
 *
 */

/** Board EMAC Power Down control pin */
#define BOARD_EMAC_PIN_PWRDN {(1<<18), PIOB, ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT}

/** Board EMAC mode - RMII/MII ( 1/0 ) */
#define BOARD_EMAC_MODE_RMII 0 // MII is the default mode

/** The PIN list of PIO for EMAC */
#define BOARD_EMAC_PINS     {0x3FFFF, PIOB, ID_PIOB, PIO_PERIPH_A, PIO_DEFAULT}

/** The power up reset latch PIO for PHY */
#define BOARD_EMAC_PIN_TEST   {(1<<15), PIOB, ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT}
#define BOARD_EMAC_PIN_RMII   {(1<<16), PIOB, ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT}
// We force the address
//(1<<5) PHY address 0, (1<<6) PHY address 1, (1<<13) PHY address 2,
//(1<<14) PHY address 3, (1<<4) PHY address 4
#define BOARD_EMAC_PINS_PHYAD {(1<<6)|(1<<13)|(1<<14)|(1<<4),\
                               PIOB, ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT},\
                              {(1<<5), PIOB, ID_PIOB, PIO_OUTPUT_1, PIO_DEFAULT}
#define BOARD_EMAC_PIN_10BT   {(1<<17), PIOB, ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT}
#define BOARD_EMAC_PIN_RPTR   {(1<< 7), PIOB, ID_PIOB, PIO_OUTPUT_0, PIO_DEFAULT}
/** The PIN Configure list for EMAC on power up reset (MII) */
#define BOARD_EMAC_RST_PINS BOARD_EMAC_PINS_PHYAD, \
                            BOARD_EMAC_PIN_TEST, BOARD_EMAC_PIN_RMII, \
                            BOARD_EMAC_PIN_10BT, BOARD_EMAC_PIN_RPTR

/** The runtime pin configure list for EMAC */
#define BOARD_EMAC_RUN_PINS BOARD_EMAC_PINS

#endif /* _BOARD_USB_ */
