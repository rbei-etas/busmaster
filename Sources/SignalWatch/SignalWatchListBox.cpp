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
 * \file      SignalWatchListBox.cpp
 * \brief     Implementation file for CSignalWatchListBox class
 * \authors   RBIN/EMC2 - Ratnadip Choudhury, Amarnath Shastri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CSignalWatchListBox class
 */


// For all standard header file include
#include "SignalWatch_stdafx.h"
// Interface file for CSignalWatchListBox class
#include "SignalWatch_resource.h"
#include "SignalWatchDefs.h"
#include "SignalWatchListBox.h"

extern HWND GUI_hDisplayWindow;
/////////////////////////////////////////////////////////////////////////////
// CSignalWatchListBox

/******************************************************************************
 Function Name  :   CSignalWatchListBox

 Description    :   Standard default constructor for list box
 Member of      :   CSignalWatchListBox

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   03-04-2002
******************************************************************************/
CSignalWatchListBox::CSignalWatchListBox()
{
    m_hParentWnd = NULL;
}
//
void CSignalWatchListBox::vUpdateParentWnd(HWND hParentWnd)
{
    m_hParentWnd = hParentWnd;
}
//
/******************************************************************************
 Function Name  :   ~CSignalWatchListBox

 Description    :   Standard destructor
 Member of      :   CSignalWatchListBox

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   03-04-2002
******************************************************************************/
CSignalWatchListBox::~CSignalWatchListBox()
{
}


BEGIN_MESSAGE_MAP(CSignalWatchListBox, CFFListCtrl)
    //{{AFX_MSG_MAP(CSignalWatchListBox)
    ON_WM_RBUTTONDOWN()
    ON_COMMAND(IDM_SG_WATCH_CLEAR, OnSgWatchClear)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSignalWatchListBox message handlers

/******************************************************************************
 Function Name  :   OnRButtonDown

 Description    :   The framework calls this member function when the user 
                    right clicks on the list box
 Input(s)       :   nFlags - 
                    point - 
 Output         :   -
 Functionality  :   Shows a popup menu to remove an entry or to clear entries
 Member of      :   CSignalWatchListBox

 Author(s)      :   Ratnadip Choudhury, Amarnath Shastri
 Date Created   :   03-04-2002
 Modified by    :   Ratnadip Choudhury
 Modified on    :   26-04-2002
 Modified by    :   Raja N
 Modified on    :   22.07.2004, Modified the function call to refer ListCtrl
******************************************************************************/
void CSignalWatchListBox::OnRButtonDown(UINT nFlags, CPoint omPoint) 
{
    if (GetItemCount() > 0)
    {
        CMenu* pomContextMenu = new CMenu;
        if (pomContextMenu != NULL)
        {
            // Load the Menu from the resource
            pomContextMenu->DestroyMenu();
            pomContextMenu->LoadMenu(IDM_MENU_SIGNAL_WATCH);
            CMenu* pomSubMenu = pomContextMenu->GetSubMenu(1);

            if (pomSubMenu != NULL)
            {
                CPoint omSrcPt = omPoint;
                ClientToScreen(&omSrcPt);
                UINT unEnable; 
                // If no item is selected, make "Delete" menu item disabled
                if (GetSelectedCount() == -1) 
                {
                    unEnable = MF_BYCOMMAND | MF_DISABLED | MF_GRAYED;
                }
                else 
                {
                    unEnable = MF_BYCOMMAND | MF_ENABLED;
                }
                pomSubMenu->EnableMenuItem(IDM_SG_WATCH_CLEAR, unEnable);
                pomSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
                                      omSrcPt.x, omSrcPt.y, this, NULL);
            }
            delete pomContextMenu;
            pomContextMenu = NULL;
        }
    }

    CFFListCtrl::OnRButtonDown(nFlags, omPoint);
}

/******************************************************************************
 Function Name  :   OnSgWatchClear

 Input(s)       :   -
 Output         :   -
 Functionality  :   Handler to remove all the signal entries in the list box
 Member of      :   CSignalWatchListBox

 Author(s)      :   Ratnadip Choudhury
 Date Created   :   03-04-2002
 Modified by    :   Ratnadip Choudbhury
 Modified on    :   26-04-2002
 Modified by    :   Amitesh Bharti
 Modified on    :   20-05-2003, Clear the content of the list box and post a
                                message to delete all entry in CMap of signal
 Modified by    :   Raja N
 Modified on    :   22.07.2004, Modified the function call to refer ListCtrl
******************************************************************************/
void CSignalWatchListBox::OnSgWatchClear() 
{
    ::PostMessage(m_hParentWnd,WM_REMOVE_SIGNAL,0,0);
}