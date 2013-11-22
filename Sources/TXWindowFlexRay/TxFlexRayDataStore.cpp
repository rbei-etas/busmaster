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
class CTxMsgFlexChildFrame;
CBaseDIL_FLEXRAY* g_pouDIL_FLEXRAY_Interface = NULL; // FLEXRAY DIL interface
CTxFlexRayDataStore CTxFlexRayDataStore::m_sTxFlexRayDataStoreObj;

CTxFlexRayDataStore::CTxFlexRayDataStore(void)
{
    m_bTransmissionON = false;
    m_bAutoUpdate = false;
    InitializeCriticalSection(&m_MsgBlockCriticalSection);
    m_nChannelsConfigured = 0;
    m_bValidTxWndSize = false;
    m_bBusConnected = false;
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

void CTxFlexRayDataStore::vCopyMsgBlockManager(CMSGBLOCKMANAGER& mouTarget, const CMSGBLOCKMANAGER& mouSource)
{
    vRemoveAllBlock(mouTarget);

    for (int nCount = 0 ; nCount < mouSource.GetSize(); nCount++)
    {
        SMSGBLOCK* pouCurrBlock = mouSource.GetAt(nCount);
        if (NULL != pouCurrBlock)
        {
            // if (pouCurrBlock->m_bActive)
            {
                SMSGBLOCK* pouNewBlock = pouNewBlock = new SMSGBLOCK;
                if (pouNewBlock->CopyValidTXData(*pouCurrBlock))
                {
                    mouTarget.Add(pouNewBlock);
                }
                else
                {
                    delete pouNewBlock;
                    pouNewBlock = NULL;
                }
            }
        }
        else
        {
            ASSERT(FALSE);
        }
    }
}


void CTxFlexRayDataStore::vRemoveAllBlock(CMSGBLOCKMANAGER& mouCurrMsgBlockMan)
{
    SMSGBLOCK* psMsgBlock = NULL;
    int nTotalCnt = mouCurrMsgBlockMan.GetSize();
    for(int nCount = 0 ; nCount < nTotalCnt ; nCount++)
    {
        psMsgBlock = mouCurrMsgBlockMan.GetAt(nCount);
        if(psMsgBlock != NULL)
        {
            psMsgBlock->m_omFlexMsgList.RemoveAll();
            delete psMsgBlock;
        }
    }
    mouCurrMsgBlockMan.RemoveAll();
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
                    pDataStoreObj->vManageTimerExecution();
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

void CTxFlexRayDataStore::vStartTxTimer()
{
    int nCount = 0;
    sg_omMainTimerEvent.ResetEvent();

    m_sCalTimerThreadStruct.m_pTxFlexRayDataStore = this;
    m_sCalTimerThreadStruct.m_bThreadStop = FALSE;
    m_sCalTimerThreadStruct.m_omExitThreadEvent.ResetEvent();

    vResetMonoshotBlocks();
    vResetMonoshotBlocks();
    if (NULL == m_sCalTimerThreadStruct.m_pomThreadPtr)
    {
        m_sCalTimerThreadStruct.m_pomThreadPtr =
            AfxBeginThread(CalcTxTimersExecTime, &m_sCalTimerThreadStruct);
    }

    m_bTransmissionON = true;
    if (m_MMTimerId == 0)
    {
        // First find out total number of active message blocks
        for (int i = 0/*, nCount = 0*/; i < m_omTxMsgBlockMan.GetSize(); i++)
        {
            SMSGBLOCK* pouCurrBlock = m_omTxMsgBlockMan.GetAt(i);
            ASSERT(pouCurrBlock != NULL);
            if (pouCurrBlock->m_bActive)
            {
                nCount++;
            }
        }
        if (nCount > 0)
        {
            int* pnTimePeriods = new int[nCount];
            for (int i = 0, j = 0; i < m_omTxMsgBlockMan.GetSize(); i++)
            {
                SMSGBLOCK* pouCurrBlock = m_omTxMsgBlockMan.GetAt(i);
                if (pouCurrBlock != NULL)
                {
                    if (pouCurrBlock->m_bActive)
                    {
                        pnTimePeriods[j++] = pouCurrBlock->m_unTimeInterval;
                    }
                }
            }
            m_MMTimerPeriod = CalculateGCF(pnTimePeriods, nCount);
            delete[] pnTimePeriods;

            m_MMTimerId = timeSetEvent(m_MMTimerPeriod, 0,
                                       (LPTIMECALLBACK) sg_omMainTimerEvent.m_hObject, NULL,
                                       TIME_CALLBACK_EVENT_SET | TIME_PERIODIC);
        }
        else
        {
            // No message block is active. Give a feedback.
        }
    }
}


int CTxFlexRayDataStore::nGetBlockCount()
{
    int nCount = 0;
    for (int i = 0/*, nCount = 0*/; i < m_omTxMsgBlockMan.GetSize(); i++)
    {
        SMSGBLOCK* pouCurrBlock = m_omTxMsgBlockMan.GetAt(i);
        ASSERT(pouCurrBlock != NULL);
        //if (pouCurrBlock->m_bActive)      commented to avoid send tx button getting disabled
        {
            nCount++;
        }
    }
    return nCount;
}

void CTxFlexRayDataStore::vStopTxTimer()
{
    // First kill the timer
    m_bTransmissionON = false;
    if (m_MMTimerId != 0)
    {
        MMRESULT Result = timeKillEvent(m_MMTimerId);
        m_MMTimerId = 0;

        m_sCalTimerThreadStruct.m_bThreadStop = TRUE;
        sg_omMainTimerEvent.SetEvent();
        WaitForSingleObject(m_sCalTimerThreadStruct.m_omExitThreadEvent, 20);
        if (m_sCalTimerThreadStruct.m_pomThreadPtr != NULL)
        {
            TerminateThread(m_sCalTimerThreadStruct.m_pomThreadPtr->m_hThread, 0);
            m_sCalTimerThreadStruct.m_pomThreadPtr = NULL;
        }
    }

}
void CTxFlexRayDataStore::vSetBusStatus(bool bConnected)
{
    m_bBusConnected = bConnected;
    if ( bConnected == true )
    {
        list<FLEXRAY_FRAME_DATA>::iterator itrFrameData =  m_ouFlexray_Frame_Data[0].begin();
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
    if ( g_pouDIL_FLEXRAY_Interface != NULL && m_bBusConnected == TRUE)
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



/***************************************************************************************
    Function Name    :  vManageTimerExecution
    Input(s)         :
    Output           :
    Functionality    :
    Member of        :  CTransmitMsg
    Author(s)        :  Anish kumar
    Date Created     :
***************************************************************************************/
void CTxFlexRayDataStore::vManageTimerExecution()
{
    int nTotalCount = m_omTxMsgBlockMan.GetSize();
    for (int i = 0; i < nTotalCount; i++)
    {
        // There is no need to validate the pointer because this has already
        // been done, for example while calculating the required time period.
        if(m_omTxMsgBlockMan.GetSize() == 0 )
        {
            break;
        }
        EnterCriticalSection(&m_MsgBlockCriticalSection);
        SMSGBLOCK* pouCurrBlock = m_omTxMsgBlockMan.GetAt(i);
        LeaveCriticalSection(&m_MsgBlockCriticalSection);
        if ((pouCurrBlock->m_bActive)&&((pouCurrBlock->m_bCyclic)||(pouCurrBlock->m_bMonoshotSent == FALSE)))
        {
            int nTemp = 0;
            pouCurrBlock->m_unCurrTime += m_MMTimerPeriod;
            if (pouCurrBlock->m_unTimeInterval != 0)
            {
                nTemp = pouCurrBlock->m_unCurrTime % pouCurrBlock->m_unTimeInterval;
            }
            if (nTemp == 0)
            {
                pouCurrBlock->m_unCurrTime = 0;
                CMSGLIST_FLEX& CurrList = pouCurrBlock->m_omFlexMsgList;
                POSITION CurrPos = CurrList.GetHeadPosition();
                for (int j = 0; j < CurrList.GetCount(); j++)
                {
                    s_FLXTXMSG& CurrTxMsg = CurrList.GetNext(CurrPos);
                    if (CurrTxMsg.m_bToTransmit && g_pouDIL_FLEXRAY_Interface != NULL)
                    {
                        HRESULT hResult = g_pouDIL_FLEXRAY_Interface->DILF_SendFlxMsg(0,&CurrTxMsg);
                        WaitForSingleObject(sg_omMainTimerEvent, INFINITE);
                    }
                }
                //For monoshot make the block inactive
                if (pouCurrBlock->m_bCyclic != TRUE)
                {
                    pouCurrBlock->m_bMonoshotSent = TRUE;
                }
            }
        }
    }
}
/***************************************************************************************
    Function Name    :  vActivateMonoshotBlocks
    Input(s)         :
    Output           :
    Functionality    :  resets monoshot for the block
    Member of        :  CTransmitMsg
    Author(s)        :  Ashwin R Uchil
    Date Created     :  23.5.2013
***************************************************************************************/
void CTxFlexRayDataStore::vResetMonoshotBlocks()
{
    int nTotalCount = m_omTxMsgBlockMan.GetSize();
    for (int i = 0; i < nTotalCount; i++)
    {
        EnterCriticalSection(&m_MsgBlockCriticalSection);
        SMSGBLOCK* pouCurrBlock = m_omTxMsgBlockMan.GetAt(i);
        pouCurrBlock->m_bMonoshotSent = FALSE;
        LeaveCriticalSection(&m_MsgBlockCriticalSection);
    }
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
            string strVar;
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
    list<FRAME_STRUCT>::iterator itrFramList = m_ouFrameList[nChannel].begin();

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

bool bIsValidMessage(int nChannelIndex, FLEXRAY_CONFIG_DATA& ouConfigFrame)
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
    vRemoveAllBlock(m_omTxMsgBlockMan);
    //CMSGBLOCKMANAGER arrBlockManager = NULL;


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



    xmlNodePtr pNode = NULL;
    xmlNodePtr pChildNode = NULL;
    xmlNodePtr pMsgNode = NULL;
    xmlNodePtr pDataBytesNode = NULL;
    CMSGLIST_FLEX psTxFlexMsgTail = NULL;
    string   strVar;
    if( NULL != pObjectPath )
    {
        xmlNodeSetPtr pNodeSet = pObjectPath->nodesetval;
        if( NULL != pNodeSet )
        {
            pNode = pNodeSet->nodeTab[0];       //Take First One only
        }
        if( NULL != pNode )
        {
            pNode = pNode->xmlChildrenNode;
            while (pNode != NULL)           //loop through the node of "CAN_Tx_Window"
            {
                if ((!xmlStrcmp(pNode->name, (const xmlChar*)DEF_FLEXRAY_MSG_BLOCK)))               //check whether block exists
                {
                    SMSGBLOCK* psTempBlock = new SMSGBLOCK;
                    pChildNode = pNode->xmlChildrenNode;
                    int     nNoOfMess = 0;
                    while(pChildNode)       //loop through the node of "Message_Block"
                    {
                        if (xmlUtils::GetDataFrmNode(pChildNode,DEF_FLEXRAY_BLOCK_NAME,strVar))
                        {
                            psTempBlock->m_omBlockName.Format("%s",strVar.c_str());
                        }
                        if (xmlUtils::GetDataFrmNode(pChildNode,DEF_FLEXRAY_MSG_ENABLED,strVar))
                        {
                            psTempBlock->m_bActive = (bool)atoi(strVar.c_str());                //copy Is enabled
                        }
                        if (xmlUtils::GetDataFrmNode(pChildNode,DEF_FLEXRAY_MSG_IS_CYCLIC,strVar))
                        {
                            psTempBlock->m_bCyclic = (UCHAR)atoi( strVar.c_str());            //Trigger value
                        }
                        //if (xmlUtils::GetDataFrmNode(pChildNode,DEF_INDEX,strVar))
                        //{
                        //    psTempBlock->m_unIndex = (UINT)atoi(strVar.c_str());                    //Index
                        //}
                        if (xmlUtils::GetDataFrmNode(pChildNode,DEF_FLEXRAY_TIME_INTERVAL,strVar))
                        {
                            psTempBlock->m_unTimeInterval = (UINT)atoi(strVar.c_str());             //Time interval
                        }
                        //if (xmlUtils::GetDataFrmNode(pChildNode,DEF_TRANSMIT_ALL_FRAMES,strVar))
                        //{
                        //    psTempBlock->m_bTxAllFrame = (bool)atoi(strVar.c_str());                //Transmit all frames
                        //}
                        if ((!xmlStrcmp(pChildNode->name, (const xmlChar*)DEF_FLEX_MSGS)))
                        {
                            pMsgNode = pChildNode->xmlChildrenNode;
                            s_FLXTXMSG sTempFlexList;
                            //STCAN_MSG*   pStcanMsg = &psTempCanList->m_sTxMsgDetails.m_sTxMsg;
                            nNoOfMess++;
                            while(pMsgNode)
                            {

                                if (xmlUtils::GetDataFrmNode(pMsgNode,DEF_FLEX_MSG_ID,strVar))
                                {
                                    sTempFlexList.m_sFlxMsg.m_nSlotID =  (UINT)atoi(strVar.c_str());                 //Msg ID
                                }
                                if (xmlUtils::GetDataFrmNode(pMsgNode,DEF_FLEX_CYCLE_NO,strVar))             //Cycle number
                                {
                                    sTempFlexList.m_sFlxMsg.m_ucCycleNumber =  (UINT)atoi(strVar.c_str());
                                }
                                if (xmlUtils::GetDataFrmNode(pMsgNode,DEF_FLEX_DATA_LENGTH,strVar))                         //DataLength
                                {
                                    sTempFlexList.m_sFlxMsg.m_nDLC = (UCHAR)atoi(strVar.c_str());
                                }
                                if (xmlUtils::GetDataFrmNode(pMsgNode,DEF_FLEX_HEADER_INFO_FLAGS,strVar))           //header info flags
                                {
                                    sTempFlexList.m_sFlxMsg.m_lHeaderInfoFlags = (DWORD)atoi(strVar.c_str());
                                }

                                if ((!xmlStrcmp(pMsgNode->name, (const xmlChar*)DEF_FLEX_DATABYTES)))
                                {
                                    pDataBytesNode = pMsgNode->xmlChildrenNode;
                                    int iCnt =0;
                                    while(pDataBytesNode)
                                    {
                                        if (xmlUtils::GetDataFrmNode(pDataBytesNode,DEF_FLEX_BYTE,strVar))
                                        {
                                            sTempFlexList.m_sFlxMsg.m_ucData[iCnt] = (WORD)atoi(strVar.c_str());  //Data
                                            iCnt++;
                                        }
                                        pDataBytesNode = pDataBytesNode->next;
                                    }
                                }

                                if (xmlUtils::GetDataFrmNode(pMsgNode,DEF_FLEX_CHANGABLE,strVar))
                                {
                                    sTempFlexList.m_dwChangeable = (DWORD)atoi(strVar.c_str());                 //Changable
                                }

                                if (xmlUtils::GetDataFrmNode(pMsgNode,DEF_FLEX_FIRST_DYN_SLOT_ID,strVar))
                                {
                                    sTempFlexList.m_wFirstDynSlotID =   (UINT)atoi(strVar.c_str());             //First dynamic slot ID
                                }
                                if (xmlUtils::GetDataFrmNode(pMsgNode,DEF_FLEX_LAST_DYN_SLOT_ID,strVar))
                                {
                                    sTempFlexList.m_wLastDynSlotID =    (UINT)atoi(strVar.c_str());             //Last dynamic slot ID
                                }
                                if (xmlUtils::GetDataFrmNode(pMsgNode,DEF_FLEX_DYN_DATA_LEN_MAX,strVar))
                                {
                                    sTempFlexList.m_wDynDataLenMax =    (WORD)atoi(strVar.c_str());             //Dynamic data length
                                }
                                if (xmlUtils::GetDataFrmNode(pMsgNode,DEF_FLEX_MSG_INDEX,strVar))
                                {
                                    sTempFlexList.m_dwMsgIndex =    (DWORD)atoi(strVar.c_str());                //MSG Index
                                }
                                if (xmlUtils::GetDataFrmNode(pMsgNode,DEF_FLEX_TO_TRANSMIT,strVar))
                                {
                                    sTempFlexList.m_bToTransmit =   (BOOL)atoi(strVar.c_str());                 //To tramsmit
                                }

                                pMsgNode = pMsgNode->next;          //incerment messages
                            }
                            psTempBlock->m_omFlexMsgList.AddTail(sTempFlexList);
                        }
                        pChildNode = pChildNode->next;              //increment mesage block
                    }
                    m_omTxMsgBlockMan.Add(psTempBlock);
                }

                if(!xmlStrcmp(pNode->name, (const xmlChar*)DEF_WND_POS))
                {
                    WINDOWPLACEMENT WndPlacement;
                    xmlUtils::ParseWindowsPlacement(pNode,WndPlacement);
                    bSetTxData(TX_WINDOW_PLACEMENT, &WndPlacement);
                }
                //if(xmlUtils::GetDataFrmNode(pNode,DEF_AUTOSAVE,strVar))
                //{
                //    m_bAutoSavedEnabled =  (bool)atoi(strVar.c_str());                  //Channel
                //}

                //if(xmlUtils::GetDataFrmNode(pNode,DEF_BLOCKS_TIME_ENABLED,strVar))
                //{
                //    //delay between enabled
                //    if(strVar == "TRUE")
                //    {
                //        m_bDelayBetweenMsgBlocks = true;
                //    }
                //    else
                //    {
                //        m_bDelayBetweenMsgBlocks = false;
                //    }
                //    CTxMsgManager::s_bDelayBetweenBlocksOnly = m_bDelayBetweenMsgBlocks;
                //}
                //if(xmlUtils::GetDataFrmNode(pNode,DEF_BLOCKS_TIME_DELAY,strVar))
                //{
                //    m_unTimeDelayBtwnMsgBlocks =  (UINT)atoi(strVar.c_str());               //delay in milli seconds
                //    CTxMsgManager::s_unTimeDelayBtnMsgBlocks = m_unTimeDelayBtwnMsgBlocks;
                //}

                pNode = pNode->next;
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

    list<FLEXRAY_FRAME_DATA>::iterator itrFrameData = m_ouFlexray_Frame_Data[nChannel].begin();
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
            string strDatabytes;
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
            }
            else
            {
                m_sTxWndPlacement.flags = 1;
                m_sTxWndPlacement.length = 44;
                m_sTxWndPlacement.ptMaxPosition.x = 0;
                m_sTxWndPlacement.ptMaxPosition.x = 0;
                m_sTxWndPlacement.ptMinPosition.x = 0;
                m_sTxWndPlacement.ptMinPosition.y = 0;
                m_sTxWndPlacement.rcNormalPosition.top = 0;
                m_sTxWndPlacement.rcNormalPosition.bottom = 660;
                m_sTxWndPlacement.rcNormalPosition.left = 660;
                m_sTxWndPlacement.rcNormalPosition.right = 875;
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
    return bRetVal;
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

HRESULT CTxFlexRayDataStore::SetFlexRayConfig(FlexConfig& ouFlexConfig)
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
    list<FLEXRAY_FRAME_DATA>::iterator itrFrameData = m_ouFlexray_Frame_Data[nChannel].begin();
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
    list<FRAME_STRUCT>::iterator itrFrameData = m_ouFrameList[nChannel].begin();
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

