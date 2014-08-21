#pragma once
#include "afxwin.h"
#include "Ethernet_Interface_Stdafx.h"
#include "Ethernet_Resource.h"

// CEConfigDlg dialog

class CEConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CEConfigDlg)

public:
	CEConfigDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEConfigDlg();
	CEConfigDlg(list<string>& strList,int nCount,CWnd* pParent = NULL);

// Dialog Data
	enum { IDD = IDD_DLG_ECONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int				m_nSel;
	string			m_strFilter;
	string			m_strDllPath;
	list<string>    m_strlistAdapters;
	CEdit			m_PortEdit;
	CEdit			m_DllPath;
	CComboBox		m_AdapterCombo;

	afx_msg void OnBnClickedBtnPath();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
