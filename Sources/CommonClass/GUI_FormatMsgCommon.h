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
 * \file      GUI_FormatMsgCommon.h
 * \brief     Definition of CFormatMsgCommon class
 * \author    Anish kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of CFormatMsgCommon class
 */

#pragma once

#include "CommonClass/RefTimeKeeper.h"

class CFormatMsgCommon: public CRefTimeKeeper
{
protected:
    CFormatMsgCommon(void);
    void vFormatTimeStamp(DWORD dwTimeStamp, char acTime[]);
public:
    ~CFormatMsgCommon(void);
    void vCalculateAndFormatTM(BYTE bExprnFlag, UINT64 TimeStamp,char acTime[]);
    void vCalAndFormatTM_Offline(BYTE bExprnFlag,  UINT64 TimeStamp, char acTime[]);
    //void vSetRelBaseTime(INT64 qwRelBaseTime); //Called to sart afresh for append mode
};
