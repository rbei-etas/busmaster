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
 * \file      GlobalObj.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "NodeSimEx_Struct.h"
#include "FileView.h"
#include "SimSysWnd.h"
#include "FnsTreeView.h"
#include "FunctionView.h"
#include "FunctionEditorDoc.h"
#include "Include/BaseDefs.h"
#include "DIL_Interface/BaseDIL_Can.h"
#include "DIL_Interface/BaseDIL_J1939.h"
class CBaseAppServices;

class CGlobalObj
{
private:
    ETYPE_BUS m_eBus;
    CGlobalObj(ETYPE_BUS eBus);
    static CGlobalObj* sm_pThis[BUS_TOTAL];
    static CBaseDIL_CAN* sm_pouDilCanInterface;
public:

    ~CGlobalObj(void);

    CMultiDocTemplate* m_pEditorDocTemplate;
    CSimSysWnd* m_pomSimSysWnd;           // Simulated system window pointer
    CString m_omStrSourceFilePathName;
    CString m_omAppDirectory;
    CStringArray m_ArrAPIsList;
    CStringArray m_ArrAPIFuncNames;
    CString m_omMsgStructName;
    CString m_omMsgStructFile;
    CStringArray m_omErrorHandlerList;
    CString m_omObjWrapperName;
    CStringArray m_omDefinedMsgHeaders;
    CMsgNameMsgCodeList m_odMsgNameMsgCodeList;
    CMapStringToPtr* m_podNodeToDllMap;
    //static CKeyPanelEntryList* g_podKeyPanelEntryList;
    WINDOWPLACEMENT m_wWindowPlacement;
    CFunctionEditorDoc* pGetDocPtrOfFile(CString);
    static CBaseDIL_CAN* GetICANDIL(void);
    static CBaseDILI_J1939* GetIJ1939DIL(void);
    CFunctionEditorDoc* podGetFunctionEditorDoc();
    CFunctionView* podGetFunctionViewPtr();
    CFnsTreeView* podGetFuncsTreeViewPtr();
    BOOL bOpenFunctioneditorfile(CString);
    CFileView* podGetFileViewPtr();
    HRESULT RegisterNodeToDIL(BOOL bRegister, PSNODEINFO pNodeInfo);
    void vCreateTraceWnd();
    BOOL bGetDefaultValue(int eParam, WINDOWPLACEMENT& sPosition);
    TRANSMIT_MSG m_pfTransmitMsg;
    EN_DIS_ABLE_LOG m_pfEnableDisableLog;
    DISCONNECT_TOOL m_pfDisconnectTool;
    WRITE_TO_LOG m_pfWriteToLog;

    //Store the help text for handler functions of function editor
    CStringArray m_omFnEditHelpText;
    //Store the SimSys Config detail data
    BYTE* m_pSimSysDataPtr;
    int   m_nSimSysDataSize;
    //static variables
    static CBaseAppServices* sm_pouITraceWndPtr;
    static HWND sm_hWndMDIParentFrame;

    //Static functions
    static BOOL bIsExist(ETYPE_BUS eBus);
    static CGlobalObj& ouGetObj(ETYPE_BUS eBus);
    static void vClearAll(void);
    static CString omGetBusSpecMsgHndlrName(ETYPE_BUS eBus);
    static CString omGetBusSpecMsgFieldName(ETYPE_BUS eBus);
    static CString omGetBusName(ETYPE_BUS eBus);
};
