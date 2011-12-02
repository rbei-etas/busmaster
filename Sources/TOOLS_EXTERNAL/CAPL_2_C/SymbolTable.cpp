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
 * \file      SymbolTable.cpp
 * \brief     Implementation file for CSymbolTable class
 * \author    Amit Ranjan
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CSymbolTable class
 */


#include "StdAfx.h"
#include "App.h"
#include "SymbolTable.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


BOOL CSymbolTable::bAdd(const CString& omValue)
{
    //this funtion will add a data to the array if it is not in that array.
    //If that element is already in the arry it will not add that.
    BOOL bSuccess = FALSE;
    if( CSymbolTable::nFind( omValue) == -1)
    {
        m_omData.Add(omValue);
        bSuccess = TRUE;
    }
    return bSuccess;
}


int CSymbolTable::nFind(CString omValue) const
{
    //this will find an element in the array and will retrun the position
    //of the element the the array. If it will not found in the array the
    //function will return -1.
    omValue.MakeLower();
    int nPos = -1;
    int size = m_omData.GetSize();
    for( int nIndex = 0; nIndex< size ; nIndex++)
    {
        CString omElement = m_omData.GetAt(nIndex);
        omElement.MakeLower();
        if ( omValue == omElement )
        {
            nPos = nIndex;
            nIndex = size;
        }
    }
    return nPos;
}

void CSymbolTable::vClearArray()
{
    //this will clear the contents of array.
    m_omData.RemoveAll();

}

const CString CSymbolTable::omGetAt(int nIndex)
{
    //this will return the data at a particular index in the array.
    ASSERT( nIndex < m_omData.GetSize());
    return m_omData.GetAt( nIndex);
}

int CSymbolTable::nGetSize()
{
    //this will return the size of an array.
    return m_omData.GetSize();
}
