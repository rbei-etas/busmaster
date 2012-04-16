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
 * \brief     CValueDescriptor class.
 * \author    Mahesh B S
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CValueDescriptor class.
 */

#pragma once

/* Project includes */
#include "stdafx.h"

class CValueDescriptor
{
public:
    CValueDescriptor();
    virtual ~CValueDescriptor();
    int Format(char *pcLine);
    //	CValueDescriptor& operator=(CValueDescriptor& valDesc);
    void writeValuDescToFile(fstream &fileOutput,char m_ucType,CList<CValueDescriptor,CValueDescriptor&> &m_listValueDescriptor);
    string		m_sDescriptor;
    SIG_VALUE	m_value;
};
