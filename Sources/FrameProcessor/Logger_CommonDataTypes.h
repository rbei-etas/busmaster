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
 * \file      Logger_CommonDataTypes.h
 * \brief     Definition of CParamLoggerBUS class.
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of CParamLoggerBUS class.
 */

#pragma once

class CParamLoggerBUS
{
public:
    TCHAR m_acVersion[MAX_PATH];        // Version info of the application suite
    Base_WrapperErrorLogger* m_pILog;   // Error logger module
    DWORD dwClientID;                   // Client identifier

    CParamLoggerBUS()                   // Default constructor
    {
        m_pILog = NULL;
        dwClientID = 0;
        for (int i = 0; i < MAX_PATH; i++)
        {
            m_acVersion[i] = L'\0';
        }
    };

    ~CParamLoggerBUS()                  // Destructor
    {
    };

    CParamLoggerBUS& operator=(const CParamLoggerBUS& RefObj)
    {
        m_pILog     = RefObj.m_pILog;
        dwClientID  = RefObj.dwClientID;
        strcpy_s(m_acVersion, RefObj.m_acVersion);

        return *this;
    }
};
