#pragma once
#include "Tx_MDIChildBase.h"       // For Common base class defintion
//#include "TxFlexRayDataStore.h"
#include "TransmitMsg.h"
#include "TransmitMsg_LIN.h"
#include "ScheduleTableCfgDlg.h"

class CTxMsgChildFrame : public CMDIChildBase
{
    DECLARE_DYNCREATE(CTxMsgChildFrame)
    // Attributes
protected:
    CSplitterWnd m_omRootSplitter;
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
    DECLARE_MESSAGE_MAP()

public:
    CTransmitMsg*       m_pTransmitMsgView;
    CTransmitMsgLIN*    m_pLinTransmitMsgView;
    CTxMsgChildFrame();
    CTxMsgChildFrame(ETYPE_BUS eBUS);
    // Standard destructor
    virtual ~CTxMsgChildFrame();
    afx_msg void OnClose();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
    afx_msg void OnDestroy();
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
    ETYPE_BUS m_eBUS;
};