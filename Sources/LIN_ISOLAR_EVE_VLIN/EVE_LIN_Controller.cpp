/*
 *  EVE_LIN_Controller.h
 *      Created on: 28.04.2013+
  *     Author: Prince Varghese (RBEI/EAR5)
 */

//using namespace std;

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "EVE_LIN_Controller.h"


int InitializeLIN()
{
    enum UDPSocketType socketType;

    unsigned int  portData_Rx = 51111;          // default UDP port for Reception
    socketType = socketData_Rx;
    if (InitializeUDP(socketType, portData_Rx) < 0)
    {
        printf("Initialize: Initialization failed \n");
        return -1;
    }

    unsigned int portData_Tx = 51112;            // default UDP port for Transmission operations
    socketType = socketData_Tx;
    if (InitializeUDP(socketType, portData_Tx) < 0)
    {
        printf("Initialize: Initialization failed \n");
        return -1;
    }

    unsigned int portData_LoopBack = 51113;            // default UDP port for Transmission operations
    socketType = socketData_LoopBack;
    if (InitializeUDP(socketType, portData_LoopBack) < 0)
    {
        printf("Initialize: Initialization failed \n");
        return -1;
    }
    printf("Initialize: SocketUDP Initialization successfull... \n");

    return 0;
}

void CleanUpLIN()
{
    printf("CleanUp: SocketUDP CleanUp successfull... \n");
    CleanUpUDP();
}

int ReceiveLINMessageFromClient( sTLIN_FRAME* stLinFrame, int* udpReadSize)
{
    char udpBuffer[sizeof(sTLIN_FRAME)];
    //    int udpReadSize = 0;
    int retval = NO_UDP_DATA;

    /*************************  Get from BusmasterClient serialized CAN message **********************************/

    *udpReadSize = ReadUDPLoopBack(udpBuffer, sizeof(sTLIN_FRAME));
    retval = TX_LOOPBACK_UDP_DATA;

    if (*udpReadSize <= 0)
    {
        retval = NO_UDP_DATA;
        *udpReadSize = ReadUDPData(udpBuffer, sizeof(sTLIN_FRAME));

        if (*udpReadSize < 0)
        {
            printf("ReadCANMessageFromClient: No UDP messages caught! \n");
            return -1;
        }
        else if(*udpReadSize == 1)
        {
            retval = RX_UDP_PID;

        }
        else
        {
            retval = RX_UDP_DATA;
        }
    }

    if (*udpReadSize > 0)
    {
        /********************** Parse CAN Msg *******************/
        memcpy((char*)stLinFrame, udpBuffer,sizeof(sTLIN_FRAME));
    }
    else
    {
        return 0; // Timeout, no messages
    }

    return retval;
}

int SendLINMessageToClient( const sTLIN_FRAME* stLinFrame)
{
    char* buff;
    buff = (char*)stLinFrame;
    int udpWriteResult = SendUDPData( buff, sizeof(sTLIN_FRAME));
    if (udpWriteResult != 0)
    {
        printf("SendLINMessageToClient: No UDP messages send \n");
        return -1;
    }
    return  udpWriteResult;     //serializedSize;
}
