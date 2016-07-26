#pragma once

#include <list>
#include <map>
#include "CommonDefines.h"
//#include "..\FlexRayDefines.h"


enum eLinSignalValueType
{
    eScalar,
    eByteArray
};


class CLINSheduleTableItem
{
public:
    UID_ELEMENT m_nFrameId;
    double m_dDelay;
    eDiagType m_eDiagType;
    UID_ELEMENT m_nNode;
    bool m_bEnabled;
    unsigned char m_chDataBytes[MAX_LIN_FRAME_LENGTH];
};



class LINMasterProps
{
public:
    double m_fJitter;
    double m_fTimeBase;
};

class ConfigFrameDetails
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
    bool operator >(ConfigFrameDetails& ouConfigFrame)
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

};


class LINUnConditionFrameProps
{
public:
    unsigned int m_unId;
    unsigned int m_nLength;

};

class LINEventTrigFrameProps
{
public:
    unsigned int m_unId;
    unsigned int m_nLength;
    std::map<UID_ELEMENT, UID_ELEMENT> m_pouUnconditionalFrame;
    UID_ELEMENT m_nCollisionResolveTable;
};

class LINSporadicFrameProps
{
public:
    std::map<UID_ELEMENT, UID_ELEMENT> m_pouUnconditionalFrame;
};

class LINDiagnosticFrameProps
{
public:
    unsigned int m_unId;
    unsigned int m_nLength;
    eDiagType m_eDiagType;
};


class LINSignalProps
{
public:
    int m_nLength;
    eSignalType m_ouSignalType;
    eSignalDataType m_ouDataType;
    eEndianess m_ouEndianess;
    eLinSignalValueType m_ouValueType;
    uint64_t m_nIntialValue;
};

class LinSignalGroupProps
{
public:
    unsigned int m_nGroupSize;
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


class PhysicalValue
{
public:
    double m_dOffset;
    double m_dFactor;
    unsigned int m_unMin;
    unsigned int m_unMax;
    std::string m_strUnit;
    std::string m_strTextInfo;
};

class LINCompuMethods
{
public:
    std::map<unsigned int, std::string> m_ouLogicalValueList;
    std::list<PhysicalValue> m_ouPhysicalValueList;
    bool m_bBCD;
    bool m_bASCII;
    //LINCompuMethods();
    //LINCompuMethods(const LINCompuMethods&);
};



class LIN_Settings
{
public:
    double m_dProtocolVers;
    double m_dLanguageVers;
    double m_dBaudRate;
    std::string m_strChannelName;
    std::string m_strHeaderComment;     //Valid Only for Editor
};


class ScheduleTableProps
{
public:
    //Name Only for Now
    std::string m_strTableName;
    std::list<CLINSheduleTableItem> m_ouCLINSheduleTableItem;
};


class CompositeNode
{
public:
    UID_ELEMENT m_uidNode;
    std::list<std::string> m_strLogicalNodes;
};
