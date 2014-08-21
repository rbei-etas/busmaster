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
 * \file      BusStatisticCAN.h
 * \brief     This File contains the BusStatics Class Declaration.
 * \author    Shashank Vernekar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This File contains the BusStatics Class Declaration.
 */
#pragma once

#include "Utility/Utility_Thread.h"
#include "Include/Struct_Ethernet.h"
#include "Datatypes/MsgBufAll_Datatypes.h"
#include "Include/Basedefs.h"
#include "DIL_Interface/BaseDIL_Ethernet.h"
#include "BaseBusStatisticEthernet.h"

class CBusStatisticEthernet : public CBaseBusStatisticEthernet
{
    //Attributes
private:
    //used to Creating threads
    CPARAM_THREADPROC m_ouReadThread;
    //Critical section
    CRITICAL_SECTION m_omCritSecBS;
    //Bus Statictics Structure
    SBUSSTATISTICS_Ethernet m_sBusStatistics_Ethernet[ defNO_OF_Ethernet_CHANNELS ];
    //Sub BusStatistics
    SSUBBUSSTATISTICS_Ethernet m_sSubBusStatistics_Ethernet[ defNO_OF_Ethernet_CHANNELS ];
    //Temporary Referance of STCAN_MSG
	STETHERNET_MSG m_sCurrEntry;
    //variable to hold current channel

    CBaseDIL_ETHERNET* m_pouDIL_Ethernet;
    //To keep previous bus data
    SBUSSTATISTICS_Ethernet m_sPrevStatData_Ethernet[ defNO_OF_Ethernet_CHANNELS ];
    int m_nFactorSTDFrame;
    int m_nFactorErrorFrame;
    double m_dDiffTime;
    UINT_PTR m_nTimerHandle;
    // Just for an intermediate calculations
    UINT m_unPrevStandardCount[ defNO_OF_Ethernet_CHANNELS ] ;
    UINT m_unPrevErrorTotalCount[ defNO_OF_Ethernet_CHANNELS ] ;
    INT  m_unPreviousTime;
    static void* sm_pouBSLin;

    UINT m_unBitsStdMsg[9];



public:
	CEthernetBufFSE m_ouEthernetBufFSE;

public:
    //Constructor
    CBusStatisticEthernet(void);
    // This function should be called for the very first time.
    HRESULT BSL_DoInitialization(void);
    // This function resets the bus statistics
    HRESULT BSL_ResetBusStatistic(void);
    // Get the total number of valid messages transmitted to or
    // received from the bus
    HRESULT BSL_GetTotalMsgCount(UINT unChannelIndex, eDirection eDir, BYTE byIdType,
                                 BYTE byMsgType, UINT& nMsgCount);
    // Get the total number of error messages occurred while receiving or
    // transmitting
    HRESULT BSL_GetTotalErrCount(UINT unChannelIndex, eDirection eDir, UINT& nErrCount);

    // Get average number of msgs per second(Msg/s)
    HRESULT BSL_GetAvgMsgCountPerSec(UINT unChannelIndex, eDirection eDir,
                                     BYTE byIdType, double& dMsgCount);
    // Get average number of errors per second(Err/s)
    HRESULT BSL_GetAvgErrCountPerSec(UINT unChannelIndex, eDirection eDir, double& dErrCount);
    // Get the bus load
    HRESULT BSL_GetBusLoad(UINT unChannelIndex, eLOAD eLoad, double& dBusLoad);
    // Get controller status
    // Note: eLoad can have only CURRENT, PEAK.
    HRESULT BSL_GetErrorCounter(UINT unChannelIndex, eDirection eDir, eLOAD eLoad, UCHAR& ucErrCounter);
    //To Set the Baud Rate
    HRESULT BSL_SetBaudRate(UINT unChannelIndex, double dBaudRate);
    //Updates the Bus Statistics on every received or transmitted Message
    BOOL BSL_bStartUpdation(BOOL bStart);
    //Get the controller status
    UCHAR BSL_ucGetControllerStatus(UINT unChannelIndex);
    //Get the required channel's BusStistic structure.
    HRESULT BSL_GetBusStatistics(UINT unChannelIndex, SBUSSTATISTICS_Ethernet& sBusStatistics_Ethernet);
    //Updates the Bus Statics Structure on every Tx/Rx Message.
	void vUpdateBusStatistics(STETHERNETDATA& sLinData);
    //Calculate the Bus statistics on timer
    void vCalculateBusParametres(void);
    //Temporary function aclled by the SetTimer function.
    static  void CALLBACK OnTimeWrapper(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
    //Destructor
    virtual ~CBusStatisticEthernet(void);
private:
    //Initialise or resets the bus statistics.
    void vInitialiseBSData(void);
    //Calculates the Time differance.
    void vCalculateDiffTime(void);
    //Starts the BS Read Thread
    BOOL bStartBSReadThread_Ethernet(void);
};
