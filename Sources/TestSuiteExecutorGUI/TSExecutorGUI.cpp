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
 * \file      TSExecutorGUI.cpp
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#pragma once
#include "TSExecutorGUI_stdafx.h"
#define USAGE_EXPORT
#include "TSExecutorGUI_ChildFrame.h"
#include "TSExecutorGUI_Extern.h"
#include "TSExecutionCAN.h"
#include "../Application/MultiLanguage.h"
#include "Utility\MultiLanguageSupport.h"

#include <afxdllx.h>
static AFX_EXTENSION_MODULE TestSuiteExecutor = { false, nullptr };


CTSExecutorChildFrame* g_pomTSExecutorChildWindow = nullptr;
CTSExecutorBase* g_podTSExecutor = nullptr;
BYTE* m_pbyConfigData = nullptr;
UINT m_unConfigSize = 0;

xmlNodePtr m_pXmlConfigNode = nullptr;
BOOL m_bByXmlConfig = TRUE;
extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{

    static HINSTANCE shLangInst=nullptr;


    // Remove this if you use lpReserved
    UNREFERENCED_PARAMETER(lpReserved);

    if (dwReason == DLL_PROCESS_ATTACH)
    {
        TRACE0("TestSuiteexecutor.DLL Initializing!\n");

        // Extension DLL one-time initialization
        if (!AfxInitExtensionModule(TestSuiteExecutor, hInstance))
        {
            return 0;
        }

        // Insert this DLL into the resource chain
        // NOTE: If this Extension DLL is being implicitly linked to by
        //  an MFC Regular DLL (such as an ActiveX Control)
        //  instead of an MFC application, then you will want to
        //  remove this line from DllMain and put it in a separate
        //  function exported from this Extension DLL.  The Regular DLL
        //  that uses this Extension DLL should then explicitly call that
        //  function to initialize this Extension DLL.  Otherwise,
        //  the CDynLinkLibrary object will not be attached to the
        //  Regular DLL's resource chain, and serious problems will
        //  result.

        // Begin of Multiple Language support
        if ( CMultiLanguage::m_nLocales <= 0 )  // Not detected yet
        {
            CMultiLanguage::DetectLangID();     // Detect language as user locale
            CMultiLanguage::DetectUILanguage(); // Detect language in MUI OS
        }
        TCHAR szModuleFileName[MAX_PATH];       // Get Module File Name and path
        int ret = ::GetModuleFileName(hInstance, szModuleFileName, MAX_PATH);
        if ( ret == 0 || ret == MAX_PATH )
        {
            ASSERT(false);
        }
        // Load resource-only language DLL. It will use the languages
        // detected above, take first available language,
        // or you can specify another language as second parameter to
        // LoadLangResourceDLL. And try that first.
        shLangInst = CMultiLanguage::LoadLangResourceDLL( szModuleFileName );
        if (shLangInst)
        {
            TestSuiteExecutor.hResource = shLangInst;
        }
        // End of Multiple Language support

        new CDynLinkLibrary(TestSuiteExecutor);

    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        if (shLangInst)
        {
            FreeLibrary(shLangInst);
        }

        TRACE0("TestSuiteExecutor.DLL Terminating!\n");
        if(m_pbyConfigData != nullptr)
        {
            delete[] m_pbyConfigData;
        }
        if( nullptr != m_pXmlConfigNode )
        {
            xmlFreeNode(m_pXmlConfigNode);
        }
        // Terminate the library before destructors are called
        AfxTermExtensionModule(TestSuiteExecutor);
    }
    return 1;   // ok
}


USAGEMODE HRESULT TS_vShowTSExecutorWindow(void* pParentWnd)
{
    //Place this code at the beginning of the export function.
    //Save previous resource handle and switch to current one.
    HINSTANCE hInst = AfxGetResourceHandle();
    AfxSetResourceHandle(TestSuiteExecutor.hResource);

    if( g_pomTSExecutorChildWindow == nullptr )
    {
        // Create New Instance
        g_pomTSExecutorChildWindow = new CTSExecutorChildFrame;
        if( g_pomTSExecutorChildWindow != nullptr )
        {
            //// Register Window Class
            //TODO::ICON
            LPCTSTR strMDIClass = AfxRegisterWndClass(
                                      CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
                                      LoadCursor(nullptr, IDC_CROSS), 0,
                                      nullptr );

            CRect omRect(63, 913, 4, 596);
            if( g_pomTSExecutorChildWindow->Create( strMDIClass,
                                                    _("Test Suite Executor"),
                                                    WS_CHILD | WS_OVERLAPPEDWINDOW|WS_THICKFRAME,
                                                    omRect, ((CMDIFrameWnd*)pParentWnd)) == TRUE )
            {
                if( TRUE == m_bByXmlConfig )
                {
                    g_pomTSExecutorChildWindow->SetConfigurationData(m_pXmlConfigNode);
                }
                else
                {
                    g_pomTSExecutorChildWindow->SetConfigurationData(m_pbyConfigData, m_unConfigSize);
                }
                // Show window and set focus
                g_pomTSExecutorChildWindow->ShowWindow( SW_SHOW);
                g_pomTSExecutorChildWindow->SetFocus();
            }
        }
        else
        {
            //Place this at the end of the export function.
            //switch back to previous resource handle.
            AfxSetResourceHandle(hInst);
            return S_FALSE;
        }
    }
    // If already exist then activate and set the focus
    else
    {
        g_pomTSExecutorChildWindow->ShowWindow( SW_RESTORE );
        g_pomTSExecutorChildWindow->MDIActivate();
        g_pomTSExecutorChildWindow->SetActiveWindow();
    }
    //Place this at the end of the export function.
    //switch back to previous resource handle.
    AfxSetResourceHandle(hInst);
    return S_OK;
}


USAGEMODE HRESULT TS_vSetDILInterfacePtr(void* /*ptrDILIntrf*/)
{
    return S_OK;
}

USAGEMODE HRESULT TS_vPostMessageToTSWnd(UINT msg, WPARAM wParam, LPARAM lParam)
{
    if(g_pomTSExecutorChildWindow)
    {
        g_pomTSExecutorChildWindow->PostMessage(msg, wParam, lParam);
    }

    return S_OK;
}

USAGEMODE HRESULT TS_hTSEexecutorWindowShown()
{
    if(g_pomTSExecutorChildWindow)
    {
        return S_OK;
    }
    else
    {
        return S_FALSE;
    }
}
//USAGEMODE HRESULT TS_hGetConfigurationData(BYTE*& pDesBuffer, UINT& nBuffSize)
USAGEMODE HRESULT TS_hGetConfigurationData(xmlNodePtr* pxmlNodePtr)
{
    if(g_pomTSExecutorChildWindow != nullptr)
    {
        return g_pomTSExecutorChildWindow->GetConfigurationData(*pxmlNodePtr);
    }
    else
    {
        *pxmlNodePtr = xmlCopyNode(m_pXmlConfigNode, 1);
        return S_OK;
    }
}
USAGEMODE HRESULT TS_hSetConfigurationData(BYTE* pSrcBuffer, UINT unBuffSize)
{
    if(m_pbyConfigData != nullptr)
    {
        delete []m_pbyConfigData;
        m_pbyConfigData = nullptr;
    }
    m_bByXmlConfig = FALSE;
    m_unConfigSize = unBuffSize;
    m_pbyConfigData = new BYTE[m_unConfigSize];
    memcpy(m_pbyConfigData, pSrcBuffer, m_unConfigSize);
    if(g_pomTSExecutorChildWindow != nullptr)
    {
        g_pomTSExecutorChildWindow->SetConfigurationData(m_pbyConfigData, m_unConfigSize);
    }
    return S_OK;
}

USAGEMODE HRESULT TS_hSetXMLConfigurationData(xmlDocPtr pDoc)
{
    m_bByXmlConfig = TRUE;
    if ( nullptr != m_pXmlConfigNode )
    {
        xmlFreeNode(m_pXmlConfigNode);
        m_pXmlConfigNode = nullptr;
    }
    xmlXPathObjectPtr pTempPathNode = xmlUtils::pGetNodes(pDoc, (xmlChar*)"//BUSMASTER_CONFIGURATION/Module_Configuration/CAN_TS_Executor");
    if( nullptr == pTempPathNode )
    {
        return S_FALSE;
    }

    m_pXmlConfigNode = xmlCopyNode(pTempPathNode->nodesetval->nodeTab[0], 1);

    if(g_pomTSExecutorChildWindow != nullptr)
    {
        g_pomTSExecutorChildWindow->SetConfigurationData(m_pXmlConfigNode);
    }


    return S_OK;
}
USAGEMODE HRESULT TS_DoInitialization(ETYPE_BUS /*eBus*/)
{
    //Place this code at the beginning of the export function.
    //Save previous resource handle and switch to current one.
    HINSTANCE hInst = AfxGetResourceHandle();
    AfxSetResourceHandle(TestSuiteExecutor.hResource);

    if(g_podTSExecutor == nullptr)
    {
        g_podTSExecutor = new CTSExecutionCAN();
    }
    //Place this at the end of the export function.
    //switch back to previous resource handle.
    AfxSetResourceHandle(hInst);
    return g_podTSExecutor->TSX_DoInitialization();
}
USAGEMODE HRESULT TS_StartStopReadThread(ETYPE_BUS /*eBus*/, BOOL bStart)
{
    if(g_podTSExecutor != nullptr)
    {
        return g_podTSExecutor->TSX_bStartStopReadThread(bStart);
    }
    else
    {
        return S_FALSE;
    }
}

USAGEMODE HRESULT TS_BUSConnected(bool bConnected)
{
    if(g_pomTSExecutorChildWindow != nullptr)
    {
        g_pomTSExecutorChildWindow->vSetBusStatus(bConnected);
        return S_OK;
    }
    else
    {
        return S_FALSE;
    }
}

USAGEMODE HRESULT TS_SetBUSMASTERVersionInfo(const BYTE bytMajor, const BYTE bytMinor, const BYTE bytBuild)
{
    CString strBUSMASTERVersion;
    strBUSMASTERVersion.Format("%d.%d.%d",bytMajor, bytMinor, bytBuild);
    g_pomTSExecutorChildWindow->vSetBUSMASTERVersionInfo(strBUSMASTERVersion);
    return S_OK;
}