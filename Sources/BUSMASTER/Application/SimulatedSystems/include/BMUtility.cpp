#define defBITS_IN_BYTE 8

#include <iostream>
#include "BMUtility.h"
#include <math.h>

//#include "Wrapper_FlexRay.h"

GETFLEXRAYPHYSICALVALUE g_pfGetPhysicalValue;
GETFLEXRAYRAWVALUE g_pfGetRawValue;
void vSetGetEngValueProcAddress(GETFLEXRAYPHYSICALVALUE pfGetPhysicalValue)
{
    g_pfGetPhysicalValue = pfGetPhysicalValue;
}


void vSetGetRawValueProcAddress(GETFLEXRAYRAWVALUE pfGetRawValue)
{
    g_pfGetRawValue = pfGetRawValue;
}

double GetEngValue(int bus, unsigned long long unRawValue, int length, int type, unsigned long long unClusterId, const char* pchCodingName )
{
    if ( NULL != g_pfGetPhysicalValue )
    {

        return g_pfGetPhysicalValue( bus, unRawValue, length, type, unClusterId, pchCodingName);
    }
    return 1.0f;
}

unsigned long long GetRawValue(int bus, double dValue, unsigned long long unClusterId, const char* pchCodingName )
{
    if ( NULL != g_pfGetRawValue )
    {
        return g_pfGetRawValue( bus, dValue, unClusterId, pchCodingName);
    }
    return 0.0f;
}
unsigned long long SetRawValue(int signalstart, int length, bool bIntel, bool bSigned, unsigned long long value, unsigned char* pchData, int dataLength)
{
    int byteOffsetBy8 = signalstart / 64;
    int signalStartBitInByte = signalstart % 8;
    int byteStart = signalstart/8 - (byteOffsetBy8*8) + 1;

    /*if ( byteOffsetBy8*8 >= dataLength )
    {
        return 0;
    }*/
    vSetSignalValue(byteStart, signalStartBitInByte, length, bIntel, value, &pchData[byteOffsetBy8*8 ], dataLength-(byteOffsetBy8*8));
}
int SetEngValue(int bus, unsigned long long unClusterId, const char* pchCodingName, int startBit, int length, int intel, int type, int dPhyval, unsigned char* pchData, int dataSize)
{
    unsigned long long raw = GetRawValue(bus, dPhyval, unClusterId, pchCodingName);
    return SetRawValue(startBit, length, intel, type, raw, pchData, dataSize);
}
unsigned long long GetRawValue(int startBit, bool bIntel, int signlalLength, int type, unsigned char* pchData, int length )
{
    unsigned long long nSigValueInBits = 0;
    unsigned int unBitNum = startBit & 7;
    unsigned int byteNumber = startBit / 8 + 1;



    if(pchData != NULL )
    {
        unsigned int  nBytesToRead = 0;
        /* Find out how many data bytes the signal consumes */
        nBytesToRead = nGetNoOfBytesToRead(unBitNum, length);

        /* Whether the format is Intel or Motorola reading bits inside
        a byte is always same */
        unsigned int  CurrBitNum = unBitNum;
        /* If Byte order is motorola then Bytes have to be read in
        reverse order */
        int nByteOrder = (bIntel == true)? 1: -1;
        bool bValid = bValidateSignal(254/**/, byteNumber, unBitNum, length, bIntel);

        if (bValid == true)
        {
            unsigned int  nBitsRead = 0;
            for (register unsigned int  i = 0; i < nBytesToRead; i++)
            {
                /* Please note: If byte number is byteNumber then array index = byteNumber - 1 */
                unsigned char byMsgByteVal = pchData[ (byteNumber -1 ) + (nByteOrder * i) ];

                if (CurrBitNum != 0)
                {
                    byMsgByteVal >>= CurrBitNum;
                }

                /* Find out how bits to read from the current byte */
                unsigned int  nCurrBitsToRead  = std::min (defBITS_IN_BYTE - CurrBitNum, length - nBitsRead);

                /*After the reading first byte reading will be always from
                byte's start index. So reset the CurrBitNum */
                CurrBitNum = 0;
                unsigned char byMask = 0;
                byMask = (unsigned char)(pow((float) 2.0, (int) nCurrBitsToRead) - 1);
                //Update the signal value
                nSigValueInBits |= (unsigned long long )(byMsgByteVal & byMask) << nBitsRead;

                nBitsRead += nCurrBitsToRead;
            }
        }
    }


    return (nSigValueInBits);
}

unsigned nGetNoOfBytesToRead(unsigned int  nBitNum, unsigned int  nSigLen)
{
    unsigned int  nBytesToRead = 1; //Consider first byte

    int nRemainingLength = nSigLen - (defBITS_IN_BYTE - nBitNum);

    if (nRemainingLength > 0)
    {
        // Add te number of bytes totally it consumes.
        nBytesToRead += (int)(nRemainingLength / defBITS_IN_BYTE);

        // Check for extra bits which traverse to the next byte.
        int nTotalBitsConsidered = ((nBytesToRead - 1) * defBITS_IN_BYTE) +
                                   (defBITS_IN_BYTE - nBitNum);
        if (nTotalBitsConsidered < (int)nSigLen)
        {
            nBytesToRead++;
        }
    }

    return nBytesToRead;
}

bool bValidateSignal(unsigned int  nDLC, unsigned int  nByteNum, unsigned int  nBitNum, unsigned int  nLength, bool bIntel)
{
    bool bValid = true;
    unsigned int  nBytesToRead = nGetNoOfBytesToRead(nBitNum, nLength);
    bValid = (bIntel == true)?
             (int)(nByteNum + nBytesToRead - 1) <= nDLC :
             (int)(nByteNum - nBytesToRead) >= 0;
    return bValid;
}

void vSetSignalValue(int byte, int startBitIndexInByte, int length, bool bIntel, unsigned long long u64SignVal, unsigned char* aucData, int dataLenght)
{
    /* Signal valuedata type happens to be of the same size of the entire CAN
    data byte array. Hence there is an opportunity to take advantage of this
    idiosyncratic characteristics. We will shifts the bit array in u64SignVal
    by the required number of bit positions to exactly map it as a data byte
    array and then interchange positions of bytes as per the endianness and
    finally use it as the etching mask on the target. */
    UINT64* pu64Target = (UINT64*) aucData; // We should be able to work on
    BYTE* pbData = (BYTE*)&u64SignVal; // these variables as an arrayof
    // bytes andvice versa.
    // First findout offset between the last significant bits of the signal
    // and theframe. Finding out the lsb will directly answer to thisquery.
    UINT64 unMaxVal = pow((double)2, (double)length);
    unMaxVal -= 1;
    u64SignVal = u64SignVal&unMaxVal;
    if(bIntel ==true )// If Intel format
    {
        int Offset = (byte - 1) * 8 + startBitIndexInByte;
        u64SignVal<<= Offset;// Exactly map the data bits on the databytes.
    }
    else// If Motorola format
    {
        int Offset = byte *8 - startBitIndexInByte;
        u64SignVal<<= (64 - Offset);
        BYTE byTmp = 0x0;
        byTmp = pbData[7];
        pbData[7] = pbData[0];
        pbData[0] = byTmp;
        byTmp = pbData[6];
        pbData[6] = pbData[1];
        pbData[1] = byTmp;
        byTmp = pbData[5];
        pbData[5] = pbData[2];
        pbData[2] = byTmp;
        byTmp = pbData[4];
        pbData[4] = pbData[3];
        pbData[3] = byTmp;
    }
    UINT64 unTmp = un64GetBitMask(byte, startBitIndexInByte, length, bIntel, aucData, u64SignVal);
    *pu64Target&= ~unTmp;// All bits related to the current signal willbe
    // be made0.
    *pu64Target |= u64SignVal;
}
// Ends static functions of sSIGNALS

// Starts static functions of sSIGNALS

UINT64 un64GetBitMask(int byte, int startBitIndexInByte, int length, bool bIntel, unsigned char* aucData, unsigned long long u64SignVal)
{
    UINT64 Result = 0x1;

    // First make the required number of bits (m_unSignalLength) up.
    Result <<= length;
    --Result; // These bits are now up.

    // Then shift them to the appropriate place.
    short Shift = (true == bIntel) ?
                  ((short)byte - 1) * 8 + startBitIndexInByte
                  : 64 - ((short)byte *8 - startBitIndexInByte);
    Result <<= Shift;

    if (false == bIntel)
    {
        BYTE* pbStr = (BYTE*) &Result;

        BYTE bTmp = 0x0;
        bTmp = pbStr[0];
        pbStr[0] = pbStr[7] ;
        pbStr[7] = bTmp;
        bTmp = pbStr[1];
        pbStr[1] = pbStr[6] ;
        pbStr[6] = bTmp;
        bTmp = pbStr[2];
        pbStr[2] = pbStr[5] ;
        pbStr[5] = bTmp;
        bTmp = pbStr[3];
        pbStr[3] = pbStr[4] ;
        pbStr[4] = bTmp;
    }
    return Result;
}


bool setByteAt(unsigned char* data, int length, int index, unsigned char val)
{
    if ( index >= length )
    {
        return false;
    }
    data[index] = val;
    return true;
}
bool setWordAt(unsigned char* data, int length, int index, unsigned short val)
{
    if ( index >= length/2 )
    {
        return false;
    }
    index = index*2;
    data[index] = LOBYTE(val);
    data[index+1] = HIBYTE(val);
    return true;
}
bool setLongAt(unsigned char* data, int length, int index, unsigned long val)
{
    if ( index >= length/4 )
    {
        return false;
    }
    index = index*4;
    data[index] = LOBYTE(LOWORD(val));
    data[index+1] = HIBYTE(LOWORD(val));
    data[index+2] = LOBYTE(HIWORD(val));
    data[index+3] = HIBYTE(HIWORD(val));
    return true;
}


unsigned char  getByteAt(unsigned char* data, int length, int index)
{
    if ( index >= length )
    {
        return 0;
    }
    return (((data[index])));
}

unsigned short getWordAt(unsigned char* data, int length, int index)
{
    if ( index >= length-1 )
    {
        return false;
    }
    unsigned short shRetVal = MAKEWORD(data[index], data[index+1]);
    return  shRetVal;
}

unsigned long getLongAt(unsigned char* data, int length, int index)
{
    if ( index >= length-3 )
    {
        return 0;
    }
    unsigned long ulRetVal = MAKELONG(MAKEWORD(data[index], data[index+1]), MAKEWORD(data[index+2], data[index+3]));
    return  ulRetVal;
}



