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
 * \file      Properties.h
 * \brief     This file contain the definition of CProperties class
 * \author    Amitesh Bharti
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition of CProperties class
 */
#if !defined(AFX_Properties_H__64B10A15_0100_11D7_8071_0010B599CE39__INCLUDED_)
#define AFX_Properties_H__64B10A15_0100_11D7_8071_0010B599CE39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Properties.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProperties dialog

class CProperties : public CDialog
{

public:
    // standard constructor
    CProperties(const SPROPERTIESLIST& sProperties);

// Dialog Data
    //{{AFX_DATA(CProperties)
    enum { IDD = IDD_DLG_PROPERTIES };
    CTreeCtrl   m_omPropertyTree;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CProperties)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CProperties)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    CImageList m_omTreeImageList;
    SPROPERTIESLIST m_sProperties;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Properties_H__64B10A15_0100_11D7_8071_0010B599CE39__INCLUDED_)
