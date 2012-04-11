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
 * \author    RBIN/EBS1 - Padmaja.A.
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CValueTable class.
 */
/**
* \file       ValueTable.h
* \brief      CValueTable class.
* \authors    Padmaja.A.
* \date       14.11.2004
* \copyright  Copyright &copy; 2011 Robert Bosch Engineering and Business Solutions.  All rights reserved.
*/

#if !defined(AFX_VALUETABLE_H__714FC1B9_4122_443B_A2D4_BDB62EEED1DB__INCLUDED_)
#define AFX_VALUETABLE_H__714FC1B9_4122_443B_A2D4_BDB62EEED1DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "ValueDescriptor.h"
#define max_TAB_NAME 1024

class CValueTable
{
    //private data members
    char m_TableName[max_TAB_NAME];
    CList <CValueDescriptor, CValueDescriptor&> m_values;

    //public member functions
public:
    CValueTable();
    virtual ~CValueTable();
    void Format_ValueTable(char *pcLine,CStdioFile &fileInput);
    CValueTable& operator=(CValueTable& Tab);
    void writeValueTabToFile(CStdioFile &fileOutput,CList<CValueTable,CValueTable&> &vTab);
};

#endif // !defined(AFX_VALUETABLE_H__714FC1B9_4122_443B_A2D4_BDB62EEED1DB__INCLUDED_)
