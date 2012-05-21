/*******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  Bottom.h
  Description   :  Interface file for CGraphBottomView class
  $Log:   X:/Archive/Sources/SigGrphWnd/GraphBottomView.h_v  $
 * 
 *    Rev 1.2   15 Apr 2011 19:43:30   rac2kor
 * Inserted RBEI Copyright information text into the file header.
 * 
 *    Rev 1.1   23 Dec 2010 18:04:44   CANMNTTM
 * Updated to support cursors feature in 
 * graph control.
 * 
 *    Rev 1.0   13 Dec 2010 22:00:34   CANMNTTM
 *  
 * 
 *    Rev 1.0   16 Aug 2010 21:20:30   rac2kor
 *  

  Author(s)     :  Raja N
  Date Created  :  10/12/2004
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
*******************************************************************************/

#if !defined(AFX_BOTTOM_H__9573691F_DD7A_46ED_AA0E_379F80C167A9__INCLUDED_)
#define AFX_BOTTOM_H__9573691F_DD7A_46ED_AA0E_379F80C167A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "afxcmn.h"
// For Color ListCtrl class definition
#include "MessageList.h"
// for Resource Symbol definitions
#include "SigGrphWnd_resource.h"

class CGraphBottomView : public CFormView
{
// Form Data
public:
    //{{AFX_DATA(CGraphBottomView)
    enum { IDD = IDD_DLG_GRAPH_RIGHT_BOTTOM };
    CButton m_omBtnRight;
    CButton m_omBtnLeft;
    CButton m_omBtnDown;
    CButton m_omBtnUp;
    int     m_nAxis;
    int     m_nAction;
    double  m_dRangeFrom;
    double  m_dRangeTo;
    //}}AFX_DATA

// Attributes
public:

// Operations
public:
    // To Initialise graph control with view style parameters
    void vInitGraphControl();
    // To Handle Connection change event
    void vHandleConnectionStatusChange(BOOL bConnectStatus);
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CGraphBottomView)
    public:
    virtual void OnInitialUpdate();
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // protected constructor used by dynamic creation
    CGraphBottomView();
    DECLARE_DYNCREATE(CGraphBottomView)
    virtual ~CGraphBottomView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // Generated message map functions
    //{{AFX_MSG(CGraphBottomView)
    afx_msg void OnBtnExport();
    afx_msg void OnBtnAutoFit();
    afx_msg void OnBtnConfigure();
    afx_msg void OnBtnCopy();
    afx_msg void OnBtnGrid();
    afx_msg void OnBtnSet();
    afx_msg void OnRadioActionMove();
    afx_msg void OnRadioActionNone();
    afx_msg void OnRadioActionTrack();
    afx_msg void OnRadioActionZoom();
    afx_msg void OnRadioAxisX();
    afx_msg void OnRadioAxisXy();
    afx_msg void OnRadioAxisY();
    afx_msg void OnBtnFitAll();
    afx_msg void OnBtnUp();
    afx_msg void OnBtnLeft();
    afx_msg void OnBtnDown();
    afx_msg void OnBtnRight();
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg LRESULT vHandleConfigFileChange( WPARAM wParam,LPARAM lParam);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    // To Update the changes in Configuration Module 
    void vSaveChangedInToConfig();
    // To Enable/ Disable Axis controls
    void vEnableAxisControls(BOOL bEnable);
    // To Enable/ Disable Action controls
    void vEnableActionControls(BOOL bEnable);
    // To Enable/ Disable UI componemts
    void vEnableDisableButtons( BOOL );
    // To Update graph control with selected Axis and Action
    void vSetAction();
    // To set Show/Hide Grid button text
    void vSetShowButtonState(BOOL bShow);
	//SGW Code commented by Arun 21-10-2010
    // To maintaing Main frame pointer
    //CMainFrame * m_pMainFrame;
	//SGW Code commented by Arun 21-10-2010
    // To have reference of Graph Control interface
    IDMGraphCtrl* m_pDMGraphCtrl;
    // To store Update Timer ID
    int nTimerID;
	// To get parent window pointer 
    CWnd * pomGetParentWindow() const;
	//Gets text extent for a string
	int nGetListCtrlTextExtent(CString omColTitle);
	void vCreateColumns();		
public:
	void vUpdateCursordetails(double X, double Y, short shCursorID);
	void vUpdateSignalData();
	double dRound(double val, unsigned int decimals);	
	void vInsertSignalData();
	double m_dblarrTime[2];	 
	double m_dblDeltaTime;
	CMessageList m_lstSignalDetails;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BOTTOM_H__9573691F_DD7A_46ED_AA0E_379F80C167A9__INCLUDED_)
