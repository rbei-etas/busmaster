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
* \file      CANTransmitter.cpp
* \author    Robin G.K.
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*/

#pragma once
#include "CANTransmitter.h"
#include "CANTxMsgItem.h"

#include "BaseDIL_CAN.h"
//#include "..\DIL_Interface\DIL_Interface_extern.h"

CCANTransmitter* CCANTransmitter::m_pouCANTransmitter = nullptr;
int CCANTransmitter::m_nRefCounter = 0;

DWORD WINAPI CCANTransmitter::TransmitThread(LPVOID pVoid)
{
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*)pVoid;
    if (pThreadParam == nullptr)
    {
        return (DWORD)-1;
    }
    CCANTransmitter* pouData = (CCANTransmitter*)pThreadParam->m_pBuffer;

    if (pouData == nullptr)
    {
        return (DWORD)-1;
    }

    bool bLoopON = true;

    while (bLoopON)
    {
        WaitForSingleObject(pThreadParam->m_hActionEvent, 1);
        switch (pThreadParam->m_unActionCode)
        {
            case INVOKE_FUNCTION:
            {
                pouData->TransmitAll();
            }
            break;

            case SUSPEND:
            {
                WaitForSingleObject(pThreadParam->m_hActionEvent, INFINITE);
            }
            break;

            case EXIT_THREAD:
            {
                bLoopON = false;
            }
            break;

            default:
                break;
        }
    }
    SetEvent(pThreadParam->hGetExitNotifyEvent());
    return 0;
}

CCANTransmitter::CCANTransmitter()
{
    m_ulClientId = 0;
    m_pouTxDataStore = nullptr;
    m_eBusStatus = BUS_DISCONNECTED;
    m_ouTransmitThread.m_pBuffer = this;
    m_ouTransmitThread.m_hActionEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

}
CCANTransmitter::~CCANTransmitter()
{
    m_ouTransmitThread.bTerminateThread();
}
/**************************************************************************************
Function Name    : nGetMsgAt
Input(s)         : nIndex - Index of MsgItem; ouCANMsg - Converted CAN Msg.
Output           : S_OK - If conversion succcessful
Functionality    : Converts MsgItem at nIndex to STCAN_MSG and used during Transmission.
Member of        : CCANTransmitter
Author(s)        : Robin G.K.
Date Created     : 14.02.2016
Requirement ID   :
***************************************************************************************/
int CCANTransmitter::nGetMsgAt(int nIndex, STCAN_MSG& ouCANMsg)
{
    int hResult = S_FALSE;
    ITxMsgItem* pouMsgItem = nullptr;
    if (nullptr == m_pouTxDataStore)
    {
        return hResult;
    }
    m_pouTxDataStore->GetMsgItem(nIndex, pouMsgItem);
    if (nullptr == pouMsgItem)
    {
        return hResult;
    }
    hResult = S_OK;

    ouCANMsg.m_unMsgID = pouMsgItem->MsgDetails.nMsgId;
    ouCANMsg.m_ucEXTENDED = 0;
    if (CCANTxMsgItem::eCANEXT & pouMsgItem->MsgDetails.nMsgType)
    {
        ouCANMsg.m_ucEXTENDED = 1;
    }
    ouCANMsg.m_ucRTR = 0;
    if (CCANTxMsgItem::eCANRTR & pouMsgItem->MsgDetails.nMsgType)
    {
        ouCANMsg.m_ucRTR = 1;
    }
    ouCANMsg.m_ucDataLen = pouMsgItem->MsgDetails.nDLC;
    ouCANMsg.m_ucChannel = pouMsgItem->MsgDetails.nChannel;
    memcpy(ouCANMsg.m_ucData, pouMsgItem->MsgDetails.pchData, pouMsgItem->MsgDetails.nDLC);
    ouCANMsg.m_bCANFD = false;

    return hResult;
}
ITransmitter* CCANTransmitter::GetInstance()
{
    if (nullptr == m_pouCANTransmitter)
    {
        m_pouCANTransmitter = new CCANTransmitter();
    }
    m_nRefCounter++;
    return m_pouCANTransmitter;
}
void CCANTransmitter::ReleaseInstance()
{
    m_nRefCounter--;
    if (nullptr != m_pouCANTransmitter && 0 == m_nRefCounter)
    {
        delete m_pouCANTransmitter;
        m_pouCANTransmitter = nullptr;
    }
}

int CCANTransmitter::SetTxDataStore(CTxDataStore* pouTxDataStore)
{
    int hResult = S_OK;
    m_pouTxDataStore = pouTxDataStore;
    return hResult;
}
int CCANTransmitter::SetClientId(unsigned long ulClientId)
{
    int hResult = S_OK;
    m_ulClientId = ulClientId;
    return hResult;
}
int CCANTransmitter::OnBusStatusChanged(ESTATUS_BUS eBusStatus)
{
    int hResult = S_FALSE;
    m_eBusStatus = eBusStatus;

    if (BUS_CONNECTED == eBusStatus)
    {
        hResult = S_OK;
        m_ouTransmitThread.m_unActionCode = INVOKE_FUNCTION;
        m_ouTransmitThread.bStartThread(TransmitThread);
        SetEvent(m_ouTransmitThread.m_hActionEvent);
    }
    else if (BUS_DISCONNECTED == eBusStatus)
    {
        hResult = S_OK;
        m_ouTransmitThread.m_unActionCode = SUSPEND;
    }
    return hResult;
}
/**************************************************************************************
Function Name    : TransmitAt
Input(s)         : nIndex - Index of the message to be transmitted. (0-Indexed)
Output           : S_OK if transmission successful, else S_FALSE.
Functionality    : Transmits message at particular index in Tx Window.
                   Used by
                   1. SendMessage in TxWindow
                   2. CCANTransmitter::TransmitAll.
Member of        : CCANTransmitter
Author(s)        : Robin G.K.
Date Created     : 14.02.2016
Requirement ID   :
***************************************************************************************/
int CCANTransmitter::TransmitAt(int nIndex)
{

    HRESULT hResult = S_FALSE;
    CBaseDIL_CAN* pouBaseDIL_CAN = nullptr;
    DIL_GetInterface(CAN, (void**)&pouBaseDIL_CAN);

    if (nullptr == pouBaseDIL_CAN)
    {
        return hResult;
    }

    STCAN_MSG ouMsg;
    if (S_OK == nGetMsgAt(nIndex, ouMsg))
    {
        hResult = pouBaseDIL_CAN->DILC_SendMsg(m_ulClientId, ouMsg);
    }

    return hResult;
}
/**************************************************************************************
Function Name    : TransmitAll
Input(s)         : -
Output           : -
Functionality    : Transmits all configured messages in Tx Window on connect.
Member of        : CCANTransmitter
Author(s)        : Robin G.K.
Date Created     : 14.02.2016
Requirement ID   :
***************************************************************************************/
int CCANTransmitter::TransmitAll()
{
    int hResult = S_FALSE;
    if (nullptr == m_pouTxDataStore)
    {
        return hResult;
    }
    hResult = S_OK;

    int nSize = m_pouTxDataStore->GetMsgItemCount();
    ITxMsgItem* pouMsgItem = nullptr;
    for (int nCount = 0; nCount < nSize; nCount++)
    {
        m_pouTxDataStore->GetMsgItem(nCount, pouMsgItem);
        if (nullptr != pouMsgItem)
        {
            if (true == pouMsgItem->TxDetails.m_bTimerEnabled)
            {
                pouMsgItem->TxDetails.nCurrentTimerVal--;
                if (pouMsgItem->TxDetails.nCurrentTimerVal <= 0)
                {
                    TransmitAt(nCount);
                    pouMsgItem->TxDetails.nCurrentTimerVal = pouMsgItem->TxDetails.nActualTimer;
                }
                m_pouTxDataStore->SetMsgItem(nCount, pouMsgItem);
            }
        }

    }
    return hResult;
}
/**************************************************************************************
Function Name    : OnKeyPressed
Input(s)         : chKey - Key associated with the message pressed by the user
Output           : S_OK - If msg is transmitted successfully on key press, else S_FALSE.
Functionality    : Handles Transmission on Key press if Bus is connected.
Member of        : CCANTransmitter
Author(s)        : Robin G.K.
Date Created     : 14.02.2016
Requirement ID   :
***************************************************************************************/
int CCANTransmitter::OnKeyPressed(char chKey)
{
    int hResult = S_FALSE;
    if (nullptr == m_pouTxDataStore)
    {
        return hResult;
    }

    int nSize = m_pouTxDataStore->GetMsgItemCount();
    ITxMsgItem* pouMsgItem = nullptr;
    if (BUS_CONNECTED == m_eBusStatus)
    {
        for (int nCount = 0; nCount < nSize; nCount++)
        {
            pouMsgItem = nullptr;
            m_pouTxDataStore->GetMsgItem(nCount, pouMsgItem);
            if (nullptr != pouMsgItem)
            {
                if (true == pouMsgItem->TxDetails.bKeyEnabled && pouMsgItem->TxDetails.m_chKeyVal == chKey)
                {
                    hResult = S_OK;
                    TransmitAt(nCount);
                }
            }
        }
    }

    return hResult;
}