#include "StdAfx.h"
#include "DBF2DBCConverter.h"
#include "Definitions.h"
#include "Converter.h"
CDBF2DBCConverter::CDBF2DBCConverter(void)
{
}
HRESULT CDBF2DBCConverter::GetHelpText(TCHAR* pchHelpText)
{
    if(pchHelpText != NULL)
    {
        _tcscpy(pchHelpText, _T("Converts the BUSMASTER Database(.dbf) file to CANoe Database(.dbc) file"));
        return S_OK;
    }
    return S_FALSE;
}
HRESULT CDBF2DBCConverter::GetConverterName(TCHAR* strConverterName)
{
    if(strConverterName != NULL)
    {
        strcpy(strConverterName, _T("DBF TO DBC Conversion"));
    }
    return S_OK;
}
HRESULT CDBF2DBCConverter::GetErrorStatus(HRESULT hResult, CString& omstrStatus)
{
    switch( hResult )
    {
       /* case ERR_OUTPUT_FILE_NOTFOUND:
            m_omstrConversionStatus = _T("Output File path is not found");
            break;
        case ERR_INPUT_FILE_NOTFOUND:
            m_omstrConversionStatus = _T("Input File path is not found");
            break;*/
        case S_OK:
            m_omstrConversionStatus = _T("Conversion success");
            break;
        case S_FALSE:
            m_omstrConversionStatus = _T("Conversion failed");
            break;
        default:
            m_omstrConversionStatus = _T("Unknown Error");
            break;
    }   
    return S_OK;
}
HRESULT CDBF2DBCConverter::GetInputFileFilters(TCHAR* pchInputDefFilters, TCHAR* pchInputFilters)
{
    HRESULT hResult = S_FALSE;
    if(NULL != pchInputDefFilters)
    {
        _tcscpy(pchInputDefFilters, _T("dbf"));
        hResult = S_OK;
    }
    else
    {
        hResult = S_FALSE;
    }
    if(NULL != pchInputFilters)
    {
        _tcscpy(pchInputFilters, _T("BUSMASTER Database File(s) (*.dbf)|*.dbf||"));
        hResult = S_OK;
    }
    return hResult;
}
HRESULT CDBF2DBCConverter::GetLastConversionStatus(HRESULT& hResult, CString& omstrStatus)
{
    hResult = m_hResult;
    omstrStatus = m_omstrConversionStatus;
    return S_OK;
}

HRESULT CDBF2DBCConverter::GetOutputFileFilters(TCHAR* pchOutputDefFilters, TCHAR* pchOutputFilters)
{
    HRESULT hResult = S_FALSE;
    if(NULL != pchOutputDefFilters)
    {
        _tcscpy(pchOutputDefFilters, _T("dbc"));
        hResult = S_OK;
    }
    else
    {
        hResult = S_FALSE;
    }
    if(NULL != pchOutputFilters)
    {
        _tcscpy(pchOutputFilters, _T("CANoe Database File(s) (*.dbc)|*.dbc||"));
        hResult = S_OK;
    }
    return hResult;
}
HRESULT CDBF2DBCConverter::ConvertFile(TCHAR* chInputFile, TCHAR* chOutputFile)
{
    HRESULT hResult = S_OK;
    /*extern FILE*  yyin;
	extern FILE*  yyout;
    */
    CConverter ouConverter;
    INT nRetVal = ouConverter.Convert((LPCTSTR)chInputFile,(LPCTSTR)chOutputFile);
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
};