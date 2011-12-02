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
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
/*********************************************************************
Project       :  CANDb convertor
FileName      :  Parameter.Cpp
Description   :  Implementation file for the Parameter class.

$Log:   X:/Archive/Sources/Ext_tools_DBF_2_DBC/Parameter.cpv  $

      Rev 1.1   04 Aug 2011 19:55:42   CANMNTTM


    Rev 1.0   03 Aug 2011 15:47:50   rac2kor

Author(s)     :  RBIN/EBS1 - Padmaja A
Date Created  :  15-11-2004
Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
*********************************************************************/
/**
* \file       Parameter.cpp
* \brief      Implementation file for the Parameter class.
* \authors    RBIN/EBS1 - Padmaja A
* \date       15.11.2004 Created
* \copyright  Copyright &copy; 2011 Robert Bosch Engineering and Business Solutions.  All rights reserved.
*/
#include "StdAfx.h"
#include "App.h"
#include "Parameter.h"
#include<limits.h>
#include<float.h>
#include <string>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/**
* \brief      Constructor of CParameters
* \param[in]  None
* \param[out] None
* \return     None
* \authors    Mahesh.B.S
* \date       15.11.2004
*/
CParameters::CParameters()
{
    m_InitVal.fValue=-1;
    m_InitVal.cValue[0]='\0';
    m_InitVal.uiValue=0;
    m_ValRange='\0';
    m_ParamType[0]='\0';
    m_ParamName[0]='\0';
    m_InitVal.iValue =-1;
    m_RangeError=false;

}

/**
* \brief      Destructor of CParameters
* \param[in]  None
* \param[out] None
* \return     None
* \authors    Mahesh.B.S
* \date       15.11.2004
*/
CParameters::~CParameters()
{
    // nothing special to do here
}

/**
* \brief      overloaded operator =
* \param[in]  CParameters&
* \param[out] None
* \return     CParameters&
* \authors    Padmaja.A
* \date       15.11.2004
*/
CParameters& CParameters::operator=( CParameters& param)
{
    // if there are some elements in the signal list clear them first
    if(!m_listParamValues[0].IsEmpty())
    {
        m_listParamValues[0].RemoveAll();
    }

    // now copy the other elements of the new message to this
    strcpy(m_ObjectId,param.m_ObjectId);
    strcpy(m_ParamName,param.m_ParamName);
    m_ValRange=param.m_ValRange;
    strcpy(m_ParamType,param.m_ParamType);
    m_InitVal=param.m_InitVal;
    m_MaxVal=param.m_MaxVal;
    m_MinVal=param.m_MinVal;
    m_RangeError=param.m_RangeError;
    for(int i=0; i<4; i++)
        m_listParamValues[i].AddTail( (&param.m_listParamValues[i]));
    return (*this);
}

/**
* \brief      Writes the parameter definition to the specified output file.
* \param[in]  OutputFileName,Parameters List.
* \param[out] None
* \return     CParameters&
* \authors    Padmaja.A
* \date       15.11.2004
*/
bool WriteParamToFile(CStdioFile& fileOutput,CList<CParameters,CParameters&> &m_listParameter)
{
    bool pResult=true;
    char acLine[defCON_MAX_LINE_LEN];

    //gets the frst param in the list.
    POSITION pos=m_listParameter .GetHeadPosition();
    while(pos!=NULL)
    {
        CParameters& rParam=m_listParameter.GetNext(pos);
        if(rParam.m_RangeError)
            pResult=false;
        //writes param def to the specified o/p file.
        if(strcmp(rParam.m_ParamType,"INT")==0 )
            sprintf(acLine,"BA_DEF_ %s  \"%s\" %s %d %d;\n",rParam.m_ObjectId,rParam.m_ParamName,rParam.m_ParamType, rParam.m_MinVal.iValue,rParam.m_MaxVal.iValue);
        else if(strcmp(rParam.m_ParamType,"HEX")==0)
            sprintf(acLine,"BA_DEF_ %s  \"%s\" %s %u %u;\n",rParam.m_ObjectId,rParam.m_ParamName,rParam.m_ParamType,rParam.m_MinVal.uiValue,rParam.m_MaxVal.uiValue);
        else if(strcmp(rParam.m_ParamType,"FLOAT")==0)
            sprintf(acLine,"BA_DEF_ %s  \"%s\" %s %f %f;\n",rParam.m_ObjectId,rParam.m_ParamName,rParam.m_ParamType,rParam.m_MinVal.fValue,rParam.m_MaxVal.fValue);
        else if(strcmp(rParam.m_ParamType,"ENUM")==0)
            sprintf(acLine,"BA_DEF_ %s  \"%s\" %s%s;\n",rParam.m_ObjectId,rParam.m_ParamName,rParam.m_ParamType,rParam.m_ValRange);
        else
            sprintf(acLine,"BA_DEF_ %s  \"%s\" %s ;\n",rParam.m_ObjectId,rParam.m_ParamName,rParam.m_ParamType);
        fileOutput.WriteString(acLine);
    }
    return pResult;
}

/**
* \brief      Format_MesgParam_Value
* \param[in]  CStdioFile &fileInput,CList<CParameters,CParameters&>& m_listParam
* \param[out] None
* \return     void
* \authors    Padmaja.A
* \date       16.11.2004
*/
void CParameters::Format_MesgParam_Value(CStdioFile &fileInput,CList<CParameters,CParameters&>& m_listParam)
{
    CParameterValues pVal;
    char acLine[defCON_MAX_LINE_LEN];
    char *pcToken, *pcLine;
    char acTemp[defCON_CHAR_LEN],*pcTemp;
    pcTemp = acTemp;
    //parses the mesg pram other values.
    while(strcmp((fileInput.ReadString(acLine,defCON_MAX_LINE_LEN)),"[END_PARAM_MSG_VAL]\n")!=0)
    {
        char type[defCON_CHAR_LEN];
        pcLine = acLine;
        //get mesg id.
        pcToken=strtok(pcLine,",");
        UINT msgId=unsigned int(atoi(pcToken));
        pcLine=pcLine+strlen(pcToken)+1;
        //get message type and validates teh type.
        pcToken=strtok(pcLine,",");
        strcpy(type,pcToken);
        if(strcmp(type,"X")==0)
            msgId=msgId+2147483648;
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
        POSITION posMsg = m_listParam.GetHeadPosition();
        //assigns other values to the matching param defintion.
        while(posMsg != NULL)
        {
            CParameters& rParam = m_listParam.GetNext(posMsg);
            // find matching Parameter from list
            if(strcmp(rParam.m_ParamName,acTemp)==0 )
            {
                pVal.Format_Param_Value(rParam.m_ParamType,pcLine,2,msgId);
                rParam.m_listParamValues[2].AddTail(pVal);
                pcTemp=acTemp;
                break;
            }
        }
    }
}

/**
* \brief      Parses the Signal Parameter's Other Values(BA_).
* \param[in]  CStdioFile &fileInput,CList<CParameters,CParameters&>& m_listParam
* \param[out] None
* \return     void
* \authors    Padmaja.A
* \date       16.11.2004
*/
void CParameters::Format_SigParam_Value(CStdioFile &fileInput,CList<CParameters,CParameters&>& m_listParam)
{
    CParameterValues pVal;
    char acLine[defCON_MAX_LINE_LEN];
    char *pcToken, *pcLine;
    char acTemp[defCON_CHAR_LEN],*pcTemp;
    pcTemp = acTemp;
    //parses the signal pram other values.
    while(strcmp((fileInput.ReadString(acLine,defCON_MAX_LINE_LEN)),"[END_PARAM_SIG_VAL]\n")!=0)
    {
        char temp[defCON_CHAR_LEN],sname[defCON_CHAR_LEN];
        pcLine = acLine;
        //get mesg id.
        pcToken=strtok(pcLine,",");
        UINT msgId=unsigned int(atoi(pcToken));
        pcLine=pcLine+strlen(pcToken)+1;
        //get message type and validates the mtype.
        pcToken=strtok(pcLine,",");
        strcpy(temp,pcToken);
        if(strcmp(temp,"X")==0)
            msgId=msgId+2147483648;
        pcLine=pcLine+strlen(pcToken)+1;
        //get signal name.
        pcToken=strtok(pcLine,",");
        if(strlen(pcToken) > 32)
            Truncate_str("signal name",pcToken,false);
        strcpy(sname,pcToken);
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
        POSITION posMsg = m_listParam.GetHeadPosition();
        while(posMsg != NULL)
        {
            CParameters& rParam = m_listParam.GetNext(posMsg);
            // find matching Parameter from list
            if(strcmp(rParam.m_ParamName,acTemp)==0 )
            {
                pVal.Format_Param_Value(rParam.m_ParamType,pcLine,3,msgId,sname);
                rParam.m_listParamValues[3].AddTail(pVal);
                pcTemp=acTemp;
                break;
            }
        }
    }
}

/**
* \brief      Parses the Node Parameter's Other Values(BA_).
* \param[in]  CStdioFile &fileInput,CList<CParameters,CParameters&>& m_listParam
* \param[out] None
* \return     void
* \authors    Padmaja.A
* \date       16.11.2004
*/
void CParameters::Format_NodeParam_Value(CStdioFile &fileInput,CList<CParameters,CParameters&>& m_listParam)
{
    CParameterValues pVal;
    char acLine[defCON_MAX_LINE_LEN];
    char *pcToken, *pcLine;
    char acTemp[defCON_CHAR_LEN],*pcTemp;
    pcTemp = acTemp;
    //parses the node pram other values.
    while(strcmp((fileInput.ReadString(acLine,defCON_MAX_LINE_LEN)),"[END_PARAM_NODE_VAL]\n")!=0)
    {
        char NodeName[defCON_CHAR_LEN];
        pcLine = acLine;
        //get node name.
        pcToken=strtok(pcLine,",");
        if(strlen(pcToken) > 32)
            Truncate_str("parameter Name",pcToken,false);

        strcpy(NodeName,pcToken);
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
        POSITION posMsg = m_listParam.GetHeadPosition();
        while(posMsg != NULL)
        {
            CParameters& rParam = m_listParam.GetNext(posMsg);
            // find matching Parameter from list
            if(strcmp(rParam.m_ParamName,acTemp)==0 )
            {
                pVal.Format_Param_Value(rParam.m_ParamType,pcLine,1,0,NodeName);
                rParam.m_listParamValues[1].AddTail(pVal);
                pcTemp=acTemp;
                break;
            }
        }
    }
}

/**
* \brief      Parses the Net Parameter's Other Values(BA_).
* \param[in]  CStdioFile &fileInput,CList<CParameters,CParameters&>& m_listParam
* \param[out] None
* \return     void
* \authors    Padmaja.A
* \date       16.11.2004
*/
void CParameters::Format_NetParam_Value(CStdioFile &fileInput,CList<CParameters,CParameters&>& m_listParam)
{
    CParameterValues pVal;
    char acLine[defCON_MAX_LINE_LEN];
    char *pcToken, *pcLine;
    char acTemp[defCON_CHAR_LEN],*pcTemp;
    pcTemp = acTemp;
    //parses the mesg pram other values.
    while(strcmp((fileInput.ReadString(acLine,defCON_MAX_LINE_LEN)),"[END_PARAM_NET_VAL]\n")!=0)
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
        POSITION posMsg = m_listParam.GetHeadPosition();
        while(posMsg != NULL)
        {
            CParameters& rParam = m_listParam.GetNext(posMsg);
            // find matching Parameter from list
            if(strcmp(rParam.m_ParamName,acTemp)==0 )
            {
                pVal.Format_Param_Value(rParam.m_ParamType,(pcLine+strlen(acTemp)+3),0);
                rParam.m_listParamValues[0].AddTail(pVal);
                pcTemp=acTemp;
                break;
            }
        }
    }
}


/**
* \brief      Parses the attribute lines from the given i/p file.
* \param[in]  char *pcLine,int index
* \param[out] None
* \return     void
* \authors    Padmaja.A
* \date       15.11.2004
*/
void CParameters::Format_ParamDef(char *pcLine,int index)
{
    //get object id and stores m_object Id with the valid value.
    if(index==0)
        strcpy(m_ObjectId,"\0");
    else if(index==1)
        strcpy(m_ObjectId,"BU_");
    else if(index==2)
        strcpy(m_ObjectId,"BO_");
    else if(index==3)
        strcpy(m_ObjectId,"SG_");
    else if(index==4)
        strcpy(m_ObjectId,"BU_SG_REL_");
    else if(index==5)
        strcpy(m_ObjectId,"BU_BO_REL_");
    //reads the param defintion.
    GetParam_Def(pcLine);
}


/**
* \brief      Parses the attribute value from the given i/p file.
* \param[in]  char *pcLine
* \param[out] None
* \return     void
* \authors    Padmaja.A
* \date       15.11.2004
*/
void CParameters::GetParam_Def(char *pcLine)
{
    char *pcToken;

    //get Param name
    pcToken = strtok(pcLine,"\"");

    if(strlen(pcToken) > 32)
        Truncate_str("parameter Name",pcToken,true);
    strcpy(m_ParamName,pcToken);
    //get Param type
    pcToken=strtok(NULL,",");
    strcpy(m_ParamType,pcToken);

    if(strcmp(m_ParamType,"STRING")==0)
        pcToken=strtok(NULL,"\n");
    else
        pcToken=strtok(NULL,",");
    //get the default value of parameter
    ReadDefault_Value(pcToken);

    if(strcmp(m_ParamType,"ENUM")==0)
    {
        m_ValRange="";
        while(pcToken=strtok(NULL,"\n"))
            m_ValRange=m_ValRange+" "+pcToken;
    }
    //gets the min/max value and validates the range
    else if(strcmp(m_ParamType,"HEX")==0)
    {
        unsigned int min_val,max_val;
        pcToken=strtok(NULL,",");
        min_val=atoi(pcToken);
        pcToken = strtok(NULL," ");
        max_val=atoi(pcToken);
        m_RangeError=m_RangeError | isValid_hexRange(min_val,max_val);
    }
    //gets the min/max value and validates the range
    else if(strcmp(m_ParamType,"FLOAT")==0)
    {
        double min_val,max_val;
        pcToken=strtok(NULL,",");
        min_val= atof(pcToken);
        pcToken = strtok(NULL," ");
        max_val=atof(pcToken);
        m_RangeError=m_RangeError | isValid_floatRange(min_val,max_val);
    }
    //gets the min/max value and validates the range
    else if(strcmp(m_ParamType,"INT")==0)
    {
        LONGLONG min_val,max_val;
        pcToken=strtok(NULL,",");
        min_val=_atoi64(pcToken);
        pcToken = strtok(NULL," ");
        max_val=_atoi64(pcToken);
        m_RangeError=m_RangeError | isValid_intRange(min_val,max_val);
    }
    //validates the default value
    m_RangeError=m_RangeError | Check_Default_Value();
}


/**
* \brief      Reads the default value of attribute from the i/p file.
* \param[in]  char *pcToken
* \param[out] None
* \return     void
* \authors    Padmaja.A
* \date       15.11.2004
*/
void CParameters::ReadDefault_Value(char *pcToken)
{
    char acTemp[defCON_CHAR_LEN],*pcTemp;
    pcTemp = acTemp;
    //get inital value of type ENUM
    if(strcmp(m_ParamType,"ENUM")==0 )
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
        strcpy(m_InitVal.cValue,acTemp);
        pcTemp=acTemp;
    }
    //get inital value of type INT
    else if(strcmp(m_ParamType,"INT")==0 )
    {
        if(strcmp(pcToken," ")!=0)
            m_InitVal.iValue =atoi(pcToken);
    }
    //get inital value of type HEX
    else if(strcmp(m_ParamType,"HEX")==0 )
    {
        if(strcmp(pcToken," ")!=0)
            m_InitVal.uiValue =atoi(pcToken);
        else
            m_InitVal.uiValue =-1;
    }
    //get inital value of type Float
    else if(strcmp(m_ParamType,"FLOAT")==0 )
    {
        if(strcmp(pcToken," ")!=0)
            m_InitVal.fValue=float(atof(pcToken));
    }
    //get inital value of type string
    else if(strcmp(m_ParamType,"STRING")==0 )
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
        strcpy(m_InitVal.cValue,acTemp);
        pcTemp=acTemp;
    }
}


/**
* \brief      Writes the parameter default values to the output file.
* \param[in]  CStdioFile& fileOutput,CList<CParameters,CParameters&> &m_listParameter
* \param[out] None
* \return     bool
* \authors    Padmaja.A
* \date       15.11.2004
*/
bool Write_DefVal_ToFile(CStdioFile& fileOutput,CList<CParameters,CParameters&> &m_listParameter)
{
    bool pResult=true;
    char acLine[defCON_MAX_LINE_LEN];
    //gets the first param from the list.
    POSITION pos=m_listParameter .GetHeadPosition();
    while(pos!=NULL)
    {
        CParameters& rParam=m_listParameter.GetNext(pos);
        //checks whether def val is with in the range or not.
        if(rParam.m_RangeError)
            pResult=false;
        //writes def val of type int to the o/p file.
        if(strcmp(rParam.m_ParamType,"INT")==0 )
            sprintf(acLine,"BA_DEF_DEF_  \"%s\" %d;\n",rParam.m_ParamName,rParam.m_InitVal.iValue);
        //writes def val of type ex to the o/p file.
        else if(strcmp(rParam.m_ParamType,"HEX")==0)
            sprintf(acLine,"BA_DEF_DEF_  \"%s\" %u;\n",rParam.m_ParamName,rParam.m_InitVal.uiValue);
        //writes def val of type flaot to the o/p file.
        else if(strcmp(rParam.m_ParamType,"FLOAT")==0)
            sprintf(acLine,"BA_DEF_DEF_  \"%s\" %f;\n",rParam.m_ParamName,rParam.m_InitVal.fValue);
        //writes def val of type enum to the o/p file.
        else if(strcmp(rParam.m_ParamType,"ENUM")==0)
            sprintf(acLine,"BA_DEF_DEF_  \"%s\" \"%s\";\n",rParam.m_ParamName,rParam.m_InitVal.cValue);
        //writes def val of type string to the o/p file.
        else
            sprintf(acLine,"BA_DEF_DEF_  \"%s\" %s;\n",rParam.m_ParamName,rParam.m_InitVal.cValue);
        fileOutput.WriteString(acLine);
    }
    return pResult;
}

/**
* \brief      Validates the default value of an attribute.
* \param[in]  None
* \param[out] None
* \return     bool
* \authors    Padmaja.A
* \date       16.11.2004
*/
bool CParameters::Check_Default_Value()
{
    bool cResult=false;
    //validates def val of type int
    if(strcmp(m_ParamType,"INT")==0)
    {
        if(m_InitVal.iValue<m_MinVal.iValue || m_InitVal.iValue>m_MaxVal.iValue)
        {
            m_InitVal.iValue=m_MinVal.iValue;
            cResult=true;
        }
    }
    //validates def val of type hex
    else if(strcmp(m_ParamType,"HEX")==0)
    {
        if(m_InitVal.uiValue<m_MinVal.uiValue || m_InitVal.uiValue>m_MaxVal.uiValue)
        {
            m_InitVal.uiValue=m_MinVal.uiValue;
            cResult= true;
        }
    }
    //validates def val of type float
    else if(strcmp(m_ParamType,"FLOAT")==0)
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
* \brief      Validates the maximum & minimum int values of an attribute.
* \param[in]  LONGLONG minValue,LONGLONG maxValue
* \param[out] None
* \return     bool
* \authors    Padmaja.A
* \date       18.11.2004
*/
bool CParameters::isValid_intRange(LONGLONG minValue,LONGLONG maxValue)
{
    bool rResult=false;
    //validates the min value
    if(minValue < INT_MIN )
    {
        m_MinVal.iValue=INT_MIN;
        rResult=rResult|true;
    }
    else
        m_MinVal.iValue =int(minValue);
    //validates the max value
    if(maxValue>INT_MAX || maxValue<m_MinVal.iValue)
    {
        m_MaxVal.iValue=INT_MAX;
        rResult=rResult | true;
    }
    else
        m_MaxVal.iValue=int(maxValue);
    return rResult;
}

/**
* \brief      Validates the maximum & minimum float values of an attribute.
* \param[in]  double minValue,double maxValue
* \param[out] None
* \return     bool
* \authors    Padmaja.A
* \date       18.11.2004
*/
bool CParameters::isValid_floatRange(double minValue,double maxValue)
{
    bool rResult=false;
    //validates the min value
    if(minValue<FLT_MIN)
    {
        m_MinVal.fValue=FLT_MIN;
        rResult=rResult | true;
    }
    else
        m_MinVal.fValue= float(minValue);
    //validates the max value
    if(maxValue>FLT_MAX || maxValue<m_MinVal.fValue)
    {
        m_MaxVal.fValue=FLT_MAX;
        rResult=rResult | true;
    }
    else
        m_MaxVal.fValue =float(maxValue);
    return rResult;
}

/**
* \brief      Validates the maximum & minimum hex values of an attribute.
* \param[in]  unsigned int minValue,unsigned int maxValue
* \param[out] None
* \return     bool
* \authors    Padmaja.A
* \date       18.11.2004
*/
bool CParameters::isValid_hexRange(unsigned int minValue,unsigned int maxValue)
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
        m_MaxVal.uiValue=maxValue;
    return rResult;
}
