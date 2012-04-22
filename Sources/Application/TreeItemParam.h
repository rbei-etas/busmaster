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
 * \file      TreeItemParam.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "Utility/FlickerFreeTree.h"

typedef CArray< HTREEITEM, HTREEITEM > CItemHandleArray;

class CTreeItemParam
{
private:
    CFlickerFreeTree       m_odTreeCtrl;
    RECT                   m_omRect;     //Rectangle will be stored to shift the position
                                  //of window if new TreeCtrl is added
	CFont				   m_omFont;	 // To set the font 
    COLORREF               m_TextColour;        
    CItemHandleArray       m_aomItemHandle;
    void vRemoveTreeCtrl(void);

public:
    static UINT     m_unID;    //This will provide the ID to the TreeCtrl 
                                  //at the time of Create function

public:
    CTreeItemParam(void);
    CTreeItemParam(CWnd* pomParent, CStringArray& omStringArray, 
                   RECT& Rect, COLORREF TextColour);
    virtual ~CTreeItemParam();
    void vShift(LONG lExtent);    // -ve lExtent for down shifting of window
    void vUpdateTreeValues(CStringArray& omStringArray, COLORREF TextColour);
    void vShiftWindowTop(LONG lExtent);
	void vUpdateTreeItemRect(RECT& rcNewPos);
};
