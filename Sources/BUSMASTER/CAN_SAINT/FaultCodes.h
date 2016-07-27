#pragma once
#include "resource.h"
#include <map>
#include "afxwin.h"
#include "HardwareListing.h"
#include "afxcmn.h"

class FaultCodes : public CResizableDialog
{

public:
	FaultCodes(CHardwareListing* h,CWnd* pParent = NULL);   // standard constructor
	virtual ~FaultCodes();
	void faultFailed();
	void fillBuffer();
	void addNewFault(CString);
	void finishedFault(CString mem);
	CListCtrl m_listControl;

// Dialog Data
	enum { IDD = IDD_FAULT_CODES };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnCancel();
	int getTotSize();
	void savethesize(int,int);
	void fixthesize();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonRequest();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonCopy();
	afx_msg void OnBnClickedButtonCopyAll();
	afx_msg void OnHdnEndtrackList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnDividerdblclickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
