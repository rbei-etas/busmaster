/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  TransferLayer.h
  Description   :  
  $Log:   X:/Archive/Sources/DIL_J1939/TransferLayer.h_v  $
 * 
 *    Rev 1.2   15 Apr 2011 18:48:38   CANMNTTM
 * Added RBEI Copyright information.
 * 
 *    Rev 1.1   13 Dec 2010 16:37:06   CANMNTTM
 * Nodes are made independent of channel.
 * Now nodes can send message in any channel.
 * 
 *    Rev 1.0   06 Dec 2010 18:47:24   rac2kor
 *  

  Author(s)     :  Pradeep Kadoor
  Date Created  :  23/11/2010
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/


#if !defined TRANSFERLAYER_H__INCLUDED_
#define TRANSFERLAYER_H__INCLUDED_

#include "DIL_Interface/BaseDIL_CAN.h"

class CTransferLayer
{
private:
    CTransferLayer(void);
    CBaseDIL_CAN* m_pIDIL_CAN;
public:
    ~CTransferLayer(void);

    // CONNECTION SET-UP SERVICE -- START
    // Request for connection establishment
    LONG lTConnectReq(short sConNumber, char bBlockSize, eCON_MODE eSMmode);
    // Confirmation of connection establishment
    void vTConnectCon(short sConNumber, eCON_STATUS ConStatus, 
                      char bBlockSize, eCON_MODE bServiceMode);
    // Indication of connection establishment
    void vTConnectInd(short sConNumber, char bBlockSize, BOOL bIsSMEnhance);
    // Indication of connection termination
    void vTDisconnectInd(short sConNumber, eREASON eReason);
    // CONNECTION SET-UP SERVICE -- END

    // CONNECTION TEST SERVICE -- START
    // Request for connection test
    LONG lTConTestReq(short sConNumber);
    // Confirmation of connection test
    void vTConTestCon(short sConNumber, char cConnectionStatus, char cBlockSize, char cServiceMode);
    void vTLongDataCon(short sConNumber, char cTransferResult);  
    void vTBroadDataInd(short sBroadcastChannel,short sDataLength, BYTE *pbData);

    //Singleton class 
    static CTransferLayer& ouGetTransLayerObj();
    void vTransmitCANMsg(DWORD dwClientID, UINT unID, 
                         UCHAR ucDataLen, BYTE* pData, UINT unChannel);
    void vSetIDIL_CAN(CBaseDIL_CAN* pIDIL_CAN);

};

#endif //TRANSFERLAYER_H__INCLUDED_