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
 * @brief Implementation file for CSignalWatchListBox class
 * @authors Ratnadip Choudhury, Amarnath Shastri
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CSignalWatchListBox class
 */

// For all standard header file include
#include "SignalWatch_stdafx.h"
// Interface file for CSignalWatchListBox class
#include "SignalWatch_resource.h"
#include "SignalWatchDefs.h"
#include "SignalWatchListBox.h"
#include "..\Application\HashDefines.h"
#include "SignalWatch_Extern.h"
#include "BaseSignalWatch_CAN.h"
#include "SigWatchDlg.h"

extern HWND GUI_hDisplayWindow;

CSignalWatchListBox::CSignalWatchListBox()
{
    m_hParentWnd = NULL;
    m_hMainWnd = NULL;
    m_eBus = CAN;
}

CSignalWatchListBox::CSignalWatchListBox(ETYPE_BUS eBus)
{
    m_hParentWnd = NULL;
    m_hMainWnd = NULL;
    m_eBus = eBus;
}

void CSignalWatchListBox::vUpdateParentWnd(HWND hParentWnd)
{
    m_hParentWnd = hParentWnd;
}

void CSignalWatchListBox::vUpdateMainWnd(HWND hMainWnd)
{
    m_hMainWnd = hMainWnd;
}

CSignalWatchListBox::~CSignalWatchListBox()
{
}

BEGIN_MESSAGE_MAP(CSignalWatchListBox, CFFListCtrl)
    ON_WM_RBUTTONDOWN()
    ON_COMMAND(IDM_SG_WATCH_CLEAR, OnSgWatchClear)
    ON_COMMAND(IDD_DLG_SIGNAL_WATCH_LIN,OnConfigure)
    ON_WM_CHAR()
    ON_WM_KEYDOWN()
END_MESSAGE_MAP()

/**
 * The framework calls this member function when the user
 * right clicks on the list box.
 *
 * Shows a popup menu to remove an entry or to clear entries.
 */
void CSignalWatchListBox::OnRButtonDown(UINT nFlags, CPoint omPoint)
{
    CMenu* pomContextMenu = new CMenu;
    if (pomContextMenu != NULL)
    {
        /* Load the Menu from the resource */
        pomContextMenu->DestroyMenu();
        pomContextMenu->LoadMenu(IDM_MENU_SIGNAL_WATCH);
        CMenu* pomSubMenu = pomContextMenu->GetSubMenu(1);

        if (pomSubMenu != NULL)
        {
            CPoint omSrcPt = omPoint;
            ClientToScreen(&omSrcPt);
            UINT unEnable = MF_BYCOMMAND | MF_ENABLED;

            /* If no item is selected, make "Delete" menu item disabled */
            if (GetSelectedCount() == -1)
            {
                unEnable = MF_BYCOMMAND | MF_DISABLED | MF_GRAYED;
            }
            else
            {
                unEnable = MF_BYCOMMAND | MF_ENABLED;
            }

            /* Clear menu to be disabled if no signals are displayed in the list window */
            if (GetItemCount() <= 0)
            {
                unEnable = MF_BYCOMMAND | MF_DISABLED | MF_GRAYED;
            }
            pomSubMenu->EnableMenuItem(IDM_SG_WATCH_CLEAR, unEnable);
            pomSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
                                       omSrcPt.x, omSrcPt.y, this, NULL);
        }
        delete pomContextMenu;
        pomContextMenu = NULL;
    }
}

/**
 * Handler to remove all the signal entries in the list box
 */
void CSignalWatchListBox::OnSgWatchClear()
{
    ::PostMessage(m_hParentWnd,WM_REMOVE_SIGNAL,0,0);
}

void CSignalWatchListBox::OnChar(UINT nChar, UINT /* nRepeatCount */, UINT /* nflags */)
{
    GetParent()->SendMessage(WM_KEYBOARD_CHAR, nChar, 0);
}

void CSignalWatchListBox::OnKeyDown(UINT nChar, UINT /* nRepeatCount */, UINT /* nflags */)
{
    GetParent()->SendMessage(WM_KEYBOARD_KEYDOWN, nChar, 0);
}

void CSignalWatchListBox::OnConfigure()
{
    ::PostMessage(m_hParentWnd,WM_ADDDEL_SIGNAL, 0, 0);
}