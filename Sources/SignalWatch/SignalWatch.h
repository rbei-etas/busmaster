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
 * \file      SignalWatch.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * main header file for the SignalWatch DLL
 */

#pragma once

#include "SignalWatch_resource.h"       // main symbols


// CSignalWatchApp
// See SignalWatch.cpp for the implementation of this class
//

class CSignalWatchApp : public CWinApp
{
public:
    CSignalWatchApp();

    // Overrides
public:
    virtual BOOL InitInstance();
    virtual INT ExitInstance();

    DECLARE_MESSAGE_MAP()

};
