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
 * \author    RBIN/EBS1 - Mahesh B S
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CValueDescriptor class.
 */
/**
* \file       ValueDescriptor.h
* \brief      CValueDescriptor class.
* \authors    Mahesh B S
* \date       14.11.2004
* \copyright  Copyright &copy; 2011 Robert Bosch Engineering and Business Solutions.  All rights reserved.
*/

#include <afxtempl.h>

#if !defined(AFX_VALUEDESCRIPTOR_H__A82ECC4A_9341_46DB_8153_D133A81DC9C3__INCLUDED_)
#define AFX_VALUEDESCRIPTOR_H__A82ECC4A_9341_46DB_8153_D133A81DC9C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CValueDescriptor
{
public:
    CValueDescriptor();
    virtual ~CValueDescriptor();
    int Format(char *pcLine);
    //	CValueDescriptor& operator=(CValueDescriptor& valDesc);
    void writeValuDescToFile(CStdioFile &fileOutput,char m_ucType,CList<CValueDescriptor,CValueDescriptor&> &m_listValueDescriptor);
    CString		m_sDescriptor;
    SIG_VALUE	m_value;
};

#endif // !defined(AFX_VALUEDESCRIPTOR_H__A82ECC4A_9341_46DB_8153_D133A81DC9C3__INCLUDED_)
