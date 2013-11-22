/*
 * SocketUDP.cpp
 *
 *  Created on: 23.11.2011
 *      Author: samir
 */
//using namespace std;

#include <iostream>
#include <stdio.h>
#include <string.h>
#include "SocketUDP.h"

struct sockaddr_in m_localAddrData_Rx;
struct sockaddr_in m_remoteAddrData_Rx;
struct sockaddr_in m_localAddrData_Tx;
struct sockaddr_in m_remoteAddData_Tx;

SOCKET m_socketFDData_Rx;
SOCKET m_socketFDData_Tx;

unsigned int m_portData_Rx;
unsigned int m_portData_Tx;

// MS Transport Provider IOCTL to control
// reporting PORT_UNREACHABLE messages
// on UDP sockets via recv/WSARecv/etc.
// Path TRUE in input buffer to enable (default if supported),
// FALSE to disable.
#define SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR,12)

SOCKET InitializeUDP(enum UDPSocketType socketType, unsigned int port)
{
    if (socketType == socketData_Rx)
    {
        m_portData_Rx = port;

        if ((m_socketFDData_Rx = CreateSocketUDP()) == -1)
        {
            fprintf(stderr, "InitializeUDP: Error creating UDP socket \n");
            return -1;
        }
        printf("m_socketFDData_Rx: %d \n", m_socketFDData_Rx);
    }
    else if (socketType == socketData_Tx)
    {
        m_portData_Tx = port;

        if ((m_socketFDData_Tx = CreateSocketUDP()) == -1)
        {
            fprintf(stderr, "InitializeUDP: Error creating UDP socket \n");
            return -1;
        }
        printf("m_socketFDData_Tx: %d \n", m_socketFDData_Tx);
    }
    else
    {
        fprintf(stderr, "InitializeUDP: Error to identify socket type \n");
        return -1;
    }

    if (InitializeParametersUDP(socketType) == -1)
    {
        fprintf(stderr, "InitializeUDP: Error binding UDP socket \n");
        return -1;
    }

    return 0;
}

void CleanUpUDP()
{
    closesocket(m_socketFDData_Rx);
    closesocket(m_socketFDData_Tx);
    WSACleanup();
}

SOCKET CreateSocketUDP()
{
    WSADATA wsaData;

    // Initialize Winsock
    int result = WSAStartup(MAKEWORD(2,2), &wsaData); // to initiate use of WS2_32.dll
    if (result != 0)
    {
        fprintf(stderr, "CreateSocketUDP: WSAStartup() failed: %d \n", result);
        return -1;
    }

    /* Open a datagram socket */
    SOCKET socketFD = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socketFD == INVALID_SOCKET)
    {
        fprintf(stderr, "CreateSocketUDP: Could not create socket.\n");
        WSACleanup();
        return -1;
    }

    return socketFD;
}

int InitializeParametersUDP(enum UDPSocketType socketType)
{
    if (socketType == socketData_Rx)
    {
        memset(&m_remoteAddrData_Rx, 0, sizeof(m_remoteAddrData_Rx));
        m_remoteAddrData_Rx.sin_family = AF_INET; // host byte order
        m_remoteAddrData_Rx.sin_port = htons(m_portData_Rx); // short, network byte order
        m_remoteAddrData_Rx.sin_addr.s_addr = inet_addr("192.168.40.14");

        memset(&m_localAddrData_Rx, 0, sizeof(m_localAddrData_Rx));
        m_localAddrData_Rx.sin_family = AF_INET; // host byte order
        m_localAddrData_Rx.sin_port = htons(m_portData_Rx); // short, network byte order
        m_localAddrData_Rx.sin_addr.s_addr = inet_addr("192.168.40.240");

        if ((bind(m_socketFDData_Rx, (struct sockaddr*)&m_localAddrData_Rx, sizeof(m_localAddrData_Rx))) < 0)
        {
            fprintf(stderr, "InitializeParametersUDP: Could not bind UDP Socket!\n");
            return -1;
        }

        DWORD   dwBytesReturned = 0;
        BOOL        bNewBehavior = FALSE;
        DWORD   status = 0;

        // disable new behavior using IOCTL: SIO_UDP_CONNRESET
        status = WSAIoctl(m_socketFDData_Rx, SIO_UDP_CONNRESET, &bNewBehavior, sizeof(bNewBehavior), NULL, 0, &dwBytesReturned, NULL, NULL);
        if (SOCKET_ERROR == status)
        {
            DWORD dwErr = WSAGetLastError();
            if (WSAEWOULDBLOCK == dwErr)
            {
                fprintf(stderr, "InitializeParametersUDP: Could not disable UDP-Socket option 'SIO_UDP_CONNRESET' \n");
                return -1;
            }
            else
            {
                fprintf(stderr, "WSAIoctl(SIO_UDP_CONNRESET) Error: %ld \n", dwErr);
                return -1;
            }
        }
    }
    else if (socketType == socketData_Tx)
    {
        memset(&m_remoteAddData_Tx, 0, sizeof(m_remoteAddData_Tx));
        m_remoteAddData_Tx.sin_family = AF_INET;                                            // host byte order
        m_remoteAddData_Tx.sin_port = htons(m_portData_Tx);                         // short, network byte order
        m_remoteAddData_Tx.sin_addr.s_addr = inet_addr("192.168.40.14");

        memset(&m_localAddrData_Tx, 0, sizeof(m_localAddrData_Tx));
        m_localAddrData_Tx.sin_family = AF_INET;                                                // host byte order
        m_localAddrData_Tx.sin_port = htons(m_portData_Tx);                             // short, network byte order
        m_localAddrData_Tx.sin_addr.s_addr = inet_addr("192.168.40.240");

        if ((bind(m_socketFDData_Tx, (struct sockaddr*)&m_localAddrData_Tx, sizeof(m_localAddrData_Tx))) < 0)
        {
            fprintf(stderr, "InitializeParametersUDP: Could not bind UDP Socket!\n");
            return -1;
        }

        DWORD   dwBytesReturned = 0;
        BOOL        bNewBehavior = FALSE;
        DWORD   status = 0;

        // disable new behavior using IOCTL: SIO_UDP_CONNRESET
        status = WSAIoctl(m_socketFDData_Tx, SIO_UDP_CONNRESET, &bNewBehavior, sizeof(bNewBehavior), NULL, 0, &dwBytesReturned, NULL, NULL);
        if (SOCKET_ERROR == status)
        {
            DWORD dwErr = WSAGetLastError();
            if (WSAEWOULDBLOCK == dwErr)
            {
                fprintf(stderr, "InitializeParametersUDP: Could not disable UDP-Socket option 'SIO_UDP_CONNRESET' \n");
                return -1;
            }
            else
            {
                fprintf(stderr, "WSAIoctl(SIO_UDP_CONNRESET) Error: %ld \n", dwErr);
                return -1;
            }
        }
    }
    else
    {
        fprintf(stderr, "InitializeParametersUDP: Unknown request for socket type \n");
        return -1;
    }
    return 0;
}

int ReadUDPData(char* buffer, unsigned int bufferLength)
{
    struct timeval tv;
    tv.tv_sec = 0; // timeout
    tv.tv_usec = 100000; // 100ms
    fd_set readSet;
    int rval = 0;

    if (buffer == NULL)
    {
        fprintf(stderr, "ReadUDPData: No memory for buffer allocated \n");
        return -1;
    }

    do
    {
        FD_ZERO(&readSet);
        FD_SET(m_socketFDData_Rx, &readSet);

        rval = select(m_socketFDData_Rx + 1, &readSet, NULL, NULL, &tv);
    }
    while(rval == -1 && WSAGetLastError() == WSAEINTR); // errno == EINTR


    if (rval == SOCKET_ERROR)
    {
        fprintf(stderr, "ReadUDPData: select() function is failed \n");
        return -1;
    }
    else if (rval > 0)
    {
        if (FD_ISSET(m_socketFDData_Rx, &readSet) != 0) // The socketFDData has data available to be read
        {
            memset(buffer, 0, bufferLength);
            struct sockaddr_in remoteAddr;
            int remoteAddrLen = sizeof(remoteAddr);

            rval =  recvfrom(m_socketFDData_Rx, (char*)buffer, bufferLength, 0, (struct sockaddr*)&remoteAddr, &remoteAddrLen);
            //          printf("RX_remoteAddr:  %s:%d \n", inet_ntoa(remoteAddr.sin_addr), ntohs(remoteAddr.sin_port));

            if (rval == 0) // This means the other side closed the socket
            {
                closesocket(m_socketFDData_Rx);
                fprintf(stderr, "ReadUDPData: Could not read from UDP socket - close!\n");
                return -1;
            }
            else if (rval == SOCKET_ERROR)
            {
                perror("ReadUDPData:recvfrom");
                return -1;
            }

            if (bufferLength < rval)
            {
                fprintf(stderr, "ReadUDPData: bufferLength (%d) is not enough \n", bufferLength);
                return -1;
            }

            printf("ReadUDPData: receiving message (%d bytes)... \n", rval);
        }
    }
    else // rval = 0
    {
        printf("ReadUDPData: UDP select timeout...\n");
    }

    return rval;
}

int SendUDPData(char* buffer, unsigned int bufferLength)
{
    int rval = sendto(m_socketFDData_Tx, buffer, bufferLength, 0, (struct sockaddr*)&m_remoteAddData_Tx, sizeof(m_remoteAddData_Tx));  //send(socketFD, buffer, bufferLength, 0);
    if (rval == SOCKET_ERROR)
    {
        printf("SendUDPData: sendto failed: %d \n", WSAGetLastError());
        return -1;
    }

    if ((int)bufferLength > rval)
    {
        printf("SendUDPCtrl: Incomplete UDP write! rval: %d \n", rval);
        return -1;
    }

    //  printf("\nNumber of Bytes Send  on Port  %d  is %d \n", m_portData_Tx, rval);

    return 0;
}