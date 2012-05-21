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
 * \authors   Venkatanarayana Makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the LogAscConverter class.
 */

/* Project includes */
#include "LogAscConverter.h"

extern "C" int nConvertFile(FILE* fpInputFile, FILE* fpOutputFile);

/**
 * \brief Constructor
 *
 * Constructor of CLogAscConverter
 */
CLogAscConverter::CLogAscConverter(void)
{
}

/**
 * \brief Destructor
 *
 * Destructor of CAscLogConverter
 */
CLogAscConverter::~CLogAscConverter(void)
{
}

/**
 * \brief      Get help text
 * \param[out] pchHelpText Help Text
 * \return     Result code
 *
 * Returns pchHelpText containing the help text.
 */
HRESULT CLogAscConverter::GetHelpText(string& pchHelpText)
{
    pchHelpText = "Converts the BUSMASTER log file(.log) to CANoe log file(.asc)";
    return S_OK;
}

/**
 * \brief      Get converter name
 * \param[out] strConverterName Converter Name
 * \return     Result code
 *
 * Returns strConverterName containing the converter name.
 */
HRESULT CLogAscConverter::GetConverterName(string& strConverterName)
{
    strConverterName = "LOG TO ASC Conversion";
    return S_OK;
}

/**
 * \brief      Get error status string
 * \param[in]  hResult Error code
 * \param[out] omstrStatus Corresponding error string
 * \return     Result code
 *
 * Returns omstrStatus containing the error string depending on hResult.
 */
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
            m_omstrConversionStatus = "Unknown";
            break;
    }

    return S_OK;
}

/**
 * \brief      Get input file filter type and name
 * \param[out] pchInputDefFilters file filter types
 * \param[out] pchInputFilters file filter name
 * \return     Result code
 *
 * Returns strings containing the file extensions and a
 * corresponding filter description.
 */
HRESULT CLogAscConverter::GetInputFileFilters(string& pchInputDefFilters, string& pchInputFilters)
{
    pchInputDefFilters = "log";
    pchInputFilters = "BUSMASTER Log File(s) (*.log)|*.log||";
    return S_OK;
}

/**
 * \brief      Get last conversion status
 * \param[out] hResult Last conversion status.
 * \param[out] omstrStatus String describing the last conversion status.
 * \return     Result code
 *
 * Returns a string containing the last conversion status.
 */
HRESULT CLogAscConverter::GetLastConversionStatus(HRESULT& hResult, string& omstrStatus)
{
    hResult = m_hResult;
    omstrStatus = m_omstrConversionStatus;
    return S_OK;
}

/**
 * \brief      Get output file filter type and name
 * \param[out] pchOutputDefFilters file filter types
 * \param[out] pchOutputFilters file filter name
 * \return     Result code
 *
 * Returns strings containing the file extensions and a
 * corresponding filter description.
 */
HRESULT CLogAscConverter::GetOutputFileFilters(string& pchOutputDefFilters, string& pchOutputFilters)
{
    pchOutputDefFilters = "asc";
    pchOutputFilters = "CANoe Log File(s) (*.asc)|*.asc||";
    return S_OK;
}

/**
 * \brief     Conversion function
 * \param[in] chInputFile Input file name to convert from
 * \param[in] chOutputFile Output file name to convert to
 * \return    Result code
 *
 * This is the actual conversion function with input and output file name.
 */
HRESULT CLogAscConverter::ConvertFile(string& chInputFile, string& chOutputFile)
{
    HRESULT hResult = S_OK;
    FILE* fpInputFile = NULL;
    FILE* fpOutputFile = NULL;
    fopen_s(&fpInputFile, chInputFile.c_str(), _T("r"));

    if(NULL != fpInputFile)
    {
        fopen_s(&fpOutputFile, chOutputFile.c_str(), _T("w"));

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

/**
 * \brief     Returns if it has an own window
 * \return    True, if it has an own window.
 *
 * This returns true, if the converter has an own window, false otherwise.
 */
BOOL CLogAscConverter::bHaveOwnWindow()
{
    return FALSE;
}
