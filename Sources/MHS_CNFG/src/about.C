/***************************************************************************
                           about.c  -  description
                             -------------------
    begin             : 14.10.2011
    copyright         : (C) 2011 by MHS-Elektronik GmbH & Co. KG, Germany
    autho             : Klaus Demlehner, klaus@mhs-elektronik.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software, you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License           *
 *   version 2.1 as published by the Free Software Foundation.             *
 *                                                                         *
 ***************************************************************************/

/**
    Library to talk to Tiny-CAN devices. You find the latest versions at
       http://www.tiny-can.com/
**/
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
        case WM_INITDIALOG :
            break;

        case WM_COMMAND    :
        {
            if ((LOWORD(wparam) == IDOK) || (LOWORD(wparam) == IDCANCEL))
            {
                EndDialog(hdlg, TRUE);
                return(TRUE);
            }

            break;
        }
    }

    return(FALSE);
}


void CreateAboutDlg(HINSTANCE hInstance, HWND hwnd)
{
    (void)DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUT), hwnd, AboutDlgProc);
}
