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
 * \author    Padmaja A
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CParameterValues class.
 */

//if !defined(AFX_PARAMETERVAL_H__3051F3F4_2978_4987_B1DF_BA4CAD29981A__INCLUDED_)
//#define AFX_PARAMETERVAL_H__3051F3F4_2978_4987_B1DF_BA4CAD29981A__INCLUDED_
#ifndef PARAMETERVAL_HEADER
#define PARAMETERVAL_HEADER
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
class CParameterValues
{
public:
    enum MSG_FRAME_FROMAT {MSG_FF_STANDARD = 'S', MSG_FF_EXTENDED = 'X'};
    char            m_NodeName[defNODE_MAX_LEN];
    unsigned int    m_MsgId;
    char            m_SignalName[defNODE_MAX_LEN];
    char			m_cFrameFormat;
    SIG_VALUE       m_ParamVal;
    CParameterValues();
    CParameterValues& operator=(CParameterValues& param);
    int GetNodeParams(char *paramType ,char *pcLine);
    int GetMesgParams(char *paramType,char *pcLine);
    int GetSignalParams(char *paramType,char *pcLine);
    int GetNetParams(char *paramType,char *pcLine);
    int ReadParamValue(char *paramType,char *pcToken);
    void WriteNetValuesToFile(CStdioFile& fileOutput,char *paramType,char *paramName);
    void WriteNodeValuesToFile(CStdioFile& fileOutput,char *paramType,char *paramName);
    void WriteMesgValuesToFile(CStdioFile& fileOutput,char *paramType,char *paramName);
    void WriteSigValuesToFile(CStdioFile& fileOutput,char *paramType,char *paramName);
    virtual ~CParameterValues();
};

#endif
