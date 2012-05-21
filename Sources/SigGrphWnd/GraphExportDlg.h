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
 * \file      GraphExportDlg.h
 * \brief     Interface file for CGraphExportDlg class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CGraphExportDlg class
 */

#pragma once

class CGraphExportDlg : public CDialog
{
    // Construction
public:
    CGraphExportDlg(CWnd* pParent = NULL);   // standard constructor
    // Dialog Data
    //{{AFX_DATA(CGraphExportDlg)
    enum { IDD = IDD_DLG_GRAPH_EXPORT };
    CString m_omStrCSVFileName;
    CString m_omStrHTMLFileName;
    CString m_omStrBMPFileName;
    IDMGraphCtrl* m_pDMGraphCtrl;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CGraphExportDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CGraphExportDlg)
    afx_msg void OnBtnBrowseCsv();
    afx_msg void OnBtnSaveCsv();
    afx_msg void OnBtnBrowseHtml();
    afx_msg void OnBtnSaveHtml();
    afx_msg void OnBtnBrowseBmp();
    afx_msg void OnBtnSaveImage();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};
