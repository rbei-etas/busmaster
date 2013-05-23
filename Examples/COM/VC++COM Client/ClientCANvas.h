#pragma once

#include "D:\BUSMASTER OSS\BUSMASTER SVN\For Release Ver1.7.2\Sources\Application\BUSMASTER_Interface.h"
#include "BMSink.h"


// CClientCANvas dialog

class CClientCANvas : public CDialog
{
	DECLARE_DYNAMIC(CClientCANvas)

public:
	CClientCANvas(CWnd* pParent = NULL);   // standard constructor
	virtual ~CClientCANvas();

	IConnectionPoint            *m_pCntPoint;
	IConnectionPointContainer   *m_pConnPtContainer;

	IUnknown *m_pSinkUnk;
	ClientSink *m_pSink;
	DWORD    m_dwAdvise;
	HANDLE m_hEvent;
	HANDLE m_hReadPIPE;
	BSTR  m_acEventName;
	BSTR  m_acPIPEName;
	CListBox m_lstReceivedData;
	CEdit m_edMsgReadCount;
	short m_shUniqueID;


// Dialog Data
	enum { IDD = IDD_DLG_CLIENT_CANVAS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    CString m_omHelpText;
    BOOL m_bConnected;
    void UpdateGUI(void);

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
protected:
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
    afx_msg void OnDestroy();
};
