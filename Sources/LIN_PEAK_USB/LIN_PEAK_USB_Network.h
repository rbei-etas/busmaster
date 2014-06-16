/*******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  Network.h
  Description   :  Interface file for CNetwork class
  $Log:   X:/Archive/Sources/DIL/DIL_CAN/CAN_PEAK_USB_Network.h_v  $
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


#if !defined(AFX_NETWORK_H__D5F3ADD7_E79E_4251_B53E_0C8C5B94F3DA__INCLUDED_)
#define AFX_NETWORK_H__D5F3ADD7_E79E_4251_B53E_0C8C5B94F3DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Include/CanUsbDefs.h"

class CNetwork
{
public:
    // Methods
    CNetwork();
    virtual ~CNetwork();
    // Members
    // Number of Channels or hardware actually present
    UINT m_nNoOfChannels;
    // Array of channel objects. The size of this array is maximun number
    // of channels the application will support.
    CChannel m_aodChannels[ defNO_OF_CHANNELS ];

protected:
    // No protected members right now

private:
    // No private members right now
public:
    void vResetChannelData();
};

#endif // !defined(AFX_NETWORK_H__D5F3ADD7_E79E_4251_B53E_0C8C5B94F3DA__INCLUDED_)
