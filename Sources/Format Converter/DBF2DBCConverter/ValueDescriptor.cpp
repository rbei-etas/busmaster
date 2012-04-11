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
 * \author    RBIN/EBS1 - Mahesh.B.S
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the CValueDescriptor class.
 */
/**
* \file       ValueDescriptor.cpp
* \brief      implementation of the CValueDescriptor class.
* \authors    Mahesh.B.S
* \date       15.11.2004
* \copyright  Copyright &copy; 2011 Robert Bosch Engineering and Business Solutions.  All rights reserved.
*/
#include "StdAfx.h"
#include "ValueDescriptor.h"
#include "Signal.h"
#include "Converter.h"


/**
* \brief      Constructor of CValueDescriptor
* \param[in]  None
* \param[out] None
* \return     None
* \authors    Mahesh.B.S
* \date       15.11.2002
*/
CValueDescriptor::CValueDescriptor()
{

}

/**
* \brief      destructor of CValueDescriptor
* \param[in]  None
* \param[out] None
* \return     None
* \authors    Mahesh.B.S
* \date       15.11.2002
*/
CValueDescriptor::~CValueDescriptor()
{

}

/**
* \brief      Extracts the Value Descriptor pair from the given Line
and creates a list of the same
* \param[in]  char *pcLine
* \param[out] None
* \return     int
* \authors    Mahesh.B.S
* \date       15.11.2002
*/
int CValueDescriptor::Format(char *pcLine)
{
	char acValue[256] = {'\0'};
    char acDesc[256] = {'\0'};

    char *pcValue = acValue;
    char *pcDesc = acDesc;
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
    {
        m_value.i64Value = _atoi64(acValue);
        m_sDescriptor = acDesc;
        if(    m_sDescriptor.GetLength() > defCON_MAX_MSGN_LEN + 2)
        {
            char logmsg[defCON_MAX_LINE_LEN];
            sprintf(logmsg,"value Descriptor %s changed as %s\n",m_sDescriptor,m_sDescriptor.Left(defCON_MAX_MSGN_LEN));
            CConverter::fileLog.WriteString(logmsg);
            CConverter::bLOG_ENTERED = true;
            m_sDescriptor = m_sDescriptor.Left(defCON_MAX_MSGN_LEN);
        }
    }
    return 1;
}

/**
* \brief      Write's the Value Descriptor in the CANoe format
* \param[in]  CStdioFile &fileOutput,char ,CList<CValueDescriptor,CValueDescriptor&>
* \param[out] None
* \return     int
* \authors    Mahesh.B.S
* \date       15.11.2002
*/
void CValueDescriptor::writeValuDescToFile(CStdioFile &fileOutput,char m_ucType,CList<CValueDescriptor,CValueDescriptor&> &m_listValueDescriptor)
{
    char acLine[defVDES_MAX_OUT_STR];
    POSITION posValDesc = m_listValueDescriptor.GetHeadPosition();
    while(posValDesc != NULL)
    {
        CValueDescriptor& rValDesc = m_listValueDescriptor.GetNext(posValDesc);
        switch(m_ucType)
        {
            case CSignal::SIG_TYPE_INT:
            case CSignal::SIG_TYPE_FLOAT:
            case CSignal::SIG_TYPE_DOUBLE:
            case CSignal::SIG_TYPE_INT64:
                sprintf(acLine,"%I64d \"%s\" ",rValDesc.m_value.i64Value,rValDesc.m_sDescriptor);
                break;

            case CSignal::SIG_TYPE_BOOL:
            case CSignal::SIG_TYPE_UINT:
            case CSignal::SIG_TYPE_UINT64:
                sprintf(acLine,"%I64u \"%s\" ",rValDesc.m_value.ui64Value,rValDesc.m_sDescriptor);
                break;

            default:
                break;
        }

        fileOutput.WriteString(acLine);
    }
    fileOutput.WriteString(";\n");
    return;
}
