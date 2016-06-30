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
 * \file      SignalWatch_CAN.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "SignalWatch_stdafx.h"
#include "Include/Utils_macro.h"
#include "SignalWatch_CAN.h"
#include "Utility/MsgInterpretation.h"
#include "include/XMLDefines.h"
#include "Utility/XMLUtils.h"
#include "MsgSignalSelect.h"
#include "BaseDIL_CAN.h"
int CSignalWatch_CAN::ReadCANDataBuffer( CSignalWatch_CAN* pSWCan )
{
    ASSERT(pSWCan != nullptr);
    while (pSWCan->m_ouCanBufFSE.GetMsgCount() > 0)
    {
        static STCANDATA sCanData;
        INT Result = pSWCan->m_ouCanBufFSE.ReadFromBuffer(&sCanData);
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
            pSWCan->vDisplayInSigWatchWnd(sCanData);
        }

    }
    return 0;
}

DWORD WINAPI CSignalWatch_CAN::SigWatchDataReadThreadProc_C( LPVOID pVoid )
{
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;
    if (pThreadParam == nullptr)
    {
        return (DWORD)-1;
    }
    CSignalWatch_CAN* pSWCan = (CSignalWatch_CAN*)pThreadParam->m_pBuffer;
    if (pSWCan == nullptr)
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
                pSWCan->ReadCANDataBuffer( pSWCan ); // Retrieve message from the driver
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

CSignalWatch_CAN::CSignalWatch_CAN() :CBaseSignalWatchImp(CAN)
{

}
CSignalWatch_CAN::~CSignalWatch_CAN( void )
{
    m_ouReadThread.bTerminateThread(); // Terminate read thread
    m_ouCanBufFSE.vClearMessageBuffer();//clear can buffer
}

HRESULT CSignalWatch_CAN::StartSigWatchReadThread()
{
    m_ouReadThread.m_hActionEvent = m_ouCanBufFSE.hGetNotifyingEvent();
    m_ouReadThread.m_pBuffer = this;
    m_ouReadThread.bStartThread(SigWatchDataReadThreadProc_C);
    return S_OK;
}

void CSignalWatch_CAN::vDisplayInSigWatchWnd(STCANDATA& sCanData)
{
    EnterCriticalSection( &m_omCritSecSW );
    if ( mDbCluster != nullptr )
    {
        //STLINDATA * sLinData;
        static CString omMsgName;
        static CStringArray omSigNames, omRawValues, omPhyValues;

        SSignalInfoArray sSingnalinfo;
        std::list<InterpreteSignals> sInterPretedSignals;
        std::string msgName;

        int channel = 0;/*sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel;*/
        auto itr = m_mapMsgIDtoSignallst[channel].find( sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID );
        if ( itr != m_mapMsgIDtoSignallst[channel].end() )
        {
            int mID = itr->first;

            //if ( mID == sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID )
            {
                IFrame* pFrame = nullptr;
                mDbCluster->GetFrame( CAN, 0 /*channel index*/, sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID, nullptr, &pFrame );
                if ( nullptr != pFrame )
                {
                    pFrame->InterpretSignals( sCanData.m_uDataInfo.m_sCANMsg.m_ucData, sCanData.m_uDataInfo.m_sCANMsg.m_ucDataLen, sInterPretedSignals, m_bHex );

                    SSignalInfoArray ouSSignalInfoArray;
                    SSignalInfo ouSSignalInfo;
                    std::string strName;
                    pFrame->GetName( strName );
                    ouSSignalInfo.m_msgName = strName.c_str();
for ( auto itr : sInterPretedSignals )
                    {
                        ouSSignalInfo.m_omEnggValue = itr.m_omEnggValue.c_str();
                        ouSSignalInfo.m_omRawValue = itr.m_omRawValue.c_str();
                        ouSSignalInfo.m_omSigName = itr.m_omSigName.c_str();
                        ouSSignalInfo.m_omUnit = itr.m_omUnit.c_str();
                        ouSSignalInfoArray.Add( ouSSignalInfo );
                    }



                    if ( ouSSignalInfoArray.GetSize() > 0 && m_mapMsgIDtoSignallst->size() != 0 )
                    {
                        m_pouSigWnd->vAddMsgToWnd( &ouSSignalInfoArray, FALSE, m_mapMsgIDtoSignallst, mID );
                    }
                }
            }
        }
    }

    LeaveCriticalSection( &m_omCritSecSW );
}


HRESULT CSignalWatch_CAN::DoInitialization()
{
    if (m_pouSigWnd != nullptr)
    {
        m_pouSigWnd->SetWindowText("Signal Watch - CAN");
    }

    CBaseDIL_CAN* pouDIL_CAN;
    if ( DIL_GetInterface( CAN, (void**)&pouDIL_CAN ) == S_OK )
    {
        DWORD dwClientId = 0;
        pouDIL_CAN->DILC_RegisterClient(TRUE, dwClientId, CAN_MONITOR_NODE);
        pouDIL_CAN->DILC_ManageMsgBuf(MSGBUF_ADD, dwClientId, &(m_ouCanBufFSE));
    }
    //Start the read thread
    return StartSigWatchReadThread()? S_OK: S_FALSE;
}

