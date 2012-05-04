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
 * \author    venkatanarayana Makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This File contains the BusStatics Class Declaration.
 */

#pragma once

#include "Utility/Utility_Thread.h"
#include "Include/struct_can.h"
#include "Datatypes/MsgBufAll_Datatypes.h"
#include "Include/Basedefs.h"
#include "DIL_Interface/BaseDIL_CAN.h"
#include "BaseBusStatisticCAN.h"

class CBusStatisticCAN : public CBaseBusStatisticCAN
{
    //Attributes
private:
    //used to Creating threads
    CPARAM_THREADPROC m_ouReadThread;
    //Critical section
    CRITICAL_SECTION m_omCritSecBS;
    //Bus Statictics Structure
    SBUSSTATISTICS m_sBusStatistics[ defNO_OF_CHANNELS ];
    //Sub BusStatistics
    SSUBBUSSTATISTICS m_sSubBusStatistics[ defNO_OF_CHANNELS ];
    //Temporary Referance of STCAN_MSG
    STCAN_MSG m_sCurrEntry;
    //variable to hold current channel

    CBaseDIL_CAN* m_pouDIL_CAN;
    //To keep previous bus data
    SBUSSTATISTICS m_sPrevStatData[ defNO_OF_CHANNELS ];
    int m_nFactorSTDFrame;
    int m_nFactorEXTDFrame;
    int m_nFactorSTDRTRFrame;
    int m_nFactorEXTDRTRFrame;
    int m_nFactorErrorFrame;
    double m_dDiffTime;
    UINT_PTR m_nTimerHandle;
    // Just for an intermediate calculations
    UINT m_unPrevStandardCount[ defNO_OF_CHANNELS ] ;
    UINT m_unPrevExtendedCount[ defNO_OF_CHANNELS ] ;
    UINT m_unPrevStandardRTRCount[ defNO_OF_CHANNELS ] ;
    UINT m_unPrevExtendedRTRCount[ defNO_OF_CHANNELS ] ;
    UINT m_unPrevErrorTotalCount[ defNO_OF_CHANNELS ] ;
    INT  m_unPreviousTime;
    static void* sm_pouBSCan;

    UINT m_unBitsStdMsg[9];
    UINT m_unBitsExdMsg[9];

public:
    CCANBufFSE m_ouCanBufFSE;

public:
    //Constructor
    CBusStatisticCAN(void);
    // This function should be called for the very first time.
    HRESULT BSC_DoInitialization(void);
    // This function resets the bus statistics
    HRESULT BSC_ResetBusStatistic(void);
    // Get the total number of valid messages transmitted to or
    // received from the bus
    HRESULT BSC_GetTotalMsgCount(UINT unChannelIndex, eDirection eDir, BYTE byIdType,
                                 BYTE byMsgType, UINT& nMsgCount);
    // Get the total number of error messages occurred while receiving or
    // transmitting
    HRESULT BSC_GetTotalErrCount(UINT unChannelIndex, eDirection eDir, UINT& nErrCount);

    // Get average number of msgs per second(Msg/s)
    HRESULT BSC_GetAvgMsgCountPerSec(UINT unChannelIndex, eDirection eDir,
                                     BYTE byIdType, double& dMsgCount);
    // Get average number of errors per second(Err/s)
    HRESULT BSC_GetAvgErrCountPerSec(UINT unChannelIndex, eDirection eDir, double& dErrCount);
    // Get the bus load
    HRESULT BSC_GetBusLoad(UINT unChannelIndex, eLOAD eLoad, double& dBusLoad);
    // Get controller status
    // Note: eLoad can have only CURRENT, PEAK.
    HRESULT BSC_GetErrorCounter(UINT unChannelIndex, eDirection eDir, eLOAD eLoad, UCHAR& ucErrCounter);
    //To Set the Baud Rate
    HRESULT BSC_SetBaudRate(UINT unChannelIndex, double dBaudRate);
    //Updates the Bus Statistics on every received or transmitted Message
    BOOL BSC_bStartUpdation(BOOL bStart);
    //Get the controller status
    UCHAR BSC_ucGetControllerStatus(UINT unChannelIndex);
    //Get the required channel's BusStistic structure.
    HRESULT BSC_GetBusStatistics(UINT unChannelIndex, SBUSSTATISTICS& sBusStatistics);
    //Updates the Bus Statics Structure on every Tx/Rx Message.
    void vUpdateBusStatistics(STCANDATA& sCanData);
    //Calculate the Bus statistics on timer
    void vCalculateBusParametres(void);
    //Temporary function aclled by the SetTimer function.
    static  void CALLBACK OnTimeWrapper(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
    //Destructor
    virtual ~CBusStatisticCAN(void);
private:
    //Initialise or resets the bus statistics.
    void vInitialiseBSData(void);
    //Calculates the Time differance.
    void vCalculateDiffTime(void);
    //Starts the BS Read Thread
    BOOL bStartBSReadThread(void);
};
