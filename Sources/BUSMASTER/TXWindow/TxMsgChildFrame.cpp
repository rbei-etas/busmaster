
#pragma once
#include "stdafx.h"
#include "resource.h"
#include "TXWindow_Extern.h"
#include "TxMsgChildFrame.h"
#include "CANTxFormView.h"
#include "LINTxFormView.h"

extern CTxMsgChildFrame* g_pomTxMsgLinChildWindow;
extern CTxMsgChildFrame* g_pomTxMsgCanChildWindow;
extern CScheduleTableCfgDlg* g_pomLINScheduleCfgDlg;
IMPLEMENT_DYNCREATE(CTxMsgChildFrame, CMDIChildBase)


CTxMsgChildFrame::CTxMsgChildFrame()
{
    // Initialise pointers
    g_pomLINScheduleCfgDlg = nullptr;
}
/*******************************************************************************
  Function Name  : CTxMsgChildFrame
  Description    : Standard default constructor
  Member of      : CTxMsgChildFrame
  Functionality  : This will initialise local variables
  Author(s)      : Ashwin R Uchil
  Date Created   : 21.3.2013
  Modifications  :
*******************************************************************************/
CTxMsgChildFrame::CTxMsgChildFrame(ETYPE_BUS eBUS) : CMDIChildBase( eBUS )
{
    // Initialise pointers
    g_pomLINScheduleCfgDlg = nullptr;
    m_eBUS = eBUS;
}



CTxMsgChildFrame::~CTxMsgChildFrame()
{
}


BEGIN_MESSAGE_MAP(CTxMsgChildFrame, CMDIChildBase)
    //{{AFX_MSG_MAP(CTxMsgChildFrame)

    //}}AFX_MSG_MAP
    //ON_MESSAGE(WM_USER_CMD,vUserCommand)
    ON_WM_SIZE()
    ON_WM_DESTROY()
    //ON_WM_SIZING()
    ON_WM_CLOSE()
    ON_WM_MDIACTIVATE()
    ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

ITxFormView* CTxMsgChildFrame::GetTxFormViewInstance()
{
    ITxFormView*  pITxFormView = nullptr;
    switch (m_eBUS)
    {
        case LIN:
            pITxFormView = CLINTxFormView::GetInstance();
            break;
        case CAN:
            pITxFormView = CCANTxFormView::GetInstance();
            break;
        default:
            break;
    }
    return pITxFormView;
}
void CTxMsgChildFrame::ReleaseTxFormViewInstance()
{
    switch (m_eBUS)
    {
        case LIN:
            CLINTxFormView::ReleaseInstance();
            break;
        case CAN:
            CCANTxFormView::ReleaseInstance();
            break;
        default:
            break;
    }
}
/*******************************************************************************
  Function Name  : OnCreateClient
  Input(s)       : lpcs - Create Parameters
                   pContext - Pointer to Create Context
  Output         : BOOL - Create Result
  Functionality  : This function will be called to create window. This function
                   will create splitter window and views.
  Member of      : CTxMsgChildFrame
  Author(s)      : Raja N
  Date Created   : 15/04/2005
  Modifications  :
*******************************************************************************/
BOOL CTxMsgChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
    BOOL bReturn = TRUE;
    // Create Parent Splitter
    //bReturn = m_omRootSplitter.CreateStatic( this, 1, 2/*,WS_VISIBLE | WS_BORDER*/ );

    CRect omRect;
    this->GetClientRect( omRect );

    //if successful
    ITxFormView*    pITxFormView = GetTxFormViewInstance();
    if (nullptr != pITxFormView)
    {
        pITxFormView->Create(this);
        pITxFormView->OnInitialUpdate();
        ReleaseTxFormViewInstance();
    }

    /* Try to load resource DLL for icons*/
    HMODULE hModAdvancedUILib = ::LoadLibrary("AdvancedUIPlugIn.dll");

    if ( hModAdvancedUILib )
    {
        SetIcon( ::LoadIcon(hModAdvancedUILib, MAKEINTRESOURCE( IDI_ICO_TX_WINDOW )), TRUE);
        ::FreeLibrary(hModAdvancedUILib);
        hModAdvancedUILib = nullptr;
    }
    else
    {
        HICON hIco = AfxGetApp()->LoadIcon(IDI_ICO_TX_WINDOW);
        SetIcon(hIco, TRUE);
    }

    return CMDIChildBase::OnCreateClient( lpcs, pContext );

}

void CTxMsgChildFrame::OnClose()
{
    if (LIN == m_eBUS || CAN == m_eBUS)
    {
        ShowWindow(SW_HIDE);
    }
    else
    {
        CMDIChildBase::OnClose();
    }
}


/*******************************************************************************
  Function Name  : OnDestroy
  Input(s)       : -
  Output         : -
  Functionality  :
  Member of      : CTxMsgChildFrame
  Author(s)      : Ashwin R Uchil
  Date Created   : 28.3.2013
  Modifications  :
*******************************************************************************/
void CTxMsgChildFrame::OnDestroy()
{
    // Save Splitter postion
    //vSaveSplitterPostion();
    // Call parent function to do clean up
    CMDIChildBase::OnDestroy();
    if (LIN == m_eBUS)
    {
        g_pomTxMsgLinChildWindow = nullptr;
    }
    else if (CAN == m_eBUS)
    {
        g_pomTxMsgCanChildWindow = nullptr;
    }

}

void CTxMsgChildFrame::OnSize(UINT nType, int cx, int cy)
{
    CMDIChildBase::OnSize(nType, cx, cy);
    CRect omRect;
    this->GetClientRect( omRect );

    ITxFormView*    pITxFormView = GetTxFormViewInstance();
    if (nullptr != pITxFormView)
    {
        pITxFormView->MoveWindow(omRect);
        ReleaseTxFormViewInstance();
    }

}

void CTxMsgChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
    if(bActivate == FALSE)
    {
        this->SetFocus();
    }
    CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
}


void CTxMsgChildFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    // TODO: Add your message handler code here and/or call default

    // if(!m_bInit)
    CRect lpRect;
    GetWindowRect(&lpRect);
    ScreenToClient(&lpRect);
    lpMMI->ptMinTrackSize.x = 860;
    lpMMI->ptMinTrackSize.y = 655;

    lpMMI->ptMaxTrackSize.x = 860;
    lpMMI->ptMaxTrackSize.y = 655;



    CWnd::OnGetMinMaxInfo(lpMMI);
}