#pragma once

#include "CustomProtocol_stdafx.h"
#include "CustomProtocol_resource.h"
#include "XMLUtils.h"

// CUserSelectionDlg dialog
using namespace std;

class CUserSelectionDlg : public CDialog
{
	DECLARE_DYNAMIC(CUserSelectionDlg)

public:
	CUserSelectionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUserSelectionDlg();

// Dialog Data
	enum { IDD = IDD_DLG_USER_SELECTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int				m_nUserSelectedDerivedProtocol;

	BOOL SaveProtocolConfiguration();
	BOOL AddBaseProtocolToConfig(xmlNodePtr pBaseProtocol );
	BOOL AddDerivedProtocol(xmlNodePtr pDerivedProtocol);
	BOOL LoadProtocolConfiguration(string omstrFileName);
	BOOL nLoadBaseProtocol(xmlDocPtr xmlConfigFiledoc);
	BOOL LoadBaseProtocol(xmlNodePtr pNode);
	BOOL LoadDerivedProtocol(xmlNodePtr pNode);

	afx_msg void OnBnClickedRadioAddPrtocol();
	afx_msg void OnBnClickedRadioCreateFrame();
	afx_msg void OnBnClickedBtnLoadProtocol();
	afx_msg void OnBnClickedBtnSaveConfig();
};
