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
 * \file      CAN_ICS_neoVI/CAN_ICS_neoVI_Network.cpp
 * \brief     Implementation file for CNetwork class
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CNetwork class
 */

//#include "stdafx.h"         // For standard includes
#include "CAN_ICS_neoVI_stdafx.h"
#include "CAN_ICS_neoVI_Channel.h" // For CChannel class declaration
#include "CAN_ICS_neoVI_Network.h" // For CNetwork class declaration

/**
 * Standard default constructor
 */
CNetwork::CNetwork()
{
    // Initialise members
    // Number of chennals present or initialised
    m_nNoOfChannels = 0;
    m_nNoOfDevices  = 0;    
}

/**
 * Standard Destructor
 */
CNetwork::~CNetwork()
{

}
