#ifndef _BOARD_CAN_
#define _BOARD_CAN_

/*
 *
 * CAN
 * \ref PIN_CAN_TRANSCEIVER_RXEN
 * \ref PIN_CAN_TRANSCEIVER_RS
 */

// ----------------------------------------------------------------------------------------------------------
// CAN
// checked - tvd
// ----------------------------------------------------------------------------------------------------------
/// CAN RXEN: Select input for high speed mode or ultra low current sleep mode
#define PIN_CAN_TRANSCEIVER_RXEN { 1<<18, PIOA, ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT }
/// CAN RS: Select input for high speed mode or low-current standby mode
#define PIN_CAN_TRANSCEIVER_RS   { 1<<19, PIOA, ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT }
/// TXD: Transmit data input
#define PINS_CAN_TRANSCEIVER_TXD { 1<<13, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT }
/// RXD: Receive data output
#define PINS_CAN_TRANSCEIVER_RXD { 1<<14, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT }


#endif /* _BOARD_CAN_ */
