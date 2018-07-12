/*******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  Channel.cpp
  Description   :  Implementation file for CChannel class
  $Log:   X:/Archive/Sources/DIL/DIL_CAN/CAN_PEAK_USB_Channel.cpv  $

      Rev 1.1   15 Apr 2011 18:47:56   CANMNTTM
   Added RBEI Copyright information.

      Rev 1.0   16 Aug 2010 21:56:32   rac2kor


  Author(s)     :  Raja N
  Date Created  :  21.2.2005
  Modified By   :  Pradeep Kadoor
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
*******************************************************************************/

#include <windows.h>
#include <tchar.h>
#include "EXTERNAL/PLinApi.h"        // For PEAK driver related definitions
#include "LIN_PEAK_USB_Channel.h" // For CChannel class declaration
//#include "Include/CanUsbDefs.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
/*******************************************************************************
  Function Name  : CChannel
  Description    : Standard default constructor
  Member of      : CChannel
  Functionality  : This will initialise local variables
  Author(s)      : Raja N
  Date Created   : 21.2.2005
  Modifications  :
*******************************************************************************/
CChannel::CChannel()
{
    // Init members with default value
    //clock
    //m_usClock = defClockVal;
    //Sampling
    //m_usSampling = defSamplingVal;
    m_unBaudrate = 19200;
    // Baud Rate
    m_usBaudRate = 19200;
    // Controller state
    m_ucControllerState = hwsNotInitialized;
    // Hardware mode
    m_unLINMode = modSlave;
    // Hardware Handle Associated with this channel
    m_hHardwareHandle = 0;
}

/*******************************************************************************
  Function Name  : ~CChannel
  Description    : Standard Destructor
  Member of      : CChannel
  Functionality  : -
  Author(s)      : Raja N
  Date Created   : 21.2.2005
  Modifications  :
*******************************************************************************/
CChannel::~CChannel()
{

}

/*******************************************************************************
  Function Name  : vUpdateErrorCounter
  Input(s)       : ucTxError - Tx Error Value
                   ucRxError - Rx Error Value
  Output         : -
  Functionality  : Updates the error counter value and controller state as per
                   the error counter values
  Member of      : CChannel
  Author(s)      : Raja N
  Date Created   : 8.3.2005
  Modifications  :
*******************************************************************************/
void CChannel::vUpdateErrorCounter(UCHAR /*ucTxError*/, UCHAR /*ucRxError*/)
{
  
}

/*******************************************************************************
  Function Name  : vResetChannel
  Input(s)       : -
  Output         : -
  Functionality  : This function will reset all state related information. This
                   will be called during the hardware reset of the machine
  Member of      : CChannel
  Author(s)      : Raja N
  Date Created   : 8.3.2005
  Modifications  :
*******************************************************************************/
void CChannel::vResetChannel()
{
    // Controller state
    m_ucControllerState = defMODE_ACTIVE;
    // Tx Error counter value
    //m_ucTxErrorCounter = 0;
    //// Rx Error counter value
    //m_ucRxErrorCounter = 0;
    //// Peak values of Rx and Tx Error Counter
    //m_ucPeakRxErrorCounter = m_ucPeakTxErrorCounter = 0;
    //// Tx Error Handler execution state
    //m_bTxErrorExecuted = FALSE;
    //// Rx Error Handler execution state
    //m_bRxErrorExecuted = FALSE;
}