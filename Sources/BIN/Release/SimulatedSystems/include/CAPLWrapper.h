#pragma once 
#include <windows.h>
#include "Wrapper_CAN.h"

#define snprintf_CAPL _snprintf
/* _max Function */
#define _max_CAPL(a,b) ((a)>(b)?(a):(b))

/* _min Function */
#define _min_CAPL(a,b) ((a)<(b)?(a):(b))

/* _pow Function */
#define _pow_CAPL	  pow

/* abs Functions */
#define abs_CAPL(a) (((a) < 0) ? (-1*(a)) : (a))

//Key Handlers

#define MSEC_TIMER 		0
#define	SEC_TIMER		1	

#define	F1				0x5400 
#define	F2              0x5500 
#define	F3              0x5600 
#define	F4              0x5700 
#define	F5              0x5800 
#define	F6              0x5900 
#define	F7              -1 
#define	F8              -1 
#define	F9              -1 
#define	F10             0x5D00
#define	F11             0x8700
#define	F12             0x8800
#define	PageUp			0x4900
#define	PageDown		0x5100
#define HOME			0x5109
/*
class ChannelSpecificMsg: public BaseMsg
{
public:
	unsigned int m_unMsgID;     // 11/29 Bit-
    unsigned char m_ucEXTENDED; // true, for (29 Bit) Frame
    unsigned char m_ucRTR;      // true, for remote request
    unsigned char m_ucDLC;  // Data len (0..8)
	unsigned char m_ucChannel;
	unsigned long m_ulTimeStamp;
    union
	{
		unsigned char      m_aucData[8];
		unsigned short int m_auwData[4];
		unsigned long  int m_aulData[2];
	}m_sWhichBit;
	ChannelSpecificMsg& operator = (BaseMsg &RefObj)
	{
		STCAN_MSG* pMsg = (STCAN_MSG*)(&RefObj);
		m_unMsgID = pMsg->m_unMsgID;
		m_ucEXTENDED = pMsg->m_ucEXTENDED;
		m_ucRTR = pMsg->m_ucRTR;
		m_ucDLC = pMsg->m_ucDLC;
		m_ucChannel = pMsg->m_ucChannel;
		m_sWhichBit.m_aulData[0] = pMsg->m_sWhichBit.m_aulData[0];
		m_sWhichBit.m_aulData[1] = pMsg->m_sWhichBit.m_aulData[1];
		return *this;
	}
};*/

INT _round_CAPL(DOUBLE number);
INT64 _round64_CAPL(DOUBLE number);
UINT random_CAPL(UINT dwLimit);
USHORT swapword_CAPL(USHORT shVal);
SHORT swapint_CAPL(SHORT shVal);
UINT swapdword_CAPL(UINT lVal);
INT swaplong_CAPL(INT lVal);
LONG fileclose_CAPL (UINT dwFileHandle);
UINT openfileread_CAPL (CHAR pchFileName[], UINT dwMode);
UINT openfilewrite_CAPL (CHAR pchFileName[], UINT dwMode);
LONG filegetbinaryblock_CAPL (byte byBuffer[], LONG lBufferSize, UINT dwFileHandle);
LONG filegetstring_CAPL (CHAR pchBuffer[], LONG lBuffSize, UINT dwFileHandle);
LONG fileputstring_CAPL (CHAR pchBuffer[], LONG lBuffSize, UINT dwFileHandle);
LONG filerewind_CAPL (UINT dwFileHandle);
LONG filewritebinaryblock_CAPL (byte byBuffer[], LONG lBufferSize, UINT dwFileHandle);
LONG getprofileint_CAPL (CHAR pchSection[], CHAR pchEntry[], LONG lDefVal, CHAR pchFileName[]);
LONG getprofilestring_CAPL (CHAR pchSection[], CHAR pchEntry[], CHAR pchDefVal[], 
						CHAR pchReturnBuff[],	LONG lBuffSize, CHAR pchFileName[] );
LONG writeprofileint_CAPL (CHAR pchSection[], CHAR pchEntry[], LONG lVal, 
															CHAR pchFileName[]);
LONG writeprofilestring_CAPL(CHAR pchSection[], CHAR pchEntry[], CHAR pchVal[], 
							CHAR pchFileName[] );
LONG writeprofilefloat_CAPL(CHAR pchSection[], CHAR pchEntry[], float fVal, 
															CHAR pchFileName[]);
FLOAT getprofilefloat_CAPL( CHAR pchSection[], CHAR pchEntry[], LONG lDefVal, 
															CHAR pchFileName[] );
LONG getprofilearray_CAPL(CHAR pchSection[], CHAR pchEntry[],CHAR pchBuff[], 
								LONG lBuffSize, CHAR pchFileName[]);
VOID setwritepath_CAPL (CHAR pchRelOrAbsPath[]);
VOID setfilepath_CAPL (CHAR pchDirPath[], UINT dwMode);
DOUBLE atodbl_CAPL(CHAR s[]);
VOID strncat_CAPL(CHAR pchDest[], CHAR pscSrc[], LONG nMaxLen);								
INT strncmp_CAPL ( CHAR s1[], CHAR s2[], ... );
LONG strncmp_off_CAPL(CHAR s1[], LONG s1offset, CHAR s2[], LONG s2offset, LONG len);
VOID strncpy_off_CAPL(CHAR dest[], LONG destOffset, CHAR src[], LONG max);
LONG strstr_CAPL(CHAR s1[], CHAR s2[]);
LONG strstr_off_CAPL(CHAR s1[], LONG offset, CHAR s2[]);
VOID substr_cpy_CAPL(CHAR dest[], CHAR src[], LONG srcStart, LONG len, LONG max);
VOID substr_cpy_off_CAPL(CHAR dest[], LONG destOffset, CHAR src[], LONG srcStart, LONG len, LONG max);
UCHAR* byte_CAPL(STCAN_MSG* sMsg, INT nByte);
USHORT* word_CAPL(STCAN_MSG* sMsg, INT nByte);
ULONG* long_CAPL(STCAN_MSG* sMsg, INT nByte);
INT output_CAPL(void* ouMsg);
INT output_CAPL();
int settimer_CAPL(int nTimerType, char* pchTimerName, int nTimerValue, int nNanoSec = 0);
UINT GetCaplKeyValue(UCHAR ucKeyVal);
//void GetMessageName_CAPL( DWORD id, char buffer[], DWORD size);
