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
 * \file      SignalWatchListBox.h
 * \brief     Interface file for CSignalWatchListBox class
 * \authors   RBIN/EMC2 - Ratnadip Choudhury, Amarnath Shastri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CSignalWatchListBox class
 */

#pragma once

// For Flicker Free List Control Definiton
#include "Utility/FFListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CSignalWatchListBox window

class CSignalWatchListBox : public CFFListCtrl
{
// Construction
private:
    HWND m_hParentWnd;
public:
    CSignalWatchListBox();
    void vUpdateParentWnd(HWND hParentWnd);
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSignalWatchListBox)
    //}}AFX_VIRTUAL

// Implementation
    virtual ~CSignalWatchListBox();

    // Generated message map functions
protected:
    //{{AFX_MSG(CSignalWatchListBox)
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnSgWatchClear();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};
