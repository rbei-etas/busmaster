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
 * \file      MessageList.h
 * \brief     Interface file for CMessageList class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CMessageList class
 */

#pragma once

// Definition of Flicker Free List Control
#include "Utility/FFListCtrl.h"

class CMessageList : public CFFListCtrl
{
private:
    // Row String
    CString m_omStrInARow;
    // GDI Objects for background and selection brush
    HBRUSH m_hWhiteBrush, m_hBlueBrush;

    // Construction
public:
    CMessageList();

    // Attributes
public:

    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMessageList)
    //}}AFX_VIRTUAL

protected:
    struct ColumnTitleState
    {
        ColumnTitleState()
            :m_bVisible(false)
            ,m_nOrigPosition(-1)
            ,m_nOrigWidth(0)
        {}
        bool m_bVisible;
        int  m_nOrigWidth;      // Width of the column before being hidden
        int  m_nOrigPosition;   // Position of the column before being hidden
    };

    void InsertColumnTitleState(int nCol, bool bVisible, int nOrgWidth = 0);
    void DeleteColumnTitleState(int nCol);
    ColumnTitleState& GetColumnTitleState(int nCol);
    int GetColumnTitleStateCount();
    CArray<ColumnTitleState, ColumnTitleState>  m_ColumnTitleStates;
public:
    bool IsColumnShown(int nCol);

    // Implementation
public:
    virtual ~CMessageList();
    BOOL MakeColumnVisible(int nCol, bool bShow);

    // Generated message map functions
protected:
    // Overwride to implement custom draw
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual afx_msg void OnContextMenu(CWnd*, CPoint point);
    virtual afx_msg LRESULT OnInsertColumn(WPARAM wParam, LPARAM lParam);
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    virtual afx_msg BOOL OnHeaderEndDrag(UINT id, NMHDR* pNmhdr, LRESULT* pResult);
    virtual afx_msg BOOL OnHeaderBeginResize(UINT id, NMHDR* pNmhdr, LRESULT* pResult);

    //{{AFX_MSG(CMessageList)
    afx_msg void OnDestroy();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};
