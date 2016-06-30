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
 * \file      can_setup.cpp
 * \brief     Source file for Can_setup definitions
 * \author
 * \copyright Copyright (c) 2012, ETAS GmbH. All rights reserved.
 *
 * Source file for Can_setup definitions
 */

#include "CAN_VSCOM_stdafx.h"
#include <windows.h>
#include <basetyps.h>
#include <commctrl.h>
#include <windowsx.h>
#include "CAN_VSCOM.h"
#include "resource.h"
#include "EXTERNAL/vs_can_api.h"


#define MAX_DEVICES         256

#define VSCAN_DEVICE_TYPE_UNKNOWN   0
#define VSCAN_DEVICE_TYPE_SERIAL    1
#define VSCAN_DEVICE_TYPE_USB       2
#define VSCAN_DEVICE_TYPE_NET_VCOM  3
#define VSCAN_DEVICE_TYPE_NET_IP    4
#define VSCAN_DEVICE_TYPE_BUS       5   // Ocera LinCAN, PCI, AM3517


typedef struct
{
    DWORD InitDone;
    VSCAN_HANDLE Handle;            // ListElement + 1
    char Location[MAX_PATH + 1];    // ComPort or NetworkAddress
    UINT32 SerialNr;
    UINT8 HwVersion;
    UINT8 SwVersion;
    int Type;                       // Type of the Device

    HANDLE hComm;                   // Handle for the ComPort

    SOCKET s;                       // Descriptor of the socket
    char Hostname[256];
    USHORT Port;

    BOOL bOverlapped;
    OVERLAPPED ovWrite;
    OVERLAPPED ovRead;

    char* RecvBuf;                  // Receive Buffer
    //pthread_t ptReadThread;

    DWORD dwChannelOpen;
    DWORD dwChannelOpenMode;
    DWORD dwSuspendReadThread;
    DWORD dwSuspendedReadThread;
    DWORD dwDontRecv;               // Thread shouldn't receive data
    VSCAN_MSG* RecvMsgs;            // Received Messages
    DWORD dwRecvMsgsSize;           // Number of Received Messages
    VSCAN_MSG* RecvMsgsHead;        // the last Received Message
    VSCAN_MSG* RecvMsgsTail;        // the first Received Message
    //pthread_mutex_t ptRecvMsgsMutex;// Mutex for the Receiving

    DWORD Status;                   // Status for which Information is set
    DWORD Flags;                    // the last Flags from the Device
} VSCAN_DEVICE;

typedef VSCAN_DEVICE* VSCAN_DEVICE_LIST;
static struct _VSCanCfg* CanCfg = nullptr;
static HWND g_hDlg = nullptr;
static VSCAN_DEVICE_LIST g_list[MAX_DEVICES];
static DWORD g_dwListCnt = 0;

#ifdef __cplusplus
extern "C" {
#endif

    int EnumDevices2List(VSCAN_DEVICE_LIST* List);
    void FreeDeviceList(VSCAN_DEVICE_LIST* List);

#ifdef __cplusplus
}
#endif


static void PrintDeviceInfo(int nDeviceNr)
{
    char temp[32];

    if (nDeviceNr == -1)
    {
        SetDlgItemText(g_hDlg, IDC_DEVICE_TYPE, "Unknown");
        SetDlgItemText(g_hDlg, IDC_DEVICE_SERIAL, "Unknown");
        SetDlgItemText(g_hDlg, IDC_DEVICE_SW_VERSION, "Unknown");
        SetDlgItemText(g_hDlg, IDC_DEVICE_HW_VERSION, "Unknown");
        return;
    }

    if (!g_dwListCnt || (int)g_dwListCnt <= nDeviceNr)
    {
        return;
    }

    switch (g_list[nDeviceNr]->Type)
    {
        case VSCAN_DEVICE_TYPE_SERIAL:
            strcpy(temp, "SER-CAN");
            break;
        case VSCAN_DEVICE_TYPE_USB:
            strcpy(temp, "USB-CAN");
            break;
        case VSCAN_DEVICE_TYPE_BUS:
            strcpy(temp, "PCI-CAN");
            break;
        case VSCAN_DEVICE_TYPE_NET_VCOM:
            strcpy(temp, "NET-CAN");
            break;
        default:
            strcpy(temp, "Unknown");
            break;
    }
    SetDlgItemText(g_hDlg, IDC_DEVICE_TYPE, temp);

    sprintf(temp, "%d", g_list[nDeviceNr]->SerialNr);
    SetDlgItemText(g_hDlg, IDC_DEVICE_SERIAL, temp);

    sprintf(temp, "%d.%d", g_list[nDeviceNr]->SwVersion >> 4, g_list[nDeviceNr]->SwVersion & 0xf);
    SetDlgItemText(g_hDlg, IDC_DEVICE_SW_VERSION, temp);

    sprintf(temp, "%d.%d", g_list[nDeviceNr]->HwVersion >> 4, g_list[nDeviceNr]->HwVersion & 0xf);
    SetDlgItemText(g_hDlg, IDC_DEVICE_HW_VERSION, temp);
}

static void InitDeviceList(void)
{
    int i, cnt;

    memset(g_list, 0, sizeof(g_list));

    g_dwListCnt = EnumDevices2List(g_list);

    cnt = SendDlgItemMessage(g_hDlg, IDC_DEVICE_LIST, CB_GETCOUNT, 0, 0);
    for (i = 0; i < cnt; i++)
    {
        SendDlgItemMessage(g_hDlg, IDC_DEVICE_LIST, CB_DELETESTRING, i, 0);
    }

    for (i = 0; i < (int)g_dwListCnt; i++)
    {
        SendDlgItemMessage(g_hDlg, IDC_DEVICE_LIST, CB_ADDSTRING, 0, (LPARAM)strrchr(g_list[i]->Location, '\\') + 1);
    }

    if (g_dwListCnt)
    {
        SendDlgItemMessage(g_hDlg, IDC_DEVICE_LIST, CB_SETCURSEL, 0, 0);
        PrintDeviceInfo(0);
    }
}

static void InitBaudrateList(void)
{
    int i, cnt;

    cnt = SendDlgItemMessage(g_hDlg, IDC_BAUDRATE, CB_GETCOUNT, 0, 0);
    for (i = 0; i < cnt; i++)
    {
        SendDlgItemMessage(g_hDlg, IDC_BAUDRATE, CB_DELETESTRING, i, 0);
    }

    SendDlgItemMessage(g_hDlg, IDC_BAUDRATE, CB_ADDSTRING, 0, (LPARAM)"Use BTR Registers");
    SendDlgItemMessage(g_hDlg, IDC_BAUDRATE, CB_ADDSTRING, 0, (LPARAM)"20 kbit");
    SendDlgItemMessage(g_hDlg, IDC_BAUDRATE, CB_ADDSTRING, 0, (LPARAM)"50 kbit");
    SendDlgItemMessage(g_hDlg, IDC_BAUDRATE, CB_ADDSTRING, 0, (LPARAM)"100 kbit");
    SendDlgItemMessage(g_hDlg, IDC_BAUDRATE, CB_ADDSTRING, 0, (LPARAM)"125 kbit");
    SendDlgItemMessage(g_hDlg, IDC_BAUDRATE, CB_ADDSTRING, 0, (LPARAM)"250 kbit");
    SendDlgItemMessage(g_hDlg, IDC_BAUDRATE, CB_ADDSTRING, 0, (LPARAM)"500 kbit");
    SendDlgItemMessage(g_hDlg, IDC_BAUDRATE, CB_ADDSTRING, 0, (LPARAM)"800 kbit");
    SendDlgItemMessage(g_hDlg, IDC_BAUDRATE, CB_ADDSTRING, 0, (LPARAM)"1000 kbit");

    SendDlgItemMessage(g_hDlg, IDC_BAUDRATE, CB_SETCURSEL, (int)CanCfg->vSpeed, 0);
}

static void SaveDeviceData(void)
{
    char temp[MAX_PATH];

    // Location
    GetDlgItemText(g_hDlg, IDC_DEVICE_LIST, temp, sizeof(temp));
    if (strchr(temp, ':'))  // network
    {
        strcpy(CanCfg->szLocation, temp);
    }
    else
    {
        if (strncmp(temp, "\\\\.\\", 4))
        {
            strcpy(CanCfg->szLocation, "\\\\.\\");
            strcat_s(CanCfg->szLocation, (sizeof(CanCfg->szLocation)-sizeof("\\\\.\\")), temp);
        }
        // convert COM number to upper case (not WinCE OS)
        int len = strlen(CanCfg->szLocation) - 1;
        do
        {
            if (CanCfg->szLocation[len] == '\\')
            {
                break;
            }
            if (CanCfg->szLocation[len] >= 97 && CanCfg->szLocation[len] <= 122)
            {
                CanCfg->szLocation[len] -= 32;
            }
        }
        while (len--);
    }

    // Baudrate
    CanCfg->vSpeed = (void*)(SendDlgItemMessage(g_hDlg, IDC_BAUDRATE, CB_GETCURSEL, 0, 0));

    // Operation Mode
    if (IsDlgButtonChecked(g_hDlg, IDC_MODE_SELF_RECEPTION) == BST_CHECKED)
    {
        CanCfg->dwMode = VSCAN_MODE_SELF_RECEPTION;
    }
    else
    {
        if (IsDlgButtonChecked(g_hDlg, IDC_MODE_LISTEN_ONLY) == BST_CHECKED)
        {
            CanCfg->dwMode = VSCAN_MODE_LISTEN_ONLY;
        }
        else
        {
            CanCfg->dwMode = VSCAN_MODE_NORMAL;
        }
    }

    // Timestamps
    if (IsDlgButtonChecked(g_hDlg, IDC_CHECK_TIMESTAMPS) == BST_CHECKED)
    {
        CanCfg->bTimestamps = TRUE;
    }
    else
    {
        CanCfg->bTimestamps = FALSE;
    }

    // Debug Mode
    if (IsDlgButtonChecked(g_hDlg, IDC_CHECK_DEBUG) == BST_CHECKED)
    {
        CanCfg->bDebug = TRUE;
    }
    else
    {
        CanCfg->bDebug = FALSE;
    }

    // BTR Register
    if (GetDlgItemText(g_hDlg, IDC_BTR0, temp, sizeof(temp)) <= 2)
    {
        CanCfg->btr.Btr0 = (UCHAR)strtoul(temp, nullptr, 16);
    }
    if (GetDlgItemText(g_hDlg, IDC_BTR1, temp, sizeof(temp)) <= 2)
    {
        CanCfg->btr.Btr1 = (UCHAR)strtoul(temp, nullptr, 16);
    }

    // Acceptance Code and Mask
    if (GetDlgItemText(g_hDlg, IDC_ACC_CODE, temp, sizeof(temp)))
    {
        CanCfg->codeMask.Code = (UINT32)strtoul(temp, nullptr, 16);
    }
    if (GetDlgItemText(g_hDlg, IDC_ACC_MASK, temp, sizeof(temp)))
    {
        CanCfg->codeMask.Mask = (UINT32)strtoul(temp, nullptr, 16);
    }
    if (IsDlgButtonChecked(g_hDlg, IDC_FILTER_DUAL) == BST_CHECKED)
    {
        CanCfg->bDualFilter = TRUE;
    }
    else
    {
        CanCfg->bDualFilter = FALSE;
    }
}

static BOOL CALLBACK CanSetupDlgProc(HWND hDlg, UINT uMessage, WPARAM wParam, LPARAM /* lParam */)
{
    int wmId, wmEvent;

    switch(uMessage)
    {
        case WM_INITDIALOG:
        {
            VSCAN_API_VERSION version;
            char temp[MAX_PATH];

            g_hDlg = hDlg;
            memset(g_list, 0, sizeof(g_list));

            VSCAN_Ioctl(CanCfg->hCan, VSCAN_IOCTL_GET_API_VERSION, &version);
            sprintf(temp, "%d.%d.%d", version.Major, version.Minor, version.SubMinor);
            SetDlgItemText(g_hDlg, IDC_API_VERSION, temp);

            InitBaudrateList();

            if (!strncmp(CanCfg->szLocation, "\\\\.\\", 4))
            {
                SetDlgItemText(g_hDlg, IDC_DEVICE_LIST, CanCfg->szLocation + 4);
            }
            else
            {
                SetDlgItemText(g_hDlg, IDC_DEVICE_LIST, CanCfg->szLocation);
            }

            switch(CanCfg->dwMode)
            {
                case VSCAN_MODE_LISTEN_ONLY:
                    CheckDlgButton(g_hDlg, IDC_MODE_LISTEN_ONLY, BST_CHECKED);
                    break;
                case VSCAN_MODE_SELF_RECEPTION:
                    CheckDlgButton(g_hDlg, IDC_MODE_SELF_RECEPTION, BST_CHECKED);
                    break;
                default:
                    CheckDlgButton(g_hDlg, IDC_MODE_NORMAL, BST_CHECKED);
                    break;
            }
            if (CanCfg->bTimestamps)
            {
                CheckDlgButton(g_hDlg, IDC_CHECK_TIMESTAMPS, BST_CHECKED);
            }
            if (CanCfg->bDebug)
            {
                CheckDlgButton(g_hDlg, IDC_CHECK_DEBUG, BST_CHECKED);
            }

            SendDlgItemMessage(g_hDlg, IDC_BTR0, EM_LIMITTEXT, 2, 0);
            SendDlgItemMessage(g_hDlg, IDC_BTR1, EM_LIMITTEXT, 2, 0);
            sprintf(temp, "%02X", CanCfg->btr.Btr0);
            SetDlgItemText(g_hDlg, IDC_BTR0, temp);
            sprintf(temp, "%02X", CanCfg->btr.Btr1);
            SetDlgItemText(g_hDlg, IDC_BTR1, temp);

            SendDlgItemMessage(g_hDlg, IDC_ACC_CODE, EM_LIMITTEXT, 8, 0);
            SendDlgItemMessage(g_hDlg, IDC_ACC_MASK, EM_LIMITTEXT, 8, 0);
            sprintf(temp, "%08X", CanCfg->codeMask.Code);
            SetDlgItemText(g_hDlg, IDC_ACC_CODE, temp);
            sprintf(temp, "%08X", CanCfg->codeMask.Mask);
            SetDlgItemText(g_hDlg, IDC_ACC_MASK, temp);
            if (CanCfg->bDualFilter)
            {
                SendDlgItemMessage(g_hDlg, IDC_FILTER_DUAL, BM_SETCHECK, BST_CHECKED, 0);
            }
        }
        return TRUE;
        case WM_COMMAND:
            wmId    = LOWORD(wParam);
            wmEvent = HIWORD(wParam);

            switch (wmId)
            {
                case IDCANCEL:
                case IDOK:
                    FreeDeviceList(g_list);
                    EndDialog(hDlg, FALSE);
                    return TRUE;
                case IDC_SAVE:
                    SaveDeviceData();
                    FreeDeviceList(g_list);
                    EndDialog(hDlg, TRUE);
                    return TRUE;
                case IDC_SEARCH:
                    InitDeviceList();
                    return TRUE;
                case IDC_DEVICE_LIST:
                    switch (wmEvent)
                    {
                        case CBN_SELCHANGE:
                        {
                            int sel = SendDlgItemMessage(g_hDlg, IDC_DEVICE_LIST, CB_GETCURSEL, 0, 0);
                            PrintDeviceInfo(sel);
                        }
                        break;
                        case CBN_SETFOCUS:
                            SetDlgItemText(g_hDlg, IDC_DEVICE_LIST, "");
                            PrintDeviceInfo(-1);
                            break;
                        case CBN_EDITCHANGE:
                            PrintDeviceInfo(-1);
                            break;
                        default:
                            return FALSE;
                    }
                    return TRUE;
            }

    }
    return FALSE;
}


int WINAPI ShowCanVsComSetup(HWND hwnd, struct _VSCanCfg* cfg)
{
    MEMORY_BASIC_INFORMATION mbi;
    static int dummy;
    VirtualQuery(&dummy, &mbi, sizeof(mbi));
    DWORD hMod = (DWORD)mbi.AllocationBase;
    char szModule[MAX_PATH];
    GetModuleFileName((HMODULE)hMod, szModule, sizeof(szModule));
    CanCfg = cfg;
    return DialogBox(GetModuleHandle(szModule), MAKEINTRESOURCE(IDD_CONFIG), hwnd, CanSetupDlgProc);
}
