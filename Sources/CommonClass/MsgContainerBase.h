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
 * \file      MsgContainerBase.h
 * \brief     Definition of MCsgContainer class
 * \author    Anish kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of MCsgContainer class
 */

#pragma once

#include "Utility/Utility_Thread.h"
#include "include/BaseDefs.h"

typedef void (*MSG_RX_CALL_BK)(void* pParam, ETYPE_BUS eBusType);

class CMsgContainerBase
{

protected:
    CMsgContainerBase(void);
    CPARAM_THREADPROC       m_sDataCopyThread;
    MSG_RX_CALL_BK          m_pRxMsgCallBack;

public:
    ~CMsgContainerBase(void);
    virtual void InitTimeParams(void) = 0;
	virtual void vRetrieveDataFromBuffer() = 0;

	virtual void vInit(void* pParam) = 0;
    virtual void vEditClearAll() = 0;
    virtual int nGetAppendBufferCount() = 0;
    virtual int nGetOWBufferCount() = 0;
    virtual BOOL bStartReadThread() = 0;        

    virtual void vGetUpdatedCurrDataPtrArray(SMSGWNDHDRCOL &sHdrColStruct,
                                     TCHAR *pomDataPtrArr[MAX_MSG_WND_COL_CNT], 
                                     BYTE bExprnFlag_Disp) = 0;
    virtual HRESULT hReadFromOWBuffer(void *psMsg, __int64 nMapIndex) = 0;
	virtual HRESULT hReadFromAppendBuffer(void *psMsg, int nMsgIndex) = 0;
    virtual HRESULT hUpdateFormattedMsgStruct(int nListIndex, int &nMsgCode,
                                      BYTE bExprnFlag_Disp, __int64 nTimeOffset = 0) = 0;
    virtual void vSetCurrMsgName(CString strMsgNameOrCode) = 0;
    virtual void vClearFormattedMsgStruct() = 0;
    virtual void vSaveOWandGetDetails(void *pMsg, __int64 &dwMapIndex, 
                              __int64 &dwTimeStamp, UINT &nMsgCode, int &nBufferIndex ) = 0;

	virtual void SetClientID(DWORD dwClientID) = 0;
	virtual void DoSortBuffer(int nField,bool bAscending) = 0;
	virtual void GetMapIndexAtID(int nIndex,__int64& nMapIndex) = 0;
    virtual HRESULT ApplyFilterScheme(void* pvFilterApplied) = 0;
    virtual HRESULT GetFilterScheme(void* pvFilterApplied) = 0;
    virtual HRESULT EnableFilterApplied(BOOL bEnable) = 0;
	virtual BOOL bGetDilInterFace() = 0;	
	virtual __int64 nCreateMapIndexKey(LPVOID pMsgData) = 0;
	virtual HRESULT hToggleDILBufferRead(BOOL bRead) = 0;

public:
    //Exported functions
	void vSetRxMsgCallBkPtr(MSG_RX_CALL_BK pFuncPtr);
	BOOL bStartReadThread(HANDLE hActionEvent);
	BOOL bStopReadThread();
	BOOL bCopyStringToTCHARArr(TCHAR acDesStr[], CString omSrc, int MaxDesLen);
};
