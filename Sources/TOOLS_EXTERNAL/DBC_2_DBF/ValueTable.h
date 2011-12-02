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
 * \file      ValueTable.h
 * \brief     CValueTable class.
 * \author    Mahesh B S
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CValueTable class.
 */

#if !defined(AFX_VALUETABLE_H__714FC1B9_4122_443B_A2D4_BDB62EEED1DB__INCLUDED_)
#define AFX_VALUETABLE_H__714FC1B9_4122_443B_A2D4_BDB62EEED1DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "ValueDescriptor.h"
#define max_TAB_NAME 50

class CValueTable
{
public:
    CValueTable();
    virtual ~CValueTable();
    int Format(char *pcLine,CStdioFile &fileInput);
    CList <CValueDescriptor, CValueDescriptor&> m_values;
    CValueTable& operator=(CValueTable& Tab);
    //CList<CValueDescriptor,CValueDescriptor&>	m_listValueDescriptor;
    char m_TableName[max_TAB_NAME];
    void writeValueTabToFile(CStdioFile &fileOutput,CList<CValueTable,CValueTable&> &vTab);
};

#endif // !defined(AFX_VALUETABLE_H__714FC1B9_4122_443B_A2D4_BDB62EEED1DB__INCLUDED_)
