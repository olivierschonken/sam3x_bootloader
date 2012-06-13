#ifndef _BOARD_ADC_
#define _BOARD_ADC_

/*
 * ADC
 * - \ref PIN_ADC0_AD1
 * - \ref PINS_ADC
 *
 */

/** ADC_AD1 pin definition. */
#define PIN_ADC0_AD1 {PIO_PA3X1_AD1_WKUP1, PIOA, ID_PIOA, PIO_INPUT, PIO_DEFAULT}

/** Pins ADC */
#define PINS_ADC PIN_ADC0_AD1

/** Startup time max, return from Idle mode (in µs) */
#define ADC_STARTUP_TIME_MAX       15
/** Track and hold Acquisition Time min (in ns) */
#define ADC_TRACK_HOLD_TIME_MIN  1200
/** ADC clock frequence */
#define BOARD_ADC_FREQ     (6000000)


#endif /* _BOARD_ADC_ */
