// CAPL2CPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "CAPL2CConverter.h"
#include "CAPL2CPropertyPage.h"
#include "List.h"
#include "Functions.hpp"
#include "ConstString.h"
#include "stdlib.h"

// CCAPL2CPropertyPage dialog

IMPLEMENT_DYNAMIC(CCAPL2CPropertyPage, CPropertyPage)

CCAPL2CPropertyPage::CCAPL2CPropertyPage()
	: CPropertyPage(CCAPL2CPropertyPage::IDD)
{

}

CCAPL2CPropertyPage::~CCAPL2CPropertyPage()
{
}

void CCAPL2CPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCAPL2CPropertyPage, CPropertyPage)
END_MESSAGE_MAP()


// CCAPL2CPropertyPage message handlers
void CCAPL2CPropertyPage::SaveSettings()
{
    //this function will save the settings in the registry.
    CString strSection       = "Files";
    CString strStringItem    = "CAPL File";

    CWinApp* pApp = AfxGetApp();
    CString omStrCAPLName;
    CWnd * pCAPL = GetDlgItem(IDC_EDIT_INPUT);
    pCAPL->GetWindowText(omStrCAPLName);
    pApp->WriteProfileString(strSection, strStringItem,omStrCAPLName);
}
