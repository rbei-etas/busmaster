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
 * \file      OutWnd.h
 * \brief     This file contain the definition COutWnd class. The output of
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition COutWnd class. The output of
 */

#pragma once

#include "Sim_MDIChildBase.h"       // For Common MDI base class

class COutWnd : public CMDIChildBase
{
    DECLARE_DYNCREATE(COutWnd)
public:
    COutWnd(ETYPE_BUS eBus = CAN);           // public constructor used by dynamic creation
// Attributes
public:

// Operations
public:
    BOOL bCreateOutputWindow();
    // Add string items to the list box of output window
    BOOL bAddString(CStringArray& omStrArray);
	// Set the content of list box.
    VOID vResetContent();
    // Set the content of list box.

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COutWnd)
    public:
    protected:
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
    //}}AFX_VIRTUAL

// Implementation
protected:
    virtual ~COutWnd();

    // Generated message map functions
    //{{AFX_MSG(COutWnd)
    afx_msg void OnDestroy();
    afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnDbClick();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    ETYPE_BUS m_eBus;
    void vGetWinStatus(WINDOWPLACEMENT& sWinCurrStatus);
    void vSaveWinStatus(WINDOWPLACEMENT sWinCurrStatus);
    CSize m_omSizeMaxTxtExtent;
    CFont m_omNewFont;
    CListBox m_omListBox;
    // Set the font of window
    VOID vSetWindowFont();
};
