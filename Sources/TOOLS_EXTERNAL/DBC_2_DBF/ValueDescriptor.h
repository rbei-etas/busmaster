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
 * \file      ValueDescriptor.h
 * \brief     CValueDescriptor class.
 * \author    Mahesh B S
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CValueDescriptor class.
 */

//
//////////////////////////////////////////////////////////////////////
//#include"definitions.h"
#if !defined(AFX_VALUEDESCRIPTOR_H__3051F3F4_2978_4987_B1DF_BA4CAD29981A__INCLUDED_)
#define AFX_VALUEDESCRIPTOR_H__3051F3F4_2978_4987_B1DF_BA4CAD29981A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
typedef union _SIG_VALUE {
int iValue;
unsigned int uiValue;
float fValue;
double	dValue;
char cValue[defVDES_MAX_DESC_LEN];
LONGLONG i64Value;
unsigned __int64 ui64Value;}	SIG_VALUE;
*/
class CValueDescriptor
{
public:
    CValueDescriptor();
    virtual ~CValueDescriptor();

    // data members
    char		m_acDescriptor[defVDES_MAX_DESC_LEN];
    SIG_VALUE	m_value;
    void writeValuDescToFile(CStdioFile &fileOutput,char m_ucType,CList<CValueDescriptor,CValueDescriptor&> &m_listValueDescriptor);
};

#endif // !defined(AFX_VALUEDESCRIPTOR_H__3051F3F4_2978_4987_B1DF_BA4CAD29981A__INCLUDED_)
