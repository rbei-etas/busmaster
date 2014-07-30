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
 * \file      GUI_FormatMsgEthernet.h
 * \brief     Definition of CFormatMsgEthernet class
 * \author    Ashwin R Uchil
 * \copyright Copyright (c) 2014, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of CFormatMsgEthernet class
 */

#pragma once

#include "GUI_FormatMsgCommon.h"
#include "include/BaseDefs.h"
#include "include/Struct_ETHERNET.h"

class CFormatMsgEthernet : public CFormatMsgCommon
{
private:
    void vFormatTime(BYTE bExprnFlag, SFORMATTEDDATA_ETHERNET* CurrDataEthernet);
    void vFormatDataAndId(BYTE bExprnFlag, SFORMATTEDDATA_ETHERNET* CurrDataEthernet);
    USHORT usProcessCurrErrorEntryEthernet(sETHERNET_ERROR_INFO& sErrorInfo);
    char* vFormatCurrErrorEntry(USHORT usErrorID);
    void vGetLinEventDescription(sETHERNET_ERROR_INFO sLinErrorInfo, string& strDesc);



public:
    CFormatMsgEthernet(void);
    ~CFormatMsgEthernet(void);
    void vFormatEthernetDataMsg(STETHERNETDATA* pMsgEthenet,
                           SFORMATTEDDATA_ETHERNET* CurrDataEthernet,
                           BYTE bExprnFlag_Log);
};