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
#pragma once
#include "TxWindowFlexRay_stdafx.h"
#include <afxdllx.h>
//#include "../Application/MultiLanguage.h"
//#include "../Application/GettextBusmaster.h"
#define USAGE_EXPORT
#include "TxWndFlexRay_Extern.h"
#include "TxMsgFlexChildFrame.h"



static AFX_EXTENSION_MODULE TXFlexRayWindowDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    static HINSTANCE shLangInst=NULL;


    // Remove this if you use lpReserved
    UNREFERENCED_PARAMETER(lpReserved);

    if (dwReason == DLL_PROCESS_ATTACH)
    {
        TRACE0("TXWindowFlexRay.DLL Initializing!\n");

        // Extension DLL one-time initialization
        if (!AfxInitExtensionModule(TXFlexRayWindowDLL, hInstance))
        {
            return 0;
        }
        new CDynLinkLibrary(TXFlexRayWindowDLL);

    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        if (shLangInst)
        {
            FreeLibrary(shLangInst);
        }

        //Clear any memory allocated
        //CTxMsgManager::s_bDeleteTxMsgManager();
        TRACE0("TXWindowFlexRay.DLL Terminating!\n");

        // Terminate the library before destructors are called
        AfxTermExtensionModule(TXFlexRayWindowDLL);
    }
    return 1;   // ok
}

CTxMsgFlexChildFrame* g_pomTxMsgFlexChildWindow = NULL;
//Export Function Definitions.

USAGEMODE HRESULT TXFlexRay_vSetMsgDBPtrInDetailsView(void* pMsgDB)
{
    //Place this code at the beginning of the export function.
    //Save previous resource handle and switch to current one.
    //HINSTANCE hInst = AfxGetResourceHandle();
    //AfxSetResourceHandle(TXWindowDLL.hResource);

    //if(g_pomTxMsgChildWindow)
    //{
    //    g_pomTxMsgChildWindow->vSetMsgDBPtrInDetailsView(pMsgDB);
    //}

    ////Place this at the end of the export function.
    ////switch back to previous resource handle.
    //AfxSetResourceHandle(hInst);
    return S_OK;
}

void SetDefaultWindowPos(WINDOWPLACEMENT& sTxWndPlacement)
{
    sTxWndPlacement.flags = 1;
    sTxWndPlacement.length = 44;
    sTxWndPlacement.ptMaxPosition.x = 0;
    sTxWndPlacement.ptMaxPosition.x = 0;
    sTxWndPlacement.ptMinPosition.x = 0;
    sTxWndPlacement.ptMinPosition.y = 0;
    sTxWndPlacement.rcNormalPosition.top = 1;
    sTxWndPlacement.rcNormalPosition.bottom = 661;
    sTxWndPlacement.rcNormalPosition.left = 4;
    sTxWndPlacement.rcNormalPosition.right = 864;
    sTxWndPlacement.showCmd = 1;
}

USAGEMODE HRESULT TXFlexRay_vShowConfigureMsgWindow(void* pParentWnd)
{
    //Place this code at the beginning of the export function.
    //Save previous resource handle and switch to current one.
    HINSTANCE hInst = AfxGetResourceHandle();
    AfxSetResourceHandle(TXFlexRayWindowDLL.hResource);
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());

    if( g_pomTxMsgFlexChildWindow == NULL )
    {
        // Create New Instance
        g_pomTxMsgFlexChildWindow = new CTxMsgFlexChildFrame;
        if( g_pomTxMsgFlexChildWindow != NULL )
        {
            //// Register Window Class
            LPCTSTR strMDIClass = AfxRegisterWndClass(
                                      CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
                                      LoadCursor(NULL, IDC_CROSS), 0,
                                      ::LoadIcon(NULL, MAKEINTRESOURCE(IDI_ICO_SEND_MSG)) );

            // Set the size got from configuration module
            WINDOWPLACEMENT sTxWndPlacement;
            SetDefaultWindowPos(sTxWndPlacement);
            WINDOWPLACEMENT* psTxWndPlacement = &sTxWndPlacement;
            CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().bGetTxData(TX_WINDOW_PLACEMENT, (void**)&psTxWndPlacement);
            if (sTxWndPlacement.rcNormalPosition.top == -1 ||
                    sTxWndPlacement.length == 0)//Load default configuration
            {
                CRect omRect;
                CWnd* pWnd = (CWnd*)pParentWnd;
                pWnd->GetClientRect(&omRect);
                omRect.NormalizeRect();
                //    // Reduce the size propotionally
                omRect.bottom -= (LONG)(omRect.Height() * defTX_MSG_WND_BOTTOM_MARGIN);
                omRect.right -= (LONG)(omRect.Width() * defTX_MSG_WND_RIGHT_MARGIN);
                //    // Update the size
                //sTxWndPlacement.rcNormalPosition = omRect;

                sTxWndPlacement.flags = 1;
                sTxWndPlacement.length = 44;
                sTxWndPlacement.ptMaxPosition.x = 0;
                sTxWndPlacement.ptMaxPosition.x = 0;
                sTxWndPlacement.ptMinPosition.x = 0;
                sTxWndPlacement.ptMinPosition.y = 0;
                sTxWndPlacement.rcNormalPosition.top = 1;
                sTxWndPlacement.rcNormalPosition.bottom = 661;
                sTxWndPlacement.rcNormalPosition.left = 4;
                sTxWndPlacement.rcNormalPosition.right = 864;
                sTxWndPlacement.showCmd = 1;
            }
            CRect omRect(&(sTxWndPlacement.rcNormalPosition));

            // Create Tx Message Configuration window
            //CRect omRect(0,0,200,200);
            if( g_pomTxMsgFlexChildWindow->Create( strMDIClass,
                                                   "Configure Transmission Messages - FLEXRAY",
                                                   WS_CHILD | WS_OVERLAPPEDWINDOW,
                                                   omRect, (CMDIFrameWnd*)pParentWnd ) == TRUE )
            {
                // Show window and set focus
                g_pomTxMsgFlexChildWindow->ShowWindow( SW_SHOW);
                g_pomTxMsgFlexChildWindow->SetFocus();
                if (sTxWndPlacement.rcNormalPosition.top != -1 &&
                        sTxWndPlacement.length != 0)
                {
                    g_pomTxMsgFlexChildWindow->SetWindowPlacement(&sTxWndPlacement);
                }

            }
        }
        else
        {
            return S_FALSE;
        }
    }
    // If already exist then activate and set the focus
    else
    {
        g_pomTxMsgFlexChildWindow->ShowWindow( SW_RESTORE );
        g_pomTxMsgFlexChildWindow->MDIActivate();
        g_pomTxMsgFlexChildWindow->SetActiveWindow();
    }
    //Place this at the end of the export function.
    //switch back to previous resource handle.
    //   CWnd objParent;
    //objParent.Attach(((CWnd*)pParentWnd)->m_hWnd);

    //objParent.Detach();

    AfxSetResourceHandle(hInst);
    return S_OK;
}


USAGEMODE HRESULT TXFlexRay_vSetClientID(DWORD dwClientID)
{
    // CTxMsgManager::s_podGetTxMsgManager()->vSetClientID(dwClientID);
    //g_txMsgManager.vSetClientID(dwClientID);
    return S_OK;
}
USAGEMODE HRESULT TXFlexRay_vSetDILInterfacePtr()
{
    CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().bSetDILInterfacePtr();
    // CTxMsgManager::s_podGetTxMsgManager()->vSetDILInterfacePtr(ptrDILIntrf);
    return S_OK;
}

USAGEMODE HRESULT TXFlexRay_vPostMessageToTxWnd(UINT msg, WPARAM wParam, LPARAM lParam)
{
    /*if(g_pomTxMsgChildWindow)
    {
        g_pomTxMsgChildWindow->PostMessage(msg, wParam, lParam);
    }*/
    if ( wParam == 0 )  //eHEXDECCMD = 0;
    {
        CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_bHexMode = (bool)lParam;
    }


    if(g_pomTxMsgFlexChildWindow != NULL)
    {
        g_pomTxMsgFlexChildWindow->m_pTransmitMsgView->SendMessage(msg, wParam, lParam);
    }
    return S_OK;
}

USAGEMODE HRESULT TXFlexRay_hConfigWindowShown()
{
    if(g_pomTxMsgFlexChildWindow)
    {
        return S_OK;
    }
    else
    {
        return S_FALSE;
    }
    return S_OK;
}

USAGEMODE HRESULT TXFlexRay_vStartTransmission(UCHAR ucKeyVal)
{

    return S_OK;
}

USAGEMODE HRESULT TXFlexRay_bAllocateMemoryForGlobalTxList()
{
    return S_OK;
}

USAGEMODE HRESULT TXFlexRay_vAssignMsgBlockList()
{
    //CTxMsgManager::s_podGetTxMsgManager()->vAssignMsgBlockList();
    return S_OK;
}

USAGEMODE HRESULT TXFlexRay_vDeleteTxBlockMemory()
{
    CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().vRemoveAllBlock(
        CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_omTxMsgBlockMan);
    // CTxMsgManager::s_podGetTxMsgManager()->vDeleteTxBlockMemory();
    return S_OK;
}

USAGEMODE HRESULT TXFlexRay_vBusStatusChanged(bool bConnected)
{
    //  CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().vStopTxTimer();
    //if(g_pomTxMsgFlexChildWindow != NULL)
    {
        CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().vSetBusStatus(bConnected);
    }
    if(g_pomTxMsgFlexChildWindow != NULL)
    {
        g_pomTxMsgFlexChildWindow->m_pTransmitMsgView->SendMessage(WM_USER+44, 1, bConnected);
    }
    //CTxMsgManager::s_podGetTxMsgManager()->vStopTransmission(unMaxWaitTime);
    return S_OK;
}

//USAGEMODE HRESULT TX_vGetTxWndConfigData(BYTE*& pDesBuffer, int& nBuffSize)
//{
//    if(g_pomTxMsgChildWindow)
//    {
//        g_pomTxMsgChildWindow->vUpdateWndCo_Ords();
//    }
//    CTxMsgManager::s_podGetTxMsgManager()->vGetTxWndConfigData(pDesBuffer, nBuffSize);
//    return S_OK;
//}
USAGEMODE HRESULT TXFlexRay_vGetTxWndConfigData(xmlNodePtr pxmlNodePtr)
{
    if(g_pomTxMsgFlexChildWindow)
    {
        g_pomTxMsgFlexChildWindow->vUpdateWndCo_Ords();
    }
    ////CTxMsgManager::s_podGetTxMsgManager()->vGetTxWndConfigData(pDesBuffer, nBuffSize);
    CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().bGetConfigData(pxmlNodePtr);
    return S_OK;
}


USAGEMODE HRESULT TXFlexRay_vSetTxWndConfigDataXML(xmlDocPtr pDoc)
{
    if(g_pomTxMsgFlexChildWindow != NULL)
    {
        //g_pomTxMsgFlexChildWindow->m_pTransmitMsgView->StartStopCyclicTx(FALSE, NULL);
    }
    CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().bSetConfigData(pDoc);
    // CTxMsgManager::s_podGetTxMsgManager()->vSetTxWndConfigData(pDoc);
    return S_OK;
}
/*USAGEMODE HRESULT TX_bIsTxWndConfigChanged()
{
    if(CTxMsgManager::s_podGetTxMsgManager()->bIsTxWndConfigChanged())
        return S_OK;
    else
        return S_FALSE;
}*/

USAGEMODE HRESULT TXFlexRay_vSetTxStopFlag(BOOL bStartStop)
{
    //CTxMsgManager::s_podGetTxMsgManager()->vSetTxStopFlag(bStartStop);
    return S_OK;
}

USAGEMODE BOOL TXFlexRay_bGetTxStopFlag()
{
    //return CTxMsgManager::s_podGetTxMsgManager()->bGetTxStopFlag();
    return S_OK;
}

USAGEMODE UINT TXFlexRay_unGetTxBlockCount()
{

    return CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().nGetBlockCount();
    //return CTxMsgManager::s_podGetTxMsgManager()->nGetBlockCount();
}

USAGEMODE HRESULT TXFlexRay_vFlexFileChanged()
{
    CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().vRemoveAllBlock(
        CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_omTxMsgBlockMan);
    if(g_pomTxMsgFlexChildWindow != NULL)
    {
        //g_pomTxMsgFlexChildWindow->m_pTransmitMsgView->vClearInitialisationFileContents();
    }

    return S_OK;
}

USAGEMODE HRESULT TXFlexray_nSetFibexConfig(FlexConfig& ouFlexConfig)
{
    CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().vRemoveAllBlock(
        CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_omTxMsgBlockMan);
    HRESULT hr = CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().SetFlexRayConfig(ouFlexConfig);
    if(g_pomTxMsgFlexChildWindow != NULL && hr == S_OK)
    {
        g_pomTxMsgFlexChildWindow->m_pTransmitMsgView->UpdateTxView(ouFlexConfig.m_nChannelsConfigured);
    }
    return S_FALSE;
}