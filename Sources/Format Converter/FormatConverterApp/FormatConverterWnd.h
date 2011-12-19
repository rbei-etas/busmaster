#pragma once
#include "PluginManagerDll.h"
#include "DefConverterPage.h"
// CFormatConverterWnd

class CFormatConverterWnd : public CPropertySheet
{
	DECLARE_DYNAMIC(CFormatConverterWnd)
    CPluginManagerBase *m_pouPluginManager;
    CDefConverterPage *m_pomDefConverterPage;
public:
	CFormatConverterWnd(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CFormatConverterWnd(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CFormatConverterWnd();
    HRESULT LoadConverters();
protected:
	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    virtual BOOL Create(CWnd* pParentWnd = NULL, DWORD dwStyle = (DWORD)-1, DWORD dwExStyle = 0);
protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


