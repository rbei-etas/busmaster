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
* \file      VciTimeStamp.h
* \brief     The header file for the class to handle VCI V3 and VCI V4 timestamps.
* \author    Peter Wucherer
* \copyright Copyright (c) 2018, HMS Technology Center Ravensburg GmbH. All rights reserved.
*
* This file contains the class declaration for CVciTimeStamp.
*/

#pragma once

/***************************************************************************************************
**    constants and macros
***************************************************************************************************/


/***************************************************************************************************
**    data types
***************************************************************************************************/

/***************************************************************************************************
      class declaration of:
***************************************************************************************************/
class CVciTimeStamp
{
    /*********************  variables  ******************/
protected:
    UINT64 m_qwTimeStampResolNSec; // time stamp precis resolution
    UINT64 m_qwTimerOverruns;      // Number of timer overruns
    UINT64 m_qwTimerZeroDiff;

    // the timer functions and variables
    LONGLONG m_qiFrequency;
    LONGLONG m_qiStartCount;
    LONGLONG m_qiTempCount;


    /******************  elementfunctions  **************/
protected:

public:
    CVciTimeStamp(void);
    ~CVciTimeStamp(void);
    UINT64 SetTimerResolution(DWORD dwTscDivisor, DWORD dwClockFreq);
    void SetCANStartMessage(DWORD dwMsgTimeStamp);
    void AddTimerOverruns(DWORD dwNumberOfOverruns);

    UINT64 GetTimeInNanoSeconds(DWORD dwMsgTimeStamp);
};

