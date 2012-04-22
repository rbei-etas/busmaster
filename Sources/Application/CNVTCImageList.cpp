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
 * \file      CNVTCImageList.cpp
 * \authors   Ratnadip Choudhury, Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
// CNVTCImageList.cpp: CNVTCImageList class implementation.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CNVTCImageList.h"

//Constructor
CNVTCImageList::CNVTCImageList()
{

}

//Destructor
CNVTCImageList::~CNVTCImageList()
{

}

BOOL CNVTCImageList::bCreateCNVTC(UINT nTBID, int nCX, int nCY)
{
	return bCreateCNVTC(nTBID, nCY, nCX, RGB(255,255,255));
}

BOOL CNVTCImageList::bCreateCNVTC(UINT nTBID, int nCX, COLORREF clrMask)
{
	return bCreateCNVTC(nTBID, nCX, nCX, clrMask);
}

BOOL CNVTCImageList::bCreateCNVTC(UINT nTBID, int nCX, int nCY, COLORREF clrMask)
{
	if(CImageList::Create(nCX, nCY,ILC_MASK | ILC_COLOR32,0,0))		
	{
		CBitmap bmpRes;
		if(bmpRes.LoadBitmap(nTBID))
		{
			Add(&bmpRes, clrMask);	
			return TRUE;
		}
		else
			return FALSE;
	}
	else
		return FALSE;
}

BOOL CNVTCImageList::bCreateCNVTC(UINT nTBID, COLORREF clrMask)
{
	CBitmap objRes;
	if(objRes.LoadBitmap(nTBID))	
	{	
		BITMAP bmpRes;
		objRes.GetBitmap(&bmpRes);		
		
		return bCreateCNVTC(nTBID, bmpRes.bmHeight, bmpRes.bmHeight, clrMask);
	}
	else
		return FALSE;
}

BOOL CNVTCImageList::bCreateCNVTC(UINT nTBID, int nCX)
{
	return bCreateCNVTC(nTBID, nCX, nCX, RGB(255,255,255));
}

BOOL CNVTCImageList::bCreateCNVTC(UINT nTBID)
{	
	CBitmap objRes;
	if(objRes.LoadBitmap(nTBID))		
	{
		BITMAP bmpRes;
		objRes.GetBitmap(&bmpRes);		

		return bCreateCNVTC(nTBID, bmpRes.bmHeight, bmpRes.bmHeight, RGB(255,255,255));
	}
	else
		return FALSE;
}

