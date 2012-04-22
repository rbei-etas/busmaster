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
 * \file      CNVTCToolBar.h
 * \brief     CNVTCToolBar class interface
 * \author    Ratnadip Choudhury, Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CNVTCToolBar class interface.
 */

#pragma once

#include <afxext.h>

class CNVTCToolBar : public CToolBar  
{
private:
	BOOL bSetCNVTCToolBar(UINT uToolBarType, UINT uToolBar, int nWidth, COLORREF colMask);
public:
	BOOL bLoadCNVTCToolBar(int nWidth, UINT unNormal, UINT unHot,	UINT unDisabled);
	BOOL bLoadCNVTCToolBar(int nWidth, UINT unNormal, UINT unHot,	UINT unDisabled, COLORREF clrMask);
	
public:
	CNVTCToolBar();
	virtual ~CNVTCToolBar();
};
