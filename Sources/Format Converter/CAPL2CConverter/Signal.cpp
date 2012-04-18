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
 * \brief     Implementation file for CSignal class
 * \authors   Amit Ranjan
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CSignal class
 */

/* Project includes */
#include "Signal.h"

/**
 * Construction
 */
CSignal::CSignal()
{
    m_acMultiplex = "";
    m_acName = "";
    m_ucLength = 1; // minimum signal length should be 1 bit
    m_ucWhichByte = 1; // ONE based index
    m_ucStartBit = 0; // ZERO based index
    m_ucType = SIG_TYPE_BOOL; // let default be bool type -- match with default length
    m_MaxValue.uiValue = 1; // use unsigned int field for bool also
    m_MinValue.uiValue = 0; //
    m_ucDataFormat = SIG_DF_INTEL;
    m_fOffset = 0.0f;
    m_fScaleFactor = 1.0f;
    m_acUnit = "";
    m_uiError = SIG_EC_NO_ERR;
    m_rxNode = "";
    m_listValueDescriptor.RemoveAll();
}

/**
 * Destruction
 */
CSignal::~CSignal()
{
    // clear the embedded value descriptor list before destroying the signal
    if(!m_listValueDescriptor.IsEmpty())
    {
        m_listValueDescriptor.RemoveAll();
    }
}

/**
 * Overload assignment operator. We should clear the existing value descriptor
 * list and copy contents of the argument's list to this signal.
 * The rest of the members are copied by value.
 */
CSignal& CSignal::operator=(CSignal& signal)
{
    if(!m_listValueDescriptor.IsEmpty())
    {
        m_listValueDescriptor.RemoveAll(); // clear the list first
    }

    // copy all the data members except the list
    m_acMultiplex = signal.m_acMultiplex;
    m_acName = signal.m_acName;
    m_ucLength = signal.m_ucLength;
    m_ucWhichByte = signal.m_ucWhichByte;
    m_ucStartBit = signal.m_ucStartBit;
    m_ucType = signal.m_ucType;
    m_MaxValue = signal.m_MaxValue;
    m_MinValue = signal.m_MinValue;
    m_ucDataFormat = signal.m_ucDataFormat;
    m_fOffset = signal.m_fOffset;
    m_fScaleFactor = signal.m_fScaleFactor;
    m_acUnit = signal.m_acUnit;
    m_uiError = signal.m_uiError;
    m_rxNode = signal.m_rxNode;
    // now copy the list
    m_listValueDescriptor.AddTail(&signal.m_listValueDescriptor);

    return (*this);
}

/**
 * Extract the signal info from the line and store it in the signal object
 * return an appropriate error code if something wrong with signalline
 */
int CSignal::Format(char *pcLine)
{
    char *pcToken;
    char acTemp[defVTAB_MAX_LINE_LEN],*pcTemp;
    pcTemp = acTemp;

    // get signal name
    // in older versions of CANoe it can be in any of this format
    // <SIG_NAME :> -- standard signal
    // <SIG_NAME M :> -- mode signal
    // <SIG_NAME mk :> -- mode dependent signal

    pcToken = strtok(pcLine,":"); // get upto colon
    // copy only signal name because we BUSMASTER does not support modes
    // skip leading spaces first
    while(*pcToken && *pcToken == ' ')
    {
        pcToken++;
    }
    // now get the signal name
    while(*pcToken && *pcToken != ' ')
    {
        *pcTemp++ = *pcToken++; // copy SIG_NAME only, i.e. till first 'space'
    }
    *pcTemp = '\0'; // terminate it

    m_acName = acTemp; // copy the name to the signal's data member
    pcTemp = acTemp; // reset pcTemp to start of buffer


    //leave blank space
    *pcToken++;
    //Find the signal's multiplexing details
    while(*pcToken && *pcToken != ' ')
    {
        *pcTemp++ = *pcToken++; // copy SIG_NAME only, i.e. till first 'space'
    }
    *pcTemp = '\0'; // terminate it
    m_acMultiplex = acTemp; // copy the name to the signal's data member

    pcTemp = acTemp; // reset pcTemp to start of buffer

    // next token (START_BIT|LENGTH@DATA_FORMAT(+/-))
    pcToken = strtok(NULL," :");
    // get start bit
    while(*pcToken && *pcToken != '|')
    {
        *pcTemp++ = *pcToken++;
    }
    *pcTemp='\0';
    pcToken++; // skip '|'

    // store the start byte and start bit information
    unsigned int uiStartBit = atoi(acTemp);
    // rajesh: 21-03-2003: begin:
    // based on INTEL/MOTOROLA - the start bit is represnted differently by CANoe.
    // With motorola format, bits are stored in reverse order. BUSMASTER
    // does not differentiate between INTEL and MOTOROLA at this stage and START_BYTE
    // and START_BIT are not depending on this. So process this after identifying the
    // format. Original code commented - moved down.
    /*
    m_ucWhichByte = uiStartBit / 8 + 1;
    m_ucStartBit = uiStartBit % 8;
    */
    // rajesh: 21-03-2003: end_1: continued below

    // get signal length
    pcTemp = acTemp;
    while(*pcToken && *pcToken != '@')
    {
        *pcTemp++ = *pcToken++;
    }
    *pcTemp='\0';
    pcToken++; // skip '@'

    m_ucLength = atoi(acTemp); // store signal length

    // get DATA_FORMAT (intel or motorola)
    m_ucDataFormat = *pcToken;

    // rajesh: 21-03-2003: begin_1: modification continued
    if(SIG_DF_MOTOROLA == m_ucDataFormat)
    {
        /*	if(m_ucLength>=8)
            {
                uiStartBit = ( 65 - ( (uiStartBit/8*8) + (8-uiStartBit%8) + m_ucLength));
            }
            else if(m_ucLength!=1)
            {
                uiStartBit = 63 - (uiStartBit + m_ucLength )+(uiStartBit+1)/8*8 ;
            }*/
        INT nTemp = 0;
        // If the signal is of type Motorola, its length and
        // endbit is indicated in the file. The end bit is represented in Intel format
        // itself.
        // Get the start bit w.r.t to bit numbering in Intel format but bits are in
        // Motorola format.
        nTemp =  8 * ( ( m_ucLength - 1 ) / 8 ) - ( ( m_ucLength - 1) % 8 );
        uiStartBit = uiStartBit + nTemp;
        // Swap the byte position to get it is intel format.
        nTemp = 8 * ( 8 - ( 2 * ( uiStartBit/8 ) + 1 ) );
        uiStartBit = uiStartBit + nTemp ;
    }
    m_ucWhichByte = uiStartBit / 8 + 1;
    m_ucStartBit = uiStartBit % 8;
    // rajesh: 21-03-2003:end:


    // get sign of signal. At this point we know only whether the
    // signal is signed or unsigned. Whether it is float or double
    // will be known later only when we process "SIG_VALTYPE_"

    if(m_ucLength >32)
    {
        m_ucType = (*(++pcToken) == '-') ? SIG_TYPE_INT64 : SIG_TYPE_UINT64;
    }
    else
    {
        m_ucType = (*(++pcToken) == '-') ? SIG_TYPE_INT : SIG_TYPE_UINT;
    }

    // next token - (SCALE_FACTOR,OFFSET)
    pcToken = strtok(NULL," (");
    pcTemp = acTemp;
    // get scale factor
    while(*pcToken && *pcToken != ',')
    {
        *pcTemp++ = *pcToken++;
    }
    *pcTemp='\0';
    pcToken++; // skip ','

    m_fScaleFactor = (float)atof(acTemp); // store scale factor

    // Get offset
    pcTemp = acTemp;
    while(*pcToken && *pcToken != ')')
    {
        *pcTemp++ = *pcToken++;
    }
    *pcTemp='\0';

    m_fOffset = (float)atof(acTemp); // store Offset


    // next token [MIN|MAX]
    pcToken = strtok(NULL," [");

    // get MIN
    pcTemp = acTemp;
    while(*pcToken && *pcToken != '|')
    {
        *pcTemp++ = *pcToken++;
    }
    *pcTemp='\0';
    pcToken++; // skip '|'

    // now store as double until we parse till SIG_VALTYPE_
    if(m_ucLength <= 32)
        m_MinValue.dValue = atof(acTemp); // store MIN value
    else
        m_MinValue.i64Value = _atoi64(acTemp);

    // get MAX value
    pcTemp = acTemp;
    while(*pcToken && *pcToken != ']')
    {
        *pcTemp++ = *pcToken++;
    }
    *pcTemp='\0';
    pcToken++; // skip ']'
    if(m_ucLength <= 32)
        m_MaxValue.dValue = atof(acTemp); // store MAX value
    else
        m_MaxValue.i64Value = _atoi64(acTemp);

    // next token -- "UNIT", ""
    pcTemp = acTemp;
    pcToken++;

    // skip first <">
    while(*pcToken && *pcToken != '\"')
    {
        pcToken++;
    }
    pcToken++;

    // copy everything, but not including the last <">
    while(*pcToken && *pcToken != '\"')
    {
        *pcTemp++ = *pcToken++;
    }
    *pcTemp='\0';
    m_acUnit = acTemp; // copy UNIT to corresponding data member.
    pcToken++;

    while(*pcToken && *pcToken != ' ')
    {
        pcToken++;
    }
    pcToken++;
    pcTemp = acTemp;
    while(*pcToken && *pcToken != '\n')
    {
        *pcTemp++ = *pcToken++;
    }
    *pcTemp='\0';
    if(strcmp(acTemp,"Vector__XXX") != 0)
    {
        m_rxNode = acTemp;
    }
    else
        m_rxNode = "";
    return 1;
}

/**
 * Extracts Values and value descriptors from the line and
 * stores them to the Signal
 */
int CSignal::AddValueDescriptors(char *pcLine)
{
    char acValue[100];
    char acDesc[100];
    char *pcValue = acValue;
    char *pcDesc = acDesc;
    // skip leading spaces
    while(*pcLine && *pcLine == ' ')
    {
        *pcLine++;
    }
    while(*pcLine && *pcLine != ';')
    {
        pcValue = acValue;
        pcDesc = acDesc;

        *pcValue = *pcDesc = '\0';
        while(*pcLine && *pcLine != ' ')
        {
            *pcValue++ = *pcLine++; // copy all but terminating space
        }
        *pcValue = '\0'; // terminate the string

        // skip leading spaces
        while(*pcLine && *pcLine == ' ')
        {
            *pcLine++;
        }

        // skip leading '"'
        while(*pcLine && *pcLine == '\"')
        {
            *pcLine++;
        }
        while(*pcLine && *pcLine != '\"')
        {
            *pcDesc++ = *pcLine;
            pcLine++;
        }
        *pcDesc = '\0';
        // skip trailing '"'.
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
            valDesc.m_value.dValue = atof(acValue);
            valDesc.m_acDescriptor = acDesc;

            m_listValueDescriptor.AddTail(valDesc);
        }
    }

    return 1;
}

/**
 * \brief validate for conformance to BUSMASTER DB format
 * \param[in] ucFormat Data format of previous signal
 *
 * Convert single bit INT and UINT to BOOL
 * if MAX_value == MIN_value update MAX_value to MAX_default
 * else if MAX_value == MIN_value == MAX_default, update MIN_value to MIN_default
 * signals with length more than 32 bits shall be discarded
 *
 * The data format of previous signal is passed as a parameter
 * For the first one it will be zero, for then on either INTEL or MOTOROLA
 * If this signals format is not matching with previous signal then this signal to be
 * discarded.
 * SIG_EC_NO_ERR, SIG_EC_DATA_FORMAT_ERR,SIG_EC_LENGTH_ERR,SIG_EC_STARTBIT_ERR,SIG_EC_TYPE_ERR
 */
unsigned int CSignal::Validate(unsigned char ucFormat)
{
    // data format mismatch with previous signal
    if(ucFormat != 0 && ucFormat != m_ucDataFormat)
    {
        return (m_uiError = SIG_EC_DATA_FORMAT_ERR);
    }

    // if more than 64 bits should be discarded
    if(m_ucLength > 64)
    {
        return (m_uiError = SIG_EC_LENGTH_ERR);
    }

    // validate start bit and byte
    if(m_ucWhichByte < 1 || m_ucWhichByte > 8)
    {
        return (m_uiError = SIG_EC_STARTBIT_ERR);
    }

    if(m_ucWhichByte == 8 && m_ucStartBit > 7)
    {
        return (m_uiError = SIG_EC_STARTBIT_ERR);
    }

    // DOUBLE & FLOAT are not supported right now
    if(m_ucType == SIG_TYPE_DOUBLE || m_ucType == SIG_TYPE_FLOAT)
    {
        return (m_uiError = SIG_EC_TYPE_ERR);
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
            m_MaxValue.uiValue = (unsigned int)m_MaxValue.dValue;
            m_MinValue.uiValue = (unsigned int)m_MinValue.dValue;
            unsigned int uiDefault;
            uiDefault = (1 << m_ucLength) - 1;
            if(m_MaxValue.uiValue == m_MinValue.uiValue)
            {
                if(m_MaxValue.uiValue <= uiDefault && m_MaxValue.uiValue >0)
                {
                    m_MinValue.uiValue = 0;
                }
                else
                {
                    m_MaxValue.uiValue = uiDefault;
                    m_MinValue.uiValue = 0;
                }
            }
            else
            {
                if(m_MaxValue.uiValue > uiDefault)
                {
                    m_MaxValue.uiValue = uiDefault;
                }
                if(m_MinValue.uiValue >m_MaxValue.uiValue )
                {
                    m_MinValue.uiValue = 0;
                }
            }
            break;

        case SIG_TYPE_INT:
            m_MaxValue.iValue = (int)m_MaxValue.dValue;
            m_MinValue.iValue = (int)m_MinValue.dValue;
            int iDefault;
            iDefault = (1 << (m_ucLength - 1)) - 1;
            if(m_MaxValue.iValue == m_MinValue.iValue)
            {
                if(m_MaxValue.iValue <= iDefault)
                {
                    m_MinValue.iValue = -1*iDefault - 1;
                }
                else
                {
                    m_MaxValue.iValue = iDefault;
                    m_MinValue.iValue = -1*iDefault - 1;
                }
            }
            else
            {
                if(m_MaxValue.iValue > iDefault)
                {
                    m_MaxValue.iValue = iDefault;
                }
                if(m_MinValue.iValue >m_MaxValue.iValue )
                {
                    m_MinValue.iValue = -1*iDefault - 1;
                }
            }
            break;

            // float is not supported right now, so no validation
        case SIG_TYPE_FLOAT:
            m_MaxValue.fValue = (float)m_MaxValue.dValue;
            m_MinValue.fValue = (float)m_MinValue.dValue;
            break;

            // double is not supported right now, so no validation
        case SIG_TYPE_DOUBLE:
            // no need to update MAX and MIN values, already stored as double
            break;

        case SIG_TYPE_INT64:
            m_MaxValue.i64Value = (LONGLONG)m_MaxValue.dValue;
            m_MinValue.i64Value = (LONGLONG)m_MinValue.dValue;
            UINT unPower;
            __int64 i64Default;
            UINT i;
            if(m_ucLength == 64 )
            {
                unPower = m_ucLength - 1;
            }
            else
            {
                unPower = m_ucLength;
            }
            i64Default = 1;
            for(i = 0; i<unPower; i++)
            {
                i64Default = (__int64)( 2*i64Default);
            }
            if(m_MaxValue.ui64Value == m_MinValue.ui64Value)
            {
                if(m_MaxValue.i64Value <= i64Default && m_MaxValue.i64Value >0)
                {
                    m_MinValue.i64Value =  -(i64Default+1);
                }
                else
                {
                    m_MaxValue.i64Value = i64Default-1;
                    m_MinValue.i64Value =  -(i64Default+1);
                }
            }
            else
            {
                if(m_MaxValue.i64Value > i64Default - 1)
                {
                    m_MaxValue.i64Value = i64Default - 1;
                }
                if(m_MinValue.i64Value > m_MaxValue.i64Value )
                {
                    m_MinValue.i64Value = -(i64Default + 1);
                }
            }
            break;

        case SIG_TYPE_UINT64:
            m_MaxValue.ui64Value = (ULONGLONG)m_MaxValue.dValue;
            m_MinValue.ui64Value = (ULONGLONG)m_MinValue.dValue;
            unsigned __int64 ui64Default;
            if(m_ucLength == 64 )
            {
                unPower = m_ucLength - 1;
            }
            else
            {
                unPower = m_ucLength;
            }
            ui64Default = 1;
            for(i = 0; i<unPower; i++)
            {
                ui64Default = (unsigned __int64)( 2*ui64Default);
            }
            if(m_MaxValue.ui64Value == m_MinValue.ui64Value)
            {
                if(m_MaxValue.ui64Value <= ui64Default - 1 && m_MaxValue.ui64Value > 0)
                {
                    m_MinValue.ui64Value = 0;
                }
                else
                {
                    m_MaxValue.ui64Value = ui64Default - 1;
                    m_MinValue.ui64Value = 0;
                }
            }
            else
            {
                if(m_MaxValue.ui64Value > ui64Default - 1)
                {
                    m_MaxValue.ui64Value = ui64Default-1;
                }
                if(m_MinValue.ui64Value >m_MaxValue.ui64Value )
                {
                    m_MinValue.ui64Value = 0;
                }
            }
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
            case SIG_TYPE_BOOL:
            case SIG_TYPE_UINT:
                rValDesc.m_value.uiValue = (unsigned int)rValDesc.m_value.dValue;
                break;

            case SIG_TYPE_INT:
                rValDesc.m_value.iValue = (int)rValDesc.m_value.dValue;
                break;

            case SIG_TYPE_FLOAT:
                rValDesc.m_value.fValue = (float)rValDesc.m_value.dValue;
                break;

            case SIG_TYPE_DOUBLE:
                break;

            case SIG_TYPE_INT64:
                rValDesc.m_value.i64Value = (LONGLONG)rValDesc.m_value.dValue;
                break;

            case SIG_TYPE_UINT64:
                rValDesc.m_value.ui64Value = (ULONGLONG)rValDesc.m_value.dValue;
                break;

            default:
                break;
        }
    }

    return (m_uiError = SIG_EC_NO_ERR);
}

void CSignal::GetErrorString(string &str)
{
	switch(m_uiError) {
		case SIG_EC_NO_ERR:
			str = "No error";
			break;
		case SIG_EC_DATA_FORMAT_ERR:
			str = "Data format mismatch";
			break;
		case SIG_EC_LENGTH_ERR:
			str = "Invalid signal length";
			break;
		case SIG_EC_STARTBIT_ERR:
			str = "Invalid start bit";
			break;
		case SIG_EC_TYPE_ERR:
			str = "Invalid signal type";
			break;
		case SIG_EC_OVERLAP:
			str = "Overlapping signal";
			break;
		default:
			str = "Unknown";
			break;
	}
}
