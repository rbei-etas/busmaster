/*******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  Channel.h
  Description   :  Interface file for CChannel class
  $Log:   X:/Archive/Sources/DIL/DIL_CAN/CAN_PEAK_USB_Channel.h_v  $
 *
 *    Rev 1.1   15 Apr 2011 18:47:56   CANMNTTM
 * Added RBEI Copyright information.
 *
 *    Rev 1.0   16 Aug 2010 21:56:32   rac2kor
 *

  Author(s)     :  Raja N
  Date Created  :  21.2.2005
  Modified By   :  Pradeep Kadoor
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
*******************************************************************************/


#if !defined(AFX_CHANNEL_H__CDA65E9D_77E5_4058_8F76_85D46585C9D0__INCLUDED_)
#define AFX_CHANNEL_H__CDA65E9D_77E5_4058_8F76_85D46585C9D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "LINDriverDefines.h"
#include <map>
using namespace std;

struct stIdData
{
    unsigned char ucData[8];
    int m_nLength;
};


class CChannel
{
public:
    // To Initialise controller state and error counter values
    void vResetChannel();
    // To update error counter and controller state
    // Returns the state transtition state
    void vUpdateErrorCounter(UCHAR ucTxError, UCHAR ucRxError);
    // Methods
    // Standard Constructor
    CChannel();
    // Don't declate it as virtual to avoid VTable as this class will not be
    // inherited
    ~CChannel();

    // Members
    USHORT m_usClock;
    UINT   m_unBaudrate;

    /* Bit Timing */
    USHORT  m_usBaudRate;

    /* LIN Mode */
    // 0 - Not Initialized
    // 1 - Slave mode
    // 2 - Master mode
    UINT  m_unLINMode;

    // To store hardware state
    // 0 - Not Initialized
    // 1 - Auto baud rate detection
    // 2 - Bus is active
    // 3 - Bus is in sleep mode
    // 6 - Bus-line shorted to ground
    UCHAR  m_ucControllerState;

    // Acceptance Filter information
    //SACC_FILTER_INFO     m_sFilter[2];

    // Hardware Handle Associated with this channel
    HLINHW m_hHardwareHandle;

    // Checksum type
    TLINChecksumType m_linChecksumType;

    // Direction
    TLINDirection m_linDirection;

    // LIN Version
    UINT  m_unLINVersion;

    map<int, stIdData> m_IdSlaveData;

protected:
    // No protected members right now
private:
    // No private members right now
};

#endif // !defined(AFX_CHANNEL_H__CDA65E9D_77E5_4058_8F76_85D46585C9D0__INCLUDED_)
