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
 * \file      DBF2DBCConverter.cpp
 * \brief     Implementation of the CDBF2DBCConverter class.
 * \authors   Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the CDBF2DBCConverter class.
 */

/* Project includes */
#include "Converter.h"
#include "DBF2DBCConverter.h"
#include "Definitions.h"

/**
 * \brief Constructor
 *
 * Constructor of CDBF2DBCConverter
 */
CDBF2DBCConverter::CDBF2DBCConverter(void)
{
}

/**
 * \brief Destructor
 *
 * Destructor of CDBF2DBCConverter
 */
CDBF2DBCConverter::~CDBF2DBCConverter(void)
{
}

/**
 * \brief      Get help text
 * \param[out] pchHelpText Help Text
 * \return     Result code
 *
 * Returns pchHelpText containing the help text.
 */
HRESULT CDBF2DBCConverter::GetHelpText(string& pchHelpText)
{
    pchHelpText = "Converts the BUSMASTER Database(.dbf) file to CANoe Database(.dbc) file";
    return S_OK;
}

/**
 * \brief      Get converter name
 * \param[out] strConverterName Converter Name
 * \return     Result code
 *
 * Returns strConverterName containing the converter name.
 */
HRESULT CDBF2DBCConverter::GetConverterName(string& strConverterName)
{
    strConverterName = "DBF TO DBC Conversion";
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
HRESULT CDBF2DBCConverter::GetErrorStatus(HRESULT hResult, string& omstrStatus)
{
    switch( hResult )
    {
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
HRESULT CDBF2DBCConverter::GetInputFileFilters(string& pchInputDefFilters, string& pchInputFilters)
{
    pchInputDefFilters = "dbf";
    pchInputFilters = "BUSMASTER Database File(s) (*.dbf)|*.dbf||";
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
HRESULT CDBF2DBCConverter::GetLastConversionStatus(HRESULT& hResult, string& omstrStatus)
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
HRESULT CDBF2DBCConverter::GetOutputFileFilters(string& pchOutputDefFilters, string& pchOutputFilters)
{
    pchOutputDefFilters = "dbc";
    pchOutputFilters = "CANoe Database File(s) (*.dbc)|*.dbc||";
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
HRESULT CDBF2DBCConverter::ConvertFile(string& chInputFile, string& chOutputFile)
{
    HRESULT hResult = S_OK;
    CConverter ouConverter;
    INT nRetVal = ouConverter.Convert(chInputFile.c_str(), chOutputFile.c_str());
    ouConverter.GetResultString(m_omstrConversionStatus);

    if(nRetVal != 0)
    {
        m_omstrConversionStatus += ouConverter.m_omLogFilePath;
        m_hResult = nRetVal;
    }

    return hResult;
}

/**
 * \brief     Returns if it has an own window
 * \return    True, if it has an own window.
 *
 * This returns true, if the converter has an own window, false otherwise.
 */
BOOL CDBF2DBCConverter::bHaveOwnWindow()
{
    return FALSE;
}
