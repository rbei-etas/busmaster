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
 * \author    RBIN/EBS1 - Padmaja.A.
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CParameters class.
 */
/**
* \file       Parameter.h
* \brief      CParameters class.
* \authors    Padmaja A
* \date       14.11.2004
* \copyright  Copyright &copy; 2011 Robert Bosch Engineering and Business Solutions.  All rights reserved.
*/
//#if !defined(AFX_PARAMETER_H__4051F3F4_2978_4987_B1DF_BA4CAD29981A__INCLUDED_)
//#define AFX_PARAMETER_H__4051F3F4_2978_4987_B1DF_BA4CAD29981A__INCLUDED_
#ifndef PARAMETER_HEADER
#define PARAMETER_HEADER
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "ParameterVal.h"

//#define	defVDES_MAX_DESC_LEN 50 // hopefully nobody puts value descriptor strings
// more than this

class CParameters
{
    //private data members
    char			m_ObjectId[50];
    SIG_VALUE       m_MaxVal;
    SIG_VALUE		m_MinVal;
    CString    		m_ValRange;
    SIG_VALUE	    m_InitVal;
    bool            m_RangeError;

    //public data members & functions
public:
    char			m_ParamName[defVDES_MAX_DESC_LEN];
    char			m_ParamType[10];
    CList<CParameterValues,CParameterValues&> m_listParamValues[6];
    CParameters();
    void Format_ParamDef(char *pcLine,int index);
    CParameters& operator=(CParameters& param);
    void GetParam_Def(char *pcLine);
    void Format_NodeParam_Value(CStdioFile &fileInput,CList<CParameters,CParameters&>& m_listParam);
    void Format_NetParam_Value(CStdioFile &fileInput,CList<CParameters,CParameters&>& m_listParam);
    void Format_MesgParam_Value(CStdioFile &fileInput,CList<CParameters,CParameters&>& m_listParam);
    void Format_SigParam_Value(CStdioFile &fileInput,CList<CParameters,CParameters&>& m_listParam);
    void ReadDefault_Value(char *pcToken);
    bool Check_Default_Value();
    friend bool WriteParamToFile(CStdioFile& fileOutput,CList<CParameters,CParameters&>& m_listParameter);
    friend bool Write_DefVal_ToFile(CStdioFile& fileOutput,CList<CParameters,CParameters&> &m_listParameter);
    bool isValid_intRange(LONGLONG minValue,LONGLONG maxValue);
    bool isValid_floatRange(double minValue,double maxValue);
    bool isValid_hexRange(unsigned int minValue,unsigned int maxValue);
    virtual ~CParameters();
};



#endif // !defined(AFX_PARAMETER_H__4051F3F4_2978_4987_B1DF_BA4CAD29981A__INCLUDED_)
