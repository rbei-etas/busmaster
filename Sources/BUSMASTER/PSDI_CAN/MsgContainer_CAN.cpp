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
 * \file      MsgContainer_CAN.cpp
 * \brief     Implementation of CMsgContainerCAN class
 * \authors   Anish Kumar, Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of CMsgContainerCAN class
 */

#include "PSDI_CAN/stdafx_CAN.h"
#include "include/Utils_Macro.h"
#include "error.h"
#include "Include/BaseDefs.h"
//#include "Include/Can_Error_defs.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
//#include "DataTypes/MsgBufAll_Datatypes.h"
//#include "DIL_Datatypes.h"
#include "NodeSimEx/BaseNodeSim.h"
#include "NodeSimEx/NodeSimEx_extern.h"
#include "MsgContainer_CAN.h"
#include "../Application/HashDefines.h"
const int nBitsIn4Bytes          = 32;


#define MAKE_RTR_MESSAGE_TYPE(MSGID)         (MSGID | 0x80000000)
#define MAKE_EXTENDED_MESSAGE_TYPE(MSGID)    (MSGID | 0x40000000)
#define MAKE_STANDARD_MESSAGE_TYPE(MSGID)    (MSGID & 0xBfffffff)
#define MAKE_DEFAULT_MESSAGE_TYPE(MSGID)     (MSGID & 0x3fffffff)
#define MAKE_RX_TX_MESSAGE(MSGID, MSGTYPE)   ((MSGTYPE) ? (MSGID) : ((MSGID) | TX_MESSAGE))
#define MAKE_CHANNEL_SPECIFIC_MESSAGE(MSGID, CHANNELID) ( ((unsigned __int64)((UINT)(MSGID))) | (((__int64)(CHANNELID)) << nBitsIn4Bytes) )
#define MAKE_ERROR_MESSAGE_TYPE(ERRID)      (ERRID | 0x40000000)
#define COLOUR_ERROR_MSG                     RGB(255, 0, 0);

const int ERRORS_DEFINED = 15;

// Dummy structure to get rid of Reference init
struct sERRORMSGINFO
{
    unsigned short m_usErrorCode; // Error code
    char* m_ptcErorMsg;          // Error message
};
typedef sERRORMSGINFO SERRORMSGINFO;
typedef sERRORMSGINFO* PERRORMSGINFO;


static SERRORMSGINFO sg_asErrorEntry[ERRORS_DEFINED] =
{
    { ERROR_UNKNOWN, "Unknown Error" },
    { BIT_ERROR_RX, "Bus Error - Bit Error(Rx)" },
    { CRC_ERROR_RX, "Bus Error - CRC Error(Rx)" },
    { FORM_ERROR_RX, "Bus Error - Form Error(Rx)" },
    { STUFF_ERROR_RX, "Error Frame - Stuff error(Rx)" },
    { OTHER_ERROR_RX, "Bus Error - Other Error(Rx)" },
    { BIT_ERROR_TX, "Bus Error - Bit Error(Tx)" },
    { CRC_ERROR_TX, "Bus Error - CRC Error(Tx)" },
    { FORM_ERROR_TX, "Bus Error - Form Error(Tx)" },
    { STUFF_ERROR_TX, "Bus Error - Stuff Error(Tx)" },
    { OTHER_ERROR_TX, "Bus Error - Other Error(Tx)" },
    { ERROR_DEVICE_BUFF_OVERFLOW, "Controller Buffer Overflow" },
    { ERROR_DRIVER_BUFF_OVERFLOW, "Driver Buffer Overflow" },
    { ERROR_APPLICATION_BUFF_OVERFLOW, "Application Buffer Overflow" },
    { ERROR_DLLMSG_BUFF_OVERFLOW, "DLL message buffer Overflow" }
};

//Structure for SubErrors
struct sSUBERRORMSGINFO
{
    char* m_ptcSubErorMsg;          // SubError message
};
typedef sSUBERRORMSGINFO SSUBERRORMSGINFO;
typedef sSUBERRORMSGINFO* PSUBERRORMSGINFO;
//max no. of error bit defined is 28,one more element so that the
//error names can be placed at the error bit no.
const int SUB_ERRORS_DEFINED = 29;
//Static array having definitions of all sub array
static SSUBERRORMSGINFO sg_asSubErrorEntry[SUB_ERRORS_DEFINED] =
{
    //for 0 and 1 no error defined
    " ",
    " ",
    "Other Errors",
    "SOF bit error",
    "Other Errors",
    "Other Errors",
    "Other Errors",
    "Other Errors",
    "CRC error",
    "Other Errors",
    "Other Errors",
    "DLC error",
    "Other Errors",
    "Other Errors",
    "Other Errors",
    "Other Errors",
    " ",
    "local error ",
    "Intermission field error",
    "Tolerate Dominant Bits Error",
    " ",
    " ",
    " ",
    "Other Errors",
    "Other Errors",
    "Acknowledgement error",
    "EOF error",
    "Other Errors",
    "over load condition"
};

/******************************************************************************
    Function Name    :  CMsgContainerCAN
    Input(s)         :
    Output           :
    Functionality    :  Constructor
    Member of        :  CMsgContainerCAN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
CMsgContainerCAN::CMsgContainerCAN(void)
{
    InitializeCriticalSection(&m_sCritSecDataSync);
    InitializeCriticalSection(&m_omCritSecFilter);
}

/******************************************************************************
    Function Name    :  ~CMsgContainerCAN
    Input(s)         :
    Output           :
    Functionality    :  Destructor
    Member of        :  CMsgContainerCAN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
CMsgContainerCAN::~CMsgContainerCAN(void)
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
    Member of        :  CMsgContainerCAN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
void CMsgContainerCAN::InitTimeParams(void)
{
    SYSTEMTIME CurrSysTime;
    UINT64 unAbsTime;
    //Kadoor HANDLE hReadHandle = m_ouCanBufFSE.hGetNotifyingEvent();
    //Kadoor WaitForSingleObject(hReadHandle, INFINITE);
    if (nullptr != m_pouDIL_CAN_Interface)
    {
        LARGE_INTEGER QueryTickCount;
        m_pouDIL_CAN_Interface->DILC_GetTimeModeMapping(CurrSysTime, unAbsTime, QueryTickCount);
        m_ouFormatCAN.vSetTimeParams(CurrSysTime, unAbsTime);
    }
}

/******************************************************************************
    Function Name    :  nCreateKeyWithCodeAndType
    Input(s)         :
    Output           :
    Functionality    :  Creates a key for SmsgDispEntry map
    Member of        :  CMsgContainerCAN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
__int64 CMsgContainerCAN::nCreateMapIndexKey( LPVOID pMsgData )
{
    STCANDATA* pouCANData = (STCANDATA*)pMsgData;
    STCAN_MSG& sMsg = pouCANData->m_uDataInfo.m_sCANMsg;

    // Form message to get message index in the CMap
    int nMsgID = 0;
    if (IS_ERR_MESSAGE(pouCANData->m_ucDataType))
    {
        nMsgID = MAKE_RX_TX_MESSAGE( usProcessCurrErrorEntry(pouCANData->m_uDataInfo.m_sErrInfo),
                                     IS_RX_MESSAGE(pouCANData->m_ucDataType));
    }
    else
    {
        nMsgID = MAKE_RX_TX_MESSAGE( sMsg.m_unMsgID,
                                     IS_RX_MESSAGE(pouCANData->m_ucDataType));
    }

    nMsgID = MAKE_DEFAULT_MESSAGE_TYPE(nMsgID);
    // For extended message
    if (sMsg.m_ucEXTENDED)
    {
        nMsgID = MAKE_EXTENDED_MESSAGE_TYPE(nMsgID);
    }
    // Apply Channel Information
    __int64 n64MapIndex = MAKE_CHANNEL_SPECIFIC_MESSAGE( nMsgID,
                          sMsg.m_ucChannel );
    return n64MapIndex;
}

//converts STCANDATA into SFRAMEINFO_BASIC_CAN
static void vFormatCANDataMsg(STCANDATA* pMsgCAN,
                              tagSFRAMEINFO_BASIC_CAN* CurrDataCAN)

{
    if (RX_FLAG == pMsgCAN->m_ucDataType)
    {
        CurrDataCAN->m_eDrctn = DIR_RX;
    }
    else
    {
        CurrDataCAN->m_eDrctn = DIR_TX;
    }

    CurrDataCAN->m_eChannel = pMsgCAN->m_uDataInfo.m_sCANMsg.m_ucChannel;

    if (pMsgCAN->m_uDataInfo.m_sCANMsg.m_ucEXTENDED != 0)
    {
        CurrDataCAN->m_byIDType = TYPE_ID_CAN_EXTENDED;
    }
    else
    {
        CurrDataCAN->m_byIDType = TYPE_ID_CAN_STANDARD;
    }

    if ( pMsgCAN->m_uDataInfo.m_sCANMsg.m_bCANFD )
    {
        CurrDataCAN->m_byMsgType = TYPE_MSG_CAN_FD;
    }
    else if (pMsgCAN->m_uDataInfo.m_sCANMsg.m_ucRTR != 0)
    {
        CurrDataCAN->m_byMsgType = TYPE_MSG_CAN_RTR;
    }
    else
    {
        CurrDataCAN->m_byMsgType = TYPE_MSG_CAN_NON_RTR;
    }


    CurrDataCAN->m_dwFrameID = pMsgCAN->m_uDataInfo.m_sCANMsg.m_unMsgID;
}

BOOL CMsgContainerCAN::bTobeBlocked(STCANDATA& sCanData)
{
    static SFRAMEINFO_BASIC_CAN sBasicCanInfo;
    vFormatCANDataMsg(&sCanData, &sBasicCanInfo);

    EnterCriticalSection(&m_omCritSecFilter);
    BOOL bBlock = m_sFilterCAN.bToBeBlocked(sBasicCanInfo);
    LeaveCriticalSection(&m_omCritSecFilter);
    return bBlock;
}

BOOL CMsgContainerCAN::bIsTransitionInState( UINT unChannel,
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
void CMsgContainerCAN::vProcessCurrErrorEntry(const SERROR_INFO& /*sErrInfo*/)
{
}

/******************************************************************************
    Function Name    :  vProcessNewData
    Input(s)         :
    Output           :
    Functionality    :  Process a new Rx/Tx msg
    Member of        :  CMsgContainerCAN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
void CMsgContainerCAN::vProcessNewData(STCANDATA& sCanData)
{

    // append : 1. Add item, get count, SetItemCount.
    // OW     : 1. Get ID, Create map index, Search if it is present in map
    //             if present change the offset and time stamp
    //          2. if not present and (get count < max count) save the Buffer index,
    //             ListCtrl index, time stamp, make offest = time stamp in disp entry
    //             and add to the map and array
    //          3. if not present and (get count = max count), do nothing
    //          4. SetItemcount

    if ( IS_A_MESSAGE(sCanData.m_ucDataType) )
    {

        // Add to append buffer
        // If its the very first entry, the time stamp must
        if (m_sCANReadDataSpl.m_lTickCount.QuadPart != 0) // be 0 and will
        {
            // retain such value.
            m_sCANReadDataSpl.m_nDeltime = _abs64(sCanData.m_lTickCount.QuadPart -
                                                  m_sCANReadDataSpl.m_lTickCount.QuadPart);
        }

        STCANDATA* pStcan = &m_sCANReadDataSpl;
        *pStcan = sCanData;
        if (!bTobeBlocked(sCanData))
        {
            unsigned char FDataType = m_sCANReadDataSpl.m_ucDataType;
            if(IS_RX_MESSAGE(FDataType))
            {
                HRESULT h_Evaluate = EvaluateMessage(m_sCANReadDataSpl.m_uDataInfo.m_sCANMsg);
            }

            m_ouAppendCanBuf.WriteIntoBuffer(&m_sCANReadDataSpl);
            if (nullptr != m_pRxMsgCallBack)
            {
                m_pRxMsgCallBack->onRxMsg((void*)&sCanData);
            }
        }
    }
    else //Add the error messages
    {
        vProcessCurrErrorEntry(sCanData.m_uDataInfo.m_sErrInfo);
        // Add to append buffer
        // If its the very first entry, the time stamp must
        if (m_sCANReadDataSpl.m_lTickCount.QuadPart != 0) // be 0 and will
        {
            // retain such value.
            m_sCANReadDataSpl.m_nDeltime = _abs64(sCanData.m_lTickCount.QuadPart -
                                                  m_sCANReadDataSpl.m_lTickCount.QuadPart);
        }
        STCANDATA* pStcan = &m_sCANReadDataSpl;
        *pStcan = sCanData;

        if (!bTobeBlocked(sCanData))
        {
            m_ouAppendCanBuf.WriteIntoBuffer(&m_sCANReadDataSpl);
            if (nullptr != m_pRxMsgCallBack)
            {
                m_pRxMsgCallBack->onRxMsg((void*)&sCanData);
            }
        }
    }

}
/******************************************************************************
    Function Name    :  vRetrieveDataFromBuffer
    Input(s)         :
    Output           :
    Functionality    :  Read data from DIL buffer
    Member of        :  CMsgContainerCAN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
void CMsgContainerCAN::vRetrieveDataFromBuffer()
{
    EnterCriticalSection(&m_sCritSecDataSync);
    while (m_ouMCCanBufFSE.GetMsgCount() > 0)
    {
        STCANDATA sCanData;
        if (m_ouMCCanBufFSE.ReadFromBuffer(&sCanData) == CALL_SUCCESS)
        {
            vProcessNewData(sCanData);
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
    Member of        :  CMsgContainerCAN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
void CMsgContainerCAN::vInit(void* /*pParam*/)
{
    //int nSize =sizeof(m_sCANReadDataSpl)*5000;
    //m_ouAppendCanBuf.nSetBufferSize(nSize);
}


/**********************************************************************************
//Function Name :   bStartReadThread
//Input(s)      :   -
//Output        :   -
//Functionality :   -
//Member of     :   CMsgContainerCAN
//Friend of     :   -
//Authors       :
//Date Created  :
//Modifications :
************************************************************************************/
BOOL CMsgContainerCAN:: bStartReadThread()
{
    int bResult = TRUE;
    HRESULT hResult;
    if (nullptr != m_pouDIL_CAN_Interface)
    {
        //if (m_pouDIL_CAN_Interface->DILC_RegisterClient(TRUE, m_dwClientId, L"PSDI_CAN") == S_OK)
        {
            hResult = m_pouDIL_CAN_Interface->DILC_ManageMsgBuf(MSGBUF_ADD, m_dwClientId, &m_ouMCCanBufFSE);
        }
    }
    //m_sDataCopyThread.m_pBuffer = this;
    //m_sDataCopyThread.m_hActionEvent = m_ouCanBufFSE.hGetNotifyingEvent();

    //if ( !m_sDataCopyThread.bStartThread(DataCopyThreadProc) )
    //{
    //    // Log error message
    //    bResult = FALSE;
    //}
    bResult = CMsgContainerBase::bStartReadThread(m_ouMCCanBufFSE.hGetNotifyingEvent());
    return bResult;
}

/**********************************************************************************
  Function Name :   hToggleDILBufferRead
  Input(s)      :   bRead
  Output        :   -
  Functionality :   -
  Member of     :   CMsgContainerCAN
  Friend of     :   -
  Author(s)     :  Arun kumar K
  Date Created  :  28.03.2011
  Modifications :
************************************************************************************/
HRESULT CMsgContainerCAN:: hToggleDILBufferRead(BOOL bRead)
{
    HRESULT hResult = S_FALSE;
    if (nullptr != m_pouDIL_CAN_Interface)
    {
        if(bRead)
        {
            hResult = m_pouDIL_CAN_Interface->DILC_ManageMsgBuf(MSGBUF_ADD, m_dwClientId, &m_ouMCCanBufFSE);
        }
        else
        {
            hResult = m_pouDIL_CAN_Interface->DILC_ManageMsgBuf(MSGBUF_CLEAR, m_dwClientId, &m_ouMCCanBufFSE);
        }
    }
    return hResult;
}

/******************************************************************************
    Function Name    :  bStopReadThread
    Input(s)         :
    Output           :
    Functionality    :  Stop the read thread
    Member of        :  CMsgContainerCAN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
BOOL CMsgContainerCAN:: bStopReadThread()
{
    BOOL bReturn = CMsgContainerBase::bStopReadThread();
    if (nullptr != m_pouDIL_CAN_Interface)
    {
        m_pouDIL_CAN_Interface->DILC_ManageMsgBuf(MSGBUF_CLEAR, m_dwClientId, &m_ouMCCanBufFSE);
    }
    return bReturn;
}

/******************************************************************************
    Function Name    :  vEditClearAll
    Input(s)         :
    Output           :
    Functionality    :  Clear all the storage for UI
    Member of        :  CMsgContainerCAN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
void CMsgContainerCAN::vEditClearAll()
{
    m_ouOWCanBuf.vClearMessageBuffer();
    m_ouAppendCanBuf.vClearMessageBuffer();
    memset(&m_sCANReadDataSpl, 0, sizeof(m_sCANReadDataSpl));
}

/******************************************************************************
    Function Name    :  nGetAppendBufferCount
    Input(s)         :
    Output           :
    Functionality    :
    Member of        :  CMsgContainerCAN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
int CMsgContainerCAN::nGetAppendBufferCount()
{
    return m_ouAppendCanBuf.GetBufferLength();
}

/******************************************************************************
    Function Name    :  nGetOWBufferCount
    Input(s)         :
    Output           :
    Functionality    :
    Member of        :  CMsgContainerCAN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
int CMsgContainerCAN::nGetOWBufferCount()
{
    return m_ouOWCanBuf.GetBufferLength();
}

/******************************************************************************
    Function Name    :  hReadFromOWBuffer
    Input(s)         :
    Output           :
    Functionality    :
    Member of        :  CMsgContainerCAN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
HRESULT CMsgContainerCAN::hReadFromOWBuffer(void* psMsg, __int64 nMapIndex)
{
    //sTMCNET_MSG *psMcNetMsg = (sTMCNET_MSG*)psMsg;
    return m_ouOWCanBuf.ReadFromBuffer((STCANDATA*)psMsg, nMapIndex);
}

/******************************************************************************
    Function Name    :  hReadFromAppendBuffer
    Input(s)         :
    Output           :
    Functionality    :
    Member of        :  CMsgContainerCAN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
HRESULT CMsgContainerCAN::hReadFromAppendBuffer(void* pvMsg, int nMsgIndex)
{
    STCANDATA* psMsg = (STCANDATA*)pvMsg;
    static STCANDATASPL sCanMsgSpl;
    HRESULT hResult =  m_ouAppendCanBuf.ReadFromBuffer(&sCanMsgSpl, nMsgIndex);
    STCANDATA* psTemp = (STCANDATA*)&sCanMsgSpl;
    *psMsg = *psTemp;
    return hResult;
}


/******************************************************************************
    Function Name    :  vSaveOWandGetDetails
    Input(s)         :
    Output           :
    Functionality    :  Save to OW buffer and provide the details requested
                        by receive child/ dll user class
    Member of        :  CMsgContainerCAN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
void CMsgContainerCAN::vSaveOWandGetDetails( void* pMsg,
        __int64& dwMapIndex,
        __int64& dwTimeStamp,
        UINT& nMsgCode,
        int& nBufferIndex, EINTERPRET_MODE& interPretable)
{
    STCANDATA* pouCANData = (STCANDATA*)pMsg;
    dwTimeStamp = pouCANData->m_lTickCount.QuadPart;
    nMsgCode   = pouCANData->m_uDataInfo.m_sCANMsg.m_unMsgID;
    dwMapIndex =  nCreateMapIndexKey((LPVOID)pouCANData);


    interPretable = MODE_NONE;
    if (nullptr != mBmNetwork)
    {
        int nCount = 0;

        IFrame* pouFrame = nullptr;
        mBmNetwork->GetFrame(CAN, 0, pouCANData->m_uDataInfo.m_sCANMsg.m_unMsgID, nullptr, &pouFrame);
        if (nullptr != pouFrame)
        {
            if (pouFrame->GetSignalCount() > 0)
            {
                interPretable = INTERPRETABLE;
            }
        }
    }
    //Now write into the array
    m_ouOWCanBuf.WriteIntoBuffer(pouCANData, dwMapIndex, nBufferIndex);
}

HRESULT CMsgContainerCAN::ApplyFilterScheme(void* pvFilterApplied)
{
    HRESULT hResult = S_FALSE;
    SFILTERAPPLIED_CAN* psFilterCAN = (SFILTERAPPLIED_CAN*)pvFilterApplied;
    if (psFilterCAN != nullptr)
    {
        EnterCriticalSection(&m_omCritSecFilter);
        //        if (m_sFilterCAN.bClone(*psFilterCAN) == TRUE)
        if (m_sFilterCAN.bClone(*psFilterCAN) == true)
        {
            //            hResult = TRUE;
            hResult = S_OK;
        }
        LeaveCriticalSection(&m_omCritSecFilter);
    }
    return hResult;
}
HRESULT CMsgContainerCAN::GetFilterScheme(void* pvFilterApplied)
{
    HRESULT hResult = S_FALSE;
    SFILTERAPPLIED_CAN* psFilterCAN = (SFILTERAPPLIED_CAN*)pvFilterApplied;
    if (psFilterCAN != nullptr)
    {
        //        if (psFilterCAN->bClone(m_sFilterCAN) == TRUE)
        if (psFilterCAN->bClone(m_sFilterCAN) == true)
        {
            //hResult = TRUE;
            hResult = S_OK;
        }
    }
    return hResult;
}
HRESULT CMsgContainerCAN::EnableFilterApplied(BOOL bEnable)
{
    EnterCriticalSection(&m_omCritSecFilter);
    m_sFilterCAN.m_bEnabled = bEnable;
    LeaveCriticalSection(&m_omCritSecFilter);
    return S_OK;
}
/******************************************************************************
    Function Name    :  hUpdateFormattedMsgStruct
    Input(s)         :
    Output           :
    Functionality    :  Format the requested Msg and save it in Format data
                        structure which is accessible from the User module
    Member of        :  CMsgContainerCAN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
HRESULT CMsgContainerCAN::hUpdateFormattedMsgStruct(int nListIndex,
        int& nMsgCode,
        BYTE bExprnFlag_Disp,
        __int64 nTimeOffset)
{
    HRESULT hResult = S_FALSE;
    nMsgCode  = 0;
    static STCANDATA sCANCurrData;
    static STCANDATASPL sCANCurrDataSpl;
    memset( &m_sOutFormattedData, 0, sizeof(m_sOutFormattedData) );
    if( IS_MODE_APPEND(bExprnFlag_Disp) )
    {
        //INT nType = 0;
        //INT nSize =sizeof(sCANCurrDataSpl);
        //In append mode providing interpret state is not required
        //hResult = m_ouAppendCanBuf.ReadEntry(nType, (BYTE*)&sCANCurrDataSpl, nSize, nListIndex, 0);
        hResult = m_ouAppendCanBuf.ReadFromBuffer(&sCANCurrDataSpl, nListIndex);
        sCANCurrData = *((STCANDATA*)&sCANCurrDataSpl);
        if (IS_TM_REL_SET(bExprnFlag_Disp))
        {
            //If relative time then the time will be delta time, for formatting
            sCANCurrData.m_lTickCount.QuadPart = sCANCurrDataSpl.m_nDeltime;
        }
    }
    else
    {
        hResult = m_ouOWCanBuf.ReadFromBuffer(&sCANCurrData, nListIndex);
        if (hResult == S_OK)
        {
            if (IS_TM_REL_SET(bExprnFlag_Disp))
            {
                //If relative time then the time will be delta time, for formatting
                sCANCurrData.m_lTickCount.QuadPart -= nTimeOffset;
            }
        }
    }

    if (hResult == S_OK)
    {
        m_ouFormatCAN.vFormatCANDataMsg(&sCANCurrData,
                                        &m_sOutFormattedData,
                                        bExprnFlag_Disp);

        //Now add the name of message if present in database else show the code
        nMsgCode = sCANCurrData.m_uDataInfo.m_sCANMsg.m_unMsgID;

        CString name;
        GetMessageName(sCANCurrData, name, IS_NUM_HEX_SET(bExprnFlag_Disp));
        strcpy(m_sOutFormattedData.m_acMsgDesc, name.GetBuffer(0));
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
    Member of        :  CMsgContainerCAN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
void CMsgContainerCAN::vSetCurrMsgName(CString strMsgNameOrCode)
{
    CMsgContainerBase::bCopyStringTocharArr (m_sOutFormattedData.m_acMsgDesc, strMsgNameOrCode,
            sizeof(m_sOutFormattedData.m_acMsgDesc));
}

void CMsgContainerCAN::vSetMsgLength(CString strsgLength)
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
  Member of      : CMsgContainerCAN
  Author(s)      : Arunkumar K
  Date Created   : 14-04-2011
  Modifications  :
*******************************************************************************/
USHORT CMsgContainerCAN::usProcessCurrErrorEntry(SERROR_INFO& sErrInfo)
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
    if (sErrInfo.m_ucErrType == ERROR_BUS)
    {
        usErrorID = sErrInfo.m_ucReg_ErrCap /*& 0xE0*/;
    }
    else if (sErrInfo.m_ucErrType == ERROR_WARNING_LIMIT_REACHED)
    {
        // Reaching warning limit isn't considered as an error.
        // In case of this interrupt there is no need for display to
        // be updated.
        // Use Channel ID as High Byte of WPARAM
        usErrorID = ERROR_UNKNOWN;
    }
    // Handle interrupts. This is not error message and assign error code
    // to unknown
    // Interrupts are state transition indicators but not real error message
    else if (sErrInfo.m_ucErrType == ERROR_INTERRUPT)
    {
        // Use Channel ID as High Byte of WPARAM
        usErrorID = ERROR_UNKNOWN;
    }
    else
    {
        usErrorID = sErrInfo.m_ucErrType;
    }

    return usErrorID;
}

/******************************************************************************
    Function Name    :  vClearFormattedMsgStruct
    Input(s)         :
    Output           :
    Functionality    :  Clear format data structure pointers
    Member of        :  CMsgContainerCAN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
void CMsgContainerCAN::vClearFormattedMsgStruct()
{
    memset(&m_sOutFormattedData, 0, sizeof(m_sOutFormattedData));
}
/******************************************************************************
    Function Name    :  vGetUpdatedCurrDataPtrArray
    Input(s)         :
    Output           :
    Functionality    :  Provide format data structure pointers to the user
    Member of        :  CMsgContainerCAN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
void CMsgContainerCAN::vGetUpdatedCurrDataPtrArray(SMSGWNDHDRCOL& sHdrColStruct,
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
}

/******************************************************************************
    Function Name    :  bGetDilInterFace
    Input(s)         :
    Output           :
    Functionality    :  Get Dil interface pointer
    Member of        :  CMsgContainerCAN
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  26.05.2010
******************************************************************************/
BOOL CMsgContainerCAN::bGetDilInterFace()
{
    BOOL bFound = FALSE;;
    DIL_GetInterface( CAN, (void**)&m_pouDIL_CAN_Interface );
    if (nullptr != m_pouDIL_CAN_Interface)
    {
        bFound = TRUE;
    }
    return bFound;
}

void CMsgContainerCAN::SetClientID(DWORD dwClientID)
{
    m_dwClientId = dwClientID;
}


void CMsgContainerCAN::DoSortBuffer(int nField,bool bAscending)
{
    m_ouAppendCanBuf.vDoSortBuffer(nField,bAscending);
    m_ouOWCanBuf.vDoSortBuffer( nField, bAscending );
}

void CMsgContainerCAN::GetMapIndexAtID(int nIndex,__int64& nMapIndex)
{
    m_ouOWCanBuf.nGetMapIndexAtID(nIndex,nMapIndex);
}

//Import Log File.
HRESULT CMsgContainerCAN::LoadPage(const unsigned long& /*nPageNo*/)
{
    return S_OK;
}
HRESULT CMsgContainerCAN::OverwritePage(const unsigned long& /*nLineNo*/)
{
    return S_OK;
}
HRESULT CMsgContainerCAN::SetIBMNetWorkGetService(void* pBmNetWork)
{
    mBmNetwork = (IBMNetWorkGetService*)pBmNetWork;
    return S_OK;
}

HRESULT CMsgContainerCAN::GetSignalInfoArray(__int64 nMapIndex, SSignalInfoArray& SigInfoArray, bool formatInHex)
{
    STCANDATA sCANMsg;
    SigInfoArray.RemoveAll();

    HRESULT hResult = hReadFromOWBuffer(&sCANMsg, nMapIndex);
    if (hResult == S_OK)
    {
        return GetInterPretedSignalList(sCANMsg.m_uDataInfo.m_sCANMsg, SigInfoArray, formatInHex);
    }

    return S_OK;
}

HRESULT CMsgContainerCAN::GetInterPretedSignalList(STCAN_MSG& ouMsg, SSignalInfoArray& ouSSignalInfoArray, bool isHexMode)
{
    if (nullptr == mBmNetwork)
    {
        return S_FALSE;
    }
    IFrame* pouFrame = nullptr;
    ERRORCODE eResult = mBmNetwork->GetFrame(CAN, 0, ouMsg.m_unMsgID, nullptr, &pouFrame);
    ouSSignalInfoArray.RemoveAll();
    if (EC_SUCCESS == eResult && nullptr != pouFrame)
    {
        std::list<InterpreteSignals> ouSignal;
        std::string strName;
        pouFrame->GetName(strName);
        pouFrame->InterpretSignals(ouMsg.m_ucData, ouMsg.m_ucDataLen, ouSignal, isHexMode, true);
for (auto ouSignalInfo : ouSignal)   //Only C++11;
        {
            SSignalInfo ouSignal;
            ouSignal.m_omEnggValue = ouSignalInfo.m_omEnggValue.c_str();
            ouSignal.m_omRawValue = ouSignalInfo.m_omRawValue.c_str();
            ouSignal.m_omSigName = ouSignalInfo.m_omSigName.c_str();
            ouSignal.m_omUnit = ouSignalInfo.m_omUnit.c_str();
            ouSignal.m_msgName = strName.c_str();
            ouSSignalInfoArray.Add(ouSignal);
        }
        return S_OK;
    }
    return S_FALSE;
}

HRESULT CMsgContainerCAN::GetMessageDetails(__int64 nMapIndex, unsigned int& msgId, CString& msgName, bool formatHexForId)
{
    if (nullptr == mBmNetwork)
    {
        return S_FALSE;
    }

    STCANDATA sCANMsg;

    HRESULT hResult = hReadFromOWBuffer(&sCANMsg, nMapIndex);
    //Message Id
    msgId = sCANMsg.m_uDataInfo.m_sCANMsg.m_unMsgID;

    //Message Name
    GetMessageName(sCANMsg, msgName, formatHexForId);

}
COLORREF CMsgContainerCAN::getMessageColor(long long key, bool isAppendMode, CMessageAttrib* msgAttributes/*has to be removed*/)
{
    if (msgAttributes == nullptr)
    {
        return RGB(0, 0, 0);
    }
    HRESULT hResult = S_FALSE;
    STCANDATASPL sCANMsg;
    if (isAppendMode == true)
    {
        STCANDATASPL sSplCANMsg;
        hResult = m_ouAppendCanBuf.ReadFromBuffer(&sSplCANMsg, (int)key);
        sCANMsg.m_ucDataType = sSplCANMsg.m_ucDataType;
        sCANMsg.m_uDataInfo = sSplCANMsg.m_uDataInfo;
    }
    else
    {
        hResult = hReadFromOWBuffer(&sCANMsg, key);
    }
    if (CALL_SUCCESS == hResult)
    {
        if (IS_ERR_MESSAGE(sCANMsg.m_ucDataType))
        {
            return COLOUR_ERROR_MSG;
        }
        else
        {
            return msgAttributes->GetCanIDColour(sCANMsg.m_uDataInfo.m_sCANMsg.m_unMsgID);
        }
    }
    return RGB(0,0,0);
}
HRESULT CMsgContainerCAN::sendMessage(long long key, bool isAppendMode/*has to be removed*/)
{
    if (nullptr!= m_pouDIL_CAN_Interface)
    {
        HRESULT hResult = S_FALSE;
        STCANDATASPL sCANMsg;
        if (isAppendMode == true)
        {
            STCANDATASPL sSplCANMsg;
            hResult = m_ouAppendCanBuf.ReadFromBuffer(&sSplCANMsg, (int)key);
            sCANMsg.m_ucDataType = sSplCANMsg.m_ucDataType;
            sCANMsg.m_uDataInfo = sSplCANMsg.m_uDataInfo;
        }
        else
        {
            hResult = hReadFromOWBuffer(&sCANMsg, key);
        }
        if (!(IS_ERR_MESSAGE(sCANMsg.m_ucDataType)))
        {
            m_pouDIL_CAN_Interface->DILC_SendMsg(m_dwClientId, sCANMsg.m_uDataInfo.m_sCANMsg);
        }
    }
    return S_FALSE;
}

HRESULT CMsgContainerCAN::GetMessageName(STCANDATA& sCANMsg, CString& msgName, bool formatHexForId)
{
    if (IS_ERR_MESSAGE(sCANMsg.m_ucDataType))
    {
        GetErrorInfo(sCANMsg, msgName);
    }
    else
    {
        //Message Name
        std::string strName = "";
        IFrame* pouFrame = nullptr;
        ERRORCODE eResult = mBmNetwork->GetFrame(CAN, 0, sCANMsg.m_uDataInfo.m_sCANMsg.m_unMsgID, nullptr, &pouFrame);
        if (EC_SUCCESS == eResult && nullptr != pouFrame)
        {
            pouFrame->GetName(strName);
            msgName = strName.c_str();
        }
        else
        {
            if (formatHexForId == true)
            {
                msgName.Format("0x%X", sCANMsg.m_uDataInfo.m_sCANMsg.m_unMsgID);
            }
            else
            {
                msgName.Format("%d", sCANMsg.m_uDataInfo.m_sCANMsg.m_unMsgID);
            }
        }
    }
    return S_OK;
}

HRESULT CMsgContainerCAN::GetErrorInfo(STCANDATA& sCANMsg, CString& msgName)
{
    CString omErrMsg("");
    BOOL bErrProcessed = FALSE;
    int nCount = 0;

    auto nMsgCode = usProcessCurrErrorEntry(sCANMsg.m_uDataInfo.m_sErrInfo);

    while ((nCount < ERRORS_DEFINED) && (bErrProcessed == FALSE))
    {
        if (nMsgCode == sg_asErrorEntry[nCount].m_usErrorCode)
        {
            msgName = _(sg_asErrorEntry[nCount].m_ptcErorMsg);
            bErrProcessed = TRUE;
        }
        nCount++;
    }

    if (bErrProcessed == FALSE)
    {
        msgName.Format(_("Error code: %X"), sCANMsg.m_uDataInfo.m_sErrInfo.m_ucReg_ErrCap);
    }
    return S_OK;
}