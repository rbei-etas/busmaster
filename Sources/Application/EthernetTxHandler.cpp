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
#include "EthernetTxHandler.h"

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
//typedef void (*FLEXFILECHANGED)();
typedef HRESULT (*PFUPDATEFIBEXCONFIG)(ETYPE_BUS eBusType, ClusterConfig& ouFlexConfig);
typedef HRESULT (*BUSSTATUSCHANGED)(ETYPE_BUS eBusType,  ESTATUS_BUS eBusStatus);
typedef HRESULT (*PFSETCHANNELCONFIG)(ETYPE_BUS eBus, ClusterConfig* pMsgData);


SETMSGPTRINDETAILSVIEW      pfEthSetMsgDBPtrInDetailsView;
SHOWCONFIGUREMSGWINDOW      pfEthShowConfigureMsgWindow;
SETCLIENTID                 pfEthSetClientId;
SETDILINTERFACEPTR          pfEthSetDILInterfacePtr;
POSTMESSAGETOTXWND          pfEthPostMessageToTxWnd;
CONFIGWINDOWSHOWN           pfEthConfigWindowShown;
STARTTRANSMISSION           pfEthStartTransmission;
ALLOCMEMFORGLOBALTXLIST     pfEthAllocateMemoryForGlobalTxList;
ASSIGNMSGBLOCKLIST          pfEthAssignMsgBlockList;
DELETETXBLOCKMEMORY         pfEthDeleteTxBlockMemory;
STOPTRANSMISSION            pfEthStopTransmission;
BUSSTATUSCHANGED            pfEthBusStatusChanged;
GETTXWNDCONFIGDATAXML       pfEthGetTxWndConfigData;
SETTXWNDCONFIGDATA          pfEthSetTxWndConfigData;
SETTXWNDCONFIGDATAXML       pfEthSetTxWndConfigDataXML;
//ISTXWNDCONFIGCHANGED      pfIsTxWndConfigChanged;
GETTXBLOCKCOUNT             pfEthGetTxBlockCount;
SETTXSTOPFLAG               pfEthSetTxStopFlag;
GETTXSTOPFLAG               pfEthGetTxStopFlag;
//FLEXFILECHANGED             pfFlexFileChanged;
//PFUPDATEFIBEXCONFIG         pfUpdateFibexConfig;
//PFSETCHANNELCONFIG          pfSetChannelConfig;

CEthernetTxHandler::CEthernetTxHandler(void)
{
    m_hTxHandle = NULL;
}

CEthernetTxHandler::~CEthernetTxHandler(void)
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
  Member of      : CEthernetTxHandler
  Author(s)      : Ashwin R Uchil
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CEthernetTxHandler::vLoadTx_DLL()
{
    if ( m_hTxHandle != NULL )
    {
        FreeLibrary(m_hTxHandle);
        m_hTxHandle = NULL;
    }
    m_hTxHandle = LoadLibrary("TXWindowEthernet.dll");
    vloadFuncPtrAddress();
}

/*******************************************************************************
  Function Name  : vInitializeFuncPtrs
  Input(s)       : -
  Output         : -
  Functionality  : Initialize Function Ptrs to null
  Member of      : CEthernetTxHandler
  Author(s)      : Ashwin R Uchil
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CEthernetTxHandler::vInitializeFuncPtrs()
{
    pfEthSetMsgDBPtrInDetailsView      = NULL;
    pfEthShowConfigureMsgWindow        = NULL;
    pfEthSetClientId                   = NULL;
    pfEthSetDILInterfacePtr            = NULL;
    pfEthPostMessageToTxWnd            = NULL;
    pfEthConfigWindowShown             = NULL;
    pfEthStartTransmission             = NULL;
    pfEthAllocateMemoryForGlobalTxList = NULL;
    pfEthAssignMsgBlockList            = NULL;
    pfEthDeleteTxBlockMemory           = NULL;
    pfEthStopTransmission              = NULL;
    pfEthGetTxWndConfigData            = NULL;
    pfEthSetTxWndConfigData            = NULL;
    //pfIsTxWndConfigChanged            = NULL;
    pfEthSetTxStopFlag                 = NULL;
    pfEthGetTxStopFlag                 = NULL;
    //pfFlexFileChanged                   = NULL;
    //pfUpdateFibexConfig                 = NULL;
    pfEthBusStatusChanged                  = NULL;
    //pfSetChannelConfig                  = NULL;
}

/*******************************************************************************
  Function Name  : vloadFuncPtrAddress
  Input(s)       : -
  Output         : -
  Functionality  : load lib and get Function pointers
  Member of      : CEthernetTxHandler
  Author(s)      : Ashwin R Uchil
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CEthernetTxHandler::vloadFuncPtrAddress()
{
    vInitializeFuncPtrs();
    pfEthSetMsgDBPtrInDetailsView          = (SETMSGPTRINDETAILSVIEW)GetProcAddress(m_hTxHandle, "TXEthernet_vSetMsgDBPtrInDetailsView");
    pfEthShowConfigureMsgWindow            = (SHOWCONFIGUREMSGWINDOW)GetProcAddress(m_hTxHandle, "TXEthernet_vShowConfigureMsgWindow");         //mess wnd creation
    pfEthSetClientId                       = (SETCLIENTID)GetProcAddress(m_hTxHandle, "TXComman_vSetClientID");
    pfEthSetDILInterfacePtr                = (SETDILINTERFACEPTR)GetProcAddress(m_hTxHandle, "TXComman_vSetDILInterfacePtr");
    pfEthPostMessageToTxWnd                = (POSTMESSAGETOTXWND)GetProcAddress(m_hTxHandle, "TXComman_vPostMessageToTxWnd");
    pfEthConfigWindowShown                 = (CONFIGWINDOWSHOWN)GetProcAddress(m_hTxHandle, "TXComman_hConfigWindowShown");
    pfEthStartTransmission                 = (STARTTRANSMISSION)GetProcAddress(m_hTxHandle, "TXEthernet_vStartTransmission");
    pfEthAllocateMemoryForGlobalTxList     = (ALLOCMEMFORGLOBALTXLIST)GetProcAddress(m_hTxHandle, "TXEthernet_bAllocateMemoryForGlobalTxList");
    pfEthAssignMsgBlockList                = (ASSIGNMSGBLOCKLIST)GetProcAddress(m_hTxHandle, "TXEthernet_vAssignMsgBlockList");
    pfEthDeleteTxBlockMemory               = (DELETETXBLOCKMEMORY)GetProcAddress(m_hTxHandle, "TXEthernet_vDeleteTxBlockMemory");
    pfEthStopTransmission                  = (STOPTRANSMISSION)GetProcAddress(m_hTxHandle, "TXEthernet_vBusStatusChanged");
    pfEthGetTxWndConfigData                = (GETTXWNDCONFIGDATAXML)GetProcAddress(m_hTxHandle, "TXComman_vGetTxWndConfigData");
    pfEthSetTxWndConfigData                = (SETTXWNDCONFIGDATA)GetProcAddress(m_hTxHandle, "TXEthernet_vSetTxWndConfigData");
    pfEthSetTxWndConfigDataXML             = (SETTXWNDCONFIGDATAXML)GetProcAddress(m_hTxHandle, "TXComman_vSetTxWndConfigDataXML");
    //pfIsTxWndConfigChanged                = (ISTXWNDCONFIGCHANGED)GetProcAddress(m_hTxHandle, "TXFlexRay_bIsTxWndConfigChanged");
    pfEthSetTxStopFlag                     = (SETTXSTOPFLAG)GetProcAddress(m_hTxHandle, "TXEthernet_vSetTxStopFlag");
    pfEthGetTxStopFlag                     = (GETTXSTOPFLAG)GetProcAddress(m_hTxHandle, "TXEthernet_bGetTxStopFlag");
    pfEthGetTxBlockCount                   = (GETTXBLOCKCOUNT)GetProcAddress(m_hTxHandle, "TXEthernet_unGetTxBlockCount");
    //pfFlexFileChanged                       = (FLEXFILECHANGED)GetProcAddress(m_hTxHandle, "TXFlexRay_vFlexFileChanged");
    //pfUpdateFibexConfig                     = (PFUPDATEFIBEXCONFIG)GetProcAddress(m_hTxHandle, "TXComman_nSetFibexConfig");
    pfEthBusStatusChanged                      = (BUSSTATUSCHANGED)GetProcAddress(m_hTxHandle, "TX_vBusStatusChanged");
    //pfSetChannelConfig                      = (PFSETCHANNELCONFIG)GetProcAddress(m_hTxHandle, "TX_vSetChannelConfig");
}


void CEthernetTxHandler::vBusStatusChanged(ETYPE_BUS eBusType, ESTATUS_BUS eBusStatus)
{
    if ( pfEthBusStatusChanged != NULL )
    {
        pfEthBusStatusChanged(eBusType, eBusStatus);
    }
}



/*******************************************************************************
  Function Name  : vSetMsgDBPtrInDetailsView
  Input(s)       : pMsgDB
  Output         : -
  Functionality  : To set CMsgSignal pointer in TxWindow
  Member of      : CEthernetTxHandler
  Author(s)      : Ashwin R Uchil
  Date Created   : 04.08.2010
  Modifications  :
*******************************************************************************/
void CEthernetTxHandler::vSetMsgDBPtrInDetailsView(void* pMsgDB)
{
    if(pfEthSetMsgDBPtrInDetailsView != NULL)
    {
        pfEthSetMsgDBPtrInDetailsView(pMsgDB);
    }
}

/*******************************************************************************
  Function Name  : vShowConfigureMsgWindow
  Input(s)       : -
  Output         : -
  Functionality  : To Show the Configure Msg Window Dialog
  Member of      : CEthernetTxHandler
  Author(s)      : Ashwin R Uchil
  Date Created   : 02.08.2010
  Modifications  :
*******************************************************************************/
void CEthernetTxHandler::vShowConfigureMsgWindow(void* pParentWnd, ETYPE_BUS eBUS)
{
    if(pfEthShowConfigureMsgWindow != NULL)
    {
        pfEthShowConfigureMsgWindow(pParentWnd, eBUS);
    }
}

/*******************************************************************************
  Function Name  : vSetClientID
  Input(s)       : dwClientID
  Output         : -
  Functionality  : To update client ID to TxWindow Dll.
  Member of      : CEthernetTxHandler
  Author(s)      : Ashwin R Uchil
  Date Created   : 03.08.2010
  Modifications  :
*******************************************************************************/
void CEthernetTxHandler::vSetClientID(ETYPE_BUS eBusType,  DWORD dwClientID)
{
    if(pfEthSetClientId != NULL)
    {
        pfEthSetClientId(eBusType, dwClientID);
    }
}

/*******************************************************************************
  Function Name  : vSetDILInterfacePtr
  Input(s)       : DILInterfacePtr (void*)
  Output         : -
  Functionality  : To update DIL interface Pointer to TxWindow Dll.
  Member of      : CEthernetTxHandler
  Author(s)      : Ashwin R Uchil
  Date Created   : 03.08.2010
  Modifications  :
*******************************************************************************/
void CEthernetTxHandler::vSetDILInterfacePtr(ETYPE_BUS eBusType, void* pDilInterface)
{
    if(pfEthSetDILInterfacePtr != NULL)
    {
        pfEthSetDILInterfacePtr(eBusType, pDilInterface );
    }
}

/*******************************************************************************
  Function Name  : vFlexFileChanged
  Input(s)       : -
  Output         : -
  Functionality  : Intimates the FlexTxWindow about the change of ClusterConfig File
  Member of      : CEthernetTxHandler
  Author(s)      : Ashwin R Uchil
  Date Created   : 14.06.2013
  Modifications  :
*******************************************************************************/
//void CEthernetTxHandler::vFlexFileChanged()
//{
//    if(pfFlexFileChanged != NULL)
//    {
//        pfFlexFileChanged();
//    }
//}

/*******************************************************************************
  Function Name  : vPostMessageToTxWnd
  Input(s)       : UINT msg, WPARAM wParam, LPARAM lParam
  Output         : -
  Functionality  : Posts the Message to TxWindow Dll.
  Member of      : CEthernetTxHandler
  Author(s)      : Ashwin R Uchil
  Date Created   : 03.08.2010
  Modifications  :
*******************************************************************************/
void CEthernetTxHandler::vPostMessageToTxWnd(UINT msg, WPARAM wParam, LPARAM lParam)
{
    if(pfEthPostMessageToTxWnd != NULL)
    {
        pfEthPostMessageToTxWnd(msg, wParam, lParam);
    }
}

/*******************************************************************************
  Function Name  : hConfigWindowShown
  Input(s)       : -
  Output         : HRESULT
  Functionality  : Gets the TxConfiguration Window availability status.
  Member of      : CEthernetTxHandler
  Author(s)      : Ashwin R Uchil
  Date Created   : 04.08.2010
  Modifications  :
*******************************************************************************/
HRESULT CEthernetTxHandler::hConfigWindowShown(ETYPE_BUS eBusType)
{
    HRESULT hResult = S_FALSE;

    if(pfEthConfigWindowShown != NULL)
    {
        hResult = pfEthConfigWindowShown(eBusType);
    }

    return hResult;
}

/*******************************************************************************
  Function Name  : vStartTransmission
  Input(s)       : ucKeyVal
  Output         : -
  Functionality  : Start transmission
  Member of      : CEthernetTxHandler
  Author(s)      : Ashwin R Uchil
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CEthernetTxHandler::vStartTransmission(UCHAR ucKeyVal)
{
    if(pfEthStartTransmission != NULL)
    {
        pfEthStartTransmission(ucKeyVal);
    }
}

/*******************************************************************************
  Function Name  : hAllocateMemoryForGlobalTxList
  Input(s)       : -
  Output         : HRESULT
  Functionality  : Allocates memory for Global Tx List
  Member of      : CEthernetTxHandler
  Author(s)      : Ashwin R Uchil
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
HRESULT CEthernetTxHandler::hAllocateMemoryForGlobalTxList()
{
    HRESULT hResult = S_OK;

    if(pfEthAllocateMemoryForGlobalTxList != NULL)
    {
        hResult = pfEthAllocateMemoryForGlobalTxList();
    }

    return hResult;
}

/*******************************************************************************
  Function Name  : vAssignMsgBlockList
  Input(s)       : -
  Output         : -
  Functionality  : Assigns Message Block List
  Member of      : CEthernetTxHandler
  Author(s)      : Ashwin R Uchil
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CEthernetTxHandler::vAssignMsgBlockList()
{
    if(pfEthAssignMsgBlockList != NULL)
    {
        pfEthAssignMsgBlockList();
    }
}

/*******************************************************************************
  Function Name  : vDeleteTxBlockMemory
  Input(s)       : -
  Output         : -
  Functionality  : Deletes Tx Block Memory
  Member of      : CEthernetTxHandler
  Author(s)      : Ashwin R Uchil
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CEthernetTxHandler::vDeleteTxBlockMemory()
{
    if(pfEthDeleteTxBlockMemory != NULL)
    {
        pfEthDeleteTxBlockMemory();
    }
}

/*******************************************************************************
  Function Name  : vStopTransmission
  Input(s)       : unMaxWaitTime
  Output         : -
  Functionality  : Stops the Transmission of Messages.
  Member of      : CEthernetTxHandler
  Author(s)      : Ashwin R Uchil
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CEthernetTxHandler::vStopTransmission(UINT unMaxWaitTime)
{
    if(pfEthStopTransmission != NULL)
    {
        pfEthStopTransmission(unMaxWaitTime);
    }
}
/*******************************************************************************
  Function Name  : vGetTxWndConfigData
  Input(s)       : xmlNodePtr
  Output         : -
  Functionality  : Gets the configuration data of Tx Window in XML format.
  Member of      : CEthernetTxHandler
  Author(s)      : Ashwin R Uchil
  Date Created   : 2-8-2012
  Modifications  :
*******************************************************************************/
void CEthernetTxHandler::vGetTxWndConfigData(ETYPE_BUS eBusType, xmlNodePtr pxmlNodePtr)
{
    if(pfEthGetTxWndConfigData != NULL)
    {
        pfEthGetTxWndConfigData(eBusType, pxmlNodePtr);
    }
}

/*******************************************************************************
  Function Name  : vGetTxWndConfigData
  Input(s)       : pDesBuffer, nBuffSize
  Output         : -
  Functionality  : Gets the configuration data of Tx Window.
  Member of      : CEthernetTxHandler
  Author(s)      : Ashwin R Uchil
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CEthernetTxHandler::vGetTxWndConfigData(BYTE*& pDesBuffer, int& nBuffSize)
{
    if(pfEthGetTxWndConfigData != NULL)
    {
        // pfEthGetTxWndConfigData(pDesBuffer, nBuffSize);
    }
}

/*******************************************************************************
  Function Name  : vSetTxWndConfigData
  Input(s)       : pSrcBuffer, nBuffSize
  Output         : -
  Functionality  : Sets the configuration data of Tx Window.
  Member of      : CEthernetTxHandler
  Author(s)      : Ashwin R Uchil
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CEthernetTxHandler::vSetTxWndConfigData(BYTE* pSrcBuffer, int nBuffSize)
{
    if(pfEthSetTxWndConfigData != NULL)
    {
        pfEthSetTxWndConfigData(pSrcBuffer,nBuffSize);
    }
}
void CEthernetTxHandler::vSetTxWndConfigData( ETYPE_BUS eBusType, xmlDocPtr pDoc)
{
    if(pfEthSetTxWndConfigDataXML != NULL)
    {
        pfEthSetTxWndConfigDataXML(eBusType, pDoc);
    }
}

/*******************************************************************************
  Function Name  : hIsTxWndConfigChanged
  Input(s)       : -
  Output         : HRESULT
  Functionality  : Gets the Configuration Changed status.
  Member of      : CEthernetTxHandler
  Author(s)      : Ashwin R Uchil
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
HRESULT CEthernetTxHandler::hIsTxWndConfigChanged()
{
    HRESULT hResult = S_OK;
    //if(pfIsTxWndConfigChanged != NULL)
    //  hResult = pfIsTxWndConfigChanged();
    return hResult;
}

UINT CEthernetTxHandler::unGetTxBlockCount(void)
{
    UINT Result = 0;

    if (NULL != pfEthGetTxBlockCount)
    {
        Result = pfEthGetTxBlockCount();
    }
    return Result;
}

/*******************************************************************************
  Function Name  : vSetTxStopFlag
  Input(s)       : bStartStop
  Output         : -
  Functionality  : Sets Stop Flag to 'bStartStop'.
  Member of      : CEthernetTxHandler
  Author(s)      : Ashwin R Uchil
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
void CEthernetTxHandler::vSetTxStopFlag(BOOL bStartStop)
{
    if (NULL != pfEthSetTxStopFlag)
    {
        pfEthSetTxStopFlag(bStartStop);
    }
}

/*******************************************************************************
  Function Name  : bGetTxStopFlag
  Input(s)       : -
  Output         : HRESULT
  Functionality  : Gets the StopFlag status.
  Member of      : CEthernetTxHandler
  Author(s)      : Ashwin R Uchil
  Date Created   : 30.07.2010
  Modifications  :
*******************************************************************************/
BOOL CEthernetTxHandler::bGetTxStopFlag(void)
{
    BOOL bResult = TRUE;

    if (NULL != pfEthGetTxStopFlag)
    {
        bResult = pfEthGetTxStopFlag();
    }

    return bResult;
}


//HRESULT CEthernetTxHandler::SetFibexConfig(ETYPE_BUS eBusType, ClusterConfig& ouFlexConfig)
//{
//    if ( NULL != pfUpdateFibexConfig )
//    {
//        return pfUpdateFibexConfig(eBusType, ouFlexConfig);
//    }
//    return S_FALSE;
//}
//
//
//HRESULT CEthernetTxHandler::SetChannelConfig(ETYPE_BUS eBus, ClusterConfig* ouFlexConfig)
//{
//    if ( NULL != pfUpdateFibexConfig )
//    {
//        return pfSetChannelConfig(eBus, ouFlexConfig);
//    }
//    return S_FALSE;
//}

