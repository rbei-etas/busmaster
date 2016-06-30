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
 * @brief Definition file for CKeyNumEdit
 * @author Ravi D
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for CKeyNumEdit
 */

#pragma once

#include "Utility_structs.h"
#include "RadixEdit.h"
#include "NumSpinCtrl.h"

class CKeyNumEdit : public CRadixEdit
{
public:
    CKeyNumEdit(int nItem, int nSubItem, CString& sContent, const SNUMERICINFO& sInfo);

    virtual ~CKeyNumEdit();

    virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
    /** Item Index */
    int m_nItem;

    /** Sub Item index */
    int m_nSubItem;

    /** Initial text */
    CString m_sContent;

    /** Pointer to Buddy control */
    CNumSpinCtrl* m_pomSpin;

    /** To store the Cancel status */
    BOOL m_bVK_ESCAPE;

    /** To store numeric information */
    SNUMERICINFO m_sInfo;

    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnNcDestroy();
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg UINT OnGetDlgCode ();

    DECLARE_MESSAGE_MAP()
};
