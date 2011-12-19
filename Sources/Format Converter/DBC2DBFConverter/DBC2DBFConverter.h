#pragma once
#include "../FormatConverterApp/BaseConverter.h"

class CDBC2DBFConverter : public CBaseConverter
{
    CString m_omstrConversionStatus;
    HRESULT m_hResult;
public:
    CDBC2DBFConverter(void);
    virtual HRESULT GetInputFileFilters(TCHAR*, TCHAR* );
    virtual HRESULT GetOutputFileFilters(TCHAR*, TCHAR* );
    virtual HRESULT ConvertFile(TCHAR* chInputFile, TCHAR* chOutputFile);
    virtual HRESULT GetConverterName(TCHAR* strConverterName); 
    virtual HRESULT GetErrorStatus(HRESULT hResult, CString& omstrStatus);
    virtual HRESULT GetLastConversionStatus(HRESULT& hResult, CString& omstrStatus);
    virtual HRESULT GetHelpText(TCHAR* pchHelpText);
    virtual BOOL bHaveOwnWindow();
    virtual HRESULT GetPropertyPage(CPropertyPage*& pPage){return S_FALSE;};
    ~CDBC2DBFConverter(void);
};