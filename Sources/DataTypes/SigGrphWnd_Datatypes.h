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
 * \file      SigGrphWnd_Datatypes.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

// Structure for Signal Graph Window interpreted data.
struct sSigValue
{
	__int64 m_nRawValue;	
	double m_dPhysical;
};
struct tagInterpretData
{
    unsigned int unMsgID;                   // Message ID
    char m_acSigName[64];					// Signal Name
    LONGLONG m_nTimeStamp;					// Time Stamp
	short m_shType;							// Signal Value Type
	sSigValue m_unValue;					//Signal value as union.
};

typedef tagInterpretData sInterpretDataList;
typedef sInterpretDataList SINTERPRETDATA_LIST;

const INT SIZE_INTRP_DATA = sizeof(SINTERPRETDATA_LIST);
