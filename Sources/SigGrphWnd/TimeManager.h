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
 * \file      TimeManager.h
 * \brief     Interface file for CTimeManager class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CTimeManager class
 */

#pragma once

class CTimeManager
{
    // Absolute Time Reference
    static int m_nAbsoluteTime;
    // System Reference Tick Count
    static const LARGE_INTEGER m_sSysRefTickCount;
    // System Reference Time Value
    static const int m_nSysRefTime;
    // Clock tick Frequency
    static const __int64 m_n64Frequency;
    
    // **** USB Related Time Variable **** //
    // To hold time difference between System time and windows startup time
    static int m_nOffsetTimeValue;
    static int nCalculateOffsetTime();
    // **** End of USB Code **** //
    
    // Private Constructor and Destructor to avoide instances of this class
    CTimeManager();
    virtual ~CTimeManager();

public:
    // To initialise the absolute time on connect
    static void vInitAbsoluteTime();
    // To Get the absolute reference time value
    static int nGetAbsoluteTime();
    // To set the absolute time value manually
    static void vSetAbsoluteTime(int nAbsoluteTime);
    //To get latest offset set by the DIL
    static int nCalculateCurrTimeStamp(BOOL bFromDIL = FALSE);    
    // To calculate and Format the time
    static void vFormatTimeStamp(int nTimeStamp, BOOL bOverWrite,
                                    WORD wDisplayTimeMode,
                                    int nIndex,
                                    __int64 n64OverWriteMapKey,
                                    CString &omStrTime);
    // To perform calculation and formatting only
    static void vFormatTimeStamp( int nTimeStamp,
                                  int nRefTimeStamp,
                                  WORD wDisplayTimeMode,
                                  CString &omStrTime);
    // To find system time value of clock tick
    static int nCalculateElapsedTimeFromTick(const __int64& n64CurrTick);
    // To reinitialise the offset time value
    static void bReinitOffsetTimeValForES581(void);
    
};
