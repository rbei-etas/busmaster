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
 * \brief     Implementation of signal class
 * \authors   Amitesh Bharti, Pemmaiah BD, Mahesh.B.S, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the signal class.
 */

#include "Converter.h"
#include "Signal.h"
#include "Tag.h"
#include "Utility.h"
#include <algorithm>


/**
 * \brief Constructor
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
    m_listValueDescriptor.clear();
}


/**
 * \brief Destructor
 */
CSignal::~CSignal()
{
    // clear the embedded value descriptor list before destroying the signal
    m_listValueDescriptor.clear();
}


/**
 * \brief overloaded operator =
 *
 * Overload assignment operator. We should clear the existing value descriptor
 * list and copy contents of the argument's list to this signal. The rest of the
 * members are copied by value
 */
CSignal& CSignal::operator=(CSignal& signal)
{
    m_listValueDescriptor.clear(); // clear the list first
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
    m_rxNode = signal.m_rxNode;
    m_uiError = signal.m_uiError;
    // now copy the list
    m_listValueDescriptor = signal.m_listValueDescriptor;
    return (*this);
}


/**
 * \brief Extracts the message data from the given Line
 *
 * Extracts the message data from the given Line and populates
 * the message structure.
 */
int CSignal::Format(char *pcLine)
{
    char *pcToken;
    char acTemp[defVTAB_MAX_LINE_LEN],*pcTemp,*pcTok;
    pcTemp = acTemp;

    // get signal name
    // in older versions of CANoe it can be in any of this format
    // <SIG_NAME :> -- standard signal
    // <SIG_NAME M :> -- mode signal
    // <SIG_NAME mk :> -- mode dependent signal

    pcToken = strtok_s(pcLine, ":", &pcTok); // get upto colon
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

    // next token (START_BIT|LENGTH@DATA_FROMAT(+/-))
    pcToken = strtok_s(NULL, " :", &pcTok);


    // get start bit
    while(*pcToken && *pcToken != '|')
    {
        *pcTemp++ = *pcToken++;
    }
    *pcTemp='\0';
    pcToken++; // skip '|'

    // store the start byte and start bit information
    int uiStartBit = atoi(acTemp);
    int uiStartBitX = uiStartBit;
    // rajesh: 21-03-2003: begin:
    // based on INTEL/MOTOROLA - the start bit is represnted differently by CANoe.
    // With motorola format, bits are stored in reverse order. BUSMASTER
    // does not differentiate between INTEL and MOTOROLA at this stage and START_BYTE
    // and START_BIT are not depending on this. So process this after identifying the
    // format. Original code commented - moved down.
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

    if(SIG_DF_MOTOROLA == m_ucDataFormat)
    {
        //The following is done because if signal type is of MOTOROLA
        //THe MSB is stored and will be in Big Endian format.
        //Get the LSB

        /*In CANoe if we view the bits as a 8x8 Matrix, for Motorala signals
        the numbering will be in the reverse order with in the each row. So to get the
        proper MSB we need to again swap the bit position with in the row*/
        //i.e here we need to get the left half of the bits to the right and right
        //to the left with in the same row
        int ntemp;
        if(uiStartBit == 0)
            ntemp = 0;
        else
            ntemp = uiStartBit % 4;//get the bit position with its the row.
        //swap left bits to the right and right bits to the left.
        if(uiStartBit % 8 <= 3)
            uiStartBit = uiStartBit + 7 - ntemp * 2;
        else
            uiStartBit = uiStartBit - 1 - ntemp * 2;

        uiStartBit = uiStartBit + m_ucLength-1;
        //Invert the bit position (Converting to little endian format)
        uiStartBitX = (CConverter::ucMsg_DLC*8)-1 - uiStartBit;

    }
    m_ucWhichByte = uiStartBit / 8 + 1;
    m_ucStartBit = uiStartBitX % 8;


    // get sign of signal. At this point we know only whether the
    // signal is signed or unsigned. Whether it is float or double
    // will be known later only when we process "SIG_VALTYPE_"
    if(m_ucLength > 32)
    {
        m_ucType = (*(++pcToken) == '-') ? SIG_TYPE_INT64 : SIG_TYPE_UINT64;
    }
    else
    {
        m_ucType = (*(++pcToken) == '-') ? SIG_TYPE_INT : SIG_TYPE_UINT;
    }

    // next token - (SCALE_FACTOR,OFFSET)
    pcToken = strtok_s(NULL, " (", &pcTok);
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
    pcToken = strtok_s(NULL, " [", &pcTok);

    // get MIN
    pcTemp = acTemp;
    while(*pcToken && *pcToken != '|')
    {
        *pcTemp++ = *pcToken++;
    }
    *pcTemp='\0';
    pcToken++; // skip '|'

    //venkat
    m_MinValue.dValue = atof(acTemp); // store MIN


    // get MAX value
    pcTemp = acTemp;
    while(*pcToken && *pcToken != ']')
    {
        *pcTemp++ = *pcToken++;
    }
    *pcTemp='\0';
    pcToken++; // skip ']'

    //venkat
    m_MaxValue.dValue = atof(acTemp); // store MAX value
    // next token -- "UNIT", ""
    pcTemp = acTemp;
    pcToken++;

    // go to '"'
    while(*pcToken && *pcToken != '\"')
    {
        pcToken++;
    }
    pcToken++; // skip '"'

    // copy everything, but not including the last <">
    while(*pcToken && *pcToken != '\"')
    {
        *pcTemp++ = *pcToken++;
    }
    pcToken++; // skip '"'
    *pcTemp='\0';
    m_acUnit = acTemp; // copy UNIT to corresponding data member.
    replace(m_acUnit.begin(), m_acUnit.end(), ',', '_'); //" not supported

    // skip ' '
    while(*pcToken && *pcToken == ' ')
    {
        pcToken++;
    }

    pcTemp = acTemp;
    while(*pcToken)
    {
        if ((*pcToken != '\r') && (*pcToken != ' '))
            *pcTemp++ = *pcToken++;
        else
            pcToken++;
    }
    *pcTemp='\0';

    // copy rx nodes
    if(strcmp(acTemp, "Vector__XXX") != 0)
    {
        m_rxNode = acTemp;
    }
    else
        m_rxNode = "";
    return 1;
}


/*
 * \brief Extracts the value descriptor data from the given Line
 *
 * Extracts the value descriptor data from the given Line and adds
 * to the signal.
 */
int CSignal::AddValueDescriptors(char *pcLine, fstream &fileInput)
{
    char acValue[300];
    char acDesc[300];
    char acLine[defVTAB_MAX_LINE_LEN] = {'\0'};
    char pcTemp[defVTAB_MAX_TEMP_LEN];
    char *pcToken, *pcTok;
    bool true_end = true;

    char *pcValue = acValue;
    char *pcDesc = acDesc;
    if(pcLine[strlen(pcLine)-1] != ';')
    {
        true_end = false;
    }
    // skip leading spaces
    while(*pcLine && (*pcLine == ' '))
    {
        *pcLine++;
    }
    while(*pcLine && (*pcLine != ';'))
    {
        pcTemp[0] = '\0';
        if(true_end == false)
        {
            fileInput.getline(acLine, 1026);
            strncpy(pcTemp, pcLine, sizeof(pcTemp));
            strncat(pcTemp, acLine, sizeof(pcTemp));
            pcLine = pcTemp;

            if(pcLine[strlen(pcLine)-1] == ';')
                true_end  = true;
        }
        pcDesc = acDesc;
        while(*pcLine && (*pcLine == ' '))
            pcLine++;
        pcToken = strtok_s(pcLine, " ", &pcTok);
        strncpy(acValue, pcToken, sizeof(acValue));
        pcLine = pcLine + (strlen(pcToken)) + 1;

        if(*pcLine == '\"')
        {
            pcLine++;
            if(*pcLine != '\"')
            {
                *pcDesc++ = *pcLine++;
                while(*pcLine && (*pcLine != '\"'))
                {
                    if (*pcLine != '\r')
                        *pcDesc++ = *pcLine;
                    pcLine++;
                }
            }
            *pcDesc = '\0';
        }
        pcLine++;
        while(*pcLine == ' ')
            pcLine++;
        CValueDescriptor valDesc;
        if(this->m_ucLength <= 32)
            valDesc.m_value.dValue = atof(acValue);
        else
            valDesc.m_value.i64Value = _atoi64(acValue);
        valDesc.m_acDescriptor = acDesc;
        m_listValueDescriptor.push_back(valDesc);
        pcDesc = acDesc;
    }
    return 1;
}


/**
 * \brief  Validate for conformance to BUSMASTER DB format
 * \return SIG_EC_NO_ERR, SIG_EC_DATA_FORMAT_ERR, SIG_EC_LENGTH_ERR, SIG_EC_STARTBIT_ERR, SIG_EC_TYPE_ERR
 *
 * Convert single bit INT and UINT to BOOL.
 * If MAX_value == MIN_value update MAX_value to MAX_default
 * else if MAX_value == MIN_value == MAX_default, update MIN_value to MIN_default
 * signals with length more than 32 bits shall be discarded.
 *
 * The data format of previous signal is passed as a parameter.
 * For the first one it will be zero, for then on either INTEL or MOTOROLA.
 * If this signals format is not matching with previous signal then this signal to be discarded.
 */
unsigned int CSignal::Validate(unsigned char ucFormat)
{
    // data format mismatch with previous signal
    /*if(ucFormat != 0 && ucFormat != m_ucDataFormat)
    {
    return (m_uiError = SIG_EC_DATA_FORMAT_ERR);
    }*/

    // if more than 64 bits should be discarded
    if(m_ucLength > 64)
    {
        return (m_uiError = SIG_EC_LENGTH_ERR);
    }

    // validate start bit and byte
    if((m_ucWhichByte < 1) || (m_ucWhichByte > 8))
    {
        return (m_uiError = SIG_EC_STARTBIT_ERR);
    }

    if((m_ucWhichByte == 8) && (m_ucStartBit > 7))
    {
        return (m_uiError = SIG_EC_STARTBIT_ERR);
    }

    // DOUBLE & FLOAT are not supported right now
    if((m_ucType == SIG_TYPE_DOUBLE) || (m_ucType == SIG_TYPE_FLOAT))
    {
        return (m_uiError = SIG_EC_TYPE_ERR);
    }

    // errors eliminated now do necessary conversions
    if(m_ucLength == 1) // single bit - change type to bool
    {
        m_ucType = SIG_TYPE_BOOL;
    }


    m_uiError = SIG_EC_NO_ERR;

    //Conversion from the physical to raw value required.


    // update MAX value and MIN value based on type
    switch(m_ucType)
    {
        case SIG_TYPE_BOOL:
        case SIG_TYPE_UINT:
        {
            unsigned long long int unMaxVal;
            unsigned long long int unMinVal;
            unConvertPhysicalToRaw(m_MaxValue.dValue, m_MinValue.dValue, m_fOffset, m_fScaleFactor, unMaxVal, unMinVal);
            m_MaxValue.uiValue = (unsigned int)unMaxVal;             //m_MaxValue.dValue;
            m_MinValue.uiValue = (unsigned int)unMinVal;             //m_MinValue.dValue;
            unsigned int uiDefault;
            uiDefault = (unsigned int)((1 << m_ucLength) - 1);
            //if both max and min value are equal set it to default
            if(m_MaxValue.uiValue == 0 && m_MinValue.uiValue == 0)
            {
                m_MaxValue.uiValue = uiDefault;
                m_MinValue.uiValue = 0;
                m_uiError = SIG_EC_OVERFLOW;
            }
            //if Max value out of range reset it to maximum possible value
            if((m_MaxValue.uiValue > uiDefault) || (m_MaxValue.uiValue < 0))
            {
                m_MaxValue.uiValue = uiDefault;
                m_uiError = SIG_EC_OVERFLOW;
            }
            //if Min value out of range reset it to minimum possible value
            if((m_MinValue.uiValue < 0) || (m_MinValue.uiValue > uiDefault))
            {
                m_MinValue.uiValue = 0;
                m_uiError = SIG_EC_OVERFLOW;
            }
            //if Max and Min are equal reset min.
            if(m_MaxValue.uiValue == m_MinValue.uiValue)
            {
                if(m_MaxValue.uiValue == 0)
                {
                    //This is because after setting min value to 0 than if max value is also 0
                    //we need to set max value to maximum
                    m_MaxValue.uiValue = uiDefault;
                }

                m_MinValue.uiValue = 0;
                m_uiError = SIG_EC_OVERFLOW;
            }
        }
        break;

        case SIG_TYPE_INT:
        {
            long long int nMaxVal;
            long long int nMinVal;
            nConvertPhysicalToRaw(m_MaxValue.dValue, m_MinValue.dValue, m_fOffset, m_fScaleFactor, nMaxVal, nMinVal);
            m_MaxValue.iValue = (int)nMaxVal;             //m_MaxValue.dValue;
            m_MinValue.iValue = (int)nMinVal;             //m_MinValue.dValue;
            int iDefault;
            iDefault = (int)(unsigned int)1 << (m_ucLength - 1);
            iDefault -= 1;
            //if both max and min value are equal set it to default
            if(m_MaxValue.iValue == 0 && m_MinValue.iValue == 0)
            {
                m_MaxValue.iValue = iDefault;
                m_MinValue.iValue = 0;
                m_uiError = SIG_EC_OVERFLOW;
            }
            //if Max value out of range reset it to maximum possible value
            if(m_MaxValue.iValue > iDefault || m_MaxValue.iValue < (-iDefault-1))
            {
                m_MaxValue.iValue = iDefault;
                m_uiError = SIG_EC_OVERFLOW;
            }
            //if Min value out of range reset it to minimum possible value
            if(m_MinValue.iValue < (-iDefault-1) || m_MinValue.iValue > iDefault )
            {
                m_MinValue.iValue = (-iDefault-1);
                m_uiError = SIG_EC_OVERFLOW;
            }
            //if Max and Min are equal reset min.
            if(m_MaxValue.iValue == m_MinValue.iValue)
            {
                m_MinValue.iValue = (-iDefault-1);
                m_uiError = SIG_EC_OVERFLOW;
            }
            break;
        }

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
        {
            //Testing required - venkat
            // no need to update MAX and MIN values, already stored as i64Value
            long long int nMaxVal;
            long long int nMinVal;
            nConvertPhysicalToRaw(m_MaxValue.dValue, m_MinValue.dValue, m_fOffset, m_fScaleFactor, nMaxVal, nMinVal);
            m_MaxValue.i64Value = nMaxVal;             //m_MaxValue.dValue;
            m_MinValue.i64Value = nMinVal;             //m_MinValue.dValue;
            unsigned int unPower;
            __int64 i64Default;
            unsigned int i;
            /*if(m_ucLength == 64 )
            {
            unPower = m_ucLength - 2;
            }
            else
            {
            unPower = m_ucLength - 2;
            }*/
            unPower = m_ucLength - 1;
            i64Default = 1;
            for(i = 0; i<unPower; i++)
            {
                i64Default = (__int64)( 2*i64Default);
            }
            i64Default -= 1;
            if(m_MaxValue.i64Value == 0 && m_MinValue.i64Value == 0)
            {
                m_MaxValue.i64Value = i64Default;
                m_MinValue.i64Value = 0;
                m_uiError = SIG_EC_OVERFLOW;
            }
            //if Max value out of range reset it to maximum possible value
            if(m_MaxValue.i64Value > i64Default || m_MaxValue.i64Value < (-i64Default-1))
            {
                m_MaxValue.i64Value = i64Default;
                m_uiError = SIG_EC_OVERFLOW;
            }
            //if Min value out of range reset it to minimum possible value
            if(m_MinValue.i64Value < (-i64Default-1) || m_MinValue.i64Value > i64Default )
            {
                m_MinValue.i64Value = (-i64Default-1);
                m_uiError = SIG_EC_OVERFLOW;
            }
            //if Max and Min are equal reset min.
            if(m_MaxValue.i64Value == m_MinValue.i64Value)
            {
                m_MinValue.i64Value = (-i64Default-1);
                m_uiError = SIG_EC_OVERFLOW;
            }
        }
        break;

        case SIG_TYPE_UINT64:
        {
            unsigned long long int unMaxVal;
            unsigned long long int unMinVal;
            unConvertPhysicalToRaw(m_MaxValue.dValue, m_MinValue.dValue, m_fOffset, m_fScaleFactor, unMaxVal, unMinVal);
            m_MaxValue.ui64Value = unMaxVal;             //m_MaxValue.dValue;
            m_MinValue.ui64Value = unMinVal;             //m_MinValue.dValue;
            unsigned __int64 ui64Default;
            /*if(m_ucLength == 64 )
            {
            unPower = m_ucLength - 1;
            }
            else
            {
            unPower = m_ucLength - 1;
            }*/
            unsigned int unPower = m_ucLength - 1;
            ui64Default = 1;
            for(unsigned int i = 0; i < unPower; i++)
            {
                ui64Default = (unsigned __int64)( 2*ui64Default);
            }
            ui64Default -= 1;
            //if both max and min value are equal set it to default
            if(m_MaxValue.ui64Value == 0 && m_MinValue.ui64Value == 0)
            {
                m_MaxValue.ui64Value = ui64Default;
                m_MinValue.ui64Value = 0;
                m_uiError = SIG_EC_OVERFLOW;
            }
            //if Max value out of range reset it to maximum possible value
            if(m_MaxValue.ui64Value > ui64Default || m_MaxValue.ui64Value < 0)
            {
                m_MaxValue.ui64Value = ui64Default;
                m_uiError = SIG_EC_OVERFLOW;
            }
            //if Min value out of range reset it to minimum possible value
            if(m_MinValue.ui64Value < 0 || m_MinValue.ui64Value > ui64Default )
            {
                m_MinValue.ui64Value = 0;
                m_uiError = SIG_EC_OVERFLOW;
            }
            //if Max and Min are equal reset min.
            if(m_MaxValue.ui64Value == m_MinValue.ui64Value)
            {
                m_MinValue.ui64Value = 0;
                m_uiError = SIG_EC_OVERFLOW;
            }

            break;
        }
        default:
            break;
    }

    // correct value descriptors according to type of signal
    list<CValueDescriptor>::iterator rValDesc;
    for (rValDesc=m_listValueDescriptor.begin(); rValDesc!=m_listValueDescriptor.end(); ++rValDesc)
    {
        switch(m_ucType)
        {
            case SIG_TYPE_BOOL:
            case SIG_TYPE_UINT:
                rValDesc->m_value.uiValue = (unsigned int)rValDesc->m_value.dValue;
                break;

            case SIG_TYPE_INT:
                rValDesc->m_value.iValue = (int)rValDesc->m_value.dValue;
                break;

            case SIG_TYPE_FLOAT:
                rValDesc->m_value.fValue = (float)rValDesc->m_value.dValue;
                break;

            case SIG_TYPE_DOUBLE:
                break;

            case SIG_TYPE_INT64:
                //rValDesc->m_value.i64Value = (long long)rValDesc->m_value.dValue;
                break;

            case SIG_TYPE_UINT64:
                rValDesc->m_value.ui64Value = (unsigned long long)rValDesc->m_value.i64Value;
                break;

            default:
                break;
        }
    }
    return (m_uiError);
}

void CSignal::GetErrorString(string &str)
{
	switch(m_uiError) {
		case SIG_EC_NO_ERR:
			str = "No error";
			break;
		case SIG_EC_DATA_FORMAT_ERR:
			str = "ERROR:Data format mismatch";
			break;
		case SIG_EC_LENGTH_ERR:
			str = "ERROR:Invalid signal length";
			break;
		case SIG_EC_STARTBIT_ERR:
			str = "ERROR:Invalid start bit";
			break;
		case SIG_EC_TYPE_ERR:
			str = "ERROR:Invalid signal type";
			break;
		case SIG_EC_OVERFLOW:
			str = "WARNING:Invalid Max or Min value.";
			break;
		default:
			str = "Unknown";
			break;
	}
}

void CSignal::GetErrorAction(string &str)
{
	switch(m_uiError) {
		case SIG_EC_OVERFLOW:
			str = "Value Set to default";
			break;
		default:
			str = "Signal Discarded";
			break;
    }
}

/**
 * \brief     writes the signals in the given list to the output file
 * \param[in] fileOutput Pointer to the Output file
 * \param[in] m_listSignals List of Signals
 * \param[in] m_ucLength Message length
 * \param[in] m_cDataFormat If 1 dataformat Intel, 0- Motorola
 * \param[in] writeErr If true write error signals also else write onlt correct signals
 *
 * Writes the signals in the given list to the output file.
 */
bool CSignal::WriteSignaltofile(fstream &fileOutput, list<CSignal> &m_listSignals, int m_ucLength, int m_cDataFormat, bool writeErr)
{
    bool bResult = true;
    list<CSignal>::iterator sig;
    for (sig=m_listSignals.begin(); sig!=m_listSignals.end(); ++sig)
    {
        // SIG_NAME,SIG_LENGTH,WHICH_BYTE_IN_MSG,START_BIT,SIG_TYPE,MAX_VAL,MIN_VAL,SIG_DATA_FORMAT,SIG_OFFSET,SIG_FACTOR,SIG_UNIT
        // write signal only if it is valid
        if((sig->m_uiError == CSignal::SIG_EC_NO_ERR) || (sig->m_uiError == CSignal::SIG_EC_OVERFLOW) || writeErr)
        {
            // For signal having motoroal format, the message length could be less
            // then eight byte. so in that case the whichByte needs to be shifted
            // accordingly.
            fileOutput << T_SIG;
            fileOutput << " ";
            fileOutput << sig->m_acName.c_str();
            fileOutput << ",";
            fileOutput << (int) sig->m_ucLength;
            fileOutput << ",";
            fileOutput << (int) sig->m_ucWhichByte;
            fileOutput << ",";
            fileOutput << (int) sig->m_ucStartBit;
            fileOutput << ",";

            switch(sig->m_ucType)
            {
                case CSignal::SIG_TYPE_BOOL:
                case CSignal::SIG_TYPE_UINT:
                    fileOutput << sig->m_ucType;
                    fileOutput << ",";
                    fileOutput << dec << sig->m_MaxValue.uiValue;
                    fileOutput << ",";
                    fileOutput << dec << sig->m_MinValue.uiValue;
                    break;

                case CSignal::SIG_TYPE_INT:
                    fileOutput << sig->m_ucType;
                    fileOutput << ",";
                    fileOutput << dec << sig->m_MaxValue.iValue;
                    fileOutput << ",";
                    fileOutput << dec << sig->m_MinValue.iValue;
                    break;


                case CSignal::SIG_TYPE_FLOAT:
                    fileOutput << sig->m_ucType;
                    fileOutput << ",";
                    fileOutput << fixed << sig->m_MaxValue.fValue;
                    fileOutput << ",";
                    fileOutput << fixed << sig->m_MinValue.fValue;
                    break;

                case CSignal::SIG_TYPE_DOUBLE:
                    fileOutput << sig->m_ucType;
                    fileOutput << ",";
                    fileOutput << fixed << sig->m_MaxValue.dValue;
                    fileOutput << ",";
                    fileOutput << fixed << sig->m_MinValue.dValue;
                    break;

                case CSignal::SIG_TYPE_INT64:
                    fileOutput << "I";
                    fileOutput << ",";
                    fileOutput << dec << sig->m_MaxValue.i64Value;
                    fileOutput << ",";
                    fileOutput << dec << sig->m_MinValue.i64Value;
                    break;

                case CSignal::SIG_TYPE_UINT64:
                    fileOutput << "U";
                    fileOutput << ",";
                    fileOutput << dec << sig->m_MaxValue.ui64Value;
                    fileOutput << ",";
                    fileOutput << dec << sig->m_MinValue.ui64Value;
                    break;

                default:
                    break;
            }
            fileOutput << ",";
            fileOutput << sig->m_ucDataFormat;
            fileOutput << ",";
            fileOutput << fixed << sig->m_fOffset;
            fileOutput << ",";
            fileOutput << fixed << sig->m_fScaleFactor;
            fileOutput << ",";
            fileOutput << sig->m_acUnit.c_str();
            fileOutput << ",";
            fileOutput << sig->m_acMultiplex.c_str();
            fileOutput << ",";
            fileOutput << sig->m_rxNode.c_str();
            fileOutput << endl;

            CValueDescriptor val;
            val.writeValueDescToFile(fileOutput, sig->m_ucType, sig->m_listValueDescriptor);
            if(sig->m_uiError == CSignal::SIG_EC_OVERFLOW)
                bResult = false;
        }
        else
            bResult = false;
    }

    return bResult;
}
