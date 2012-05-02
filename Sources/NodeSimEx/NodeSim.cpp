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
 * \file      NodeSim.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
// NodeSimEx.cpp : Defines the initialization routines for the DLL.
//
#include "NodeSimEx_stdafx.h"
#include "include/BaseDefs.h"
#include "GlobalObj.h"
#include "ExecuteManager.h"
#include "EditFrameWnd.h"
#include "SimSysManager.h"

#include "DIL_Interface/DIL_Interface_extern.h"
#include "NodeSim.h"

#define defSTR_SIMSYS_WINDOW_TITLE              _T("Configure Simulated Systems")

ETYPE_BUS CEditFrameWnd::sm_eBus = CAN;
ETYPE_BUS CFileView::sm_eBus = CAN;

CNodeSim::CNodeSim(ETYPE_BUS eBus)
{
    m_eBus = eBus;
}
CNodeSim::~CNodeSim()
{
}
int CNodeSim::ExitInstance(void)
{
    //All the memory release should happen here
    if ( CGlobalObj::ouGetObj(m_eBus).m_pEditorDocTemplate != NULL)
    {
        delete CGlobalObj::ouGetObj(m_eBus).m_pEditorDocTemplate;
        CGlobalObj::ouGetObj(m_eBus).m_pEditorDocTemplate = NULL;
    }

    CGlobalObj::ouGetObj(m_eBus).m_ArrAPIsList.RemoveAll();
    CGlobalObj::sm_hWndMDIParentFrame = NULL;
    CGlobalObj::ouGetObj(m_eBus).m_omStrSourceFilePathName.Empty();

    if ( CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr != NULL)
    {
        delete[] CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr;
        CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr = NULL;
    }

    //remove the singleton class objects
    CSimSysManager::vClearObj(m_eBus);
    CExecuteManager::vClearObj(m_eBus);
    return 0;
}

HRESULT CNodeSim::FE_CreateFuncEditorTemplate(HWND handle, S_EXFUNC_PTR& sExInitStruct)
{
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (CGlobalObj::ouGetObj(m_eBus).m_pEditorDocTemplate == NULL)
    {
        CEditFrameWnd::sm_eBus = m_eBus;
        CFileView::sm_eBus = m_eBus;
        CGlobalObj::ouGetObj(m_eBus).m_pEditorDocTemplate = new CMultiDocTemplate(IDI_ICON_FN_EDITOR,
                RUNTIME_CLASS(CFunctionEditorDoc),
                RUNTIME_CLASS(CEditFrameWnd), // custom MDI child frame
                RUNTIME_CLASS(CFileView));
        //AddDocTemplate(CGlobalObj::m_pEditorDocTemplate);
    }

    CGlobalObj::sm_hWndMDIParentFrame = handle;
    NS_UpdateFuncStructsNodeSimEx((PVOID)&sExInitStruct, UPDATE_ALL);
    CFrameWnd* pParent = (CFrameWnd*)CWnd::FromHandle(CGlobalObj::sm_hWndMDIParentFrame);

    // Get Window rectangle from configuration module
    if ((CGlobalObj::ouGetObj(m_eBus).m_wWindowPlacement.length == 0) ||
            (CGlobalObj::ouGetObj(m_eBus).m_wWindowPlacement.rcNormalPosition.top == -1))
    {
        CGlobalObj::ouGetObj(m_eBus).bGetDefaultValue(SIMSYS_WND_PLACEMENT, CGlobalObj::ouGetObj(m_eBus).m_wWindowPlacement);
    }

    // Check for window pointer
    if( CGlobalObj::ouGetObj(m_eBus).m_pomSimSysWnd == NULL )
    {
        // Create New Instance
        CGlobalObj::ouGetObj(m_eBus).m_pomSimSysWnd = new CSimSysWnd(m_eBus);

        if( CGlobalObj::ouGetObj(m_eBus).m_pomSimSysWnd != NULL )
        {
            // Register Window Class
            LPCTSTR strMDIClass = AfxRegisterWndClass(
                                      CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
                                      LoadCursor(NULL, IDC_CROSS), 0,
                                      AfxGetApp()->LoadIcon(IDI_ICON_FN_EDITOR) );
            // Set the size got from configuration module
            CRect omRect(CGlobalObj::ouGetObj(m_eBus).m_wWindowPlacement.rcNormalPosition);
            // Create Simulated system Configuration window
            CString omTitle;
            omTitle.Format(_T("%s - %s"), defSTR_SIMSYS_WINDOW_TITLE, CGlobalObj::omGetBusName(m_eBus));

            if( CGlobalObj::ouGetObj(m_eBus).m_pomSimSysWnd->Create( strMDIClass,
                    omTitle.GetBuffer(MAX_PATH),
                    WS_CHILD | WS_OVERLAPPEDWINDOW,
                    omRect, (CMDIFrameWnd*)pParent ) == TRUE )
            {
                // Show window and set focus
                CGlobalObj::ouGetObj(m_eBus).m_pomSimSysWnd->ShowWindow( SW_SHOW );
                CGlobalObj::ouGetObj(m_eBus).m_pomSimSysWnd->SetFocus();

                if ((CGlobalObj::ouGetObj(m_eBus).m_wWindowPlacement.rcNormalPosition.bottom == 0) &&
                        (CGlobalObj::ouGetObj(m_eBus).m_wWindowPlacement.rcNormalPosition.left   == 0) &&
                        (CGlobalObj::ouGetObj(m_eBus).m_wWindowPlacement.rcNormalPosition.right  == 0) &&
                        (CGlobalObj::ouGetObj(m_eBus).m_wWindowPlacement.rcNormalPosition.top    == 0))
                {
                    //Propably new configuration
                    CGlobalObj::ouGetObj(m_eBus).bGetDefaultValue(SIMSYS_WND_PLACEMENT, CGlobalObj::ouGetObj(m_eBus).m_wWindowPlacement);
                }

                CGlobalObj::ouGetObj(m_eBus).m_pomSimSysWnd->m_wWndPlacement = CGlobalObj::ouGetObj(m_eBus).m_wWindowPlacement;
                CGlobalObj::ouGetObj(m_eBus).m_pomSimSysWnd->SetWindowPlacement
                (&(CGlobalObj::ouGetObj(m_eBus).m_pomSimSysWnd->m_wWndPlacement));
                CGlobalObj::ouGetObj(m_eBus).m_pomSimSysWnd->ShowWindow( SW_RESTORE );
            }
        }
    }
    // If already exist then activate and set the focus
    else
    {
        CGlobalObj::ouGetObj(m_eBus).m_pomSimSysWnd->ShowWindow( SW_RESTORE );
        CGlobalObj::ouGetObj(m_eBus).m_pomSimSysWnd->MDIActivate();
        CGlobalObj::ouGetObj(m_eBus).m_pomSimSysWnd->SetActiveWindow();
    }

    return S_OK;
}


void CNodeSim::NS_EnableAllHandlers(BOOL bEnable)
{
    CExecuteManager::ouGetExecuteManager(m_eBus).vEnableDisableAllHandlers(bEnable);
}

void CNodeSim::NS_EnableAllTimers(BOOL bEnable)
{
    CExecuteManager::ouGetExecuteManager(m_eBus).vEnableDisableAllTimers(bEnable);
}

void CNodeSim::NS_EnableAllKeyHandler(BOOL bEnable)
{
    CExecuteManager::ouGetExecuteManager(m_eBus).vEnableAllKeyHandler(bEnable);
}

void CNodeSim::NS_EnableAllMessageHandler(BOOL bEnable)
{
    CExecuteManager::ouGetExecuteManager(m_eBus).vEnableAllMessageHandler(bEnable);
}

void CNodeSim::NS_EnableAllErrorHandler(BOOL bEnable)
{
    CExecuteManager::ouGetExecuteManager(m_eBus).vEnableAllErrorHandler(bEnable);
}

void CNodeSim::NS_ManageOnKeyHandler(UCHAR ucKey)
{
    CExecuteManager::ouGetExecuteManager(m_eBus).vManageOnKeyHandler(ucKey);
}

void CNodeSim::NS_ManageOnMessageHandler(void* psRxMsgInfo)
{
    STCAN_MSG* psRxMsg = (STCAN_MSG*)psRxMsgInfo;
    CExecuteManager::ouGetExecuteManager(m_eBus).vManageOnMessageHandlerCAN(*psRxMsg, CAN_MONITOR_CLIENT_ID);
}

void CNodeSim::NS_ManageOnErrorHandler(eERROR_STATE eErrorCode,void* pvErrorVal)
{
    SCAN_ERR* psCanErr = (SCAN_ERR*)pvErrorVal;
    CExecuteManager::ouGetExecuteManager(m_eBus).vManageOnErrorHandlerCAN(eErrorCode, *psCanErr, 0);
}
HRESULT CNodeSim::NS_DLLBuildAll(CStringArray* pomStrErrorFiles)
{
    return CExecuteManager::ouGetExecuteManager(m_eBus).
           bDLLBuildAll(pomStrErrorFiles);
}

HRESULT CNodeSim::NS_DllLoadAll(CStringArray* pomStrErrorFiles)
{
    return CExecuteManager::ouGetExecuteManager(m_eBus).
           bDllLoadAll(pomStrErrorFiles);
}

HRESULT CNodeSim::NS_DllUnloadAll(CStringArray* pomStrErrorFiles)
{
    return CExecuteManager::ouGetExecuteManager(m_eBus).
           bDllUnloadAll(pomStrErrorFiles);
}

HRESULT CNodeSim::NS_DLLBuildLoadAll(CStringArray* pomStrErrorFiles)
{
    return CExecuteManager::ouGetExecuteManager(m_eBus).
           bDLLBuildLoadAll(pomStrErrorFiles);
}
void CNodeSim::NS_SetHandlersHelpText(CStringArray* pomTextArray)
{
    CGlobalObj::ouGetObj(m_eBus).m_omFnEditHelpText.RemoveAll();
    CGlobalObj::ouGetObj(m_eBus).m_omFnEditHelpText.Append(*pomTextArray);
}

void CNodeSim::NS_UpdateFuncStructsNodeSimEx(PVOID pvFuncStructs, E_UPDATE_TYPE eType)
{
    if (pvFuncStructs != NULL)
    {
        switch (eType)
        {
            case UPDATE_ALL:
            {
                S_EXFUNC_PTR* psExInitStruct = (S_EXFUNC_PTR*)pvFuncStructs;
                CGlobalObj::sm_hWndMDIParentFrame = psExInitStruct->m_hWmdMDIParentFrame;
                //CGlobalObj::g_pouTraceWnd = psExInitStruct->m_pouTraceWnd;
                //CGlobalObj::g_pNetwork_McNet = psExInitStruct->m_pNetWorkMcNet;
                CGlobalObj::ouGetObj(m_eBus).m_ArrAPIsList.RemoveAll();
                CGlobalObj::ouGetObj(m_eBus).m_ArrAPIsList.Append(psExInitStruct->m_omAPIList);
                CGlobalObj::ouGetObj(m_eBus).m_ArrAPIFuncNames.RemoveAll();
                CGlobalObj::ouGetObj(m_eBus).m_ArrAPIFuncNames.Append(psExInitStruct->m_omAPINames);
                CGlobalObj::ouGetObj(m_eBus).m_omObjWrapperName = psExInitStruct->m_omObjWrapperName;
                CGlobalObj::ouGetObj(m_eBus).m_omMsgStructName = psExInitStruct->m_omStructName;
                CGlobalObj::ouGetObj(m_eBus).m_omMsgStructFile = psExInitStruct->m_omStructFile;
                CGlobalObj::ouGetObj(m_eBus).m_omDefinedMsgHeaders.RemoveAll();
                CGlobalObj::ouGetObj(m_eBus).m_omDefinedMsgHeaders.Copy(psExInitStruct->m_omDefinedMsgHeaders);
                CGlobalObj::ouGetObj(m_eBus).m_odMsgNameMsgCodeList.RemoveAll();
                CGlobalObj::ouGetObj(m_eBus).m_odMsgNameMsgCodeList.AddTail(&(psExInitStruct->m_odMsgNameMsgCodeList));
                CGlobalObj::ouGetObj(m_eBus).m_omErrorHandlerList.RemoveAll();
                CGlobalObj::ouGetObj(m_eBus).m_omErrorHandlerList.Copy(psExInitStruct->m_omErrorHandlerList);
                //CGlobalObj::g_podNodeToDllMap = psExInitStruct->m_podNodeToDllMap;
                //CGlobalObj::g_podKeyPanelEntryList = psExInitStruct->m_podKeyPanelEntryList;
                CGlobalObj::ouGetObj(m_eBus).m_omAppDirectory = psExInitStruct->m_omAppDirectory;
                //CGlobalObj::TransmitMsg        = psExInitStruct->Send_Msg;
                CGlobalObj::ouGetObj(m_eBus).m_pfEnableDisableLog = psExInitStruct->EnDisableLog;
                CGlobalObj::ouGetObj(m_eBus).m_pfDisconnectTool   = psExInitStruct->DisConnectTool;
                CGlobalObj::ouGetObj(m_eBus).m_pfWriteToLog       = psExInitStruct->WriteToLog;
                CGlobalObj::sm_pouITraceWndPtr  = psExInitStruct->m_pouITraceWndPtr;
            }
            break;

            case UPDATE_DATABASE_MSGS:
            {
                CMsgNameMsgCodeList* podMsgNameCodeList = (CMsgNameMsgCodeList*)pvFuncStructs;
                CGlobalObj::ouGetObj(m_eBus).m_odMsgNameMsgCodeList.RemoveAll();
                CGlobalObj::ouGetObj(m_eBus).m_odMsgNameMsgCodeList.AddTail(podMsgNameCodeList);
            }
            break;

            case UPDATE_UNIONS_HEADER_FILES:
            {
                CStringArray* omDefMsgHeadrs = (CStringArray*)pvFuncStructs;
                CGlobalObj::ouGetObj(m_eBus).m_omDefinedMsgHeaders.RemoveAll();
                CGlobalObj::ouGetObj(m_eBus).m_omDefinedMsgHeaders.Copy(*omDefMsgHeadrs);
            }
            break;
        }
    }
}

BOOL CNodeSim::NS_ShouldToolBarBeEnabled(E_TB_FN_EDITOR eToolBarFlag)
{
    BOOL bReturn = FALSE;
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();

    if (pSimSysNodeInfo)
    {
        switch (eToolBarFlag)
        {
            case TB_LOAD:
            {
                bReturn = pSimSysNodeInfo->bIsLoadAllValid("");
            }
            break;

            case TB_UNLOAD:
            {
                bReturn = pSimSysNodeInfo->bIsUnLoadAllValid("");
            }
            break;

            case TB_BUILD:
            {
                bReturn = pSimSysNodeInfo->bIsBuildAllValid("");
            }
            break;

            case TB_BUILDLOAD:
            {
                bReturn = pSimSysNodeInfo->bIsBuildAllValid("");
            }
            break;

            case TB_SAVE_FILE:
            {
            }
            break;

            default:
            {
            }
        }
    }

    return bReturn;
}
BOOL CNodeSim::NS_GetHandlerStatus(eSIMSYSFLAG eHandlerFlag)
{
    return CSimSysManager::ouGetSimSysManager(m_eBus).ouGetFlags().nGetFlagStatus(eHandlerFlag);
}

void CNodeSim::NS_GetSimSysConfigData(BYTE*& pDesBuffer, int& nBuffSize)
{
    //First update pSimSysDataPtr
    CSimSysManager::ouGetSimSysManager(m_eBus).vSaveSimSysWndConfig();

    if (nBuffSize < CGlobalObj::ouGetObj(m_eBus).m_nSimSysDataSize)
    {
        if (pDesBuffer != NULL)
        {
            delete[] pDesBuffer;
        }

        nBuffSize = CGlobalObj::ouGetObj(m_eBus).m_nSimSysDataSize;
        pDesBuffer = new BYTE[nBuffSize];
    }

    memcpy (pDesBuffer, CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr, nBuffSize);
}

void CNodeSim::NS_SetSimSysConfigData(BYTE* pSrcBuffer, int nBuffSize)
{
    if ( CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr != NULL)
    {
        delete[] CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr;
        CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr = NULL;
    }

    if (nBuffSize > 0)
    {
        CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr = new BYTE[nBuffSize];
        memcpy(CGlobalObj::ouGetObj(m_eBus).m_pSimSysDataPtr, pSrcBuffer, nBuffSize);
        CGlobalObj::ouGetObj(m_eBus).m_nSimSysDataSize = nBuffSize;
    }

    //Update Internal Data
    CSimSysManager::ouGetSimSysManager(m_eBus).vLoadSimSysWndConfig();
}
BOOL CNodeSim::NS_IsSimSysConfigChanged()
{
    return CSimSysManager::ouGetSimSysManager(m_eBus).bIsConfigChanged();
}

HRESULT CNodeSim::FE_OpenFunctioneditorFile(CString omStrNewCFileName, HWND hMainFrame, S_EXFUNC_PTR& sExInitStruct)
{
    if (CGlobalObj::ouGetObj(m_eBus).m_pEditorDocTemplate == NULL)
    {
        CGlobalObj::ouGetObj(m_eBus).m_pEditorDocTemplate = new CMultiDocTemplate(IDI_ICON_FN_EDITOR,
                RUNTIME_CLASS(CFunctionEditorDoc),
                RUNTIME_CLASS(CEditFrameWnd), // custom MDI child frame
                RUNTIME_CLASS(CFileView));
        //AddDocTemplate(CGlobalObj::m_pEditorDocTemplate);
    }

    CGlobalObj::sm_hWndMDIParentFrame = hMainFrame;
    NS_UpdateFuncStructsNodeSimEx((PVOID)&sExInitStruct, UPDATE_ALL);
    return CGlobalObj::ouGetObj(m_eBus).bOpenFunctioneditorfile(omStrNewCFileName) ? S_OK : S_FALSE;
}

BOOL CNodeSim::InitInstance(void)
{
    return TRUE;
}
