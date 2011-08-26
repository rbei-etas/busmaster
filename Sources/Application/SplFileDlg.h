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
 * \file      SplFileDlg.h
 * \brief     This file contain the definition of SplFileDlg class. This class is
 * \author    Amitesh Bharti
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition of SplFileDlg class. This class is
 */
#if !defined(AFX_SPLFILEDLG_H__94E95C59_363E_11D6_BFEF_0010B599CE39__INCLUDED_)
#define AFX_SPLFILEDLG_H__94E95C59_363E_11D6_BFEF_0010B599CE39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SplFileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSplFileDlg dialog

class CSplFileDlg : public CFileDialog
{
    DECLARE_DYNAMIC(CSplFileDlg)

public:
    CSplFileDlg(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
                LPCTSTR lpszDefExt = NULL,
                LPCTSTR lpszFileName = NULL,
                DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
                LPCTSTR lpszFilter = NULL,
                CWnd* pParentWnd = NULL,
                CString omStrButtonCaption = "Select");

protected:
    //{{AFX_MSG(CSplFileDlg)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    CString m_omStrButtonCaption;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLFILEDLG_H__94E95C59_363E_11D6_BFEF_0010B599CE39__INCLUDED_)
