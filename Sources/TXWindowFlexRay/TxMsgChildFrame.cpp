
#pragma once
#include "TxWindowFlexRay_stdafx.h"
#include "TxWindowFlexRay_resource.h"
#include "TxWndFlexRay_Extern.h"
#include "TxMsgChildFrame.h"
#include "TransmitMsg.h"


extern CTxMsgChildFrame* g_pomTxMsgFlexChildWindow;
extern CTxMsgChildFrame* g_pomTxMsgLinChildWindow;

IMPLEMENT_DYNCREATE(CTxMsgChildFrame, CMDIChildBase)


CTxMsgChildFrame::CTxMsgChildFrame()
{
    // Initialise pointers
    m_pTransmitMsgView = NULL;
    m_pLinTransmitMsgView = NULL;
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
    m_pTransmitMsgView = NULL;
    m_pLinTransmitMsgView = NULL;

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
    //ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


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
BOOL CTxMsgChildFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
    BOOL bReturn = TRUE;
    // Create Parent Splitter
    bReturn = m_omRootSplitter.CreateStatic( this,1,1/*,WS_VISIBLE | WS_BORDER*/);

    //if successful
    if( bReturn == TRUE )
    {
        CSize omSize(0,0);

        if(m_eBUS == FLEXRAY)
        {
            // Create Message blocks view splitter
            bReturn =
                m_omRootSplitter.CreateView( 0,
                                             0,
                                             RUNTIME_CLASS( CTransmitMsg),
                                             CSize(400,400),pContext );
        }
        else if(m_eBUS == LIN)
        {
            // Create Message blocks view splitter
            bReturn =
                m_omRootSplitter.CreateView( 0,
                                             0,
                                             RUNTIME_CLASS( CTransmitMsgLIN),
                                             CSize(400,400),pContext );
        }
    }

    /*CWnd * pWnd = CreateView(pContext);
    if(pWnd != NULL)
    {
        pWnd->SetActiveWindow();
        pWnd->ShowWindow(SW_SHOW);
    }*/
    /* Try to load resource DLL for icons*/
    HMODULE hModAdvancedUILib = ::LoadLibrary("AdvancedUIPlugIn.dll");

    if ( hModAdvancedUILib )
    {
        SetIcon( ::LoadIcon(hModAdvancedUILib, MAKEINTRESOURCE( IDI_ICO_TX_WINDOW )), TRUE);
        ::FreeLibrary(hModAdvancedUILib);
        hModAdvancedUILib = NULL;
    }
    else
    {
        HICON hIco = AfxGetApp()->LoadIcon(IDI_ICO_TX_WINDOW);
        SetIcon(hIco, TRUE);
    }
    return TRUE;

}

void CTxMsgChildFrame::OnClose()
{
    CMDIChildBase::OnClose();
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
    if ( m_eBUS == FLEXRAY )
    {
        g_pomTxMsgFlexChildWindow = NULL;
    }
    else
    {
        g_pomTxMsgLinChildWindow = NULL;
    }


}

void CTxMsgChildFrame::OnSize(UINT nType, int cx, int cy)
{
    CMDIChildBase::OnSize(nType, cx, cy);
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
    {
        lpMMI->ptMinTrackSize.x = 860;
        lpMMI->ptMinTrackSize.y = 550;
    }

    CWnd::OnGetMinMaxInfo(lpMMI);
}