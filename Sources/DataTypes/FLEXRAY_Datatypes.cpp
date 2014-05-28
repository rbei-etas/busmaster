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
 * \file      FLEXRAY_Datatypes.cpp
 * \brief     Implementation file for FLEXRAYDatatypes.h
 * \author    Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for FLEXRAYDatatypes.h
 */

/* Insert your headers here */
#include "DataTypes_StdAfx.h"
#include "FLEXRAY_Datatypes.h"

/* START OF CFlxTxMsgBuffer IMPLEMENTATION */
CFlxTxMsgBuffer::CFlxTxMsgBuffer():
    m_dwTxMsgCount(0),
    m_psFlxTxMsgList(nullptr)
{
}

CFlxTxMsgBuffer::~CFlxTxMsgBuffer()
{
}

void CFlxTxMsgBuffer::vResetAll(void)
{
    if (nullptr != m_psFlxTxMsgList)
    {
        delete[] m_psFlxTxMsgList;
        m_psFlxTxMsgList = nullptr;
        m_dwTxMsgCount = 0x0;
    }
}
/* END OF CFlxTxMsgBuffer IMPLEMENTATION */