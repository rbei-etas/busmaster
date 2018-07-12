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
typedef HRESULT (*SHOWCONFIGUREMSGWINDOW)(void* pParentWnd, ETYPE_BUS eBUS);
typedef HRESULT (*CREATESCHEDULECONFIGDLG)(void* pParentWnd, IBMNetWorkGetService* pClusterConfig);
typedef HRESULT (*SHOWSCHEDULECONFIGDLG)(bool);
typedef HRESULT (*SETSCHECULECONFIG)(xmlDocPtr pxmlDocPtr);
typedef HRESULT (*GETSCHECULECONFIG)(xmlNodePtr pxmlNodePtr);
typedef HRESULT (*SETCLIENTID)(ETYPE_BUS eBusType, DWORD dwClientID);
typedef HRESULT (*SETDILINTERFACEPTR)(ETYPE_BUS eBusType, void*);
typedef HRESULT (*POSTMESSAGETOTXWND)(UINT msg, WPARAM wParam, LPARAM lParam);
typedef HRESULT (*CONFIGWINDOWSHOWN)(ETYPE_BUS);
typedef HRESULT (*GETTXWNDCONFIGDATA)(BYTE*& pDesBuffer, int& nBuffSize);
typedef HRESULT (*GETTXWNDCONFIGDATAXML)( ETYPE_BUS eBusType, xmlNodePtr pxmlNodePtr);
typedef HRESULT (*SETTXWNDCONFIGDATAXML)( ETYPE_BUS eBusType, xmlDocPtr pDoc);
typedef HRESULT (*PFUPDATENETWORKCONFIG)(ETYPE_BUS eBusType, IBMNetWorkGetService*);
typedef HRESULT (*BUSSTATUSCHANGED)(ETYPE_BUS eBusType,  ESTATUS_BUS eBusStatus);


SHOWCONFIGUREMSGWINDOW      pfFlexShowConfigureMsgWindow;
CREATESCHEDULECONFIGDLG       pfCreateLINScheduleConfigDlg;
SHOWSCHEDULECONFIGDLG       pfLINShowScheduleConfigDlg;
SETSCHECULECONFIG           pfLINSetScheduleConfig;
GETSCHECULECONFIG           pfLINGetScheduleConfig;
SETCLIENTID                 pfFlexSetClientId;
SETDILINTERFACEPTR          pfFlexSetDILInterfacePtr;
POSTMESSAGETOTXWND          pfFlexPostMessageToTxWnd;
CONFIGWINDOWSHOWN           pfFlexConfigWindowShown;
BUSSTATUSCHANGED            pfBusStatusChanged;
GETTXWNDCONFIGDATAXML       pfFlexGetTxWndConfigData;
SETTXWNDCONFIGDATAXML       pfFlexSetTxWndConfigDataXML;
PFUPDATENETWORKCONFIG       pfUpdateNetworkConfig;

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
    m_hTxHandle = LoadLibrary("TXWindow.dll");
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
    pfFlexShowConfigureMsgWindow        = nullptr;
    pfCreateLINScheduleConfigDlg        = nullptr;
    pfLINShowScheduleConfigDlg          = nullptr;
    pfLINSetScheduleConfig              = nullptr;
    pfLINGetScheduleConfig              = nullptr;
    pfFlexSetClientId                   = nullptr;
    pfFlexSetDILInterfacePtr            = nullptr;
    pfFlexPostMessageToTxWnd            = nullptr;
    pfFlexConfigWindowShown             = nullptr;
    pfFlexGetTxWndConfigData            = nullptr;
    pfUpdateNetworkConfig               = nullptr;
    pfBusStatusChanged                  = nullptr;

}

/*******************************************************************************
  Function Name  : vloadFuncPtrAddress
  Input(s)       : -
  Output         : -
  Functionality  : load lib and get Function pointers
  Member of      : CFlexTxHandler
  Author(s)      :
  Date Created   :
  Modifications  :
*******************************************************************************/
void CFlexTxHandler::vloadFuncPtrAddress()
{
    vInitializeFuncPtrs();
    pfFlexShowConfigureMsgWindow            = (SHOWCONFIGUREMSGWINDOW)GetProcAddress(m_hTxHandle, "TXComman_vShowConfigureMsgWindow");
    pfCreateLINScheduleConfigDlg            = (CREATESCHEDULECONFIGDLG)GetProcAddress(m_hTxHandle, "TXLIN_vCreateScheduleConfigDlg");
    pfLINShowScheduleConfigDlg              = (SHOWSCHEDULECONFIGDLG)GetProcAddress(m_hTxHandle, "TXLIN_vShowScheduleConfigDlg");
    pfLINSetScheduleConfig                  = (SETSCHECULECONFIG)GetProcAddress(m_hTxHandle, "TXComman_vSetScheduleConfig");
    pfLINGetScheduleConfig                  = (GETSCHECULECONFIG)GetProcAddress(m_hTxHandle, "TXComman_vGetScheduleConfig");
    pfFlexSetClientId                       = (SETCLIENTID)GetProcAddress(m_hTxHandle, "TXComman_vSetClientID");
    pfFlexSetDILInterfacePtr                = (SETDILINTERFACEPTR)GetProcAddress(m_hTxHandle, "TXComman_vSetDILInterfacePtr");
    pfFlexPostMessageToTxWnd                = (POSTMESSAGETOTXWND)GetProcAddress(m_hTxHandle, "TXComman_vPostMessageToTxWnd");
    pfFlexConfigWindowShown                 = (CONFIGWINDOWSHOWN)GetProcAddress(m_hTxHandle, "TXComman_hConfigWindowShown");
    pfFlexGetTxWndConfigData                = (GETTXWNDCONFIGDATAXML)GetProcAddress(m_hTxHandle, "TXComman_vGetTxWndConfigData");
    pfFlexSetTxWndConfigDataXML             = (SETTXWNDCONFIGDATAXML)GetProcAddress(m_hTxHandle, "TXComman_vSetTxWndConfigDataXML");
    pfUpdateNetworkConfig                   = (PFUPDATENETWORKCONFIG)GetProcAddress(m_hTxHandle, "TXComman_nSetNetworkConfig");
    pfBusStatusChanged                      = (BUSSTATUSCHANGED)GetProcAddress(m_hTxHandle, "TX_vBusStatusChanged");
}


void CFlexTxHandler::vBusStatusChanged(ETYPE_BUS eBusType, ESTATUS_BUS eBusStatus)
{
    if ( pfBusStatusChanged != nullptr )
    {
        pfBusStatusChanged(eBusType, eBusStatus);
    }
}

void CFlexTxHandler::vShowLINScheduleConfigDlg(bool bShow)
{
    if ( pfLINShowScheduleConfigDlg != nullptr )
    {
        pfLINShowScheduleConfigDlg(bShow);
    }

}


void CFlexTxHandler::vCreateLINScheduleConfigDlg(void* pParentWnd, IBMNetWorkGetService* pClusterConfig)
{
    if(pfCreateLINScheduleConfigDlg != nullptr)
    {
        pfCreateLINScheduleConfigDlg(pParentWnd, pClusterConfig);
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
  Function Name  : vSetScheduleConfig
  Input(s)       : xmlDocPtr
  Output         : -
  Functionality  : Sets the schedule config
  Member of      : CFlexTxHandler
  Author(s)      : Prathiba p
*******************************************************************************/
void CFlexTxHandler::vSetScheduleConfig(xmlDocPtr pxmlDocPtr)
{
    if(pfLINSetScheduleConfig != nullptr)
    {
        pfLINSetScheduleConfig(pxmlDocPtr);
    }
}

/*******************************************************************************
  Function Name  : vGetScheduleConfig
  Input(s)       : xmlNodePtr
  Output         : -
  Functionality  : Gets the schedule table configuration
  Member of      : CFlexTxHandler
  Author(s)      : Prathiba p
*******************************************************************************/
void CFlexTxHandler::vGetScheduleConfig(xmlNodePtr pxmlNodePtr)
{
    if(pfLINGetScheduleConfig != nullptr)
    {
        pfLINGetScheduleConfig(pxmlNodePtr);
    }
}
/*******************************************************************************
  Function Name  : vSetTxWndConfigData
  Input(s)       : ETYPE_BUS eBusType, xmlDocPtr pDoc
  Output         : -
  Functionality  : Sets the configuration data of Tx Window.
  Member of      : CFlexTxHandler
  Author(s)      :
  Date Created   :
  Modifications  :
*******************************************************************************/
void CFlexTxHandler::vSetTxWndConfigData( ETYPE_BUS eBusType, xmlDocPtr pDoc)
{
    if(pfFlexSetTxWndConfigDataXML != nullptr)
    {
        pfFlexSetTxWndConfigDataXML(eBusType, pDoc);
    }
}

HRESULT CFlexTxHandler::SetNetworkConfig(ETYPE_BUS eBusType, IBMNetWorkGetService* ouNetWorkConfig)
{
    if (nullptr != pfUpdateNetworkConfig)
    {
        return pfUpdateNetworkConfig(eBusType, ouNetWorkConfig);
    }
    return S_FALSE;
}



