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
 * \file      ComputeCheckSum.h
 * \brief     This file contains definition of CComputeCheckSum class
 * \author    Anish Kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contains definition of CComputeCheckSum class
 */

#pragma once

class CComputeCheckSum  
{
public:
	// Default constructor
	CComputeCheckSum();
	// Default destructor
	virtual ~CComputeCheckSum();
protected:
	// compute the check for a buffer pass as parameter
	BOOL bComputeCheckSum(UCHAR *pcBuff,DWORD dwSize, UCHAR* pucCheckSum);
	// Get the computed checksum of file whose name is passed as parameter
    // Also the computed checksum and stored checksum is returned.
	BOOL bGetCheckSum(CString &omStrConfigFileName, UCHAR* pucCheckSum,
					  UCHAR* pucCheckSumInFile);
	// Sets the checksum at last byte location after computing it.
	BOOL bSetCheckSum(CString& omStrConfigFileName,UCHAR* pucCheckSum);
    BOOL COM_bSetCheckSum(CString &omStrConfigFileName,
                                        UCHAR* pucCheckSum, CString &strError);

};
