#pragma once

#include "Common.h"
#include <windows.h>
typedef double (*GETFLEXRAYPHYSICALVALUE)( int bus, unsigned long long unRawValue, int length, int type, unsigned long long unClusterId, const char* pchCodingName );
typedef unsigned long long (*GETFLEXRAYRAWVALUE)( int bus, double dRawValue, unsigned long long unClusterId, const char* pchCodingName );


GCC_EXTERN void GCC_EXPORT vSetGetEngValueProcAddress(GETFLEXRAYPHYSICALVALUE pfGetPhysicalValue);
GCC_EXTERN void GCC_EXPORT vSetGetRawValueProcAddress(GETFLEXRAYRAWVALUE pfGetRawValue);



double GetEngValue(int bus, unsigned long long unRawValue, int length, int type,  unsigned long long unClusterId, const char* pchCodingName );
unsigned long long GetRawValue(int startBit, bool bIntel, int signlalLength, int type,unsigned char* pchData, int length );

unsigned long long SetRawValue(int signalstart, int length, bool bIntel, bool bSigned, unsigned long long value, unsigned char* pchData, int dataLength);
int SetEngValue(int bus, unsigned long long unClusterId, const char* pchCodingName, int startBit, int length, int intel, int type, int dPhyval, unsigned char* pchData, int dataSize);

bool bValidateSignal(unsigned int  nDLC, unsigned int  nByteNum, unsigned int  nBitNum, unsigned int  nLength, bool bIntel);

unsigned nGetNoOfBytesToRead(unsigned int  nBitNum, unsigned int  nSigLen);
unsigned long long un64GetBitMask(int byte, int startBitIndexInByte, int length, bool bIntel, unsigned char* aucData, UINT64 u64SignVal);
void vSetSignalValue(int byte, int startBitIndexInByte, int length, bool bIntel, unsigned long long u64SignVal, unsigned char* aucData, int dataLenght);


bool setByteAt(unsigned char* data, int length, int index, unsigned char val);
bool setWordAt(unsigned char* data, int length, int index, unsigned short val);
bool setLongAt(unsigned char* data, int length, int index, unsigned long val);

unsigned char getByteAt(unsigned char* data, int length, int index);
unsigned short getWordAt(unsigned char* data, int length, int index);
unsigned long getLongAt(unsigned char* data, int length, int index);