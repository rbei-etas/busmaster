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
 * \file      Selectdb.h
 * \brief     Interface file for CSelectdb class
 * \author    Amit Ranjan
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CSelectdb class
 */

#if !defined(AFX_SELECTDB_H__EDFDED33_A7AE_11D8_90DB_0010B5A6E0E0__INCLUDED_)
#define AFX_SELECTDB_H__EDFDED33_A7AE_11D8_90DB_0010B5A6E0E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Selectdb.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectdb dialog

class CSelectdb : public CDialog
{
// Construction
public:
    void SaveSettings();
    CSelectdb(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CSelectdb)
    enum { IDD = IDD_DLG_ASSIGN_DB };
    BOOL	m_option;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSelectdb)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CSelectdb)
    afx_msg void OnBrowseCANoeDb();
    afx_msg void OnBrowseBUSMASTERDb();
    afx_msg void OnChkbOption();
    virtual void OnOK();
    afx_msg void OnChangeEditAdcanoe();
    virtual void OnCancel();
    afx_msg void OnClose();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTDB_H__EDFDED33_A7AE_11D8_90DB_0010B5A6E0E0__INCLUDED_)
