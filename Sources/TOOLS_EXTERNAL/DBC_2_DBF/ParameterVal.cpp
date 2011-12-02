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
 * \author    Padmaja A
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for the ParameterValues class.
 */

#include "StdAfx.h"
#include "ParameterVal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/**
 * \brief Default Constructor used to initialse parameterVal class object.
 */
CParameterValues::CParameterValues()
{
    m_NodeName[0]='\0';
    m_MsgId=0;
    m_ParamVal.dValue =0;
    m_SignalName[0]='\0';
    m_ParamVal.iValue =-1;
    m_ParamVal.fValue=-1;
    m_ParamVal.uiValue =-1;
}

/**
 * \brief      Destructor
 */
CParameterValues::~CParameterValues()
{

}

/**
 * \brief copy operator
 */
CParameterValues& CParameterValues::operator=(CParameterValues& param)
{

    // now copy the other elements of the new message to this
    strcpy(m_NodeName,param.m_NodeName);
    strcpy(m_SignalName,param.m_SignalName );
    m_MsgId=param.m_MsgId;
    m_ParamVal=param.m_ParamVal;
    m_cFrameFormat=param.m_cFrameFormat;
    return (*this);
}

/**
 * \brief Parses the Node Parameter Values
 *
 * Parses the Node Parameter Values other than Default value from the input CanoeDB file.
 */
int CParameterValues::GetNodeParams(char *paramType,char *pcLine)
{
    char *pcToken;
    char acTemp[defCON_TEMP_LEN],*pcTemp;
    pcTemp = acTemp;
    int success=1;
    //reads the nodes name
    pcToken=strtok(pcLine,";");
    while(*pcToken && *pcToken != ' ')
    {
        *pcTemp++ = *pcToken++;
    }
    *pcTemp = '\0';
    strcpy(m_NodeName,acTemp);
    pcTemp=acTemp;
    //Gets the node param value.
    ReadParamValue(paramType,pcToken);
    return success;;
}

/**
 * \brief Parses the Network Parameter Values
 *
 * Parses the Network Parameter Values other than Default value from the input CanoeDB file.
 */
int CParameterValues::GetNetParams(char *paramType,char *pcLine)
{
    char *pcToken=pcLine;
    int success=1;
    //reads the net param value.
    pcToken=strtok(pcToken,";");
    ReadParamValue(paramType,pcToken);
    return success;
}

/**
 * \brief Parses the Message Parameter
 *
 * This function Parses the Message Parameter other Values rather than Default value from the input CanoeDB
 * file and finds the frame foramt for that Message ID.
 */
int CParameterValues::GetMesgParams( char *paramType,char *pcLine)
{
    char *pcToken;
    char acTemp[defCON_TEMP_LEN],*pcTemp;
    int success=1;
    pcTemp = acTemp;
    //get Message Id.
    pcToken=strtok(pcLine,";");
    while(*pcToken && *pcToken != ' ')
    {
        *pcTemp++ = *pcToken++;
    }
    *pcTemp = '\0';
    m_MsgId=(unsigned int)atoi(acTemp);
    pcTemp=acTemp;

    //Validates the MsgId to get frame format.
    if(m_MsgId < 0x80000000UL)
    {
        m_cFrameFormat =CParameterValues::MSG_FF_STANDARD;
    }
    else
    {
        m_cFrameFormat = CParameterValues::MSG_FF_EXTENDED;
        m_MsgId &= 0x7FFFFFFF;
    }
    //pcToken=strtok(NULL,";");
    //get the mesg param value.
    ReadParamValue(paramType,pcToken);
    return success;
}

/**
 * \brief Parses the Signal Parameter Values
 *
 * Parses the Signal Parameter Values other than Default value from the
 * input CanoeDB file and calculates the frame format
 * for the corresponding Message ID.
 */
int CParameterValues::GetSignalParams(char *paramType,char *pcLine)
{
    char *pcToken;
    char acTemp[defCON_TEMP_LEN],*pcTemp;
    int success=1;
    pcTemp = acTemp;
    pcToken=strtok(pcLine," ");
    //get the message Id.
    while(*pcToken && *pcToken != ' ')
    {
        *pcTemp++ = *pcToken++;
    }
    *pcTemp = '\0';
    m_MsgId=atoi(acTemp);
    pcTemp=acTemp;
    pcToken=strtok(NULL,";");

    //get signal name.
    while(*pcToken && *pcToken != ' ')
    {
        *pcTemp++ = *pcToken++;
    }
    *pcTemp = '\0';
    strcpy(m_SignalName,acTemp);
    pcTemp=acTemp;

    //validates the msgId to get the frame format.
    if(m_MsgId < 0x80000000UL)
    {
        m_cFrameFormat =CParameterValues::MSG_FF_STANDARD;
    }
    else
    {
        m_cFrameFormat = CParameterValues::MSG_FF_EXTENDED;
        m_MsgId &= 0x7FFFFFFF;
    }
    //get signal param value.
    ReadParamValue(paramType,pcToken);

    return success;
}

/**
 * \brief Reads the other vlaue of attributes from CanoeDB file.
 *
 * Reads the other vlaue of attributes from CanoeDB file.
 */
int CParameterValues::ReadParamValue(char *paramType,char *pcToken)
{
    int success=1;

    //Param type :STRING
    if(strcmp(paramType,"STRING")==0 )
    {
        while(*pcToken != '"')
        {
            *pcToken++;
        }
        strcpy(m_ParamVal.cValue,pcToken);
    }

    //Param type :INT/HEX
    else if(strcmp(paramType,"INT")==0 || strcmp(paramType,"HEX")==0)
    {
        if(strcmp(pcToken," ")!=0)
            m_ParamVal.iValue=atoi(pcToken);
    }
    //Param type :ENUM
    else if (strcmp(paramType,"ENUM")==0)
    {
        while(*pcToken==' ')
            *pcToken++;
        strcpy(m_ParamVal.cValue,pcToken);
    }

    //Param type :FLOAT
    else if(strcmp(paramType,"FLOAT")==0)
    {
        if(strcmp(pcToken," ")!=0)
            m_ParamVal.fValue=float(atof(pcToken));
    }

    return success;
}

/**
 * \brief Writes network values to file
 *
 * Writes network values to file.
 */
void CParameterValues::WriteNetValuesToFile(CStdioFile& fileOutput,char *paramType,char *paramName)
{
    char acLine[defVAL_MAX_LINE_LEN];

    //writes int/hex param net values to o/p file and validates initial value.
    if(strcmp(paramType,"INT")==0 || strcmp(paramType,"HEX")==0)
    {
        if(m_ParamVal.iValue ==-1)
            sprintf(acLine,"\"%s\",\n",paramName);
        else
            sprintf(acLine,"\"%s\",%d\n",paramName,m_ParamVal.iValue);
    }
    //writes Float param net values to o/p file and validates initial value.
    else if(strcmp(paramType,"FLOAT")==0)
    {
        if(m_ParamVal.fValue==-1)
            sprintf(acLine,"\"%s\",\n",paramName);
        else
            sprintf(acLine,"\"%s\",%f\n",paramName,m_ParamVal.fValue);
    }
    //writes enum param net values to o/p file and validates initial value.
    else if(strcmp(paramType,"ENUM")==0)
    {
        if(strcmp(m_ParamVal.cValue,"")==0)
            sprintf(acLine,"\"%s\",\"\"\n",paramName);
        else
            sprintf(acLine,"\"%s\",\"%s\"\n",paramName,m_ParamVal.cValue);
    }
    //writes string param net values to o/p file and validates initial value.
    else if(strcmp(paramType,"STRING")==0)
        sprintf(acLine,"\"%s\",%s\n",paramName,m_ParamVal.cValue);
    fileOutput.WriteString(acLine);
}

/**
 * \brief Writes node value to file
 *
 * Writes node value to file.
 */
void CParameterValues::WriteNodeValuesToFile(CStdioFile& fileOutput,char *paramType,char *paramName)
{
    char acLine[defVAL_MAX_LINE_LEN];
    //writes int/hex param node values to o/p file and validates initial value.
    if(strcmp(paramType,"INT")==0  || strcmp(paramType,"HEX")==0)
    {
        if(m_ParamVal.iValue ==-1)
            sprintf(acLine,"%s,\"%s\",\n",m_NodeName,paramName);
        else
            sprintf(acLine,"%s,\"%s\",%d\n",m_NodeName,paramName,m_ParamVal.iValue);
    }
    //writes enum param node values to o/p file and validates initial value.
    else if(strcmp(paramType,"ENUM")==0)
    {
        if(strcmp(m_ParamVal.cValue,"")==0)
            sprintf(acLine,"%s,\"%s\",\"\"\n", m_NodeName,paramName);
        else
            sprintf(acLine,"%s,\"%s\",\"%s\"\n", m_NodeName,paramName, m_ParamVal.cValue);
    }
    //writes float param node values to o/p file and validates initial value.
    else if(strcmp(paramType,"FLOAT")==0)
    {
        if(m_ParamVal.fValue==-1)
            sprintf(acLine,"%s,\"%s\",\n",m_NodeName,paramName);
        else
            sprintf(acLine,"%s,\"%s\",%f\n",m_NodeName,paramName,m_ParamVal.fValue);
    }
    //writes string param node values to o/p file and validates initial value.
    else if(strcmp(paramType,"STRING")==0)
        sprintf(acLine,"%s,\"%s\",%s\n",m_NodeName,paramName,m_ParamVal.cValue);
    fileOutput.WriteString(acLine);
}

/**
 * \brief Writes message values to file
 *
 * Writes message values to file.
 */
void CParameterValues::WriteMesgValuesToFile(CStdioFile& fileOutput,char *paramType,char *paramName)
{
    char acLine[defVAL_MAX_LINE_LEN];
    //writes int/hex param mesg values to o/p file and validates initial value.
    if(strcmp(paramType,"INT")==0   || strcmp(paramType,"HEX")==0)
    {
        if(m_ParamVal.iValue ==-1)
            sprintf(acLine,"%u,%c,\"%s\",\n",m_MsgId ,m_cFrameFormat,paramName);
        else
            sprintf(acLine,"%u,%c,\"%s\",%d\n",m_MsgId , m_cFrameFormat,paramName, m_ParamVal.iValue);
    }
    //writes enum param mesg values to o/p file and validates initial value.
    else if(strcmp(paramType,"ENUM")==0)
    {
        if(strcmp(m_ParamVal.cValue,"")==0)
            sprintf(acLine,"%u,%c,\"%s\",\"\"\n", m_MsgId ,m_cFrameFormat,paramName);
        else
            sprintf(acLine,"%u,%c,\"%s\",\"%s\"\n", m_MsgId , m_cFrameFormat,paramName,m_ParamVal.cValue);
    }
    //writes float param mesg values to o/p file and validates initial value.
    else if(strcmp(paramType,"FLOAT")==0)
    {
        if(m_ParamVal.fValue==-1)
            sprintf(acLine,"%u,%c,\"%s\",\n",m_MsgId,m_cFrameFormat ,paramName);
        else
            sprintf(acLine,"%u,%c,\"%s\",%f\n",m_MsgId,m_cFrameFormat ,paramName,m_ParamVal.fValue);
    }
    //writes string param mesg values to o/p file and validates initial value.
    else if(strcmp(paramType,"STRING")==0)
        sprintf(acLine,"%u,%c,\"%s\",%s\n",m_MsgId,m_cFrameFormat ,paramName,m_ParamVal.cValue);
    fileOutput.WriteString(acLine);
}

/**
 * \brief Writes signal values to file
 *
 * Writes signal values to file.
 */
void CParameterValues::WriteSigValuesToFile(CStdioFile& fileOutput,char *paramType,char *paramName)
{
    char acLine[defVAL_MAX_LINE_LEN];
    //writes int/hex param sig values to o/p file and validates initial value.
    if(strcmp(paramType,"INT")==0 || strcmp(paramType,"HEX")==0)
    {
        if(m_ParamVal.iValue ==-1)
            sprintf(acLine,"%u,%c,%s,\"%s\",\n",m_MsgId,m_cFrameFormat,m_SignalName ,paramName);
        else
            sprintf(acLine,"%u,%c,%s,\"%s\",%d\n",m_MsgId,m_cFrameFormat,m_SignalName ,paramName,m_ParamVal.iValue);
    }
    //writes enum param sig values to o/p file and validates initial value.
    else if(strcmp(paramType,"ENUM")==0)
    {
        if(strcmp(m_ParamVal.cValue,"")==0)
            sprintf(acLine,"%u,%c,%s,\"%s\",\"\"\n",m_MsgId,m_cFrameFormat,m_SignalName ,paramName);
        else
            sprintf(acLine,"%u,%c,%s,\"%s\",\"%s\"\n",m_MsgId,m_cFrameFormat,m_SignalName ,paramName,m_ParamVal.cValue);
    }

    //writes float param sig values to o/p file and validates initial value.
    else if(strcmp(paramType,"FLOAT")==0)
    {
        if(m_ParamVal.fValue==-1)
            sprintf(acLine,"%u,%c,%s,\"%s\",\n",m_MsgId,m_cFrameFormat,m_SignalName ,paramName);
        else
            sprintf(acLine,"%u,%c,%s,\"%s\",%f\n",m_MsgId,m_cFrameFormat,m_SignalName ,paramName,m_ParamVal.fValue);
    }
    //writes string param sig values to o/p file and validates initial value.
    else if(strcmp(paramType,"STRING")==0)
        sprintf(acLine,"%u,%c,%s,\"%s\",%s\n",m_MsgId ,m_cFrameFormat,m_SignalName,paramName,m_ParamVal.cValue);
    fileOutput.WriteString(acLine);
}
