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
#include "..\DIL_Interface\BaseDIL_LIN.h"
#include "TxLINDataStore.h"
#include "Utility/Utility.h"
#include "..\include/XMLDefines.h"
#include "Utility/XMLUtils.h"
#include "..\DIL_Interface\DIL_Interface_extern.h"
#include "HashDefines.h"

static CEvent sg_omMainTimerEvent;
class CTxMsgChildFrame;
CTxLINDataStore CTxLINDataStore::m_sTxLINDataStoreObj;

CTxLINDataStore::CTxLINDataStore(void)
{
    m_bTransmissionON = false;
    m_bAutoUpdate = false;
    m_nChannelsConfigured = 1;
    m_bValidTxWndSize = false;
    m_eBusStatus = BUS_DISCONNECTED;
    m_bHexMode = false;
    m_ouClusterConfig = NULL;
    SetDefaultWindowPos();

    m_ouTransmitThread.m_pBuffer = this;
    m_ouTransmitThread.m_hActionEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    InitializeCriticalSection(&m_ouCSMsgList);
    InitializeCriticalSection(&m_ouCSKeyMap);

    m_pouDIL_LIN_Interface = NULL;
    m_ouMapIndexToKey.clear();
}

void CTxLINDataStore::SetDefaultWindowPos()
{
    /* m_sTxWndPlacement.flags = 1;
     m_sTxWndPlacement.length = 44;
     m_sTxWndPlacement.ptMaxPosition.x = 0;
     m_sTxWndPlacement.ptMaxPosition.x = 0;
     m_sTxWndPlacement.ptMinPosition.x = 0;
     m_sTxWndPlacement.ptMinPosition.y = 0;
     m_sTxWndPlacement.rcNormalPosition.top = 1;
     m_sTxWndPlacement.rcNormalPosition.bottom = 661;
     m_sTxWndPlacement.rcNormalPosition.left = 4;
     m_sTxWndPlacement.rcNormalPosition.right = 864;
     m_sTxWndPlacement.showCmd = 1;*/
}

CTxLINDataStore::~CTxLINDataStore(void)
{

}


CTxLINDataStore& CTxLINDataStore::ouGetTxLINDataStoreObj()
{
    return m_sTxLINDataStoreObj;
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
//
//UINT CalcTxTimersExecTime(LPVOID pParam)
//{
//    PSCALCEXECTIMETHREAD psThreadInfo = (PSCALCEXECTIMETHREAD) pParam;
//    if (psThreadInfo != NULL)
//    {
//        CTxLINDataStore* pDataStoreObj = psThreadInfo->m_pTxLINDataStore;
//        if (pDataStoreObj != NULL)
//        {
//            while (!psThreadInfo->m_bThreadStop)
//            {
//                // WaitForSingleObject(sg_omMainTimerEvent, INFINITE);
//                if (!psThreadInfo->m_bThreadStop)
//                {
//                    pDataStoreObj->vManageTimerExecution();
//                }
//            }
//            psThreadInfo->m_pomThreadPtr = NULL;
//            psThreadInfo->m_omExitThreadEvent.SetEvent();
//        }
//        else
//        {
//            ASSERT(FALSE);
//        }
//    }
//    else
//    {
//        ASSERT(FALSE);
//    }
//    return 0;
//}

void CTxLINDataStore::vSetBusStatus(ESTATUS_BUS eBusStatus)
{
    m_eBusStatus = eBusStatus;

    if ( eBusStatus == BUS_PRECONNECT )
    {
        nPreMessageTransmission();
    }

    else if ( eBusStatus == BUS_CONNECTED )
    {
        m_ouTransmitThread.m_unActionCode = INVOKE_FUNCTION;
        m_ouTransmitThread.bStartThread(LINTxWndTransmitThread);
        SetEvent(m_ouTransmitThread.m_hActionEvent);

        list<LIN_FRAME_DATA>::iterator itrFrameData =  m_ouLIN_Frame_Data.begin();
        while( itrFrameData != m_ouLIN_Frame_Data.end() )
        {
            if ( itrFrameData->bSelected == true )
            {
                UpdateMessagetoDIL(*itrFrameData, false);
            }
            itrFrameData++;
        }
    }
    else if( eBusStatus == BUS_DISCONNECTED )
    {
        m_ouTransmitThread.m_unActionCode = SUSPEND;
    }
}

int CTxLINDataStore::UpdateMessagetoDIL(LIN_FRAME_DATA& ouFlexData,  bool bDelete)
{
    if ( m_pouDIL_LIN_Interface != NULL && m_eBusStatus == BUS_CONNECTED)
    {
        //s_FLXTXMSG ouFlxMsg;
        //ouFlxMsg.m_sFlxMsg.m_nSlotID = ouFlexData.m_ouFlexFrame.m_nSlotId;
        //ouFlxMsg.m_sFlxMsg.m_nBaseCycle = ouFlexData.m_ouFlexFrame.m_nBaseCycle;
        //ouFlxMsg.m_sFlxMsg.m_nDLC = ouFlexData.m_ouFlexFrame.m_nLength;
        //ouFlxMsg.m_sFlxMsg.m_eChannel = ouFlexData.m_ouFlexFrame.m_ouChannel;

        //memcpy(ouFlxMsg.m_sFlxMsg.m_ucData, ouFlexData.m_ouData, ouFlexData.m_ouFlexFrame.m_nLength);

        //if ( bDelete == false )
        //{
        //    g_pouDIL_LIN_Interface->DILL_SendMsg(0, &ouFlxMsg);
        //}
        //else
        //{
        //   // g_pouDIL_LIN_Interface->DILF_DeleteFlxMsg(0, &ouFlxMsg);
        //}
    }
    return 0;
}


bool CTxLINDataStore::bSetChannelConfig(xmlNodePtr pNode)
{
    //Index
    m_ouLIN_Frame_Data.clear();
    m_ouMapIndexToKey.clear();

    xmlChar* pchPathMsg = (xmlChar*)"Message";
    xmlXPathObjectPtr pObjectPath = xmlUtils::pGetChildNodes(pNode, pchPathMsg);
    if  (pObjectPath != NULL )
    {
        xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
        if( NULL != pNodeSet )
        {
            m_ouLIN_Frame_Data.clear();
            for ( int i =0 ; i < pNodeSet->nodeNr; i++ )
            {
                xmlNodePtr pNode = pNodeSet->nodeTab[i];       //Take First One only
                LIN_FRAME_DATA ouData;
                if ( parseForMessage(pNode, ouData) == true )
                {
                    m_ouLIN_Frame_Data.push_back(ouData);
                }
            }
        }
        xmlFree(pObjectPath);
    }
    return true;
}
bool CTxLINDataStore::bAddToChannelList(LIN_FRAME_DATA& ouData)
{
    list<LIN_FRAME_DATA>::iterator itrFramList = m_ouLIN_Frame_Data.begin();

    /* for( ; itrFramList != m_ouFrameList[nChannel].end(); itrFramList++ )
     {
         if ( (itrFramList->m_strFrameName == ouData.m_strMessageName) && (itrFramList->m_nSlotId == ouData.m_nSlotId) )
         {
            LIN_FRAME_DATA ouFrameData;
             ouFrameData.m_ouFlexFrame = *itrFramList;
             ouFrameData.bSelected = ouData.m_bEnable;
             memcpy( ouFrameData.m_ouData, ouData.m_ucData, ouData.m_nDLC);
             m_ouFlexray_Frame_Data[nChannel].push_back(ouFrameData);
         }
     }*/
    return true;
}



bool CTxLINDataStore::parseForMessage(xmlNodePtr ptrNode, LIN_FRAME_DATA& ouData)
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
                ouData.m_strFrameName = (char*)key;
            }
        }

        //Channel
        pchPathMsg = (xmlChar*)DEF_CHANNEL;
        pObjectPath = xmlUtils::pGetChildNodes(ptrNode, pchPathMsg);
        if  (pObjectPath != NULL )
        {
            xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
            if( NULL != pNodeSet )
            {
                xmlNodePtr pNode = pNodeSet->nodeTab[0];       //Take First One only
                xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode , 1);
                ouData.m_ouLinMessage.m_ucChannel = atoi((char*)key);
            }
        }


        //Message ID
        pchPathMsg = (xmlChar*)DEF_MESSAGE_ID;
        pObjectPath = xmlUtils::pGetChildNodes(ptrNode, pchPathMsg);
        if  (pObjectPath != NULL )
        {
            xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
            if( NULL != pNodeSet )
            {
                xmlNodePtr pNode = pNodeSet->nodeTab[0];       //Take First One only
                xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode , 1);
                ouData.m_ouLinMessage.m_ucMsgID = atoi((char*)key);
            }
        }

        //Message Type
        pchPathMsg = (xmlChar*)DEF_MESSAGE_TYPE;
        pObjectPath = xmlUtils::pGetChildNodes(ptrNode, pchPathMsg);
        if  (pObjectPath != NULL )
        {
            xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
            if( NULL != pNodeSet )
            {
                xmlNodePtr pNode = pNodeSet->nodeTab[0];       //Take First One only
                xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode , 1);
                ouData.m_ouLinMessage.m_ucMsgTyp = atoi((char*)key);
            }
        }

        //ChecksumType
        pchPathMsg = (xmlChar*)DEF_LIN_MESSAGE_CHECKSUM;
        pObjectPath = xmlUtils::pGetChildNodes(ptrNode, pchPathMsg);
        if  (pObjectPath != NULL )
        {
            xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
            if( NULL != pNodeSet )
            {
                xmlNodePtr pNode = pNodeSet->nodeTab[0];       //Take First One only
                xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode , 1);
                ouData.m_ouLinMessage.m_ucChksumTyp = atoi((char*)key);
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
                ouData.m_ouLinMessage.m_ucDataLen = atoi((char*)key);
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
                    ouData.m_ouLinMessage.m_ucData[i] = atoi(pch);
                    pch = strtok (NULL, " ,.-");
                    i++;
                }
            }
        }

        //Repetition
        pchPathMsg = (xmlChar*)DEF_REPETION;
        pObjectPath = xmlUtils::pGetChildNodes(ptrNode, pchPathMsg);
        if  (pObjectPath != NULL )
        {
            xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
            if( NULL != pNodeSet )
            {
                xmlNodePtr pNode = pNodeSet->nodeTab[0];       //Take First One only
                xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode , 1);
                ouData.nActualTimer = atoi((char*)key);
                ouData.nCurrentTimerVal = ouData.nActualTimer;
            }
        }

        //Repetition Enabled
        pchPathMsg = (xmlChar*)DEF_REPETITION_ENABLED;
        pObjectPath = xmlUtils::pGetChildNodes(ptrNode, pchPathMsg);
        if  (pObjectPath != NULL )
        {
            xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
            if( NULL != pNodeSet )
            {
                xmlNodePtr pNode = pNodeSet->nodeTab[0];       //Take First One only
                xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode , 1);
                ouData.m_bTimerEnabled = true;
                if  ( strcmp((char*)key, "FALSE" ) == 0 )
                {
                    ouData.m_bTimerEnabled = false;
                }
            }
        }

        //Key Value
        pchPathMsg = (xmlChar*)DEF_KEY_VAL;
        pObjectPath = xmlUtils::pGetChildNodes(ptrNode, pchPathMsg);
        if  (pObjectPath != NULL )
        {
            xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
            if( NULL != pNodeSet )
            {
                xmlNodePtr pNode = pNodeSet->nodeTab[0];       //Take First One only
                xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode , 1);
                ouData.m_chKeyVal = ((char*)key)[0];
            }
        }

        //Key Value Enabled
        pchPathMsg = (xmlChar*)DEF_KEY_ENABLED;
        pObjectPath = xmlUtils::pGetChildNodes(ptrNode, pchPathMsg);
        if  (pObjectPath != NULL )
        {
            xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
            if( NULL != pNodeSet )
            {
                xmlNodePtr pNode = pNodeSet->nodeTab[0];       //Take First One only
                xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode , 1);
                ouData.bKeyEnabled = true;
                if  ( strcmp((char*)key, "FALSE" ) == 0 )
                {
                    ouData.bKeyEnabled = false;
                }
            }
        }

        //Enabled
        pchPathMsg = (xmlChar*)DEF_ENABLED;
        pObjectPath = xmlUtils::pGetChildNodes(ptrNode, pchPathMsg);
        if  (pObjectPath != NULL )
        {
            xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
            if( NULL != pNodeSet )
            {
                xmlNodePtr pNode = pNodeSet->nodeTab[0];       //Take First One only
                xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode , 1);
                ouData.bSelected = true;
                if  ( strcmp((char*)key, "FALSE" ) == 0 )
                {
                    ouData.bSelected = false;
                }
            }
        }

    }
    return true;
}


bool CTxLINDataStore::bSetConfigData(xmlDocPtr pDoc)
{
    if(pDoc == NULL)
    {
        m_sTxWndPlacement.rcNormalPosition.top = -1;
        m_sTxWndPlacement.length = 0;
        for ( int  i = 0 ; i < CHANNEL_ALLOWED; i++ )
        {
            m_ouLIN_Frame_Data.clear();
            m_ouFrameList[i].clear();
            m_ouMapIndexToKey.clear();
			m_strMessageNames[i].clear();
        }
        return false;
    }

    xmlChar* pXpath = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/LIN_Tx_Window/Window_Position";

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

    xmlChar* pXpathChannel = (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/LIN_Tx_Window/Channel_Msg_List";
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


BOOL CTxLINDataStore::bGetConfigData(xmlNodePtr pxmlNodePtr)
{
    //windows position
    xmlNodePtr pNodeWndPos = xmlNewNode(NULL, BAD_CAST DEF_WND_POS);
    xmlAddChild(pxmlNodePtr, pNodeWndPos);

    xmlUtils::CreateXMLNodeFrmWindowsPlacement(pNodeWndPos,m_sTxWndPlacement);

    //Channel Messages;
    pNodeWndPos = xmlNewNode(NULL, BAD_CAST DEF_CHANNEL_MSG_NAME);
    if ( bGetMessageListConfig( pNodeWndPos) == TRUE )
    {
        xmlAddChild(pxmlNodePtr, pNodeWndPos);
    }


    return TRUE;
}
BOOL CTxLINDataStore::bGetMessageListConfig( xmlNodePtr pNode)
{
    char pchData[1024];

    list<LIN_FRAME_DATA>::iterator itrFrameData = m_ouLIN_Frame_Data.begin();
    for (; itrFrameData != m_ouLIN_Frame_Data.end(); itrFrameData++ )
    {
        xmlNodePtr pMsgNode = xmlNewNode(NULL, BAD_CAST DEF_MESSAGE );
        if ( NULL != pMsgNode )
        {
            //Name
            xmlNewChild(pMsgNode, NULL, BAD_CAST DEF_NAME, BAD_CAST itrFrameData->m_strFrameName.c_str());

            //Channel
            sprintf(pchData, "%d", itrFrameData->m_ouLinMessage.m_ucChannel);
            xmlNewChild(pMsgNode, NULL, BAD_CAST DEF_CHANNEL, BAD_CAST pchData);

            //Message Id
            sprintf(pchData, "%d", itrFrameData->m_ouLinMessage.m_ucMsgID);
            xmlNewChild(pMsgNode, NULL, BAD_CAST DEF_MSG_ID, BAD_CAST pchData);

            //Checksum Type
            sprintf(pchData, "%d", itrFrameData->m_ouLinMessage.m_ucChksumTyp);
            xmlNewChild(pMsgNode, NULL, BAD_CAST DEF_LIN_MESSAGE_CHECKSUM, BAD_CAST pchData);

            //Message Type
            sprintf(pchData, "%d", itrFrameData->m_ouLinMessage.m_ucMsgTyp);
            xmlNewChild(pMsgNode, NULL, BAD_CAST DEF_MESSAGE_TYPE, BAD_CAST pchData);

            //DLC
            sprintf(pchData, "%d", itrFrameData->m_ouLinMessage.m_ucDataLen);
            xmlNewChild(pMsgNode, NULL, BAD_CAST DEF_DLC, BAD_CAST pchData);

            //Data Bytes
            string strDatabytes;
            sprintf(pchData, "%d", itrFrameData->m_ouLinMessage.m_ucData[0]);
            strDatabytes = pchData;
            for ( int i = 1 ; i < itrFrameData->m_ouLinMessage.m_ucDataLen; i++)
            {
                sprintf(pchData, ",%d", itrFrameData->m_ouLinMessage.m_ucData[i]);
                strDatabytes.append(pchData);
            }

            xmlNewChild(pMsgNode, NULL, BAD_CAST DEF_DATABYTES, BAD_CAST strDatabytes.c_str());

            //Repetition
            sprintf(pchData, "%d", itrFrameData->nActualTimer);
            xmlNewChild(pMsgNode, NULL, BAD_CAST DEF_REPETION, BAD_CAST pchData);

            //Repetition Enabled
            if ( itrFrameData->m_bTimerEnabled == true )
            {
                xmlNewChild(pMsgNode, NULL, BAD_CAST DEF_REPETITION_ENABLED, BAD_CAST "TRUE");
            }
            else
            {
                xmlNewChild(pMsgNode, NULL, BAD_CAST DEF_REPETITION_ENABLED, BAD_CAST "FALSE");
            }

            //Key
            sprintf(pchData, "%c", itrFrameData->m_chKeyVal);
            xmlNewChild(pMsgNode, NULL, BAD_CAST DEF_KEY_VAL, BAD_CAST pchData);

            //Key Enabled
            if ( itrFrameData->bKeyEnabled == true )
            {
                xmlNewChild(pMsgNode, NULL, BAD_CAST DEF_KEY_ENABLED, BAD_CAST "TRUE");
            }
            else
            {
                xmlNewChild(pMsgNode, NULL, BAD_CAST DEF_KEY_ENABLED, BAD_CAST "FALSE");
            }

            //Key Enabled
            if ( itrFrameData->bSelected == true )
            {
                xmlNewChild(pMsgNode, NULL, BAD_CAST DEF_IsEnabled, BAD_CAST "TRUE");
            }
            else
            {
                xmlNewChild(pMsgNode, NULL, BAD_CAST DEF_IsEnabled, BAD_CAST "FALSE");
            }
        }
        xmlAddChild(pNode, pMsgNode);
    }
    return TRUE;
}


BOOL CTxLINDataStore::bSetTxData(eTXWNDDETAILS  eParam, LPVOID lpVoid)
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
BOOL CTxLINDataStore::bGetTxData(eTXWNDDETAILS  eParam, LPVOID* lpData)
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
BOOL CTxLINDataStore::bSetDILInterfacePtr(CBaseDIL_LIN* pLinDIL)
{
    HRESULT hResult = S_OK;
    if (m_pouDIL_LIN_Interface == NULL)
    {
        hResult = DIL_GetInterface(LIN, (void**)&m_pouDIL_LIN_Interface);
    }
    return (BOOL)hResult;
}

HRESULT CTxLINDataStore::GetMessageIDNames(int nChannelIndex, map<int, string>& ouMsgIDNamesMap)
{
    if ( NULL != m_ouClusterConfig )
    {
        map<ECU_ID, ECU_Struct>::iterator itrEcu = m_ouClusterConfig->m_ouFlexChannelConfig[nChannelIndex].m_ouClusterInfo.m_ouEcuList.begin();
        while ( itrEcu != m_ouClusterConfig->m_ouFlexChannelConfig[nChannelIndex].m_ouClusterInfo.m_ouEcuList.end() )
        {
            map<UINT, list<FRAME_STRUCT>>::iterator itrFrame = itrEcu->second.m_ouTxFrames.begin();

            while ( itrFrame != itrEcu->second.m_ouTxFrames.end())
            {
                ouMsgIDNamesMap.insert(map<int, string>::value_type(itrFrame->second.begin()->m_nSlotId, itrFrame->second.begin()->m_strFrameName));
                itrFrame++;
            }

            itrFrame = itrEcu->second.m_ouRxFrames.begin();

            while ( itrFrame != itrEcu->second.m_ouRxFrames.end())
            {
                ouMsgIDNamesMap.insert(map<int, string>::value_type(itrFrame->second.begin()->m_nSlotId, itrFrame->second.begin()->m_strFrameName));
                itrFrame++;
            }

            itrEcu++;
        }
    }
    return S_OK;
}

HRESULT CTxLINDataStore::SetFlexRayConfig(ClusterConfig* ouFlexConfig)
{
    m_ouClusterConfig = ouFlexConfig;
    for ( int i = 0 ; i < m_ouClusterConfig->m_nChannelsConfigured; i++ )
    {
        m_strMessageNames[i].clear();
        GetMessageIDNames(i, m_strMessageNames[i]);
        //TODO
        //vUpdateTxList(i);
    }
    //m_ouFlexConfig = ouFlexConfig;
    m_nChannelsConfigured = m_ouClusterConfig->m_nChannelsConfigured;

    //Upddate the Tx Window List;


    return S_OK;
}

void CTxLINDataStore::vUpdateTxList(int nChannel)
{
    if ( nChannel < 0 || nChannel > CHANNEL_ALLOWED )
    {
        return;
    }
    list<LIN_FRAME_DATA>::iterator itrFrameData = m_ouLIN_Frame_Data.begin();
    while ( itrFrameData != m_ouLIN_Frame_Data.end() )
    {
        if ( bExistInTxList(nChannel, *itrFrameData) == false )
        {
            itrFrameData = m_ouLIN_Frame_Data.erase (itrFrameData);

            DIL_GetInterface(LIN, (void**)&m_pouDIL_LIN_Interface);
            if ( m_pouDIL_LIN_Interface != NULL )
            {
                //g_pouDIL_LIN_Interface->DILL_SendMsg(
            }
        }
        else
        {
            itrFrameData++;
        }
    }
}
bool CTxLINDataStore::bExistInTxList(int nChannel, LIN_FRAME_DATA& ouData)
{
    /*list<FRAME_STRUCT>::iterator itrFrameData = m_ouFrameList[nChannel].begin();
    for ( ; itrFrameData != m_ouFrameList[nChannel].end(); itrFrameData++)
    {
        if ( itrFrameData->m_strFrameName == ouData.m_ouFlexFrame.m_strFrameName &&
                itrFrameData->m_nSlotId == ouData.m_ouFlexFrame.m_nSlotId &&
                itrFrameData->m_nBaseCycle == ouData.m_ouFlexFrame.m_nBaseCycle )
        {
            return true;
        }
    }*/
    return false;
}


int CTxLINDataStore::nInsertMessage( LIN_FRAME_DATA& ouLinMsg )
{
    EnterCriticalSection(&m_ouCSMsgList);
    m_ouLIN_Frame_Data.push_back(ouLinMsg);
    LeaveCriticalSection(&m_ouCSMsgList);
    return 0;
}
int CTxLINDataStore::nHandleSelectionState(int nIndex, bool bEnable)
{
    if ( bEnable == true )
    {
        return nTransmitMsgAt(nIndex);
    }
    else
    {
        return nStopTransmissionAt(nIndex);
    }
}

int CTxLINDataStore::nInsertKey(int nIndex, char chKey)
{
    EnterCriticalSection(&m_ouCSKeyMap);
    m_ouMapIndexToKey[nIndex] = chKey;
    LeaveCriticalSection(&m_ouCSKeyMap);

    return 0;
}

int CTxLINDataStore::nDeleteKey(int nIndex)
{
    EnterCriticalSection(&m_ouCSKeyMap);
    m_ouMapIndexToKey.erase(nIndex);
    LeaveCriticalSection(&m_ouCSKeyMap);

    return 0;
}

int CTxLINDataStore::nDeleteAllKeyEntrs()
{
    EnterCriticalSection(&m_ouCSKeyMap);
    m_ouMapIndexToKey.clear();
    LeaveCriticalSection(&m_ouCSKeyMap);

    return 0;
}

int CTxLINDataStore::nDeleteMessageAt( int nIndex )
{
    EnterCriticalSection(&m_ouCSMsgList);
    list<LIN_FRAME_DATA>::iterator itrList = m_ouLIN_Frame_Data.begin();
    std::advance(itrList, nIndex);
    m_ouLIN_Frame_Data.erase(itrList);
    LeaveCriticalSection(&m_ouCSMsgList);
    return 0;

}

int CTxLINDataStore::nDeleteAllMessages( )
{
    EnterCriticalSection(&m_ouCSMsgList);
    m_ouLIN_Frame_Data.clear();
    LeaveCriticalSection(&m_ouCSMsgList);
    return 0;

}
int CTxLINDataStore::nPreMessageTransmission()
{
    EnterCriticalSection(&m_ouCSMsgList);
    list<LIN_FRAME_DATA>::iterator itrLinData = m_ouLIN_Frame_Data.begin();
    while ( itrLinData != m_ouLIN_Frame_Data.end() )
    {
        //Transmit Messages
        //if ( itrLinData->bSelected == true )
        {
            //itrLinData->nCurrentTimerVal--;
            //if ( itrLinData->nCurrentTimerVal <= 0 )
            {
                //TODO::Send
                m_pouDIL_LIN_Interface->DILL_SendMsg(m_dwClientID, itrLinData->m_ouLinMessage);
                if ( itrLinData->bSelected != true )
                {
                    m_pouDIL_LIN_Interface->DILL_DisableSlaveRespData(m_dwClientID, itrLinData->m_ouLinMessage);
                }

                //itrLinData->nCurrentTimerVal =  itrLinData->nActualTimer;
            }
        }
        itrLinData++;
    }
    LeaveCriticalSection(&m_ouCSMsgList);
    return 0;
}
int CTxLINDataStore::nStopTransmissionAt(int nIndex)
{
    HRESULT hResult = S_FALSE;
    if (  m_pouDIL_LIN_Interface == NULL )
    {
        return hResult;
    }
    EnterCriticalSection(&m_ouCSMsgList);
    LIN_FRAME_DATA ouLinFrmData;
    if((ouGetLinFrame(nIndex, ouLinFrmData) == S_OK) && (ouLinFrmData.bSelected == false))
    {
        m_pouDIL_LIN_Interface->DILL_DisableSlaveRespData(m_dwClientID, ouLinFrmData.m_ouLinMessage);
        hResult = S_OK;
    }
    LeaveCriticalSection(&m_ouCSMsgList);
    return hResult;
}

int CTxLINDataStore::nTransmitMsgAt(int nIndex)
{
    HRESULT hResult = S_FALSE;
    if (  m_pouDIL_LIN_Interface == NULL )
    {
        return hResult;
    }
    EnterCriticalSection(&m_ouCSMsgList);
    LIN_FRAME_DATA ouLinFrmData;
    if((ouGetLinFrame(nIndex, ouLinFrmData) == S_OK) && (ouLinFrmData.bSelected == true))
    {
        m_pouDIL_LIN_Interface->DILL_SendMsg(m_dwClientID, ouLinFrmData.m_ouLinMessage);
        hResult = S_OK;
    }
    LeaveCriticalSection(&m_ouCSMsgList);
    return hResult;
}

int CTxLINDataStore::nHandleKeyEvent(char chKey)
{
    HRESULT hResult = S_FALSE;
    if (  m_pouDIL_LIN_Interface == NULL )
    {
        return hResult;
    }

    EnterCriticalSection(&m_ouCSKeyMap);

    // If connected to hardware
    if(m_eBusStatus == BUS_CONNECTED)
    {
        map<int, char>::iterator itrMap =  m_ouMapIndexToKey.begin();

        while(itrMap != m_ouMapIndexToKey.end())
        {
            if(itrMap->second == chKey)
            {
                nTransmitMsgAt(itrMap->first);
            }

            itrMap++;
        }
    }
    LeaveCriticalSection(&m_ouCSKeyMap);

    return hResult;
}


INT CTxLINDataStore::nTransmitMessages()
{
    if ( m_pouDIL_LIN_Interface == NULL )
    {
        return 0;
    }

    EnterCriticalSection(&m_ouCSMsgList);
    list<LIN_FRAME_DATA>::iterator itrLinData = m_ouLIN_Frame_Data.begin();
    while ( itrLinData != m_ouLIN_Frame_Data.end() )
    {
        //Transmit Messages
        if ( itrLinData->bSelected == true && itrLinData->m_bTimerEnabled == true )
        {
            itrLinData->nCurrentTimerVal--;
            if ( itrLinData->nCurrentTimerVal <= 0 )
            {
                //TODO::Send
                m_pouDIL_LIN_Interface->DILL_SendMsg(m_dwClientID, itrLinData->m_ouLinMessage);
                itrLinData->nCurrentTimerVal =  itrLinData->nActualTimer;
            }
        }
        itrLinData++;
    }
    LeaveCriticalSection(&m_ouCSMsgList);

    return 0;
}

int CTxLINDataStore::ouGetLinFrame(int nIndex,  LIN_FRAME_DATA& ouLinFramData)
{
    HRESULT hResult = S_FALSE;
    list<LIN_FRAME_DATA>::iterator itrLinData;
    EnterCriticalSection(&m_ouCSMsgList);
    itrLinData =m_ouLIN_Frame_Data.begin();
    std::advance(itrLinData, nIndex);

    if(itrLinData != m_ouLIN_Frame_Data.end())
    {
        hResult = S_OK;
        ouLinFramData = *itrLinData;
    }
    LeaveCriticalSection(&m_ouCSMsgList);

    return hResult;
}

DWORD WINAPI CTxLINDataStore::LINTxWndTransmitThread(LPVOID pVoid)
{
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;
    if (pThreadParam == NULL)
    {
        return (DWORD)-1;
    }
    CTxLINDataStore* pouData = (CTxLINDataStore*)pThreadParam->m_pBuffer;

    if (pouData == NULL)
    {
        return (DWORD)-1;
    }

    bool bLoopON = true;

    while (bLoopON)
    {
        WaitForSingleObject(pThreadParam->m_hActionEvent, 1);
        switch (pThreadParam->m_unActionCode)
        {
            case INVOKE_FUNCTION:
            {
                pouData->nTransmitMessages();
            }
            break;

            case SUSPEND:
            {
                WaitForSingleObject(pThreadParam->m_hActionEvent, INFINITE);
            }
            break;

            case EXIT_THREAD:
            {
                bLoopON = false;
            }
            break;

            default:
                break;
        }
    }
    SetEvent(pThreadParam->hGetExitNotifyEvent());
    return 0;
}