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
 * \file      MsgWndHdrInfo.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "StdAfx.h"
#include "MsgWndHdrInfo.h"

const int nInitialColCount = 7;

CMsgWndHdrInfo::CMsgWndHdrInfo(void)
{
    m_nColCount = nInitialColCount;
    m_somArrColTitle[0] = _T("Time              ");
    m_somArrColTitle[1] =_T("Name / Code       ");
    m_somArrColTitle[2] =_T("ID      ");
    m_somArrColTitle[3] =_T("Dir          ");
    m_somArrColTitle[4] =_T("Msg Type");
    m_somArrColTitle[5] =_T("DLC ");
    m_somArrColTitle[6] =_T("Data Bytes                                     ");
}

CMsgWndHdrInfo::~CMsgWndHdrInfo(void)
{
}

void CMsgWndHdrInfo:: vInitializeColDetails(SMSGWNDHDRCOL sHdrColPosStruct,
        CString omArrColTitle[MAX_MSG_WND_COL_CNT],
        int nArrTitleCnt)
{
    memcpy(&m_ssHdrColPos, &sHdrColPosStruct, sizeof(SMSGWNDHDRCOL));
    int nTitleCnt = nArrTitleCnt;

    if (nTitleCnt > MAX_MSG_WND_COL_CNT)
    {
        nTitleCnt = MAX_MSG_WND_COL_CNT;
    }

    for (int i = 0; i < nTitleCnt; i++)
    {
        m_somArrColTitle[i] = omArrColTitle[i];
    }

    m_nColCount = nTitleCnt;
}


void CMsgWndHdrInfo::vGetHdrColStruct(SMSGWNDHDRCOL& sHdrColStruct)
{
    memcpy(&sHdrColStruct,&m_ssHdrColPos, sizeof(SMSGWNDHDRCOL));
}

void CMsgWndHdrInfo::vGetHdrColNames(CString omArrColTitle[MAX_MSG_WND_COL_CNT], int& NameCount)
{
    for (int i = 0; i < m_nColCount; i++)
    {
        omArrColTitle[i] = m_somArrColTitle[i];
    }

    NameCount = m_nColCount;
}