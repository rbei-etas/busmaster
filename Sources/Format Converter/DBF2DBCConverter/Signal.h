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
 * \file      Signal.h
 * \brief     CSignal class.
 * \authors   Mahesh B S, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CSignal class.
 */

#pragma once

/* C++ includes */
#include <fstream>
#include <list>
#include <string>

/* Project includes */
#include "SignalValue.h"
#include "ValueDescriptor.h"

using namespace std;

class CSignal
{
public:
    CSignal();
    virtual ~CSignal();
    int Format(char* pcLine);
    CSignal& operator=(CSignal& signal);
    unsigned int Validate();
    bool WriteSignaltofile(fstream& fileOutput);
    void GetErrorString(string& str);
    int operator==(const CSignal& Signal) const;
    enum SIG_DATA_FORMAT {SIG_DF_MOTOROLA='0', SIG_DF_INTEL = '1'};
    enum SIG_TYPE {SIG_TYPE_BOOL = 'B',SIG_TYPE_INT = 'I',SIG_TYPE_UINT = 'U',SIG_TYPE_FLOAT = 'F', SIG_TYPE_DOUBLE = 'D', SIG_TYPE_INT64 = 'X',SIG_TYPE_UINT64 = 'Y'};
    enum SIG_ERROR_CODE {SIG_EC_NO_ERR=0,SIG_EC_LENGTH_ERR,SIG_EC_STARTBIT_ERR};

    //members
    string          m_sName;
    string          m_sMultiplex;
    unsigned int    m_ucLength;
    unsigned int    m_ucWhichByte;
    unsigned int    m_ucStartBit;
    char            m_ucType;
    CSignalValue       m_MaxValue;
    CSignalValue       m_MinValue;
    unsigned char   m_ucDataFormat;
    float           m_fOffset;
    float           m_fScaleFactor;
    unsigned int    m_uiError;

    string          m_sUnit;
    string          m_sNode;
    list<CValueDescriptor>   m_listValueDescriptor;
};
