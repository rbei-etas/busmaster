#pragma once
#include "resource.h"

// CCAPL2CPropertyPage dialog

class CCAPL2CPropertyPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CCAPL2CPropertyPage)
    
public:
    BOOL	m_check;
    BOOL	m_savedb;
    void SaveSettings();
	CCAPL2CPropertyPage();
	virtual ~CCAPL2CPropertyPage();

// Dialog Data
	enum { IDD = IDD_DLG_LANG_CNVRTR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CConverterDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnBrowseSource();
    afx_msg void OnBrowseDest();
    afx_msg void OnShowLog();
    afx_msg void OnConvert();
    afx_msg void OnCancel();
    afx_msg void OnChangeEditInput();
    afx_msg void OnChkbOptn();
    afx_msg void OnBrowseCANoeDb();
    afx_msg void OnBrowseBUSMASTERDb();
    afx_msg void OnChkbSavedb();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};
