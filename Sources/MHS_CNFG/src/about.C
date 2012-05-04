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
 * \file      about.c
 * \brief     Library to talk to Tiny-CAN devices
 * \author    Klaus Demlehner
 * \copyright Copyright (c) 2011, MHS-Elektronik GmbH & Co. KG
 *
 * Library to talk to Tiny-CAN devices.
 * You find the latest versions at http://www.tiny-can.com/
 */

#include <windows.h>
#include <basetyps.h>
#include <commctrl.h>
#include <windowsx.h>
#include "resource.h"
#include "about.h"

static BOOL CALLBACK AboutDlgProc(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    switch(uMessage)
    {
        case WM_INITDIALOG:
            break;

        case WM_COMMAND:
            if ((LOWORD(wparam) == IDOK) || (LOWORD(wparam) == IDCANCEL))
            {
                EndDialog(hdlg, TRUE);
                return(TRUE);
            }

            break;
    }

    return(FALSE);
}

void CreateAboutDlg(HINSTANCE hInstance, HWND hwnd)
{
    (void)DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUT), hwnd, AboutDlgProc);
}
