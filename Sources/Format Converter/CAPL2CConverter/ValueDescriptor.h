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
 * \brief     Interface file for CValueDescriptor class
 * \author    Amit Ranjan
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CValueDescriptor class
 */

#pragma once

#define	defVDES_MAX_DESC_LEN 50 // hopefully nobody puts value descriptor strings
// more than this
typedef union _SIG_VALUE {
    int iValue;
    unsigned int uiValue;
    float fValue;
    double	dValue;
    LONGLONG i64Value;
    ULONGLONG ui64Value;
}	SIG_VALUE;

class CValueDescriptor
{
public:
    CValueDescriptor();
    virtual ~CValueDescriptor();

// data members
    char		m_acDescriptor[defVDES_MAX_DESC_LEN];
    SIG_VALUE	m_value;
};
