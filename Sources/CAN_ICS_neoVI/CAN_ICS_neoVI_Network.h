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
 * \file      CAN_ICS_neoVI/CAN_ICS_neoVI_Network.h
 * \brief     Interface file for CNetwork class
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CNetwork class
 */

#pragma once

#include "Include/CanUsbDefs.h"
class CNetwork
{
public:
    /** Constructor */
    CNetwork();

    /** Destructor */
    virtual ~CNetwork();

    /** Number of Channels or hardware actually present */
    UINT m_nNoOfChannels;
    /** Number of devices connected. */
    UINT m_nNoOfDevices;

    /**
     * Array of channel objects. The size of this array is maximun number
     * of channels the application will support.
     */
    CChannel m_aodChannels[ defNO_OF_CHANNELS ];
};
