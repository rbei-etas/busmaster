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
 * \file      GUI_FormatMsgJ1939.h
 * \brief     Definition of CFormatMsgJ1939 class
 * \author    Anish kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of CFormatMsgJ1939 class
 */

#pragma once

#include "GUI_FormatMsgCommon.h"
#include "include/BaseDefs.h"
#include "DataTypes/J1939_DataTypes.h"

class CFormatMsgJ1939 : public CFormatMsgCommon
{
private:
    void vFormatTime(BYTE bExprnFlag, PSTJ1939_MSG psJ1939BData,
                     PSFORMATTEDATA_J1939 psJ1939FData);
    void vFormatDataAndId(BYTE bExprnFlag, PSTJ1939_MSG psJ1939BData,
                          PSFORMATTEDATA_J1939 psJ1939FData);
    USHORT usProcessCurrErrorEntry(SERROR_INFO& sErrorInfo);
    char* vFormatCurrErrorEntry(USHORT usErrorID);
public:
    CFormatMsgJ1939(void);
    ~CFormatMsgJ1939(void);
    void vFormatJ1939DataMsg(PSTJ1939_MSG psJ1939BData,
                             SFORMATTEDATA_J1939* CurrDataJ1939,
                             BYTE bExprnFlag_Log);
};
