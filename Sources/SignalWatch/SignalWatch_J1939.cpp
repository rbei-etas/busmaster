/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  SignalWatch_J1939.cpp
  Description   :
  $Log:   X:/Archive/Sources/SignalWatch/SignalWatch_J1939.cpv  $

  Author(s)     :  Pradeep Kadoor
  Date Created  :  16/02/2011
  Modified By   :
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/
#include "SignalWatch_stdafx.h"
#include "Include/Utils_macro.h"
#include "Include/DIL_CommonDefs.h"
#include "SignalWatch_J1939.h"
#include "include/XMLDefines.h"

int ReadJ1939DataBuffer(CSignalWatch_J1939* pSWJ1939)
{
    VALIDATE_POINTER_RETURN_VAL(pSWJ1939, -1);
    while (pSWJ1939->m_ouMsgBufVSE_J.GetMsgCount() > 0)
    {
        static STJ1939_MSG sMsg;
        static BYTE abyData[MAX_MSG_LEN_J1939] = {0xFF};
        INT nType = 0;
        INT nSize = MAX_MSG_LEN_J1939;
        HRESULT Result = pSWJ1939->m_ouMsgBufVSE_J.ReadFromBuffer(nType, abyData, nSize);
        if (Result == ERR_READ_MEMORY_SHORT)
        {
            TRACE("ERR_READ_MEMORY_SHORT");
        }
        else if (Result == EMPTY_APP_BUFFER)
        {
            TRACE("EMPTY_APP_BUFFER");
        }
        else if (Result == CALL_SUCCESS)
        {
            sMsg.vSetDataStream(abyData);
            pSWJ1939->vDisplayInSigWatchWnd(sMsg);
        }

    }
    return 0;
}

DWORD WINAPI SigWatchDataReadThreadProc_J(LPVOID pVoid)
{
    /*USES_CONVERSION;*/

    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;
    if (pThreadParam == NULL)
    {
        return (DWORD)-1;
    }
    CSignalWatch_J1939* pSWJ1939 = (CSignalWatch_J1939*)pThreadParam->m_pBuffer;
    if (pSWJ1939 == NULL)
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
                ReadJ1939DataBuffer(pSWJ1939); // Retrieve message from the driver
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

BOOL CSignalWatch_J1939::InitInstance(void)
{
    InitializeCriticalSection(&m_omCritSecSW);
    m_pouSigWnd = NULL;	
    m_pMsgInterPretObj_J = NULL;
    m_ouReadThread.m_hActionEvent = m_ouMsgBufVSE_J.hGetNotifyingEvent();
    return TRUE;
}

int CSignalWatch_J1939::ExitInstance(void)
{
    m_ouReadThread.bTerminateThread(); // Terminate read thread
    m_ouMsgBufVSE_J.vClearMessageBuffer();//clear J1939 buffer

    if (m_pMsgInterPretObj_J != NULL) // clear interpretation object
    {
        m_pMsgInterPretObj_J->vClear();
        delete m_pMsgInterPretObj_J;
        m_pMsgInterPretObj_J = NULL;
    }

    DeleteCriticalSection(&m_omCritSecSW); //delete critical section

    if (m_pouSigWnd != NULL)
    {
        m_pouSigWnd->DestroyWindow();
        delete m_pouSigWnd;
        m_pouSigWnd = NULL;		
    }
    return TRUE;
}

BOOL CSignalWatch_J1939::bStartSigWatchReadThread()
{
    m_ouReadThread.m_pBuffer = this;
    m_ouReadThread.bStartThread(SigWatchDataReadThreadProc_J);
    return TRUE;
}

void CSignalWatch_J1939::vDisplayInSigWatchWnd(STJ1939_MSG& sMsg)
{
    EnterCriticalSection(&m_omCritSecSW);
    if (m_pMsgInterPretObj_J != NULL)
    {
        static CString omMsgName;
        static SSignalInfoArray omSigInfoArray;
        static CStringArray omSigNames, omRawValues, omPhyValues;
        omSigNames.RemoveAll();
        omRawValues.RemoveAll();
        omPhyValues.RemoveAll();

        if (m_pMsgInterPretObj_J->bInterPretJ1939_MSGS(m_bHex ? HEXADECIMAL : DEC,
                sMsg.m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN(),
                sMsg.m_unDLC,
                sMsg.m_pbyData,
                omMsgName,
                omSigInfoArray) == TRUE)
        {
            UINT unCount = (UINT)omSigInfoArray.GetSize();
            for (UINT i = 0; i < unCount; i++)
            {
                SSignalInfo& sSignalInfo = omSigInfoArray.GetAt(i);
                omSigNames.Add(sSignalInfo.m_omSigName);
                omRawValues.Add(sSignalInfo.m_omRawValue);
                omPhyValues.Add(sSignalInfo.m_omEnggValue);
            }
            if ((m_pouSigWnd != NULL) && (m_pouSigWnd->IsWindowVisible()))
            {
                m_pouSigWnd->vAddMsgSigIntoList(omMsgName,omSigNames, omRawValues, omPhyValues, FALSE);
            }
        }
    }
    LeaveCriticalSection(&m_omCritSecSW);

    //delete the invalid entries
    vDeleteRemovedListEntries();
}

void CSignalWatch_J1939::vDeleteRemovedListEntries()

{
    if ((m_pMsgInterPretObj_J != NULL) && (m_pouSigWnd != NULL))
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
            int iSignalCount = m_pMsgInterPretObj_J->nGetJ1939SignalCount(strMsgName);
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

HRESULT CSignalWatch_J1939::SW_DoInitialization()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    //Create the signal watch window for J1939
    if (m_pouSigWnd == NULL)
    {		
        m_pouSigWnd = new CSigWatchDlg(AfxGetMainWnd(), J1939);
        m_pouSigWnd->Create(IDD_DLG_SIGNAL_WATCH, NULL);
        m_pouSigWnd->SetWindowText("Signal Watch - J1939");
    }

    CBaseDILI_J1939* pouDIL_J1939;
    if (DIL_GetInterface(J1939, (void**)&pouDIL_J1939) == S_OK)
    {
        DWORD dwClientId = 0;
        pouDIL_J1939->DILIJ_RegisterClient(TRUE, J1939_MONITOR_NODE, J1939_ECU_NAME, 0, dwClientId);
        pouDIL_J1939->DILIJ_ManageMsgBuf(MSGBUF_ADD, dwClientId, &(m_ouMsgBufVSE_J));
    }
    //Start the read thread
    return bStartSigWatchReadThread()? S_OK: S_FALSE;
}
HRESULT CSignalWatch_J1939::SW_ShowAddDelSignalsDlg(CWnd* pParent, void* podMainSubList)
{
	CMainEntryList * podMainSubListfinal= (CMainEntryList *) podMainSubList;
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CSigWatchAddDelDlg odDlg(pParent, podMainSubListfinal);
    return (HRESULT)odDlg.DoModal();
}

HRESULT CSignalWatch_J1939::SW_ShowSigWatchWnd(CWnd* /*pParent*/, HWND hMainWnd, INT nCmd)
{
    if (m_pouSigWnd != NULL)
    {		
		m_pouSigWnd->vUpdateMainWndHandle(hMainWnd);
		m_pouSigWnd->SetParent(CWnd::FromHandle(hMainWnd));
        return m_pouSigWnd->ShowWindow(nCmd);
    }
    return S_FALSE;
}
HRESULT CSignalWatch_J1939::SW_GetConfigSize(void)
{
    return (sizeof (WINDOWPLACEMENT) + sizeof(UINT) * defSW_LIST_COLUMN_COUNT);
}
HRESULT CSignalWatch_J1939::SW_GetConfigData(void* pbyConfigData)
{
    WINDOWPLACEMENT WndPlace;
    UINT nDebugSize  = 0;
    BYTE* pbyTemp = (BYTE*)pbyConfigData;
    if ((m_pouSigWnd != NULL) && (pbyTemp != NULL))
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

HRESULT CSignalWatch_J1939::SW_SetConfigData(const void* pbyConfigData)
{
    const BYTE* pbyTemp = (BYTE*)pbyConfigData;
    if ((pbyConfigData != NULL) && (m_pouSigWnd != NULL))
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
    if(m_pouSigWnd != NULL)
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

        if(pbyConfigData == NULL)
        {
            m_pouSigWnd->ShowWindow(SW_HIDE);
        }
    }
    return S_OK;
}

// PTV XML
HRESULT CSignalWatch_J1939::SW_GetConfigData(xmlNodePtr pNodePtr)
{
    // Setting signal watch window placement and column width
    xmlNodePtr pWndPositn = xmlNewNode(NULL, BAD_CAST DEF_WINDOW_POSITION);
    xmlAddChild(pNodePtr, pWndPositn);

    WINDOWPLACEMENT WndPlace;
    UINT nDebugSize  = 0;
    //BYTE* pbyTemp = (BYTE*)pbyConfigData;
    if ((m_pouSigWnd != NULL))
    {
        m_pouSigWnd->GetWindowPlacement(&WndPlace);
        //COPY_DATA(pbyTemp, &WndPlace, sizeof (WINDOWPLACEMENT));

        BOOL bIsWndwVisible = m_pouSigWnd->IsWindowVisible();

        CString strWindwVisibility = "";

        strWindwVisibility = xmlUtils::nSetWindowVisibility(WndPlace.showCmd);

        // Writing visibility in to xml
        xmlNodePtr pVisibility = xmlNewChild(pWndPositn, NULL, BAD_CAST DEF_VISIBILITY, BAD_CAST strWindwVisibility.GetBuffer(strWindwVisibility.GetLength()));
        xmlAddChild(pWndPositn, pVisibility);

        // Writing window Placement
        UINT nFlag = WndPlace.flags;

        CString strWindPlcmnt = "";

        strWindPlcmnt = xmlUtils::nSetWindowVisibility(nFlag);

        // Setting window Placement
        xmlNodePtr pWndwVisibility = xmlNewChild(pWndPositn, NULL, BAD_CAST DEF_WINDOW_PLACEMENT, BAD_CAST strWindPlcmnt.GetBuffer(strWindPlcmnt.GetLength()));
        xmlAddChild(pWndPositn, pWndwVisibility);

        CString strBottomPos = "", strLeftPos = "", strTopPos = "", strRightPos = "";

        strTopPos.Format("%d", WndPlace.rcNormalPosition.top);
        strLeftPos.Format("%d", WndPlace.rcNormalPosition.left);
        strRightPos.Format("%d", WndPlace.rcNormalPosition.right);
        strBottomPos.Format("%d", WndPlace.rcNormalPosition.bottom);

        // Writing co-ordinates of the window in to XML file
        xmlNodePtr pTopPos = xmlNewChild(pWndPositn, NULL, BAD_CAST DEF_TOP, BAD_CAST strTopPos.GetBuffer(strTopPos.GetLength()));
        xmlAddChild(pWndPositn, pTopPos);

        xmlNodePtr pLeftPos = xmlNewChild(pWndPositn, NULL, BAD_CAST DEF_Left,BAD_CAST strLeftPos.GetBuffer(strLeftPos.GetLength()));
        xmlAddChild(pWndPositn, pLeftPos);

        xmlNodePtr pRightPos = xmlNewChild(pWndPositn, NULL, BAD_CAST DEF_Right,BAD_CAST strRightPos.GetBuffer(strRightPos.GetLength()));
        xmlAddChild(pWndPositn, pRightPos);

        xmlNodePtr pBottomPos = xmlNewChild(pWndPositn, NULL, BAD_CAST DEF_Bottom,BAD_CAST strBottomPos.GetBuffer(strBottomPos.GetLength()));
        xmlAddChild(pWndPositn, pBottomPos);

        // Setting Column width in to the xml file
        xmlNodePtr pColWidthPtr = xmlNewNode(NULL, BAD_CAST DEF_Columns_Width);
        xmlAddChild(pNodePtr, pColWidthPtr);

        for (UINT i = 0; i < defSW_LIST_COLUMN_COUNT; i++)
        {
            INT nWidth = m_pouSigWnd->m_omSignalList.GetColumnWidth(i);
            CString strWidth = "";
            strWidth.Format("%d", nWidth);

            if(i == 0)
            {
                // Writing message column value in to xml
                xmlNodePtr pMsgClmnPtr = xmlNewChild(pColWidthPtr, NULL, BAD_CAST DEF_Message_Column, BAD_CAST strWidth.GetBuffer(strWidth.GetLength()));
                xmlAddChild(pColWidthPtr, pMsgClmnPtr);
            }
            if(i == 1)
            {
                // Writing Raw value column value in to xml
                xmlNodePtr pRawValClmn = xmlNewChild(pColWidthPtr, NULL, BAD_CAST DEF_Raw_Val_Column, BAD_CAST strWidth.GetBuffer(strWidth.GetLength()));
                xmlAddChild(pColWidthPtr, pRawValClmn);
            }
            if(i == 2)
            {
                // Writing Raw value column value in to xml
                xmlNodePtr pPhyalClmn = xmlNewChild(pColWidthPtr, NULL, BAD_CAST DEF_Physical_Val_Column, BAD_CAST strWidth.GetBuffer(strWidth.GetLength()));
                xmlAddChild(pColWidthPtr, pPhyalClmn);
            }
            if(i == 3)
            {
                // Writing Raw value column value in to xml
                xmlNodePtr pSigValClmn = xmlNewChild(pColWidthPtr, NULL, BAD_CAST DEF_Signal_Column, BAD_CAST strWidth.GetBuffer(strWidth.GetLength()));
                xmlAddChild(pColWidthPtr, pSigValClmn);
            }
        }
    }
    return S_OK;
}
// PTV XML

//MVN
HRESULT CSignalWatch_J1939::SW_SetConfigData(xmlNodePtr pNode)
{
    INT nRetValue  = S_OK;
    if ((pNode != NULL) && (m_pouSigWnd != NULL))
    {
        WINDOWPLACEMENT WndPlace;
        while(pNode != NULL)
        {
            if ((!xmlStrcmp(pNode->name, (const xmlChar*)"Window_Position")))
            {
                nRetValue = xmlUtils::ParseWindowsPlacement(pNode, WndPlace);
                if(nRetValue == S_OK)
                {
                    m_pouSigWnd->MoveWindow(&(WndPlace.rcNormalPosition), FALSE);
                }
                break;
            }
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
//~MVN
HRESULT CSignalWatch_J1939::SW_ClearSigWatchWnd(void)
{
    if (m_pouSigWnd != NULL)
    {
        m_pouSigWnd->PostMessage(WM_REMOVE_SIGNAL);
    }
    return S_OK;
}

HRESULT CSignalWatch_J1939::SW_UpdateMsgInterpretObj(void* pvRefObj)
{
    CMsgInterpretationJ1939* RefObj = (CMsgInterpretationJ1939*) pvRefObj;
    EnterCriticalSection(&m_omCritSecSW);
    if (m_pMsgInterPretObj_J == NULL)
    {
        m_pMsgInterPretObj_J = new CMsgInterpretationJ1939;
    }
    RefObj->vCopy(m_pMsgInterPretObj_J);
    LeaveCriticalSection(&m_omCritSecSW);
    return S_OK;
}
BOOL CSignalWatch_J1939::SW_IsWindowVisible()
{
    BOOL bResult = FALSE;
    if (m_pouSigWnd != NULL)
    {
        bResult = m_pouSigWnd->IsWindowVisible();
    }
    return bResult;
}
HRESULT CSignalWatch_J1939::SW_SetDisplayMode(BOOL bHex)
{
    m_bHex = bHex;
    return S_OK;
}