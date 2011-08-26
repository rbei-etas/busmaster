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
 * \file      UtlityDlg.h
 * \brief     This header file contains the defintion of class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the defintion of class
 */
#if !defined(AFX_UTLITYDLG_H__BEB9A628_2CC3_11D6_A620_00D0B76BEBF5__INCLUDED_)
#define AFX_UTLITYDLG_H__BEB9A628_2CC3_11D6_A620_00D0B76BEBF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UtlityDlg.h : header file
//

#include "NodeSimEx_resource.h"

/////////////////////////////////////////////////////////////////////////////
// CUtlityDlg dialog

class CUtlityDlg : public CDialog
{
// Construction
private:
    CFunctionEditorDoc* m_pDoc;
public:
    CString m_omStrFunPrototype;
    CUtlityDlg(CFunctionEditorDoc* pDoc, CWnd* pParent = NULL);   // standard constructor
	void vSetUtilFuncChange(CString omStrReturnType);

// Dialog Data
    //{{AFX_DATA(CUtlityDlg)
	enum { IDD = IDD_DLG_DEFINE_UTILITY_FN };
	CEdit	m_omEditUtilsFunText;
    CComboBox   m_omComboRetType;
    CString m_omStrFunctionName;
	CString	m_omStrUtilsFunText;
	//}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CUtlityDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CUtlityDlg)
    afx_msg void OnOk();
    virtual BOOL OnInitDialog();
    afx_msg void OnChangeEditFnPrototype();
    afx_msg void OnCancel();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
	BOOL m_bChangeUtilFunc;
	CString m_omStrTempReturnType;
public:
	afx_msg void OnCbnSelchangeCombFnReturnType();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UTLITYDLG_H__BEB9A628_2CC3_11D6_A620_00D0B76BEBF5__INCLUDED_)
