#pragma once



#include <map>
#include <string>
#include <list>
#include <vector>
#include "CommonDefines.h"
#include "ProtocolsDefinitions.h"

enum eAction
{
	eModify,
	eNameChange,
	eTxNodeAdded,
	eRxNodeAdded,
	eRxNodeRemoved,
	ePropertyChanged,
	eBeforeDeletion,
	eDeleted
};

enum eErrorSeverity
{
	eError,
	eWarning
};

enum eMultiplexSignalIndicator
{
	eMultiplexSwitch,
	eMutiplexedSignal,
	eBoth,
	eNA
};

enum ePropertyType
{
    eCANClusterProperties,
    eLINClusterProperties,
    eFlexRayClusterProperties,
    eFlexRayChannelList,
    eLdfNodeComposition, /*Not Valid */
    eLdfDyanmicFrameList
};

enum eClusterElementType
{
    eFrameElement,
    eSignalElement,
    eEcuElement,
    eCodingElement,
    eScheduleTableElement,
    ePduElement,
    eSignalGroupElement,
    eLinNodeComposition,
    eAllElement,
    eInvalidElement,
};


enum eDIR
{
    eTx = DIRECTION_TX,
    eRx = DIRECTION_RX,
    eAllDir = DIRECTION_ALL
};

class ParsingResults
{
public:
	eErrorSeverity m_ouErrorType;
	unsigned int m_unErrorCode;

	unsigned int m_unLineNum;
	std::string m_strErrorDesc;
	std::string m_strActionTaken;


};

class SignalInstanse
{
public:
    int m_nStartBit;
    eEndianess m_ouSignalEndianess;
    int m_nUpdateBitPos;
};
class SignalValue
{
public:
	std::string mName;
	std::string mUnit;
	union
	{
		unsigned __int64 mUnValue = 0;
		signed __int64 mValue;
	};
	bool mIsSigned = true;
	double mPhyicalValue = 0;
};

class InterpreteSignals
{
public:
    std::string m_omSigName;
    std::string m_omRawValue;
    std::string m_omEnggValue;
    std::string m_omUnit;
};

class PduInstanse
{
public:
    int m_nStartBit;
    eEndianess m_ooPduEndianess;
    int m_nUpdateBitPos;
};



class EcuProperties
{
public:
	eProtocolType m_eProtocol;


	EcuProperties()
{
		m_eProtocol = eProtocolType::eInvalidProtocol;
	}
};

class FrameProps
{
public:
	FrameProps()

{
		m_eProtocol = eProtocolType::eInvalidProtocol;
    //...
	}
	eProtocolType m_eProtocol;
	unsigned int m_nMsgId;
	unsigned int m_unMsgSize;
};


class SignalGroupProps
{
public:
	SignalGroupProps()
	{
		eType = eProtocolType::eInvalidProtocol;
	}
    eProtocolType eType;
};

class SignalProps
{
public:
	SignalProps()
	{
		eType = eProtocolType::eInvalidProtocol;
	}
    eProtocolType eType;
	unsigned int m_unSignalSize;	// signal size is always in bit
	eSignalType m_ouSignalType;		
	eSignalDataType m_ouDataType;   /*eSigned, eUnsigned*/
	eEndianess m_ouEndianess;
	unsigned __int64 m_nIntialValue;
	eMultiplexSignalIndicator m_eMultiplex; // M = Switch , m = mulplexed signal, mIntM = Multipled signal and switch both
	int m_nMuliplexedValue;
	std::string m_omUnit;
};


class PduProps
{
public:
	PduProps()
	{
		eType = eProtocolType::eInvalidProtocol;
	}
    eProtocolType eType;
	unsigned int m_unByteLength;
};




class CompuMethodProps
{
public:
	CompuMethodProps()
	{
		m_eType = eProtocolType::eInvalidProtocol;
	}
    eProtocolType m_eType;
};



class eNameChangeActionData
{
public:
    std::string m_strOldName;
    std::string m_strNewName;
};







