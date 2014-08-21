// EConfigDlg.cpp : implementation file
//
#pragma once
#include "EConfigDlg.h"


// CEConfigDlg dialog

IMPLEMENT_DYNAMIC(CEConfigDlg, CDialog)

CEConfigDlg::CEConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEConfigDlg::IDD, pParent)
{

}

CEConfigDlg::~CEConfigDlg()
{
}

CEConfigDlg::CEConfigDlg(list<string>& strList, int nCount,CWnd* pParent /*=NULL*/): CDialog(CEConfigDlg::IDD, pParent)
{

	for(list<string>::iterator nItr = strList.begin();nItr != strList.end(); nItr++)
	{
		m_strlistAdapters.push_back(*nItr);
	}
}

void CEConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PROTOCOL_PATH, m_DllPath);
	DDX_Control(pDX, IDC_COMB_ADAPTER, m_AdapterCombo);
	DDX_Control(pDX, IDC_EDIT_PORT, m_PortEdit);
}


BEGIN_MESSAGE_MAP(CEConfigDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_PATH, OnBnClickedBtnPath)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CEConfigDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	for(list<string>::iterator nItr = m_strlistAdapters.begin();nItr != m_strlistAdapters.end(); nItr++)
	{
		m_AdapterCombo.AddString(nItr->c_str());
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEConfigDlg::OnBnClickedBtnPath()
{
	CFileDialog oFileDlg(true);
	oFileDlg.m_ofn.lpstrFilter = "Dll files (*.dll)";
	oFileDlg.m_ofn.lpstrTitle = "Select Protocol dll file";
	oFileDlg.DoModal();
	string    strfFileName = oFileDlg.GetPathName();
	m_DllPath.SetWindowTextA(strfFileName.c_str());
}


void CEConfigDlg::OnBnClickedOk()
{
	char chTemp[1000];
	//Set the selected adapter index
	m_nSel = m_AdapterCombo.GetCurSel();

	//Set the port address
	m_PortEdit.GetWindowTextA(chTemp, 100);
	m_strFilter = chTemp;

	//Set the Custom .dll path
	m_DllPath.GetWindowTextA(chTemp, 1000);
	m_strDllPath = chTemp;

	CDialog::OnOK();
}
