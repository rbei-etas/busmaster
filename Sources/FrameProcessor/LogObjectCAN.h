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
 * \file      LogObjectCAN.h
 * \brief     Definition file for CLogObjectCAN class.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for CLogObjectCAN class.
 */

#if !defined LOGOBJECTCAN_H__INCLUDED_
#define LOGOBJECTCAN_H__INCLUDED_

#include "DataTypes/Log_Datatypes.h"
#include "DataTypes/Filter_Datatypes.h"
#include "BaseLogObject.h"
#include "include/BaseDefs.h"


class CLogObjectCAN : public CBaseLogObject
{
private:
	// The filter object
	SFILTERAPPLIED_CAN m_sFilterApplied;

    // To format the header 
    void vFormatHeader(CString& omHeader);

    // To format the footer 
    void vFormatFooter(CString& omFooter);

	BOOL bToBeLogged(SFRAMEINFO_BASIC_CAN& CANInfo_Basic);

protected:
    // To copy specific data pertaining to the conrete class. 
    void Der_CopySpecificData(const CBaseLogObject* pouLogObjRef);
    // Set configuration data - concrete class specific logics
    BYTE* Der_SetConfigData(BYTE* pvDataStream);
    // Get configuration data - concrete class specific logics
    BYTE* Der_GetConfigData(BYTE* pvDataStream) const;
    // To get the total buffer size - concrete class specific logics
	UINT Der_unGetBufSize(void) const;

public:

    CLogObjectCAN(CString omVersion);    // Overloaded constructor
    ~CLogObjectCAN();   // Destructor

    // Log a CAN data object
    BOOL bLogData(const SFORMATTEDDATA_CAN&);

    // Enable / disable filter
    void EnableFilter(BOOL bEnable);

    // Get the filter info structure
    void GetFilterInfo(SFILTERAPPLIED_CAN& sFilterInfo) const;
    // Set the filter info structure
    void SetFilterInfo(const SFILTERAPPLIED_CAN& sFilterInfo);
};

#endif // LOGOBJECTCAN_H__INCLUDED_
