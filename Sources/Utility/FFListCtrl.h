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
 * \file      FFListCtrl.h
 * \brief     This file contain the definition of CFFListCtrl class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition of CFFListCtrl class
 */

#if !defined(AFX_FFLISTCTRL_H__7E863E67_7E2C_4E67_9628_DC9F9D6CC6D0__INCLUDED_)
#define AFX_FFLISTCTRL_H__7E863E67_7E2C_4E67_9628_DC9F9D6CC6D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FFListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFFListCtrl window

class CFFListCtrl : public CListCtrl
{
    // Screen Rectangle
    CRect m_rectClient;
// Construction
public:
    CFFListCtrl();
    VOID vInitMemDCRect();
// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFFListCtrl)
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CFFListCtrl();

    // Generated message map functions
protected:
    //{{AFX_MSG(CFFListCtrl)
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FFLISTCTRL_H__7E863E67_7E2C_4E67_9628_DC9F9D6CC6D0__INCLUDED_)
