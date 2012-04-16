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
 * \file      ValueDescriptor.h
 * \brief     Definition of value descriptor class
 * \author    Mahesh B S, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of the value descriptor class.
 */

#pragma once

/* C++ includes */
#include <fstream>
#include <list>

/* Project includes */
#include "Definitions.h"

using namespace std;

class CValueDescriptor
{
public:
    CValueDescriptor();
    virtual ~CValueDescriptor();

    // data members
    string      m_acDescriptor;
    SIG_VALUE   m_value;
    void writeValueDescToFile(fstream &fileOutput, char m_ucType, list<CValueDescriptor> &m_listValueDescriptor);
};
