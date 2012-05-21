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
 * \file      TSExecutorGUI_Extern.h
 * \author    Venkatanarayana makam
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
    USAGEMODE HRESULT TS_DoInitialization(ETYPE_BUS eBus);
    USAGEMODE HRESULT TS_StartStopReadThread(ETYPE_BUS eBus, BOOL bStart);
    USAGEMODE HRESULT TS_vShowTSExecutorWindow(void* pParentWnd);
    USAGEMODE HRESULT TS_vSetDILInterfacePtr(void* ptrDILIntrf);
    USAGEMODE HRESULT TS_vPostMessageToTSWnd(UINT msg, WPARAM wParam, LPARAM lParam);
    USAGEMODE HRESULT TS_hTSEexecutorWindowShown();
    USAGEMODE HRESULT TS_hGetConfigurationData(BYTE*& pDesBuffer, UINT& nBuffSize);
    USAGEMODE HRESULT TS_hSetConfigurationData(BYTE* pSrcBuffer, UINT nBuffSize);
    USAGEMODE HRESULT TS_GetInterface(ETYPE_BUS eBus, void** ppvInterface);
    USAGEMODE HRESULT TS_BUSConnected(BOOL bConnected);
    USAGEMODE HRESULT TS_SetBUSMASTERVersionInfo(const BYTE bytMajor, const BYTE bytMinor, const BYTE bytBuild);

#ifdef __cplusplus
}
#endif
