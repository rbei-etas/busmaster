/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  NodeConManager.h
  Description   :  
  $Log:   X:/Archive/Sources/DIL_J1939/NodeConManager.h_v  $
 * 
 *    Rev 1.8   15 Apr 2011 18:48:38   CANMNTTM
 * Added RBEI Copyright information.
 * 
 *    Rev 1.7   07 Apr 2011 12:22:58   CANMNTTM
 * 'vClearBuffer' modified inorder to remove individual buffers.
 * 
 *    Rev 1.6   02 Mar 2011 11:36:44   CANMNTTM
 * SetCallBackFuncPtr function is added.
 * 
 *    Rev 1.5   13 Jan 2011 14:47:02   CANMNTTM
 * GoOnline() return value is used.
 * 
 *    Rev 1.4   23 Dec 2010 16:52:20   CANMNTTM
 * Macro MAX_MSG_LEN_J1939
 *  instead of MAX_DATA_LEN_J1939 wherever applicable.
 * 
 *    Rev 1.3   22 Dec 2010 19:23:42   CANMNTTM
 * 1. Implemented Call back mechanism.
 * 2. Exported function SetClBckFnPtrs and GetTimeOut added.
 * 
 *    Rev 1.2   14 Dec 2010 17:53:04   CANMNTTM
 * Defined a virtual function vTransmitMessage.
 * 
 *    Rev 1.1   13 Dec 2010 16:37:06   CANMNTTM
 * Nodes are made independent of channel.
 * Now nodes can send message in any channel.
 * 
 *    Rev 1.0   06 Dec 2010 18:47:22   rac2kor
 *  

  Author(s)     :  Pradeep Kadoor
  Date Created  :  23/11/2010
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/
#if !defined NODE_CONMANAGER_H__INCLUDED_
#define NODE_CONMANAGER_H__INCLUDED_

#include "ConnectionDet.h"
#include "DataTypes/MsgBufVSE.h"
#define DEF_MAX_CON_OF_A_NODE           15


class CNodeConManager
{
private:
    CConnectionDet* m_pNodeConDetArr[DEF_MAX_CON_OF_A_NODE];
    BYTE   m_byNodeNo;
    CPARAM_THREADPROC m_ouTransmitThread;
    CPARAM_THREADPROC m_ouReceiveThread;
    BYTE m_byNodeAddress;
    UNION_ECU_NAME m_u64ECUName;
    BOOL m_bIsMonNode;
    
    STJ1939_MSG m_sTxMsg;
    STJ1939_MSG m_sRxMsg;
    //BYTE m_byChannel;
    HANDLE m_hTxActionEvent;
    HANDLE m_hRxActionEvent;
    PCLBCK_FN_LDATA_CONF    m_pClBckLDataConf;
    PCLBCK_FN_LDATA_IND     m_pClBckLDataInd;
    PCLBCK_FN_BC_LDATA_CONF m_pClBckBcLDataConf;
    PCLBCK_FN_BC_LDATA_IND  m_pClBckBcLDataInd;
    PCLBCK_FN_NM_ACL        m_pClBckNM_ACL;
protected:

    CConnectionDet* m_pConDet;
    CArray <CBaseMsgBufVSE*, CBaseMsgBufVSE*> m_OutBufArr;
    CCANBufFSE m_ouCANBuff;
    //CString m_omStrNodeName;
    BYTE   m_byConCount;
    BOOL   m_bIsActive;
    HANDLE m_hTimer;
    
private:
    void vTransmitLongData(short sDataLength, BYTE *pbData, CConnectionDet *pConDet);   
    INT nIsMsgSentSuccess(UINT32 unExtId, BYTE* pbyData, UINT unNoOfPackets, 
                                    UINT unLastFrameLen, CConnectionDet *pConDet, UINT unChannel);
    VOID vSendMsgWithoutAckReq(UCHAR ucFrameLen, BYTE* pFrameData, 
                                        UINT unID, UINT unChannel);
    HRESULT SendPackets(UINT32 unExtId, BYTE* pbyData, UINT unActualPackets, UINT nLastFrameLen,
                        CConnectionDet* pConDet, UINT unChannel);
    void vSendRequestForPGN(UINT32 unPGN, BYTE byDestAdres, UINT unChannel);
    HRESULT DelaySendProcess(CConnectionDet* pConDet);
    CConnectionDet* pCreateAndGetConDet(BYTE byDestAdres);
    
    BOOL bProcessConLevelMsg(const STCANDATA& sCanMsg);
    void vProcBAMMsg(const STCAN_MSG& sCanMsg);    
    
    void vReInitConDet();
    void vProcessBroadCastData(const STCANDATA& sCanMsg);
    void vProcessLongData(const STCANDATA& sCanMsg);
    void vSendConAbortMsg(UINT32 unExtId, BYTE byReason, UINT unChannel);
    HRESULT PerformOperation(BYTE byOperation);
    void vSendACLMsg(BYTE byDestAddress, UINT unChannel, BOOL bNewEvent);
    BOOL bIsMsgForThisNode(UINT32 unExtId);
    void vFormJ1939MsgForSending(UINT unChannel, STJ1939_MSG& sMsg, 
                                 EJ1939_MSG_TYPE eMsgType, UINT32 unPGN, 
                                 BYTE* pbyData, UINT unDLC, BYTE byPriority, 
                                 BYTE byDestAddress);
    void vProcessACLMsg(const STCAN_MSG& sCanMsg);
    void vProcessCmdAdresMsg(const STCAN_MSG& sCANMsg, UINT64 un64ECU_NAME);
protected:
    void vSendFrame(UCHAR ucFrameLen, BYTE* pFrameData, UINT unID, UINT unChannel); 
    virtual void vProcConRequestMsg(const STCAN_MSG& sCanMsg);
public:
    virtual CBaseCANBufFSE* pouGetBuf(void);
    virtual BOOL bAddConDetObj(CConnectionDet* pConDet);
    virtual BOOL bProcessNodeLevelMsg(const sTCANDATA& CurrMsgCAN);
    virtual void vReadCANdataBuffer();
    virtual void vRemoveAllConnections();
    virtual void vTransmitMessage(STJ1939_MSG* psMsg);
    HRESULT TransmitRequestToSend(BYTE byDestAddress, BYTE byPriority, 
                                  UINT unDLC, UINT32 unPGN, UINT unChannel);
    HRESULT StartAdresClaimProc(BYTE byAddress);
    void vSendBAM(UINT unMsgSize, UINT32 unPGN, BYTE byDestAddress, 
                  BYTE byPriority, UINT unChannel);
    void vSendBroadCastData(BYTE* pbyData, UINT unMsgSize, BYTE byDestAddress, 
                            BYTE byPriority, UINT unChannel);
    HRESULT SendLongMsg(STJ1939_MSG* psMsg, CConnectionDet* pConDet, HRESULT hPreviousResult);
    HRESULT WaitForMorePackets(void);
    HRESULT WaitAfterClear2Send(void);
    HRESULT WaitandProcessBroadcastData(void);
    void vCloseConnection(CConnectionDet* pConDet);
    void WriteIntoClientsBuffer(STJ1939_MSG& sJ19Msg);
public:
    BYTE   m_WDMsg[2];
    DWORD  m_dwClientID;
    CMsgBufVSE m_ouMsgBufVSE;
    BYTE m_abyData[MAX_MSG_LEN_J1939];
    TCHAR m_acNodeName[MAX_PATH];
    BYTE m_byPrefAddress;
public:
    CNodeConManager(int nNodeNo, TCHAR* pacNodeName, UINT64 un64NAME, 
                    BYTE byPrefAddress = ADDRESS_NULL, BOOL bIsMonNode = FALSE);
    virtual ~CNodeConManager(void);
    BYTE byGetConCount();
    BYTE byGetNodeAddress();
    CConnectionDet* pouGetConDetObj(BYTE byConDetNum);
    BOOL bStopTimerFunction();
    void vStartTimerFunction(UINT unTimePeriod);
    LONG lAddMsgBuffer( CBaseMsgBufVSE* pouMsgBuf);
    void vSendConAckMsg(BYTE byDestAdres, BYTE byTotalPackets, UINT32 unPGN, UINT unChannel);
    void vClearMsgBuffer(CBaseMsgBufVSE* pBufObj);
    void vSendMessage(UINT unChannel, EJ1939_MSG_TYPE eMsgType, UINT32 unPGN,
                      BYTE* pbyData, UINT unDLC,BYTE byPriority, BYTE byDestAdres);
    void vActivate(void);
    void vDeactivate(void);
    BOOL bIsActive(void);
    BOOL bIsMonitorNode(void);
    void vSetNodeAddress(BYTE byAddress);
    HRESULT SetCallBackFuncPtr(ETYPE_CLBCK_FN eClBckFnType, void* pvClBckFn);
    void vExecuteClbckFuncPtrs(ETYPE_CLBCK_FN eClbckType, UINT32 unPGN, BYTE bySrc, 
                                BYTE byDest, BOOL bSuccess);
};

#endif // NODE_CONMANAGER_H__INCLUDED_
