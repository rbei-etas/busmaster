/*
 * Copyright (c) 1999 - 2005 NetGroup, Politecnico di Torino (Italy)
 * Copyright (c) 2005 - 2006 CACE Technologies, Davis (California)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Politecnico di Torino, CACE Technologies 
 * nor the names of its contributors may be used to endorse or promote 
 * products derived from this software without specific prior written 
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#pragma once
#include "Ethernet_Interface_Stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include "pcap.h"
#include "Win32-Extensions.h"
#include "../../Include/Struct_ETHERNET.h"

/* 4 bytes IP address */
typedef struct ip_address
{
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ip_address;

/* IPv4 header */
typedef struct ip_header
{
	u_char	ver_ihl;		// Version (4 bits) + Internet header length (4 bits)
	u_char	tos;			// Type of service 
	u_short tlen;			// Total length 
	u_short identification; // Identification
	u_short flags_fo;		// Flags (3 bits) + Fragment offset (13 bits)
	u_char	ttl;			// Time to live
	u_char	proto;			// Protocol
	u_short crc;			// Header checksum
	ip_address	saddr;		// Source address
	ip_address	daddr;		// Destination address
	u_int	op_pad;			// Option + Padding
}ip_header;

/* UDP header*/
typedef struct udp_header
{
	u_short sport;			// Source port
	u_short dport;			// Destination port
	u_short len;			// Datagram length
	u_short crc;			// Checksum
}udp_header;

typedef struct mac_header{
	u_char dest[6];			// Destination port
    u_char source[6];		// Source port
    u_short type;			// Datagram length
    u_short crc;			// Checksum
}mac_header;

typedef pcap_pkthdr  PPKTHDR;

#ifdef _MSC_VER
/*
 * we do not want the warnings about the old deprecated and unsecure CRT functions
 * since these examples can be compiled under *nix as well
 */
#define _CRT_SECURE_NO_WARNINGS
#endif
#define WIN32_LEAN_AND_MEAN
//
//
//// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
//#pragma comment (lib, "Ws2_32.lib")
//#pragma comment (lib, "Mswsock.lib")
//#pragma comment (lib, "AdvApi32.lib")

#define MAX_BUFF_ALLOWED 2000

typedef struct sReceivedInfo
{
	string			strData;
	string			strSrcIP;
	int				nPort;
} SRECEIVEDINFO;

class CConnector
{
private:
	u_int			m_unNetmask;
	struct addrinfo *m_sResult;
	SOCKET m_ListenSocket;
	SOCKET m_ClientSocket;

	pcap_if_t *m_pAllDevs;
	
	char errbuf[PCAP_ERRBUF_SIZE];
	//static CRITICAL_SECTION   m_CritSectReadSync;

public:
	CConnector();
	~CConnector();
	//HRESULT Initialize(string strPort,string& strError);
	//HRESULT CreateSocket();
	//HRESULT ListenToSocket();
	//HRESULT ShutDownSocket();
	//HRESULT CleanUp();	
	//HRESULT SendMsgs(string strMsg);
	//HRESULT ReceiveMsgs(SRECEIVEDINFO &strMsg, string &strError);
	//SOCKET& GetClientSocket();
	
	pcap_if_t* SearchAdapters();
	HRESULT ConnectToAdapter(pcap_if_t* pAdapter);
	HRESULT ReceivePackets(PPKTHDR** pPktHdr,u_char** pcPktData);
	HRESULT SendMsg(const STETHERNET_MSG& sEthMsg);
	HRESULT SetFilter(string strFilter);
	HANDLE GetEvent();
	HRESULT SetReceiveCallBackFn();
	void StartStopReceptionLoop(bool bStopReceptionLoop);
};