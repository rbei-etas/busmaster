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
 * \file      CAN_ICS_neoVI.h
 * \brief     Exports API functions for IntrepidCS neoVI CAN Hardware interface
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Exports API functions for IntrepidCS neoVI CAN Hardware interface
 */

#pragma once

/**
 * See CAN_ICS_neoVI.cpp for the implementation of this class
 */
class CCAN_ICS_neoVIApp : public CWinApp
{
public:
    CCAN_ICS_neoVIApp();

    // Overrides
public:
    virtual BOOL InitInstance();

    DECLARE_MESSAGE_MAP()
};
