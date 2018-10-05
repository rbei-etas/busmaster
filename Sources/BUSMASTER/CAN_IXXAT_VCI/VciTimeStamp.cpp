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
* \file      VciTimeStamp.cpp
* \brief     The implementation file for the class to handle VCI V3 and VCI V4 timestamps.
* \author    Peter Wucherer
* \copyright Copyright (c) 2018, HMS Technology Center Ravensburg GmbH. All rights reserved.
*
* This file contain all function implementations of the class CVciTimeStamp.
*/

/***************************************************************************************************
**    include-files
***************************************************************************************************/
#ifdef _DEBUG
#include "CAN_IXXAT_VCI_stdafx.h"
#else
#include <windows.h>
#endif
#include <stdio.h>
#include <tchar.h>
#include "VciTimeStamp.h"
#ifdef _DEBUG
#include "DBGOUT.H"
#endif

/**
 * Constructor of for the class object. Reset same member variables
 * and initialize the performance counter.
*/
CVciTimeStamp::CVciTimeStamp(void)
{
    m_qwTimeStampResolNSec = 0;
    m_qwTimerOverruns      = 0;
    m_qwTimerZeroDiff      = 0;

    m_qiStartCount         = 0;
    m_qiTempCount          = 0;

    QueryPerformanceFrequency( (LARGE_INTEGER*) &m_qiFrequency );
}


/**
 * Destructor of the class object.
 * Nothing todo.
*/
CVciTimeStamp::~CVciTimeStamp(void)
{
}

/**
 * \brief        Initialize timestamp calculation.
 * \param[in]    dwTscDivisor Divisor for the message time stamp counter.
 * \param[in]    dwClockFreq Clock frequency of the primary counter in Hz
 * \return       The timestamp in nanoseconds per timer tick.
 *
 * Initialize the member variables for timestamp calculation.
 * Must call with the parameters in the structure CANCAPABILITIES filled by the function
 * canControlGetCaps.
*/
UINT64 CVciTimeStamp::SetTimerResolution(DWORD dwTscDivisor, DWORD dwClockFreq)
{
    // reset the timer overruns
    m_qwTimerOverruns = 0;

    // calculate it in 0.1 nanoseconds
    m_qwTimeStampResolNSec = ((UINT64) 10000000000 * dwTscDivisor) / dwClockFreq;

    // round it up
    m_qwTimeStampResolNSec += 5;

    // convert it back to 1 nanosecond
    m_qwTimeStampResolNSec /=10;

    return m_qwTimeStampResolNSec;
}

/**
* \brief         To be called when the start CAN info message from the CAN controller was received.
* \param[in]     dwMsgTimeStamp The timer tick of the info message.
*
* Call this function when the VCI info message arrives with CAN_INFO_START in the data byte 0.
* When the timestamp is zero then the difference time value will be set to the moment when
* CVciTimeStamp::StartTimer was called.
* When the timestamp is not zero this timestamp is set to the difference time value.
*/
void CVciTimeStamp::SetCANStartMessage(DWORD dwMsgTimeStamp)
{
    m_qwTimerZeroDiff = 0;

    // reset the timer overruns
    m_qwTimerOverruns = 0;

    if (dwMsgTimeStamp == 0)
    {
        m_qwTimerZeroDiff = 0;
    }
    else
    {
        m_qwTimerZeroDiff = GetTimeInNanoSeconds(dwMsgTimeStamp);;
    }
}

/**
 * \brief         To call when the VCI CAN message CAN_MSGTYPE_TIMEOVR arrives.
 * \param[in]     dwNumberOfOverruns The number of occured overruns since the last overrun message.
 *
 * To call when the VCI CAN message CAN_MSGTYPE_TIMEOVR arrives. The field Identifier
 * contains the number of overrun messages since the last overrun.
*/
void CVciTimeStamp::AddTimerOverruns(DWORD dwNumberOfOverruns)
{
    m_qwTimerOverruns += dwNumberOfOverruns;
}

/**
 * \brief         Return the nanoseconds since CAN controller start.
 * \param[in]     dwMsgTimeStamp The VCI timer ticks of to convert in nanoseconds.
 * \return        The number of nanoseconds till the CAN controller was started.
 *
 * Return the given timer ticks value as Nanoseconds since CAN controller start.
 * Call this function for every received CAN message to get the time instead of ticks.
 * Remark: Use %I64d if you use this result in a printf function
*/
UINT64 CVciTimeStamp::GetTimeInNanoSeconds(DWORD dwMsgTimeStamp)
{
    // calculate the time stamp in ticks
    UINT64 qwTimeInTicks = (UINT64)(dwMsgTimeStamp) + (UINT64)(m_qwTimerOverruns << 32);

    // calculate the time stamp in nano seconds.
    UINT64 qwTimeStamp = m_qwTimeStampResolNSec * qwTimeInTicks;

    // substract now to the start point of the CAN controller
    qwTimeStamp -= m_qwTimerZeroDiff;

    return qwTimeStamp;
}
