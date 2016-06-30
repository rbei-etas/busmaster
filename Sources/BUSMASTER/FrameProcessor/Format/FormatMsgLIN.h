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
* \file      FormatMsgLIN.h
* \brief     Definition of CFormatMsgLIN class.
* \author    Shashank Vernekar
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*
* Definition of CFormatMsgLIN class.
*/

#pragma once

#include "FormatMsgCommon.h"
#include "include/BaseDefs.h"
#include "CANDriverDefines.h"

class CFormatMsgLIN : public CFormatMsgCommon
{
public:
    CFormatMsgLIN(CRefTimeKeeper& ouRefTimeKeeper);
    ~CFormatMsgLIN();
    void vFormatLINDataMsg(STLINDATA* pMsgLIN,
                           SFORMATTEDDATA_LIN* CurrDataLIN,
                           BYTE bExprnFlag_Log);

private:
    //[RS_LIN_06_07]
    void vFormatTime(BYTE bExprnFlag, SFORMATTEDDATA_LIN* CurrDataLIN);
    void vFormatDataAndId(BYTE bExprnFlag, SFORMATTEDDATA_LIN* CurrDataLIN);
    void vGetLinEventDescription(SERROR_INFO_LIN sLinErrorInfo, std::string& strDesc);
};
