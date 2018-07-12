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
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CBaseBusStatisticCAN declaration.
 */

#pragma once

enum eLOAD
{
    CURRENT = 0,
    AVERAGE,
    PEAK
};

struct sSUBBUSSTATISTICS
{
    /** error transmit count */
    unsigned int m_unErrorTxCount;

    /** total transmit message count */
    unsigned int m_unTotalTxMsgCount;

    /** transmit standard message count */
    unsigned int m_unTxSTDMsgCount;

    /** total bits per second */
    unsigned int m_unTotalBitsperSec;

    /** transmit extended message count */
    unsigned int m_unTxEXTDMsgCount;

    /** transmit standard remote message count */
    unsigned int m_unTxSTD_RTRMsgCount;

    /** transmit extended remote message count */
    unsigned int m_unTxEXTD_RTRMsgCount;

    /** total receive message count */
    unsigned int m_unTotalRxMsgCount;

    /** receive standard message count */
    unsigned int m_unRxSTDMsgCount;

    /** receive extended message count */
    unsigned int m_unRxEXTDMsgCount;

    /** receive standard remote message count */
    unsigned int m_unRxSTD_RTRMsgCount;

    /** receive extended remote message count */
    unsigned int m_unRxEXTD_RTRMsgCount;

    /** error total count */
    unsigned int m_unErrorTotalCount;

    /** error receive count */
    unsigned int m_unErrorRxCount;

    /** data length code count */
    unsigned int m_unDLCCount;
};
typedef sSUBBUSSTATISTICS SSUBBUSSTATISTICS;
typedef sSUBBUSSTATISTICS* PSSUBBUSSTATISTICS;

/**
* Bus statistics structure
*/
struct sBUSSTATISTICS
{
    /** total bits per second */
    unsigned int m_unTotalBitsperSec;

    /** bus load */
    double m_dBusLoad;

    /** peak bus load */
    double m_dPeakBusLoad;

    /** total message count */
    unsigned int m_unTotalMsgCount;

    /** messages per second */
    unsigned int m_unMsgPerSecond;

    /** total transmit message count */
    unsigned int m_unTotalTxMsgCount;

    /** total transmit message rate */
    double m_dTotalTxMsgRate;

    /** transmit standard message count */
    unsigned int m_unTxSTDMsgCount;

    /** transmit standard message rate */
    double m_dTxSTDMsgRate;

    /** transmit extended message count */
    unsigned int m_unTxEXTDMsgCount;

    /** transmit extended message rate */
    double m_dTxEXTMsgRate;

    /** transmit standard remote message count */
    unsigned int m_unTxSTD_RTRMsgCount;

    /** transmit extended remote message count */
    unsigned int m_unTxEXTD_RTRMsgCount;

    /** total receive message count */
    unsigned int m_unTotalRxMsgCount;

    /** total receive message rate */
    double m_dTotalRxMsgRate;

    /** receive standard message count */
    unsigned int m_unRxSTDMsgCount;

    /** receive standard message rate */
    double m_dRxSTDMsgRate;

    /** receive extended message count */
    unsigned int m_unRxEXTDMsgCount;

    /** receive extended message rate */
    double m_dRxEXTMsgRate;

    /** receive standard remote message count */
    unsigned int m_unRxSTD_RTRMsgCount;

    /** receive extended remote message count */
    unsigned int m_unRxEXTD_RTRMsgCount;

    /** error transmit count */
    unsigned int m_unErrorTxCount;

    /** error transmit rate */
    double m_dErrorTxRate;

    /** error receive count */
    unsigned int m_unErrorRxCount;

    /** error receive rate */
    double m_dErrorRxRate;

    /** error total count */
    unsigned int m_unErrorTotalCount;

    /** error rate */
    double m_dErrorRate;

    /** data length code count */
    unsigned int m_unDLCCount;

    /** baud rate */
    double m_dBaudRate;

    /** total bus load */
    double m_dTotalBusLoad;

    /** samples */
    int m_nSamples;

    /** average bus load */
    double m_dAvarageBusLoad;

    /** transmit error counter */
    unsigned char m_ucTxErrorCounter;

    /** receive error counter */
    unsigned char m_ucRxErrorCounter;

    /** transmit peak error count */
    unsigned char m_ucTxPeakErrorCount;

    /** receive peak error count */
    unsigned char m_ucRxPeakErrorCount;

    /** status */
    unsigned char m_ucStatus;


    sBUSSTATISTICS& operator = ( sSUBBUSSTATISTICS& objRef )
    {
        m_unErrorTxCount = objRef.m_unErrorTxCount;
        m_unTotalTxMsgCount = objRef.m_unTotalTxMsgCount;
        m_unTxSTDMsgCount = objRef.m_unTxSTDMsgCount;
        m_unTotalBitsperSec = objRef.m_unTotalBitsperSec;
        m_unTxEXTDMsgCount = objRef.m_unTxEXTDMsgCount;
        m_unTxSTD_RTRMsgCount = objRef.m_unTxSTD_RTRMsgCount;
        m_unTxEXTD_RTRMsgCount = objRef.m_unTxEXTD_RTRMsgCount;
        m_unTotalRxMsgCount = objRef.m_unTotalRxMsgCount;
        m_unRxSTDMsgCount = objRef.m_unRxSTDMsgCount;
        m_unRxEXTDMsgCount = objRef.m_unRxEXTDMsgCount;
        m_unRxSTD_RTRMsgCount = objRef.m_unRxSTD_RTRMsgCount;
        m_unRxEXTD_RTRMsgCount = objRef.m_unRxEXTD_RTRMsgCount;
        m_unErrorTotalCount = objRef.m_unErrorTotalCount;
        m_unErrorRxCount = objRef.m_unErrorRxCount;
        m_unDLCCount = objRef.m_unDLCCount;
        return *this;
    }
};
typedef sBUSSTATISTICS SBUSSTATISTICS;
typedef sBUSSTATISTICS* PBUSSTATISTICS;

class CBaseBusStatisticCAN
{
    /* NOTE:
        1. unChannelIndex can be CAN_CHANNEL_0, CAN_CHANNEL_1, CAN_CHANNEL_ALL
        2. eDir can be DIR_RX, DIR_TX, DIR_ALL
        3. byIdType can be TYPE_ID_CAN_STANDARD, TYPE_ID_CAN_EXTENDED, TYPE_ID_CAN_ALL
        4. byMsgType can be TYPE_MSG_CAN_RTR, TYPE_MSG_CAN_NON_RTR, TYPE_MSG_CAN_ALL
        5. eLoad can be CURRENT, AVERAGE, PEAK
    */
public:

    /* This function should be called for the very first time. */
    virtual HRESULT BSC_DoInitialization(void) = 0;
    /* This function resets the bus statistics */
    virtual HRESULT BSC_ResetBusStatistic(void) = 0;
    /* Get the total number of valid messages transmitted to or
       received from the bus */
    virtual HRESULT BSC_GetTotalMsgCount(UINT unChannelIndex, eDirection eDir, BYTE byIdType,
                                         BYTE byMsgType, UINT& nMsgCount) = 0;
    /* Get the total number of error messages occurred while receiving or
       transmitting */
    virtual HRESULT BSC_GetTotalErrCount(UINT unChannelIndex, eDirection eDir, UINT& nErrCount) = 0;

    /* Get average number of msgs per second(Msg/s)*/
    virtual HRESULT BSC_GetAvgMsgCountPerSec(UINT unChannelIndex, eDirection eDir,
            BYTE byIdType, double& dMsgCount) = 0;
    /* Get average number of errors per second(Err/s)*/
    virtual HRESULT BSC_GetAvgErrCountPerSec(UINT unChannelIndex, eDirection eDir, double& dErrCount) = 0;
    /* Get the bus load */
    virtual HRESULT BSC_GetBusLoad(UINT unChannelIndex, eLOAD eLoad, double& dBusLoad) = 0;
    /* Get controller status */
    /* Note: eLoad can have only CURRENT, PEAK.*/
    virtual HRESULT BSC_GetErrorCounter(UINT unChannelIndex, eDirection eDir, eLOAD eLoad, UCHAR& ucErrCounter) = 0;
    //Setting BaudRate
    virtual HRESULT BSC_SetBaudRate(UINT unChannelIndex, double dBaudRate) = 0;
    //Start updating BusStatistics data
    virtual BOOL BSC_bStartUpdation(BOOL bStart)= 0;
    //Get the controller status
    virtual UCHAR BSC_ucGetControllerStatus(UINT unChannelIndex) = 0;
    //Get the required channel's BusStistic structure.
    virtual HRESULT BSC_GetBusStatistics(UINT unChannelIndex, SBUSSTATISTICS& sBusStatistics) = 0;
};
