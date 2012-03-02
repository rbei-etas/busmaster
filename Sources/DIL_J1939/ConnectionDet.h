
/******************************************************************************
  Project       :  Frame_McNet
  FileName      :  ConnectionDet.h
  Description   :  Keep details of each connenction
  $Log:   X:/Archive/Sources/DIL_J1939/ConnectionDet.h_v  $
 * 
 *    Rev 1.3   15 Apr 2011 18:48:34   CANMNTTM
 * Added RBEI Copyright information.
 * 
 *    Rev 1.2   29 Dec 2010 19:32:38   CANMNTTM
 * Connection mode for both transmission and reception added.
 * 
 *    Rev 1.1   23 Dec 2010 16:52:20   CANMNTTM
 * Macro MAX_MSG_LEN_J1939
 *  instead of MAX_DATA_LEN_J1939 wherever applicable.
 * 
 *    Rev 1.0   06 Dec 2010 18:47:20   rac2kor
 *  
 * 
 *    Rev 1.3   12 Jan 2010 14:22:46   mcnetpl
 * Bugfixing
 * 
 *    Rev 1.2   04 Jan 2010 14:46:12   mcnetpl
 *  
 * 
 *    Rev 1.1   07 Dec 2009 12:58:04   mcnetpl
 * Bugfix
 * 
 *    Rev 1.0   02 Dec 2009 17:53:04   mcnetpl
 * Initial version

  Author(s)     :  Anish Kumar
  Date Created  :  15/09/2009
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/

#if !defined CONNECTIONDET_H__INCLUDED_
#define CONNECTIONDET_H__INCLUDED_

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

#endif //CONNECTIONDET_H__INCLUDED_