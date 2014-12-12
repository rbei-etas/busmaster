#include "stdafx.h"
#include "ParserHelper.h"
#include <string>

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

int CParserHelper::OnSectionStarted(eCurrentSection ouCurrentSection)
{
    m_eCurrentSection = ouCurrentSection;
    m_ouSectionStack.push(ouCurrentSection);

#ifdef _DEBUG
    std::string strName;
    GetStringSectionName(m_eCurrentSection, strName);
    printf("CParserClass::OnSection Start  %s \n", strName.c_str());
#endif // _DEBUG
    return 0;
}
int CParserHelper::OnSectionClosed()
{
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
int CParserHelper::nOnProtocolVersion(std::string strProtocolVer)
{
    CString strVal = strProtocolVer.c_str();
    strVal.Replace("\"", "");

    m_strProtocol = "LIN ";
    m_strProtocol += strVal;
    return 0;
}
int CParserHelper::nOnLinBaudRate(std::string strBaudRate)
{
    double dBaud = atof(strBaudRate.c_str());
    m_nBaudRate = static_cast<int> ( dBaud * 1000.0);
    return 0;
}


int CParserHelper::nOnSignal(LdfSignal& ouSignal)
{
    m_LdfSignalList[ouSignal.m_strName] = (ouSignal);
    return 0;
}
int CParserHelper::nAddSignaltoFrame(std::string strName, int nAt)
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
        std::map<int, LdfFrame>::iterator itrFrameMap = m_LdfFrameMap.find(m_nLastFrameId);
        if ( itrFrameMap != m_LdfFrameMap.end())
        {
            std::map<std::string, LdfSignal>::iterator itrSignal = m_LdfSignalList.find(strName);
            if ( itrSignal != m_LdfSignalList.end() )
            {
                itrFrameMap->second.m_SignalMap[nAt] = strName;
            }
        }
    }
    return 0;
}

int CParserHelper::OnEventTriggeredFrame(std::string strId, int nId)
{
    m_LdfEventTriggerFrameList[strId] = nId;
    return S_OK;
}

int CParserHelper::OnSporadicFrame(std::string strName)
{
    m_LdfSporadicFrameList.push_back(strName);
    return S_OK;
}



int CParserHelper::OnSporadicOrCompuType(std::string strId)
{
    if ( m_eCurrentSection == SEC_SIGNAL_REP_DEC )
    {
        std::list<std::string>::iterator itrSignal =  m_strIdList.begin();
        while(itrSignal != m_strIdList.end() )
        {
            std::map<std::string, LdfSignal>::iterator itr = m_LdfSignalList.find(*itrSignal);
            if ( itr != m_LdfSignalList.end() )
            {
                itr->second.m_strCompuMethodName = strId;
            }

            itrSignal++;
        }
    }
    else if ( m_eCurrentSection == SEC_SPORADIC_FRAME_DEC )
    {
        m_LdfSporadicFrameList.push_back(strId);
    }


    return 0;
}

int CParserHelper::OnFrameStarted( std::string strName, int nID, std::string strTxNode, int nLength)
{
    if ( nID == 0x3C )
    {
        m_ouDiagMasterFrame.m_strName = strName;
        m_ouDiagMasterFrame.m_nId = nID;
        m_ouDiagMasterFrame.m_strTxNode = strTxNode;
        m_ouDiagMasterFrame.nLength = nLength;

    }
    else if ( nID == 0x3d )
    {
        m_ouDiagSlaveFrame.m_strName = strName;
        m_ouDiagSlaveFrame.m_nId = nID;
        m_ouDiagSlaveFrame.m_strTxNode = strTxNode;
        m_ouDiagSlaveFrame.nLength = nLength;
    }
    else
    {
        LdfFrame ouLdfFrame;
        ouLdfFrame.m_strName = strName;
        ouLdfFrame.m_nId = nID;
        ouLdfFrame.m_strTxNode = strTxNode;
        ouLdfFrame.nLength = nLength;
        m_LdfFrameMap[nID] = ouLdfFrame;
    }
    m_nLastFrameId = nID;
    return 0;
}


int CParserHelper::nOnSignalEncoding( unsigned int m_unMin, unsigned int m_unMax, double m_fFactor, double m_fOffset, std::string m_strUnit)
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



int CParserHelper::nCreateMapList(std::map<std::string, std::string>& strDes, std::list<std::string>& strSourceList)
{
    for( std::list<std::string>::iterator itrEcu = strSourceList.begin(); itrEcu != strSourceList.end(); itrEcu++ )
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

    for ( std::list<std::string>::iterator itrEcu = m_strSlaveEcuList.begin(); itrEcu != m_strSlaveEcuList.end(); itrEcu++ )
    {
        ouEcu.m_strECUName = ouEcu.m_strEcuId = *itrEcu;
        m_ouCluster.m_ouClusterInfo.m_ouEcuList[*itrEcu] = ouEcu;
    }


    return 0;
}
int CParserHelper::nAaddFrameToEcu(std::string& strTxNode, std::map<std::string, std::string>& ouEcuFrameMap, FRAME_STRUCT& ouFrame)
{
    ECUMAP::iterator itrEcu = m_ouCluster.m_ouClusterInfo.m_ouEcuList.find(strTxNode);
    if ( itrEcu != m_ouCluster.m_ouClusterInfo.m_ouEcuList.end() )
    {
        std::list<FRAME_STRUCT> ouFrameList;
        ouFrameList.push_back(ouFrame);
        itrEcu->second.m_ouTxFrames[ouFrame.m_nSlotId] = ouFrameList;
        std::list<ECU_ID> ouTxEcuList;
        ouTxEcuList.push_back(strTxNode);
        m_ouCluster.m_ouClusterInfo.m_mapSlotEcu[ouFrame.m_nSlotId] = ouTxEcuList;
    }

    for( std::map<std::string, std::string>::iterator itrRxEcu = ouEcuFrameMap.begin(); itrRxEcu != ouEcuFrameMap.end(); itrRxEcu++ )
    {
        ECUMAP::iterator itrEcu = m_ouCluster.m_ouClusterInfo.m_ouEcuList.find(itrRxEcu->first);
        if ( itrEcu != m_ouCluster.m_ouClusterInfo.m_ouEcuList.end() && strTxNode != itrRxEcu->first )
        {
            std::list<FRAME_STRUCT> ouFrameList;
            ouFrameList.push_back(ouFrame);
            itrEcu->second.m_ouRxFrames[ouFrame.m_nSlotId] = ouFrameList;
        }
    }

    return 0;
}

INT CParserHelper::CreateNetwork()
{
    std::list<FRAME_STRUCT> ouList;
    m_ouCluster.m_ouLinParams.m_nBaudRate = m_nBaudRate;
    m_ouCluster.m_ouLinParams.m_strProtocolVersion = m_strProtocol;

    if ( m_nBaudRate < 200 || m_nBaudRate > 30000 )
    {
        return S_FALSE;
    }

    nCreateEcus();
    std::map<int, LdfFrame>::iterator itrFrame = m_LdfFrameMap.begin();

    std::map<std::string, std::string> ouEcuFrameMap;
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
        std::map<int, std::string>::iterator itrSignalName = itrFrame->second.m_SignalMap.begin();

        while( itrSignalName != itrFrame->second.m_SignalMap.end())
        {
            std::map<std::string, LdfSignal>::iterator itrSignal = m_LdfSignalList.find(itrSignalName->second);
            if ( itrSignal != m_LdfSignalList.end())
            {
                ouSignal.m_strSignalName = itrSignal->second.m_strName;
                ouSignal.m_nLength = itrSignal->second.m_nLength;
                ouSignal.m_ouEndianness = INTEL;
                ouSignal.m_unStartbit = itrSignalName->first;
                ouSignal.m_unDefaultVal = itrSignal->second.m_nDefVal;
                ouSignal.m_bDataType = 'U';

                std::map<std::string, LdfCompuMethod>::iterator itCompuMethod = m_LdfCompuMethodMap.find(itrSignal->second.m_strCompuMethodName);
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
        std::map<int, std::string>::iterator itrSignalName = m_ouDiagMasterFrame.m_SignalMap.begin();

        while( itrSignalName !=  m_ouDiagMasterFrame.m_SignalMap.end())
        {
            std::map<std::string, LdfSignal>::iterator itrSignal = m_LdfSignalList.find(itrSignalName->second);
            if ( itrSignal != m_LdfSignalList.end())
            {
                ouSignal.m_strSignalName = itrSignal->second.m_strName;
                ouSignal.m_nLength = itrSignal->second.m_nLength;
                ouSignal.m_ouEndianness = INTEL;
                ouSignal.m_unStartbit = itrSignalName->first;
                ouSignal.m_unDefaultVal = itrSignal->second.m_nDefVal;

                std::map<std::string, LdfCompuMethod>::iterator itCompuMethod = m_LdfCompuMethodMap.find(itrSignal->second.m_strName);
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
        std::map<int, std::string>::iterator itrSignalName = m_ouDiagSlaveFrame.m_SignalMap.begin();

        while( itrSignalName != m_ouDiagSlaveFrame.m_SignalMap.end())
        {
            std::map<std::string, LdfSignal>::iterator itrSignal = m_LdfSignalList.find(itrSignalName->second);
            if ( itrSignal != m_LdfSignalList.end())
            {
                ouSignal.m_strSignalName = itrSignal->second.m_strName;
                ouSignal.m_nLength = itrSignal->second.m_nLength;
                ouSignal.m_ouEndianness = INTEL;
                ouSignal.m_unStartbit = itrSignalName->first;
                ouSignal.m_unDefaultVal = itrSignal->second.m_nDefVal;

                std::map<std::string, LdfCompuMethod>::iterator itCompuMethod = m_LdfCompuMethodMap.find(itrSignal->second.m_strName);
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
for ( auto itrTable : m_listScheduleTable )
    {
        m_ouCluster.m_ouLinParams.ouLinParams.push_back(itrTable);
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
            std::list<FRAME_STRUCT> ouFrameList;
            ouFrameList.push_back(ouFrame);
            if ( itrEcu->second.m_strECUName == m_strMasterEcu)
            {
                std::list<ECU_ID> ouTxEcuList;
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
            std::list<FRAME_STRUCT> ouFrameList;
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

int CParserHelper::nAddCommand (  CScheduleCommands& ouScheduleCommand )
{

    /*if ( ouScheduleCommand.m_eCommandType == COMMAND_INVALID )
    {
        ouScheduleCommand.m_eCommandType = eGetCommandType(ouScheduleCommand.m_strCommandName);
    }*/
    if ( ouScheduleCommand.m_eCommandType != COMMAND_INVALID )
    {
        nFillDiagnosticDetails(ouScheduleCommand);
        m_listTempScheduleCommands.push_back(ouScheduleCommand);
    }
    return 0;
}


int CParserHelper::nFillDiagnosticDetails(CScheduleCommands& ouScheduleCommand)
{
    std::map<std::string, Ecu_Lin_Params>::iterator itrEcuDetails = m_ouEcuList.find(ouScheduleCommand.m_strNodeName);
    if ( itrEcuDetails == m_ouEcuList.end() )
    {
        return S_FALSE;
    }
    switch ( ouScheduleCommand.m_eCommandType )
    {
        case COMMAND_ASSIGN_FRAME_ID:
        {
            ouScheduleCommand.m_listIDs[LIN_NAD_INDEX] = itrEcuDetails->second.m_nConfiguredNAD;
            ouScheduleCommand.m_listIDs[LIN_PCI_INDEX] = 0x6;
            ouScheduleCommand.m_listIDs[LIN_SID_INDEX] = LIN_SID_ASSIGN_FRAME_ID;
            ouScheduleCommand.m_listIDs[3] = itrEcuDetails->second.m_unProductId[0];
            ouScheduleCommand.m_listIDs[4] = 0;
            ouScheduleCommand.m_listIDs[5] = 0xFF;
            ouScheduleCommand.m_listIDs[6] = 0xFF;
            ouScheduleCommand.m_listIDs[7] = 0;             //TODO::PID
        }
        break;
        case COMMAND_ASSIGN_NAD:
        {
            ouScheduleCommand.m_listIDs[LIN_NAD_INDEX] = itrEcuDetails->second.m_nInitialNAD;
            ouScheduleCommand.m_listIDs[LIN_PCI_INDEX] = 0x6;
            ouScheduleCommand.m_listIDs[LIN_SID_INDEX] = LIN_SID_ASSIGN_NAD_ID;
            ouScheduleCommand.m_listIDs[3] = itrEcuDetails->second.m_unProductId[0];
            ouScheduleCommand.m_listIDs[4] = 0;
            ouScheduleCommand.m_listIDs[5] = itrEcuDetails->second.m_unProductId[1];
            ouScheduleCommand.m_listIDs[6] = 0;
            ouScheduleCommand.m_listIDs[7] = (unsigned char)itrEcuDetails->second.m_nConfiguredNAD;             //TODO::PID
        }
        break;
        case COMMAND_CONDITIONAL_CHANGE_NAD:
        {
            unsigned char ucTempData[8];
            memcpy(ucTempData, ouScheduleCommand.m_listIDs, sizeof(ucTempData));
            ouScheduleCommand.m_listIDs[LIN_NAD_INDEX] = ucTempData[0];
            ouScheduleCommand.m_listIDs[LIN_PCI_INDEX] = 0x6;
            ouScheduleCommand.m_listIDs[LIN_SID_INDEX] = LIN_SID_COND_CHANGE_NAD;
            memcpy( &ouScheduleCommand.m_listIDs[3], &ucTempData[1], 5 );
        }
        break;
        case COMMAND_DATA_DUMP:
        {
            ouScheduleCommand.m_listIDs[LIN_NAD_INDEX] = itrEcuDetails->second.m_nConfiguredNAD;
            ouScheduleCommand.m_listIDs[LIN_PCI_INDEX] = 0x6;
            ouScheduleCommand.m_listIDs[LIN_SID_INDEX] = LIN_SID_DATA_DUMP;
            memset( &ouScheduleCommand.m_listIDs[3], 0XFF, 5 );
        }
        break;
        case COMMAND_SAVE_CONFIGURATION:
        {
            ouScheduleCommand.m_listIDs[LIN_NAD_INDEX] = itrEcuDetails->second.m_nConfiguredNAD;
            ouScheduleCommand.m_listIDs[LIN_PCI_INDEX] = 0x6;
            ouScheduleCommand.m_listIDs[LIN_SID_INDEX] = LIN_SID_SAVE_CONFIG;
            memset( &ouScheduleCommand.m_listIDs[3], 0XFF, 5 );
        }
        break;
        case COMMAND_FREE_FORMAT:
        {
            unsigned char ucTempData[8];
            memcpy(ucTempData, ouScheduleCommand.m_listIDs, sizeof(ucTempData));
            ouScheduleCommand.m_listIDs[LIN_NAD_INDEX] = itrEcuDetails->second.m_nConfiguredNAD;
            ouScheduleCommand.m_listIDs[LIN_PCI_INDEX] = 0x6;
            ouScheduleCommand.m_listIDs[LIN_SID_INDEX] = LIN_SID_SAVE_CONFIG;
            memcpy( &ouScheduleCommand.m_listIDs[3], &ucTempData[0], 6 );
        }
        break;
        default:
            memset( ouScheduleCommand.m_listIDs, 0, 8 );
    }

    return S_OK;
}


HRESULT CParserHelper::nGetMessageTypeId( std::string strFrameName, int& nId, eCommandType& m_eCommandType )
{
    //1. Check in Unconditional & Diag Frames.
    std::map<int, LdfFrame>::iterator itrFrame =  m_LdfFrameMap.begin();
    while ( m_LdfFrameMap.end()  != itrFrame )
    {
        if ( itrFrame->second.m_strName == strFrameName )
        {
            nId = itrFrame->first;
            m_eCommandType = COMMAND_UNCONDITIONAL;
            return S_OK;
        }
        itrFrame++;
    }

    if ( m_ouDiagMasterFrame.m_strName == strFrameName )
    {
        nId = 0x3c;
        m_eCommandType = COMMAND_UNCONDITIONAL;
        return S_OK;
    }

    if ( m_ouDiagSlaveFrame.m_strName == strFrameName )
    {
        nId = 0x3d;
        m_eCommandType = COMMAND_UNCONDITIONAL;
        return S_OK;
    }

    std::list<std::string>::iterator itrName = std::find(m_LdfSporadicFrameList.begin(), m_LdfSporadicFrameList.end(), strFrameName );

    if ( ( itrName != m_LdfSporadicFrameList.end() ) && ( *itrName == strFrameName ) )
    {
        nId = -1;
        m_eCommandType = COMMAND_SPORADIC;
        return S_OK;
    }
    std::map<std::string, int>::iterator itr = m_LdfEventTriggerFrameList.find(strFrameName);
    if ( itr != m_LdfEventTriggerFrameList.end() )
    {
        nId = itr->second;
        m_eCommandType = COMMAND_EVENT;
        return S_OK;
    }
    return S_FALSE;
}


eCommandType CParserHelper::eGetCommandType(std::string& strName)
{
    eCommandType ouCommandType = COMMAND_UNCONDITIONAL;     //By default
    std::list<std::string>::iterator itrName =  find(m_LdfSporadicFrameList.begin(), m_LdfSporadicFrameList.end(), strName );
    if ( itrName != m_LdfSporadicFrameList.end() )
    {
        return COMMAND_SPORADIC;
    }
    std::map<std::string, int>::iterator itr =  m_LdfEventTriggerFrameList.find(strName);
    if ( itr != m_LdfEventTriggerFrameList.end() )
    {
        return COMMAND_EVENT;
    }
    return ouCommandType;
}


int CParserHelper::nAddScheduleTable( std::string strName )
{
    CSheduleTable ouTable;
    ouTable.m_strTableName = strName;
    ouTable.m_listCommands = m_listTempScheduleCommands;
    m_listTempScheduleCommands.clear();
    if ( ouTable.m_listCommands.size() > 0 )
    {
        m_listScheduleTable.push_back(ouTable);
    }
    return 0;
}

int CParserHelper::nUpdateEcuDetails(std::string strEcuName, Ecu_Lin_Params& ouLinParams )
{
    m_ouEcuList[strEcuName] = ouLinParams;
    return 0;
}



void CParserHelper::GetStringSectionName(eCurrentSection ouCurrentSection, std::string& strSectionName)
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
