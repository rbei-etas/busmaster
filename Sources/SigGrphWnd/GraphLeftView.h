/*******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  LeftView.h
  Description   :  Interface file for CGraphLeftView class
  $Log:   X:/Archive/Sources/SigGrphWnd/GraphLeftView.h_v  $
 * 
 *    Rev 1.1   15 Apr 2011 19:43:32   rac2kor
 * Inserted RBEI Copyright information text into the file header.
 * 
 *    Rev 1.0   13 Dec 2010 22:00:34   CANMNTTM
 *  
 * 
 *    Rev 1.0   16 Aug 2010 21:20:32   rac2kor
 *  

  Author        :  Raja N
  Date Created  :  10/12/2004
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
*******************************************************************************/

#if !defined(AFX_LEFTVIEW_H__4A0CCF22_7C4C_4FD1_BFB7_33420FBAFA17__INCLUDED_)
#define AFX_LEFTVIEW_H__4A0CCF22_7C4C_4FD1_BFB7_33420FBAFA17__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

// For Graph Element class 
#include "GraphElement.h"
// For Color ListCtrl class definition
#include "MessageList.h"
// For Color Pocker class definition
#include "Utility/ColourPicker.h"

class CGraphLeftView : public CFormView
{
protected:
    // protected constructor used by dynamic creation
    CGraphLeftView();
    DECLARE_DYNCREATE(CGraphLeftView)

// Form Data
public:
    //{{AFX_DATA(CGraphLeftView)
    enum { IDD = IDD_DLG_GRAPH_LEFT_TOP };
    CButton m_omBtnVisible;
    CButton m_omBtnEnable;
    CMessageList    m_omSignalList;
    CColourPicker   m_omPointColor;
    CColourPicker   m_omLineColor;
    int     m_nLineType;
    int     m_nSymbolType;
	IDMGraphCtrl* m_pDMGraphCtrl;
    //}}AFX_DATA

// Attributes
public:

// Operations
public:
    // To handle tool connection change event
    void vHandleConnectionStatusChange(BOOL bConnect);
	// To populate list with graph element details
    void vPopulateElementList();
	// To Populate graph control with list of elements
    void vPopulateGraphControl();
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CGraphLeftView)
    public:
    virtual void OnInitialUpdate();
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    virtual ~CGraphLeftView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // Generated message map functions
    //{{AFX_MSG(CGraphLeftView)
    afx_msg void OnBtnVisible();
    afx_msg void OnItemchangedListSignals(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSelchangeComboType();
    afx_msg void OnSelchangeComboSymbol();
    afx_msg void OnBtnEnable();
    afx_msg LRESULT OnColorChange( WPARAM wparam,LPARAM lparam);
    afx_msg LRESULT vHandleConfigFileChange( WPARAM wParam,LPARAM lParam);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    BOOL m_bConnectionStatus;
    void vUpdateGraphControl(int nFromControl, long lIndex);
    // To get selected item details
    //CGraphElement odGetSelectedElement();
    // To update UI controls with the element details
    void vSetElementDetails(CGraphElement odElement);
    // To Enable/ Disable controls.using the value bEnable
    void vEnableControls(BOOL bEnable);
    // To Enable/ Disable controls based on selection in UI element List
    void vEnableDisableControls();    
	// To get parent window pointer
    CWnd * pomGetParentWindow() const;
public:	
	int m_nLineDisplay;
	HRESULT GetCurrentElement(IDMGraphCtrl* pGraphCtrl, IDMGraphElement** ppGraphElement);	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEFTVIEW_H__4A0CCF22_7C4C_4FD1_BFB7_33420FBAFA17__INCLUDED_)
