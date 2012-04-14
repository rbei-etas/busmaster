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
 * \file      ValueDescriptor.cpp
 * \brief     Implementation file for CValueDescriptor class
 * \author    Amit Ranjan
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CValueDescriptor class
 */


#include "StdAfx.h"
//#include "CANDBConverter.h"
#include "ValueDescriptor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CValueDescriptor::CValueDescriptor()
{
    m_value.dValue = 0; // assume double type for default constructor
    m_acDescriptor[0] = '\0';
}

CValueDescriptor::~CValueDescriptor()
{
    // nothing special to do here
}

