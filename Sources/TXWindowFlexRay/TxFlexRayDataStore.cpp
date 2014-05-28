/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      TxWndDataStore.cpp
 * \author    Ashwin R Uchil
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once
#include "TxWindowFlexRay_stdafx.h"
#include "..\DIL_Interface\BaseDIL_FLEXRAY.h"
#include "TxFlexRayDataStore.h"
#include "Utility/Utility.h"
#include "..\include/XMLDefines.h"
#include "Utility/XMLUtils.h"
#include "..\DIL_Interface\DIL_Interface_extern.h"

static CEvent sg_omMainTimerEvent;
class CTxMsgChildFrame;
CBaseDIL_FLEXRAY* g_pouDIL_FLEXRAY_Interface = NULL; // FLEXRAY DIL interface
CTxFlexRayDataStore CTxFlexRayDataStore::m_sTxFlexRayDataStoreObj;

CTxFlexRayDataStore::CTxFlexRayDataStore(void)
{
    m_bTransmissionON = false;
    m_bAutoUpdate = false;
    InitializeCriticalSection(&m_MsgBlockCriticalSection);
    m_nChannelsConfigured = 0;
    m_bValidTxWndSize = false;
    m_eBusStatus = BUS_DISCONNECTED;
    m_bHexMode = false;

    SetDefaultWindowPos();
}

void CTxFlexRayDataStore::SetDefaultWindowPos()
{
    m_sTxWndPlacement.flags = 1;
    m_sTxWndPlacement.length = 44;
    m_sTxWndPlacement.ptMaxPosition.x = 0;
    m_sTxWndPlacement.ptMaxPosition.x = 0;
    m_sTxWndPlacement.ptMinPosition.x = 0;
    m_sTxWndPlacement.ptMinPosition.y = 0;
    m_sTxWndPlacement.rcNormalPosition.top = 1;
    m_sTxWndPlacement.rcNormalPosition.bottom = 661;
    m_sTxWndPlacement.rcNormalPosition.left = 4;
    m_sTxWndPlacement.rcNormalPosition.right = 864;
    m_sTxWndPlacement.showCmd = 1;
}

CTxFlexRayDataStore::~CTxFlexRayDataStore(void)
{
    DeleteCriticalSection(&m_MsgBlockCriticalSection);
}


CTxFlexRayDataStore& CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj()
{
    return m_sTxFlexRayDataStoreObj;
}

/////////global functions

/*************************************************************************
    Function Name    : CalcTimersExecTime
    Input(s)         :
    Output           :
    Functionality    :
    Member of        :
    Author(s)        :  Ashwin R Uchil
    Date Created     :
**************************************************************************/

UINT CalcTxTimersExecTime(LPVOID pParam)
{
    PSCALCEXECTIMETHREAD psThreadInfo = (PSCALCEXECTIMETHREAD) pParam;
    if (psThreadInfo != NULL)
    {
        CTxFlexRayDataStore* pDataStoreObj = psThreadInfo->m_pTxFlexRayDataStore;
        if (pDataStoreObj != NULL)
        {
            while (!psThreadInfo->m_bThreadStop)
            {
                // WaitForSingleObject(sg_omMainTimerEvent, INFINITE);
                if (!psThreadInfo->m_bThreadStop)
                {
                    //pDataStoreObj->vManageTimerExecution();
                }
            }
            psThreadInfo->m_pomThreadPtr = NULL;
            psThreadInfo->m_omExitThreadEvent.SetEvent();
        }
        else
        {
            ASSERT(FALSE);
        }
    }
    else
    {
        ASSERT(FALSE);
    }
    return 0;
}

void CTxFlexRayDataStore::vSetBusStatus(ESTATUS_BUS eBusStatus)
{
    m_eBusStatus = eBusStatus;
    if ( m_eBusStatus == BUS_CONNECTED )
    {
        std::list<FLEXRAY_FRAME_DATA>::iterator itrFrameData =  m_ouFlexray_Frame_Data[0].begin();
        while( itrFrameData != m_ouFlexray_Frame_Data[0].end() )
        {
            if ( itrFrameData->bSelected == true )
            {
                UpdateMessagetoDIL(*itrFrameData, false);
            }
            itrFrameData++;
        }
    }
}

int CTxFlexRayDataStore::UpdateMessagetoDIL(FLEXRAY_FRAME_DATA& ouFlexData,  bool bDelete)
{
    if ( g_pouDIL_FLEXRAY_Interface != NULL && m_eBusStatus == BUS_CONNECTED)
    {
        s_FLXTXMSG ouFlxMsg;
        ouFlxMsg.m_sFlxMsg.m_nSlotID = ouFlexData.m_ouFlexFrame.m_nSlotId;
        ouFlxMsg.m_sFlxMsg.m_nBaseCycle = ouFlexData.m_ouFlexFrame.m_nBaseCycle;
        ouFlxMsg.m_sFlxMsg.m_nDLC = ouFlexData.m_ouFlexFrame.m_nLength;
        ouFlxMsg.m_sFlxMsg.m_eChannel = ouFlexData.m_ouFlexFrame.m_ouChannel;

        memcpy(ouFlxMsg.m_sFlxMsg.m_ucData, ouFlexData.m_ouData, ouFlexData.m_ouFlexFrame.m_nLength);

        if ( bDelete == false )
        {
            g_pouDIL_FLEXRAY_Interface->DILF_SendFlxMsg(0, &ouFlxMsg);
        }
        else
        {
            g_pouDIL_FLEXRAY_Interface->DILF_DeleteFlxMsg(0, &ouFlxMsg);
        }
    }
    return 0;
}


bool CTxFlexRayDataStore::bSetChannelConfig(xmlNodePtr pNode)
{
    //Index
    int nChannel = -1;
    xmlChar* pchPath = (xmlChar*)"Index";
    xmlXPathObjectPtr pObjectPath = xmlUtils::pGetChildNodes(pNode, pchPath);
    if  (pObjectPath != NULL )
    {
        xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
        if( NULL != pNodeSet )
        {
            xmlNodePtr pNode = pNodeSet->nodeTab[0];       //Take First One only
            std::string strVar;
            xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode , 1);
            nChannel = atoi( (char*)key);            //Trigger value
        }
    }

    if ( nChannel < 0 || nChannel > CHANNEL_ALLOWED )
    {
        return false;
    }

    m_ouFlexray_Frame_Data[nChannel].clear();

    xmlChar* pchPathMsg = (xmlChar*)"Message";
    pObjectPath = xmlUtils::pGetChildNodes(pNode, pchPathMsg);
    if  (pObjectPath != NULL )
    {
        xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
        if( NULL != pNodeSet )
        {

            for ( int i =0 ; i < pNodeSet->nodeNr; i++ )
            {
                xmlNodePtr pNode = pNodeSet->nodeTab[i];       //Take First One only
                FLEXRAY_CONFIG_DATA ouData;
                if ( parseForMessage(pNode, ouData) == true )
                {
                    bAddToChannelList(nChannel, ouData);
                }

            }

        }
    }
    return true;
}
bool CTxFlexRayDataStore::bAddToChannelList(int nChannel, FLEXRAY_CONFIG_DATA ouData)
{
    std::list<FRAME_STRUCT>::iterator itrFramList = m_ouFrameList[nChannel].begin();

    for( ; itrFramList != m_ouFrameList[nChannel].end(); itrFramList++ )
    {
        if ( (itrFramList->m_strFrameName == ouData.m_strMessageName) && (itrFramList->m_nSlotId == ouData.m_nSlotId) )
        {
            FLEXRAY_FRAME_DATA ouFrameData;
            ouFrameData.m_ouFlexFrame = *itrFramList;
            ouFrameData.bSelected = ouData.m_bEnable;
            memcpy( ouFrameData.m_ouData, ouData.m_ucData, ouData.m_nDLC);
            m_ouFlexray_Frame_Data[nChannel].push_back(ouFrameData);
        }
    }
    return true;
}

bool bIsValidMessage(int /* nChannelIndex */, FLEXRAY_CONFIG_DATA & /* ouConfigFrame */)
{
    return true;
}


bool CTxFlexRayDataStore::parseForMessage(xmlNodePtr ptrNode, FLEXRAY_CONFIG_DATA& ouData)
{
    if ( ptrNode == NULL)
    {
        return false;
    }
    else
    {
        //Name
        xmlChar* pchPathMsg = (xmlChar*)DEF_NAME;
        xmlXPathObjectPtr pObjectPath = xmlUtils::pGetChildNodes(ptrNode, pchPathMsg);
        if  (pObjectPath != NULL )
        {
            xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
            if( NULL != pNodeSet )
            {
                xmlNodePtr pNode = pNodeSet->nodeTab[0];       //Take First One only
                xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode , 1);
                ouData.m_strMessageName = (char*)key;
            }
        }

        //Slot Id
        pchPathMsg = (xmlChar*)DEF_SLOT;
        pObjectPath = xmlUtils::pGetChildNodes(ptrNode, pchPathMsg);
        if  (pObjectPath != NULL )
        {
            xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
            if( NULL != pNodeSet )
            {
                xmlNodePtr pNode = pNodeSet->nodeTab[0];       //Take First One only
                xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode , 1);
                ouData.m_nSlotId = atoi((char*)key);
            }
        }


        //BaseCycle
        pchPathMsg = (xmlChar*)DEF_BASECYCLE;
        pObjectPath = xmlUtils::pGetChildNodes(ptrNode, pchPathMsg);
        if  (pObjectPath != NULL )
        {
            xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
            if( NULL != pNodeSet )
            {
                xmlNodePtr pNode = pNodeSet->nodeTab[0];       //Take First One only
                xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode , 1);
                ouData.m_nBaseCycle = atoi((char*)key);
            }
        }


        //Repetion
        pchPathMsg = (xmlChar*)DEF_REPETION;
        pObjectPath = xmlUtils::pGetChildNodes(ptrNode, pchPathMsg);
        if  (pObjectPath != NULL )
        {
            xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
            if( NULL != pNodeSet )
            {
                xmlNodePtr pNode = pNodeSet->nodeTab[0];       //Take First One only
                xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode , 1);
                ouData.m_nRepetition = atoi((char*)key);
            }
        }

        //DLC
        pchPathMsg = (xmlChar*)DEF_DLC;
        pObjectPath = xmlUtils::pGetChildNodes(ptrNode, pchPathMsg);
        if  (pObjectPath != NULL )
        {
            xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
            if( NULL != pNodeSet )
            {
                xmlNodePtr pNode = pNodeSet->nodeTab[0];       //Take First One only
                xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode , 1);
                ouData.m_nDLC = atoi((char*)key);
            }
        }

        //DataBytes
        pchPathMsg = (xmlChar*)DEF_DATABYTES;
        pObjectPath = xmlUtils::pGetChildNodes(ptrNode, pchPathMsg);
        if  (pObjectPath != NULL )
        {
            xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
            if( NULL != pNodeSet )
            {
                xmlNodePtr pNode = pNodeSet->nodeTab[0];       //Take First One only
                xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode , 1);

                char* pch;
                pch = strtok ((char*)key," ,.-");
                int i =0;
                while (pch != NULL)
                {
                    ouData.m_ucData[i] = atoi(pch);
                    pch = strtok (NULL, " ,.-");
                    i++;
                }
            }
        }

        //Enabled?
        pchPathMsg = (xmlChar*)DEF_ENABLE;
        pObjectPath = xmlUtils::pGetChildNodes(ptrNode, pchPathMsg);
        if  (pObjectPath != NULL )
        {
            xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
            if( NULL != pNodeSet )
            {
                xmlNodePtr pNode = pNodeSet->nodeTab[0];       //Take First One only
                xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode , 1);
                ouData.m_bEnable = true;
                if  ( strcmp((char*)key, "FALSE" ) == 0 )
                {
                    ouData.m_bEnable = false;
                }
            }
        }

    }
    return true;
}


bool CTxFlexRayDataStore::bSetConfigData(xmlDocPtr pDoc)
{
    if(pDoc == NULL)
    {
        m_sTxWndPlacement.rcNormalPosition.top = -1;
        m_sTxWndPlacement.length = 0;
        for ( int  i = 0 ; i < CHANNEL_ALLOWED; i++ )
        {
            m_ouFlexray_Frame_Data[i].clear();
            m_ouFrameList[i].clear();
        }
        return false;
    }

    xmlChar* pXpath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/FLEX_Tx_Window/Window_Position";

    xmlXPathObjectPtr pObjectPath = xmlUtils::pGetNodes(pDoc, pXpath);
    if  (pObjectPath != NULL )
    {
        xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
        if( NULL != pNodeSet )
        {
            xmlNodePtr pNode = pNodeSet->nodeTab[0];       //Take First One only
            WINDOWPLACEMENT WndPlacement;
            xmlUtils::ParseWindowsPlacement(pNode,WndPlacement);
            bSetTxData(TX_WINDOW_PLACEMENT, &WndPlacement);
            m_bValidTxWndSize = true;
        }
    }
    else
    {
        m_bValidTxWndSize = false;
    }

    xmlChar* pXpathChannel = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/FLEX_Tx_Window/Channel_Msg_List";
    pObjectPath = xmlUtils::pGetNodes(pDoc, pXpathChannel);
    if  (pObjectPath != NULL )
    {
        xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
        if( NULL != pNodeSet )
        {
            for ( int i = 0 ; i < pNodeSet->nodeNr; i++)
            {
                xmlNodePtr pNode = pNodeSet->nodeTab[0];       //Take First One only
                bSetChannelConfig(pNode);
            }
        }
    }

    return true;
}


BOOL CTxFlexRayDataStore::bGetConfigData(xmlNodePtr pxmlNodePtr)
{
    //windows position
    xmlNodePtr pNodeWndPos = xmlNewNode(NULL, BAD_CAST DEF_WND_POS);
    xmlAddChild(pxmlNodePtr, pNodeWndPos);

    xmlUtils::CreateXMLNodeFrmWindowsPlacement(pNodeWndPos,m_sTxWndPlacement);

    //Channel Messages;
    for ( int i = 0; i < m_nChannelsConfigured; i++ )
    {
        xmlNodePtr pNodeWndPos = xmlNewNode(NULL, BAD_CAST DEF_CHANNEL_MSG_NAME);
        if ( bGetMessageListConfig(i, pNodeWndPos) == TRUE )
        {
            xmlAddChild(pxmlNodePtr, pNodeWndPos);
        }
    }


    return TRUE;
}
BOOL CTxFlexRayDataStore::bGetMessageListConfig(int nChannel, xmlNodePtr pNode)
{
    if ( nChannel < 0  || nChannel > CHANNEL_ALLOWED )
    {
        return FALSE;
    }
    char pchData[1024];


    //Index
    sprintf(pchData, "%d", nChannel);
    xmlNewChild(pNode, NULL, BAD_CAST DEF_INDEX, BAD_CAST pchData);

    std::list<FLEXRAY_FRAME_DATA>::iterator itrFrameData = m_ouFlexray_Frame_Data[nChannel].begin();
    for (; itrFrameData != m_ouFlexray_Frame_Data[nChannel].end(); itrFrameData++ )
    {
        xmlNodePtr pMsgNode = xmlNewNode(NULL, BAD_CAST DEF_MESSAGE );
        if ( NULL != pMsgNode )
        {
            //Name
            xmlNewChild(pMsgNode, NULL, BAD_CAST DEF_NAME, BAD_CAST itrFrameData->m_ouFlexFrame.m_strFrameName.c_str());

            //Slot Id
            sprintf(pchData, "%d", itrFrameData->m_ouFlexFrame.m_nSlotId);
            xmlNewChild(pMsgNode, NULL, BAD_CAST DEF_SLOT, BAD_CAST pchData);

            //Base Cycle
            sprintf(pchData, "%d", itrFrameData->m_ouFlexFrame.m_nBaseCycle);
            xmlNewChild(pMsgNode, NULL, BAD_CAST DEF_BASECYCLE, BAD_CAST pchData);

            //Repetition
            sprintf(pchData, "%d", itrFrameData->m_ouFlexFrame.m_nReptition);
            xmlNewChild(pMsgNode, NULL, BAD_CAST DEF_REPETION, BAD_CAST pchData);

            //DLC
            sprintf(pchData, "%d", itrFrameData->m_ouFlexFrame.m_nLength);
            xmlNewChild(pMsgNode, NULL, BAD_CAST DEF_DLC, BAD_CAST pchData);

            //Data Bytes
            std::string strDatabytes;
            sprintf(pchData, "%d", itrFrameData->m_ouData[0]);
            strDatabytes = pchData;
            for ( int i = 1 ; i < itrFrameData->m_ouFlexFrame.m_nLength; i++)
            {
                sprintf(pchData, ",%d", itrFrameData->m_ouData[i]);
                strDatabytes.append(pchData);
            }

            xmlNewChild(pMsgNode, NULL, BAD_CAST DEF_DATABYTES, BAD_CAST strDatabytes.c_str());

            //is selected
            if ( itrFrameData->bSelected == true )
            {
                xmlNewChild(pMsgNode, NULL, BAD_CAST DEF_ENABLE, BAD_CAST "TRUE");
            }
            else
            {
                xmlNewChild(pMsgNode, NULL, BAD_CAST DEF_ENABLE, BAD_CAST "FALSE");
            }
        }
        xmlAddChild(pNode, pMsgNode);
    }
    return TRUE;
}


BOOL CTxFlexRayDataStore::bSetTxData(eTXWNDDETAILS  eParam, LPVOID lpVoid)
{
    BOOL bRetVal = TRUE;
    switch(eParam)
    {
        case TX_WINDOW_PLACEMENT:
        {
            WINDOWPLACEMENT* psData =
                static_cast<WINDOWPLACEMENT*>(lpVoid);
            m_sTxWndPlacement = *psData;
        }
        break;
        default:
        {
            ASSERT(FALSE);
        }
        break;
    }
    return bRetVal;
}
BOOL CTxFlexRayDataStore::bGetTxData(eTXWNDDETAILS  eParam, LPVOID* lpData)
{
    BOOL bRetVal = TRUE;
    switch(eParam)
    {
        case TX_WINDOW_PLACEMENT:
        {
            if ( m_bValidTxWndSize == true)
            {
                WINDOWPLACEMENT* psData =
                    static_cast<WINDOWPLACEMENT*>(*lpData);
                *psData = m_sTxWndPlacement;
                m_bValidTxWndSize = false;
            }
            else
            {
                m_sTxWndPlacement.flags = 1;
                m_sTxWndPlacement.length = 44;
                m_sTxWndPlacement.ptMaxPosition.x = 0;
                m_sTxWndPlacement.ptMaxPosition.x = 0;
                m_sTxWndPlacement.ptMinPosition.x = 0;
                m_sTxWndPlacement.ptMinPosition.y = 0;
                m_sTxWndPlacement.rcNormalPosition.top = 1;
                m_sTxWndPlacement.rcNormalPosition.bottom = 661;
                m_sTxWndPlacement.rcNormalPosition.left = 4;
                m_sTxWndPlacement.rcNormalPosition.right = 864;
                m_sTxWndPlacement.showCmd = 1;
                m_bValidTxWndSize = true;
            }

        }
        break;
        default:
        {
            ASSERT(FALSE);
        }
        break;
    }
    return !m_bValidTxWndSize;
}
BOOL CTxFlexRayDataStore::bSetDILInterfacePtr()
{
    HRESULT hResult = S_OK;
    if (g_pouDIL_FLEXRAY_Interface == NULL)
    {
        hResult = DIL_GetInterface(FLEXRAY, (void**)&g_pouDIL_FLEXRAY_Interface);
    }
    return (BOOL)hResult;
}

HRESULT CTxFlexRayDataStore::SetFlexRayConfig(ClusterConfig& ouFlexConfig)
{
    for ( int i = 0 ; i < ouFlexConfig.m_nChannelsConfigured; i++ )
    {
        m_ouFrameList[i].clear();
        ouFlexConfig.m_ouFlexChannelConfig[i].GetSelectedECUTxFrames(m_ouFrameList[i]);
        vUpdateTxList(i);
    }
    //m_ouFlexConfig = ouFlexConfig;
    m_nChannelsConfigured = ouFlexConfig.m_nChannelsConfigured;

    //Upddate the Tx Window List;


    return S_OK;
}

void CTxFlexRayDataStore::vUpdateTxList(int nChannel)
{
    if ( nChannel < 0 || nChannel > CHANNEL_ALLOWED )
    {
        return;
    }
    std::list<FLEXRAY_FRAME_DATA>::iterator itrFrameData = m_ouFlexray_Frame_Data[nChannel].begin();
    while ( itrFrameData != m_ouFlexray_Frame_Data[nChannel].end() )
    {
        if ( bExistInTxList(nChannel, *itrFrameData) == false )
        {
            itrFrameData = m_ouFlexray_Frame_Data[nChannel].erase (itrFrameData);
        }
        else
        {
            itrFrameData++;
        }
    }
}
bool CTxFlexRayDataStore::bExistInTxList(int nChannel, FLEXRAY_FRAME_DATA& ouData)
{
    std::list<FRAME_STRUCT>::iterator itrFrameData = m_ouFrameList[nChannel].begin();
    for ( ; itrFrameData != m_ouFrameList[nChannel].end(); itrFrameData++)
    {
        if ( itrFrameData->m_strFrameName == ouData.m_ouFlexFrame.m_strFrameName &&
                itrFrameData->m_nSlotId == ouData.m_ouFlexFrame.m_nSlotId &&
                itrFrameData->m_nBaseCycle == ouData.m_ouFlexFrame.m_nBaseCycle )
        {
            return true;
        }
    }
    return false;
}

