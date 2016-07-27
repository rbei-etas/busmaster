#pragma once

#include "resource.h"
#include <map>
#include "afxwin.h"
#include "HardwareListing.h"
#include "afxcmn.h"

// Filter dialog
class Filter : public CResizableDialog
{
public:
	Filter(CHardwareListing* h,CWnd* pParent = NULL);   // standard constructor
	virtual ~Filter();

// Dialog Data
	enum { IDD = IDD_FILTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	void stopFiltering();
	void startFiltering();
	void savePropersize(int,int);
	void restorePropersize();
	int getTotalSize();
	virtual void OnCancel();
	CString listOfIncludes[5000];
	CString listOfExcludes[5000];
	int amtOfIncludes;
	int amtOfExcludes;
	void fixSizing();



	int transmitting;
	CListCtrl * getListBox();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedSave();
	CButton checkEnabledMessage;
	CEdit editMessageBox;
	CEdit editCommentBox;
	afx_msg void OnBnClickedAddLine();
	afx_msg void OnBnClickedRemoveLine();
	afx_msg void OnBnClickedSaveLine();
	afx_msg void OnBnClickedStartFiltering();
	CListCtrl listControl;
	CButton buttonFiltering;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox comboOptions;
	CComboBox comboEnabled;
	afx_msg void OnHdnEndtrackList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnDividerdblclickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
