#pragma once


#include <stack>
#include <list>
using namespace std;

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
    string m_strName;
    int m_nLength;
    unsigned int m_nDefVal;
    list<string> m_strTxEcuList;
    string m_strCompuMethodName;
    CCompuMethodEx m_ouCompuMethod;
};
struct LdfFrame
{
    int m_nId;
    string m_strName;
    string m_strTxNode;
    int nLength;
    map<int, string> m_SignalMap;
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
    string m_strName;
    UINT m_unMin;
    UINT m_unMax;
    DOUBLE m_fFactor;
    DOUBLE m_fOffset;
    string m_strUnit;
};

class CParserHelper
{


    stack<eCurrentSection> m_ouSectionStack;
    eCurrentSection m_eCurrentSection;
    CHANNEL_CONFIG& m_ouCluster;



public:
    int m_nBaudRate;
    string m_strProtocol;
    list<string> m_strIdList;
    string m_strLastId;
    int m_nLastFrameId;
    list<string> m_strSlaveEcuList;
    string m_strMasterEcu;
    map<string, LdfSignal> m_LdfSignalList;
    map<string, LdfCompuMethod> m_LdfCompuMethodMap;
    map<int, LdfFrame> m_LdfFrameMap;

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


    int nOnProtocolVersion(string strProtocolVer);
    int nOnLinBaudRate(string strBaudRate);

    void GetStringSectionName(eCurrentSection ouCurrebtSection, string& strSectionName);
    int nOnSignal(LdfSignal& ouSignal);
    int nAddSignaltoFrame(string strName, int nAt);
    int nOnSignalEncoding(UINT m_unMin, UINT m_unMax, DOUBLE m_fFactor, DOUBLE m_fOffset, string m_strUnit);
    int OnFrameStarted( string, int, string, string);
    int OnSporadicOrCompuType(string strId);

    INT CreateNetwork();
private:
    int nCreateEcus();
    int nAaddFrameToEcu(string& strTxNode, map<string, string>& ouEcuFrameMap, FRAME_STRUCT& ouFrame);
    int nCreateMapList(map<string, string>& strDes, list<string>& strSourceList);
    int nAddDiadFrameToEcu(FRAME_STRUCT& ouFrame);
};

