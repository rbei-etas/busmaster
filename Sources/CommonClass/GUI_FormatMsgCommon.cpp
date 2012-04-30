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
 * \file      GUI_FormatMsgCommon.cpp
 * \brief     Implementation of CFormatMsgCommon class
 * \author    Anish kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of CFormatMsgCommon class
 */

#include "CommonClass_stdafx.h"

#include "GUI_FormatMsgCommon.h"
#include "RefTimeKeeper.h"
#include "include/Utils_Macro.h"

/******************************************************************************
    Function Name    :  CFormatMsgCommon
    Input(s)         :
    Output           :
    Functionality    :  Default Constructor
    Member of        :  CFormatMsgCommon
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
CFormatMsgCommon::CFormatMsgCommon(void)
{
    m_qwAbsBaseTime = 0;
    m_qwRefSysTime  = 0;
}

/******************************************************************************
    Function Name    :  ~CFormatMsgCommon
    Input(s)         :
    Output           :
    Functionality    :  Destructor
    Member of        :  CFormatMsgCommon
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
CFormatMsgCommon::~CFormatMsgCommon(void)
{
}

/******************************************************************************
    Function Name    :  vCalculateAndFormatTM
    Input(s)         :  bExprnFlag - Details of time mode
                        TimeStamp - Msg time stamp, Rel time in case of Rel. mode
                        acTime - Buffer to store formatted time
    Output           :
    Functionality    :  Format time details
    Member of        :  CFormatMsgCommon
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
void CFormatMsgCommon::vCalculateAndFormatTM(BYTE bExprnFlag, UINT64 TimeStamp,
        TCHAR acTime[])
{
    /* In order to make this function work properly ENSURE bExprnFlag has ONLY
    1 time mode bit up */
    DWORD dwTSTmp = 0; // temporary time stamp

    if (IS_TM_SYS_SET(bExprnFlag))
    {
        dwTSTmp = (DWORD) ((TimeStamp - m_qwAbsBaseTime) + m_qwRefSysTime);
    }
    else if (IS_TM_REL_SET(bExprnFlag))
    {
        dwTSTmp = (DWORD) TimeStamp;
    }
    else if (IS_TM_ABS_SET(bExprnFlag))
    {
        dwTSTmp = (DWORD) (TimeStamp - m_qwAbsBaseTime);
    }
    else
    {
        ASSERT(FALSE);
    }

    vFormatTimeStamp(dwTSTmp, acTime);
}

/******************************************************************************
    Function Name    :  vFormatTimeStamp
    Input(s)         :  dwTimeStamp - time stamp to be formatted
                        acTime - Buffer to store formatted time
    Output           :
    Functionality    :  Format time details
    Member of        :  CFormatMsgCommon
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
void CFormatMsgCommon::vFormatTimeStamp(DWORD dwTimeStamp, TCHAR acTime[])
{
    // Static variables to reduce the creation time
    static int nTemp, nMicSec, nSec, nMinute, nHour;
    nMicSec = dwTimeStamp % 10000;  // hundreds of microseconds left
    nTemp = dwTimeStamp / 10000;    // expressed in seconds
    nSec = nTemp % 60;          // seconds left
    nTemp = nTemp / 60;         // expressed in minutes
    nMinute = nTemp % 60;       // minutes left
    nHour = nTemp / 60;         // expressed in hours
    _stprintf(acTime, _T("%02d:%02d:%02d:%04d"), nHour, nMinute, nSec, nMicSec);
}

/******************************************************************************
    Function Name    :  vFormatTimeStamp
    Input(s)         :  dwTimeStamp - time stamp to be formatted
                        acTime - Buffer to store formatted time
    Output           :
    Functionality    :  Format time details
    Member of        :  CFormatMsgCommon
    Friend of        :      -
    Author(s)        :  Anish kumar
    Date Created     :  01.04.2010
******************************************************************************/
//void CFormatMsgCommon::vSetRelBaseTime(INT64 qwRelBaseTime)
//{
//    m_qwRelBaseTime = qwRelBaseTime;
//}

