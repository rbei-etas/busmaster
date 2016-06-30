#pragma once

#include "..\CommonClass\MsgContainerBase.h"

// CProgressBarDlg dialog
struct sLoadFile
{
    std::string m_strFileName;
    class CMsgContainerBase* m_pouMsgContainerIntrf;
    class CMsgFrmtWnd* m_oupMsgFrmtWnd;
    HRESULT hResult;
};

class CProgressBarDlg : public CDialog
{
    DECLARE_DYNAMIC(CProgressBarDlg)
private:
    CProgressCtrl m_omProgressCtrl;
    CStatic m_omProgressText;
    CStatic m_omErrorText;
    sLoadFile m_structLoadFile;
    UINT m_unProgressBarTimerId;
    CWinThread* m_pThread;
public:
    CProgressBarDlg(const sLoadFile& structLoadFile, CWnd* pParent = NULL);   // standard constructor
    virtual ~CProgressBarDlg();

    // Dialog Data
    enum { IDD = IDD_DLG_PROGRESS_BAR };

    HRESULT SetProgressBarRange(int nMin,int nMax);
    HRESULT SetProgressBarPos(int nPos);
    HRESULT SetProgressText(std::string strProgressText);


protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    afx_msg void OnCancel();
    afx_msg void OnOK();
    afx_msg LRESULT OnCloseProgressBar(WPARAM wParam, LPARAM lParam);
    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();

private:
    static UINT LoadLogFileThreadProc(LPVOID lParams);

public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};
