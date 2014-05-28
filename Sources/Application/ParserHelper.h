#pragma once


#include <stack>
#include <list>

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
    UINT m_unMin;
    UINT m_unMax;
    DOUBLE m_fFactor;
    DOUBLE m_fOffset;
    std::string m_strUnit;
};

class CParserHelper
{
    std::stack<eCurrentSection> m_ouSectionStack;
    eCurrentSection m_eCurrentSection;
    CHANNEL_CONFIG& m_ouCluster;



public:
    int m_nBaudRate;
    std::string m_strProtocol;
    std::list<std::string> m_strIdList;
    std::string m_strLastId;
    int m_nLastFrameId;
    std::list<std::string> m_strSlaveEcuList;
    std::string m_strMasterEcu;
    std::map<std::string, LdfSignal> m_LdfSignalList;
    std::map<std::string, LdfCompuMethod> m_LdfCompuMethodMap;
    std::map<int, LdfFrame> m_LdfFrameMap;

    LdfFrame m_ouDiagMasterFrame;
    LdfFrame m_ouDiagSlaveFrame;

    LdfFrame m_ouLdfFrame;

    unsigned _int64 m_unSignalVal;
    CParserHelper(CHANNEL_CONFIG&);
    ~CParserHelper(void);
    void Initialise(void);
    int OnSectionStarted(eCurrentSection ouCurrentSection);
    //int OnSectionStarted(string strSection);
    int OnSectionClosed();


    int nOnProtocolVersion(std::string strProtocolVer);
    int nOnLinBaudRate(std::string strBaudRate);

    void GetStringSectionName(eCurrentSection ouCurrebtSection, std::string & strSectionName);
    int nOnSignal(LdfSignal& ouSignal);
    int nAddSignaltoFrame(std::string strName, int nAt);
    int nOnSignalEncoding(UINT m_unMin, UINT m_unMax, DOUBLE m_fFactor, DOUBLE m_fOffset, std::string m_strUnit);
    int OnFrameStarted(std::string, int, std::string, std::string);
    int OnSporadicOrCompuType(std::string strId);

    INT CreateNetwork();

private:
    int nCreateEcus();
    int nAaddFrameToEcu(std::string & strTxNode, std::map<std::string, std::string>& ouEcuFrameMap, FRAME_STRUCT& ouFrame);
    int nCreateMapList(std::map<std::string, std::string>& strDes, std::list<std::string>& strSourceList);
    int nAddDiadFrameToEcu(FRAME_STRUCT& ouFrame);
};

