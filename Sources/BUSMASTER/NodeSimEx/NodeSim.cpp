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
#include "Utility/XMLUtils.h"
#include "include/BaseDefs.h"
#include "GlobalObj.h"
#include "ExecuteManager.h"
#include "EditFrameWnd.h"
#include "SimSysManager.h"
#include "Utility/Utility.h"

//#include "DIL_Interface_extern.h"
#include "NodeSim.h"

#define defSTR_SIMSYS_WINDOW_TITLE              "Configure Simulated Systems"

ETYPE_BUS CEditFrameWnd::sm_eBus = CAN;
ETYPE_BUS CFileView::sm_eBus = CAN;

CNodeSim::CNodeSim(ETYPE_BUS eBus, CGlobalObj* pGlobalObj, CExecuteManager* pExecuteMgr)
{
    m_eBus = eBus;
    mpGlobalObj = pGlobalObj;
    m_pExecuteMgr = pExecuteMgr;
}
CNodeSim::~CNodeSim()
{

}
int CNodeSim::ExitInstance(void)
{
    //All the memory release should happen here
    if ( mpGlobalObj->m_pEditorDocTemplate != nullptr)
    {
        delete mpGlobalObj->m_pEditorDocTemplate;
        mpGlobalObj->m_pEditorDocTemplate = nullptr;
    }
    mpGlobalObj->m_ArrAPIsList.RemoveAll();
    CGlobalObj::sm_hWndMDIParentFrame = nullptr;
    mpGlobalObj->m_omStrSourceFilePathName.Empty();

    if ( mpGlobalObj->m_pSimSysDataPtr != nullptr)
    {
        delete[] mpGlobalObj->m_pSimSysDataPtr;
        mpGlobalObj->m_pSimSysDataPtr = nullptr;
    }
    //remove the singleton class objects
    CSimSysManager::vClearObj(m_eBus);
    CExecuteManager::vClearObj(m_eBus);
    return 0;
}
/*
 * \req      RS_FLX_08
 * \design   DS_FLX_08
 * \codetag  CT_FLX_08
 */
HRESULT CNodeSim::FE_CreateFuncEditorTemplate(HWND handle, S_EXFUNC_PTR& sExInitStruct)
{
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (mpGlobalObj->m_pEditorDocTemplate == nullptr)
    {
        CEditFrameWnd::sm_eBus = m_eBus;
        CFileView::sm_eBus = m_eBus;
        mpGlobalObj->m_pEditorDocTemplate = new CMultiDocTemplate(IDI_ICON_FN_EDITOR,

                RUNTIME_CLASS(CFunctionEditorDoc),
                RUNTIME_CLASS(CEditFrameWnd), // custom MDI child frame
                RUNTIME_CLASS(CFileView));
        //AddDocTemplate(CGlobalObj::m_pEditorDocTemplate);
    }
    CGlobalObj::sm_hWndMDIParentFrame = handle;
    NS_UpdateFuncStructsNodeSimEx((PVOID)&sExInitStruct, UPDATE_ALL);

    CFrameWnd* pParent = (CFrameWnd*)CWnd::FromHandle(CGlobalObj::sm_hWndMDIParentFrame);
    // Get Window rectangle from configuration module
    if ((mpGlobalObj->m_wWindowPlacement.length == 0) ||
            (mpGlobalObj->m_wWindowPlacement.rcNormalPosition.top == -1))
    {
        mpGlobalObj->bGetDefaultValue(SIMSYS_WND_PLACEMENT, mpGlobalObj->m_wWindowPlacement);
    }
    // Check for window pointer
    if( mpGlobalObj->m_pomSimSysWnd == nullptr )
    {
        CSimSysTreeView* pSimSysTreeView = new CSimSysTreeView(m_eBus,
                &CSimSysManager::ouGetSimSysManager(m_eBus, mpGlobalObj), &CExecuteManager::ouGetExecuteManager(m_eBus, mpGlobalObj),
                mpGlobalObj, mpGlobalObj->omGetBusName(m_eBus));
        // Create New Instance
        mpGlobalObj->m_pomSimSysWnd = new CSimSysWnd(m_eBus, pSimSysTreeView, &CGlobalObj::ouGetObj(m_eBus), CGlobalObj::omGetBusName(m_eBus));
        if( mpGlobalObj->m_pomSimSysWnd != nullptr )
        {
            // Register Window Class
            LPCTSTR strMDIClass = AfxRegisterWndClass(
                                      CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
                                      LoadCursor(nullptr, IDC_CROSS), 0,
                                      AfxGetApp()->LoadIcon(IDI_ICON_FN_EDITOR) );
            // Set the size got from configuration module
            CRect omRect(mpGlobalObj->m_wWindowPlacement.rcNormalPosition);
            // Create Simulated system Configuration window
            CString omTitle;
            omTitle.Format("%s - %s", defSTR_SIMSYS_WINDOW_TITLE, CGlobalObj::omGetBusName(m_eBus));
            if( mpGlobalObj->m_pomSimSysWnd->Create( strMDIClass,
                    omTitle.GetBuffer(MAX_PATH),
                    WS_CHILD | WS_OVERLAPPEDWINDOW,
                    omRect, (CMDIFrameWnd*)pParent ) == TRUE )
            {

                // Show window and set focus
                mpGlobalObj->m_pomSimSysWnd->ShowWindow( SW_SHOW );
                mpGlobalObj->m_pomSimSysWnd->SetFocus();
                if ((mpGlobalObj->m_wWindowPlacement.rcNormalPosition.bottom == 0) &&
                        (mpGlobalObj->m_wWindowPlacement.rcNormalPosition.left   == 0) &&
                        (mpGlobalObj->m_wWindowPlacement.rcNormalPosition.right  == 0) &&
                        (mpGlobalObj->m_wWindowPlacement.rcNormalPosition.top    == 0))
                {
                    //Propably new configuration
                    mpGlobalObj->bGetDefaultValue(SIMSYS_WND_PLACEMENT, mpGlobalObj->m_wWindowPlacement);
                }
                mpGlobalObj->m_pomSimSysWnd->m_wWndPlacement = mpGlobalObj->m_wWindowPlacement;
                mpGlobalObj->m_pomSimSysWnd->SetWindowPlacement
                (&(mpGlobalObj->m_pomSimSysWnd->m_wWndPlacement));
                mpGlobalObj->m_pomSimSysWnd->ShowWindow( SW_RESTORE );

            }
        }
    }
    // If already exist then activate and set the focus
    else
    {
        mpGlobalObj->m_pomSimSysWnd->ShowWindow( SW_RESTORE );
        mpGlobalObj->m_pomSimSysWnd->MDIActivate();
        mpGlobalObj->m_pomSimSysWnd->SetActiveWindow();
    }
    return S_OK;
}



void CNodeSim::NS_ManageOnKeyHandler(UCHAR ucKey)
{
    m_pExecuteMgr->vManageOnKeyHandler(ucKey);
}

void CNodeSim::NS_ManageBusEventHandler(eBUSEVEHANDLER eBusEvent)
{
    //m_pExecuteMgr->vManageBusEventHandler(eBusEvent);
    m_pExecuteMgr->vManageBusEventHandler(eBusEvent);
}


void CNodeSim::NS_ManageOnMessageHandler(void* psRxMsgInfo)
{
    STCAN_TIME_MSG* psRxMsg = (STCAN_TIME_MSG*)psRxMsgInfo;
    DWORD ix = CAN_MONITOR_CLIENT_ID;
    m_pExecuteMgr->vManageOnMessageHandlerCAN_(psRxMsg, ix);
}

void CNodeSim::NS_ManageOnErrorHandler(ERROR_STATE eErrorCode,void* pvErrorVal)
{
    SCAN_ERR* psCanErr = (SCAN_ERR*)pvErrorVal;
    m_pExecuteMgr->vManageOnErrorHandlerCAN(eErrorCode, *psCanErr, 0);
}
HRESULT CNodeSim::NS_DLLBuildAll()
{
    return m_pExecuteMgr->
           bDLLBuildAll();
}
HRESULT CNodeSim::NS_DllUnloadAll(CStringArray* pomStrErrorFiles)
{
    return m_pExecuteMgr->
           bDllUnloadAll(pomStrErrorFiles);
}

HRESULT CNodeSim::NS_DLLBuildLoadAllEnabled()
{
    return m_pExecuteMgr->
           bDLLBuildLoadAllEnabled();
}
HRESULT CNodeSim::NS_DLLUnloadAllEnabled()
{
    return m_pExecuteMgr->bDLLUnloadAllEnabled();
}



void CNodeSim::NS_SetHandlersHelpText(CStringArray* pomTextArray)
{
    mpGlobalObj->m_omFnEditHelpText.RemoveAll();
    mpGlobalObj->m_omFnEditHelpText.Append(*pomTextArray);
}

void CNodeSim::NS_UpdateFuncStructsNodeSimEx(PVOID pvFuncStructs, E_UPDATE_TYPE eType)
{
    if (pvFuncStructs != nullptr)
    {
        switch (eType)
        {
            case UPDATE_ALL:
            {
                S_EXFUNC_PTR* psExInitStruct = (S_EXFUNC_PTR*)pvFuncStructs;
                CGlobalObj::sm_hWndMDIParentFrame = psExInitStruct->m_hWmdMDIParentFrame;
                //CGlobalObj::g_pouTraceWnd = psExInitStruct->m_pouTraceWnd;
                //CGlobalObj::g_pNetwork_McNet = psExInitStruct->m_pNetWorkMcNet;
                mpGlobalObj->m_ArrAPIsList.RemoveAll();
                mpGlobalObj->m_ArrAPIsList.Append(psExInitStruct->m_omAPIList);
                mpGlobalObj->m_ArrAPIFuncNames.RemoveAll();
                mpGlobalObj->m_ArrAPIFuncNames.Append(psExInitStruct->m_omAPINames);
                mpGlobalObj->m_omObjWrapperName = psExInitStruct->m_omObjWrapperName;
                mpGlobalObj->m_omMsgStructName = psExInitStruct->m_omStructName;
                mpGlobalObj->m_omMsgStructFile = psExInitStruct->m_omStructFile;
                mpGlobalObj->m_omDefinedMsgHeaders.RemoveAll();
                mpGlobalObj->m_omDefinedMsgHeaders.Copy(psExInitStruct->m_omDefinedMsgHeaders);
                mpGlobalObj->m_odMsgNameMsgCodeListDb.RemoveAll();      //CAPL_DB_NAME_CHANGE
                mpGlobalObj->m_odMsgNameMsgCodeListDb.AddTail(&(psExInitStruct->m_odMsgNameMsgCodeListDB));
                mpGlobalObj->m_omErrorHandlerList.RemoveAll();
                mpGlobalObj->m_omErrorHandlerList.Copy(psExInitStruct->m_omErrorHandlerList);
                //CGlobalObj::g_podNodeToDllMap = psExInitStruct->m_podNodeToDllMap;
                //CGlobalObj::g_podKeyPanelEntryList = psExInitStruct->m_podKeyPanelEntryList;
                mpGlobalObj->m_omAppDirectory = psExInitStruct->m_omAppDirectory;
                //CGlobalObj::TransmitMsg        = psExInitStruct->Send_Msg;
                mpGlobalObj->m_pfEnableDisableLog = psExInitStruct->EnDisableLog;
                mpGlobalObj->m_pfDisconnectTool   = psExInitStruct->DisConnectTool;
                mpGlobalObj->m_pfWriteToLog       = psExInitStruct->WriteToLog;
                CGlobalObj::sm_pouITraceWndPtr  = psExInitStruct->m_pouITraceWndPtr;
            }
            break;
            case UPDATE_DATABASE_MSGS:
            {
                //CAPL_DB_NAME_CHANGE
                CMsgNameMsgCodeListDataBase* podMsgNameCodeListDb = (CMsgNameMsgCodeListDataBase*)pvFuncStructs;
                mpGlobalObj->m_odMsgNameMsgCodeListDb.RemoveAll();
                mpGlobalObj->m_odMsgNameMsgCodeListDb.AddTail(podMsgNameCodeListDb);
            }
            break;
            case UPDATE_UNIONS_HEADER_FILES:
            {
                CStringArray* omDefMsgHeadrs = (CStringArray*)pvFuncStructs;
                mpGlobalObj->m_omDefinedMsgHeaders.RemoveAll();
                mpGlobalObj->m_omDefinedMsgHeaders.Copy(*omDefMsgHeadrs);
            }
            break;
        }
    }
}

void CNodeSim::NS_SetJ1939ActivationStatus(bool bActivated)
{
    mpGlobalObj->bJ1939Activated  = bActivated;
}

bool CNodeSim::NS_GetSimSysConfigData(xmlNodePtr& pNodePtr)
{
    if(m_eBus == CAN)
    {
        pNodePtr = xmlNewNode(nullptr, BAD_CAST DEF_CAN_SIM_SYS);
    }
    else if( m_eBus == J1939 )
    {
        pNodePtr = xmlNewNode(nullptr, BAD_CAST DEF_J1939_SIM_SYS);
    }
    else if( m_eBus == LIN )
    {
        pNodePtr = xmlNewNode(nullptr, BAD_CAST DEF_LIN_SIM_SYS);
    }

    CSimSysManager::ouGetSimSysManager(m_eBus, mpGlobalObj).bGetConfigData(pNodePtr);
    return true;
}
void CNodeSim::NS_SetSimSysConfigData(BYTE* pSrcBuffer, int nBuffSize)
{
    if ( mpGlobalObj->m_pSimSysDataPtr != nullptr)
    {
        delete[] mpGlobalObj->m_pSimSysDataPtr;
        mpGlobalObj->m_pSimSysDataPtr = nullptr;
    }
    if (nBuffSize > 0)
    {
        mpGlobalObj->m_pSimSysDataPtr = new BYTE[nBuffSize];
        memcpy(mpGlobalObj->m_pSimSysDataPtr, pSrcBuffer, nBuffSize);
        mpGlobalObj->m_nSimSysDataSize = nBuffSize;
    }

    //Update Internal Data
    CSimSysManager::ouGetSimSysManager(m_eBus, mpGlobalObj).vLoadSimSysWndConfig();
    if(pSrcBuffer == nullptr)
    {
        if(mpGlobalObj->m_pomSimSysWnd != nullptr)
        {
            mpGlobalObj->m_pomSimSysWnd->ShowWindow( SW_HIDE );
            mpGlobalObj->bCloseFunctionEditorFrame();
        }
    }
}

void CNodeSim::NS_SetSimSysConfigData(xmlDocPtr pXmlDoc)
{
    if ( mpGlobalObj->m_pSimSysDataPtr != nullptr)
    {
        delete[] mpGlobalObj->m_pSimSysDataPtr;
        mpGlobalObj->m_pSimSysDataPtr = nullptr;
    }
    if (pXmlDoc != nullptr)
    {
        /*mpGlobalObj->m_pSimSysDataPtr = new BYTE[nBuffSize];
        memcpy(mpGlobalObj->m_pSimSysDataPtr, pSrcBuffer, nBuffSize);
        mpGlobalObj->m_nSimSysDataSize = nBuffSize;*/
        CSimSysManager::ouGetSimSysManager(m_eBus, mpGlobalObj).vLoadSimSysWndConfig(pXmlDoc, m_eBus);

        /* Close the previosly opened function editor view */
        if(mpGlobalObj->m_pomSimSysWnd != nullptr)
        {
            mpGlobalObj->m_pomSimSysWnd->ShowWindow( SW_HIDE );
            mpGlobalObj->bCloseFunctionEditorFrame();
        }
    }
    //Update Internal Data

}

void CNodeSim::NS_SetSimSysConfigData(xmlNodePtr pXmlNode)
{
    if ( mpGlobalObj->m_pSimSysDataPtr != nullptr)
    {
        delete[] mpGlobalObj->m_pSimSysDataPtr;
        mpGlobalObj->m_pSimSysDataPtr = nullptr;
    }
    if (pXmlNode != nullptr)
    {
        /*mpGlobalObj->m_pSimSysDataPtr = new BYTE[nBuffSize];
        memcpy(mpGlobalObj->m_pSimSysDataPtr, pSrcBuffer, nBuffSize);
        mpGlobalObj->m_nSimSysDataSize = nBuffSize;*/
        CSimSysManager::ouGetSimSysManager(m_eBus, mpGlobalObj).vSetConfigData(pXmlNode);
    }
    //Update Internal Data


}
int  CNodeSim::NS_nOnBusConnected(bool bConnected)
{
    if(bConnected)
    {
        m_n64TimeElapsedSinceConnection = gnGetCpuClocks();
    }
    else
    {
        m_n64TimeElapsedSinceConnection = 0;
    }
    return TRUE;
}
BOOL CNodeSim::NS_IsSimSysConfigChanged()
{
    return CSimSysManager::ouGetSimSysManager(m_eBus, mpGlobalObj).bIsConfigChanged();
}

HRESULT CNodeSim::FE_OpenFunctioneditorFile(CString omStrNewCFileName, HWND hMainFrame, S_EXFUNC_PTR& sExInitStruct)
{
    if (mpGlobalObj->m_pEditorDocTemplate == nullptr)
    {
        mpGlobalObj->m_pEditorDocTemplate = new CMultiDocTemplate(IDI_ICON_FN_EDITOR,

                RUNTIME_CLASS(CFunctionEditorDoc),
                RUNTIME_CLASS(CEditFrameWnd), // custom MDI child frame
                RUNTIME_CLASS(CFileView));
        //AddDocTemplate(CGlobalObj::m_pEditorDocTemplate);
    }
    CGlobalObj::sm_hWndMDIParentFrame = hMainFrame;
    NS_UpdateFuncStructsNodeSimEx((PVOID)&sExInitStruct, UPDATE_ALL);

    return mpGlobalObj->bOpenFunctioneditorfile(omStrNewCFileName) ? S_OK : S_FALSE;
}

BOOL CNodeSim::InitInstance(void)
{
    return TRUE;
}


void CNodeSim::NS_SetBmNetworkConfig(IBMNetWorkGetService* ouLINConfig, bool bModified)
{
    CSimSysManager::ouGetSimSysManager(m_eBus, mpGlobalObj).vSetDatabaseConfiguration(ouLINConfig, bModified);
}
