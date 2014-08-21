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
 * \file      MsgContainer_Ethernet.cpp
 * \brief     Implementation of CMsgContainerEthernet class
 * \authors   Ashwin R Uchil
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of CMsgContainerEthernet class
 */

#include "PSDI_CAN/stdafx_CAN.h"
#include "include/Utils_Macro.h"
#include "include/error.h"
#include "Include/BaseDefs.h"
#include "DataTypes/MsgBufAll_Datatypes.h"
#include "DataTypes/DIL_Datatypes.h"
#include "MsgContainer_Ethernet.h"

const int nBitsIn4Bytes          = 32;
const int TX_MESSAGE = 0x20000000;  // bitwise OR to make it a Tx message
const int RX_MESSAGE = 0xdfffffff;  // bitwise AND to make it a Rx message
#define MAKE_CHANNEL_SPECIFIC_MESSAGE(MSGID, CHANNELID) ( ((unsigned __int64)((UINT)(MSGID))) | (((__int64)(CHANNELID)) << nBitsIn4Bytes) )
#define MAKE_RX_TX_MESSAGE(MSGID, MSGTYPE)   ((MSGTYPE) ? (MSGID) : ((MSGID) | TX_MESSAGE))
#define MAKELONGLONG(HINT, LINT) ( ((unsigned __int64)((UINT)(HINT))) | (((__int64)(LINT)) << nBitsIn4Bytes) )

CMsgContainerEthernet::CMsgContainerEthernet(void)
{
	InitializeCriticalSection(&m_sCritSecDataSync);
    InitializeCriticalSection(&m_omCritSecFilter);
}

CMsgContainerEthernet::~CMsgContainerEthernet(void)
{
	DeleteCriticalSection(&m_sCritSecDataSync);
    DeleteCriticalSection(&m_omCritSecFilter);
}

void CMsgContainerEthernet::InitTimeParams(void) 
{
	SYSTEMTIME CurrSysTime;
    UINT64 unAbsTime;

    if (NULL != m_pouDIL_Ethernet_Interface)
    {
        m_pouDIL_Ethernet_Interface->DILE_GetTimeModeMapping(CurrSysTime, unAbsTime);
        m_ouFormatEthernet.vSetTimeParams(CurrSysTime, unAbsTime);
    }
}

void CMsgContainerEthernet::vProcessNewData(STETHERNETDATA& sEthernetData)
{

    // append : 1. Add item, get count, SetItemCount.
    // OW     : 1. Get ID, Create map index, Search if it is present in map
    //             if present change the offset and time stamp
    //          2. if not present and (get count < max count) save the Buffer index,
    //             ListCtrl index, time stamp, make offest = time stamp in disp entry
    //             and add to the map and array
    //          3. if not present and (get count = max count), do nothing
    //          4. SetItemcount

    //if ( IS_A_MESSAGE(sCanData.m_ucDataType) )
    {

        // Add to append buffer
        // If its the very first entry, the time stamp must
        if (m_sEthernetReadDataSpl.m_lTickCount.QuadPart != 0) // be 0 and will
        {
            // retain such value.
            m_sEthernetReadDataSpl.m_nDeltime = _abs64(sEthernetData.m_lTickCount.QuadPart -
                                                  m_sEthernetReadDataSpl.m_lTickCount.QuadPart);
        }

        STETHERNETDATA* pStEth = &m_sEthernetReadDataSpl;
        *pStEth = sEthernetData;
       // if (!bTobeBlocked(sEthernetData))
        {
            m_ouAppendEthernetBuf.WriteIntoBuffer(&m_sEthernetReadDataSpl);
            if (NULL != m_pRxMsgCallBack)
            {
				m_pRxMsgCallBack((void*)&sEthernetData, ETHERNET);
            }
        }
    }
    //else //Add the error messages
    //{
    //    vProcessCurrErrorEntry(sCanData.m_uDataInfo.m_sErrInfo);
    //    // Add to append buffer
    //    // If its the very first entry, the time stamp must
    //    if (m_sCANReadDataSpl.m_lTickCount.QuadPart != 0) // be 0 and will
    //    {
    //        // retain such value.
    //        m_sCANReadDataSpl.m_nDeltime = _abs64(sCanData.m_lTickCount.QuadPart -
    //                                              m_sCANReadDataSpl.m_lTickCount.QuadPart);
    //    }
    //    STCANDATA* pStcan = &m_sCANReadDataSpl;
    //    *pStcan = sCanData;

    //    if (!bTobeBlocked(sCanData))
    //    {
    //        m_ouAppendCanBuf.WriteIntoBuffer(&m_sCANReadDataSpl);

    //        if (NULL != m_pRxMsgCallBack)
    //        {
    //            m_pRxMsgCallBack((void*)&sCanData, CAN);
    //        }
    //    }
    //}

}
void CMsgContainerEthernet::vRetrieveDataFromBuffer()
{
	EnterCriticalSection(&m_sCritSecDataSync);
    while (m_ouMCEthernetBufFSE.GetMsgCount() > 0)
    {
        STETHERNETDATA sEthernetData;
        if (m_ouMCEthernetBufFSE.ReadFromBuffer(&sEthernetData) == CALL_SUCCESS)
        {
            vProcessNewData(sEthernetData);
        }
    }
    LeaveCriticalSection(&m_sCritSecDataSync);
}

void CMsgContainerEthernet::vInit(void* pParam) 
{

}

void CMsgContainerEthernet::vEditClearAll()
{
	m_ouOWEthernetBuf.vClearMessageBuffer();
    m_ouAppendEthernetBuf.vClearMessageBuffer();
    memset(&m_sEthernetReadDataSpl, 0, sizeof(m_sEthernetReadDataSpl));
}

int CMsgContainerEthernet::nGetAppendBufferCount() 
{
	return m_ouAppendEthernetBuf.GetBufferLength();
}

int CMsgContainerEthernet::nGetOWBufferCount() 
{
	 return m_ouOWEthernetBuf.GetBufferLength();
}

BOOL CMsgContainerEthernet::bStartReadThread() 
{
	 int bResult = TRUE;
    HRESULT hResult;
    if (NULL != m_pouDIL_Ethernet_Interface)
    {
        //if (m_pouDIL_CAN_Interface->DILC_RegisterClient(TRUE, m_dwClientId, L"PSDI_CAN") == S_OK)
        {
            hResult = m_pouDIL_Ethernet_Interface->DILE_ManageMsgBuf(MSGBUF_ADD, m_dwClientId, &m_ouMCEthernetBufFSE);
        }
    }
    //m_sDataCopyThread.m_pBuffer = this;
    //m_sDataCopyThread.m_hActionEvent = m_ouCanBufFSE.hGetNotifyingEvent();

    //if ( !m_sDataCopyThread.bStartThread(DataCopyThreadProc) )
    //{
    //    // Log error message
    //    bResult = FALSE;
    //}
    bResult = CMsgContainerBase::bStartReadThread(m_ouMCEthernetBufFSE.hGetNotifyingEvent());
    return bResult;
}

void CMsgContainerEthernet::vGetUpdatedCurrDataPtrArray(SMSGWNDHDRCOL& sHdrColStruct,
														char* pomDataPtrArr[MAX_MSG_WND_COL_CNT],
														BYTE bExprnFlag_Disp) 
{
	//Time mode
    if (IS_TM_ABS_SET(bExprnFlag_Disp)) //for Absolute non-reset timestamp
    {
        pomDataPtrArr[sHdrColStruct.m_byTimePos] = m_sOutFormattedData.m_acTimeAbs;
    }

    else if (IS_TM_REL_SET(bExprnFlag_Disp))
    {
        pomDataPtrArr[sHdrColStruct.m_byTimePos] = m_sOutFormattedData.m_acTimeRel;
    }

    else if (IS_TM_SYS_SET(bExprnFlag_Disp))
    {
        pomDataPtrArr[sHdrColStruct.m_byTimePos] = m_sOutFormattedData.m_acTimeSys;
    }

    // Dec/Hex mode for DATA and ID
    if (IS_NUM_HEX_SET(bExprnFlag_Disp))
    {
        pomDataPtrArr[sHdrColStruct.m_byDataPos] = m_sOutFormattedData.m_acDataHex;
        pomDataPtrArr[sHdrColStruct.m_byIDPos]   = m_sOutFormattedData.m_acMsgIDHex;
    }
    else if (IS_NUM_DEC_SET(bExprnFlag_Disp))
    {
        pomDataPtrArr[sHdrColStruct.m_byDataPos] = m_sOutFormattedData.m_acDataDec;
        pomDataPtrArr[sHdrColStruct.m_byIDPos]   = m_sOutFormattedData.m_acMsgIDDec;
    }

    //Rest will always be same
	pomDataPtrArr[sHdrColStruct.m_byDestMAC]      = m_sOutFormattedData.m_DestMAC;
	pomDataPtrArr[sHdrColStruct.m_bySourceMAC]    = m_sOutFormattedData.m_SourceMAC;
	pomDataPtrArr[sHdrColStruct.m_byDestIP]       = m_sOutFormattedData.m_DestIP;
	pomDataPtrArr[sHdrColStruct.m_bySourceIP]     = m_sOutFormattedData.m_SourceIP;

    pomDataPtrArr[sHdrColStruct.m_byDLCPos]       = m_sOutFormattedData.m_acDataLen;
    pomDataPtrArr[sHdrColStruct.m_byMsgTypePos]   = m_sOutFormattedData.m_acMsgDir;
    pomDataPtrArr[sHdrColStruct.m_byRxTxPos]      = m_sOutFormattedData.m_acMsgDir;
    pomDataPtrArr[sHdrColStruct.m_byCodeNamePos]  = m_sOutFormattedData.m_acMsgDesc;
    pomDataPtrArr[sHdrColStruct.m_byChannel]      = m_sOutFormattedData.m_acChannel;

	
}

HRESULT CMsgContainerEthernet::hReadFromOWBuffer(void* psMsg, __int64 nMapIndex) 
{
	 return m_ouOWEthernetBuf.ReadFromBuffer((STETHERNETDATA*)psMsg, nMapIndex);
}

HRESULT CMsgContainerEthernet::hReadFromAppendBuffer(void* pvMsg, int nMsgIndex) 
{
	STETHERNETDATA* psMsg = (STETHERNETDATA*)pvMsg;
    static STETHERNETDATASPL sEthMsgSpl;
    HRESULT hResult =  m_ouAppendEthernetBuf.ReadFromBuffer(&sEthMsgSpl, nMsgIndex);
    STETHERNETDATA* psTemp = (STETHERNETDATA*)&sEthMsgSpl;
    *psMsg = *psTemp;
    return hResult;
}

HRESULT CMsgContainerEthernet::hUpdateFormattedMsgStruct(int nListIndex, int& nMsgCode,
														 BYTE bExprnFlag_Disp, __int64 nTimeOffset = 0) 
{
	HRESULT hResult = S_FALSE;
    nMsgCode  = 0;
    static STETHERNETDATA sEthCurrData;
    static STETHERNETDATASPL sEthCurrDataSpl;
    memset( &m_sOutFormattedData, 0, sizeof(m_sOutFormattedData) );
    if( IS_MODE_APPEND(bExprnFlag_Disp) )
    {
        hResult = m_ouAppendEthernetBuf.ReadFromBuffer(&sEthCurrDataSpl, nListIndex);
        sEthCurrData = *((STETHERNETDATA*)&sEthCurrDataSpl);
        if (IS_TM_REL_SET(bExprnFlag_Disp))
        {
            //If relative time then the time will be delta time, for formatting
            sEthCurrData.m_lTickCount.QuadPart = sEthCurrDataSpl.m_nDeltime;
        }
    }
    else
    {
        hResult = m_ouOWEthernetBuf.ReadFromBuffer(&sEthCurrData, nListIndex);
        if (hResult == S_OK)
        {
            if (IS_TM_REL_SET(bExprnFlag_Disp))
            {
                //If relative time then the time will be delta time, for formatting
                sEthCurrData.m_lTickCount.QuadPart -= nTimeOffset;
            }
        }
    }

    if (hResult == S_OK)
    {
        m_ouFormatEthernet.vFormatEthernetDataMsg(&sEthCurrData,
                                        &m_sOutFormattedData,
                                        bExprnFlag_Disp);

        //If Message is erroneous, return S_FALSE
       /* if(IS_ERR_MESSAGE(sCANCurrData.m_ucDataType))
        {
            nMsgCode = usProcessCurrErrorEntry(sCANCurrData.m_uDataInfo.m_sErrInfo);
            return S_FALSE;
        }*/

        //Now add the name of message if present in database else show the code
        nMsgCode = sEthCurrData.m_uDataInfo.m_sETHERNETMsg.m_unMsgID;
    }
    else
    {
        memset (&m_sOutFormattedData, 0, sizeof(m_sOutFormattedData));
        nMsgCode = -1;
    }

    return hResult;
}

void CMsgContainerEthernet::vSetCurrMsgName(CString strMsgNameOrCode) 
{
	CMsgContainerBase::bCopyStringTocharArr (m_sOutFormattedData.m_acMsgDesc, strMsgNameOrCode,
            sizeof(m_sOutFormattedData.m_acMsgDesc));
}

void CMsgContainerEthernet::vSetMsgLength(CString strsgLength) 
{
	CMsgContainerBase::bCopyStringTocharArr (m_sOutFormattedData.m_acDataLen, strsgLength,
            sizeof(m_sOutFormattedData.m_acDataLen));

    m_sOutFormattedData.m_nDataLength = atoi(strsgLength);
}

void CMsgContainerEthernet::vClearFormattedMsgStruct() 
{
	memset(&m_sOutFormattedData, 0, sizeof(m_sOutFormattedData));
}

void CMsgContainerEthernet::vSaveOWandGetDetails(void* pMsg, __int64& dwMapIndex,
												 __int64& dwTimeStamp, UINT& nMsgCode, int& nBufferIndex ) 
{
	STETHERNETDATA* pouEthData = (STETHERNETDATA*)pMsg;
    dwTimeStamp = pouEthData->m_lTickCount.QuadPart;
    nMsgCode   = pouEthData->m_uDataInfo.m_sETHERNETMsg.m_unMsgID;
    dwMapIndex =  nCreateMapIndexKey((LPVOID)pouEthData);
    //Now write into the array
    m_ouOWEthernetBuf.WriteIntoBuffer(pouEthData, dwMapIndex, nBufferIndex);
}

void CMsgContainerEthernet::SetClientID(DWORD dwClientID) 
{
	m_dwClientId = dwClientID;
}

void CMsgContainerEthernet::DoSortBuffer(int nField,bool bAscending) 
{
	 m_ouAppendEthernetBuf.vDoSortBuffer(nField,bAscending);
    m_ouOWEthernetBuf.vDoSortBuffer(nField,bAscending);
}

void CMsgContainerEthernet::GetMapIndexAtID(int nIndex,__int64& nMapIndex) 
{
	m_ouOWEthernetBuf.nGetMapIndexAtID(nIndex,nMapIndex);
}

HRESULT CMsgContainerEthernet::ApplyFilterScheme(void* pvFilterApplied) 
{
	return S_OK;
}

HRESULT CMsgContainerEthernet::GetFilterScheme(void* pvFilterApplied) 
{
	return S_OK;
}

HRESULT CMsgContainerEthernet::EnableFilterApplied(BOOL bEnable) 
{
	return S_OK;
}

BOOL CMsgContainerEthernet::bGetDilInterFace() 
{
	BOOL bFound = FALSE;
	DIL_GetInterface(ETHERNET, (void**)&m_pouDIL_Ethernet_Interface);
    if (NULL != m_pouDIL_Ethernet_Interface)
    {
        bFound = TRUE;
    }
    return bFound;
}

__int64 CMsgContainerEthernet::nCreateMapIndexKey(LPVOID pMsgData) 
{
	STETHERNETDATA* pouEthernetData = (STETHERNETDATA*)pMsgData;
	STETHERNET_MSG& sMsg = pouEthernetData->m_uDataInfo.m_sETHERNETMsg;

    // Form message to get message index in the CMap
    int nMsgID = 0;
	//TODO: do error message handling here
    /*if (IS_ERR_MESSAGE(pouCANData->m_ucDataType))
    {
        nMsgID = MAKE_RX_TX_MESSAGE( usProcessCurrErrorEntry(pouCANData->m_uDataInfo.m_sErrInfo),
                                     IS_RX_MESSAGE(pouCANData->m_ucDataType));
    }
    else*/
    //{
        nMsgID = MAKE_RX_TX_MESSAGE( sMsg.m_unMsgID,
                                     IS_RX_MESSAGE(pouEthernetData->m_ucDataType));
    //}

    //nMsgID = MAKE_DEFAULT_MESSAGE_TYPE(nMsgID);
    //// For extended message
    //if (sMsg.m_ucEXTENDED)
    //{
    //    nMsgID = MAKE_EXTENDED_MESSAGE_TYPE(nMsgID);
    //}
    // Apply Channel Information
    __int64 n64MapIndex = MAKELONGLONG( nMsgID,pouEthernetData->m_uDataInfo.m_sETHERNETMsg.m_ucDestMac );
    return n64MapIndex;
}

HRESULT CMsgContainerEthernet::hToggleDILBufferRead(BOOL bRead) 
{
	 HRESULT hResult = S_FALSE;
    if (NULL != m_pouDIL_Ethernet_Interface)
    {
        if(bRead)
        {
            hResult = m_pouDIL_Ethernet_Interface->DILE_ManageMsgBuf(MSGBUF_ADD, m_dwClientId, &m_ouMCEthernetBufFSE);
        }
        else
        {
            hResult = m_pouDIL_Ethernet_Interface->DILE_ManageMsgBuf(MSGBUF_CLEAR, m_dwClientId, &m_ouMCEthernetBufFSE);
        }
    }
    return hResult;
}

BOOL CMsgContainerEthernet:: bStopReadThread()
{
    BOOL bReturn = CMsgContainerBase::bStopReadThread();
    if (NULL != m_pouDIL_Ethernet_Interface)
    {
        m_pouDIL_Ethernet_Interface->DILE_ManageMsgBuf(MSGBUF_CLEAR, m_dwClientId, &m_ouMCEthernetBufFSE);
    }
    return bReturn;
}
