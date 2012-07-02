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
 * \file      EditItem.h
 * \brief     This header file contains the defintion of class EditItem.
 * \authors   Zafir Anjum
  *
 * This header file contains the defintion of class EditItem.
 */

/*  Code contained in this file is taken from codeguru article
    http://www.codeguru.com/Cpp/controls/listview/editingitemsandsubitem/article.php/c923
    Written by Zafir Anjum  */

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CEditItem window

class CEditItem : public CEdit
{
    // Construction
public:
    CEditItem(int nItem, int nSubItem, const CString& sContent);

    // Attributes
public:

    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CEditItem)
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    //}}AFX_VIRTUAL

    // Implementation
public:
    virtual ~CEditItem();

    // Generated message map functions
protected:
    // To keep the Item index
    int     m_nItem;
    // To keep the SubItem index
    int     m_nSubItem;
    // To store the content as text
    CString m_sContent;
    // Canceled through ESCAPE key
    BOOL    m_bVK_ESCAPE;
    //{{AFX_MSG(CEditItem)
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnNcDestroy();
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};
