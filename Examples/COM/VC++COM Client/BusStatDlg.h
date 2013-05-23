#pragma once


// CBusStatDlg dialog

class CBusStatDlg : public CDialog
{
	DECLARE_DYNAMIC(CBusStatDlg)

	CBusStatDlg(CWnd* pParent = NULL);   // standard constructor

public:
	CBusStatDlg(sBUSSTATISTICS_USR* psBusStat, CWnd* pParent = NULL);
	virtual ~CBusStatDlg();

// Dialog Data
	enum { IDD = IDD_DLG_BUSSTAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
    CString m_omDetails;
    sBUSSTATISTICS_USR* m_psBusStat;
public:
    virtual BOOL OnInitDialog();
};
