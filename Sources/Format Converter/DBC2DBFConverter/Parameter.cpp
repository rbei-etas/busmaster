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
 * \brief     Implementation of parameter class
 * \authors   Padmaja A, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the parameter class.
 */

/* C includes */
#include <float.h>
#include <limits.h>

/* Project includes */
#include "Parameter.h"

using namespace std;

/**
 * \brief Constructor
 *
 * Constructor of CParameter
 */
CParameter::CParameter()
{
    strncpy(m_MaxVal.cValue, "", sizeof(m_MaxVal.cValue));
    m_MaxVal.dValue = -1;
    m_MaxVal.fValue = -1;
    m_MaxVal.i64Value = -1;
    m_MaxVal.iValue = -1;
    m_MaxVal.ui64Value = 0;
    m_MaxVal.uiValue = 0;
    strncpy(m_MinVal.cValue, "", sizeof(m_MinVal.cValue));
    m_MinVal.dValue = -1;
    m_MinVal.fValue = -1;
    m_MinVal.i64Value = -1;
    m_MinVal.iValue = -1;
    m_MinVal.ui64Value = 0;
    m_MinVal.uiValue = 0;
    strncpy(m_InitVal.cValue, "", sizeof(m_InitVal.cValue));
    m_InitVal.dValue = -1;
    m_InitVal.fValue = -1;
    m_InitVal.i64Value = -1;
    m_InitVal.iValue = -1;
    m_InitVal.ui64Value = 0;
    m_InitVal.uiValue = 0;
    m_ValRange = "";
    m_ParamType = "";
    m_ParamName = "";
    m_RangeError = false;
    m_defError = false;
}

/**
 * \brief Destructor
 *
 * Destructor of CParameter
 */
CParameter::~CParameter()
{
    // nothing special to do here
}

/**
 * \brief     Operator overloading for =
 * \param[in] param Data to copy from
 * \return    Reference to local object with new data
 *
 * Copies the data from param to the local object.
 */
CParameter& CParameter::operator=( CParameter& param)
{
    // if there are some elements in the signal list clear them first
    m_listParamValues[0].clear();
    // now copy the other elements of the new message to this
    m_ObjectId = param.m_ObjectId;
    m_ParamName = param.m_ParamName;
    m_ValRange = param.m_ValRange;
    m_ParamType = param.m_ParamType;
    m_InitVal = param.m_InitVal;
    m_MaxVal = param.m_MaxVal;
    m_MinVal = param.m_MinVal;
    m_RangeError = param.m_RangeError;
    m_defError = param.m_defError;

    for(int i=0; i<6; i++)
    {
        m_listParamValues[i] = param.m_listParamValues[i];
    }

    return (*this);
}

/**
 * \brief     Writes the parameter definition
 * \param[in] fileOutput filename of output file
 * \param[in] m_listParameter Parameters List
 * \return    Status code
 *
 * Writes the parameter definition and default value to the
 * specified output file.
 */
bool WriteParametersToFile(fstream& fileOutput, list<CParameter> &m_listParameter)
{
    bool pResult=true;
    // if no parameter in the list then it simply returns true.
    // otherwise writes definition and default value of all parameters to the o/p file
    list<CParameter>::iterator rParam;

    for(rParam=m_listParameter.begin(); rParam!=m_listParameter.end(); rParam++)
    {
        //Gets the next parameter from the list.
        if(rParam->m_RangeError)
        {
            pResult=false;
        }

        //Validation of  Default value of the parameter before writing to the o/p file.
        fileOutput << "\"" << rParam->m_ParamName.c_str() << "\"";
        fileOutput << "," << rParam->m_ParamType.c_str();

        //Parameter : INT type
        if(rParam->m_ParamType == "INT")
        {
            //Default value is NULL
            if(rParam->m_InitVal.iValue == -1)
            {
                rParam->m_defError = rParam->m_defError | true;
                pResult = pResult & false;
                rParam->m_InitVal.iValue = rParam->m_MinVal.iValue;
                fileOutput << "," << dec << rParam->m_InitVal.iValue;
                fileOutput << "," << dec << rParam->m_MinVal.iValue;
                fileOutput << "," << dec << rParam->m_MaxVal.iValue;
            }
            //Default value is not NULL
            else
            {
                fileOutput << "," << dec << rParam->m_InitVal.iValue;
                fileOutput << "," << dec << rParam->m_MinVal.iValue;
                fileOutput << "," << dec << rParam->m_MaxVal.iValue;
            }
        }
        //Parameter : HEX type
        else if(rParam->m_ParamType == "HEX")
        {
            //Default value is NULL
            if(rParam->m_InitVal.uiValue == -1)
            {
                rParam->m_defError = rParam->m_defError | true;
                pResult = pResult & false;
                rParam->m_InitVal.uiValue = rParam->m_MinVal.uiValue;
                fileOutput << "," << dec << rParam->m_InitVal.uiValue;
                fileOutput << "," << dec << rParam->m_MinVal.uiValue;
                fileOutput << "," << dec << rParam->m_MaxVal.uiValue;
            }
            //Default value is not NULL
            else
            {
                fileOutput << "," << dec << rParam->m_InitVal.uiValue;
                fileOutput << "," << dec << rParam->m_MinVal.uiValue;
                fileOutput << "," << dec << rParam->m_MaxVal.uiValue;
            }
        }
        //Parameter : FLOAT type
        else if(rParam->m_ParamType == "FLOAT")
        {
            //Default value is NULL
            if(rParam->m_InitVal.fValue == -1)
            {
                rParam->m_defError = rParam->m_defError | true;
                pResult = pResult & false;
                rParam->m_InitVal.fValue = rParam->m_MinVal.fValue;
                fileOutput << "," << rParam->m_InitVal.fValue;
                fileOutput << "," << rParam->m_MinVal.fValue;
                fileOutput << "," << rParam->m_MaxVal.fValue;
            }
            //Default value is not NULL
            else
            {
                fileOutput << "," << rParam->m_InitVal.fValue;
                fileOutput << "," << rParam->m_MinVal.fValue;
                fileOutput << "," << rParam->m_MaxVal.fValue;
            }
        }
        //Parameter : ENUM type
        else if(rParam->m_ParamType == "ENUM")
        {
            //Default value is NULL
            if(strcmp(rParam->m_InitVal.cValue, "")==0)
            {
                fileOutput << ",\"\"";
                fileOutput << "," << rParam->m_ValRange.c_str();
            }
            //Default value is not NULL
            else
            {
                fileOutput << ",\"" << rParam->m_InitVal.cValue << "\"";
                fileOutput << "," << rParam->m_ValRange.c_str();
            }
        }
        //Parameter : STRING type
        else
        {
            fileOutput << "," << rParam->m_InitVal.cValue;
        }

        //After validation parameter is written to the o/p file.
        fileOutput << endl;
    }

    return pResult;
}

/**
 * \brief     Calls the appropriate function of CParamValues
 * \param[in] pcLine Input line
 * \return    Status code
 *
 * This function calls the appropriate function of CParamValues
 * to get the Other Value of Attributes form CanoeDB file.
 */
int CParameter::FormatParamValue(char* pcLine)
{
    char* pcToken, *pcTok;
    CParameterValues pVal;

    //get object id
    while(*pcLine && *pcLine==' ')
    {
        *pcLine++;
    }

    pcToken = strtok_s(pcLine, " ", &pcTok);

    // if object id is node then calls GetNodeParams funtion to read the other values lilke min/max
    if(strcmp(pcToken, "BU_")==0)
    {
        pVal.GetNodeParams(m_ParamType, (pcLine+strlen(pcToken)+1));
        //adds the other calues to the list
        m_listParamValues[1].push_back(pVal);
    }
    // if object id is Message then calls GetMsgParams funtion to read the other values lilke min/max
    else if(strcmp(pcToken, "BO_")==0)
    {
        pVal.GetMesgParams(m_ParamType, (pcLine+strlen(pcToken)+1));
        //adds the other calues to the list
        m_listParamValues[2].push_back(pVal);
    }
    // if object id is signal then calls GetSignalParams funtion to read the other values lilke min/max
    else if(strcmp(pcToken, "SG_")==0)
    {
        pVal.GetSignalParams(m_ParamType, (pcLine+strlen(pcToken)+1));
        //adds the other calues to the list
        m_listParamValues[3].push_back(pVal);
    }
    // if object id is net then calls GetNetParams funtion to read the other values lilke min/max
    else
    {
        pVal.GetNetParams(m_ParamType,pcToken);
        //adds the other calues to the list
        m_listParamValues[0].push_back(pVal);
    }

    return 1;
}

/**
 * \brief     Parses the attribute lines from CanoeDB file.
 * \param[in] pcLine Input file string
 * \return    Status code
 *
 * Parses the attribute lines from CanoeDB file.
 */
int CParameter::Format(char* pcLine)
{
    char* pcToken, *pcTok;
    char acTemp[defCON_TEMP_LEN];
    char* pcTemp = acTemp;
    int success = 1;
    //get object id
    pcToken = strtok_s(pcLine, "\"", &pcTok);

    while(*pcToken == ' ')
    {
        *pcToken++;
    }

    while(*pcToken && *pcToken != ' ')
    {
        *pcTemp++ = *pcToken++; // copy OBJECT_ID only, i.e. till first 'space'
    }

    *pcTemp = '\0'; // terminate it
    m_ObjectId = acTemp; // copy the object_id to the parameter's data member
    pcTemp = acTemp; // reset pcTemp to start of buffer
    //get Attribute name
    pcToken = strtok_s(NULL, "\"", &pcTok);

    while(*pcToken && *pcToken != '"')
    {
        *pcTemp++ = *pcToken++; // copy PARAM_NAME only, i.e. till first 'space'
    }

    *pcTemp = '\0'; // terminate it
    m_ParamName = acTemp; // copy the name to the parameter's data member
    pcTemp = acTemp; // reset pcTemp to start of buffer
    //get Value Type
    pcToken = strtok_s(NULL, " ", &pcTok);

    while(*pcToken == ' ')
    {
        *pcToken++;
    }

    while(*pcToken && *pcToken != ' ' && *pcToken != ';')
    {
        *pcTemp++ = *pcToken++; // copy PARAM_TYPE only, i.e. till first 'space'
    }

    *pcTemp = '\0'; // terminate it
    m_ParamType = acTemp; // copy the type to the parameter's data member
    pcTemp = acTemp; // reset pcTemp to start of buffer

    if(m_ParamType != "STRING")
    {
        pcToken = strtok_s(NULL, ";", &pcTok);

        if(m_ParamType == "ENUM")
        {
            while(*pcToken && *pcToken == ' ')
            {
                *pcToken++;
            }

            m_ValRange = pcToken;
        }
        //Reads the flaot min,max values and validates those values.
        else if(m_ParamType == "FLOAT")
        {
            double temp;
            pcToken = strtok_s(pcToken, " ", &pcTok);
            temp = atof(pcToken);

            if(temp < FLT_MIN) //min val valildation
            {
                m_MinVal.fValue = FLT_MIN;
                m_RangeError = m_RangeError | true;
            }
            else
            {
                m_MinVal.fValue = float(temp);
            }

            pcToken = strtok_s(NULL, " ", &pcTok);
            temp=atof(pcToken);

            if((temp>FLT_MAX) || (temp<m_MinVal.fValue)) //max value validation
            {
                m_MaxVal.fValue = FLT_MAX;
                m_RangeError = m_RangeError | true;
            }
            else
            {
                m_MaxVal.fValue = float(temp);
            }

            pcTemp = acTemp; // reset pcTemp to start of buffer
        }
        //Reads the flaot min,max values and validates those values.
        else if(m_ParamType == "INT")
        {
            long long temp;
            pcToken = strtok_s(pcToken, " ", &pcTok);
            temp = _atoi64(pcToken);

            if(temp < INT_MIN ) //min value validation
            {
                m_MinVal.iValue = INT_MIN;
                m_RangeError = m_RangeError | true;
            }
            else
            {
                m_MinVal.iValue = int(temp);
            }

            pcToken = strtok_s(NULL, " ", &pcTok);
            temp = _atoi64(pcToken);

            if((temp>INT_MAX) || (temp<m_MinVal.iValue)) //max value validation
            {
                m_MaxVal.iValue = INT_MAX;
                m_RangeError = m_RangeError | true;
            }
            else
            {
                m_MaxVal.iValue = int(temp);
            }

            pcTemp = acTemp; // reset pcTemp to start of buffer*/
        }
        //Reads the flaot min,max values and validates those values.
        else if(m_ParamType == "HEX")
        {
            pcToken = strtok_s(pcToken, " ", &pcTok);
            m_MinVal.uiValue = atoi(pcToken);

            if(m_MinVal.uiValue < 0) //min vlaue validation
            {
                m_MinVal.uiValue = 0;
                m_RangeError = m_RangeError | true;
            }

            pcToken = strtok_s(NULL, ";", &pcTok);
            m_MaxVal.uiValue = atoi(pcToken);

            //max value validation
            if((m_MaxVal.uiValue == 0) || (m_MaxVal.uiValue < m_MinVal.uiValue))
            {
                m_MaxVal.uiValue = 0xffffffff;
                m_RangeError = m_RangeError | true;
            }

            pcTemp = acTemp;
        }
    }

    return success;
}

/**
 * \brief     Reads the default value of attribute from the CanoeDB file
 * \param[in] pcToken Input string
 * \return    Status code
 *
 * Reads the default value of attribute from the CanoeDB file.
 */
int CParameter::ReadDefaultVal(char* pcToken)
{
    char acTemp[defCON_TEMP_LEN];
    char* pcTemp = acTemp;
    int success=1;
    //Default value validation with respect to max.min values.

    //Param Type: ENUM :No need to validate.
    if(m_ParamType == "ENUM")
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
        strncpy(m_InitVal.cValue, acTemp, sizeof(m_InitVal.cValue));
        pcTemp=acTemp;
    }
    //Param Type: INT
    else if(m_ParamType == "INT")
    {
        if(strcmp(pcToken, " ") != 0)
        {
            m_InitVal.iValue = atoi(pcToken);
        }

        //if default value is not with in the range then set default vlaue as min value.
        if((m_InitVal.iValue<m_MinVal.iValue) || (m_InitVal.iValue>m_MaxVal.iValue))
        {
            m_InitVal.iValue = m_MinVal.iValue;
            m_RangeError = m_RangeError | true;
        }
    }
    //PArameter Type: HEX
    else if(m_ParamType == "HEX")
    {
        if(strcmp(pcToken, " ") != 0)
        {
            m_InitVal.uiValue = atoi(pcToken);
        }

        //if default value is not with in the range then set default vlaue as min value.
        if((m_InitVal.uiValue<m_MinVal.uiValue) || (m_InitVal.uiValue>m_MaxVal.uiValue))
        {
            m_InitVal.uiValue = m_MinVal.uiValue;
            m_RangeError = m_RangeError | true;
        }
    }
    //Parameter Type : FLOAT
    else if(m_ParamType == "FLOAT")
    {
        if(strcmp(pcToken," ")!=0)
        {
            m_InitVal.fValue=float(atof(pcToken));
        }

        //if default value is not with in the range then set default vlaue as min value.
        if(m_InitVal.fValue<m_MinVal.fValue || m_InitVal.fValue>m_MaxVal.fValue)
        {
            m_InitVal.fValue=m_MinVal.fValue;
            m_RangeError=m_RangeError | true;
        }
    }
    ////Parameter Type :String : Not Required.
    else if(m_ParamType == "STRING")
    {
        while(*pcToken && *pcToken == ' ')
        {
            *pcToken++;
        }

        strncpy(m_InitVal.cValue, pcToken, sizeof(m_InitVal.cValue));
    }

    return success;
}
