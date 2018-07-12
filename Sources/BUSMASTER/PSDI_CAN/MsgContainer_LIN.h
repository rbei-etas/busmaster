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
 * \file      MsgContainer_LIN.h
 * \brief     Definition of CMsgContainerLIN class
 * \authors   Anish Kumar, Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of CMsgContainerLIN class
 */

#pragma once

#include "afxtempl.h"
#include "Utility/Utility_Thread.h"
#include "Application/MsgWndHdrInfo.h"
#include "CommonClass/GUI_FormatMsgLIN.h"
#include "include/BaseDefs.h"
//#include "DataTypes/MsgBufAll_DataTypes.h"
#include "MsgBufLINVFSE.h"
#include "MsgBufVVSE.h"
#include "DataTypes/Filter_Datatypes.h"
#include "CommonClass/RefTimeKeeper.h"
#include "CommonClass/MsgContainerBase.h"
#include "BaseDIL_LIN.h"
//#include "DIL_Interface_extern.h"
#include "MsgBufFSE.h"
typedef CMsgBufLINVFSE<STLINDATA> CLINBufVFSE;
typedef void (*MSG_RX_CALL_BK)(void* pParam, ETYPE_BUS eBusType);

// To save delta time in case of append mode
typedef struct stLinDataSpl : public STLINDATA
{
    __int64      m_nDeltime;

    stLinDataSpl()
    {
        m_nDeltime = 0;
    }
} STLINDATASPL;

typedef CMsgBufLINVFSE<STLINDATASPL> CMsgBufLINVFSEspl;

class CMsgContainerLIN: public CMsgContainerBase
{
private:
    CMsgBufFSE<STLINDATA>  m_ouMCLinBufFSE;
    CLINBufVFSE             m_ouOWLinBuf;
    CMsgBufLINVFSEspl       m_ouAppendLinBuf;
    SFORMATTEDDATA_LIN      m_sOutFormattedData;
    //STLINDATA               m_sLINReadData;
    STLINDATASPL            m_sLINReadDataSpl;
    CFormatMsgLIN           m_ouFormatLIN;
    DWORD                   m_dwClientId;
    CBaseDIL_LIN*           m_pouDIL_LIN_Interface;
    IBMNetWorkGetService*   mBmNetwork = nullptr;
private:
    //ReadMcNetData virtual function
    void vProcessNewData(STLINDATA& sLinData);
    void vProcessCurrErrorEntryLin(const SERROR_INFO_LIN& sErrInfo);
    BOOL bIsTransitionInState( UINT unChannel,
                               BYTE byRxError,
                               BYTE byTxError );
    BOOL bTobeBlocked(STLINDATA& sLinData);
    __int64 n64CreateMapIDForLinEvevnt(STLINDATA* sLinData);
    USHORT usProcessCurrErrorEntry(SERROR_INFO_LIN& sErrInfo);

    CRITICAL_SECTION m_sCritSecDataSync;
    CRITICAL_SECTION m_omCritSecFilter;
    SFILTERAPPLIED_LIN m_sFilterLIN;
    ERROR_STATE m_eCurrErrorState[ defNO_OF_CHANNELS ];
    HRESULT GetMessageName(STLINDATA& sCANMsg, CString& msgName, bool formatHexForId);
public:
    CMsgContainerLIN(void);
    ~CMsgContainerLIN(void);
    void InitTimeParams(void);
    void vRetrieveDataFromBuffer();

public:
    //Exported functions
    BOOL bGetDilInterFace();
    void vInit(void* pParam);
    void vEditClearAll();
    int nGetAppendBufferCount();
    int nGetOWBufferCount();
    BOOL bStartReadThread();
    BOOL bStopReadThread();
    //void vSetRelBaseTime(__int64 nTimeStamp);

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
                              __int64& dwTimeStamp, UINT& nMsgCode, int& nBufferIndex, EINTERPRET_MODE&);


    //void vSetRxMsgCallBkPtr(MSG_RX_CALL_BK pFuncPtr);
    void SetClientID(DWORD dwClientID);
    void DoSortBuffer(int nField,bool bAscending);
    void GetMapIndexAtID(int nIndex,__int64& nMapIndex);
    HRESULT hToggleDILBufferRead(BOOL bRead);
    HRESULT ApplyFilterScheme(void* pvFilterApplied);
    HRESULT GetFilterScheme(void* pvFilterApplied);
    HRESULT EnableFilterApplied(BOOL bEnable);
    __int64 nCreateMapIndexKey( LPVOID pMsgData );
    //Import Log File.
    HRESULT LoadPage(const unsigned long& nPageNo);
    HRESULT OverwritePage(const unsigned long& nLineNo);
    HRESULT SetIBMNetWorkGetService(void* pBmNetWork);

    HRESULT GetSignalInfoArray(__int64 nMapIndex, SSignalInfoArray& SigInfoArray, bool formatInHex);
    HRESULT GetInterPretedSignalList(STLINDATA& ouMsg, SSignalInfoArray& ouSSignalInfoArray, bool isHexMode);
    HRESULT GetMessageDetails(__int64 nMapIndex, unsigned int& msgId, CString& msgName, bool formatHexForId);
    HRESULT sendMessage(long long key, bool bOverwriteMode/*has to be removed*/);
    COLORREF getMessageColor(long long key, bool isAppendMode, CMessageAttrib* /*has to be removed*/);
};
