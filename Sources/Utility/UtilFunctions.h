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
 * \file      UtilFunctions.h
 * \brief     Interface file for CUtilFunctions class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CUtilFunctions class
 */


#if !defined(AFX_UTILFUNCTIONS_H__38F67345_0E25_4FAD_A33C_24EFA2697689__INCLUDED_)
#define AFX_UTILFUNCTIONS_H__38F67345_0E25_4FAD_A33C_24EFA2697689__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CUtilFunctions  
{
public:
    // Top mask unwanted bit of the data
    static void s_vRemoveUnwantedBits(__int64& n64rData, int nLength);
    // To extend the sign bit of a signal to get 64 bit euavelent value
    static void s_vExtendSignBit( __int64& n64Val, int nSize);

protected:

// Make constructor as private so that object can not be created for this class
private:
    CUtilFunctions();
    virtual ~CUtilFunctions();
};

#endif // !defined(AFX_UTILFUNCTIONS_H__38F67345_0E25_4FAD_A33C_24EFA2697689__INCLUDED_)
