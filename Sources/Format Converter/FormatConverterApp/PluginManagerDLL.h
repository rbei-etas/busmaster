#pragma once

// CPluginManagerDLL command target

#include "PluginManagerBase.h"

#define defDLLFILEEXTENSION         _T("*.dll")
#define defDEFAULTPLUGINFOLDER      _T("\\ConverterPlugins");
#define defCONVERTERINTERFACE       _T("GetBaseConverter")

class CPluginManagerDLL:public CPluginManagerBase
{
    
public:
    CPluginManagerDLL();
    virtual HRESULT LoadConvertersFromFolder(CONST TCHAR* pchPluginFolder = NULL);
    virtual HRESULT LoadConverter(CString& strFileName);
    virtual HRESULT UnLoadAllPlugins();
    
    virtual ~CPluginManagerDLL();
};


