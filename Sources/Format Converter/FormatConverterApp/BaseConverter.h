#pragma once
#include "stdafx.h"

class CBaseConverter
{
public:
    virtual HRESULT GetInputFileFilters(TCHAR*, TCHAR* ) = 0;
    virtual HRESULT GetOutputFileFilters(TCHAR*, TCHAR* ) = 0;
    virtual HRESULT ConvertFile(TCHAR* chInputFile, TCHAR* chOutputFile) = 0;
    virtual HRESULT GetConverterName(string& strConverterName) = 0; 
    virtual HRESULT GetErrorStatus(HRESULT hResult, CString& omstrStatus) = 0;
    virtual HRESULT GetLastConversionStatus(HRESULT& hResult, CString& omstrStatus) = 0;
    virtual HRESULT GetHelpText(TCHAR* pchHelpText)=0;
    virtual BOOL bHaveOwnWindow() = 0;
    virtual HRESULT GetPropertyPage(CPropertyPage*& pPage ) = 0;
    virtual ~CBaseConverter(){};
};