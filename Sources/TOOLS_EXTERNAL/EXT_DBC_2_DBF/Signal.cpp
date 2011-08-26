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
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
/*********************************************************************
  Project       :  CANDb convertor
  FileName      :  ParameterVal.Cpp
  Description   :  Implementation file for the ParameterValues class.
  
  $Log:   X:/Archive/Sources/Ext_tools_DBC_2_DBF/Signal.cpv  $
   
      Rev 1.1   04 Aug 2011 19:52:56   CANMNTTM
    
   
      Rev 1.0   03 Aug 2011 15:43:52   rac2kor
  Modification:
        Amitesh Bharti
            Taken care for signal discriptor name having space.
            The validation for max and min value is corrected for all conditions.
	    Pemmaiah BD - 3/4/03
			- Added support for INT64 type
		Mahesh.B.S - 5/11/2004
			-Added Support for the receiving node.
			-Added  Function writeSigToFile to write all the signal to a file
			 in a given list.
  Author(s)     :  
  Date Created  :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
*********************************************************************/

/**
* \file       Signal.cpp
* \brief      implementation of the CSignal class.
* \authors    Amitesh Bharti, Pemmaiah BD, Mahesh.B.S 
* \date       03.04.2003 Created
* \date       15.04.2011 Added RBEI Copyright information
* \copyright  Copyright &copy; 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
*/

#include "stdafx.h"
#include "CANDBConverter.h"
#include "Signal.h"
#include "Utility.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/**
* \brief      Constructort for CSignal
* \param[in]  None 
* \param[out] None
* \return     None
* \authors    Mahesh.B.S
* \date       15.11.2004
*/
CSignal::CSignal()
{
    m_acMultiplex[0]= '\0';
    m_acName[0] = '\0'; // 
    m_ucLength = 1; // minimum signal length should be 1 bit
    m_ucWhichByte = 1; // ONE based index 
    m_ucStartBit = 0; // ZERO based index
    m_ucType = SIG_TYPE_BOOL; // let default be bool type -- match with default length
    m_MaxValue.uiValue = 1; // use unsigned int field for bool also
    m_MinValue.uiValue = 0; //
    m_ucDataFormat = SIG_DF_INTEL;
    m_fOffset = 0.0f;
    m_fScaleFactor = 1.0f;
    m_acUnit[0] = '\0';
    m_uiError = SIG_EC_NO_ERR;
    m_rxNode = "\0";
    m_listValueDescriptor.RemoveAll();
}

/**
* \brief      Destructor for CSignal
* \param[in]  None 
* \param[out] None
* \return     None
* \authors    Mahesh.B.S
* \date       15.11.2004
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
* \brief      overloaded operator = 
Overload assignment operator. We should clear the existing value descriptor
list and copy contents of the argument's list to this signal. The rest of the 
members are copied by value
* \param[in]  CSignal& 
* \param[out] None
* \return     CSignal&
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
    strcpy(m_acMultiplex,signal.m_acMultiplex);
    strcpy(m_acName,signal.m_acName);
    m_ucLength = signal.m_ucLength;
    m_ucWhichByte = signal.m_ucWhichByte;
    m_ucStartBit = signal.m_ucStartBit;
    m_ucType = signal.m_ucType;
    m_MaxValue = signal.m_MaxValue;
    m_MinValue = signal.m_MinValue;
    m_ucDataFormat = signal.m_ucDataFormat;
    m_fOffset = signal.m_fOffset;
    m_fScaleFactor = signal.m_fScaleFactor;
    strcpy(m_acUnit,signal.m_acUnit);
    m_rxNode = signal.m_rxNode;
    m_uiError = signal.m_uiError; 
    // now copy the list 
    m_listValueDescriptor.AddTail(&signal.m_listValueDescriptor);
    return (*this);
}

/**
* \brief      Extracts the message data from the given Line and populates 
the message structure.
* \param[in]  char *pcLine
* \param[out] None
* \return     int
* \authors    Mahesh.B.S
* \date       15.11.2004
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
    // copy only signal name because we CANvas does not support modes
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

    strcpy(m_acName,acTemp); // copy the name to the signal's data member

    pcTemp = acTemp; // reset pcTemp to start of buffer


    //leave blank space
    *pcToken++;
    //Find the signal's multiplexing details
    while(*pcToken && *pcToken != ' ')
    {
        *pcTemp++ = *pcToken++; // copy SIG_NAME only, i.e. till first 'space'
    }
    *pcTemp = '\0'; // terminate it 
    strcpy(m_acMultiplex,acTemp); // copy the name to the signal's data member

    pcTemp = acTemp; // reset pcTemp to start of buffer

    // next token (START_BIT|LENGTH@DATA_FROMAT(+/-))
    pcToken = strtok(NULL," :");


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
    // With motorola format, bits are stored in reverse order. CANvas
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
    strcpy(m_acUnit,acTemp); // copy UNIT to corresponding data member.
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

/*
* \brief      Extracts the value descriptor data from the given Line and adds 
to the signal.
* \param[in]  char *pcLine
* \param[out] None
* \return     int
* \authors    Mahesh.B.S
* \date       15.11.2004
*/
int CSignal::AddValueDescriptors(char *pcLine,CStdioFile &fileInput)
{
    char acValue[300];
    char acDesc[300];
    char acLine[defVTAB_MAX_LINE_LEN] = {'\0'};
    char pcTemp[defVTAB_MAX_TEMP_LEN];
    char *pcToken;
    bool true_end = true;

    char *pcValue = acValue;
    char *pcDesc = acDesc;
    if(pcLine[strlen(pcLine)-2] != ';')
    {
        true_end  = false;
    }
    // skip leading spaces
    while(*pcLine && *pcLine == ' ')
    {
        *pcLine++;
    }
    while(*pcLine && *pcLine != ';')
    {   pcTemp[0]='\0';
    if(strlen(pcLine) < 100 && true_end == false)
    {
        fileInput.ReadString(acLine,1026);
        strcpy(pcTemp,pcLine);
        strcat(pcTemp,acLine);
        pcLine = pcTemp;
        if(pcLine[strlen(pcLine)-2] == ';')
            true_end  = true;
    }
    pcDesc=acDesc;
    while(*pcLine && *pcLine==' ')
        pcLine++;
    pcToken=strtok(pcLine," ");
    strcpy(acValue,pcToken);
    pcLine=pcLine+(strlen(pcToken))+1;

    if(*pcLine=='\"')
    {
        pcLine++;
        if(*pcLine!='\"')
        {
            *pcDesc++=*pcLine++;
            while(*pcLine && *pcLine!='\"')
            {
                *pcDesc++=*pcLine++;
            }
        }
        *pcDesc='\0';
    }
    pcLine++;
    while(*pcLine==' ')
        pcLine++;
    CValueDescriptor valDesc;
    if(this->m_ucLength <= 32)
        valDesc.m_value.dValue = atof(acValue);
    else
        valDesc.m_value.i64Value = _atoi64(acValue);
    strcpy(valDesc.m_acDescriptor,acDesc);																		
    m_listValueDescriptor.AddTail(valDesc);
    pcDesc=acDesc;
    }
    return 1;
}

/**
* \brief      validate for conformance to CANvas DB format

Convert single bit INT and UINT to BOOL
if MAX_value == MIN_value update MAX_value to MAX_default
else if MAX_value == MIN_value == MAX_default, update MIN_value to MIN_default
signals with length more than 32 bits shall be discarded

The data format of previous signal is passed as a parameter
For the first one it will be zero, for then on either INTEL or MOTOROLA
If this signals format is not matching with previous signal then this signal to be discarded. 
SIG_EC_NO_ERR, SIG_EC_DATA_FORMAT_ERR,SIG_EC_LENGTH_ERR,SIG_EC_STARTBIT_ERR,SIG_EC_TYPE_ERR

* \param[in]  unsigned char ucFormat
* \param[out] None
* \return     unsigned int
* \authors    Mahesh.B.S
* \date       15.11.2004
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


    m_uiError = SIG_EC_NO_ERR;

    //Conversion from the physical to raw value required.


    // update MAX value and MIN value based on type
    switch(m_ucType)
    {
    case SIG_TYPE_BOOL:
    case SIG_TYPE_UINT:
        {
            UINT64 unMaxVal;
            UINT64 unMinVal;
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
            if(m_MaxValue.uiValue > uiDefault || m_MaxValue.uiValue < 0) 
            {
                m_MaxValue.uiValue = uiDefault;
                m_uiError = SIG_EC_OVERFLOW;
            }
            //if Min value out of range reset it to minimum possible value
            if(m_MinValue.uiValue < 0 || m_MinValue.uiValue > uiDefault )
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
            INT64 nMaxVal;
            INT64 nMinVal;
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
            INT64 nMaxVal;
            INT64 nMinVal;
            nConvertPhysicalToRaw(m_MaxValue.dValue, m_MinValue.dValue, m_fOffset, m_fScaleFactor, nMaxVal, nMinVal);
            m_MaxValue.i64Value = nMaxVal;             //m_MaxValue.dValue;
            m_MinValue.i64Value = nMinVal;             //m_MinValue.dValue;
            UINT unPower; 
            __int64 i64Default;
            UINT i;
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
            for(i = 0; i<unPower;i++)
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
            UINT64 unMaxVal;
            UINT64 unMinVal;
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
            UINT unPower = m_ucLength - 1;
            ui64Default = 1;
            for(int i = 0; i<unPower;i++)
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
            //rValDesc.m_value.i64Value = (LONGLONG)rValDesc.m_value.dValue;
            break;

        case SIG_TYPE_UINT64:
            rValDesc.m_value.ui64Value = (ULONGLONG)rValDesc.m_value.i64Value;
            break;

        default:
            break;
        }			
    }
    return (m_uiError);
}

// SIG_EC_NO_ERR, SIG_EC_DATA_FORMAT_ERR,SIG_EC_LENGTH_ERR,SIG_EC_STARTBIT_ERR,SIG_EC_TYPE_ERR
const char* CSignal::m_pacErrorStrings[] =
{
    "No error",
    "ERROR:Data format mismatch",
    "ERROR:Invalid signal length",
    "ERROR:Invalid start bit",
    "ERROR:Invalid signal type",	
    "WARNING:Invalid Max or Min value."
};

const char* CSignal::GetErrorString()
{
    return m_pacErrorStrings[m_uiError];
}

const char* CSignal::GetErrorAction()
{
    if(m_uiError == SIG_EC_OVERFLOW)
    {
        return "Value Set to default";
    }
    else
    {
        return "Signal Discarded";
    }
}

/**
* \brief      writes the signals in the given list to the output file
* \param[in]  1.CStdioFile &fileOutput[in]
Pointer to the Output file
2.CList<CSignal,CSignal&> &m_listSignals [in]
List of Signals
3.int m_ucLength
Message length
4.int m_cDataFormat
If 1 dataformat Intel, 0- Motorola
5.bool writeErr
If true write error signals also else write onlt correct signals
* \param[out] None
* \return     bool
* \authors    Mahesh.B.S
* \date       15.11.2004 Created
* \date       Raja N on 11.08.2005, Changed signal type for __int64 and unsigned __int64.
*/
bool CSignal::WriteSignaltofile(CStdioFile &fileOutput,CList<CSignal,CSignal&> &m_listSignals,int m_ucLength,int m_cDataFormat,bool writeErr)
{
    bool bResult = true;
    char acLine[defCON_MAX_LINE_LEN];
    POSITION posSig = m_listSignals.GetHeadPosition();
    while(posSig != NULL)
    {
        // SIG_NAME,SIG_LENGTH,WHICH_BYTE_IN_MSG,START_BIT,SIG_TYPE,MAX_VAL,MIN_VAL,SIG_DATA_FORMAT,SIG_OFFSET,SIG_FACTOR,SIG_UNIT
        CSignal& sig = m_listSignals.GetNext(posSig);
        // write signal only if it is valid
        if(sig.m_uiError == CSignal::SIG_EC_NO_ERR || sig.m_uiError == CSignal::SIG_EC_OVERFLOW || writeErr)
        {
            // For signal having motoroal format, the message length could be less
            // then eight byte. so in that case the whichByte needs to be shifted
            // accordingly.

            switch(sig.m_ucType)
            {
            case CSignal::SIG_TYPE_BOOL:
            case CSignal::SIG_TYPE_UINT:
                sprintf(acLine,"%s %s,%u,%u,%u,%c,%u,%u,%c,%f,%f,%s,%s,%s\n",T_SIG,
                    sig.m_acName,sig.m_ucLength,sig.m_ucWhichByte,sig.m_ucStartBit,sig.m_ucType,
                    sig.m_MaxValue.uiValue,sig.m_MinValue.uiValue,sig.m_ucDataFormat,
                    sig.m_fOffset,sig.m_fScaleFactor,sig.m_acUnit,sig.m_acMultiplex,sig.m_rxNode);

                break;

            case CSignal::SIG_TYPE_INT:
                sprintf(acLine,"%s %s,%u,%u,%u,%c,%d,%d,%c,%f,%f,%s,%s,%s\n",T_SIG,
                    sig.m_acName,sig.m_ucLength,sig.m_ucWhichByte,sig.m_ucStartBit,sig.m_ucType,
                    sig.m_MaxValue.iValue,sig.m_MinValue.iValue,sig.m_ucDataFormat,
                    sig.m_fOffset,sig.m_fScaleFactor,sig.m_acUnit,sig.m_acMultiplex,sig.m_rxNode);

                break;


            case CSignal::SIG_TYPE_FLOAT:	
                sprintf(acLine,"%s %s,%u,%u,%u,%c,%f,%f,%c,%f,%f,%s,%s,%s\n",T_SIG,
                    sig.m_acName,sig.m_ucLength,sig.m_ucWhichByte,sig.m_ucStartBit,sig.m_ucType,
                    sig.m_MaxValue.fValue,sig.m_MinValue.fValue,sig.m_ucDataFormat,
                    sig.m_fOffset,sig.m_fScaleFactor,sig.m_acUnit,sig.m_acMultiplex,sig.m_rxNode);

                break;

            case CSignal::SIG_TYPE_DOUBLE:
                sprintf(acLine,"%s %s,%u,%u,%u,%c,%f,%f,%c,%f,%f,%s,%s,%s\n",T_SIG,
                    sig.m_acName,sig.m_ucLength,sig.m_ucWhichByte,sig.m_ucStartBit,sig.m_ucType,
                    sig.m_MaxValue.dValue,sig.m_MinValue.dValue,sig.m_ucDataFormat,
                    sig.m_fOffset,sig.m_fScaleFactor,sig.m_acUnit,sig.m_acMultiplex,sig.m_rxNode);

                break;


            case CSignal::SIG_TYPE_INT64:
                sprintf(acLine,"%s %s,%u,%u,%u,%c,%I64d,%I64d,%c,%f,%f,%s,%s,%s\n",T_SIG,
                    sig.m_acName,sig.m_ucLength,sig.m_ucWhichByte,sig.m_ucStartBit,/*sig.m_ucType*/'I',
                    sig.m_MaxValue.i64Value,sig.m_MinValue.i64Value,sig.m_ucDataFormat,
                    sig.m_fOffset,sig.m_fScaleFactor,sig.m_acUnit,sig.m_acMultiplex,sig.m_rxNode);
                break;

            case CSignal::SIG_TYPE_UINT64:
                sprintf(acLine,"%s %s,%u,%u,%u,%c,%I64u,%I64u,%c,%f,%f,%s,%s,%s\n",T_SIG,
                    sig.m_acName,sig.m_ucLength,sig.m_ucWhichByte,sig.m_ucStartBit,/*sig.m_ucType*/'U',
                    sig.m_MaxValue.ui64Value,sig.m_MinValue.ui64Value,sig.m_ucDataFormat,
                    sig.m_fOffset,sig.m_fScaleFactor,sig.m_acUnit,sig.m_acMultiplex,sig.m_rxNode);
                break;

            default:
                break;
            }
            fileOutput.WriteString(acLine);		

            CValueDescriptor val;
            val.writeValuDescToFile (fileOutput,sig.m_ucType,sig.m_listValueDescriptor);
            if(sig.m_uiError == CSignal::SIG_EC_OVERFLOW )
                bResult = false;
        }
        else 
            bResult = false;
    }

    return bResult;
}
