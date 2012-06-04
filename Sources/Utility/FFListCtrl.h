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

#pragma once

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
