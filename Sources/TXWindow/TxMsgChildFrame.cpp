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
 * \file      TxMsgChildFrame.cpp
 * \brief     Implementation file for CTxMsgChildFrame class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CTxMsgChildFrame class
 */

#include "TxWindow_stdafx.h"       // Stander Header file
#include "TxWindow_resource.h"
#include "Utility/SignalMatrix.h"  // For Signal Matrix declaration
#include "TxMsgChildFrame.h"       // For CTxMsgChildFrame class defintions

extern CTxMsgChildFrame* g_pomTxMsgChildWindow;

// Application object declaration
//extern CCANMonitorApp theApp;

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

/*******************************************************************************
  Function Name  : CTxMsgChildFrame
  Description    : Standard default constructor
  Member of      : CTxMsgChildFrame
  Functionality  : This will initialise local variables
  Author(s)      : Raja N
  Date Created   : 19.4.2005
  Modifications  :
*******************************************************************************/
CTxMsgChildFrame::CTxMsgChildFrame() : CMDIChildBase( TX_WND_PLACEMENT )
{
    // Initialise pointers
    m_pomTxMsgBlocksView = NULL;
    m_pomTxMsgListView = NULL;
    m_pomTxMsgDetailsView = NULL;
    m_pomFunctionsView = NULL;
    m_sSplitterPosition.m_nRootSplitterData[0][0] = -1;
    m_bInit = TRUE;
}

/*******************************************************************************
  Function Name  : ~CTxMsgChildFrame
  Description    : Standard Destructor
  Member of      : CTxMsgChildFrame
  Functionality  : -
  Author(s)      : Raja N
  Date Created   : 19.4.2005
  Modifications  :
*******************************************************************************/
CTxMsgChildFrame::~CTxMsgChildFrame()
{
}


BEGIN_MESSAGE_MAP(CTxMsgChildFrame, CMDIChildBase)
    //{{AFX_MSG_MAP(CTxMsgChildFrame)
    ON_WM_ERASEBKGND()
    ON_WM_DESTROY()
    //}}AFX_MSG_MAP
    ON_MESSAGE(WM_USER_CMD,vUserCommand)
    ON_WM_SIZE()
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
    }

    return bReturn;
}

/*******************************************************************************
  Function Name  : vGetSplitterStatus
  Input(s)       : -
  Output         : -
  Functionality  : This function will get splitter information from
                   configuration module
  Member of      : CTxMsgChildFrame
  Author(s)      : Raja N
  Date Created   : 19.4.2005
  Modifications  :
*******************************************************************************/
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

/*******************************************************************************
  Function Name  : vSetSplitterPostion
  Input(s)       : -
  Output         : -
  Functionality  : This function will set splitter postion. This will refer
                   member variable m_sSplitterPostion to get splitter postion
  Member of      : CTxMsgChildFrame
  Author(s)      : Raja N
  Date Created   : 19.4.2005
  Modifications  :
*******************************************************************************/
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

/*******************************************************************************
  Function Name  : vSaveSplitterPostion
  Input(s)       : -
  Output         : -
  Functionality  : This function will save splitter postion. This will get
                   current splitter postion and will compare with
                   m_sSplitterPostion. If current position is different it will
                   update member varaible and configuration module.
  Member of      : CTxMsgChildFrame
  Author(s)      : Raja N
  Date Created   : 19.4.2005
  Modifications  :
*******************************************************************************/
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


/*******************************************************************************
  Function Name  : vUpdateWndCo_Ords
  Input(s)       : -
  Output         : -
  Functionality  : This function will save window postion in to configuration
                   module using parent class function & will also save splitter
                   window postion.
  Member of      : CGraphChildFrame
  Author(s)      : Raja N
  Date Created   : 19.4.2005
  Modifications  :
*******************************************************************************/
void CTxMsgChildFrame::vUpdateWndCo_Ords()
{
    // Call parent to save window postion
    CMDIChildBase::vUpdateWndCo_Ords();
    // Save Splitter postion
    vSaveSplitterPostion();
}

/*******************************************************************************
  Function Name  : vUpdateWinStatus
  Input(s)       : -
  Output         : -
  Functionality  : This function will set window postion and splitter postion.
                   For setting window postion this will use parent class
                   function and then will set splitter position.
  Member of      : CGraphChildFrame
  Author(s)      : Raja N
  Date Created   : 19.4.2005
  Modifications  :
*******************************************************************************/
void CTxMsgChildFrame::vUpdateWinStatus()
{
    // Call parent window function for setting window status
    CMDIChildBase::vUpdateWinStatus();
    // Get Splitter postion from configuration file
    vGetSplitterStatus();
    // Set Splitter Postion
    vSetSplitterPostion();
}

/*******************************************************************************
  Function Name  : OnEraseBkgnd
  Input(s)       : pDC - Pointer to Device context
  Output         : -
  Functionality  : This function will simply return true to avoid flickering due
                   to background erase
  Member of      : CTxMsgChildFrame
  Author(s)      : Raja N
  Date Created   : 19.04.2005
  Modifications  :
*******************************************************************************/
BOOL CTxMsgChildFrame::OnEraseBkgnd(CDC* /*pDC*/)
{
    return TRUE;
}

/*******************************************************************************
  Function Name  : OnDestroy
  Input(s)       : -
  Output         : -
  Functionality  :
  Member of      : CTxMsgChildFrame
  Author(s)      : Raja N
  Date Created   : 19.4.2005
  Modifications  :
*******************************************************************************/
void CTxMsgChildFrame::OnDestroy()
{
    // Save Splitter postion
    vSaveSplitterPostion();
    // Call parent function to do clean up
    CMDIChildBase::OnDestroy();
    g_pomTxMsgChildWindow = NULL;
}

/*******************************************************************************
  Function Name  : vSetTxMsgViewPointers
  Input(s)       : eViewIdentity - View Identity parameter
                   pomWnd - View Pointer
  Output         : -
  Functionality  : This function will set view pointer
  Member of      : CTxMsgChildFrame
  Author(s)      : Raja N
  Date Created   : 20.4.2005
  Modifications  :
*******************************************************************************/
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

/*******************************************************************************
  Function Name  : vSetMsgDBPtrInDetailsView
  Input(s)       : pMsgDB - CMsgSignal Pointer
  Output         : -
  Functionality  : Sets the m_pouDBPtr pointer in TxMsgDetailsView to pMsgDB.

  Member of      : CTxMsgChildFrame
  Author(s)      : ArunKumar K
  Date Created   : 04.08.2010
  Modifications  :
*******************************************************************************/
void CTxMsgChildFrame::vSetMsgDBPtrInDetailsView(void* pMsgDB)
{
    m_pomTxMsgDetailsView->vSetMsgDBPtr(pMsgDB);
}

/*******************************************************************************
  Function Name  : pomGetTxMsgViewPointers
  Input(s)       : eViewIdentity - View Identity parameter
  Output         : -
  Functionality  : This function will return view pointer spefied by the param
                   eViewIdentity
  Member of      : CTxMsgChildFrame
  Author(s)      : Raja N
  Date Created   : 20.4.2005
  Modifications  :
*******************************************************************************/
CWnd* CTxMsgChildFrame::pomGetTxMsgViewPointers( ETXMSGVIEWTYPE eViewIdentity)
const
{
    CWnd* pomWnd = NULL;

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

/******************************************************************************/
/*  Function Name    :  vUserCommand                                          */
/*  Input(s)         :  WPARAM wParam : enumerator eUSERSELCTION              */
/*                      LPARAM lParam:  NOT USED                              */
/*  Output           :                                                        */
/*  Functionality    :  This function is message mapped and will be called    */
/*                      when selects, connect/disconnect start/stop tx and    */
/*                      hex/dec button/menu. The setting of controls will be  */
/*                      changed accordingly.                                  */
/*  Member of        :  CTxMsgChildFrame                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */
/*  Modification By  :  ArunKumar K                                           */
/*  Modification on  :  07.04.2011, Addede new case eCHANNELCOUNTUPDATED.     */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  12.05.2004, Added SetWindowText to indicate mode      */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  22.07.2004, Included  Signla List update during mode  */
/*                      change                                                */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  01.08.2004, Included  Enable/Disable check for timer  */
/*                      and key edit boxes with respect to selection          */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  26.04.2005, Moved this function from CSendMultiMsgDlg */
/*                      to this class as per new GUI for Tx msg conf window   */
/******************************************************************************/
LRESULT CTxMsgChildFrame::vUserCommand(WPARAM wParam, LPARAM lParam)
{
    if( m_pomTxMsgBlocksView != NULL &&
            m_pomTxMsgDetailsView != NULL &&
            m_pomTxMsgListView != NULL &&
            m_pomFunctionsView != NULL )
    {
        INT nBase              = 0;
        BOOL bTxON             = FALSE;
        BOOL bConnect          = FALSE;
        eUSERSELCTION eUserSel = eDATABASEIMPORTCMD;
        //CFlags * pouFlag       = NULL;
        eUserSel               = static_cast <eUSERSELCTION>(wParam);

        switch(eUserSel)
        {
            case eHEXDECCMD:
                //// Get the flag status.
                //pouFlag = pouGetFlagsPtr();
                //if ( pouFlag != NULL )
                //{
                //    nBase    = pouFlag->nGetFlagStatus( HEX );
                //}
                nBase = (INT)lParam;

                // Check if the flag is changed and if so,
                // change the display format.
                if(nBase != CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_HEX))
                {
                    CTxMsgManager::s_TxFlags.vSetFlagStatus(TX_HEX, nBase);
                    // Hide any edit controls if it is visible
                    m_pomTxMsgDetailsView->m_omLctrSigList.SetFocus();

                    if ( m_pomTxMsgBlocksView->m_psMsgBlockList != NULL )
                    {
                        PSMSGBLOCKLIST psMsgBlock =
                            m_pomTxMsgBlocksView->psGetMsgBlockPointer(
                                m_pomTxMsgBlocksView->m_nSelectedMsgBlockIndex,
                                m_pomTxMsgBlocksView->m_psMsgBlockList );

                        if(psMsgBlock != NULL)
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
                    m_pomTxMsgDetailsView->vSetValues(NULL);
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
                                        defSTR_HEX_MODE : defSTR_DEC_MODE;
                    CWnd* pomLabel =
                        m_pomTxMsgDetailsView->GetDlgItem(IDC_STAT_HEADER2);

                    if( pomLabel != NULL )
                    {
                        // Set the text with Hex/Dec mode
                        pomLabel->SetWindowText(omStrText);
                    }
                }

                break;

            case eTXMSGCMD:
                // Get the flag status.
                /*pouFlag = pouGetFlagsPtr();
                if(pouFlag != NULL )
                {
                    bTxON    =
                        static_cast<BOOL> (pouFlag->nGetFlagStatus( SENDMESG ));
                }*/
                bTxON = (BOOL)lParam;

                // If transmission is ON, user should not be able to
                // delete any message or block.
                if(bTxON != CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG))
                {
                    CTxMsgManager::s_TxFlags.vSetFlagStatus(TX_SENDMESG, bTxON);

                    // Get the message count and disable if
                    // there is some message list in the currently
                    // selected block.
                    if ( m_pomTxMsgBlocksView->m_psMsgBlockList != NULL )
                    {
                        PSMSGBLOCKLIST psMsgBlock =
                            m_pomTxMsgBlocksView->psGetMsgBlockPointer(
                                m_pomTxMsgBlocksView->m_nSelectedMsgBlockIndex,
                                m_pomTxMsgBlocksView->m_psMsgBlockList );

                        if(psMsgBlock != NULL)
                        {
                            if( psMsgBlock->m_unMsgCount > 0 )
                            {
                                m_pomTxMsgListView->m_omButtonDeleteAllMsg.
                                EnableWindow(!bTxON);
                            }
                        }

                        // If it is time triggered then time interval edit box
                        if( IS_TIME_TRIGGERED (psMsgBlock->m_ucTrigger) )
                        {
                            m_pomTxMsgBlocksView->m_omEditTrgTimeIntervalVal.
                            EnableWindow(!bTxON);
                        }

                        //If it is key triggered then Enable/Diable All Messages
                        //Check box and Key val edit box
                        if( IS_KEY_TRIGGERED (psMsgBlock->m_ucTrigger) )
                        {
                            /*m_pomTxMsgBlocksView->m_omButtonTxAllFrame.
                                                        EnableWindow(!bTxON);*/
                            m_pomTxMsgBlocksView->m_omComboAllMsgs.EnableWindow(!bTxON);
                            m_pomTxMsgBlocksView->m_omEditTrgKeyVal.
                            EnableWindow(!bTxON);
                        }

                        m_pomTxMsgBlocksView->m_omButtonTimeTrigger.
                        EnableWindow(!bTxON);
                        m_pomTxMsgBlocksView->m_omButtonKeyTrigger.
                        EnableWindow(!bTxON);
                        CButton* pRadioMonoshot = (CButton*)m_pomTxMsgBlocksView->GetDlgItem(IDC_RADIOMONOSHOT);
                        CButton* pRadioCyclic = (CButton*)m_pomTxMsgBlocksView->GetDlgItem(IDC_RADIOCYCLIC);

                        if ((pRadioMonoshot != NULL) && (pRadioCyclic != NULL))
                        {
                            pRadioMonoshot->EnableWindow(!bTxON);
                            pRadioCyclic->EnableWindow(!bTxON);
                        }

                        /*m_pomTxMsgBlocksView->m_omButtonTriggerType.
                                                        EnableWindow(!bTxON);*/
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
                }

                break;

            case eCONNECTCMD:
                // Get the flag status.
                /*pouFlag = pouGetFlagsPtr();
                if(pouFlag != NULL )
                {
                    bConnect = static_cast<BOOL>
                                  (pouFlag->nGetFlagStatus( CONNECTED ));
                }*/
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
                if ( m_pomTxMsgBlocksView->m_psMsgBlockList != NULL )
                {
                    PSMSGBLOCKLIST psMsgBlock =
                        m_pomTxMsgBlocksView->psGetMsgBlockPointer(
                            m_pomTxMsgBlocksView->m_nSelectedMsgBlockIndex,
                            m_pomTxMsgBlocksView->m_psMsgBlockList );

                    if(psMsgBlock != NULL)
                    {
                        m_pomTxMsgListView->m_omLctrMsgList.DeleteAllItems();
                        m_pomTxMsgBlocksView->
                        vDisplayMsgBlockDetails(psMsgBlock);
                    }
                }

                m_pomTxMsgDetailsView->vSetValues(NULL);
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

    if ( !m_bInit )
    {
        // Splitter data structure
        //PSTXMSGSPLITTERDATA psData = &m_sSplitterPosition;
        //////////////////////////////////////////////////
        //WINDOWPLACEMENT omWndSize;
        //GetWindowPlacement( &omWndSize );
        //CTxWndDataStore::ouGetTxWndDataStoreObj().bSetTxData(TX_WINDOW_PLACEMENT, &omWndSize);
        //CTxWndDataStore::ouGetTxWndDataStoreObj().
        //bGetDefaultTXSplitterPostion( omWndSize.rcNormalPosition, (void **)&psData );
        //vSetSplitterPostion();
    }
    else
    {
        m_bInit = FALSE;
    }

    // TODO: Add your message handler code here
}
