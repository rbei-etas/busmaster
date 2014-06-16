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
 * \file      GUI_FormatMsgCAN.cpp
 * \brief     Implementation of CFormatMsgFlexRay class
 * \author    Anish Kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of CFormatMsgFlexRay class
 */


#include "CommonClass_stdafx.h"
#include "GUI_FormatMsgFlexRay.h"
#include "include/Utils_macro.h"
#include "Include/CAN_Error_Defs.h"
#include "Utility\MultiLanguageSupport.h"

struct sFLXCNTRINFO
{
    unsigned short m_usFLXCNTRCode; // Controller status code
    char* m_ptcFLXCNTRMsg;          // Controller message
};
typedef sFLXCNTRINFO  SFLXCNTRMSGINFO;
typedef sFLXCNTRINFO* PFLXCNTRMSGINFO;
const int ERRORS_DEFINED = 15;
const int SIZE_WORD = sizeof(WORD);

#define defBITS_IN_FOUR_BYTE              32
const int TX_MESSAGE   = 0x10000000;  // bitwise OR to make it a Tx message
const int RX_MESSAGE   = 0x20000000;  // bitwise OR to make it a Rx message
const int CH_A_MESSAGE = 0x40000000;  // bitwise OR to make it a Channnel A message
const int CH_B_MESSAGE = 0x80000000;  // bitwise OR to make it a Channnel B  message

/*
    FLEXRAY_POCS_DEFAULT_CONFIG,
    FLEXRAY_POCS_READY,
    FLEXRAY_POCS_NORMAL_ACTIVE,
    FLEXRAY_POCS_NORMAL_PASSIVE,
    FLEXRAY_POCS_HALT,
    FLEXRAY_POCS_MONITOR_MODE,
    FLEXRAY_POCS_CONFIG,
    FLEXRAY_POCS_WAKEUP_STANDBY,
    FLEXRAY_POCS_WAKEUP_LISTEN,
    FLEXRAY_POCS_WAKEUP_SEND,
    FLEXRAY_POCS_WAKEUP_DETECT,
    FLEXRAY_POCS_STARTUP_PREPARE,
    FLEXRAY_POCS_COLDSTART_LISTEN,
    FLEXRAY_POCS_COLDSTART_COLLISION_RESOLUTION,
    FLEXRAY_POCS_COLDSTART_CONSISTENCY_CHECK,
    FLEXRAY_POCS_COLDSTART_GAP,
    FLEXRAY_POCS_COLDSTART_JOIN,
    FLEXRAY_POCS_INTEGRATION_COLDSTART_CHECK,
    FLEXRAY_POCS_INTEGRATION_LISTEN,
    FLEXRAY_POCS_INTEGRATION_CONSISTENCY_CHECK,
    FLEXRAY_POCS_INITIALIZE_SCHEDULE,
    FLEXRAY_POCS_ABORT_STARTUP  */

static SFLXCNTRMSGINFO sg_asFLXCNTREntry[] =
{
    {FLEXRAY_POCS_DEFAULT_CONFIG,                   "FLEXRAY_POCS_DEFAULT_CONFIG"},
    {FLEXRAY_POCS_READY,                            "FLEXRAY_POCS_READY"},
    {FLEXRAY_POCS_NORMAL_ACTIVE,                    "FLEXRAY_POCS_NORMAL_ACTIVE"},
    {FLEXRAY_POCS_NORMAL_PASSIVE,                   "FLEXRAY_POCS_NORMAL_PASSIVE"},
    {FLEXRAY_POCS_HALT,                             "FLEXRAY_POCS_HALT"},
    {FLEXRAY_POCS_MONITOR_MODE,                     "FLEXRAY_POCS_MONITOR_MODE"},
    {FLEXRAY_POCS_CONFIG,                           "FLEXRAY_POCS_CONFIG"},
    {FLEXRAY_POCS_WAKEUP_STANDBY,                   "FLEXRAY_POCS_WAKEUP_STANDBY"},
    {FLEXRAY_POCS_WAKEUP_LISTEN,                    "FLEXRAY_POCS_WAKEUP_LISTEN"},
    {FLEXRAY_POCS_WAKEUP_SEND,                      "FLEXRAY_POCS_WAKEUP_SEND"},
    {FLEXRAY_POCS_WAKEUP_DETECT,                    "FLEXRAY_POCS_WAKEUP_DETECT"},
    {FLEXRAY_POCS_STARTUP_PREPARE,                  "FLEXRAY_POCS_STARTUP_PREPARE"},
    {FLEXRAY_POCS_COLDSTART_LISTEN,                 "FLEXRAY_POCS_COLDSTART_LISTEN"},
    {FLEXRAY_POCS_COLDSTART_COLLISION_RESOLUTION,   "FLEXRAY_POCS_COLDSTART_COLLISION_RESOLUTION"},
    {FLEXRAY_POCS_COLDSTART_CONSISTENCY_CHECK,      "FLEXRAY_POCS_COLDSTART_CONSISTENCY_CHECK"},
    {FLEXRAY_POCS_COLDSTART_GAP,                    "FLEXRAY_POCS_COLDSTART_GAP"},
    {FLEXRAY_POCS_COLDSTART_JOIN,                   "FLEXRAY_POCS_COLDSTART_JOIN"},
    {FLEXRAY_POCS_INTEGRATION_COLDSTART_CHECK,      "FLEXRAY_POCS_INTEGRATION_COLDSTART_CHECK"},
    {FLEXRAY_POCS_INTEGRATION_LISTEN,               "FLEXRAY_POCS_INTEGRATION_LISTEN"},
    {FLEXRAY_POCS_INTEGRATION_CONSISTENCY_CHECK,    "FLEXRAY_POCS_INTEGRATION_CONSISTENCY_CHECK"},
    {FLEXRAY_POCS_INITIALIZE_SCHEDULE,              "FLEXRAY_POCS_INITIALIZE_SCHEDULE"},
    {FLEXRAY_POCS_ABORT_STARTUP,                    "FLEXRAY_POCS_ABORT_STARTUP"},
};

/*******************************************************************************
  Function Name  : CFormatMsgFlexRay
  Input(s)       : -
  Output         : -
  Functionality  : Constructor
  Member of      : CFormatMsgFlexRay
  Author(s)      : Ratnadip
  Date Created   : 8.7.2009
  Modifications  :
*******************************************************************************/
CFormatMsgFlexRay::CFormatMsgFlexRay(void)
{
}

/*******************************************************************************
  Function Name  : ~CFormatMsgFlexRay
  Input(s)       : -
  Output         : -
  Functionality  : Destructor
  Member of      : CFormatMsgFlexRay
  Author(s)      : Ratnadip
  Date Created   : 8.7.2009
  Modifications  :
*******************************************************************************/
CFormatMsgFlexRay::~CFormatMsgFlexRay(void)
{
}

/*******************************************************************************
  Function Name  : vFormatTime
  Input(s)       : -
  Output         : -
  Functionality  : Format Time
  Member of      : CFormatMsgFlexRay
  Author(s)      : Ratnadip
  Date Created   : 8.7.2009
  Modifications  :
*******************************************************************************/
void CFormatMsgFlexRay::vFormatTime(BYTE bExprnFlag, FLXMSGDATA* CurrDataFlexRay)
{
    BYTE bTmpExprnFlag = bExprnFlag;
    if (IS_TM_ABS_SET(bExprnFlag))
    {
        CLEAR_EXPR_TM_BITS(bTmpExprnFlag);
        SET_TM_ABS(bTmpExprnFlag);
        vCalculateAndFormatTM(bTmpExprnFlag, CurrDataFlexRay->m_u64TimeStamp,
                              CurrDataFlexRay->m_acTimeAbs);
    }

    if (IS_TM_REL_SET(bExprnFlag))
    {
        CLEAR_EXPR_TM_BITS(bTmpExprnFlag);
        SET_TM_REL(bTmpExprnFlag);
        vCalculateAndFormatTM(bTmpExprnFlag, CurrDataFlexRay->m_u64TimeStamp,
                              CurrDataFlexRay->m_acTimeRel);
    }

    if (IS_TM_SYS_SET(bExprnFlag))
    {
        CLEAR_EXPR_TM_BITS(bTmpExprnFlag);
        SET_TM_SYS(bTmpExprnFlag);
        vCalculateAndFormatTM(bTmpExprnFlag, CurrDataFlexRay->m_u64TimeStamp,
                              CurrDataFlexRay->m_acTimeSys);
    }
}


/*******************************************************************************
  Function Name  : vFormatDataAndId
  Input(s)       : -
  Output         : -
  Functionality  : Format data and id
  Member of      : CFormatMsgFlexRay
  Author(s)      : Ratnadip
  Date Created   : 8.7.2009
  Modifications  :
*******************************************************************************/
void CFormatMsgFlexRay::vFormatDataAndId(BYTE bExprnFlag, FLXMSGDATA* CurrFlxData)
{
    if (IS_NUM_HEX_SET(bExprnFlag))
    {
        if ( CurrFlxData->m_ucCycleRepetetion == 0 )
        {
            //Database message
            sprintf(CurrFlxData->m_acMsgIDHex, FORMAT_STR_ID_HEX, CurrFlxData->m_wMsgID);
        }
        else
        {
            sprintf(CurrFlxData->m_acMsgIDHex, "0x%03X [0x%X, 0x%X]", CurrFlxData->m_wMsgID, CurrFlxData->m_nBaseCycle, CurrFlxData->m_ucCycleRepetetion);
        }


        int i = 0, j = 0;
        for (i = 0, j = 0; i < CurrFlxData->m_byDataLength; i++)
        {
            unsigned char CurrDat = CurrFlxData->m_awData[i];
            sprintf(&(CurrFlxData->m_acDataHex[j]), FORMAT_STR_DATA_HEX, CurrDat);
            j += 3;
        }
        CurrFlxData->m_acDataHex[j] = '\0';
    }

    if (IS_NUM_DEC_SET(bExprnFlag))
    {
        if ( CurrFlxData->m_ucCycleRepetetion == 0 )
        {
            //Database message
            sprintf(CurrFlxData->m_acMsgIDDec, FORMAT_STR_ID_DEC, CurrFlxData->m_wMsgID);
        }
        else
        {
            sprintf(CurrFlxData->m_acMsgIDDec, "%04d [%d, %d]", CurrFlxData->m_wMsgID, CurrFlxData->m_nBaseCycle, CurrFlxData->m_ucCycleRepetetion);
        }

        int j = 0;
        for (int i = 0, j = 0; i < CurrFlxData->m_byDataLength; i++)
        {
            unsigned char CurrDat = CurrFlxData->m_awData[i];
            sprintf(&(CurrFlxData->m_acDataDec[j]), FORMAT_STR_DATA_DEC, CurrDat);
            j += 4;
            CurrFlxData->m_acDataDec[j-1] = L' ';
        }
        CurrFlxData->m_acDataDec[j-1] = '\0';
    }
}

/*******************************************************************************
  Function Name  : usProcessCurrErrorEntry
  Input(s)       : SERROR_INFO& sErrorInfo
  Output         : Returns Error ID as USHORT.
  Functionality  : Processes the current Error entry and returns the Error code.
  Member of      : CFormatMsgFlexRay
  Author(s)      : Arunkumar K
  Date Created   : 08-09-2010
  Modifications  :
*******************************************************************************/
USHORT CFormatMsgFlexRay::usProcessCurrErrorEntry(SERROR_INFO& sErrorInfo)
{
    // Get the Error code
    USHORT usErrorID;

    if (sErrorInfo.m_ucErrType == ERROR_BUS)
    {
        // Update Statistics information
        usErrorID = sErrorInfo.m_ucReg_ErrCap /*& 0xE0*/;
    }
    else if (sErrorInfo.m_ucErrType == ERROR_WARNING_LIMIT_REACHED)
    {
        usErrorID = ERROR_UNKNOWN;
    }
    else if (sErrorInfo.m_ucErrType == ERROR_INTERRUPT)
    {
        usErrorID = ERROR_UNKNOWN;
    }
    else
    {
        usErrorID = sErrorInfo.m_ucErrType;
    }

    return usErrorID;
}

/*******************************************************************************
  Function Name  : vFormatCurrErrorEntry
  Input(s)       : usErrorID
  Output         : TCHAR* (Error Name)
  Functionality  : Gets the pointer to Message data Buffer from PSDI_CAN DLL
                   and updates the same in List Control.
  Member of      : CFormatMsgFlexRay
  Author(s)      : Arunkumar K
  Date Created   : 08-09-2010
  Modifications  :
*******************************************************************************/
char* CFormatMsgFlexRay::vFormatCurrErrorEntry(USHORT /* usErrorID */)
{
    return nullptr;
}

/*******************************************************************************
  Function Name  : vFormatCANDataMsg
  Input(s)       : -
  Output         : -
  Functionality  : Format CAN data bytes
  Member of      : CFormatMsgFlexRay
  Author(s)      : Ratnadip
  Date Created   : 8.7.2009
  Modifications  : ArunKumar K
                   08.09.2010,
                   Added handler for error messages. ERR_FLAG
*******************************************************************************/
void CFormatMsgFlexRay::vFormatFlexRayDataMsg(s_FLXMSG* pMsgFlexRay,
        FLXMSGDATA* CurrDataFlexRay,
        BYTE bExprnFlag_Log)
{
    DWORD dwIFlags = pMsgFlexRay->stcDataMsg.m_lHeaderInfoFlags;

    if ( pMsgFlexRay->stcDataMsg.m_bIsRxMsg == true )
    {
        CurrDataFlexRay->m_acDirection[0] = 'R';
    }
    else
    {
        CurrDataFlexRay->m_acDirection[0] = 'T';
    }
    CurrDataFlexRay->m_acDirection[1] = 'x';

    // Find out the type of the frame
    if (dwIFlags & RBIN_FLXHDRINFO_NULLFRAME)
    {
        // nullptr frame
        CurrDataFlexRay->m_ColourCode = RGB(0,0,0);//m_MsgColorProp.m_sMsgProp.m_clrNullFrame;
    }
    else if (dwIFlags & RBIN_FLXHDRINFO_SYNCFRAME)
    {
        // Sync frame
        CurrDataFlexRay->m_ColourCode = RGB(0,0,0);//m_MsgColorProp.m_sMsgProp.m_clrSyncFrame;
    }
    else if (dwIFlags & RBIN_FLXHDRINFO_FRAMETYPE_DYNAMIC)
    {
        // Dynamic frame
        CurrDataFlexRay->m_ColourCode =RGB(0,0,0);// m_MsgColorProp.m_sMsgProp.m_clrDynamicFrame;
    }
    else
    {
        // Static frame
        CurrDataFlexRay->m_ColourCode = RGB(128,0,0);//m_MsgColorProp.m_sMsgProp.m_clrStaticFrame;
    }

    CurrDataFlexRay->m_Channel = pMsgFlexRay->stcDataMsg.m_eChannel;

    if (CurrDataFlexRay->m_Channel == CHANNEL_AB)
    {
        //Indicates that the message is transmitted on Channel A or B
        strcpy(CurrDataFlexRay->m_acChannel, "AB");
    }
    else
    {
        CurrDataFlexRay->m_acChannel[1] = '\0';
        if (CurrDataFlexRay->m_Channel == CHANNEL_B)
        {
            CurrDataFlexRay->m_Channel = CHANNEL_B;
            CurrDataFlexRay->m_acChannel[0] = 'B';
        }
        else
        {
            CurrDataFlexRay->m_Channel = CHANNEL_A;
            CurrDataFlexRay->m_acChannel[0] = 'A';
        }
    }


    if (dwIFlags & RBIN_FLXHDRINFO_FRAMETYPE_DYNAMIC)
    {
        CurrDataFlexRay->m_eFrameType = FLX_FRAMETYPE_DYNAMIC;
        strcpy(CurrDataFlexRay->m_acSlotType, "dynamic");
    }
    else
    {
        CurrDataFlexRay->m_eFrameType = FLX_FRAMETYPE_STATIC;
        strcpy(CurrDataFlexRay->m_acSlotType, "static");
    }

    if ( ( dwIFlags & RBIN_FLXHDRINFO_STARTUPFRAME ) && (dwIFlags & RBIN_FLXHDRINFO_SYNCFRAME) )
    {
        strcpy(CurrDataFlexRay->m_acFrameType, "sync, startup");
    }

    else if (dwIFlags & RBIN_FLXHDRINFO_SYNCFRAME)
    {
        CurrDataFlexRay->m_eFrameType = FLX_FRAMETYPE_SYNC;
        strcpy(CurrDataFlexRay->m_acFrameType, "sync");
    }
    else if (dwIFlags & RBIN_FLXHDRINFO_SYNCFRAME)
    {
        CurrDataFlexRay->m_eFrameType = FLX_FRAMETYPE_SYNC;
        strcpy(CurrDataFlexRay->m_acFrameType, "sync");
    }

    else if (dwIFlags & RBIN_FLXHDRINFO_NULLFRAME)
    {
        CurrDataFlexRay->m_eFrameType = FLX_FRAMETYPE_NULL;
        strcpy(CurrDataFlexRay->m_acFrameType, "Null");
    }

    else
    {
        CurrDataFlexRay->m_eFrameType = FLX_FRAMETYPE_SYNC;
        strcpy(CurrDataFlexRay->m_acFrameType, "Data");
    }

    if ( pMsgFlexRay->m_eMessageType == FLXMSGTYPE_STATUS )
    {
        CurrDataFlexRay->m_eFrameType = FLX_FRAMETYPE_STATUS;
        strcpy(CurrDataFlexRay->m_acFrameType, "status");
    }

    CurrDataFlexRay->m_ucCycleRepetetion = pMsgFlexRay->stcDataMsg.m_ucRepetition;
    CurrDataFlexRay->m_nBaseCycle = pMsgFlexRay->stcDataMsg.m_nBaseCycle;
    _itoa(pMsgFlexRay->stcDataMsg.m_ucCycleNumber, CurrDataFlexRay->m_acCycleNum, 10);
    _itoa(pMsgFlexRay->stcDataMsg.m_nDLC, CurrDataFlexRay->m_acDataLen, 10);
    strcpy_s(CurrDataFlexRay->m_acMsgDesc, 256, "Description");

    CurrDataFlexRay->m_u64TimeStamp = pMsgFlexRay->m_unTimestamp;
    CurrDataFlexRay->m_n64MapId = n64MakeChannelTxSpecific(*pMsgFlexRay);
    CurrDataFlexRay->m_wMsgID = pMsgFlexRay->stcDataMsg.m_nSlotID;
    CurrDataFlexRay->m_byDataLength = pMsgFlexRay->stcDataMsg.m_nDLC;

    memcpy(CurrDataFlexRay->m_awData, pMsgFlexRay->stcDataMsg.m_ucData, pMsgFlexRay->stcDataMsg.m_nDLC);

    vFormatTime(bExprnFlag_Log, CurrDataFlexRay);

    if ( CurrDataFlexRay->m_eFrameType == FLX_FRAMETYPE_STATUS )
    {
        if ( pMsgFlexRay->stcStatusMsg.m_oeFlexRayStatus < FLEXRAY_POCS_END )
        {
            int nType = pMsgFlexRay->stcStatusMsg.m_oeFlexRayStatus;
            if (IS_NUM_HEX_SET(bExprnFlag_Log))
            {
                sprintf(CurrDataFlexRay->m_acDataHex, FORMAT_FLX_CONTROLLER_STATE, sg_asFLXCNTREntry[nType].m_ptcFLXCNTRMsg);
            }
            else
            {
                sprintf(CurrDataFlexRay->m_acDataDec, FORMAT_FLX_CONTROLLER_STATE, sg_asFLXCNTREntry[nType].m_ptcFLXCNTRMsg);
            }
        }
    }
    else
    {
        vFormatDataAndId(bExprnFlag_Log, CurrDataFlexRay);
    }
}

//void CFormatMsgFlexRay::vCalculateAndFormatTM(BYTE bExprnFlag, UINT64 TimeStamp,
//                                             __int64 n64MapId, char acTime[])
//{
//    /* In order to make this function work properly ENSURE bExprnFlag has ONLY
//    ONE time mode bit up */
//
//    DWORD dwTSTmp = 0; // temporary time stamp
//
//    if (IS_TM_SYS_SET(bExprnFlag))
//    {
//        dwTSTmp = (TimeStamp - m_qwAbsBaseTime) + m_qwRefSysTime;
//    }
//    else if (IS_TM_REL_SET(bExprnFlag))
//    {
//        if (IS_MODE_APPEND(bExprnFlag))
//        {
//            if (m_qwRelBaseTime == 0)
//            {
//                m_qwRelBaseTime = TimeStamp;
//            }
//            dwTSTmp = TimeStamp - m_qwRelBaseTime;
//        }
//        else if (IS_MODE_OVER(bExprnFlag) || IS_MODE_INTRP(bExprnFlag) )
//        {
//            SMSGDISPMAPENTRY nNewPos;
//            if (m_omMessageIndex.Lookup(n64MapId, nNewPos)) // ID is found
//            {
//              dwTSTmp =  TimeStamp - nNewPos.m_nTimeStamp;
//            }
//            else
//            {
//              dwTSTmp = 0;
//            }
//        }
//        else
//        {
//            ASSERT(false);
//        }
//        m_qwRelBaseTime = TimeStamp;
//    }
//    else if (IS_TM_ABS_SET(bExprnFlag))
//    {
//        dwTSTmp = TimeStamp - m_qwAbsBaseTime;
//    }
//    else
//    {
//        ASSERT(false);
//    }
//
//     // Static variables to reduce the creation time
//    static int nTemp, nMicSec, nSec, nMinute, nHour;
//
//    nMicSec = dwTSTmp % 10000;  // hundreds of microseconds left
//    nTemp = dwTSTmp / 10000;    // expressed in seconds
//    nSec = nTemp % 60;          // seconds left
//    nTemp = nTemp / 60;         // expressed in minutes
//    nMinute = nTemp % 60;       // minutes left
//    nHour = nTemp / 60;         // expressed in hours
//
//    sprintf(acTime, "%02d:%02d:%02d:%04d", nHour, nMinute, nSec, nMicSec);
//}

__int64 CFormatMsgFlexRay::n64MakeChannelTxSpecific(s_FLXMSG FlxMsg)
{
    __int64 n64Return = 0;
    WORD wMsgId = FlxMsg.stcDataMsg.m_nSlotID;
    DWORD dwIFlags = FlxMsg.stcDataMsg.m_lHeaderInfoFlags;
    if (dwIFlags & RBIN_FLXHDRINFO_SELFRECEPTION)
    {
        //TX
        n64Return = wMsgId | TX_MESSAGE;
    }
    else
    {
        //RX
        n64Return = wMsgId | RX_MESSAGE;
    }

    if (dwIFlags & RBIN_FLXHDRINFO_CHANNELID)
    {
        //Channel b
        n64Return |= wMsgId | CH_B_MESSAGE;
    }
    else
    {
        //Channel a
        n64Return |= wMsgId | CH_A_MESSAGE;
    }
    return n64Return;
}