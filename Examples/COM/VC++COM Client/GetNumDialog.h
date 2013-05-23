#pragma once


// GetNumDialog dialog

class GetNumDialog : public CDialog
{
	DECLARE_DYNAMIC(GetNumDialog)

public:
	GetNumDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~GetNumDialog();

// Dialog Data
	enum { IDD = IDD_DLG_NUMBER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    DWORD m_dwNumber;
protected:
    virtual void OnOK();
};
