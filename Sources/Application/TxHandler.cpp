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
typedef HRESULT (*SETTXWNDCONFIGDATA)(BYTE* pSrcBuffer, int nBuffSize);
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
GETTXWNDCONFIGDATA          pfGetTxWndConfigData;
SETTXWNDCONFIGDATA          pfSetTxWndConfigData;
//ISTXWNDCONFIGCHANGED      pfIsTxWndConfigChanged;
GETTXBLOCKCOUNT             pfGetTxBlockCount;
SETTXSTOPFLAG               pfSetTxStopFlag;
GETTXSTOPFLAG               pfGetTxStopFlag;

CTxHandler::CTxHandler(void)
{
    m_hTxHandle = NULL;
}

CTxHandler::~CTxHandler(void)
{
    if ( m_hTxHandle != NULL )
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
    if ( m_hTxHandle != NULL )
    {
        FreeLibrary(m_hTxHandle);
        m_hTxHandle = NULL;
    }

    m_hTxHandle = LoadLibrary(_T("TXWindow.dll"));
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
    pfSetMsgDBPtrInDetailsView      = NULL;
    pfShowConfigureMsgWindow        = NULL;
    pfSetClientId                   = NULL;
    pfSetDILInterfacePtr            = NULL;
    pfPostMessageToTxWnd            = NULL;
    pfConfigWindowShown             = NULL;
    pfStartTransmission             = NULL;
    pfAllocateMemoryForGlobalTxList = NULL;
    pfAssignMsgBlockList            = NULL;
    pfDeleteTxBlockMemory           = NULL;
    pfStopTransmission              = NULL;
    pfGetTxWndConfigData            = NULL;
    pfSetTxWndConfigData            = NULL;
    //pfIsTxWndConfigChanged            = NULL;
    pfSetTxStopFlag                 = NULL;
    pfGetTxStopFlag                 = NULL;
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
    pfGetTxWndConfigData                = (GETTXWNDCONFIGDATA)GetProcAddress(m_hTxHandle, "TX_vGetTxWndConfigData");
    pfSetTxWndConfigData                = (SETTXWNDCONFIGDATA)GetProcAddress(m_hTxHandle, "TX_vSetTxWndConfigData");
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
    if(pfSetMsgDBPtrInDetailsView != NULL)
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
    if(pfShowConfigureMsgWindow != NULL)
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
    if(pfSetClientId != NULL)
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
    if(pfSetDILInterfacePtr != NULL)
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
    if(pfPostMessageToTxWnd != NULL)
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

    if(pfConfigWindowShown != NULL)
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
    if(pfStartTransmission != NULL)
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

    if(pfAllocateMemoryForGlobalTxList != NULL)
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
    if(pfAssignMsgBlockList != NULL)
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
    if(pfDeleteTxBlockMemory != NULL)
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
    if(pfStopTransmission != NULL)
    {
        pfStopTransmission(unMaxWaitTime);
    }
}

/*******************************************************************************
  Function Name  : vGetTxWndConfigData
  Input(s)       : pDesBuffer, nBuffSize
  Output         : -
  Functionality  : Gets the configuration data of Tx Window.
  Member of      : CTxHandler
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CTxHandler::vGetTxWndConfigData(BYTE*& pDesBuffer, int& nBuffSize)
{
    if(pfGetTxWndConfigData != NULL)
    {
        pfGetTxWndConfigData(pDesBuffer, nBuffSize);
    }
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
    if(pfSetTxWndConfigData != NULL)
    {
        pfSetTxWndConfigData(pSrcBuffer, nBuffSize);
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
    //if(pfIsTxWndConfigChanged != NULL)
    //  hResult = pfIsTxWndConfigChanged();
    return hResult;
}

UINT CTxHandler::unGetTxBlockCount(void)
{
    UINT Result = 0;

    if (NULL != pfGetTxBlockCount)
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
    if (NULL != pfSetTxStopFlag)
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

    if (NULL != pfGetTxStopFlag)
    {
        bResult = pfGetTxStopFlag();
    }

    return bResult;
}
