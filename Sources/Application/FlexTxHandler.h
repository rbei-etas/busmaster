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
#include "TxWindowFlexRay/TxWndFlexRay_Extern.h"

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
    void vBusStatusChanged(bool bChanged);
    void vSetMsgDBPtrInDetailsView(void* pMsgDB);
    void vShowConfigureMsgWindow(VOID* pParentWnd);
    void vSetClientID(DWORD dwClientID);
    void vSetDILInterfacePtr();
    void vPostMessageToTxWnd(UINT msg, WPARAM wParam, LPARAM lParam);
    HRESULT hConfigWindowShown();
    void vStartTransmission(UCHAR ucKeyVal);
    HRESULT hAllocateMemoryForGlobalTxList();
    void vAssignMsgBlockList();
    void vDeleteTxBlockMemory();
    void vStopTransmission(UINT unMaxWaitTime);
    void vGetTxWndConfigData(BYTE*& pDesBuffer, int& nBuffSize);
    void vGetTxWndConfigData(xmlNodePtr pxmlNodePtr);
    void vSetTxWndConfigData(BYTE* pSrcBuffer, int nBuffSize);
    void vSetTxWndConfigData(xmlDocPtr pDoc);
    HRESULT hIsTxWndConfigChanged();
    UINT unGetTxBlockCount(void);
    void vSetTxStopFlag(BOOL bStartStop);
    BOOL bGetTxStopFlag(void);
    void vFlexFileChanged();

    HRESULT SetFibexConfig(FlexConfig& ouFlexConfig);
};