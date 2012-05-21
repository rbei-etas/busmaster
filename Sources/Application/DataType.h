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
 * \file      DataType.h
 * \brief     This header file contains definitions of necessary user
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains definitions of necessary user
 */

#pragma once

const int DATA_BUFFER_LENGTH = 120;
const int DATA_LENGTH_MAX = 8;
const int DATA_TX_FLAG = 3;

typedef struct tagMsgParameters
{
    unsigned int unMessageID;
    unsigned char ucDLC;
    int nMessageType;
    char acTxMode[DATA_TX_FLAG];
    unsigned char aucData[DATA_LENGTH_MAX];
    CString omMsgStr;
} SMsgParameters;

typedef struct tagCanIDList
{
    CString  omCANIDName;
    unsigned int nCANID;
    COLORREF Colour;
} SCanIDList;
