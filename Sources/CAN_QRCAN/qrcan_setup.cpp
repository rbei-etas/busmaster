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

static HWND qrconfig_hDlg = nullptr;
HANDLE q_hComm;

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

static void SendDataToSerialPort(void)
{
	char serialPortName[5];

	// Get the serial port selected from GUI
	GetDlgItemText(qrconfig_hDlg, IDC_SERIAL_PORT, serialPortName, sizeof(serialPortName));	
	q_hComm = CreateFile(serialPortName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if(q_hComm == INVALID_HANDLE_VALUE){
		if(GetLastError() == ERROR_FILE_NOT_FOUND){
			// Inform user that serial port does not exist
			AfxMessageBox("Serial port does not exist");
		}
		// Some other error occured.
		AfxMessageBox("Unknown error occured");
	}
	
	DCB serialParams = {0};
	serialParams.DCBlength = sizeof(serialParams);

	if (!GetCommState (q_hComm, &serialParams)){
		// Error getting state
		AfxMessageBox("Failed to get the state of port");
	}

	serialParams.BaudRate = CBR_115200;
	serialParams.ByteSize = 8;
	serialParams.StopBits = ONESTOPBIT;
	serialParams.Parity = NOPARITY;

	if (!SetCommState(q_hComm, &serialParams)){
		// Error setting serial port state
		AfxMessageBox("Failed to set the state of port");
	}

	COMMTIMEOUTS timeouts = {0};
	timeouts.ReadIntervalTimeout = 500;
	timeouts.ReadTotalTimeoutConstant = 500;
	timeouts.ReadTotalTimeoutMultiplier = 100;
	timeouts.WriteTotalTimeoutConstant = 500;
	timeouts.WriteTotalTimeoutMultiplier = 100;

	if (!SetCommTimeouts(q_hComm, &timeouts)){
		// Error occured
	}

	char serialBuff[10] = "TestData\r";
	int dataLength = strlen(serialBuff);
	DWORD dwBytesRead = 0;


	for(int i = 0; i <5; i++){
		if (!WriteFile(q_hComm, serialBuff, dataLength+1, &dwBytesRead, NULL)){
			// Error Occured
			AfxMessageBox("Failed to send data");
		}
	}
	// Close the handle after the data transfer
	CloseHandle (q_hComm);	
}

static void SendDataToEtherent(void)
{
	WSAData wsaData;
	SOCKET sendingSocket;
	SOCKADDR_IN serverAddr;
	char pcHost[20] = "";
	unsigned int serverPort = 7;
	char messageBuffer[1024] = "Test Data from Busmaster";
	int bytesSent, messageLength, returnCode;

	// Initialize Winsock version 2.2
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// Create a new socket to make a client connection
	// AF_INET = 2, The Internet Protocol version 4 (IPv4) address family, TCP protocol
	sendingSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sendingSocket == INVALID_SOCKET){
		CString msg;
		msg.Format(_T("%d"), WSAGetLastError());
		AfxMessageBox(msg);
		AfxMessageBox("Invalid Socket");
		WSACleanup();
		return;	
	}
	// Get the IP Address from GUI
	if (GetDlgItemText(qrconfig_hDlg, IDC_IP_ADDRESS, pcHost, sizeof(pcHost)) == 0){
		// The above function returns the number of characters copied to the buffer if success or 0 for failure
		strcpy(pcHost, "127.0.0.1");
	}

	// Setting up SOCKADDR_IN structure that will be used to connect to a listening server on port 7.
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(serverPort);
	serverAddr.sin_addr.s_addr = inet_addr(pcHost);

	// Make a connection to the server with socket sendingSocket
	returnCode = connect(sendingSocket, (SOCKADDR *) &serverAddr, sizeof(serverAddr));

	if (returnCode != 0){
		CString msg;
		msg.Format(_T("%d"), WSAGetLastError());
		AfxMessageBox(msg);
		AfxMessageBox("Connection to the server failed");
		closesocket(sendingSocket);
		WSACleanup();
		return;
	}

	// Send data to the Server
	bytesSent = send(sendingSocket, messageBuffer, strlen(messageBuffer), 0);


	if (bytesSent == SOCKET_ERROR){
		CString msg;
		msg.Format(_T("%d"), WSAGetLastError());
		AfxMessageBox(msg);
		AfxMessageBox("Sending data Failed");	
	}

	 // Shutdown connection
	if (shutdown(sendingSocket, SD_SEND) != 0){
		CString msg;
		msg.Format(_T("%d"), WSAGetLastError());
		AfxMessageBox(msg);
		AfxMessageBox("Error during shudown");
	}

	/* Closing the socket and Cleanup throws an exception at the server: an established connection was aborted by the software in your host machine  */

	//// Close the socket.
	//if (closesocket(sendingSocket) != 0){
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
					SendDataToSerialPort();
					SendDataToEtherent();
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