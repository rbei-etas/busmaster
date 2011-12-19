#pragma once
#include "resource.h"
#include "PluginManagerBase.h"
#include "afxwin.h"
// CDefConverterPage dialog

class CDefConverterPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CDefConverterPage)
    CPluginManagerBase* m_pouPluginManager;
public:
    CDefConverterPage();
	HRESULT SetPluginManager(CPluginManagerBase *m_pouPluginManager);
    

    virtual ~CDefConverterPage();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_DEF_CONVERTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
    CComboBox m_omComboConverterNames;
    HRESULT LoadConverters();
    INT GetConverterPos();
public:
    virtual BOOL OnInitDialog();
    CEdit m_omEditInputPath;
    CEdit m_omEditOutputPath;
    afx_msg void OnBnClickedBtnInput();
    afx_msg void OnBnClickedBtnOutput();
    afx_msg void OnBnClickedBtnConvert();
    CString m_omStrInputFilePath;
    CString m_omStrOutputFilePath;
    afx_msg void OnCbnSelchangeComboConversions();
    CString m_omstrConversionComment;
    CString m_omstrEditHelp;
};
