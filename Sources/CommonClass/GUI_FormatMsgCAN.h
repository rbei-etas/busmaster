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
 * \file      GUI_FormatMsgCAN.h
 * \brief     Definition of CFormatMsgCAN class
 * \author    Anish Kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of CFormatMsgCAN class
 */

#pragma once

#include "GUI_FormatMsgCommon.h"
#include "include/BaseDefs.h"
#include "include/struct_can.h"

class CFormatMsgCAN : public CFormatMsgCommon
{
private:
    void vFormatTime(BYTE bExprnFlag, SFORMATTEDDATA_CAN* CurrDataCAN);
    void vFormatDataAndId(BYTE bExprnFlag, SFORMATTEDDATA_CAN* CurrDataCAN);
    USHORT usProcessCurrErrorEntry(SERROR_INFO& sErrorInfo);
    TCHAR* vFormatCurrErrorEntry(USHORT usErrorID);
public:
    CFormatMsgCAN(void);
    ~CFormatMsgCAN(void);
    void vFormatCANDataMsg(STCANDATA* pMsgCAN,
                           SFORMATTEDDATA_CAN* CurrDataCAN,
                           BYTE bExprnFlag_Log);
};
