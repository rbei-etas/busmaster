#include "StdAfx.h"
#include "DBF2DBCConverter.h"
#include "Definitions.h"
#include "Converter.h"

CDBF2DBCConverter::CDBF2DBCConverter(void)
{
}

HRESULT CDBF2DBCConverter::GetHelpText(string& pchHelpText)
{
    pchHelpText = "Converts the BUSMASTER Database(.dbf) file to CANoe Database(.dbc) file";
    return S_OK;
}

HRESULT CDBF2DBCConverter::GetConverterName(string& strConverterName)
{
	strConverterName = "DBF TO DBC Conversion";
    return S_OK;
}

HRESULT CDBF2DBCConverter::GetErrorStatus(HRESULT hResult, string& omstrStatus)
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

HRESULT CDBF2DBCConverter::GetInputFileFilters(string& pchInputDefFilters, string& pchInputFilters)
{
    pchInputDefFilters = "dbf";
    pchInputFilters = "BUSMASTER Database File(s) (*.dbf)|*.dbf||";
    return S_OK;
}

HRESULT CDBF2DBCConverter::GetLastConversionStatus(HRESULT& hResult, string& omstrStatus)
{
    hResult = m_hResult;
    omstrStatus = m_omstrConversionStatus;
    return S_OK;
}

HRESULT CDBF2DBCConverter::GetOutputFileFilters(string& pchOutputDefFilters, string& pchOutputFilters)
{
    pchOutputDefFilters = "dbc";
    pchOutputFilters = "CANoe Database File(s) (*.dbc)|*.dbc||";
    return S_OK;
}

HRESULT CDBF2DBCConverter::ConvertFile(string& chInputFile, string& chOutputFile)
{
    HRESULT hResult = S_OK;
    CConverter ouConverter;
	INT nRetVal = ouConverter.Convert(chInputFile.c_str(), chOutputFile.c_str());
    m_omstrConversionStatus = ouConverter.GetResultString();
    if(nRetVal != 0)
    {
        m_omstrConversionStatus += ouConverter.m_omLogFilePath;
        m_hResult = nRetVal;
    }
    return hResult;
}

BOOL CDBF2DBCConverter::bHaveOwnWindow()
{
    return FALSE;
}

CDBF2DBCConverter::~CDBF2DBCConverter(void)
{
}
