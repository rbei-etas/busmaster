/*********************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  TimeManager.h
  Description   :  Interface file for CTimeManager class 
  $Log:   X:/Archive/Sources/SigGrphWnd/TimeManager.h_v  $
 * 
 *    Rev 1.1   15 Apr 2011 19:43:54   rac2kor
 * Inserted RBEI Copyright information text into the file header.
 * 
 *    Rev 1.0   13 Dec 2010 22:00:52   CANMNTTM
 *  
 * 
 *    Rev 1.1   03 Sep 2010 20:34:50   rac2kor
 * Removed dead code.
 * 
 *    Rev 1.0   16 Aug 2010 21:20:46   rac2kor
 *  

  Author(s)     :  Raja N
  Date Created  :  23.06.2004
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
 *********************************************************************/

#if !defined(AFX_TIMEMANAGER_H__03C4B0FC_5A6E_4330_821C_F4109863D8C0__INCLUDED_)
#define AFX_TIMEMANAGER_H__03C4B0FC_5A6E_4330_821C_F4109863D8C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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
#endif // !defined(AFX_TIMEMANAGER_H__03C4B0FC_5A6E_4330_821C_F4109863D8C0__INCLUDED_)