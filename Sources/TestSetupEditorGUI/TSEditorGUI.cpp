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
 * \file      TSEditorGUI.cpp
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "TSEditorGUI_stdafx.h"
#include <afxdllx.h>
#define USAGE_EXPORT

#include "TSEditorGUI_Extern.h"
#include "TSEditorGUI_ChildFrame.h"
#include "TSEditorGUI_resource.h"

BYTE* m_pbyTEConfigData = NULL;
UINT m_unTEConfigSize = 0;

static AFX_EXTENSION_MODULE TestSetupEditor = { NULL, NULL };
CTSEditorChildFrame *g_pomTSEditorChildWindow = NULL;
extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("TestSetupEditor.DLL Initializing!\n");
		m_hDllInstance = hInstance;
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(TestSetupEditor, hInstance))
			return 0;

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

		new CDynLinkLibrary(TestSetupEditor);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("TestSetupEditor.DLL Terminating!\n");
        if(m_pbyTEConfigData != NULL)
        {
            delete[] m_pbyTEConfigData;
        }
        // Terminate the library before destructors are called
		AfxTermExtensionModule(TestSetupEditor);
	}
	return 1;   // ok
}


USAGEMODE HRESULT TS_vShowTSEditorWindow(void* pParentWnd)
{	
	//Place this code at the beginning of the export function.
	//Save previous resource handle and switch to current one.
	HINSTANCE hInst = AfxGetResourceHandle();
	AfxSetResourceHandle(TestSetupEditor.hResource);

	if( g_pomTSEditorChildWindow == NULL )
    {
        // Create New Instance
        g_pomTSEditorChildWindow = new CTSEditorChildFrame;
        if( g_pomTSEditorChildWindow != NULL )
        {
            //// Register Window Class
            //TODO::ICON
            LPCTSTR strMDIClass = AfxRegisterWndClass(
                                CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
                                LoadCursor(NULL, IDC_CROSS), 0,
                                NULL );			           
            //TODO::Update position.
            //CRect omRect(&(sTxWndPlacement.rcNormalPosition));
            CRect omRect(63, 913, 4, 596);
            // Create Tx Message Configuration window
            //TODO::defSTR_TX_WINDOW_TITLE
            if( g_pomTSEditorChildWindow->Create( strMDIClass,
                                          _T("Test Setup Editor"),
                                          WS_CHILD | WS_OVERLAPPEDWINDOW|WS_THICKFRAME,
                                          omRect, ((CMDIFrameWnd*)pParentWnd)) == TRUE )
            {
                g_pomTSEditorChildWindow->SetConfigurationData(m_pbyTEConfigData, m_unTEConfigSize);
                g_pomTSEditorChildWindow->ShowWindow(SW_SHOW);
                g_pomTSEditorChildWindow->SetFocus();
                
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
        g_pomTSEditorChildWindow->ShowWindow( SW_RESTORE );
        g_pomTSEditorChildWindow->MDIActivate();
        g_pomTSEditorChildWindow->SetActiveWindow();
    }
	//Place this at the end of the export function.
	//switch back to previous resource handle.
	AfxSetResourceHandle(hInst); 
	return S_OK;
}

USAGEMODE HWND hGetHwnd()
{
    if(g_pomTSEditorChildWindow != NULL)
    {
        return g_pomTSEditorChildWindow->GetSafeHwnd();
    }
    else
    {
        return NULL;
    }
}
USAGEMODE HRESULT TS_vSetDILInterfacePtr(void* /*ptrDILIntrf*/)
{
    //TODO::
    //CTxMsgManager::s_podGetTxMsgManager()->vSetDILInterfacePtr(ptrDILIntrf);
	return S_OK;
}

USAGEMODE HRESULT TS_vPostMessageToTSWnd(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(g_pomTSEditorChildWindow)
		g_pomTSEditorChildWindow->PostMessage(msg, wParam, lParam);
	return S_OK;
}

USAGEMODE HRESULT TS_hTSEditorWindowShown()
{
	if(g_pomTSEditorChildWindow)
		return S_OK;
	else
		return S_FALSE;
}
USAGEMODE HRESULT TS_hLoadTestSetupFile(CString omFilePath)
{
    g_pomTSEditorChildWindow->vLoadTestSetupFile(omFilePath);
    return S_OK;
}
USAGEMODE HRESULT TSE_hGetConfigurationData(BYTE*& pDesBuffer, UINT& nBuffSize)
{
    if(g_pomTSEditorChildWindow != NULL)
    {
        return g_pomTSEditorChildWindow->GetConfigurationData(pDesBuffer, nBuffSize);
    }
    else
    {
		nBuffSize = m_unTEConfigSize;
		pDesBuffer = new BYTE[m_unTEConfigSize];		
		memcpy(pDesBuffer, m_pbyTEConfigData, m_unTEConfigSize);		
        return S_FALSE;
    }
}
USAGEMODE HRESULT TSE_hSetConfigurationData(BYTE* pSrcBuffer, UINT unBuffSize)
{
    if(m_pbyTEConfigData != NULL)
    {
        delete []m_pbyTEConfigData;
        m_pbyTEConfigData = NULL;
    }
    m_unTEConfigSize = unBuffSize;
    m_pbyTEConfigData = new BYTE[m_unTEConfigSize];
    memcpy(m_pbyTEConfigData, pSrcBuffer, m_unTEConfigSize);
    if(g_pomTSEditorChildWindow != NULL)
    {
        g_pomTSEditorChildWindow->SetConfigurationData(m_pbyTEConfigData, m_unTEConfigSize);
    }
    return S_OK;
}