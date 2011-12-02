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
 * \author    Padmaja A
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for the Parameter class.
 */

#include "StdAfx.h"
#include "Parameter.h"
#include<limits.h>
#include<float.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
 * \brief Constructor
 */
CParameters::CParameters()
{
    m_InitVal.fValue=-1;
    strcpy(m_InitVal.cValue,"");
    m_InitVal.uiValue=0;
    m_ValRange='\0';
    m_ParamType[0]='\0';
    m_ParamName[0]='\0';
    m_InitVal.iValue =-1;
    m_RangeError=false;
    m_defError=false;
}

/**
 * \brief Destructor
 */
CParameters::~CParameters()
{
    // nothing special to do here
}

/**
 * \brief Operator overloading for =
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
    m_defError=param.m_defError;
    for(int i=0; i<6; i++)
        m_listParamValues[i].AddTail( (&param.m_listParamValues[i]));
    return (*this);
}

/**
 * \brief     Writes the parameter definition
 * \param[in] fileOutput filename of output file
 * \param[in] m_listParameter Parameters List
 *
 * Writes the parameter definition and default value to the
 * specified output file.
 */
bool WriteParametersToFile(CStdioFile& fileOutput,CList<CParameters,CParameters&> &m_listParameter)
{
    bool pResult=true;
    char acLine[defCON_MAX_LINE_LEN];
    POSITION pos=m_listParameter .GetHeadPosition();

    // if no parameter in the list then it simply returns true.
    // otherwise writes definition and default value of all parameters to the o/p file
    while(pos!=NULL)
    {
        //Gets the next parameter from the list.
        CParameters& rParam=m_listParameter.GetNext(pos);
        if(rParam.m_RangeError)
            pResult=false;

        //Validation of  Default value of the parameter before writing to the o/p file.

        //Parameter : INT type
        if(strcmp(rParam.m_ParamType,"INT")==0 )
        {
            //Default value is NULL
            if(rParam.m_InitVal.iValue ==-1)
            {
                rParam.m_defError=rParam.m_defError | true;
                pResult=pResult & false;
                rParam.m_InitVal.iValue=rParam.m_MinVal.iValue;
                sprintf(acLine,"\"%s\",%s,%d,%d,%d\n",rParam.m_ParamName,rParam.m_ParamType,rParam.m_InitVal.iValue,rParam.m_MinVal.iValue,rParam.m_MaxVal.iValue);
            }

            //Default value is not NULL
            else
                sprintf(acLine,"\"%s\",%s,%d,%d,%d\n",rParam.m_ParamName,rParam.m_ParamType,rParam.m_InitVal.iValue,rParam.m_MinVal.iValue,rParam.m_MaxVal.iValue);
        }

        //Parameter : HEX type
        else if(strcmp(rParam.m_ParamType,"HEX")==0)
        {
            //Default value is NULL
            if(rParam.m_InitVal.uiValue ==-1)
            {
                rParam.m_defError=rParam.m_defError | true;
                pResult=pResult & false;
                rParam.m_InitVal.uiValue=rParam.m_MinVal.uiValue;
                sprintf(acLine,"\"%s\",%s,%u,%u,%u\n",rParam.m_ParamName,rParam.m_ParamType,rParam.m_InitVal.uiValue,rParam.m_MinVal.uiValue,rParam.m_MaxVal.uiValue);
            }
            //Default value is not NULL
            else
                sprintf(acLine,"\"%s\",%s,%u,%u,%u\n",rParam.m_ParamName,rParam.m_ParamType,rParam.m_InitVal.uiValue,rParam.m_MinVal.uiValue,rParam.m_MaxVal.uiValue);
        }

        //Parameter : FLOAT type
        else if(strcmp(rParam.m_ParamType,"FLOAT")==0)
        {   //Default value is NULL
            if(rParam.m_InitVal.fValue ==-1)
            {
                rParam.m_defError=rParam.m_defError | true;
                pResult=pResult & false;
                rParam.m_InitVal.fValue=rParam.m_MinVal.fValue;
                sprintf(acLine,"\"%s\",%s,%f,%f,%f\n",rParam.m_ParamName,rParam.m_ParamType,rParam.m_InitVal.fValue,rParam.m_MinVal.fValue,rParam.m_MaxVal.fValue);
            }
            //Default value is not NULL
            else
                sprintf(acLine,"\"%s\",%s,%f,%f,%f\n",rParam.m_ParamName,rParam.m_ParamType,rParam.m_InitVal.fValue,rParam.m_MinVal.fValue,rParam.m_MaxVal.fValue);
        }

        //Parameter : ENUM type
        else if(strcmp(rParam.m_ParamType,"ENUM")==0)
        {
            //Default value is NULL
            if(strcmp(rParam.m_InitVal.cValue,"")==0)
                sprintf(acLine,"\"%s\",%s,\"\",%s\n",rParam.m_ParamName,rParam.m_ParamType,rParam.m_ValRange);

            //Default value is not NULL
            else
                sprintf(acLine,"\"%s\",%s,\"%s\",%s\n",rParam.m_ParamName,rParam.m_ParamType,rParam.m_InitVal.cValue,rParam.m_ValRange);
        }

        //Parameter : STRING type
        else
            sprintf(acLine,"\"%s\",%s,%s\n",rParam.m_ParamName,rParam.m_ParamType,rParam.m_InitVal.cValue );

        //After validation parameter is written to the o/p file.
        fileOutput.WriteString(acLine);
    }
    return pResult;
}

/**
 * \brief Calls the appropriate function of CParamValues
 *
 * This function calls the appropriate function of CParamValues
 * to get the Other Value of Attributes form CanoeDB file.
 */
int CParameters::FormatParamValue(char *pcLine)
{
    char *pcToken;
    CParameterValues pVal;

    //get object id
    while(*pcLine && *pcLine==' ')
    {
        *pcLine++;
    }
    pcToken=strtok(pcLine," ");

    // if object id is node then calls GetNodeParams funtion to read the other values lilke min/max
    if(strcmp(pcToken,"BU_")==0)
    {
        pVal.GetNodeParams(m_ParamType,(pcLine+strlen(pcToken)+1));
        //adds the other calues to the list
        m_listParamValues[1].AddTail(pVal);
    }
    // if object id is Message then calls GetMsgParams funtion to read the other values lilke min/max
    else if(strcmp(pcToken,"BO_")==0)
    {
        pVal.GetMesgParams(m_ParamType,(pcLine+strlen(pcToken)+1));
        //adds the other calues to the list
        m_listParamValues[2].AddTail(pVal);
    }
    // if object id is signal then calls GetSignalParams funtion to read the other values lilke min/max
    else if(strcmp(pcToken,"SG_")==0)
    {
        pVal.GetSignalParams(m_ParamType,(pcLine+strlen(pcToken)+1));
        //adds the other calues to the list
        m_listParamValues[3].AddTail(pVal);
    }
    // if object id is net then calls GetNetParams funtion to read the other values lilke min/max
    else
    {
        pVal.GetNetParams(m_ParamType,pcToken);
        //adds the other calues to the list
        m_listParamValues[0].AddTail(pVal);
    }
    return 1;
}

/**
 * \brief     Parses the attribute lines from CanoeDB file.
 * \param[in] pcLine Input file string
 *
 * Parses the attribute lines from CanoeDB file.
 */
int CParameters::Format(char *pcLine)
{
    char *pcToken;
    char acTemp[defCON_TEMP_LEN];
    char *pcTemp = acTemp;
    int success=1;

    //get object id

    pcToken = strtok(pcLine,"\"");

    while(*pcToken==' ')
        *pcToken++;
    while(*pcToken && *pcToken != ' ')
    {
        *pcTemp++ = *pcToken++; // copy OBJECT_ID only, i.e. till first 'space'
    }
    *pcTemp = '\0'; // terminate it
    strcpy(m_ObjectId,acTemp); // copy the object_id to the parameter's data member
    pcTemp = acTemp; // reset pcTemp to start of buffer


    //get Attribute name
    pcToken = strtok(NULL,"\"");
    while(*pcToken && *pcToken != '"')
    {
        *pcTemp++ = *pcToken++; // copy PARAM_NAME only, i.e. till first 'space'
    }
    *pcTemp = '\0'; // terminate it
    strcpy(m_ParamName,acTemp); // copy the name to the parameter's data member
    pcTemp = acTemp; // reset pcTemp to start of buffer

    //get Value Type
    pcToken = strtok(NULL," ");
    while(*pcToken==' ')
        *pcToken++;
    while(*pcToken && *pcToken != ' ' && *pcToken != ';')
    {
        *pcTemp++ = *pcToken++; // copy PARAM_TYPE only, i.e. till first 'space'
    }
    *pcTemp = '\0'; // terminate it
    strcpy(m_ParamType,acTemp); // copy the type to the parameter's data member
    pcTemp = acTemp; // reset pcTemp to start of buffer


    if(strcmp(m_ParamType,"STRING")!=0)
    {
        pcToken = strtok(NULL,";");
        if(strcmp(m_ParamType,"ENUM")==0)
        {
            while(*pcToken && *pcToken == ' ')
                *pcToken++;
            m_ValRange=pcToken;
        }
        //Reads the flaot min,max values and validates those values.
        else if(strcmp(m_ParamType,"FLOAT")==0)
        {
            double temp;
            pcToken=strtok(pcToken," ");
            temp = atof(pcToken);
            if(temp<FLT_MIN) //min val valildation
            {
                m_MinVal.fValue=FLT_MIN;
                m_RangeError=m_RangeError | true;
            }
            else
                m_MinVal.fValue= float(temp);
            pcToken = strtok(NULL," ");
            temp=atof(pcToken);
            if(temp>FLT_MAX || temp<m_MinVal.fValue) //max value validation
            {
                m_MaxVal.fValue=FLT_MAX;
                m_RangeError=m_RangeError | true;
            }
            else
                m_MaxVal.fValue =float(temp);
            pcTemp = acTemp; // reset pcTemp to start of buffer
        }

        //Reads the flaot min,max values and validates those values.
        else if(strcmp(m_ParamType,"INT")==0)
        {
            LONGLONG temp;
            pcToken=strtok(pcToken," ");
            temp=_atoi64(pcToken);
            if(temp < INT_MIN ) //min value validation
            {
                m_MinVal.iValue=INT_MIN;
                m_RangeError=m_RangeError | true;
            }
            else
                m_MinVal.iValue =int(temp);
            pcToken = strtok(NULL," ");
            temp=_atoi64(pcToken);
            if(temp>INT_MAX || temp<m_MinVal.iValue) //max value validation
            {
                m_MaxVal.iValue=INT_MAX;
                m_RangeError=m_RangeError | true;
            }
            else
                m_MaxVal.iValue=int(temp);
            pcTemp = acTemp; // reset pcTemp to start of buffer*/
        }

        //Reads the flaot min,max values and validates those values.
        else if(strcmp(m_ParamType,"HEX")==0)
        {
            pcToken=strtok(pcToken," ");
            m_MinVal.uiValue=atoi(pcToken);
            if(m_MinVal.uiValue<0) //min vlaue validation
            {
                m_MinVal.uiValue =0;
                m_RangeError=m_RangeError|true;
            }
            pcToken = strtok(NULL,";");
            m_MaxVal.uiValue=atoi(pcToken);
            //max value validation
            if(m_MaxVal.uiValue==0 || m_MaxVal.uiValue <m_MinVal.uiValue)
            {
                m_MaxVal.uiValue=0xffffffff;
                m_RangeError=m_RangeError|true;
            }
            pcTemp = acTemp;
        }

    }

    return success;
}

/**
 * \brief     Reads the default value of attribute from the CanoeDB file
 * \param[in] pcToken Input string
 *
 * Reads the default value of attribute from the CanoeDB file.
 */
int CParameters::ReadDefaultVal(char *pcToken)
{
    char acTemp[defCON_TEMP_LEN];
    char *pcTemp = acTemp;
    int success=1;
    //Default value validation with respect to max.min values.

    //Param Type: ENUM :No need to validate.
    if(strcmp(m_ParamType,"ENUM")==0 )
    {
        //omits all balcnk spaces.
        while(*pcToken && *pcToken == ' ')
        {
            *pcToken++;
        }
        pcToken++;
        //reads the enum default value.
        while(*pcToken && *pcToken != '"')
        {
            *pcTemp++ = *pcToken++;
        }
        *pcTemp = '\0';
        strcpy(m_InitVal.cValue,acTemp);
        pcTemp=acTemp;
    }

    //Param Type: INT
    else if(strcmp(m_ParamType,"INT")==0 )
    {
        if(strcmp(pcToken," ")!=0)
            m_InitVal.iValue =atoi(pcToken);
        //if default value is not with in the range then set default vlaue as min value.
        if(m_InitVal.iValue<m_MinVal.iValue || m_InitVal.iValue>m_MaxVal.iValue)
        {
            m_InitVal.iValue=m_MinVal.iValue;
            m_RangeError=m_RangeError || true;
        }
    }

    //PArameter Type: HEX
    else if(strcmp(m_ParamType,"HEX")==0 )
    {
        if(strcmp(pcToken," ")!=0)
            m_InitVal.uiValue =atoi(pcToken);
        //if default value is not with in the range then set default vlaue as min value.
        if(m_InitVal.uiValue<m_MinVal.uiValue || m_InitVal.uiValue>m_MaxVal.uiValue)
        {
            m_InitVal.uiValue=m_MinVal.uiValue;
            m_RangeError=m_RangeError || true;
        }
    }

    //Parameter Type : FLOAT
    else if(strcmp(m_ParamType,"FLOAT")==0 )
    {
        if(strcmp(pcToken," ")!=0)
            m_InitVal.fValue=float(atof(pcToken));
        //if default value is not with in the range then set default vlaue as min value.
        if(m_InitVal.fValue<m_MinVal.fValue || m_InitVal.fValue>m_MaxVal.fValue)
        {
            m_InitVal.fValue=m_MinVal.fValue;
            m_RangeError=m_RangeError || true;
        }
    }

    ////Parameter Type :String : Not Required.
    else if(strcmp(m_ParamType,"STRING")==0 )
    {
        while(*pcToken && *pcToken == ' ')
        {
            *pcToken++;
        }
        strcpy(m_InitVal.cValue,pcToken);
    }
    return success;
}
