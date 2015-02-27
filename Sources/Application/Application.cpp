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
 * \file      Application.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#include "StdAfx.h"
#include "Application.h"
#include "BUSMASTER_Interface.h"

IMPLEMENT_DYNCREATE(CApplication, CCmdTarget)

BEGIN_MESSAGE_MAP(CApplication, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CApplication, CCmdTarget)
    DISP_FUNCTION_ID(CApplication, "Connect",               dispidConnect,                  Connect,                VT_EMPTY, VTS_BOOL)
    DISP_FUNCTION_ID(CApplication, "GetMsgInfo",            dispidGetMsgInfo,               GetMsgInfo,             VT_EMPTY, VTS_BSTR VTS_PVARIANT )
    DISP_FUNCTION_ID(CApplication, "GetNetworkStatistics",  dispidGetNetworkStatistics,     GetNetworkStatistics,   VT_EMPTY, VTS_I4 VTS_PVARIANT )
    DISP_FUNCTION_ID(CApplication, "GetErrorCounter",       dispidGetErrorCounter,          GetErrorCounter,        VT_EMPTY, VTS_UI1 VTS_UI1 VTS_I4  )
    DISP_FUNCTION_ID(CApplication, "ImportDatabase",        dispidImportDatabase,           ImportDatabase,         VT_EMPTY, VTS_BSTR )
    DISP_FUNCTION_ID(CApplication, "LoadConfiguration",     dispidLoadConfiguration,        LoadConfiguration,      VT_EMPTY, VTS_BSTR )
    DISP_FUNCTION_ID(CApplication, "SaveConfiguration",     dispidSaveConfiguration,        SaveConfiguration,      VT_EMPTY, VTS_NONE)
    DISP_FUNCTION_ID(CApplication, "SaveConfigurationAs",   dispidSaveConfigurationAs,      SaveConfigurationAs,    VT_EMPTY, VTS_BSTR)
    DISP_FUNCTION_ID(CApplication, "RegisterClientForRx",   dispidRegisterClientForRx,      RegisterClientForRx,    VT_EMPTY, VTS_I2 VTS_PBSTR VTS_PBSTR)
    DISP_FUNCTION_ID(CApplication, "UnRegisterClient",      dispidUnRegisterClient,         UnRegisterClient,       VT_EMPTY, VTS_I2)
    //DISP_FUNCTION_ID(CApplication, "GetApplication",      dispidGetApplication,           GetApplication,         VT_EMPTY, VTS_PVARIANT)

END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CApplication, CCmdTarget)
INTERFACE_PART(CApplication, IID_IBusMasterApp, LocalClass)
INTERFACE_PART(CApplication, IID_IConnectionPointContainer, ConnPtContainer)
END_INTERFACE_MAP()

BEGIN_CONNECTION_MAP(CApplication, CCmdTarget)
CONNECTION_PART(CApplication, IID__IAppEvents, AppEvents)
END_CONNECTION_MAP()

IMPLEMENT_OLECREATE(CApplication, "CAN_MonitorApp.Application", 0x92d435c1, 0xa552, 0x4435,  0xad, 0x1e, 0x46, 0x8b, 0x4c, 0x17, 0xbd, 0xc7)

CApplication::CApplication(void)
{
    EnableAutomation();
    ::AfxOleLockApp();
    // enable this object for connection points
    EnableConnections();


    if ( !g_bInitCOMThread )
    {
        g_ouCOMReadThread.m_hActionEvent = nullptr;
        g_ouCOMReadThread.m_unActionCode = IDLE;

        bStartCOMReadThread();
        g_bInitCOMThread = true;
    }
}

/* Read thread function for distributing Rx messages to COM clients */
DWORD WINAPI COMReadThreadProc(LPVOID pVoid)
{
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC*) pVoid;
    if (pThreadParam == nullptr)
    {
        return (DWORD)-1;
    }

    CApplication* pouApp = static_cast<CApplication*> (pThreadParam->m_pBuffer);

    if (pouApp == nullptr)
    {
        return (DWORD)-1;
    }

    pThreadParam->m_unActionCode = CREATE_TIME_MAP;
    bool bLoopON = true;

    while (bLoopON)
    {
        WaitForSingleObject(pThreadParam->m_hActionEvent, INFINITE);
        switch (pThreadParam->m_unActionCode)
        {
            case INVOKE_FUNCTION:
            {
                pouApp->ReadCOMDataBuffer(); // Retrieve message from the driver
            }
            break;
            case EXIT_THREAD:
            {
                bLoopON = false;
            }
            break;
            case CREATE_TIME_MAP:
            {
                pThreadParam->m_unActionCode = INVOKE_FUNCTION;
                pouApp->ReadCOMDataBuffer();
                SetEvent(pThreadParam->m_hActionEvent);
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

/* routine to read the data from buffer */
void CApplication::ReadCOMDataBuffer()
{
    static STCANDATA sCanData;
    BOOL bRet = FALSE;
    DWORD dwCount = 0;

    while (g_ouCanBufForCOM.GetMsgCount())
    {
        if (g_ouCanBufForCOM.ReadFromBuffer(&sCanData) == CALL_SUCCESS)
        {
            static CAN_MSGS sMsg;
            sMsg.m_bEXTENDED = sCanData.m_uDataInfo.m_sCANMsg.m_ucEXTENDED;
            sMsg.m_bRTR = sCanData.m_uDataInfo.m_sCANMsg.m_ucRTR;
            sMsg.m_ucChannel = sCanData.m_uDataInfo.m_sCANMsg.m_ucChannel;
            sMsg.m_ucDataLen = sCanData.m_uDataInfo.m_sCANMsg.m_ucDataLen;
            memcpy(sMsg.m_ucData, sCanData.m_uDataInfo.m_sCANMsg.m_ucData, sMsg.m_ucDataLen);
            sMsg.m_unMsgID = sCanData.m_uDataInfo.m_sCANMsg.m_unMsgID;

            //vSendCANMsgToClients(sMsg);

            for ( int i = 0; i < 8 ; i++ )
            {
                if ( g_shUniqueID[i]!= -1 )
                {
                    bRet = WriteFile(g_hndPIPE[i], (BYTE*)&sMsg, sizeof(sMsg), &dwCount, nullptr);
                    /* Inform clients about data availability */
                    SetEvent(g_hndEvent[i]);
                }
            }
        }
    }
}

/* Function to start Msg read thread to distribute CAN Rx messages to COM clients */
BOOL CApplication::bStartCOMReadThread()
{
    BOOL bReturn = FALSE;
    //First stop the thread if running
    bStopCOMReadThread();
    g_ouCOMReadThread.m_pBuffer = this;
    g_ouCOMReadThread.m_hActionEvent = g_ouCanBufForCOM.hGetNotifyingEvent();
    bReturn = g_ouCOMReadThread.bStartThread(COMReadThreadProc);

    return bReturn;
}

/* Function to stop msg read thread of COM */
BOOL CApplication::bStopCOMReadThread()
{
    BOOL bReturn = FALSE;
    bReturn = g_ouCOMReadThread.bTerminateThread();
    g_ouCOMReadThread.m_hActionEvent = nullptr;
    g_ouCOMReadThread.m_unActionCode = IDLE;
    return bReturn;
}

void CApplication::vInitializeCOMReadBuffer()
{
    CBaseDIL_CAN* pDIL_CAN = GetICANDIL();
    if (pDIL_CAN != nullptr)
    {
        DWORD dwClientId = 0;
        pDIL_CAN->DILC_RegisterClient(TRUE, dwClientId, CAN_MONITOR_NODE);
        if (pDIL_CAN->DILC_ManageMsgBuf(MSGBUF_ADD, dwClientId, &g_ouCanBufForCOM) != S_OK)
        {
            TRACE("The function vInitializeCOMReadBuffer() failed.");
        }
    }
}

CApplication::~CApplication(void)
{
    ::AfxOleUnlockApp();

    /* Check if no client exists */
    bool bClientExists = false;
    for ( int i = 0 ; i < 8 ; i++ )
    {
        if ( g_shUniqueID[i] != -1 )
        {
            bClientExists = true;
        }
    }

    if ( !bClientExists )
    {
        g_bInitCOMThread = false;
        bStopCOMReadThread();
    }

}

STDMETHODIMP_(ULONG) CApplication::XLocalClass::AddRef()
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->ExternalAddRef();
}
STDMETHODIMP_(ULONG) CApplication::XLocalClass::Release()
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->ExternalRelease();
}
STDMETHODIMP CApplication::XLocalClass::QueryInterface(
    REFIID iid, LPVOID* ppvObj)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->ExternalQueryInterface(&iid, ppvObj);
}
STDMETHODIMP CApplication::XLocalClass::GetTypeInfoCount(
    UINT FAR* pctinfo)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    LPDISPATCH lpDispatch = pThis->GetIDispatch(FALSE);
    ASSERT(lpDispatch != nullptr);
    return lpDispatch->GetTypeInfoCount(pctinfo);
}
STDMETHODIMP CApplication::XLocalClass::GetTypeInfo(
    UINT itinfo, LCID lcid, ITypeInfo FAR* FAR* pptinfo)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    LPDISPATCH lpDispatch = pThis->GetIDispatch(FALSE);
    ASSERT(lpDispatch != nullptr);
    return lpDispatch->GetTypeInfo(itinfo, lcid, pptinfo);
}
STDMETHODIMP CApplication::XLocalClass::GetIDsOfNames(
    REFIID riid, OLECHAR FAR* FAR* rgszNames, UINT cNames,
    LCID lcid, DISPID FAR* rgdispid)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    LPDISPATCH lpDispatch = pThis->GetIDispatch(FALSE);
    ASSERT(lpDispatch != nullptr);
    return lpDispatch->GetIDsOfNames(riid, rgszNames, cNames,
                                     lcid, rgdispid);
}
STDMETHODIMP CApplication::XLocalClass::Invoke(
    DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags,
    DISPPARAMS FAR* pdispparams, VARIANT FAR* pvarResult,
    EXCEPINFO FAR* pexcepinfo, UINT FAR* puArgErr)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    LPDISPATCH lpDispatch = pThis->GetIDispatch(FALSE);
    ASSERT(lpDispatch != nullptr);
    return lpDispatch->Invoke(dispidMember, riid, lcid,
                              wFlags, pdispparams, pvarResult,
                              pexcepinfo, puArgErr);
}

STDMETHODIMP CApplication::XLocalClass::Connect(BOOL bConnect)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->Connect(bConnect);
}

STDMETHODIMP CApplication::XLocalClass::GetMsgInfo(BSTR MsgName,sMESSAGESTRUCT*  sMsgStruct)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->GetMsgInfo(MsgName,sMsgStruct);
}

STDMETHODIMP CApplication::XLocalClass::GetNetworkStatistics(int nChannel, sBUSSTATISTICS_USR* sStat)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->GetNetworkStatistics( nChannel,  sStat);
}

STDMETHODIMP CApplication::XLocalClass::GetErrorCounter(UCHAR* Tx, UCHAR* Rx, INT nChannel)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->GetErrorCounter(Tx,Rx,nChannel);
}

STDMETHODIMP CApplication::XLocalClass::ImportDatabase(BSTR DBFilePath)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->ImportDatabase(DBFilePath);
}

STDMETHODIMP CApplication::XLocalClass::LoadConfiguration(BSTR FileName)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->LoadConfiguration(FileName);
}

STDMETHODIMP CApplication::XLocalClass::SaveConfiguration()
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->SaveConfiguration();
}

STDMETHODIMP CApplication::XLocalClass::SaveConfigurationAs(BSTR ConfigPath)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->SaveConfigurationAs( ConfigPath);
}

STDMETHODIMP CApplication::XLocalClass::RegisterClientForRx( USHORT usUniqueID, BSTR* pEventName, BSTR* pPIPEName )
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->RegisterClientForRx( usUniqueID, pEventName, pPIPEName);
}

STDMETHODIMP CApplication::XLocalClass::UnRegisterClient( USHORT usUniqueID)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->UnRegisterClient( usUniqueID);
}

IMPLEMENT_DYNCREATE(CCommClass, CCmdTarget)

BEGIN_MESSAGE_MAP(CCommClass, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CCommClass, CCmdTarget)
    DISP_FUNCTION_ID(CCommClass, "SendCANMSg", dispidSendCANMSg, SendCANMSg, VT_EMPTY, VTS_PVARIANT )
    DISP_FUNCTION_ID(CCommClass, "GetApplication",  dispidGetApplication,       GetApplication,     VT_EMPTY, VTS_PVARIANT)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CCommClass, CCmdTarget)
INTERFACE_PART(CCommClass, IID_ICANComm, CommClass)
INTERFACE_PART(CCommClass, IID_IConnectionPointContainer, ConnPtContainer)
END_INTERFACE_MAP()

BEGIN_CONNECTION_MAP(CCommClass, CCmdTarget)
CONNECTION_PART(CCommClass, IID__IAppEvents, AppEvents)
END_CONNECTION_MAP()

IMPLEMENT_OLECREATE(CCommClass, "CAN_MonitorApp.Comm", 0x1b90ca83, 0x8652, 0x41fc, 0xb2, 0x9a, 0x19, 0x1d, 0x57, 0xb4, 0x55, 0xa3)


CCommClass::CCommClass(void)
{
    EnableAutomation();
    ::AfxOleLockApp();
    // enable this object for connection points
    EnableConnections();
}

CCommClass::~CCommClass(void)
{
    ::AfxOleUnlockApp();
}

STDMETHODIMP_(ULONG) CCommClass::XCommClass::AddRef()
{
    METHOD_PROLOGUE(CCommClass, CommClass)
    return pThis->ExternalAddRef();
}
STDMETHODIMP_(ULONG) CCommClass::XCommClass::Release()
{
    METHOD_PROLOGUE(CCommClass, CommClass)
    return pThis->ExternalRelease();
}
STDMETHODIMP CCommClass::XCommClass::QueryInterface(
    REFIID iid, LPVOID* ppvObj)
{
    METHOD_PROLOGUE(CCommClass, CommClass)
    return pThis->ExternalQueryInterface(&iid, ppvObj);
}
STDMETHODIMP CCommClass::XCommClass::GetTypeInfoCount(
    UINT FAR* pctinfo)
{
    METHOD_PROLOGUE(CCommClass, CommClass)
    LPDISPATCH lpDispatch = pThis->GetIDispatch(FALSE);
    ASSERT(lpDispatch != nullptr);
    return lpDispatch->GetTypeInfoCount(pctinfo);
}
STDMETHODIMP CCommClass::XCommClass::GetTypeInfo(
    UINT itinfo, LCID lcid, ITypeInfo FAR* FAR* pptinfo)
{
    METHOD_PROLOGUE(CCommClass, CommClass)
    LPDISPATCH lpDispatch = pThis->GetIDispatch(FALSE);
    ASSERT(lpDispatch != nullptr);
    return lpDispatch->GetTypeInfo(itinfo, lcid, pptinfo);
}
STDMETHODIMP CCommClass::XCommClass::GetIDsOfNames(
    REFIID riid, OLECHAR FAR* FAR* rgszNames, UINT cNames,
    LCID lcid, DISPID FAR* rgdispid)
{
    METHOD_PROLOGUE(CCommClass, CommClass)
    LPDISPATCH lpDispatch = pThis->GetIDispatch(FALSE);
    ASSERT(lpDispatch != nullptr);
    return lpDispatch->GetIDsOfNames(riid, rgszNames, cNames,
                                     lcid, rgdispid);
}
STDMETHODIMP CCommClass::XCommClass::Invoke(
    DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags,
    DISPPARAMS FAR* pdispparams, VARIANT FAR* pvarResult,
    EXCEPINFO FAR* pexcepinfo, UINT FAR* puArgErr)
{
    METHOD_PROLOGUE(CCommClass, CommClass)
    LPDISPATCH lpDispatch = pThis->GetIDispatch(FALSE);
    ASSERT(lpDispatch != nullptr);
    return lpDispatch->Invoke(dispidMember, riid, lcid,
                              wFlags, pdispparams, pvarResult,
                              pexcepinfo, puArgErr);
}

STDMETHODIMP CCommClass::XCommClass::SendCANMSg(CAN_MSGS* sMsg)
{
    METHOD_PROLOGUE(CCommClass, CommClass)
    return pThis->SendCANMSg(sMsg);
}

STDMETHODIMP CCommClass::XCommClass::GetApplication( IBusMasterApp** pApplication)
{
    METHOD_PROLOGUE(CCommClass, CommClass)
    return pThis->GetApplication(pApplication);
}

IMPLEMENT_DYNCREATE(CLogClass, CCmdTarget)

BEGIN_MESSAGE_MAP(CLogClass, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CLogClass, CCmdTarget)
    DISP_FUNCTION_ID(CLogClass, "StopLogging",           dispidStopLogging,              StopLogging,            VT_EMPTY, VTS_NONE)
    DISP_FUNCTION_ID(CLogClass, "StartLogging",          dispidStartLogging,             StartLogging,           VT_EMPTY, VTS_NONE)
    DISP_FUNCTION_ID(CLogClass, "AddLoggingBlock",       dispidAddLoggingBlock,          AddLoggingBlock,        VT_EMPTY, VTS_PVARIANT)
    DISP_FUNCTION_ID(CLogClass, "GetLoggingBlock",       dispidGetLoggingBlock,          GetLoggingBlock,        VT_EMPTY, VTS_I2 VTS_PVARIANT)
    DISP_FUNCTION_ID(CLogClass, "RemoveLoggingBlock",    dispidRemoveLoggingBlock,       RemoveLoggingBlock,     VT_EMPTY, VTS_I2)
    DISP_FUNCTION_ID(CLogClass, "GetLoggingBlockCount",  dispidGetLoggingBlockCount,     GetLoggingBlockCount,   VT_EMPTY, VTS_PI2)
    DISP_FUNCTION_ID(CLogClass, "ClearLoggingBlockList", dispidClearLoggingBlockList,    ClearLoggingBlockList,  VT_EMPTY, VTS_NONE)
    DISP_FUNCTION_ID(CLogClass, "WriteToLogFile",        dispidWriteToLogFile,           WriteToLogFile,         VT_EMPTY, VTS_I2 VTS_BSTR)
    DISP_FUNCTION_ID(CLogClass, "GetApplication",       dispidGetApplication,            GetApplication,         VT_EMPTY, VTS_PVARIANT)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CLogClass, CCmdTarget)
INTERFACE_PART(CLogClass, IID_ICANLog, LogClass)
INTERFACE_PART(CLogClass, IID_IConnectionPointContainer, ConnPtContainer)
END_INTERFACE_MAP()

BEGIN_CONNECTION_MAP(CLogClass, CCmdTarget)
CONNECTION_PART(CLogClass, IID__IAppEvents, AppEvents)
END_CONNECTION_MAP()

IMPLEMENT_OLECREATE(CLogClass, "CAN_MonitorApp.Log", 0xbadffc69, 0x8fcb, 0x4181, 0x88, 0x0c, 0x61, 0x5d, 0x09, 0x84, 0x8c, 0xd6)


CLogClass::CLogClass(void)
{
    EnableAutomation();
    ::AfxOleLockApp();
    // enable this object for connection points
    EnableConnections();
}

CLogClass::~CLogClass(void)
{
    ::AfxOleUnlockApp();
}

STDMETHODIMP_(ULONG) CLogClass::XLogClass::AddRef()
{
    METHOD_PROLOGUE(CLogClass, LogClass)
    return pThis->ExternalAddRef();
}
STDMETHODIMP_(ULONG) CLogClass::XLogClass::Release()
{
    METHOD_PROLOGUE(CLogClass, LogClass)
    return pThis->ExternalRelease();
}
STDMETHODIMP CLogClass::XLogClass::QueryInterface(
    REFIID iid, LPVOID* ppvObj)
{
    METHOD_PROLOGUE(CLogClass, LogClass)
    return pThis->ExternalQueryInterface(&iid, ppvObj);
}
STDMETHODIMP CLogClass::XLogClass::GetTypeInfoCount(
    UINT FAR* pctinfo)
{
    METHOD_PROLOGUE(CLogClass, LogClass)
    LPDISPATCH lpDispatch = pThis->GetIDispatch(FALSE);
    ASSERT(lpDispatch != nullptr);
    return lpDispatch->GetTypeInfoCount(pctinfo);
}
STDMETHODIMP CLogClass::XLogClass::GetTypeInfo(
    UINT itinfo, LCID lcid, ITypeInfo FAR* FAR* pptinfo)
{
    METHOD_PROLOGUE(CLogClass, LogClass)
    LPDISPATCH lpDispatch = pThis->GetIDispatch(FALSE);
    ASSERT(lpDispatch != nullptr);
    return lpDispatch->GetTypeInfo(itinfo, lcid, pptinfo);
}
STDMETHODIMP CLogClass::XLogClass::GetIDsOfNames(
    REFIID riid, OLECHAR FAR* FAR* rgszNames, UINT cNames,
    LCID lcid, DISPID FAR* rgdispid)
{
    METHOD_PROLOGUE(CLogClass, LogClass)
    LPDISPATCH lpDispatch = pThis->GetIDispatch(FALSE);
    ASSERT(lpDispatch != nullptr);
    return lpDispatch->GetIDsOfNames(riid, rgszNames, cNames,
                                     lcid, rgdispid);
}
STDMETHODIMP CLogClass::XLogClass::Invoke(
    DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags,
    DISPPARAMS FAR* pdispparams, VARIANT FAR* pvarResult,
    EXCEPINFO FAR* pexcepinfo, UINT FAR* puArgErr)
{
    METHOD_PROLOGUE(CLogClass, LogClass)
    LPDISPATCH lpDispatch = pThis->GetIDispatch(FALSE);
    ASSERT(lpDispatch != nullptr);
    return lpDispatch->Invoke(dispidMember, riid, lcid,
                              wFlags, pdispparams, pvarResult,
                              pexcepinfo, puArgErr);
}

STDMETHODIMP CLogClass::XLogClass::GetLoggingBlock(USHORT BlockIndex, SLOGGINGBLOCK_USR* psLoggingBlock)
{
    METHOD_PROLOGUE(CLogClass, LogClass)
    return pThis->GetLoggingBlock( BlockIndex, psLoggingBlock);
}

STDMETHODIMP CLogClass::XLogClass::WriteToLogFile(USHORT BlockIndex, BSTR pcStr)
{
    METHOD_PROLOGUE(CLogClass, LogClass)
    return pThis->WriteToLogFile(BlockIndex, pcStr);
}

STDMETHODIMP CLogClass::XLogClass::StopLogging()
{
    METHOD_PROLOGUE(CLogClass, LogClass)
    return pThis->StopLogging();
}

STDMETHODIMP CLogClass::XLogClass::StartLogging()
{
    METHOD_PROLOGUE(CLogClass, LogClass)
    return pThis->StartLogging();
}

STDMETHODIMP CLogClass::XLogClass::AddLoggingBlock(SLOGGINGBLOCK_USR* psLoggingBlock)
{
    METHOD_PROLOGUE(CLogClass, LogClass)
    return pThis->AddLoggingBlock( psLoggingBlock );
}

STDMETHODIMP CLogClass::XLogClass::RemoveLoggingBlock(USHORT BlockIndex)
{
    METHOD_PROLOGUE(CLogClass, LogClass)
    return pThis->RemoveLoggingBlock(BlockIndex);
}

STDMETHODIMP CLogClass::XLogClass::GetLoggingBlockCount(USHORT* BlockCount)
{
    METHOD_PROLOGUE(CLogClass, LogClass)
    return pThis->GetLoggingBlockCount(BlockCount);
}

STDMETHODIMP CLogClass::XLogClass::ClearLoggingBlockList(void)
{
    METHOD_PROLOGUE(CLogClass, LogClass)
    return pThis->ClearLoggingBlockList();
}

STDMETHODIMP CLogClass::XLogClass::GetApplication( IBusMasterApp** pApplication)
{
    METHOD_PROLOGUE(CLogClass, LogClass)
    IBusMasterApp* pApp = NULL;
    pThis->GetApplication(&pApp);
    int i = 10;
    return S_OK;
}