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
 * \file      Parameter.cpp
 * \brief     Implementation file for the Parameter class.
 * \authors   Ratnadip Choudhury, Padmaja A, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for the Parameter class.
 */

#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers */

/* MFC includes */
#include <afxwin.h>         /* MFC core and standard components */
#include <afxext.h>         /* MFC extensions */
#include <afxdisp.h>        /* MFC Automation classes */
#include <afxdtctl.h>       /* MFC support for Internet Explorer 4 Common Controls */
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>         /* MFC support for Windows Common Controls */
#include <afxtempl.h>
#endif /* _AFX_NO_AFXCMN_SUPPORT */

/* C includes */
#include <float.h>
#include <limits.h>

/* Project includes */
#include "Definitions.h"
#include "Parameter.h"
#include "tag.h"

using namespace std;

/**
 * \brief Constructor
 *
 * Constructor of CParameter
 */
CParameter::CParameter()
{
    m_MaxVal.iValue = -1;
    m_MaxVal.uiValue = 0;
    m_MaxVal.fValue = -1;
    m_MaxVal.dValue = -1;
    m_MaxVal.cValue = "";
    m_MaxVal.i64Value = -1;
    m_MaxVal.ui64Value = 0;
    m_MinVal.iValue = -1;
    m_MinVal.uiValue = 0;
    m_MinVal.fValue = -1;
    m_MinVal.dValue = -1;
    m_MinVal.cValue = "";
    m_MinVal.i64Value = -1;
    m_MinVal.ui64Value = 0;
    m_InitVal.iValue = -1;
    m_InitVal.uiValue = 0;
    m_InitVal.fValue = -1;
    m_InitVal.dValue = -1;
    m_InitVal.cValue = "";
    m_InitVal.i64Value = -1;
    m_InitVal.ui64Value = 0;
    m_ValRange = "";
    m_ParamType = "";
    m_ParamName = "";
    m_RangeError = false;
}

/**
 * Destructor of CParameter
 */
CParameter::~CParameter()
{
    // nothing special to do here
}

/**
 * \brief     overloaded operator =
 * \param[in] param Other element to copy data from
 * \return    Local object with new data
 *
 * Copy the other elements of the new message to this.
 */
CParameter& CParameter::operator=( CParameter& param)
{
    // now copy the other elements of the new message to this
    m_ObjectId = param.m_ObjectId;
    m_ParamName = param.m_ParamName;
    m_ValRange=param.m_ValRange;
    m_ParamType=param.m_ParamType;
    m_InitVal=param.m_InitVal;
    m_MaxVal=param.m_MaxVal;
    m_MinVal=param.m_MinVal;
    m_RangeError=param.m_RangeError;

    for(int i=0; i<4; i++)
    {
        m_listParamValues[i] = param.m_listParamValues[i];
    }

    return (*this);
}

/**
 * \brief      Writes the parameter definition to the specified output file.
 * \param[in]  fileOutput Output File
 * \param[in]  m_listParameter Parameters List
 *
 * Writes the parameter definition to the specified output file.
 */
bool WriteParamToFile(fstream& fileOutput, list<CParameter> &m_listParameter)
{
    bool pResult=true;
    //gets the frst param in the list.
    list<CParameter>::iterator rParam;

    for(rParam=m_listParameter.begin(); rParam!=m_listParameter.end(); rParam++)
    {
        if(rParam->m_RangeError)
        {
            pResult=false;
        }

        //writes param def to the specified o/p file.
        fileOutput << "BA_DEF_ " << rParam->m_ObjectId;
        fileOutput << "  \"" << rParam->m_ParamName << "\"";
        fileOutput << " " << rParam->m_ParamType;

        if(rParam->m_ParamType == "INT")
        {
            fileOutput << " " << dec << rParam->m_MinVal.iValue;
            fileOutput << " " << dec << rParam->m_MaxVal.iValue;
        }
        else if(rParam->m_ParamType == "HEX")
        {
            fileOutput << " " << dec << rParam->m_MinVal.uiValue;
            fileOutput << " " << dec << rParam->m_MaxVal.uiValue;
        }
        else if(rParam->m_ParamType == "FLOAT")
        {
            fileOutput << " " << rParam->m_MinVal.fValue;
            fileOutput << " " << rParam->m_MaxVal.fValue;
        }
        else if(rParam->m_ParamType == "ENUM")
        {
            fileOutput << rParam->m_ValRange;
        }
        else
        {
            fileOutput << " ";
        }

        fileOutput << ";" << endl;
    }

    return pResult;
}

/**
 * \brief     Format Message Parameter Value
 * \param[in] fileInput Input file
 * \param[in] m_listParam List of CParameter
 *
 * Format the message parameter value and write to output file.
 */
void CParameter::Format_MesgParam_Value(fstream& fileInput, list<CParameter>& m_listParam)
{
    CParameterValues pVal;
    char acLine[defCON_MAX_LINE_LEN];
    char* pcToken, *pcLine;
    char acTemp[defCON_CHAR_LEN],*pcTemp;
    pcTemp = acTemp;

    //parses the mesg pram other values.
    while(fileInput.getline(acLine,defCON_MAX_LINE_LEN) && strcmp(acLine,"[END_PARAM_MSG_VAL]\n")!=0)
    {
        char type[defCON_CHAR_LEN];
        pcLine = acLine;
        //get mesg id.
        pcToken=strtok(pcLine,",");
        UINT msgId=unsigned int(strtoul(pcToken, NULL, 10));
        pcLine=pcLine+strlen(pcToken)+1;
        //get message type and validates teh type.
        pcToken=strtok(pcLine,",");
        strncpy(type, pcToken, sizeof(type));

        if(strcmp(type,"X")==0)
        {
            msgId=msgId-2147483648;
        }

        pcLine=pcLine+strlen(pcToken)+1;
        //get other param values.
        pcToken=strtok(pcLine,",");
        pcLine=pcLine+strlen(pcToken)+1;
        pcToken++;

        while(*pcToken && *pcToken!='"')
        {
            *pcTemp++=*pcToken++;
        }

        *pcTemp='\0';
        list<CParameter>::iterator rParam;

        for(rParam=m_listParam.begin(); rParam!=m_listParam.end(); rParam++)
            //assigns other values to the matching param defintion.
        {
            // find matching Parameter from list
            if(strcmp(rParam->m_ParamName.c_str(), acTemp)==0 )
            {
                pVal.Format_Param_Value(rParam->m_ParamType, pcLine, 2, msgId);
                rParam->m_listParamValues[2].push_back(pVal);
                pcTemp=acTemp;
                break;
            }
        }
    }
}

/**
 * \brief    Parses the Signal Parameter's Other Values.
 * \param[in] fileInput Input file
 * \param[in] m_listParam List of CParameter
 *
 * Parses the Signal Parameter's Other Values.
 */
void CParameter::Format_SigParam_Value(fstream& fileInput, list<CParameter>& m_listParam)
{
    CParameterValues pVal;
    char acLine[defCON_MAX_LINE_LEN];
    char* pcToken, *pcLine;
    char acTemp[defCON_CHAR_LEN],*pcTemp;
    pcTemp = acTemp;

    //parses the signal pram other values.
    while(fileInput.getline(acLine,defCON_MAX_LINE_LEN) && strcmp(acLine, "[END_PARAM_SIG_VAL]\n")!=0)
    {
        char temp[defCON_CHAR_LEN],sname[defCON_CHAR_LEN];
        pcLine = acLine;
        //get mesg id.
        pcToken=strtok(pcLine,",");
        UINT msgId=unsigned int(strtoul(pcToken, NULL, 10));
        pcLine=pcLine+strlen(pcToken)+1;
        //get message type and validates the mtype.
        pcToken=strtok(pcLine,",");
        strncpy(temp, pcToken, sizeof(temp));

        if(strcmp(temp,"X")==0)
        {
            msgId=msgId-2147483648;
        }

        pcLine=pcLine+strlen(pcToken)+1;
        //get signal name.
        pcToken=strtok(pcLine,",");
        strncpy(sname, pcToken, sizeof(sname));
        pcLine=pcLine+strlen(pcToken)+1;
        //get other values.
        pcToken=strtok(pcLine,",");
        pcLine=pcLine+strlen(pcToken)+1;
        pcToken++;

        while(*pcToken && *pcToken!='"')
        {
            *pcTemp++=*pcToken++;
        }

        *pcTemp='\0';
        //assigns other values to the matching param.
        list<CParameter>::iterator rParam;

        for(rParam=m_listParam.begin(); rParam!=m_listParam.end(); rParam++)
        {
            // find matching Parameter from list
            if(strcmp(rParam->m_ParamName.c_str(),acTemp)==0 )
            {
                pVal.Format_Param_Value(rParam->m_ParamType, pcLine, 3, msgId, sname);
                rParam->m_listParamValues[3].push_back(pVal);
                pcTemp=acTemp;
                break;
            }
        }
    }
}

/**
 * \brief     Parses the Node Parameter's Other Values
 * \param[in] fileInput Input file
 * \param[in] m_listParam List of CParameter
 *
 * Parses the Node Parameter's Other Values.
 */
void CParameter::Format_NodeParam_Value(fstream& fileInput, list<CParameter>& m_listParam)
{
    CParameterValues pVal;
    char acLine[defCON_MAX_LINE_LEN];
    char* pcToken, *pcLine;
    char acTemp[defCON_CHAR_LEN],*pcTemp;
    pcTemp = acTemp;

    //parses the node pram other values.
    while(fileInput.getline(acLine,defCON_MAX_LINE_LEN) && strcmp(acLine, "[END_PARAM_NODE_VAL]\n")!=0)
    {
        char NodeName[defCON_CHAR_LEN];
        pcLine = acLine;
        //get node name.
        pcToken=strtok(pcLine,",");
        strncpy(NodeName, pcToken, sizeof(NodeName));
        pcLine=pcLine+strlen(pcToken)+1;
        //get other values.
        pcToken=strtok(NULL,",");
        pcLine=pcLine+strlen(pcToken)+1;
        pcToken++;

        while(*pcToken && *pcToken!='"')
        {
            *pcTemp++=*pcToken++;
        }

        *pcTemp='\0';
        //assigns other values to the matching param.
        list<CParameter>::iterator rParam;

        for(rParam=m_listParam.begin(); rParam!=m_listParam.end(); rParam++)
        {
            // find matching Parameter from list
            if(strcmp(rParam->m_ParamName.c_str(), acTemp)==0)
            {
                pVal.Format_Param_Value(rParam->m_ParamType, pcLine, 1, 0, NodeName);
                rParam->m_listParamValues[1].push_back(pVal);
                pcTemp=acTemp;
                break;
            }
        }
    }
}

/**
 * \brief Parses the Net Parameter's Other Values
 * \param[in] fileInput Input file
 * \param[in] m_listParam List of CParameter
 *
 * Parses the Net Parameter's Other Values.
 */
void CParameter::Format_NetParam_Value(fstream& fileInput, list<CParameter>& m_listParam)
{
    CParameterValues pVal;
    char acLine[defCON_MAX_LINE_LEN];
    char* pcToken, *pcLine;
    char acTemp[defCON_CHAR_LEN],*pcTemp;
    pcTemp = acTemp;

    //parses the mesg pram other values.
    while(fileInput.getline(acLine, defCON_MAX_LINE_LEN) && strcmp(acLine, "[END_PARAM_NET_VAL]\n")!=0)
    {
        pcLine = acLine;
        //get other values.
        pcToken=strtok(pcLine,",");
        pcToken++;

        while(*pcToken && *pcToken!='"')
        {
            *pcTemp++=*pcToken++;
        }

        *pcTemp='\0';
        //assigns other values to the matching param.
        list<CParameter>::iterator rParam;

        for(rParam=m_listParam.begin(); rParam!=m_listParam.end(); rParam++)
        {
            // find matching Parameter from list
            if(strcmp(rParam->m_ParamName.c_str(),acTemp)==0 )
            {
                pVal.Format_Param_Value(rParam->m_ParamType, (pcLine+strlen(acTemp)+3), 0);
                rParam->m_listParamValues[0].push_back(pVal);
                pcTemp=acTemp;
                break;
            }
        }
    }
}


/**
 * \brief     Parses the attribute lines from the given i/p file.
 * \param[in] pcLine Attribute line
 * \param[in] index Index
 *
 * Parses the attribute lines from the given i/p file.
 */
void CParameter::Format_ParamDef(char* pcLine, int index)
{
    //get object id and stores m_object Id with the valid value.
    switch(index)
    {
        case 0:
            m_ObjectId = "";
            break;

        case 1:
            m_ObjectId = "BU_";
            break;

        case 2:
            m_ObjectId = "BO_";
            break;

        case 3:
            m_ObjectId = "SG_";
            break;

        case 4:
            m_ObjectId = "BU_SG_REL_";
            break;

        case 5:
            m_ObjectId = "BU_BO_REL_";
            break;
    }

    //reads the param defintion.
    GetParam_Def(pcLine);
}

/**
 * \brief     Parses the attribute value from the given i/p file
 * \param[in] pcLine Attribute value
 *
 * Parses the attribute value from the given i/p file.
 */
void CParameter::GetParam_Def(char* pcLine)
{
    char* pcToken;
    //get Param name
    pcToken = strtok(pcLine,"\"");
    m_ParamName = pcToken;
    //get Param type
    pcToken=strtok(NULL,",");
    m_ParamType = pcToken;

    if(m_ParamType == "STRING")
    {
        pcToken=strtok(NULL,"\n");
    }
    else
    {
        pcToken=strtok(NULL,",");
    }

    //get the default value of parameter
    ReadDefault_Value(pcToken);

    if(m_ParamType == "ENUM")
    {
        m_ValRange="";

        while(pcToken=strtok(NULL,"\n"))
        {
            m_ValRange=m_ValRange+" "+pcToken;
        }
    }
    //gets the min/max value and validates the range
    else if(m_ParamType == "HEX")
    {
        unsigned int min_val,max_val;
        pcToken=strtok(NULL,",");
        min_val=strtoul(pcToken, NULL, 10);
        pcToken = strtok(NULL," ");
        max_val= strtoul(pcToken, NULL, 10);
        m_RangeError |= isValid_hexRange(min_val,max_val);
    }
    //gets the min/max value and validates the range
    else if(m_ParamType == "FLOAT")
    {
        double min_val,max_val;
        pcToken=strtok(NULL,",");
        min_val= atof(pcToken);
        pcToken = strtok(NULL," ");
        max_val=atof(pcToken);
        m_RangeError |= isValid_floatRange(min_val,max_val);
    }
    //gets the min/max value and validates the range
    else if(m_ParamType == "INT")
    {
        LONGLONG min_val,max_val;
        pcToken=strtok(NULL,",");
        min_val=_atoi64(pcToken);
        pcToken = strtok(NULL," ");
        max_val=_atoi64(pcToken);
        m_RangeError |= isValid_intRange(min_val,max_val);
    }

    //validates the default value
    m_RangeError |= Check_Default_Value();
}

/**
 * \brief     Reads the default value of attribute from the i/p file
 * \param[in] pcToken Default value of attribute
 *
 * Reads the default value of attribute from the i/p file.
 */
void CParameter::ReadDefault_Value(char* pcToken)
{
    char acTemp[defCON_CHAR_LEN],*pcTemp;
    pcTemp = acTemp;

    //get inital value of type ENUM
    if(m_ParamType == "ENUM")
    {
        //omits all balnk spaces
        while(*pcToken && *pcToken == ' ')
        {
            *pcToken++;
        }

        pcToken++;

        while(*pcToken && *pcToken != '"')
        {
            *pcTemp++ = *pcToken++;
        }

        *pcTemp = '\0';
        //copies inital value to the object member.
        m_InitVal.cValue = acTemp;
        pcTemp=acTemp;
    }
    //get inital value of type INT
    else if(m_ParamType == "INT")
    {
        if(strcmp(pcToken," ")!=0)
        {
            m_InitVal.iValue =atoi(pcToken);
        }
    }
    //get inital value of type HEX
    else if(m_ParamType == "HEX")
    {
        if(strcmp(pcToken," ")!=0)
        {
            m_InitVal.uiValue =strtoul(pcToken, NULL, 10);
        }
        else
        {
            m_InitVal.uiValue =-1;
        }
    }
    //get inital value of type Float
    else if(m_ParamType == "FLOAT")
    {
        if(strcmp(pcToken," ")!=0)
        {
            m_InitVal.fValue=float(atof(pcToken));
        }
    }
    //get inital value of type string
    else if(m_ParamType == "STRING")
    {
        while(*pcToken && *pcToken == ' ')
        {
            *pcToken++;
        }

        while(*pcToken && *pcToken!='\n')
        {
            *pcTemp++=*pcToken++;
        }

        *pcTemp='\0';
        m_InitVal.cValue = acTemp;
        pcTemp=acTemp;
    }
}


/**
 * \brief     Writes the parameter default values to the output file
 * \param[in] fileOutput Output file
 * \param[in] m_listParameter List of CParameter
 * \return    Return code
 *
 * Writes the parameter default values to the output file.
 */
bool Write_DefVal_ToFile(fstream& fileOutput,list<CParameter> &m_listParameter)
{
    bool pResult=true;
    //gets the first param from the list.
    list<CParameter>::iterator rParam;

    for(rParam=m_listParameter.begin(); rParam!=m_listParameter.end(); rParam++)
    {
        //checks whether def val is with in the range or not.
        if(rParam->m_RangeError)
        {
            pResult=false;
        }

        //writes def val of type int to the o/p file.
        fileOutput << "BA_DEF_DEF_";
        fileOutput << "  \"" << rParam->m_ParamName << "\"";

        if(rParam->m_ParamType == "INT")
        {
            fileOutput << " " << dec << rParam->m_InitVal.iValue;
        }
        //writes def val of type ex to the o/p file.
        else if(rParam->m_ParamType == "HEX")
        {
            fileOutput << " " << dec << rParam->m_InitVal.uiValue;
        }
        //writes def val of type flaot to the o/p file.
        else if(rParam->m_ParamType == "FLOAT")
        {
            fileOutput << " " << rParam->m_InitVal.fValue;
        }
        //writes def val of type enum to the o/p file.
        else if(rParam->m_ParamType == "ENUM")
        {
            fileOutput << " \"" << rParam->m_InitVal.cValue << "\"";
        }
        //writes def val of type string to the o/p file.
        else
        {
            fileOutput << " " << rParam->m_InitVal.cValue;
        }

        fileOutput << ";" << endl;
    }

    return pResult;
}

/**
 * \brief  Validates the default value of an attribute
 * \return Status code
 *
 * Validates the default value of an attribute.
 */
bool CParameter::Check_Default_Value(void)
{
    bool cResult=false;

    //validates def val of type int
    if(m_ParamType == "INT")
    {
        if(m_InitVal.iValue<m_MinVal.iValue || m_InitVal.iValue>m_MaxVal.iValue)
        {
            m_InitVal.iValue=m_MinVal.iValue;
            cResult=true;
        }
    }
    //validates def val of type hex
    else if(m_ParamType == "HEX")
    {
        if(m_InitVal.uiValue<m_MinVal.uiValue || m_InitVal.uiValue>m_MaxVal.uiValue)
        {
            m_InitVal.uiValue=m_MinVal.uiValue;
            cResult= true;
        }
    }
    //validates def val of type float
    else if(m_ParamType == "FLOAT")
    {
        if(m_InitVal.fValue<m_MinVal.fValue || m_InitVal.fValue>m_MaxVal.fValue)
        {
            m_InitVal.fValue=m_MinVal.fValue;
            cResult=true;
        }
    }

    return cResult;
}

/**
 * \brief     Validates the maximum and minimum int values of an attribute
 * \param[in] minValue minimum value of attribute
 * \param[in] maxValue maximum value of attribute
 * \return    Status code
 *
 * Validates the maximum and minimum int values of an attribute.
 */
bool CParameter::isValid_intRange(long long int minValue, long long int maxValue)
{
    bool rResult=false;

    //validates the min value
    if(minValue < INT_MIN )
    {
        m_MinVal.iValue=INT_MIN;
        rResult=rResult|true;
    }
    else
    {
        m_MinVal.iValue =int(minValue);
    }

    //validates the max value
    if(maxValue>INT_MAX || maxValue<m_MinVal.iValue)
    {
        m_MaxVal.iValue=INT_MAX;
        rResult=rResult | true;
    }
    else
    {
        m_MaxVal.iValue=int(maxValue);
    }

    return rResult;
}

/**
 * \brief     Validates the maximum and minimum float values of an attribute
 * \param[in] minValue minimum value of an attribute
 * \param[in] maxValue maximum value of an attribute
 * \return    Status code
 *
 * Validates the maximum and minimum float values of an attribute.
 */
bool CParameter::isValid_floatRange(double minValue,double maxValue)
{
    bool rResult=false;

    //validates the min value
    if(minValue<FLT_MIN)
    {
        m_MinVal.fValue=FLT_MIN;
        rResult=rResult | true;
    }
    else
    {
        m_MinVal.fValue= float(minValue);
    }

    //validates the max value
    if(maxValue>FLT_MAX || maxValue<m_MinVal.fValue)
    {
        m_MaxVal.fValue=FLT_MAX;
        rResult=rResult | true;
    }
    else
    {
        m_MaxVal.fValue =float(maxValue);
    }

    return rResult;
}

/**
 * \brief     Validates the maximum and minimum hex values of an attribute.
 * \param[in] minValue Minimum hex value of an attribute
 * \param[in] maxValue Maximum hex value of an attribute
 * \return    Status code
 *
 * Validates the maximum and minimum hex values of an attribute.
 */
bool CParameter::isValid_hexRange(unsigned int minValue, unsigned int maxValue)
{
    bool rResult=false;
    //validates the min value
    m_MinVal.uiValue=minValue;

    //validates the max value
    if(maxValue==0 || maxValue <m_MinVal.uiValue)
    {
        m_MaxVal.uiValue=0xffffffff;
        rResult=true;
    }
    else
    {
        m_MaxVal.uiValue=maxValue;
    }

    return rResult;
}
