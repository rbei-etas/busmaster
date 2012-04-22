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
 * \file      TXWindow.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
// TXWindow.cpp : Defines the initialization routines for the DLL.
//

#include "TxWindow_stdafx.h"
#include <afxdllx.h>

#define USAGE_EXPORT
#include "TxWnd_Extern.h"
#include "TxMsgManager.h"
#include "TxMsgChildFrame.h"

static AFX_EXTENSION_MODULE TXWindowDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("TXWindow.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(TXWindowDLL, hInstance))
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

		new CDynLinkLibrary(TXWindowDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
        //Clear any memory allocated
        CTxMsgManager::s_bDeleteTxMsgManager();
		TRACE0("TXWindow.DLL Terminating!\n");

		// Terminate the library before destructors are called
		AfxTermExtensionModule(TXWindowDLL);
	}
	return 1;   // ok
}

//CTxMsgManager g_txMsgManager;
CTxMsgChildFrame* g_pomTxMsgChildWindow = NULL;

//Export Function Definitions.

USAGEMODE HRESULT TX_vSetMsgDBPtrInDetailsView(void* pMsgDB)
{
	//Place this code at the beginning of the export function.
	//Save previous resource handle and switch to current one.
	HINSTANCE hInst = AfxGetResourceHandle();
	AfxSetResourceHandle(TXWindowDLL.hResource);

	if(g_pomTxMsgChildWindow)
		g_pomTxMsgChildWindow->vSetMsgDBPtrInDetailsView(pMsgDB);

	//Place this at the end of the export function.
	//switch back to previous resource handle.
	AfxSetResourceHandle(hInst); 
	return S_OK;
}

USAGEMODE HRESULT TX_vShowConfigureMsgWindow(void* pParentWnd)
{	
	//Place this code at the beginning of the export function.
	//Save previous resource handle and switch to current one.
	HINSTANCE hInst = AfxGetResourceHandle();
	AfxSetResourceHandle(TXWindowDLL.hResource);

	if( g_pomTxMsgChildWindow == NULL )
    {
        // Create New Instance
        g_pomTxMsgChildWindow = new CTxMsgChildFrame;
        if( g_pomTxMsgChildWindow != NULL )
        {
            //// Register Window Class
            LPCTSTR strMDIClass = AfxRegisterWndClass(
                                CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
                                LoadCursor(NULL, IDC_CROSS), 0,
                                ::LoadIcon(NULL, MAKEINTRESOURCE(IDI_ICO_SEND_MSG)) );
			
            //// Set the size got from configuration module
            WINDOWPLACEMENT sTxWndPlacement;
            WINDOWPLACEMENT* psTxWndPlacement = &sTxWndPlacement;
            CTxWndDataStore::ouGetTxWndDataStoreObj().bGetTxData(TX_WINDOW_PLACEMENT, (void**)&psTxWndPlacement);
            if (sTxWndPlacement.rcNormalPosition.top == -1 ||
                sTxWndPlacement.length == 0)//Load default configuration
            {
                CRect omRect;
                CWnd* pWnd = (CWnd*)pParentWnd;
                pWnd->GetClientRect(&omRect);
                omRect.NormalizeRect();
                // Reduce the size propotionally
                omRect.bottom -= (LONG)(omRect.Height() * defTX_MSG_WND_BOTTOM_MARGIN);
                omRect.right -= (LONG)(omRect.Width() * defTX_MSG_WND_RIGHT_MARGIN);
                // Update the size
                sTxWndPlacement.rcNormalPosition = omRect;
            }
            CRect omRect(&(sTxWndPlacement.rcNormalPosition));

            // Create Tx Message Configuration window
            if( g_pomTxMsgChildWindow->Create( strMDIClass,
                                          defSTR_TX_WINDOW_TITLE,
                                          WS_CHILD | WS_OVERLAPPEDWINDOW,
                                          omRect, (CMDIFrameWnd*)pParentWnd ) == TRUE )
            {
                // Show window and set focus
                g_pomTxMsgChildWindow->ShowWindow( SW_SHOW);
                g_pomTxMsgChildWindow->SetFocus();
                g_pomTxMsgChildWindow->SetWindowPlacement(&sTxWndPlacement);
                g_pomTxMsgChildWindow->ShowWindow( SW_SHOW);
            }
        }
		else 
			return S_FALSE;
    }
	// If already exist then activate and set the focus
    else
    {
        g_pomTxMsgChildWindow->ShowWindow( SW_RESTORE );
        g_pomTxMsgChildWindow->MDIActivate();
        g_pomTxMsgChildWindow->SetActiveWindow();
    }
	//Place this at the end of the export function.
	//switch back to previous resource handle.
	AfxSetResourceHandle(hInst); 
	return S_OK;
}

USAGEMODE HRESULT TX_vSetClientID(DWORD dwClientID)
{
    CTxMsgManager::s_podGetTxMsgManager()->vSetClientID(dwClientID);
	//g_txMsgManager.vSetClientID(dwClientID);
	return S_OK;
}
USAGEMODE HRESULT TX_vSetDILInterfacePtr(void* ptrDILIntrf)
{
	CTxMsgManager::s_podGetTxMsgManager()->vSetDILInterfacePtr(ptrDILIntrf);
	return S_OK;
}

USAGEMODE HRESULT TX_vPostMessageToTxWnd(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(g_pomTxMsgChildWindow)
		g_pomTxMsgChildWindow->PostMessage(msg, wParam, lParam);
	return S_OK;
}

USAGEMODE HRESULT TX_hConfigWindowShown()
{
	if(g_pomTxMsgChildWindow)
		return S_OK;
	else
		return S_FALSE;
}

USAGEMODE HRESULT TX_vStartTransmission(UCHAR ucKeyVal)
{
	CTxMsgManager::s_podGetTxMsgManager()->vStartTransmission(ucKeyVal);
	return S_OK;
}

USAGEMODE HRESULT TX_bAllocateMemoryForGlobalTxList()
{
	if(CTxMsgManager::s_podGetTxMsgManager()->bAllocateMemoryForGlobalTxList())
		return S_OK;
	else
		return S_FALSE;
}

USAGEMODE HRESULT TX_vAssignMsgBlockList()
{
	CTxMsgManager::s_podGetTxMsgManager()->vAssignMsgBlockList();
	return S_OK;
}

USAGEMODE HRESULT TX_vDeleteTxBlockMemory()
{
	CTxMsgManager::s_podGetTxMsgManager()->vDeleteTxBlockMemory();
	return S_OK;
}

USAGEMODE HRESULT TX_vStopTransmission(UINT unMaxWaitTime)
{
	CTxMsgManager::s_podGetTxMsgManager()->vStopTransmission(unMaxWaitTime);
	return S_OK;
}

USAGEMODE HRESULT TX_vGetTxWndConfigData(BYTE*& pDesBuffer, int& nBuffSize)
{
	if(g_pomTxMsgChildWindow)
		g_pomTxMsgChildWindow->vUpdateWndCo_Ords();
	CTxMsgManager::s_podGetTxMsgManager()->vGetTxWndConfigData(pDesBuffer, nBuffSize);
	return S_OK;
}

USAGEMODE HRESULT TX_vSetTxWndConfigData(BYTE* pSrcBuffer, int nBuffSize)
{
	CTxMsgManager::s_podGetTxMsgManager()->vSetTxWndConfigData(pSrcBuffer, nBuffSize);
	return S_OK;
}

/*USAGEMODE HRESULT TX_bIsTxWndConfigChanged()
{
	if(CTxMsgManager::s_podGetTxMsgManager()->bIsTxWndConfigChanged())
		return S_OK;
	else
		return S_FALSE;
}*/

USAGEMODE HRESULT TX_vSetTxStopFlag(BOOL bStartStop)
{
	CTxMsgManager::s_podGetTxMsgManager()->vSetTxStopFlag(bStartStop);
	return S_OK;
}

USAGEMODE BOOL TX_bGetTxStopFlag()
{
    return CTxMsgManager::s_podGetTxMsgManager()->bGetTxStopFlag();
}

USAGEMODE UINT TX_unGetTxBlockCount()
{
    return CTxMsgManager::s_podGetTxMsgManager()->nGetBlockCount();
}