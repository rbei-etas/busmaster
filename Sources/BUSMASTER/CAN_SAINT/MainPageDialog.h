#pragma once
#include "resource.h"
#include <map>
#include "HardwareListing.h"
#include "afxwin.h"

class MainPageDialog : public CResizableDialog
{

public:
	MainPageDialog(CHardwareListing* h,CWnd* pParent = NULL);   // standard constructor
	virtual ~MainPageDialog();

// Dialog Data
	enum { IDD = IDD_MAIN_DIALOG };
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void addStringToList(CString string);
	virtual void OnCancel();
	CListBox * getMessageListBox();
	CButton* getCheck1();
	CButton* getCheck2();
	CString edittextBuffer;
	CString curEntry;
	void updateCAN1();
	void updateCAN2();
	void  getMessage(CString str);


	DECLARE_MESSAGE_MAP()
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnCbnSelchangeCombo4();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedOk3();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnCbnSelchangeCombo5();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCbnEditupdateCombo5();
	afx_msg void OnBnClickedCheckCan1();
	afx_msg void OnBnClickedCheckCan2();
	afx_msg void OnCbnSelchangeComboCan1Type();
	afx_msg void OnCbnSelchangeComboCan1Speed();
	afx_msg void OnCbnSelchangeComboCan2Speed();
	afx_msg void OnCbnSelchangeComboCan2Type();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonLoadMessageFile();
	afx_msg void OnCbnSelchangeComboMessage();
	afx_msg void OnCbnEditupdateComboMessage();
	CListBox messageList;
	CComboBox combo5;
};
