#pragma once
#include "CustomProtocol_resource.h"
#include "LinFlexListCtrl.h"
#include "CustomTxDataStore.h"

// CProtocolAdderDlg dialog

class CProtocolAdderDlg : public CDialog
{
	DECLARE_DYNAMIC(CProtocolAdderDlg)

public:
	CProtocolAdderDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProtocolAdderDlg();

// Dialog Data
	enum { IDD = IDD_DLG_ADD_PROTOCOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CLinFlexListCtrl				m_omAddHeaderList;
	CImageList						m_omHeaderImageList;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnAddHeader();
	
	virtual BOOL OnInitDialog();
	int nAddMessageToList(int nRow);
	static void vCheckBoxClickHandler( CListCtrl* pThis, int nItem, int nSubItem, void* uParam);

};
