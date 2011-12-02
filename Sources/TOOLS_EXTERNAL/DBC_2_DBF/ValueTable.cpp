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
 * \file      ValueTable.cpp
 * \brief     Implementation of the value table class
 * \author    Mahesh.B.S, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the value table class.
 */

#include <list>
#include <string>

#include "ValueTable.h"
#include "Signal.h"
#include "Tag.h"

using namespace std;


/**
 * \brief Constructor
 */
CValueTable::CValueTable()
{
    m_TableName = "";
}


/**
 * \brief Destructor
 */
CValueTable::~CValueTable()
{
}


/**
 * \brief = operator overloading
 */
CValueTable& CValueTable::operator=(CValueTable& Tab)
{
    m_TableName = Tab.m_TableName;
    m_values = Tab.m_values;
    return (*this);
}


/**
 * \brief Extracts Values and value descriptors from the line
 * \param[in] pcLine String having the value table name followed by the
 *            value, value descriptor as in canoe format
 *
 * Extracts Values and value descriptors from the line.
 */
int CValueTable::Format(char *pcLine, fstream &fileInput)
{
    char acValue[defVTAB_MAX_VALU_LEN];
    char acLine[defVTAB_MAX_LINE_LEN] = {'\0'};
    char acDesc[defVTAB_MAX_DESC_LEN];
    char table_Name[defVTAB_MAX_TNAM_LEN];
    bool true_end = true;
    char pcTemp[defVTAB_MAX_TEMP_LEN];
    char *pcValue = acValue;
    char *pcDesc = acDesc;
    char *pcTab = table_Name;

    // skip leading spaces
    while(*pcLine && *pcLine == ' ')
    {
        *pcLine++;
    }

    // get table name
    while(*pcLine && *pcLine != ' ')
    {
        if (*pcLine != '\r')
            *pcTab++ = *pcLine;
        pcLine++;
    }
    *pcTab = '\0';
    m_TableName = table_Name;

    // skip spaces
    while(*pcLine && *pcLine == ' ')
    {
        *pcLine++;
    }
    if(pcLine[strlen(pcLine)-1] != ';')
    {
        true_end  = false;
    }

    while(*pcLine && *pcLine != ';')
    {
        //Value table can be of more than 1024 char. So when the we have almost reached
        //the end of the buffer check if we have reached the correct end i.e ";".If not than
        //read the next line and proceed.
        if(true_end == false)
        {
            fileInput.getline(acLine, defVTAB_MAX_LINE_LEN);
            strcpy_s(pcTemp, sizeof(pcTemp), pcLine);
            strcat_s(pcTemp, sizeof(pcTemp), acLine);
            pcLine = pcTemp;
            if(pcLine[strlen(pcLine)-1] == ';')
                true_end  = true;
        }
        pcValue = acValue;
        pcDesc = acDesc;

        // get value
        *pcValue = *pcDesc = '\0';
        while(*pcLine && *pcLine != ' ')
        {
            if (*pcLine != '\r')
                *pcValue++ = *pcLine;
            pcLine++;
        }
        *pcValue = '\0'; // terminate the string

        // skip spaces
        while(*pcLine && *pcLine == ' ')
        {
            *pcLine++;
        }

        // skip '"'
        while(*pcLine && *pcLine == '\"')
        {
            *pcLine++;
        }

        // get description
        while(*pcLine && *pcLine != '\"')
        {
            if (*pcLine != '\r')
                *pcDesc++ = *pcLine;
            pcLine++;
        }
        *pcDesc = '\0';

        // skip trailing '"'
        pcLine++;

        // skip spaces if any before next iteration.
        while(*pcLine && *pcLine == ' ')
        {
            *pcLine++;
        }

        // if any value read then add it to list
        if(acDesc[0] != '\0')
        {
            CValueDescriptor valDesc;
            valDesc.m_value.i64Value = atoi(acValue);
            valDesc.m_acDescriptor = acDesc;
            m_values.push_back(valDesc);
        }
    }

    return 1;
}


/**
 * \brief     writes the value tebles in the given list to the output file
 * \param[in] fileOutput Pointer to the Output file
 * \param[in] vTab List of Value tables
 *
 * Writes the value tebles in the given list to the output file.
 */
void CValueTable::writeValueTabToFile(fstream &fileOutput, list<CValueTable> &vTab)
{
    list<CValueTable>::iterator tab;
    for (tab=vTab.begin(); tab!=vTab.end(); ++tab)
    {
        fileOutput << T_ST_TAB " ";
        fileOutput << tab->m_TableName.c_str();
        fileOutput << "\n";

        CValueDescriptor desc;
        desc.writeValueDescToFile (fileOutput,CSignal::SIG_TYPE_INT64, tab->m_values);

        fileOutput << T_END_TAB "\n";
    }
}
