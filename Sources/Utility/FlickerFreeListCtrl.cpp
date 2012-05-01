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
 * \file      FlickerFreeListCtrl.cpp
 * \author    Ravi D
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#include "Utils_stdafx.h"
#include "FlickerFreeListCtrl.h"
#include "memdc.h"

// CFlickerFreeListCtrl

IMPLEMENT_DYNAMIC(CFlickerFreeListCtrl, CListCtrl)
/**********************************************************************************
Function Name   :   CFlickerFreeListCtrl
Input(s)        :   -
Output          :   -
Functionality   :   Constructor
Member of       :   CFlickerFreeListCtrl
Friend of       :   -
Authors         :   Anupam
Date Created    :   21/02/2007
Modifications   :   -
************************************************************************************/

CFlickerFreeListCtrl::CFlickerFreeListCtrl()
{
    m_rectClient = CRect(0,0,0,0);
}


/**********************************************************************************
Function Name   :   ~CFlickerFreeListCtrl
Input(s)        :   -
Output          :   -
Functionality   :   Destructor
Member of       :   CFlickerFreeListCtrl
Friend of       :   -
Authors         :   Ravi D
Date Created    :   19/09/2006
Modifications   :   -
************************************************************************************/
CFlickerFreeListCtrl::~CFlickerFreeListCtrl()
{
}


BEGIN_MESSAGE_MAP(CFlickerFreeListCtrl, CListCtrl)
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    ON_WM_SIZE()
    //ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
    //ON_WM_LBUTTONDBLCLK()
    ON_WM_GETDLGCODE()
END_MESSAGE_MAP()

UINT CFlickerFreeListCtrl::OnGetDlgCode( )
{
    UINT result = CListCtrl::OnGetDlgCode();
    result = result | DLGC_WANTALLKEYS;
    return(result);
}


// CFlickerFreeListCtrl message handlers

/**********************************************************************************
Function Name   :   OnEraseBkgnd
Input(s)        :   pDC - device context
Output          :   true - to erase the window background
                    flase - otherwise
Functionality   :   To paint the window background
Member of       :   CFlickerFreeListCtrl
Friend of       :   -
Authors         :   Ravi D
Date Created    :   19/09/2006
Modifications   :   -
************************************************************************************/
BOOL CFlickerFreeListCtrl::OnEraseBkgnd(CDC* /*pDC*/)
{
    // TODO: Add your message handler code here and/or call default
    //return CListCtrl::OnEraseBkgnd(pDC);
    return TRUE;
}


/**********************************************************************************
Function Name   :   OnPaint
Input(s)        :   -
Output          :   -
Functionality   :   To paint the window
Member of       :   CFlickerFreeListCtrl
Friend of       :   -
Authors         :   Ravi D
Date Created    :   19/09/2006
Modifications   :   -
************************************************************************************/
void CFlickerFreeListCtrl::OnPaint()
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

/**********************************************************************************
Function Name   :   OnSize
Input(s)        :   nType - type of rezsizing
                    cx - width of the client area
                    cy - height of trhe client area
Output          :   -
Functionality   :   To resize the window controls after the window has resized
Member of       :   CFlickerFreeListCtrl
Friend of       :   -
Authors         :   Ravi D
Date Created    :   19/09/2006
Modifications   :   -
************************************************************************************/
void CFlickerFreeListCtrl::OnSize(UINT nType, int cx, int cy)
{
    CListCtrl::OnSize(nType, cx, cy);
    // TODO: Add your message handler code here
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
#if 0
void CFlickerFreeListCtrl::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult)
{
    CWnd* pParent = GetParent();

    if( pParent != NULL && pParent->m_hWnd != 0 )
    {
        ::SendMessage( pParent->m_hWnd,
                       WM_OWNER_DATA_NOTIFICATION,
                       (WPARAM)pNMHDR,
                       (LPARAM)pResult );
    }
}

void CFlickerFreeListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // Send corresponding message to parent window with current selection
    INT nItem = GetNextItem(-1, LVNI_SELECTED);

    if (nItem != -1)
    {
        GetParent()->SendMessage(WM_LSTC_DBLCLK, nItem);
    }

    CListCtrl::OnLButtonDblClk(nFlags, point);
}
#endif