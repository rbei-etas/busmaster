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
 * \file      MsgContainer_Ethernet.h
 * \brief     Definition of CMsgContainerEthernet class
 * \authors   Ashwin R Uchil
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of CMsgContainerLIN class
 */

#pragma once

#include "afxtempl.h"
#include "Utility/Utility_Thread.h"
#include "Application/MsgWndHdrInfo.h"
#include "CommonClass/GUI_FormatMsgEthernet.h"
#include "include/BaseDefs.h"
#include "DataTypes/MsgBufAll_DataTypes.h"
#include "DataTypes/MsgBufEthernetVFSE.h"
#include "DataTypes/MsgBufVVSE.h"
#include "DataTypes/Filter_Datatypes.h"
#include "CommonClass/RefTimeKeeper.h"
#include "CommonClass/MsgContainerBase.h"
#include "DIL_Interface/BaseDIL_ETHERNET.h"
#include "DIL_Interface/DIL_Interface_extern.h"

typedef CMsgBufEthernetVFSE<STETHERNETDATA> CEthernetBufVFSE; // <to do - To rename CMsgBufCANVFSE to make it generic>

// To save delta time in case of append mode
typedef struct stEthernetDataSpl : public STETHERNETDATA
{
    __int64      m_nDeltime;

    stEthernetDataSpl()
    {
        m_nDeltime = 0;
    }
} STETHERNETDATASPL;

typedef CMsgBufEthernetVFSE<STETHERNETDATASPL> CMsgBufEthernetVFSEspl;


class CMsgContainerEthernet : public CMsgContainerBase
{
private:
	CEthernetBufFSE				m_ouMCEthernetBufFSE;
	CEthernetBufVFSE			m_ouOWEthernetBuf;
	CMsgBufEthernetVFSEspl		m_ouAppendEthernetBuf;
	SFORMATTEDDATA_ETHERNET		m_sOutFormattedData;
	STETHERNETDATASPL			m_sEthernetReadDataSpl;
	CFormatMsgEthernet			m_ouFormatEthernet;
	DWORD						m_dwClientId;
	CRITICAL_SECTION			m_sCritSecDataSync;
    CRITICAL_SECTION			m_omCritSecFilter;
	CBaseDIL_ETHERNET*			m_pouDIL_Ethernet_Interface;

public:
    CMsgContainerEthernet(void);
    ~CMsgContainerEthernet(void);

	void InitTimeParams(void) ;
    void vRetrieveDataFromBuffer();

    void vInit(void* pParam) ;
    void vEditClearAll();
    int nGetAppendBufferCount() ;
    int nGetOWBufferCount() ;
    BOOL bStartReadThread() ;

    void vGetUpdatedCurrDataPtrArray(SMSGWNDHDRCOL& sHdrColStruct,
            char* pomDataPtrArr[MAX_MSG_WND_COL_CNT],
            BYTE bExprnFlag_Disp) ;
    HRESULT hReadFromOWBuffer(void* psMsg, __int64 nMapIndex) ;
    HRESULT hReadFromAppendBuffer(void* psMsg, int nMsgIndex) ;
    HRESULT hUpdateFormattedMsgStruct(int nListIndex, int& nMsgCode,
            BYTE bExprnFlag_Disp, __int64 nTimeOffset ) ;
	void vSetCurrMsgName(CString strMsgNameOrCode) ;
	void vSetMsgLength(CString strMsgNameOrCode) ;
	void vClearFormattedMsgStruct() ;
	void vSaveOWandGetDetails(void* pMsg, __int64& dwMapIndex,
		__int64& dwTimeStamp, UINT& nMsgCode, int& nBufferIndex ) ;

	void SetClientID(DWORD dwClientID) ;
	void DoSortBuffer(int nField,bool bAscending) ;
	void GetMapIndexAtID(int nIndex,__int64& nMapIndex) ;
	HRESULT ApplyFilterScheme(void* pvFilterApplied) ;
	HRESULT GetFilterScheme(void* pvFilterApplied) ;
	HRESULT EnableFilterApplied(BOOL bEnable) ;
	BOOL bGetDilInterFace() ;
	__int64 nCreateMapIndexKey(LPVOID pMsgData) ;
	HRESULT hToggleDILBufferRead(BOOL bRead) ;
	void vProcessNewData(STETHERNETDATA& sEthernetData);
	BOOL  bStopReadThread();
};