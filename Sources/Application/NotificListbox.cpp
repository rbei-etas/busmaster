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
 * \file      Application/NotificListbox.cpp
 * \brief     Inplementation file for CNotificListbox class.
 * \author    Ravikumar Patil
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Inplementation file for CNotificListbox class.
 */

// For all standard header file include
#include "stdafx.h"
// Definition of App class
#include "resource.h"
// Definition of CNotificListbox
#include "NotificListbox.h"
// Utility functions
#include "Utility/Utility.h"

/////////////////////////////////////////////////////////////////////////////
// CNotificListbox

/******************************************************************************
 Function Name  :   CNotificListbox

 Description    :   Standard default constructor for list box
 Member of      :   CNotificListbox

 Author(s)      :   Ravikumar Patil
 Date Created   :   27-03-2003
******************************************************************************/

CNotificListbox::CNotificListbox()
{
}


/******************************************************************************
 Function Name  :   ~CNotificListbox

 Description    :   Standard destructor
 Member of      :   CNotificListbox

 Author(s)      :   Ravikumar Patil
 Date Created   :   27-03-2003
******************************************************************************/
CNotificListbox::~CNotificListbox()
{
}


BEGIN_MESSAGE_MAP(CNotificListbox, CListBox)
    //{{AFX_MSG_MAP(CNotificListbox)
    ON_WM_RBUTTONDOWN()
    ON_COMMAND(IDM_OPRTN_CLEAR, OnTraceWndClear)
    ON_COMMAND(IDM_OPRTN_DELETE, OnTraceWndDelete)
    //}}AFX_MSG_MAP
    ON_COMMAND(ID_NOTIFICWND_SELECTALL, OnNotificwndSelectall)
    ON_COMMAND(ID_NOTIFICWND_COPYTOCLIPBOARD, OnNotificwndCopytoclipboard)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNotificListbox message handlers


/******************************************************************************
 Function Name  :   OnRButtonDown

 Description    :   The framework calls this member function when the user
                    right clicks on the list box
 Input(s)       :    nFlags -
                    point -
 Output         :   -
 Functionality  :   Shows a popup menu to clear the contents of the listbox
 Member of      :   CNotificListbox

 Author(s)      :   Ravikumar Patil
 Date Created   :   27-03-2003
******************************************************************************/
void CNotificListbox::OnRButtonDown(UINT nFlags, CPoint point)
{
    if (GetCount() > 0)
    {
        CMenu* pomContextMenu = new CMenu;

        if (pomContextMenu != NULL)
        {
            // Load the Menu from the resource
            pomContextMenu->DestroyMenu();
            pomContextMenu->LoadMenu(IDM_OPERATION_LIST);
            CMenu* pomSubMenu = pomContextMenu->GetSubMenu(0);

            if (pomSubMenu != NULL)
            {
                CPoint omSrcPt = point;
                ClientToScreen(&omSrcPt);
                UINT unEnable;

                /* If no item is selected, make "Clear" and "Delete" menu
                items disabled */
                if (GetCurSel() == -1)
                {
                    unEnable = MF_BYCOMMAND | MF_DISABLED | MF_GRAYED;
                }
                else
                {
                    unEnable = MF_BYCOMMAND | MF_ENABLED;
                }

                pomSubMenu->EnableMenuItem(IDM_OPRTN_CLEAR, unEnable);
                pomSubMenu->EnableMenuItem(IDM_OPRTN_DELETE, unEnable);
                pomSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
                                           omSrcPt.x, omSrcPt.y, this, NULL);
            }

            delete pomContextMenu;
            pomContextMenu = NULL;
        }
    }

    CListBox::OnRButtonDown(nFlags, point);
}

/******************************************************************************
 Function Name  :   OnTraceWndClear

 Description    :   Handler to remove all the trace entries in the list box
 Input(s)       :   nFlags -
                    point -
 Output         :   -
 Functionality  :   Deletes all the items in the listbox
 Member of      :   CNotificListbox

 Author(s)      :   Ravikumar Patil
 Date Created   :   27-03-2003
******************************************************************************/
void CNotificListbox::OnTraceWndClear()
{
    // TODO: Add your command handler code here
    // Delete all strings in the list box
    ResetContent();
}

/* OnClick menu Select All*/
void CNotificListbox::OnNotificwndSelectall()
{
    for (int i = 0; i < GetCount(); i++)
    {
        SetSel(i, TRUE);
    }
}

/* OnClick menu Copy Text */
void CNotificListbox::OnNotificwndCopytoclipboard()
{
    CString omFullText = "";

    for (int i = 0; i < GetCount(); i++)
    {
        if (GetSel(i))
        {
            CString omText = "";
            GetText(i, omText);

            if (!omText.IsEmpty())
            {
                //omText = omText + L"\r\n";
                omText.Format(_T("%s\r\n"), omText);
                omFullText = omFullText + omText;
            }
        }
    }

    CopyTextToClipboard(omFullText.GetBuffer(MAX_PATH), NULL);
}

/******************************************************************************
 Function Name  :   OnTraceWndDelete

 Description    :   Handler to remove the selected trace entry in the list box
 Input(s)       :   -
                    -
 Output         :   -
 Functionality  :   Deletes the selected item in the Listbox
 Member of      :   CNotificListbox

 Author(s)      :   Ravikumar Patil
 Date Created   :   27-03-2003
 Modification   :   By Pradeep Kadoor on 3/12/2008
******************************************************************************/
void CNotificListbox::OnTraceWndDelete()
{
    for (int i = 0; i < GetCount(); i++)
    {
        if (GetSel(i))
        {
            DeleteString(i);
            --i;
        }
    }
}