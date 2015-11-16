#pragma once


// CBusEventHandlerDlg dialog
#include "NodeSimEx_resource.h"
class CBusEventHandlerDlg : public CDialog
{
    DECLARE_DYNAMIC(CBusEventHandlerDlg)

private:
    CFunctionEditorDoc* m_pDoc;
    BOOL m_bIsDelete;
    ETYPE_BUS m_eBus;
public:
    CBusEventHandlerDlg(ETYPE_BUS eBus, CFunctionEditorDoc* pDoc, CWnd* pParent =nullptr, BOOL bIsDelete = FALSE);   // standard constructor
    CStringArray m_omStrArrayBusEventHandler;
    virtual ~CBusEventHandlerDlg();

    // Dialog Data
    enum { IDD = IDD_DLG_BUSEV_HANDLER };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    BOOL m_bChkBusEventConnect;
    BOOL m_bChkBusEventPreConnect;
    BOOL m_bChkBusEventDisConnect;
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
};
