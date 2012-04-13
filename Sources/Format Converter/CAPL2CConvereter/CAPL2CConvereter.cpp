#include "StdAfx.h"
#include "CAPL2CConvereter.h"
#include "Definitions.h"
#include "Converter.h"
#include "CAPL2CPropertyPage.h"

CCAPL2CConvereter::CCAPL2CConvereter(void)
{
}

HRESULT CCAPL2CConvereter::GetHelpText(string& pchHelpText)
{
    /*if(pchHelpText != NULL)
    {
        _tcscpy(pchHelpText, _T("Converts the CANoe CAPL(.can) file to BUSMASTER Node Simulation(.c) file"));
        return S_OK;
    }*/
    return S_FALSE;
}

HRESULT CCAPL2CConvereter::GetConverterName(string& strConverterName)
{
    /*if(strConverterName != NULL)
    {
        strcpy(strConverterName, _T("CAPL TO C Conversion"));
    }*/
    return S_FALSE;
}

HRESULT CCAPL2CConvereter::GetErrorStatus(HRESULT hResult, string& omstrStatus)
{
    /*switch( hResult )
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
    }*/
    return S_FALSE;
}

HRESULT CCAPL2CConvereter::GetInputFileFilters(string& pchInputDefFilters, string& pchInputFilters)
{
    /*HRESULT hResult = S_FALSE;
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
    }*/
    return S_FALSE;
}

HRESULT CCAPL2CConvereter::GetLastConversionStatus(HRESULT& hResult, string& omstrStatus)
{
    /*hResult = m_hResult;
    omstrStatus = m_omstrConversionStatus;*/
    return S_FALSE;
}

HRESULT CCAPL2CConvereter::GetOutputFileFilters(string& pchOutputDefFilters, string& pchOutputFilters)
{
    /*HRESULT hResult = S_FALSE;
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
    }*/
    return S_FALSE;
}

HRESULT CCAPL2CConvereter::ConvertFile(string& chInputFile, string& chOutputFile)
{
    return S_FALSE;
}

BOOL CCAPL2CConvereter::bHaveOwnWindow()
{
    return TRUE;
}

CCAPL2CConvereter::~CCAPL2CConvereter(void)
{
};

HRESULT CCAPL2CConvereter::GetPropertyPage(CPropertyPage*& pPage)
{
    pPage = new CCAPL2CPropertyPage();
    return S_FALSE;
};