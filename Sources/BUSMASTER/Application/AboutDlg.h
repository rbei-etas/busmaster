#pragma once

#include "resource.h"
// CAboutDlg dialog
class CAboutDlg : public CDialog
{
    DECLARE_MESSAGE_MAP()

public:
    CAboutDlg();

    enum { IDD = IDD_ABOUTBOX };

protected:
    /** DDX/DDV support */
    virtual void DoDataExchange(CDataExchange* pDX);
	void OnContactLinkClicked(NMHDR *pNMHDR, LRESULT *pResult);
protected:
    virtual BOOL OnInitDialog();
};