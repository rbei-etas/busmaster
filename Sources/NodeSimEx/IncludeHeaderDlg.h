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
 * \file      IncludeHeaderDlg.h
 * \brief     This header file contains the defintion of class 
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the defintion of class 
 */
#if !defined(AFX_INCLUDEHEADERDLG_H__C0FAD574_3631_11D6_A628_00D0B76BEBF5__INCLUDED_)
#define AFX_INCLUDEHEADERDLG_H__C0FAD574_3631_11D6_A628_00D0B76BEBF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NodeSimEx_resource.h"

class CIncludeHeaderDlg : public CDialog
{
private:
    CFunctionEditorDoc* m_pDoc;
// Construction
public:
    CIncludeHeaderDlg(CFunctionEditorDoc* pDoc, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CIncludeHeaderDlg)
    enum { IDD = IDD_DLG_INCLUDE_HEADERS };
    CString m_omStrIncludeFilename;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CIncludeHeaderDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CIncludeHeaderDlg)
    afx_msg void OnOk();
    afx_msg void OnBrowse();
    afx_msg void OnCancel();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INCLUDEHEADERDLG_H__C0FAD574_3631_11D6_A628_00D0B76BEBF5__INCLUDED_)
