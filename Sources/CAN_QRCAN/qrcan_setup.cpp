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
 * \file      qrcan_setup.cpp
 * \author    
 * \copyright Copyright (c) ((Year)), QRTECH AB. All rights reserved.
 *
 * Implementation of QRCAN
 */

#include "CAN_QRCAN_stdafx.h"
#include <windows.h>
#include <basetyps.h>
#include <commctrl.h>
#include <windowsx.h>
#include "CAN_QRCAN.h"
#include "resource.h"

static HWND qrconfig_hDlg = nullptr;

static void InitSerialPortList(void)
{
    int i, cnt;

    cnt = SendDlgItemMessage(qrconfig_hDlg, IDC_SERIAL_PORT, CB_GETCOUNT, 0, 0);
    for (i = 0; i < cnt; i++)
    {
        SendDlgItemMessage(qrconfig_hDlg, IDC_SERIAL_PORT, CB_DELETESTRING, i, 0);
    }

    SendDlgItemMessage(qrconfig_hDlg, IDC_SERIAL_PORT, CB_ADDSTRING, 0, (LPARAM)"COM6");
	SendDlgItemMessage(qrconfig_hDlg, IDC_SERIAL_PORT, CB_ADDSTRING, 0, (LPARAM)"COM7");
	SendDlgItemMessage(qrconfig_hDlg, IDC_SERIAL_PORT, CB_ADDSTRING, 0, (LPARAM)"COM8");
	SendDlgItemMessage(qrconfig_hDlg, IDC_SERIAL_PORT, CB_ADDSTRING, 0, (LPARAM)"COM9");
	SendDlgItemMessage(qrconfig_hDlg, IDC_SERIAL_PORT, CB_ADDSTRING, 0, (LPARAM)"COM5");
	SendDlgItemMessage(qrconfig_hDlg, IDC_SERIAL_PORT, CB_ADDSTRING, 0, (LPARAM)"COM8");


    SendDlgItemMessage(qrconfig_hDlg, IDC_SERIAL_PORT, CB_SETCURSEL, 0, 0);
}

static BOOL CALLBACK CanSetupDlgProc(HWND hDlg, UINT uMessage, WPARAM wParam, LPARAM /* lParam */)
{
	int wmId, wmEvent;

    switch(uMessage)
    {
        case WM_INITDIALOG:
        {
			qrconfig_hDlg = hDlg;
			
			// Scan for available serial ports and display them
			InitSerialPortList();
        }
        return TRUE;
        case WM_COMMAND:
            wmId    = LOWORD(wParam);
            wmEvent = HIWORD(wParam);

            switch (wmId)
            {
                case IDCANCEL:
                case IDOK:
                    EndDialog(hDlg, FALSE);
                    return TRUE;
                case IDC_SAVE:
                    EndDialog(hDlg, TRUE);
                    return TRUE;
				default:
					return FALSE;
            }

    }
    return FALSE;
}

// Function to display QRCAN Configuration Dialog
int WINAPI ShowQRCANConfig(HWND hwnd)
{
    MEMORY_BASIC_INFORMATION mbi;
    static int dummy;
    VirtualQuery(&dummy, &mbi, sizeof(mbi));
    DWORD hMod = (DWORD)mbi.AllocationBase;
    char szModule[MAX_PATH];
    GetModuleFileName((HMODULE)hMod, szModule, sizeof(szModule));
    return DialogBox(GetModuleHandle(szModule), MAKEINTRESOURCE(IDD_CONFIG), hwnd, CanSetupDlgProc);
}