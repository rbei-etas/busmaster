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
    DISP_FUNCTION_ID(CApplication, "SendCANMSg",            dispidSendCANMSg,               SendCANMSg,             VT_EMPTY, VTS_PVARIANT )
    DISP_FUNCTION_ID(CApplication, "GetErrorCounter",       dispidGetErrorCounter,          GetErrorCounter,        VT_EMPTY, VTS_UI1 VTS_UI1 VTS_I4  )
    DISP_FUNCTION_ID(CApplication, "EnableDisableHandlers", dispidEnableDisableHandlers,    EnableDisableHandlers,  VT_EMPTY, VTS_BOOL VTS_PVARIANT )
    DISP_FUNCTION_ID(CApplication, "ImportDatabase",        dispidImportDatabase,           ImportDatabase,         VT_EMPTY, VTS_BSTR )
    DISP_FUNCTION_ID(CApplication, "LoadConfiguration",     dispidLoadConfiguration,        LoadConfiguration,      VT_EMPTY, VTS_BSTR )
    DISP_FUNCTION_ID(CApplication, "SendKeyValue",          dispidSendKeyValue,             SendKeyValue,           VT_EMPTY, VTS_UI1 )
    //DISP_FUNCTION_ID(CApplication, "GetControllerMode",       dispidGetControllerMode,        GetControllerMode,      VT_EMPTY, VTS_PVARIANT)
    //DISP_FUNCTION_ID(CApplication, "SetControllerMode",       dispidSetControllerMode,        SetControllerMode,      VT_EMPTY, VTS_PVARIANT)
    //DISP_FUNCTION_ID(CApplication, "GetBaudRate",         dispidGetBaudRate,              GetBaudRate,            VT_EMPTY, VTS_I4 VTS_I4  VTS_UI1 VTS_UI1)
    //DISP_FUNCTION_ID(CApplication, "SetBaudRate",         dispidSetBaudRate,              SetBaudRate,            VT_EMPTY, VTS_I4 VTS_I4  VTS_UI1 VTS_UI1 )
    DISP_FUNCTION_ID(CApplication, "DisplayWindow",         dispidDisplayWindow,            DisplayWindow,          VT_EMPTY, VTS_I2 VTS_PVARIANT  )
    DISP_FUNCTION_ID(CApplication, "WriteToLogFile",        dispidWriteToLogFile,           WriteToLogFile,         VT_EMPTY, VTS_I2 VTS_BSTR)
    DISP_FUNCTION_ID(CApplication, "StopLogging",           dispidStopLogging,              StopLogging,            VT_EMPTY, VTS_NONE)
    DISP_FUNCTION_ID(CApplication, "StartLogging",          dispidStartLogging,             StartLogging,           VT_EMPTY, VTS_NONE)
    DISP_FUNCTION_ID(CApplication, "ResetSW",               dispidReSetSW,                  ResetSW,                VT_EMPTY, VTS_NONE)
    DISP_FUNCTION_ID(CApplication, "ResetHW",               dispidReSetHW,                  ResetHW,                VT_EMPTY, VTS_NONE)
    DISP_FUNCTION_ID(CApplication, "StopTxMsgBlock",        dispidStopTxMsgBlock,           StopTxMsgBlock,         VT_EMPTY, VTS_NONE)
    DISP_FUNCTION_ID(CApplication, "StartTxMsgBlock",       dispidStartTxMsgBlock,          StartTxMsgBlock,        VT_EMPTY, VTS_NONE)
    DISP_FUNCTION_ID(CApplication, "UnLoadAllDll",          dispidUnLoadAllDll,             UnLoadAllDll,           VT_EMPTY, VTS_NONE)
    DISP_FUNCTION_ID(CApplication, "LoadAllDll",            dispidLoadAllDll,               LoadAllDll,             VT_EMPTY, VTS_NONE)
    //DISP_FUNCTION_ID(CApplication, "BuildLoadAllDll",     dispidBuildLoadAllDll,          BuildLoadAllDll,        VT_EMPTY, VTS_NONE)
    //DISP_FUNCTION_ID(CApplication, "BuildAllDll",         dispidBuildAllDll,              BuildAllDll,            VT_EMPTY, VTS_NONE)
    DISP_FUNCTION_ID(CApplication, "SaveConfiguration",     dispidSaveConfiguration,        SaveConfiguration,      VT_EMPTY, VTS_NONE)
    //DISP_FUNCTION_ID(CApplication, "GetHwFilter",         dispidGetHwFilter,              GetHwFilter,            VT_EMPTY, VTS_BOOL VTS_R8 VTS_R8)
    //DISP_FUNCTION_ID(CApplication, "SetHwFilter",         dispidSetHwFilter,              SetHwFilter,            VT_EMPTY, VTS_BOOL  VTS_R8 VTS_R8 )
    DISP_FUNCTION_ID(CApplication, "AddLoggingBlock",       dispidAddLoggingBlock,          AddLoggingBlock,        VT_EMPTY, VTS_PVARIANT)
    DISP_FUNCTION_ID(CApplication, "SaveConfigurationAs",   dispidSaveConfigurationAs,      SaveConfigurationAs,    VT_EMPTY, VTS_BSTR)
    DISP_FUNCTION_ID(CApplication, "AddTxBlock",            dispidAddTxBlock,               AddTxBlock,             VT_EMPTY, VTS_PVARIANT)
    DISP_FUNCTION_ID(CApplication, "GetTxBlockCount",       dispidGetTxBlockCount,          GetTxBlockCount,        VT_EMPTY, VTS_PI2)
    DISP_FUNCTION_ID(CApplication, "GetTxBlock",            dispidGetTxBlock,               GetTxBlock,             VT_EMPTY, VTS_I2 VTS_PVARIANT)
    DISP_FUNCTION_ID(CApplication, "DeleteTxBlock",         dispidDeleteTxBlock,            DeleteTxBlock,          VT_EMPTY, VTS_I2)
    DISP_FUNCTION_ID(CApplication, "ClearTxBlockList",      dispidClearTxBlockList,         ClearTxBlockList,       VT_EMPTY, VTS_NONE)
    DISP_FUNCTION_ID(CApplication, "AddMsgToTxBlock",       dispidAddMsgToTxBlock,          AddMsgToTxBlock,        VT_EMPTY, VTS_I2 VTS_PVARIANT)
    DISP_FUNCTION_ID(CApplication, "GetMsgCount",           dispidGetMsgCount,              GetMsgCount,            VT_EMPTY, VTS_I2 VTS_PI2)
    DISP_FUNCTION_ID(CApplication, "GetMsgFromTxBlock",     dispidGetMsgFromTxBlock,        GetMsgFromTxBlock,      VT_EMPTY, VTS_I2 VTS_I2 VTS_PVARIANT)
    DISP_FUNCTION_ID(CApplication, "DeleteMsgFromTxBlock",  dispidDeleteMsgFromTxBlock,     DeleteMsgFromTxBlock,   VT_EMPTY, VTS_I2 VTS_PI2)
    DISP_FUNCTION_ID(CApplication, "ClearMsgList",          dispidClearMsgList,             ClearMsgList,           VT_EMPTY, VTS_I2)
    DISP_FUNCTION_ID(CApplication, "AddFilterScheme",       dispidAddFilterScheme,          AddFilterScheme,        VT_EMPTY, VTS_BSTR VTS_BOOL)
    DISP_FUNCTION_ID(CApplication, "GetFilterScheme",       dispidGetFilterScheme,          GetFilterScheme,        VT_EMPTY, VTS_I2 VTS_BSTR VTS_PBOOL)
    DISP_FUNCTION_ID(CApplication, "GetFilterSchCount",     dispidGetFilterSchCount,        GetFilterSchCount,      VT_EMPTY, VTS_PI2)
    DISP_FUNCTION_ID(CApplication, "UpdateFilterSch",       dispidUpdateFilterSch,          UpdateFilterSch,        VT_EMPTY, VTS_I2 VTS_PVARIANT)
    DISP_FUNCTION_ID(CApplication, "GetFilterCountInSch",   dispidGetFilterCountInSch,      GetFilterCountInSch,    VT_EMPTY, VTS_I2 VTS_PI2)
    DISP_FUNCTION_ID(CApplication, "GetFilterInFilterSch",  dispidGetFilterInFilterSch,     GetFilterInFilterSch,   VT_EMPTY, VTS_I2 VTS_I2 VTS_PVARIANT)
    DISP_FUNCTION_ID(CApplication, "DeleteFilterInSch",     dispidDeleteFilterInSch,        DeleteFilterInSch,      VT_EMPTY, VTS_I2 VTS_I2)
    DISP_FUNCTION_ID(CApplication, "EnableFilterSch",       dispidEnableFilterSch,          EnableFilterSch,        VT_EMPTY, VTS_PVARIANT VTS_BOOL)
    DISP_FUNCTION_ID(CApplication, "AddSimulatedSystem",    dispidAddSimulatedSystem,       AddSimulatedSystem,     VT_EMPTY, VTS_BSTR)
    DISP_FUNCTION_ID(CApplication, "GetSimulatedSystemCount",dispidGetSimulatedSystemCount, GetSimulatedSystemCount,VT_EMPTY, VTS_PI2)
    DISP_FUNCTION_ID(CApplication, "GetSimulatedSystemName",dispidGetSimulatedSystemName,   GetSimulatedSystemName, VT_EMPTY, VTS_I2 VTS_PBSTR)
    DISP_FUNCTION_ID(CApplication, "DeleteSimulatedSystem", dispidDeleteSimulatedSystem,    DeleteSimulatedSystem,  VT_EMPTY, VTS_I2)
    DISP_FUNCTION_ID(CApplication, "RemoveLoggingBlock",    dispidRemoveLoggingBlock,       RemoveLoggingBlock,     VT_EMPTY, VTS_I2)
    DISP_FUNCTION_ID(CApplication, "GetLoggingBlockCount",  dispidGetLoggingBlockCount,     GetLoggingBlockCount,   VT_EMPTY, VTS_PI2)
    DISP_FUNCTION_ID(CApplication, "ClearLoggingBlockList", dispidClearLoggingBlockList,    ClearLoggingBlockList,  VT_EMPTY, VTS_NONE)
    DISP_FUNCTION_ID(CApplication, "GetLoggingBlock",       dispidGetLoggingBlock,          GetLoggingBlock,        VT_EMPTY, VTS_I2 VTS_PVARIANT)
END_DISPATCH_MAP()

BEGIN_INTERFACE_MAP(CApplication, CCmdTarget)
INTERFACE_PART(CApplication, IID_IApplication, LocalClass)
END_INTERFACE_MAP()

IMPLEMENT_OLECREATE(CApplication, "CAN_MonitorApp.Application", 0x92d435c1, 0xa552, 0x4435,  0xad, 0x1e, 0x46, 0x8b, 0x4c, 0x17, 0xbd, 0xc7)

CApplication::CApplication(void)
{
    EnableAutomation();
    ::AfxOleLockApp();
}

CApplication::~CApplication(void)
{
    ::AfxOleUnlockApp();
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
    ASSERT(lpDispatch != NULL);
    return lpDispatch->GetTypeInfoCount(pctinfo);
}
STDMETHODIMP CApplication::XLocalClass::GetTypeInfo(
    UINT itinfo, LCID lcid, ITypeInfo FAR* FAR* pptinfo)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    LPDISPATCH lpDispatch = pThis->GetIDispatch(FALSE);
    ASSERT(lpDispatch != NULL);
    return lpDispatch->GetTypeInfo(itinfo, lcid, pptinfo);
}
STDMETHODIMP CApplication::XLocalClass::GetIDsOfNames(
    REFIID riid, OLECHAR FAR* FAR* rgszNames, UINT cNames,
    LCID lcid, DISPID FAR* rgdispid)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    LPDISPATCH lpDispatch = pThis->GetIDispatch(FALSE);
    ASSERT(lpDispatch != NULL);
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
    ASSERT(lpDispatch != NULL);
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

STDMETHODIMP CApplication::XLocalClass::SendCANMSg(CAN_MSGS* sMsg)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->SendCANMSg(sMsg);
}

STDMETHODIMP CApplication::XLocalClass::GetErrorCounter(UCHAR* Tx, UCHAR* Rx, INT nChannel)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->GetErrorCounter(Tx,Rx,nChannel);
}

STDMETHODIMP CApplication::XLocalClass::EnableDisableHandlers(BOOL bEnable,eHandlerType eType)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->EnableDisableHandlers( bEnable, eType);
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

STDMETHODIMP CApplication::XLocalClass::SendKeyValue(UCHAR keyval)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->SendKeyValue(keyval);
}

/*STDMETHODIMP CApplication::XLocalClass::GetControllerMode(eControllerMode *eMode)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->GetControllerMode(eMode);
}

STDMETHODIMP CApplication::XLocalClass::SetControllerMode(eControllerMode eMode)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->SetControllerMode(eMode);
}

STDMETHODIMP CApplication::XLocalClass::GetBaudRate(int nChannelNo, BYTE *bBTR0,BYTE *bBTR1)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->GetBaudRate(nChannelNo,bBTR0,bBTR1);
}

STDMETHODIMP CApplication::XLocalClass::SetBaudRate(int nChannelNo, BYTE bBTR0,BYTE bBTR1)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->SetBaudRate(nChannelNo,bBTR0,bBTR1);
}*/

STDMETHODIMP CApplication::XLocalClass::DisplayWindow(eWindow eWhichWindow)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->DisplayWindow(eWhichWindow);
}

STDMETHODIMP CApplication::XLocalClass::WriteToLogFile(USHORT BlockIndex, BSTR pcStr)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->WriteToLogFile(BlockIndex, pcStr);
}

STDMETHODIMP CApplication::XLocalClass::StopLogging()
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->StopLogging();
}

STDMETHODIMP CApplication::XLocalClass::StartLogging()
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->StartLogging();
}

STDMETHODIMP CApplication::XLocalClass::ResetSW()
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->ResetSW();
}

STDMETHODIMP CApplication::XLocalClass::ResetHW()
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->ResetHW();
}

STDMETHODIMP CApplication::XLocalClass::StopTxMsgBlock()
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->StopTxMsgBlock();
}

STDMETHODIMP CApplication::XLocalClass::StartTxMsgBlock()
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->StartTxMsgBlock();
}

STDMETHODIMP CApplication::XLocalClass::UnLoadAllDll()
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->UnLoadAllDll();
}

STDMETHODIMP CApplication::XLocalClass::LoadAllDll()
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->LoadAllDll();
}

/*STDMETHODIMP CApplication::XLocalClass::BuildLoadAllDll()
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->BuildLoadAllDll();
}

STDMETHODIMP CApplication::XLocalClass::BuildAllDll()
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->BuildAllDll();
}

STDMETHODIMP CApplication::XLocalClass::GetHwFilter(BOOL *pnExtended, double *dBeginMsgId, double *dEndMsgId)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->GetHwFilter(pnExtended, dBeginMsgId, dEndMsgId);
}

STDMETHODIMP CApplication::XLocalClass::SetHwFilter(BOOL pnExtended, double dBeginMsgId, double dEndMsgId)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->SetHwFilter(pnExtended, dBeginMsgId, dEndMsgId);
}*/
STDMETHODIMP CApplication::XLocalClass::SaveConfiguration()
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->SaveConfiguration();
}
STDMETHODIMP CApplication::XLocalClass::AddLoggingBlock(SLOGGINGBLOCK_USR* psLoggingBlock)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->AddLoggingBlock( psLoggingBlock );
}
STDMETHODIMP CApplication::XLocalClass::SaveConfigurationAs(BSTR ConfigPath)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->SaveConfigurationAs( ConfigPath);
}
STDMETHODIMP CApplication::XLocalClass::AddTxBlock(STXBLOCK_USR* psTxBlock)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->AddTxBlock(psTxBlock);
}
STDMETHODIMP CApplication::XLocalClass::GetTxBlockCount (USHORT* Result)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->GetTxBlockCount (Result);
}
STDMETHODIMP CApplication::XLocalClass::GetTxBlock (USHORT BlockIndex, STXBLOCK_USR* psTxBlock)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->GetTxBlock (BlockIndex, psTxBlock);
}
STDMETHODIMP CApplication::XLocalClass::DeleteTxBlock (USHORT BlockIndex)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->DeleteTxBlock (BlockIndex);
}
STDMETHODIMP CApplication::XLocalClass::ClearTxBlockList ()
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->ClearTxBlockList ();
}
STDMETHODIMP CApplication::XLocalClass::AddMsgToTxBlock (USHORT BlockIndex, CAN_MSGS* psMsg)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->AddMsgToTxBlock (BlockIndex, psMsg);
}
STDMETHODIMP CApplication::XLocalClass::GetMsgCount (USHORT BlockIndex, USHORT* Result)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->GetMsgCount (BlockIndex, Result);
}
STDMETHODIMP CApplication::XLocalClass::GetMsgFromTxBlock  ( USHORT BlockIndex, USHORT MsgIndex, CAN_MSGS* psMsg)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->GetMsgFromTxBlock  ( BlockIndex, MsgIndex, psMsg);
}
STDMETHODIMP CApplication::XLocalClass::DeleteMsgFromTxBlock (USHORT BlockIndex, USHORT FrameIndex)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->DeleteMsgFromTxBlock (BlockIndex, FrameIndex);
}
STDMETHODIMP CApplication::XLocalClass::ClearMsgList (USHORT BlockIndex)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->ClearMsgList (BlockIndex);
}
STDMETHODIMP CApplication::XLocalClass::AddFilterScheme (BSTR pcFilterName, VARIANT_BOOL  FilterType)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->AddFilterScheme (pcFilterName, FilterType);
}
STDMETHODIMP CApplication::XLocalClass::GetFilterScheme ( USHORT FilterSchINdex,BSTR pcFilterName,VARIANT_BOOL*  FilterType)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->GetFilterScheme ( FilterSchINdex, pcFilterName, FilterType);
}
STDMETHODIMP CApplication::XLocalClass::GetFilterSchCount (USHORT* pTotal)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->GetFilterSchCount (pTotal);
}
STDMETHODIMP CApplication::XLocalClass::UpdateFilterSch (USHORT FilterSchIndex,SFILTER_USR* psFilter)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->UpdateFilterSch (FilterSchIndex, psFilter);
}
STDMETHODIMP CApplication::XLocalClass::GetFilterCountInSch (USHORT FilterSchIndex, USHORT* pTotal)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->GetFilterCountInSch (FilterSchIndex, pTotal);
}
STDMETHODIMP CApplication::XLocalClass::GetFilterInFilterSch (USHORT FilterSchIndex, USHORT FilterIndex, SFILTER_USR* psFilter)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->GetFilterInFilterSch (FilterSchIndex, FilterIndex, psFilter);
}

STDMETHODIMP CApplication::XLocalClass:: DeleteFilterInSch (USHORT FilterSchIndex, USHORT FilterIndex)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis-> DeleteFilterInSch (FilterSchIndex, FilterIndex);
}
STDMETHODIMP CApplication::XLocalClass::EnableFilterSch (EFILTERMODULE eModule, BOOL Enable)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->EnableFilterSch (eModule, Enable);
}

STDMETHODIMP CApplication::XLocalClass::AddSimulatedSystem(BSTR pcSimPath)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->AddSimulatedSystem(pcSimPath);
}

STDMETHODIMP CApplication::XLocalClass::GetSimulatedSystemCount(USHORT* pSimTotal)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->GetSimulatedSystemCount(pSimTotal);
}

STDMETHODIMP CApplication::XLocalClass::GetSimulatedSystemName(USHORT SimSysIndex, BSTR* pcSimPath)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->GetSimulatedSystemName(SimSysIndex, pcSimPath);
}

STDMETHODIMP CApplication::XLocalClass::DeleteSimulatedSystem(USHORT SimSysIndex)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->DeleteSimulatedSystem(SimSysIndex);
}

STDMETHODIMP CApplication::XLocalClass::RemoveLoggingBlock(USHORT BlockIndex)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->RemoveLoggingBlock(BlockIndex);
}

STDMETHODIMP CApplication::XLocalClass::GetLoggingBlockCount(USHORT* BlockCount)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->GetLoggingBlockCount(BlockCount);
}

STDMETHODIMP CApplication::XLocalClass::ClearLoggingBlockList(void)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->ClearLoggingBlockList();
}

STDMETHODIMP CApplication::XLocalClass::GetLoggingBlock(USHORT BlockIndex, SLOGGINGBLOCK_USR* psLoggingBlock)
{
    METHOD_PROLOGUE(CApplication, LocalClass)
    return pThis->GetLoggingBlock( BlockIndex, psLoggingBlock);
}