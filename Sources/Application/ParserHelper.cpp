#include "stdafx.h"
#include "ParserHelper.h"
#include <string>

using namespace std;



CParserHelper::CParserHelper(CHANNEL_CONFIG& ouCluster):
    m_ouCluster(ouCluster)
{
    Initialise();
}


CParserHelper::~CParserHelper(void)
{

}


void CParserHelper::Initialise(void)
{
    m_eCurrentSection = SEC_NONE;
    m_unSignalVal = 0;
    m_nLastFrameId = -1;
    m_nBaudRate = -1;
}

//int CParserHelper::OnSectionStarted(string strSection)
//{
//  /*,
//  ,
//  SEC_DIAG_SIGNAL_DEC,
//  SEC_FRAME_DEC,
//  SEC_FRAME_DEF,
//  SEC_SPORADIC_FRAME_DEC,
//  SEC_EVENT_TRIGGER_FRAME_DEC,
//  SEC_DIAG_FRAME_DEC,
//  SEC_DIAG_FRAME_DEF
//
//
//
//
//"Diagnostic_signals"      return DIAGNOSTIC_SIGNALS;
//"Sporadic_frames"         return SPORADIC_FRAMES;
//"Event_triggered_frames"  return EVENT_TRIGGERED_FRAMES;
//""                    return FRAMES;
//"Diagnostic_frames"           return DIAGNOSTIC_FRAMES;*/
//  if ( strSection == "Nodes")
//  {
//
//  }
//  else if ( strSection == "Signals")
//  {
//
//  }
//  else if ( strSection == "Frames")
//  {
//
//  }
//  else if ( strSection == "Nodes")
//  {
//
//  }
//  else if ( strSection == "Nodes")
//  {
//
//  }
//  else if ( strSection == "Nodes")
//  {
//
//  }
//  else if ( strSection == "Nodes")
//  {
//
//  }
//  else if ( strSection == "Nodes")
//  {
//
//  }
//  else if ( strSection == "Nodes")
//  {
//
//  }
//}
int CParserHelper::OnSectionStarted(eCurrentSection ouCurrentSection)
{
    m_eCurrentSection = ouCurrentSection;
    m_ouSectionStack.push(ouCurrentSection);






#ifdef _DEBUG
    string strName;
    GetStringSectionName(m_eCurrentSection, strName);
    printf("CParserClass::OnSection Start  %s \n", strName.c_str());
#endif // _DEBUG
    return 0;
}
int CParserHelper::OnSectionClosed()
{
#ifdef _DEBUG
    string strName;
    GetStringSectionName(m_eCurrentSection, strName);
    printf("CParserClass::OnSection End  %s \n", strName.c_str());
#endif // _DEBUG

    if ( m_ouSectionStack.size() > 0 )
    {
        m_ouSectionStack.pop();

        if ( m_ouSectionStack.size() <= 0 )
        {
            m_eCurrentSection = SEC_NONE;
        }
        else
        {
            m_eCurrentSection = m_ouSectionStack.top();
        }

    }
    else
    {
        //Something Wrong;
    }

    return 0;
}
int CParserHelper::nOnProtocolVersion(string strProtocolVer)
{
    CString strVal = strProtocolVer.c_str();
    strVal.Replace("\"", "");

    m_strProtocol = "LIN ";
    m_strProtocol += strVal;
    return 0;
}
int CParserHelper::nOnLinBaudRate(string strBaudRate)
{
    double dBaud = atof(strBaudRate.c_str());
    m_nBaudRate = dBaud * 1000;
    return 0;
}


int CParserHelper::nOnSignal(LdfSignal& ouSignal)
{
    m_LdfSignalList[ouSignal.m_strName] = (ouSignal);
    return 0;
}
int CParserHelper::nAddSignaltoFrame(string strName, int nAt)
{
    if ( m_nLastFrameId == 0x3C )
    {
        m_ouDiagMasterFrame.m_SignalMap[nAt] = strName;
    }
    else if ( m_nLastFrameId == 0x3D )
    {
        m_ouDiagSlaveFrame.m_SignalMap[nAt] = strName;
    }
    else
    {
        map<int, LdfFrame>::iterator itrFrameMap = m_LdfFrameMap.find(m_nLastFrameId);
        if ( itrFrameMap != m_LdfFrameMap.end())
        {
            map<string, LdfSignal>::iterator itrSignal = m_LdfSignalList.find(strName);
            if ( itrSignal != m_LdfSignalList.end() )
            {
                itrFrameMap->second.m_SignalMap[nAt] = strName;
            }
        }
    }
    return 0;
}

int CParserHelper::OnSporadicOrCompuType(string strId)
{
    if ( m_eCurrentSection == SEC_SIGNAL_REP_DEC )
    {
        list<string>::iterator itrSignal =  m_strIdList.begin();
        while(itrSignal != m_strIdList.end() )
        {
            map<string, LdfSignal>::iterator itr = m_LdfSignalList.find(*itrSignal);
            if ( itr != m_LdfSignalList.end() )
            {
                itr->second.m_strCompuMethodName = strId;
            }

            itrSignal++;
        }
    }

    return 0;
}

int CParserHelper::OnFrameStarted( string strName, int nID, string strTxNode, string strLength)
{
    if ( nID == 0x3C )
    {
        m_ouDiagMasterFrame.m_strName = strName;
        m_ouDiagMasterFrame.m_nId = nID;
        m_ouDiagMasterFrame.m_strTxNode = strTxNode;
        m_ouDiagMasterFrame.nLength = atoi(strLength.c_str());

    }
    else if ( nID == 0x3d )
    {
        m_ouDiagSlaveFrame.m_strName = strName;
        m_ouDiagSlaveFrame.m_nId = nID;
        m_ouDiagSlaveFrame.m_strTxNode = strTxNode;
        m_ouDiagSlaveFrame.nLength = atoi(strLength.c_str());
    }
    else
    {
        LdfFrame ouLdfFrame;
        ouLdfFrame.m_strName = strName;
        ouLdfFrame.m_nId = nID;
        ouLdfFrame.m_strTxNode = strTxNode;
        ouLdfFrame.nLength = atoi(strLength.c_str());
        m_LdfFrameMap[nID] = ouLdfFrame;
    }
    m_nLastFrameId = nID;
    return 0;
}


int CParserHelper::nOnSignalEncoding( UINT m_unMin, UINT m_unMax, DOUBLE m_fFactor, DOUBLE m_fOffset, string m_strUnit)
{
    if ( m_strLastId != "" )
    {
        LdfCompuMethod ouCompuMethod;
        ouCompuMethod.m_unMin = m_unMin;
        ouCompuMethod.m_unMax = m_unMax;
        ouCompuMethod.m_fFactor = m_fFactor;
        ouCompuMethod.m_fOffset = m_fOffset;
        ouCompuMethod.m_strUnit = m_strUnit;
        m_LdfCompuMethodMap[m_strLastId] = ouCompuMethod;
    }

    return 0;
}



list<FRAME_STRUCT> ouList;

int CParserHelper::nCreateMapList(map<string, string>& strDes, list<string>& strSourceList)
{
    for( list<string>::iterator itrEcu = strSourceList.begin(); itrEcu != strSourceList.end(); itrEcu++ )
    {
        strDes[*itrEcu] = *itrEcu;
    }
    return 0;
}


int CParserHelper::nCreateEcus()
{
    ECU_Struct ouEcu;
    ouEcu.m_strECUName = ouEcu.m_strEcuId = m_strMasterEcu;
    m_ouCluster.m_ouClusterInfo.m_ouEcuList[m_strMasterEcu] = ouEcu;

    for ( list<string>::iterator itrEcu = m_strSlaveEcuList.begin(); itrEcu != m_strSlaveEcuList.end(); itrEcu++ )
    {
        ouEcu.m_strECUName = ouEcu.m_strEcuId = *itrEcu;
        m_ouCluster.m_ouClusterInfo.m_ouEcuList[*itrEcu] = ouEcu;
    }


    return 0;
}
int CParserHelper::nAaddFrameToEcu(string& strTxNode, map<string, string>& ouEcuFrameMap, FRAME_STRUCT& ouFrame)
{
    ECUMAP::iterator itrEcu = m_ouCluster.m_ouClusterInfo.m_ouEcuList.find(strTxNode);
    if ( itrEcu != m_ouCluster.m_ouClusterInfo.m_ouEcuList.end() )
    {
        list<FRAME_STRUCT> ouFrameList;
        ouFrameList.push_back(ouFrame);
        itrEcu->second.m_ouTxFrames[ouFrame.m_nSlotId] = ouFrameList;
        list<ECU_ID> ouTxEcuList;
        ouTxEcuList.push_back(strTxNode);
        m_ouCluster.m_ouClusterInfo.m_mapSlotEcu[ouFrame.m_nSlotId] = ouTxEcuList;
    }

    for( map<string, string>::iterator itrRxEcu = ouEcuFrameMap.begin(); itrRxEcu != ouEcuFrameMap.end(); itrRxEcu++ )
    {
        ECUMAP::iterator itrEcu = m_ouCluster.m_ouClusterInfo.m_ouEcuList.find(itrRxEcu->first);
        if ( itrEcu != m_ouCluster.m_ouClusterInfo.m_ouEcuList.end() && strTxNode != itrRxEcu->first )
        {
            list<FRAME_STRUCT> ouFrameList;
            ouFrameList.push_back(ouFrame);
            itrEcu->second.m_ouRxFrames[ouFrame.m_nSlotId] = ouFrameList;
        }
    }

    return 0;
}

INT CParserHelper::CreateNetwork()
{
    m_ouCluster.m_ouLinParams.m_nBaudRate = m_nBaudRate;
    m_ouCluster.m_ouLinParams.m_strProtocolVersion = m_strProtocol;

    if ( m_nBaudRate < 200 || m_nBaudRate > 30000 )
    {
        return S_FALSE;
    }

    nCreateEcus();
    map<int, LdfFrame>::iterator itrFrame = m_LdfFrameMap.begin();

    map<string, string> ouEcuFrameMap;
    while ( itrFrame != m_LdfFrameMap.end() )
    {
        PDU_STRUCT ouPdu;
        SIGNAL_STRUCT ouSignal;
        FRAME_STRUCT ouFrame;
        ouFrame.m_nBaseCycle = 0;
        ouFrame.m_nReptition = 1;
        ouFrame.m_ouChannel = UNSPECIFIED;
        ouFrame.m_strFrameName = itrFrame->second.m_strName;
        ouFrame.m_strFrameId = itrFrame->second.m_strName;
        ouFrame.m_nSlotId = itrFrame->second.m_nId;
        ouFrame.m_nLength = itrFrame->second.nLength;
        ouEcuFrameMap.clear();
        map<int, string>::iterator itrSignalName = itrFrame->second.m_SignalMap.begin();

        while( itrSignalName != itrFrame->second.m_SignalMap.end())
        {
            map<string, LdfSignal>::iterator itrSignal = m_LdfSignalList.find(itrSignalName->second);
            if ( itrSignal != m_LdfSignalList.end())
            {
                ouSignal.m_strSignalName = itrSignal->second.m_strName;
                ouSignal.m_nLength = itrSignal->second.m_nLength;
                ouSignal.m_ouEndianness = INTEL;
                ouSignal.m_unStartbit = itrSignalName->first;
                ouSignal.m_unDefaultVal = itrSignal->second.m_nDefVal;
                ouSignal.m_bDataType = 'U';

                map<string, LdfCompuMethod>::iterator itCompuMethod = m_LdfCompuMethodMap.find(itrSignal->second.m_strCompuMethodName);
                ouSignal.m_ouCompuMethod.m_eCompuType = IDENTICAL_ENUM;
                // Updating Range
                ouSignal.m_ouCompuMethod.m_uMethod.m_LinearCode.m_sRange.m_dwLowerLimit = 0;
                ouSignal.m_ouCompuMethod.m_uMethod.m_LinearCode.m_sRange.m_dwUpperLimit = (pow((double)2, (double)(ouSignal.m_nLength))) - 1;
                if ( itCompuMethod != m_LdfCompuMethodMap.end() )
                {
                    ouSignal.m_ouCompuMethod.m_eCompuType = LINEAR_ENUM;
                    ouSignal.m_ouCompuMethod.m_uMethod.m_LinearCode.m_dD0 = 1;
                    ouSignal.m_ouCompuMethod.m_uMethod.m_LinearCode.m_dN0 = itCompuMethod->second.m_fOffset;
                    ouSignal.m_ouCompuMethod.m_uMethod.m_LinearCode.m_dN1 = itCompuMethod->second.m_fFactor;
                    ouSignal.m_omUnit  = itCompuMethod->second.m_strUnit;
                    ouSignal.m_ouCompuMethod.m_uMethod.m_LinearCode.m_sRange.m_dwLowerLimit = itCompuMethod->second.m_unMin;
                    ouSignal.m_ouCompuMethod.m_uMethod.m_LinearCode.m_sRange.m_dwUpperLimit = itCompuMethod->second.m_unMax;
                }

                if(ouSignal.m_ouCompuMethod.m_eCompuType == IDENTICAL_ENUM)
                {
                    // Updating Range
                    ouSignal.m_ouCompuMethod.m_uMethod.m_IdenticalCode.m_sRange.m_dwLowerLimit = 0;
                    ouSignal.m_ouCompuMethod.m_uMethod.m_IdenticalCode.m_sRange.m_dwUpperLimit = (pow((double)2, (double)(ouSignal.m_nLength))) - 1;
                }
            }
            nCreateMapList(ouEcuFrameMap, itrSignal->second.m_strTxEcuList);
            ouPdu.m_ouSignalList.push_back(ouSignal);
            itrSignalName++;
        }
        ouFrame.m_ouPduList.push_back(ouPdu);
        nAaddFrameToEcu(itrFrame->second.m_strTxNode, ouEcuFrameMap, ouFrame);
        ouList.push_back(ouFrame);
        itrFrame++;
    }

    //Diag Frames
    if ( m_ouDiagMasterFrame.m_nId != -1 )
    {
        PDU_STRUCT ouPdu;
        SIGNAL_STRUCT ouSignal;
        FRAME_STRUCT ouFrame;
        ouFrame.m_strFrameName = m_ouDiagMasterFrame.m_strName;
        ouFrame.m_strFrameId = m_ouDiagMasterFrame.m_strName;
        ouFrame.m_nSlotId = m_ouDiagMasterFrame.m_nId;
        ouFrame.m_nLength = 8;          //m_ouDiagSlaveFrame.nLength;
        ouFrame.m_nBaseCycle = 0;
        ouFrame.m_nReptition = 1;
        ouFrame.m_ouChannel = UNSPECIFIED;

        ouEcuFrameMap.clear();
        map<int, string>::iterator itrSignalName = m_ouDiagMasterFrame.m_SignalMap.begin();

        while( itrSignalName !=  m_ouDiagMasterFrame.m_SignalMap.end())
        {
            map<string, LdfSignal>::iterator itrSignal = m_LdfSignalList.find(itrSignalName->second);
            if ( itrSignal != m_LdfSignalList.end())
            {
                ouSignal.m_strSignalName = itrSignal->second.m_strName;
                ouSignal.m_nLength = itrSignal->second.m_nLength;
                ouSignal.m_ouEndianness = INTEL;
                ouSignal.m_unStartbit = itrSignalName->first;
                ouSignal.m_unDefaultVal = itrSignal->second.m_nDefVal;

                map<string, LdfCompuMethod>::iterator itCompuMethod = m_LdfCompuMethodMap.find(itrSignal->second.m_strName);
                ouSignal.m_ouCompuMethod.m_eCompuType = IDENTICAL_ENUM;

                if ( itCompuMethod != m_LdfCompuMethodMap.end() )
                {
                    ouSignal.m_ouCompuMethod.m_eCompuType = LINEAR_ENUM;
                    ouSignal.m_ouCompuMethod.m_uMethod.m_LinearCode.m_dD0 = 1;
                    ouSignal.m_ouCompuMethod.m_uMethod.m_LinearCode.m_dN0 = itCompuMethod->second.m_fOffset;
                    ouSignal.m_ouCompuMethod.m_uMethod.m_LinearCode.m_dN1 = itCompuMethod->second.m_fFactor;
                    ouSignal.m_omUnit  = itCompuMethod->second.m_strUnit;

                    ouSignal.m_ouCompuMethod.m_uMethod.m_LinearCode.m_sRange.m_dwLowerLimit = itCompuMethod->second.m_unMin;
                    ouSignal.m_ouCompuMethod.m_uMethod.m_LinearCode.m_sRange.m_dwUpperLimit = itCompuMethod->second.m_unMax;
                }

                if(ouSignal.m_ouCompuMethod.m_eCompuType == IDENTICAL_ENUM)
                {
                    // Updating Range
                    ouSignal.m_ouCompuMethod.m_uMethod.m_IdenticalCode.m_sRange.m_dwLowerLimit = 0;
                    ouSignal.m_ouCompuMethod.m_uMethod.m_IdenticalCode.m_sRange.m_dwUpperLimit = (pow((double)2, (double)(ouSignal.m_nLength))) - 1;
                }

            }
            ouPdu.m_ouSignalList.push_back(ouSignal);
            itrSignalName++;
        }
        ouFrame.m_ouPduList.push_back(ouPdu);

        nAddDiadFrameToEcu(ouFrame);
    }

    if ( m_ouDiagSlaveFrame.m_nId != -1 )
    {
        PDU_STRUCT ouPdu;
        SIGNAL_STRUCT ouSignal;
        FRAME_STRUCT ouFrame;
        ouFrame.m_strFrameName = m_ouDiagSlaveFrame.m_strName;
        ouFrame.m_strFrameId = m_ouDiagSlaveFrame.m_strName;
        ouFrame.m_nSlotId = m_ouDiagSlaveFrame.m_nId;
        ouFrame.m_nLength = 8;              //m_ouDiagSlaveFrame.nLength;
        ouFrame.m_nBaseCycle = 0;
        ouFrame.m_nReptition = 1;
        ouFrame.m_ouChannel = UNSPECIFIED;

        ouEcuFrameMap.clear();
        map<int, string>::iterator itrSignalName = m_ouDiagSlaveFrame.m_SignalMap.begin();

        while( itrSignalName != m_ouDiagSlaveFrame.m_SignalMap.end())
        {
            map<string, LdfSignal>::iterator itrSignal = m_LdfSignalList.find(itrSignalName->second);
            if ( itrSignal != m_LdfSignalList.end())
            {
                ouSignal.m_strSignalName = itrSignal->second.m_strName;
                ouSignal.m_nLength = itrSignal->second.m_nLength;
                ouSignal.m_ouEndianness = INTEL;
                ouSignal.m_unStartbit = itrSignalName->first;
                ouSignal.m_unDefaultVal = itrSignal->second.m_nDefVal;

                map<string, LdfCompuMethod>::iterator itCompuMethod = m_LdfCompuMethodMap.find(itrSignal->second.m_strName);
                ouSignal.m_ouCompuMethod.m_eCompuType = IDENTICAL_ENUM;

                if ( itCompuMethod != m_LdfCompuMethodMap.end() )
                {
                    ouSignal.m_ouCompuMethod.m_eCompuType = LINEAR_ENUM;
                    ouSignal.m_ouCompuMethod.m_uMethod.m_LinearCode.m_dD0 = 1;
                    ouSignal.m_ouCompuMethod.m_uMethod.m_LinearCode.m_dN0 = itCompuMethod->second.m_fOffset;
                    ouSignal.m_ouCompuMethod.m_uMethod.m_LinearCode.m_dN1 = itCompuMethod->second.m_fFactor;
                    ouSignal.m_omUnit  = itCompuMethod->second.m_strUnit;
                    ouSignal.m_ouCompuMethod.m_uMethod.m_LinearCode.m_sRange.m_dwLowerLimit = itCompuMethod->second.m_unMin;
                    ouSignal.m_ouCompuMethod.m_uMethod.m_LinearCode.m_sRange.m_dwUpperLimit = itCompuMethod->second.m_unMax;
                }

                if(ouSignal.m_ouCompuMethod.m_eCompuType == IDENTICAL_ENUM)
                {
                    // Updating Range
                    ouSignal.m_ouCompuMethod.m_uMethod.m_IdenticalCode.m_sRange.m_dwLowerLimit = 0;
                    ouSignal.m_ouCompuMethod.m_uMethod.m_IdenticalCode.m_sRange.m_dwUpperLimit = (pow((double)2, (double)(ouSignal.m_nLength))) - 1;
                }
            }
            ouPdu.m_ouSignalList.push_back(ouSignal);
            itrSignalName++;
        }
        ouFrame.m_ouPduList.push_back(ouPdu);


        nAddDiadFrameToEcu(ouFrame);
    }
    return S_OK;
}
int CParserHelper::nAddDiadFrameToEcu(FRAME_STRUCT& ouFrame)
{
    if ( ouFrame.m_nSlotId == 0x3C )
    {
        ECUMAP::iterator itrEcu = m_ouCluster.m_ouClusterInfo.m_ouEcuList.begin();
        while ( itrEcu != m_ouCluster.m_ouClusterInfo.m_ouEcuList.end() )
        {
            list<FRAME_STRUCT> ouFrameList;
            ouFrameList.push_back(ouFrame);
            if ( itrEcu->second.m_strECUName == m_strMasterEcu)
            {
                list<ECU_ID> ouTxEcuList;
                ouTxEcuList.push_back(itrEcu->second.m_strECUName);
                m_ouCluster.m_ouClusterInfo.m_mapSlotEcu[ouFrame.m_nSlotId] = ouTxEcuList;
                itrEcu->second.m_ouTxFrames[ouFrame.m_nSlotId] = ouFrameList;
            }
            else
            {
                itrEcu->second.m_ouRxFrames[ouFrame.m_nSlotId] = ouFrameList;
            }
            itrEcu++;
        }
    }
    else if ( ouFrame.m_nSlotId == 0x3D )
    {
        ECUMAP::iterator itrEcu = m_ouCluster.m_ouClusterInfo.m_ouEcuList.begin();
        while ( itrEcu != m_ouCluster.m_ouClusterInfo.m_ouEcuList.end() )
        {
            list<FRAME_STRUCT> ouFrameList;
            ouFrameList.push_back(ouFrame);
            if ( itrEcu->second.m_strECUName != m_strMasterEcu)
            {

                m_ouCluster.m_ouClusterInfo.m_mapSlotEcu[ouFrame.m_nSlotId].push_back(itrEcu->second.m_strECUName);
                itrEcu->second.m_ouTxFrames[ouFrame.m_nSlotId] = ouFrameList;
            }
            else
            {
                itrEcu->second.m_ouRxFrames[ouFrame.m_nSlotId] = ouFrameList;
            }
            itrEcu++;
        }
    }

    return 0;
}


void CParserHelper::GetStringSectionName(eCurrentSection ouCurrentSection, string& strSectionName)
{
    switch (ouCurrentSection)
    {
        case SEC_NONE:
            strSectionName = "SEC_NONE";
            break;
        case SEC_NODE_DEC:
            strSectionName = "SEC_NODE_DEC";
            break;
        case SEC_SIGNAL_DEC:
            strSectionName = "SEC_SIGNAL_DEC";
            break;

        case SEC_DIAG_SIGNAL_DEC:
            strSectionName = "SEC_DIAG_SIGNAL_DEC";
            break;

        case SEC_FRAME_DEC:
            strSectionName = "SEC_FRAME_DEC";
            break;

        case SEC_FRAME_DEF:
            strSectionName = "SEC_FRAME_DEF";
            m_ouLdfFrame.Clear();
            break;

        case SEC_SPORADIC_FRAME_DEC:
            strSectionName = "SEC_SPORADIC_FRAME_DEC";
            break;

        case SEC_EVENT_TRIGGER_FRAME_DEC:
            strSectionName = "SEC_EVENT_TRIGGER_FRAME_DEC";
            break;

        case SEC_DIAG_FRAME_DEC:
            strSectionName = "SEC_DIAG_FRAME_DEC";
            break;
        case SEC_DIAG_FRAME_DEF:
            strSectionName = "SEC_DIAG_FRAME_DEF";
            break;
        case SEC_SIGNAL_ENCODING_DEC:
            strSectionName = "SEC_SIGNAL_ENCODING_DEC";
            break;
        case SEC_SIGNAL_REP_DEC:
            strSectionName = "SEC_SIGNAL_REP_DEC";
            break;

        default:
            strSectionName = "Invalid";
            break;
    }
}



