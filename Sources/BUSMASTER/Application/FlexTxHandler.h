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
 * \file      TxHandler.h
 * \brief     CTxHandler class
 * \author    ArunKumar K
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CTxHandler class
 */
#pragma once
#include "TxWindow/TXWindow_Extern.h"
#include "MsgSignal.h"
class CFlexTxHandler
{
private:
    HMODULE m_hTxHandle;
    void vInitializeFuncPtrs();
    void vloadFuncPtrAddress();
public:
    CFlexTxHandler(void);
    ~CFlexTxHandler(void);
    void vLoadTx_DLL();
    void vBusStatusChanged(ETYPE_BUS eBusType, ESTATUS_BUS eBusStatus);
    void vShowConfigureMsgWindow(void* pParentWnd, ETYPE_BUS eBUS);
    void vSetClientID(ETYPE_BUS eBusType, DWORD dwClientID);
    void vSetDILInterfacePtr(ETYPE_BUS eBusType, void*);
    void vPostMessageToTxWnd(UINT msg, WPARAM wParam, LPARAM lParam);
    HRESULT hConfigWindowShown(ETYPE_BUS eBusType);

    void vGetTxWndConfigData( ETYPE_BUS eBusType, xmlNodePtr pxmlNodePtr);

    void vSetTxWndConfigData( ETYPE_BUS eBusType, xmlDocPtr pDoc);


    HRESULT SetNetworkConfig(ETYPE_BUS eBusType, IBMNetWorkGetService* ouFlexConfig);


    //LIN
    void vShowLINScheduleConfigDlg(bool bShow);
    void vCreateLINScheduleConfigDlg(void* pParentWnd, IBMNetWorkGetService* pClusterConfig);
    void vSetScheduleConfig(xmlDocPtr pxmlDocPtr);
    void vGetScheduleConfig(xmlNodePtr pxmlNodePtr);
};