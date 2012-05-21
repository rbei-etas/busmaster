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
 * \file      LogObjectJ1939.h
 * \brief     Declaration file for the exported function used to get the
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Declaration file for the exported function used to get the
 */

#pragma once

#include "include/BaseDefs.h"
#include "DataTypes/Log_Datatypes.h"
#include "DataTypes/Filter_Datatypes.h"
#include "BaseLogObject.h"
#include "DataTypes/J1939_DataTypes.h"


class CLogObjectJ1939 : public CBaseLogObject
{
private:
    // The filter object
    SFILTERAPPLIED_J1939 m_sFilterApplied;

    // To format the header
    void vFormatHeader(CString& omHeader);

    // To format the footer
    void vFormatFooter(CString& omFooter);

    BOOL bToBeLogged(SFRAMEINFO_BASIC_J1939& J1939Info_Basic);

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

    CStringArray        m_omListDBFiles;
    SCONTROLLER_DETAILS*    m_pasControllerDetails;
    int                 m_nNumChannels;

    CLogObjectJ1939(CString omVersion);    // Overloaded constructor
    ~CLogObjectJ1939();   // Destructor

    // Log a J1939 data object
    BOOL bLogData(const SFORMATTEDATA_J1939&);

    // Enable / disable filter
    void EnableFilter(BOOL bEnable);

    // Get the filter info structure
    void GetFilterInfo(SFILTERAPPLIED_J1939& sFilterInfo) const;
    // Set the filter info structure
    void SetFilterInfo(const SFILTERAPPLIED_J1939& sFilterInfo);

    // Get the list of database files associated
    void Der_GetDatabaseFiles(CStringArray& omList);
    // Set the list of database files associated
    void Der_SetDatabaseFiles(const CStringArray& omList);
    // Set the baud rate details for each channel
    void Der_SetChannelBaudRateDetails(SCONTROLLER_DETAILS* controllerDetails,
                                       int nNumChannels);
    // To get the channel baud rate info for each channel
    void Der_GetChannelBaudRateDetails(SCONTROLLER_DETAILS* controllerDetails,
                                       int& nNumChannels);

};
