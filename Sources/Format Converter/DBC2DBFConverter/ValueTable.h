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
 * \file      ValueTable.h
 * \brief     Definition of value table class
 * \authors   Mahesh B S, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of the value table class.
 */

#pragma once

/* C++ includes */
#include <fstream>
#include <list>
#include <string>

/* Project includes */
#include "ValueDescriptor.h"

using namespace std;

class CValueTable
{
public:
    CValueTable();
    virtual ~CValueTable();
    int Format(char* pcLine, fstream& fileInput);
    list <CValueDescriptor> m_values;
    CValueTable& operator=(CValueTable& Tab);
    string m_TableName;
    void writeValueTabToFile(fstream& fileOutput, list<CValueTable> &vTab);
};
