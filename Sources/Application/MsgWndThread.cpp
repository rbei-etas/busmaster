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
 * \file      MsgWndThread.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
// CMsgWndThread.cpp : implementation file
//

#include "stdafx.h"
#include <afxcoll.h>
#include "BUSMASTER.h"
#include "MsgWndThread.h"
#include "MsgWndHdrInfo.h"
#include "MsgFrmtWnd.h"

CStringArray g_arrstrBusNames;

// CMsgWndThread

IMPLEMENT_DYNCREATE(CMsgWndThread, CWinThread)

CMsgWndThread::CMsgWndThread()
{	
	g_arrstrBusNames.Add(CString(" - CAN"));
	g_arrstrBusNames.Add(CString(" - MCNET"));
	g_arrstrBusNames.Add(CString(" - J1939"));
	g_arrstrBusNames.Add(CString(" - FLEXRAY"));

	for(short shBusID = CAN; shBusID < BUS_TOTAL; shBusID++)
	{
		m_pMsgWnd[shBusID] = NULL;
	}
}

CMsgWndThread::~CMsgWndThread()
{
}

BOOL CMsgWndThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CMsgWndThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMsgWndThread, CWinThread)
	ON_THREAD_MESSAGE(WM_MODIFY_VISIBILITY, vModifyVisibilityStatus)
    //ON_THREAD_MESSAGE(WM_SHUTDOWN_MSGWNDTHREAD, vShutdownThread)
END_MESSAGE_MAP()

// CMsgWndThread message handlers

BOOL CMsgWndThread::CreateMsgWnd(HWND hFrameWnd, eTYPE_BUS eBusType, DWORD dwClientID, void* pParam)
{
	CString strWndText;
	strWndText.LoadString(IDR_MESSAGE_WINDOW_TEXT);
	strWndText+= g_arrstrBusNames.GetAt((int)eBusType);

    m_bAutoDelete = TRUE;
	m_pMsgWnd[eBusType] = new CMsgFrmtWnd(eBusType);
	m_pMainWnd = m_pMsgWnd[eBusType];
	
    CString omArrColTitle[MAX_MSG_WND_COL_CNT] = {_T("")};
    int nCount = 0;
	CMsgWndHdrInfo objMsgHdrInfo;
    objMsgHdrInfo.vGetHdrColNames(omArrColTitle, nCount); 
    m_pMsgWnd[eBusType]->vSetColTitles(omArrColTitle, nCount);
	
    m_pMsgWnd[eBusType]->Create(strWndText, WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
            CRect(0,0, CFrameWnd::rectDefault.right, CFrameWnd::rectDefault.bottom),
            (CMDIFrameWnd*) (CWnd::FromHandle(hFrameWnd)));
     // Set specific parameters on successful creation    	
    m_pMsgWnd[eBusType]->vInit(pParam);	
	m_pMsgWnd[eBusType]->vSetClientID(dwClientID);		

	return TRUE;
}

void CMsgWndThread::vUpdateClientID(eTYPE_BUS eBusType, DWORD dwClientID)
{
	if(m_pMsgWnd[eBusType])
	{		
		m_pMsgWnd[eBusType]->vSetClientID(dwClientID);
		//Client ID updation needs the thread restart
		m_pMsgWnd[eBusType]->bStartReadThread();
	}
}

void CMsgWndThread::vModifyVisibilityStatus(UINT unParam, LONG lParam)
{
	eTYPE_BUS eBusType = (eTYPE_BUS) lParam;	
	if(unParam == SW_SHOW)
	{
		if (m_pMsgWnd[eBusType] != NULL)
		{
			m_pMsgWnd[eBusType]->ShowWindow(SW_SHOW);
			m_pMsgWnd[eBusType]->MDIActivate();
		}
	}
	else if(unParam == SW_HIDE)
	{
		if (m_pMsgWnd[eBusType] != NULL)
		{
			m_pMsgWnd[eBusType]->ShowWindow(SW_HIDE);
		}
	}
}

HWND CMsgWndThread::hGetHandleMsgWnd(eTYPE_BUS eBusType)
{
	if(m_pMsgWnd[eBusType])
		return m_pMsgWnd[eBusType]->GetSafeHwnd();
	else
		return NULL;
}

void CMsgWndThread::vSetDILInterfacePointer(eTYPE_BUS eBusType, void** ppvJ1939DIL)
{	
	if(m_pMsgWnd[eBusType])
		m_pMsgWnd[eBusType]->vSetDILInterfacePointer(ppvJ1939DIL);
}

/*void CMsgWndThread::vShutdownThread(UINT unParam, LONG lParam)
{
    PostQuitMessage(0);
}

void CMsgWndThread::vUpdateSigWatchList(PSSIGNALWATCHLIST psSigWatchList, CMsgSignal* psMsgDatabase)
{
    if (m_pMsgWnd != NULL)
    {
        m_pMsgWnd->vUpdateSigWatchList(psSigWatchList, psMsgDatabase);
    }
}
void CMsgWndUIThread::vUpdateSigWatchDlgPtr(CSigWatchDlg* pouSigWatchDlg)
{
    if (m_pMsgWnd != NULL)
    {
        m_pMsgWnd->vUpdateSigWatchDlgPtr(pouSigWatchDlg);
    }
}*/