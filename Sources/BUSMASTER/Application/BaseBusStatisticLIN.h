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
 * \file      BaseBusStatisticCAN.h
 * \brief     CBaseBusStatisticCAN declaration.
 * \author    Shashank Vernekar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CBaseBusStatisticLIN declaration.
 */

#pragma once

#include"BaseBusStatisticCAN.h"

struct sSUBBUSSTATISTICS_LIN
{
    unsigned int m_unErrorTxCount;
    unsigned int m_unTotalTxMsgCount;
    unsigned int m_unTotalBitsperSec;
    unsigned int m_unTotalRxMsgCount;
    unsigned int m_unErrorTotalCount;
    unsigned int m_unErrorRxCount;
    unsigned int m_unErrorSyncCount;
    unsigned int m_unDLCCount;
    unsigned int m_unTotalWakeUpsCount;
};
typedef sSUBBUSSTATISTICS_LIN SSUBBUSSTATISTICS_LIN;
typedef sSUBBUSSTATISTICS_LIN* PSSUBBUSSTATISTICS_LIN;

/**
* Bus statistics structure
*/
struct sBUSSTATISTICS_LIN
{
    unsigned int    m_unTotalBitsperSec;
    //unsigned int    m_unRxSTDMsgBits;

    double          m_dBusLoad;
    double          m_dPeakBusLoad;
    unsigned int    m_unTotalMsgCount;
    unsigned int    m_unMsgPerSecond;

    unsigned int    m_unTotalTxMsgCount;
    double          m_dTotalTxMsgRate;

    unsigned int    m_unTotalRxMsgCount;
    double          m_dTotalRxMsgRate;

    unsigned int    m_unErrorTxCount;
    double          m_dErrorTxRate;
    unsigned int    m_unErrorRxCount;
    double          m_dErrorRxRate;
    unsigned int    m_unErrorTotalCount;
    double          m_dErrorRate;

    unsigned int    m_unDLCCount;

    double  m_dBaudRate;

    double  m_dTotalBusLoad;
    int     m_nSamples;
    double  m_dAvarageBusLoad;

    unsigned int m_unTotalWakeUpsCount;
    unsigned int m_unTotalWakeUpsRate;

    unsigned char   m_ucTxErrorCounter;
    unsigned char   m_ucRxErrorCounter;
    unsigned char   m_ucTxPeakErrorCount;
    unsigned char   m_ucRxPeakErrorCount;
    unsigned char   m_ucStatus;
    sBUSSTATISTICS_LIN& operator = ( sSUBBUSSTATISTICS_LIN& objRef )
    {
        m_unErrorTxCount = objRef.m_unErrorTxCount;
        m_unTotalTxMsgCount = objRef.m_unTotalTxMsgCount;
        m_unTotalBitsperSec = objRef.m_unTotalBitsperSec;
        m_unTotalRxMsgCount = objRef.m_unTotalRxMsgCount;
        m_unErrorTotalCount = objRef.m_unErrorTotalCount;
        m_unErrorRxCount = objRef.m_unErrorRxCount;
        m_unDLCCount = objRef.m_unDLCCount;
        m_unTotalWakeUpsCount = objRef.m_unTotalWakeUpsCount;
        return *this;
    }
};
typedef sBUSSTATISTICS_LIN SBUSSTATISTICS_LIN;
typedef sBUSSTATISTICS_LIN* PBUSSTATISTICS_LIN;

class CBaseBusStatisticLIN
{

public:

    /* This function should be called for the very first time. */
    virtual HRESULT BSL_DoInitialization(void) = 0;
    /* This function resets the bus statistics */
    virtual HRESULT BSL_ResetBusStatistic(void) = 0;
    /* Get the total number of valid messages transmitted to or
       received from the bus */
    virtual HRESULT BSL_GetTotalMsgCount(UINT unChannelIndex, eDirection eDir, BYTE byIdType,
                                         BYTE byMsgType, UINT& nMsgCount) = 0;
    /* Get the total number of error messages occurred while receiving or
       transmitting */
    virtual HRESULT BSL_GetTotalErrCount(UINT unChannelIndex, eDirection eDir, UINT& nErrCount) = 0;

    /* Get average number of msgs per second(Msg/s)*/
    virtual HRESULT BSL_GetAvgMsgCountPerSec(UINT unChannelIndex, eDirection eDir,
            BYTE byIdType, double& dMsgCount) = 0;
    /* Get average number of errors per second(Err/s)*/
    virtual HRESULT BSL_GetAvgErrCountPerSec(UINT unChannelIndex, eDirection eDir, double& dErrCount) = 0;
    /* Get the bus load */
    virtual HRESULT BSL_GetBusLoad(UINT unChannelIndex, eLOAD eLoad, double& dBusLoad) = 0;
    /* Get controller status */
    /* Note: eLoad can have only CURRENT, PEAK.*/
    virtual HRESULT BSL_GetErrorCounter(UINT unChannelIndex, eDirection eDir, eLOAD eLoad, UCHAR& ucErrCounter) = 0;
    //Setting BaudRate
    virtual HRESULT BSL_SetBaudRate(UINT unChannelIndex, double dBaudRate) = 0;
    //Start updating BusStatistics data
    virtual BOOL BSL_bStartUpdation(BOOL bStart)= 0;
    //Get the controller status
    virtual UCHAR BSL_ucGetControllerStatus(UINT unChannelIndex) = 0;
    //Get the required channel's BusStistic structure.
    virtual HRESULT BSL_GetBusStatistics(UINT unChannelIndex, SBUSSTATISTICS_LIN& sBusStatistics_LIN) = 0;
};
