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
 * \file      Parameter.h
 * \brief     CParameters class.
 * \authors   Padmaja.A., Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CParameters class.
 */

#pragma once

/* MFC includes */
#include <afxtempl.h>

/* C++ includes */
#include <fstream>

/* Project includes */
#include "ParameterVal.h"

class CParameters
{
    //private data members
    string          m_ObjectId;
    SIG_VALUE       m_MaxVal;
    SIG_VALUE       m_MinVal;
    string          m_ValRange;
    SIG_VALUE       m_InitVal;
    bool            m_RangeError;

    //public data members & functions
public:
    string          m_ParamName;
    string          m_ParamType;
    CList<CParameterValues,CParameterValues&> m_listParamValues[6];
    CParameters();
    void Format_ParamDef(char* pcLine,int index);
    CParameters& operator=(CParameters& param);
    void GetParam_Def(char* pcLine);
    void Format_NodeParam_Value(fstream& fileInput,CList<CParameters,CParameters&>& m_listParam);
    void Format_NetParam_Value(fstream& fileInput,CList<CParameters,CParameters&>& m_listParam);
    void Format_MesgParam_Value(fstream& fileInput,CList<CParameters,CParameters&>& m_listParam);
    void Format_SigParam_Value(fstream& fileInput,CList<CParameters,CParameters&>& m_listParam);
    void ReadDefault_Value(char* pcToken);
    bool Check_Default_Value();
    friend bool WriteParamToFile(fstream& fileOutput,CList<CParameters,CParameters&>& m_listParameter);
    friend bool Write_DefVal_ToFile(fstream& fileOutput,CList<CParameters,CParameters&> &m_listParameter);
    bool isValid_intRange(long long int minValue,long long int maxValue);
    bool isValid_floatRange(double minValue,double maxValue);
    bool isValid_hexRange(unsigned int minValue,unsigned int maxValue);
    virtual ~CParameters();
};
