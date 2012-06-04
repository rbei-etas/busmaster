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
 * \file      TSExecutorGUI_ChildFrame.h
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "TreeViewEx.h"
#include "TSExecutorLIB.h"
#include "ResultView.h"
#include "Utility/Utility_Thread.h"
#include "TestSetupEditorLib/TestSetupEntity.h"
#include "TestSetupEditorLib/DataBaseMsgList.h"

#define szFilter    "TestSetup Files (*.xml)|*.xml||"


class CTSExecutorChildFrame : public CMDIChildWnd
{
    DECLARE_DYNCREATE(CTSExecutorChildFrame)
    CBaseEntityTA *m_pCurrentEntity;
    DWORD m_dwCurrentEntityID;
    HTREEITEM m_hCurrentTreeItem;
    CDataBaseMsgList m_odDataBaseManager;
    CString m_omCurrentTSFile;
    BOOL    m_bFileSaved;
    WINDOWPLACEMENT m_sTSDefPlacement;
    CPARAM_THREADPROC m_ouExecutionThread;
    CSemaphore m_ouExecutionSync;
    BOOL m_bConnected;
    CSplitterWnd m_omSplitterWnd;
    CImageList  *m_pomImageList;
	CString m_strVersionInfo;
public:
    CTSExecutorLIB m_ouTSExecutor;
    BOOL m_bModified;
    CTreeViewEx *m_odTreeView;
    CResultView *m_odResultView;
    HTREEITEM m_hParentTreeItem;
    BOOL m_bPasted;

//Member Functions
public:
    CTSExecutorChildFrame(void);
    HRESULT GetConfigurationData(BYTE*& pDesBuffer, UINT& nBuffSize);
    HRESULT SetConfigurationData(BYTE* pSrcBuffer, UINT unBuffSize);
    VOID vEnableItem(DWORD dwID, BOOL& bEnable);
    VOID vSetBusStatus(BOOL bConnected);
	VOID vSetBUSMASTERVersionInfo(CString strVersion);
    BOOL bGetBusStatus(void);
    UINT unRepisitonEntry(DWORD dwRepositionItemID, DWORD dwInsertAfterItemID);

protected:
	// protected constructor used by dynamic creation
	virtual ~CTSExecutorChildFrame(void);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
    DECLARE_MESSAGE_MAP()

private:
    VOID vInitialise(void);
    VOID vUpdateTreeView(void);
    BOOL bParseTestSetup(INT nIndex);
    
//Message Handlers
public:
    afx_msg void OnDestroy(void);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg HRESULT OnAddTestSetup(WPARAM wparam, LPARAM lparam);
    afx_msg HRESULT OnDeleteTestSetup(WPARAM wparam, LPARAM lparam);
    afx_msg HRESULT OnUpdate(WPARAM wparam, LPARAM lparam);
    afx_msg HRESULT OnExecute(WPARAM wparam, LPARAM lparam);
    afx_msg HRESULT OnExecutionOver(WPARAM wparam, LPARAM lparam);
    afx_msg void OnClose();
};
