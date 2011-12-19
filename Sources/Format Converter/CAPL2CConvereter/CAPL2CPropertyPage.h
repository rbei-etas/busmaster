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
 * \file      ConverterDlg.h
 * \brief     Interface file for CCAPL2CPropertyPage class
 * \author    Amit Ranjan
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CCAPL2CPropertyPage class
 */


#if !defined(AFX_KEYOPTIONDLG_H__0AAD69C7_73D0_11D8_90D3_0010B5A6E0E0__INCLUDED_)
#define AFX_KEYOPTIONDLG_H__0AAD69C7_73D0_11D8_90D3_0010B5A6E0E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCAPL2CPropertyPage dialog
#include "resource.h"
class CCAPL2CPropertyPage : public CPropertyPage
{
// Construction
public:
    void SaveSettings();
    CCAPL2CPropertyPage(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
    //{{AFX_DATA(CCAPL2CPropertyPage)
    enum { IDD = IDD_DLG_LANG_CNVRTR };
    BOOL	m_check;
    BOOL	m_savedb;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCAPL2CPropertyPage)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CCAPL2CPropertyPage)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnBrowseSource();
    afx_msg void OnBrowseDest();
    afx_msg void OnShowLog();
    afx_msg void OnConvert();
    afx_msg void OnCancel();
    afx_msg void OnChangeEditInput();
    afx_msg void OnChkbOptn();
    afx_msg void OnBrowseCANoeDb();
    afx_msg void OnBrowseBUSMASTERDb();
    afx_msg void OnChkbSavedb();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYOPTIONDLG_H__0AAD69C7_73D0_11D8_90D3_0010B5A6E0E0__INCLUDED_)
