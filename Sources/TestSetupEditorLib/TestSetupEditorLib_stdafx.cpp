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
 * \file      TestSetupEditorLib_stdafx.cpp
 * \brief     Source file that includes just the standard includes
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file that includes just the standard includes
 */

// stdafx.cpp :
// TestSetupEditorLib.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "TestSetupEditorLib_stdafx.h"

/******************************************************************************
Function Name  :  strCopyBSTRToCString
Input(s)       :  CComVariant& varSrc
Output         :  CString
Functionality  :  Copies BSTR to CString
Member of      :  -
Friend of      :  -
Author(s)      :  Arunkumar K
Date Created   :  20/07/2011
Modifications  :
Codetag        :
******************************************************************************/
CString strCopyBSTRToCString(CComVariant& varSrc)
{
    CString strDest ;
    char tChar = NULL;
    _bstr_t bstrNodeValue(varSrc.bstrVal);
    ULONG ulLen = bstrNodeValue.length();

    for (unsigned int i = 0; i< ulLen*2; i+=2)
    {
        tChar = (char)*(varSrc.pcVal+i);
        strDest.Insert(i/2, tChar);
    }

    tChar = NULL;
    strDest.Insert(ulLen, tChar);
    return strDest;
}

