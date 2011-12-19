#pragma once

#define ERR_INPUT_FILE_NOTFOUND          -1
#define ERR_OUTPUT_FILE_NOTFOUND         -2   
#include "../FormatConverterApp/BaseConverter.h"

class CAscLogConverter : public CBaseConverter
{
    CString m_omstrConversionStatus;
    HRESULT m_hResult;
public:
    CAscLogConverter(void);
    virtual HRESULT GetInputFileFilters(TCHAR*, TCHAR* );
    virtual HRESULT GetOutputFileFilters(TCHAR*, TCHAR* );
    virtual HRESULT ConvertFile(TCHAR* chInputFile, TCHAR* chOutputFile);
    virtual HRESULT GetConverterName(TCHAR* strConverterName);
    virtual HRESULT GetErrorStatus(HRESULT hResult, CString& omstrStatus);
    virtual HRESULT GetLastConversionStatus(HRESULT& hResult, CString& omstrStatus);
    virtual HRESULT GetHelpText(TCHAR* pchHelpText);
    virtual BOOL bHaveOwnWindow();
    virtual HRESULT GetPropertyPage(CPropertyPage*& pPage){return S_FALSE;};
    ~CAscLogConverter(void);
};
