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
 * \file      AlphanumiricEdit.h
 * \brief     This header file contains the defintion of class
 * \author    Anish Kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the defintion of class
 */
#pragma once


// CAlphaCharEdit

class CAlphaCharEdit : public CEdit
{
    DECLARE_DYNAMIC(CAlphaCharEdit)
    int m_nRow;
    int m_nColumn;
    BOOL    m_bVK_ESCAPE;
    CString m_strContent;
public:
    CAlphaCharEdit(int nItem, int nSubItem, CString& strContent);
    virtual ~CAlphaCharEdit();

protected:
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    int OnCreate(LPCREATESTRUCT lpCreateStruct);

};


