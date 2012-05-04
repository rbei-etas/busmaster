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
 * \file      can_setup.c
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
#include "global.h"
#include "mhs_types.h"
#include "resource.h"
#include "util.h"
#include "gui_util.h"
#include "mhsbmcfg.h"
#include "can_setup.h"

static struct TMhsCanCfg* CanCfg = NULL;

#define CanSpeedTabSize 9
static const char* CanSpeedTabStr[] = {"10 kBit/s", "20 kBit/s", "50 kBit/s", "100 kBit/s",
                                       "125 kBit/s", "250 kBit/s", "500 kBit/s", "800 kBit/s",
                                       "1 MBit/s"
                                      };
static const uint32_t CanSpeedTab[] = {10, 20, 50, 100, 125, 250, 500, 800, 1000};

static void InitCanSetupDlg(HWND hdlg, struct TMhsCanCfg* cfg)
{
    SetDlgItemText(hdlg, IDC_SNR_EDIT, cfg->CanSnrStr);
    SetDlgItemHex(hdlg, IDC_BTR_EDIT, HEX_WORD, cfg->CanBtrValue);
    FillComboBox(hdlg, IDC_CAN_SPEED, CanSpeedTabStr, CanSpeedTab,
                 CanSpeedTabSize, cfg->CanSpeed);
}

static void SaveCanSetup(HWND hdlg, struct TMhsCanCfg* cfg)
{
    GetDlgItemTextCpy(cfg->CanSnrStr, hdlg, IDC_SNR_EDIT, MHS_STR_SIZE);
    cfg->CanBtrValue = GetDlgItemHex(hdlg, IDC_BTR_EDIT);
    cfg->CanSpeed = GetComboBox(hdlg, IDC_CAN_SPEED);
}

static BOOL CALLBACK CanSetupDlgProc(HWND hdlg, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    switch(uMessage)
    {
        case WM_INITDIALOG:
        {
            InitCanSetupDlg(hdlg, CanCfg);
            break;
        }

        case WM_COMMAND:
        {
            switch (LOWORD(wparam))
            {
                case IDOK:
                {
                    SaveCanSetup(hdlg, CanCfg);
                    EndDialog(hdlg, TRUE);
                    return(TRUE);
                }

                case IDCANCEL:
                {
                    EndDialog(hdlg, TRUE);
                    return(TRUE);
                }
            }

            break;
        }
    }

    return(FALSE);
}

int ShowCanSetupDlg(HINSTANCE hInstance, HWND hwnd, struct TMhsCanCfg* cfg)
{
    CanCfg = cfg;

    return ((DWORD)DialogBox(hInstance, MAKEINTRESOURCE(IDD_CAN_SETUP), hwnd, CanSetupDlgProc)) ? 1 : 0;
}
