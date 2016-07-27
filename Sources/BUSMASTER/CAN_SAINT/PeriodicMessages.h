#pragma once
#include "resource.h"
#include <map>
#include "afxwin.h"
#include "HardwareListing.h"
#include "afxcmn.h"
#include "CAN_SAINT.h"

void CALLBACK transmitCallback( HWND hWnd,UINT nMsg,UINT_PTR nIDEvent,DWORD dwTime);
void CALLBACK groupCallback( HWND hWnd,UINT nMsg,UINT_PTR nIDEvent,DWORD dwTime);
bool checkForValidity(CString str);
void removeFromList(UINT_PTR val);
void handleGroupMessage(HWND hWnd,int index);

class PeriodicMessages : public CResizableDialog
{

public:
	PeriodicMessages(CHardwareListing* h,CWnd* pParent = NULL);   // standard constructor
	virtual ~PeriodicMessages();

// Dialog Data
	enum { IDD = IDD_PERIODIC };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:

	void saveSiz(int,int);
	void restoreSiz();
	int getSiz();
	void fixSiz();

	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnCancel();
	CListCtrl * getListBox();
	CListCtrl m_listControl;
	void stopPeriodicTransmission();
	void startPeriodicTransmission();

	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	CEdit editMessage;
	CEdit editPeriod;
	CEdit editComments;
	CButton checkMessageEnabled;
	CButton checkCycleEnabled;
	CButton checkEnabled;
	afx_msg void OnBnClickedButton10();
	CButton buttonEnable;
	CEdit editCycles;
	afx_msg void OnEnChangeEdit16();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCbnSelchangeCombo5();
	CComboBox comboOptions;
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonSaveFile();
	afx_msg void OnBnClickedButtonAddLine();
	afx_msg void OnBnClickedButtonRemove();
	afx_msg void OnBnClickedButtonSaveLine();
	afx_msg void OnBnClickedButtonStart();
	CComboBox comboEnabled;
	afx_msg void OnHdnDividerdblclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnEndtrackList1(NMHDR *pNMHDR, LRESULT *pResult);
};
