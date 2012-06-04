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
 * \file      RightView.h
 * \brief     Interface file for CGraphRightView class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CGraphRightView class
 */

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CRightView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "GraphChildFrame.h"
#include "GraphSink.h"

class CGraphRightView : public CFormView
{
protected:
    // protected constructor used by dynamic creation
    CGraphRightView();
    DECLARE_DYNCREATE(CGraphRightView)

// Form Data
public:
    //{{AFX_DATA(CGraphRightView)
    enum { IDD = IDD_DLG_GRAPH_RIGHT_TOP };    
    //}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CRightView)
    public:
    virtual void OnInitialUpdate();
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    virtual ~CGraphRightView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // Generated message map functions
    //{{AFX_MSG(CGraphRightView)
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
	// To get parent window pointer
    CWnd * pomGetParentWindow() const;
	CGraphChildFrame* m_pParentWnd;

	//Pointer to hold CGraphSink class pointer
	CGraphSink* m_pGraphSink;
	//Get hold of the Sink cookie ID
	DWORD m_dwCookie;
	CWnd* m_pWndGraphCtrl;
public:
	DECLARE_EVENTSINK_MAP()
	void CursorPositionGraphCtrl(double X, double Y, short shCursorID);	
	IDMGraphCtrl* m_pDMGraphCtrl;	
public:
	afx_msg void OnDestroy();
	void vHandleConnectionStatusChange(BOOL bConnectStatus);
};
