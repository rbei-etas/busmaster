#include "CAPL2CConverter.h"
#include "Definitions.h"
#include "Converter.h"
#include "CAPL2CPropertyPage.h"

CCAPL2CConverter::CCAPL2CConverter(void)
{
}

HRESULT CCAPL2CConverter::GetHelpText(string& pchHelpText)
{
    pchHelpText = "Converts the CANoe CAPL(.can) file to BUSMASTER Node Simulation(.c) file";
    return S_FALSE;
}

HRESULT CCAPL2CConverter::GetConverterName(string& strConverterName)
{
    strConverterName = "CAPL TO C Conversion";
    return S_FALSE;
}

HRESULT CCAPL2CConverter::GetErrorStatus(HRESULT hResult, string& omstrStatus)
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

    return S_FALSE;
}

HRESULT CCAPL2CConverter::GetInputFileFilters(string& pchInputDefFilters, string& pchInputFilters)
{
    return S_FALSE;
}

HRESULT CCAPL2CConverter::GetLastConversionStatus(HRESULT& hResult, string& omstrStatus)
{
    return S_FALSE;
}

HRESULT CCAPL2CConverter::GetOutputFileFilters(string& pchOutputDefFilters, string& pchOutputFilters)
{
    return S_FALSE;
}

HRESULT CCAPL2CConverter::ConvertFile(string& chInputFile, string& chOutputFile)
{
    return S_FALSE;
}

BOOL CCAPL2CConverter::bHaveOwnWindow()
{
    return TRUE;
}

CCAPL2CConverter::~CCAPL2CConverter(void)
{
};

HRESULT CCAPL2CConverter::GetPropertyPage(CPropertyPage*& pPage)
{
    pPage = new CCAPL2CPropertyPage();
    return S_FALSE;
};
