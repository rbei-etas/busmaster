/*
 * Common.h
 *
 *  Created on: Jul 5, 2012
 *      Author: samir
 */

#ifndef COMMON_H_
#define COMMON_H_

#ifndef SOFTWARE_VERSION
#define SOFTWARE_VERSION "unknown"
#endif

#define UDP_BUFFER_SIZE 128
#define TCP_BUFFER_SIZE 1460

enum UDPSocketType
{
    socketData_Tx = 1,
    socketData_Rx = 2
};
#endif /* COMMON_H_ */
