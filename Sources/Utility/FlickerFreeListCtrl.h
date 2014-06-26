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
 * @brief Definition file for CFlickerFreeTree class.
 * @author Ravi D.
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for CFlickerFreeTree class.
 */

#pragma once

class CFlickerFreeListCtrl : public CListCtrl
{
public:
    CFlickerFreeListCtrl();
    virtual ~CFlickerFreeListCtrl();

    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg void OnSize(UINT nType, int cx, int cy);

    CRect m_rectClient;

protected:
    afx_msg UINT OnGetDlgCode( );

    DECLARE_MESSAGE_MAP()

private:
    DECLARE_DYNAMIC(CFlickerFreeListCtrl)
};
