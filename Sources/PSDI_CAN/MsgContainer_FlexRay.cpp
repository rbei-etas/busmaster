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
 * \file      MsgContainer_FlexRay.cpp
 * \brief     Implementation of CMsgContainerFlexRay class
 * \authors   Saravanan K S
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of CMsgContainerFlexRay class
 */

#include "PSDI_CAN/stdafx_CAN.h"
#include "include/Utils_Macro.h"
#include "include/error.h"
#include "Include/BaseDefs.h"
#include "DataTypes/MsgBufAll_Datatypes.h"
#include "DataTypes/DIL_Datatypes.h"
#include "MsgContainer_FlexRay.h"
//#include "../FLEXRAY_ETAS_ES520/EXTERNAL_INCLUDE/UCIFLX.H"

const int nBitsIn4Bytes          = 32;

const int TX_MESSAGE = 0x20000000;  // bitwise OR to make it a Tx message
const int RX_MESSAGE = 0xdfffffff;  // bitwise AND to make it a Rx message

#define MAKE_RTR_MESSAGE_TYPE(MSGID)         (MSGID | 0x80000000)
#define MAKE_EXTENDED_MESSAGE_TYPE(MSGID)    (MSGID | 0x40000000)
#define MAKE_STANDARD_MESSAGE_TYPE(MSGID)    (MSGID & 0xBfffffff)
#define MAKE_DEFAULT_MESSAGE_TYPE(MSGID)     (MSGID & 0x3fffffff)
#define MAKE_RX_TX_MESSAGE(MSGID, MSGTYPE)   ((MSGTYPE) ? (MSGID) : ((MSGID) | TX_MESSAGE))
#define MAKE_CHANNEL_SPECIFIC_MESSAGE(MSGID, CHANNELID) ( ((unsigned __int64)((UINT)(MSGID))) | (((__int64)(CHANNELID)) << nBitsIn4Bytes) )
#define MAKE_ERROR_MESSAGE_TYPE(ERRID)    (ERRID | 0x40000000)

/******************************************************************************
    Function Name    :  CMsgContainerFlexRay
    Input(s)         :
    Output           :
    Functionality    :  Constructor
    Member of        :  CMsgContainerFlexRay
    Friend of        :      -
    Author(s)        :  Saravanan
    Date Created     :  21.03.2013
******************************************************************************/
CMsgContainerFlexRay::CMsgContainerFlexRay(void)
{
    InitializeCriticalSection(&m_sCritSecDataSync);
    InitializeCriticalSection(&m_omCritSecFilter);

    m_pouDIL_FlexRay_Interface = nullptr;
}

/******************************************************************************
    Function Name    :  ~CMsgContainerFlexRay
    Input(s)         :
    Output           :
    Functionality    :  Destructor
    Member of        :  CMsgContainerFlexRay
    Friend of        :      -
    Author(s)        :  Saravanan
    Date Created     :  21.03.2013
******************************************************************************/
CMsgContainerFlexRay::~CMsgContainerFlexRay(void)
{
    DeleteCriticalSection(&m_sCritSecDataSync);
    DeleteCriticalSection(&m_omCritSecFilter);
    //m_sDataCopyThread.bTerminateThread();
}

/******************************************************************************
    Function Name    :  InitTimeParams
    Input(s)         :
    Output           :
    Functionality    :  Initialize the refrence time
    Member of        :  CMsgContainerFlexRay
    Friend of        :      -
    Author(s)        :  Saravanan
    Date Created     :  21.03.2013
******************************************************************************/
void CMsgContainerFlexRay::InitTimeParams(void)
{
    SYSTEMTIME CurrSysTime;
    UINT64 unAbsTime;
    if (nullptr != m_pouDIL_FlexRay_Interface)
    {
        m_pouDIL_FlexRay_Interface->DILF_GetTimeModeMapping(CurrSysTime, unAbsTime);
        m_ouFormatFlexRay.vSetTimeParams(CurrSysTime, unAbsTime);
    }
}

/******************************************************************************
    Function Name    :  nCreateKeyWithCodeAndType
    Input(s)         :
    Output           :
    Functionality    :  Creates a key for SmsgDispEntry map
    Member of        :  CMsgContainerFlexRay
    Friend of        :      -
    Author(s)        :  Saravanan
    Date Created     :  21.03.2013
******************************************************************************/
__int64 CMsgContainerFlexRay::nCreateMapIndexKey( LPVOID pMsgData )
{
    s_FLXMSG* pouFLEXData = (s_FLXMSG*)pMsgData;
    s_FLXSTATUSMSG& stcStatusMsg = pouFLEXData->stcStatusMsg;
    s_FLXDATAMSG& stcDataMsg = pouFLEXData->stcDataMsg;
    DWORD dwIFlags = 0;

    if (pouFLEXData->m_eMessageType == FLXMSGTYPE_STATUS)
    {
        dwIFlags = pouFLEXData->stcStatusMsg.dwStatusInfoFlags;
    }
    else if (pouFLEXData->m_eMessageType == FLXMSGTYPE_DATA)
    {
        dwIFlags = pouFLEXData->stcDataMsg.m_lHeaderInfoFlags;
    }

    // Form message to get message index in the CMap
    int nMsgID = 0;
    if (pouFLEXData->m_eMessageType == FLXMSGTYPE_STATUS)
    {
        nMsgID = MAKE_RX_TX_MESSAGE( 0, dwIFlags & RBIN_FLXHDRINFO_SELFRECEPTION);
    }
    else if (pouFLEXData->m_eMessageType == FLXMSGTYPE_DATA)
    {

        nMsgID = MAKELONG(stcDataMsg.m_nSlotID, MAKEWORD(stcDataMsg.m_nBaseCycle, stcDataMsg.m_eChannel));
        //nMsgID = MAKE_RX_TX_MESSAGE( nMsgID, dwIFlags & RBIN_FLXHDRINFO_SELFRECEPTION);
    }

    //nMsgID = MAKE_DEFAULT_MESSAGE_TYPE(nMsgID);

    // Apply Channel Information
    int nFlag = dwIFlags & RBIN_FLXHDRINFO_NULLFRAME;

    if ( stcDataMsg.m_ucRepetition == 0 )
    {
        nFlag = nFlag || RBIN_FLXHDRINFO_NONDBMSG;
    }


    __int64 n64MapIndex = MAKE_CHANNEL_SPECIFIC_MESSAGE(nMsgID, nFlag );

    return (n64MapIndex);
}

BOOL CMsgContainerFlexRay::bTobeBlocked(s_FLXMSG& /* sFlexRayData */)
{
    return FALSE;
}

BOOL CMsgContainerFlexRay::bIsTransitionInState(UINT /* unChannel */,
        BYTE /* byRxError */,
        BYTE /* byTxError */)
{
    return FALSE;
}

void CMsgContainerFlexRay::vProcessCurrErrorEntry(const SERROR_INFO& /* sErrInfo */)
{
}

/******************************************************************************
    Function Name    :  vProcessNewData
    Input(s)         :
    Output           :
    Functionality    :  Process a new Rx/Tx msg
    Member of        :  CMsgContainerFlexRay
    Friend of        :      -
    Author(s)        :  Saravanan
    Date Created     :  21.03.2013
******************************************************************************/
void CMsgContainerFlexRay::vProcessNewData(s_FLXMSG& sFlexRayData)
{
    switch (sFlexRayData.m_eMessageType)
    {
        case FLXMSGTYPE_DATA:
        {
            if (/*bIsInRunningState() && */(sFlexRayData.stcDataMsg.m_nDLC >= 0))
            {
                // Add to append buffer
                // If its the very first entry, the time stamp must
                if (m_sFlexRayReadDataSpl.m_unTimestamp != 0) // be 0 and will
                {
                    // retain such value.
                    m_sFlexRayReadDataSpl.m_nDeltime = _abs64((sFlexRayData.m_unTimestamp) -
                                                       (m_sFlexRayReadDataSpl.m_unTimestamp));
                }

                s_FLXMSG* pStFLEX = &m_sFlexRayReadDataSpl;
                *pStFLEX = sFlexRayData;

                m_ouAppendFlexRayBuf.WriteIntoBuffer(&m_sFlexRayReadDataSpl);

                // Invoke the callback function
                if (nullptr != m_pRxMsgCallBack)
                {
                    m_pRxMsgCallBack((void*)&sFlexRayData, FLEXRAY);
                }
            }
        }
        break;
        case FLXMSGTYPE_STATUS:
        {
            /*if (nullptr != m_pRxMsgCallBack)
            {
                m_pRxMsgCallBack((void*)&sFlexRayData, FLEXRAY);
            }*/
        }
        case FLXMSGTYPE_UNKNOWN:
        {
            // Error processing
        }
        break;
    }
    //else
    //{
    //    vRetrieveAndLog(sg_pFlxIntfPtr, hResult, __FILE__, __LINE__);
    //}
}

/******************************************************************************
    Function Name    :  vRetrieveDataFromBuffer
    Input(s)         :
    Output           :
    Functionality    :  Read data from DIL buffer
    Member of        :  CMsgContainerFlexRay
    Friend of        :      -
    Author(s)        :  Saravanan
    Date Created     :  21.03.2013
******************************************************************************/
void CMsgContainerFlexRay::vRetrieveDataFromBuffer()
{
    EnterCriticalSection(&m_sCritSecDataSync);
    // <to do flexray>
    while (m_ouVSEBufFlexRay.GetMsgCount() > 0)
    {
        s_FLXMSG sFlexRayData;
        HRESULT hResult = m_ouVSEBufFlexRay.ReadFromBuffer(&sFlexRayData);

        if (hResult == S_OK)
        {
            vProcessNewData(sFlexRayData);
        }
    }
    LeaveCriticalSection(&m_sCritSecDataSync);
}

//****************Exported functions*******************************

/******************************************************************************
    Function Name    :  vInit
    Input(s)         :  CMcNetMessageMap pointer as void*
    Output           :
    Functionality    :
    Member of        :  CMsgContainerFlexRay
    Friend of        :      -
    Author(s)        :  Saravanan
    Date Created     :  21.03.2013
******************************************************************************/
void CMsgContainerFlexRay::vInit(void* /*pParam*/)
{
    //int nSize =sizeof(m_sFlexRayReadDataSpl)*5000;
    //m_ouAppendFlexRayBuf.nSetBufferSize(nSize);
}


/**********************************************************************************
    Function Name :   bStartReadThread
    Input(s)      :   -
    Output        :   -
    Functionality :   -
    Member of     :   CMsgContainerFlexRay
    Friend of     :   -
    Authors       :
    Author(s)     :  Saravanan
    Date Created  :  21.03.2013
************************************************************************************/
BOOL CMsgContainerFlexRay:: bStartReadThread()
{
    int bResult = TRUE;
    HRESULT hResult;
    if (nullptr != m_pouDIL_FlexRay_Interface)
    {
        hResult = m_pouDIL_FlexRay_Interface->DILF_ManageMsgBuf(MSGBUF_ADD, m_dwClientId, &m_ouVSEBufFlexRay);
    }
    bResult = CMsgContainerBase::bStartReadThread(m_ouVSEBufFlexRay.hGetNotifyingEvent());
    return bResult;
}

/**********************************************************************************
  Function Name :   hToggleDILBufferRead
  Input(s)      :   bRead
  Output        :   -
  Functionality :   -
  Member of     :   CMsgContainerFlexRay
  Friend of     :   -
  Author(s)     :  Saravanan
  Date Created  :  21.03.2013
  Modifications :
************************************************************************************/
HRESULT CMsgContainerFlexRay:: hToggleDILBufferRead(BOOL bRead)
{
    HRESULT hResult = S_FALSE;
    if (nullptr != m_pouDIL_FlexRay_Interface)
    {
        if(bRead)
        {
            hResult = m_pouDIL_FlexRay_Interface->DILF_ManageMsgBuf(MSGBUF_ADD, m_dwClientId, &m_ouVSEBufFlexRay);
        }
        else
        {
            hResult = m_pouDIL_FlexRay_Interface->DILF_ManageMsgBuf(MSGBUF_CLEAR, m_dwClientId, &m_ouVSEBufFlexRay);
        }
    }
    return hResult;
}

/******************************************************************************
    Function Name    :  bStopReadThread
    Input(s)         :
    Output           :
    Functionality    :  Stop the read thread
    Member of        :  CMsgContainerFlexRay
    Friend of        :      -
    Author(s)        :  Saravanan
    Date Created     :  21.03.2013
******************************************************************************/
BOOL CMsgContainerFlexRay:: bStopReadThread()
{
    BOOL bReturn = CMsgContainerBase::bStopReadThread();
    if (nullptr != m_pouDIL_FlexRay_Interface)
    {
        m_pouDIL_FlexRay_Interface->DILF_ManageMsgBuf(MSGBUF_CLEAR, m_dwClientId, &m_ouVSEBufFlexRay);
    }
    return bReturn;
}

/******************************************************************************
    Function Name    :  vEditClearAll
    Input(s)         :
    Output           :
    Functionality    :  Clear all the storage for UI
    Member of        :  CMsgContainerFlexRay
    Friend of        :      -
    Author(s)        :  Saravanan
    Date Created     :  21.03.2013
******************************************************************************/
void CMsgContainerFlexRay::vEditClearAll()
{
    m_ouOWFlexRayBuf.vClearMessageBuffer();
    m_ouAppendFlexRayBuf.vClearMessageBuffer();
    memset(&m_sFlexRayReadDataSpl, 0, sizeof(m_sFlexRayReadDataSpl));
}

/******************************************************************************
    Function Name    :  nGetAppendBufferCount
    Input(s)         :
    Output           :
    Functionality    :
    Member of        :  CMsgContainerFlexRay
    Friend of        :      -
    Author(s)        :  Saravanan
    Date Created     :  21.03.2013
******************************************************************************/
int CMsgContainerFlexRay::nGetAppendBufferCount()
{
    return m_ouAppendFlexRayBuf.GetBufferLength();
}

/******************************************************************************
    Function Name    :  nGetOWBufferCount
    Input(s)         :
    Output           :
    Functionality    :
    Member of        :  CMsgContainerFlexRay
    Friend of        :      -
    Author(s)        :  Saravanan
    Date Created     :  21.03.2013
******************************************************************************/
int CMsgContainerFlexRay::nGetOWBufferCount()
{
    return m_ouAppendFlexRayBuf.GetBufferLength();
}

/******************************************************************************
    Function Name    :  hReadFromOWBuffer
    Input(s)         :
    Output           :
    Functionality    :
    Member of        :  CMsgContainerFlexRay
    Friend of        :      -
    Author(s)        :  Saravanan
    Date Created     :  21.03.2013
******************************************************************************/
HRESULT CMsgContainerFlexRay::hReadFromOWBuffer(void* psMsg, __int64 nMapIndex)
{
    return m_ouOWFlexRayBuf.ReadFromBuffer((STFLEXRAYDATASPL*)psMsg, nMapIndex);
}

/******************************************************************************
    Function Name    :  hReadFromAppendBuffer
    Input(s)         :
    Output           :
    Functionality    :
    Member of        :  CMsgContainerFlexRay
    Friend of        :      -
    Author(s)        :  Saravanan
    Date Created     :  21.03.2013
******************************************************************************/
HRESULT CMsgContainerFlexRay::hReadFromAppendBuffer(void* pvMsg, int nMsgIndex)
{
    s_FLXMSG* psMsg = (s_FLXMSG*)pvMsg;
    static STFLEXRAYDATASPL sFLEXMsgSpl;
    HRESULT hResult =  m_ouAppendFlexRayBuf.ReadFromBuffer(&sFLEXMsgSpl, nMsgIndex);
    s_FLXMSG* psTemp = (s_FLXMSG*)&sFLEXMsgSpl;
    *psMsg = *psTemp;
    return hResult;
}


/******************************************************************************
    Function Name    :  vSaveOWandGetDetails
    Input(s)         :
    Output           :
    Functionality    :  Save to OW buffer and provide the details requested
                        by receive child/ dll user class
    Member of        :  CMsgContainerFlexRay
    Friend of        :      -
    Author(s)        :  Saravanan
    Date Created     :  21.03.2013
******************************************************************************/
void CMsgContainerFlexRay::vSaveOWandGetDetails( void* pMsg,
        __int64& dwMapIndex,
        __int64& dwTimeStamp,
        UINT& nMsgCode,
        int& nBufferIndex )
{
    s_FLXMSG* pouFLEXData = (s_FLXMSG*)pMsg;
    dwTimeStamp = pouFLEXData->m_unTimestamp;
    if ( pouFLEXData->m_eMessageType == FLXMSGTYPE_DATA )
    {
        nMsgCode   = MAKELONG(pouFLEXData->stcDataMsg.m_nSlotID, MAKEWORD(pouFLEXData->stcDataMsg.m_ucCycleNumber, pouFLEXData->stcDataMsg.m_eChannel));
    }
    dwMapIndex =  nCreateMapIndexKey((LPVOID)pouFLEXData);
    //Now write into the array
    m_ouOWFlexRayBuf.WriteIntoBuffer(pouFLEXData, dwMapIndex, nBufferIndex);
}

HRESULT CMsgContainerFlexRay::ApplyFilterScheme(void* /* pvFilterApplied */)
{
    return S_FALSE;
}

HRESULT CMsgContainerFlexRay::GetFilterScheme(void* /* pvFilterApplied */)
{
    return S_FALSE;
}

HRESULT CMsgContainerFlexRay::EnableFilterApplied(BOOL /* bEnable */)
{
    EnterCriticalSection(&m_omCritSecFilter);
    //m_sFilterCAN.m_bEnabled = bEnable;
    LeaveCriticalSection(&m_omCritSecFilter);
    return S_OK;
}
/******************************************************************************
    Function Name    :  hUpdateFormattedMsgStruct
    Input(s)         :
    Output           :
    Functionality    :  Format the requested Msg and save it in Format data
                        structure which is accessible from the User module
    Member of        :  CMsgContainerFlexRay
    Friend of        :      -
    Author(s)        :  Saravanan
    Date Created     :  21.03.2013
******************************************************************************/
HRESULT CMsgContainerFlexRay::hUpdateFormattedMsgStruct(int nListIndex,
        int& nMsgCode,
        BYTE bExprnFlag_Disp,
        __int64 nTimeOffset)
{
    HRESULT hResult = S_FALSE;
    nMsgCode  = 0;
    static s_FLXMSG sFLEXCurrData;
    static STFLEXRAYDATASPL sFLEXCurrDataSpl;
    memset( &m_sOutFormattedData, 0, sizeof(m_sOutFormattedData) );
    if( IS_MODE_APPEND(bExprnFlag_Disp) )
    {
        //INT nType = 0;
        //INT nSize =sizeof(sFLEXCurrDataSpl);
        //In append mode providing interpret state is not required
        //hResult = m_ouAppendFlexRayBuf.ReadEntry(nType, (BYTE*)&sFLEXCurrDataSpl, nSize, nListIndex, 0);
        hResult = m_ouAppendFlexRayBuf.ReadFromBuffer(&sFLEXCurrDataSpl, nListIndex);
        sFLEXCurrData = *((s_FLXMSG*)&sFLEXCurrDataSpl);
        if (IS_TM_REL_SET(bExprnFlag_Disp))
        {
            //If relative time then the time will be delta time, for formatting
            //sFLEXCurrData.m_lTickCount.QuadPart = sFLEXCurrDataSpl.m_nDeltime;
            sFLEXCurrData.m_unTimestamp = sFLEXCurrDataSpl.m_nDeltime; //to do
        }
    }
    else
    {
        hResult = m_ouOWFlexRayBuf.ReadFromBuffer(&sFLEXCurrData, nListIndex);
        if (hResult == S_OK)
        {
            if (IS_TM_REL_SET(bExprnFlag_Disp))
            {
                //If relative time then the time will be delta time, for formatting
                sFLEXCurrData.m_unTimestamp -= nTimeOffset; //to do
            }
        }
    }

    if (hResult == S_OK)
    {
        m_ouFormatFlexRay.vFormatFlexRayDataMsg(&sFLEXCurrData,
                                                &m_sOutFormattedData,
                                                bExprnFlag_Disp);

        //If Message is erroneous, return S_FALSE
        //if(IS_ERR_MESSAGE(sFLEXCurrData.m_ucDataType))
        //{
        //    nMsgCode = usProcessCurrErrorEntry(sFLEXCurrData.m_uDataInfo.m_sErrInfo);
        //    return S_FALSE;
        //}

        //Now add the name of message if present in database else show the code
        if ( sFLEXCurrData.m_eMessageType == FLXMSGTYPE_DATA )
        {
            nMsgCode = MAKELONG(sFLEXCurrData.stcDataMsg.m_nSlotID, MAKEWORD(sFLEXCurrData.stcDataMsg.m_ucCycleNumber, sFLEXCurrData.stcDataMsg.m_eChannel));
        }
    }
    else
    {
        memset (&m_sOutFormattedData, 0, sizeof(m_sOutFormattedData));
        nMsgCode = -1;
    }

    return hResult;
}

/******************************************************************************
    Function Name    :  vSetCurrMsgName
    Input(s)         :
    Output           :
    Functionality    :  Current msg name from DB
    Member of        :  CMsgContainerFlexRay
    Friend of        :      -
    Author(s)        :  Saravanan
    Date Created     :  21.03.2013
******************************************************************************/
void CMsgContainerFlexRay::vSetCurrMsgName(CString strMsgNameOrCode)
{
    CMsgContainerBase::bCopyStringTocharArr (m_sOutFormattedData.m_acMsgDesc, strMsgNameOrCode,
            sizeof(m_sOutFormattedData.m_acMsgDesc));
}

void CMsgContainerFlexRay::vSetMsgLength(CString strsgLength)
{
    CMsgContainerBase::bCopyStringTocharArr (m_sOutFormattedData.m_acDataLen, strsgLength,
            sizeof(m_sOutFormattedData.m_acDataLen));

    m_sOutFormattedData.m_byDataLength = atoi(strsgLength);
}
/*******************************************************************************
  Function Name  : usProcessCurrErrorEntry
  Input(s)       : SERROR_INFO&
  Output         : Returns Error ID as USHORT.
  Functionality  : Processes the current Error entry and returns the Error code.
  Member of      : CMsgContainerFlexRay
  Author(s)      : Arunkumar K
  Date Created   : 14-04-2011
  Modifications  :
*******************************************************************************/
USHORT CMsgContainerFlexRay::usProcessCurrErrorEntry(SERROR_INFO& /* sErrInfo */)
{
    return 0;
}

/******************************************************************************
    Function Name    :  vClearFormattedMsgStruct
    Input(s)         :
    Output           :
    Functionality    :  Clear format data structure pointers
    Member of        :  CMsgContainerFlexRay
    Friend of        :      -
    Author(s)        :  Saravanan
    Date Created     :  21.03.2013
******************************************************************************/
void CMsgContainerFlexRay::vClearFormattedMsgStruct()
{
    memset(&m_sOutFormattedData, 0, sizeof(m_sOutFormattedData));
}
/******************************************************************************
    Function Name    :  vGetUpdatedCurrDataPtrArray
    Input(s)         :
    Output           :
    Functionality    :  Provide format data structure pointers to the user
    Member of        :  CMsgContainerFlexRay
    Friend of        :      -
    Author(s)        :  Saravanan
    Date Created     :  21.03.2013
******************************************************************************/
void CMsgContainerFlexRay::vGetUpdatedCurrDataPtrArray(SMSGWNDHDRCOL& sHdrColStruct,
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
    pomDataPtrArr[sHdrColStruct.m_byFrameTypePos]    = m_sOutFormattedData.m_acFrameType;
    pomDataPtrArr[sHdrColStruct.m_byDLCPos]       = m_sOutFormattedData.m_acDataLen;
    pomDataPtrArr[sHdrColStruct.m_byRxTxPos]      = m_sOutFormattedData.m_acDirection;
    pomDataPtrArr[sHdrColStruct.m_byCodeNamePos]  = m_sOutFormattedData.m_acMsgDesc;
    pomDataPtrArr[sHdrColStruct.m_byChannel]      = m_sOutFormattedData.m_acChannel;
    pomDataPtrArr[sHdrColStruct.m_byCycleNoPos]   = m_sOutFormattedData.m_acCycleNum;
    pomDataPtrArr[sHdrColStruct.m_bySlotTypePos] = m_sOutFormattedData.m_acSlotType;
}

/******************************************************************************
    Function Name    :  bGetDilInterFace
    Input(s)         :
    Output           :
    Functionality    :  Get Dil interface pointer
    Member of        :  CMsgContainerFlexRay
    Friend of        :      -
    Author(s)        :  Saravanan
    Date Created     :  21.03.2013
******************************************************************************/
BOOL CMsgContainerFlexRay::bGetDilInterFace()
{
    BOOL bFound = FALSE;;
    DIL_GetInterface(FLEXRAY, (void**)&m_pouDIL_FlexRay_Interface);
    if (nullptr != m_pouDIL_FlexRay_Interface)
    {
        bFound = TRUE;
    }
    return bFound;
}

void CMsgContainerFlexRay::SetClientID(DWORD dwClientID)
{
    m_dwClientId = dwClientID;
}

void CMsgContainerFlexRay::DoSortBuffer(int nField,bool bAscending)
{
    m_ouAppendFlexRayBuf.vDoSortBuffer(nField, bAscending);
    m_ouOWFlexRayBuf.vDoSortBuffer(nField,bAscending);
}

void CMsgContainerFlexRay::GetMapIndexAtID(int nIndex,__int64& nMapIndex)
{
    m_ouOWFlexRayBuf.nGetMapIndexAtID(nIndex,nMapIndex);
}

