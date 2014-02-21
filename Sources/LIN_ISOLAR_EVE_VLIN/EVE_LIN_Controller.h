/*
 *  EVE_LIN_Controller.h
 *      Created on: 28.04.2013
 *      Author: Prince Varghese (RBEI/EAR5)
 */

#ifndef EVE_LIN_CONTROLLER_H_
#define EVE_LIN_CONTROLLER_H_

//#include "EtasProtoBuf.h"
#include "Common.h"
#include "SocketUDP.h"
#include "include/Struct_CAN.h"
#include "include/struct_LIN.h"

#define NO_UDP_DATA             0x00
#define RX_UDP_PID              0x01
#define RX_UDP_DATA             0x02
#define TX_LOOPBACK_UDP_DATA    0x03

/**
 * This structure is used for sending/reciving messages to/from the LIN network
 */
//typedef struct sTLIN_MSG
//{
//    unsigned char m_ucMsgTyp;                 // Message Type (0 - Header / 1 - Response)
//    unsigned char m_ucChksumTyp;          // Checksum Type (0 - Classical / 1 - Enhanced)
//    unsigned char m_ucMsgID;                      // Protected Identifier
//    unsigned char m_ucChannel;                    // Channel Number
//    unsigned char m_ucDataLen;                    // Data len (0..8)
//    unsigned char m_ucData[8];                    // Databytes 0..8
//    unsigned char m_ucChksum;                 // Checksum
//} stlin_msg, *PSTLIN_MSG;

//typedef  sTLIN_MSG stlin_msg ;

int InitializeLIN();
void CleanUpLIN();
int ReceiveLINMessageFromClient( sTLIN_FRAME* stLinFrame, int* udpReadSize);
int SendLINMessageToClient( const sTLIN_FRAME* stLinFrame);

#endif /* EVE_LIN_CONTROLLER_H_ */


