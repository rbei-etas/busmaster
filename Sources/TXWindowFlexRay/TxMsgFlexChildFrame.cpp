
#pragma once
#include "TxWindowFlexRay_stdafx.h"
#include "TxWindowFlexRay_resource.h"
#include "TxWndFlexRay_Extern.h"
#include "TxMsgFlexChildFrame.h"
#include "TransmitMsg.h"


extern CTxMsgFlexChildFrame* g_pomTxMsgFlexChildWindow;
IMPLEMENT_DYNCREATE(CTxMsgFlexChildFrame, CMDIChildBase)

/*******************************************************************************
  Function Name  : CTxMsgFlexChildFrame
  Description    : Standard default constructor
  Member of      : CTxMsgFlexChildFrame
  Functionality  : This will initialise local variables
  Author(s)      : Ashwin R Uchil
  Date Created   : 21.3.2013
  Modifications  :
*******************************************************************************/
CTxMsgFlexChildFrame::CTxMsgFlexChildFrame() : CMDIChildBase( TX_WND_PLACEMENT )
{
    // Initialise pointers
    m_pTransmitMsgView = NULL;
}



CTxMsgFlexChildFrame::~CTxMsgFlexChildFrame()
{

}


BEGIN_MESSAGE_MAP(CTxMsgFlexChildFrame, CMDIChildBase)
    //{{AFX_MSG_MAP(CTxMsgFlexChildFrame)

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
  Member of      : CTxMsgFlexChildFrame
  Author(s)      : Raja N
  Date Created   : 15/04/2005
  Modifications  :
*******************************************************************************/
BOOL CTxMsgFlexChildFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
    BOOL bReturn = TRUE;
    // Create Parent Splitter
    bReturn = m_omRootSplitter.CreateStatic( this,1,1/*,WS_VISIBLE | WS_BORDER*/);

    //if successful
    if( bReturn == TRUE )
    {
        CSize omSize(0,0);
        // Create Message blocks view splitter
        bReturn =
            m_omRootSplitter.CreateView( 0,
                                         0,
                                         RUNTIME_CLASS( CTransmitMsg),
                                         CSize(400,400),pContext );
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

void CTxMsgFlexChildFrame::OnClose()
{
    if(m_pTransmitMsgView)
    {
        /* if(m_pTransmitMsgView->m_bModified == TRUE)
         {
             if(AfxMessageBox("Do you want to save changes?",MB_YESNO) == IDYES)
             {
                 m_pTransmitMsgView->OnBnClickedBtUpdate();
             }
         }*/
        CMDIChildBase::OnClose();
    }
}


/*******************************************************************************
  Function Name  : OnDestroy
  Input(s)       : -
  Output         : -
  Functionality  :
  Member of      : CTxMsgFlexChildFrame
  Author(s)      : Ashwin R Uchil
  Date Created   : 28.3.2013
  Modifications  :
*******************************************************************************/
void CTxMsgFlexChildFrame::OnDestroy()
{
    // Save Splitter postion
    //vSaveSplitterPostion();
    // Call parent function to do clean up
    CMDIChildBase::OnDestroy();
    g_pomTxMsgFlexChildWindow = NULL;
}

void CTxMsgFlexChildFrame::OnSize(UINT nType, int cx, int cy)
{
    CMDIChildBase::OnSize(nType, cx, cy);
}

void CTxMsgFlexChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
    if(bActivate == FALSE)
    {
        this->SetFocus();
    }
    CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
}


void CTxMsgFlexChildFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    // TODO: Add your message handler code here and/or call default

    // if(!m_bInit)
    {
        lpMMI->ptMinTrackSize.x = 860;
        lpMMI->ptMinTrackSize.y = 550;
    }

    CWnd::OnGetMinMaxInfo(lpMMI);
}