/*******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  ChildFrm.h
  Description   :  Interface file for CGraphChildFrame class
  $Log:   X:/Archive/Sources/SigGrphWnd/GraphChildFrame.h_v  $
 * 
 *    Rev 1.2   25 Jul 2011 11:44:54   CANMNTTM
 * Updated to make the graph clearing done only in two scenarios:
 * 1. When initially connected.
 * 2. DIsconnected and connected back.
 * 
 *    Rev 1.1   15 Apr 2011 19:43:30   rac2kor
 * Inserted RBEI Copyright information text into the file header.
 * 
 *    Rev 1.0   13 Dec 2010 22:00:34   CANMNTTM
 *  
 * 
 *    Rev 1.0   16 Aug 2010 21:20:32   rac2kor
 *  

  Author(s)     :  Raja N
  Date Created  :  10/12/2004
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
*******************************************************************************/

#if !defined(AFX_GRAPHCHILDFRM_H__C04AE5FA_F6F5_43C2_BF39_6C21A5F5E4A3__INCLUDED_)
#define AFX_GRAPHCHILDFRM_H__C04AE5FA_F6F5_43C2_BF39_6C21A5F5E4A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MDIChildBase.h"
#include "SigGrphWnd_Defines.h"
#include "GraphList.h"

class CGraphChildFrame : public CMDIChildBase
{
    DECLARE_DYNCREATE(CGraphChildFrame)
public:
// Operations
    // Default Constructor
    CGraphChildFrame();
    // Destructor
    virtual ~CGraphChildFrame();
public:
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CGraphChildFrame)
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    //}}AFX_VIRTUAL


#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
public:
    void vSetSplitterConfig(SGRAPHSPLITTERDATA& sSplitterPostion);

    void vGetSplitterConfig(SGRAPHSPLITTERDATA& sSplitterPostion);
    // To update window size and splitter after loading a conf file
    void vUpdateWinStatus();
    // To save window size and splitter postion before unloading a conf file
    void vUpdateWndCo_Ords();
    // To Handle Configuration File Change Event
    void vHandleConFigChange();
    // To Handle Connection state Change Event
    void vHandleConnectChange(BOOL bConnect);
    // To get Left View Pointer
    CView* pomGetLeftViewPointer();
    // To get Right Top View Pointer
    CView* pomGetRightTopViewPointer();
    // To get Right Bottom View Pointer
    CView* pomGetRightBottomViewPointer();
    // Pointer to each split views
    // These pointers will be initialised by each view itself after creation
    CView * m_pomBottomView, * m_pomRightView, * m_pomLeftView;
	// Graph Control
    IDMGraphCtrl* m_pDMGraphCtrl;
	//Configuration data
	CGraphList* m_pGraphList;
    // Create function of client. This will create splitted views
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	void vSetSignalListDetails(CGraphList* pGraphList);
	CGraphList* pGetSignalListDetails();
    //{{AFX_MSG(CGraphChildFrame)
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnClose();
	afx_msg LRESULT vUserCommand(WPARAM wParam, LPARAM lParam);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    // To save splitter postion
    void vSaveSplitterPostion();
    // Function to set Splitter postion
    void vSetSplitterPostion( );
    // To get Splitter postion
    void vGetSplitterStatus();
    // Splitter Window Objects
    CSplitterWnd m_wndSplitter, m_wndSecondSplitter;
    // For storing splitter window postion
    SGRAPHSPLITTERDATA m_sSplitterPostion;

    BOOL bGetDefaultSplitterPostion(CRect& omRect, 
                                    SGRAPHSPLITTERDATA& sGraphSplitterData);		

	//To store connect state
	BOOL m_bIsPrevConnected;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // 
