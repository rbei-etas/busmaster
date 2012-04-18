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
 * \file      DBC2DBFConverter.h
 * \brief     Definition of converter class
 * \authors   Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of converter class.
 */

#pragma once

/* Project includes */
#include "../FormatConverterApp/BaseConverter.h"

class CDBC2DBFConverter : public CBaseConverter
{
    string m_omstrConversionStatus;
    HRESULT m_hResult;
public:
    CDBC2DBFConverter(void);
    ~CDBC2DBFConverter(void);
    virtual HRESULT GetInputFileFilters(string&, string&);
    virtual HRESULT GetOutputFileFilters(string&, string&);
    virtual HRESULT ConvertFile(string& chInputFile, string& chOutputFile);
    virtual HRESULT GetConverterName(string& strConverterName);
    virtual HRESULT GetErrorStatus(HRESULT hResult, string& omstrStatus);
    virtual HRESULT GetLastConversionStatus(HRESULT& hResult, string& omstrStatus);
    virtual HRESULT GetHelpText(string& pchHelpText);
    virtual BOOL bHaveOwnWindow();
    virtual HRESULT GetPropertyPage(CPropertyPage*& pPage)
    {
        return S_FALSE;
    };
};
