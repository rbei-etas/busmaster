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
 * \file      GlobalObj.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "NodeSimEx_stdafx.h"
#include "include/Dil_CommonDefs.h"
#include "include/BaseDefs.h"
#include "Include/Utils_Macro.h"
#include "GlobalObj.h"
#include "ExecuteFunc.h"
#include "EditFrameWnd.h"
#include "FunctionEditorStruct.h"
#include "FunctionEditorBase.h"
#include "DIL_Interface/DIL_Interface_extern.h"

#define defCHILD_FRAME_PROP_H               0.75
#define defOUTPUT_WND_PROP_X                0.5
#define defTX_SIM_WND_BOTTOM_MARGIN        0.05
#define defTX_SIM_WND_RIGHT_MARGIN         0.05
// Simulated system Configuration window
#define defSTR_SIMSYS_SAVE_CONFIRMATION         _T("Do you want to save changes?")

//definition
CGlobalObj* CGlobalObj::sm_pThis[BUS_TOTAL] = {0};
HWND CGlobalObj::sm_hWndMDIParentFrame = NULL;
CBaseAppServices* CGlobalObj::sm_pouITraceWndPtr = NULL;
CBaseDIL_CAN* CGlobalObj::sm_pouDilCanInterface = NULL;

static void sg_vDataConfEventFnJ1939(DWORD dwClient, UINT32 unPGN, BYTE bySrc,
                                     BYTE byDest, BOOL bSuccess)
{
    CExecuteManager::ouGetExecuteManager(J1939).vManageOnDataConfHandlerJ1939(dwClient, unPGN, bySrc,
            byDest, bSuccess);
}
static void sg_vAddressClaimEventFnJ1939(DWORD dwClient, BYTE byAddress)
{
    CExecuteManager::ouGetExecuteManager(J1939).vManageOnAddressClaimHandlerJ1939(dwClient, byAddress);
}

CGlobalObj::CGlobalObj(ETYPE_BUS eBus)
{
    m_eBus = eBus;
    m_pEditorDocTemplate = NULL;
    m_pomSimSysWnd = NULL;
    m_omStrSourceFilePathName = "";
    m_ArrAPIsList.RemoveAll();
    m_ArrAPIFuncNames.RemoveAll();
    m_omDefinedMsgHeaders.RemoveAll();
    m_odMsgNameMsgCodeList.RemoveAll();
    m_omMsgStructName = _T("STCAN_MSG");
    m_omMsgStructFile = _T("Struct.h");
    m_omErrorHandlerList.RemoveAll();
    m_omObjWrapperName = _T("wrapper.o");
    m_podNodeToDllMap = NULL;
    //CKeyPanelEntryList* CGlobalObj::g_podKeyPanelEntryList = NULL;
    m_pfTransmitMsg         = NULL;
    m_pfEnableDisableLog    = NULL;
    m_pfDisconnectTool      = NULL;
    m_pfWriteToLog          = NULL;
    m_omFnEditHelpText.RemoveAll();
    m_pSimSysDataPtr = NULL;
    m_nSimSysDataSize = 0;
    m_omAppDirectory = "";
    memset(&m_wWindowPlacement, 0, sizeof(WINDOWPLACEMENT));
}

CGlobalObj::~CGlobalObj(void)
{
}
CBaseDIL_CAN* CGlobalObj::GetICANDIL()
{
    if (sm_pouDilCanInterface == NULL)
    {
        DIL_GetInterface(CAN, (void**)&(sm_pouDilCanInterface));
    }

    return sm_pouDilCanInterface;
}
CBaseDILI_J1939* CGlobalObj::GetIJ1939DIL(void)
{
    CBaseDILI_J1939* Result = NULL;
    DIL_GetInterface(J1939, (void**)&(Result));
    return Result;
}
CFunctionEditorDoc* CGlobalObj::pGetDocPtrOfFile(CString strTempName)
{
    CString strPath = "";
    CFunctionEditorDoc* pDocRet = NULL;
    CFunctionEditorDoc* pDoc = NULL;

    if (m_pEditorDocTemplate != NULL)
    {
        POSITION pos = m_pEditorDocTemplate->GetFirstDocPosition();

        while (pos && !pDocRet)
        {
            pDoc = static_cast<CFunctionEditorDoc*> (m_pEditorDocTemplate->GetNextDoc(pos));

            if (pDoc->IsKindOf(RUNTIME_CLASS(CFunctionEditorDoc)))
            {
                strPath = pDoc->GetPathName();

                if (!(strPath.Compare(strTempName)))
                {
                    pDocRet = pDoc;
                }
            }
        }
    }

    return pDocRet;
}
CFunctionEditorDoc* CGlobalObj::podGetFunctionEditorDoc()
{
    CFunctionEditorDoc* pDoc = NULL;
    //Get the active window frame and document attached to it
    CFrameWnd* pParent = (CFrameWnd*)CWnd::FromHandle(sm_hWndMDIParentFrame);
    CEditFrameWnd* pWnd = static_cast<CEditFrameWnd*> (pParent->GetActiveFrame());

    if (pWnd != NULL)
    {
        if (pWnd->IsKindOf(RUNTIME_CLASS(CEditFrameWnd)))
        {
            pDoc = static_cast<CFunctionEditorDoc*> (pWnd->GetActiveDocument());
        }
    }

    return pDoc;
}
CFunctionView* CGlobalObj::podGetFunctionViewPtr()
{
    CView* pTempView = NULL;
    CFunctionView* pFunctionView = NULL;
    BOOL bFound = FALSE;
    //Get the active document and find the CFunctionView attached to it
    CFunctionEditorDoc* pDoc = podGetFunctionEditorDoc();

    if (pDoc != NULL)
    {
        POSITION pos = pDoc->GetFirstViewPosition();

        while (pos && !bFound)
        {
            pTempView = pDoc->GetNextView(pos);

            if (pTempView->IsKindOf(RUNTIME_CLASS(CFunctionView)))
            {
                pFunctionView = static_cast<CFunctionView*> (pTempView);
                bFound = TRUE;
            }
        }
    }

    return pFunctionView;
}
CFnsTreeView* CGlobalObj::podGetFuncsTreeViewPtr()
{
    CFnsTreeView* pFnTreeView = NULL;
    CView* pTempView = NULL;
    BOOL bFound = FALSE;
    //Get the active document and find the FunctionTreeView attached to it
    CFunctionEditorDoc* pDoc = podGetFunctionEditorDoc();

    if (pDoc != NULL)
    {
        POSITION pos = pDoc->GetFirstViewPosition();

        while (pos && !bFound)
        {
            pTempView = pDoc->GetNextView(pos);

            if (pTempView->IsKindOf(RUNTIME_CLASS(CFnsTreeView)))
            {
                pFnTreeView = static_cast<CFnsTreeView*> (pTempView);
                bFound = TRUE;
            }
        }
    }

    return pFnTreeView;
}
BOOL CGlobalObj::bOpenFunctioneditorfile(CString omStrNewCFileName)
{
    BOOL bFileFound = TRUE;
    CFunctionEditorDoc* pDoc = CFunctionEditorBase::pCreateNewDocument(m_eBus);

    // file-attribute information
    if (pDoc != NULL)
    {
        CEditFrameWnd::sm_eBus = m_eBus;
        struct _tfinddata_t fileinfo;

        // Check if file exists
        if (_tfindfirst( omStrNewCFileName.GetBuffer(MAX_PATH), &fileinfo) == -1L)
        {
            bFileFound = pDoc->bCreateNewDocument(omStrNewCFileName);
        }

        if (bFileFound == TRUE)
        {
            //// Now open the selected file
            pDoc->OnOpenDocument(omStrNewCFileName);
            CMultiDocTemplate* pTemplate = m_pEditorDocTemplate;
            CEditFrameWnd* pNewFrame
                = static_cast<CEditFrameWnd*> (pTemplate->CreateNewFrame(pDoc, NULL));

            //If null is passed as parameter the m_pdoc->GetNextView(pos)  will
            // give null value
            if (pNewFrame != NULL)
            {
                ASSERT_KINDOF(CEditFrameWnd, pNewFrame);
                pTemplate->InitialUpdateFrame(pNewFrame, /*NULL*/pDoc);
            }
        }
    }

    return bFileFound;
}
CFileView* CGlobalObj::podGetFileViewPtr()
{
    CFileView* pFileView = NULL;
    CView* pTempView = NULL;
    BOOL bFound = FALSE;
    //Get the active document and find the CFileView attached to it
    CFunctionEditorDoc* pDoc = podGetFunctionEditorDoc();

    if (pDoc != NULL)
    {
        POSITION pos = pDoc->GetFirstViewPosition();

        while (pos && !bFound)
        {
            pTempView = pDoc->GetNextView(pos);

            if (pTempView->IsKindOf(RUNTIME_CLASS(CFileView)))
            {
                pFileView = static_cast<CFileView*> (pTempView);
                bFound = TRUE;
            }
        }
    }

    return pFileView;
}
BOOL CGlobalObj::bGetDefaultValue(eCONFIGDETAILS eParam, WINDOWPLACEMENT& sPosition)
{
    BOOL bSizeFound = TRUE;
    CRect omMainFrameSize;
    // Get Main Frame Size
    GetClientRect( sm_hWndMDIParentFrame, &omMainFrameSize );
    omMainFrameSize.NormalizeRect();

    switch( eParam )
    {
        case OUT_WND_PLACEMENT:
        {
            CRect omRect = omMainFrameSize;
            omRect.top   += static_cast<LONG> ( omRect.Height() *
                                                defCHILD_FRAME_PROP_H );
            omRect.right -= static_cast<LONG> ( omRect.right *
                                                defOUTPUT_WND_PROP_X );
            sPosition.rcNormalPosition = omRect;
            /*sPosition.rcNormalPosition.top -= 2 * omToolBarRect.Height();
            sPosition.rcNormalPosition.bottom -= 2 * omToolBarRect.Height();*/
        }
        break;

        case SIMSYS_WND_PLACEMENT:
        {
            CRect omRect = omMainFrameSize;
            // Reduce the size propotionally
            omRect.bottom -= (LONG)(omRect.Height() * defTX_SIM_WND_BOTTOM_MARGIN);
            omRect.right -= (LONG)(omRect.Width() * defTX_SIM_WND_RIGHT_MARGIN);
            // Update the size
            sPosition.rcNormalPosition = omRect;
        }
        break;

        default:
            ASSERT( FALSE );
            bSizeFound = FALSE;
            break;
    }

    return bSizeFound;
}

BOOL CGlobalObj::bIsExist(ETYPE_BUS eBus)
{
    return (sm_pThis[eBus] != NULL);
}

CGlobalObj& CGlobalObj::ouGetObj(ETYPE_BUS eBus)
{
    if (sm_pThis[eBus] == NULL)
    {
        sm_pThis[eBus] = new CGlobalObj(eBus);
    }

    return *(sm_pThis[eBus]);
}
void CGlobalObj::vClearAll(void)
{
    for (UINT i = 0; i < BUS_TOTAL; i++)
    {
        if (sm_pThis[i] != NULL)
        {
            DELETE_PTR(sm_pThis[i]);
        }
    }
}
CString CGlobalObj::omGetBusName(ETYPE_BUS eBus)
{
    CString omBusName;

    switch (eBus)
    {
        case CAN:
        {
            omBusName = _T("CAN Bus");
        }
        break;

        case J1939:
        {
            omBusName = _T("J1939 Bus");
        }
        break;
    }

    return omBusName;
}

CString CGlobalObj::omGetBusSpecMsgHndlrName(ETYPE_BUS eBus)
{
    CString omName;

    switch (eBus)
    {
        case CAN:
        {
            omName = defMESSAGE_HANDLER;
        }
        break;

        case J1939:
        {
            omName = defPGN_HANDLER;
        }
        break;
    }

    return omName;
}
CString CGlobalObj::omGetBusSpecMsgFieldName(ETYPE_BUS eBus)
{
    CString omName;

    switch (eBus)
    {
        case CAN:
        {
            omName = _T("Message");
        }
        break;

        case J1939:
        {
            omName = _T("PGN");
        }
        break;
    }

    return omName;
}
HRESULT CGlobalObj::RegisterNodeToDIL(BOOL bRegister, PSNODEINFO pNodeInfo)
{
    VALIDATE_POINTER_RETURN_VAL(pNodeInfo, S_FALSE);
    HRESULT hResult = S_FALSE;

    switch (m_eBus)
    {
        case CAN:
        {
            hResult = CGlobalObj::GetICANDIL()->DILC_RegisterClient(bRegister,
                      pNodeInfo->m_dwClientId,
                      pNodeInfo->m_omStrNodeName.GetBuffer(MAX_PATH));

            if ((hResult == S_OK) && (bRegister == TRUE))
            {
                //Set the buffer
                hResult = CGlobalObj::GetICANDIL()->DILC_ManageMsgBuf(MSGBUF_ADD,
                          pNodeInfo->m_dwClientId, &(pNodeInfo->m_ouCanBufFSE));
            }
        }
        break;

        case J1939:
        {
            hResult = CGlobalObj::GetIJ1939DIL()->DILIJ_RegisterClient(bRegister,
                      pNodeInfo->m_omStrNodeName.GetBuffer(MAX_CHAR),
                      pNodeInfo->m_unEcuName,
                      pNodeInfo->m_byPrefAddress,
                      pNodeInfo->m_dwClientId);

            if (((hResult == S_OK) || (hResult == ERR_CLIENT_EXISTS)) && (bRegister == TRUE))
            {
                //Set the buffer
                hResult = CGlobalObj::GetIJ1939DIL()->DILIJ_ManageMsgBuf(MSGBUF_ADD,
                          pNodeInfo->m_dwClientId, &(pNodeInfo->m_ouMsgBufVSE));
                GetIJ1939DIL()->DILIJ_SetCallBckFuncPtr(pNodeInfo->m_dwClientId,
                                                        CLBCK_FN_LDATA_CONF, (PCLBCK_FN_LDATA_CONF)sg_vDataConfEventFnJ1939);
                GetIJ1939DIL()->DILIJ_SetCallBckFuncPtr(pNodeInfo->m_dwClientId,
                                                        CLBCK_FN_BC_LDATA_CONF, (PCLBCK_FN_BC_LDATA_CONF)sg_vDataConfEventFnJ1939);
                GetIJ1939DIL()->DILIJ_SetCallBckFuncPtr(pNodeInfo->m_dwClientId,
                                                        CLBCK_FN_NM_ACL, (PCLBCK_FN_NM_ACL)sg_vAddressClaimEventFnJ1939);
            }
        }
        break;
    }

    return hResult;
}
