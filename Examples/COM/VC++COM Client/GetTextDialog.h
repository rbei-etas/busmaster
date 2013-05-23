#pragma once


// CGetTextDialog dialog

class CGetTextDialog : public CDialog
{
	DECLARE_DYNAMIC(CGetTextDialog)

public:
	CGetTextDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGetTextDialog();

// Dialog Data
	enum { IDD = IDD_DLG_TEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CString m_omText;
protected:
    virtual void OnOK();
};
