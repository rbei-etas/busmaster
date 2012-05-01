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
 * \file      OwnerDataListCtrl.h
 * \brief     Interface file for COwnerDataListCtrl class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for COwnerDataListCtrl class
 */

#pragma once

#include "Utility/FFListCtrl.h" // For flicker free list class

#define WM_OWNER_GET_DATA         WM_USER + 151
#define WM_OW_LIST_DBLCLK         WM_USER + 152

class COwnerDataListCtrl : public CFFListCtrl
{
    // Construction
public:
    COwnerDataListCtrl();
    // Attributes
public:

    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COwnerDataListCtrl)
    //}}AFX_VIRTUAL

    // Implementation
public:
    virtual ~COwnerDataListCtrl();

    // Generated message map functions
protected:
    //{{AFX_MSG(COwnerDataListCtrl)
    afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
private:
    // Methods
    // Data
};
