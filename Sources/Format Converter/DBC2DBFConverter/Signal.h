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
 * \brief     Definition of signal class
 * \authors   Padmaja A, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of the signal class.
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
    void GetErrorString(string& str);
    void GetErrorAction(string& str);
    unsigned int Validate(unsigned char ucFormat = 0);
    unsigned int m_uiError;
    int AddValueDescriptors(char* pcLine, fstream& fileInput);
    int Format(char* pcLine);
    CSignal();
    virtual ~CSignal();
    CSignal& operator=(CSignal& signal);
    enum SIG_DATA_FORMAT {SIG_DF_MOTOROLA='0',SIG_DF_INTEL = '1'};
    enum SIG_TYPE {SIG_TYPE_BOOL = 'B',SIG_TYPE_INT = 'I',SIG_TYPE_UINT = 'U',SIG_TYPE_FLOAT = 'F', SIG_TYPE_DOUBLE = 'D', SIG_TYPE_INT64 = 'X',SIG_TYPE_UINT64 = 'Y'};
    enum SIG_ERROR_CODE {SIG_EC_NO_ERR, SIG_EC_DATA_FORMAT_ERR,SIG_EC_LENGTH_ERR,SIG_EC_STARTBIT_ERR,SIG_EC_TYPE_ERR,SIG_EC_OVERFLOW};
    // data members

    string          m_acName;
    string          m_acMultiplex;
    unsigned int    m_ucLength;
    unsigned int    m_ucWhichByte;
    unsigned int    m_ucStartBit;
    char            m_ucType;
    CSignalValue    m_MaxValue;
    CSignalValue    m_MinValue;
    unsigned char   m_ucDataFormat;
    float           m_fOffset;
    float           m_fScaleFactor;
    string          m_acUnit;
    string          m_rxNode;

    list<CValueDescriptor>  m_listValueDescriptor;
    bool WriteSignaltofile(fstream& fileOutput, list<CSignal> &m_listSignals, int m_ucLength, int m_cDataFormat, bool writeErr);
};
