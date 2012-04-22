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
 * \file      AppServices_Impl.h
 * \brief     Concrete class definition for the application service layer
 * \authors   Ratnadip Choudhury, Anish Kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Concrete class definition for the application service layer
 */

#pragma once

#include "DataTypes/BaseAppServices.h"

class CAppServices_Impl : public CBaseAppServices
{
public:
    // Variables
    CUIThread* m_podUIThread;

public:
    CAppServices_Impl();       // Standard constructor
    ~CAppServices_Impl();      // Destructor

    // Functions
    BOOL bWriteToTrace(TCHAR* pcTraceStr);
};
