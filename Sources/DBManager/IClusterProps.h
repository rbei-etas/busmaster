#pragma once

#ifdef DBMANAGER_EXPORTS
#define EXPORTS __declspec(dllexport)
#else
#define EXPORTS __declspec(dllimport)
#endif

#include <map>
#include <string>
#include <list>

typedef unsigned __int64        UID_ELEMENT;
typedef int                     ERRORCODE;
typedef int                     STARTBIT;

#define EC_SUCCESS                  0
#define EC_WARNING_SUCCESS          0x80000000
#define EC_FAILURE                  0x80000001
#define EC_FAIL_ELEMENT_NOT_FOUND   0x80000002
#define EC_NA                       0x80000003

#define INVALID_UID_ELEMENT         0
#define MAX_LIN_FRAME_LENGTH        8
#define DIR_TX                      0
#define DIR_RX                      1
#define DIR_ALL                     2
#define MAX_SUPPORTED_STRING        256

#define INVALID_DATA            -1
#define EMPTY_STRING            ""
class IFrame;
enum eClusterType
{
    CAN,
    LIN,
    FlexRay,
    NONE,
};

enum ePropertyType
{
    eCANClusterProperties,
    eLINClusterProperties,
    eFlexRayClusterProperties,
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
    eNone
};

enum eDIR
{
    eTx = DIR_TX,
    eRx = DIR_RX,
    eAll = DIR_ALL
};

enum eFrameType
{
    eCAN_Frame_start,
    eCAN_Std,
    eCAN_Extended,
    eCAN_Frame_end,

    eLIN_Frame_start,
    eLIN_Unconditional,
    eLIN_Sporadic,
    eLIN_EventTriggered,
    eLIN_Diagnostic,
    eLIN_Frame_end,
    eFrame_Invalid
};

class EXPORTS SignalInstanse
{
public:
    int m_nStartBit;
    SignalInstanse();
};

enum eDiagType
{
    eLIN_NORMAL_FRAME_ID,
    eLIN_MASTER_FRAME_ID,
    eLIN_SLAVE_FRAME_ID,
    eLIN_SID_ASSIGN_FRAME_ID ,
    eLIN_SID_UNASSIGN_FRAME_ID,
    eLIN_SID_ASSIGN_NAD_ID ,
    eLIN_SID_COND_CHANGE_NAD,
    eLIN_SID_DATA_DUMP,
    eLIN_SID_SAVE_CONFIG ,
    eLIN_SID_ASSIGN_FRAME_RANGE,
    eLIN_SID_FREEFORMAT,
    eLIN_SID_READ_BY_IDENTIFIER,
    eLIN_SID_ALL,
    eLIN_SID_NONE,
};


class EXPORTS CSheduleTableItem
{
public:
    UID_ELEMENT m_nFrameId;
    double m_dDelay;
    eDiagType m_eDiagType;
    UID_ELEMENT m_nNode;
    unsigned char m_chDataBytes[MAX_LIN_FRAME_LENGTH];          //Applicable

    CSheduleTableItem();
    void vInitialiseData();
};



class LINMasterProps
{
public:
    double m_fJitter;
    double m_fTimeBase;
    LINMasterProps();
    void vInitialiseData();
};

class EXPORTS ConfigFrameDetails
{
public:
    UID_ELEMENT m_uidFrame;
    unsigned int m_unConfigMsgId; // [0-0xFFFE] valid only for slave protocol version 2.0

    ConfigFrameDetails()
    {
        m_uidFrame = INVALID_UID_ELEMENT;
        m_unConfigMsgId = 0;
    }

    bool operator==(ConfigFrameDetails ouConfigFrame)
    {
        return (m_uidFrame == ouConfigFrame.m_uidFrame);
    }
    bool operator < (ConfigFrameDetails& ouConfigFrame)
    {
        return (m_uidFrame < ouConfigFrame.m_uidFrame);
    }
    bool operator > (ConfigFrameDetails& ouConfigFrame)
    {
        return (m_uidFrame > ouConfigFrame.m_uidFrame);
    }
};

class LINSlaveProps
{
public:
    double m_fProtocolVersion;
    double m_dNASTimeout;
    double m_dNCRTimeout;
    double m_dP2Min;
    double m_dSTMin;
    int m_nConfiguredNAD;;
    int m_nInitialNAD;
    int m_nFunctionId;              //Product id = Supplier id, Function id, variant
    int m_nSupplierId;
    int m_nVariant;
    std::list<UID_ELEMENT> m_nFaultStateSignals;
    UID_ELEMENT m_nRespErrSignal;

    std::list<ConfigFrameDetails> m_nConfiurableFrameIdList;


    LINSlaveProps();
    void vInitialiseData();
};


class EXPORTS LINUnConditionFrameProps
{
public:
    unsigned int m_unId;
    unsigned int m_nLength;
    void vInitialise()
    {

    }
};

class EXPORTS LINEventTrigFrameProps
{
public:
    unsigned int m_unId;
    unsigned int m_nLength;
    std::map<UID_ELEMENT, UID_ELEMENT> m_pouUnconditionalFrame;
    UID_ELEMENT m_nCollisionResolveTable;
    //ERRORCODE GetUnconditionalFrameList(std::list<IFrame*>&);
};

class EXPORTS LINSporadicFrameProps
{
public:
    std::map<UID_ELEMENT, UID_ELEMENT> m_pouUnconditionalFrame;
    //ERRORCODE GetUnconditionalFrameList(std::list<IFrame*>&);
};

class LINDiagnosticFrameProps
{
public:
    unsigned int m_unId;
    unsigned int m_nLength;
    eDiagType m_eDiagType;
};

class EXPORTS EcuProperties
{
public:
    eEcuType m_eEcuType;
    LINMasterProps m_ouMasterProps;
    LINSlaveProps m_ouSlavePros;
    EcuProperties();
};

class EXPORTS FrameProps
{
public:
    eFrameType m_eFrameType;
    LINUnConditionFrameProps m_ouLINUnConditionFrameProps;
    LINEventTrigFrameProps m_ouLINEventTrigFrameProps;
    LINSporadicFrameProps m_ouLINSporadicFrameProps;
    LINDiagnosticFrameProps m_ouLINDiagnosticFrameProps;
    FrameProps();
};

enum eProtocolType
{
    eCANProtocol,
    eLINProtocol,
    eInvalidProtocol
};

enum eSignalDataType
{
    eSigned,
    eUnsigned
};
enum eSignalEndianess
{
    eMotorola,
    eIntel
};
enum eLinSignalValueType
{
    eScalar,
    eByteArray
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

enum eSignalType
{
    eSignalNormal,
    eSignalDiag
};

class EXPORTS LINSignalProps
{
public:
    int m_nLength;
    eSignalType m_ouSignalType;
    eSignalDataType m_ouDataType;
    eSignalEndianess m_ouEndianess;
    eLinSignalValueType m_ouValueType;
    unsigned __int64 m_nIntialValue;

    LINSignalProps();
};

class LinSignalGroupProps
{
public:
    unsigned int m_nGroupSize;
};

class SignalGroupProps
{
public:
    eProtocolType eType;
    LinSignalGroupProps m_ouLinSignalGroupProps;
};

class EXPORTS SignalProps
{
public:
    eProtocolType eType;
    LINSignalProps m_ouLINSignalProps;

    SignalProps();
};

class NodeComposition
{
public:
    UID_ELEMENT m_nUid;
    std::list<std::string> m_strLogicalNodes;
};

class NodeCompositionConfiguration
{
public:

    std::string m_strConfigName;
    std::list<NodeComposition> m_ouldfNodeConfigList;
};
class NodeCompositionConfigurationList
{
public:
    std::list<NodeCompositionConfiguration> m_ouNodeConfiguration;
};

class EXPORTS PhysicalValue
{
public:
    double m_dOffset;
    double m_dFactor;
    unsigned int m_unMin;
    unsigned int m_unMax;
    std::string m_strUnit;
    std::string m_strTextInfo;
    PhysicalValue();
};

class LINCompuMethods
{
public:
    std::map<unsigned int, std::string> m_ouLogicalValueList;
    std::list<PhysicalValue> m_ouPhysicalValueList;
    bool m_bBCD;
    bool m_bASCII;
    LINCompuMethods();
    LINCompuMethods(const LINCompuMethods&);
};

class EXPORTS CompuMethodProps
{
public:
    eProtocolType m_eType;
    LINCompuMethods m_ouLinCompuMethods;
    CompuMethodProps();
};

class EXPORTS LIN_Settings
{
public:
    double m_dProtocolVers;
    double m_dLanguageVers;
    double m_dBaudRate;
    std::string m_strChannelName;
    std::string m_strHeaderComment;     //Valid Only for Editor
    void clear();
    LIN_Settings();
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

//LIN_Settings m_ouLinSettings;
/*class Protocolsettings
{
public:
    eProtocolType m_oueProtocolType;

    Protocolsettings();
};*/