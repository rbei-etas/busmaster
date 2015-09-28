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
 * \author    Malligaraj Malleswaran
 * \copyright Copyright (c) 2015, QRTECH AB. All rights reserved.
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

#include <winsock2.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
//#pragma comment (lib, "Mswsock.lib")
//#pragma comment (lib, "AdvApi32.lib")


static HWND qrconfig_hDlg = nullptr;                    // Handle for Dialog box
static struct QRCanCfg* CanCfg = nullptr;

/**
*
*   Checks the list of Serial ports available and displays it in GUI
*
*/
static void InitSerialPortList(void)
{
    int i, cnt;

    cnt = SendDlgItemMessage(qrconfig_hDlg, IDC_SERIAL_PORT, CB_GETCOUNT, 0, 0);
    for (i = 0; i < cnt; i++)
    {
        SendDlgItemMessage(qrconfig_hDlg, IDC_SERIAL_PORT, CB_DELETESTRING, i, 0);
    }

    // Find all available serial ports and display it in GUI
    TCHAR szPort[32];
    HANDLE hComm;
    for (UINT i=1; i<AVAILABLE_SERIAL_PORTS; i++){
        _stprintf_s(szPort, _T("\\\\.\\COM%u"), i);
        hComm = CreateFile(szPort, 
                           GENERIC_READ | GENERIC_WRITE,
                           0,
                           0,
                           OPEN_EXISTING,
                           FILE_FLAG_OVERLAPPED,
                           0);

        if (hComm != INVALID_HANDLE_VALUE){
            _stprintf_s(szPort, _T("COM%u"), i);
            SendDlgItemMessage(qrconfig_hDlg, IDC_SERIAL_PORT, CB_ADDSTRING, 0, (LPARAM)szPort);
            CloseHandle(hComm);
        }
    }
    SendDlgItemMessage(qrconfig_hDlg, IDC_SERIAL_PORT, CB_SETCURSEL, 0, 0);
}

/**
*
*   Initializes available CAN baud rates in GUI
*
*/
static void InitBaudRateList(void){
    int i, cnt;

    cnt = SendDlgItemMessage(qrconfig_hDlg, IDC_BAUDRATE, CB_GETCOUNT, 0, 0);
    for (i = 0; i < cnt; i++)
    {
        SendDlgItemMessage(qrconfig_hDlg, IDC_BAUDRATE, CB_DELETESTRING, i, 0);
    }

    SendDlgItemMessage(qrconfig_hDlg, IDC_BAUDRATE, CB_ADDSTRING, 0, (LPARAM)"20 kbit");
    SendDlgItemMessage(qrconfig_hDlg, IDC_BAUDRATE, CB_ADDSTRING, 0, (LPARAM)"50 kbit");
    SendDlgItemMessage(qrconfig_hDlg, IDC_BAUDRATE, CB_ADDSTRING, 0, (LPARAM)"100 kbit");
    SendDlgItemMessage(qrconfig_hDlg, IDC_BAUDRATE, CB_ADDSTRING, 0, (LPARAM)"125 kbit");
    SendDlgItemMessage(qrconfig_hDlg, IDC_BAUDRATE, CB_ADDSTRING, 0, (LPARAM)"250 kbit");
    SendDlgItemMessage(qrconfig_hDlg, IDC_BAUDRATE, CB_ADDSTRING, 0, (LPARAM)"500 kbit");
    SendDlgItemMessage(qrconfig_hDlg, IDC_BAUDRATE, CB_ADDSTRING, 0, (LPARAM)"800 kbit");
    SendDlgItemMessage(qrconfig_hDlg, IDC_BAUDRATE, CB_ADDSTRING, 0, (LPARAM)"1000 kbit");

    SendDlgItemMessage(qrconfig_hDlg, IDC_BAUDRATE, CB_SETCURSEL, 3/*(int)CanCfg->canBaudRate*/, 0);
}

/**
*
*   Gets the serial port selected from GUI and stores it for further initialization
*
*/
static void SetupSerialPort(void){    
    // Get the serial port selected from GUI
    if (GetDlgItemText(qrconfig_hDlg, IDC_SERIAL_PORT, qrcanDevice.serialPortName, sizeof(qrcanDevice.serialPortName)) == 0){
        CString msg;
        msg.Format(_T("%d"), GetLastError());
        AfxMessageBox(msg);
        AfxMessageBox("Error getting Serial Port from GUI");
    }
}

/**
*
*   Get IP address from the GUI and stores it for further initialization
*
*/
static void SetupEthernet(void){
    WSAData wsaData;
    qrcanDevice.serverPort = 7; 
    
    // Initialize Winsock version 2.2
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Create a new socket to make a client connection
    // AF_INET = 2, The Internet Protocol version 4 (IPv4) address family, TCP protocol
    qrcanDevice.tcpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    

    if (qrcanDevice.tcpSocket == INVALID_SOCKET){
        CString msg;
        msg.Format(_T("%d"), WSAGetLastError());
        AfxMessageBox(msg);
        AfxMessageBox("Invalid Socket");
        WSACleanup();
        return; 
    }
    // Get the IP Address from GUI
    if (GetDlgItemText(qrconfig_hDlg, IDC_IP_ADDRESS, qrcanDevice.pcHost, sizeof(qrcanDevice.pcHost)) == 0){
        // The above function returns the number of characters copied to the buffer if success or 0 for failure
        strcpy(qrcanDevice.pcHost, "127.0.0.1");
    }
}

/**
*
*   Saves the data from the QRCAN Configuration dialog box
*
*/
static void SaveDeviceData(void){
    // Selection of Communication Mode
    if (IsDlgButtonChecked(qrconfig_hDlg, IDC_MODE_USB) == BST_CHECKED){
        qrcanDevice.commMode = QRCAN_USE_USB;
        SetupSerialPort();
    }
    else if (IsDlgButtonChecked(qrconfig_hDlg, IDC_MODE_ETHERNET) == BST_CHECKED){
        qrcanDevice.commMode = QRCAN_USE_ETHERNET;
        SetupEthernet();
    }

    // Save CAN Baudrate from GUI

    //CanCfg->canBaudRate = (void*)(SendDlgItemMessage(qrconfig_hDlg, IDC_BAUDRATE, CB_GETCURSEL, 0, 0));

    char canBaudRate[15];
    if (GetDlgItemText(qrconfig_hDlg, IDC_BAUDRATE, canBaudRate, sizeof(canBaudRate)) == 0){
        CString msg;
        msg.Format(_T("%d"), GetLastError());
        AfxMessageBox(msg);
        AfxMessageBox("Error getting Baud Rate from GUI");   
    }

    if (strcmp("20 kbit", canBaudRate) == 0){
        CanCfg->canBaudRate = QRCAN_SPEED_20K;
    }else if (strcmp("50 kbit", canBaudRate) == 0){
        CanCfg->canBaudRate = QRCAN_SPEED_50K;
    }else if (strcmp("100 kbit", canBaudRate) == 0){
        CanCfg->canBaudRate = QRCAN_SPEED_100K;
    }else if (strcmp("125 kbit", canBaudRate) == 0){
        CanCfg->canBaudRate = QRCAN_SPEED_125K;
    }else if (strcmp("250 kbit", canBaudRate) == 0){
        CanCfg->canBaudRate = QRCAN_SPEED_250K;
    }else if (strcmp("500 kbit", canBaudRate) == 0){
        CanCfg->canBaudRate = QRCAN_SPEED_500K;
    }else if (strcmp("800 kbit", canBaudRate) == 0){
        CanCfg->canBaudRate = QRCAN_SPEED_800K;
    }else if (strcmp("1000 kbit", canBaudRate) == 0){
        CanCfg->canBaudRate = QRCAN_SPEED_1M;
    }else{
        AfxMessageBox("Invalid Baud Rate");
    }

}

/**
*
*   Callback function for QRCAN Configuration dialog box
*
*/
static BOOL CALLBACK CanSetupDlgProc(HWND hDlg, UINT uMessage, WPARAM wParam, LPARAM /* lParam */){
    int wmId, wmEvent;

    switch(uMessage)
    {
        case WM_INITDIALOG:
        {
            qrconfig_hDlg = hDlg;
            // Check USB Radio button by default
            CheckDlgButton(hDlg, IDC_MODE_USB, BST_CHECKED);

            // Disable IP Address text box during initialization
            EnableWindow(GetDlgItem(hDlg,IDC_IP_ADDRESS),FALSE);

            // Scan for available serial ports and display them
            InitSerialPortList();

            // Display available CAN baud rates
            InitBaudRateList();
        }
        return TRUE;
        case WM_COMMAND:
            wmId    = LOWORD(wParam);
            wmEvent = HIWORD(wParam);

            switch (wmId)
            {
                // For Close button
                case IDCANCEL:
                case IDOK:
                    EndDialog(hDlg, FALSE);
                    return TRUE;

                // To save configuration when OK button is pressed
                case IDC_SAVE:
                    SaveDeviceData();
                    EndDialog(hDlg, TRUE);
                    return TRUE;
                
                // To disable IP Address box when USB is selected as mode
                case IDC_MODE_USB:
                    EnableWindow(GetDlgItem(hDlg,IDC_IP_ADDRESS),FALSE);
                    EnableWindow(GetDlgItem(hDlg,IDC_SERIAL_PORT),TRUE);
                    return TRUE;
                
                // To disable Serial port drop down box when Ethernet is selected as mode
                case IDC_MODE_ETHERNET:
                    EnableWindow(GetDlgItem(hDlg,IDC_SERIAL_PORT),FALSE);
                    EnableWindow(GetDlgItem(hDlg,IDC_IP_ADDRESS),TRUE);
                    return TRUE;
                
                default:
                    return FALSE;
            }
    }
    return FALSE;
}

/**
*
*   To display QRCAN Configuration dialog box
*
*/
int WINAPI ShowQRCANConfig(HWND hwnd, struct QRCanCfg* cfg)
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