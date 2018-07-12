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

#pragma once

#include "BuildProgram.h"
#include "SetResetTimer.h"
#include "ExecuteFunc.h"
#include "GlobalObj.h"
#define defMAX_NO_OF_HANDLES 16 //Max no. of Nodes = 15 + Event to trigger on 
//add or remove Node.


class CExecuteManager
{

public:
    CExecuteManager(ETYPE_BUS eBus, CGlobalObj* pGlobalObj, CSimSysManager* pSimSysMgr);
    ESTATUS_BUS m_eBusStatus;
    CBuildProgram* m_pouBuildProgram;
    virtual ~CExecuteManager();
    static CExecuteManager& ouGetExecuteManager(ETYPE_BUS eBus, CGlobalObj* pGlobalObj);
    static BOOL bIsExist(ETYPE_BUS eBus);
    static void vClearObj(ETYPE_BUS eBus);
    void vAddNode (const PSNODEINFO psNodeInfo,CExecuteFunc* pExecuteFunc);
    BOOL vDeleteNode(const CString omStrNodeName);
    bool bSetNodeState(const PSNODEINFO psNodeInfo, eNODE_STATE eNodeState);
    void vEnableDisableAllTimers(BOOL bState);
    void vManageOnKeyHandler(UCHAR ucKey);
    void vManageBusEventHandler(eBUSEVEHANDLER eBusEvent);
    void vManageOnMessageHandlerCAN_(PSTCAN_TIME_MSG sRxMsgInfo, DWORD& dwClientId);
    void vManageOnErrorHandlerCAN(ERROR_STATE eErrorCode,SCAN_ERR sErrorVal, DWORD dwClientId);
    void vManageOnErrorHandlerLIN(SERROR_INFO_LIN ouErrorInfo, DWORD dwClientId);
    void vManageOnDataConfHandlerJ1939(DWORD dwClientId, UINT32 unPGN,
                                       BYTE bySrc, BYTE byDest, BOOL bSuccess);
    void vManageOnAddressClaimHandlerJ1939(DWORD dwClientId, BYTE byAddress);
    void vExSetNodeInfo(const CString NodeName ,const PSNODEINFO psNodeInf);

    void vManageOnMessageHandlerLIN(PSTLIN_TIME_MSG sRxMsgInfo, DWORD& dwClientId);

    const HMODULE hReturnDllHandle(const CString NodeName);
    BOOL bDllLoaded;//if any one dll is loaded
    BOOL bExecuteDllBuildLoad(PSNODEINFO psNodeInfo,BOOL bDisplaySuccessful);
    BOOL bExecuteDllBuild(PSNODEINFO psNodeInfo,BOOL bDisplaySuccessful);
    BOOL bExecuteDllUnload(PSNODEINFO psNodeInfo);//It can be called for delete system
    BOOL bExecuteDllLoad(PSNODEINFO psNodeInfo,BOOL bDisplaySuccessful);
    //manage starting of timers
    void vManageTimerThreads();
    //provide the node object list to reset the timer structures of the CExecuteFunc
    //objects whose timer info is changed by user
    const PSNODEOBJECT psGetNodeObjectList();
    //return CExecuteFunc pointer for API's
    CExecuteFunc* pmGetNodeObject(HMODULE hModule);
    //manage the message handlers for messages coming from DLL
    void vManageDllMessageHandler(SDLL_MSG sDllMessages);
    //On clicking timer handler
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
    BOOL bDLLBuildAll();
    BOOL bDllLoadAll(CStringArray* pomStrErrorFiles);
    BOOL bDllUnloadAll(CStringArray* pomStrErrorFiles);
    BOOL bDLLBuildLoadAll(CStringArray* pomStrErrorFiles);
    virtual BOOL bDLLBuildLoadAllEnabled();
    BOOL bDLLUnloadAllEnabled();

    std::string strGetFilePath(HANDLE hDir);
    CExecuteFunc* pouGetExecuteFunc(DWORD dwClient);
    CEvent m_omDllMsgEvent;
    ETYPE_BUS m_eBus;
    HANDLE arrhDirNotify[defMAX_NO_OF_HANDLES];
    std::map<HANDLE,std::string> m_mapHandleFilePath;
protected:
    CRITICAL_SECTION m_CritSectPsNodeObject;
    PSNODEOBJECT m_psFirstNodeObject;
    CSimSysManager* m_pSimSysMgr;
private:

    virtual CExecuteFunc* vCreateExecuteFunc(CString);
    static CExecuteManager* sm_pouManager[BUS_TOTAL];

    PSNODEOBJECT m_psLastNodeObject;
    //CCriticalSection m_omCritiSec;
    CSetResetTimer m_odSetResetTimer;
    CWinThread* m_pomReadDllThrd;
    //event for terminating read dll msg thread

    CPARAM_THREADPROC m_MonitorNodeFileThreadProc;
    HANDLE m_hThread;

    CGlobalObj* m_pGlobalObj;
};
