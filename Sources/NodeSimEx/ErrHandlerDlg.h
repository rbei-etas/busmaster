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
 * \file      ErrHandlerDlg.h
 * \brief     This file contain the definition of CErrHandlerDlg class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition of CErrHandlerDlg class
 */

#pragma once

#include "NodeSimEx_resource.h"
/////////////////////////////////////////////////////////////////////////////
// CErrHandlerDlg dialog

class CErrHandlerDlg : public CDialog
{
    // Construction
public:
    CStringArray m_omStrArrayErrorHandler;
    // standard constructor
    CErrHandlerDlg(CFunctionEditorDoc* pDoc, CWnd* pParent = NULL,BOOL bIsDelete = FALSE);

    // Dialog Data
    //{{AFX_DATA(CErrHandlerDlg)
    enum { IDD = IDD_DLG_ERROR_HANDLER };
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CErrHandlerDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CErrHandlerDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnCbtnErrorCancel();
    afx_msg void OnCbtnErrorOk();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    CFunctionEditorDoc* m_pDoc;
    BOOL m_bIsDelete;
};
