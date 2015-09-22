/* Copyright (c) [[[YEAR OF LAST UPDATE]]] QRTECH AB Gothenburg Sweden. All Rights Reserved.
*
* This document is provided to you in a non-exclusive agreement, and
* shall not without QRTECH AB’s written permission be copied, imparted
* to a third party or be used for any other unauthorized purpose.
* QRTECH retain the right to reuse or re-license for other purposes.
*
* The document is provided "AS IS", WITHOUT WARRANTY and is only
* supported by QRTECH in its complete original distribution unless
* otherwise is agreed in a separate written business agreement.
*
* QRTECH AB
* Flöjelbergsgatan 1C
* 431 35  Mölndal
* SWEDEN
* Web: www.qrtech.se
* Tel: +46 31 773 76 00

*/

/**
 * \file      qrcan_api.h
 * \author    
 * 
 * Implementation of QRCAN API - A wrapper for the ASCII protocol
 */

#ifndef _QR_CAN_API_H_
#define _QR_CAN_API_H_

#ifdef WIN32
#include <windows.h>
#else
#include <semaphore.h>
typedef void VOID;
typedef char CHAR;
typedef unsigned long DWORD;
typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned long ULONG;
#endif

typedef int QRCAN_HANDLE;
typedef int QRCAN_STATUS;
typedef int QRCAN_COMM_MODE;

// CAN Message Structure
typedef struct
{
    UINT32 Id;
    UINT8 Length;
    UINT8 Data[8];
    UINT8 Flags;
    UINT16 Timestamp;
} QRCAN_MSG;

// Status and Errors
#define QRCAN_ERR_OK                    0
#define QRCAN_ERR_NOT_OK                -1

// Communication Modes
#define QRCAN_USE_USB                   1
#define QRCAN_USE_ETHERNET              2

// CAN Baud Rate
#define QRCAN_SPEED_1M                  '8'
#define QRCAN_SPEED_800K                '7'
#define QRCAN_SPEED_500K                '6'
#define QRCAN_SPEED_250K                '5'
#define QRCAN_SPEED_125K                '4'
#define QRCAN_SPEED_100K                '3'
#define QRCAN_SPEED_50K                 '2'
#define QRCAN_SPEED_20K                 '1'

// ASCII Protocol symbols
#define QRCAN_SEND_11BIT_ID                't'
#define QRCAN_SET_BAUD_RATE                'S'

#define ASCII_RETURN                    '\r'



/* Function Definitions */
// Config Device parameters
QRCAN_STATUS QRCAN_Open();

// Configure CAN parameters
QRCAN_STATUS QRCAN_Config(QRCAN_HANDLE Handle, struct QRCanCfg* cfg);

// Send CAN Message from BUSMASTER
QRCAN_STATUS QRCAN_SendCANMessage(QRCAN_MSG* Buf);

// Handles sending data through different media
QRCAN_STATUS QRCAN_SendDataToHardware(char *data);

// Recveive CAN Message from Hardware
QRCAN_STATUS QRCAN_Recv(QRCAN_HANDLE Handle, QRCAN_MSG* Buf);

QRCAN_STATUS QRCAN_Close();

#ifdef WIN32
    QRCAN_STATUS QRCAN_SetRcvEvent();
#else
    QRCAN_STATUS QRCAN_SetRcvEvent(QRCAN_HANDLE Handle, sem_t* Event);
#endif

#endif