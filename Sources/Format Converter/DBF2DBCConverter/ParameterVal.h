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
 * \authors   Padmaja.A, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CParameterValues class.
 */

#pragma once

/* C++ includes */
#include <fstream>

/* Project includes */
#include "Definitions.h"

using namespace std;

class CParameterValues
{
    //public member functions
public:
    enum MSG_FRAME_FORMAT {MSG_FF_STANDARD = 'S', MSG_FF_EXTENDED = 'X'};
    string          m_NodeName;
    unsigned int    m_MsgId;
    string          m_SignalName;
    char            m_cFrameFormat;
    SIG_VALUE       m_ParamVal;

    CParameterValues();
    CParameterValues& operator=(CParameterValues& param);
    void Format_Param_Value(string& paramType, char* pcLine, const int& index, unsigned int msgId=0, char* Node_Name="");
    void ReadParamValue(string& paramType, char* pcToken);
    void WriteNetValuesToFile(fstream& fileOutput, string& paramType, string& paramName);
    void WriteNodeValuesToFile(fstream& fileOutput, string& paramType, string& paramName);
    void WriteMesgValuesToFile(fstream& fileOutput, string& paramType, string& paramName);
    void WriteSigValuesToFile(fstream& fileOutput, string& paramType, string& paramName);
    virtual ~CParameterValues();
};
