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
 * \file      Application/UIThread.cpp
 * \brief     This is the implementation file for CNotificWnd class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This is the implementation file for CNotificWnd class
 */

// For all standard header file include
#include "StdAfx.h"

// Global definitions
#include "Common.h"

// Interface file for CUIThread class
#include "UIThread.h"

/////////////////////////////////////////////////////////////////////////////
// CUIThread


IMPLEMENT_DYNCREATE(CUIThread, CWinThread)

/******************************************************************************
    Function Name    :  CUIThread
    Input(s)         :  -
    Output           :  -
    Functionality    :  Standard constructor
    Member of        :  CUIThread
    Author(s)        :  Ratnadip Choudhury
    Date Created     :  29-01-2004
    Modified by      :  
    Modification     :  
/*****************************************************************************/
CUIThread::CUIThread()
{
    //m_omDispStrArray.RemoveAll();
    m_podTraceWinObj = NULL;
    //InitializeCriticalSection(&m_sCriticalSection);
}
/******************************************************************************
    Function Name    :  ~CUIThread
    Input(s)         :  -
    Output           :  -
    Functionality    :  Standard destructor
    Member of        :  CUIThread
    Author(s)        :  Ratnadip Choudhury
    Date Created     :  29-01-2004
    Modified by      :  
    Modification     :  
/*****************************************************************************/
CUIThread::~CUIThread()
{
    //DeleteCriticalSection(&m_sCriticalSection);
}
/******************************************************************************
    Function Name    :  InitInstance
    Input(s)         :  -
    Output           :  TRUE if successful, else FALSE
    Functionality    :  Overridden to initialize each new instance of this 
                        user interface thread
    Member of        :  CUIThread
    Author(s)        :  Ratnadip Choudhury
    Date Created     :  29-01-2004
    Modified by      :  
    Modification     :  
/*****************************************************************************/
BOOL CUIThread::InitInstance()
{
    // TODO:  perform and per-thread initialization here
    m_bAutoDelete = TRUE;
    return TRUE;
}
/******************************************************************************
    Function Name    :  ExitInstance
    Input(s)         :  -
    Output           :  The thread's exit code; 0 indicates no errors, and values
                        greater than 0 indicate an error
    Functionality    :  Called by the framework to exit this instance of the thread
    Member of        :  CUIThread
    Author(s)        :  Ratnadip Choudhury
    Date Created     :  29-01-2004
    Modified by      :  
    Modification     :  
/*****************************************************************************/
int CUIThread::ExitInstance()
{
    // TODO:  perform any per-thread cleanup here
    m_podTraceWinObj->PostMessage(WM_DESTROY, 0, 0);
    Sleep(0);

    return CWinThread::ExitInstance();
}
/******************************************************************************
    Function Name    :  bCreateTraceWindow
    Input(s)         :  pomParentWnd - Pointer to the parent MDI frame window.
    Output           :  TRUE if successful, else FALSE
    Functionality    :  Called to create the trace window
    Member of        :  CUIThread
    Author(s)        :  Ratnadip Choudhury
    Date Created     :  29-01-2004
    Modified by      :  
    Modification     :  
/*****************************************************************************/
BOOL CUIThread::bCreateTraceWindow(CMDIFrameWnd* pomParentWnd)
{
    BOOL bResult = FALSE;

    if (m_podTraceWinObj == NULL)
    {
        m_podTraceWinObj = new CNotificWnd();
        if (m_podTraceWinObj != NULL)
        {
            m_pMainWnd = m_podTraceWinObj;
            bResult = m_podTraceWinObj->bCreateNotificWindow(pomParentWnd);
            if (bResult == TRUE)
            {
                m_podTraceWinObj->ShowWindow(SW_SHOW);
                m_podTraceWinObj->UpdateWindow();
                m_podTraceWinObj->SendMessage(WM_NCPAINT, 1, 0);
                m_podTraceWinObj->MDIActivate();
            }
        }
    }

    return bResult;
}
/******************************************************************************
    Function Name    :  vUpdateWndCo_Ords
    Input(s)         :  -
    Output           :  -
    Functionality    :  Called to save trace window status info into the 
                        current configuration file.
    Member of        :  CUIThread
    Author(s)        :  Ratnadip Choudhury
    Date Created     :  29-01-2004
    Modified by      :  
    Modification     :  
/*****************************************************************************/
void CUIThread::vUpdateWndCo_Ords(WINDOWPLACEMENT& wndPlacement, BOOL bLoadIntoTrace)
{
    if (m_podTraceWinObj != NULL)
    {
        if (bLoadIntoTrace == TRUE)
        {
            if ((wndPlacement.length == 0) || (wndPlacement.rcNormalPosition.top == -1))
            {
                CRect omRect;
                GetClientRect(m_pMainWnd->GetParent()->GetSafeHwnd(), &omRect);
                omRect.top   += static_cast<LONG> ( omRect.Height() * 
                    defCHILD_FRAME_PROP_H);
                omRect.right = static_cast<LONG> ( omRect.right * 
                    defREPLAY_WND_PROP_X);
                wndPlacement.rcNormalPosition = omRect;
            }
            m_podTraceWinObj->SetWindowPlacement(&wndPlacement);
        }
        else
        {
            m_podTraceWinObj->GetWindowPlacement(&wndPlacement);
        }
    }
    else
    {
        if (bLoadIntoTrace == FALSE)
        {
            wndPlacement.length = 0;
            wndPlacement.rcNormalPosition.top = -1;
        }
    }
}

BEGIN_MESSAGE_MAP(CUIThread, CWinThread)
    //{{AFX_MSG_MAP(CUIThread)
        // NOTE - the ClassWizard will add and remove mapping macros here.
        ON_THREAD_MESSAGE(WM_LOAD_SAVE_WINSTATUS, vUpdateWinStatusFromCfg)
        ON_THREAD_MESSAGE(WM_THREADMSG_PROC, vProcessThreadMsg)
        ON_THREAD_MESSAGE(WM_WRITE_TO_TRACE, vWriteTextToTrace)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUIThread message handlers

/******************************************************************************
    Function Name    :  vUpdateWinStatusFromCfg
    Input(s)         :  unParam - TRUE when window status info is to 
                                  be retrieved from the configuration file.
                                  FALSE when window status info is to 
                                  be saved into the configuration file.
    Output           :  -
    Functionality    :  Handler for the user defined message WM_LOAD_SAVE_WINSTATUS
                        posted to save or retrieve trace window status info.
    Member of        :  CUIThread
    Author(s)        :  Ratnadip Choudhury
    Date Created     :  29-01-2004
    Modified by      :  
    Modification     :  
/*****************************************************************************/
void CUIThread::vUpdateWinStatusFromCfg(UINT /*unParam*/, LONG lParam)
{
    if (m_podTraceWinObj != NULL)
    {
        WINDOWPLACEMENT* pWndCo_ords = (WINDOWPLACEMENT*)lParam;
        vUpdateWndCo_Ords(*pWndCo_ords, (BOOL)lParam);
    }
}

/******************************************************************************
    Function Name    :  vModifyVisibilityStatus
    Input(s)         :  unParam - SW_SHOW if trace window is to be shown
                                  SW_HIDE if trace window is to be hidden
                        lParam - Unused
    Output           :  -
    Functionality    :  Handler for the user defined message 
                        WM_MODIFY_VISIBILITY sent to show/hide trace window
    Member of        :  CUIThread
    Author(s)        :  Ratnadip Choudhury
    Date Created     :  29-01-2004
    Modified by      :  
    Modification     :  
/*****************************************************************************/

/******************************************************************************
    Function Name    :  vAddString
    Input(s)         :  omStr - Text to be displayed
    Output           :  -
    Functionality    :  Call to display texts on trace window
    Member of        :  CUIThread
    Author(s)        :  Ratnadip Choudhury
    Date Created     :  10-11-2008
    Modified by      :  
    Modification     :  
/*****************************************************************************/
void CUIThread::vAddString(CString omStr)
{
    if (m_podTraceWinObj != NULL)
    {
        m_podTraceWinObj->vDisplayString(omStr);
    }
}

BOOL CUIThread::bIsTraceWindowVisible(void)
{
    return m_podTraceWinObj->IsWindowVisible();
}

void CUIThread::vWriteTextToTrace(UINT /*unParam*/, LONG lParam)
{
    //CHAR* pacText = (CHAR*)lParam;
	CString strText((CHAR*)lParam);
    m_podTraceWinObj->vDisplayString(strText);
}

void CUIThread::vProcessThreadMsg(UINT unParam, LONG lParam)
{
    switch (unParam)
    {
        case WM_CLOSE:
        {
            PostQuitMessage(0);
        }
        break;
        case WM_MODIFY_VISIBILITY:
        {
            m_podTraceWinObj->ShowWindow(lParam);
            if (lParam == SW_SHOW)
            {
                m_podTraceWinObj->SendMessage(WM_NCPAINT, 1, 0);
            }
        }
        break;
    }
}

/******************************************************************************
    Function Name    :  hGetHandleTraceWnd
    Input(s)         :  
    Output           :  HWND (Window handle of the trace window)
    Functionality    :  returns window handle of the trace window
    Member of        :  CUIThread
    Author(s)        :  Pradeep Kadoor
    Date Created     :  19-01-2009
    Modification     :  
/*****************************************************************************/
HWND CUIThread::hGetHandleTraceWnd(void)
{
	return m_podTraceWinObj->m_hWnd;
}
