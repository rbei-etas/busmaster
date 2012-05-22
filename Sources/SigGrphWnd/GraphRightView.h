/*******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  RightView.h
  Description   :  Interface file for CGraphRightView class 
  $Log:   X:/Archive/Sources/SigGrphWnd/GraphRightView.h_v  $
 * 
 *    Rev 1.2   15 Apr 2011 19:43:32   rac2kor
 * Inserted RBEI Copyright information text into the file header.
 * 
 *    Rev 1.1   23 Dec 2010 18:04:44   CANMNTTM
 * Updated to support cursors feature in 
 * graph control.
 * 
 *    Rev 1.0   13 Dec 2010 22:00:36   CANMNTTM
 *  
 * 
 *    Rev 1.0   16 Aug 2010 21:20:34   rac2kor
 *  

  Author        :  Raja N
  Date Created  :  10/12/2004
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
*******************************************************************************/

//{{AFX_INCLUDES()
//}}AFX_INCLUDES
#if !defined(AFX_RIGHTVIEW_H__AAAA44BC_7F6F_4721_A794_63BA29612960__INCLUDED_)
#define AFX_RIGHTVIEW_H__AAAA44BC_7F6F_4721_A794_63BA29612960__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RightView.h : header file
//

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

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RIGHTVIEW_H__AAAA44BC_7F6F_4721_A794_63BA29612960__INCLUDED_)
