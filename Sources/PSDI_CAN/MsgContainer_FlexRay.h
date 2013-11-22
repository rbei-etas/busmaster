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
 * \file      MsgContainer_CAN.h
 * \brief     Definition of CMsgContainerCAN class
 * \authors   Saravanan K S
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of CMsgContainerFlexRay class
 */

#pragma once

#include "afxtempl.h"
#include "Utility/Utility_Thread.h"
#include "Application/MsgWndHdrInfo.h"
#include "CommonClass/GUI_FormatMsgFlexRay.h"
#include "include/BaseDefs.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/MsgBufCANVFSE.h"
#include "DataTypes/MsgBufVVSE.h"
#include "DataTypes/Filter_Datatypes.h"
#include "CommonClass/RefTimeKeeper.h"
#include "CommonClass/MsgContainerBase.h"
#include "DIL_Interface/BaseDIL_FLEXRAY.h"
#include "DIL_Interface/DIL_Interface_extern.h"

#include "../DataTypes/MsgBufVSE.h"

typedef CMsgBufCANVFSE<s_FLXMSG> CFLEXBufVFSE; // <to do - To rename CMsgBufCANVFSE to make it generic>
typedef void (*MSG_RX_CALL_BK)(void* pParam, ETYPE_BUS eBusType);

// To save delta time in case of append mode
typedef struct stFlexRayDataSpl : public s_FLXMSG
{
    __int64      m_nDeltime;

    stFlexRayDataSpl()
    {
        m_nDeltime = 0;
    }
} STFLEXRAYDATASPL;

typedef CMsgBufCANVFSE<STFLEXRAYDATASPL> CMsgBufFLEXBufVFSEspl;

class CMsgContainerFlexRay : public CMsgContainerBase
{
private:
    CFLEXBufFSE             m_ouVSEBufFlexRay;
    CFLEXBufVFSE            m_ouOWFlexRayBuf;
    CMsgBufFLEXBufVFSEspl   m_ouAppendFlexRayBuf;
    s_FLXMSG                m_FlexRayData;
    FLXMSGDATA              m_sOutFormattedData;
    DWORD                   m_dwClientId;
    STFLEXRAYDATASPL        m_sFlexRayReadDataSpl;
    CBaseDIL_FLEXRAY*       m_pouDIL_FlexRay_Interface;
    CFormatMsgFlexRay       m_ouFormatFlexRay;

private:
    void vProcessNewData(s_FLXMSG& sFlexRayData);
    void vProcessCurrErrorEntry(const SERROR_INFO& sErrInfo);
    BOOL bIsTransitionInState( UINT unChannel,
                               BYTE byRxError,
                               BYTE byTxError );
    BOOL bTobeBlocked(s_FLXMSG& sFlexRayData);
    USHORT usProcessCurrErrorEntry(SERROR_INFO& sErrInfo);

    CRITICAL_SECTION m_sCritSecDataSync;
    CRITICAL_SECTION m_omCritSecFilter;

public:
    CMsgContainerFlexRay(void);
    ~CMsgContainerFlexRay(void);
    void InitTimeParams(void);
    void vRetrieveDataFromBuffer();

    //Exported functions
    BOOL bGetDilInterFace();
    void vInit(void* pParam);
    void vEditClearAll();
    int nGetAppendBufferCount();
    int nGetOWBufferCount();
    BOOL bStartReadThread();
    BOOL bStopReadThread();

    void vGetUpdatedCurrDataPtrArray(SMSGWNDHDRCOL& sHdrColStruct,
                                     char* pomDataPtrArr[MAX_MSG_WND_COL_CNT],
                                     BYTE bExprnFlag_Disp);
    HRESULT hReadFromOWBuffer(void* psMsg, __int64 nMapIndex);
    HRESULT hReadFromAppendBuffer(void* psMsg, int nMsgIndex);
    HRESULT hUpdateFormattedMsgStruct(int nListIndex, int& nMsgCode,
                                      BYTE bExprnFlag_Disp, __int64 nTimeOffset = 0);
    void vSetCurrMsgName(CString strMsgNameOrCode);
    void vSetMsgLength(CString strMsgLength);
    void vClearFormattedMsgStruct();
    void vSaveOWandGetDetails(void* pMsg, __int64& dwMapIndex,
                              __int64& dwTimeStamp, UINT& nMsgCode, int& nBufferIndex );

    void SetClientID(DWORD dwClientID);
    void DoSortBuffer(int nField,bool bAscending);
    void GetMapIndexAtID(int nIndex,__int64& nMapIndex);
    HRESULT hToggleDILBufferRead(BOOL bRead);
    HRESULT ApplyFilterScheme(void* pvFilterApplied);
    HRESULT GetFilterScheme(void* pvFilterApplied);
    HRESULT EnableFilterApplied(BOOL bEnable);
    __int64 nCreateMapIndexKey( LPVOID pMsgData );
};
