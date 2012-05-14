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
	CDC pDC;
    pDC.CreateCompatibleDC(&dc);
	CBitmap*	m_oldBitmap;
	CBitmap m_NewBitmap;

	BOOL bSuccess = m_NewBitmap.CreateCompatibleBitmap(&dc, m_rectClient.Width(), m_rectClient.Height());
 
    m_oldBitmap = pDC.SelectObject(&m_NewBitmap);

    pDC.FillSolidRect( &m_rectClient, GetSysColor(COLOR_WINDOW));

	CWnd::DefWindowProc( WM_PAINT, (WPARAM)pDC.m_hDC, 0 );

    dc.BitBlt(m_rectClient.left, m_rectClient.top, m_rectClient.Width(), m_rectClient.Height(), 
					&pDC, m_rectClient.left, m_rectClient.top, SRCCOPY);

	pDC.SelectObject(m_oldBitmap);
}