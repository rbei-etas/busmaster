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
 * \file      SelectFunctions.h
 * \brief     This header file contains the defintion of class       
 * \author    Amarnath Shastry
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the defintion of class       
 */

#pragma once

#include "NodeSimEx_resource.h"
#include "Include/BaseDefs.h"
/////////////////////////////////////////////////////////////////////////////
// CSelectFunctions dialog

class CSelectFunctions : public CDialog
{
// Construction
private:
    ETYPE_BUS m_eBus;
public:
    CString m_omSelectedFunctionPrototype;
    CString m_omStrSelectedText;
    CSelectFunctions(ETYPE_BUS eBus, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CSelectFunctions)
    enum { IDD = IDD_DLG_SELECT_FNS };
    CListBox    m_omFnListBox;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSelectFunctions)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CSelectFunctions)
    afx_msg void OnDblclkListFunctions();
    afx_msg void OnSelect();
    virtual BOOL OnInitDialog();
    afx_msg void OnCancel();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
	BOOL bFormatFunctionPrototype(CString& omStrFunctionPrototype);
};
