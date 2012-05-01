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
 * \file      FlickerFreeTree.cpp
 * \author    Ravi D.
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#include "Utils_stdafx.h"
#include "FlickerFreeTree.h"
#include "memdc.h"

BEGIN_MESSAGE_MAP(CFlickerFreeTree, CTreeCtrl)
    ON_WM_ERASEBKGND()
    ON_WM_SIZE()
    ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CFlickerFreeTree::OnEraseBkgnd(CDC* pDC)
{
    // TODO: Add your message handler code here and/or call default
    UNUSED_ALWAYS(pDC);
    //return CTreeCtrl::OnEraseBkgnd(pDC);
    return TRUE;
}

void CFlickerFreeTree::OnSize(UINT nType, int cx, int cy)
{
    CTreeCtrl::OnSize(nType, cx, cy);
    GetClientRect(&m_rectClient);
}
void CFlickerFreeTree::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: Add your message handler code here
    // Do not call CListCtrl::OnPaint() for painting messages
    // Paint to a memory device context to reduce screen flicker.
    CMemDC memDC(&dc, &m_rectClient);
    // To change the background replace it with approp. background color
    memDC.FillSolidRect( &m_rectClient, GetSysColor(COLOR_WINDOW));
    // Let the window do its default painting...
    CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
}
