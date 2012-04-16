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
 * \brief     Interface file for CSignal class
 * \author    Amit Ranjan
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CSignal class
 */

#pragma once

/* C++ includes */
#include <string>
#include <afxtempl.h>

/* Project includes */
#include "ValueDescriptor.h"

using namespace std;

#define defSIG_MAX_NAME_LEN	100 // normally name > 100 chars not expected
#define defSIG_MAX_MULTIPLEX_LEN 33
#define defVTAB_MAX_LINE_LEN 1026
#define	defSIG_MAX_UNIT_LEN	20	// 
class CValueDescriptor;
class CSignal
{
public:
    void GetErrorString(string &str);
    unsigned int Validate(unsigned char ucFormat = 0);
    unsigned int m_uiError;
    int AddValueDescriptors(char * pcLine);
    int Format(char *pcLine);
    CSignal();
    virtual ~CSignal();
    CSignal& operator=(CSignal& signal);
    enum SIG_DATA_FORMAT {SIG_DF_MOTOROLA='0',SIG_DF_INTEL = '1'};
    enum SIG_TYPE {SIG_TYPE_BOOL = 'B',SIG_TYPE_INT = 'I',SIG_TYPE_UINT = 'U',SIG_TYPE_FLOAT = 'F', SIG_TYPE_DOUBLE = 'D', SIG_TYPE_INT64 = 'X',SIG_TYPE_UINT64 = 'Y'};
    enum SIG_ERROR_CODE {SIG_EC_NO_ERR, SIG_EC_DATA_FORMAT_ERR,SIG_EC_LENGTH_ERR,SIG_EC_STARTBIT_ERR,SIG_EC_TYPE_ERR, SIG_EC_OVERLAP};
// data members

    string          m_acName;
    string          m_acMultiplex;
    unsigned int	m_ucLength;
    unsigned int	m_ucWhichByte;
    unsigned int	m_ucStartBit;
    char			m_ucType;
    SIG_VALUE		m_MaxValue;
    SIG_VALUE		m_MinValue;
    unsigned char	m_ucDataFormat;
    float			m_fOffset;
    float			m_fScaleFactor;
    string			m_acUnit;
    string			m_rxNode;

    CList<CValueDescriptor,CValueDescriptor>	m_listValueDescriptor;
};
