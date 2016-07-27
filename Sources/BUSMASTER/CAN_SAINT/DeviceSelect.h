#pragma once
#include "resource.h"
#include <map>
#include "afxwin.h"
#include "afxcmn.h"
// DeviceSelect dialog

class DeviceSelect : public CDialog
{

public:
	DeviceSelect(CString list,CWnd* pParent = NULL);   // standard constructor
	virtual ~DeviceSelect();

	CString getSelectedDevice();
// Dialog Data
	enum { IDD = IDD_DEVICE_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl listControl;
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
};
