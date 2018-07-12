#pragma once
#include "FormatConverter_Resource.h"
#include "PluginManagerBase.h"
#include "afxwin.h"
// CDefConverterPage dialog

class CDefConverterPage : public CPropertyPage
{
    DECLARE_DYNAMIC(CDefConverterPage)
    CPluginManagerBase* m_pouPluginManager;
    INT m_nTabPosition;
    HANDLE mConvThrdHandle = nullptr;
public:
    CDefConverterPage(INT nTabPosition);
    HRESULT SetPluginManager(CPluginManagerBase* m_pouPluginManager);


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
    void vAdjustWidthMessageComboBox();



public:
    virtual BOOL OnInitDialog();
    HRESULT ValidateFileExtensions(CString& m_omStrInputFilePath,
                                   CString& m_omStrOutputFilePath, CBaseConverter*& m_pouConverter);
    CEdit m_omEditInputPath;
    CEdit m_omEditOutputPath;
    afx_msg void OnBnClickedBtnInput();
    afx_msg void OnBnClickedBtnOutput();
    afx_msg void OnBnClickedBtnConvert();
    afx_msg void OnBnClickedViewLog();
    afx_msg LPARAM UpdateUI(WPARAM, LPARAM);
    CString m_omStrInputFilePath;
    CString m_omStrOutputFilePath;
    afx_msg void OnCbnSelchangeComboConversions();
    CString m_omstrConversionComment;
    CString m_omstrEditHelp;
    CString m_omstrLogFilePath;
    CBaseConverter* m_pouConverter;
};
