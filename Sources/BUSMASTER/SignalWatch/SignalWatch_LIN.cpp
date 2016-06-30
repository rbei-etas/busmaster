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
* \file      SignalWatch_LIN.cpp
* \author    Shashank Vernekar
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*/
#include "SignalWatch_stdafx.h"
#include "Include/Utils_macro.h"
#include "SignalWatch_LIN.h"
#include "Utility/MsgInterpretation.h"
#include "include/XMLDefines.h"
#include "Utility/XMLUtils.h"


int CSignalWatch_LIN::ReadLINDataBuffer( CSignalWatch_LIN* pSWLin )
{
    ASSERT(pSWLin != nullptr);


    while (pSWLin->m_ouLinBufFSE.GetMsgCount() > 0)
    {
        static STLINDATA sLinData;
        INT Result = pSWLin->m_ouLinBufFSE.ReadFromBuffer(&sLinData);
        if (Result == ERR_READ_MEMORY_SHORT)
        {
            TRACE("ERR_READ_MEMORY_SHORT");
        }
        else if (Result == EMPTY_APP_BUFFER)
        {
            TRACE("EMPTY_APP_BUFFER");
        }
        else
        {
            pSWLin->vDisplayInSigWatchWnd(sLinData);
        }

    }
    return 0;
}

DWORD WINAPI CSignalWatch_LIN::SigWatchDataReadThreadProc_L( LPVOID pVoid )
{
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;
    if (pThreadParam == nullptr)
    {
        return (DWORD)-1;
    }
    CSignalWatch_LIN* pSWLin = (CSignalWatch_LIN*)pThreadParam->m_pBuffer;
    if (pSWLin == nullptr)
    {
        return (DWORD)-1;
    }

    bool bLoopON = true;
    pThreadParam->m_unActionCode = INVOKE_FUNCTION;
    while (bLoopON)
    {
        WaitForSingleObject(pThreadParam->m_hActionEvent, INFINITE);
        switch (pThreadParam->m_unActionCode)
        {
            case INVOKE_FUNCTION:
            {
                pSWLin->ReadLINDataBuffer( pSWLin ); // Retrieve message from the driver
            }
            break;
            case EXIT_THREAD:
            {
                bLoopON = false;
            }
            break;
            case CREATE_TIME_MAP:
            {
                //Nothing at this moment
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

CSignalWatch_LIN::CSignalWatch_LIN() :CBaseSignalWatchImp( LIN )
{

}

CSignalWatch_LIN::~CSignalWatch_LIN( void )
{
    m_ouReadThread.bTerminateThread(); // Terminate read thread
    m_ouLinBufFSE.vClearMessageBuffer();//clear can buffer
}

HRESULT CSignalWatch_LIN::StartSigWatchReadThread()
{
    m_ouReadThread.m_pBuffer = this;
    m_ouReadThread.m_hActionEvent = m_ouLinBufFSE.hGetNotifyingEvent();
    m_ouReadThread.bStartThread(SigWatchDataReadThreadProc_L);
    return S_OK;
}

void CSignalWatch_LIN::vDisplayInSigWatchWnd(STLINDATA& sLinData)
{
    EnterCriticalSection(&m_omCritSecSW);
    if ( mDbCluster != nullptr )
    {
        //STLINDATA * sLinData;
        static CString omMsgName;
        static CStringArray omSigNames, omRawValues, omPhyValues;
        /*if (m_pMsgInterPretObj->bInterpretMsgSigList(sLinData.m_uDataInfo.m_sLINMsg.m_ucMsgID,
        sLinData.m_uDataInfo.m_sLINMsg.m_ucData,omMsgName, omSigNames, omRawValues, omPhyValues, m_bHex))*/
        SSignalInfoArray sSingnalinfo;
        std::list<InterpreteSignals> sInterPretedSignals;
        std::string msgName;

        int mID = 0;
        auto itr = m_mapMsgIDtoSignallst->find(sLinData.m_uDataInfo.m_sLINMsg.m_ucMsgID);

        if (itr != m_mapMsgIDtoSignallst->end())
        {
            mID = itr->first;
            if (mID == sLinData.m_uDataInfo.m_sLINMsg.m_ucMsgID)
            {
                IFrame* pFrame = nullptr;
                mDbCluster->GetFrame(LIN, 0 /*channel index*/, sLinData.m_uDataInfo.m_sLINMsg.m_ucMsgID, nullptr, &pFrame);
                if (nullptr != pFrame)
                {
                    pFrame->InterpretSignals(sLinData.m_uDataInfo.m_sLINMsg.m_ucData, sLinData.m_uDataInfo.m_sLINMsg.m_ucDataLen, sInterPretedSignals, m_bHex);

                    SSignalInfoArray ouSSignalInfoArray;
                    SSignalInfo ouSSignalInfo;
                    std::string strName;
                    pFrame->GetName(strName);
                    ouSSignalInfo.m_msgName = strName.c_str();
for (auto itr : sInterPretedSignals)
                    {
                        ouSSignalInfo.m_omEnggValue = itr.m_omEnggValue.c_str();
                        ouSSignalInfo.m_omRawValue = itr.m_omRawValue.c_str();
                        ouSSignalInfo.m_omSigName = itr.m_omSigName.c_str();
                        ouSSignalInfo.m_omUnit = itr.m_omUnit.c_str();
                        ouSSignalInfoArray.Add(ouSSignalInfo);
                    }



                    if (ouSSignalInfoArray.GetSize() > 0 && m_mapMsgIDtoSignallst->size() != 0)
                    {
                        m_pouSigWnd->vAddMsgToWnd(&ouSSignalInfoArray, FALSE, m_mapMsgIDtoSignallst, mID);
                    }
                }
            }
        }

    }

    LeaveCriticalSection( &m_omCritSecSW );
}


HRESULT CSignalWatch_LIN::DoInitialization( )
{
    if ( m_pouSigWnd != nullptr )
    {
        m_pouSigWnd->SetWindowText( "Signal Watch - LIN" );
    }
    CBaseDIL_LIN* pouDIL_LIN;
    if (DIL_GetInterface(LIN, (void**)&pouDIL_LIN) == S_OK)
    {
        DWORD dwClientId = 0;
        pouDIL_LIN->DILL_RegisterClient(TRUE, dwClientId, LIN_MONITOR_NODE);
        pouDIL_LIN->DILL_ManageMsgBuf(MSGBUF_ADD, dwClientId, &(m_ouLinBufFSE));
    }
    //Start the read thread
    return StartSigWatchReadThread()? S_OK: S_FALSE;
}