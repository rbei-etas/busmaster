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
 * \file      ExecuteManager.h
 * \brief     This header file contain definition ofCExecuteManager class.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contain definition ofCExecuteManager class.
 */
#if !defined(AFX_EXECUTEMANAGER_H__CF19BB37_7172_41A9_A2E0_72558E722223__INCLUDED_)
#define AFX_EXECUTEMANAGER_H__CF19BB37_7172_41A9_A2E0_72558E722223__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BuildProgram.h"
#include "SetResetTimer.h"
#include "ExecuteFunc.h"


class CExecuteManager  
{

public:
	CBuildProgram* m_pouBuildProgram;
	virtual ~CExecuteManager();
	static CExecuteManager& ouGetExecuteManager(ETYPE_BUS eBus);
    static BOOL bIsExist(ETYPE_BUS eBus);
    static void vClearObj(ETYPE_BUS eBus);
	void vAddNode (const PSNODEINFO psNodeInfo,CExecuteFunc* pExecuteFunc);
	BOOL vDeleteNode(const CString omStrNodeName);
	//void vDeleteAllNode(void);
	void vEnableDisableAllHandlers(BOOL bState);
	void vEnableDisableNodeHandlers(const PSNODEINFO psNodeInfo,BOOL bState);
	void vEnableDisableAllTimers(BOOL bState);
	void vEnableAllKeyHandler(BOOL bState);
	void vEnableAllMessageHandler(BOOL bState);
	void vEnableAllErrorHandler(BOOL bState);
	void vEnableNodeKeyHandler(const PSNODEINFO psNodeInfo,BOOL bState);
	void vEnableNodeMessageHandler(const PSNODEINFO psNodeInfo,BOOL bState);
	void vEnableNodeErrorHandler(const PSNODEINFO psNodeInfo,BOOL bState);
	void vEnableNodeTimerHandler(const PSNODEINFO psNodeInfo,BOOL bState);
	void vManageOnKeyHandler(UCHAR ucKey);
	void vManageOnMessageHandlerCAN(STCAN_MSG sRxMsgInfo, DWORD dwClientId);
	void vManageOnErrorHandlerCAN(eERROR_STATE eErrorCode,SCAN_ERR sErrorVal, DWORD dwClientId);
    void vManageOnDataConfHandlerJ1939(DWORD dwClientId, UINT32 unPGN, 
                                            BYTE bySrc, BYTE byDest, BOOL bSuccess);
    void vManageOnAddressClaimHandlerJ1939(DWORD dwClientId, BYTE byAddress);
	void vExSetNodeInfo(const CString NodeName ,const PSNODEINFO psNodeInf);
	const HMODULE hReturnDllHandle(const CString NodeName);
	BOOL bDllLoaded;//if any one dll is loaded
	BOOL bExecuteDllBuildLoad(PSNODEINFO psNodeInfo);
	BOOL bExecuteDllBuild(PSNODEINFO psNodeInfo);
	BOOL bExecuteDllUnload(PSNODEINFO psNodeInfo);//It can be called for delete system
	BOOL bExecuteDllLoad(PSNODEINFO psNodeInfo);
	//manage starting of timers
	void vManageTimerThreads();
	//provide the node object list to reset the timer structures of the CExecuteFunc
	//objects whose timer info is changed by user
	const PSNODEOBJECT psGetNodeObjectList();
	//return CExecuteFunc pointer for API's
	CExecuteFunc* pmGetNodeObject(HMODULE hModule);
	//manage the message handlers for messages coming from DLL
	void vManageDllMessageHandler(SDLL_MSG sDllMessages);
	//Destroy timer dialog
	void vDestroyTimerDialog();

	//On clicking timer handler
	VOID vSetResetOnTimerHandler(const CStringArray& omSysNode,BOOL bFromSimSys);
	//deleting all node application is closed
	void vDeleteAllNode();
	//Copy the timer informations from each node to the local memory of 
	//CSetResetTimer
	void vCopyTimersInSetResetMemory(PSNODETIMERLIST psSRsetTimers);
	//Update Timer info from CSetReset to each node if user has changed
	//timers value
	void vUpdateTimersFromUser(PSNODETIMERLIST psSRsetTimers);
	//For updating all timer enable/disable from CExecuteFunc to CSetReset
	void vSetResetNodeTimers(CExecuteFunc* m_pCexecuteFunc,BOOL bEnable);
	//clear output window
	void vClearOutputWnd();
	//start dll read thread
	void vStartDllReadThread();
	//stop threads
	void vStopThreads();
	void vStopTimer();
	void vStartTimer();
    void vManageTimerExecution();
    //Building and loading of all the nodes
    BOOL bDLLBuildAll(CStringArray *pomStrErrorFiles);
    BOOL bDllLoadAll(CStringArray *pomStrErrorFiles); 
    BOOL bDllUnloadAll(CStringArray *pomStrErrorFiles);
    BOOL bDLLBuildLoadAll(CStringArray *pomStrErrorFiles);
    ///////////////////
    BOOL bActivateDeactivateHandlers(bool bActive,HMODULE hModule);
    void vUpdateHandlerDetailsInDetView();
    CExecuteFunc* pouGetExecuteFunc(DWORD dwClient);
    CEvent m_omDllMsgEvent;
    ETYPE_BUS m_eBus;
private:
	CExecuteManager(ETYPE_BUS eBus);
	static CExecuteManager* sm_pouManager[BUS_TOTAL];
	PSNODEOBJECT m_psFirstNodeObject;
    PSNODEOBJECT m_psLastNodeObject;
	//CCriticalSection m_omCritiSec;
	CSetResetTimer m_odSetResetTimer;
    
    CWinThread* m_pomReadDllThrd;
    //event for terminating read dll msg thread    
    CRITICAL_SECTION m_CritSectPsNodeObject;
};

#endif // !defined(AFX_EXECUTEMANAGER_H__CF19BB37_7172_41A9_A2E0_72558E722223__INCLUDED_)
