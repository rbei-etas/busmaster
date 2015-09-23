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
    char serialPortName[10];                    // To hold serial port name

    SOCKET sendingSocket;                       // Socket for Etherent communication
    char pcHost[256];                           // To hold IP address
    unsigned int serverPort;                    // To hold port number   

    OVERLAPPED ovRead;
    OVERLAPPED ovWrite;

} QRCAN_DEVICE;

extern QRCAN_DEVICE qrcanDevice;

struct QRCanCfg
{
    QRCAN_HANDLE hCan;
    char canBaudRate;
};

int WINAPI ShowQRCANConfig(HWND hwnd, struct QRCanCfg* cfg);

#define MAX_PACKET_SIZE         1000000
#define BUFFER_SIZE             100
#define AVAILABLE_SERIAL_PORTS  256


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
