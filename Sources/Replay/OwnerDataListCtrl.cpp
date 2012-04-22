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
 * \file      OwnerDataListCtrl.cpp
 * \brief     Implementation file for COwnerDataListCtrl class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for COwnerDataListCtrl class
 */

#include "Replay_stdafx.h"                 // For Standard Includes
#include "OwnerDataListCtrl.h"      // For Owner data class declaration

/*******************************************************************************
  Function Name  : COwnerDataListCtrl
  Description    : Standard default constructor
  Member of      : COwnerDataListCtrl
  Functionality  : This will initialise local variables
  Author(s)      : Raja N
  Date Created   : 20.7.2005
  Modifications  :
*******************************************************************************/
COwnerDataListCtrl::COwnerDataListCtrl()
{
}

/*******************************************************************************
  Function Name  : ~COwnerDataListCtrl
  Description    : Standard Destructor
  Member of      : COwnerDataListCtrl
  Functionality  : -
  Author(s)      : Raja N
  Date Created   : 20.7.2005
  Modifications  :
*******************************************************************************/
COwnerDataListCtrl::~COwnerDataListCtrl()
{
}


BEGIN_MESSAGE_MAP(COwnerDataListCtrl, CFFListCtrl)
	//{{AFX_MSG_MAP(COwnerDataListCtrl)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*******************************************************************************
  Function Name  : OnGetdispinfo
  Input(s)       : pNMHDR - Pointer to Notification struct, pResult - Pointer to
                   the result
  Output         : -
  Functionality  : Handler for Display info message. This function will be
                   called to get display string. This function will pass this
                   message to parent.
  Member of      : COwnerDataListCtrl
  Author(s)      : Raja N
  Date Created   : 20.7.2005
  Modifications  : 
*******************************************************************************/
void COwnerDataListCtrl::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
    CWnd * pParent = GetParent();
    if( pParent != NULL && pParent->m_hWnd != 0 )
    {
        ::SendMessage( pParent->m_hWnd,
                       WM_OWNER_GET_DATA,
                       (WPARAM)pNMHDR,
                       (LPARAM)pResult );
    }
}

/*******************************************************************************
  Function Name  : OnCreate
  Input(s)       : lpCreateStruct - Create structure pointer
  Output         : -
  Functionality  : This function will be called before creating list control.
                   This function will append Owner Data style to create style
  Member of      : COwnerDataListCtrl
  Author(s)      : Raja N
  Date Created   : 20.7.2005
  Modifications  : 
*******************************************************************************/
int COwnerDataListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    lpCreateStruct->style |= LVS_OWNERDATA;
	if (CFFListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
    return 0;
}

/*******************************************************************************
  Function Name  : OnLButtonDblClk
  Input(s)       : -
  Output         : -
  Functionality  : Left button click handler. This will pass the information to
                   the parent class.
  Member of      : COwnerDataListCtrl
  Author(s)      : Raja N
  Date Created   : 20.7.2005
  Modifications  : 
*******************************************************************************/
void COwnerDataListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
    // Send corresponding message to parent window with current selection
    INT nItem = GetNextItem(-1, LVNI_SELECTED);
    if (nItem != -1)
    {
        GetParent()->SendMessage(WM_OW_LIST_DBLCLK, nItem);
    }

	CFFListCtrl::OnLButtonDblClk(nFlags, point);
}