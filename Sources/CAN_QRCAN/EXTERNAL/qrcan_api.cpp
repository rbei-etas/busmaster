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

QRCAN_DEVICE qrcanDevice;

// Configure CAN bus
QRCAN_STATUS QRCAN_Open(){
    if (qrcanDevice.commMode == QRCAN_USE_ETHERNET){

        SOCKADDR_IN serverAddr;

        // Setting up SOCKADDR_IN structure that will be used to connect to a listening server on port 7.
        serverAddr.sin_family      = AF_INET;
        serverAddr.sin_port        = htons(qrcanDevice.serverPort);
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
    else if (qrcanDevice.commMode == QRCAN_USE_USB){

        qrcanDevice.q_hComm = CreateFile(qrcanDevice.serialPortName, 
                                        GENERIC_READ | GENERIC_WRITE,
                                        0, 
                                        0, 
                                        OPEN_EXISTING,
                                        FILE_FLAG_OVERLAPPED,
                                        0);

        if(qrcanDevice.q_hComm == INVALID_HANDLE_VALUE){
            if(GetLastError() == ERROR_FILE_NOT_FOUND){
                // Inform user that serial port does not exist
                AfxMessageBox("Serial port does not exist");
            }
            else{
            // Some other error occured.
            AfxMessageBox("Unknown error occured");
            }
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

        // The values for timeouts are in ms. The values should be the following if we use the event based reception.
        // These values can be 20, 10, 100, 10, 100 if polling technique is used
        COMMTIMEOUTS timeouts = {0};
        timeouts.ReadIntervalTimeout = MAXDWORD;
        timeouts.ReadTotalTimeoutConstant = 0;
        timeouts.ReadTotalTimeoutMultiplier = 0;
        timeouts.WriteTotalTimeoutConstant = 0;
        timeouts.WriteTotalTimeoutMultiplier = 0;

        if (!SetCommTimeouts(qrcanDevice.q_hComm, &timeouts)){
            // Error occured
        }
    }
    else{
        return QRCAN_ERR_NOT_OK;
    }

    return QRCAN_ERR_OK;

}

QRCAN_STATUS QRCAN_Config(QRCAN_HANDLE Handle, struct QRCanCfg* cfg)
{
    int i = 0;
    char sendMessage[32] = "";

    sendMessage[i++] = QRCAN_SET_BAUD_RATE;
    sendMessage[i++] = cfg->canBaudRate;
    sendMessage[i++] = ASCII_RETURN;

    return QRCAN_SendDataToHardware(sendMessage);

    return QRCAN_ERR_OK;
}

//  Set Serial Communication Events
QRCAN_STATUS QRCAN_SetEvent(){
    if (qrcanDevice.commMode == QRCAN_USE_ETHERNET){
        // Add the code for Ethernet

        return QRCAN_ERR_OK;
    }

    else if (qrcanDevice.commMode == QRCAN_USE_USB){
        //  Setting COM port to create an event on data reception or data transfer complete 
        if (!SetCommMask(qrcanDevice.q_hComm, EV_RXCHAR)){
            AfxMessageBox("Error in setting communication event mask");
            return QRCAN_ERR_NOT_OK;
        }

        // Initialize Overload for Read
        qrcanDevice.ovRead.Internal     = NULL;
        qrcanDevice.ovRead.InternalHigh = NULL;
        qrcanDevice.ovRead.Offset       = 0;
        qrcanDevice.ovRead.OffsetHigh   = 0;
        qrcanDevice.ovRead.Pointer      = 0;

        // Initialize Overload for Write
        qrcanDevice.ovWrite.Internal     = NULL;
        qrcanDevice.ovWrite.InternalHigh = NULL;
        qrcanDevice.ovWrite.Offset       = 0;
        qrcanDevice.ovWrite.OffsetHigh   = 0;
        qrcanDevice.ovWrite.Pointer      = 0;

        memset(&qrcanDevice.ovRead, 0, sizeof(qrcanDevice.ovRead));
        memset(&qrcanDevice.ovWrite, 0, sizeof(qrcanDevice.ovWrite));

        //  Create an event to denote completion of data transfer
        qrcanDevice.ovWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        if (qrcanDevice.ovWrite.hEvent == NULL){
            AfxMessageBox("Error creating overlapped write event");
        }

        //  Create an event to denote the arrival of a character
        qrcanDevice.ovRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        if (qrcanDevice.ovRead.hEvent == NULL){
            AfxMessageBox("Error creating overlapped read event");
        }

        return QRCAN_ERR_OK;
    }
}

// Send CAN Message from BUSMASTER
QRCAN_STATUS QRCAN_SendCANMessage(QRCAN_MSG* Buf){
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

    return QRCAN_SendDataToHardware(sendMessage);
}

QRCAN_STATUS QRCAN_SendDataToHardware(char *data){
    if (qrcanDevice.commMode == QRCAN_USE_ETHERNET){
        int bytesSent;

        // Send data to the Server
        bytesSent = send(qrcanDevice.sendingSocket, data, strlen(data), 0);

        if (bytesSent == SOCKET_ERROR){
            CString msg;
            msg.Format(_T("%d"), WSAGetLastError());
            AfxMessageBox(msg);
            AfxMessageBox("Sending data Failed through Ethernet");
            return QRCAN_ERR_NOT_OK;
        }
        return QRCAN_ERR_OK; 
    }

    else if(qrcanDevice.commMode == QRCAN_USE_USB){
        DWORD dwBytesWritten = 0;
        // Issue Write
        if (!WriteFile(qrcanDevice.q_hComm, data, strlen(data), &dwBytesWritten, &qrcanDevice.ovWrite)){
            if (GetLastError() != ERROR_IO_PENDING){
                AfxMessageBox("WriteFile failed, but it is not delayed");            
            }
            else {
                // Write is pending
                if (!GetOverlappedResult(qrcanDevice.q_hComm, &qrcanDevice.ovWrite, &dwBytesWritten, TRUE)) {
                    AfxMessageBox("WriteFile is pending");
                }
                else {
                    return QRCAN_ERR_OK;
                }            
            }
            // Error Occured
            AfxMessageBox("Failed to send data through serial port");
            return QRCAN_ERR_NOT_OK;
        }
        else {
            return QRCAN_ERR_OK;
        }           
    }

    else{
        // The Communication mode is neither USB nor Ethernet
        AfxMessageBox("Invalid Communication Media");
        return QRCAN_ERR_NOT_OK;
    }
}

// Receive CAN Message from Hardware
QRCAN_STATUS QRCAN_RecveiveDataFromHardware(QRCAN_MSG* Buf, DWORD* CanMsgArrived){
    char messageReceived[BUFFER_SIZE] = {};

    if (qrcanDevice.commMode == QRCAN_USE_ETHERNET){
        int bytesReceived = 0;

        // Something more should be added here
        bytesReceived = recv(qrcanDevice.sendingSocket, messageReceived, MAX_PACKET_SIZE, 0);
    
    }
    else if (qrcanDevice.commMode == QRCAN_USE_USB){      
        DWORD dwBytesRead = 0;
        char receivedByte;
        int i = 0;
        
        do {
            if (!ReadFile(qrcanDevice.q_hComm, &receivedByte, 1, &dwBytesRead, &qrcanDevice.ovRead)){
                AfxMessageBox("Failed to receive data from Serial port");
                return QRCAN_ERR_NOT_OK;
            }
            messageReceived[i++] = receivedByte;
        } while(dwBytesRead > 0);
                
        // Reset the read event after successful completion of read
        ResetEvent(qrcanDevice.ovRead.hEvent);
    }

    switch(messageReceived[0]){
    case QRCAN_SEND_11BIT_ID:
        QRCAN_RecvCANMessage(Buf, messageReceived);
        *CanMsgArrived = 1;
        break;
    case QRCAN_SET_BAUD_RATE:
        *CanMsgArrived = 0;
        break;
    default:
        // Could be an unrecognized message. Report error
        *CanMsgArrived = 0;
        break;
    }

    return QRCAN_ERR_OK;

}

QRCAN_STATUS QRCAN_RecvCANMessage(QRCAN_MSG* Buf, char *receivedData){
    UINT32 messageId    = 0;
    UINT8 messageLength = 0;
    UINT8 messageData   = 0;

    switch(*receivedData){
    case QRCAN_SEND_11BIT_ID:
        // Setting Message ID
        // TODO: ASCIItoInt function returns wrong results. Fix it

        messageId += ASCIItoInt(*(++receivedData)) * 256;
        messageId += ASCIItoInt(*(++receivedData)) * 16;
        messageId += ASCIItoInt(*(++receivedData));

        Buf->Id = messageId;

        // Setting Message Length
        messageLength = ASCIItoInt(*(++receivedData));
        Buf->Length = messageLength;

        // Setting Message data
        for (int i = 0; i< messageLength; i++){
            messageData = 0;

            messageData += ASCIItoInt(*(++receivedData)) * 16;
            messageData += ASCIItoInt(*(++receivedData));

            Buf->Data[i] = messageData;        
        }
        break;

    default:
        // Could be an unrecognized CAN message format. Report Error
        break;
    }    
    return QRCAN_ERR_OK; 
}

UINT8 ASCIItoInt(UINT8 asciiCharacter)
{
  if (asciiCharacter - '0' >= 0 && asciiCharacter - '0' <= 9)
  {
    return asciiCharacter - '0';
  }
  else if (asciiCharacter - 55 >= 10 && asciiCharacter - 55 <= 15)
  {
    return asciiCharacter - '0';
  }
  else 
    AfxMessageBox("Unrecognized character in CAN Message");
}

// Close the QRCAN Device
QRCAN_STATUS QRCAN_Close(){
    if (qrcanDevice.commMode == QRCAN_USE_ETHERNET){
        // Shutdown connection
        if (shutdown(qrcanDevice.sendingSocket, SD_SEND) != 0){
            CString msg;
            msg.Format(_T("%d"), WSAGetLastError());
            AfxMessageBox(msg);
            AfxMessageBox("Error during shutdown");
            return QRCAN_ERR_NOT_OK;
        }
        return QRCAN_ERR_OK;
    }
    else if (qrcanDevice.commMode == QRCAN_USE_USB){
        CloseHandle (qrcanDevice.q_hComm);
        return QRCAN_ERR_OK;
    }
}