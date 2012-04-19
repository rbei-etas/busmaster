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
 * \brief     Implementation file for the ParameterValues class.
 * \authors   Padmaja A, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for the ParameterValues class.
 */

/* Project includes */
#include "ParameterVal.h"

using namespace std;

/**
 * \brief Constructor
 *
 * Constructor of CParameterValues
 */
CParameterValues::CParameterValues()
{
    m_NodeName[0]='\0';
    m_MsgId=0;
    m_ParamVal.dValue =0;
    m_SignalName[0]='\0';
    m_cFrameFormat=MSG_FF_STANDARD;
    m_ParamVal.iValue =-1;
    m_ParamVal.fValue=-1;
    m_ParamVal.uiValue =-1;
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
 * \brief     overloaded operator =
 * \param[in] param Parameter to copy content from
 * \return    Local parameter with new contents
 *
 * Copies the contents of param to local object.
 */
CParameterValues& CParameterValues::operator=(CParameterValues& param)
{
    // now copy the other elements of the new message to this
    m_NodeName=param.m_NodeName;
    m_SignalName=param.m_SignalName;
    m_MsgId=param.m_MsgId;
    m_ParamVal=param.m_ParamVal;
    m_cFrameFormat=param.m_cFrameFormat;
    return (*this);
}

/**
 * \brief Format Parameter Value
 * \param[in] paramType Parameter Type
 * \param[in] pcLine Input line
 * \param[in] index Index
 * \param[in] msgId Message Identifier
 * \param[in] Node_Name Node Name
 *
 * Parses the Parameter Values other than Default value
 * from the input file.
 */
void CParameterValues::Format_Param_Value(string& paramType, char* pcLine,const int& index,unsigned int msgId,char* Node_Name)
{
    switch(index)
    {
        case 1:
            //copies the node name to the member
            m_NodeName = Node_Name;
            break;

        case 2:
            //copies the message id.
            m_MsgId=msgId;
            break;

        case 3:
            //store the signal name.
            m_MsgId =msgId;
            m_SignalName = Node_Name;
            break;
    }

    //get param value.
    ReadParamValue(paramType, pcLine);
}

/**
 * \brief     Read Parameter Value
 * \param[in] paramType Parameter type
 * \param[in] pcToken   Input token
 *
 * Reads the other value of attributes from i/p file.
 */
void CParameterValues::ReadParamValue(string& paramType, char* pcToken)
{
    char acTemp[defCON_CHAR_LEN],*pcTemp;
    pcTemp = acTemp;

    //get param value of type String
    if(paramType == "STRING")
    {
        pcToken++;

        while(*pcToken != '"')
        {
            *pcTemp++=*pcToken++;
        }

        *pcTemp='\0';
        m_ParamVal.cValue = acTemp;
        pcTemp=acTemp;
    }
    //get param value of type int
    else if((paramType == "INT") || (paramType == "HEX"))
    {
        if(strcmp(pcToken," ")!=0)
        {
            m_ParamVal.iValue=atoi(pcToken);
        }
    }
    //get param value of type enum
    else if (paramType == "ENUM")
    {
        *pcToken++;

        while(*pcToken!='"')
        {
            *pcTemp++=*pcToken++;
        }

        *pcTemp='\0';
        m_ParamVal.cValue = acTemp;
        pcTemp=acTemp;
    }
    //get param value of type float
    else if(paramType == "FLOAT")
    {
        if(strcmp(pcToken," ")!=0)
        {
            m_ParamVal.fValue=float(atof(pcToken));
        }
    }
}

/**
 * \brief     Write the net values into the file
 * \param[in] fileOutput Output file
 * \param[in] paramType Parameter Type
 * \param[in] paramName Parameter Name
 *
 * Write the net values into the file.
 */
void CParameterValues::WriteNetValuesToFile(fstream& fileOutput, string& paramType, string& paramName)
{
    fileOutput << "BA_ \"" << paramName.c_str() << "\"";

    //writes net values of type int to o/p file.
    if((paramType == "INT") || (paramType == "HEX"))
    {
        if(m_ParamVal.iValue != -1)
        {
            fileOutput << " " << dec << m_ParamVal.iValue;
        }
    }
    //writes net values of type float to o/p file.
    else if(paramType == "FLOAT")
    {
        if(m_ParamVal.fValue != -1)
        {
            fileOutput << " " << m_ParamVal.fValue;
        }
    }
    //writes net values of type enum to o/p file.
    else if(paramType == "ENUM")
    {
        if(m_ParamVal.cValue.length() == 0)
        {
            fileOutput << " \"\"";
        }
        else
        {
            fileOutput << " " << m_ParamVal.cValue;
        }
    }
    //writes net values of type string to o/p file.
    else if(paramType == "STRING")
    {
        fileOutput << " \"" << m_ParamVal.cValue << "\"";
    }

    fileOutput << ";" << endl;
}

/**
 * \brief     Write the node values into the file
 * \param[in] fileOutput Output file
 * \param[in] paramType Parameter Type
 * \param[in] paramName Parameter Name
 *
 * Write the node values into the file.
 */
void CParameterValues::WriteNodeValuesToFile(fstream& fileOutput, string& paramType, string& paramName)
{
    fileOutput << "BA_ \"" << paramName.c_str() << "\"";
    fileOutput << " BU_ " << m_NodeName.c_str();

    //writes node values of type int/hex to o/p file.
    if((paramType == "INT") || (paramType == "HEX"))
    {
        if(m_ParamVal.iValue != -1)
        {
            fileOutput << " " << dec << m_ParamVal.iValue;
        }
    }
    //writes node values of type enum to o/p file.
    else if(paramType == "ENUM")
    {
        if(m_ParamVal.cValue.length() == 0)
        {
            fileOutput << " \"\"";
        }
        else
        {
            fileOutput << " \"" << m_ParamVal.cValue << "\"";
        }
    }
    //writes node values of type float to o/p file.
    else if(paramType == "FLOAT")
    {
        if(m_ParamVal.fValue != -1)
        {
            fileOutput << m_ParamVal.fValue;
        }
    }
    //writes node values of type string to o/p file.
    else if(paramType == "STRING")
    {
        fileOutput << " \"" << m_ParamVal.cValue << "\"";
    }

    fileOutput << ";" << endl;
}

/**
 * \brief     Write the Message values into the file
 * \param[in] fileOutput Output file
 * \param[in] paramType Parameter Type
 * \param[in] paramName Parameter Name
 *
 * Write the Message values into the file.
 */
void CParameterValues::WriteMesgValuesToFile(fstream& fileOutput, string& paramType, string& paramName)
{
    fileOutput << "BA_ \"" << paramName.c_str() << "\"";
    fileOutput << " BO_ " << dec << m_MsgId;

    //writes mesg values of type int/hex to o/p file.
    if((paramType == "INT") || (paramType == "HEX"))
    {
        if(m_ParamVal.iValue != -1)
        {
            fileOutput << " " << dec << m_ParamVal.iValue;
        }
    }
    //writes mesg values of type enum to o/p file.
    else if(paramType == "ENUM")
    {
        if(m_ParamVal.cValue.length() == 0)
        {
            fileOutput << " \"\"";
        }
        else
        {
            fileOutput << " " << m_ParamVal.cValue;
        }
    }
    //writes mesg values of type float to o/p file.
    else if(paramType == "FLOAT")
    {
        if(m_ParamVal.fValue != -1)
        {
            fileOutput << " " << m_ParamVal.fValue;
        }
    }
    //writes mesg values of type string to o/p file.
    else if(paramType == "STRING")
    {
        fileOutput << " \"" << m_ParamVal.cValue << "\"";
    }

    fileOutput << ";" << endl;
}

/**
 * \brief     Write the Signal values into the file
 * \param[in] fileOutput Output file
 * \param[in] paramType Parameter Type
 * \param[in] paramName Parameter Name
 *
 * Write the Signal values into the file.
 */
void CParameterValues::WriteSigValuesToFile(fstream& fileOutput, string& paramType, string& paramName)
{
    fileOutput << "BA_ \"" << paramName.c_str() << "\"";
    fileOutput << " SG_ " << dec << m_MsgId;
    fileOutput << " " << m_SignalName.c_str();

    //writes sig values of type int/hex to o/p file.
    if((paramType == "INT") || (paramType == "HEX"))
    {
        if(m_ParamVal.iValue != -1)
        {
            fileOutput << " " << dec << m_ParamVal.iValue;
        }
    }
    //writes sig values of type enum to o/p file.
    else if(paramType == "ENUM")
    {
        if(m_ParamVal.cValue.length() == 0)
        {
            fileOutput << " \"\"";
        }
        else
        {
            fileOutput << " " << m_ParamVal.cValue;
        }
    }
    //writes sig values of type flaot to o/p file.
    else if(paramType == "FLOAT")
    {
        if(m_ParamVal.fValue != -1)
        {
            fileOutput << " " << m_ParamVal.fValue;
        }
    }
    //writes sig values of type string to o/p file.
    else if(paramType == "STRING")
    {
        fileOutput << " \"" << m_ParamVal.cValue << "\"";
    }

    fileOutput << ";" << endl;
}
