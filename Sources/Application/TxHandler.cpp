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
#include ".\txhandler.h"

//Function Pointer Declarations
typedef HRESULT (*SETMSGPTRINDETAILSVIEW)(void* pMsgDB);
typedef HRESULT (*SHOWCONFIGUREMSGWINDOW)(void* pParentWnd);
typedef HRESULT (*SETCLIENTID)(DWORD dwClientID);
typedef HRESULT (*SETDILINTERFACEPTR)(void* ptrDILIntrf);
typedef HRESULT (*POSTMESSAGETOTXWND)(UINT msg, WPARAM wParam, LPARAM lParam);
typedef HRESULT (*CONFIGWINDOWSHOWN)();
typedef HRESULT (*STARTTRANSMISSION)(UCHAR ucKeyVal);
typedef HRESULT (*ALLOCMEMFORGLOBALTXLIST)();
typedef HRESULT (*ASSIGNMSGBLOCKLIST)();
typedef HRESULT (*DELETETXBLOCKMEMORY)();
typedef HRESULT (*STOPTRANSMISSION)(UINT unMaxWaitTime);
typedef HRESULT (*GETTXWNDCONFIGDATA)(BYTE*& pDesBuffer, int& nBuffSize);
typedef HRESULT (*GETTXWNDCONFIGDATAXML)(xmlNodePtr pxmlNodePtr);
typedef HRESULT (*SETTXWNDCONFIGDATA)(BYTE* pSrcBuffer, int nBuffSize);
typedef HRESULT (*SETTXWNDCONFIGDATAXML)(xmlDocPtr pDoc);
//typedef HRESULT (*ISTXWNDCONFIGCHANGED)();
typedef UINT    (*GETTXBLOCKCOUNT)(void);
typedef HRESULT (*SETTXSTOPFLAG)(BOOL bStartStop);
typedef BOOL (*GETTXSTOPFLAG)();

SETMSGPTRINDETAILSVIEW      pfSetMsgDBPtrInDetailsView;
SHOWCONFIGUREMSGWINDOW      pfShowConfigureMsgWindow;
SETCLIENTID                 pfSetClientId;
SETDILINTERFACEPTR          pfSetDILInterfacePtr;
POSTMESSAGETOTXWND          pfPostMessageToTxWnd;
CONFIGWINDOWSHOWN           pfConfigWindowShown;
STARTTRANSMISSION           pfStartTransmission;
ALLOCMEMFORGLOBALTXLIST     pfAllocateMemoryForGlobalTxList;
ASSIGNMSGBLOCKLIST          pfAssignMsgBlockList;
DELETETXBLOCKMEMORY         pfDeleteTxBlockMemory;
STOPTRANSMISSION            pfStopTransmission;
GETTXWNDCONFIGDATAXML       pfGetTxWndConfigData;
SETTXWNDCONFIGDATA          pfSetTxWndConfigData;
SETTXWNDCONFIGDATAXML       pfSetTxWndConfigDataXML;
//ISTXWNDCONFIGCHANGED      pfIsTxWndConfigChanged;
GETTXBLOCKCOUNT             pfGetTxBlockCount;
SETTXSTOPFLAG               pfSetTxStopFlag;
GETTXSTOPFLAG               pfGetTxStopFlag;

CTxHandler::CTxHandler(void)
{
    m_hTxHandle = nullptr;
}

CTxHandler::~CTxHandler(void)
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
  Member of      : CTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CTxHandler::vLoadTx_DLL()
{
    if ( m_hTxHandle != nullptr )
    {
        FreeLibrary(m_hTxHandle);
        m_hTxHandle = nullptr;
    }
    m_hTxHandle = LoadLibrary("TXWindow.dll");
    vloadFuncPtrAddress();
}

/*******************************************************************************
  Function Name  : vInitializeFuncPtrs
  Input(s)       : -
  Output         : -
  Functionality  : Initialize Function Ptrs to null
  Member of      : CTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CTxHandler::vInitializeFuncPtrs()
{
    pfSetMsgDBPtrInDetailsView      = nullptr;
    pfShowConfigureMsgWindow        = nullptr;
    pfSetClientId                   = nullptr;
    pfSetDILInterfacePtr            = nullptr;
    pfPostMessageToTxWnd            = nullptr;
    pfConfigWindowShown             = nullptr;
    pfStartTransmission             = nullptr;
    pfAllocateMemoryForGlobalTxList = nullptr;
    pfAssignMsgBlockList            = nullptr;
    pfDeleteTxBlockMemory           = nullptr;
    pfStopTransmission              = nullptr;
    pfGetTxWndConfigData            = nullptr;
    pfSetTxWndConfigData            = nullptr;
    //pfIsTxWndConfigChanged            = nullptr;
    pfSetTxStopFlag                 = nullptr;
    pfGetTxStopFlag                 = nullptr;
}

/*******************************************************************************
  Function Name  : vloadFuncPtrAddress
  Input(s)       : -
  Output         : -
  Functionality  : load lib and get Function pointers
  Member of      : CTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CTxHandler::vloadFuncPtrAddress()
{
    vInitializeFuncPtrs();
    pfSetMsgDBPtrInDetailsView          = (SETMSGPTRINDETAILSVIEW)GetProcAddress(m_hTxHandle, "TX_vSetMsgDBPtrInDetailsView");
    pfShowConfigureMsgWindow            = (SHOWCONFIGUREMSGWINDOW)GetProcAddress(m_hTxHandle, "TX_vShowConfigureMsgWindow");
    pfSetClientId                       = (SETCLIENTID)GetProcAddress(m_hTxHandle, "TX_vSetClientID");
    pfSetDILInterfacePtr                = (SETDILINTERFACEPTR)GetProcAddress(m_hTxHandle, "TX_vSetDILInterfacePtr");
    pfPostMessageToTxWnd                = (POSTMESSAGETOTXWND)GetProcAddress(m_hTxHandle, "TX_vPostMessageToTxWnd");
    pfConfigWindowShown                 = (CONFIGWINDOWSHOWN)GetProcAddress(m_hTxHandle, "TX_hConfigWindowShown");
    pfStartTransmission                 = (STARTTRANSMISSION)GetProcAddress(m_hTxHandle, "TX_vStartTransmission");
    pfAllocateMemoryForGlobalTxList     = (ALLOCMEMFORGLOBALTXLIST)GetProcAddress(m_hTxHandle, "TX_bAllocateMemoryForGlobalTxList");
    pfAssignMsgBlockList                = (ASSIGNMSGBLOCKLIST)GetProcAddress(m_hTxHandle, "TX_vAssignMsgBlockList");
    pfDeleteTxBlockMemory               = (DELETETXBLOCKMEMORY)GetProcAddress(m_hTxHandle, "TX_vDeleteTxBlockMemory");
    pfStopTransmission                  = (STOPTRANSMISSION)GetProcAddress(m_hTxHandle, "TX_vStopTransmission");
    pfGetTxWndConfigData                = (GETTXWNDCONFIGDATAXML)GetProcAddress(m_hTxHandle, "TX_vGetTxWndConfigData");
    pfSetTxWndConfigData                = (SETTXWNDCONFIGDATA)GetProcAddress(m_hTxHandle, "TX_vSetTxWndConfigData");
    pfSetTxWndConfigDataXML             = (SETTXWNDCONFIGDATAXML)GetProcAddress(m_hTxHandle, "TX_vSetTxWndConfigDataXML");
    //pfIsTxWndConfigChanged                = (ISTXWNDCONFIGCHANGED)GetProcAddress(m_hTxHandle, "TX_bIsTxWndConfigChanged");
    pfSetTxStopFlag                     = (SETTXSTOPFLAG)GetProcAddress(m_hTxHandle, "TX_vSetTxStopFlag");
    pfGetTxStopFlag                     = (GETTXSTOPFLAG)GetProcAddress(m_hTxHandle, "TX_bGetTxStopFlag");
    pfGetTxBlockCount                   = (GETTXBLOCKCOUNT)GetProcAddress(m_hTxHandle, "TX_unGetTxBlockCount");
}

/*******************************************************************************
  Function Name  : vSetMsgDBPtrInDetailsView
  Input(s)       : pMsgDB
  Output         : -
  Functionality  : To set CMsgSignal pointer in TxWindow
  Member of      : CTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 04.08.2010
  Modifications  :
*******************************************************************************/
void CTxHandler::vSetMsgDBPtrInDetailsView(void* pMsgDB)
{
    if(pfSetMsgDBPtrInDetailsView != nullptr)
    {
        pfSetMsgDBPtrInDetailsView(pMsgDB);
    }
}

/*******************************************************************************
  Function Name  : vShowConfigureMsgWindow
  Input(s)       : -
  Output         : -
  Functionality  : To Show the Configure Msg Window Dialog
  Member of      : CTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 02.08.2010
  Modifications  :
*******************************************************************************/
void CTxHandler::vShowConfigureMsgWindow(void* pParentWnd)
{
    if(pfShowConfigureMsgWindow != nullptr)
    {
        pfShowConfigureMsgWindow(pParentWnd);
    }
}

/*******************************************************************************
  Function Name  : vSetClientID
  Input(s)       : dwClientID
  Output         : -
  Functionality  : To update client ID to TxWindow Dll.
  Member of      : CTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 03.08.2010
  Modifications  :
*******************************************************************************/
void CTxHandler::vSetClientID(DWORD dwClientID)
{
    if(pfSetClientId != nullptr)
    {
        pfSetClientId(dwClientID);
    }
}

/*******************************************************************************
  Function Name  : vSetDILInterfacePtr
  Input(s)       : DILInterfacePtr (void*)
  Output         : -
  Functionality  : To update DIL interface Pointer to TxWindow Dll.
  Member of      : CTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 03.08.2010
  Modifications  :
*******************************************************************************/
void CTxHandler::vSetDILInterfacePtr(void* ptrDILIntrf)
{
    if(pfSetDILInterfacePtr != nullptr)
    {
        pfSetDILInterfacePtr(ptrDILIntrf);
    }
}

/*******************************************************************************
  Function Name  : vPostMessageToTxWnd
  Input(s)       : UINT msg, WPARAM wParam, LPARAM lParam
  Output         : -
  Functionality  : Posts the Message to TxWindow Dll.
  Member of      : CTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 03.08.2010
  Modifications  :
*******************************************************************************/
void CTxHandler::vPostMessageToTxWnd(UINT msg, WPARAM wParam, LPARAM lParam)
{
    if(pfPostMessageToTxWnd != nullptr)
    {
        pfPostMessageToTxWnd(msg, wParam, lParam);
    }
}

/*******************************************************************************
  Function Name  : hConfigWindowShown
  Input(s)       : -
  Output         : HRESULT
  Functionality  : Gets the TxConfiguration Window availability status.
  Member of      : CTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 04.08.2010
  Modifications  :
*******************************************************************************/
HRESULT CTxHandler::hConfigWindowShown()
{
    HRESULT hResult = S_FALSE;

    if(pfConfigWindowShown != nullptr)
    {
        hResult = pfConfigWindowShown();
    }

    return hResult;
}

/*******************************************************************************
  Function Name  : vStartTransmission
  Input(s)       : ucKeyVal
  Output         : -
  Functionality  : Start transmission
  Member of      : CTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CTxHandler::vStartTransmission(UCHAR ucKeyVal)
{
    if(pfStartTransmission != nullptr)
    {
        pfStartTransmission(ucKeyVal);
    }
}

/*******************************************************************************
  Function Name  : hAllocateMemoryForGlobalTxList
  Input(s)       : -
  Output         : HRESULT
  Functionality  : Allocates memory for Global Tx List
  Member of      : CTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
HRESULT CTxHandler::hAllocateMemoryForGlobalTxList()
{
    HRESULT hResult = S_OK;

    if(pfAllocateMemoryForGlobalTxList != nullptr)
    {
        hResult = pfAllocateMemoryForGlobalTxList();
    }

    return hResult;
}

/*******************************************************************************
  Function Name  : vAssignMsgBlockList
  Input(s)       : -
  Output         : -
  Functionality  : Assigns Message Block List
  Member of      : CTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CTxHandler::vAssignMsgBlockList()
{
    if(pfAssignMsgBlockList != nullptr)
    {
        pfAssignMsgBlockList();
    }
}

/*******************************************************************************
  Function Name  : vDeleteTxBlockMemory
  Input(s)       : -
  Output         : -
  Functionality  : Deletes Tx Block Memory
  Member of      : CTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CTxHandler::vDeleteTxBlockMemory()
{
    if(pfDeleteTxBlockMemory != nullptr)
    {
        pfDeleteTxBlockMemory();
    }
}

/*******************************************************************************
  Function Name  : vStopTransmission
  Input(s)       : unMaxWaitTime
  Output         : -
  Functionality  : Stops the Transmission of Messages.
  Member of      : CTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CTxHandler::vStopTransmission(UINT unMaxWaitTime)
{
    if(pfStopTransmission != nullptr)
    {
        pfStopTransmission(unMaxWaitTime);
    }
}
/*******************************************************************************
  Function Name  : vGetTxWndConfigData
  Input(s)       : xmlNodePtr
  Output         : -
  Functionality  : Gets the configuration data of Tx Window in XML format.
  Member of      : CTxHandler
  Author(s)      : Ashwin R Uchil
  Date Created   : 2-8-2012
  Modifications  :
*******************************************************************************/
void CTxHandler::vGetTxWndConfigData(xmlNodePtr pxmlNodePtr)
{
    if(pfGetTxWndConfigData != nullptr)
    {
        pfGetTxWndConfigData(pxmlNodePtr);
    }
}

/**
 * Gets the configuration data of Tx Window.
 */
void CTxHandler::vGetTxWndConfigData(BYTE * & /* pDesBuffer */, int & /* nBuffSize */)
{
}

/*******************************************************************************
  Function Name  : vSetTxWndConfigData
  Input(s)       : pSrcBuffer, nBuffSize
  Output         : -
  Functionality  : Sets the configuration data of Tx Window.
  Member of      : CTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CTxHandler::vSetTxWndConfigData(BYTE* pSrcBuffer, int nBuffSize)
{
    if(pfSetTxWndConfigData != nullptr)
    {
        pfSetTxWndConfigData(pSrcBuffer,nBuffSize);
    }
}
void CTxHandler::vSetTxWndConfigData(xmlDocPtr pDoc)
{
    if(pfSetTxWndConfigDataXML != nullptr)
    {
        pfSetTxWndConfigDataXML(pDoc);
    }
}

/*******************************************************************************
  Function Name  : hIsTxWndConfigChanged
  Input(s)       : -
  Output         : HRESULT
  Functionality  : Gets the Configuration Changed status.
  Member of      : CTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
HRESULT CTxHandler::hIsTxWndConfigChanged()
{
    HRESULT hResult = S_OK;
    //if(pfIsTxWndConfigChanged != nullptr)
    //  hResult = pfIsTxWndConfigChanged();
    return hResult;
}

UINT CTxHandler::unGetTxBlockCount(void)
{
    UINT Result = 0;

    if (nullptr != pfGetTxBlockCount)
    {
        Result = pfGetTxBlockCount();
    }
    return Result;
}

/*******************************************************************************
  Function Name  : vSetTxStopFlag
  Input(s)       : bStartStop
  Output         : -
  Functionality  : Sets Stop Flag to 'bStartStop'.
  Member of      : CTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CTxHandler::vSetTxStopFlag(BOOL bStartStop)
{
    if (nullptr != pfSetTxStopFlag)
    {
        pfSetTxStopFlag(bStartStop);
    }
}

/*******************************************************************************
  Function Name  : bGetTxStopFlag
  Input(s)       : -
  Output         : HRESULT
  Functionality  : Gets the StopFlag status.
  Member of      : CTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
BOOL CTxHandler::bGetTxStopFlag(void)
{
    BOOL bResult = TRUE;

    if (nullptr != pfGetTxStopFlag)
    {
        bResult = pfGetTxStopFlag();
    }

    return bResult;
}
