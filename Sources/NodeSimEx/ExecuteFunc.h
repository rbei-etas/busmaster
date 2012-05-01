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
 * \file      ExecuteFunc.h
 * \brief     This header file contain definition ofCExecuteFunc class.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contain definition ofCExecuteFunc class.
 */

#pragma once

#include "NodeSimEx_stdafx.h"
#include "HashDefines.h"
#include "SimSysNodeInfo.h"
#include "ExecuteManager.h"

typedef enum eNODEFLAG
{
    EXKEY_HANDLER=0,
    EXERROR_HANDLER,
    EXDLL_HANDLER,
    EXMSG_HANDLER,
    EXTIMER_HANDLER,
    ACTIVENODE
};


class CExecuteFunc
{
public:
    void vEnableDisableAllTimers(BOOL);
    BOOL bInitErrorStruct(CStringArray& omErrorArray);
    BOOL bInitEventStructJ1939(CStringArray& omErrorArray);

    // Execute key handlers
    VOID vExecuteOnKeyHandler(UCHAR ucKey);
    // Execute message handlers
    VOID vExecuteOnMessageHandlerCAN(STCAN_MSG sRxMsgInfo);
    VOID vExecuteOnPGNHandler(void* pRxMsg);
    VOID vExecuteOnErrorHandler(eERROR_STATE eErrorCode, SCAN_ERR sErrorVal );
    VOID vExecuteOnDataConfHandlerJ1939(UINT32 unPGN, BYTE bySrc, BYTE byDest, BOOL bSuccess);
    VOID vExecuteOnAddressClaimHandlerJ1939(BYTE byAddress);
    VOID vExecuteOnDLLHandler(eDLLHANDLER eDLLHandler);

    // Initialise all structure
    BOOL bInitStruct(CStringArray& omErrorArray);
    // constructor
    CExecuteFunc(ETYPE_BUS eBus, CONST CString& omStrDllFileName);
    virtual ~CExecuteFunc();
    //ani1
    void vSetNodeInfo(PSNODEINFO ps_TempNodeInfo);
    void vGetNodeInfo(sNODEINFO& psNodeInfo) const;
    void vInitialiseInterfaceFnPtrs(HMODULE);//from mainframe

    void vSetActivePassiveNode(BOOL ActiveNode); //used for Dll
    BOOL bEnableDisableMsgHandlers(BOOL bEnable );//frm main frm
    BOOL bEnableDisableErrorHandlers(BOOL bEnable);
    BOOL bActivateDeactivateHandlers(BOOL bActivate);
    BOOL bEnableDisableKeyHandlers(BOOL bEnable);
    HMODULE hGetDllHandle();
    BOOL bGetFlagStatus(eNODEFLAG eWhichFlag);
    BOOL bUnloadDll();//it is called from BL & UL  to stop handler execution
    CString omGetPrevFileName();
    void vSetDllHandle(HMODULE hModuleHandle);
    VOID vDestroyUtilityThreads(UINT unMaxWaitTime, BYTE byThreadCode);
    STHREADINFO m_asUtilThread[defEVENT_EXFUNC_TOTAL];
    CEvent  m_aomState[defEVENT_EXFUNC_TOTAL];
    //associated to handler thread
    void vWriteInQMsg(STCAN_MSG sRxMsgInfo);
    STCAN_MSG sReadFromQMsg();
    CEvent m_omReadFromQEvent;       //event set after writing into buffer,for reading
    CRITICAL_SECTION m_CritSectForFuncBuf; //critical section for buffer
    UINT unGetBufferMsgCnt(); //called from read buffer thread
    BOOL bIsDllLoaded() ;
    //made public to make it easily accessed by read msg handler thread
    BOOL m_bStopMsgHandlers;
    BOOL bIsTimerThreadStarted();
    //start execution of timer handlers
    void vStartTimerThreads();
    //the timer information are copied from linked list manage for
    //UI part
    void vResetTimerStructure(PSTIMERHANDLERLIST psFirstTimerPointer);
    //destroy timer threads at unloading of DLL
    void vDestroyTimerThreads();
    //called when timer is set reset from DLL
    BOOL bResetTimer(char* pcTimerFunctionName, int type, BOOL bStart);
    //called when timervalue is reset from DLL
    BOOL bResetTimerVal(char* pcTimerFunctionName,UINT unTimeVal);
    //checking message transmission flag
    BOOL bGetMsgTxFlag();
    void vSetMsgTxFlag(BOOL);
    //get the pointer to the Timer structure list
    const PSTIMERHANDLERLIST psGetTimerListPtr();
    //provide handler details to UI part
    void vCopyHandlersArrayToUI(PSNODEINFO psNodeInfo);



private:
    ETYPE_BUS m_eBus;
    PFKEY_HANDLER m_pFGenericKeyHandler;
    PFMSG_HANDLER_CAN pFSearchMsgIdRangeHandlerCAN(UINT unMsgID);
    PFMSG_HANDLER pFSearchMsgIdRangeHandler(UINT unMsgId);
    BOOL bInitMsgIDRangeHandlStruct(UINT unMsgIDRangeCount,
                                    CStringArray& omErrorArray);
    BOOL bInitMsgIDandNameHandlStruct(UINT unMsgIDandNameCount,
                                      CStringArray& omErrorArray);
    // Allocate memory for handler info structure
    BOOL bAllocateMemory(UINT unMsgRangeCount );
    void vCatagoriseMsgHandlers(const CString& omStrFuncName);
    // Initialise timer structure
    BOOL bInitTimerStruct(CStringArray& omErrorArray);
    // Initialise timer structure
    BOOL bInitOnKeyStruct(CStringArray& omErrorArray);
    // Initialise timer structure
    BOOL bInitMSGStruct(CStringArray& omErrorArray);
    // Initialise DLL structure
    BOOL bInitDLLStruct(CStringArray& omErrorArray);
    // Read .Def File and add function name.
    BOOL bReadDefFile(CStringArray& omErrorArray);

    void vInitialiseInterfaceFnPtrsJ1939(HMODULE);
    void vInitialiseInterfaceFnPtrsCAN(HMODULE);
    void vExecuteOnEventHandlerJ1939(UINT32 unPGN, BYTE bySrc, BYTE byDest, BOOL bSuccess, BYTE byType);
    CStringArray m_omStrArrayTimerHandlers;
    CStringArray m_omStrArrayKeyHandlers;
    CStringArray m_omStrArrayMsgHandlers;
    CStringArray m_omStrArrayErrorHandlers;
    CStringArray m_omStrArrayEventHandlers;
    CStringArray m_omStrArrayDLLHandlers;

    PSKEYHANDLER  m_psOnKeyHandlers;

    PSMSGINFO_FOR_HANDLER  m_psMsgHandlersInfo;
    // Cmap for message ID & Message Handler Data association
    CMap<int,int, SMSGHANDLERDATA_CAN, SMSGHANDLERDATA_CAN> m_omMsgHandlerMapCAN;
    CMap<int,int, SMSGHANDLERDATA, SMSGHANDLERDATA> m_omMsgHandlerMap;
    PSMSGID_RANGE_HANDLER_CAN m_psOnMsgIDRangeHandlersCAN;
    PSMSGID_RANGE_HANDLER m_psOnMsgIDRangeHandlers;
    PFMSG_HANDLER_CAN m_pFGenericMsgHandlerCAN;
    PFMSG_HANDLER m_pFGenericMsgHandler;
    PSDLLHANDLER   m_psOnDLLHandlers ;
    PSEVENTHANDLER m_psOnEventHandlers;
    PSERRORHANDLER m_psOnErrorHandlers;
    CString m_omStrDllFileName;

    HMODULE m_hDllModule;
    CString m_omStrGenericHandlerName;
    CStringArray m_omStrArrayMsgRange;
    CStringArray m_omStrArrayMsgIDandName;


    BOOL m_bIsStatWndCreated;


    sNODEINFO m_sNodeInfo;
    UINT m_unQMsgCount;
    STCAN_MSG m_asQMsg[ defMAX_FUNC_MSG ];      //for message handler msg array
    UINT m_unReadQMsgIndex;         // index from which the message to be read
    UINT m_unWriteQMsgIndex;        //index at which message is to be written



    BOOL m_bDllLoaded;
    BOOL m_bStopKeyHandlers;
    BOOL m_bStopErrorHandlers;
    BOOL m_bStopDLLHandlers;
    CWinThread* m_pomMsgHandlerThrd;
    PSTIMERHANDLERLIST m_psFirstTimerStrList;
    PSTIMERHANDLERLIST m_psLastTimerStrList;
    BOOL m_bTimerThreadStarted;
    //to start stop message transmission from DLL
    BOOL m_bMsgTxOnFlag;
};
