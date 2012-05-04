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
 * \file      WrapperErrorLogger.h
 * \brief     Concrete data type definition file for error logger wrapper.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Concrete data type definition file for error logger wrapper.
 */

#pragma once

/* C++ includes */
#include <string>

/* Project includes */
#include "DataTypes/Base_WrapperErrorLogger.h"

using namespace std;

class WrapperErrorLogger : public Base_WrapperErrorLogger
{
public:
    WrapperErrorLogger(void);
    ~WrapperErrorLogger(void);

    void vLogAMessage(char* File, int Line, string Msg);
};
