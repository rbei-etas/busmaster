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
 * \file      MsgWndHdrInfo.h
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "include/BaseDefs.h"

class CMsgWndHdrInfo
{
private:
    CString m_somArrColTitle[MAX_MSG_WND_COL_CNT];
    SMSGWNDHDRCOL m_ssHdrColPos;
    int m_nColCount;
public:
    CMsgWndHdrInfo(void);
    ~CMsgWndHdrInfo(void);
    void vInitializeColDetails( SMSGWNDHDRCOL sHdrColPosStruct, 
                                       CString omArrColTitle[MAX_MSG_WND_COL_CNT],
                                       int nArrTitleCnt);
    void vGetHdrColStruct(SMSGWNDHDRCOL& sHdrColStruct);
    void vGetHdrColNames(CString omArrColTitle[MAX_MSG_WND_COL_CNT], int &NameCount);
};
