/*******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  Network.cpp
  Description   :  Implementation file for CNetwork class
  $Log:   X:/Archive/Sources/DIL/DIL_CAN/CAN_PEAK_USB_Network.cpv  $

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
#include "LIN_PEAK_USB_Network.h" // For CNetwork class declaration

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*******************************************************************************
  Function Name  : CNetwork
  Description    : Standard default constructor
  Member of      : CNetwork
  Functionality  : This will initialise local variables
  Author(s)      : Raja N
  Date Created   : 21.2.2005
  Modifications  :
*******************************************************************************/
CNetwork::CNetwork()
{
    // Initialise members
    // Number of chennals present or initialised
    m_nNoOfChannels = 0;
}

/*******************************************************************************
  Function Name  : ~CNetwork
  Description    : Standard Destructor
  Member of      : CNetwork
  Functionality  : -
  Author(s)      : Raja N
  Date Created   : 21.2.2005
  Modifications  :
*******************************************************************************/
CNetwork::~CNetwork()
{

}


void CNetwork::vResetChannelData()
{
    for ( int i = 0 ; i < defNO_OF_CHANNELS; i++ )
    {
        m_aodChannels[i].m_IdSlaveData.clear();
    }

}
