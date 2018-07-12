#pragma once
#include "Utils_stdafx.h"

#define  defFACT_BAUD_RATE               1000.0
#define  defFACT_FREQUENCY               1000000.0
#define  defVALID_DECIMAL_VALUE          0.004

// Maximum value of SJW    = SJWbit + 1 ( BTR0 :6-7 )
#define  defMAX_SJW                    4
// Maximum value of NBT    = TSEG2+TSEG1+1
#define  defMAX_NBT                    25
// Maximum value of BRP    = BRPbit + 1 ( BTR0 :0-5 )
#define  defMAX_BRP                    64
// Minimum value of NBT    = TSEG2+TSEG1+1
#define  defMIN_NBT                    8
// Find Min of two numbers
#define  defmcMIN2(X,Y)    ( ( X<Y ) ? X:Y )
// Maximum value of TSEG2   = TSEG2bit + 1 ( BTR1 : 4-6)
#define  defMAX_TSEG2                      8
// Minimum value of TSEG1   = TSEG1bit + 1 ( BTR1 : 0-3)
#define  defMIN_TSEG1                      3
// Find Max of three numbers
#define  defmcMAX3(X,Y,Z)  ( ( ( ( X>Y ) ? X:Y ) > Z ) ? ( ( X>Y ) ? X:Y ) : Z )
// Minimum value of TSEG2  = TSEG2bit + 1 ( BTR1 : 4-6)
#define  defMIN_TSEG2                  2
// Maximum value of TSEG1   = TSEG1bit + 1 ( BTR1 : 0-3)
#define  defMAX_TSEG1                      16
// Minimum vaue of Sampling point = ( (NBT-TSEG2)/NBT ) * 100
#define  defMIN_SAMPLING                   50
// Maximum list of values can be displayed in list box
#define  defREG_VALUE_LIST_COUNT_MAX     200
#define  defHEXADECIMAL                   16
#define defCLOCK            ("16")

// Defination of BTR0 Register
struct sBTR0
{
	UCHAR  ucBRPbit : 6;    // 0..5   (6 bits)
	UCHAR  ucSJWbit : 2;    // 6..7   (2 bits)
};

// Packing of Struct BTR0 in one Byte
union uBTR0
{
	UCHAR ucBTR0;
	sBTR0 sBTR0Bit;
};

// Defination of BTR1 Register
struct sBTR1
{
	UCHAR ucTSEG1bit : 4;    // 0..3   (4 bits)
	UCHAR ucTSEG2bit : 3;    // 4..7   (3 bits)
	UCHAR ucSAMbit : 1;    // 8      (1 bit )
};

// Packing of Struct BTR1 in one Byte
union  uBTR1
{
	UCHAR ucBTR1;
	sBTR1 sBTR1Bit;
};

// For Values of BRP, NBT , Sampling Percentage and SJW
struct sBRP_NBT_SAMP_n_SJW
{
	USHORT usBRP;
	USHORT usNBT;
	USHORT usSampling;
	USHORT usSJW;
};
// structure defination of all five columns in the list control
struct sBTRTemp
{
	uBTR1                uBTRReg1;
	uBTR0                uBTRReg0;
	sBRP_NBT_SAMP_n_SJW  sBRPNBTSampNSJW;
};