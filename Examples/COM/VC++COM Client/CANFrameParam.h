#pragma once


// CCANFrameParam dialog

class CCANFrameParam : public CDialog
{
	DECLARE_DYNAMIC(CCANFrameParam)

public:
    CAN_MSGS m_sCFrame;
	CCANFrameParam(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCANFrameParam();

// Dialog Data
	enum { IDD = IDD_DLG_PARAM_CANFRAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
    BOOL m_bExtended;
public:
    BOOL m_bRTR;
protected:
    virtual void OnOK();
public:
    virtual BOOL OnInitDialog();
};
