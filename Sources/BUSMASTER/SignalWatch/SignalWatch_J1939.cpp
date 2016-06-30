/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  SignalWatch_J1939.cpp
  Description   :
  $Log:   X:/Archive/Sources/SignalWatch/SignalWatch_J1939.cpv  $

  Author(s)     :  Pradeep Kadoor
  Date Created  :  16/02/2011
  Modified By   :
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/
#include "SignalWatch_stdafx.h"
#include "Include/Utils_macro.h"
//#include "Include/DIL_CommonDefs.h"
#include "DILPluginHelperDefs.h"
#include "SignalWatch_J1939.h"
#include "include/XMLDefines.h"
#include "MsgSignalSelect.h"
#include "Error.h"
int CSignalWatch_J1939::ReadJ1939DataBuffer( CSignalWatch_J1939* pSWJ1939 )
{
    VALIDATE_POINTER_RETURN_VAL(pSWJ1939, -1);
    while (pSWJ1939->m_ouMsgBufVSE_J.GetMsgCount() > 0)
    {
        static STJ1939_MSG sMsg;
        static BYTE abyData[MAX_MSG_LEN_J1939] = {0xFF};
        INT nType = 0;
        INT nSize = MAX_MSG_LEN_J1939;
        HRESULT Result = pSWJ1939->m_ouMsgBufVSE_J.ReadFromBuffer(nType, abyData, nSize);
        if (Result == ERR_READ_MEMORY_SHORT)
        {
            TRACE("ERR_READ_MEMORY_SHORT");
        }
        else if (Result == EMPTY_APP_BUFFER)
        {
            TRACE("EMPTY_APP_BUFFER");
        }
        else if (Result == CALL_SUCCESS)
        {
            sMsg.vSetDataStream(abyData);
            pSWJ1939->vDisplayInSigWatchWnd(sMsg);
        }

    }
    return 0;
}

DWORD WINAPI CSignalWatch_J1939::SigWatchDataReadThreadProc_J( LPVOID pVoid )
{
    /*USES_CONVERSION;*/

    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;
    if (pThreadParam == nullptr)
    {
        return (DWORD)-1;
    }
    CSignalWatch_J1939* pSWJ1939 = (CSignalWatch_J1939*)pThreadParam->m_pBuffer;
    if (pSWJ1939 == nullptr)
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
                pSWJ1939->ReadJ1939DataBuffer( pSWJ1939 ); // Retrieve message from the driver
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
CSignalWatch_J1939::CSignalWatch_J1939():CBaseSignalWatchImp( J1939 )
{

}
CSignalWatch_J1939::~CSignalWatch_J1939( void )
{
    m_ouReadThread.bTerminateThread(); // Terminate read thread
    m_ouMsgBufVSE_J.vClearMessageBuffer();//clear J1939 buffer
}
HRESULT CSignalWatch_J1939::StartSigWatchReadThread()
{
    m_ouReadThread.m_pBuffer = this;
    m_ouReadThread.m_hActionEvent = m_ouMsgBufVSE_J.hGetNotifyingEvent();
    m_ouReadThread.bStartThread(SigWatchDataReadThreadProc_J);
    return S_OK;
}

void CSignalWatch_J1939::vDisplayInSigWatchWnd(STJ1939_MSG& sMsg)
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
        auto itr = m_mapMsgIDtoSignallst[channel].find( sMsg.m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN() );
        if ( itr != m_mapMsgIDtoSignallst[channel].end() )
        {
            int mID = itr->first;

            //if ( mID == sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID )
            {
                IFrame* pFrame = nullptr;
                mDbCluster->GetFrame( J1939, 0 /*channel index*/, sMsg.m_sMsgProperties.m_uExtendedID.m_s29BitId.unGetPGN(), nullptr, &pFrame );
                if ( nullptr != pFrame )
                {
                    pFrame->InterpretSignals(sMsg.m_pbyData, sMsg.m_unDLC, sInterPretedSignals, m_bHex);

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


HRESULT CSignalWatch_J1939::DoInitialization( )
{
    if ( m_pouSigWnd != nullptr )
    {
        m_pouSigWnd->SetWindowText( "Signal Watch - J1939" );
    }
    CBaseDILI_J1939* pouDIL_J1939;
    if ( DIL_GetInterface( J1939, (void**)&pouDIL_J1939 ) == S_OK )
    {
        DWORD dwClientId = 0;
        pouDIL_J1939->DILIJ_RegisterClient(TRUE, J1939_MONITOR_NODE, J1939_ECU_NAME, 0, dwClientId);
        pouDIL_J1939->DILIJ_ManageMsgBuf(MSGBUF_ADD, dwClientId, &(m_ouMsgBufVSE_J));
    }
    //Start the read thread
    return StartSigWatchReadThread()? S_OK: S_FALSE;
}