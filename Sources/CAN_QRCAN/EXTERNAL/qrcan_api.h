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
#define QRCAN_ERR_OK					0
#define QRCAN_ERR_NOT_OK				-1

// Communication Modes
#define QRCAN_USE_USB					1
#define QRCAN_USE_ETHERNET				2

// CAN Baud Rate
#define QRCAN_SPEED_1M                  (void*)8
#define QRCAN_SPEED_800K                (void*)7
#define QRCAN_SPEED_500K                (void*)6
#define QRCAN_SPEED_250K                (void*)5
#define QRCAN_SPEED_125K                (void*)4
#define QRCAN_SPEED_100K                (void*)3
#define QRCAN_SPEED_50K                 (void*)2
#define QRCAN_SPEED_20K                 (void*)1

// ASCII Protocol symbols
#define QR_SEND_11BIT_ID				't'
#define ASCII_RETURN					'\r'



/* Function Definitions */
// Config CAN parameters
QRCAN_STATUS QRCAN_Open();

// Send CAN Message from BUSMASTER
QRCAN_STATUS QRCAN_Send(QRCAN_HANDLE Handle, QRCAN_MSG* Buf);

// Recveive CAN Message from Hardware
QRCAN_STATUS QRCAN_Recv(QRCAN_HANDLE Handle, QRCAN_MSG* Buf);

QRCAN_STATUS QRCAN_Close();



#endif