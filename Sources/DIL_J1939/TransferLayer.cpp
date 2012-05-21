/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  TransferLayer.cpp
  Description   :  
  $Log:   X:/Archive/Sources/DIL_J1939/TransferLayer.cpv  $
   
      Rev 1.3   07 Jun 2011 11:11:18   CANMNTTM
    
   
      Rev 1.2   15 Apr 2011 18:48:38   CANMNTTM
   Added RBEI Copyright information.
   
      Rev 1.1   13 Dec 2010 16:37:06   CANMNTTM
   Nodes are made independent of channel.
   Now nodes can send message in any channel.
   
      Rev 1.0   06 Dec 2010 18:47:22   rac2kor
    

  Author(s)     :  Pradeep Kadoor
  Date Created  :  23/11/2010
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/

#include "DIL_J1939_stdafx.h"
#include "J1939_UtilityFuncs.h"
#include "TransferLayer.h"
#include "NetworkMgmt.h"
#include "NodeConManager.h"
#include "DIL_Interface/BaseDIL_CAN.h"
// Forward declarations
class CConnectionDet;



const int nSIZE_OF_BYTE = sizeof(BYTE);
/**************************************************************
 ************************************************************** */
CTransferLayer::CTransferLayer()
{
    m_pIDIL_CAN = NULL;
}

/**************************************************************
 ************************************************************** */
CTransferLayer::~CTransferLayer(void)
{
}

/**************************************************************
 ************************************************************** */
CTransferLayer& CTransferLayer::ouGetTransLayerObj()
{
    static CTransferLayer ouTransferLayerObj;
    return ouTransferLayerObj;
}



/**************************************************************
 ************************************************************** */
LONG CTransferLayer::lTConnectReq(short /*sConNumber*/, char /*cBlockSize*/, eCON_MODE /*eSMmode*/)
{
    return 0L;
}
//{
//    //Get ID for con no., cReate and send CS message, start timer for ack
//    //Give indication, Save data for retransmission???
//    LONG lError = S_FALSE;
//    CConnectionDet* pConDetObj = 
//        CNetworkMgmt::ouGetNWManagementObj().pouGetConDetObj(sConNumber);
//    CNodeConManager* pManager = 
//        CNetworkMgmt::ouGetNWManagementObj().pouGetConMagrFromConnection(sConNumber); 
//    if (pConDetObj != NULL)
//    {
//         lError = pManager->unSendConnectionReq(cBlockSize, eSMmode, pConDetObj);
//    }
//    return lError;
//kadoor}


/**************************************************************
 ************************************************************** */
void CTransferLayer::vTConnectCon(short /*sConNumber*/, eCON_STATUS /*ConStatus*/,
                                  char /*cBlockSize*/, eCON_MODE /*eServiceMode*/)
{
}

//{
//    //Connection indication to network layer
//    CConnectionDet* pConDet =
//        CNetworkMgmt::ouGetNWManagementObj().pouGetConDetObj(sConNumber);
//    CNodeConManager* pConManager =
//        CNetworkMgmt::ouGetNWManagementObj().pouGetConMagrFromConnection(sConNumber);
//    if ((pConDet != NULL) && (pConManager != NULL))
//    {
//        const CNetwork_McNet* pouNetwork 
//                        = CNetworkMgmt::ouGetNWManagementObj().pouGetNetwork();
//        ASSERT(pouNetwork != NULL);
//        DWORD dwLocalLCN, dwRemoteLCN;
//        pouNetwork->bGetLCNValues(pConDet->m_unRemoteCANid, dwLocalLCN, dwRemoteLCN);
//        CNetworkMgmt::ouGetNWManagementObj().vMConnectionCon((short) dwLocalLCN,
//            (short) dwRemoteLCN, pConManager->m_eWDStatus, pConDet->m_eConStatus);
//    }
//kadoor}

/**************************************************************
 ************************************************************** */
void CTransferLayer::vTConnectInd(short /*sConNumber*/,char /*cBlockSize*/, BOOL /*bIsSMEnhance*/)
{
}
//{
//    //Connection indication to network layer
//    CConnectionDet* pConDet =
//        CNetworkMgmt::ouGetNWManagementObj().pouGetConDetObj(sConNumber);
//    CNodeConManager* pConManager =
//        CNetworkMgmt::ouGetNWManagementObj().pouGetConMagrFromConnection(sConNumber);
//    if ((pConDet != NULL) && (pConManager != NULL))
//    {
//        const CNetwork_McNet* pouNetwork 
//                        = CNetworkMgmt::ouGetNWManagementObj().pouGetNetwork();
//        ASSERT(pouNetwork != NULL);
//        DWORD dwLocalLCN, dwRemoteLCN;
//        pouNetwork->bGetLCNValues(pConDet->m_unRemoteCANid, dwLocalLCN, dwRemoteLCN);
//        CNetworkMgmt::ouGetNWManagementObj().vMConnectionInd((short) dwLocalLCN, 
//            (short) dwRemoteLCN, pConManager->m_eWDStatus, pConDet->m_eConStatus);
//    }
//}


/**************************************************************
 ************************************************************** */
void CTransferLayer::vTDisconnectInd(short /*sConNumber*/,eREASON /*eReason*/)
{
}
//{
//    //Change the state of connection
//    //Connection indication to network layer
//    CConnectionDet* pConDet =
//        CNetworkMgmt::ouGetNWManagementObj().pouGetConDetObj(sConNumber);
//   
//    if ((pConDet != NULL))
//    {
//        pConDet->m_eConStatus = T_DISCONNECTED;
//        const CNetwork_McNet* pouNetwork 
//                        = CNetworkMgmt::ouGetNWManagementObj().pouGetNetwork();
//        ASSERT(pouNetwork != NULL);
//        DWORD dwLocalLCN, dwRemoteLCN;
//        pouNetwork->bGetLCNValues(pConDet->m_unRemoteCANid, dwLocalLCN, dwRemoteLCN);
//        //CNetworkMgmt::ouGetNWManagementObj().vMConnectionInd((short) dwLocalLCN, 
//            //(short) dwRemoteLCN, pConManager->m_eWDStatus, pConDet->m_eConStatus);
//        CNetworkMgmt::ouGetNWManagementObj().vMDisconnectInd((short) dwLocalLCN, 
//            (short) dwRemoteLCN, eReason);
//    }
//kadoor}

/**************************************************************
 ************************************************************** */
void CTransferLayer::vTLongDataCon(short /*sConNumber*/, char /*cTransferResult*/)
{
}

//{
//    //Send the confirmation to adaptation layer
//    //Provide msg to adaptation layer
//    //Now it is ready to send new data
//    const CNetwork_McNet* pouNetwork 
//                        = CNetworkMgmt::ouGetNWManagementObj().pouGetNetwork();
//    DWORD dwLC = 0, dwRC = 0;
//    pouNetwork->bGetLCNValues(sConNumber, dwLC, dwRC);
//    CAdaptationLayer::ouGetAdaptLayerObj().vADataCon(dwLC, dwRC, 'L', cTransferResult);
//}

/**************************************************************
 ************************************************************** */
//void CTransferLayer::vTLongDataInd(STMCNET_MSG& sMcNetMsg)
//{
//    //Indication of RX long data, provide data to adaptation layer
//    //Initialize the counters and timers
//    //CAdaptationLayer::ouGetAdaptLayerObj().vADataInd(sMcNetMsg);
//}



/**************************************************************
 ************************************************************** */
void CTransferLayer::vTBroadDataInd(short /*sBroadcastCANId*/,short /*sDataLength*/, BYTE* /*pbData*/)
{
    //Indicate a RX broad cast data
    //CAdaptationLayer::ouGetAdaptLayerObj().vABroadDataInd(sBroadcastCANId, *pbData, 
    //    pbData, sDataLength);
}
/**************************************************************
 ************************************************************** */
void CTransferLayer::vTConTestCon(short /*sConNumber*/, char /*cConnectionStatus*/, char /*cBlockSize*/, char /*cServiceMode*/)
{
    //Confirm the connection request 
}

void CTransferLayer::vTransmitCANMsg(DWORD dwClientID, UINT unID, 
                                     UCHAR ucDataLen, BYTE* pData, UINT unChannel)
{
    STCAN_MSG sMsgCAN = {'\0'};

    sMsgCAN.m_unMsgID = unID;
    sMsgCAN.m_ucDataLen = ucDataLen;
    memcpy(sMsgCAN.m_ucData, pData, ucDataLen);
    //sMsgCAN.m_ucChannel = unChannel;
    sMsgCAN.m_ucChannel = (UCHAR)unChannel;
    sMsgCAN.m_ucRTR = 0;
    sMsgCAN.m_ucEXTENDED = 1;
    m_pIDIL_CAN->DILC_SendMsg(dwClientID, sMsgCAN);
}


void CTransferLayer::vSetIDIL_CAN(CBaseDIL_CAN* pIDIL_CAN)
{
    m_pIDIL_CAN = pIDIL_CAN;
}