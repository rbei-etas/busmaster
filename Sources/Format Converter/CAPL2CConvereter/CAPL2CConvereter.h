#pragma once
#include "../FormatConverterApp/BaseConverter.h"

class CCAPL2CConvereter : public CBaseConverter
{
    CString m_omstrConversionStatus;
    HRESULT m_hResult;
public:
    CCAPL2CConvereter(void);
    virtual HRESULT GetInputFileFilters(TCHAR*, TCHAR* );
    virtual HRESULT GetOutputFileFilters(TCHAR*, TCHAR* );
    virtual HRESULT ConvertFile(TCHAR* chInputFile, TCHAR* chOutputFile);
    virtual HRESULT GetConverterName(string& strConverterName); 
    virtual HRESULT GetErrorStatus(HRESULT hResult, CString& omstrStatus);
    virtual HRESULT GetLastConversionStatus(HRESULT& hResult, CString& omstrStatus);
    virtual HRESULT GetHelpText(TCHAR* pchHelpText);
    virtual BOOL bHaveOwnWindow();
    virtual HRESULT GetPropertyPage(CPropertyPage*& pPage);
    ~CCAPL2CConvereter(void);
};