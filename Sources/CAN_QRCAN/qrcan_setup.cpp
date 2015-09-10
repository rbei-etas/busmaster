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

#include <winsock2.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
//#pragma comment (lib, "Mswsock.lib")
//#pragma comment (lib, "AdvApi32.lib")

QRCAN_DEVICE qrcanDevice;
static HWND qrconfig_hDlg = nullptr;					// Handle for Dialog box
static struct QRCanCfg* CanCfg = nullptr; 

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
	for (UINT i=1; i<256; i++){
		_stprintf_s(szPort, _T("\\\\.\\COM%u"), i);
		hComm = CreateFile(szPort, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hComm != INVALID_HANDLE_VALUE){
			_stprintf_s(szPort, _T("COM%u"), i);
			SendDlgItemMessage(qrconfig_hDlg, IDC_SERIAL_PORT, CB_ADDSTRING, 0, (LPARAM)szPort);
			CloseHandle(hComm);
		}
	}
    SendDlgItemMessage(qrconfig_hDlg, IDC_SERIAL_PORT, CB_SETCURSEL, 0, 0);
}

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

    SendDlgItemMessage(qrconfig_hDlg, IDC_BAUDRATE, CB_SETCURSEL, 7/*(int)CanCfg->canBaudRate*/, 0);
}

static void SetupSerialPort(void){
	char serialPortName[10];
	// Get the serial port selected from GUI
	GetDlgItemText(qrconfig_hDlg, IDC_SERIAL_PORT, serialPortName, sizeof(serialPortName));	
	qrcanDevice.q_hComm = CreateFile(serialPortName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if(qrcanDevice.q_hComm == INVALID_HANDLE_VALUE){
		if(GetLastError() == ERROR_FILE_NOT_FOUND){
			// Inform user that serial port does not exist
			AfxMessageBox("Serial port does not exist");
		}
		// Some other error occured.
		AfxMessageBox("Unknown error occured");
	}
	
	DCB serialParams = {0};
	serialParams.DCBlength = sizeof(serialParams);

	if (!GetCommState (qrcanDevice.q_hComm, &serialParams)){
		// Error getting state
		AfxMessageBox("Failed to get the state of port");
	}

	serialParams.BaudRate = CBR_115200;
	serialParams.ByteSize = 8;
	serialParams.StopBits = ONESTOPBIT;
	serialParams.Parity = NOPARITY;

	if (!SetCommState(qrcanDevice.q_hComm, &serialParams)){
		// Error setting serial port state
		AfxMessageBox("Failed to set the state of port");
	}

	// The values for timeouts are in ms
	COMMTIMEOUTS timeouts = {0};
	timeouts.ReadIntervalTimeout = 500;
	timeouts.ReadTotalTimeoutConstant = 500;
	timeouts.ReadTotalTimeoutMultiplier = 100;
	timeouts.WriteTotalTimeoutConstant = 500;
	timeouts.WriteTotalTimeoutMultiplier = 100;

	if (!SetCommTimeouts(qrcanDevice.q_hComm, &timeouts)){
		// Error occured
	}
}

static void SetupEthernet(void){
	WSAData wsaData;
	qrcanDevice.serverPort = 7;	
	
	// Initialize Winsock version 2.2
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// Create a new socket to make a client connection
	// AF_INET = 2, The Internet Protocol version 4 (IPv4) address family, TCP protocol
	qrcanDevice.sendingSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (qrcanDevice.sendingSocket == INVALID_SOCKET){
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

	// CAN Baudrate
	CanCfg->canBaudRate = (void*)(SendDlgItemMessage(qrconfig_hDlg, IDC_BAUDRATE, CB_GETCURSEL, 0, 0));
}

QRCAN_STATUS OpenDevice(){
	
	if (qrcanDevice.commMode == QRCAN_USE_ETHERNET){

		SOCKADDR_IN serverAddr;

		// Setting up SOCKADDR_IN structure that will be used to connect to a listening server on port 7.
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(qrcanDevice.serverPort);
		serverAddr.sin_addr.s_addr = inet_addr(qrcanDevice.pcHost);

		// Make a connection to the server with socket sendingSocket
		int returnCode = connect(qrcanDevice.sendingSocket, (SOCKADDR *) &serverAddr, sizeof(serverAddr));

		if (returnCode != 0){
			CString msg;
			msg.Format(_T("%d"), WSAGetLastError());
			AfxMessageBox(msg);
			AfxMessageBox("Connection to the server failed");
			closesocket(qrcanDevice.sendingSocket);
			WSACleanup();
			return QRCAN_ERR_NOT_OK;
		}		
	}
	else{
		return QRCAN_ERR_NOT_OK;
	}
	
	return QRCAN_ERR_OK;

}

QRCAN_STATUS CloseDevice(void){

	if (qrcanDevice.commMode == QRCAN_USE_ETHERNET){
		// Shutdown connection
		if (shutdown(qrcanDevice.sendingSocket, SD_SEND) != 0){
			CString msg;
			msg.Format(_T("%d"), WSAGetLastError());
			AfxMessageBox(msg);
			AfxMessageBox("Error during shutdown");
			return QRCAN_ERR_NOT_OK;

			/* Closing the socket and Cleanup throws an exception at the server: an established connection was aborted by the software in your host machine  */

			//// Close the socket.
			//if (closesocket(qrcanDevice.sendingSocket) != 0){
			//	CString msg;
			//	msg.Format(_T("%d"), WSAGetLastError());
			//	AfxMessageBox(msg);
			//	AfxMessageBox("Error during closing the socket");
			//}
			//// Run a cleanup
			//if (WSACleanup() != 0){
			//	CString msg;
			//	msg.Format(_T("%d"), WSAGetLastError());
			//	AfxMessageBox(msg);
			//	AfxMessageBox("Error during WSACleanup");
			//}	

		}
		return QRCAN_ERR_OK;
	}
	else if (qrcanDevice.commMode == QRCAN_USE_USB){
		CloseHandle (qrcanDevice.q_hComm);
		return QRCAN_ERR_OK;
	}
}

QRCAN_STATUS SendDataToHardware(char *data){
	if (qrcanDevice.commMode == QRCAN_USE_ETHERNET){
		char messageBuffer[1024] = "Test Data from Busmaster";
		int bytesSent, messageLength;

		// Send data to the Server
		bytesSent = send(qrcanDevice.sendingSocket, data, strlen(data), 0);


		if (bytesSent == SOCKET_ERROR){
			CString msg;
			msg.Format(_T("%d"), WSAGetLastError());
			AfxMessageBox(msg);
			AfxMessageBox("Sending data Failed");
			return QRCAN_ERR_NOT_OK;
		}

		return QRCAN_ERR_OK;

		// // Shutdown connection
		//if (shutdown(qrcanDevice.sendingSocket, SD_SEND) != 0){
		//	CString msg;
		//	msg.Format(_T("%d"), WSAGetLastError());
		//	AfxMessageBox(msg);
		//	AfxMessageBox("Error during shudown");
		//}

		/* Closing the socket and Cleanup throws an exception at the server: an established connection was aborted by the software in your host machine  */

		//// Close the socket.
		//if (closesocket(qrcanDevice.sendingSocket) != 0){
		//	CString msg;
		//	msg.Format(_T("%d"), WSAGetLastError());
		//	AfxMessageBox(msg);
		//	AfxMessageBox("Error during closing the socket");
		//}
		//// Run a cleanup
		//if (WSACleanup() != 0){
		//	CString msg;
		//	msg.Format(_T("%d"), WSAGetLastError());
		//	AfxMessageBox(msg);
		//	AfxMessageBox("Error during WSACleanup");
		//}	
	}

	else if(qrcanDevice.commMode == QRCAN_USE_USB){
		char serialBuff[10] = "TestData\r";
		int dataLength = strlen(serialBuff);
		DWORD dwBytesRead = 0;

			if (!WriteFile(qrcanDevice.q_hComm, data, strlen(data) + 1, &dwBytesRead, NULL)){
				// Error Occured
				AfxMessageBox("Failed to send data");
				return QRCAN_ERR_NOT_OK;
			}

		// Close the handle after the data transfer
		//CloseHandle (qrcanDevice.q_hComm);	

		return QRCAN_ERR_OK;
	}

	// The Communication mode is neither USB nor Ethernet
	else{
		return QRCAN_ERR_NOT_OK;
	}
}

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

// Function to display QRCAN Configuration Dialog
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