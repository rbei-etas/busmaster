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
 * \file      TxWnd_Extern.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#if defined USAGEMODE
#undef USAGEMODE
#endif

#include "Utility/XMLUtils.h"
#include "DataTypes/Cluster.h"

#if defined USAGE_EXPORT
#define USAGEMODE   __declspec(dllexport)
#else
#define USAGEMODE   __declspec(dllimport)
#endif

#ifdef __cplusplus



/**
 * See CAN_Vector_XL.cpp for the implementation of this class
 */
//class CFlexRay_TxWindow: public CWinApp
//{
//public:
//    CFlexRay_TxWindow();
//
//    // Overrides
//public:
//    virtual BOOL InitInstance();
//
//    DECLARE_MESSAGE_MAP()
//};



extern "C" {  // only need to export C interface if used by C++ source code
#endif

    USAGEMODE HRESULT TXComman_nSetFibexConfig(ETYPE_BUS eBus, ClusterConfig& ouFlexConfig);
    USAGEMODE HRESULT TXEthernet_vSetMsgDBPtrInDetailsView(void* pMsgDB);
    USAGEMODE HRESULT TXEthernet_vShowConfigureMsgWindow(void* pParentWnd, ETYPE_BUS eBUS);
    USAGEMODE HRESULT TXComman_vSetClientID(ETYPE_BUS eBusType, DWORD dwClientID);
    USAGEMODE HRESULT TXComman_vSetDILInterfacePtr(ETYPE_BUS eBusType, void* pDilPointer);
    USAGEMODE HRESULT TXComman_vPostMessageToTxWnd(UINT msg, WPARAM wParam, LPARAM lParam);
    USAGEMODE HRESULT TXComman_hConfigWindowShown(ETYPE_BUS eBusType);
    USAGEMODE HRESULT TXEthernet_vStartTransmission(UCHAR ucKeyVal);
    USAGEMODE HRESULT TXEthernet_bAllocateMemoryForGlobalTxList();
    USAGEMODE HRESULT TXEthernet_vAssignMsgBlockList();
    USAGEMODE HRESULT TXEthernet_vDeleteTxBlockMemory();
    USAGEMODE HRESULT TX_vBusStatusChanged(ETYPE_BUS eBusType,  ESTATUS_BUS eBusStatus);

    USAGEMODE HRESULT TXComman_vGetTxWndConfigData( ETYPE_BUS eBusType, xmlNodePtr pxmlNodePtr);
    USAGEMODE HRESULT TXEthernet_vSetTxWndConfigData(BYTE* pSrcBuffer, int nBuffSize);
    USAGEMODE HRESULT TXComman_vSetTxWndConfigDataXML(ETYPE_BUS eBusType, xmlDocPtr pDoc);
    USAGEMODE UINT    TXEthernet_unGetTxBlockCount(void);
    USAGEMODE HRESULT TXEthernet_vSetTxStopFlag(BOOL bStartStop);
    USAGEMODE BOOL    TXEthernet_bGetTxStopFlag();
    //USAGEMODE HRESULT TXEthernet_FileChanged();



#ifdef __cplusplus
}
#endif