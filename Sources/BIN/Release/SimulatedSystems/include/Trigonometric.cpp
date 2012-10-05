/**
 * \file      CAPLTrigonometricFunctions
 * \brief
 * \author    Venkatanarayana Makam
 * \date      Created 12/01/2011
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */


#include "CANIncludes.h"
#include <stdio.h>
#include <math.h>
#include <basetsd.h>

/* _round Function */
int _round_CAPL(double number)
{
    return (number >= 0) ? (int)(number + 0.5) : (int)(number - 0.5);
}

/* _round64 Function */
INT64 _round64_CAPL(double number)
{
    return (number >= 0) ? (INT64)(number + 0.5) : (INT64)(number - 0.5);
}

/* random function */
UINT random_CAPL(UINT dwLimit)
{
    int nRandomNumber = rand();
    return (nRandomNumber % dwLimit);
}

/* http://www.codeguru.com/forum/showthread.php?t=292902*/
/* swapWord */
unsigned short swapword_CAPL(unsigned short shVal)
{
    shVal = (shVal>>8) |
            (shVal<<8);
    return shVal;
    //
}

/* swapInt */
short swapint_CAPL(short shVal)
{
    shVal = (shVal>>8) |
            (shVal<<8);
    return ( (shVal < 0) ? (-1 * shVal) : shVal );
}

/* swapDWord */
unsigned int swapdword_CAPL(unsigned int lVal)
{
    lVal = (lVal>>24) |
           ((lVal<<8) & 0x00FF0000) |
           ((lVal>>8) & 0x0000FF00) |
           (lVal<<24);
    return lVal;
}
/* swapLong */
int swaplong_CAPL(int lVal)
{
    lVal = (lVal>>24) |
           ((lVal<<8) & 0x00FF0000) |
           ((lVal>>8) & 0x0000FF00) |
           (lVal<<24);
    return ( (lVal < 0) ? (-1 * lVal) : lVal );
}

