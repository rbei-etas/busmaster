#pragma once
#include <windows.h>


#define snprintf_CAPL _snprintf
/* _max Function */
#define _max_CAPL(a,b) ((a)>(b)?(a):(b))

/* _min Function */
#define _min_CAPL(a,b) ((a)<(b)?(a):(b))

/* _pow Function */
#define _pow_CAPL     pow

/* abs Functions */
#define abs_CAPL(a) (((a) < 0) ? (-1*(a)) : (a))

//Key Handlers

#define MSEC_TIMER      0
#define SEC_TIMER       1

//CAPL KeyEvent  value
#define F1              0x5400
#define F2              0x5500
#define F3              0x5600
#define F4              0x5700
#define F5              0x5800
#define F6              0x5900
#define F7              -1
#define F8              -1
#define F9              -1
#define F10             0x5D00
#define F11             0x8700
#define F12             0x8800
#define PageUp          0x4900
#define PageDown        0x5100
#define HOME            0x5109

GCC_EXTERN INT _round_CAPL(DOUBLE number);
GCC_EXTERN INT64 _round64_CAPL(DOUBLE number);
GCC_EXTERN UINT random_CAPL(UINT dwLimit);
GCC_EXTERN USHORT swapword_CAPL(USHORT shVal);
GCC_EXTERN SHORT swapint_CAPL(SHORT shVal);
GCC_EXTERN UINT swapdword_CAPL(UINT lVal);
GCC_EXTERN INT swaplong_CAPL(INT lVal);
GCC_EXTERN LONG fileclose_CAPL (UINT dwFileHandle);
GCC_EXTERN UINT openfileread_CAPL (CHAR pchFileName[], UINT dwMode);
GCC_EXTERN UINT openfilewrite_CAPL (CHAR pchFileName[], UINT dwMode);
GCC_EXTERN LONG filegetbinaryblock_CAPL (byte byBuffer[], LONG lBufferSize, UINT dwFileHandle);
GCC_EXTERN LONG filegetstring_CAPL (CHAR pchBuffer[], LONG lBuffSize, UINT dwFileHandle);
GCC_EXTERN LONG fileputstring_CAPL (CHAR pchBuffer[], LONG lBuffSize, UINT dwFileHandle);
GCC_EXTERN LONG filerewind_CAPL (UINT dwFileHandle);
GCC_EXTERN LONG filewritebinaryblock_CAPL (byte byBuffer[], LONG lBufferSize, UINT dwFileHandle);
GCC_EXTERN LONG getprofileint_CAPL (CHAR pchSection[], CHAR pchEntry[], LONG lDefVal, CHAR pchFileName[]);
GCC_EXTERN LONG getprofilestring_CAPL (CHAR pchSection[], CHAR pchEntry[], CHAR pchDefVal[],
                                       CHAR pchReturnBuff[],   LONG lBuffSize, CHAR pchFileName[] );
GCC_EXTERN LONG writeprofileint_CAPL (CHAR pchSection[], CHAR pchEntry[], LONG lVal,
                                      CHAR pchFileName[]);
GCC_EXTERN LONG writeprofilestring_CAPL(CHAR pchSection[], CHAR pchEntry[], CHAR pchVal[],
                                        CHAR pchFileName[] );
GCC_EXTERN LONG writeprofilefloat_CAPL(CHAR pchSection[], CHAR pchEntry[], float fVal,
                                       CHAR pchFileName[]);
GCC_EXTERN FLOAT getprofilefloat_CAPL( CHAR pchSection[], CHAR pchEntry[], LONG lDefVal,
                                       CHAR pchFileName[] );
GCC_EXTERN LONG getprofilearray_CAPL(CHAR pchSection[], CHAR pchEntry[],CHAR pchBuff[],
                                     LONG lBuffSize, CHAR pchFileName[]);
GCC_EXTERN VOID setwritepath_CAPL (CHAR pchRelOrAbsPath[]);
GCC_EXTERN VOID setfilepath_CAPL (CHAR pchDirPath[], UINT dwMode);
GCC_EXTERN DOUBLE atodbl_CAPL(CHAR s[]);
GCC_EXTERN VOID strncat_CAPL(CHAR pchDest[], CHAR pscSrc[], LONG nMaxLen);
GCC_EXTERN INT strncmp_CAPL ( CHAR s1[], CHAR s2[], ... );
GCC_EXTERN LONG strncmp_off_CAPL(CHAR s1[], LONG s1offset, CHAR s2[], LONG s2offset, LONG len);
GCC_EXTERN VOID strncpy_off_CAPL(CHAR dest[], LONG destOffset, CHAR src[], LONG max);
GCC_EXTERN LONG strstr_CAPL(CHAR s1[], CHAR s2[]);
GCC_EXTERN LONG strstr_off_CAPL(CHAR s1[], LONG offset, CHAR s2[]);
GCC_EXTERN VOID substr_cpy_CAPL(CHAR dest[], CHAR src[], LONG srcStart, LONG len, LONG max);
GCC_EXTERN VOID substr_cpy_off_CAPL(CHAR dest[], LONG destOffset, CHAR src[], LONG srcStart, LONG len, LONG max);
GCC_EXTERN int settimer_CAPL(int nTimerType, char* pchTimerName, int nTimerValue, int nNanoSec = 0);
GCC_EXTERN UINT GetCaplKeyValue(UCHAR ucKeyVal);

GCC_EXTERN unsigned char* byteAt_CAPL(unsigned char pchData[],int dlc, int nByte);
GCC_EXTERN unsigned short* wordAt_CAPL(unsigned char pchData[],int dlc, int nByte);
GCC_EXTERN unsigned long* dwordAt_CAPL(unsigned char pchData[],int dlc, int nByte);
GCC_EXTERN unsigned long long* qwordAt_CAPL(unsigned char pchData[],int dlc, int nByte);
GCC_EXTERN signed char* charAt_CAPL(unsigned char pchData[],int dlc, int nByte);
GCC_EXTERN signed short* intAt_CAPL(unsigned char pchData[],int dlc, int nByte);
GCC_EXTERN signed long* longAt_CAPL(unsigned char pchData[],int dlc, int nByte);
GCC_EXTERN signed long long* int64At_CAPL(unsigned char pchData[],int dlc, int nByte);

