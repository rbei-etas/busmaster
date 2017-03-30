// AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AboutDlg.h"
#include "MainFrm.h"
#include "BUSMASTER.h"

#define defSTR_NAME				"Name"
#define defSTR_VERSION			"Version"
#define defSTR_TYPE			    "Type"
#define defSTR_LICENSE_VALIDITY  "Validity"
#define defSTR_LICENSE_VALID_TILL "Valid Till"
#define defSTR_OSS_COPYRIGHT    "Copyright 2011 ETAS GmbH and Robert Bosch Engineering and Business Solutions Limited"
#define defSTR_OSS_LICENSE      "This program is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version."
#define defSTR_COPYRIGHT         "Copyright 2017 ETAS GmbH and Robert Bosch Engineering and Business Solutions Private Limited. All Rights Reserved."
#define defSTR_COPYRIGHT_INST    "Copyright 2017 Robert Bosch Engineering and Business Solutions Private Limited. All Rights Reserved."
#define defSTR_LICENSE           "Your rights with respect to %s Add-On are subject to your acceptance of the End-User License Agreement (EULA) provided by the licensor and payment of requisite fee."
#define defSTR_REGISTRY          "Software\\RBEI-ETAS\\BUSMASTER_v%d.%d.%d\\Add-Ons"
#define defSTR_BUSMASTER         "BUSMASTER"
#define defSTR_OPEN_SOURCE       "Open Source"
#define defSTR_ADDON             "Add-On"
#define defSTR_FLEXRAY           "FlexRay"
#define defSTR_INSTRUMENTS       "Instruments"

// CAboutDlg dialog


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_CONTACT_LINK, &CAboutDlg::OnContactLinkClicked)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_COMPONENTS, &CAboutDlg::OnLvnItemchangedListComponents)
END_MESSAGE_MAP()

extern CCANMonitorApp theApp;
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_COMPONENTS, m_lstComponents);
}

BOOL CAboutDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    CString omVerStr("");
	m_lstComponents.EnableWindow(TRUE);
	char caColumnName[][50] = { defSTR_NAME,
		defSTR_TYPE,
		defSTR_LICENSE_VALIDITY,
		defSTR_LICENSE_VALID_TILL
	};

	INT nColumnFormat[] = { LVCFMT_LEFT,
		LVCFMT_LEFT,
		LVCFMT_LEFT,
		LVCFMT_LEFT
	};

	INT nTotalColunmSize = 0;
	RECT rListCtrlRect;

	m_lstComponents.GetWindowRect(&rListCtrlRect);
	nTotalColunmSize = rListCtrlRect.right - rListCtrlRect.left;

	m_lstComponents.InsertColumn(0, caColumnName[0], nColumnFormat[0], (int)(0.25 * nTotalColunmSize));
	m_lstComponents.InsertColumn(1, caColumnName[1], nColumnFormat[1], (int)(0.25 * nTotalColunmSize));
	m_lstComponents.InsertColumn(2, caColumnName[2], nColumnFormat[2], (int)(0.2 * nTotalColunmSize));
	m_lstComponents.InsertColumn(3, caColumnName[3], nColumnFormat[3], (int)(0.3 * nTotalColunmSize));

	// Set the extended style

	TCHAR    achKey[255];   // buffer for subkey name
	DWORD    cbName;                   // size of name string 	
	DWORD    cSubKeys = 0;               // number of subkeys 
	HKEY sKey;
	LONG lError = 0;

	CString strCompleteSubKey;
	strCompleteSubKey.Format(defSTR_REGISTRY, VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);

	CString strVersion;
	strVersion.Format("v%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);

	lError = RegOpenKeyEx(HKEY_CURRENT_USER, strCompleteSubKey, 0, KEY_READ, &sKey);

	if (ERROR_SUCCESS == lError)
	{
		lError = RegQueryInfoKey(sKey, NULL, NULL, NULL, &cSubKeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	}

	int nItemIndex = m_lstComponents.InsertItem(m_lstComponents.GetItemCount(), achKey);
	m_lstComponents.SetItemText(nItemIndex, 0, defSTR_BUSMASTER);
	m_lstComponents.SetItemText(nItemIndex, 1, defSTR_OPEN_SOURCE);
	m_lstComponents.SetItemText(nItemIndex, 2, "-");
	m_lstComponents.SetItemText(nItemIndex, 3, "-");

	CMainFrame* pMainFrame = static_cast<CMainFrame*> (theApp.m_pMainWnd);	
	
	// If the registry key open successfully, get the value in "path"
	// sub key
	int nIndex = 0;
	if (lError == ERROR_SUCCESS)
	{
		CString omAddOnName = "";
		while (nIndex < cSubKeys)
		{
			cbName = 255;
			lError = RegEnumKeyEx(sKey, nIndex,
				achKey,
				&cbName,
				NULL,
				NULL,
				NULL,
				NULL);
			nIndex++;

			if (lError == ERROR_SUCCESS)
			{
				int nCount = m_lstComponents.GetItemCount();
				int nItemIndex = m_lstComponents.InsertItem(m_lstComponents.GetItemCount(), achKey);
				m_lstComponents.SetItemText(nItemIndex, 0, achKey);
				m_lstComponents.SetItemText(nItemIndex, 1, defSTR_ADDON);
				int nDays = 0, nValidLicense = 0;
				std::string strDateTime = "";
				char result[MAX_PATH] = { 0 }, month[100] = { 0 };
				if (nullptr != pMainFrame)
				{
					CLicenseDetails licenseDetails;
					pMainFrame->GetLicenseDetails(achKey, licenseDetails);
					nDays = licenseDetails.nLicenseValidity;
					strDateTime = licenseDetails.strDateTime;
					nValidLicense = licenseDetails.nValidLicense;
				}

				CString strValidity = "0";

				if (nDays > 0)
				{
					strValidity.Format("%d days", nDays);
				}
				
				if (1 == nValidLicense)
				{
					m_lstComponents.SetItemText(nItemIndex, 2, strValidity);
					m_lstComponents.SetItemText(nItemIndex, 3, strDateTime.c_str());
				}
				else
				{
					m_lstComponents.SetItemText(nItemIndex, 2, "0");
					m_lstComponents.SetItemText(nItemIndex, 3, "Invalid");
				}
				
			}
		}
	}

	omVerStr.Format(IDS_VERSION);

	m_lstComponents.SetExtendedStyle(m_lstComponents.GetExtendedStyle()
	| LVS_EX_FULLROWSELECT);

	m_lstComponents.SetItemState(0, LVIS_SELECTED , LVIS_SELECTED);
	m_lstComponents.SetSelectionMark(0);

	GetDlgItem(IDC_STATIC_COPYRIGHT)->SetWindowText(defSTR_OSS_COPYRIGHT);
	GetDlgItem(IDC_STATIC_LICENSE_INFO)->SetWindowText(defSTR_OSS_LICENSE);
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
void CAboutDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


void CAboutDlg::OnLvnItemchangedListComponents(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	if (pNMLV->uNewState & LVIS_SELECTED)
	{
		UINT nSelectedItem = pNMLV->iItem;

		if (-1 != nSelectedItem)
		{
			CString strComponent = "";
			strComponent = m_lstComponents.GetItemText(nSelectedItem, 0);

			if (defSTR_BUSMASTER == strComponent)
			{
				GetDlgItem(IDC_STATIC_COPYRIGHT)->SetWindowText(defSTR_OSS_COPYRIGHT);
				GetDlgItem(IDC_STATIC_LICENSE_INFO)->SetWindowText(defSTR_OSS_LICENSE);
			}
			else if (defSTR_FLEXRAY == strComponent)
			{
				CString strLicense = "";
				strLicense.Format(defSTR_LICENSE, strComponent);
				GetDlgItem(IDC_STATIC_COPYRIGHT)->SetWindowText(defSTR_COPYRIGHT);
				GetDlgItem(IDC_STATIC_LICENSE_INFO)->SetWindowText(strLicense);
			}
			else if (defSTR_INSTRUMENTS == strComponent)
			{
				CString strLicense = "";
				strLicense.Format(defSTR_LICENSE, strComponent);
				GetDlgItem(IDC_STATIC_COPYRIGHT)->SetWindowText(defSTR_COPYRIGHT_INST);
				GetDlgItem(IDC_STATIC_LICENSE_INFO)->SetWindowText(strLicense);
			}
		}		
	}

	*pResult = 0;
}
