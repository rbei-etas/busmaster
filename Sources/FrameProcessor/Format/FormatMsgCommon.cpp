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
 * \file      FormatMsgCommon.cpp
 * \brief     Source of CFormatMsgCommon class.
 * \author    Anish Kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source of CFormatMsgCommon class.
 */

#include "../FrameProcessor_stdafx.h"
#include "FormatMsgCommon.h"

#include "include/Utils_Macro.h"

CFormatMsgCommon::CFormatMsgCommon(CRefTimeKeeper & ouRefTimeKeeper) :
    m_ouRefTimeKeeper(ouRefTimeKeeper)
{
    m_qwRelBaseTime = 0;
    m_qwResTime = 0;
    m_bResetMsgAbsTime = FALSE;
    m_qwLogDelayTime =0;
}

CFormatMsgCommon::~CFormatMsgCommon(void)
{
}

void CFormatMsgCommon::vCalculateAndFormatTM(BYTE bExprnFlag, UINT64 TimeStamp, char acTime[])
{
    UINT64 qwRefSysTime, qwAbsBaseTime;
    m_ouRefTimeKeeper.vGetTimeParams(qwRefSysTime, qwAbsBaseTime);

    /* temporary time stamp */
    DWORD dwTSTmp = 0;
    if (IS_TM_SYS_SET(bExprnFlag))
    {
        dwTSTmp = (DWORD) ((TimeStamp - qwAbsBaseTime) + qwRefSysTime);
    }
    else if (IS_TM_REL_SET(bExprnFlag))
    {
        if (m_qwRelBaseTime == 0)
        {
            m_qwRelBaseTime = TimeStamp;
        }

        /* Time difference should be +ve value */
        if(TimeStamp >= m_qwRelBaseTime)
        {
            dwTSTmp = (DWORD) (TimeStamp - m_qwRelBaseTime);
        }
        else
        {
            dwTSTmp = (DWORD) (m_qwRelBaseTime - TimeStamp);
        }

        m_qwRelBaseTime = TimeStamp;
    }
    else if (IS_TM_ABS_SET(bExprnFlag))
    {
        /* Time difference should be +ve value */
        if(TimeStamp >= qwAbsBaseTime)
        {
            dwTSTmp = (DWORD) (TimeStamp - qwAbsBaseTime);
        }
        else
        {
            dwTSTmp = (DWORD) (qwAbsBaseTime - TimeStamp);
        }

    }
    else if (IS_TM_ABS_RES(bExprnFlag))
    {
        if(m_bResetMsgAbsTime == TRUE)
        {
            /* reset the time for new logging session */
            SYSTEMTIME LocalCurrTime;
            GetLocalTime(&LocalCurrTime);

            UINT64 RefCurrTime =
                LocalCurrTime.wHour         * 36000000 +
                LocalCurrTime.wMinute       *   600000 +
                LocalCurrTime.wSecond       *    10000 +
                LocalCurrTime.wMilliseconds *       10;

            UINT64 RefLogTime =
                m_LogSysTime.wHour         * 36000000 +
                m_LogSysTime.wMinute       *   600000 +
                m_LogSysTime.wSecond       *    10000 +
                m_LogSysTime.wMilliseconds *       10;

            /* for log & msg delay time */
            m_qwLogDelayTime = (DWORD)(RefCurrTime - RefLogTime);

            /* Time difference should be +ve value */
            if(TimeStamp >= qwAbsBaseTime)
            {
                m_qwResTime = TimeStamp - qwAbsBaseTime;
            }
            else
            {
                m_qwResTime = qwAbsBaseTime - TimeStamp;
            }
            m_bResetMsgAbsTime = FALSE;
        }

        /* Time difference should be +ve value */
        if(TimeStamp >= (qwAbsBaseTime + m_qwResTime))
        {
            dwTSTmp = (DWORD) (TimeStamp - qwAbsBaseTime - m_qwResTime );
        }
        else
        {
            dwTSTmp = (DWORD) (m_qwResTime + qwAbsBaseTime - TimeStamp );
        }

        /* add msg delay time */
        dwTSTmp = dwTSTmp + m_qwLogDelayTime;

    }
    else
    {
        ASSERT(false);
    }

    vFormatTimeStamp(dwTSTmp, acTime);
}

void CFormatMsgCommon::vFormatTimeStamp(DWORD dwTimeStamp, char acTime[])
{
    /* Static variables to reduce the creation time */
    static int nTemp, nMicSec, nSec, nMinute, nHour;

    nMicSec = dwTimeStamp % 10000;  // hundreds of microseconds left
    nTemp = dwTimeStamp / 10000;    // expressed in seconds
    nSec = nTemp % 60;          // seconds left
    nTemp = nTemp / 60;         // expressed in minutes
    nMinute = nTemp % 60;       // minutes left
    nHour = nTemp / 60;         // expressed in hours

    sprintf(acTime,  "%02d:%02d:%02d:%04d", nHour, nMinute, nSec, nMicSec);
}

void CFormatMsgCommon::vSetRelBaseTime(INT64 qwRelBaseTime)
{
    m_qwRelBaseTime = qwRelBaseTime;
}
