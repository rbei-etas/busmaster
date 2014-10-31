#pragma once
#include <string>
#include "LDFCluster.h"
#include "IClusterProps.h"


class signalInitialValue
{
public:
    bool m_bScalar;
    unsigned __int64 m_un64Value;
};

class ldfsignalDefinition
{
public:
    std::string m_strName;
    int m_nSize;
    unsigned __int64 m_unInitValue;
    std::string m_strPublisher;
    std::list<std::string> m_strSuscriberList;
    eLinSignalValueType m_ouValueType;
    eSignalType m_ouSignalType;
};

class ldfFrameDefinition
{
public:
    std::string m_strName;
    unsigned int m_unId;
    std::string m_strPublisher;
    unsigned int m_unSize;
    std::map<std::string, int> m_mapSignalPos;

    eFrameType m_oueFrameType;
};

class ldfSporadicFrameDef
{
public:
    std::string m_strName;
    std::list<std::string> m_FrameList;
};

class ldfEventTriggerdFrame
{
public:
    std::string m_strName;
    unsigned int m_unId;
    std::string m_strScheduleTable;
    std::list<std::string> m_strAssociatedFrames;
};

class ldfDiagFrame
{
public:
    std::string m_strName;
    unsigned int m_unId;
    std::map<std::string, int> m_strAssociatedSignals;
};

class ldfNodeAttributes
{
public:
    unsigned int m_unSupplierId;
    unsigned int m_unFunctionId;
    unsigned int m_unVariant;
    std::string m_strRespErrSigName;
    std::list<std::string> m_strFaultSignalList;
    double m_dP2Min;
    double m_dSTMin;
    double m_dN_As_timeout;
    double m_dN_Cr_timeout;
    std::map<std::string, unsigned int> FrameToIdMap;
};
class FrameIdPos
{
public:
    std::string m_strFrameName;
    unsigned int unId;
    unsigned int unIndex;
};

class ldfNodeProperties
{
public:
    std::string m_strName;
    double m_dProtocolVer;
    unsigned int m_unConfigNAD;
    unsigned int m_unInitialNAD;
    unsigned int m_unSupplierId;
    unsigned int m_unFunctionId;
    unsigned int m_unVariant;
    std::string m_strRespErrSigName;
    std::list<std::string> m_strFaultSignalList;
    double m_dP2Min;
    double m_dSTMin;
    double m_dN_As_timeout;
    double m_dN_Cr_timeout;
    std::list<FrameIdPos> FrameToIdMap;         //Name Id ( not frame Id );

    ldfNodeProperties()
    {
        m_dProtocolVer=1.3;
        m_dN_As_timeout=1000;
        m_dN_Cr_timeout=1000;
        m_dP2Min=50;
        m_dSTMin=0;
        m_unConfigNAD=INVALID_DATA;
        m_unInitialNAD=INVALID_DATA;
        m_unFunctionId=INVALID_DATA;
        m_unSupplierId=INVALID_DATA;
        m_unVariant=INVALID_DATA;
        m_strFaultSignalList.clear();
        m_strRespErrSigName.clear();
    }
};
class ldfScheduleTableCommand
{
public:
    std::string m_strCommandName;
    double m_dDelay;
    eDiagType m_eDiagType;
    std::string m_strNodeName;
    std::string m_strFrameName;
    unsigned char m_chDataBytes[MAX_LIN_FRAME_LENGTH];          //Applicable
};

class ldfScheduleTable
{
public:
    std::string m_strTableName;
    std::list<ldfScheduleTableCommand> m_ouCommandList;
};

class ldfLogicalValue
{
public:
    unsigned int m_unVale;
    std::string m_strTextInfo;
};

class ldfNodeComposition
{
public:
    std::string m_strEcuName;
    std::list<std::string> m_strLogicalNodeList;
};



class CLDFParserHelper
{
    LDFCluster* m_pouLDFCluster;
    char** m_pchTokens;
    std::map<std::string, UID_ELEMENT> m_mapSignalUID;
    std::map<std::string, UID_ELEMENT> m_mapFrameUID;
    std::map<std::string, UID_ELEMENT> m_mapEcuUID;
    std::map<std::string, std::list<UID_ELEMENT>> m_ScheduleEventFrameMap;
    std::map<std::string, UID_ELEMENT> m_mapCompuMethodToUID;
    bool m_bLinTagFound;
    bool m_bParseError;
    bool m_bMasterAdded;
    int m_nSlaveCount;
    LIN_Settings m_ouProtocolSettings;
public:
    std::list<ParsingResults> m_ouErrors;
    std::list<ParsingResults> m_ouWarnings;

private:
    bool bAreEqual(double a, double b);
    bool bIsValidProtocol(LIN_Settings& ouSettings);
    void AddError(eErrorSeverity, std::string strDesc, std::string strActionTaken);

public:
    CLDFParserHelper(LDFCluster* pCluster);
    CLDFParserHelper();
    ~CLDFParserHelper(void);
    ERRORCODE LinDescrptionTagFound();
    ERRORCODE AddComment(std::string& strComment);
    ERRORCODE AddProtocolInfo(double dProtocolVer, double dLanguagever, double dSpeed);
    ERRORCODE AddChannelName(std::string& strChannelName);
    ERRORCODE AddMaster(std::string& strName, double dTimeBase, double dJitterBase);
    ERRORCODE AddSlaves(std::list<std::string>& ouSlaveList);
    ERRORCODE AddSignal(ldfsignalDefinition&);
    ERRORCODE AddMessage(ldfFrameDefinition& ouldfFrameDefinition);
    ERRORCODE AddMessage(ldfSporadicFrameDef& ouldfFrameDefinition);
    ERRORCODE AddMessage(ldfEventTriggerdFrame& ouldfFrameDefinition);
    ERRORCODE AddMessage(ldfDiagFrame& ouldfDiagFrame);
    ERRORCODE AddScheduleTable(ldfScheduleTable& ouScheduleTable);
    ERRORCODE AddNodeAttributes(ldfNodeProperties& ouNodeProps);
    ERRORCODE AddNodeDiagAddress(std::string& strNode, unsigned int unDiagId);//unDiagId is ConfigNAD;
    ERRORCODE AddSignalCompuMethod(std::string& strName, LINCompuMethods& ouCompuMethod);
    ERRORCODE MapCompuMethodToSignals(std::string& strCompuMethodName, std::list<std::string>& signalsList);
    ERRORCODE AddNodeConfiguration(std::string& strConfigName, std::list<ldfNodeComposition> ouNodeConfigurationList);
    ERRORCODE AddSignalGroup(std::string&, unsigned int nGroupSize, std::map<std::string, unsigned int>& strSignalOffset);
    ERRORCODE AddDynamicFrames(std::list<unsigned int>& listDynamicFrames);
    ERRORCODE AddParsingResult(ParsingResults& ouParsingResult);
    //Helper Functions
    void Trim(std::string&, char chChar);
    void SetCluster(LDFCluster* pCluster, char**);




};

