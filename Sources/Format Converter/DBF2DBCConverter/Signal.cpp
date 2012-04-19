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
 * \file      Signal.cpp
 * \brief     Implementation of the CSignal class.
 * \authors   Mahesh.B.S, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the CSignal class.
 */

/* Project inclues */
#include "Converter.h"
#include "Signal.h"
#include "Utility.h"

using namespace std;

/**
 * \brief Constructor
 *
 * Constructor of CSignal
 */
CSignal::CSignal()
{
    m_ucLength = 1; // minimum signal length should be 1 bit
    m_ucWhichByte = 1; // ONE based index
    m_ucStartBit = 0; // ZERO based index
    m_ucType = SIG_TYPE_BOOL; // let default be bool type -- match with default length
    m_MaxValue.uiValue = 1; // use unsigned int field for bool also
    m_MinValue.uiValue = 0; //
    m_ucDataFormat = SIG_DF_INTEL;
    m_fOffset = 0.0f;
    m_fScaleFactor = 1.0f;
    m_uiError = SIG_EC_NO_ERR;
    m_listValueDescriptor.clear();
}

/**
 * \brief Destructor
 *
 * Destructor of CSignal
 */
CSignal::~CSignal()
{
}

/**
 * \brief     Overloaded operator =
 * \param[in] signal Object to copy content from
 * \return    Local object with new contents
 *
 * Copies the contents of signal into the local object.
 */
CSignal& CSignal::operator=(CSignal& signal)
{
    // copy all the data members except the list
    m_sName = signal.m_sName;
    m_sMultiplex = signal.m_sMultiplex;
    m_ucLength = signal.m_ucLength;
    m_ucWhichByte = signal.m_ucWhichByte;
    m_ucStartBit = signal.m_ucStartBit;
    m_ucType = signal.m_ucType;
    m_MaxValue = signal.m_MaxValue;
    m_MinValue = signal.m_MinValue;
    m_ucDataFormat = signal.m_ucDataFormat;
    m_fOffset = signal.m_fOffset;
    m_fScaleFactor = signal.m_fScaleFactor;
    m_sUnit = signal.m_sUnit;
    m_sNode = signal.m_sNode;
    m_uiError = signal.m_uiError;
    //  now copy the list
    m_listValueDescriptor = signal.m_listValueDescriptor;
    return (*this);
}

/**
 * \brief     Operator==
 * \param[in] signal Reference signal
 * \return    1 if identical, 0 if not.
 *
 * Compare content of signal with local object.
 */
int CSignal::operator==(const CSignal& signal) const
{
    if (m_sName == signal.m_sName)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * \brief     Extracts the signal data from the given Line
 * \param[in] pcLine The given line
 * \return    Return code
 *
 * Extracts the signal data from the given Line and populates
 * message structure.
 */
int CSignal::Format(char* pcLine)
{
    char* pcToken;
    char acTemp[defCON_MAX_TOKN_LEN],*pcTemp;
    pcTemp = acTemp;
    // get signal name
    pcToken = strtok(pcLine,","); // get upto colon
    // now get the signal name
    m_sName = pcToken; // copy the name to the signal's data member
    // Signal length
    pcToken = strtok(NULL,",");
    m_ucLength = atoi(pcToken); // store signal length
    // get which byte
    pcToken = strtok(NULL,",");
    m_ucWhichByte = atoi(pcToken);
    // get start bit
    unsigned int ucStartBit;
    pcToken = strtok(NULL,",");
    m_ucStartBit = atoi(pcToken);
    ucStartBit = m_ucStartBit + (m_ucWhichByte - 1) * 8;
    //get Data type
    pcToken = strtok(NULL,",");
    m_ucType = *pcToken;
    // get MAX
    pcToken = strtok(NULL,",");
    // now store as double until we parse till SIG_VALTYPE_
    m_MaxValue.ui64Value = _strtoui64(pcToken, NULL, 10);
    // get MIN value
    pcToken = strtok(NULL,",");
    //venkat - unsiged __int64 is enough to store any value
    m_MinValue.ui64Value = _strtoui64(pcToken, NULL, 10);
    // get DATA_FORMAT (intel or motorola)
    pcToken = strtok(NULL,",");
    m_ucDataFormat = *pcToken;

    if(m_ucDataFormat == '0')
    {
        unsigned int nByte = (m_ucLength/8) + ((m_ucLength % 8)?1:0);
        unsigned int nStartBit = (m_ucWhichByte - nByte) * 8;
        unsigned int nBitSize = m_ucLength - (8 * (nByte - 1))+ m_ucStartBit;

        if(nBitSize == 0)
        {
            ucStartBit = ucStartBit + m_ucLength;
        }
        else
        {
            ucStartBit = nStartBit + nBitSize-1;
        }
    }

    m_ucStartBit = ucStartBit;
    // next token - (SCALE_FACTOR,OFFSET)
    pcToken = strtok(NULL,",");
    m_fOffset = (float)atof(pcToken); // store scale factor
    // Get offset
    pcToken = strtok(NULL,",");
    m_fScaleFactor = (float)atof(pcToken); // store Offset
    // next token -- "UNIT", ""
    pcTemp = acTemp;
    pcToken = strtok(NULL,"");

    //pcToken++;
    // copy everything, but not including the last <">
    while(*pcToken && *pcToken != ',' && *pcToken != 0xa)
    {
        *pcTemp++ = *pcToken++;
    }

    *pcTemp='\0';
    string strUnit_Temp = acTemp;
    m_sUnit = "\"";
    m_sUnit = m_sUnit + acTemp; // copy UNIT to corresponding data member.
    m_sUnit = m_sUnit + "\"";

    if(strUnit_Temp.length() > defCON_MAX_MSGN_LEN + 2)
    {
        CConverter::fileLog << "unit " << strUnit_Temp;
        CConverter::fileLog << " changed to " << m_sUnit << endl;
        CConverter::bLOG_ENTERED = true;
    }

    //for multiplexing field
    string strTemp = pcToken;
    int nIndex = strTemp.find(',');

    if(nIndex != -1)
    {
        int nLength = strTemp.length();
        strTemp = strTemp.substr(nLength - nIndex - 1, nIndex);
        nIndex = strTemp.find(',');

        if(nIndex != -1)
        {
            if(nIndex != 0)
            {
                strTemp = strTemp.substr(0, nIndex-1);
            }
            else
            {
                strTemp.empty();
            }

            m_sMultiplex = strTemp;
        }
    }

    //rx'ing nodes
    string strNodes = pcToken;
    nIndex = strNodes.find(",", 0);
    strNodes = strNodes.substr(nIndex+1);
    nIndex = strNodes.find(",", nIndex);
    strNodes = strNodes.substr(nIndex+1);
    m_sNode = strNodes;

    while ((nIndex = m_sNode.find('\n', 0)) != string::npos)
    {
        m_sNode.replace(nIndex, 1, " ");
    }

    if(m_sNode.length() == 0)
    {
        m_sNode = "Vector__XXX";
    }

    return 1;
}

/**
 * \brief  Validates the signal
 * \return Content of m_uiError
 *
 * Validates the signal , sets the Max and Min value to the data type
 * and if any error sets the error variable accordingly.
 */
unsigned int CSignal::Validate()
{
    // data format mismatch with previous signal

    // if more than 64 bits should be discarded
    if(m_ucLength > 64)
    {
        return (m_uiError = SIG_EC_LENGTH_ERR);
    }

    // validate start bit and byte
    if(m_ucStartBit > 63)
    {
        return (m_uiError = SIG_EC_STARTBIT_ERR);
    }

    // errors eliminated now do necessary conversions
    if(m_ucLength == 1) // single bit - change type to bool
    {
        m_ucType = SIG_TYPE_BOOL;
    }

    // update MAX value and MIN value based on type
    switch(m_ucType)
    {
        case SIG_TYPE_BOOL:
        case SIG_TYPE_UINT:
        case SIG_TYPE_UINT64:
            unConvertRawToPhysical(m_MaxValue.ui64Value, m_MinValue.ui64Value, m_fOffset, m_fScaleFactor, m_MaxValue.dValue, m_MinValue.dValue);
            break;

        case SIG_TYPE_INT:
        case SIG_TYPE_INT64:
            nConvertRawToPhysical(m_MaxValue.ui64Value, m_MinValue.ui64Value, m_fOffset, m_fScaleFactor, m_MaxValue.dValue, m_MinValue.dValue);
            break;

        default:
            break;
    }

    // correct value descriptors according to type of signal
    list<CValueDescriptor>::iterator rValDesc;
    for(rValDesc=m_listValueDescriptor.begin(); rValDesc!=m_listValueDescriptor.end(); rValDesc++)
    {
        switch(m_ucType)
        {
            case SIG_TYPE_INT:
            case SIG_TYPE_FLOAT:
            case SIG_TYPE_DOUBLE:
            case SIG_TYPE_INT64:
                break;

            case SIG_TYPE_BOOL:
            case SIG_TYPE_UINT:
            case SIG_TYPE_UINT64:
                rValDesc->m_value.ui64Value = (unsigned long long int)rValDesc->m_value.i64Value;
                break;

            default:
                break;
        }
    }

    return (m_uiError = SIG_EC_NO_ERR);
}

/**
 * \brief     Writes the signal in the CANoe format
 * \param[in] fileOutput Output file
 * \return    Return code
 *
 * Writes the signal in the CANoe format and returns false
 * if any of the error signal is not stored in the file
 */
bool CSignal::WriteSignaltofile(fstream& fileOutput)
{
    bool bResult = true;

    if(m_uiError == SIG_EC_NO_ERR)
    {
        fileOutput << " SG_ " << m_sName;
        fileOutput << " " << m_sMultiplex;
        fileOutput << ": " << dec << m_ucStartBit;
        fileOutput << "|" << dec << m_ucLength;
        fileOutput << "@" << m_ucDataFormat;

        if ((m_ucType == SIG_TYPE_UINT) || (m_ucType == SIG_TYPE_UINT64))
        {
            fileOutput << "+";
        }
        else
        {
            fileOutput << "-";
        }

        fileOutput << " (" << m_fScaleFactor;
        fileOutput << "," << m_fOffset;
        fileOutput << ") [" << m_MinValue.dValue;
        fileOutput << "|" << m_MaxValue.dValue;
        fileOutput << "] " << m_sUnit;
        fileOutput << " " << m_sNode << endl;
    }
    else
    {
        bResult = false;
    }

    return bResult;
}

/**
 * \brief      Returns the error string of the signal
 * \param[out] str Error string depending on m_uiError
 *
 * Returns the error string of the signal.
 */
void CSignal::GetErrorString(string& str)
{
    switch(m_uiError)
    {
        case 0:
            str = "No error";
            break;

        case 1:
            str = "Invalid signal length";
            break;

        case 2:
            str = "Invalid start bit";
            break;
    }
}
