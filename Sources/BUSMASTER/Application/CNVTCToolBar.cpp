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
 * \file      CNVTCToolBar.cpp
 * \authors   Ratnadip Choudhury, Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
//CNVTCToolBar.cpp: CNVTCToolBar class implementation.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CNVTCToolBar.h"
#include "CNVTCImageList.h"

//Constructor
CNVTCToolBar::CNVTCToolBar()
{
}

//Destructor
CNVTCToolBar::~CNVTCToolBar()
{
}

BOOL CNVTCToolBar::bLoadCNVTCToolBar(int nWidth, UINT unNormal, UINT unHot, UINT unDisabled, COLORREF clrMask)
{
    if (bSetCNVTCToolBar(TB_SETIMAGELIST, unNormal, nWidth, clrMask))
    {
        if (bSetCNVTCToolBar(TB_SETHOTIMAGELIST, unHot, nWidth, clrMask))
        {
            if (unDisabled)
            {
                if (bSetCNVTCToolBar(TB_SETDISABLEDIMAGELIST, unDisabled, nWidth, clrMask))
                {
                    return TRUE;
                }
                else
                {
                    return FALSE;
                }
            }
        }

        return FALSE;
    }
    else
    {
        return FALSE;
    }
}

BOOL CNVTCToolBar::bSetCNVTCToolBar(UINT unToolBarType, UINT unToolBar, int nWidth, COLORREF clrMask)
{
    CNVTCImageList objImageList;
    objImageList.bCreateCNVTC(unToolBar,nWidth,clrMask);
    SendMessage(unToolBarType, 0, (LPARAM)objImageList.m_hImageList);
    objImageList.Detach();
    return TRUE;
}

BOOL CNVTCToolBar::bLoadCNVTCToolBar(int nWidth, UINT unNormal, UINT unHot, UINT unDisabled)
{
    return bLoadCNVTCToolBar(nWidth, unNormal, unHot, unDisabled, RGB(255,255,255));
}
