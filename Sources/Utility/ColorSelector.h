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
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "afxwin.h"
#include "afxdlgs.h"

#define WM_CPN_COLORSELECTED (WM_USER + 55)

class CColorSelector : public CButton
{
public:
    CColorSelector();
    virtual ~CColorSelector();

    COLORREF GetColour()
    {
        return m_omColorBkg;
    };

    void SetColour(COLORREF omColor);

    void SetSelectedIndex(INT nIndex);

    CMap<UINT,UINT, COLORREF, COLORREF> m_MapMsgIdToClr;

    COLORREF GetColourForSelectedIndex(INT nSelectedIndex);

    afx_msg virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    afx_msg void OnBnClicked();

protected:
    DECLARE_MESSAGE_MAP()

    virtual void PreSubclassWindow();

private:
    COLORREF m_omColorBkg;
    INT m_nSelIndex;
};
