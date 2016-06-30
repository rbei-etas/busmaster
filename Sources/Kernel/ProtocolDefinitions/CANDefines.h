#ifndef CANDEFINES_H
#define CANDEFINES_H

#include "CommonDefines.h"
#include "CCompuMethod.h"
#define FRAMEFORMATE 2147483648
#define DEFAULT_DBC_IDENTIFIER "Vector__XXX"

#pragma once

/*class CANEcuProperties
{
public:
    std::string m_ouEcuname;
};*/


enum eCANMultiplexSignalIndicator
{
    eMultiplexSwitch,
    eMutiplexedSignal,
    eBoth,
    eNA
};
enum eCANMsgType
{
    eCan_Standard,
    eCan_Extended
};

class CANFrameProps
{
public:
    unsigned int m_nMsgId;
    unsigned int m_unMsgSize;
    eCANMsgType m_canMsgType;
};


class CANCompuMethods
{
public:
    CCompuMethod m_CompuMethod;
};


class CANSignalProps
{
public:
    eCANMultiplexSignalIndicator m_eMultiplex; // M = Switch , m = mulplexed signal, mIntM = Multipled signal and switch both
    int m_nMuliplexedValue;
    unsigned int m_unSignalSize; // signal size is always in bit
    eSignalDataType m_eSignalValueType; // can be + for unsigned and - for unsigned
    /*double m_nSignalFactor;
    double m_nSignalOffset;
    double m_nSignalMaximum;
    double m_nSignalMinimum;*/
};

#endif