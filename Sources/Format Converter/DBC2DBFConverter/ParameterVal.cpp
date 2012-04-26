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
 * \file      ParameterVal.cpp
 * \brief     Implementation of parameter value class
 * \authors   Padmaja A, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the parameter value class.
 */

/* Project includes */
#include "ParameterVal.h"

using namespace std;

/**
 * \brief Constructor of CParameterValues
 *
 * Default Constructor used to initialse parameterVal class object.
 */
CParameterValues::CParameterValues()
{
    m_NodeName = "";
    m_MsgId = 0;
    m_SignalName = "";
    m_cFrameFormat = MSG_FF_STANDARD;
    m_ParamVal.iValue = -1;
    m_ParamVal.uiValue = 0;
    m_ParamVal.fValue = -1;
    m_ParamVal.dValue = -1;
    m_ParamVal.cValue = "";
    m_ParamVal.i64Value = -1;
    m_ParamVal.ui64Value = 0;
}

/**
 * \brief Destructor
 *
 * Destructor of CParameterValues
 */
CParameterValues::~CParameterValues()
{
}


/**
 * \brief     copy operator
 * \param[in] param Data to assign from
 * \return    Reference to local object with new contents
 *
 * Copies the contents of param to the local object.
 */
CParameterValues& CParameterValues::operator=(CParameterValues& param)
{
    // now copy the other elements of the new message to this
    m_NodeName = param.m_NodeName;
    m_SignalName = param.m_SignalName;
    m_MsgId = param.m_MsgId;
    m_ParamVal = param.m_ParamVal;
    m_cFrameFormat = param.m_cFrameFormat;
    return (*this);
}


/**
 * \brief     Parses the Node Parameter Values
 * \param[in] paramType Parameter Type
 * \param[in] pcLine Input line
 * \return    Status code
 *
 * Parses the Node Parameter Values other than Default value from the input CanoeDB file.
 */
int CParameterValues::GetNodeParams(string& paramType, char* pcLine)
{
    char* pcToken, *pcTok;
    char acTemp[defCON_TEMP_LEN],*pcTemp;
    pcTemp = acTemp;
    int success=1;
    //reads the nodes name
    pcToken = strtok_s(pcLine, ";", &pcTok);

    while(*pcToken && *pcToken != ' ')
    {
        *pcTemp++ = *pcToken++;
    }

    *pcTemp = '\0';
    m_NodeName = acTemp;
    pcTemp = acTemp;
    //Gets the node param value.
    ReadParamValue(paramType, pcToken);
    return success;
}

/**
 * \brief     Parses the Network Parameter Values
 * \param[in] paramType Parameter Type
 * \param[in] pcLine Input line
 * \return    Status code
 *
 * Parses the Network Parameter Values other than Default value from the input CanoeDB file.
 */
int CParameterValues::GetNetParams(string& paramType, char* pcLine)
{
    char* pcToken = pcLine;
    char* pcTok;
    int success = 1;
    //reads the net param value.
    pcToken=strtok_s(pcToken, ";", &pcTok);
    ReadParamValue(paramType, pcToken);
    return success;
}

/**
 * \brief     Parses the Message Parameter
 * \param[in] paramType Parameter Type
 * \param[in] pcLine Input line
 * \return    Status code
 *
 * This function Parses the Message Parameter other Values rather than Default value from the input CanoeDB
 * file and finds the frame foramt for that Message ID.
 */
int CParameterValues::GetMesgParams(string& paramType,char* pcLine)
{
    char* pcToken, *pcTok;
    char acTemp[defCON_TEMP_LEN], *pcTemp;
    int success = 1;
    pcTemp = acTemp;
    //get Message Id.
    pcToken = strtok_s(pcLine, ";", &pcTok);

    while(*pcToken && (*pcToken != ' '))
    {
        *pcTemp++ = *pcToken++;
    }

    *pcTemp = '\0';
    m_MsgId = (unsigned int)strtoul(acTemp, NULL, 10);
    pcTemp = acTemp;

    //Validates the MsgId to get frame format.
    if(m_MsgId < 0x80000000UL)
    {
        m_cFrameFormat = CParameterValues::MSG_FF_STANDARD;
    }
    else
    {
        m_cFrameFormat = CParameterValues::MSG_FF_EXTENDED;
        m_MsgId &= 0x7FFFFFFF;
    }

    //pcToken=strtok(NULL,";");
    //get the mesg param value.
    ReadParamValue(paramType, pcToken);
    return success;
}

/**
 * \brief     Parses the Signal Parameter Values
 * \param[in] paramType Parameter Type
 * \param[in] pcLine Input line
 * \return    Status code
 *
 * Parses the Signal Parameter Values other than Default value from the
 * input CanoeDB file and calculates the frame format
 * for the corresponding Message ID.
 */
int CParameterValues::GetSignalParams(string& paramType,char* pcLine)
{
    char* pcToken, *pcTok;
    char acTemp[defCON_TEMP_LEN],*pcTemp;
    int success=1;
    pcTemp = acTemp;
    pcToken = strtok_s(pcLine, " ", &pcTok);

    //get the message Id.
    while(*pcToken && *pcToken != ' ')
    {
        *pcTemp++ = *pcToken++;
    }

    *pcTemp = '\0';
    m_MsgId = strtoul(acTemp, NULL, 10);
    pcTemp = acTemp;
    pcToken = strtok_s(NULL, ";", &pcTok);

    //get signal name.
    while(*pcToken && *pcToken != ' ')
    {
        *pcTemp++ = *pcToken++;
    }

    *pcTemp = '\0';
    m_SignalName = acTemp;
    pcTemp=acTemp;

    //validates the msgId to get the frame format.
    if(m_MsgId < 0x80000000UL)
    {
        m_cFrameFormat = CParameterValues::MSG_FF_STANDARD;
    }
    else
    {
        m_cFrameFormat = CParameterValues::MSG_FF_EXTENDED;
        m_MsgId &= 0x7FFFFFFF;
    }

    //get signal param value.
    ReadParamValue(paramType, pcToken);
    return success;
}

/**
 * \brief     Reads the other vlaue of attributes from CanoeDB file.
 * \param[in] paramType Parameter Type
 * \param[in] pcToken Input line
 * \return    Status code
 *
 * Reads the other vlaue of attributes from CanoeDB file.
 */
int CParameterValues::ReadParamValue(string& paramType,char* pcToken)
{
    int success = 1;

    //Param type :STRING
    if(paramType == "STRING")
    {
        while(*pcToken != '"')
        {
            *pcToken++;
        }

        m_ParamVal.cValue = pcToken;
    }
    //Param type :INT/HEX
    else if((paramType == "INT") || (paramType == "HEX"))
    {
        if(strcmp(pcToken, " ") != 0)
        {
            m_ParamVal.iValue = atoi(pcToken);
        }
    }
    //Param type :ENUM
    else if(paramType == "ENUM")
    {
        while(*pcToken == ' ')
        {
            *pcToken++;
        }

        m_ParamVal.cValue = pcToken;
    }
    //Param type :FLOAT
    else if(paramType == "FLOAT")
    {
        if(strcmp(pcToken, " ") != 0)
        {
            m_ParamVal.fValue = float(atof(pcToken));
        }
    }

    return success;
}

/**
 * \brief     Writes network values to file
 * \param[in] fileOutput Output file
 * \param[in] paramType Parameter Type
 * \param[in] paramName Parameter Name
 *
 * Writes network values to file.
 */
const void CParameterValues::WriteNetValuesToFile(fstream& fileOutput, string& paramType, string& paramName)
{
    fileOutput << "\"";
    fileOutput << paramName.c_str();
    fileOutput << "\",";

    //writes int/hex param net values to o/p file and validates initial value.
    if((paramType == "INT") || (paramType == "HEX"))
    {
        if(m_ParamVal.iValue != -1)
        {
            fileOutput << dec << m_ParamVal.iValue;
        }
    }
    //writes Float param net values to o/p file and validates initial value.
    else if(paramType == "FLOAT")
    {
        if(m_ParamVal.fValue != -1)
        {
            fileOutput << m_ParamVal.fValue;
        }
    }
    //writes enum param net values to o/p file and validates initial value.
    else if(paramType == "ENUM")
    {
        fileOutput << "\"";
        fileOutput << m_ParamVal.cValue;
        fileOutput << "\"";
    }
    //writes string param net values to o/p file and validates initial value.
    else if(paramType == "STRING")
    {
        fileOutput << m_ParamVal.cValue;
    }

    fileOutput << endl;
}

/**
 * \brief     Writes node value to file
 * \param[in] fileOutput Output file
 * \param[in] paramType Parameter Type
 * \param[in] paramName Parameter Name
 *
 * Writes node value to file.
 */
const void CParameterValues::WriteNodeValuesToFile(fstream& fileOutput, string& paramType, string& paramName)
{
    fileOutput << m_NodeName.c_str();
    fileOutput << ",\"";
    fileOutput << paramName.c_str();
    fileOutput << "\",";

    //writes int/hex param node values to o/p file and validates initial value.
    if((paramType == "INT") || (paramType == "HEX"))
    {
        if(m_ParamVal.iValue != -1)
        {
            fileOutput << dec << m_ParamVal.iValue;
        }
    }
    //writes enum param node values to o/p file and validates initial value.
    else if(paramType == "ENUM")
    {
        fileOutput << "\"";
        fileOutput << m_ParamVal.cValue;
        fileOutput << "\"";
    }
    //writes float param node values to o/p file and validates initial value.
    else if(paramType == "FLOAT")
    {
        if(m_ParamVal.fValue != -1)
        {
            fileOutput << m_ParamVal.fValue;
        }
    }
    //writes string param node values to o/p file and validates initial value.
    else if(paramType == "STRING")
    {
        fileOutput << m_ParamVal.cValue;
    }

    fileOutput << endl;
}

/**
 * \brief     Writes message values to file
 * \param[in] fileOutput Output file
 * \param[in] paramType Parameter Type
 * \param[in] paramName Parameter Name
 *
 * Writes message values to file.
 */
const void CParameterValues::WriteMesgValuesToFile(fstream& fileOutput, string& paramType, string& paramName)
{
    fileOutput << dec << m_MsgId;
    fileOutput << ",";
    fileOutput << m_cFrameFormat;
    fileOutput << ",\"";
    fileOutput << paramName.c_str();
    fileOutput << "\",";

    //writes int/hex param mesg values to o/p file and validates initial value.
    if((paramType == "INT") || (paramType == "HEX"))
    {
        if(m_ParamVal.iValue != -1)
        {
            fileOutput << dec << m_ParamVal.iValue;
        }
    }
    //writes enum param mesg values to o/p file and validates initial value.
    else if(paramType == "ENUM")
    {
        fileOutput << "\"";
        fileOutput << m_ParamVal.cValue;
        fileOutput << "\"";
    }
    //writes float param mesg values to o/p file and validates initial value.
    else if(paramType == "FLOAT")
    {
        if(m_ParamVal.fValue != -1)
        {
            fileOutput << m_ParamVal.fValue;
        }
    }
    //writes string param mesg values to o/p file and validates initial value.
    else if(paramType == "STRING")
    {
        fileOutput << m_ParamVal.cValue;
    }

    fileOutput << endl;
}

/**
 * \brief     Writes signal values to file
 * \param[in] fileOutput Output file
 * \param[in] paramType Parameter Type
 * \param[in] paramName Parameter Name
 *
 * Writes signal values to file.
 */
const void CParameterValues::WriteSigValuesToFile(fstream& fileOutput, string& paramType, string& paramName)
{
    fileOutput << m_MsgId;
    fileOutput << ",";
    fileOutput << m_cFrameFormat;
    fileOutput << ",";
    fileOutput << m_SignalName.c_str();
    fileOutput << ",\"";
    fileOutput << paramName.c_str();
    fileOutput << "\",";

    //writes int/hex param sig values to o/p file and validates initial value.
    if((paramType == "INT") || (paramType == "HEX"))
    {
        if(m_ParamVal.iValue != -1)
        {
            fileOutput << dec << m_ParamVal.iValue;
        }
    }
    //writes enum param sig values to o/p file and validates initial value.
    else if(paramType == "ENUM")
    {
        fileOutput << "\"";
        fileOutput << m_ParamVal.cValue;
        fileOutput << "\"";
    }
    //writes float param sig values to o/p file and validates initial value.
    else if(paramType == "FLOAT")
    {
        if(m_ParamVal.fValue != -1)
        {
            fileOutput << m_ParamVal.fValue;
        }
    }
    //writes string param sig values to o/p file and validates initial value.
    else if(paramType == "STRING")
    {
        fileOutput << m_ParamVal.cValue;
    }

    fileOutput << endl;
}
