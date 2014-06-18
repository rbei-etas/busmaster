/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @brief Implementation file for CTxMsgChildFrame class
 * @author Raja N
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CTxMsgChildFrame class
 */

#include "TxWindow_stdafx.h"       // Stander Header file
#include "TxWindow_resource.h"
#include "Utility/SignalMatrix.h"  // For Signal Matrix declaration
#include "TxMsgChildFrame.h"       // For CTxMsgChildFrame class defintions
#include "Utility\MultiLanguageSupport.h"

extern CTxMsgChildFrame* g_pomTxMsgChildWindow;

// Local #definitions
#define defTX_MSG_ROOTSPLITTER_ROWS 2
#define defTX_MSG_ROOTSPLITTER_COLS 1

#define defTX_MSG_LEFT_VIEW_ROWS 1
#define defTX_MSG_LEFT_VIEW_COLS 2

#define defTX_MSG_RIGHT_VIEW_ROWS 2
#define defTX_MSG_RIGHT_VIEW_COLS 1

#define defTX_LEFT_VIEW_ROW 0
#define defTX_LEFT_VIEW_COL 0

#define defTX_RIGHT_VIEW_ROW 0
#define defTX_RIGHT_VIEW_COL 1

#define defTX_BLOCKS_VIEW_ROW 0
#define defTX_BLOCKS_VIEW_COL 0

#define defTX_FUNC_VIEW_ROW 1
#define defTX_FUNC_VIEW_COL 0

#define defTX_LIST_VIEW_ROW 0
#define defTX_LIST_VIEW_COL 0

#define defTX_DETAILS_VIEW_ROW 1
#define defTX_DETAILS_VIEW_COL 0

IMPLEMENT_DYNCREATE(CTxMsgChildFrame, CMDIChildBase)

CTxMsgChildFrame::CTxMsgChildFrame() : CMDIChildBase( TX_WND_PLACEMENT )
{
    // Initialise pointers
    m_pomTxMsgBlocksView = nullptr;
    m_pomTxMsgListView = nullptr;
    m_pomTxMsgDetailsView = nullptr;
    m_pomFunctionsView = nullptr;
    m_sSplitterPosition.m_nRootSplitterData[0][0] = -1;
    m_bInit = TRUE;
}

CTxMsgChildFrame::~CTxMsgChildFrame()
{
}


BEGIN_MESSAGE_MAP(CTxMsgChildFrame, CMDIChildBase)
    ON_WM_ERASEBKGND()
    ON_WM_DESTROY()
    ON_WM_CLOSE()
    ON_MESSAGE(WM_USER_CMD,vUserCommand)
    ON_WM_SIZE()
    ON_WM_SIZING()
    ON_WM_MDIACTIVATE()
    ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

BOOL CTxMsgChildFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
    m_bInit = TRUE;
    BOOL bReturn = TRUE;
    // Create Parent Splitter
    bReturn = m_omRootSplitter.CreateStatic( this,
              defTX_MSG_ROOTSPLITTER_ROWS,
              defTX_MSG_ROOTSPLITTER_COLS );

    // Check for success
    if( bReturn == TRUE )
    {
        // Default size
        CSize omSize(100,100);
        // Get ID of Top view
        int nChild = m_omRootSplitter.IdFromRowCol( defTX_LEFT_VIEW_ROW,
                     defTX_LEFT_VIEW_COL );
        // Create Left Side Message Block details splitter
        bReturn =
            m_omLeftViewSplitter.CreateStatic( &m_omRootSplitter,
                                               defTX_MSG_LEFT_VIEW_ROWS,
                                               defTX_MSG_LEFT_VIEW_COLS,
                                               WS_CHILD | WS_VISIBLE | WS_BORDER,
                                               nChild );
        // Irder of view creation
        // 1. Message Details View
        // 2. Message List View
        // 3. Function view
        // 4. Message Blocks view

        if( bReturn == TRUE )
        {
            // Get Child ID for Right side view
            int nChildID = m_omLeftViewSplitter.IdFromRowCol(
                               defTX_RIGHT_VIEW_ROW, defTX_RIGHT_VIEW_COL );
            // Create right side Splitter
            bReturn =
                m_omRightViewSplitter.CreateStatic( &m_omLeftViewSplitter,
                                                    defTX_MSG_RIGHT_VIEW_ROWS,
                                                    defTX_MSG_RIGHT_VIEW_COLS,
                                                    WS_CHILD | WS_VISIBLE | WS_BORDER,
                                                    nChildID );
            // Create right side bottom view
            if( bReturn == TRUE )
            {
                bReturn =
                    m_omRightViewSplitter.CreateView( defTX_DETAILS_VIEW_ROW,
                                                      defTX_DETAILS_VIEW_COL,
                                                      RUNTIME_CLASS( CTxMsgDetailsView),
                                                      omSize,
                                                      pContext );
            }
            // Create Right side top view
            if( bReturn == TRUE )
            {
                bReturn =
                    m_omRightViewSplitter.CreateView( defTX_LIST_VIEW_ROW,
                                                      defTX_LIST_VIEW_COL,
                                                      RUNTIME_CLASS( CTxMsgListView),
                                                      omSize,
                                                      pContext );
            }
        }
        if( bReturn == TRUE )
        {
            // Create bottom view
            bReturn =
                m_omRootSplitter.CreateView( defTX_FUNC_VIEW_ROW,
                                             defTX_FUNC_VIEW_COL,
                                             RUNTIME_CLASS(CTxFunctionsView),
                                             omSize,
                                             pContext );
        }

        //if successful
        if( bReturn == TRUE )
        {
            // Create Message blocks view splitter
            bReturn =
                m_omLeftViewSplitter.CreateView( defTX_BLOCKS_VIEW_ROW,
                                                 defTX_BLOCKS_VIEW_COL,
                                                 RUNTIME_CLASS( CTxMsgBlocksView),
                                                 omSize,pContext );
        }

        // Set the size of parent splitter
        if( bReturn == TRUE )
        {
            // Get Splitter information
            vGetSplitterStatus();
            // Set Splitter postion
            vSetSplitterPostion();
        }
        m_bInit = FALSE;
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

    return bReturn;
}

void CTxMsgChildFrame::OnClose()
{
    if(nullptr != m_pomFunctionsView)
    {
        m_pomFunctionsView->OnInvokeClose();
        CMDIChildBase::OnClose();
    }
}

void CTxMsgChildFrame::vGetSplitterStatus()
{
    // Splitter data structure
    PSTXMSGSPLITTERDATA psData = &m_sSplitterPosition;
    // Get Splitter postion from configuration module
    CTxWndDataStore::ouGetTxWndDataStoreObj().bGetTxData( TX_WND_SPLITTER_DATA, (void**)&psData );
    // Check for initial condition
    if( m_sSplitterPosition.m_nRootSplitterData[0][0] == -1 )
    {
        CRect omWndSize;
        GetWindowRect( omWndSize );
        CTxWndDataStore::ouGetTxWndDataStoreObj().
        bGetDefaultTXSplitterPostion( omWndSize, (void**)&psData );
    }
}

void CTxMsgChildFrame::vSetSplitterPostion()
{
    // Set Parent Splitter Postion
    // In the splitter postion array data is stored in the sequence as
    // For First Row or Col
    // [0][0] - Ideal Value
    // [0][1] - Min Value

    // For Second Row or Col
    // [1][0] - Ideal Value
    // [1][1] - Min Value

    // Root Splitter
    m_omRootSplitter.SetRowInfo( 0,
                                 m_sSplitterPosition.m_nRootSplitterData[0][0],
                                 m_sSplitterPosition.m_nRootSplitterData[0][1]);
    m_omRootSplitter.SetRowInfo( 1,
                                 m_sSplitterPosition.m_nRootSplitterData[1][0],
                                 m_sSplitterPosition.m_nRootSplitterData[1][1]);
    // Update UI
    m_omRootSplitter.RecalcLayout();

    // Set Left Splitter
    m_omLeftViewSplitter.SetColumnInfo( 0,
                                        m_sSplitterPosition.m_nLeftViewSplitterData[0][0],
                                        m_sSplitterPosition.m_nLeftViewSplitterData[0][1]);
    m_omLeftViewSplitter.SetColumnInfo( 1,
                                        m_sSplitterPosition.m_nLeftViewSplitterData[1][0],
                                        m_sSplitterPosition.m_nLeftViewSplitterData[1][1]);
    // Update UI
    m_omLeftViewSplitter.RecalcLayout();

    // Set Right Spliter
    m_omRightViewSplitter.SetRowInfo(0,
                                     m_sSplitterPosition.m_nRightViewSplitterData[0][0],
                                     m_sSplitterPosition.m_nRightViewSplitterData[0][1]);

    m_omRightViewSplitter.SetRowInfo(1,
                                     m_sSplitterPosition.m_nRightViewSplitterData[1][0],
                                     m_sSplitterPosition.m_nRightViewSplitterData[1][1]);
    // Update UI
    m_omRightViewSplitter.RecalcLayout();
}

void CTxMsgChildFrame::vSaveSplitterPostion()
{
    // For First Row or Col
    // [0][0] - Ideal Value
    // [0][1] - Min Value

    // For Second Row or Col
    // [1][0] - Ideal Value
    // [1][1] - Min Value

    // Get Splitter Window Postion and save it
    STXMSGSPLITTERDATA sPostion;
    // Root Splitter Window
    m_omRootSplitter.GetRowInfo( 0,
                                 sPostion.m_nRootSplitterData[0][0],
                                 sPostion.m_nRootSplitterData[0][1]);
    m_omRootSplitter.GetRowInfo( 1,
                                 sPostion.m_nRootSplitterData[1][0],
                                 sPostion.m_nRootSplitterData[1][1]);

    // Left Splitter
    m_omLeftViewSplitter.GetColumnInfo( 0,
                                        sPostion.m_nLeftViewSplitterData[0][0],
                                        sPostion.m_nLeftViewSplitterData[0][1]);
    m_omLeftViewSplitter.GetColumnInfo( 1,
                                        sPostion.m_nLeftViewSplitterData[1][0],
                                        sPostion.m_nLeftViewSplitterData[1][1]);

    // Right Splitter
    m_omRightViewSplitter.GetRowInfo(0,
                                     sPostion.m_nRightViewSplitterData[0][0],
                                     sPostion.m_nRightViewSplitterData[0][1]);
    m_omRightViewSplitter.GetRowInfo(1,
                                     sPostion.m_nRightViewSplitterData[1][0],
                                     sPostion.m_nRightViewSplitterData[1][1]);

    // Do memory compare before storing the postion
    if( memcmp( &m_sSplitterPosition,
                &sPostion,
                sizeof(STXMSGSPLITTERDATA)) != 0)
    {
        // Update Local copy
        m_sSplitterPosition = sPostion;
        // Save it in configuration file
        CTxWndDataStore::ouGetTxWndDataStoreObj().bSetTxData( TX_WND_SPLITTER_DATA,
                (void* )&m_sSplitterPosition );
    }
}


void CTxMsgChildFrame::vUpdateWndCo_Ords()
{
    // Call parent to save window postion
    CMDIChildBase::vUpdateWndCo_Ords();
    // Save Splitter postion
    vSaveSplitterPostion();
}

void CTxMsgChildFrame::vUpdateWinStatus()
{
    // Call parent window function for setting window status
    CMDIChildBase::vUpdateWinStatus();
    // Get Splitter postion from configuration file
    vGetSplitterStatus();
    // Set Splitter Postion
    vSetSplitterPostion();
}

BOOL CTxMsgChildFrame::OnEraseBkgnd(CDC* /*pDC*/)
{
    return TRUE;
}

void CTxMsgChildFrame::OnDestroy()
{
    // Save Splitter postion
    vSaveSplitterPostion();
    // Call parent function to do clean up
    CMDIChildBase::OnDestroy();
    g_pomTxMsgChildWindow = nullptr;
}

void CTxMsgChildFrame::vSetTxMsgViewPointers( ETXMSGVIEWTYPE eViewIdentity,
        CWnd* pomWnd )
{
    switch( eViewIdentity )
    {
        case eTxMsgBlocksView: // Message Blocks View
        {
            m_pomTxMsgBlocksView = static_cast<CTxMsgBlocksView*>( pomWnd );
        }
        break;
        case eTxMsgMessageListView: // Message List View
        {
            m_pomTxMsgListView = static_cast<CTxMsgListView*>( pomWnd );;
        }
        break;
        case eTxMsgMessageDetailsView: // Message Details View
        {
            m_pomTxMsgDetailsView = static_cast<CTxMsgDetailsView*>( pomWnd );
        }
        break;
        case eTxMsgFunctionsView: // Functions View
        {
            m_pomFunctionsView = static_cast<CTxFunctionsView*>( pomWnd );
        }
        break;
        default:    // Invalid Parameter
            ASSERT( FALSE );
    }
}

void CTxMsgChildFrame::vSetMsgDBPtrInDetailsView(void* pMsgDB)
{
    m_pomTxMsgDetailsView->vSetMsgDBPtr(pMsgDB);
}

CWnd* CTxMsgChildFrame::pomGetTxMsgViewPointers( ETXMSGVIEWTYPE eViewIdentity)
const
{
    CWnd* pomWnd = nullptr;
    switch( eViewIdentity )
    {
        case eTxMsgBlocksView: // Message Blocks View
        {
            pomWnd = m_pomTxMsgBlocksView;
        }
        break;
        case eTxMsgMessageListView: // Message List View
        {
            pomWnd = m_pomTxMsgListView;
        }
        break;
        case eTxMsgMessageDetailsView: // Message Details View
        {
            pomWnd = m_pomTxMsgDetailsView;
        }
        break;
        case eTxMsgFunctionsView: // Functions View
        {
            pomWnd = m_pomFunctionsView;
        }
        break;
        default:    // Invalid Parameter
            ASSERT( FALSE );
    }
    return pomWnd;
}

void CTxMsgChildFrame::vCallAutoUpdate()
{
    if(m_pomTxMsgBlocksView != nullptr)
    {
        m_pomTxMsgBlocksView->AutoUpdateChanges();
    }
}

LRESULT CTxMsgChildFrame::vUserCommand(WPARAM wParam, LPARAM lParam)
{

    if( m_pomTxMsgBlocksView != nullptr &&
            m_pomTxMsgDetailsView != nullptr &&
            m_pomTxMsgListView != nullptr &&
            m_pomFunctionsView != nullptr )
    {
        INT nBase              = 0;
        BOOL bTxON             = FALSE;
        BOOL bConnect          = FALSE;
        eUSERSELCTION eUserSel = eDATABASEIMPORTCMD;

        eUserSel               = static_cast <eUSERSELCTION>(wParam);
        switch(eUserSel)
        {
            case eHEXDECCMD:
                //// Get the flag status.
                nBase = (INT)lParam;
                // Check if the flag is changed and if so,
                // change the display format.
                if(nBase != CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_HEX))
                {
                    CTxMsgManager::s_TxFlags.vSetFlagStatus(TX_HEX, nBase);
                    // Hide any edit controls if it is visible
                    m_pomTxMsgDetailsView->m_omLctrSigList.SetFocus();

                    if ( m_pomTxMsgBlocksView->m_psMsgBlockList != nullptr )
                    {
                        PSMSGBLOCKLIST psMsgBlock =
                            m_pomTxMsgBlocksView->psGetMsgBlockPointer(
                                m_pomTxMsgBlocksView->m_nSelectedMsgBlockIndex,
                                m_pomTxMsgBlocksView->m_psMsgBlockList );
                        if(psMsgBlock != nullptr)
                        {
                            m_pomTxMsgListView->
                            m_omLctrMsgList.DeleteAllItems();
                            m_pomTxMsgBlocksView->
                            vDisplayMsgBlockDetails(psMsgBlock);
                        }
                    }
                    m_pomTxMsgBlocksView->vSetControlProperties();
                    m_pomTxMsgDetailsView->vSetControlProperties();
                    // Clear the controls after changing the properties.
                    m_pomTxMsgDetailsView->vSetValues(nullptr);
                    // Remove all entries in the list and disable
                    m_pomTxMsgDetailsView->vDisableSignalComponents();
                    // Disable Delete Button & Send Button as the focus is lost
                    m_pomTxMsgListView->
                    m_omButtonDeleteSelMsg.EnableWindow(FALSE);
                    m_pomTxMsgListView->
                    m_omButtonSendMsg.EnableWindow(FALSE);
                    m_pomTxMsgListView->m_nSelectedMsgIndex = -1;
                    // Set Lable to indicate Mode
                    CString omStrText = CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_HEX) ?
                                        _(defSTR_HEX_MODE) : _(defSTR_DEC_MODE);
                    CWnd* pomLabel =
                        m_pomTxMsgDetailsView->GetDlgItem(IDC_STAT_HEADER2);
                    if( pomLabel != nullptr )
                    {
                        // Set the text with Hex/Dec mode
                        pomLabel->SetWindowText(omStrText);
                    }
                }
                break;
            case eTXMSGCMD:
                // Get the flag status.
                bTxON = (BOOL)lParam;
                // If transmission is ON, user should not be able to
                // delete any message or block.
                if(bTxON != CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG))
                {
                    CTxMsgManager::s_TxFlags.vSetFlagStatus(TX_SENDMESG, bTxON);
                    // Get the message count and disable if
                    // there is some message list in the currently
                    // selected block.
                    if ( m_pomTxMsgBlocksView->m_psMsgBlockList != nullptr )
                    {
                        PSMSGBLOCKLIST psMsgBlock =
                            m_pomTxMsgBlocksView->psGetMsgBlockPointer(
                                m_pomTxMsgBlocksView->m_nSelectedMsgBlockIndex,
                                m_pomTxMsgBlocksView->m_psMsgBlockList );

                        if(psMsgBlock != nullptr)
                        {
                            if( psMsgBlock->m_unMsgCount > 0 )
                            {
                                m_pomTxMsgListView->m_omButtonDeleteAllMsg.
                                EnableWindow(!bTxON);
                            }
                        }
                        //If it is key triggered then Enable/Diable All Messages
                        //Check box and Key val edit box
                        if( IS_KEY_TRIGGERED (psMsgBlock->m_ucTrigger) )
                        {
                            m_pomTxMsgBlocksView->m_omComboAllMsgs.EnableWindow(!bTxON);
                            m_pomTxMsgBlocksView->m_omEditTrgKeyVal.
                            EnableWindow(!bTxON);
                        }

                        if(bTxON)
                        {
                            m_pomTxMsgBlocksView->m_omComboAllMsgs.EnableWindow(!bTxON);
                            m_pomTxMsgBlocksView->m_omEditTrgKeyVal.
                            EnableWindow(!bTxON);
                        }


                        //only if DElay btn msg blocks check box is not checked den check time trigger button
                        if(((CButton*) m_pomTxMsgBlocksView->
                                GetDlgItem(IDC_CHECK_MSG_BLOCK_DELAY))->GetCheck() == BST_UNCHECKED)
                        {
                            m_pomTxMsgBlocksView->m_omButtonTimeTrigger.
                            EnableWindow(!bTxON);
                            // If it is time triggered then time interval edit box
                            if( IS_TIME_TRIGGERED (psMsgBlock->m_ucTrigger) )
                            {
                                m_pomTxMsgBlocksView->m_omEditTrgTimeIntervalVal.
                                EnableWindow(!bTxON);
                            }
                        }
                        ((CButton*) m_pomTxMsgBlocksView->GetDlgItem(IDC_CHECK_MSG_BLOCK_DELAY))->EnableWindow(!bTxON);

                        if((((CButton*) m_pomTxMsgBlocksView->
                                GetDlgItem(IDC_CHECK_MSG_BLOCK_DELAY))->IsWindowEnabled() == TRUE)&&
                                ((CButton*) m_pomTxMsgBlocksView->
                                 GetDlgItem(IDC_CHECK_MSG_BLOCK_DELAY))->GetCheck() == BST_CHECKED)
                        {
                            ((CEdit*) m_pomTxMsgBlocksView->
                             GetDlgItem(IDC_EDIT_BLOCK_TRG_TIMER_VAL))->EnableWindow(TRUE);
                        }
                        else
                        {
                            ((CEdit*) m_pomTxMsgBlocksView->
                             GetDlgItem(IDC_EDIT_BLOCK_TRG_TIMER_VAL))->EnableWindow(FALSE);
                        }
                        m_pomTxMsgBlocksView->m_omButtonKeyTrigger.
                        EnableWindow(!bTxON);
                        CButton* pRadioMonoshot = (CButton*)m_pomTxMsgBlocksView->GetDlgItem(IDC_RADIOMONOSHOT);
                        CButton* pRadioCyclic = (CButton*)m_pomTxMsgBlocksView->GetDlgItem(IDC_RADIOCYCLIC);
                        if ((pRadioMonoshot != nullptr) && (pRadioCyclic != nullptr))
                        {
                            pRadioMonoshot->EnableWindow(!bTxON);
                            pRadioCyclic->EnableWindow(!bTxON);
                        }
                        m_pomTxMsgBlocksView->m_omButtonAddMsgBlock.EnableWindow(!bTxON);
                        m_pomTxMsgBlocksView->m_omEditMsgBlockName.EnableWindow(!bTxON);
                    }

                    if( m_pomTxMsgListView->m_nSelectedMsgIndex != -1 )
                    {
                        m_pomTxMsgListView->m_omButtonDeleteSelMsg.
                        EnableWindow(!bTxON);
                    }
                    if(m_pomTxMsgBlocksView->m_nSelectedMsgBlockIndex !=
                            -1 )
                    {
                        m_pomTxMsgBlocksView->m_omButtonDeleteMsgBlock.
                        EnableWindow(!bTxON);
                    }

                    // Set the focus to the block list control if transmission is started
                    // to capture the key events
                    if (bTxON)
                    {
                        m_pomTxMsgBlocksView->m_omLctrMsgBlockName.SetFocus();
                    }
                }
                break;
            case eCONNECTCMD:
                // Get the flag status.
                bConnect = (BOOL)lParam;
                // If the tool is disconnected, user should not be able
                // send any message.

                if(bConnect != CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_CONNECTED))
                {
                    if( m_pomTxMsgListView->m_nSelectedMsgIndex != -1 )
                    {
                        m_pomTxMsgListView->m_omButtonSendMsg.
                        EnableWindow(bConnect);
                    }
                    else
                    {
                        m_pomTxMsgListView->m_omButtonSendMsg.
                        EnableWindow(FALSE);
                    }
                    CTxMsgManager::s_TxFlags.vSetFlagStatus(TX_CONNECTED, bConnect);
                }
                break;

            case eDATABASEIMPORTCMD:

                m_pomTxMsgDetailsView->m_omComboMsgIDorName.ResetContent();
                m_pomTxMsgDetailsView->vPopulateMessageComboBox();

                // The icon for database message may change to change the
                // display.
                if ( m_pomTxMsgBlocksView->m_psMsgBlockList != nullptr )
                {
                    PSMSGBLOCKLIST psMsgBlock =
                        m_pomTxMsgBlocksView->psGetMsgBlockPointer(
                            m_pomTxMsgBlocksView->m_nSelectedMsgBlockIndex,
                            m_pomTxMsgBlocksView->m_psMsgBlockList );
                    if(psMsgBlock != nullptr)
                    {
                        m_pomTxMsgListView->m_omLctrMsgList.DeleteAllItems();
                        m_pomTxMsgBlocksView->
                        vDisplayMsgBlockDetails(psMsgBlock);
                        m_pomTxMsgDetailsView->vUpdateAllBlocksFrmDB();
                        m_pomTxMsgBlocksView->AutoUpdateChanges();
                    }
                }
                m_pomTxMsgDetailsView->vSetValues(nullptr);
                // Clear Signal Components
                m_pomTxMsgDetailsView->vDisableSignalComponents();
                // Disable Delete Button & Send Button as the focus is lost
                m_pomTxMsgListView->m_omButtonDeleteSelMsg.EnableWindow(FALSE);
                m_pomTxMsgListView->m_omButtonSendMsg.EnableWindow(FALSE);
                m_pomTxMsgListView->m_nSelectedMsgIndex = -1;
                break;

            case eCHANNELCOUNTUPDATED:
                m_pomTxMsgDetailsView->vUpdateChannelIDInfo();
                break;

            default:
                ASSERT( FALSE );
                break;
        }
    }

    return 0;
}

void CTxMsgChildFrame::OnSize(UINT nType, int cx, int cy)
{
    CMDIChildBase::OnSize(nType, cx, cy);
}

void CTxMsgChildFrame::OnSizing(UINT fwSide, LPRECT pRect)
{
    CMDIChildBase::OnSizing( fwSide,  pRect);
    if ( !m_bInit )
    {
        CRect cr;
        GetWindowRect(&cr);

        m_omRootSplitter.SetRowInfo( 0, cr.Height()*5.75/7, 0);

        m_omRootSplitter.SetRowInfo( 1, cr.Height()/7, 50 );


        m_omLeftViewSplitter.SetColumnInfo(0, cr.Width()/3.75, 0);
        m_omLeftViewSplitter.SetColumnInfo(1, cr.Width()/2, 0);

        m_omRightViewSplitter.SetRowInfo(0,cr.Height()/3, 0);
        m_omRightViewSplitter.SetRowInfo(1,cr.Height()/3, 0);
        m_omRootSplitter.RecalcLayout();
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
    if(!m_bInit)
    {
        lpMMI->ptMinTrackSize.x = 850;
        lpMMI->ptMinTrackSize.y = 675;
    }

    CWnd::OnGetMinMaxInfo(lpMMI);
}
