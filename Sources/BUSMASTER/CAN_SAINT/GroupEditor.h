#pragma once

#include "resource.h"
#include <map>
#include "afxwin.h"
#include "HardwareListing.h"
#include "afxcmn.h"

// GroupEditor dialog

class GroupEditor : public CResizableDialog
{

public:
	GroupEditor(CHardwareListing* h,CWnd* pParent = NULL);   // standard constructor
	virtual ~GroupEditor();

// Dialog Data
	enum { IDD = IDD_GROUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:

	void fixSize();
	void saveSize(int,int);
	void restoreSize();
	int getTotal();
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CListCtrl * getListBox();
	CButton checkEnabled;
	CEdit editMessage;
	CEdit editDelay;
	CEdit editComment;
	CListCtrl listControl;
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonSaveFile();
	afx_msg void OnBnClickedButtonAddLine();
	afx_msg void OnBnClickedButtonRemoveLine();
	afx_msg void OnBnClickedButtonSaveLine();
	afx_msg void OnBnClickedButtonSend();
	CComboBox comboEnabled;
	afx_msg void OnHdnEndtrackList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnDividerdblclickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
