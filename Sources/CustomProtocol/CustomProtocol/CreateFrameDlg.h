#pragma once

#include "CustomProtocol_resource.h"
#include "LinFlexListCtrl.h"
#include "CustomTxDataStore.h"
// CCreateFrameDlg dialog

class CCreateFrameDlg : public CDialog
{
	DECLARE_DYNAMIC(CCreateFrameDlg)

public:
	CCreateFrameDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCreateFrameDlg();

// Dialog Data
	enum { IDD = IDD_DLG_CREATE_FRAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CLinFlexListCtrl			m_CreateFrameListc;
	int							m_SelectedDerivedProtocolIndex;
	
	virtual BOOL OnInitDialog();
	int nAddMessageToList( SBASEPROTOCOL sBaseProtocol, int nHeaderIndex );
	void AddHeaderValuesToBaseProtocol(SBASEPROTOCOL& sBaseProtocol, int nRow);
	void AddHeaderValues(SHEADER& sHeader, int nRow);
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnDeleteProtocol();
	afx_msg void OnCbnSelchangeComboAddBaseProtocol();
};
