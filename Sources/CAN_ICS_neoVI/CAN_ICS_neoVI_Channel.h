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
 * \file      CAN_ICS_neoVI/CAN_ICS_neoVI_Channel.h
 * \brief     Interface file for CChannel class
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CChannel class
 */

#pragma once

#include "include/struct_can.h"

class CChannel
{
public:
    /** To Initialise controller state and error counter values */
    void vResetChannel();

    /**
     * To update error counter and controller state
     * Returns the state transtition state
     */
    void vUpdateErrorCounter(UCHAR ucTxError, UCHAR ucRxError);

    /** Standard Constructor */
    CChannel();

    /**
     * Don't declate it as virtual to avoid VTable as this class will not be
     * inherited
     */
    ~CChannel();

    USHORT m_usClock;
    UINT   m_unBaudrate;
    USHORT m_usSampling;

    /** To store baud rate information */
    USHORT  m_usBaudRate;
    BYTE    m_bCNF1;
    BYTE    m_bCNF2;
    BYTE    m_bCNF3;

    /**
     * To store controller state
     * 0 - Active State
     * 1 - Passive State
     * 2 - Bus Off State
     * -1 - For invalid value
     */
    UCHAR  m_ucControllerState;

    /** Acceptance Filter information */
    SACC_FILTER_INFO     m_sFilter;

    /** Hardware Handle Associated with this channel */
    BYTE m_hHardwareHandle;
    /** Network Handle Associated with this channel */
    BYTE m_hNetworkHandle;

    /** Programmed warning limit of this channel */
    UCHAR m_ucWarningLimit;

    /** Tx Error counter value */
    UCHAR m_ucTxErrorCounter;

    /** Rx Error counter value */
    UCHAR m_ucRxErrorCounter;

    /** Peak Tx Error counter value */
    UCHAR m_ucPeakTxErrorCounter;

    /** Peak Rx Error counter value */
    UCHAR m_ucPeakRxErrorCounter;

    /** Tx Error Handler execution state */
    BOOL m_bTxErrorExecuted;

    /** Rx Error Handler execution state */
    BOOL m_bRxErrorExecuted;
};
