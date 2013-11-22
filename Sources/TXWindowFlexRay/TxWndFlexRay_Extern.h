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

    USAGEMODE HRESULT TXFlexray_nSetFibexConfig(FlexConfig& ouFlexConfig);
    USAGEMODE HRESULT TXFlexRay_vSetMsgDBPtrInDetailsView(void* pMsgDB);
    USAGEMODE HRESULT TXFlexRay_vShowConfigureMsgWindow(void* pParentWnd);
    USAGEMODE HRESULT TXFlexRay_vSetClientID(DWORD dwClientID);
    USAGEMODE HRESULT TXFlexRay_vSetDILInterfacePtr();
    USAGEMODE HRESULT TXFlexRay_vPostMessageToTxWnd(UINT msg, WPARAM wParam, LPARAM lParam);
    USAGEMODE HRESULT TXFlexRay_hConfigWindowShown();
    USAGEMODE HRESULT TXFlexRay_vStartTransmission(UCHAR ucKeyVal);
    USAGEMODE HRESULT TXFlexRay_bAllocateMemoryForGlobalTxList();
    USAGEMODE HRESULT TXFlexRay_vAssignMsgBlockList();
    USAGEMODE HRESULT TXFlexRay_vDeleteTxBlockMemory();
    USAGEMODE HRESULT TXFlexRay_vBusStatusChanged(bool bConnected);

    USAGEMODE HRESULT TXFlexRay_vGetTxWndConfigData(xmlNodePtr pxmlNodePtr);
    USAGEMODE HRESULT TXFlexRay_vSetTxWndConfigData(BYTE* pSrcBuffer, int nBuffSize);
    USAGEMODE HRESULT TXFlexRay_vSetTxWndConfigDataXML(xmlDocPtr pDoc);
    USAGEMODE UINT    TXFlexRay_unGetTxBlockCount(void);
    USAGEMODE HRESULT TXFlexRay_vSetTxStopFlag(BOOL bStartStop);
    USAGEMODE BOOL    TXFlexRay_bGetTxStopFlag();
    USAGEMODE HRESULT TXFlexRay_vFlexFileChanged();


#ifdef __cplusplus
}
#endif
