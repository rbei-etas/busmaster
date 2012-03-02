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
 * \file      MsgSignalDBWnd.h
 * \brief     This header file contains the defintion of class
 * \author    Amarnath Shastry
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the defintion of class
 */
#if !defined(AFX_MSGSIGNALDBWND_H__9861E0E3_241D_11D6_A618_00D0B76BEBF5__INCLUDED_)
#define AFX_MSGSIGNALDBWND_H__9861E0E3_241D_11D6_A618_00D0B76BEBF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgSignalDBWnd.h : header file
//
#include "CFilesDefs_CAN.h"
/////////////////////////////////////////////////////////////////////////////
// CMsgSignalDBWnd frame

class CMsgSignalDBWnd : public CMDIChildWnd
{
    DECLARE_DYNCREATE(CMsgSignalDBWnd)
protected:
               // protected constructor used by dynamic creation

// Attributes
public:
    static BOOL sm_bValidJ1939Wnd;
public:
    SDBPARAMS m_sDbParams;
// Operations
public:
    CMsgSignalDBWnd();
    CMsgSignalDBWnd(const SDBPARAMS& sDbParams);
    virtual ~CMsgSignalDBWnd();
    void vSetDBName(CString& omDBName);
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMsgSignalDBWnd)
    public:
    protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CMsgSignalDBWnd)
    afx_msg void OnClose();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    void vCalculateSplitterPosition(CSize& cSize);
    BOOL m_bSplitWndCreated;
    CSplitterWnd m_omSplitterWnd;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGSIGNALDBWND_H__9861E0E3_241D_11D6_A618_00D0B76BEBF5__INCLUDED_)
