#pragma once

#include "BaseConverter.h"
#include "afxtempl.h"
typedef struct tagConverterInfo
{
    CBaseConverter *m_pouConverter;
    HMODULE m_hModule;
    CString m_strFileName;
    CPropertyPage* m_pomPage;
    tagConverterInfo()
    {
        m_pomPage = NULL;
        m_pouConverter = NULL;
        m_hModule = NULL;
    }
}ConverterInfo;

typedef CList<ConverterInfo, ConverterInfo& > ConvertersInfoList;

class CPluginManagerBase
{
//Member variables
public:
    ConvertersInfoList m_ConverterList;
//Member functions
public:
    CPluginManagerBase(void);
    virtual ~CPluginManagerBase(void);
    virtual HRESULT LoadConvertersFromFolder(CONST TCHAR* pchPluginFolder = NULL)=0;
    virtual HRESULT LoadConverter(CString& strFileName)=0;
    virtual HRESULT UnLoadAllPlugins()=0;
protected:
};
