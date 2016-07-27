#pragma once

#include "resource.h"
#include <map>
#include "afxwin.h"
#include "HardwareListing.h"
#include "afxcmn.h"
// CCANConfig dialog

class CCANConfig : public CDialog
{

public:
	CCANConfig(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCANConfig();

// Dialog Data
	enum { IDD = IDD_CAN_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CButton checkCAN1;
	CButton checkCAN2;
	afx_msg void OnBnClickedOk();
};
