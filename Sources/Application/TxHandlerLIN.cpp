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
#include ".\txhandlerlin.h"

//Function Pointer Declarations
typedef HRESULT (*SETMSGPTRINDETAILSVIEW)(void* pMsgDB);
typedef HRESULT (*SHOWCONFIGUREMSGWINDOW)(void* pParentWnd, SHORT shWndShowCmd);
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

SETMSGPTRINDETAILSVIEW      pfSetMsgDBPtrInDetailsViewLin;
SHOWCONFIGUREMSGWINDOW      pfShowConfigureMsgWindowLin;
SETCLIENTID                 pfSetClientIdLin;
SETDILINTERFACEPTR          pfSetDILInterfacePtrLin;
POSTMESSAGETOTXWND          pfPostMessageToTxWndLin;
CONFIGWINDOWSHOWN           pfConfigWindowShownLin;
STARTTRANSMISSION           pfStartTransmissionLin;
ALLOCMEMFORGLOBALTXLIST     pfAllocateMemoryForGlobalTxListLin;
ASSIGNMSGBLOCKLIST          pfAssignMsgBlockListLin;
DELETETXBLOCKMEMORY         pfDeleteTxBlockMemoryLin;
STOPTRANSMISSION            pfStopTransmissionLin;
GETTXWNDCONFIGDATAXML       pfGetTxWndConfigDataLin;
SETTXWNDCONFIGDATA          pfSetTxWndConfigDataLin;
SETTXWNDCONFIGDATAXML       pfSetTxWndConfigDataLinXML;
//ISTXWNDCONFIGCHANGED      pfIsTxWndConfigChanged;
GETTXBLOCKCOUNT             pfGetTxBlockCountLin;
SETTXSTOPFLAG               pfSetTxStopFlagLin;
GETTXSTOPFLAG               pfGetTxStopFlagLin;

CTxHandlerLIN::CTxHandlerLIN(void)
{
    m_hTxHandle = nullptr;
}

CTxHandlerLIN::~CTxHandlerLIN(void)
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
  Member of      : CTxHandlerLIN
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CTxHandlerLIN::vLoadTx_DLL()
{
    if ( m_hTxHandle != nullptr )
    {
        FreeLibrary(m_hTxHandle);
        m_hTxHandle = nullptr;
    }
    m_hTxHandle = LoadLibrary("TXWindowLIN.dll");
    vloadFuncPtrAddress();
}

/*******************************************************************************
  Function Name  : vInitializeFuncPtrs
  Input(s)       : -
  Output         : -
  Functionality  : Initialize Function Ptrs to null
  Member of      : CTxHandlerLIN
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CTxHandlerLIN::vInitializeFuncPtrs()
{
    pfSetMsgDBPtrInDetailsViewLin      = nullptr;
    pfShowConfigureMsgWindowLin        = nullptr;
    pfSetClientIdLin                   = nullptr;
    pfSetDILInterfacePtrLin            = nullptr;
    pfPostMessageToTxWndLin            = nullptr;
    pfConfigWindowShownLin             = nullptr;
    pfStartTransmissionLin             = nullptr;
    pfAllocateMemoryForGlobalTxListLin = nullptr;
    pfAssignMsgBlockListLin            = nullptr;
    pfDeleteTxBlockMemoryLin           = nullptr;
    pfStopTransmissionLin              = nullptr;
    pfGetTxWndConfigDataLin            = nullptr;
    pfSetTxWndConfigDataLin            = nullptr;
    //pfIsTxWndConfigChanged           = nullptr;
    pfSetTxStopFlagLin                 = nullptr;
    pfGetTxStopFlagLin                 = nullptr;
}

/*******************************************************************************
  Function Name  : vloadFuncPtrAddress
  Input(s)       : -
  Output         : -
  Functionality  : load lib and get Function pointers
  Member of      : CTxHandlerLIN
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CTxHandlerLIN::vloadFuncPtrAddress()
{
    vInitializeFuncPtrs();
    pfSetMsgDBPtrInDetailsViewLin          = (SETMSGPTRINDETAILSVIEW)GetProcAddress(m_hTxHandle, "TX_vSetMsgDBPtrInDetailsView");
    pfShowConfigureMsgWindowLin            = (SHOWCONFIGUREMSGWINDOW)GetProcAddress(m_hTxHandle, "TX_vShowConfigureMsgWindow");
    pfSetClientIdLin                       = (SETCLIENTID)GetProcAddress(m_hTxHandle, "TX_vSetClientID");
    pfSetDILInterfacePtrLin                = (SETDILINTERFACEPTR)GetProcAddress(m_hTxHandle, "TX_vSetDILInterfacePtr");
    pfPostMessageToTxWndLin                = (POSTMESSAGETOTXWND)GetProcAddress(m_hTxHandle, "TX_vPostMessageToTxWnd");
    pfConfigWindowShownLin                 = (CONFIGWINDOWSHOWN)GetProcAddress(m_hTxHandle, "TX_hConfigWindowShown");
    pfStartTransmissionLin                 = (STARTTRANSMISSION)GetProcAddress(m_hTxHandle, "TX_vStartTransmission");
    pfAllocateMemoryForGlobalTxListLin     = (ALLOCMEMFORGLOBALTXLIST)GetProcAddress(m_hTxHandle, "TX_bAllocateMemoryForGlobalTxList");
    pfAssignMsgBlockListLin                = (ASSIGNMSGBLOCKLIST)GetProcAddress(m_hTxHandle, "TX_vAssignMsgBlockList");
    pfDeleteTxBlockMemoryLin               = (DELETETXBLOCKMEMORY)GetProcAddress(m_hTxHandle, "TX_vDeleteTxBlockMemory");
    pfStopTransmissionLin                  = (STOPTRANSMISSION)GetProcAddress(m_hTxHandle, "TX_vStopTransmission");
    pfGetTxWndConfigDataLin                = (GETTXWNDCONFIGDATAXML)GetProcAddress(m_hTxHandle, "TX_vGetTxWndConfigData");
    pfSetTxWndConfigDataLin                = (SETTXWNDCONFIGDATA)GetProcAddress(m_hTxHandle, "TX_vSetTxWndConfigData");
    pfSetTxWndConfigDataLinXML             = (SETTXWNDCONFIGDATAXML)GetProcAddress(m_hTxHandle, "TX_vSetTxWndConfigDataXML");
    //pfIsTxWndConfigChanged                = (ISTXWNDCONFIGCHANGED)GetProcAddress(m_hTxHandle, "TX_bIsTxWndConfigChanged");
    pfSetTxStopFlagLin                     = (SETTXSTOPFLAG)GetProcAddress(m_hTxHandle, "TX_vSetTxStopFlag");
    pfGetTxStopFlagLin                     = (GETTXSTOPFLAG)GetProcAddress(m_hTxHandle, "TX_bGetTxStopFlag");
    pfGetTxBlockCountLin                   = (GETTXBLOCKCOUNT)GetProcAddress(m_hTxHandle, "TX_unGetTxBlockCount");
}

/*******************************************************************************
  Function Name  : vSetMsgDBPtrInDetailsView
  Input(s)       : pMsgDB
  Output         : -
  Functionality  : To set CMsgSignal pointer in TxWindow
  Member of      : CTxHandlerLIN
  Author(s)      : ArunKumar K
  Date Created   : 04.08.2010
  Modifications  :
*******************************************************************************/
void CTxHandlerLIN::vSetMsgDBPtrInDetailsView(void* pMsgDB)
{
    if(pfSetMsgDBPtrInDetailsViewLin != nullptr)
    {
        pfSetMsgDBPtrInDetailsViewLin(pMsgDB);
    }
}

/*******************************************************************************
  Function Name  : vShowConfigureMsgWindow
  Input(s)       : -
  Output         : -
  Functionality  : To Show the Configure Msg Window Dialog
  Member of      : CTxHandlerLIN
  Author(s)      : ArunKumar K
  Date Created   : 02.08.2010
  Modifications  :
*******************************************************************************/
void CTxHandlerLIN::vShowConfigureMsgWindow(void* pParentWnd, SHORT shWndShowCmd)
{
    if(pfShowConfigureMsgWindowLin != nullptr)
    {
        pfShowConfigureMsgWindowLin(pParentWnd, shWndShowCmd);
    }
}

/*******************************************************************************
  Function Name  : vSetClientID
  Input(s)       : dwClientID
  Output         : -
  Functionality  : To update client ID to TxWindow Dll.
  Member of      : CTxHandlerLIN
  Author(s)      : ArunKumar K
  Date Created   : 03.08.2010
  Modifications  :
*******************************************************************************/
void CTxHandlerLIN::vSetClientID(DWORD dwClientID)
{
    if(pfSetClientIdLin != nullptr)
    {
        pfSetClientIdLin(dwClientID);
    }
}

/*******************************************************************************
  Function Name  : vSetDILInterfacePtr
  Input(s)       : DILInterfacePtr (void*)
  Output         : -
  Functionality  : To update DIL interface Pointer to TxWindow Dll.
  Member of      : CTxHandlerLIN
  Author(s)      : ArunKumar K
  Date Created   : 03.08.2010
  Modifications  :
*******************************************************************************/
void CTxHandlerLIN::vSetDILInterfacePtr(void* ptrDILIntrf)
{
    if(pfSetDILInterfacePtrLin != nullptr)
    {
        pfSetDILInterfacePtrLin(ptrDILIntrf);
    }
}

/*******************************************************************************
  Function Name  : vPostMessageToTxWnd
  Input(s)       : UINT msg, WPARAM wParam, LPARAM lParam
  Output         : -
  Functionality  : Posts the Message to TxWindow Dll.
  Member of      : CTxHandlerLIN
  Author(s)      : ArunKumar K
  Date Created   : 03.08.2010
  Modifications  :
*******************************************************************************/
void CTxHandlerLIN::vPostMessageToTxWnd(UINT msg, WPARAM wParam, LPARAM lParam)
{
    if(pfPostMessageToTxWndLin != nullptr)
    {
        pfPostMessageToTxWndLin(msg, wParam, lParam);
    }

}

/*******************************************************************************
  Function Name  : hConfigWindowShown
  Input(s)       : -
  Output         : HRESULT
  Functionality  : Gets the TxConfiguration Window availability status.
  Member of      : CTxHandlerLIN
  Author(s)      : ArunKumar K
  Date Created   : 04.08.2010
  Modifications  :
*******************************************************************************/
HRESULT CTxHandlerLIN::hConfigWindowShown()
{
    HRESULT hResult = S_FALSE;

    if(pfConfigWindowShownLin != nullptr)
    {
        hResult = pfConfigWindowShownLin();
    }

    return hResult;
}

/*******************************************************************************
  Function Name  : vStartTransmission
  Input(s)       : ucKeyVal
  Output         : -
  Functionality  : Start transmission
  Member of      : CTxHandlerLIN
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CTxHandlerLIN::vStartTransmission(UCHAR ucKeyVal)
{
    if(pfStartTransmissionLin != nullptr)
    {
        pfStartTransmissionLin(ucKeyVal);
    }
}

/*******************************************************************************
  Function Name  : hAllocateMemoryForGlobalTxList
  Input(s)       : -
  Output         : HRESULT
  Functionality  : Allocates memory for Global Tx List
  Member of      : CTxHandlerLIN
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
HRESULT CTxHandlerLIN::hAllocateMemoryForGlobalTxList()
{
    HRESULT hResult = S_OK;

    if(pfAllocateMemoryForGlobalTxListLin != nullptr)
    {
        hResult = pfAllocateMemoryForGlobalTxListLin();
    }

    return hResult;
}

/*******************************************************************************
  Function Name  : vAssignMsgBlockList
  Input(s)       : -
  Output         : -
  Functionality  : Assigns Message Block List
  Member of      : CTxHandlerLIN
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CTxHandlerLIN::vAssignMsgBlockList()
{
    if(pfAssignMsgBlockListLin != nullptr)
    {
        pfAssignMsgBlockListLin();
    }
}

/*******************************************************************************
  Function Name  : vDeleteTxBlockMemory
  Input(s)       : -
  Output         : -
  Functionality  : Deletes Tx Block Memory
  Member of      : CTxHandlerLIN
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CTxHandlerLIN::vDeleteTxBlockMemory()
{
    if(pfDeleteTxBlockMemoryLin != nullptr)
    {
        pfDeleteTxBlockMemoryLin();
    }
}

/*******************************************************************************
  Function Name  : vStopTransmission
  Input(s)       : unMaxWaitTime
  Output         : -
  Functionality  : Stops the Transmission of Messages.
  Member of      : CTxHandlerLIN
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CTxHandlerLIN::vStopTransmission(UINT unMaxWaitTime)
{
    if(pfStopTransmissionLin != nullptr)
    {
        pfStopTransmissionLin(unMaxWaitTime);
    }
}
/*******************************************************************************
  Function Name  : vGetTxWndConfigData
  Input(s)       : xmlNodePtr
  Output         : -
  Functionality  : Gets the configuration data of Tx Window in XML format.
  Member of      : CTxHandlerLIN
  Author(s)      : Ashwin R Uchil
  Date Created   : 2-8-2012
  Modifications  :
*******************************************************************************/
void CTxHandlerLIN::vGetTxWndConfigData(xmlNodePtr pxmlNodePtr)
{
    if(pfGetTxWndConfigDataLin != nullptr)
    {
        pfGetTxWndConfigDataLin(pxmlNodePtr);
    }
}

/**
 * Gets the configuration data of Tx Window.
 */
void CTxHandlerLIN::vGetTxWndConfigData(BYTE*& /* pDesBuffer */, int& /* nBuffSize */)
{
}

/*******************************************************************************
  Function Name  : vSetTxWndConfigData
  Input(s)       : pSrcBuffer, nBuffSize
  Output         : -
  Functionality  : Sets the configuration data of Tx Window.
  Member of      : CTxHandlerLIN
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CTxHandlerLIN::vSetTxWndConfigData(BYTE* pSrcBuffer, int nBuffSize)
{
    if(pfSetTxWndConfigDataLin != nullptr)
    {
        pfSetTxWndConfigDataLin(pSrcBuffer,nBuffSize);
    }
}
void CTxHandlerLIN::vSetTxWndConfigData(xmlDocPtr pDoc)
{
    if(pfSetTxWndConfigDataLinXML != nullptr)
    {
        pfSetTxWndConfigDataLinXML(pDoc);
    }
}

/*******************************************************************************
  Function Name  : hIsTxWndConfigChanged
  Input(s)       : -
  Output         : HRESULT
  Functionality  : Gets the Configuration Changed status.
  Member of      : CTxHandlerLIN
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
HRESULT CTxHandlerLIN::hIsTxWndConfigChanged()
{
    HRESULT hResult = S_OK;
    //if(pfIsTxWndConfigChanged != nullptr)
    //  hResult = pfIsTxWndConfigChanged();
    return hResult;
}

UINT CTxHandlerLIN::unGetTxBlockCount(void)
{
    UINT Result = 0;

    if (nullptr != pfGetTxBlockCountLin)
    {
        Result = pfGetTxBlockCountLin();
    }
    return Result;
}

/*******************************************************************************
  Function Name  : vSetTxStopFlag
  Input(s)       : bStartStop
  Output         : -
  Functionality  : Sets Stop Flag to 'bStartStop'.
  Member of      : CTxHandlerLIN
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CTxHandlerLIN::vSetTxStopFlag(BOOL bStartStop)
{
    if (nullptr != pfSetTxStopFlagLin)
    {
        pfSetTxStopFlagLin(bStartStop);
    }
}

/*******************************************************************************
  Function Name  : bGetTxStopFlag
  Input(s)       : -
  Output         : HRESULT
  Functionality  : Gets the StopFlag status.
  Member of      : CTxHandlerLIN
  Author(s)      : ArunKumar K
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
BOOL CTxHandlerLIN::bGetTxStopFlag(void)
{
    BOOL bResult = TRUE;

    if (nullptr != pfGetTxStopFlagLin)
    {
        bResult = pfGetTxStopFlagLin();
    }

    return bResult;
}
