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
 * \file      GraphChildFrm.h
 * \brief     Interface file for CGraphChildFrame class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CGraphChildFrame class
 */

#pragma once

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
