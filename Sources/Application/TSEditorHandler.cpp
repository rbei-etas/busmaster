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
 * \file      TSEditorHandler.cpp
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "stdafx.h"
#include ".\tseditorhandler.h"

typedef HRESULT (*SHOWTSEDITORWINDOW)(void* pParentWnd);
typedef HRESULT (*SETTSDILINTERFACEPTR)(void* ptrDILIntrf);
typedef HRESULT (*POSTMESSAGETOTSWND)(UINT msg, WPARAM wParam, LPARAM lParam);
typedef HRESULT (*TSEDITORWINDOWSHOWN)();
typedef HRESULT (*TSEDITORLOADTESTSETUPFILE)(CString omFilePath);
typedef HWND (*TSEDITORHWND)();
typedef HRESULT (*TSEDITORGETCONFIGDATA)(BYTE*& pDesBuffer, UINT& nBuffSize);
typedef HRESULT (*TSEDITORSETCONFIGDATA)(BYTE* pSrcBuffer, UINT nBuffSize);


SHOWTSEDITORWINDOW  pfShowTSEditorwindow;
SETTSDILINTERFACEPTR  pfSetTSDILInterfacePtr;
POSTMESSAGETOTSWND  pfPostMessageToTSWnd;
TSEDITORWINDOWSHOWN pfTSEditorWindowShown;
TSEDITORLOADTESTSETUPFILE pfTSEditorLoadTestSetupFile;
TSEDITORHWND pfTSEditorHwnd;
TSEDITORGETCONFIGDATA pfTSEditorGetConfigdata;
TSEDITORSETCONFIGDATA pfTSEditorSetConfigdata;


TSEditorHandler::TSEditorHandler(void)
{
    m_hTSEditorHandle = NULL;
}

TSEditorHandler::~TSEditorHandler(void)
{
    if ( m_hTSEditorHandle != NULL )
    {
        FreeLibrary(m_hTSEditorHandle);
    }
}
void TSEditorHandler::vLoadTSEditor_DLL()
{
    if ( m_hTSEditorHandle != NULL )
    {
        FreeLibrary(m_hTSEditorHandle);
        m_hTSEditorHandle = NULL;
    }

    m_hTSEditorHandle = LoadLibrary(def_STR_TESTSETUPEDITORDLL);
    vloadFuncPtrAddress();
}
void TSEditorHandler::vInitializeFuncPtrs()
{
    pfShowTSEditorwindow = NULL;
    pfSetTSDILInterfacePtr = NULL;
    pfPostMessageToTSWnd = NULL;
    pfTSEditorWindowShown = NULL;
    pfTSEditorLoadTestSetupFile = NULL;
    pfTSEditorHwnd = NULL;
    pfTSEditorGetConfigdata = NULL;
    pfTSEditorSetConfigdata = NULL;
}
void TSEditorHandler::vloadFuncPtrAddress()
{
    pfShowTSEditorwindow = (SHOWTSEDITORWINDOW)GetProcAddress(m_hTSEditorHandle, "TS_vShowTSEditorWindow");
    pfSetTSDILInterfacePtr = (SETTSDILINTERFACEPTR)GetProcAddress(m_hTSEditorHandle, "TS_vSetDILInterfacePtr");
    pfPostMessageToTSWnd = (POSTMESSAGETOTSWND)GetProcAddress(m_hTSEditorHandle, "TS_vPostMessageToTSWnd");
    pfTSEditorWindowShown = (TSEDITORWINDOWSHOWN)GetProcAddress(m_hTSEditorHandle, "TS_hTSEditorWindowShown");
    pfTSEditorLoadTestSetupFile = (TSEDITORLOADTESTSETUPFILE)GetProcAddress(m_hTSEditorHandle, "TS_hLoadTestSetupFile");
    pfTSEditorHwnd = (TSEDITORHWND)GetProcAddress(m_hTSEditorHandle, "hGetHwnd");
    pfTSEditorGetConfigdata = (TSEDITORGETCONFIGDATA)GetProcAddress(m_hTSEditorHandle, "TSE_hGetConfigurationData");
    pfTSEditorSetConfigdata = (TSEDITORSETCONFIGDATA)GetProcAddress(m_hTSEditorHandle, "TSE_hSetConfigurationData");
}
void TSEditorHandler::vShowTSEditorWindow(void* pParentWnd)
{
    if(pfShowTSEditorwindow != NULL)
    {
        pfShowTSEditorwindow(pParentWnd);
    }
}
void TSEditorHandler::vLoadTestSetupFile(CString omFilePath)
{
    if(pfTSEditorLoadTestSetupFile != NULL)
    {
        pfTSEditorLoadTestSetupFile(omFilePath);
    }
}
HWND TSEditorHandler::vGetSafeHWnd()
{
    HWND hWnd;

    if(pfTSEditorHwnd != NULL)
    {
        hWnd = pfTSEditorHwnd();
        return hWnd;
    }
    else
    {
        return NULL;
    }
}
void TSEditorHandler::vGetConfigurationData(BYTE*& pDesBuffer, UINT& unBuffSize)
{
    if(pfTSEditorLoadTestSetupFile != NULL)
    {
        pfTSEditorGetConfigdata(pDesBuffer, unBuffSize);
    }
}

void TSEditorHandler::vSetConfigurationData(BYTE*& pSrcBuffer, UINT& unBuffSize)
{
    if(pfTSEditorSetConfigdata!= NULL)
    {
        pfTSEditorSetConfigdata(pSrcBuffer, unBuffSize);
    }
}