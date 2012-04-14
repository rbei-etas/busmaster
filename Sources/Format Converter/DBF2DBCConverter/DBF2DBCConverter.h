#pragma once

#include "../FormatConverterApp/BaseConverter.h"

class CDBF2DBCConverter : public CBaseConverter
{
    string m_omstrConversionStatus;
    HRESULT m_hResult;
public:
    CDBF2DBCConverter(void);
    virtual HRESULT GetInputFileFilters(string&, string& );
    virtual HRESULT GetOutputFileFilters(string&, string& );
    virtual HRESULT ConvertFile(string& chInputFile, string& chOutputFile);
    virtual HRESULT GetConverterName(string& strConverterName); 
    virtual HRESULT GetErrorStatus(HRESULT hResult, string& omstrStatus);
    virtual HRESULT GetLastConversionStatus(HRESULT& hResult, string& omstrStatus);
    virtual HRESULT GetHelpText(string& pchHelpText);
    virtual BOOL bHaveOwnWindow();
    virtual HRESULT GetPropertyPage(CPropertyPage*& pPage ){return S_FALSE;};
    ~CDBF2DBCConverter(void);
};