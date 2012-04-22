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
 * \file      GraphBottomView.h
 * \brief     Interface file for CGraphBottomView class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CGraphBottomView class
 */

#pragma once

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
