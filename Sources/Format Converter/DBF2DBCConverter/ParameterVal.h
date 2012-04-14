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
 * \file      ParameterVal.h
 * \brief     CParameterValues class.
 * \author    RBIN/EBS1 - Padmaja.A
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CParameterValues class.
 */
/**
* \file       ParameterVal.h
* \brief      CParameterValues class.
* \authors    Padmaja A
* \date       4.11.2004
* \copyright  Copyright &copy; 2011 Robert Bosch Engineering and Business Solutions.  All rights reserved.
*/

#pragma once

#include <afxtempl.h>
#define	defNODE_MAX_LEN 50 // hopefully nobody puts value descriptor strings

class CParameterValues
{
    //public member functions
public:
    enum MSG_FRAME_FORMAT {MSG_FF_STANDARD = 'S', MSG_FF_EXTENDED = 'X'};
    char            m_NodeName[defNODE_MAX_LEN];
    unsigned int    m_MsgId;
    char            m_SignalName[defNODE_MAX_LEN];
    char			m_cFrameFormat;
    SIG_VALUE       m_ParamVal;

    CParameterValues();
    CParameterValues& operator=(CParameterValues& param);
    void Format_Param_Value(char *paramType,char *pcLine,const int& index,UINT msgId=0,char *Node_Name="");
    void ReadParamValue(char *paramType,char *pcToken);
    void WriteNetValuesToFile(CStdioFile& fileOutput,char *paramType,char *paramName);
    void WriteNodeValuesToFile(CStdioFile& fileOutput,char *paramType,char *paramName);
    void WriteMesgValuesToFile(CStdioFile& fileOutput,char *paramType,char *paramName);
    void WriteSigValuesToFile(CStdioFile& fileOutput,char *paramType,char *paramName);
    virtual ~CParameterValues();
};
