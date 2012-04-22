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

class CFormatMsgCommon
{
protected:
    CFormatMsgCommon(void);
    //The relative base time will be diff. for each read thread so it is made member
    //Variable. Thus for each read thread an object of class derivedfrom this class
    //should be present
	UINT64 m_qwResTime;//for reset log time
	UINT64   m_qwRelBaseTime;
    void vFormatTimeStamp(DWORD dwTimeStamp, TCHAR acTime[]);
public:
    ~CFormatMsgCommon(void);
    void vCalculateAndFormatTM(BYTE bExprnFlag, UINT64 TimeStamp,TCHAR acTime[]);
    void vCalAndFormatTM_Offline(BYTE bExprnFlag,  UINT64 TimeStamp, TCHAR acTime[]);
    void vSetRelBaseTime(INT64 qwRelBaseTime); //Called to sart afresh for append mode
	BOOL m_bResetMsgAbsTime;
	SYSTEMTIME m_LogSysTime;
	DWORD m_qwLogDelayTime; //for log-msg difference

};
