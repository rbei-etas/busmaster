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
 * \file      SimSysWnd.h
 * \brief     Interface file for CSimSysWnd class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CSimSysWnd class
 */

#pragma once

#include "Sim_MDIChildBase.h"
/////////////////////////////////////////////////////////////////////////////
// CSimSysWnd frame

class CSimSysWnd : public CMDIChildBase
{
	DECLARE_DYNCREATE(CSimSysWnd)
protected:
	
	
	// Attributes
public:
	
	// Operations
public:
	// Default constructor
	CSimSysWnd(ETYPE_BUS eBus = CAN);
	// Standard destructor
	virtual ~CSimSysWnd();
	// To update window size and splitter after loading a conf file
	void vUpdateWinStatus();
	// To save window size and splitter postion before unloading a conf file
	void vUpdateWndCo_Ords();
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimSysWnd)
protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSimSysWnd)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	ETYPE_BUS m_eBus;
    BOOL m_bSplitWndCreated;
    CSplitterWnd m_omSplitterWnd;
    // To calculate the splitter position when the window is resized.
	void vCalculateSplitterPosition(CSize& cSize);
    void vSaveSimSysFIles();
    
public:
    WINDOWPLACEMENT m_wWndPlacement;
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
