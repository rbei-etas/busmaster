#pragma once

#include <list>
#include <stack>
#include <string>

#include "..\DataTypes\Cluster.h"

typedef enum _eCurrentSection
{
    SEC_NONE,
    SEC_NODE_DEC,
    SEC_SIGNAL_DEC,
    SEC_DIAG_SIGNAL_DEC,
    SEC_FRAME_DEC,
    SEC_FRAME_DEF,
    SEC_SPORADIC_FRAME_DEC,
    SEC_EVENT_TRIGGER_FRAME_DEC,
    SEC_DIAG_FRAME_DEC,
    SEC_DIAG_FRAME_DEF,
    SEC_SIGNAL_ENCODING_DEC,
    SEC_SIGNAL_REP_DEC
} eCurrentSection;

struct LdfSignal
{
    std::string m_strName;
    int m_nLength;
    unsigned int m_nDefVal;
    std::list<std::string> m_strTxEcuList;
    std::string m_strCompuMethodName;
    CCompuMethodEx m_ouCompuMethod;
};

struct LdfFrame
{
    int m_nId;
    std::string m_strName;
    std::string m_strTxNode;
    int nLength;
    std::map<int, std::string> m_SignalMap;

    void Clear()
    {
        m_nId = 0;
        m_strTxNode = "";
        nLength = 0;
        m_SignalMap.clear();
        m_strName = "";
    }

};

struct LdfCompuMethod
{
    std::string m_strName;
    unsigned int m_unMin;
    unsigned int m_unMax;
    double m_fFactor;
    double m_fOffset;
    std::string m_strUnit;
};

class CParserHelper
{
public:
    CParserHelper(CHANNEL_CONFIG&);
    ~CParserHelper(void);

    std::string m_strMasterName;
    int m_nBaudRate;
    std::string m_strProtocol;
    std::list<std::string> m_strIdList;
    std::string m_strLastId;
    int m_nLastFrameId;
    std::list<std::string> m_strSlaveEcuList;
    std::map<std::string, Ecu_Lin_Params> m_ouEcuList;
    std::string m_strMasterEcu;
    std::map<std::string, LdfSignal> m_LdfSignalList;
    std::list<std::string> m_LdfSporadicFrameList;
    std::map<std::string, int> m_LdfEventTriggerFrameList;
    std::map<std::string, LdfCompuMethod> m_LdfCompuMethodMap;
    std::map<int, LdfFrame> m_LdfFrameMap;

    LdfFrame m_ouDiagMasterFrame;
    LdfFrame m_ouDiagSlaveFrame;
    LdfFrame m_ouLdfFrame;

    unsigned _int64 m_unSignalVal;

    void Initialise(void);
    int OnSectionStarted(eCurrentSection ouCurrentSection);
    int OnSectionClosed();
    int nOnProtocolVersion(std::string strProtocolVer);
    int nOnLinBaudRate(std::string strBaudRate);
    void GetStringSectionName(eCurrentSection ouCurrebtSection, std::string& strSectionName);
    int nOnSignal(LdfSignal& ouSignal);
    int nAddSignaltoFrame(std::string strName, int nAt);
    int nOnSignalEncoding(unsigned int m_unMin, unsigned int m_unMax, double m_fFactor, double m_fOffset, std::string m_strUnit);
    int OnFrameStarted( std::string, int, std::string, int);
    int OnSporadicOrCompuType(std::string strId);
    int OnSporadicFrame(std::string strName);
    int OnEventTriggeredFrame(std::string strId, int nId);
    int nAddCommand (  CScheduleCommands& ouScheduleCommand );
    int nAddScheduleTable( std::string strName );
    int nUpdateEcuDetails(std::string strEcuName, Ecu_Lin_Params& );
    HRESULT nGetMessageTypeId( std::string strFrameName, int& nId, eCommandType& m_eCommandType );
    INT CreateNetwork();

private:
    std::stack<eCurrentSection> m_ouSectionStack;
    eCurrentSection m_eCurrentSection;
    CHANNEL_CONFIG& m_ouCluster;
    std::list<CSheduleTable> m_listScheduleTable;
    std::list<CScheduleCommands> m_listTempScheduleCommands;

    int nCreateEcus();
    int nAaddFrameToEcu(std::string& strTxNode, std::map<std::string, std::string>& ouEcuFrameMap, FRAME_STRUCT& ouFrame);
    int nCreateMapList(std::map<std::string, std::string>& strDes, std::list<std::string>& strSourceList);
    int nAddDiadFrameToEcu(FRAME_STRUCT& ouFrame);
    eCommandType eGetCommandType(std::string& strName);
    int nFillDiagnosticDetails(CScheduleCommands& ouScheduleCommand);
};
