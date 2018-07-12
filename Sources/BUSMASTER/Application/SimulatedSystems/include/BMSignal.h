#pragma once
#include <string>
#include <string.h>
#include <map>
#include <list>
#include "BMUtility.h"

template <typename TSignalType>
class BMSignal
{
    unsigned char* m_pchData;
    std::string m_strCoding;
    int bus;

public:
    const std::string m_Name;
    const unsigned long long m_ClusterID;
    const int type;				//1->signed 0->unsigned
    const int startBit;
    const int length;
    const bool intel;

public:
    void operator = (TSignalType signalValue)
    {
        rawvalue(signalValue);
    }

    operator TSignalType()
    {
        return rawvalue();
    }

    void operator += (TSignalType signalValue)
    {
        rawvalue(rawvalue()+signalValue);
    }
    void operator -= (TSignalType signalValue)
    {
        rawvalue(rawvalue()-signalValue);
    }
    void operator ++ (int)
    {
        rawvalue(rawvalue()+1);
    }
    void operator ++ ()
    {
        rawvalue(rawvalue()+1);
    }

    void operator -- (int)
    {
        rawvalue(rawvalue()-1);
    }
    void operator -- ()
    {
        rawvalue(rawvalue()-1);
    }
    BMSignal(int _bus, unsigned long long ClusterID, /*std::string name,*/ bool bIntel, int nStartBit, int nLength, int Type, unsigned char* pchData, std::string strCoding):
        bus(_bus), m_ClusterID(ClusterID), m_pchData(pchData), /*m_Name(name),*/ type(Type), startBit(nStartBit), length(nLength), intel(bIntel)
    {
        m_strCoding = strCoding;
    }

    TSignalType rawvalue()
    {
        return GetRawValue(startBit, intel, length, type, m_pchData, length );
    }
    void rawvalue(TSignalType rawVal)
    {
        SetRawValue(startBit, length, intel, type, rawVal, m_pchData, length);
    }

    double physicalvalue()
    {
        return GetEngValue(bus, rawvalue(), length, type, m_ClusterID, m_strCoding.c_str() );
    }
    void physicalvalue(double dPhyval)
    {
        SetEngValue(bus, m_ClusterID, m_strCoding.c_str(),startBit, length, intel, type, dPhyval, m_pchData, length);
    }

private:

    unsigned int nGetNoOfBytesToRead(unsigned int  nBitNum, unsigned int  nSigLen);
    bool bValidateSignal(unsigned int  nDLC, unsigned int  nByteNum, unsigned int  nBitNum, unsigned int  nLength, bool bIntel);
};
