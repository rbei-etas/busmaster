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
 * \file      CEnvVarHandlerDlg.h
 * \brief     Interface file for CEnvVarHandlerDlg class
 * \author    Amit Ranjan
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CEnvVarHandlerDlg class
 */

#if !defined(AFX_LIST_H__0AAD69D1_73D0_11D8_90D3_0010B5A6E0E0__INCLUDED_)
#define AFX_LIST_H__0AAD69D1_73D0_11D8_90D3_0010B5A6E0E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CEnvVarHandlerDlg.h : header file
//
#include "CAPL2CConverter_Resource.h"
/////////////////////////////////////////////////////////////////////////////
// CEnvVarHandlerDlg dialog

class CEnvVarHandlerDlg : public CDialog
{
    // Construction
public:
    CEnvVarHandlerDlg(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CEnvVarHandlerDlg)
    enum { IDD = IDD_DLG_USER_INTER };
    CListBox    m_UnSupKeys;
    CListBox    m_AltKeys;
    CListBox    m_EquiKeys;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CEnvVarHandlerDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void OnCancel();
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CEnvVarHandlerDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnAdd();
    afx_msg void OnTerminate();
    afx_msg void OnRemove();
    afx_msg void OnClose();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LIST_H__0AAD69D1_73D0_11D8_90D3_0010B5A6E0E0__INCLUDED_)
