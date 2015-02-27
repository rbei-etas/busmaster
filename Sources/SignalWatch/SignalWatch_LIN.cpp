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
* \file      SignalWatch_LIN.cpp
* \author    Shashank Vernekar
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*/
#include "SignalWatch_stdafx.h"
#include "Include/Utils_macro.h"
#include "SignalWatch_LIN.h"
#include "Utility/MsgInterpretation.h"
#include "include/XMLDefines.h"
#include "Utility/XMLUtils.h"


int ReadLINDataBuffer(CSignalWatch_LIN* pSWLin)
{
    ASSERT(pSWLin != nullptr);
    while (pSWLin->m_ouLinBufFSE.GetMsgCount() > 0)
    {
        static STLINDATA sLinData;
        INT Result = pSWLin->m_ouLinBufFSE.ReadFromBuffer(&sLinData);
        if (Result == ERR_READ_MEMORY_SHORT)
        {
            TRACE("ERR_READ_MEMORY_SHORT");
        }
        else if (Result == EMPTY_APP_BUFFER)
        {
            TRACE("EMPTY_APP_BUFFER");
        }
        else
        {
            pSWLin->vDisplayInSigWatchWnd(sLinData);
        }

    }
    return 0;
}

DWORD WINAPI SigWatchDataReadThreadProc_L(LPVOID pVoid)
{
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;
    if (pThreadParam == nullptr)
    {
        return (DWORD)-1;
    }
    CSignalWatch_LIN* pSWLin = (CSignalWatch_LIN*)pThreadParam->m_pBuffer;
    if (pSWLin == nullptr)
    {
        return (DWORD)-1;
    }
    bool bLoopON = true;
    pThreadParam->m_unActionCode = INVOKE_FUNCTION;
    while (bLoopON)
    {
        WaitForSingleObject(pThreadParam->m_hActionEvent, INFINITE);
        switch (pThreadParam->m_unActionCode)
        {
            case INVOKE_FUNCTION:
            {
                ReadLINDataBuffer(pSWLin); // Retrieve message from the driver
            }
            break;
            case EXIT_THREAD:
            {
                bLoopON = false;
            }
            break;
            case CREATE_TIME_MAP:
            {
                //Nothing at this moment
            }
            break;
            default:
            case INACTION:
            {
                // nothing right at this moment
            }
            break;
        }
    }
    SetEvent(pThreadParam->hGetExitNotifyEvent());
    return 0;
}

BOOL CSignalWatch_LIN::InitInstance(void)
{
    InitializeCriticalSection(&m_omCritSecSW);
    m_pouSigWnd = nullptr;
    m_pMsgInterPretObj = nullptr;
    m_ouReadThread.m_hActionEvent = m_ouLinBufFSE.hGetNotifyingEvent();
    return TRUE;
}

int CSignalWatch_LIN::ExitInstance(void)
{
    m_ouReadThread.bTerminateThread(); // Terminate read thread
    m_ouLinBufFSE.vClearMessageBuffer();//clear can buffer

    if (m_pMsgInterPretObj != nullptr) // clear interpretation object
    {
        // m_pMsgInterPretObj->vClear();
        delete m_pMsgInterPretObj;
        m_pMsgInterPretObj = nullptr;
    }

    DeleteCriticalSection(&m_omCritSecSW); //delete critical section

    if (m_pouSigWnd != nullptr)
    {
        m_pouSigWnd->DestroyWindow();
        delete m_pouSigWnd;
        m_pouSigWnd = nullptr;
    }
    return TRUE;
}

void  CSignalWatch_LIN::vGetWindowPosition()
{
    //CMsgSignalSelect odDlg;
    //WndPlace=odDlg.wpPosition();

}
BOOL CSignalWatch_LIN::bStartSigWatchReadThread()
{
    m_ouReadThread.m_pBuffer = this;
    m_ouReadThread.bStartThread(SigWatchDataReadThreadProc_L);
    return TRUE;
}

void CSignalWatch_LIN::vDisplayInSigWatchWnd(STLINDATA& sLinData)
{
    EnterCriticalSection(&m_omCritSecSW);
    if (m_pMsgInterPretObj != nullptr)
    {


        m_pMsgInterPretObj->vSetLINClusterInfo(cluster);
        //STLINDATA * sLinData;
        static CString omMsgName;
        static CStringArray omSigNames, omRawValues, omPhyValues;
        /*if (m_pMsgInterPretObj->bInterpretMsgSigList(sLinData.m_uDataInfo.m_sLINMsg.m_ucMsgID,
        sLinData.m_uDataInfo.m_sLINMsg.m_ucData,omMsgName, omSigNames, omRawValues, omPhyValues, m_bHex))*/
        SSignalInfoArray sSingnalinfo;
        std::string msgName;

        int mID=m_mapMsgIDtoSignallst->find(sLinData.m_uDataInfo.m_sLINMsg.m_ucMsgID)->first;

        if(mID==sLinData.m_uDataInfo.m_sLINMsg.m_ucMsgID)
        {
            if (m_pMsgInterPretObj->bInterpretMsgs(DEC,&(sLinData.m_uDataInfo.m_sLINMsg),sSingnalinfo))

            {

                if ((m_pouSigWnd != nullptr) && (m_pouSigWnd->IsWindowVisible()) && (sLinData.m_eLinMsgType != LIN_EVENT))
                {

                    m_pouSigWnd->vAddMsgToWnd(&sSingnalinfo,FALSE,m_mapMsgIDtoSignallst,mID);

                }

            }
        }
    }

    LeaveCriticalSection(&m_omCritSecSW);

    //delete the invalid entries
    //vDeleteRemovedListEntries();
}

void CSignalWatch_LIN::vDeleteRemovedListEntries()
{
    if ((m_pMsgInterPretObj != nullptr) && (m_pouSigWnd != nullptr))
    {
        CStringArray strMsgList;
        int inSize = 0;

        //get the number of signals
        int iCount = (int)m_pouSigWnd->m_omSignalList.GetItemCount();

        for(int iIndex = 0; iIndex < iCount ; iIndex++ )
        {
            CString strMsgName = m_pouSigWnd->m_omSignalList.GetItemText(iIndex, 0);
            inSize = (int)strMsgList.GetSize();
            bool bFound = false;

            //check for the unique message name
            for(int inPos = 0; inPos < inSize ; inPos++ )
            {
                if(strMsgName.CompareNoCase(strMsgList.GetAt(inPos)) == 0 )
                {
                    bFound = true;
                    break;
                }
            }

            //add the message in list
            if(bFound == false)
            {
                strMsgList.Add(strMsgName);
            }
        }

        //get the message count
        inSize = (int)strMsgList.GetSize();

        for(int inPos = 0; inPos < inSize ; inPos++ )
        {
            CString strMsgName = strMsgList.GetAt(inPos);
            int iSignalCount = m_pMsgInterPretObj->nGetSignalCount(strMsgName);
            if(iSignalCount == 0 ) //no signal is configured
            {
                if((int)m_pouSigWnd->m_omSignalList.GetItemCount() > 0  )
                {
                    //remove the signal from signal watch window.
                    m_pouSigWnd->SendMessage( WM_REMOVE_SIGNAL, 0, (LPARAM) &strMsgName);
                }
            }
        }
    }
}

HRESULT CSignalWatch_LIN::SW_DoInitialization(void* RefObj, void* ouCluster)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    cluster= (ClusterConfig*)ouCluster;
    SW_UpdateMsgInterpretObj(RefObj);

    //Create the signal watch window for CAN
    if (m_pouSigWnd == nullptr)
    {
        m_pouSigWnd = new CSigWatchDlg(AfxGetMainWnd(), LIN);
        m_pouSigWnd->Create(IDD_DLG_SIGNAL_WATCH, nullptr);
        m_pouSigWnd->SetWindowText("Signal Watch - LIN");
    }

    CBaseDIL_LIN* pouDIL_LIN;
    if (DIL_GetInterface(LIN, (void**)&pouDIL_LIN) == S_OK)
    {
        DWORD dwClientId = 0;
        pouDIL_LIN->DILL_RegisterClient(TRUE, dwClientId, LIN_MONITOR_NODE);
        pouDIL_LIN->DILL_ManageMsgBuf(MSGBUF_ADD, dwClientId, &(m_ouLinBufFSE));
    }
    //Start the read thread
    return bStartSigWatchReadThread()? S_OK: S_FALSE;
}

/**
* \req RS_18_01 Signal watch configuration dialog box
*
* Signal watch configuration dialog box
* lists frames / messages from the loaded database (combo box / list box).
*/
HRESULT CSignalWatch_LIN::SW_ShowAddDelSignalsDlg(CWnd* pParent, void* m_ouCluster)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    //cluster[LIN].m_nChannelsConfigured;
    CMsgSignalSelect odDlg(cluster, pParent, m_mapMsgIDtoSignallst);

    //WndPlace=odDlg.wpPosition();

    int nRet = odDlg.DoModal();
    return (HRESULT)nRet;

}

HRESULT CSignalWatch_LIN::SW_ShowSigWatchWnd(CWnd* /*pParent*/, HWND hMainWnd, INT nCmd)
{
    if (m_pouSigWnd != nullptr)
    {
        m_pouSigWnd->vUpdateMainWndHandle(hMainWnd);
        m_pouSigWnd->SetParent(CWnd::FromHandle(hMainWnd));
        return m_pouSigWnd->ShowWindow(nCmd);
    }
    return S_FALSE;
}

HRESULT CSignalWatch_LIN::SW_GetConfigSize(void)
{
    return (sizeof (WINDOWPLACEMENT) + sizeof(UINT) * defSW_LIST_COLUMN_COUNT);
}

HRESULT CSignalWatch_LIN::SW_GetConfigData(void* pbyConfigData)
{
    WINDOWPLACEMENT WndPlace;
    UINT nDebugSize  = 0;
    BYTE* pbyTemp = (BYTE*)pbyConfigData;
    if ((m_pouSigWnd != nullptr) && (pbyTemp != nullptr))
    {
        m_pouSigWnd->GetWindowPlacement(&WndPlace);
        COPY_DATA(pbyTemp, &WndPlace, sizeof (WINDOWPLACEMENT));
        for (UINT i = 0; i < defSW_LIST_COLUMN_COUNT; i++)
        {
            INT nWidth = m_pouSigWnd->m_omSignalList.GetColumnWidth(i);
            COPY_DATA(pbyTemp, &nWidth, sizeof (INT));
        }
    }
    return S_OK;
}

HRESULT CSignalWatch_LIN::SW_GetConfigData(xmlNodePtr pNodePtr)
{
    std::list<FRAME_STRUCT> lstNumofFrames;
    if(nullptr != cluster)
    {
        cluster->m_ouFlexChannelConfig[0].m_ouClusterInfo.GetFrames(lstNumofFrames);
        for(int i=0; i<cluster->m_nChannelsConfigured; i++)
        {
            std::list<FRAME_STRUCT>::iterator itr= lstNumofFrames.begin();
            while(itr != lstNumofFrames.end())
            {
                int selkey = -1;
                if ( m_mapMsgIDtoSignallst->find(itr->m_nSlotId) != m_mapMsgIDtoSignallst->end())
                {
                    selkey=m_mapMsgIDtoSignallst->find(itr->m_nSlotId)->first;
                }
                CString strMessageId = "";
                if(selkey== itr->m_nSlotId)
                {
                    xmlNodePtr pMsgTagPtr = xmlNewNode(nullptr, BAD_CAST DEF_MESSAGE);
                    xmlAddChild(pNodePtr, pMsgTagPtr);
                    strMessageId.Format("%d",selkey );

                    /* Generating Message names */
                    xmlNodePtr pMsgPtr = xmlNewChild(pMsgTagPtr, nullptr, BAD_CAST DEF_MSGID
                                                     , BAD_CAST strMessageId.GetBufferSetLength(strMessageId.GetLength()));
                    xmlAddChild(pMsgTagPtr, pMsgPtr);
                    CString strSignalName = "";
                    if(selkey == itr->m_nSlotId)
                    {
                        std::list<std::string> lstSignals=m_mapMsgIDtoSignallst->find(itr->m_nSlotId)->second;
                        std::list<std::string>::iterator itrselSignals= lstSignals.begin();

                        while(itrselSignals != lstSignals.end())
                        {
                            strSignalName=(*itrselSignals).c_str();
                            // strSignalName.Format("%s", *itrselSignals);

                            xmlNodePtr pSignalPtr = xmlNewChild(pMsgTagPtr, nullptr, BAD_CAST DEF_SIGNAL
                                                                , BAD_CAST strSignalName.GetBufferSetLength(strSignalName.GetLength()));
                            xmlAddChild(pMsgTagPtr, pSignalPtr);
                            itrselSignals++;
                        }
                    }
                }
                itr++;
            }
        }

    }
    // Setting signal watch window placement and column width
    xmlNodePtr pWndPositn = xmlNewNode(nullptr, BAD_CAST DEF_WINDOW_POSITION);
    xmlAddChild(pNodePtr, pWndPositn);
    WINDOWPLACEMENT WndPlace;
    UINT nDebugSize  = 0;
    //BYTE* pbyTemp = (BYTE*)pbyConfigData;
    if ((m_pouSigWnd != nullptr))
    {
        m_pouSigWnd->GetWindowPlacement(&WndPlace);
        //COPY_DATA(pbyTemp, &WndPlace, sizeof (WINDOWPLACEMENT));

        BOOL bIsWndwVisible = m_pouSigWnd->IsWindowVisible();

        CString strWindwVisibility = "";

        strWindwVisibility = xmlUtils::nSetWindowVisibility(WndPlace.showCmd);

        // Writing visibility in to xml
        xmlNodePtr pVisibility = xmlNewChild(pWndPositn, nullptr, BAD_CAST DEF_VISIBILITY, BAD_CAST strWindwVisibility.GetBuffer(strWindwVisibility.GetLength()));
        xmlAddChild(pWndPositn, pVisibility);

        UINT nFlag = WndPlace.flags;

        CString strWindPlcmnt = "";

        strWindPlcmnt = xmlUtils::nSetWindowVisibility(nFlag);

        // Setting window Placement
        xmlNodePtr pWndwVisibility = xmlNewChild(pWndPositn, nullptr, BAD_CAST DEF_WINDOW_PLACEMENT, BAD_CAST strWindPlcmnt.GetBuffer(strWindPlcmnt.GetLength()));
        xmlAddChild(pWndPositn, pWndwVisibility);

        CString strBottomPos = "", strLeftPos = "", strTopPos = "", strRightPos = "";

        strTopPos.Format("%d", WndPlace.rcNormalPosition.top);
        strLeftPos.Format("%d", WndPlace.rcNormalPosition.left);
        strRightPos.Format("%d", WndPlace.rcNormalPosition.right);
        strBottomPos.Format("%d", WndPlace.rcNormalPosition.bottom);

        // Writing co-ordinates of the window in to XML file
        xmlNodePtr pTopPos = xmlNewChild(pWndPositn, nullptr, BAD_CAST DEF_TOP, BAD_CAST strTopPos.GetBuffer(strTopPos.GetLength()));
        xmlAddChild(pWndPositn, pTopPos);

        xmlNodePtr pLeftPos = xmlNewChild(pWndPositn, nullptr, BAD_CAST DEF_Left,BAD_CAST strLeftPos.GetBuffer(strLeftPos.GetLength()));
        xmlAddChild(pWndPositn, pLeftPos);

        xmlNodePtr pRightPos = xmlNewChild(pWndPositn, nullptr, BAD_CAST DEF_Right,BAD_CAST strRightPos.GetBuffer(strRightPos.GetLength()));
        xmlAddChild(pWndPositn, pRightPos);

        xmlNodePtr pBottomPos = xmlNewChild(pWndPositn, nullptr, BAD_CAST DEF_Bottom,BAD_CAST strBottomPos.GetBuffer(strBottomPos.GetLength()));
        xmlAddChild(pWndPositn, pBottomPos);

        // Setting Column width in to the xml file
        xmlNodePtr pColWidthPtr = xmlNewNode(nullptr, BAD_CAST DEF_Columns_Width);
        xmlAddChild(pNodePtr, pColWidthPtr);

        for (UINT i = 0; i < defSW_LIST_COLUMN_COUNT; i++)
        {
            INT nWidth = m_pouSigWnd->m_omSignalList.GetColumnWidth(i);
            CString strWidth = "";
            strWidth.Format("%d", nWidth);

            if(i == 0)
            {
                // Writing message column value in to xml
                xmlNodePtr pMsgClmnPtr = xmlNewChild(pColWidthPtr, nullptr, BAD_CAST DEF_Message_Column, BAD_CAST strWidth.GetBuffer(strWidth.GetLength()));
                xmlAddChild(pColWidthPtr, pMsgClmnPtr);
            }
            if(i == 1)
            {
                // Writing Raw value column value in to xml
                xmlNodePtr pRawValClmn = xmlNewChild(pColWidthPtr, nullptr, BAD_CAST DEF_Raw_Val_Column, BAD_CAST strWidth.GetBuffer(strWidth.GetLength()));
                xmlAddChild(pColWidthPtr, pRawValClmn);
            }
            if(i == 2)
            {
                // Writing Raw value column value in to xml
                xmlNodePtr pPhyalClmn = xmlNewChild(pColWidthPtr, nullptr, BAD_CAST DEF_Physical_Val_Column, BAD_CAST strWidth.GetBuffer(strWidth.GetLength()));
                xmlAddChild(pColWidthPtr, pPhyalClmn);
            }
            if(i == 3)
            {
                // Writing Raw value column value in to xml
                xmlNodePtr pSigValClmn = xmlNewChild(pColWidthPtr, nullptr, BAD_CAST DEF_Signal_Column, BAD_CAST strWidth.GetBuffer(strWidth.GetLength()));
                xmlAddChild(pColWidthPtr, pSigValClmn);
            }
        }


    }
    return S_OK;
}

HRESULT CSignalWatch_LIN::SW_SetConfigData(const void* pbyConfigData)
{
    const BYTE* pbyTemp = (BYTE*)pbyConfigData;

    if ((pbyConfigData != nullptr) && (m_pouSigWnd != nullptr))
    {
        WINDOWPLACEMENT WndPlace;
        memcpy(&WndPlace, pbyConfigData, sizeof (WINDOWPLACEMENT));
        m_pouSigWnd->MoveWindow(&(WndPlace.rcNormalPosition), FALSE);
        pbyTemp += sizeof (WINDOWPLACEMENT);
        for (UINT i = 0; i < defSW_LIST_COLUMN_COUNT; i++)
        {
            m_pouSigWnd->m_omSignalList.SetColumnWidth(i, pbyTemp[i]);
        }
    }
    if(m_pouSigWnd != nullptr)
    {
        //Signal watch window will move the List control in OnSize().
        //So the default values should be as followes.
        for (UINT i = 0; i < defSW_LIST_COLUMN_COUNT; i++)
        {
            RECT sClientRect;
            m_pouSigWnd->GetClientRect(&sClientRect);
            int ClientWidth = abs(sClientRect.left - sClientRect.right);
            m_pouSigWnd->m_omSignalList.SetColumnWidth(0, (int)(0.2 * ClientWidth));
            m_pouSigWnd->m_omSignalList.SetColumnWidth(1, (int)(0.2 * ClientWidth));
            m_pouSigWnd->m_omSignalList.SetColumnWidth(2, (int)(0.4 * ClientWidth));
            m_pouSigWnd->m_omSignalList.SetColumnWidth(3, (int)(0.2 * ClientWidth));
            m_pouSigWnd->m_omSignalList.MoveWindow(&sClientRect);
        }

        if(pbyConfigData == nullptr)
        {
            WINDOWPLACEMENT WndPlace;
            m_mapMsgIDtoSignallst->clear();
            //     // memcpy(&WndPlace, pbyConfigData, sizeof (WINDOWPLACEMENT));
            //      m_pouSigWnd->MoveWindow(&(WndPlace.rcNormalPosition), FALSE);
            //Signal watch window will move the List control in OnSize().
            //So the default values should be as followes.

            WndPlace.rcNormalPosition.top=70;
            WndPlace.rcNormalPosition.left=10;
            WndPlace.rcNormalPosition.right=500;
            WndPlace.rcNormalPosition.bottom=300;
            m_pouSigWnd->MoveWindow(&(WndPlace.rcNormalPosition), TRUE);

            m_pouSigWnd->ShowWindow(SW_HIDE);
        }
    }
    return S_OK;
}

HRESULT CSignalWatch_LIN::SW_SetConfigData(xmlNodePtr pNode)
{
    m_pouSigWnd->ShowWindow(SW_HIDE);
    INT nRetValue  = S_OK;
    if ((pNode != nullptr) && (m_pouSigWnd != nullptr))
    {
        WINDOWPLACEMENT WndPlace;
        while(pNode != nullptr)
        {

            if((!xmlStrcmp(pNode->name, (const xmlChar*)"Message")))
            {
                int id;
                std::list<std::string> signame;

                xmlNodePtr child= pNode->children;
                while(child != nullptr)
                {
                    if((!xmlStrcmp(child->name, (const xmlChar*)"Id")))
                    {

                        xmlChar* key = xmlNodeListGetString(child->doc, child->xmlChildrenNode, 1);
                        if(nullptr != key)
                        {
                            id = atoi((char*)key);
                        }

                    }

                    if((!xmlStrcmp(child->name, (const xmlChar*)"Signal")))
                    {
                        xmlChar* key = xmlNodeListGetString(child->doc, child->xmlChildrenNode, 1);
                        if(nullptr != key)
                        {
                            signame.push_back((char*)key);
                        }

                    }

                    child = child->next;
                }

                m_mapMsgIDtoSignallst[0].insert(std::map<int, std::list<std::string>>::value_type(id,signame));
            }


            if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Window_Position")))
            {
                nRetValue = xmlUtils::ParseWindowsPlacement(pNode, WndPlace);
                if(nRetValue == S_OK)
                {
                    m_pouSigWnd->MoveWindow(&(WndPlace.rcNormalPosition), FALSE);
                }
                break;
            }

            /*  if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Activate_Window_Position")))
            {
            nRetValue = xmlUtils::ParseWindowsPlacement(pNode, WndPlace);
            if(nRetValue == S_OK)
            {
            m_pouSigWnd->MoveWindow(&(WndPlace.rcNormalPosition), FALSE);
            }
            break;
            }*/


            pNode = pNode->next;
        }
    }
    if(m_pouSigWnd != S_OK)
    {
        //Signal watch window will move the List control in OnSize().
        //So the default values should be as followes.
        for (UINT i = 0; i < defSW_LIST_COLUMN_COUNT; i++)
        {
            RECT sClientRect;
            m_pouSigWnd->GetClientRect(&sClientRect);
            int ClientWidth = abs(sClientRect.left - sClientRect.right);
            m_pouSigWnd->m_omSignalList.SetColumnWidth(0, (int)(0.2 * ClientWidth));
            m_pouSigWnd->m_omSignalList.SetColumnWidth(1, (int)(0.2 * ClientWidth));
            m_pouSigWnd->m_omSignalList.SetColumnWidth(2, (int)(0.4 * ClientWidth));
            m_pouSigWnd->m_omSignalList.SetColumnWidth(3, (int)(0.2 * ClientWidth));
            m_pouSigWnd->m_omSignalList.MoveWindow(&sClientRect);
        }
    }
    return S_OK;
}
INT CSignalWatch_LIN::nParseXMLColumn(xmlNodePtr pNode)
{
    INT nRetVal = S_OK;
    pNode = pNode->children;
    while(pNode != nullptr)
    {
        if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Message_Column")) == FALSE)
        {
            xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
            if(nullptr != key)
            {
                m_pouSigWnd->m_omSignalList.SetColumnWidth(0, atoi((char*)key));
                xmlFree(key);
            }
        }

        if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Raw_Val_Column")) == FALSE)
        {
            xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
            if(nullptr != key)
            {
                m_pouSigWnd->m_omSignalList.SetColumnWidth(1, atoi((char*)key));
                xmlFree(key);
            }
        }

        if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Physical_Val_Column")) == FALSE)
        {
            xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
            if(nullptr != key)
            {
                m_pouSigWnd->m_omSignalList.SetColumnWidth(2, atoi((char*)key));
                xmlFree(key);
            }
        }

        if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Signal_Column")) == FALSE)
        {
            xmlChar* key = xmlNodeListGetString(pNode->doc, pNode->xmlChildrenNode, 1);
            if(nullptr != key)
            {
                m_pouSigWnd->m_omSignalList.SetColumnWidth(3, atoi((char*)key));
                xmlFree(key);
            }
        }
        pNode = pNode->next;
    }
    return nRetVal;
}

/**
* \req RS_18_23 Popup menu item 'Clear' (clears the signal watch window)
*/
HRESULT CSignalWatch_LIN::SW_ClearSigWatchWnd(void)
{
    if (m_pouSigWnd != nullptr)
    {
        //m_pouSigWnd->PostMessage(WM_REMOVE_SIGNAL);

        //send the message without any extra value in LPARAM
        m_pouSigWnd->PostMessage(WM_REMOVE_SIGNAL,0,0);
    }
    return S_OK;
}

HRESULT CSignalWatch_LIN::SW_UpdateMsgInterpretObj(void* pvRefObj)
{
    CMsgInterpretationLIN* RefObj = (CMsgInterpretationLIN*) pvRefObj;
    EnterCriticalSection(&m_omCritSecSW);
    if (m_pMsgInterPretObj == nullptr)
    {
        m_pMsgInterPretObj = new CMsgInterpretationLIN;
    }
    RefObj->vCopy(m_pMsgInterPretObj);
    LeaveCriticalSection(&m_omCritSecSW);
    return S_OK;
}

BOOL CSignalWatch_LIN::SW_IsWindowVisible()
{
    BOOL bResult = FALSE;
    if (m_pouSigWnd != nullptr)
    {
        bResult = m_pouSigWnd->IsWindowVisible();
    }
    return bResult;
}

HRESULT CSignalWatch_LIN::SW_SetDisplayMode(BOOL bHex)
{
    m_bHex = bHex;
    return S_OK;
}