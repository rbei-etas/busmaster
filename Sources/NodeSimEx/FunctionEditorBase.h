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
 * \file      FunctionEditorBase.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "include\basedefs.h"

class CFunctionEditorBase
{
public:
    CFunctionEditorBase();
    ~CFunctionEditorBase();

    static BOOL bInitBusInfo(SBUS_SPECIFIC_INFO& sMcNetSpecInfo, eTYPE_BUS eBus);
    /* This function create new document of type CFunctionEditorDoc,
       initializes bus specific information, return pointer of the object type
       CFunctionEditorDoc */
    static CFunctionEditorDoc* pCreateNewDocument(eTYPE_BUS eBus);
};
