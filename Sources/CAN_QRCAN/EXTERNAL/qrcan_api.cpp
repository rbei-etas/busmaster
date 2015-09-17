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
 * \file      qrcan_api.cpp
 * \author    
 * 
 * Implementation of QRCAN API - A wrapper for the ASCII protocol
 */

#include "..\CAN_QRCAN_stdafx.h"
#include "..\CAN_QRCAN.h"

#include "qrcan_api.h"

// Configure CAN bus
QRCAN_STATUS QRCAN_Open(){

    OpenDevice();
    return QRCAN_ERR_OK;

}

QRCAN_STATUS QRCAN_Config(QRCAN_HANDLE Handle, struct QRCanCfg* cfg)
{
    int i = 0;
    char sendMessage[32] = "";

    sendMessage[i++] = QRCAN_SET_BAUD_RATE;
    sendMessage[i++] = cfg->canBaudRate;
    sendMessage[i++] = ASCII_RETURN;

    return SendDataToHardware(sendMessage);

    return QRCAN_ERR_OK;
}


// Send CAN Message from BUSMASTER
QRCAN_STATUS QRCAN_Send(QRCAN_HANDLE Handle, QRCAN_MSG* Buf){
    int i = 0;
    char sendMessage[32] = "";

    sendMessage[i++] = QRCAN_SEND_11BIT_ID;
    sendMessage[i++] = (Buf->Id / 256) <= 9  ? ((Buf->Id / 256) + 48) : ((Buf->Id / 256) + 55);
    sendMessage[i++] = ((Buf->Id % 256) / 16) <= 9  ? (((Buf->Id % 256) / 16) + 48) : (((Buf->Id % 256) / 16) + 55);
    sendMessage[i++] = (Buf->Id % 16) <= 9  ? ((Buf->Id % 16) + 48) : ((Buf->Id % 16) + 55);
    sendMessage[i++] = Buf->Length + 48;

    for (UINT8 l = 0; l < (Buf->Length); l++){
        sendMessage[i++] = (Buf->Data[l] / 16) <= 9  ? ((Buf->Data[l] / 16) + 48) : ((Buf->Data[l] / 16) + 55);
        sendMessage[i++] = (Buf->Data[l] % 16) <= 9  ? ((Buf->Data[l] % 16) + 48) : ((Buf->Data[l] % 16) + 55);
    }

    sendMessage[i++] = ASCII_RETURN;

    return SendDataToHardware(sendMessage);

    //return QRCAN_ERR_OK;
}

//CAN Reception Event
QRCAN_STATUS QRCAN_SetRcvEvent(QRCAN_HANDLE Handle, HANDLE Event){

    return QRCAN_ERR_OK;
}

// Receive CAN Message from Hardware
QRCAN_STATUS QRCAN_Recv(QRCAN_HANDLE Handle, QRCAN_MSG* Buf){

    return ReceiveDataFromHardware();

}

// Close the QRCAN Device
QRCAN_STATUS QRCAN_Close(){

    return CloseDevice();

}
