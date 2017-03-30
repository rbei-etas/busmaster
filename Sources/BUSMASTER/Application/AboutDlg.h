#pragma once

#include "resource.h"
#include "Utility/FFListCtrl.h"
// CAboutDlg dialog
class CAboutDlg : public CDialog
{
    DECLARE_MESSAGE_MAP()

public:
    CAboutDlg();

    enum { IDD = IDD_ABOUTBOX };

	CFFListCtrl m_lstComponents;
protected:
    /** DDX/DDV support */
    virtual void DoDataExchange(CDataExchange* pDX);
	void OnContactLinkClicked(NMHDR *pNMHDR, LRESULT *pResult);
protected:
    virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnLvnItemchangedListComponents(NMHDR *pNMHDR, LRESULT *pResult);
};