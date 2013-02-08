#ifndef __MHS_TYPES_H__
#define __MHS_TYPES_H__

#include "global.h"

#ifdef __cplusplus
extern "C" {
#endif

	// Minimum value of NBT    = TSEG2+TSEG1+1
	#define  defMIN_NBT                    8

	// Maximum value of NBT    = TSEG2+TSEG1+1
	#define  defMAX_NBT                    25

	// Maximum value of BRP    = BRPbit + 1 ( BTR0 :0-5 )
	#define  defMAX_BRP                    64

	// Minimum value of   BRP   = BRPbit + 1 ( BTR0 :0-5 )
	#define  defMIN_BRP                       1

	#define  defFACT_BAUD_RATE               1000.0
	#define  defFACT_FREQUENCY               1000000.0

    struct TMhsCanCfg
    {
        char CanSnrStr[MHS_STR_SIZE];
        unsigned int CanSpeed;
        unsigned int CanBtrValue;
		BOOL m_bBitRateSelected;
    };

    // Defination of BTR0 Register
    typedef struct sBTR0
    {
        UCHAR  ucBRPbit   : 6;    // 0..5   (6 bits)
        UCHAR  ucSJWbit   : 2;    // 6..7   (2 bits)
    }SBTR0;

    // Packing of Struct BTR0 in one Byte
    typedef union uBTR0
    {
        UCHAR ucBTR0;
        SBTR0 sBTR0Bit;
    }UBTR0;

    // Defination of BTR1 Register
    typedef struct sBTR1
    {
        UCHAR ucTSEG1bit  : 4;    // 0..3   (4 bits)
        UCHAR ucTSEG2bit  : 3;    // 4..7   (3 bits)
        UCHAR ucSAMbit    : 1;    // 8      (1 bit )
    }SBTR1;

    // Packing of Struct BTR1 in one Byte
    typedef union  uBTR1
    {
        UCHAR ucBTR1;
        SBTR1 sBTR1Bit;
    }UBTR1;

#ifdef __cplusplus
}
#endif

#endif
