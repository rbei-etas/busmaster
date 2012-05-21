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
 * \file      ConnectionDet.cpp
 * \brief     Connection Details
 * \author    Pradeep Kadoor, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Defines the connction details.
 */

/* Project includes */
#include "DIL_J1939_stdafx.h"
#include "J1939_UtilityFuncs.h"
#include "ConnectionDet.h"

const int nMNT             =10;
const int nMNTC            =10;
const int nMNCT            =10;
const int nMNBE            =5;
const int nTA              =100; //ms
const int nTAC             =1000;//ms
const int nTAT             =100; //ms
const int nTWDC            =5000;//ms
const int nTCT             =1000;//ms
const int nTD              =50; //ms
const int nTDE             =100; //ms

/**
 * \brief Constructor
 *
 * Constructor
 */
CConnectionDet::CConnectionDet(BYTE bySrcAddress,
                               BYTE byDestAddress)
{
    vInitializeMemberVar();
    m_bySrcAddress = bySrcAddress;
    m_byDestAddress = byDestAddress;
    //Create Event
    m_hDataAckWait = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hBCDataRXWait = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hDataRxWait = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hClear2SendWait = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_hDataDelayWait = CreateEvent(NULL, FALSE, FALSE, NULL);
}

/**
 * \brief Destructor
 *
 * Destructor
 */
CConnectionDet::~CConnectionDet(void)
{
    CloseHandle(m_hDataAckWait);
    CloseHandle(m_hDataRxWait);
    CloseHandle(m_hClear2SendWait);
    CloseHandle(m_hBCDataRXWait);
    CloseHandle(m_hDataDelayWait);
}

/**
 * \brief Initialize Member Variables
 *
 * Initializes the member variables.
 */
void CConnectionDet::vInitializeMemberVar()
{
    m_byTxAckSeqNo      = 0;
    m_byRxSeqNo         = 0;
    m_byDestAddress     = 0;
    m_bySrcAddress      = 0;
    m_eConStatus        = T_DISCONNECTED;
    m_eRxConMode        = CM_STANDARD;
    m_eTxConMode        = CM_STANDARD;
    m_unTXLongDataLen   = 0;
    m_unRXLongDataLen   = 0;
    m_unRxTotalPackets  = 0;
    m_unRxLastFrameLen  = 0;
    m_unTimeStamp       = 0;
    m_BCTimeStamp       = 0;
    m_BCRxSeqVar        = 0;
    m_BCTotalPackets    = 0;
    m_BCPGN             = 0;
    m_BCRXLongDataLen   = 0;
    m_BCLastFrameLen    = 0;
    m_bySrcAddress      = ADDRESS_NULL;
    m_byDestAddress     = ADDRESS_NULL;
    m_byResult          = DATA_FAILURE;
    m_unNextPacket      = 0;
    m_byCurrPacket      = 0;
    m_byMaxPacketWOC2S  = 0;
}

/**
 * \brief Is Message Received For This Connection
 *
 * Checks if this message received for this connection.
 */
BOOL CConnectionDet::bIsMsgRxForThisConnection(UINT32 unExtId)
{
    UNION_29_BIT_ID uExtId = {0};
    uExtId.m_unExtID = unExtId;
    BYTE bySrcAdres = uExtId.m_s29BitId.m_bySrcAddress;
    BYTE byDestAdres = uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific;
    BYTE byPF = uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Format;
    return (((byDestAdres == m_bySrcAddress) ||(byDestAdres == ADDRESS_ALL) || (byPF > 239)) &&
            (m_byDestAddress == bySrcAdres));
}

/**
 * \brief Set Connection Status
 *
 * Sets the connection status.
 */
void CConnectionDet::vSetConStatus(eCON_STATUS eConStatus)
{
    m_eConStatus = eConStatus;
}

/**
 * \brief Get Connection Status
 *
 * Gets the connection status.
 */
eCON_STATUS CConnectionDet::eGetConStatus(void)
{
    return m_eConStatus;
}