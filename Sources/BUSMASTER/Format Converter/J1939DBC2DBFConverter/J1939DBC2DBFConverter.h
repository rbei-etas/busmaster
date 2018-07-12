#pragma once
#include "../FormatConverterApp/BaseConverter.h"
#include "../DBC2DBFConverterLibrary/DBCConverterBase.h"
class CJ1939DBC2DBFConverter : public CBaseConverter
{
    CString m_omstrConversionStatus;
    HRESULT m_hResult;
    HMODULE m_hDLLModule;
    CDBCConverterBase* m_pouDBC2DBFConverter;
    GETCONVERTER m_pfGetConverter;
public:
    CJ1939DBC2DBFConverter(void);
    virtual HRESULT GetInputFileFilters(string& pchInputDefFilters, string& pchInputFilters);
    virtual HRESULT GetOutputFileFilters(string& pchOutputDefFilters, string& pchOutputFilters);
    virtual HRESULT ConvertFile(string& chInputFile, string& chOutputFile);
    virtual HRESULT GetConverterName(string& strConverterName);
    virtual HRESULT GetErrorStatus(HRESULT hResult, string& omstrStatus);
    virtual HRESULT GetLastConversionStatus(HRESULT& hResult, string& omstrStatus);
    virtual HRESULT GetHelpText(CString& pchHelpText);
    virtual BOOL bHaveOwnWindow();
    virtual HRESULT GettextBusmaster();
    virtual HRESULT GetPropertyPage(CPropertyPage*& pPage)
    {
        return S_FALSE;
    };
    ~CJ1939DBC2DBFConverter(void);

private:
    CJ1939DBC2DBFConverter(const CJ1939DBC2DBFConverter&)
    {
    }
    CJ1939DBC2DBFConverter& operator=(const CJ1939DBC2DBFConverter&)
    {
        return *this;
    }
};