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
 * \file      CAN_ICS_neoVI/CAN_ICS_neoVI_Channel.cpp
 * \brief     Implementation file for CChannel class
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CChannel class
 */

#include "CAN_ICS_neoVI_stdafx.h"
#include "CAN_ICS_neoVI_Channel.h" // For CChannel class declaration
#include "Include/CanUsbDefs.h"

/**
 * Standard default constructor
 */
CChannel::CChannel()
{
    // Init members with default value
    // Baud Rate
    m_usBaudRate = defBAUD_RATE;
    // Controller state
    m_ucControllerState = defMODE_ACTIVE;
    // Hardware Handle Associated with this channel
    m_hHardwareHandle = 0;
    // Network Handle Associated with this channel
    m_hNetworkHandle = 0;
    // Programmed warning limit of this channel
    m_ucWarningLimit = defWARNING_LIMIT_INT;
    // Tx Error counter value
    m_ucTxErrorCounter = 0;
    // Rx Error counter value
    m_ucRxErrorCounter = 0;
    // Peak values of Rx and Tx Error Counter
    m_ucPeakRxErrorCounter = m_ucPeakTxErrorCounter = 0;
    // Tx Error Handler execution state
    m_bTxErrorExecuted = FALSE;
    // Rx Error Handler execution state
    m_bRxErrorExecuted = FALSE;
}

/**
 * Standard Destructor
 */
CChannel::~CChannel()
{
}

/**
 * \param[in] ucTxError Tx Error Value
 * \param[in] ucRxError Rx Error Value
 *
 * Updates the error counter value and controller state as per
 * the error counter values
 */
void CChannel::vUpdateErrorCounter(UCHAR ucTxError, UCHAR ucRxError)
{
    // Update Tx Error counter and peak Tx Error Counter
    m_ucTxErrorCounter = ucTxError;

    // Update Peak Value
    if( m_ucTxErrorCounter > m_ucPeakTxErrorCounter )
    {
        m_ucPeakTxErrorCounter = m_ucTxErrorCounter;
    }

    // Update Rx Error counter and peak Rx Error Counter
    m_ucRxErrorCounter = ucRxError;

    // Update Peak Value
    if( m_ucRxErrorCounter > m_ucPeakRxErrorCounter )
    {
        m_ucPeakRxErrorCounter = m_ucRxErrorCounter;
    }

    // Update Controller state
    if ((m_ucTxErrorCounter <= 127) && (m_ucRxErrorCounter <= 127))
    {
        // Error Active Mode
        m_ucControllerState = defCONTROLLER_ACTIVE;
    }
    // The sudden shift to the third state is to avoid
    // "else if ((byTxError > 127) || (byRxError > 127))"
    else if (m_ucTxErrorCounter == 255)
    {
        // Bus off
        m_ucControllerState = defCONTROLLER_BUSOFF;
    }
    else
    {
        // Error passive
        m_ucControllerState = defCONTROLLER_PASSIVE;
    }
}

/**
 * This function will reset all state related information. This
 * will be called during the hardware reset of the machine
 */
void CChannel::vResetChannel()
{
    // Controller state
    m_ucControllerState = defMODE_ACTIVE;
    // Tx Error counter value
    m_ucTxErrorCounter = 0;
    // Rx Error counter value
    m_ucRxErrorCounter = 0;
    // Peak values of Rx and Tx Error Counter
    m_ucPeakRxErrorCounter = m_ucPeakTxErrorCounter = 0;
    // Tx Error Handler execution state
    m_bTxErrorExecuted = FALSE;
    // Rx Error Handler execution state
    m_bRxErrorExecuted = FALSE;
}
