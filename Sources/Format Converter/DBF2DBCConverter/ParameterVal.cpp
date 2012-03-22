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
 * \author    RBIN/EBS1 - Padmaja A
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for the ParameterValues class.
 */
/**
* \file       ParameterVal.cpp
* \brief      Implementation file for the ParameterValues class.
* \authors    RBIN/EBS1 - Padmaja A
* \date       16.11.2004 Created
* \copyright  Copyright &copy; 2011 Robert Bosch Engineering and Business Solutions.  All rights reserved.
*/
#include "StdAfx.h"
#include <string.h>
#include "ParameterVal.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/**
* \brief      Constructor of CParameterValues
* \param[in]  None
* \param[out] None
* \return     None
* \authors    Padmaja.A
* \date       15.11.2004
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
* \brief      destructor of CParameterValues
* \param[in]  None
* \param[out] None
* \return     None
* \authors    Padmaja.A
* \date       16.11.2004
*/
CParameterValues::~CParameterValues()
{

}

/**
* \brief      overloaded operator =
* \param[in]  CParameterValues& param
* \param[out] None
* \return     CParameterValues& param
* \authors    Padmaja.A
* \date       16.11.2004
*/
CParameterValues& CParameterValues::operator=(CParameterValues& param)
{

    // now copy the other elements of the new message to this
    strncpy(m_NodeName, param.m_NodeName, sizeof(m_NodeName));
    strncpy(m_SignalName,param.m_SignalName, sizeof(m_SignalName));
    m_MsgId=param.m_MsgId;
    m_ParamVal=param.m_ParamVal;
    m_cFrameFormat=param.m_cFrameFormat;
    return (*this);
}

/**
* \brief      Parses the Parameter Values other than Default value
from the input file.
* \param[in]  char *paramType,char *pcLine,const int& index,UINT msgId,char *Node_Name
* \param[out] None
* \return     void
* \authors    Padmaja.A
* \date       16.11.2004
*/
void CParameterValues::Format_Param_Value(char *paramType,char *pcLine,const int& index,UINT msgId,char *Node_Name)
{
    //copies the node name to the member
    if(index==1)
        strncpy(m_NodeName, Node_Name, sizeof(m_NodeName));
    //copies the message id.
    else if(index==2)
        m_MsgId=msgId;
    //store the signal name.
    else if(index==3)
    {
        m_MsgId =msgId;
        strncpy(m_SignalName, Node_Name, sizeof(m_SignalName));
    }
    //get param value.
    ReadParamValue(paramType,pcLine);
}

/**
* \brief      Reads the other vlaue of attributes from i/p file.
* \param[in]  char *paramType,char *pcToken
* \param[out] None
* \return     void
* \authors    Padmaja.A
* \date       16.11.2004
*/
void CParameterValues::ReadParamValue(char *paramType,char *pcToken)
{
    char acTemp[defCON_CHAR_LEN],*pcTemp;
    pcTemp = acTemp;
    //get param value of type String
    if(strcmp(paramType,"STRING")==0 )
    {   pcToken++;
        while(*pcToken != '"')
        {
            *pcTemp++=*pcToken++;
        }
        *pcTemp='\0';
        strncpy(m_ParamVal.cValue, acTemp, sizeof(m_ParamVal.cValue));
        pcTemp=acTemp;
    }
    //get param value of type int
    else if(strcmp(paramType,"INT")==0 || strcmp(paramType,"HEX")==0)
    {
        if(strcmp(pcToken," ")!=0)
            m_ParamVal.iValue=atoi(pcToken);
    }
    //get param value of type enum
    else if (strcmp(paramType,"ENUM")==0)
    {
        *pcToken++;
        while(*pcToken!='"')
        {
            *pcTemp++=*pcToken++;
        }
        *pcTemp='\0';
        strncpy(m_ParamVal.cValue, acTemp, sizeof(m_ParamVal.cValue));
        pcTemp=acTemp;
    }
    //get param value of type float
    else if(strcmp(paramType,"FLOAT")==0)
    {
        if(strcmp(pcToken," ")!=0)
            m_ParamVal.fValue=float(atof(pcToken));
    }

}

/**
* \brief      Write the net values into the file
* \param[in]  CStdioFile& fileOutput,char *paramType,char *paramName
* \param[out] None
* \return     void
* \authors    Padmaja.A
* \date       16.11.2004
*/
void CParameterValues::WriteNetValuesToFile(CStdioFile& fileOutput,char *paramType,char *paramName)
{
    char acLine[defCON_MAX_LINE_LEN];
    //writes net values of type int to o/p file.
    if(strcmp(paramType,"INT")==0 || strcmp(paramType,"HEX")==0)
    {
        if(m_ParamVal.iValue ==-1)
            sprintf(acLine,"BA_ \"%s\";\n",paramName);
        else
            sprintf(acLine,"BA_ \"%s\" %d;\n",paramName,m_ParamVal.iValue);
    }
    //writes net values of type float to o/p file.
    else if(strcmp(paramType,"FLOAT")==0)
    {
        if(m_ParamVal.fValue==-1)
            sprintf(acLine,"BA_ \"%s\";\n",paramName);
        else
            sprintf(acLine,"BA_ \"%s\" %f;\n",paramName,m_ParamVal.fValue);
    }
    //writes net values of type enum to o/p file.
    else if(strcmp(paramType,"ENUM")==0)
    {
        if(strcmp(m_ParamVal.cValue,"")==0)
            sprintf(acLine,"BA_ \"%s\" """,paramName);
        else
            sprintf(acLine,"BA_ \"%s\" %s;\n",paramName,m_ParamVal.cValue);
    }
    //writes net values of type string to o/p file.
    else if(strcmp(paramType,"STRING")==0)
        sprintf(acLine,"BA_ \"%s\" \"%s\";\n",paramName,m_ParamVal.cValue);
    fileOutput.WriteString(acLine);

}

/**
* \brief      Write the node values into the file
* \param[in]  CStdioFile& fileOutput,char *paramType,char *paramName
* \param[out] None
* \return     void
* \authors    Padmaja.A
* \date       16.11.2004
*/
void CParameterValues::WriteNodeValuesToFile(CStdioFile& fileOutput,char *paramType,char *paramName)
{
    char acLine[defCON_MAX_LINE_LEN];
    //writes node values of type int/hex to o/p file.
    if(strcmp(paramType,"INT")==0  || strcmp(paramType,"HEX")==0)
    {
        if(m_ParamVal.iValue ==-1)
            sprintf(acLine,"BA_ \"%s\" BU_ %s;\n",paramName,m_NodeName);
        else
            sprintf(acLine,"BA_ \"%s\" BU_ %s %d;\n",paramName,m_NodeName,m_ParamVal.iValue);
    }
    //writes node values of type enum to o/p file.
    else if(strcmp(paramType,"ENUM")==0)
    {
        if(strcmp(m_ParamVal.cValue,"")==0)
            sprintf(acLine,"BA_ \"%s\" %s "";",paramName,m_NodeName);
        else
            sprintf(acLine,"BA_ \"%s\" BU_ %s %s;\n", paramName,m_NodeName, m_ParamVal.cValue);
    }
    //writes node values of type float to o/p file.
    else if(strcmp(paramType,"FLOAT")==0)
    {
        if(m_ParamVal.fValue==-1)
            sprintf(acLine,"BA_ \"%s\" BU_ %s;\n",paramName,m_NodeName);
        else
            sprintf(acLine,"BA_ \"%s\" BU_ %s %f;\n",paramName,m_NodeName,m_ParamVal.fValue);
    }
    //writes node values of type string to o/p file.
    else if(strcmp(paramType,"STRING")==0)
        sprintf(acLine,"BA_ \"%s\" BU_ %s \"%s\";\n",paramName,m_NodeName,m_ParamVal.cValue);
    fileOutput.WriteString(acLine);

}

/**
* \brief      Write the Message values into the file
* \param[in]  CStdioFile& fileOutput,char *paramType,char *paramName
* \param[out] None
* \return     void
* \authors    Padmaja.A
* \date       16.11.2004
*/
void CParameterValues::WriteMesgValuesToFile(CStdioFile& fileOutput,char *paramType,char *paramName)
{
    char acLine[defCON_MAX_LINE_LEN];
    //writes mesg values of type int/hex to o/p file.
    if(strcmp(paramType,"INT")==0   || strcmp(paramType,"HEX")==0)
    {
        if(m_ParamVal.iValue ==-1)
            sprintf(acLine,"BA_ \"%s\" BO_ %u;\n",paramName,m_MsgId);
        else
            sprintf(acLine,"BA_ \"%s\" BO_ %u %d;\n",paramName,m_MsgId , m_ParamVal.iValue);
    }
    //writes mesg values of type enum to o/p file.
    else if(strcmp(paramType,"ENUM")==0)
    {
        if(strcmp(m_ParamVal.cValue,"")==0)
            sprintf(acLine,"BA_ \"%s\" BO_ %u "";\n",paramName,m_MsgId);
        else
            sprintf(acLine,"BA_ \"%s\" BO_ %u %s;\n", paramName,m_MsgId ,m_ParamVal.cValue);
    }
    //writes mesg values of type float to o/p file.
    else if(strcmp(paramType,"FLOAT")==0)
    {
        if(m_ParamVal.fValue==-1)
            sprintf(acLine,"BA_ \"%s\" BO_ %u;\n",paramName,m_MsgId);
        else
            sprintf(acLine,"BA_ \"%s\" BO_ %u %f;\n",paramName,m_MsgId,m_ParamVal.fValue);
    }
    //writes mesg values of type string to o/p file.
    else if(strcmp(paramType,"STRING")==0)
        sprintf(acLine,"BA_ \"%s\" BO_ %u \"%s\";\n",paramName,m_MsgId,m_ParamVal.cValue);
    fileOutput.WriteString(acLine);
}

/**
* \brief      Write the Signal values into the file
* \param[in]  CStdioFile& fileOutput,char *paramType,char *paramName
* \param[out] None
* \return     void
* \authors    Padmaja.A
* \date       16.11.2004
*/
void CParameterValues::WriteSigValuesToFile(CStdioFile& fileOutput,char *paramType,char *paramName)
{
    char acLine[defCON_MAX_LINE_LEN];
    //writes sig values of type int/hex to o/p file.
    if(strcmp(paramType,"INT")==0 || strcmp(paramType,"HEX")==0)
    {
        if(m_ParamVal.iValue ==-1)
            sprintf(acLine,"BA_ \"%s\" SG_ %u %s;\n",paramName,m_MsgId,m_SignalName);
        else
            sprintf(acLine,"BA_ \"%s\" SG_ %u %s %d;\n",paramName,m_MsgId,m_SignalName ,m_ParamVal.iValue);
    }
    //writes sig values of type enum to o/p file.
    else if(strcmp(paramType,"ENUM")==0)
    {
        if(strcmp(m_ParamVal.cValue,"")==0)
            sprintf(acLine,"BA_ \"%s\" SG_ %u %s "";\n",paramName,m_MsgId,m_SignalName);
        else
            sprintf(acLine,"BA_ \"%s\" SG_ %u %s %s;\n",paramName,m_MsgId ,m_SignalName ,m_ParamVal.cValue);
    }
    //writes sig values of type flaot to o/p file.
    else if(strcmp(paramType,"FLOAT")==0)
    {
        if(m_ParamVal.fValue==-1)
            sprintf(acLine,"BA_ \"%s\" SG_ %u %s;\n",paramName,m_MsgId,m_SignalName);
        else
            sprintf(acLine,"BA_ \"%s\" SG_ %u %s %f;\n",paramName,m_MsgId ,m_SignalName ,m_ParamVal.fValue);
    }
    //writes sig values of type string to o/p file.
    else if(strcmp(paramType,"STRING")==0)
        sprintf(acLine,"BA_ \"%s\" SG_ %u %s \"%s\";\n",paramName,m_MsgId ,m_SignalName,m_ParamVal.cValue);
    fileOutput.WriteString(acLine);
}
