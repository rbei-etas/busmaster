#include "LINScheduleDataStore.h"
//#include "DIL_Interface_extern.h"
#include "BaseDIL_LIN.h"
#include "HashDefines.h"

CLINScheduleDataStore CLINScheduleDataStore::m_sTxLINDataStoreObj;

CLINScheduleDataStore::CLINScheduleDataStore()
{
    m_pouDIL_LIN_Interface = NULL;
    m_ouClusterConfig = NULL;
    m_bValidWndSize = FALSE;
    DIL_GetInterface( LIN, (void**)&m_pouDIL_LIN_Interface );
}

HRESULT CLINScheduleDataStore::hSetConfigData(xmlDocPtr pDoc)
{
    // Set Schedule window position
    xmlChar* pXpath = (xmlChar*)DEF_SCHD_TABLE_WNDPOS;

    xmlXPathObjectPtr pObjectPath = xmlUtils::pGetNodes(pDoc, pXpath);
    if  (pObjectPath != NULL )
    {
        xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
        if( NULL != pNodeSet )
        {
            xmlNodePtr pNode = pNodeSet->nodeTab[0];       //Take First One only
            WINDOWPLACEMENT WndPlacement;
            xmlUtils::ParseWindowsPlacement(pNode,WndPlacement);
            vSetWindowPlacement(WndPlacement);
            m_bValidWndSize = TRUE;
        }
    }
    else
    {
        m_bValidWndSize = FALSE;
    }

    //  Set Channel Config
    hSetClusterConfig(m_ouClusterConfig);

    pXpath = (xmlChar*)DEF_SCHD_TABLE_CHNL;
    pObjectPath = xmlUtils::pGetNodes(pDoc, pXpath);
    if  (pObjectPath != NULL )
    {
        xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
        if( NULL != pNodeSet )
        {
            for ( int nIndex = 0 ; nIndex < pNodeSet->nodeNr; nIndex++)
            {
                xmlNodePtr pNode = pNodeSet->nodeTab[nIndex];
                vSetScheduleConfig(pNode);
            }
        }
    }

    return S_OK;
}
void CLINScheduleDataStore::vSetBusStatus(ESTATUS_BUS eBusStatus)
{
    m_eBusStatus = eBusStatus;
    int nHandle;
    int nChannel = 0;
    std::list<int>::iterator itrHandle;
    if ( eBusStatus == BUS_PRECONNECT && m_pouDIL_LIN_Interface != NULL )
    {
        for ( int i =0; i < 1; i++ )                //Number of channels
        {
            ChannelSettings ouChannelSettings;
            m_ouClusterConfig->GetChannelSettings(LIN, i, &ouChannelSettings);

            if ( ouChannelSettings.m_ouLINSettings.m_bIsMasterMode == true )
            {
                m_ouTableData[i].m_ouTableHandles.clear();
for (auto itrTable:m_ouTableData[i].m_ouScheduleTables)
                {
                    nChannel = i+1;
                    m_pouDIL_LIN_Interface->DILL_RegisterLinScheduleTable(m_dwClientID, nChannel, itrTable, nHandle);
                    m_ouTableData[i].m_ouTableHandles.push_back(nHandle);
                }
            }
        }
    }
    else if ( eBusStatus == BUS_CONNECTED && m_pouDIL_LIN_Interface != NULL )
    {
        for ( int i =0; i < 1; i++ )            //Number of channels
        {

            ChannelSettings ouChannelSettings;
            m_ouClusterConfig->GetChannelSettings(LIN, i, &ouChannelSettings);
            int  nIndex = 0;
            if ( ouChannelSettings.m_ouLINSettings.m_bIsMasterMode == true )
            {
for (auto& itrTable:m_ouTableData[i].m_ouScheduleTables)
                {
                    if ( itrTable.m_strTableName == m_ouTableData[i].m_strCurrentTable )
                    {
                        itrHandle = m_ouTableData[i].m_ouTableHandles.begin();
                        advance(itrHandle, nIndex );
                        if ( itrHandle != m_ouTableData[i].m_ouTableHandles.end() )
                        {
                            nChannel = i+1;
                            m_pouDIL_LIN_Interface->DILL_StartLinScheduleTable(m_dwClientID, nChannel, *itrHandle);
                        }

                    }
                    nIndex++;
                }
            }
        }
    }
    else if( eBusStatus == BUS_DISCONNECTED )
    {

    }
}

void CLINScheduleDataStore::vSetScheduleConfig(xmlNodePtr pNode)
{
    xmlChar* pchPath = (xmlChar*)DEF_INDEX;
    xmlXPathObjectPtr pObjectPath = xmlUtils::pGetChildNodes(pNode, pchPath);
    INT nChannel = 0;
    if  (pObjectPath != NULL )
    {
        xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
        if( NULL != pNodeSet )
        {
            xmlNodePtr pNode = pNodeSet->nodeTab[0];       //Take First One only

            xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode , 1);
            nChannel = atoi( (char*)key) - 1;
        }
    }

    // Validating channel number aginst available channels after conversion
    int nNumChannels = 0;
    m_ouClusterConfig->GetChannelCount(LIN, nNumChannels);
    if (nChannel > nNumChannels || nChannel < 0)
    {
        nChannel = 0;
    }

    pchPath = (xmlChar*)DEF_NAME;
    pObjectPath = xmlUtils::pGetChildNodes(pNode, pchPath);
    std::string strScheduleName;
    if  (pObjectPath != NULL )
    {
        xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
        if( NULL != pNodeSet )
        {
            xmlNodePtr pNode = pNodeSet->nodeTab[0];       //Take First One only

            xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode , 1);
            strScheduleName = (char*)key;
        }
    }

    std::list<FrameState> lstFrames;

    xmlChar* pchPathMsg = (xmlChar*)DEF_FRAMES;
    pObjectPath = xmlUtils::pGetChildNodes(pNode, pchPathMsg);
    if  (pObjectPath != NULL )
    {
        xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
        if( NULL != pNodeSet )
        {
            for ( int nChldIndex =0 ; nChldIndex < pNodeSet->nodeNr; nChldIndex++ )
            {
                xmlNodePtr pNode = pNodeSet->nodeTab[nChldIndex];       //Take First One only
                xmlChar* pchPathMsg = (xmlChar*)DEF_FRAME;
                xmlXPathObjectPtr pObjectPath = xmlUtils::pGetChildNodes(pNode, pchPathMsg);
                if  (pObjectPath != NULL )
                {
                    xmlNodeSetPtr pFrameNodeSet = pObjectPath->nodesetval;
                    FrameState omFrameState;
                    // Get the frame attributes
                    for ( int nFrameIndex =0 ; nFrameIndex < pFrameNodeSet->nodeNr; nFrameIndex++ )
                    {
                        xmlNodePtr pFrameNode = pFrameNodeSet->nodeTab[nFrameIndex];       //Take First One only
                        xmlChar* pchPathMsg = (xmlChar*)DEF_NAME;
                        xmlXPathObjectPtr pObjectPath = xmlUtils::pGetChildNodes(pFrameNode, pchPathMsg);
                        if  (pObjectPath != NULL )
                        {
                            pNodeSet = pObjectPath->nodesetval;
                            pNode = pNodeSet->nodeTab[0];
                            xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode , 1);
                            omFrameState.omName = (char*)key;
                        }

                        pchPathMsg = (xmlChar*)DEF_IsEnabled;
                        pObjectPath = xmlUtils::pGetChildNodes(pFrameNode, pchPathMsg);
                        if  (pObjectPath != NULL )
                        {
                            pNodeSet = pObjectPath->nodesetval;
                            pNode = pNodeSet->nodeTab[0];
                            xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode , 1);
                            std::string omIsEnabled = (char*)key;
                            if(omIsEnabled == DEF_TRUE)
                            {
                                omFrameState.bIsEnabled = TRUE;
                            }
                            else
                            {
                                omFrameState.bIsEnabled = FALSE;
                            }
                        }
                        lstFrames.push_back(omFrameState);
                    }
                }
            }
        }
    }

    std::list<CSheduleTable>::iterator itrTable = m_ouTableData[nChannel].m_ouScheduleTables.begin();

    while( itrTable != m_ouTableData[nChannel].m_ouScheduleTables.end() )
    {
        if ( itrTable->m_strTableName == strScheduleName )
        {
            m_ouTableData[nChannel].m_strCurrentTable = strScheduleName;
            std::list<CScheduleCommands>::iterator itrCommand;
            INT nIndex = 0;
            for (itrCommand = itrTable->m_listCommands.begin(); itrCommand != itrTable->m_listCommands.end(); itrCommand++ )
            {
                std::list<FrameState>::iterator itrFrame = lstFrames.begin();
                advance(itrFrame, nIndex);
                if ( itrFrame != lstFrames.end() )
                {
                    if(itrFrame->omName.compare(itrCommand->m_strCommandName.c_str()) == 0)
                    {
                        itrCommand->m_bEnabled = itrFrame->bIsEnabled;
                    }

                }
                nIndex++;
            }
            break;
            m_ouTableData[nChannel].m_strCurrentTable = strScheduleName;
        }
        itrTable++;
    }
}

HRESULT CLINScheduleDataStore::hGetConfigData(xmlNodePtr pNodePtr)
{
    std::map<std::string, std::string>::iterator itrFrame;

    int nChannelCount;
    m_ouClusterConfig->GetChannelCount(LIN, nChannelCount);

    //Channel Messages;
    for ( int nIndex = 0; nIndex < nChannelCount; nIndex++ )
    {

        xmlNodePtr pNodeChannel = xmlNewNode(NULL, BAD_CAST DEF_CHANNEL);
        xmlAddChild(pNodePtr, pNodeChannel);

        char chChannel[MAX_PATH] = {0};
        sprintf_s(chChannel, "%d", nIndex+1);
        xmlNewChild(pNodeChannel, NULL, BAD_CAST DEF_INDEX, BAD_CAST chChannel);

        if(CLINScheduleDataStore::pGetLINSchedDataStore().m_ouTableData[nIndex].m_strCurrentTable.empty() == false)
        {
            std::string strSchdTableName = CLINScheduleDataStore::pGetLINSchedDataStore().m_ouTableData[nIndex].m_strCurrentTable;

            xmlNewChild(pNodeChannel, NULL, BAD_CAST DEF_NAME, BAD_CAST strSchdTableName.c_str());


for ( auto &itr :  CLINScheduleDataStore::pGetLINSchedDataStore().m_ouTableData[nIndex].m_ouScheduleTables )
            {
                if ( itr.m_strTableName == strSchdTableName )
                {
                    xmlNodePtr pFramesNode = xmlNewNode(NULL, BAD_CAST DEF_FRAMES);
                    xmlAddChild(pNodeChannel, pFramesNode);
for ( auto &itrCommand : itr.m_listCommands )
                    {
                        xmlNodePtr pFrameNode = xmlNewNode(NULL, BAD_CAST DEF_FRAME);
                        xmlAddChild(pFramesNode, pFrameNode);

                        xmlNewChild(pFrameNode, NULL, BAD_CAST DEF_NAME, BAD_CAST itrCommand.m_strCommandName.c_str());
                        if ( itrCommand.m_bEnabled == true )
                        {
                            xmlNewChild(pFrameNode, NULL, BAD_CAST DEF_IsEnabled, BAD_CAST DEF_TRUE);
                        }
                        else
                        {
                            xmlNewChild(pFrameNode, NULL, BAD_CAST DEF_IsEnabled, BAD_CAST DEF_FALSE);
                        }
                    }
                }
            }
        }
    }

    return S_OK;
}

HRESULT CLINScheduleDataStore::hSetClusterConfig(IBMNetWorkGetService* objLINClusterConfig)
{
    int unChannels =0;
    objLINClusterConfig->GetChannelCount(LIN, unChannels);
    for( int i= 0 ; i < unChannels; i++ )
    {
        ChannelSettings ouChannelSettings;
        objLINClusterConfig->GetChannelSettings(LIN, i, &ouChannelSettings);
        bool bTableFound = bAddTableListToList( m_ouTableData[i].m_strCurrentTable, m_ouTableData[i].m_ouScheduleTables, ouChannelSettings.m_ouLINSettings.ouLinParams);
        if ( bTableFound == false )
        {
            m_ouTableData[i].m_strCurrentTable.clear();
        }

    }

    m_ouClusterConfig = objLINClusterConfig;
    return S_OK;
}

void CLINScheduleDataStore::vAddCommandsToTable(CSheduleTable& ouDestTable, CSheduleTable& ouSourceTable )
{
    std::list<CScheduleCommands>::iterator itrSource, itrDest;
    CSheduleTable ouTempTable = ouDestTable;

    ouDestTable.m_listCommands.clear();
    itrDest = ouTempTable.m_listCommands.begin();
    CScheduleCommands ouTemp;
    for ( itrSource = ouSourceTable.m_listCommands.begin(); itrSource != ouSourceTable.m_listCommands.end(); itrSource++, itrDest++ )
    {
        ouTemp = *itrDest;
        if ( itrDest->m_strCommandName == itrSource->m_strCommandName )
        {
            ouTemp.m_bEnabled = itrDest->m_bEnabled;
        }
        ouDestTable.m_listCommands.push_back(ouTemp);
    }
}



bool CLINScheduleDataStore::bAddTableListToList(std::string strSelectedTable, std::list<CSheduleTable>& ouDestTable, std::list<CSheduleTable>& ouSourceTable )
{
    std::list<CSheduleTable>::iterator itr;
    CSheduleTable ouTempTable;
    bool bTableFound = false;

    for ( itr = ouDestTable.begin(); itr != ouDestTable.end(); itr++ )
    {
        if( itr->m_strTableName == strSelectedTable )
        {
            ouTempTable = *itr;
            break;
        }
    }
    ouDestTable.clear();
    for ( itr = ouSourceTable.begin(); itr != ouSourceTable.end(); itr++ )
    {
        if ( itr->m_strTableName == strSelectedTable )
        {
            vAddCommandsToTable(ouTempTable, *itr);
            ouDestTable.push_back(ouTempTable);
            bTableFound = true;
        }
        else
        {
            ouDestTable.push_back(*itr);
        }
    }
    return bTableFound;
}



int CLINScheduleDataStore::nGetTableHandle(int nChannel, int nTableIndex)
{
    if ( ( nChannel < 0 ) && ( nChannel > CHANNEL_ALLOWED ) )
    {
        return -1;
    }
    std::list<int>::iterator itr = m_ouTableData[nChannel].m_ouTableHandles.begin();
    advance(itr, nTableIndex);
    if ( itr != m_ouTableData[nChannel].m_ouTableHandles.end() )
    {
        return *itr;
    }
    return -1;
}

bool CLINScheduleDataStore::bIsFrameSupported(eCommandType eCmdType)
{
    switch(eCmdType)
    {
        case COMMAND_SPORADIC:
        case COMMAND_EVENT:
        {
            return false;
        }

        break;
        default:
        {
            return true;
        }
        break;
    }
}

int CLINScheduleDataStore::nEnableCommands(CSheduleTable& ouTable, bool bEnable)
{
    std::list<CScheduleCommands>::iterator itr = ouTable.m_listCommands.begin();
    while( itr != ouTable.m_listCommands.end() )
    {
        if (bIsFrameSupported(itr->m_eCommandType) == true )
        {
            itr->m_bEnabled = bEnable;
        }
        else
        {
            itr->m_bEnabled = false;
        }
        itr++;
    }
    return S_OK;
}

/*
1. Click on Schedule table
    ->Enable
        1. Enable All Items
        2. Disable all other tables
    ->Disable
        1. Disable all items
        */
int CLINScheduleDataStore::nEnableScheduleTable(int nChannel, int nIndex, bool bEnable )
{
    if ( ( nChannel < 0 ) && ( nChannel > CHANNEL_ALLOWED ) )
    {
        return S_FALSE;
    }

    std::list<CSheduleTable>::iterator itrTable = m_ouTableData[nChannel].m_ouScheduleTables.begin();
    int i = 0;

    if ( bEnable == true )
    {
        while ( itrTable != m_ouTableData[nChannel].m_ouScheduleTables.end() )
        {
            if ( i == nIndex )
            {
                nEnableCommands(*itrTable);
                m_ouTableData[nChannel].m_strCurrentTable = itrTable->m_strTableName;
            }
            else
            {
                nEnableCommands(*itrTable, false);
            }
            itrTable++;
            i++;
        }
    }
    else
    {
        advance(itrTable, nIndex );
        if ( itrTable != m_ouTableData[nChannel].m_ouScheduleTables.end() )
        {
            nEnableCommands(*itrTable, false);
            m_ouTableData[nChannel].m_strCurrentTable.clear();
        }
    }
    nTransmitScheduleTable(nChannel, nIndex, bEnable);

    return S_OK;
}
/*
2. Click on Command
    ->Enable
        1. Enable It
        2. Enable Parent
        3. Disable Other Parents(Tables)
    ->Disable
        1. Disable It
        2. Disable parent if all child are disabled
*/

int CLINScheduleDataStore::nTransmitScheduleCommand(int nChannelIndex, int nTableIndex, int nItem, bool bEnable )
{
    int nHandle = nGetTableHandle(nChannelIndex, nTableIndex);
    if ( nHandle != -1 )
    {
        m_pouDIL_LIN_Interface->DIIL_EnableLinScheuleCommand(m_dwClientID, nChannelIndex, nHandle, nItem, bEnable);
        if ( bEnable == true  )
        {
            int nChannel = nChannelIndex+1;
            m_pouDIL_LIN_Interface->DILL_StartLinScheduleTable(m_dwClientID, nChannel, nHandle);
        }
        return S_OK;
    }
    return S_FALSE;
}

int CLINScheduleDataStore::nTransmitScheduleTable(int nChannelIndex, int nTableIndex, bool bEnable )
{
    if ( m_eBusStatus != BUS_CONNECTED )
    {
        return S_FALSE;
    }

    int nHandle = nGetTableHandle(nChannelIndex, nTableIndex);

    ChannelSettings ouChannelSettings;
    m_ouClusterConfig->GetChannelSettings(LIN, nChannelIndex, &ouChannelSettings);
    if ( nHandle != -1 && ouChannelSettings.m_ouLINSettings.m_bIsMasterMode == true)
    {
        std::list<CSheduleTable>::iterator itrTable = m_ouTableData[nChannelIndex].m_ouScheduleTables.begin();
        advance(itrTable, nTableIndex);

        if ( itrTable != m_ouTableData[nChannelIndex].m_ouScheduleTables.end() )
        {
            m_pouDIL_LIN_Interface->DILL_UpdateLinScheduleTable(m_dwClientID, nChannelIndex, nHandle, *itrTable);
        }

        if ( bEnable == true )
        {
            int nChannel = nChannelIndex+1;
            m_pouDIL_LIN_Interface->DILL_StartLinScheduleTable(m_dwClientID, nChannel, nHandle);
        }
        return S_OK;
    }
    return S_FALSE;
}


int CLINScheduleDataStore::nEnableScheduleCommand(int nChannel, int nTableIndex, int nItemIndex, bool bEnable )
{
    if ( ( nChannel < 0 ) && ( nChannel >= CHANNEL_ALLOWED ) )
    {
        return S_FALSE;
    }

    if ( bEnable == true )
    {

        std::list<CSheduleTable>::iterator itrTable = m_ouTableData[nChannel].m_ouScheduleTables.begin();
        int nCurrentIndex = 0;
        while (itrTable != m_ouTableData[nChannel].m_ouScheduleTables.end())
        {
            if ( nCurrentIndex == nTableIndex )
            {
                std::list<CScheduleCommands>::iterator itrCommand = itrTable->m_listCommands.begin();
                advance(itrCommand, nItemIndex);
                if ( itrCommand !=  itrTable->m_listCommands.end() )
                {
                    itrCommand->m_bEnabled = bEnable;
                    if ( m_ouTableData[nChannel].m_strCurrentTable != itrTable->m_strTableName)
                    {
                        m_ouTableData[nChannel].m_strCurrentTable = itrTable->m_strTableName;

                        nTransmitScheduleTable(nChannel, nTableIndex);
                    }
                    else
                    {
                        nTransmitScheduleCommand(nChannel, nTableIndex, nItemIndex, bEnable);
                    }
                }
            }
            else
            {
                nEnableCommands(*itrTable, false );
            }
            itrTable++;
            nCurrentIndex++;
        }
    }
    else
    {
        std::list<CSheduleTable>::iterator itrTable = m_ouTableData[nChannel].m_ouScheduleTables.begin();
        advance(itrTable, nTableIndex);
        if (itrTable != m_ouTableData[nChannel].m_ouScheduleTables.end())
        {
            std::list<CScheduleCommands>::iterator itrCommand = itrTable->m_listCommands.begin();
            int i = 0;
            int bENabled = 0;
            while ( itrCommand != itrTable->m_listCommands.end() )
            {
                if ( i == nItemIndex )
                {
                    itrCommand->m_bEnabled = false;
                }
                if ( itrCommand->m_bEnabled )
                {
                    bENabled++;
                }
                itrCommand++;
                i++;
            }
            if ( bENabled == 0 )
            {
                m_ouTableData[nChannel].m_strCurrentTable.clear();
            }
            nTransmitScheduleCommand(nChannel, nTableIndex, nItemIndex, bEnable);
        }
    }

    return S_OK;
}




IBMNetWorkGetService* CLINScheduleDataStore::pGetClusterConfig()
{
    return m_ouClusterConfig;
}

CLINScheduleDataStore& CLINScheduleDataStore::pGetLINSchedDataStore()
{
    return m_sTxLINDataStoreObj;
}

HRESULT CLINScheduleDataStore::hSaveSchdSelectionDetails()
{
    return S_OK;
}

void CLINScheduleDataStore::vSetWindowPlacement(WINDOWPLACEMENT wndPlacement)
{
    m_sSchedWndPlacement = wndPlacement;
}

void CLINScheduleDataStore::vGetWindowPlacement(WINDOWPLACEMENT& wndPlacement)
{
    if (m_bValidWndSize == FALSE)
    {
        m_sSchedWndPlacement.flags = 1;
        //m_sSchedWndPlacement.length = 44;
        m_sSchedWndPlacement.ptMaxPosition.x = 0;
        m_sSchedWndPlacement.ptMaxPosition.x = 0;
        m_sSchedWndPlacement.ptMinPosition.x = 0;
        m_sSchedWndPlacement.ptMinPosition.y = 0;
        m_sSchedWndPlacement.rcNormalPosition.top = 183;
        m_sSchedWndPlacement.rcNormalPosition.bottom = 716;
        m_sSchedWndPlacement.rcNormalPosition.left = 535;
        m_sSchedWndPlacement.rcNormalPosition.right = 1185;
        m_sSchedWndPlacement.showCmd = 1;
        m_bValidWndSize = true;
    }

    wndPlacement = m_sSchedWndPlacement;
}