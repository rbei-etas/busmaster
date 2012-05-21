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
 * \file      SimSysDetView.h
 * \brief     This header file contains the defintion of class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the defintion of class
 */
#if !defined(AFX_SIMSYSDETVIEW_H__009CBAA8_59D7_4E52_86CF_B51B2C8A14F8__INCLUDED_)
#define AFX_SIMSYSDETVIEW_H__009CBAA8_59D7_4E52_86CF_B51B2C8A14F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SimSysDetView.h : header file
//
#include "Utility/RadixEdit.h"
#include "Include/BaseDefs.h"
#include "SimSysNodeInfo.h"
#include "NodeSimEx_resource.h"
/////////////////////////////////////////////////////////////////////////////
// CSimSysDetView form view


class CSimSysDetView : public CFormView
{
protected:
	CSimSysDetView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSimSysDetView)
		
		// Form Data
public:
	//{{AFX_DATA(CSimSysDetView)
	enum { IDD = IDD_DLG_SIMSYSDETVIEW };
	CListCtrl	m_omListCtrlHanDet;
	CListCtrl	m_omListCtrlHanVal;
	CString	m_omStrNodeName;
	CString	m_omStrCFile;
	//}}AFX_DATA
	
	// Attributes
public:
	
	// Operations
public:
    //BOOL bCreateNewFile( CString omStrFileName );
	void vEnableButtons (BOOL bEnable );
    BOOL bGetControlStatus();
    void vHideControls(UINT unEnableHide);
	void vChangeLUButtonText(BOOL bIsLoaded);
	
	void vChangeEDAllHanButtonText(BOOL bIsEnabled);
    
    void vDisplayNodeInformation(PSNODEINFO pNodeInfo);
    void vEnableHandlerButtons( BOOL bIsEnabled );
	void vGetNodeInfo(PSNODEINFO psNodeInfo);
    void vUpdateHandlerList();
	void vSetNodeAddress(BYTE byAddress);
	void vUpdateHandlerEnableDisableStatus(int nItem, BOOL bEnableHandler);
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimSysDetView)
public:
	virtual void OnInitialUpdate();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	virtual ~CSimSysDetView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
	// Generated message map functions
	//{{AFX_MSG(CSimSysDetView)
	afx_msg void OnButtonBuild();
	afx_msg void OnButtonBuildandload();
	afx_msg void OnButtonEditfile();
	afx_msg void OnButtonEnableDisableallhandlers();
	afx_msg void OnButtonEnableDisablehandler();
	afx_msg void OnButtonLoadUnload();
	afx_msg void OnButtonOpenfile();
	afx_msg void OnClickLstcHandlerDetails(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnItemchangedLstcHandlerDetails(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    ETYPE_BUS m_eBus;
    CStringList m_omSourceCodeTextList;
    CStringArray m_omIncludeFileArray ;
    DWORD m_dwSourceCodeLineNo;
	PSNODEINFO m_psNodeInfo;
    BOOL bConnected;   
	BOOL m_bIsTimerHanEnabled;
	BOOL m_bIsKeyHanEnabled;
	BOOL m_bIsMsgHanEnabled;
	BOOL m_bIsErrorHanEnabled;
    
	BOOL m_bAreControlsHidden;
	BOOL m_bIsDllLoadSuccess; 
	void vAddItemToHanDetList( sNODEINFO* pNode );
	void vEnableHandlerDetails( BOOL bIsEnabled );
    void vUpDownArrowKeySelection(int nSel);
    void vSetNodeInfoEDHanStatus( int nSelItem , BOOL bIsEnabled );
    BOOL bUpdateNodeInfoFile(CString omStrCFile);
    
    void vChangeEDHanButtonText(BOOL bIsEnabled);
    void vUpdateNodeDetailsAndView();
    void vSetHandlerDetailRowText(int nRow, int nNoOfHandler, BOOL bEnabled);
public:
    static ETYPE_BUS CSimSysDetView::sm_eBus;
    //afx_msg void OnBnClickedStatNodeDetails();
    CRadixEdit m_omPrefAdres;
    CRadixEdit m_omECU_NAME;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMSYSDETVIEW_H__009CBAA8_59D7_4E52_86CF_B51B2C8A14F8__INCLUDED_)
