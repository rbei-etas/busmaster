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
 * \file      CAN_MHS.h
 * \brief     Exports API functions for MHS-Elektronik Tiny-CAN Hardware interface
 * \author    Klaus Demlehner
 * \copyright Copyright (c) 2011, MHS-Elektronik GmbH & Co. KG
 *
 * Exports API functions for Vector CAN Hardware interface
 */

#pragma once

/**
 * See CAN_MHS.cpp for the implementation of this class
 */
class CCAN_MHS : public CWinApp
{
public:
    CCAN_MHS();

// Overrides
public:
    virtual BOOL InitInstance();

    DECLARE_MESSAGE_MAP()
};
