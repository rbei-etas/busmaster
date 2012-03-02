/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  J1939TimeOutCfg.h
  Description   :  
  $Log:   X:/Archive/Sources/Application/J1939TimeOutCfg.h_v  $
 * 
 *    Rev 1.1   15 Apr 2011 20:00:40   CANMNTTM
 * Added RBEI Copyright information.
 * 
 *    Rev 1.0   22 Dec 2010 19:06:28   CANMNTTM
 *  

  Author(s)     :  Pradeep Kadoor
  Date Created  :  22/12/2010
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/
// CJ1939TimeOutCfg dialog
#include "Utility/RadixEdit.h"
class CJ1939TimeOutCfg : public CDialog
{
	DECLARE_DYNAMIC(CJ1939TimeOutCfg)

public:
	CJ1939TimeOutCfg(CWnd* pParent = NULL);   // standard constructor
    virtual BOOL OnInitDialog();
	virtual ~CJ1939TimeOutCfg();

// Dialog Data
	enum { IDD = IDD_TIMEOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CRadixEdit m_omBroadcast;
    CRadixEdit m_omResponse;
    CRadixEdit m_omHolding;
    CRadixEdit m_omT1;
    CRadixEdit m_omT2;
    CRadixEdit m_omT3;
    CRadixEdit m_omT4;
    afx_msg void OnBnClickedCheckBroadcast();
    afx_msg void OnBnClickedCheckResponse();
    afx_msg void OnBnClickedCheckHolding();
    afx_msg void OnBnClickedCheckT1();
    afx_msg void OnBnClickedCheckT2();
    afx_msg void OnBnClickedCheckT3();
    afx_msg void OnBnClickedCheckT4();
    afx_msg void OnEnChangeEditBroadcast();
    afx_msg void OnEnChangeEditResponse();
    afx_msg void OnEnChangeEditHolding();
    afx_msg void OnEnChangeEditT1();
    afx_msg void OnEnChangeEditT2();
    afx_msg void OnEnChangeEditT3();
    afx_msg void OnEnChangeEditT4();
    afx_msg void OnBnClickedOk();
};
