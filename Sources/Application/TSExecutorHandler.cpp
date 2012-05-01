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
 * \file      TSExecutorHandler.cpp
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "stdafx.h"
#include ".\TSExecutorHandler.h"

typedef HRESULT (*SHOWTSEXECUTORWINDOW)(void* pParentWnd);
typedef HRESULT (*TSEXECUTORWINDOWSHOWN)();
typedef HRESULT (*TSEXECUTORGETCONFIGDATA)(BYTE*& pDesBuffer, UINT& nBuffSize);
typedef HRESULT (*TSEXECUTORSETCONFIGDATA)(BYTE* pSrcBuffer, UINT nBuffSize);
typedef HRESULT (*PFTSSTARTSTOPREADTHREAD)(ETYPE_BUS eBus, BOOL bStart);
typedef HRESULT (*PTSDOINITIALIZATION)(ETYPE_BUS eBus);
typedef HRESULT (*PTSBUSCONNECTED)(BOOL bConnected);
typedef HRESULT (*PTSSETVERSIONINFO)(BYTE bytMajor, BYTE bytMinor, BYTE bytBuild);

SHOWTSEXECUTORWINDOW  pfShowTSExecutorwindow;
TSEXECUTORWINDOWSHOWN pfTSExecutorWindowShown;
TSEXECUTORGETCONFIGDATA pfTSExecutorGetConfigdata;
TSEXECUTORSETCONFIGDATA pfTSExecutorSetConfigdata;
PFTSSTARTSTOPREADTHREAD pfTSStartStopReadThread;
PTSDOINITIALIZATION pfTSDoInitialization;
PTSBUSCONNECTED pfTSBusConnected;
PTSSETVERSIONINFO pfTSSetVersion;

TSExecutorHandler::TSExecutorHandler(void)
{
    m_bConnected = FALSE;
    m_hTSExecutorHandle = NULL;
}

TSExecutorHandler::~TSExecutorHandler(void)
{
    if ( m_hTSExecutorHandle != NULL )
    {
        FreeLibrary(m_hTSExecutorHandle);
    }
}
void TSExecutorHandler::vLoadTSExecutor_DLL()
{
    if ( m_hTSExecutorHandle != NULL )
    {
        FreeLibrary(m_hTSExecutorHandle);
        m_hTSExecutorHandle = NULL;
    }

    m_hTSExecutorHandle = LoadLibrary(def_STR_TESTSIUTEEXECUTORDLL);
    vloadFuncPtrAddress();
}
void TSExecutorHandler::vInitializeFuncPtrs()
{
    pfShowTSExecutorwindow = NULL;
    pfTSExecutorWindowShown = NULL;
    pfTSExecutorGetConfigdata = NULL;
    pfTSExecutorSetConfigdata = NULL;
    pfTSStartStopReadThread = NULL;
    pfTSDoInitialization = NULL;
    pfTSBusConnected = NULL;
    pfTSSetVersion = NULL;
}
void TSExecutorHandler::vloadFuncPtrAddress()
{
    pfShowTSExecutorwindow = (SHOWTSEXECUTORWINDOW)GetProcAddress(m_hTSExecutorHandle, "TS_vShowTSExecutorWindow");
    pfTSExecutorWindowShown = (TSEXECUTORWINDOWSHOWN)GetProcAddress(m_hTSExecutorHandle, "TS_hTSEexecutorWindowShown");
    pfTSExecutorGetConfigdata = (TSEXECUTORGETCONFIGDATA)GetProcAddress(m_hTSExecutorHandle, "TS_hGetConfigurationData");
    pfTSExecutorSetConfigdata = (TSEXECUTORSETCONFIGDATA)GetProcAddress(m_hTSExecutorHandle, "TS_hSetConfigurationData");
    pfTSStartStopReadThread = (PFTSSTARTSTOPREADTHREAD)GetProcAddress(m_hTSExecutorHandle, "TS_StartStopReadThread");
    pfTSDoInitialization = (PTSDOINITIALIZATION)GetProcAddress(m_hTSExecutorHandle, "TS_DoInitialization");
    pfTSBusConnected = (PTSBUSCONNECTED)GetProcAddress(m_hTSExecutorHandle, "TS_BUSConnected");
    pfTSSetVersion = (PTSSETVERSIONINFO)GetProcAddress(m_hTSExecutorHandle, "TS_SetBUSMASTERVersionInfo");
}
void TSExecutorHandler::vShowTSExecutorWindow(void* pParentWnd)
{
    if(pfShowTSExecutorwindow != NULL)
    {
        pfShowTSExecutorwindow(pParentWnd);
        pfTSBusConnected(m_bConnected);
    }
}
void TSExecutorHandler::vGetConfigurationData(BYTE*& pDesBuffer, UINT& unBuffSize)
{
    if(pfShowTSExecutorwindow != NULL)
    {
        pfTSExecutorGetConfigdata(pDesBuffer, unBuffSize);
    }
}

void TSExecutorHandler::vSetConfigurationData(BYTE*& pSrcBuffer, UINT& unBuffSize)
{
    if(pfTSExecutorSetConfigdata!= NULL)
    {
        pfTSExecutorSetConfigdata(pSrcBuffer, unBuffSize);
    }
}
void TSExecutorHandler::vStartStopReadThread(ETYPE_BUS eBus, BOOL bStart)
{
    if(pfShowTSExecutorwindow != NULL)
    {
        pfTSStartStopReadThread(eBus, bStart);
    }
}
void TSExecutorHandler::vDoInitailization(ETYPE_BUS eBus)
{
    if(pfTSDoInitialization != NULL)
    {
        pfTSDoInitialization(eBus);
    }
}
void TSExecutorHandler::vBusConnected(BOOL bConnected)
{
    if(pfTSBusConnected != NULL)
    {
        m_bConnected = bConnected;
        pfTSBusConnected(m_bConnected);
    }
}
void TSExecutorHandler::vSetTSEVersionInfo(BYTE bytMajor, BYTE bytMinor, BYTE bytBuild)
{
    if(pfTSSetVersion != NULL)
    {
        pfTSSetVersion(bytMajor, bytMinor, bytBuild);
    }
}