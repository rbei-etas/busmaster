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
 * @brief Implementation of CFormatMsgCommon class
 * @author Anish kumar
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of CFormatMsgCommon class
 */

#include "CommonClass_stdafx.h"
#include "GUI_FormatMsgCommon.h"
#include "RefTimeKeeper.h"
#include "include/Utils_Macro.h"

CFormatMsgCommon::CFormatMsgCommon(void) :
    CRefTimeKeeper()
{
}

void CFormatMsgCommon::vCalculateAndFormatTM(BYTE bExprnFlag, UINT64 TimeStamp, char acTime[], int size)
{
    DWORD dwTimeStamp = 0; // temporary time stamp

    if (IS_TM_SYS_SET(bExprnFlag))
    {
        dwTimeStamp = (DWORD) ((TimeStamp - m_qwAbsBaseTime) + m_qwRefSysTime);
    }
    else if (IS_TM_REL_SET(bExprnFlag))
    {
        dwTimeStamp = (DWORD) TimeStamp;
    }
    else if (IS_TM_ABS_SET(bExprnFlag))
    {
        dwTimeStamp = (DWORD) (TimeStamp - m_qwAbsBaseTime);
    }
    else
    {
        ASSERT(false);
    }

    vFormatTimeStamp( dwTimeStamp, acTime, size );
}

void CFormatMsgCommon::vFormatTimeStamp(DWORD dwTimeStamp, char acTime[], int bufferSize)
{
    int nTemp, nMicSec, nSec, nMinute, nHour;

    nMicSec = dwTimeStamp % 10000;  // hundreds of microseconds left
    nTemp = dwTimeStamp / 10000;    // expressed in seconds
    nSec = nTemp % 60;          // seconds left
    nTemp = nTemp / 60;         // expressed in minutes
    nMinute = nTemp % 60;       // minutes left
    nHour = nTemp / 60;         // expressed in hours
    sprintf_s(acTime, bufferSize, "%02d:%02d:%02d:%04d", nHour, nMinute, nSec, nMicSec);
}
