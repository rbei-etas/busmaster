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
 * \file      ConnectionDet.h
 * \brief     Definition file for ConnectionDet
 * \author    Anish Kumar, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for ConnectionDet
 */

#pragma once

/* Project includes */
#include "Datatypes/J1939_DataTypes.h"
#include "J1939_UtilityFuncs.h"

class CConnectionDet
{
private:
    eCON_STATUS m_eConStatus;
public:
    //STJ1939_MSG m_sJ1939Msg;
    //Only for long message
    BYTE  m_byCurrPacket;
    BYTE  m_byMaxPacketWOC2S;
    BYTE m_byTxAckSeqNo;
    BYTE m_byRxSeqNo;

    BYTE   m_RxLongData[MAX_DATA_LEN_J1939];             //RX long data storage
    BYTE   m_BCRXLongData[MAX_DATA_LEN_J1939];
    BYTE   m_TxLongData[MAX_DATA_LEN_J1939];             //TX long data storage
    UINT   m_unTXLongDataLen;
    UINT   m_unRXLongDataLen;
    UINT   m_unRxTotalPackets;
    UINT   m_unRxLastFrameLen;
    UINT64 m_unTimeStamp;
    HANDLE m_hDataAckWait;
    HANDLE m_hDataRxWait;
    HANDLE m_hClear2SendWait;
    HANDLE m_hBCDataRXWait;
    HANDLE m_hDataDelayWait; // Dummy handle just to wait till the delay expires
    // instead of implementing timer function
    UINT32 m_unPGN;
    //STCAN_MSG m_sCanMsg;

    //All broadcast
    UINT64 m_BCTimeStamp;
    BYTE m_BCRxSeqVar;
    BYTE m_BCTotalPackets;
    eCON_MODE m_eRxConMode;
    eCON_MODE m_eTxConMode;
    UINT32 m_BCPGN;
    UINT m_BCRXLongDataLen;
    UINT m_BCLastFrameLen;
public:
    BYTE       m_bySrcAddress;             // Tx msg id for this connection
    BYTE       m_byDestAddress;            // Rx msg id for this connection

    BYTE       m_byResult;
    UINT       m_unNextPacket;

public:
    CConnectionDet(BYTE bySrcAddress,
                   BYTE byDestAddress);
    ~CConnectionDet();
    BOOL bIsMsgRxForThisConnection(UINT32 unExtId);
    //BOOL  bGetAckFrame(BYTE &byData, BOOL bIsLong, BOOL bIsRxReady = TRUE);
    void vInitializeMemberVar();
    void vSetConStatus(eCON_STATUS eConStatus);
    eCON_STATUS eGetConStatus(void);

};
