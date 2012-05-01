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
 * \file      LogObjectCAN.cpp
 * \brief     Source file for CLogObjectCAN class.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file for CLogObjectCAN class.
 */

#include "FrameProcessor_stdafx.h"
#include "include/Struct_CAN.h"
#include "LogObjectCAN.h"            // For CLogObjectCAN class declaration


#define CAN_VERSION           _T("***BUSMASTER Ver 1.6.2***")
#define CAN_LOG_HEADER        _T("***NOTE: PLEASE DO NOT EDIT THIS DOCUMENT***")
#define CAN_LOG_START         _T("***[START LOGGING SESSION]***")
#define CAN_LOG_STOP          _T("***[STOP LOGGING SESSION]***")
#define CAN_LOG_START_DATE_TIME \
    _T("***START DATE AND TIME %d:%d:%d %d:%d:%d:%d***")
#define CAN_LOG_END_DATE_TIME _T("***END DATE AND TIME %d:%d:%d %d:%d:%d:%d***")
#define CAN_LOG_HEXFORMAT     _T("***HEX***")
#define CAN_LOG_DECFORMAT     _T("***DEC***")
#define CAN_LOG_ABSMODE       _T("***ABSOLUTE MODE***")
#define CAN_LOG_RELMODE       _T("***RELATIVE MODE***")
#define CAN_LOG_SYSMODE       _T("***SYSTEM MODE***")
#define CAN_LOG_COLUMNS       _T("***<Time><Tx/Rx><Channel><CAN ID><Type><DLC><DataBytes>***")



CLogObjectCAN::CLogObjectCAN(CString omVersion):CBaseLogObject(omVersion)
{
    // Initialise the filtering block
    m_sFilterApplied.vClear();
    m_pasControllerDetails = NULL;
    m_nNumChannels = 0;
}

CLogObjectCAN::~CLogObjectCAN()
{
    if (NULL != m_pasControllerDetails)
    {
        delete[] m_pasControllerDetails;
        m_pasControllerDetails = NULL;
    }
}

void CLogObjectCAN::Der_CopySpecificData(const CBaseLogObject* pouLogObjRef)
{
    const CLogObjectCAN* pouLobObjCANRef = static_cast <const CLogObjectCAN*> (pouLogObjRef);
    pouLobObjCANRef->GetFilterInfo(m_sFilterApplied);
}

BOOL CLogObjectCAN::bLogData(const SFORMATTEDDATA_CAN& sDataCAN)
{
    // Multiple return statements are used to keep the code precise.
    SFRAMEINFO_BASIC_CAN CANInfo_Basic =
    {
        sDataCAN.m_dwMsgID, sDataCAN.m_eChannel, sDataCAN.m_eDirection,
        sDataCAN.m_byIDType, sDataCAN.m_byMsgType
    };

    // Assign appropriate values to FrameInfo_Basic

    if (bToBeLogged(CANInfo_Basic) == FALSE)
    {
        return FALSE;
    }

    CString omLogText = _T("");
    TCHAR* pTimeData = NULL;
    TCHAR* pId = NULL;
    TCHAR* pData = NULL;

    switch (m_sLogInfo.m_eLogTimerMode) // Time Mode
    {
        case TIME_MODE_ABSOLUTE:
        {
            if(m_sLogInfo.m_bResetAbsTimeStamp)
            {
                pTimeData = (TCHAR*) (sDataCAN.m_acTimeAbsReset);
            }
            else
            {
                pTimeData = (TCHAR*) (sDataCAN.m_acTimeAbs);
            }
        }
        break;

        case TIME_MODE_RELATIVE:
        {
            pTimeData = (TCHAR*) (sDataCAN.m_acTimeRel);
        }
        break;

        case TIME_MODE_SYSTEM:
        {
            pTimeData = (TCHAR*) (sDataCAN.m_acTimeSys);
        }
        break;

        default:
            ASSERT(FALSE);
            break;
    }

    switch (m_sLogInfo.m_eNumFormat)
    {
        case HEXADECIMAL:
        {
            pId = (TCHAR*) (sDataCAN.m_acMsgIDHex);
            pData = (TCHAR*) (sDataCAN.m_acDataHex);
        }
        break;

        case DEC:
        {
            pId = (TCHAR*) (sDataCAN.m_acMsgIDDec);
            pData = (TCHAR*) (sDataCAN.m_acDataDec);
        }
        break;

        default:
            ASSERT(FALSE);
            break;
    }

    // First put everything in a string to get the length
    omLogText.Format(  _T("%s %s %s %s %s %s %s\n"),
                       pTimeData,
                       sDataCAN.m_acMsgDir,
                       sDataCAN.m_acChannel,
                       pId,
                       sDataCAN.m_acType,
                       sDataCAN.m_acDataLen,
                       pData);
    vWriteTextToFile(omLogText);
    return TRUE;
}

// To format the header
void CLogObjectCAN::vFormatHeader(CString& omHeader)
{
    CBaseLogObject::vFormatHeader(omHeader);
    omHeader += CAN_LOG_COLUMNS;
    omHeader += L'\n';
}

// To format the footer
void CLogObjectCAN::vFormatFooter(CString& omFooter)
{
    CBaseLogObject::vFormatFooter(omFooter);
}

/*******************************************************************************
//  Function Name  : bLogData
//  Input(s)       : none
//  Output         : none
//  Description    : Logs data. This will open the file in
//                   appropriate mode and will insert header if it is new.
//  Member of      : CBaseLogObject
//  Friend of      : None
//  Author         : Arun Kumar
//  Creation Date  : 10/11/06
//  Modifications  : Anish, the message structure is changed
//  Modifications  : Ratnadip Choudhury.
                     1. Changed the prototype
                     2. Shifted the formatting codes into vWriteTextToFile(...)
*******************************************************************************/

BOOL CLogObjectCAN::bToBeLogged(SFRAMEINFO_BASIC_CAN& CANInfo_Basic)
{
    // Multiple return statements are used to keep the code precise.
    if (m_sLogInfo.m_bEnabled == FALSE)
    {
        return FALSE;
    }

    if (NULL == m_pLogFile)
    {
        ASSERT(FALSE);
        return FALSE;
    }

    // Return if the curent frame occurs in different channel.
    if (CAN_CHANNEL_ALL != m_sLogInfo.m_ChannelSelected)
    {
        if (m_sLogInfo.m_ChannelSelected != CANInfo_Basic.m_eChannel)
        {
            return FALSE;
        }
    }

    if (m_sFilterApplied.m_bEnabled == TRUE)
    {
        if (m_sFilterApplied.bToBeBlocked(CANInfo_Basic) == TRUE)
        {
            return FALSE;
        }
    }

    // Check for the triggering conditions
    switch (m_CurrTriggerType)
    {
        case NONE:
            break;

        case STOPPED:
        {
            //If the log file is stopped then don't log
            return FALSE;
        }
        break;

        case START:
        {
            if ((m_sLogInfo.m_sLogTrigger.m_unStartID == CANInfo_Basic.m_dwFrameID)
                    && (CANInfo_Basic.m_eDrctn == DIR_RX))
            {
                m_CurrTriggerType = NONE;
            }
            else
            {
                return FALSE;
            }
        }
        break;

        case STOP:
        {
            if ((m_sLogInfo.m_sLogTrigger.m_unStopID == CANInfo_Basic.m_dwFrameID)
                    && (CANInfo_Basic.m_eDrctn  == DIR_RX))
            {
                m_CurrTriggerType = STOPPED;
            }
        }
        break;

        case BOTH:
        {
            if ((m_sLogInfo.m_sLogTrigger.m_unStartID == CANInfo_Basic.m_dwFrameID)
                    && (CANInfo_Basic.m_eDrctn == DIR_RX))
            {
                m_CurrTriggerType = STOP;
            }
            else
            {
                return FALSE;
            }
        }
        break;

        default:
            ASSERT(FALSE);
            break;
    }

    return TRUE;
}

BYTE* CLogObjectCAN::Der_SetConfigData(BYTE* pvDataStream)
{
    bool bResult = false;
    BYTE* pbSStream = pvDataStream;
    pbSStream = m_sFilterApplied.pbSetConfigData(pbSStream, bResult);
    return pbSStream;
}

BYTE* CLogObjectCAN::Der_GetConfigData(BYTE* pvDataStream) const
{
    BYTE* pbTStream = pvDataStream;
    pbTStream = m_sFilterApplied.pbGetConfigData(pbTStream);
    return pbTStream;
}

UINT CLogObjectCAN::Der_unGetBufSize(void) const
{
    return m_sFilterApplied.unGetSize();
}

void CLogObjectCAN::EnableFilter(BOOL bEnable)
{
    m_sFilterApplied.m_bEnabled = bEnable;
}

void CLogObjectCAN::GetFilterInfo(SFILTERAPPLIED_CAN& sFilterInfo) const
{
    sFilterInfo.bClone(m_sFilterApplied);
}

void CLogObjectCAN::SetFilterInfo(const SFILTERAPPLIED_CAN& sFilterInfo)
{
    m_sFilterApplied.bClone(sFilterInfo);
}

void CLogObjectCAN::Der_SetDatabaseFiles(const CStringArray& omList)
{
    // Clear before updating
    m_omListDBFiles.RemoveAll();

    for (int nIdx = 0; nIdx < omList.GetSize(); nIdx++)
    {
        m_omListDBFiles.Add(omList.GetAt(nIdx));
    }
}

// Get the list of database files associated
void CLogObjectCAN::Der_GetDatabaseFiles(CStringArray& omList)
{
    omList.Append(m_omListDBFiles);
}

void CLogObjectCAN::Der_SetChannelBaudRateDetails
(SCONTROLLER_DETAILS* controllerDetails,
 int nNumChannels)
{
    if (NULL != m_pasControllerDetails)
    {
        delete[] m_pasControllerDetails;
    }

    m_pasControllerDetails = NULL;
    m_pasControllerDetails = new SCONTROLLER_DETAILS [nNumChannels];

    for (int nIdx = 0; nIdx < nNumChannels; nIdx++)
    {
        memcpy(m_pasControllerDetails + nIdx, controllerDetails + nIdx, sizeof(SCONTROLLER_DETAILS));
    }

    m_nNumChannels = nNumChannels;
}

// To get the channel baud rate info for each channel
void CLogObjectCAN::Der_GetChannelBaudRateDetails
(SCONTROLLER_DETAILS* controllerDetails, int& nNumChannels)
{
    if (NULL != m_pasControllerDetails)
    {
        for (int nIdx = 0; nIdx < m_nNumChannels; nIdx++)
        {
            memcpy(controllerDetails + nIdx, m_pasControllerDetails + nIdx, sizeof(SCONTROLLER_DETAILS));
        }

        nNumChannels = m_nNumChannels;
    }
}

