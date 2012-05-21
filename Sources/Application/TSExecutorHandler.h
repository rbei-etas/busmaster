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
 * \file      TSExecutorHandler.h
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#define def_STR_TESTSIUTEEXECUTORDLL  _T("TestSuiteExecutorGUI.dll")
#include "TestSuiteExecutorGUI/TSExecutorGUI_Extern.h"
class TSExecutorHandler
{
private:
    HMODULE m_hTSExecutorHandle;
    BOOL m_bConnected;
    void vInitializeFuncPtrs();
    void vloadFuncPtrAddress();
public:
    TSExecutorHandler(void);
    virtual ~TSExecutorHandler(void);
    void vLoadTSExecutor_DLL();
    void vShowTSExecutorWindow(VOID* pParentWnd);
    void vGetConfigurationData(BYTE*& pDesBuffer, UINT& nBuffSize);
    void vSetConfigurationData(BYTE*& pSrcBuffer, UINT& nBuffSize);
    void vStartStopReadThread(ETYPE_BUS eBus, BOOL bStart);
    void vDoInitailization(ETYPE_BUS eBus);
    void vBusConnected(BOOL bConnected);
    void vSetTSEVersionInfo(BYTE bytMajor, BYTE bytMinor, BYTE bytBuild);
};
