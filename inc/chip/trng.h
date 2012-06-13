#ifndef _RANDOMGEN_
#define _RANDOMGEN_

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "chip.h"

/*----------------------------------------------------------------------------
 *        Macros
 *----------------------------------------------------------------------------*/
extern void TRNG_Init( Trng* trng );
extern void TRNG_GetData( unsigned int *data, unsigned int length);

#endif /* #ifndef _TRNG_ */
