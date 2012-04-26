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
 * \file      ValueDescriptor.cpp
 * \brief     Implementation of the CValueDescriptor class.
 * \authors   Mahesh.B.S, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the CValueDescriptor class.
 */

/* Project includes */
#include "Converter.h"
#include "Signal.h"
#include "ValueDescriptor.h"

using namespace std;

/**
 * \brief Constructor
 *
 * Constructor of CValueDescriptor
 */
CValueDescriptor::CValueDescriptor()
{
    m_value.iValue = 1;
    m_value.uiValue = 1;
    m_value.fValue = 1;
    m_value.dValue = 1;
    m_value.cValue = "";
    m_value.i64Value = 1;
    m_value.ui64Value = 1;
}

/**
 * \brief Destructor
 *
 * Destructor of CValueDescriptor
 */
CValueDescriptor::~CValueDescriptor()
{
}

/**
 * \brief      Extracts the Value Descriptor pair from the given Line
 * \param[in]  pcLine The given line
 * \return     Status code
 *
 * Extracts the Value Descriptor pair from the given Line
 * and creates a list of the same
 */
int CValueDescriptor::Format(char* pcLine)
{
    char acValue[256] = {'\0'};
    char acDesc[256] = {'\0'};
    char* pcValue = acValue;
    char* pcDesc = acDesc;
    pcValue = acValue;
    pcDesc = acDesc;

    // skip leading spaces
    while(*pcLine && *pcLine == ' ')
    {
        *pcLine++;
    }

    // skip leading '"'
    while(*pcLine && *pcLine == '\"')
    {
        if(*pcLine == '\"')
        {
            *pcLine++;
            break;
        }

        *pcLine++;
    }

    if(*pcLine != '\"')
        while(*pcLine && *pcLine != '\"')
        {
            *pcDesc++ = *pcLine;
            pcLine++;
        }

    *pcDesc = '\0';
    pcLine++;

    // skip spaces if any before next iteration.
    while(*pcLine && *pcLine == ',')
    {
        *pcLine++;
    }

    while(*pcLine && *pcLine != ' ')
    {
        *pcValue++ = *pcLine++; // copy all but terminating space
    }

    *pcValue = '\0'; // terminate the string
    // if any value read then add it to list
    m_value.i64Value = _atoi64(acValue);
    m_sDescriptor = acDesc;
    return 1;
}

/**
 * \brief     Writes the Value Descriptor in the CANoe format
 * \param[in] fileOutput Output file
 * \param[in] m_ucType Value Descriptor Type
 * \param[in] m_listValueDescriptor List of Value Descriptors
 *
 * Writes the Value Descriptor in the CANoe format in the output file
 */
void CValueDescriptor::writeValueDescToFile(fstream& fileOutput,char m_ucType,list<CValueDescriptor> &m_listValueDescriptor)
{
    list<CValueDescriptor>::iterator rValDesc;

    for(rValDesc=m_listValueDescriptor.begin(); rValDesc!=m_listValueDescriptor.end(); ++rValDesc)
    {
        switch(m_ucType)
        {
            case CSignal::SIG_TYPE_INT:
            case CSignal::SIG_TYPE_FLOAT:
            case CSignal::SIG_TYPE_DOUBLE:
            case CSignal::SIG_TYPE_INT64:
                fileOutput << dec << rValDesc->m_value.i64Value;
                fileOutput << " \"" << rValDesc->m_sDescriptor.c_str() << "\" ";
                break;

            case CSignal::SIG_TYPE_BOOL:
            case CSignal::SIG_TYPE_UINT:
            case CSignal::SIG_TYPE_UINT64:
                fileOutput << dec << rValDesc->m_value.ui64Value;
                fileOutput << " \"" << rValDesc->m_sDescriptor.c_str() << "\" ";
                break;

            default:
                break;
        }
    }

    fileOutput << ";" << endl;
    return;
}
