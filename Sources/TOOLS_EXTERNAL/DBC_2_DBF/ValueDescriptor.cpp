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
 * \brief     Implementation file for the ParameterValues class.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for the ParameterValues class.
 */

#include "StdAfx.h"
#include "App.h"
#include "ValueDescriptor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/**
 * \brief Constructor
 */
CValueDescriptor::CValueDescriptor()
{
    m_value.dValue = 1; // assume double type for default constructor
    m_acDescriptor[0] = '\0';
}

/**
 * \brief Destructor
 */
CValueDescriptor::~CValueDescriptor()
{
    // nothing special to do here
}

/**
 * \brief writes the value descriptors in the given list to the output file
 * \param[in] fileOutput Pointer to the Output file
 * \param[in] m_ucType data type of the value
 * \param[in] m_listSignals List of Value descriptors
 *
 * Writes the value descriptors in the given list to the output file.
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
            case CSignal::SIG_TYPE_BOOL:
            case CSignal::SIG_TYPE_UINT:
                sprintf(acLine,"%s \"%s\",%u\n",T_VALUE_DESC,rValDesc.m_acDescriptor,rValDesc.m_value.uiValue);
                break;

            case CSignal::SIG_TYPE_INT:
                sprintf(acLine,"%s \"%s\",%d\n",T_VALUE_DESC,rValDesc.m_acDescriptor,rValDesc.m_value.iValue);
                break;

                // when FLOAT and DOUBLE are supported enable this
                /*
                case CSignal::SIG_TYPE_FLOAT:
                sprintf(acLine,"%s \"%s\",%f\n",T_VALUE_DESC,rValDesc.m_acDescriptor,rValDesc.m_value.fValue);
                break;

                case CSignal::SIG_TYPE_DOUBLE:
                sprintf(acLine,"%s \"%s\",%f\n",T_VALUE_DESC,rValDesc.m_acDescriptor,rValDesc.m_value.dValue);
                break;
                */

            case CSignal::SIG_TYPE_INT64:
                sprintf(acLine,"%s \"%s\",%I64d\n",T_VALUE_DESC,rValDesc.m_acDescriptor,rValDesc.m_value.i64Value);
                break;

            case CSignal::SIG_TYPE_UINT64:
                sprintf(acLine,"%s \"%s\",%I64u\n",T_VALUE_DESC,rValDesc.m_acDescriptor,rValDesc.m_value.ui64Value);
                break;

            default:
                break;
        }

        fileOutput.WriteString(acLine);
    }
    return;
}
