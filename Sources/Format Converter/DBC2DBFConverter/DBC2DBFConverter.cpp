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
 * \file      DBC2DBFConverter.cpp
 * \brief     Definition of converter class
 * \author    Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of the converter class.
 */

#include "StdAfx.h"
#include "DBC2DBFConverter.h"
#include "Definitions.h"
#include "Converter.h"

CDBC2DBFConverter::CDBC2DBFConverter(void)
{
}

HRESULT CDBC2DBFConverter::GetHelpText(string& pchHelpText)
{
    pchHelpText = "Converts the CANoe Database(.dbc) file to BUSMASTER Database(.dbf) file";
    return S_OK;
}

HRESULT CDBC2DBFConverter::GetConverterName(string& strConverterName)
{
    strConverterName = "DBC TO DBF Conversion";
    return S_OK;
}

HRESULT CDBC2DBFConverter::GetErrorStatus(HRESULT hResult, string& omstrStatus)
{
    switch( hResult )
    {
            /* case ERR_OUTPUT_FILE_NOTFOUND:
                 m_omstrConversionStatus = "Output File path is not found";
                 break;
             case ERR_INPUT_FILE_NOTFOUND:
                 m_omstrConversionStatus = "Input File path is not found";
                 break;*/
        case S_OK:
            m_omstrConversionStatus = "Conversion success";
            break;
        case S_FALSE:
            m_omstrConversionStatus = "Conversion failed";
            break;
        default:
            m_omstrConversionStatus = "Unknown Error";
            break;
    }
    return S_OK;
}

HRESULT CDBC2DBFConverter::GetInputFileFilters(string& pchInputDefFilters, string& pchInputFilters)
{
    pchInputDefFilters = "dbc";
    pchInputFilters = "CANoe Database File(s) (*.dbc)|*.dbc||";
    return S_OK;
}

HRESULT CDBC2DBFConverter::GetLastConversionStatus(HRESULT& hResult, string& omstrStatus)
{
    hResult = m_hResult;
    omstrStatus = m_omstrConversionStatus;
    return S_OK;
}

HRESULT CDBC2DBFConverter::GetOutputFileFilters(string& pchOutputDefFilters, string& pchOutputFilters)
{
    pchOutputDefFilters = "dbf";
    pchOutputFilters = "BUSMASTER Database File(s) (*.dbf)|*.dbf||";
    return S_OK;
}

HRESULT CDBC2DBFConverter::ConvertFile(string& chInputFile, string& chOutputFile)
{
    HRESULT hResult = S_OK;
    CConverter ouConverter;

    INT nRetVal = ouConverter.Convert(chInputFile, chOutputFile);
    // display final result
    m_omstrConversionStatus = ouConverter.GetResultString();
    if(nRetVal != 0)
    {
        m_omstrConversionStatus += ouConverter.m_omLogFilePath.data();
        m_hResult = nRetVal;
    }
    return hResult;
}

BOOL CDBC2DBFConverter::bHaveOwnWindow()
{
    return FALSE;
}

CDBC2DBFConverter::~CDBC2DBFConverter(void)
{
}
