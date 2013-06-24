/**
 * \file      CAPLStrtingWrapper.c
 * \brief     Include the Wrapper functions for CAPL String operation Functions
 * \author    Venkatanarayana Makam
 * \date      Created 12/01/2011
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#include "CANIncludes.h"
#include <stdio.h>
#include <stdlib.h>


/* CAPL Function Details                                                    */
/* Syntax:      double atodbl(char s[]);                                    */
/* Parameters:  s is Input string to be converted.                          */
/* Return Value: Double number, the converted string.                       */
//typedef unsigned char byte;



double atodbl_CAPL(char s[])
{
    double dValue;
    char* chStopString;
    dValue = strtod(s, &chStopString);
    return dValue;
}

/* CAPL Function Details                                                    */
/* Syntax:      long atol(char s[]);                                        */
/* Parameters:  s is Input string to be converted.                          */
/* Return Value: long integer                                               */

/* CAPL Function Details                                                    */
/* Syntax:      void ltoa(long val, char s[], long base);                   */
/* Parameters:  val, Number to be converted.                                */
/*              s, String, which contains the converted number.             */
/*              base, Number base.                                          */
/* Return Value: void                                                       */

/*
Unlike the strncat C-function, rather than the number of characters to be
appended, len indicates the maximum length of the composite string, including
the terminating "\0".
*/

void strncat_CAPL(char pchDest[], char pscSrc[], long nMaxLen)
{
    int nDestLen;
    int nSrcLen;
    nDestLen = strlen(pchDest);
    strncat(pchDest, pscSrc, nMaxLen-nDestLen-1);
}
int strncmp_CAPL ( char s1[], char s2[], ... )
{
    /*int nLen, nOffset, nArgCount;

    va_list marker;

    va_start( marker, s2 );

    int n = va_arg( argptr, int );
    i = va_arg( marker, int);
    while( i != -1 )
    {
      sum += i;
      count++;
      i = va_arg( marker, int);
    }
    va_end( marker );
    return( sum ? (sum / count) : 0 );*/
    return 0;
}

long strncmp_off_CAPL(char s1[], long s1offset, char s2[], long s2offset, long len)
{
    return strncmp( s1+s1offset, s2+s2offset, len);
}

/*
This function copies src to dest. max indicates the maximum length of src
and dest. The function ensures that there is a terminating ‘\0’. Thus, a
maximum of max-1-destOffset characters are copied. Characters are overwritten
in dest starting at destOffset.
*/
void strncpy_off_CAPL(char dest[], long destOffset, char src[], long max)
{
    strncpy(dest+destOffset, src, strlen(src));
}

long strstr_CAPL(char s1[], char s2[])
{
    char* pdest;
    int  result;
    pdest = strstr( s1, s2 );
    result = (int)(pdest - s1);
    return ( ( pdest != NULL ) ? result : -1 );
}
long strstr_off_CAPL(char s1[], long offset, char s2[])
{
    return strstr_CAPL(s1+offset, s2);
}

void substr_cpy_CAPL(char dest[], char src[], long srcStart, long len, long max)
{
    char* pchTemp;
    if (-1 == len )
    {
        strcpy(dest, src+srcStart);//, strlen(src);
    }
    else
    {
        pchTemp = (char*)malloc( ( len ) * sizeof(char) );
        strncpy(pchTemp, src+srcStart, len);
        strcpy(dest, pchTemp);
    }
    free(pchTemp);
}

void substr_cpy_off_CAPL(char dest[], long destOffset, char src[], long srcStart, long len, long max)
{
    substr_cpy_CAPL(dest+destOffset, src, srcStart, len, max);
}

unsigned char* byte_CAPL(STCAN_MSG* sMsg, int nByte)
{
    return ((&(sMsg->m_sWhichBit.m_aucData[nByte])));
}

unsigned short* word_CAPL(STCAN_MSG* sMsg, int nByte)
{
    return  ((unsigned short*)(&(sMsg->m_sWhichBit.m_aucData[nByte])));
}

unsigned long* long_CAPL(STCAN_MSG* sMsg, int nByte)
{
    return  ((unsigned long*)(&(sMsg->m_sWhichBit.m_aucData[nByte])));
}

int settimer_CAPL(int nTimerType, char* pchTimerName, int nTimerValue, int nNanoSec)
{
    int nMilliSec = nNanoSec  % (1000 * 1000);
    nTimerValue += nMilliSec;
    if(nTimerType == 1)
    {
        SetTimerVal(pchTimerName, nTimerValue*1000);
        StartTimer(pchTimerName, 0);
    }
    else
    {
        SetTimerVal(pchTimerName, nTimerValue*1);
        StartTimer(pchTimerName, 0);
    }
    return 0;
}
