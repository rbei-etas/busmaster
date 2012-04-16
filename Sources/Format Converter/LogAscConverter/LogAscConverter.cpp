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
 * \file      LogAscConverter.cpp
 * \brief     Implementation of the AscLogConverter class.
 * \author    Venkatanarayana Makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the LogAscConverter class.
 */

/* Project includes */
#include "LogAscConverter.h"

extern "C" int nConvertFile(FILE* fpInputFile, FILE* fpOutputFile);

CLogAscConverter::CLogAscConverter(void)
{
}

HRESULT CLogAscConverter::GetHelpText(string& pchHelpText)
{
    pchHelpText = "Converts the BUSMASTER log file(.log) to CANoe log file(.asc)";
    return S_OK;
}

HRESULT CLogAscConverter::GetConverterName(string& strConverterName)
{
	strConverterName = "LOG TO ASC Conversion";
    return S_OK;
}

HRESULT CLogAscConverter::GetErrorStatus(HRESULT hResult, string& omstrStatus)
{
    switch( hResult )
    {
        case ERR_OUTPUT_FILE_NOTFOUND:
            m_omstrConversionStatus = "Output File path is not found";
            break;
        case ERR_INPUT_FILE_NOTFOUND:
            m_omstrConversionStatus = "Input File path is not found";
            break;
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

HRESULT CLogAscConverter::GetInputFileFilters(string& pchInputDefFilters, string& pchInputFilters)
{
    pchInputDefFilters = "log";
    pchInputFilters = "BUSMASTER Log File(s) (*.log)|*.log||";
    return S_OK;
}

HRESULT CLogAscConverter::GetLastConversionStatus(HRESULT& hResult, string& omstrStatus)
{
    hResult = m_hResult;
    omstrStatus = m_omstrConversionStatus;
    return S_OK;
}

HRESULT CLogAscConverter::GetOutputFileFilters(string& pchOutputDefFilters, string& pchOutputFilters)
{
    pchOutputDefFilters = "asc";
    pchOutputFilters = "CANoe Log File(s) (*.asc)|*.asc||";
    return S_OK;
}

HRESULT CLogAscConverter::ConvertFile(string& chInputFile, string& chOutputFile)
{
    HRESULT hResult = S_OK;
    FILE* fpInputFile = NULL;
    FILE* fpOutputFile = NULL;
    fpInputFile = _tfopen(chInputFile.c_str(), _T("r"));
    if(NULL != fpInputFile)
    {
        fpOutputFile = _tfopen(chOutputFile.c_str(), _T("w"));
        if(NULL != fpOutputFile)
        {
            //yydebug = 1;
            nConvertFile(fpInputFile, fpOutputFile);
            m_omstrConversionStatus = "Conversion Completed Successfully"; 
            fclose(fpInputFile);
            fclose(fpOutputFile);
        }
        else
        {
            m_omstrConversionStatus = "Output File path is not found";
            hResult = ERR_OUTPUT_FILE_NOTFOUND;
        }
    }
    else
    {
         m_omstrConversionStatus = "Input File path is not found";
        hResult = ERR_INPUT_FILE_NOTFOUND;
    }
    m_hResult = hResult;
    return hResult;
}

BOOL CLogAscConverter::bHaveOwnWindow()
{
    return FALSE;
}

CLogAscConverter::~CLogAscConverter(void)
{
};