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
 * \file      CNVTCImageList.h
 * \brief     CNVTCImageList class interface
 * \authors   Ratnadip Choudhury, Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CNVTCImageList class interface.
 */

#pragma once

#include <afxcmn.h>

class CNVTCImageList 
	  : public CImageList  
{
public:				
	BOOL bCreateCNVTC(UINT nTBID, int nCX, int nCY, COLORREF clrMask);
	BOOL bCreateCNVTC(UINT nTBID, int nCX, int nCY);
	BOOL bCreateCNVTC(UINT nTBID, int nCX, COLORREF clrMask);
	BOOL bCreateCNVTC(UINT nTBID, int nCX);
	BOOL bCreateCNVTC(UINT nTBID, COLORREF clrMask);
	BOOL bCreateCNVTC(UINT nTBID);

public:
	CNVTCImageList();
	virtual ~CNVTCImageList();
};
