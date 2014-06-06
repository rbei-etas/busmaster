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
 * \file      FormatMsgCommon.h
 * \brief     Definition of CFormatMsgCommon class.
 * \author    Anish Kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of CFormatMsgCommon class.
 */

#pragma once

#include "CommonClass/RefTimeKeeper.h"

/**
 * Common class for Message Formatting.
 *
 * The relative base time will be diff. for each read thread so it is made member
 * Variable. Thus for each read thread an object of class derived from this class
 * should be present
 */
class CFormatMsgCommon
{
public:
    CFormatMsgCommon(CRefTimeKeeper & ouRefTimeKeeper);
    virtual ~CFormatMsgCommon();

    /**
     * In order to make this function work properly ENSURE bExprnFlag has ONLY
     * ONE time mode bit up
     */
    void vCalculateAndFormatTM(BYTE bExprnFlag, UINT64 TimeStamp, char acTime[]);

    void vCalAndFormatTM_Offline(BYTE bExprnFlag,  UINT64 TimeStamp, char acTime[]);

    /**
     * Called to sart afresh for append mode
     */
    void vSetRelBaseTime(INT64 qwRelBaseTime);

    BOOL m_bResetMsgAbsTime;

    SYSTEMTIME m_LogSysTime;

    /** for log-msg difference */
    DWORD m_qwLogDelayTime;

protected:
    /** for reset log time */
    UINT64 m_qwResTime;
    
    UINT64 m_qwRelBaseTime;

    CRefTimeKeeper & m_ouRefTimeKeeper;
    
    void vFormatTimeStamp(DWORD dwTimeStamp, char acTime[]);
};
