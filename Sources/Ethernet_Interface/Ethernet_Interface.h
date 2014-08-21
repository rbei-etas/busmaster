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
 * \file      CAN_Kvaser_CAN.h
 * \brief     Exports API functions for Kvaser CAN Hardware interface
 * \author    Ashwin R Uchil
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Exports API functions for Kvaser CAN Hardware interface
 */
#pragma once
#include "Ethernet_Interface_Stdafx.h"

/**
 * See Ethernet_Interface.cpp for the implementation of this class
 */
class CEthernet_Interface : public CWinApp
{
public:
    CEthernet_Interface();

    // Overrides
public:
    virtual BOOL InitInstance();

    DECLARE_MESSAGE_MAP()
};
