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
#if !defined(AFX_EXECUTEFUNC_H__5A542614_2BFB_11D6_BFE9_0010B599CE39__INCLUDED_)
#define AFX_EXECUTEFUNC_H__5A542614_2BFB_11D6_BFE9_0010B599CE39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "NodeSimEx_stdafx.h"
#include "HashDefines.h"
#include "SimSysNodeInfo.h"
#include "ExecuteManager.h"

typedef enum eNODEFLAG
{
    EXKEY_HANDLER=0,
    EXERROR_HANDLER,
    EXEVENT_HANDLER,
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
    BOOL bInitEventStructLIN(CStringArray& omErrorArray);
    BOOL bInitEventStructJ1939(CStringArray& omErrorArray);

    // Execute key handlers
    VOID vExecuteOnKeyHandler(UCHAR ucKey);
    // Execute message handlers
    VOID vExecuteOnMessageHandlerCAN(STCAN_TIME_MSG sRxMsgInfo);
    VOID vExecuteOnPGNHandler(void* pRxMsg);
    VOID vExecuteOnErrorHandler(eERROR_STATE eErrorCode, SCAN_ERR sErrorVal );
    VOID vExecuteOnErrorHandlerLIN( SERROR_INFO_LIN ouLinEventInfo );
    VOID vExecuteOnDataConfHandlerJ1939(UINT32 unPGN, BYTE bySrc, BYTE byDest, BOOL bSuccess);
    VOID vExecuteOnAddressClaimHandlerJ1939(BYTE byAddress);
    VOID vExecuteOnDLLHandler(eDLLHANDLER eDLLHandler);
    VOID vExecuteOnBusEventHandler(eBUSEVEHANDLER eBusEventHandler);

    // LIN handlers
    VOID vExecuteOnMessageHandlerLIN(STLIN_TIME_MSG sRxMsgInfo);

    // Initialise all structure
    BOOL bInitStruct(CStringArray& omErrorArray);
    // constructor
    CExecuteFunc(ETYPE_BUS eBus, CONST CString& omStrDllFileName);
    virtual ~CExecuteFunc();
    //ani1
    void vSetNodeInfo(PSNODEINFO ps_TempNodeInfo);
    void vGetNodeInfo(sNODEINFO& psNodeInfo) const;

    DWORD dwGetNodeClientId();

    void vInitialiseInterfaceFnPtrs(HMODULE);//from mainframe

    void vSetActivePassiveNode(BOOL ActiveNode); //used for Dll
    BOOL bEnableDisableMsgHandlers(BOOL bEnable );//frm main frm
    BOOL bEnableDisableErrorHandlers(BOOL bEnable);
    BOOL bEnableDisableEventHandlers(BOOL bEnable);
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
    void vWriteInQMsg(STCAN_TIME_MSG sRxMsgInfo);
    void vWriteInQMsgLIN(STLIN_TIME_MSG sRxMsgInfo);
    STLIN_TIME_MSG sReadFromQMsgLIN();

    STCAN_TIME_MSG sReadFromQMsg();
    CEvent m_omReadFromQEvent;       //event set after writing into buffer,for reading
    CEvent m_omReadFromQEventLIN;       //event set after writing into buffer,for reading
    CRITICAL_SECTION m_CritSectForFuncBuf; //critical section for buffer
    CRITICAL_SECTION m_CritSectForFuncBufLIN; //critical section for buffer
    UINT unGetBufferMsgCnt(); //called from read buffer thread
    UINT unGetBufferMsgCntLIN();
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
    PFMSG_HANDLER_LIN pFSearchMsgIdRangeHandlerLIN(UINT unMsgID);
    PFMSG_HANDLER pFSearchMsgIdRangeHandler(UINT unMsgId);
    PFMSG_HANDLER_CAN pFSearchMsgIdListHandlerCAN(UINT unMsgId);
    PFMSG_HANDLER_LIN pFSearchMsgIdListHandlerLIN(UINT unMsgId);
    BOOL bInitMsgIDRangeHandlStruct(UINT unMsgIDRangeCount,
                                    CStringArray& omErrorArray);
    BOOL bInitMsgListHandleStruct(UINT  unMsgIDandNameCount,
                                  CStringArray& omErrorArray);
    BOOL bInitMsgIDandNameHandlStruct(UINT unMsgIDandNameCount,
                                      CStringArray& omErrorArray);
    // Allocate memory for handler info structure
    BOOL bAllocateMemory(UINT unMsgRangeCount );
    BOOL bAllocateMemoryForMsgListHandler(UINT unMsgListCount );
    void vCatagoriseMsgHandlers(const CString& omStrFuncName);
    // Initialise timer structure
    BOOL bInitTimerStruct(CStringArray& omErrorArray);
    // Initialise timer structure
    BOOL bInitOnKeyStruct(CStringArray& omErrorArray);
    // Initialise timer structure
    BOOL bInitMSGStruct(CStringArray& omErrorArray);
    // Initialise DLL structure
    BOOL bInitDLLStruct(CStringArray& omErrorArray);
    BOOL bInitBusEventStruct(CStringArray& omErrorArray);
    // Read .Def File and add function name.
    BOOL bReadDefFile(CStringArray& omErrorArray);
    void vTokenize(CString strInput, CString strToken, CString& strOutput, int& nStart);

    void vInitialiseInterfaceFnPtrsJ1939(HMODULE);
    void vInitialiseInterfaceFnPtrsCAN(HMODULE);
    void vInitialiseInterfaceFnPtrsLIN(HMODULE);
    void vExecuteOnEventHandlerJ1939(UINT32 unPGN, BYTE bySrc, BYTE byDest, BOOL bSuccess, BYTE byType);
    CStringArray m_omStrArrayTimerHandlers;
    CStringArray m_omStrArrayKeyHandlers;
    CStringArray m_omStrArrayMsgHandlers;
    CStringArray m_omStrArrayErrorHandlers;
    CStringArray m_omStrArrayEventHandlers;
    CStringArray m_omStrArrayDLLHandlers;
    CStringArray m_omStrArrayBusEventHandler;

    PSKEYHANDLER  m_psOnKeyHandlers;

    PSMSGINFO_FOR_HANDLER  m_psMsgHandlersInfo;
    // Cmap for message ID & Message Handler Data association
    CMap<int,int, SMSGHANDLERDATA_CAN, SMSGHANDLERDATA_CAN> m_omMsgHandlerMapCAN;
    CMap<int,int, SMSGHANDLERDATA_LIN, SMSGHANDLERDATA_LIN> m_omMsgHandlerMapLIN;
    CMap<int,int, SMSGHANDLERDATA, SMSGHANDLERDATA> m_omMsgHandlerMap;
    PSMSGID_RANGE_HANDLER_CAN m_psOnMsgIDRangeHandlersCAN;
    PSMSGID_RANGE_HANDLER_LIN m_psOnMsgIDRangeHandlersLIN;
    PSMSGID_RANGE_HANDLER m_psOnMsgIDRangeHandlers;
    PSMSGID_LIST_HANDLER_CAN m_psOnMsgIDListHandlersCAN;
    PSMSGID_LIST_HANDLER_LIN m_psOnMsgIDListHandlersLIN;
    PFMSG_HANDLER_CAN m_pFGenericMsgHandlerCAN;
    PFMSG_HANDLER_LIN m_pFGenericMsgHandlerLIN;
    PFMSG_HANDLER m_pFGenericMsgHandler;
    PSDLLHANDLER   m_psOnDLLHandlers ;
    PSBUSEVHANDLER m_psOnBusEventHandlers;
    PSEVENTHANDLER m_psOnEventHandlers;
    PSERRORHANDLER m_psOnErrorHandlers;

    PSEVENTHANDLERLIN m_psOnEventHandlersLin;
    CString m_omStrDllFileName;

    HMODULE m_hDllModule;
    CString m_omStrGenericHandlerName;
    CStringArray m_omStrArrayMsgRange;
    CStringArray m_omStrArrayMsgIDandName;
    CStringArray m_omStrArrayMsgList;

    BOOL m_bIsStatWndCreated;


    sNODEINFO m_sNodeInfo;
    UINT m_unQMsgCount;
    UINT m_unQMsgCountLIN;
    STCAN_TIME_MSG m_asQMsg[ defMAX_FUNC_MSG ];     //for message handler msg array
    STLIN_TIME_MSG m_asQMsgLIN[ defMAX_FUNC_MSG ];     //for message handler msg array

    UINT m_unReadQMsgIndex;         // index from which the message to be read
    UINT m_unWriteQMsgIndex;        //index at which message is to be written


    UINT m_unReadQMsgIndexLIN;         // index from which the message to be read
    UINT m_unWriteQMsgIndexLIN;        //index at which message is to be written

    BOOL m_bDllLoaded;
    BOOL m_bStopKeyHandlers;
    BOOL m_bStopErrorHandlers;
    BOOL m_bStopEventHandlers;
    BOOL m_bStopDLLHandlers;
    CWinThread* m_pomMsgHandlerThrd;
    CWinThread* m_pomMsgHandlerThrdLIN;
    PSTIMERHANDLERLIST m_psFirstTimerStrList;
    PSTIMERHANDLERLIST m_psLastTimerStrList;
    BOOL m_bTimerThreadStarted;
    //to start stop message transmission from DLL
    BOOL m_bMsgTxOnFlag;
};

#endif // !defined(AFX_EXECUTEFUNC_H__5A542614_2BFB_11D6_BFE9_0010B599CE39__INCLUDED_)
