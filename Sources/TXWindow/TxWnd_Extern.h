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

#if defined USAGE_EXPORT
#define USAGEMODE   __declspec(dllexport)
#else
#define USAGEMODE   __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {  // only need to export C interface if used by C++ source code
#endif

USAGEMODE HRESULT TX_vSetMsgDBPtrInDetailsView(void* pMsgDB);
USAGEMODE HRESULT TX_vShowConfigureMsgWindow(void* pParentWnd);
USAGEMODE HRESULT TX_vSetClientID(DWORD dwClientID);
USAGEMODE HRESULT TX_vSetDILInterfacePtr(void* ptrDILIntrf);
USAGEMODE HRESULT TX_vPostMessageToTxWnd(UINT msg, WPARAM wParam, LPARAM lParam);
USAGEMODE HRESULT TX_hConfigWindowShown();
USAGEMODE HRESULT TX_vStartTransmission(UCHAR ucKeyVal);
USAGEMODE HRESULT TX_bAllocateMemoryForGlobalTxList();
USAGEMODE HRESULT TX_vAssignMsgBlockList();
USAGEMODE HRESULT TX_vDeleteTxBlockMemory();
USAGEMODE HRESULT TX_vStopTransmission(UINT unMaxWaitTime);
USAGEMODE HRESULT TX_vGetTxWndConfigData(BYTE*& pDesBuffer, int& nBuffSize);
USAGEMODE HRESULT TX_vSetTxWndConfigData(BYTE* pSrcBuffer, int nBuffSize);
USAGEMODE UINT    TX_unGetTxBlockCount(void);
USAGEMODE HRESULT TX_vSetTxStopFlag(BOOL bStartStop);
USAGEMODE BOOL    TX_bGetTxStopFlag();
 
#ifdef __cplusplus
}
#endif
