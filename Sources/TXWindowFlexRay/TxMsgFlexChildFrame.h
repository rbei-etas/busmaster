#pragma once
#include "Tx_MDIChildBase.h"       // For Common base class defintion
//#include "TxFlexRayDataStore.h"
#include "TransmitMsg.h"

class CTxMsgFlexChildFrame : public CMDIChildBase
{
    DECLARE_DYNCREATE(CTxMsgFlexChildFrame)
    // Attributes
protected:
    CSplitterWnd m_omRootSplitter;
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
    DECLARE_MESSAGE_MAP()

public:
    CTransmitMsg*       m_pTransmitMsgView;
    CTxMsgFlexChildFrame();
    // Standard destructor
    virtual ~CTxMsgFlexChildFrame();
    afx_msg void OnClose();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
    afx_msg void OnDestroy();
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};