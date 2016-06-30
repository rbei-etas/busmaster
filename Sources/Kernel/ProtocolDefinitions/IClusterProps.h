#pragma once



#include <map>
#include <string>
#include <list>
#include "CommonDefines.h"
#include "LINDefines.h"
#include "FlexRayDefines.h"
#include "CANDefines.h"
#include "ProtocolsDefinitions.h"


/*enum eClusterType
{
    CAN_CLUSTER,
    J1939_CLUSTER,
    LIN_CLUSTER,
    FlexRay_CLUSTER,
    ALL,
    INVALID_CLUSTER,
};*/

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

enum eEcuType
{
    eCAN,
    eLIN_Master,
    eLIN_Slave,
    eFlexRay,
    eEcuNone
};

enum eDIR
{
    eTx = DIRECTION_TX,
    eRx = DIRECTION_RX,
    eAllDir = DIRECTION_ALL
};

enum eFrameType
{

    eCAN_Frame_start,
    eCAN_Frame,
    eCAN_Frame_end,

    eLIN_Frame_start,
    eLIN_Unconditional,
    eLIN_Sporadic,
    eLIN_EventTriggered,
    eLIN_Diagnostic,
    eLIN_Frame_end,

    eFlexRay_Frame_Start,
    eFlexRay_Normal,
    eFlexRay_Switch,
    eFlexRay_Frame_End,

    eFrame_Invalid
};

class SignalInstanse
{
public:
    int m_nStartBit;
    eEndianess m_ouSignalEndianess;
    int m_nUpdateBitPos;
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
    eEcuType m_eEcuType;
    LINMasterProps m_ouMasterProps;
    LINSlaveProps m_ouSlavePros;
    FlexRayEcuProperties m_ouFlexRayProperties;
};


class SwitchProps
{
public:
    std::string m_omSwitchName;
    unsigned int m_unStartBit;
    eEndianess m_oueEndianess;
    unsigned int m_unBitLength;
    UID_ELEMENT m_uidSignal;
};

class FrameProps
{
public:
    eFrameType m_eFrameType;
    LINUnConditionFrameProps m_ouLINUnConditionFrameProps;
    LINEventTrigFrameProps m_ouLINEventTrigFrameProps;
    LINSporadicFrameProps m_ouLINSporadicFrameProps;
    LINDiagnosticFrameProps m_ouLINDiagnosticFrameProps;
    FlexRayFrameProps m_ouFlexRayFrameProps;
    SwitchProps m_ouFlexRaySwitchProps;
    CANFrameProps m_ouCANFrameProp;
};

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
    //...
};

class FlexRayFrameId
{
public:
    unsigned int m_unSlotId;
    unsigned int m_unBaseCycle;
    eFlexRayChannel m_oueChannel;
};


class SignalGroupProps
{
public:
    eProtocolType eType;
    LinSignalGroupProps m_ouLinSignalGroupProps;
};

class SignalProps
{
public:
    eProtocolType eType;
    LINSignalProps m_ouLINSignalProps;
    FlexRaySignalProps m_ouFlexRaySignalProps;
    CANSignalProps m_ouCANSignalProps;
};


class PduProps
{
public:
    eProtocolType eType;
    FlexRayPduProps m_ouFlexRayPduProps;
    SwitchProps m_ouFlexRaySwitchProps;
};




class CompuMethodProps
{
public:
    eProtocolType m_eType;
    LINCompuMethods m_ouLinCompuMethods;
    FlxRayCompuMethods m_ouFlexRayCompuMethods;
    CANCompuMethods m_ouCanComputeMethod;
};



class eNameChangeActionData
{
public:
    std::string m_strOldName;
    std::string m_strNewName;
};



enum eErrorSeverity
{
    eError,
    eWarning
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


