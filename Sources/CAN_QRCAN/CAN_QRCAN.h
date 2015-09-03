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
 * \file      CAN_QRCAN.h
 * \author    
 * \copyright Copyright (c) ((Year)), QRTECH AB. All rights reserved.
 */
#pragma once


int WINAPI ShowQRCANConfig(HWND hwnd);

/**
 * See CAN_QRCAN.cpp for the implementation of this class
 */
class CCAN_QRCAN : public CWinApp
{
public:
    CCAN_QRCAN();

    // Overrides
public:
    virtual BOOL InitInstance();

    DECLARE_MESSAGE_MAP()
};
