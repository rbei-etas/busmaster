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
 * \file      DefineTimerHandler.h
 * \brief     This file contain the definition all of CDefineTimerHandler
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition all of CDefineTimerHandler
 */

#pragma once

// costomised edit control class definition
#include "Utility/RadixEdit.h"
#include "FunctionEditorDoc.h"
#include "NodeSimEx_resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDefineTimerHandler dialog

class CDefineTimerHandler : public CDialog
{
// Construction
public:
    CString m_omStrTimerFunctionName;
    CDefineTimerHandler(CFunctionEditorDoc* pDoc, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CDefineTimerHandler)
    enum { IDD = IDD_DLG_DEFINE_TIMER_HANDLER };
    CRadixEdit  m_omEditTimerValue;
    CEdit   m_omEditTimerFuncName;
    CString m_omEditStrTimerFuncName;
    UINT    m_unEditTimerValue;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDefineTimerHandler)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CDefineTimerHandler)
    afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    CFunctionEditorDoc* m_pDoc;
};
