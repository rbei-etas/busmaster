/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      Struct_ETHERNET.h
 * \brief     This contains definitions of various ETHERNET related structures.
 * \authors   Ashwin R Uchil
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This contains definitions of various ETHERNET related structures.
 */

#pragma once
//MVN
//libxml file includes
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
//~MVN
/* C++ includes */
#include <string>
#include <sstream>

#include <Windows.h>
using namespace std;

/**
 * This structure is used for sending/reciving messages to/from the ETHERNET network
 */
typedef struct sTETHERNET_MSG
{
    unsigned int			m_unMsgID;			// Defined by the user
	unsigned char			m_ucName[100];
    unsigned char			m_ucSourceIP[15];		// IP from where the data is generated
    unsigned char			m_ucDestIP[15];   
	long					m_lSrcPort;
	long					m_lDestPort;
	unsigned char			m_ucSourceMAC[13];
	unsigned char			m_ucDestMac[13];
    unsigned int			m_unDataLen;		// Data len (0..1500)
    unsigned int			m_ucChannel;  
    unsigned char			m_ucData[1500];		// Databytes 0..1500
	unsigned char			m_ucProtocolName[15];	// Name of the Application Layer Protocol	

} STETHERNET_MSG, *PSTETHERNET_MSG;
//typedef sTCAN_MSG* PSTCAN_MSG;

struct sETHERNET_ERROR_INFO
{
    //TODO:
};

/*****************************************************************************/
/*This structure is used for communicating between Driver & ETHERNET Application*/
struct sTETHERNETDATAINFO
{
    STETHERNET_MSG			m_sETHERNETMsg;      //The received / transmitted message
    sETHERNET_ERROR_INFO	m_sErrInfo;
};
typedef sTETHERNETDATAINFO STETHERNETDATAINFO;
typedef sTETHERNETDATAINFO* PSTETHERNETDATAINFO;
/*****************************************************************************/

class sCONTROLLERDETAILSETHERNET
{
private:
    void vCopyMembers(const sCONTROLLERDETAILSETHERNET& objRef)
    {
		m_bMaster						= objRef.m_bMaster;
		m_unDataBitRate					= objRef.m_unDataBitRate;
		m_strApplicationLayerProtocol	= objRef.m_strApplicationLayerProtocol;	
	}
	bool    m_bMaster;
	UINT32  m_unDataBitRate;
	string  m_strApplicationLayerProtocol;	
};

/*****************************************************************************/
/*This structure is used for communicating between Driver & ETHERNET Application*/

typedef struct sTETHERNETDATA
{

public:
    unsigned char    m_ucDataType;  //Type of the message
    LARGE_INTEGER    m_lTickCount;  //Time stamp, Contains the val returned from
    //QueryPerf..Counter()
    STETHERNETDATAINFO       m_uDataInfo;

    static void vSetSortField(int nField);
    static void vSetSortAscending(bool bAscending);
    static int DoCompareIndiv(const void* pEntry1, const void* pEntry2);
    static __int64 GetSlotID(sTETHERNETDATA& pDatCAN);

} STETHERNETDATA, *PSTETHERNETDATA;

#define TX_FLAG                 0x01
#define RX_FLAG                 0x02
#define ERR_FLAG                0x04
#define INTR_FLAG               0x08

#define IS_TX_MESSAGE(a)        (a & TX_FLAG)
#define IS_RX_MESSAGE(a)        (a & RX_FLAG)
#define IS_A_MESSAGE(a)         ((a) & 0x03)
#define IS_ERR_MESSAGE(a)       (a & ERR_FLAG)
#define IS_INTR_MESSAGE(a)      (((a) & INTR_FLAG))