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
 * \file      TimeManager.cpp
 * \brief     Implementation file for CTimeManager class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CTimeManager class
 */

// Standard include header
#include "SigGrphWnd_stdafx.h"
// For Time calculation related functions
#include "TimeManager.h"
#include "SigGrphWnd_Defines.h"

// For DIL Interface Class
#include "Include/BaseDefs.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_Datatypes.h"
#include "DataTypes/DIL_Datatypes.h"
#include "DIL_Interface/BaseDIL_CAN.h"

// Initialise the static members

// Initialise absolute Time
int CTimeManager::m_nAbsoluteTime = 0;
// Init System time and reference tick
static LARGE_INTEGER s_Temp;
BOOL bDummy = QueryPerformanceCounter(&s_Temp);
const LARGE_INTEGER CTimeManager::m_sSysRefTickCount = s_Temp;
// Init the frequency
const int CTimeManager::m_nSysRefTime = CTimeManager::nCalculateCurrTimeStamp(FALSE);
BOOL bDummy0001 = QueryPerformanceFrequency((LARGE_INTEGER*) &s_Temp);
const __int64 CTimeManager::m_n64Frequency = s_Temp.QuadPart;

int CTimeManager::m_nOffsetTimeValue =
    CTimeManager::nCalculateOffsetTime();

/**
 * \brief Constructor
 *
 * Standard default constructor for dialog box
 */
CTimeManager::CTimeManager()
{
}

/**
 * \brief Destructor
 *
 * Standard Destructor
 */
CTimeManager::~CTimeManager()
{
}

/**
 * \brief      Format Time Stamp
 * \param[in]  nTimeStamp Current time stamp
 * \param[in]  bOverWrite Mode of Time Calculation
 * \param[in]  wDisplayTimeMode Type of Time Calculation
 * \param[in]  nIndex Index of the element
 * \param[in]  n64OverWriteMapKey Unique key to CMap to get previous time stamp
 * \param[out] omStrTime Object Reference to store the formatted time value
 *
 * This function will compute the time and will format it and
 * will store it in the omStrTime string.
 */
void CTimeManager::vFormatTimeStamp(int nTimeStamp, BOOL bOverWrite,
                                    WORD wDisplayTimeMode,
                                    int nIndex,
                                    __int64 /*n64OverWriteMapKey*/,
                                    CString& omStrTime)
{
    // Static variables to reduce the creation time
    static int nTemp, nMicSec, nSec, nMinute, nHour;
    int nRefTime = 0;

    // Find reference time stamp for relative mode
    if( wDisplayTimeMode == eRELATIVE_MODE)
    {
        if(bOverWrite == FALSE )
        {
            // To get the previous time stamp
            // If index is not zero then get it from the buffer
            if( nIndex != 0 )
            {
                // ???
            }
            // If index is zero then refer the backup copy of time stamp
            else
            {
                // ???
            }
        }
        else
        {
            // ???
        }
    }

    // Format the time stamp
    vFormatTimeStamp(nTimeStamp, nRefTime, wDisplayTimeMode, omStrTime);
}

/**
 * \brief      Format Time Stamp
 * \param[in]  nTimeStamp Current time stamp
 * \param[in]  nRefTimeStamp Reference time stamp for relative mode
 * \param[in]  wDisplayTimeMode Type of Time Calculation
 * \param[out] omStrTime Object Reference to store the formatted time value
 *
 * This function will compute the time and will format it and
 * will store it in the omStrTime string.
 */
void CTimeManager::vFormatTimeStamp( int nTimeStamp,
                                     int nRefTimeStamp,
                                     WORD wDisplayTimeMode,
                                     CString& omStrTime)
{
    // Static variables to reduce the creation time
    static int nTemp, nMicSec, nSec, nMinute, nHour;

    if(wDisplayTimeMode == eABSOLUTE_MODE )
    {
        // Subract the value of absolute time reference.
        nTimeStamp -= m_nAbsoluteTime;
    }
    else if( wDisplayTimeMode == eRELATIVE_MODE )
    {
        nTimeStamp -= nRefTimeStamp;
    }

    // Make sure that there wont be any negative time value
    if( nTimeStamp < 0)
    {
        nTimeStamp = 0;
    }

    nMicSec = nTimeStamp % 10000;// hundreds of microseconds left
    nTemp = nTimeStamp / 10000;  // expressed in seconds
    nSec = nTemp % 60;           // seconds left
    nTemp = nTemp / 60;          // expressed in minutes
    nMinute = nTemp % 60;        // minutes left
    nHour = nTemp / 60;          // expressed in hours

    // Check for system mode and wrap up with in 24 Hours
    if( wDisplayTimeMode == eSYSTEM_MODE )
    {
        nHour %= 24;
    }

    // Format the time value in the output string
    omStrTime.Format(defTIME_STAMP_FORMAT, nHour, nMinute,nSec, nMicSec);
}

/**
 * \brief     Set Absolute Time
 * \param[in] nAbsoluteTime Absolute Time Reference
 *
 * This function will init the absolute reference variable with
 * given time value
 */
void CTimeManager::vSetAbsoluteTime(int nAbsoluteTime)
{
    m_nAbsoluteTime = nAbsoluteTime;
}

/**
 * \brief  Get Absolute Time
 * \return Absolute Time Reference
 *
 * This function will return the absolute reference value
 */
int CTimeManager::nGetAbsoluteTime()
{
    return m_nAbsoluteTime;
}

/**
 * \brief  Calculate Current Time Stamp
 * \return Current system time in 10's of Milli Seconds
 *
 * This function will return the current system time in 10's of
 * milliseconds. This will be called to init reference variables
 */
int CTimeManager::nCalculateCurrTimeStamp(BOOL bFromDIL)
{
    SYSTEMTIME CurrSysTime;
    //UINT64 TimeStamp;
    int nResult = 0;

    if (bFromDIL == FALSE)
    {
        GetLocalTime(&CurrSysTime);
        nResult = (CurrSysTime.wHour * 3600 + CurrSysTime.wMinute * 60
                   + CurrSysTime.wSecond) * 10000 + CurrSysTime.wMilliseconds * 10;
    }
    else
    {
        //g_pouDIL_CAN_Interface->DILC_GetTimeModeMapping(CurrSysTime, TimeStamp);
    }

    return nResult;
}

/**
 * \brief Init Absolute Time
 *
 * This function will initialize the absolute reference value
 */
void CTimeManager::vInitAbsoluteTime()
{
    m_nAbsoluteTime = nCalculateCurrTimeStamp();
}

/**
 * \brief     Calculate Elapsed Time From Tick
 * \param[in] n64CurrTick Time Tick Value
 * \return    System Time in 10s of millisecond
 *
 * This function will calculate the saystem time value of the
 * clock tick. This will refer the reference time time and clock
 * tick frequency to compute the system time
 */
int CTimeManager::nCalculateElapsedTimeFromTick(const __int64& /*n64CurrTick*/)
{
    return 0;
}

/**
 * \brief Reinit Offset Time Value for ES581
 *
 * Reinit Offset Time Value for ES581
 */
void CTimeManager::bReinitOffsetTimeValForES581(void)
{
    CTimeManager::m_nOffsetTimeValue = nCalculateCurrTimeStamp(TRUE);
}

/**
 * \brief  Calculate Offset Time
 * \return Offset Time in 10s of millisecond
 *
 * This function will calculate the time offset between windows
 * start-up time and system time. This will be added to time
 * stamping in the CAN message from driver.
 */
int CTimeManager::nCalculateOffsetTime()
{
    return 0;
}
