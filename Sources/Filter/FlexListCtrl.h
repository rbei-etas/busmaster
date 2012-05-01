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
 * \file      FlexListCtrl.h
 * \brief     This file contain the definition of CFlexListCtrl class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition of CFlexListCtrl class
 */

#pragma once

// FlexListCtrl.h : header file
#include "Utility/Utility_Structs.h"                 // For data struct definition
#include "Utility/ComboItem.h"
#include "Utility/NumEdit.h"
/////////////////////////////////////////////////////////////////////////////
// CFlexListCtrl window
class CFlexListCtrl : public CFFListCtrl
{
    // Construction
public:
    CFlexListCtrl();
    // Attributes
public:

    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CFlexListCtrl)
    //}}AFX_VIRTUAL

    // Implementation
public:
    // This is used to map the Row X Column 2d data into
    // one dimentional map key. This value should be greater then the maximum
    // Expected list column count
    void vSetMapColumnCount(int nColumn);
    // This will show appropriate controls to edit the column data. This will be
    // called when validation fails and to prompt to reenter the data
    void vGoToEditMode(int nItem, int nSubItem);
    // This is used to get the modification information stored during last
    // notification message. This will be useful to get Row & Column information
    // of the modified element
    BOOL sGetModificationStructure(LV_DISPINFO&);
    // This will show the editable controls over the list item. This will use
    // backend map to get the type and parameter information
    void vShowControl(int nItem, int nSubItem);
    // This will set the column type and edit control type
    void vSetColumnInfo(int nRow, int nColunm, SLISTINFO eType);
    // This will set numeric control parameter of a column
    void vSetNumericInfo(int nRow, int nColunm, const SNUMERICINFO& sInfo);
    // This will return the column type and edit control type
    SLISTINFO sGetColumnInfo(int nRow, int nColunm);
    // This function will create a editable combobox.
    CComboItem* pomComboList( int nItem, int nSubItem,
                              const CStringArray& omList);
    // This function will create a non-editable combobox.
    CComboItem* pomComboItem( int nItem, int nSubItem,
                              const CStringArray& omList);
    // This function will create a editbox.
    CEdit* pomEditItem(int nItem, int nSubItem);
    // This function will create a numeric editbox.
    CNumEdit* pomNumItem(int nItem, int nSubItem, const SNUMERICINFO& sInfo);
    // This will set the user program informatrion of a column
    void vSetUserProgInfo( int nRow, int nColunm,
                           const SUSERPROGINFO& sUSerProgInfo);
    // Destructor
    virtual ~CFlexListCtrl();

    // Generated message map functions
protected:
    //{{AFX_MSG(CFlexListCtrl)
    afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnDoubleClick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
    //}}AFX_MSG
private:
    // Signle Click edit or double click edit
    BOOL            m_bSingleClickActivate;
    // CMap for list item types
    CListTypes      m_omListItemType;
    // CMap for numeric parameters
    CNumericInfo    m_omNumDetails;
    // CMap for User Program information
    CUserProgList   m_omUserProg;
    // Structure to store last modified information
    LV_DISPINFO     m_sModifiedInfo;
    // Flag to indicate that controls are being created. This is to avoid
    // validation while loading the signal list
    BOOL            m_bCreating;
    // This is to store virtual column count
    int             m_nVirtualColumnCount;

    // Private Functions
    long lGetMapID(int nRow, int nCol);
    DECLARE_MESSAGE_MAP()
};
