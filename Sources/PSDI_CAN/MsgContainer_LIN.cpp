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
 * \file      MsgContainer_LIN.cpp
 * \brief     Implementation of CMsgContainerLIN class
 * \authors   Anish Kumar, Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of CMsgContainerLIN class
 */

#include "PSDI_CAN/stdafx_CAN.h"
#include "include/Utils_Macro.h"
#include "include/error.h"
#include "Include/BaseDefs.h"
#include "Include/Can_Error_defs.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_Datatypes.h"
#include "DataTypes/DIL_Datatypes.h"
#include "NodeSimEx/BaseNodeSim.h"
#include "NodeSimEx/NodeSimEx_extern.h"
#include "MsgContainer_LIN.h"

const int nBitsIn4Bytes          = 32;

const int TX_MESSAGE = 0x20000000;  // bitwise OR to make it a Tx message
const int RX_MESSAGE = 0xdfffffff;  // bitwise AND to make it a Rx message

#define MAKE_RTR_MESSAGE_TYPE(MSGID)         (MSGID | 0x80000000)
#define MAKE_EXTENDED_MESSAGE_TYPE(MSGID)    (MSGID | 0x40000000)
#define MAKE_STANDARD_MESSAGE_TYPE(MSGID)    (MSGID & 0xBfffffff)
#define MAKE_DEFAULT_MESSAGE_TYPE(MSGID)     (MSGID & 0x3fffffff)
#define MAKE_RX_TX_MESSAGE(MSGID, MSGTYPE)   ((MSGTYPE) ? (MSGID) : ((MSGID) | TX_MESSAGE))
#define MAKE_CHANNEL_SPECIFIC_MESSAGE(MSGID, CHANNELID) ( ((unsigned __int64)((UINT)(MSGID))) | (((__int64)(CHANNELID)) << nBitsIn4Bytes) )
#define MAKELONGLONG(HINT, LINT) ( ((unsigned __int64)((UINT)(HINT))) | (((__int64)(LINT)) << nBitsIn4Bytes) )
#define MAKE_ERROR_MESSAGE_TYPE(ERRID)    (ERRID | 0x40000000)

/******************************************************************************
    Function Name    :  CMsgContainerLIN
    Input(s)         :
    Output           :
    Functionality    :  Constructor
    Member of        :  CMsgContainerLIN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
CMsgContainerLIN::CMsgContainerLIN(void)
{
    InitializeCriticalSection(&m_sCritSecDataSync);
    InitializeCriticalSection(&m_omCritSecFilter);
}

/******************************************************************************
    Function Name    :  ~CMsgContainerLIN
    Input(s)         :
    Output           :
    Functionality    :  Destructor
    Member of        :  CMsgContainerLIN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
CMsgContainerLIN::~CMsgContainerLIN(void)
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
    Member of        :  CMsgContainerLIN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
void CMsgContainerLIN::InitTimeParams(void)
{
    SYSTEMTIME CurrSysTime;
    UINT64 unAbsTime;
    //Kadoor HANDLE hReadHandle = m_ouLinBufFSE.hGetNotifyingEvent();
    //Kadoor WaitForSingleObject(hReadHandle, INFINITE);
    if (NULL != m_pouDIL_LIN_Interface)
    {
        m_pouDIL_LIN_Interface->DILL_GetTimeModeMapping(CurrSysTime, unAbsTime);
        m_ouFormatLIN.vSetTimeParams(CurrSysTime, unAbsTime);
    }
}

/******************************************************************************
    Function Name    :  nCreateKeyWithCodeAndType
    Input(s)         :
    Output           :
    Functionality    :  Creates a key for SmsgDispEntry map
    Member of        :  CMsgContainerLIN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
__int64 CMsgContainerLIN::nCreateMapIndexKey( LPVOID pMsgData )
{
    STLINDATA* pouLINData = (STLINDATA*)pMsgData;
    STLIN_MSG& sMsg = pouLINData->m_uDataInfo.m_sLINMsg;

    return n64CreateMapIDForLinEvevnt(pouLINData);
}



//converts STLINDATA into SFRAMEINFO_BASIC_LIN
static void vFormatLINDataMsg(STLINDATA* pMsgLIN,
                              tagSFRAMEINFO_BASIC_LIN* CurrDataLIN)

{
    if(pMsgLIN->m_eLinMsgType == LIN_MSG)
    {
        CurrDataLIN->m_eEventType = EVENT_LIN_NONE;
        if (RX_FLAG == pMsgLIN->m_ucDataType)
        {
            CurrDataLIN->m_eDrctn = DIR_RX;
        }
        else
        {
            CurrDataLIN->m_eDrctn = DIR_TX;
        }
    }
    else if(pMsgLIN->m_eLinMsgType == LIN_EVENT)
    {
        CurrDataLIN->m_eEventType = pMsgLIN->m_uDataInfo.m_sErrInfo.m_eEventType;
    }

    CurrDataLIN->m_eChannel = pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucChannel;

    /*
    {
        CurrDataLIN->m_byIDType = TYPE_ID_LIN_STANDARD;
    }
    */
    CurrDataLIN->m_dwFrameID = pMsgLIN->m_uDataInfo.m_sLINMsg.m_ucMsgID;
}

BOOL CMsgContainerLIN::bTobeBlocked(STLINDATA& sLinData)
{
    static SFRAMEINFO_BASIC_LIN sBasicLinInfo;
    vFormatLINDataMsg(&sLinData, &sBasicLinInfo);

    EnterCriticalSection(&m_omCritSecFilter);
    BOOL bBlock = m_sFilterLIN.bToBeBlocked(sBasicLinInfo);
    LeaveCriticalSection(&m_omCritSecFilter);
    return bBlock;
}

BOOL CMsgContainerLIN::bIsTransitionInState( UINT unChannel,
        BYTE byRxError,
        BYTE byTxError )
{
    BOOL bIsTransition = FALSE;
    // Based on the value of transmit and receive error counter decide
    // the current error state

    if ((byTxError <= 127) && (byRxError <= 127))
    {
        // Error Active Mode
        if (m_eCurrErrorState[ unChannel ] != ERROR_ACTIVE)
        {
            bIsTransition = TRUE;
            m_eCurrErrorState[ unChannel ] = ERROR_ACTIVE;
        }
    }
    else if (byTxError == 255)
        // The sudden shift to the third state is to avoid
        // "else if ((byTxError > 127) || (byRxError > 127))"
    {
        // Bus off
        if (m_eCurrErrorState[ unChannel ] != ERROR_BUS_OFF)
        {
            bIsTransition = TRUE;
            m_eCurrErrorState[ unChannel ] = ERROR_BUS_OFF;
        }
    }
    else
    {
        // Error passive
        if (m_eCurrErrorState[ unChannel ] != ERROR_PASSIVE)
        {
            bIsTransition = TRUE;
            m_eCurrErrorState[ unChannel ] = ERROR_PASSIVE;
        }
    }

    return bIsTransition;
}
void CMsgContainerLIN::vProcessCurrErrorEntryLin(const SERROR_INFO_LIN& /*sErrInfo*/)
{
}

/******************************************************************************
    Function Name    :  vProcessNewData
    Input(s)         :
    Output           :
    Functionality    :  Process a new Rx/Tx msg
    Member of        :  CMsgContainerLIN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
void CMsgContainerLIN::vProcessNewData(STLINDATA& sLinData)
{

    // append : 1. Add item, get count, SetItemCount.
    // OW     : 1. Get ID, Create map index, Search if it is present in map
    //             if present change the offset and time stamp
    //          2. if not present and (get count < max count) save the Buffer index,
    //             ListCtrl index, time stamp, make offest = time stamp in disp entry
    //             and add to the map and array
    //          3. if not present and (get count = max count), do nothing
    //          4. SetItemcount

    if ( sLinData.m_eLinMsgType == LIN_MSG /*IS_A_MESSAGE(sLinData.m_ucDataType)*/ )
    {

        // Add to append buffer
        // If its the very first entry, the time stamp must
        if (m_sLINReadDataSpl.m_lTickCount.QuadPart != 0) // be 0 and will
        {
            // retain such value.
            m_sLINReadDataSpl.m_nDeltime = _abs64(sLinData.m_lTickCount.QuadPart -
                                                  m_sLINReadDataSpl.m_lTickCount.QuadPart);
        }

        STLINDATA* pStlin = &m_sLINReadDataSpl;
        *pStlin = sLinData;
        if (!bTobeBlocked(sLinData))
        {
            m_ouAppendLinBuf.WriteIntoBuffer(&m_sLINReadDataSpl);

            if (NULL != m_pRxMsgCallBack)
            {
                m_pRxMsgCallBack((void*)&sLinData, LIN);
            }
        }
    }
    else if(sLinData.m_eLinMsgType == LIN_EVENT)//Add the error messages
    {
        vProcessCurrErrorEntryLin(sLinData.m_uDataInfo.m_sErrInfo);
        // Add to append buffer
        // If its the very first entry, the time stamp must
        if (m_sLINReadDataSpl.m_lTickCount.QuadPart != 0) // be 0 and will
        {
            // retain such value.
            m_sLINReadDataSpl.m_nDeltime = _abs64(sLinData.m_lTickCount.QuadPart -
                                                  m_sLINReadDataSpl.m_lTickCount.QuadPart);
        }
        STLINDATA* pStlin = &m_sLINReadDataSpl;
        *pStlin = sLinData;

        if (!bTobeBlocked(sLinData))
        {
            m_ouAppendLinBuf.WriteIntoBuffer(&m_sLINReadDataSpl);

            if (NULL != m_pRxMsgCallBack)
            {
                m_pRxMsgCallBack((void*)&sLinData, LIN);
            }
        }
    }

}
/******************************************************************************
    Function Name    :  vRetrieveDataFromBuffer
    Input(s)         :
    Output           :
    Functionality    :  Read data from DIL buffer
    Member of        :  CMsgContainerLIN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
void CMsgContainerLIN::vRetrieveDataFromBuffer()
{
    EnterCriticalSection(&m_sCritSecDataSync);
    while (m_ouMCLinBufFSE.GetMsgCount() > 0)
    {
        STLINDATA sLinData;
        if (m_ouMCLinBufFSE.ReadFromBuffer(&sLinData) == CALL_SUCCESS)
        {
            vProcessNewData(sLinData);
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
    Member of        :  CMsgContainerLIN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
void CMsgContainerLIN::vInit(void* /*pParam*/)
{
    //int nSize =sizeof(m_sLINReadDataSpl)*5000;
    //m_ouAppendLinBuf.nSetBufferSize(nSize);
}


/**********************************************************************************
//Function Name :   bStartReadThread
//Input(s)      :   -
//Output        :   -
//Functionality :   -
//Member of     :   CMsgContainerLIN
//Friend of     :   -
//Authors       :
//Date Created  :
//Modifications :
************************************************************************************/
BOOL CMsgContainerLIN:: bStartReadThread()
{
    int bResult = TRUE;
    HRESULT hResult;
    if (NULL != m_pouDIL_LIN_Interface)
    {
        //if (m_pouDIL_LIN_Interface->DILL_RegisterClient(TRUE, m_dwClientId, L"PSDI_LIN") == S_OK)
        {
            hResult = m_pouDIL_LIN_Interface->DILL_ManageMsgBuf(MSGBUF_ADD, m_dwClientId, &m_ouMCLinBufFSE);
        }
    }
    //m_sDataCopyThread.m_pBuffer = this;
    //m_sDataCopyThread.m_hActionEvent = m_ouLinBufFSE.hGetNotifyingEvent();

    //if ( !m_sDataCopyThread.bStartThread(DataCopyThreadProc) )
    //{
    //    // Log error message
    //    bResult = FALSE;
    //}
    bResult = CMsgContainerBase::bStartReadThread(m_ouMCLinBufFSE.hGetNotifyingEvent());
    return bResult;
}

/**********************************************************************************
  Function Name :   hToggleDILBufferRead
  Input(s)      :   bRead
  Output        :   -
  Functionality :   -
  Member of     :   CMsgContainerLIN
  Friend of     :   -
  Author(s)     :  Arun kumar K
  Date Created  :  28.03.2011
  Modifications :
************************************************************************************/
HRESULT CMsgContainerLIN:: hToggleDILBufferRead(BOOL bRead)
{
    HRESULT hResult = S_FALSE;
    if (NULL != m_pouDIL_LIN_Interface)
    {
        if(bRead)
        {
            hResult = m_pouDIL_LIN_Interface->DILL_ManageMsgBuf(MSGBUF_ADD, m_dwClientId, &m_ouMCLinBufFSE);
        }
        else
        {
            hResult = m_pouDIL_LIN_Interface->DILL_ManageMsgBuf(MSGBUF_CLEAR, m_dwClientId, &m_ouMCLinBufFSE);
        }
    }
    return hResult;
}

/******************************************************************************
    Function Name    :  bStopReadThread
    Input(s)         :
    Output           :
    Functionality    :  Stop the read thread
    Member of        :  CMsgContainerLIN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
BOOL CMsgContainerLIN:: bStopReadThread()
{
    BOOL bReturn = CMsgContainerBase::bStopReadThread();
    if (NULL != m_pouDIL_LIN_Interface)
    {
        m_pouDIL_LIN_Interface->DILL_ManageMsgBuf(MSGBUF_CLEAR, m_dwClientId, &m_ouMCLinBufFSE);
    }
    return bReturn;
}

/******************************************************************************
    Function Name    :  vEditClearAll
    Input(s)         :
    Output           :
    Functionality    :  Clear all the storage for UI
    Member of        :  CMsgContainerLIN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
void CMsgContainerLIN::vEditClearAll()
{
    m_ouOWLinBuf.vClearMessageBuffer();
    m_ouAppendLinBuf.vClearMessageBuffer();
    memset(&m_sLINReadDataSpl, 0, sizeof(m_sLINReadDataSpl));
}

/******************************************************************************
    Function Name    :  nGetAppendBufferCount
    Input(s)         :
    Output           :
    Functionality    :
    Member of        :  CMsgContainerLIN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
int CMsgContainerLIN::nGetAppendBufferCount()
{
    return m_ouAppendLinBuf.GetBufferLength();
}

/******************************************************************************
    Function Name    :  nGetOWBufferCount
    Input(s)         :
    Output           :
    Functionality    :
    Member of        :  CMsgContainerLIN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
int CMsgContainerLIN::nGetOWBufferCount()
{
    return m_ouOWLinBuf.GetBufferLength();
}

/******************************************************************************
    Function Name    :  hReadFromOWBuffer
    Input(s)         :
    Output           :
    Functionality    :
    Member of        :  CMsgContainerLIN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
HRESULT CMsgContainerLIN::hReadFromOWBuffer(void* psMsg, __int64 nMapIndex)
{
    //sTMCNET_MSG *psMcNetMsg = (sTMCNET_MSG*)psMsg;
    return m_ouOWLinBuf.ReadFromBuffer((STLINDATA*)psMsg, nMapIndex);
}

/******************************************************************************
    Function Name    :  hReadFromAppendBuffer
    Input(s)         :
    Output           :
    Functionality    :
    Member of        :  CMsgContainerLIN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
HRESULT CMsgContainerLIN::hReadFromAppendBuffer(void* pvMsg, int nMsgIndex)
{
    STLINDATA* psMsg = (STLINDATA*)pvMsg;
    static STLINDATASPL sLinMsgSpl;
    HRESULT hResult =  m_ouAppendLinBuf.ReadFromBuffer(&sLinMsgSpl, nMsgIndex);
    STLINDATA* psTemp = (STLINDATA*)&sLinMsgSpl;
    *psMsg = *psTemp;
    return hResult;
}


/******************************************************************************
    Function Name    :  vSaveOWandGetDetails
    Input(s)         :
    Output           :
    Functionality    :  Save to OW buffer and provide the details requested
                        by receive child/ dll user class
    Member of        :  CMsgContainerLIN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
void CMsgContainerLIN::vSaveOWandGetDetails( void* pMsg,
        __int64& dwMapIndex,
        __int64& dwTimeStamp,
        UINT& nMsgCode,
        int& nBufferIndex )
{
    STLINDATA* pouLINData = (STLINDATA*)pMsg;
    dwTimeStamp = pouLINData->m_lTickCount.QuadPart;
    nMsgCode   = pouLINData->m_uDataInfo.m_sLINMsg.m_ucMsgID;
    dwMapIndex =  nCreateMapIndexKey((LPVOID)pouLINData);
    //Now write into the array
    m_ouOWLinBuf.WriteIntoBuffer(pouLINData, dwMapIndex, nBufferIndex);
}

HRESULT CMsgContainerLIN::ApplyFilterScheme(void* pvFilterApplied)
{
    HRESULT hResult = S_FALSE;
    SFILTERAPPLIED_LIN* psFilterLIN = (SFILTERAPPLIED_LIN*)pvFilterApplied;
    if (psFilterLIN != NULL)
    {
        EnterCriticalSection(&m_omCritSecFilter);
        //        if (m_sFilterLIN.bClone(*psFilterLIN) == TRUE)
        if (m_sFilterLIN.bClone(*psFilterLIN) == true)
        {
            //            hResult = TRUE;
            hResult = S_OK;
        }
        LeaveCriticalSection(&m_omCritSecFilter);
    }
    return hResult;
}
HRESULT CMsgContainerLIN::GetFilterScheme(void* pvFilterApplied)
{
    HRESULT hResult = S_FALSE;
    SFILTERAPPLIED_LIN* psFilterLIN = (SFILTERAPPLIED_LIN*)pvFilterApplied;
    if (psFilterLIN != NULL)
    {
        //        if (psFilterLIN->bClone(m_sFilterLIN) == TRUE)
        if (psFilterLIN->bClone(m_sFilterLIN) == true)
        {
            //hResult = TRUE;
            hResult = S_OK;
        }
    }
    return hResult;
}
HRESULT CMsgContainerLIN::EnableFilterApplied(BOOL bEnable)
{
    EnterCriticalSection(&m_omCritSecFilter);
    m_sFilterLIN.m_bEnabled = bEnable;
    LeaveCriticalSection(&m_omCritSecFilter);
    return S_OK;
}
/******************************************************************************
    Function Name    :  hUpdateFormattedMsgStruct
    Input(s)         :
    Output           :
    Functionality    :  Format the requested Msg and save it in Format data
                        structure which is accessible from the User module
    Member of        :  CMsgContainerLIN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
HRESULT CMsgContainerLIN::hUpdateFormattedMsgStruct(int nListIndex,
        int& nMsgCode,
        BYTE bExprnFlag_Disp,
        __int64 nTimeOffset)
{
    HRESULT hResult = S_FALSE;
    nMsgCode  = 0;
    static STLINDATA sLINCurrData;
    static STLINDATASPL sLINCurrDataSpl;
    memset( &m_sOutFormattedData, 0, sizeof(m_sOutFormattedData) );
    if( IS_MODE_APPEND(bExprnFlag_Disp) )
    {
        //INT nType = 0;
        //INT nSize =sizeof(sLINCurrDataSpl);
        //In append mode providing interpret state is not required
        //hResult = m_ouAppendLinBuf.ReadEntry(nType, (BYTE*)&sLINCurrDataSpl, nSize, nListIndex, 0);
        hResult = m_ouAppendLinBuf.ReadFromBuffer(&sLINCurrDataSpl, nListIndex);
        sLINCurrData = *((STLINDATA*)&sLINCurrDataSpl);
        if (IS_TM_REL_SET(bExprnFlag_Disp))
        {
            //If relative time then the time will be delta time, for formatting
            sLINCurrData.m_lTickCount.QuadPart = sLINCurrDataSpl.m_nDeltime;
        }
    }
    else
    {
        hResult = m_ouOWLinBuf.ReadFromBuffer(&sLINCurrData, nListIndex);
        if (hResult == S_OK)
        {
            if (IS_TM_REL_SET(bExprnFlag_Disp))
            {
                //If relative time then the time will be delta time, for formatting
                sLINCurrData.m_lTickCount.QuadPart -= nTimeOffset;
            }
        }
    }

    if (hResult == S_OK)
    {
        m_ouFormatLIN.vFormatLINDataMsg(&sLINCurrData,
                                        &m_sOutFormattedData,
                                        bExprnFlag_Disp);
        nMsgCode = n64CreateMapIDForLinEvevnt( &sLINCurrData) ;
        //If Message is erroneous, return S_FALSE
        //if(sLINCurrData.m_eLinMsgType == LIN_EVENT)
        //      {
        //  nMsgCode = n64CreateMapIDForLinEvevnt( &sLINCurrData) ;//usProcessCurrErrorEntry(sLINCurrData.m_uDataInfo.m_sErrInfo);
        //          return S_FALSE;
        //      }
        //if(sLINCurrData.m_eLinMsgType == LIN_MSG)
        //{
        //  //Now add the name of message if present in database else show the code
        //  nMsgCode = sLINCurrData.m_uDataInfo.m_sLINMsg.m_ucMsgID;
        //}
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
    Member of        :  CMsgContainerLIN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
void CMsgContainerLIN::vSetCurrMsgName(CString strMsgNameOrCode)
{
    CMsgContainerBase::bCopyStringTocharArr (m_sOutFormattedData.m_acMsgDesc, strMsgNameOrCode,
            sizeof(m_sOutFormattedData.m_acMsgDesc));
}

void CMsgContainerLIN::vSetMsgLength(CString strsgLength)
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
  Member of      : CMsgContainerLIN
  Author(s)      : Arunkumar K
  Date Created   : 14-04-2011
  Modifications  :
*******************************************************************************/
USHORT CMsgContainerLIN::usProcessCurrErrorEntry(SERROR_INFO_LIN& sErrInfo)
{
    // Get the Error code
    USHORT usErrorID;
    // Get the channel number
    CHAR nChannel = sErrInfo.m_ucChannel - 1;
    if( nChannel < 0 || nChannel >= defNO_OF_CHANNELS )
    {
        ASSERT( FALSE );
        // Take prevension
        nChannel = 0;
    }

    // Decide which module(s) to notify by analysing the error code
    // Accordingly notify the modules by sending/posting message

    usErrorID = sErrInfo.m_eEventType;

    return usErrorID;
}

__int64 CMsgContainerLIN::n64CreateMapIDForLinEvevnt(STLINDATA* psLinData)
{
    if ( psLinData == NULL )
    {
        return -1;
    }

    __int64 n64MapIndex = -1;
    if  ( psLinData->m_eLinMsgType == LIN_MSG )
    {
        WORD wTxId = MAKEWORD(psLinData->m_ucDataType, psLinData->m_uDataInfo.m_sLINMsg.m_ucMsgID);
        WORD wEventType = MAKEWORD( LIN_MSG, 0 );
        INT nMsgAttrib = MAKELONG ( wEventType, wTxId);
        //n64MapIndex = MAKELONGLONG(psLinData->m_uDataInfo.m_sLINMsg.m_ucChannel - 1, nMsgAttrib );
        n64MapIndex = MAKELONGLONG(nMsgAttrib, psLinData->m_uDataInfo.m_sLINMsg.m_ucChannel - 1 );
    }
    else if ( psLinData->m_eLinMsgType == LIN_EVENT )
    {
        WORD wTxId = MAKEWORD(psLinData->m_ucDataType, psLinData->m_uDataInfo.m_sErrInfo.m_ucId);
        WORD wEventType = MAKEWORD( LIN_EVENT, psLinData->m_uDataInfo.m_sErrInfo.m_eEventType );
        INT nMsgAttrib = MAKELONG ( wEventType, wTxId);
        //n64MapIndex = MAKELONGLONG(psLinData->m_uDataInfo.m_sLINMsg.m_ucChannel - 1, nMsgAttrib );
        n64MapIndex = MAKELONGLONG(nMsgAttrib, psLinData->m_uDataInfo.m_sLINMsg.m_ucChannel - 1 );
    }


    return n64MapIndex;
}


/******************************************************************************
    Function Name    :  vClearFormattedMsgStruct
    Input(s)         :
    Output           :
    Functionality    :  Clear format data structure pointers
    Member of        :  CMsgContainerLIN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
void CMsgContainerLIN::vClearFormattedMsgStruct()
{
    memset(&m_sOutFormattedData, 0, sizeof(m_sOutFormattedData));
}
/******************************************************************************
    Function Name    :  vGetUpdatedCurrDataPtrArray
    Input(s)         :
    Output           :
    Functionality    :  Provide format data structure pointers to the user
    Member of        :  CMsgContainerLIN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
void CMsgContainerLIN::vGetUpdatedCurrDataPtrArray(SMSGWNDHDRCOL& sHdrColStruct,
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
    pomDataPtrArr[sHdrColStruct.m_byDLCPos]       = m_sOutFormattedData.m_acDataLen;
    pomDataPtrArr[sHdrColStruct.m_byMsgTypePos]   = m_sOutFormattedData.m_acType;
    pomDataPtrArr[sHdrColStruct.m_byRxTxPos]      = m_sOutFormattedData.m_acMsgDir;
    pomDataPtrArr[sHdrColStruct.m_byCodeNamePos]  = m_sOutFormattedData.m_acMsgDesc;
    pomDataPtrArr[sHdrColStruct.m_byChannel]      = m_sOutFormattedData.m_acChannel;
    pomDataPtrArr[sHdrColStruct.m_byChecksumPos]  = m_sOutFormattedData.m_acChecksum;
}

/******************************************************************************
    Function Name    :  bGetDilInterFace
    Input(s)         :
    Output           :
    Functionality    :  Get Dil interface pointer
    Member of        :  CMsgContainerLIN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  26.05.2010
******************************************************************************/
BOOL CMsgContainerLIN::bGetDilInterFace()
{
    BOOL bFound = FALSE;;
    DIL_GetInterface(LIN, (void**)&m_pouDIL_LIN_Interface);
    if (NULL != m_pouDIL_LIN_Interface)
    {
        bFound = TRUE;
    }
    return bFound;
}

void CMsgContainerLIN::SetClientID(DWORD dwClientID)
{
    m_dwClientId = dwClientID;
}

void CMsgContainerLIN::DoSortBuffer(int nField,bool bAscending)
{
    m_ouAppendLinBuf.vDoSortBuffer(nField,bAscending);
    m_ouOWLinBuf.vDoSortBuffer(nField,bAscending);
}

void CMsgContainerLIN::GetMapIndexAtID(int nIndex,__int64& nMapIndex)
{
    m_ouOWLinBuf.nGetMapIndexAtID(nIndex,nMapIndex);
}

