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
 * \file      FFListCtrl.cpp
 * \brief     This file contain the definition all member function of
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition all member function of
 */
// For all standard header file include
#include "Utils_stdafx.h"
// Definition of App class
// For Flicker Free List Control Definition
#include "FFListCtrl.h"
// For Double Buffer Definition
#include "OffScreenDC.h"

/////////////////////////////////////////////////////////////////////////////
// CFFListCtrl

CFFListCtrl::CFFListCtrl()
{
}

CFFListCtrl::~CFFListCtrl()
{
}


BEGIN_MESSAGE_MAP(CFFListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CFFListCtrl)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFFListCtrl message handlers

/******************************************************************************
 Function Name  :   OnSize

 Input(s)       :   -
 Output         :   -
 Functionality  :   This function will be called by framewrok to take approp.
                    action while resizing the window. This function will update
                    the screen rectangle
 Member of      :   CFFListCtrl

 Author(s)      :   Raja N
 Date Created   :   23.06.2004
 Modification   :
******************************************************************************/
void CFFListCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CListCtrl::OnSize(nType, cx, cy);
	vInitMemDCRect();
}

/******************************************************************************
 Function Name  :   OnEraseBkgnd
 Functionality  :   This function will be called by framewrok to erase the
                    background of the client area. Paint takes care of painting
                    the background so rerutn TRUE.
 Member of      :   CFFListCtrl

 Author(s)      :   Raja N
 Date Created   :   23.06.2004
 Modification   :
******************************************************************************/
BOOL CFFListCtrl::OnEraseBkgnd(CDC* pDC) 
{
    // To avoid compilation warning
    UNUSED_ALWAYS(pDC);
    return TRUE;
}

/******************************************************************************
 Function Name  :   OnPaint

 Input(s)       :   -
 Output         :   -
 Functionality  :   This function will be called by framewrok to paint the
                    client area. The double buffering concept is used to avoid
                    flickering of display
 Member of      :   CFFListCtrl

 Author(s)      :   Raja N
 Date Created   :   23.06.2004
 Modification   :
******************************************************************************/
void CFFListCtrl::OnPaint() 
{
    CPaintDC dc(this); // device context for painting
    // Create Offscreen Buffer
    COffScreenDC memDC(&dc, &m_rectClient);
	// Do not call CListCtrl::OnPaint() for painting messages
    CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
}

/******************************************************************************
 Function Name  :   vInitMemDCRect

 Input(s)       :   -
 Output         :   -
 Functionality  :   This function will update the screen rectangle. This will
                    shift the display down to exclude the header control area
 Member of      :   CFFListCtrl

 Author(s)      :   Raja N
 Date Created   :   23.06.2004
 Modification   :
******************************************************************************/
VOID CFFListCtrl::vInitMemDCRect()
{
    // Get the screen rectangle
	GetClientRect(&m_rectClient);
    CHeaderCtrl* pHC;
    // Get the Header Control
    pHC = GetHeaderCtrl();
    if (pHC != NULL)
    {
        CRect rectHeader;
        // Get the size of Header
        BOOL bSuccess = pHC->GetItemRect( 0, &rectHeader );
        if(bSuccess == TRUE )
        {
            // Shift the screen rectangle down
            m_rectClient.top += rectHeader.bottom;
            m_rectClient.bottom += rectHeader.bottom;
        }
    }
}