#include "StdAfx.h"
#include "DBC2DBFConverter.h"
#include "Definitions.h"
#include "Converter.h"
CDBC2DBFConverter::CDBC2DBFConverter(void)
{
}
HRESULT CDBC2DBFConverter::GetHelpText(TCHAR* pchHelpText)
{
    if(pchHelpText != NULL)
    {
        _tcscpy(pchHelpText, _T("Converts the CANoe Database(.dbc) file to BUSMASTER Database(.dbf) file"));
        return S_OK;
    }
    return S_FALSE;
}
HRESULT CDBC2DBFConverter::GetConverterName(string& strConverterName)
{
	strConverterName = "DBC TO DBF Conversion";
    return S_OK;
}
HRESULT CDBC2DBFConverter::GetErrorStatus(HRESULT hResult, CString& omstrStatus)
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
HRESULT CDBC2DBFConverter::GetInputFileFilters(TCHAR* pchInputDefFilters, TCHAR* pchInputFilters)
{
    HRESULT hResult = S_FALSE;
    if(NULL != pchInputDefFilters)
    {
        _tcscpy(pchInputDefFilters, _T("dbc"));
        hResult = S_OK;
    }
    else
    {
        hResult = S_FALSE;
    }
    if(NULL != pchInputFilters)
    {
        _tcscpy(pchInputFilters, _T("CANoe Database File(s) (*.dbc)|*.dbc||"));
        hResult = S_OK;
    }
    return hResult;
}
HRESULT CDBC2DBFConverter::GetLastConversionStatus(HRESULT& hResult, CString& omstrStatus)
{
    hResult = m_hResult;
    omstrStatus = m_omstrConversionStatus;
    return S_OK;
}

HRESULT CDBC2DBFConverter::GetOutputFileFilters(TCHAR* pchOutputDefFilters, TCHAR* pchOutputFilters)
{
    HRESULT hResult = S_FALSE;
    if(NULL != pchOutputDefFilters)
    {
        _tcscpy(pchOutputDefFilters, _T("dbf"));
        hResult = S_OK;
    }
    else
    {
        hResult = S_FALSE;
    }
    if(NULL != pchOutputFilters)
    {
        _tcscpy(pchOutputFilters, _T("BUSMASTER Database File(s) (*.dbf)|*.dbf||"));
        hResult = S_OK;
    }
    return hResult;
}
HRESULT CDBC2DBFConverter::ConvertFile(TCHAR* chInputFile, TCHAR* chOutputFile)
{
    HRESULT hResult = S_OK;
    /*extern FILE*  yyin;
	extern FILE*  yyout;
    */
    CConverter ouConverter;

    INT nRetVal = ouConverter.Convert((LPCTSTR)chInputFile,(LPCTSTR)chOutputFile);
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
};