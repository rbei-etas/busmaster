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
 * \file      BaseConverter.h
 * \brief     Definition of base converter class
 * \author    Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of the base converter class.
 */

#pragma once

#include "stdafx.h"

class CBaseConverter
{
public:
    virtual HRESULT GetInputFileFilters(string&, string& ) = 0;
    virtual HRESULT GetOutputFileFilters(string&, string& ) = 0;
    virtual HRESULT ConvertFile(string& chInputFile, string& chOutputFile) = 0;
    virtual HRESULT GetConverterName(string& strConverterName) = 0; 
    virtual HRESULT GetErrorStatus(HRESULT hResult, string& omstrStatus) = 0;
    virtual HRESULT GetLastConversionStatus(HRESULT& hResult, string& omstrStatus) = 0;
    virtual HRESULT GetHelpText(string& pchHelpText)=0;
    virtual BOOL bHaveOwnWindow() = 0;
    virtual HRESULT GetPropertyPage(CPropertyPage*& pPage ) = 0;
    virtual ~CBaseConverter(){};
};
