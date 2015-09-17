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
 * \file      CAN_QRCAN.h
 * \author    
 * \copyright Copyright (c) ((Year)), QRTECH AB. All rights reserved.
 */
#pragma once

#include "EXTERNAL/qrcan_api.h"

typedef struct{

    QRCAN_COMM_MODE commMode;                   // Communication Mode for the Hardware

    HANDLE q_hComm;                             // Handle for Serial Communication

    SOCKET sendingSocket;                       // Socket for Etherent communication
    char pcHost[256];                           // To hold IP address
    unsigned int serverPort;                    // To hold port number      

} QRCAN_DEVICE;

struct QRCanCfg
{
    QRCAN_HANDLE hCan;
    char canBaudRate;
};

int WINAPI ShowQRCANConfig(HWND hwnd, struct QRCanCfg* cfg);

QRCAN_STATUS SendDataToHardware(char *data);
QRCAN_STATUS ReceiveDataFromHardware();
QRCAN_STATUS CloseDevice(void);
QRCAN_STATUS OpenDevice(void);

/**
 * See CAN_QRCAN.cpp for the implementation of this class
 */
class CCAN_QRCAN : public CWinApp
{
public:
    CCAN_QRCAN();

    // Overrides
public:
    virtual BOOL InitInstance();

    DECLARE_MESSAGE_MAP()
};
