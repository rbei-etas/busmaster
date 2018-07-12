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
* \file      LinTransmitter.cpp
* \author    Robin G.K.
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*/


#pragma once
#include "LINTransmitter.h"
#include "BaseDIL_LIN.h"
//#include "..\DIL_Interface\DIL_Interface_extern.h"

CLINTransmitter* CLINTransmitter::m_pouLINTransmitter = nullptr;
int CLINTransmitter::m_nRefCounter = 0;

DWORD WINAPI CLINTransmitter::TransmitThread(LPVOID pVoid)
{
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*)pVoid;
    if (pThreadParam == nullptr)
    {
        return (DWORD)-1;
    }
    CLINTransmitter* pouData = (CLINTransmitter*)pThreadParam->m_pBuffer;

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

CLINTransmitter::CLINTransmitter()
{
    m_ulClientId = 0;
    m_pouTxDataStore = nullptr;
    m_eBusStatus = BUS_DISCONNECTED;
    m_ouTransmitThread.m_pBuffer = this;
    m_ouTransmitThread.m_hActionEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

}
CLINTransmitter::~CLINTransmitter()
{
    m_ouTransmitThread.bTerminateThread();
}
int CLINTransmitter::nGetMsgAt(int nIndex, STLIN_MSG& ouLinMsg)
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
    ouLinMsg.m_ucChannel = pouMsgItem->MsgDetails.nChannel;
    memcpy(ouLinMsg.m_ucData, pouMsgItem->MsgDetails.pchData, pouMsgItem->MsgDetails.nDLC);
    ouLinMsg.m_ucDataLen = pouMsgItem->MsgDetails.nDLC;
    ouLinMsg.m_ucMsgID = pouMsgItem->MsgDetails.nMsgId;
    ouLinMsg.m_ucMsgTyp = pouMsgItem->MsgDetails.nMsgType;

    return hResult;
}
ITransmitter* CLINTransmitter::GetInstance()
{
    if (nullptr == m_pouLINTransmitter)
    {
        m_pouLINTransmitter = new CLINTransmitter();
    }
    m_nRefCounter++;
    return m_pouLINTransmitter;
}
void CLINTransmitter::ReleaseInstance()
{
    m_nRefCounter--;
    if (nullptr != m_pouLINTransmitter && 0 == m_nRefCounter)
    {
        delete m_pouLINTransmitter;
        m_pouLINTransmitter = nullptr;
    }
}
int CLINTransmitter::nPreMessageTransmission()
{
    int hResult = S_FALSE;
    if (nullptr == m_pouTxDataStore)
    {
        return hResult;
    }
    int nSize = m_pouTxDataStore->GetMsgItemCount();
    for (int nCount = 0; nCount < nSize; nCount++)
    {
        TransmitAt(nCount);
    }
    return hResult;
}
int CLINTransmitter::SetTxDataStore(CTxDataStore* pouTxDataStore)
{
    int hResult = S_OK;
    m_pouTxDataStore = pouTxDataStore;
    return hResult;
}
int CLINTransmitter::SetClientId(unsigned long ulClientId)
{
    int hResult = S_OK;
    m_ulClientId = ulClientId;
    return hResult;
}
int CLINTransmitter::OnBusStatusChanged(ESTATUS_BUS eBusStatus)
{
    int hResult = S_FALSE;
    m_eBusStatus = eBusStatus;

    if (BUS_PRECONNECT == eBusStatus)
    {
        hResult = S_OK;
        nPreMessageTransmission();
    }

    else if (BUS_CONNECTED == eBusStatus)
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
int CLINTransmitter::TransmitAt(int nIndex)
{

    HRESULT hResult = S_FALSE;
    CBaseDIL_LIN* pouBaseDIL_LIN = nullptr;
    DIL_GetInterface(LIN, (void**)&pouBaseDIL_LIN);

    if (nullptr == pouBaseDIL_LIN)
    {
        return hResult;
    }

    STLIN_MSG ouMsg;
    if (S_OK == nGetMsgAt(nIndex,ouMsg))
    {
        hResult = pouBaseDIL_LIN->DILL_SendMsg(m_ulClientId, ouMsg);
    }

    return hResult;
}
int CLINTransmitter::TransmitAll()
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
int CLINTransmitter::OnKeyPressed(char chKey)
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