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
#include "stdafx.h"
#include <afxdllx.h>
//#include "../Application/MultiLanguage.h"
//#include "../Application/GettextBusmaster.h"
#define USAGE_EXPORT
#include "TXWindow_Extern.h"
#include "TxMsgChildFrame.h"
#include "LINScheduleDataStore.h"
#include "CANTxFormView.h"
#include "LINTxFormView.h"
#include "CANTransmitter.h"
#include "LINTransmitter.h"

CTxMsgChildFrame* g_pomTxMsgLinChildWindow = nullptr;
CTxMsgChildFrame* g_pomTxMsgCanChildWindow = nullptr;
CScheduleTableCfgDlg* g_pomLINScheduleCfgDlg = NULL;

static AFX_EXTENSION_MODULE TXWindowDLL = { false, nullptr };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    static HINSTANCE shLangInst=nullptr;


    // Remove this if you use lpReserved
    UNREFERENCED_PARAMETER(lpReserved);

    if (dwReason == DLL_PROCESS_ATTACH)
    {
        TRACE0("TXWindow.DLL Initializing!\n");

        // Extension DLL one-time initialization
        if (!AfxInitExtensionModule(TXWindowDLL, hInstance))
        {
            return 0;
        }
        new CDynLinkLibrary(TXWindowDLL);

    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        if (shLangInst)
        {
            FreeLibrary(shLangInst);
        }

        //Clear any memory allocated
        //CTxMsgManager::s_bDeleteTxMsgManager();
        TRACE0("TXWindow.DLL Terminating!\n");

        // Terminate the library before destructors are called
        AfxTermExtensionModule(TXWindowDLL);
    }
    return 1;   // ok
}


//Export Function Definitions.

void SetDefaultWindowPos(WINDOWPLACEMENT& sTxWndPlacement)
{
    sTxWndPlacement.flags = 1;
    sTxWndPlacement.length = 44;
    sTxWndPlacement.ptMaxPosition.x = 0;
    sTxWndPlacement.ptMaxPosition.x = 0;
    sTxWndPlacement.ptMinPosition.x = 0;
    sTxWndPlacement.ptMinPosition.y = 0;
    sTxWndPlacement.rcNormalPosition.top = 3;
    sTxWndPlacement.rcNormalPosition.bottom = 612;
    sTxWndPlacement.rcNormalPosition.left = 11;
    sTxWndPlacement.rcNormalPosition.right = 883;
    sTxWndPlacement.showCmd = 1;
}

HRESULT nShowScheduleConfigDlg(void* pParentWnd, IBMNetWorkGetService* pClusterConfig)
{
    HRESULT hResult = S_FALSE;

    if(g_pomLINScheduleCfgDlg == NULL)
    {
        g_pomLINScheduleCfgDlg = new CScheduleTableCfgDlg(pClusterConfig, (CWnd*)pParentWnd);

        hResult = g_pomLINScheduleCfgDlg->Create(IDD_DLG_LIN_SCHEDULE_CONFIG, (CWnd*)pParentWnd);
        //g_pomLINScheduleCfgDlg->SetParent((CWnd*)pParentWnd);
        WINDOWPLACEMENT wndPos;
        CLINScheduleDataStore::pGetLINSchedDataStore().vGetWindowPlacement(wndPos);
        g_pomLINScheduleCfgDlg->SetWindowPlacement(&(wndPos));
        g_pomLINScheduleCfgDlg->ShowWindow(SW_SHOW);
        hResult = S_OK;
    }
    else
    {
        g_pomLINScheduleCfgDlg->ShowWindow(SW_SHOW);
    }

    return hResult;
}
int nShowTxWindow(void* pParentWnd, ETYPE_BUS eBUS)
{
    CTxMsgChildFrame* pFrameWindow = nullptr;
    std::string strTitle = "";       //Used for Title.
    bool bValidBus = false;
    if (eBUS == LIN)
    {
        pFrameWindow = g_pomTxMsgLinChildWindow;
        strTitle = "Configure Transmission Messages - LIN";
        bValidBus = true;
    }
    else if (eBUS == CAN)
    {
        pFrameWindow = g_pomTxMsgCanChildWindow;
        strTitle = "Configure Transmission Messages - CAN";
        bValidBus = true;
    }

    if (bValidBus == false)
    {
        return S_FALSE;
    }
    WINDOWPLACEMENT sTxWndPlacement;
    WINDOWPLACEMENT* psTxWndPlacement = &sTxWndPlacement;
    bool bRetVal = false;

    if (CAN == eBUS)
    {
        CCANTxFormView* pouCANTxFormView = CCANTxFormView::GetInstance();
        if (nullptr != pouCANTxFormView)
        {
            bRetVal = pouCANTxFormView->GetTxData(TX_WINDOW_PLACEMENT, (void**)&psTxWndPlacement);
            if (nullptr != g_pomTxMsgCanChildWindow)
            {
                pouCANTxFormView->EnsureLastItemVisible();
            }
            pouCANTxFormView->ReleaseInstance();
        }
    }
    else if (LIN == eBUS)
    {
        CLINTxFormView* pouLINTxFormView = CLINTxFormView::GetInstance();
        if (nullptr != pouLINTxFormView)
        {
            bRetVal = pouLINTxFormView->GetTxData(TX_WINDOW_PLACEMENT, (void**)&psTxWndPlacement);
            if (nullptr != g_pomTxMsgLinChildWindow)
            {
                pouLINTxFormView->EnsureLastItemVisible();
            }
            pouLINTxFormView->ReleaseInstance();
        }
    }
    CRect omRect;
    CWnd* pWnd = (CWnd*)pParentWnd;
    pWnd->GetClientRect(&omRect);

    if (pFrameWindow == nullptr)
    {
        pFrameWindow = new CTxMsgChildFrame(eBUS);
        if (pFrameWindow != nullptr)
        {
            LPCTSTR strMDIClass = AfxRegisterWndClass(
                                      CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
                                      LoadCursor(nullptr, IDC_CROSS), 0,
                                      ::LoadIcon(nullptr, MAKEINTRESOURCE(IDI_ICO_SEND_MSG)));
            // Set the size got from configuration module


            if (pFrameWindow->Create(strMDIClass,
                                     strTitle.c_str(),
                                     WS_CHILD | WS_OVERLAPPEDWINDOW,
                                     omRect, (CMDIFrameWnd*)pParentWnd) == TRUE)
            {
                // Show window and set focus
                pFrameWindow->ModifyStyle(WS_MINIMIZEBOX | WS_MAXIMIZEBOX, 0);

                pFrameWindow->ShowWindow(SW_SHOWNORMAL);
                pFrameWindow->UpdateWindow();
                if (sTxWndPlacement.rcNormalPosition.top != -1 &&
                        sTxWndPlacement.length != 0)
                {
                    pFrameWindow->SetWindowPlacement(&sTxWndPlacement);
                }
                else
                {
                    //CLINTxFormView::GetLINTxFormView().bSetTxData(TX_WINDOW_PLACEMENT, (void**)&sTxWndPlacement);
                    // Set the defaul position
                    pFrameWindow->SetWindowPlacement(&sTxWndPlacement);
                }
                pFrameWindow->SetFocus();
                pFrameWindow->MDIActivate();
                pFrameWindow->SetActiveWindow();
            }
            if (LIN == eBUS)
            {
                g_pomTxMsgLinChildWindow = pFrameWindow;
            }
            else if (CAN == eBUS)
            {
                g_pomTxMsgCanChildWindow = pFrameWindow;
            }
        }
        else
        {
            return S_FALSE;
        }
    }
    else
    {
        pFrameWindow->ShowWindow(SW_SHOWNORMAL);
        pFrameWindow->UpdateWindow();
        pFrameWindow->SetFocus();
        pFrameWindow->MDIActivate();
        pFrameWindow->SetActiveWindow();
    }
    return S_OK;
}



USAGEMODE HRESULT TXLIN_vShowScheduleConfigDlg(bool bShow)
{
    if ( NULL != g_pomLINScheduleCfgDlg )
    {
        int nShow  = bShow ? SW_SHOW : SW_HIDE;
        g_pomLINScheduleCfgDlg->ShowWindow(nShow);
    }
    return S_OK;
}


USAGEMODE HRESULT TXLIN_vCreateScheduleConfigDlg(void* pParentWnd, IBMNetWorkGetService* pClusterConfig)
{
    //Place this code at the beginning of the export function.
    //Save previous resource handle and switch to current one.
    HINSTANCE hInst = AfxGetResourceHandle();
    AfxSetResourceHandle(TXWindowDLL.hResource);

    HRESULT nResult = nShowScheduleConfigDlg(pParentWnd, pClusterConfig);

    AfxSetResourceHandle(hInst);
    return nResult;
}

USAGEMODE HRESULT TXComman_vSetScheduleConfig( xmlDocPtr pDoc)
{

    CLINScheduleDataStore::pGetLINSchedDataStore().hSetConfigData(pDoc);
    if ( g_pomLINScheduleCfgDlg != NULL )
    {
        g_pomLINScheduleCfgDlg->nUpdateScheduleView();
    }

    return S_OK;
}

USAGEMODE HRESULT TXComman_vGetScheduleConfig( xmlNodePtr pDoc)
{
    WINDOWPLACEMENT wndPlacement;
    if ( g_pomLINScheduleCfgDlg != NULL )
    {
        g_pomLINScheduleCfgDlg->GetWindowPlacement(&wndPlacement);
    }
    else
    {
        CLINScheduleDataStore::pGetLINSchedDataStore().vGetWindowPlacement(wndPlacement);
    }
    //1. Save Window Positions
    xmlNodePtr pNodeWndPos = xmlNewNode(NULL, BAD_CAST DEF_WND_POS);
    xmlUtils::CreateXMLNodeFrmWindowsPlacement(pNodeWndPos,wndPlacement);
    xmlAddChild(pDoc, pNodeWndPos);
    CLINScheduleDataStore::pGetLINSchedDataStore().hGetConfigData(pDoc);
    return S_OK;
}
USAGEMODE HRESULT TXComman_vShowConfigureMsgWindow(void* pParentWnd, ETYPE_BUS eBUS)
{
    //Place this code at the beginning of the export function.
    //Save previous resource handle and switch to current one.
    HINSTANCE hInst = AfxGetResourceHandle();
    AfxSetResourceHandle(TXWindowDLL.hResource);

    int nRes = nShowTxWindow(pParentWnd, eBUS);

    AfxSetResourceHandle(hInst);
    return nRes;
}


USAGEMODE HRESULT TXComman_vSetClientID(ETYPE_BUS eBusType, DWORD dwClientID)
{
    if (LIN == eBusType)
    {
        CLINTxFormView* pouLINTxFormView = CLINTxFormView::GetInstance();
        if (nullptr != pouLINTxFormView)
        {
            pouLINTxFormView->SetClientId(dwClientID);
            pouLINTxFormView->ReleaseInstance();
        }
        CLINScheduleDataStore::pGetLINSchedDataStore().m_dwClientID = dwClientID;
    }
    else if (CAN == eBusType)
    {
        CCANTxFormView* pouCANTxFormView = CCANTxFormView::GetInstance();
        if (nullptr != pouCANTxFormView)
        {
            pouCANTxFormView->SetClientId(dwClientID);
            pouCANTxFormView->ReleaseInstance();
        }
    }
    return S_OK;
}
USAGEMODE HRESULT TXComman_vSetDILInterfacePtr(ETYPE_BUS /*eBusType*/, void* /*pDilPointer*/)
{
    return S_OK;
}

USAGEMODE HRESULT TXComman_vPostMessageToTxWnd(UINT msg, WPARAM wParam, LPARAM lParam)
{
    CCANTxFormView* pCANTxFormView = CCANTxFormView::GetInstance();
    CLINTxFormView* pLINTxFormView = CLINTxFormView::GetInstance();
    if (nullptr != pCANTxFormView && nullptr != pLINTxFormView)
    {
        switch (wParam)
        {
            case 0: //eHEXDECCMD
                pCANTxFormView->SetHexMode(lParam != 0);
				pLINTxFormView->SetHexMode(lParam != 0);
                break;
            case 6://eKeyPress
                pCANTxFormView->OnKeyPressed((char)lParam);
                pLINTxFormView->OnKeyPressed((char)lParam);
                break;
            case 7:
                //Create and Release TxFormInstance. eCREATEDESTROYCMD
				if (0 != lParam )
                {
                    CLINTxFormView::GetInstance();
                    CCANTxFormView::GetInstance();
                }
                else
                {
                    CLINTxFormView::ReleaseInstance();
                    CCANTxFormView::ReleaseInstance();
                }
            default:
                break;
        }
        if (nullptr != g_pomTxMsgLinChildWindow)
        {
            pLINTxFormView->SendMessage(msg, wParam, lParam);
        }
        if (nullptr != g_pomTxMsgCanChildWindow)
        {
            pCANTxFormView->SendMessage(msg, wParam, lParam);
        }
        pCANTxFormView->ReleaseInstance();
        pLINTxFormView->ReleaseInstance();
    }
    return S_OK;
}

USAGEMODE HRESULT TXComman_hConfigWindowShown(ETYPE_BUS eBusType)
{
    int hResult = S_FALSE;
    if (LIN == eBusType)
    {
        if(g_pomTxMsgLinChildWindow)
        {
            hResult = S_OK;
        }
    }
    else if (CAN == eBusType)
    {
        if (g_pomTxMsgCanChildWindow)
        {
            hResult = S_OK;
        }
    }
    return hResult;
}


USAGEMODE HRESULT TX_vBusStatusChanged(ETYPE_BUS eBusType, ESTATUS_BUS eBusStatus)
{
    if (LIN == eBusType)
    {
        CLINScheduleDataStore::pGetLINSchedDataStore().vSetBusStatus(eBusStatus);
        CLINTxFormView* pLinTxFormView = CLINTxFormView::GetInstance();
        if (nullptr != pLinTxFormView)
        {
            //Set Bus Status even if the window is not created.
            //ie. g_pomTxMsgLinChildWindow = nullptr.
            pLinTxFormView->SetBusStatus(eBusStatus);
            if (nullptr != g_pomTxMsgLinChildWindow)
            {
                //If Window is created then update its status
                //such as the form parameters to non - editable, Button Status of Send, Delete etc.
                pLinTxFormView->SendMessage(WM_USER + 44, 2, eBusStatus);
            }
            pLinTxFormView->ReleaseInstance();
        }
        ITransmitter* pITransmitter = CLINTransmitter::GetInstance();
        if (nullptr != pITransmitter)
        {
            pITransmitter->OnBusStatusChanged(eBusStatus);
            ((CLINTransmitter*)pITransmitter)->ReleaseInstance();
        }
    }
    else if (CAN == eBusType)
    {
        CCANTxFormView* pCanTxFormView = CCANTxFormView::GetInstance();
        if (nullptr != pCanTxFormView)
        {
            //Set Bus Status even if the window is not created.
            //ie. g_pomTxMsgCanChildWindow = nullptr.
            pCanTxFormView->SetBusStatus(eBusStatus);
            if (nullptr != g_pomTxMsgCanChildWindow)
            {
                //If Window is created then update its status
                //such as the form parameters to non - editable, Button Status of Send, Delete etc.
                pCanTxFormView->SendMessage(WM_USER + 44, 2, eBusStatus);
            }
            pCanTxFormView->ReleaseInstance();
        }
        ITransmitter* pITransmitter = CCANTransmitter::GetInstance();
        if (nullptr != pITransmitter)
        {
            pITransmitter->OnBusStatusChanged(eBusStatus);
            ((CCANTransmitter*)pITransmitter)->ReleaseInstance();
        }
    }
    //CTxMsgManager::s_podGetTxMsgManager()->vStopTransmission(unMaxWaitTime);
    return S_OK;
}

USAGEMODE HRESULT TXComman_vGetTxWndConfigData( ETYPE_BUS eBusType, xmlNodePtr pxmlNodePtr)
{
    if (LIN == eBusType)
    {
        if(g_pomTxMsgLinChildWindow)
        {
            g_pomTxMsgLinChildWindow->vUpdateWndCo_Ords();
        }

        CLINTxFormView* pouLINTxFormView = CLINTxFormView::GetInstance();
        if (nullptr != pouLINTxFormView)
        {
            pouLINTxFormView->GetConfigData(pxmlNodePtr);
            pouLINTxFormView->ReleaseInstance();
        }
    }
    else if (CAN == eBusType)
    {
        if (g_pomTxMsgCanChildWindow)
        {
            g_pomTxMsgCanChildWindow->vUpdateWndCo_Ords();
        }

        CCANTxFormView* pouCANTxFormView = CCANTxFormView::GetInstance();
        if (nullptr != pouCANTxFormView)
        {
            pouCANTxFormView->GetConfigData(pxmlNodePtr);
            pouCANTxFormView->ReleaseInstance();
        }
    }

    return S_OK;
}


USAGEMODE HRESULT TXComman_vSetTxWndConfigDataXML( ETYPE_BUS eBusType, xmlDocPtr pDoc)
{
    if (eBusType == LIN)
    {
        CLINTxFormView* pouLINTxFormView = CLINTxFormView::GetInstance();
        if (nullptr != pouLINTxFormView)
        {
            pouLINTxFormView->SetConfigData(pDoc);
            if (g_pomTxMsgLinChildWindow != nullptr)
            {
                WINDOWPLACEMENT sTxWndPlacement;
                WINDOWPLACEMENT* psTxWndPlacement = &sTxWndPlacement;
                pouLINTxFormView->GetTxData(TX_WINDOW_PLACEMENT, (void**)&psTxWndPlacement);
                if (TRUE == g_pomTxMsgLinChildWindow->IsWindowVisible())
                {
                    g_pomTxMsgLinChildWindow->SetWindowPlacement(&sTxWndPlacement);
                }
                pouLINTxFormView->UpdateTxView();
                //On changing to new configuration the Tx Window is hidden
                //OnClose the TxWindow is hidden.
                g_pomTxMsgLinChildWindow->OnClose();
            }
            pouLINTxFormView->ReleaseInstance();
        }
    }
    else if (eBusType == CAN)
    {
        CCANTxFormView* pouCANTxFormView = CCANTxFormView::GetInstance();
        if (nullptr != pouCANTxFormView)
        {
            pouCANTxFormView->SetConfigData(pDoc);
            if (g_pomTxMsgCanChildWindow != nullptr)
            {
                WINDOWPLACEMENT sTxWndPlacement;
                WINDOWPLACEMENT* psTxWndPlacement = &sTxWndPlacement;
                pouCANTxFormView->GetTxData(TX_WINDOW_PLACEMENT, (void**)&psTxWndPlacement);
                if (TRUE == g_pomTxMsgCanChildWindow->IsWindowVisible())
                {
                    g_pomTxMsgCanChildWindow->SetWindowPlacement(&sTxWndPlacement);
                }
                pouCANTxFormView->UpdateTxView();
                g_pomTxMsgCanChildWindow->OnClose();
            }
            pouCANTxFormView->ReleaseInstance();
        }
    }
    return S_OK;
}

USAGEMODE HRESULT TXComman_nSetNetworkConfig(ETYPE_BUS eBus, IBMNetWorkGetService* ouFlexConfig)
{
    HRESULT hr = S_FALSE;
    int nChannels = 0;
    ouFlexConfig->GetChannelCount(eBus, nChannels);
    if ( eBus == LIN )
    {
        CLINTxFormView* pouLINTxFormView = CLINTxFormView::GetInstance();
        if (nullptr != pouLINTxFormView)
        {
            hr = pouLINTxFormView->SetIBMNetwork(ouFlexConfig);
            if (g_pomTxMsgLinChildWindow != nullptr && hr == S_OK)
            {
                pouLINTxFormView->UpdateTxView();
            }
            pouLINTxFormView->ReleaseInstance();
        }
        CLINScheduleDataStore::pGetLINSchedDataStore().hSetClusterConfig(ouFlexConfig);
        if (NULL != g_pomLINScheduleCfgDlg )
        {
            g_pomLINScheduleCfgDlg->nUpdateScheduleView();
        }

    }
    else if (eBus == CAN)
    {
        CCANTxFormView* pouCANTxFormView = CCANTxFormView::GetInstance();
        if (nullptr != pouCANTxFormView)
        {
            hr = pouCANTxFormView->SetIBMNetwork(ouFlexConfig);
            if (g_pomTxMsgCanChildWindow != nullptr && hr == S_OK)
            {
                pouCANTxFormView->UpdateTxView();
            }
            pouCANTxFormView->ReleaseInstance();
        }
    }

    return hr;
}
