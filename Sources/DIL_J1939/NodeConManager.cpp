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
 * \file      NodeConManager.cpp
 * \brief     Node Connection Manager
 * \author    Pradeep Kadoor, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Defines the Node Connection Manager.
 */

/* Project includes */
#include "DIL_J1939_stdafx.h"
#include "Include/BaseDefs.h"
#include "DIL_J1939_Extern.h"
#include "DIL_Interface/BaseDIL_CAN.h"
#include "DIL_Interface/BaseDIL_CAN.h"
#include "J1939_UtilityFuncs.h"
#include "NodeConManager.h"
#include "NetworkMgmt.h"
#include "TransferLayer.h"

/**
 * \brief Receive Message Thread Procedure
 *
 * Sets the receive message thread procedure.
 */
DWORD WINAPI TP_RxMsgThreadProc(LPVOID pVoid)
{
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;

    if (pThreadParam == NULL)
    {
        return ((DWORD)-1);
    }

    CNodeConManager* pNodeConMgr = (CNodeConManager*)pThreadParam->m_pBuffer;

    if (pNodeConMgr == NULL)
    {
        return ((DWORD)-1);
    }

    bool bLoopON = true;

    while (bLoopON)
    {
        WaitForSingleObject(pThreadParam->m_hActionEvent, INFINITE);

        switch (pThreadParam->m_unActionCode)
        {
            case WF_BROADCAST:
            {
                pNodeConMgr->WaitandProcessBroadcastData();
            }
            break;

            case WF_LONGDATA:
            {
                pNodeConMgr->WaitForMorePackets();
            }
            break;

            case WF_CLEAR2SEND:
            {
                pNodeConMgr->WaitAfterClear2Send();
            }
            break;

            case EXIT_THREAD:
            {
                bLoopON = false;
            }
            break;

            default:
            case INACTION:
            {
                // nothing right at this moment
            }
            break;
        }
    }

    SetEvent(pThreadParam->hGetExitNotifyEvent());
    return 0;
}

/**
 * \brief Transmit Message Thread Procedure
 *
 * Sets the transmit message thread procedure.
 */
DWORD WINAPI TransmitMsgThreadProc(LPVOID pVoid)
{
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;

    if (pThreadParam == NULL)
    {
        return ((DWORD)-1);
    }

    CNodeConManager* psNodeConMgr = (CNodeConManager*)pThreadParam->m_pBuffer;

    if (psNodeConMgr == NULL)
    {
        return ((DWORD)-1);
    }

    bool bLoopON = true;

    while (bLoopON)
    {
        WaitForSingleObject(pThreadParam->m_hActionEvent, INFINITE);

        switch (pThreadParam->m_unActionCode)
        {
            case INVOKE_FUNCTION:
            {
                while (psNodeConMgr->m_ouMsgBufVSE.GetMsgCount() > 0)
                {
                    int nType = 0, nSize = MAX_MSG_LEN_J1939;
                    int nResult = psNodeConMgr->m_ouMsgBufVSE.ReadFromBuffer(nType, psNodeConMgr->m_abyData, nSize);
                    ASSERT(nResult == CALL_SUCCESS);
                    STJ1939_MSG sJ1939Msg;
                    sJ1939Msg.vSetDataStream(psNodeConMgr->m_abyData);
                    psNodeConMgr->vTransmitMessage(&sJ1939Msg);
                }
            }
            break;

            case EXIT_THREAD:
            {
                bLoopON = false;
            }
            break;

            default:
            case INACTION:
            {
                // nothing right at this moment
            }
            break;
        }
    }

    SetEvent(pThreadParam->hGetExitNotifyEvent());
    return 0;
}

/**
 * \brief Activate
 *
 * Activates a node to transmit or receive messages.
 */
void CNodeConManager::vActivate(void)
{
    m_bIsActive = TRUE;
    m_ouMsgBufVSE.vClearMessageBuffer();
    /*start threads */
    m_ouTransmitThread.m_hActionEvent = m_hTxActionEvent;
    m_ouReceiveThread.m_hActionEvent = m_hRxActionEvent;
    m_ouTransmitThread.m_pBuffer = this;
    m_ouReceiveThread.m_pBuffer = this;
    m_ouTransmitThread.bStartThread(TransmitMsgThreadProc);
    m_ouReceiveThread.bStartThread(TP_RxMsgThreadProc);
}

/**
 * \brief Deactivate
 *
 * Deactivates a node. Stop transmission and reception.
 */
void CNodeConManager::vDeactivate(void)
{
    vRemoveAllConnections();
    m_byNodeAddress = ADDRESS_NULL;
    m_bIsActive = FALSE;
    m_ouMsgBufVSE.vClearMessageBuffer();
    /*start threads */
    m_ouReceiveThread.bTerminateThread();
    m_ouTransmitThread.bTerminateThread();
}

/**
 * \brief  Is Active
 * \return TRUE if node is active, FALSE if node is not active.
 *
 * Returns if node is active or not.
 */
BOOL CNodeConManager::bIsActive(void)
{
    return (m_bIsActive == TRUE);
}

/**
 * \brief Constructor
 *
 * Constructor
 */
CNodeConManager::CNodeConManager(int nNodeNo, char* pacNodeName, UINT64 un64ECUName, BYTE byPrefAddress, BOOL bIsMonNode)
{
    m_bIsMonNode = bIsMonNode;
    m_byPrefAddress = byPrefAddress;
    m_bIsActive    = FALSE;
    //m_byNodeNo   = nNodeNo;
    m_byNodeNo   = (CHAR)nNodeNo;
    strcpy_s(m_acNodeName, pacNodeName);
    m_byConCount = 0;
    m_dwClientID = nNodeNo;
    m_byNodeAddress = ADDRESS_NULL;
    m_u64ECUName.m_un64ECU_NAME = un64ECUName;

    for (int i = 0; i < DEF_MAX_CON_OF_A_NODE; i++)
    {
        m_pNodeConDetArr[i] = NULL;
    }

    m_pConDet = m_pNodeConDetArr[0] = new CConnectionDet(ADDRESS_NULL, ADDRESS_NULL);
    /* Allocate memory to Tx Msg */
    m_sTxMsg.m_pbyData = new BYTE[MAX_DATA_LEN_J1939];
    m_sTxMsg.m_sMsgProperties.m_eType = MSG_TYPE_NONE;
    m_sTxMsg.m_unDLC = 0;
    m_ouTransmitThread.m_hActionEvent = m_hTxActionEvent =  CreateEvent(NULL, FALSE, FALSE, NULL);
    m_ouReceiveThread.m_hActionEvent = m_hRxActionEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_ouTransmitThread.m_pBuffer = this;
    m_ouReceiveThread.m_pBuffer = this;
    m_ouTransmitThread.bStartThread(TransmitMsgThreadProc);
    m_ouReceiveThread.bStartThread(TP_RxMsgThreadProc);
    m_pClBckLDataConf   = NULL;
    m_pClBckLDataInd    = NULL;
    m_pClBckBcLDataConf = NULL;
    m_pClBckBcLDataInd  = NULL;
    m_pClBckNM_ACL      = NULL;
}

/**
 * \brief Destructor
 *
 * Destructor
 */
CNodeConManager::~CNodeConManager(void)
{
    vRemoveAllConnections();
    m_pConDet = NULL;

    //Deallocate memory
    if (m_sTxMsg.m_pbyData != NULL)
    {
        delete[] m_sTxMsg.m_pbyData;
        m_sTxMsg.m_pbyData = NULL;
    }

    m_ouTransmitThread.bTerminateThread();
    m_ouReceiveThread.bTerminateThread();
    CloseHandle(m_ouTransmitThread.m_hActionEvent);
    CloseHandle(m_ouReceiveThread.m_hActionEvent);
}

/**
 * \brief  Get Node Adress
 * \return Node address
 *
 * Returns Node's 8 bit address aquired.
 */
BYTE CNodeConManager::byGetNodeAddress()
{
    return m_byNodeAddress;
}

/**
 * \brief  Get Connection Count
 * \return Connection Count
 *
 * Returns the number connection of count.
 */
BYTE CNodeConManager::byGetConCount()
{
    return m_byConCount;
}

/**
 * \brief Get Connection Details Object
 *
 * Gets the connection details object.
 */
CConnectionDet* CNodeConManager::pouGetConDetObj(BYTE byConDetNum)
{
    CConnectionDet* pcondet = NULL;

    if (byConDetNum < m_byConCount)
    {
        pcondet = m_pNodeConDetArr[byConDetNum];
    }

    return pcondet;
}

/**
 * \brief Add Connection Details Object
 *
 * Adds a connection details object.
 */
BOOL CNodeConManager::bAddConDetObj(CConnectionDet* pConDet)
{
    BOOL bReturn = FALSE;

    if (m_byConCount < DEF_MAX_CON_OF_A_NODE)
    {
        m_pNodeConDetArr[m_byConCount++] = pConDet;
        bReturn = TRUE;
    }

    return bReturn;
}

/**
 * \brief     Is Message For This Node
 * \param[in] unExtId Extended CAN Identifier
 *
 * Checks if the message should be processed or not.
 */
BOOL CNodeConManager::bIsMsgForThisNode(UINT32 unExtId)
{
    UNION_29_BIT_ID uExtId;
    uExtId.m_unExtID = unExtId;
    return ((uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific == m_byNodeAddress)
            ||(uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific == ADDRESS_ALL));
}

/**
 * \brief Read CAN Data Buffer
 *
 * Reads the CAN buffer.
 */
void CNodeConManager::vReadCANdataBuffer()
{
    static sTCANDATA CurrMsgCAN;

    while (m_ouCANBuff.GetMsgCount() > 0)
    {
        //int nCnt = m_ouCANBuff.GetBufferLength();
        //Now broadcast frames to each of the connection det obj
        //They should filter it out and work
        m_ouCANBuff.ReadFromBuffer(&CurrMsgCAN);

        //If the message is RX then only process it
        //If Error then notify user ****TBD****
        if ((m_byNodeAddress != ADDRESS_NULL) && (m_bIsActive == TRUE))
        {
            if (CurrMsgCAN.m_uDataInfo.m_sCANMsg.m_ucEXTENDED == 1)
            {
                if (bIsMsgForThisNode(CurrMsgCAN.m_uDataInfo.m_sCANMsg.m_unMsgID))
                {
                    bProcessNodeLevelMsg(CurrMsgCAN);

                    if (bIsConLevelMsg(CurrMsgCAN.m_uDataInfo.m_sCANMsg.m_unMsgID))
                    {
                        bProcessConLevelMsg(CurrMsgCAN);
                    }
                }
                else if (CurrMsgCAN.m_ucDataType == TX_FLAG)
                {
                    //Just save the timestamp.
                    if (bIsTPDT(CurrMsgCAN.m_uDataInfo.m_sCANMsg.m_unMsgID))
                    {
                        if (m_pConDet->m_eTxConMode == CM_BROADCAST)
                        {
                            m_pConDet->m_BCTimeStamp = CurrMsgCAN.m_lTickCount;
                        }
                        else if (m_pConDet->m_eTxConMode == CM_STANDARD)
                        {
                            m_pConDet->m_unTimeStamp = CurrMsgCAN.m_lTickCount;
                        }
                    }
                }
            }
        }
    }
}

/**
 * \brief Process ACL message
 *
 * Processes address claimed message.
 */
void CNodeConManager::vProcessACLMsg(const STCAN_MSG& sCanMsg)
{
    UNION_29_BIT_ID uExtId = {0};
    UINT64 unECUName;
    memcpy(&unECUName, sCanMsg.m_ucData, MAX_FRAME_DATA_SIZE);
    uExtId.m_unExtID = sCanMsg.m_unMsgID;

    if (uExtId.m_s29BitId.m_bySrcAddress == m_byNodeAddress)
    {
        if (m_u64ECUName.m_un64ECU_NAME < unECUName)
        {
            vSendACLMsg(uExtId.m_s29BitId.m_bySrcAddress, sCanMsg.m_ucChannel, FALSE);
        }
        else if (m_u64ECUName.m_un64ECU_NAME > unECUName)
        {
            //If dynamic address capable
            if (m_u64ECUName.m_sECU_NAME.m_byARB_ADRS_CPL == 0x1)
            {
                m_byNodeAddress = CNetworkMgmt::ouGetNWManagementObj().byGetUnclaimedAddress();
                vSendACLMsg(ADDRESS_ALL, sCanMsg.m_ucChannel, TRUE);
            }
            else
            {
                m_byNodeAddress = ADDRESS_NULL;//Cannot claim address
                vSendACLMsg(ADDRESS_ALL, sCanMsg.m_ucChannel, TRUE);
            }
        }
    }
    else
    {
        CNetworkMgmt::ouGetNWManagementObj().vUpdateAddressMap(unECUName, uExtId.m_s29BitId.m_bySrcAddress);
    }
}

/**
 * \brief Process Node Level Message
 *
 * Processes node level message.
 */
BOOL CNodeConManager::bProcessNodeLevelMsg(const STCANDATA& sCanData)
{
    //Send it to each connection
    BOOL bResult = TRUE;
    STJ1939_MSG sJ1939Msg;
    EJ1939_MSG_TYPE eType = eGetMsgType(sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID,
                                        (BYTE*)sCanData.m_uDataInfo.m_sCANMsg.m_ucData);

    if (eType == MSG_TYPE_NM_ACL)
    {
        vProcessACLMsg(sCanData.m_uDataInfo.m_sCANMsg);
    }
    else if (eType == MSG_TYPE_NM_RQST_ACL)
    {
        UNION_29_BIT_ID uExtId = {0};
        uExtId.m_unExtID = sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID;
        UINT unChannel = sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel;
        vSendACLMsg(uExtId.m_s29BitId.m_bySrcAddress, unChannel, FALSE);
    }

    vCreateTempJ1939Msg(sJ1939Msg, sCanData, sCanData.m_uDataInfo.m_sCANMsg.m_ucDataLen,
                        (BYTE*)sCanData.m_uDataInfo.m_sCANMsg.m_ucData, eType);
    WriteIntoClientsBuffer(sJ1939Msg);
    return bResult;
}

/**
 * \brief Remove All Connections
 *
 * Removes all existing connection.
 */
void CNodeConManager::vRemoveAllConnections()
{
    //Stop the read thread and then stop any proccessing of data
    //in det object and delete them. Delete the node manager
    //and set the value in array to NULL
    for (int i = 0; i < DEF_MAX_CON_OF_A_NODE; i++)
    {
        if (NULL != m_pNodeConDetArr[i])
        {
            delete m_pNodeConDetArr[i];
            m_pNodeConDetArr[i] = NULL;
        }
    }

    m_pConDet = NULL;
}

/**
 * \brief Add Message Buffer
 *
 * Adds message buffers for a node.
 */
LONG CNodeConManager::lAddMsgBuffer(CBaseMsgBufVSE* pouMsgBuf)
{
    LONG lResult = S_OK;

    if (pouMsgBuf != NULL)
    {
        //Find if the buffer is alrady present
        BOOL bFound = FALSE;
        INT_PTR nCount = m_OutBufArr.GetSize();

        for(int i = 0; ((i < nCount) && (!bFound)); i++ )
        {
            if (pouMsgBuf == m_OutBufArr.GetAt(i))
            {
                bFound = TRUE;
            }
        }

        //If not then add to the array
        if ( !bFound )
        {
            m_OutBufArr.Add(pouMsgBuf);
        }
    }

    return lResult;
}

/**
 * \brief Clear Message Buffer
 *
 * Clears all message buffer of a node.
 */
void CNodeConManager::vClearMsgBuffer(CBaseMsgBufVSE* pBufObj)
{
    CBaseMsgBufVSE* pouMsgBuf = NULL;
    INT nCount = (INT)m_OutBufArr.GetSize();

    for(int i = 0; i < nCount; i++ )
    {
        pouMsgBuf = m_OutBufArr.GetAt(i);

        if (pouMsgBuf == pBufObj)
        {
            pouMsgBuf->vClearMessageBuffer();
            m_OutBufArr.RemoveAt(i);
            i = nCount; //break the loop
        }
    }
}

/**
 * \brief Wait Or Timer Callback
 *
 * Call back timer function.
 */
void CALLBACK WaitOrTimerCallback(PVOID lpParameter, BOOLEAN /*TimerOrWaitFired*/)
{
    if (NULL != lpParameter)
    {
        /*CNodeConManager* pNodeConMgr = (CNodeConManager*)lpParameter;
        pNodeConMgr->vSendWatchDogFrame();*/
    }
}

/**
 * \brief Start Timer Function
 *
 * Starts the timer function.
 */
void CNodeConManager::vStartTimerFunction(UINT /*unTimePeriod*/)
{
    if (m_hTimer != NULL)
    {
        DeleteTimerQueueTimer(NULL, m_hTimer, NULL);
    }
    else
    {
        //If the timer was not created then set the WD message
        //        MAKE_WD_TPCI(m_WDMsg[0]);
        //      m_WDMsg[1] = 0;
    }

    CreateTimerQueueTimer(&m_hTimer,
                          NULL,
                          WaitOrTimerCallback,
                          this,
                          0,
                          nTWD,
                          WT_EXECUTEINTIMERTHREAD);
}

/**
 * \brief Stop Timer Function
 *
 * Stops the timer function.
 */
BOOL CNodeConManager::bStopTimerFunction()
{
    BOOL bReturn = FALSE;

    if (m_hTimer != NULL)
    {
        DeleteTimerQueueTimer(NULL, m_hTimer, NULL);
        bReturn = TRUE;
    }

    return bReturn;
}

/**
 * \brief Get Buffer
 *
 * Returns the pointer to CAN buffer.
 */
CBaseCANBufFSE* CNodeConManager::pouGetBuf(void)
{
    return &m_ouCANBuff;
}

/**
 * \brief Write Into Clients Buffer
 *
 * Writes the J1939 messages into its buffers.
 */
void CNodeConManager::WriteIntoClientsBuffer(STJ1939_MSG& sJ1939Msg)
{
    TRACE("%x, %x, %x, %x, %x\n",
          sJ1939Msg.m_sMsgProperties.m_uExtendedID.m_unExtID,
          sJ1939Msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.m_bySrcAddress,
          sJ1939Msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific,
          sJ1939Msg.m_sMsgProperties.m_eDirection,
          m_byNodeAddress);
    static BYTE abyData[MAX_MSG_LEN_J1939] = {0xFF};

    for (int i = 0; i < m_OutBufArr.GetSize(); i++)
    {
        CBaseMsgBufVSE* podCurrBuf = m_OutBufArr.GetAt(i);
        sJ1939Msg.vGetDataStream(abyData);
        podCurrBuf->WriteIntoBuffer(J1939, abyData, sJ1939Msg.unGetSize());
    }
}

/**
 * \brief Get Source Address
 *
 * Extracts the source address from the CAN extended ID.
 */
static BYTE byGetSrcAddress(UINT32 unExtId)
{
    UNION_29_BIT_ID uExtId;
    uExtId.m_unExtID = unExtId;
    return uExtId.m_s29BitId.m_bySrcAddress;
}

/**
 * \brief Perform Operation
 *
 * Performs the operation specified in the input param.
 */
HRESULT CNodeConManager::PerformOperation(BYTE byOperation)
{
    m_ouReceiveThread.m_unActionCode = byOperation;
    SetEvent(m_ouReceiveThread.m_hActionEvent);
    return S_FALSE;
}

/**
 * \brief Procedure Broadcast-Announce-Message
 *
 * Processes broadcast announce message.
 */
void CNodeConManager::vProcBAMMsg(const STCAN_MSG& sCanMsg)
{
    m_pConDet->m_BCRxSeqVar = 0;
    UINT unMsgLen = (UINT)sCanMsg.m_ucData[1];
    unMsgLen |= ((UINT)sCanMsg.m_ucData[2]) << 8;
    m_pConDet->m_BCRXLongDataLen = unMsgLen;
    m_pConDet->m_BCTotalPackets = sCanMsg.m_ucData[3];
    m_pConDet->m_BCLastFrameLen = (UINT)byGetLastFrameLen(m_pConDet->m_BCRXLongDataLen);
    UINT32 unPGN = 0;
    unPGN = (UINT32)sCanMsg.m_ucData[5];
    unPGN |= ((UINT32)(sCanMsg.m_ucData[6])) << 8;
    unPGN |= ((UINT32)(sCanMsg.m_ucData[7])) << 16;
    m_pConDet->m_BCPGN = unPGN;
    m_pConDet->vSetConStatus(T_CONNECTED);
    m_pConDet->m_eRxConMode = CM_BROADCAST;
    PerformOperation(WF_BROADCAST);
}

/**
 * \brief Procedure Connection Request Message
 *
 * Processes request to send message.
 */
void CNodeConManager::vProcConRequestMsg(const STCAN_MSG& sCanMsg)
{
    //If it is not in monitor mode then send ack message
    m_pConDet->m_eRxConMode = CM_STANDARD;
    m_pConDet->vSetConStatus(T_STARTUP);
    m_pConDet->m_byRxSeqNo = 0;
    WORD* pwLen = (WORD*)&(sCanMsg.m_ucData[1]);
    m_pConDet->m_unRXLongDataLen = *pwLen;
    m_pConDet->m_unRxTotalPackets = sCanMsg.m_ucData[3];
    m_pConDet->m_unRxLastFrameLen = (UINT)byGetLastFrameLen(m_pConDet->m_unRXLongDataLen);
    m_pConDet->m_byMaxPacketWOC2S = sCanMsg.m_ucData[4];
    m_pConDet->m_unPGN = (UINT32)sCanMsg.m_ucData[5];
    m_pConDet->m_unPGN |= ((UINT32)sCanMsg.m_ucData[6]) << 8;
    m_pConDet->m_unPGN |= ((UINT32)sCanMsg.m_ucData[7]) << 16;
    vSendConAckMsg(m_pConDet->m_byDestAddress, sCanMsg.m_ucData[4], m_pConDet->m_unPGN, sCanMsg.m_ucChannel) ;
    PerformOperation(WF_CLEAR2SEND);

    //Provide indication to concerned layer
    switch (m_pConDet->eGetConStatus())
    {
        case T_DISCONNECTED:
        {
            CTransferLayer::ouGetTransLayerObj().vTConnectInd( 0,
                    0,
                    0);
        }
        break;

        case T_STARTUP:
        {
            CTransferLayer::ouGetTransLayerObj().vTConnectCon(0, T_CONNECTED,
                    0, CM_STANDARD);
        }
        break;

        case T_CONNECTED:
        {
            CTransferLayer::ouGetTransLayerObj().vTLongDataCon(0, 'F');
            CTransferLayer::ouGetTransLayerObj().vTConnectInd( 0, 0, 0);
        }
        break;
    }
}

/**
 * \brief Reinitialize Connection Details
 *
 * Re-initializes the connection details.
 */
void CNodeConManager::vReInitConDet()
{
    m_pConDet->vInitializeMemberVar();
}

/**
 * \brief Process Command Address Message
 *
 * Processes command address.
 */
void CNodeConManager::vProcessCmdAdresMsg(const STCAN_MSG& sCANMsg, UINT64 un64ECU_NAME)
{
    if (un64ECU_NAME == m_u64ECUName.m_un64ECU_NAME)
    {
        if (m_u64ECUName.m_sECU_NAME.m_byARB_ADRS_CPL == 0x1)//dynamic adres capable
        {
            if (m_byNodeAddress != sCANMsg.m_ucData[2])
            {
                m_byNodeAddress = sCANMsg.m_ucData[2];
                vSendACLMsg(ADDRESS_ALL, sCANMsg.m_ucChannel, TRUE);
            }
        }
    }
}

/**
 * \brief Process Broadcast Data
 *
 * Processes broadcast data.
 */
void CNodeConManager::vProcessBroadCastData(const STCANDATA& sCanData)
{
    const STCAN_MSG& sCanMsg = sCanData.m_uDataInfo.m_sCANMsg;
    BYTE byLastReceived = (m_pConDet->m_BCRxSeqVar) % MAX_SEQ_NO;

    if (byLastReceived == (BYTE)(sCanMsg.m_ucData[0] - 1))
    {
        m_pConDet->m_BCRxSeqVar++;

        if (m_pConDet->m_BCRxSeqVar == m_pConDet->m_BCTotalPackets)
        {
            m_pConDet->m_byResult = DATA_EOM;
            SetEvent(m_pConDet->m_hBCDataRXWait);
            UINT unDataIndex = (m_pConDet->m_BCTotalPackets - 1) * MAX_TPDU_DATA_SIZE;

            for (UINT i = 0; i < m_pConDet->m_BCLastFrameLen; i++)
            {
                m_pConDet->m_BCRXLongData[unDataIndex + i] = sCanMsg.m_ucData[i + 1];
            }

            STJ1939_MSG sJ1939Msg;
            vCreateTempJ1939Msg(sJ1939Msg, sCanData, m_pConDet->m_BCRXLongDataLen,
                                m_pConDet->m_BCRXLongData, MSG_TYPE_BROADCAST);
            sJ1939Msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.vSetPGN(m_pConDet->m_BCPGN);

            //Check if it is a COMMAND ADDRESS MSG
            if (bIsCommandAddress((BYTE)(m_pConDet->m_BCPGN >> 8),
                                  m_pConDet->m_BCRXLongDataLen))
            {
                UINT64 un64ECU_NAME;
                memcpy(&un64ECU_NAME,  m_pConDet->m_BCRXLongData, MAX_FRAME_DATA_SIZE);
                vProcessCmdAdresMsg(sCanData.m_uDataInfo.m_sCANMsg, un64ECU_NAME);
            }

            WriteIntoClientsBuffer(sJ1939Msg);
            vCloseConnection(m_pConDet);
        }
        else
        {
            m_pConDet->m_byResult = DATA_SUCCESS;
            SetEvent(m_pConDet->m_hBCDataRXWait);
            UINT unDataIndex = (m_pConDet->m_BCRxSeqVar - 1) * MAX_TPDU_DATA_SIZE;
            memcpy(&(m_pConDet->m_BCRXLongData[unDataIndex]),
                   &(sCanMsg.m_ucData[1]), MAX_TPDU_DATA_SIZE );
        }
    }
}

/**
 * \brief Process Long Data
 *
 * Processes long data.
 */
void CNodeConManager::vProcessLongData(const STCANDATA& sCanData)
{
    const STCAN_MSG& sCanMsg = sCanData.m_uDataInfo.m_sCANMsg;
    BYTE byLastReceived = (m_pConDet->m_byRxSeqNo) % MAX_SEQ_NO;

    if (byLastReceived == (sCanMsg.m_ucData[0] - 1))
    {
        //First increment the seq counter
        m_pConDet->m_byRxSeqNo++;

        /* If it is the last packet of the session it should send clear to
         send wait for more data packets*/
        if (m_pConDet->m_byCurrPacket == (m_pConDet->m_byMaxPacketWOC2S - 1))
        {
            BYTE abyCanData[MAX_FRAME_DATA_SIZE] = {0xFF};
            //Prepare Clear 2 send msg
            PrepareClear_2_Send(abyCanData, m_pConDet->m_byMaxPacketWOC2S,
                                m_pConDet->m_byRxSeqNo, m_pConDet->m_unPGN);
            //prepare id
            UINT unExtId = Prepare_P2P_Id(PDU_FORMAT_TPCM, m_byNodeAddress,
                                          m_pConDet->m_byDestAddress, DEFAULT_PRIORITY);
            //Send can frame
            vSendFrame(MAX_FRAME_DATA_SIZE, abyCanData, unExtId, sCanMsg.m_ucChannel);
            SetEvent(m_pConDet->m_hDataRxWait);
            PerformOperation(WF_CLEAR2SEND);
        }

        //If it is first packet of the session wait for more data packets
        if (m_pConDet->m_byCurrPacket == 0)
        {
            m_pConDet->m_byResult = DATA_SUCCESS;
            SetEvent(m_pConDet->m_hClear2SendWait);
        }

        m_pConDet->m_byCurrPacket = (m_pConDet->m_byCurrPacket + 1) % m_pConDet->m_byMaxPacketWOC2S;
        UINT unDataIndex = (m_pConDet->m_byRxSeqNo - 1) * MAX_TPDU_DATA_SIZE;

        //If it is the last packet of the whole msg send EOM msg come out of wait
        if (m_pConDet->m_byRxSeqNo == m_pConDet->m_unRxTotalPackets)
        {
            m_pConDet->m_byResult = DATA_EOM;
            SetEvent(m_pConDet->m_hDataRxWait);

            for (UINT i = 0; i < m_pConDet->m_unRxLastFrameLen; i++)
            {
                m_pConDet->m_RxLongData[i + unDataIndex] = sCanMsg.m_ucData[i + 1];
            }

            STJ1939_MSG sJ1939Msg;
            vCreateTempJ1939Msg(sJ1939Msg, sCanData, m_pConDet->m_unRXLongDataLen,
                                m_pConDet->m_RxLongData, MSG_TYPE_DATA);
            sJ1939Msg.m_sMsgProperties.m_uExtendedID.m_s29BitId.vSetPGN(m_pConDet->m_unPGN);
            WriteIntoClientsBuffer(sJ1939Msg);
            EXECUTE_CLBCK_FN(CLBCK_FN_LDATA_IND, m_pConDet->m_unPGN,
                             m_pConDet->m_bySrcAddress,
                             m_pConDet->m_byDestAddress,
                             TRUE);
            BYTE abyCanData[MAX_FRAME_DATA_SIZE] = {0xFF};
            PrepareEOM_ACK(abyCanData, m_pConDet->m_unRXLongDataLen,
                           (BYTE)m_pConDet->m_unRxTotalPackets, m_pConDet->m_unPGN);
            UINT32 unExtId = Prepare_P2P_Id(PDU_FORMAT_TPCM, m_byNodeAddress,
                                            m_pConDet->m_byDestAddress, DEFAULT_PRIORITY);
            vSendFrame(MAX_FRAME_DATA_SIZE, abyCanData, unExtId, sCanMsg.m_ucChannel);
            vCloseConnection(m_pConDet);
        }
        else
        {
            m_pConDet->m_byResult = DATA_SUCCESS;
            SetEvent(m_pConDet->m_hDataRxWait);
            memcpy(&(m_pConDet->m_RxLongData[unDataIndex]), &(sCanMsg.m_ucData[1]),
                   MAX_TPDU_DATA_SIZE);
        }
    }
}

/**
 * \brief Send Connection Abort Message
 *
 * Sends connection abort message.
 */
void CNodeConManager::vSendConAbortMsg(UINT32 unExtId, BYTE byReason, UINT unChannel)
{
    UNION_29_BIT_ID uExtId = {0};
    BYTE abyFrame[MAX_FRAME_DATA_SIZE] = {0xFF};
    uExtId.m_unExtID = unExtId;
    UINT unId = Prepare_P2P_Id(PDU_FORMAT_TPCM, m_byNodeAddress, uExtId.m_s29BitId.m_bySrcAddress,
                               DEFAULT_PRIORITY - 1);
    abyFrame[0] = CB_CON_ABORT;
    abyFrame[1] = byReason;
    abyFrame[2] = 0xFF;
    abyFrame[3] = 0xFF;
    abyFrame[4] = 0xFF;
    UINT32 unPGN = uExtId.m_s29BitId.unGetPGN();
    abyFrame[5] = (BYTE)unPGN;      //W4 Removal
    abyFrame[6] = (BYTE)(unPGN >> 8);
    abyFrame[7] = (BYTE)(unPGN >> 16);
    vSendFrame(MAX_FRAME_DATA_SIZE, abyFrame, unId, unChannel);
}

/**
 * \brief Process Connection Level Message
 *
 * Process connection level msg like TPCM, TPDT, BAM, CON ABORT, etc.
 */
BOOL CNodeConManager::bProcessConLevelMsg(const sTCANDATA& CurrMsgCAN)
{
    BOOL bIsProcessed = TRUE;
    STCAN_MSG sCanMsg = CurrMsgCAN.m_uDataInfo.m_sCANMsg;
    //UINT64 unTimeStamp = CurrMsgCAN.m_lTickCount;
    UNION_29_BIT_ID uExtId = {0};
    uExtId.m_unExtID = sCanMsg.m_unMsgID;

    if (bIsEOM_ACK(sCanMsg.m_unMsgID, sCanMsg.m_ucData[0]))
    {
        if (m_pConDet != NULL)
        {
            m_pConDet->m_byResult = DATA_EOM;
            SetEvent(m_pConDet->m_hDataAckWait);
            vCloseConnection(m_pConDet);
        }
    }
    else if (bIsConAbortMsg(sCanMsg.m_unMsgID, sCanMsg.m_ucData[0]))
    {
        if (m_pConDet != NULL)
        {
            m_pConDet->m_byResult = DATA_CON_ABORT;
            SetEvent(m_pConDet->m_hDataAckWait);
            vCloseConnection(m_pConDet);
        }
    }
    else if (bIsConAckMsg(sCanMsg))
    {
        if (m_pConDet != NULL)
        {
            m_pConDet->m_byResult = DATA_CLEAR_2_SEND;

            if (sCanMsg.m_ucData[1] == 0)
            {
                m_pConDet->m_byResult = DATA_DELAY_2_SEND;
            }

            SetEvent(m_pConDet->m_hDataAckWait);
            m_pConDet->vSetConStatus(T_CONNECTED);
            m_pConDet->m_byMaxPacketWOC2S = sCanMsg.m_ucData[1];
            UINT32 unPGN;
            unPGN = (UINT32)sCanMsg.m_ucData[5];
            unPGN |= ((UINT32)sCanMsg.m_ucData[6]) << 8;
            unPGN |= ((UINT32)sCanMsg.m_ucData[7]) << 16;
            ASSERT(m_pConDet->m_unPGN == unPGN);
        }
    }
    else if (bIsDataAckMsg(sCanMsg))
    {
        if (m_pConDet != NULL)
        {
            m_pConDet->m_byResult = DATA_CLEAR_2_SEND;

            if (sCanMsg.m_ucData[1] == 0)
            {
                m_pConDet->m_byResult = DATA_DELAY_2_SEND;
            }

            m_pConDet->m_byMaxPacketWOC2S = sCanMsg.m_ucData[1];
            m_pConDet->m_byTxAckSeqNo = sCanMsg.m_ucData[2];
            SetEvent(m_pConDet->m_hDataAckWait);
            UINT32 unPGN;
            unPGN = (UINT32)sCanMsg.m_ucData[5];
            unPGN |= ((UINT32)sCanMsg.m_ucData[6]) << 8;
            unPGN |= ((UINT32)sCanMsg.m_ucData[7]) << 16;
            ASSERT(m_pConDet->m_unPGN == unPGN);
        }
    }
    else if(bIsConReqMsg(sCanMsg.m_unMsgID, sCanMsg.m_ucData[0]))
    {
        if (m_pConDet != NULL)
        {
            if (m_pConDet->eGetConStatus() == T_DISCONNECTED)
            {
                vReInitConDet();
                m_pConDet->m_bySrcAddress = m_byNodeAddress;
                m_pConDet->m_byDestAddress = uExtId.m_s29BitId.m_bySrcAddress;
                vProcConRequestMsg(sCanMsg);
            }
            else
            {
                vSendConAbortMsg(sCanMsg.m_unMsgID, REASON_NODE_ENGAGED, sCanMsg.m_ucChannel);
            }
        }
        else
        {
            m_pConDet = pCreateAndGetConDet((BYTE)sCanMsg.m_unMsgID);
            ASSERT(m_pConDet != NULL);
            vReInitConDet();
            m_pConDet->m_bySrcAddress = m_byNodeAddress;
            m_pConDet->m_byDestAddress = uExtId.m_s29BitId.m_bySrcAddress;
            vProcConRequestMsg(sCanMsg);
        }
    }
    else if (bIsBAM(sCanMsg.m_unMsgID, sCanMsg.m_ucData[0]))    /* BROADCAST Announce*/
    {
        if (m_pConDet != NULL)
        {
            if (m_pConDet->eGetConStatus() == T_DISCONNECTED)
            {
                vReInitConDet();
                m_pConDet->m_bySrcAddress = m_byNodeAddress;
                m_pConDet->m_byDestAddress = uExtId.m_s29BitId.m_bySrcAddress;
                vProcBAMMsg(sCanMsg);
            }
        }
        else
        {
            m_pConDet = pCreateAndGetConDet((BYTE)sCanMsg.m_unMsgID);   //W4 Removal
            ASSERT(m_pConDet != NULL);
            vReInitConDet();
            m_pConDet->m_bySrcAddress = m_byNodeAddress;
            m_pConDet->m_byDestAddress = uExtId.m_s29BitId.m_bySrcAddress;
            vProcBAMMsg(sCanMsg);
        }
    }
    else if (bIsTPDT(sCanMsg.m_unMsgID))
    {
        if (m_pConDet != NULL)
        {
            if (m_pConDet->bIsMsgRxForThisConnection(sCanMsg.m_unMsgID))
            {
                if (m_pConDet->m_eRxConMode == CM_BROADCAST)
                {
                    vProcessBroadCastData(CurrMsgCAN);
                }
                else if (m_pConDet->m_eRxConMode == CM_STANDARD)
                {
                    vProcessLongData(CurrMsgCAN);
                }
            }
        }
    }
    else
    {
        bIsProcessed = FALSE;
    }

    return bIsProcessed;
}

/**
 * \brief Send ACL Message
 *
 * Sends address claimed msg.
 */
void CNodeConManager::vSendACLMsg(BYTE byDestAddress, UINT unChannel, BOOL bNewEvent)
{
    UINT32 unExtId = Prepare_P2P_Id(PDU_FORMAT_ACL, m_byNodeAddress, byDestAddress,
                                    DEFAULT_PRIORITY - 1);
    vSendFrame(MAX_FRAME_DATA_SIZE, (BYTE*)&(m_u64ECUName.m_un64ECU_NAME), unExtId, unChannel);
    //Update address map
    CNetworkMgmt::ouGetNWManagementObj().vUpdateAddressMap(m_u64ECUName.m_un64ECU_NAME, m_byNodeAddress);

    if (bNewEvent == TRUE)
    {
        EXECUTE_CLBCK_FN(CLBCK_FN_NM_ACL, 0, m_byNodeAddress, 0xFF, TRUE);
    }
}

/**
 * \brief Transmit Message
 *
 * Transmit J1939 message.
 */
void CNodeConManager::vTransmitMessage(STJ1939_MSG* psMsg)
{
    switch (psMsg->m_sMsgProperties.m_eType)
    {
        case MSG_TYPE_BROADCAST:
        {
            UINT unPGN = psMsg->m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN();
            BYTE byDestAddress = psMsg->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific;

            if (psMsg->m_unDLC > MAX_FRAME_DATA_SIZE)
            {
                BYTE byPriority = psMsg->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPriority;
                pCreateAndGetConDet(byDestAddress);
                m_pConDet->m_eTxConMode = CM_BROADCAST;
                vSendBAM(psMsg->m_unDLC, unPGN, byDestAddress, byPriority, psMsg->m_sMsgProperties.m_byChannel);
                vSendBroadCastData(psMsg->m_pbyData, psMsg->m_unDLC, byDestAddress, byPriority, psMsg->m_sMsgProperties.m_byChannel);
                psMsg->m_sMsgProperties.m_eDirection = DIR_TX;
                psMsg->m_sMsgProperties.m_un64TimeStamp = m_pConDet->m_BCTimeStamp;
                WriteIntoClientsBuffer(*psMsg);
                EXECUTE_CLBCK_FN(CLBCK_FN_BC_LDATA_CONF,
                                 unPGN, m_byNodeAddress,
                                 byDestAddress, TRUE);
            }
            else
            {
                vSendFrame((UCHAR)psMsg->m_unDLC, psMsg->m_pbyData,
                           psMsg->m_sMsgProperties.m_uExtendedID.m_unExtID, psMsg->m_sMsgProperties.m_byChannel);
                EXECUTE_CLBCK_FN(CLBCK_FN_BC_LDATA_CONF,
                                 unPGN, m_byNodeAddress,
                                 byDestAddress, TRUE);
            }
        }
        break;

        case MSG_TYPE_DATA:
        {
            BYTE byDestAddress = psMsg->m_sMsgProperties.
                                 m_uExtendedID.m_s29BitId.
                                 m_uPGN.m_sPGN.m_byPDU_Specific;
            BYTE byPriorirty = psMsg->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPriority;
            UINT unPGN = psMsg->m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN();

            if (psMsg->m_unDLC > MAX_FRAME_DATA_SIZE)
            {
                HRESULT hResult = TransmitRequestToSend(byDestAddress, byPriorirty,
                                                        psMsg->m_unDLC, unPGN, psMsg->m_sMsgProperties.m_byChannel);
                TRACE("SENT REQUEST TO SEND\n");

                if ((hResult == DATA_CLEAR_2_SEND) || (hResult == DATA_DELAY_2_SEND))
                {
                    m_pConDet->vSetConStatus(T_CONNECTED);
                    m_pConDet->m_eTxConMode = CM_STANDARD;
                    m_pConDet->m_unPGN = unPGN;

                    if (SendLongMsg(psMsg, m_pConDet, hResult) == DATA_EOM)
                    {
                        psMsg->m_sMsgProperties.m_eDirection = DIR_TX;
                        psMsg->m_sMsgProperties.m_un64TimeStamp = m_pConDet->m_unTimeStamp;
                        WriteIntoClientsBuffer(*psMsg);
                        EXECUTE_CLBCK_FN(CLBCK_FN_LDATA_CONF,
                                         unPGN, m_byNodeAddress,
                                         byDestAddress, TRUE);
                    }
                    else
                    {
                        EXECUTE_CLBCK_FN(CLBCK_FN_LDATA_CONF,
                                         unPGN, m_byNodeAddress,
                                         byDestAddress, FALSE);
                    }
                }
                else
                {
                    EXECUTE_CLBCK_FN(CLBCK_FN_LDATA_CONF,
                                     unPGN, m_byNodeAddress,
                                     byDestAddress, FALSE);
                }

                vCloseConnection(m_pConDet);
            }
            else
            {
                vSendFrame((UCHAR)psMsg->m_unDLC, psMsg->m_pbyData,
                           psMsg->m_sMsgProperties.m_uExtendedID.m_unExtID, psMsg->m_sMsgProperties.m_byChannel);
                EXECUTE_CLBCK_FN(CLBCK_FN_LDATA_CONF,
                                 unPGN, m_byNodeAddress,
                                 byDestAddress, TRUE);
            }
        }
        break;

        case MSG_TYPE_NM_ACL:
        {
            vSendFrame((UCHAR)psMsg->m_unDLC, psMsg->m_pbyData,
                       psMsg->m_sMsgProperties.m_uExtendedID.m_unExtID, psMsg->m_sMsgProperties.m_byChannel);
            EXECUTE_CLBCK_FN(CLBCK_FN_NM_ACL, 0, m_byNodeAddress, 0xFF, TRUE);
            Sleep(CNetworkMgmt::sg_unTO_RESPONSE);
        }
        break;

        case MSG_TYPE_GROUP_FUNCTIONS:
        {
            //TBD
        }
        break;
    }
}

/**
 * \brief Close Connection
 *
 * Closes all the current connetions.
 */
void CNodeConManager::vCloseConnection(CConnectionDet* pConDet)
{
    ASSERT(pConDet != NULL);
    //pConDet->m_byResult = CON_ABORT;
    //SetEvent(pConDet->m_hBCDataRXWait);
    //SetEvent(pConDet->m_hBCDataRXWait);
    //SetEvent(pConDet->m_hClear2SendWait);
    //Sleep(0);
    pConDet->vSetConStatus(T_DISCONNECTED);
}

/**
 * \brief Send Long Message
 *
 * Sends multi packet data using transport protocol function.
 */
HRESULT CNodeConManager::SendLongMsg(STJ1939_MSG* psMsg, CConnectionDet* pConDet, HRESULT hPreviousResult)
{
    HRESULT hResult = S_FALSE;

    if (hPreviousResult == DATA_DELAY_2_SEND)
    {
        hPreviousResult = DelaySendProcess(pConDet);
    }

    if (hPreviousResult == DATA_CLEAR_2_SEND)
    {
        //BYTE TxFrame[MAX_FRAME_DATA_SIZE] = {0};
        //BYTE byTPCI = 0;
        int nLastFrameNo = (int)unGetNoOfPacketsRequired(psMsg->m_unDLC);
        int nLastFrameLen = (int)byGetLastFrameLen(psMsg->m_unDLC);
        //INT nDataSuccess = DATA_SUCCESS;
        //Send all the frame except the last frame,(nCurrFrameNo < (nLastFrameNo - 1))
        UINT32 unExtId = Prepare_P2P_Id(PDU_FORMAT_TPDT, m_byNodeAddress,
                                        psMsg->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPDU_Specific,
                                        psMsg->m_sMsgProperties.m_uExtendedID.m_s29BitId.m_uPGN.m_sPGN.m_byPriority);
        //int nCurrFrameNo = 0;
        hResult = SendPackets(unExtId, psMsg->m_pbyData, nLastFrameNo, nLastFrameLen, pConDet, psMsg->m_sMsgProperties.m_byChannel);
    }

    return hResult;
}

/**
 * \brief Send Packets
 *
 * Sends the required number of packets and waits for acknowledgement.
 */
HRESULT CNodeConManager::SendPackets(UINT32 unExtId, BYTE* pbyData, UINT unActualPackets,
                                     UINT unLastFrameLen, CConnectionDet* pConDet, UINT unChannel)
{
    HRESULT hResult = S_FALSE;
    UINT unPacksToBeSent = min (pConDet->m_byMaxPacketWOC2S, (unActualPackets - pConDet->m_unNextPacket));
    //Just for the safety
    ResetEvent(pConDet->m_hDataAckWait);

    for (UINT i = 0; i < unPacksToBeSent; i++)
    {
        BYTE byFrame[MAX_FRAME_DATA_SIZE];
        memset(byFrame, 0xFF, MAX_FRAME_DATA_SIZE);
        UINT nDataIndex = (pConDet->m_unNextPacket + i) * MAX_TPDU_DATA_SIZE;
        UINT unFrameSize = MAX_TPDU_DATA_SIZE;

        if ((pConDet->m_unNextPacket + i + 1) == unActualPackets)//If last packet
        {
            unFrameSize = unLastFrameLen;
        }

        memcpy(&byFrame[1], &pbyData[nDataIndex], unFrameSize);
        byFrame[0] = (BYTE)(pConDet->m_unNextPacket + i + 1); //W4 Removal
        vSendFrame(MAX_FRAME_DATA_SIZE, byFrame, unExtId, unChannel);
    }

    Sleep(0); //Allow threads to perform
    pConDet->m_unNextPacket += unPacksToBeSent;
    hResult = nIsMsgSentSuccess(unExtId, pbyData, unActualPackets, unLastFrameLen, pConDet, unChannel);
    return hResult;
}

/**
 * \brief Delay Send Process
 *
 * Delays sending of packets as per recepient request.
 */
HRESULT CNodeConManager::DelaySendProcess(CConnectionDet* pConDet)
{
    INT nDataAck = DATA_CON_ABORT;
    //int nRetrans = 0;
    DWORD dwWaitRet;
    //Just make sure that the event is reset
    ResetEvent(pConDet->m_hDataDelayWait);
    dwWaitRet = WaitForSingleObject(pConDet->m_hDataDelayWait, CNetworkMgmt::sg_unTO_T4);

    if (dwWaitRet == WAIT_OBJECT_0)
    {
        nDataAck = pConDet->m_byResult;

        if (pConDet->m_byResult == DATA_DELAY_2_SEND)
        {
            nDataAck = DelaySendProcess(pConDet);
        }
    }

    return nDataAck;
}

/**
 * \brief Is Message Sent Success
 *
 * Check if it's a message sent success.
 */
INT CNodeConManager::nIsMsgSentSuccess(UINT32 unExtId, BYTE* pbyData, UINT unActualPackets,
                                       UINT unLastFrameLen, CConnectionDet* pConDet, UINT unChannel)
{
    //Now wait for confirmation
    INT nDataAck = DATA_FAILURE;
    //int nRetrans = 0;
    DWORD dwWaitRet;
    //Just make sure that the event is reset
    DWORD dwTimeOut = CNetworkMgmt::sg_unTO_RESPONSE;
    dwWaitRet = WaitForSingleObject(pConDet->m_hDataAckWait, dwTimeOut);

    if (dwWaitRet == WAIT_OBJECT_0)
    {
        nDataAck = pConDet->m_byResult;

        if (pConDet->m_byResult == DATA_CLEAR_2_SEND)
        {
            if (pConDet->m_byTxAckSeqNo == (pConDet->m_unNextPacket % MAX_SEQ_NO))
            {
                if ((pConDet->m_unNextPacket + 1) == unActualPackets) //EOM
                {
                    nDataAck = DATA_EOM;
                }
                else if ((pConDet->m_unNextPacket + 1) < unActualPackets)
                {
                    nDataAck = SendPackets(unExtId, pbyData, unActualPackets, unLastFrameLen, pConDet, unChannel);
                }
            }
            else
            {
                nDataAck = DATA_OUT_OF_ORDER;
            }
        }
        else if (pConDet->m_byResult == DATA_DELAY_2_SEND)
        {
            nDataAck = DelaySendProcess(pConDet);

            if (nDataAck == DATA_CLEAR_2_SEND)
            {
                nDataAck = SendPackets(unExtId, pbyData, unActualPackets, unLastFrameLen, pConDet, unChannel);
            }
        }
    }
    else if ( dwWaitRet == WAIT_TIMEOUT)
    {
        nDataAck = DATA_TIMEOUT;
        /*
        ++nRetrans;*/
    }
    else
    {
        ASSERT(FALSE);
    }

    return nDataAck;
}

/**
 * \brief Send Message Without Acknowledge Request
 *
 * Sends packets without waiting for acknowledgement.
 */
VOID CNodeConManager::vSendMsgWithoutAckReq(UCHAR ucFrameLen, BYTE* pFrameData,
        UINT unID, UINT unChannel)
{
    //BOOL bReturn = FALSE;
    vSendFrame(ucFrameLen, pFrameData, unID, unChannel);
    Sleep(0); //Allow other threads to perform
    //wait for delay time
    Sleep(CNetworkMgmt::sg_unTO_BROADCAST);
}

/**
 * \brief Send Connection Acknowledge Message
 *
 * Sends connection acknowlegment msg.
 */
void CNodeConManager::vSendConAckMsg(BYTE byDestAdres, BYTE byTotalPackets, UINT32 unPGN, UINT unChannel)
{
    //Create CA message
    BYTE byCANMsg[MAX_FRAME_DATA_SIZE] = {0xFF};
    PrepareClear_2_Send(byCANMsg, byTotalPackets, 0, unPGN);
    UINT32 unID = Prepare_P2P_Id(PDU_FORMAT_TPCM, m_byNodeAddress, byDestAdres, DEFAULT_PRIORITY);
    //Send clear to send
    vSendFrame(MAX_FRAME_DATA_SIZE, byCANMsg, unID, unChannel);
}

/**
 * \brief Wait After Clear-to-send
 *
 * Waits for more data packets after sending CLEAR_2_SEND msg.
 */
HRESULT CNodeConManager::WaitAfterClear2Send(void)
{
    HRESULT hResult = S_FALSE;
    ResetEvent(m_pConDet->m_hClear2SendWait);
    DWORD dwWaitRet = WaitForSingleObject(m_pConDet->m_hClear2SendWait, CNetworkMgmt::sg_unTO_T2);

    if (dwWaitRet == WAIT_OBJECT_0)
    {
        if (m_pConDet->m_byResult == DATA_CON_ABORT)
        {
            vCloseConnection(m_pConDet);
        }
        else
        {
            hResult = WaitForMorePackets();
        }
    }
    else if ( dwWaitRet == WAIT_TIMEOUT)
    {
        vCloseConnection(m_pConDet);
    }
    else
    {
        ASSERT(FALSE);
    }

    return hResult;
}

/**
 * \brief Wait For More Packets
 *
 * Waits for more packets after receivng a series of packets.
 */
HRESULT CNodeConManager::WaitForMorePackets(void)
{
    HRESULT hResult = S_FALSE;
    ResetEvent(m_pConDet->m_hDataRxWait);
    DWORD dwWaitRet = WaitForSingleObject(m_pConDet->m_hDataRxWait, CNetworkMgmt::sg_unTO_T1);

    if (dwWaitRet == WAIT_OBJECT_0)
    {
        if (m_pConDet->m_byResult == DATA_LAST_PACKET)
        {
            hResult = WaitAfterClear2Send();
        }
        else if (m_pConDet->m_byResult == DATA_EOM)
        {
            hResult = S_OK;
        }
        else
        {
            hResult = WaitForMorePackets();
        }
    }
    else if ( dwWaitRet == WAIT_TIMEOUT)
    {
        vCloseConnection(m_pConDet);
        EXECUTE_CLBCK_FN(CLBCK_FN_LDATA_IND, m_pConDet->m_unPGN,
                         m_pConDet->m_bySrcAddress,
                         m_pConDet->m_byDestAddress, FALSE);
    }
    else
    {
        ASSERT(FALSE);
    }

    return hResult;
}

/**
 * \brief Wait and Process Broadcast Data
 *
 * Waits for more broadcast data and if received processes it.
 */
HRESULT CNodeConManager::WaitandProcessBroadcastData(void)
{
    HRESULT hResult = S_FALSE;
    DWORD dwWaitRet = WaitForSingleObject(m_pConDet->m_hBCDataRXWait, CNetworkMgmt::sg_unTO_T1);

    if (dwWaitRet == WAIT_OBJECT_0)
    {
        if (m_pConDet->m_byResult == DATA_EOM)
        {
            hResult = S_OK;
        }
        else
        {
            hResult = WaitandProcessBroadcastData();
        }
    }
    else if ( dwWaitRet == WAIT_TIMEOUT)
    {
        vCloseConnection(m_pConDet);
        EXECUTE_CLBCK_FN(CLBCK_FN_LDATA_IND, m_pConDet->m_BCPGN,
                         m_pConDet->m_bySrcAddress,
                         m_pConDet->m_byDestAddress, FALSE);
    }
    else
    {
        ASSERT(FALSE);
    }

    return hResult;
}

/**
 * \brief Send Frame
 *
 * Send a CAN frame.
 */
void CNodeConManager::vSendFrame(UCHAR ucFrameLen, BYTE* pFrameData, UINT unID, UINT unChannel)
{
    CTransferLayer::ouGetTransLayerObj().vTransmitCANMsg( m_dwClientID,
            unID,
            ucFrameLen,
            pFrameData,
            unChannel);
}

/**
 * \brief Form J1939 Message For Sending
 *
 * Constructs a J1939 message for sending from various message types.
 */
void CNodeConManager::vFormJ1939MsgForSending(UINT unChannel,
        STJ1939_MSG& sMsg,
        EJ1939_MSG_TYPE eMsgType,
        UINT32 unPGN,
        BYTE* pbyData,
        UINT unDLC,
        BYTE byPriority,
        BYTE byDestAddress)
{
    sMsg.m_sMsgProperties.m_byChannel = (BYTE)unChannel;    //W4 Removal
    sMsg.m_sMsgProperties.m_eType = eMsgType;

    switch (eMsgType)
    {
        case MSG_TYPE_COMMAND: //Normal Data
        {
            sMsg.m_sMsgProperties.m_eType = MSG_TYPE_DATA;
            sMsg.m_sMsgProperties.m_uExtendedID.m_unExtID
                = Prepare_P2P_Id((BYTE)(unPGN >> 8), m_byNodeAddress, byDestAddress, byPriority);
            sMsg.m_unDLC = unDLC;
            memcpy(sMsg.m_pbyData, pbyData, sMsg.m_unDLC);
        }
        break;

        case MSG_TYPE_REQUEST:
        {
            sMsg.m_sMsgProperties.m_eType = MSG_TYPE_DATA;
            sMsg.m_sMsgProperties.m_uExtendedID.m_unExtID =
                Prepare_P2P_Id(PDU_FORMAT_REQUEST4, m_byNodeAddress,
                               byDestAddress, byPriority);
            sMsg.m_unDLC = REQUEST_PDU_SIZE;
            memcpy(sMsg.m_pbyData, (BYTE*)&unPGN, REQUEST_PDU_SIZE);
        }
        break;

        case MSG_TYPE_DATA:
        {
            sMsg.m_sMsgProperties.m_uExtendedID.m_unExtID
                = Prepare_P2P_Id((BYTE)(unPGN >> 8), m_byNodeAddress, byDestAddress, byPriority);
            sMsg.m_unDLC = unDLC;
            memcpy(sMsg.m_pbyData, pbyData, sMsg.m_unDLC);
        }
        break;

        case MSG_TYPE_BROADCAST:
        {
            sMsg.m_sMsgProperties.m_uExtendedID.m_unExtID
                = Prepare_P2P_Id((BYTE)(unPGN >> 8), m_byNodeAddress, byDestAddress, byPriority);
            sMsg.m_unDLC = unDLC;
            memcpy(sMsg.m_pbyData, pbyData, sMsg.m_unDLC);
        }
        break;

        case MSG_TYPE_ACKNOWLEDGEMENT:
        {
            sMsg.m_sMsgProperties.m_eType = MSG_TYPE_DATA;
            sMsg.m_sMsgProperties.m_uExtendedID.m_unExtID
                = Prepare_P2P_Id(PDU_FORMAT_ACK, m_byNodeAddress, byDestAddress, byPriority);
            sMsg.m_unDLC = MAX_FRAME_DATA_SIZE;
            memcpy(sMsg.m_pbyData, pbyData, MAX_FRAME_DATA_SIZE); // ACKNOWLEDGEMENT_TYPE
        }
        break;

        case MSG_TYPE_GROUP_FUNCTIONS:
        {
            //TBD
        }
        break;

        case MSG_TYPE_NM_ACL:
        {
            m_byNodeAddress = (BYTE)unPGN;
            sMsg.m_sMsgProperties.m_eType = MSG_TYPE_NM_ACL;
            sMsg.m_sMsgProperties.m_uExtendedID.m_unExtID
                = Prepare_P2P_Id(PDU_FORMAT_ACL, m_byNodeAddress, byDestAddress, byPriority);
            sMsg.m_unDLC = MAX_FRAME_DATA_SIZE;
            memcpy(sMsg.m_pbyData, (BYTE*)&(m_u64ECUName.m_un64ECU_NAME),
                   sMsg.m_unDLC);
            /* update network management entry */
            CNetworkMgmt::ouGetNWManagementObj().vUpdateAddressMap(m_u64ECUName.m_un64ECU_NAME, (BYTE)m_byNodeAddress);
        }
        break;

        case MSG_TYPE_NM_RQST_ACL:
        {
            sMsg.m_sMsgProperties.m_eType = MSG_TYPE_DATA;
            sMsg.m_sMsgProperties.m_uExtendedID.m_unExtID
                = Prepare_P2P_Id(PDU_FORMAT_REQUEST4, m_byNodeAddress, byDestAddress, byPriority);
            sMsg.m_unDLC = REQUEST_PDU_SIZE;
            sMsg.m_pbyData[0] = 0x0;
            sMsg.m_pbyData[1] = PDU_FORMAT_ACL;
            sMsg.m_pbyData[2] = 0x0;
        }
        break;

        case MSG_TYPE_NM_CMD_ADDRESS:
        {
            sMsg.m_sMsgProperties.m_eType = MSG_TYPE_BROADCAST;
            sMsg.m_sMsgProperties.m_uExtendedID.m_unExtID
                = Prepare_P2P_Id(PDU_FORMAT_ACL, m_byNodeAddress, byDestAddress, byPriority);
            sMsg.m_unDLC = DATA_LEN_CMD_ADDRESS;
            memcpy(sMsg.m_pbyData, pbyData, sMsg.m_unDLC);
        }
        break;
    }
}

/**
 * \brief Send Message
 *
 * Transmits a J1939 message.
 */
void CNodeConManager::vSendMessage(UINT unChannel, EJ1939_MSG_TYPE eMsgType,
                                   UINT32 unPGN, BYTE* pbyData, UINT unDLC,
                                   BYTE byPriority, BYTE byDestAdres)
{
    vFormJ1939MsgForSending(unChannel, m_sTxMsg, eMsgType, unPGN, pbyData, unDLC, byPriority, byDestAdres);
    static BYTE abyData[MAX_MSG_LEN_J1939] = {0xFF};
    m_sTxMsg.vGetDataStream(abyData);
    UINT unSize = m_sTxMsg.unGetSize();
    m_ouMsgBufVSE.WriteIntoBuffer(J1939, abyData, unSize);
    m_ouTransmitThread.m_unActionCode = INVOKE_FUNCTION;
    m_ouTransmitThread.m_pBuffer = this;
    SetEvent(m_ouTransmitThread.m_hActionEvent);
}

/**
 * \brief Set Callback Function Pointer
 *
 * Set Call back for events.
 */
HRESULT CNodeConManager::SetCallBackFuncPtr(ETYPE_CLBCK_FN eClBckFnType,
        void* pvClBckFn)
{
    HRESULT hResult = S_OK;

    switch (eClBckFnType)
    {
        case CLBCK_FN_LDATA_CONF:
        {
            m_pClBckLDataConf = (PCLBCK_FN_LDATA_CONF)pvClBckFn;
        }
        break;

        case CLBCK_FN_LDATA_IND:
        {
            m_pClBckLDataInd = (PCLBCK_FN_LDATA_IND)pvClBckFn;
        }
        break;

        case CLBCK_FN_BC_LDATA_CONF:
        {
            m_pClBckBcLDataConf = (PCLBCK_FN_BC_LDATA_CONF)pvClBckFn;
        }
        break;

        case CLBCK_FN_BC_LDATA_IND:
        {
            m_pClBckBcLDataInd = (PCLBCK_FN_BC_LDATA_IND)pvClBckFn;
        }
        break;

        case CLBCK_FN_NM_ACL:
        {
            m_pClBckNM_ACL = (PCLBCK_FN_NM_ACL)pvClBckFn;
        }
        break;

        default:
        {
            hResult = S_FALSE;
        }
        break;
    }

    return hResult;
}

/**
 * \brief Execute Callback Function Pointers
 *
 * Executes Callback when events occur.
 */
void CNodeConManager::vExecuteClbckFuncPtrs(ETYPE_CLBCK_FN eClbckType,
        UINT32 unPGN, BYTE bySrc,
        BYTE byDest, BOOL bSuccess)
{
    switch (eClbckType)
    {
        case CLBCK_FN_LDATA_CONF:
        {
            if ((m_pClBckLDataConf != NULL) && (m_bIsActive == TRUE))
            {
                m_pClBckLDataConf(m_dwClientID, unPGN, bySrc, byDest, bSuccess);
            }
        }
        break;

        case CLBCK_FN_LDATA_IND:
        {
            if ((m_pClBckLDataInd != NULL) && (m_bIsActive == TRUE))
            {
                m_pClBckLDataInd(m_dwClientID, unPGN, bySrc, byDest, bSuccess);
            }
        }
        break;

        case CLBCK_FN_BC_LDATA_CONF:
        {
            if ((m_pClBckBcLDataConf != NULL) && (m_bIsActive == TRUE))
            {
                m_pClBckBcLDataConf(m_dwClientID, unPGN, bySrc, byDest, bSuccess);
            }
        }
        break;

        case CLBCK_FN_BC_LDATA_IND:
        {
            if ((m_pClBckBcLDataInd != NULL) && (m_bIsActive == TRUE))
            {
                m_pClBckBcLDataInd(m_dwClientID, unPGN, bySrc, byDest, bSuccess);
            }
        }
        break;

        case CLBCK_FN_NM_ACL:
        {
            if (m_pClBckNM_ACL != NULL)
            {
                m_pClBckNM_ACL(m_dwClientID, bySrc);
            }
        }
        break;
    }
}

/**
 * \brief Send Broadcast-Announce-Message
 *
 * Sends broadcast announce message.
 */
void CNodeConManager::vSendBAM(UINT unMsgSize, UINT32 unPGN,
                               BYTE byDestAddress, BYTE byPriority, UINT unChannel)
{
    UINT unPackets = unGetNoOfPacketsRequired(unMsgSize);
    BYTE byPF = (BYTE)(unPGN >> 8);

    if (byPF > 239) // PDU FORMAT
    {
        byDestAddress = ADDRESS_ALL;
    }

    UINT unExtId = Prepare_P2P_Id(PDU_FORMAT_TPCM, m_byNodeAddress, byDestAddress, byPriority);
    /* Data preparation */
    BYTE abyCANMsg[MAX_FRAME_DATA_SIZE];
    abyCANMsg[0] = 32; //control byte
    WORD* pwTemp = (WORD*)&abyCANMsg[1]; //Message size in BYTE number 2,3
    *pwTemp = (WORD)unMsgSize;
    abyCANMsg[3] = (BYTE)unPackets; // Packet number //W4 Removal
    abyCANMsg[4] = 0xFF; //reserved
    abyCANMsg[5] = (BYTE)unPGN; //PGN in last three bytes.
    abyCANMsg[6] = (BYTE)(unPGN >> 8);
    abyCANMsg[7] = (BYTE)(unPGN >> 16);
    //Send msg
    vSendFrame(MAX_FRAME_DATA_SIZE, abyCANMsg, unExtId, unChannel);
    //Sleep for some time
    Sleep(CNetworkMgmt::sg_unTO_BROADCAST);
}

/**
 * \brief Send Broadcast Data
 *
 * Transmits broadcast data.
 */
void CNodeConManager::vSendBroadCastData(BYTE* pbyData, UINT unMsgSize, BYTE byDestAddress,
        BYTE byPriority, UINT unChannel)
{
    BYTE TxFrame[MAX_FRAME_DATA_SIZE] = {0};
    //BYTE byTPCI = 0;
    int nLastFrameNo = (int)unGetNoOfPacketsRequired(unMsgSize);
    int nLastFrameLen = (int)byGetLastFrameLen(unMsgSize);
    UINT unDataIndex = 0;
    //INT nDataSuccess = DATA_SUCCESS;
    //Send all the frame except the last frame,(nCurrFrameNo < (nLastFrameNo - 1))
    //UCHAR ucVS_SendSeqStateVar = 0;
    UINT32 unExtId;
    unExtId = Prepare_P2P_Id(PDU_FORMAT_TPDT, m_byNodeAddress, byDestAddress, byPriority);

    for (int nCurrFrameNo = 0; nCurrFrameNo < nLastFrameNo; nCurrFrameNo++)
    {
        if (nCurrFrameNo == nLastFrameNo - 1) //last frame
        {
            memset(TxFrame, 0xFF, MAX_FRAME_DATA_SIZE);
            //TxFrame << (nLastFrameLen + 1) * MAX_FRAME_DATA_SIZE;
            //W4 Removal;
            TxFrame[0] = (BYTE)(nCurrFrameNo + 1);
            memcpy(&TxFrame[1], &pbyData[unDataIndex], nLastFrameLen);
            vSendMsgWithoutAckReq(MAX_FRAME_DATA_SIZE, TxFrame,
                                  unExtId, unChannel);
        }
        else
        {
            TxFrame[0] = (BYTE)(nCurrFrameNo + 1);      //W4 Removal
            memcpy(&TxFrame[1], pbyData + unDataIndex, MAX_TPDU_DATA_SIZE);
            vSendMsgWithoutAckReq(MAX_FRAME_DATA_SIZE, TxFrame,
                                  unExtId, unChannel);
            unDataIndex += MAX_TPDU_DATA_SIZE; //Max byte data of J1939 in one CAN frame
        }
    }
}

/**
 * \brief Create And Get Connection Details
 *
 * Creates a new connection, returns the pointer of the
 * CConnectionDet object.
 */
CConnectionDet* CNodeConManager::pCreateAndGetConDet(BYTE byDestAdres)
{
    if (m_pConDet == NULL)
    {
        m_pConDet = m_pNodeConDetArr[0] = new CConnectionDet(m_byNodeAddress, byDestAdres);
        m_byConCount = 1;
    }
    else
    {
        vReInitConDet();
        m_pConDet->m_bySrcAddress = m_byNodeAddress;
        m_pConDet->m_byDestAddress = byDestAdres;
        m_pConDet->vSetConStatus(T_DISCONNECTED);
    }

    return m_pConDet;
}

/**
 * \brief Transmit Request To Send
 *
 * Transmits Request To Send message.
 */
HRESULT CNodeConManager::TransmitRequestToSend(BYTE byDestAddress,
        BYTE byPriority,
        UINT unDLC,
        UINT32 unPGN,
        UINT unChannel)
{
    HRESULT hResult = S_FALSE;
    pCreateAndGetConDet(byDestAddress);

    if ((m_pConDet != NULL) && (m_pConDet->eGetConStatus() == T_DISCONNECTED))
    {
        m_pConDet->m_unPGN = unPGN;
        BYTE byFrameData[MAX_FRAME_DATA_SIZE] = {0xFF};
        vPrepareData(byFrameData, CB_REQ_TO_SEND, unDLC, unPGN);
        UINT32 unExtId = Prepare_P2P_Id(PDU_FORMAT_TPCM, m_byNodeAddress,
                                        byDestAddress, byPriority);
        vSendFrame(MAX_FRAME_DATA_SIZE, byFrameData, unExtId, unChannel);
        //Now wait for confirmation
        ResetEvent(m_pConDet->m_hDataAckWait);
        DWORD dwWaitRet = WaitForSingleObject(m_pConDet->m_hDataAckWait, CNetworkMgmt::sg_unTO_RESPONSE);// pConDet->m_cTAC_AckConnectionTO);

        if (dwWaitRet == WAIT_OBJECT_0)
        {
            hResult = m_pConDet->m_byResult;
        }
        else if ( dwWaitRet == WAIT_TIMEOUT)
        {
            hResult = DATA_TIMEOUT;
        }
        else
        {
            ASSERT(FALSE);
        }
    }

    return hResult;
}

/**
 * \brief Send Request For PGN
 *
 * Requests a node for the PGN.
 */
void CNodeConManager::vSendRequestForPGN(UINT32 unPGN, BYTE byDestAdres,
        UINT unChannel)
{
    UINT32 unExtId = Prepare_P2P_Id(PDU_FORMAT_REQUEST4, m_byNodeAddress,
                                    byDestAdres, DEFAULT_PRIORITY - 1);
    BYTE byFrameData[REQUEST_PDU_SIZE] = {0xFF};
    byFrameData[0] = (BYTE)unPGN;
    byFrameData[1] = (BYTE)(unPGN >> 8);
    byFrameData[2] = (BYTE)(unPGN >> 16);
    vSendFrame(REQUEST_PDU_SIZE, byFrameData, unExtId, unChannel);
}

/**
 * \brief Start Address Claim Procedure
 *
 * Starts address claiming procedure for a node.
 */
HRESULT CNodeConManager::StartAdresClaimProc(BYTE byAddress)
{
    HRESULT hResult = S_FALSE;
    m_byNodeAddress = byAddress;
    LPARAM lParam;

    if (CNetworkMgmt::ouGetNWManagementObj().GetICANDIL()
            ->DILC_GetControllerParams(lParam, 0, NUMBER_HW) == S_OK)
    {
        /* Note address to be claimed is sent with PGN */
        for (UINT i = 0; i < (UINT)lParam; i++)
        {
            vSendMessage(i + 1, MSG_TYPE_NM_ACL, m_byNodeAddress, (BYTE*)&m_u64ECUName.m_un64ECU_NAME,
                         MAX_FRAME_DATA_SIZE, DEFAULT_PRIORITY - 1, ADDRESS_ALL);
        }

        hResult = S_OK;
    }

    return hResult;
}

/**
 * \brief Is Monitor Node
 *
 * Returns whether the client is a monitor or not.
 */
BOOL CNodeConManager::bIsMonitorNode(void)
{
    return m_bIsMonNode;
}

/**
 * \brief Set Node Address
 *
 * Sets clients node address.
 */
void CNodeConManager::vSetNodeAddress(BYTE byAddress)
{
    m_byNodeAddress = byAddress;
}
