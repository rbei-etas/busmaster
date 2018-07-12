#pragma once
#include "Tx_MDIChildBase.h"       // For Common base class defintion

#include "ScheduleTableCfgDlg.h"
#include "ITxFormView.h"

class CTxMsgChildFrame : public CMDIChildBase
{
    DECLARE_DYNCREATE(CTxMsgChildFrame)
    // Attributes
private:
    ITxFormView* GetTxFormViewInstance();
    void ReleaseTxFormViewInstance();
protected:
    //CSplitterWnd m_omRootSplitter;
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
    DECLARE_MESSAGE_MAP()

public:
    CTxMsgChildFrame();
    CTxMsgChildFrame(ETYPE_BUS eBUS);

    // Standard destructor
    virtual ~CTxMsgChildFrame();
    afx_msg void OnClose();
    CListBox m_omListBox;
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
    afx_msg void OnDestroy();
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
    ETYPE_BUS m_eBUS;
};