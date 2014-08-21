// This is the main DLL file.

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
#include "TxWindowEthernet_stdafx.h"
#include <afxdllx.h>
//#include "../Application/MultiLanguage.h"
//#include "../Application/GettextBusmaster.h"
#define USAGE_EXPORT
#include "TxWindowEthernet_Extern.h"
#include "TxMsgChildFrame.h"

CTxMsgChildFrame* g_pomTxMsgEthernetChildWindow = NULL;

static AFX_EXTENSION_MODULE TXEthernetWindowDLL = { NULL, NULL };

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
        if (!AfxInitExtensionModule(TXEthernetWindowDLL, hInstance))
        {
            return 0;
        }
        new CDynLinkLibrary(TXEthernetWindowDLL);

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
        AfxTermExtensionModule(TXEthernetWindowDLL);
    }
    return 1;   // ok
}


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
    //sTxWndPlacement.length = 44;
    //sTxWndPlacement.ptMaxPosition.x = 0;
    //sTxWndPlacement.ptMaxPosition.x = 0;
    //sTxWndPlacement.ptMinPosition.x = 0;
    //sTxWndPlacement.ptMinPosition.y = 0;
    //sTxWndPlacement.rcNormalPosition.top = 3;
    //sTxWndPlacement.rcNormalPosition.bottom = 612;
    //sTxWndPlacement.rcNormalPosition.left = 11;
    //sTxWndPlacement.rcNormalPosition.right = 883;
    sTxWndPlacement.showCmd = 1;
}
int nShowTxWindow( void* pParentWnd, ETYPE_BUS eBUS )
{
    CTxMsgChildFrame* pFrameWindow = NULL;
    string strTitle = "";       //Used for Title.
    bool bValidBus = false;

        pFrameWindow = g_pomTxMsgEthernetChildWindow;
        strTitle = "Configure Transmission Messages - ETHERNET";
        bValidBus = true;

    if ( bValidBus == false )
    {
        return S_FALSE;
    }


    if( pFrameWindow == NULL )
    {
        pFrameWindow = new CTxMsgChildFrame(eBUS);
        if( pFrameWindow != NULL )
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
            bool bRetVal = false;

			bRetVal = CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().bGetTxData(TX_WINDOW_PLACEMENT, (void**)&psTxWndPlacement);



			if ( bRetVal == false)//Load default configuration
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
            if( pFrameWindow->Create( strMDIClass,
                                      strTitle.c_str(),
                                      WS_CHILD | WS_OVERLAPPEDWINDOW,
                                      omRect, (CMDIFrameWnd*)pParentWnd ) == TRUE )
            {
                // Show window and set focus
                pFrameWindow->ShowWindow( SW_SHOW);
                pFrameWindow->SetFocus();
                if (sTxWndPlacement.rcNormalPosition.top != -1 &&
                        sTxWndPlacement.length != 0)
                {
                    pFrameWindow->SetWindowPlacement(&sTxWndPlacement);
                }


            }

            g_pomTxMsgEthernetChildWindow = pFrameWindow;
            
        }
        else
        {
            return S_FALSE;
        }
    }
    else
    {
        pFrameWindow->ShowWindow( SW_RESTORE );
        pFrameWindow->MDIActivate();
        pFrameWindow->SetActiveWindow();
    }

}

USAGEMODE HRESULT TXEthernet_vShowConfigureMsgWindow(void* pParentWnd, ETYPE_BUS eBUS)
{
    //Place this code at the beginning of the export function.
    //Save previous resource handle and switch to current one.
    HINSTANCE hInst = AfxGetResourceHandle();
    AfxSetResourceHandle(TXEthernetWindowDLL.hResource);

    int nRes = nShowTxWindow(pParentWnd, eBUS);

    AfxSetResourceHandle(hInst);
    return nRes;
}


USAGEMODE HRESULT TXComman_vSetClientID(ETYPE_BUS eBusType, DWORD dwClientID)
{
    CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().m_dwClientID = dwClientID;
    
    return S_OK;
}
USAGEMODE HRESULT TXComman_vSetDILInterfacePtr(ETYPE_BUS eBusType, void* pDilPointer)
{

	CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().bSetDILInterfacePtr((CBaseDIL_ETHERNET *)pDilPointer);

	return S_OK;
}

USAGEMODE HRESULT TXComman_vPostMessageToTxWnd(UINT msg, WPARAM wParam, LPARAM lParam)
{
    /*if(g_pomTxMsgChildWindow)
    {
        g_pomTxMsgChildWindow->PostMessage(msg, wParam, lParam);
    }*/
    if ( wParam == 0 )  //eHEXDECCMD = 0;
    {
        CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().m_bHexMode = (bool)lParam;
    }


    if(g_pomTxMsgEthernetChildWindow != NULL)
    {
        g_pomTxMsgEthernetChildWindow->m_pEthernetTransmitMsgView->SendMessage(msg, wParam, lParam);
    }


    CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().nHandleKeyEvent((char)lParam);

    return S_OK;
}

USAGEMODE HRESULT TXComman_hConfigWindowShown(ETYPE_BUS eBusType)
{
    if ( eBusType == FLEXRAY )
    {
        if(g_pomTxMsgEthernetChildWindow)
        {
            return S_OK;
        }
        else
        {
            return S_FALSE;
        }
    }
    return S_OK;
}

USAGEMODE HRESULT TXEthernet_vStartTransmission(UCHAR ucKeyVal)
{

    return S_OK;
}

USAGEMODE HRESULT TXEthernet_bAllocateMemoryForGlobalTxList()
{
    return S_OK;
}

USAGEMODE HRESULT TXEthernet_vAssignMsgBlockList()
{
    //CTxMsgManager::s_podGetTxMsgManager()->vAssignMsgBlockList();
    return S_OK;
}

USAGEMODE HRESULT TXEthernet_vDeleteTxBlockMemory()
{
    /* CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().vRemoveAllBlock(
         CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_omTxMsgBlockMan);*/
    // CTxMsgManager::s_podGetTxMsgManager()->vDeleteTxBlockMemory();
    return S_OK;
}

USAGEMODE HRESULT TX_vBusStatusChanged(ETYPE_BUS eBusType, ESTATUS_BUS eBusStatus)
{

        //  CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().vStopTxTimer();
        if(g_pomTxMsgEthernetChildWindow != NULL)
        {
            CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().vSetBusStatus(eBusStatus);
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
USAGEMODE HRESULT TXComman_vGetTxWndConfigData( ETYPE_BUS eBusType, xmlNodePtr pxmlNodePtr)
{

        if(g_pomTxMsgEthernetChildWindow)
        {
            g_pomTxMsgEthernetChildWindow->vUpdateWndCo_Ords();
        }
        CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().bGetConfigData(pxmlNodePtr);


    return S_OK;
}


USAGEMODE HRESULT TXComman_vSetTxWndConfigDataXML( ETYPE_BUS eBusType, xmlDocPtr pDoc)
{
	CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().bSetConfigData(pDoc);

	return S_OK;
}
/*USAGEMODE HRESULT TX_bIsTxWndConfigChanged()
{
    if(CTxMsgManager::s_podGetTxMsgManager()->bIsTxWndConfigChanged())
        return S_OK;
    else
        return S_FALSE;
}*/

USAGEMODE HRESULT TXEthernet_vSetTxStopFlag(BOOL bStartStop)
{
    //CTxMsgManager::s_podGetTxMsgManager()->vSetTxStopFlag(bStartStop);
    return S_OK;
}

USAGEMODE BOOL TXEthernet_bGetTxStopFlag()
{
    //return CTxMsgManager::s_podGetTxMsgManager()->bGetTxStopFlag();
    return S_OK;
}

USAGEMODE UINT TXEthernet_unGetTxBlockCount()
{
    return 0;
    //return CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().nGetBlockCount();
    //return CTxMsgManager::s_podGetTxMsgManager()->nGetBlockCount();
}

//USAGEMODE HRESULT TXEthernet_vFlexFileChanged()
//{
//    //CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().vRemoveAllBlock(
//    //    CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_omTxMsgBlockMan);
//    //if(g_pomTxMsgFlexChildWindow != NULL)
//    //{
//    //    //g_pomTxMsgFlexChildWindow->m_pTransmitMsgView->vClearInitialisationFileContents();
//    //}
//
//    return S_OK;
//}

USAGEMODE HRESULT TXComman_nSetFibexConfig(ETYPE_BUS eBus, ClusterConfig& ouFlexConfig)
{
    /* CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().vRemoveAllBlock(
         CTxFlexRayDataStore::ouGetTxFlexRayDataStoreObj().m_omTxMsgBlockMan);*/
    HRESULT hr = S_FALSE;

        /*hr =CTxEthernetDataStore::ouGetTxEthernetDataStoreObj().SetFlexRayConfig(ouFlexConfig);
        if(g_pomTxMsgFlexChildWindow != NULL && hr == S_OK)
        {
            g_pomTxMsgFlexChildWindow->m_pTransmitMsgView->UpdateTxView(ouFlexConfig.m_nChannelsConfigured);
        }*/


    return hr;
}