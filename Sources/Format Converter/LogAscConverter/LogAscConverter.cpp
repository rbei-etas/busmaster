// AscLogConverter.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "LogAscConverter.h"

extern "C" int nConvertFile(FILE* fpInputFile, FILE* fpOutputFile);

CLogAscConverter::CLogAscConverter(void)
{
}
HRESULT CLogAscConverter::GetHelpText(TCHAR* pchHelpText)
{
    if(pchHelpText != NULL)
    {
        _tcscpy(pchHelpText, _T("Converts the BUSMASTER log file(.log) to CANoe log file(.asc)"));
        return S_OK;
    }
    return S_FALSE;
}

HRESULT CLogAscConverter::GetConverterName(TCHAR* strConverterName)
{
    if(strConverterName != NULL)
    {
        strcpy(strConverterName, _T("LOG TO ASC Conversion"));
    }
    return S_OK;
}
HRESULT CLogAscConverter::GetErrorStatus(HRESULT hResult, CString& omstrStatus)
{
    switch( hResult )
    {
        case ERR_OUTPUT_FILE_NOTFOUND:
            m_omstrConversionStatus = _T("Output File path is not found");
            break;
        case ERR_INPUT_FILE_NOTFOUND:
            m_omstrConversionStatus = _T("Input File path is not found");
            break;
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
HRESULT CLogAscConverter::GetInputFileFilters(TCHAR* pchInputDefFilters, TCHAR* pchInputFilters)
{
    HRESULT hResult = S_FALSE;
    if(NULL != pchInputDefFilters)
    {
        _tcscpy(pchInputDefFilters, _T("log"));
        hResult = S_OK;
    }
    else
    {
        hResult = S_FALSE;
    }
    if(NULL != pchInputFilters)
    {
        _tcscpy(pchInputFilters, _T("BUSMASTER Log File(s) (*.log)|*.log||"));
        hResult = S_OK;
    }
    return hResult;
}
HRESULT CLogAscConverter::GetLastConversionStatus(HRESULT& hResult, CString& omstrStatus)
{
    hResult = m_hResult;
    omstrStatus = m_omstrConversionStatus;
    return S_OK;
}

HRESULT CLogAscConverter::GetOutputFileFilters(TCHAR* pchOutputDefFilters, TCHAR* pchOutputFilters)
{
    HRESULT hResult = S_FALSE;
    if(NULL != pchOutputDefFilters)
    {
        _tcscpy(pchOutputDefFilters, _T("asc"));
        hResult = S_OK;
    }
    else
    {
        hResult = S_FALSE;
    }
    if(NULL != pchOutputFilters)
    {
        _tcscpy(pchOutputFilters, _T("CANoe Log File(s) (*.asc)|*.asc||"));
        hResult = S_OK;
    }
    return hResult;
}
HRESULT CLogAscConverter::ConvertFile(TCHAR* chInputFile, TCHAR* chOutputFile)
{
    HRESULT hResult = S_OK;
    /*extern FILE*  yyin;
	extern FILE*  yyout;
    */
    FILE* fpInputFile = NULL;
    FILE* fpOutputFile = NULL;
    fpInputFile = _tfopen(chInputFile, _T("r"));
    if(NULL != fpInputFile)
    {
        fpOutputFile = _tfopen(chOutputFile, _T("w"));
        if(NULL != fpOutputFile)
        {
            //yydebug = 1;
            nConvertFile(fpInputFile, fpOutputFile);
            m_omstrConversionStatus = _T("Conversion Completed Successfully"); 
            fclose(fpInputFile);
            fclose(fpOutputFile);
        }
        else
        {
            m_omstrConversionStatus = _T("Output File path is not found");
            hResult = ERR_OUTPUT_FILE_NOTFOUND;
        }
    }
    else
    {
         m_omstrConversionStatus = _T("Input File path is not found");
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