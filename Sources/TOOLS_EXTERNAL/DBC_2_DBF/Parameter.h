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
 * \author    Padmaja A
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CParameters class.
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

class CParameters
{
public:
    char			m_ObjectId[50];
    char			m_ParamType[10];
    SIG_VALUE       m_MaxVal;
    SIG_VALUE		m_MinVal;
    CString    		m_ValRange;
    SIG_VALUE	    m_InitVal;
    char			m_ParamName[defVDES_MAX_DESC_LEN];
    bool            m_RangeError;
    bool            m_defError;
    CList<CParameterValues,CParameterValues&> m_listParamValues[6];
    CParameters();
    int Format(char *pcLine);
    CParameters& operator=(CParameters& param);
    int FormatParamValue(char *pcLine);
    int ReadDefaultVal(char *pcToken);
    friend bool WriteParametersToFile(CStdioFile& fileOutput,CList<CParameters,CParameters&>& m_listParameter);
    virtual ~CParameters();
};



#endif // !defined(AFX_PARAMETER_H__4051F3F4_2978_4987_B1DF_BA4CAD29981A__INCLUDED_)
