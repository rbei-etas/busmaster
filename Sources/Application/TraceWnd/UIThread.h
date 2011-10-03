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
 * \file      Application/TraceWnd/UIThread.h
 * \brief     This is the interface file for CNotificWnd class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This is the interface file for CNotificWnd class
 */

#if !defined(AFX_UITHREAD_H__35BE3D97_728D_431E_AD3A_323E93D32A2B__INCLUDED_)
#define AFX_UITHREAD_H__35BE3D97_728D_431E_AD3A_323E93D32A2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UIThread.h : header file
//

// Interface file for class CNotificWnd
#include "NotificWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CUIThread thread

class CUIThread : public CWinThread
{
    DECLARE_DYNCREATE(CUIThread)
protected:
    CUIThread();           // protected constructor used by dynamic creation
    CNotificWnd* m_podTraceWinObj;

// Attributes
private:

// Operations
public:
    BOOL bCreateTraceWindow(CMDIFrameWnd*);
    void vUpdateWndCo_Ords(WINDOWPLACEMENT& wndPlacement, BOOL bLoadIntoTrace);
    // To display texts on the trace window
    void vAddString(CString omStr);
    // To know if the trace window is visible
    BOOL bIsTraceWindowVisible(void);
	HWND hGetHandleTraceWnd();

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CUIThread)
    public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    //}}AFX_VIRTUAL

// Implementation
protected:
    virtual ~CUIThread();

    // To update the window status from the configuration file
    afx_msg void vUpdateWinStatusFromCfg(UINT, LONG);
    // To process thread messages
    afx_msg void vWriteTextToTrace(UINT, LONG);
    afx_msg void vProcessThreadMsg(UINT, LONG);
    

    // Generated message map functions
    //{{AFX_MSG(CUIThread)
        // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UITHREAD_H__35BE3D97_728D_431E_AD3A_323E93D32A2B__INCLUDED_)
