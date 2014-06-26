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
 * @brief This header file contains the defintion of class
 * @author Anish Kumar
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the defintion of class
 */

#pragma once

class CAlphanumiricEdit : public CEdit
{
public:
    CAlphanumiricEdit(int nItem = -1, int nSubItem = -1);
    virtual ~CAlphanumiricEdit();

    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKillFocus(CWnd* pNewWnd);

protected:
    DECLARE_MESSAGE_MAP()

private:
    DECLARE_DYNAMIC(CAlphanumiricEdit)
    int m_nRow;
    int m_nColumn;
    BOOL m_bVK_ESCAPE;
};
