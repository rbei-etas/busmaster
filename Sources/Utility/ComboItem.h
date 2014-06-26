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
 * @brief This file contains definition of CComboItem class
 * @author Ravi D
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contains definition of CComboItem class
 */

#pragma once

class CComboItem : public CComboBox
{
public:
    CComboItem( int nItem,              // Row
                int nSubItem,           // Sub Item
                const CStringArray& psList,   // List of strings to populate list box
                const CString& omStrText,      // Selected text
                BOOL bIsEditable);      // Editable list or not

    virtual ~CComboItem();

    virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
    CStringArray m_sList;
    int m_nSubItem;
    int m_nItem;
    BOOL m_bVK_ESCAPE;
    BOOL m_bIsEditable;
    CString omStrText;

    afx_msg void OnNcDestroy();
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnCloseup();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

    DECLARE_MESSAGE_MAP()
};
