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
 * \file      NumEdit.h
 * \brief     Definition file for CNumEdit
 * \author    Ravi D
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for CNumEdit
 */

#pragma once

#include "Utility_structs.h"
#include "RadixEdit.h"
#include "NumSpinCtrl.h"
class CNumEdit : public CRadixEdit
{
// Construction
public:
    CNumEdit(int nItem, int nSubItem, CString &sContent, const SNUMERICINFO& sInfo);

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CEditItem)
    public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CNumEdit();

    // Generated message map functions
protected:
    int     m_nItem;            // Item Index
    int     m_nSubItem;         // Sub Item index
    CString m_sContent;         // Initial text
    CNumSpinCtrl *m_pomSpin;    // Pointer to Buddy control
    BOOL    m_bVK_ESCAPE;       // To store the Cancel status
    SNUMERICINFO    m_sInfo;    // To store numeric information
    //{{AFX_MSG(CEditItem)
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnNcDestroy();
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};
