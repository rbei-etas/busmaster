// AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AboutDlg.h"


// CAboutDlg dialog


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_CONTACT_LINK, &CAboutDlg::OnContactLinkClicked)
END_MESSAGE_MAP()

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BOOL CAboutDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    CString omVerStr("");
    omVerStr.Format(IDS_VERSION);
    GetDlgItem(IDC_STATIC_VERSION)->SetWindowText(omVerStr);
    return TRUE;
}

void CAboutDlg::OnContactLinkClicked(NMHDR *pNMHDR, LRESULT *pResult)
{
	PNMLINK pNMLink = (PNMLINK)pNMHDR;

	CString contactUrl = "http://www.etas.com/en/products/applications_busmaster.php";

	if (::ShellExecute(NULL, NULL, contactUrl, NULL, NULL, SW_SHOWNORMAL) <(HINSTANCE)32)
	{
		TRACE(_T("Can't open URL: %s\n"), contactUrl);
	}

	*pResult = 0;
}
