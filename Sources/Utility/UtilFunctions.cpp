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
 * \file      UtilFunctions.cpp
 * \brief     Implementation file for CUtilFunctions class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CUtilFunctions class
 */

// Standard include header
#include "Utils_stdafx.h"
// For CUtilFunctions class definition
#include "Utility_Structs.h"
#include "UtilFunctions.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*******************************************************************************
 Function Name  : CUtilFunctions
 Description    : Standard default constructor. This is not required as it is
                  private.
 Member of      : CUtilFunctions
 Functionality  : -
 Author(s)      : Raja N
 Date Created   : 30.07.2004
*******************************************************************************/
CUtilFunctions::CUtilFunctions()
{
}

/*******************************************************************************
 Function Name  : ~CUtilFunctions
 Description    : Standard Destructor. This is not required as it is private.
 Member of      : CUtilFunctions
 Functionality  : -
 Author(s)      : Raja N
 Date Created   : 30.07.2004
*******************************************************************************/
CUtilFunctions::~CUtilFunctions()
{
}

/*******************************************************************************
 Function Name    : s_vRemoveUnwantedBits
 Input(s)         : n64rData    - Int 64 bit data
                    nLength     - Valid Data length
 Output           :  -
 Functionality    : This function will set the unsued portion to zero. This is
                    required to get the 2's value of a signal lessage then 64
                    bit length
 Member of        : Static member of CUtilFunctions
 Friend of        :  -
 Author(s)        : Raja N
 Date Created     : 30.07.2004
 Modification By  :
 Modification on  :
*******************************************************************************/
void CUtilFunctions::s_vRemoveUnwantedBits(__int64& n64rData, int nLength)
{
    // So -1 is FFFFFFFFFFFFFFFF (as it is __int64 type)
    // if the data length is 8 bits take only FF. *th bit is sign
    // bit and that is set to denote negative number
    // Set all bits to 1. That is FFFFFFFF FFFFFFFF (-1)
    unsigned __int64 un64Mask = static_cast<unsigned __int64>(-1);
    // Create the mask
    un64Mask = un64Mask >> (defMAX_BITS - nLength);
    // Mask unwanted portion of signal details
    n64rData = n64rData & un64Mask;
}

/*******************************************************************************
 Function Name   : s_vExtendSignBit
 Input(s)        : n64Val    - Int 64 bit data
                   nSize     - Valid Data length
 Output          :  -
 Functionality   : This function extends the sign bit of a data to get actual
                   value of a signed number that is stored in 2s complement form
 Member of       : Static member of CUtilFunctions
 Friend of       :  -
 Author(s)       : Raja N
 Date Created    : 31.07.2004
 Modification By :
 Modification on :
*******************************************************************************/
void CUtilFunctions::s_vExtendSignBit( __int64& n64Val, int nSize)
{
    // Avoid processing 64 bit signals
    if( nSize < defMAX_BITS )
    {
        __int64 n64Mask = 1;
        BOOL bSignBit;
        // Shift the mask by Length  - 1 times to get sign bit value
        n64Mask <<= nSize - 1;
        // Get the sign bit value
        bSignBit = n64Val & n64Mask ? TRUE : FALSE;

        // Set the value only for negative numbers
        if( bSignBit )
        {
            // Set the Sign bit to 1
            __int64 nVal = defSIGN_MASK;
            // Shift the value to extend the value
            nVal >>= ( defMAX_BITS - nSize - 1);
            // Apply the mask
            n64Val |= nVal;
        }
    }
}
