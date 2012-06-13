
#ifndef MEDIA_INIT_H
#define MEDIA_INIT_H

/*---------------------------------------------------------------------------
                        MODULE INCLUDES (ONLY FOR SUB H FILES)
-----------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                             GLOBAL DEFINED CONSTANTS
-----------------------------------------------------------------------------*/
#define TRANSLATENANDFLASH	1
//#define SKIPBLOCKNANDFLASH	1
/// Maximum number of LUNs which can be defined.
/// (Logical drive = physical drive = medium number)
#define MAX_LUNS        3


/*---------------------------------------------------------------------------
                                  GLOBAL MACROS
-----------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                                   GLOBAL DATA TYPES
-----------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------
                                 GLOBAL VARIABLES
-----------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                                     EXTERNALS
-----------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------
                             GLOBAL FUNCTION PROTOTYPES
-----------------------------------------------------------------------------*/

extern int Medias_Init(void);

#endif /* NANDDRV_H */
