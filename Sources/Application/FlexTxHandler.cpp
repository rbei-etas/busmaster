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
 * \file      TxHandler.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "stdafx.h"
#include "FlexTxHandler.h"

//Function Pointer Declarations
typedef HRESULT (*SETMSGPTRINDETAILSVIEW)(void* pMsgDB);
typedef HRESULT (*SHOWCONFIGUREMSGWINDOW)(void* pParentWnd, ETYPE_BUS eBUS);
typedef HRESULT (*SETCLIENTID)(ETYPE_BUS eBusType, DWORD dwClientID);
typedef HRESULT (*SETDILINTERFACEPTR)(ETYPE_BUS eBusType, void*);
typedef HRESULT (*POSTMESSAGETOTXWND)(UINT msg, WPARAM wParam, LPARAM lParam);
typedef HRESULT (*CONFIGWINDOWSHOWN)(ETYPE_BUS);
typedef HRESULT (*STARTTRANSMISSION)(UCHAR ucKeyVal);
typedef HRESULT (*ALLOCMEMFORGLOBALTXLIST)();
typedef HRESULT (*ASSIGNMSGBLOCKLIST)();
typedef HRESULT (*DELETETXBLOCKMEMORY)();
typedef HRESULT (*STOPTRANSMISSION)(UINT unMaxWaitTime);
typedef HRESULT (*GETTXWNDCONFIGDATA)(BYTE*& pDesBuffer, int& nBuffSize);
typedef HRESULT (*GETTXWNDCONFIGDATAXML)( ETYPE_BUS eBusType, xmlNodePtr pxmlNodePtr);
typedef HRESULT (*SETTXWNDCONFIGDATA)(BYTE* pSrcBuffer, int nBuffSize);
typedef HRESULT (*SETTXWNDCONFIGDATAXML)( ETYPE_BUS eBusType, xmlDocPtr pDoc);
//typedef HRESULT (*ISTXWNDCONFIGCHANGED)();
typedef UINT    (*GETTXBLOCKCOUNT)(void);
typedef HRESULT (*SETTXSTOPFLAG)(BOOL bStartStop);
typedef HRESULT (*GETTXSTOPFLAG)();
typedef void (*FLEXFILECHANGED)();
typedef HRESULT (*PFUPDATEFIBEXCONFIG)(ETYPE_BUS eBusType, ClusterConfig& ouFlexConfig);
typedef HRESULT (*BUSSTATUSCHANGED)(ETYPE_BUS eBusType,  ESTATUS_BUS eBusStatus);
typedef HRESULT (*PFSETCHANNELCONFIG)(ETYPE_BUS eBus, ClusterConfig* pMsgData);


SETMSGPTRINDETAILSVIEW      pfFlexSetMsgDBPtrInDetailsView;
SHOWCONFIGUREMSGWINDOW      pfFlexShowConfigureMsgWindow;
SETCLIENTID                 pfFlexSetClientId;
SETDILINTERFACEPTR          pfFlexSetDILInterfacePtr;
POSTMESSAGETOTXWND          pfFlexPostMessageToTxWnd;
CONFIGWINDOWSHOWN           pfFlexConfigWindowShown;
STARTTRANSMISSION           pfFlexStartTransmission;
ALLOCMEMFORGLOBALTXLIST     pfFlexAllocateMemoryForGlobalTxList;
ASSIGNMSGBLOCKLIST          pfFlexAssignMsgBlockList;
DELETETXBLOCKMEMORY         pfFlexDeleteTxBlockMemory;
STOPTRANSMISSION            pfFlexStopTransmission;
BUSSTATUSCHANGED            pfBusStatusChanged;
GETTXWNDCONFIGDATAXML       pfFlexGetTxWndConfigData;
SETTXWNDCONFIGDATA          pfFlexSetTxWndConfigData;
SETTXWNDCONFIGDATAXML       pfFlexSetTxWndConfigDataXML;
//ISTXWNDCONFIGCHANGED      pfIsTxWndConfigChanged;
GETTXBLOCKCOUNT             pfFlexGetTxBlockCount;
SETTXSTOPFLAG               pfFlexSetTxStopFlag;
GETTXSTOPFLAG               pfFlexGetTxStopFlag;
FLEXFILECHANGED             pfFlexFileChanged;
PFUPDATEFIBEXCONFIG         pfUpdateFibexConfig;
PFSETCHANNELCONFIG          pfSetChannelConfig;

CFlexTxHandler::CFlexTxHandler(void)
{
    m_hTxHandle = nullptr;
}

CFlexTxHandler::~CFlexTxHandler(void)
{
    if ( m_hTxHandle != nullptr )
    {
        FreeLibrary(m_hTxHandle);
    }
}

/*******************************************************************************
  Function Name  : vInitializeFuncPtrs
  Input(s)       : -
  Output         : -
  Functionality  : Load the TxWindow DLL
  Member of      : CFlexTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CFlexTxHandler::vLoadTx_DLL()
{
    if ( m_hTxHandle != nullptr )
    {
        FreeLibrary(m_hTxHandle);
        m_hTxHandle = nullptr;
    }
    m_hTxHandle = LoadLibrary("TXWindowFlexRay.dll");
    vloadFuncPtrAddress();
}

/*******************************************************************************
  Function Name  : vInitializeFuncPtrs
  Input(s)       : -
  Output         : -
  Functionality  : Initialize Function Ptrs to null
  Member of      : CFlexTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CFlexTxHandler::vInitializeFuncPtrs()
{
    pfFlexSetMsgDBPtrInDetailsView      = nullptr;
    pfFlexShowConfigureMsgWindow        = nullptr;
    pfFlexSetClientId                   = nullptr;
    pfFlexSetDILInterfacePtr            = nullptr;
    pfFlexPostMessageToTxWnd            = nullptr;
    pfFlexConfigWindowShown             = nullptr;
    pfFlexStartTransmission             = nullptr;
    pfFlexAllocateMemoryForGlobalTxList = nullptr;
    pfFlexAssignMsgBlockList            = nullptr;
    pfFlexDeleteTxBlockMemory           = nullptr;
    pfFlexStopTransmission              = nullptr;
    pfFlexGetTxWndConfigData            = nullptr;
    pfFlexSetTxWndConfigData            = nullptr;
    //pfIsTxWndConfigChanged            = nullptr;
    pfFlexSetTxStopFlag                 = nullptr;
    pfFlexGetTxStopFlag                 = nullptr;
    pfFlexFileChanged                   = nullptr;
    pfUpdateFibexConfig                 = nullptr;
    pfBusStatusChanged                  = nullptr;
    pfSetChannelConfig                  = nullptr;
}

/*******************************************************************************
  Function Name  : vloadFuncPtrAddress
  Input(s)       : -
  Output         : -
  Functionality  : load lib and get Function pointers
  Member of      : CFlexTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CFlexTxHandler::vloadFuncPtrAddress()
{
    vInitializeFuncPtrs();
    pfFlexSetMsgDBPtrInDetailsView          = (SETMSGPTRINDETAILSVIEW)GetProcAddress(m_hTxHandle, "TXFlexRay_vSetMsgDBPtrInDetailsView");
    pfFlexShowConfigureMsgWindow            = (SHOWCONFIGUREMSGWINDOW)GetProcAddress(m_hTxHandle, "TXFlexRay_vShowConfigureMsgWindow");         //mess wnd creation
    pfFlexSetClientId                       = (SETCLIENTID)GetProcAddress(m_hTxHandle, "TXComman_vSetClientID");
    pfFlexSetDILInterfacePtr                = (SETDILINTERFACEPTR)GetProcAddress(m_hTxHandle, "TXComman_vSetDILInterfacePtr");
    pfFlexPostMessageToTxWnd                = (POSTMESSAGETOTXWND)GetProcAddress(m_hTxHandle, "TXComman_vPostMessageToTxWnd");
    pfFlexConfigWindowShown                 = (CONFIGWINDOWSHOWN)GetProcAddress(m_hTxHandle, "TXComman_hConfigWindowShown");
    pfFlexStartTransmission                 = (STARTTRANSMISSION)GetProcAddress(m_hTxHandle, "TXFlexRay_vStartTransmission");
    pfFlexAllocateMemoryForGlobalTxList     = (ALLOCMEMFORGLOBALTXLIST)GetProcAddress(m_hTxHandle, "TXFlexRay_bAllocateMemoryForGlobalTxList");
    pfFlexAssignMsgBlockList                = (ASSIGNMSGBLOCKLIST)GetProcAddress(m_hTxHandle, "TXFlexRay_vAssignMsgBlockList");
    pfFlexDeleteTxBlockMemory               = (DELETETXBLOCKMEMORY)GetProcAddress(m_hTxHandle, "TXFlexRay_vDeleteTxBlockMemory");
    pfFlexStopTransmission                  = (STOPTRANSMISSION)GetProcAddress(m_hTxHandle, "TXFlexRay_vBusStatusChanged");
    pfFlexGetTxWndConfigData                = (GETTXWNDCONFIGDATAXML)GetProcAddress(m_hTxHandle, "TXComman_vGetTxWndConfigData");
    pfFlexSetTxWndConfigData                = (SETTXWNDCONFIGDATA)GetProcAddress(m_hTxHandle, "TXFlexRay_vSetTxWndConfigData");
    pfFlexSetTxWndConfigDataXML             = (SETTXWNDCONFIGDATAXML)GetProcAddress(m_hTxHandle, "TXComman_vSetTxWndConfigDataXML");
    //pfIsTxWndConfigChanged                = (ISTXWNDCONFIGCHANGED)GetProcAddress(m_hTxHandle, "TXFlexRay_bIsTxWndConfigChanged");
    pfFlexSetTxStopFlag                     = (SETTXSTOPFLAG)GetProcAddress(m_hTxHandle, "TXFlexRay_vSetTxStopFlag");
    pfFlexGetTxStopFlag                     = (GETTXSTOPFLAG)GetProcAddress(m_hTxHandle, "TXFlexRay_bGetTxStopFlag");
    pfFlexGetTxBlockCount                   = (GETTXBLOCKCOUNT)GetProcAddress(m_hTxHandle, "TXFlexRay_unGetTxBlockCount");
    pfFlexFileChanged                       = (FLEXFILECHANGED)GetProcAddress(m_hTxHandle, "TXFlexRay_vFlexFileChanged");
    pfUpdateFibexConfig                     = (PFUPDATEFIBEXCONFIG)GetProcAddress(m_hTxHandle, "TXComman_nSetFibexConfig");
    pfBusStatusChanged                      = (BUSSTATUSCHANGED)GetProcAddress(m_hTxHandle, "TX_vBusStatusChanged");
    pfSetChannelConfig                      = (PFSETCHANNELCONFIG)GetProcAddress(m_hTxHandle, "TX_vSetChannelConfig");
}


void CFlexTxHandler::vBusStatusChanged(ETYPE_BUS eBusType, ESTATUS_BUS eBusStatus)
{
    if ( pfBusStatusChanged != nullptr )
    {
        pfBusStatusChanged(eBusType, eBusStatus);
    }
}



/*******************************************************************************
  Function Name  : vSetMsgDBPtrInDetailsView
  Input(s)       : pMsgDB
  Output         : -
  Functionality  : To set CMsgSignal pointer in TxWindow
  Member of      : CFlexTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 04.08.2010
  Modifications  :
*******************************************************************************/
void CFlexTxHandler::vSetMsgDBPtrInDetailsView(void* pMsgDB)
{
    if(pfFlexSetMsgDBPtrInDetailsView != nullptr)
    {
        pfFlexSetMsgDBPtrInDetailsView(pMsgDB);
    }
}

/*******************************************************************************
  Function Name  : vShowConfigureMsgWindow
  Input(s)       : -
  Output         : -
  Functionality  : To Show the Configure Msg Window Dialog
  Member of      : CFlexTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 02.08.2010
  Modifications  :
*******************************************************************************/
void CFlexTxHandler::vShowConfigureMsgWindow(void* pParentWnd, ETYPE_BUS eBUS)
{
    if(pfFlexShowConfigureMsgWindow != nullptr)
    {
        pfFlexShowConfigureMsgWindow(pParentWnd, eBUS);
    }
}

/*******************************************************************************
  Function Name  : vSetClientID
  Input(s)       : dwClientID
  Output         : -
  Functionality  : To update client ID to TxWindow Dll.
  Member of      : CFlexTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 03.08.2010
  Modifications  :
*******************************************************************************/
void CFlexTxHandler::vSetClientID(ETYPE_BUS eBusType,  DWORD dwClientID)
{
    if(pfFlexSetClientId != nullptr)
    {
        pfFlexSetClientId(eBusType, dwClientID);
    }
}

/*******************************************************************************
  Function Name  : vSetDILInterfacePtr
  Input(s)       : DILInterfacePtr (void*)
  Output         : -
  Functionality  : To update DIL interface Pointer to TxWindow Dll.
  Member of      : CFlexTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 03.08.2010
  Modifications  :
*******************************************************************************/
void CFlexTxHandler::vSetDILInterfacePtr(ETYPE_BUS eBusType, void* pDilInterface)
{
    if(pfFlexSetDILInterfacePtr != nullptr)
    {
        pfFlexSetDILInterfacePtr(eBusType, pDilInterface );
    }
}

/*******************************************************************************
  Function Name  : vFlexFileChanged
  Input(s)       : -
  Output         : -
  Functionality  : Intimates the FlexTxWindow about the change of ClusterConfig File
  Member of      : CFlexTxHandler
  Author(s)      : Ashwin R Uchil
  Date Created   : 14.06.2013
  Modifications  :
*******************************************************************************/
void CFlexTxHandler::vFlexFileChanged()
{
    if(pfFlexFileChanged != nullptr)
    {
        pfFlexFileChanged();
    }
}

/*******************************************************************************
  Function Name  : vPostMessageToTxWnd
  Input(s)       : UINT msg, WPARAM wParam, LPARAM lParam
  Output         : -
  Functionality  : Posts the Message to TxWindow Dll.
  Member of      : CFlexTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 03.08.2010
  Modifications  :
*******************************************************************************/
void CFlexTxHandler::vPostMessageToTxWnd(UINT msg, WPARAM wParam, LPARAM lParam)
{
    if(pfFlexPostMessageToTxWnd != nullptr)
    {
        pfFlexPostMessageToTxWnd(msg, wParam, lParam);
    }
}

/*******************************************************************************
  Function Name  : hConfigWindowShown
  Input(s)       : -
  Output         : HRESULT
  Functionality  : Gets the TxConfiguration Window availability status.
  Member of      : CFlexTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 04.08.2010
  Modifications  :
*******************************************************************************/
HRESULT CFlexTxHandler::hConfigWindowShown(ETYPE_BUS eBusType)
{
    HRESULT hResult = S_FALSE;

    if(pfFlexConfigWindowShown != nullptr)
    {
        hResult = pfFlexConfigWindowShown(eBusType);
    }

    return hResult;
}

/*******************************************************************************
  Function Name  : vStartTransmission
  Input(s)       : ucKeyVal
  Output         : -
  Functionality  : Start transmission
  Member of      : CFlexTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CFlexTxHandler::vStartTransmission(UCHAR ucKeyVal)
{
    if(pfFlexStartTransmission != nullptr)
    {
        pfFlexStartTransmission(ucKeyVal);
    }
}

/*******************************************************************************
  Function Name  : hAllocateMemoryForGlobalTxList
  Input(s)       : -
  Output         : HRESULT
  Functionality  : Allocates memory for Global Tx List
  Member of      : CFlexTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
HRESULT CFlexTxHandler::hAllocateMemoryForGlobalTxList()
{
    HRESULT hResult = S_OK;

    if(pfFlexAllocateMemoryForGlobalTxList != nullptr)
    {
        hResult = pfFlexAllocateMemoryForGlobalTxList();
    }

    return hResult;
}

/*******************************************************************************
  Function Name  : vAssignMsgBlockList
  Input(s)       : -
  Output         : -
  Functionality  : Assigns Message Block List
  Member of      : CFlexTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CFlexTxHandler::vAssignMsgBlockList()
{
    if(pfFlexAssignMsgBlockList != nullptr)
    {
        pfFlexAssignMsgBlockList();
    }
}

/*******************************************************************************
  Function Name  : vDeleteTxBlockMemory
  Input(s)       : -
  Output         : -
  Functionality  : Deletes Tx Block Memory
  Member of      : CFlexTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CFlexTxHandler::vDeleteTxBlockMemory()
{
    if(pfFlexDeleteTxBlockMemory != nullptr)
    {
        pfFlexDeleteTxBlockMemory();
    }
}

/*******************************************************************************
  Function Name  : vStopTransmission
  Input(s)       : unMaxWaitTime
  Output         : -
  Functionality  : Stops the Transmission of Messages.
  Member of      : CFlexTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CFlexTxHandler::vStopTransmission(UINT unMaxWaitTime)
{
    if(pfFlexStopTransmission != nullptr)
    {
        pfFlexStopTransmission(unMaxWaitTime);
    }
}
/*******************************************************************************
  Function Name  : vGetTxWndConfigData
  Input(s)       : xmlNodePtr
  Output         : -
  Functionality  : Gets the configuration data of Tx Window in XML format.
  Member of      : CFlexTxHandler
  Author(s)      : Ashwin R Uchil
  Date Created   : 2-8-2012
  Modifications  :
*******************************************************************************/
void CFlexTxHandler::vGetTxWndConfigData(ETYPE_BUS eBusType, xmlNodePtr pxmlNodePtr)
{
    if(pfFlexGetTxWndConfigData != nullptr)
    {
        pfFlexGetTxWndConfigData(eBusType, pxmlNodePtr);
    }
}

/*******************************************************************************
  Function Name  : vGetTxWndConfigData
  Input(s)       : pDesBuffer, nBuffSize
  Output         : -
  Functionality  : Gets the configuration data of Tx Window.
  Member of      : CFlexTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CFlexTxHandler::vGetTxWndConfigData(BYTE * & /* pDesBuffer */, int & /* nBuffSize */)
{
}

/*******************************************************************************
  Function Name  : vSetTxWndConfigData
  Input(s)       : pSrcBuffer, nBuffSize
  Output         : -
  Functionality  : Sets the configuration data of Tx Window.
  Member of      : CFlexTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CFlexTxHandler::vSetTxWndConfigData(BYTE* pSrcBuffer, int nBuffSize)
{
    if(pfFlexSetTxWndConfigData != nullptr)
    {
        pfFlexSetTxWndConfigData(pSrcBuffer,nBuffSize);
    }
}
void CFlexTxHandler::vSetTxWndConfigData( ETYPE_BUS eBusType, xmlDocPtr pDoc)
{
    if(pfFlexSetTxWndConfigDataXML != nullptr)
    {
        pfFlexSetTxWndConfigDataXML(eBusType, pDoc);
    }
}

/*******************************************************************************
  Function Name  : hIsTxWndConfigChanged
  Input(s)       : -
  Output         : HRESULT
  Functionality  : Gets the Configuration Changed status.
  Member of      : CFlexTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
HRESULT CFlexTxHandler::hIsTxWndConfigChanged()
{
    HRESULT hResult = S_OK;
    //if(pfIsTxWndConfigChanged != nullptr)
    //  hResult = pfIsTxWndConfigChanged();
    return hResult;
}

UINT CFlexTxHandler::unGetTxBlockCount(void)
{
    UINT Result = 0;

    if (nullptr != pfFlexGetTxBlockCount)
    {
        Result = pfFlexGetTxBlockCount();
    }
    return Result;
}

/*******************************************************************************
  Function Name  : vSetTxStopFlag
  Input(s)       : bStartStop
  Output         : -
  Functionality  : Sets Stop Flag to 'bStartStop'.
  Member of      : CFlexTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CFlexTxHandler::vSetTxStopFlag(BOOL bStartStop)
{
    if (nullptr != pfFlexSetTxStopFlag)
    {
        pfFlexSetTxStopFlag(bStartStop);
    }
}

/*******************************************************************************
  Function Name  : bGetTxStopFlag
  Input(s)       : -
  Output         : HRESULT
  Functionality  : Gets the StopFlag status.
  Member of      : CFlexTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
BOOL CFlexTxHandler::bGetTxStopFlag(void)
{
    BOOL bResult = TRUE;

    if (nullptr != pfFlexGetTxStopFlag)
    {
        bResult = pfFlexGetTxStopFlag();
    }

    return bResult;
}


HRESULT CFlexTxHandler::SetFibexConfig(ETYPE_BUS eBusType, ClusterConfig& ouFlexConfig)
{
    if ( nullptr != pfUpdateFibexConfig )
    {
        return pfUpdateFibexConfig(eBusType, ouFlexConfig);
    }
    return S_FALSE;
}


HRESULT CFlexTxHandler::SetChannelConfig(ETYPE_BUS eBus, ClusterConfig* ouFlexConfig)
{
    if ( nullptr != pfUpdateFibexConfig )
    {
        return pfSetChannelConfig(eBus, ouFlexConfig);
    }
    return S_FALSE;
}

