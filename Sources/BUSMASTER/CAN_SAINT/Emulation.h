#pragma once

#include "resource.h"
#include <map>
#include "afxwin.h"
#include "HardwareListing.h"
#include "afxcmn.h"
#include "CAN_SAINT.h"

// Emulation dialog
class Emulation : public CResizableDialog
{

public:
	Emulation(CHardwareListing* h,CWnd* pParent = NULL);   // standard constructor
	static std::vector<GroupTransmit> groupEmulations;
	std::vector<Message> emulations;
	int amtOfIncludes;
	int transmit;

	virtual ~Emulation();

// Dialog Data
	enum { IDD = IDD_EMULATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnCancel();
	void handleGroupEmulation(HWND hWnd, int index);
	int getTot();
	void fixProperSizing();
	void saveProperSizing(int,int);
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl * getListBox();
	void handleEmulations(CString str);
	void stopEmulation();
	void startEmulation();
	CButton checkEnabled;
	CListCtrl listControl;
	CEdit editReceive;
	CEdit reditTransmit;
	CEdit editTransmit;
	CComboBox comboOptions;
	CEdit editComments;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCbnSelchangeCombo5();
	CButton buttonEnable;
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonSaveFile();
	afx_msg void OnBnClickedButtonAddNewLine();
	afx_msg void OnBnClickedButtonRemoveLine();
	afx_msg void OnBnClickedButtonSaveLine();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnCbnSelchangeComboType();
	CComboBox comboEnabled;
	afx_msg void OnHdnEndtrackList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnDividerdblclickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
