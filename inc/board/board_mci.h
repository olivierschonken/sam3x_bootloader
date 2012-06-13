#ifndef _BOARD_MCI_
#define _BOARD_MCI_

/*
 * MCI
 * - \ref PINS_MCI
 *
 */
 
// ----------------------------------------------------------------------------------------------------------
// MCI
// ----------------------------------------------------------------------------------------------------------
/** MCI pins definition. */
#define PINS_MCI   {0x3fUL << 19, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}, \
				   {0x1UL << 19, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
/** MCI pin Card Detect. */
#define PIN_MCI_CD {PIO_PE6, PIOE, ID_PIOE, PIO_INPUT, PIO_PULLUP}

/// MCI pin DAT0 (busy)
#define PIN_MCI_DAT0 {PIO_PA21, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}


#endif /* _BOARD_MCI_ */
