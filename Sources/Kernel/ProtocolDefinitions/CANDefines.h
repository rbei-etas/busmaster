#ifndef CANDEFINES_H
#define CANDEFINES_H
#include "IClusterProps.h"
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



enum eCANMsgType
{
    eCan_Standard,
    eCan_Extended
};

class CANFrameProps : public FrameProps
{
public:
	CANFrameProps()
	{
		m_eProtocol = eProtocolType::eCANProtocol;
	}
    eCANMsgType m_canMsgType;
};


class CANCompuMethods : public CompuMethodProps
{
public:
	CANCompuMethods()
	{
		this->m_eType = eCANProtocol;
	}
    CCompuMethod m_CompuMethod;
};


class CANSignalProps : public SignalProps
{
public:
	CANSignalProps()
	{
		this->eType  = eCANProtocol;
	}
    /*double m_nSignalFactor;
    double m_nSignalOffset;
    double m_nSignalMaximum;
    double m_nSignalMinimum;*/
};

class CANEcuProperties : public EcuProperties
{
public:
	eProtocolType m_eProtocol;


	CANEcuProperties()
	{
		m_eProtocol = eProtocolType::eCANProtocol;
	}
};

#endif