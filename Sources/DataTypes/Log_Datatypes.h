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
 * \file      Log_Datatypes.h
 * \brief     Definition file for log data types.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for log data types.
 */

#pragma once

#include "include/BaseDefs.h"

// Triggering means the state to which logging operation should transit.
typedef enum eLogTriggerState
{
    NONE    = 0,
    START   = 1,    // To start logging
    STOP    = 2,    // To stop logging
    BOTH    = 3,    // Shuttle.
    STOPPED = 4     // Logging already stopped
} ELOGTRIGGERSTATE;

// Triggering operation with all the necessary parameters.
typedef struct tagLogTrigger
{
    ELOGTRIGGERSTATE m_unTriggerType; // 0 - None, 1 - Start, 2 - Stop, 3 - Both
    UINT             m_unStartID;     // Start Trigger Id
    UINT             m_unStopID;      // Stop Trigger Id
} SLOGTRIGGER,*PSLOGTRIGGER;
//
// Advance log file settings
typedef struct tagLogAdvancedSettings
{
    // Holds Log on measurement state checked or Unchecked
    BOOL m_bIsLogOnMesurement;

    // Holds Log on size state checked or Unchecked
    BOOL m_bIsLogOnSize;

    // Holds Log on Time state checked or Unchecked
    BOOL m_bIsLogOnTime;

    // Holds Size value specified by user
    CString m_omSizeInMB;

    // Holds Hrs Time specified by user
    CString m_omLogTimeHrs;

    // Holds Mins Time specified by user
    CString m_omLogTimeMins;

    // Holds Maximum Number of log files to be incremented
    CString m_omMaxNoOfLogFiles;

    double   m_qwLogSysTime;

    //Holds the number of Connections made by BM
    int m_nConnectionCount;//Shailesh - connection count per log file (log object)

    // Holds Comments specified by the user
    CString m_omLogComment; //arun


} SLOGADVSETTNGS;
const USHORT ID_INVALID = (USHORT) -1;
const USHORT ID_MAX = (USHORT) ~0;

// The below structure describes a logging block fulfilling the condition of
// necessity and sufficience.
typedef struct tagLogInfo
{
    USHORT       m_ushID;            // Logging block identifier
    bool         m_bIsUpdated;       // To indicate if it needs updation
    bool         m_bEnabled;         // To indicate if its enabled
    eTimerMode   m_eLogTimerMode;    // The time mode - system / relative
    eFormat      m_eNumFormat;       // Numeric mode - hexadecimal / decimal
    eMode        m_eFileMode;        // Mode - overwrite / append
    bool         m_bResetAbsTimeStamp; // To indicate if Absolute Time Stamp is Reseted
    TYPE_CHANNEL m_ChannelSelected;  // The current channel
    char         m_sLogFileName[_MAX_PATH]; // Log file name with absolute path
    SLOGTRIGGER  m_sLogTrigger;      // The triggering condition

    SLOGADVSETTNGS m_sLogAdvStngs;

    tagLogInfo();                    // Standard constructor
    void vClear(void);               // To clear the logging information
    UINT unGetSize(void) const;      // To get the size of the block in bytes
    BYTE* pbGetConfigData(BYTE* pbTarget) const; // To get configuration data
    bool pbGetConfigData(xmlNodePtr pxmlNodePtr) const; // To get configuration data

    BYTE* pbSetConfigData(BYTE* pbTarget, BYTE bytLogVersion); // To set configuration data
    INT nSetConfigData(xmlNodePtr pNode);
} SLOGINFO,*PSLOGINFO;
