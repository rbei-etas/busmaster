/*
 * SocketUDP.h
 *
 *  Created on: 23.11.2011
 *      Author: samir
 */

#ifndef SOCKETUDP_H_
#define SOCKETUDP_H_

#include "Common.h"
#include <WinSock2.h>
#include <ws2tcpip.h>

SOCKET InitializeUDP(enum UDPSocketType socketType, unsigned int port);
void CleanUpUDP();
SOCKET CreateSocketUDP();
int InitializeParametersUDP(enum UDPSocketType socketType);
int ReadUDPData(char* buffer, unsigned int bufferLength);
int SendUDPData(char* buffer, unsigned int bufferLength);
int ReadUDPLoopBack(char* buffer, unsigned int bufferLength);

#endif   /* SOCKETUDP_H_ */

