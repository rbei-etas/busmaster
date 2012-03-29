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
 * \author    RBIN/EBS1 - Mahesh.B.S
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the CSignal class.
 */
/**
* \file       Signal.cpp
* \brief      implementation of the CSignal class.
* \authors    Mahesh.B.S
* \date       15.11.2004 Created
* \copyright  Copyright &copy; 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
*/

#include "StdAfx.h"
#include "Signal.h"
#include "Converter.h"
#include "Utility.h"

/**
* \brief      Constructor of CSignal
* \param[in]  None
* \param[out] None
* \return     None
* \authors    Mahesh.B.S
* \date       15.11.2004
*/
CSignal::CSignal()
{

}

/**
* \brief      destructor of CSignal
* \param[in]  None
* \param[out] None
* \return     None
* \authors    Mahesh.B.S
* \date       15.11.2004
*/
CSignal::~CSignal()
{

}

/**
* \brief      overloaded operator =
* \param[in]  None
* \param[out] None
* \return     None
* \authors    Mahesh.B.S
* \date       15.11.2004
*/
CSignal& CSignal::operator=(CSignal& signal)
{
    if(!m_listValueDescriptor.IsEmpty())
    {
        m_listValueDescriptor.RemoveAll(); // clear the list first
    }

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
    m_listValueDescriptor.AddTail(&signal.m_listValueDescriptor);
    return (*this);
}

/**
* \brief      operator==
* \param[in]  const CSignal &Signal
* \param[out] None
* \return     int
* \authors    Mahesh.B.S
* \date       15.11.2004
*/
int CSignal::operator==(const CSignal &Signal) const
{
    if (m_sName == Signal.m_sName)
        return 1;
    else return 0;
}

/**
* \brief      Extracts the signal data from the given Line and populates
              message structure.
* \param[in]  char *pcLine
* \param[out] None
* \return     int
* \authors    Mahesh.B.S
* \date       15.11.2004
*/
int CSignal::Format(char *pcLine)
{
    char *pcToken;
    char acTemp[defCON_MAX_TOKN_LEN],*pcTemp;
    pcTemp = acTemp;

    // get signal name
    pcToken = strtok(pcLine,","); // get upto colon

    // now get the signal name
    m_sName = pcToken; // copy the name to the signal's data member

    if(m_sName.GetLength() > defCON_MAX_MSGN_LEN)
        Truncate_str("Signal name",m_sName,true);

    // Signal length
    pcToken = strtok(NULL,",");
    m_ucLength = atoi(pcToken); // store signal length

    // get which byte
    pcToken = strtok(NULL,",");
    m_ucWhichByte = atoi(pcToken);

    // get start bit
    UCHAR ucStartBit;
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
        UINT unByteIndex = m_ucWhichByte - 1;
        UINT nByte = (m_ucLength/8) + ((m_ucLength % 8)?1:0);
        UINT nStartBit = (m_ucWhichByte - nByte) * 8;
        UINT nBitSize = m_ucLength - (8 * (nByte - 1))+ m_ucStartBit;
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
    CString strUnit_Temp = acTemp;
    m_sUnit = "\"";
    m_sUnit = m_sUnit + acTemp; // copy UNIT to corresponding data member.
    m_sUnit = m_sUnit.Left(defCON_MAX_MSGN_LEN + 1);
    m_sUnit = m_sUnit + "\"";
    if(strUnit_Temp.GetLength() > defCON_MAX_MSGN_LEN + 2)
    {
        char logmsg[defCON_MAX_LINE_LEN];
        sprintf(logmsg,"unit %s changed to %s\n",strUnit_Temp,m_sUnit);
        CConverter::fileLog.WriteString(logmsg);
        CConverter::bLOG_ENTERED = true;
    }
    //for multiplexing field
    CString strTemp = pcToken;
    int nIndex = strTemp.Find(',');
    if(nIndex != -1)
    {
        int nLength = strTemp.GetLength();
        strTemp = strTemp.Right(nLength - nIndex -1);
        nIndex = strTemp.Find(',');
        if(nIndex != -1)
        {
            if(nIndex != 0)
            {
                strTemp = strTemp.Left(nIndex);
            }
            else
            {
                strTemp.Empty();
            }
            m_sMultiplex = strTemp;
        }
    }


    //rx'ing nodes
    pcToken = strtok(pcToken,",");
    pcToken = strtok(NULL,",");
    pcToken = strtok(NULL,"\n");
    if(pcToken)
        m_sNode = pcToken;
    else
        m_sNode = "Vector__XXX";

    return 1;
}


/**
* \brief      Validates the signal , sets the Max and Min value to the data type
              and if any error sets the error variable accordingly.
* \param[in]  None
* \param[out] None
* \return     unsigned int
* \authors    Mahesh.B.S
* \date       15.11.2004
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
    POSITION posValDesc = m_listValueDescriptor.GetHeadPosition();
    while(posValDesc != NULL)
    {
        CValueDescriptor& rValDesc = m_listValueDescriptor.GetNext(posValDesc);
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
                rValDesc.m_value.ui64Value = (ULONGLONG)rValDesc.m_value.i64Value;
                break;

            default:
                break;
        }
    }

    return (m_uiError = SIG_EC_NO_ERR);
}

/**
* \brief      Write's the signal in the CANoe format and returns false
              if any of the error signal is not stored in the file
* \param[in]  CStdioFile &fileOutput
* \param[out] None
* \return     bool
* \authors    Mahesh.B.S
* \date       15.11.2004
*/
bool CSignal::WriteSignaltofile(CStdioFile &fileOutput)
{
    char acLine[defCON_MAX_LINE_LEN]; // I don't expect one line to be more than this
    bool bResult = true;
    if(m_uiError == SIG_EC_NO_ERR)
    {
        sprintf(acLine," SG_ %s %s:  %u|%u@%c+ (%f,%f) [%lf|%lf] %s %s\n",
                m_sName, m_sMultiplex, m_ucStartBit, m_ucLength, m_ucDataFormat,
                m_fScaleFactor, m_fOffset, m_MinValue.dValue,
                m_MaxValue.dValue, m_sUnit, m_sNode);
        fileOutput.WriteString(acLine);
    }
    else
        bResult = false;
    return bResult;
}


const char* CSignal::m_pacErrorStrings[] =
{
    "No error",
    "ERROR:Invalid signal length",
    "ERROR:Invalid start bit"
};

/**
* \brief      Returns the error string of the signal
* \param[in]  None
* \param[out] None
* \return     const char*
* \authors    Mahesh.B.S
* \date       15.11.2004
*/
const char* CSignal::GetErrorString()
{
    return m_pacErrorStrings[m_uiError];
}
