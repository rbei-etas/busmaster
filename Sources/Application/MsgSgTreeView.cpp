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
 * \file      MsgSgTreeView.cpp
 * \brief     This file contain definition of all function of
 * \author    Amarnath Shastry
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of
 */

#include "stdafx.h"         // Contains standard include header files.
#include "BUSMASTER.h"    // App class header file
#include "MsgSgTreeView.h"  // Class defintion included here"
#include "MainFrm.h"        // Pointers of this and "CMsgSgDetView" defined here
#include "HashDefines.h"    // All hash defines are here
#include "MsgSignal.h"      // Data structure class header
#include "MsgSgDetView.h"   // uses functions of this class to update right pane
// Dialog class for new message
#include "MessageDetailsDlg.h"

extern CCANMonitorApp theApp;

IMPLEMENT_DYNCREATE(CMsgSgTreeView, CTreeView)

/**
 * \brief Constructor
 *
 * Constructor
 */
CMsgSgTreeView::CMsgSgTreeView()
{
    m_sDbParams             = sm_sDbParams;
    m_omCurrDbName          = m_sDbParams.m_omDBPath;
    m_bIsNewMessage         = FALSE;
    m_omSelectedItemText    = STR_EMPTY;
    m_hTreeItem             = NULL;
}

/**
 * \brief Destructor
 *
 * Destructor
 */
CMsgSgTreeView::~CMsgSgTreeView()
{
}

BEGIN_MESSAGE_MAP(CMsgSgTreeView, CTreeView)
    //{{AFX_MSG_MAP(CMsgSgTreeView)
    ON_WM_CREATE()
    ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
    ON_WM_LBUTTONDOWN()
    ON_NOTIFY_REFLECT(NM_RCLICK, OnRightClickTreeItem)
    ON_WM_RBUTTONDOWN()
    ON_COMMAND(IDM_DELETE_MSG, OnDeleteMessage)
    ON_COMMAND(IDM_NEW_MSG, OnNewMessage)
    ON_NOTIFY_REFLECT(TVN_GETINFOTIP, OnInfoToolTip)
    ON_COMMAND(IDM_EDIT_MSG, OnEditMsg)
    ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**
 * \brief Drawing
 *
 * Drawing
 */
void CMsgSgTreeView::OnDraw(CDC* /*pDC*/)
{
    //CDocument* pDoc = GetDocument();
    // TODO: add draw code here
}

/**
 * \brief Populate Tree
 *
 * This function will populate the tree with database
 * messages.
 */
BOOL CMsgSgTreeView::bPopulateTree()
{
    // Insert the database filename as the root item
    //    CMainFrame* pMainFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
    BOOL bReturnValue = TRUE;
    CMsgSignal* pTempMsgSg = NULL;
    pTempMsgSg = *((CMsgSignal**)m_sDbParams.m_ppvActiveDB);
    // Get reference to the tree control
    CTreeCtrl& om_tree = GetTreeCtrl();
    //
    om_tree.DeleteAllItems();
    om_tree.SetTextColor( BLUE_COLOR );
    CString omStrDatabaseFilename = m_omCurrDbName;

    if ( omStrDatabaseFilename.IsEmpty() )
    {
        bReturnValue = FALSE;
        AfxMessageBox(MSG_DB_NOT_FOUND, MB_OK);
    }
    else
    {
        //        om_tree.DeleteAllItems();
        // Insert database filename
        HTREEITEM hRootItem = om_tree.InsertItem( omStrDatabaseFilename );
        om_tree.SetItemImage(hRootItem, 0, 0);
        CStringList omMessageNames;
        // Clean the list
        omMessageNames.RemoveAll();
        // Get all the database message names
        pTempMsgSg->omStrListGetMessageNamesInactive(omMessageNames);
        POSITION pos = omMessageNames.GetHeadPosition();
        BOOL bSelFirstChild = FALSE;

        // Insert all message names
        while ( pos != NULL )
        {
            CString omStrMsgName = omMessageNames.GetNext(pos);
            HTREEITEM hMsg = om_tree.InsertItem( omStrMsgName, hRootItem  );

            if(bSelFirstChild != TRUE )
            {
                om_tree.SelectItem( hMsg );
                bSelFirstChild = TRUE;
            }

            om_tree.SetItemImage(hMsg, 1, 1);
        }

        // Expand the root
        om_tree.Expand( hRootItem, TVE_EXPAND );

        if(bSelFirstChild != TRUE )
        {
            // Select the root item
            om_tree.SelectItem( hRootItem );
        }
    }

    return (bReturnValue);
}

/**
 * \brief Pre Create Window
 *
 * Window style specified here
 */
BOOL CMsgSgTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style |= WS_VISIBLE          |
                WS_TABSTOP          |
                WS_CHILD            |
                WS_BORDER           |
                TVS_HASBUTTONS      |
                TVS_LINESATROOT     |
                TVS_HASLINES        |
                TVS_DISABLEDRAGDROP |
                TVS_SHOWSELALWAYS   |
                TVS_FULLROWSELECT   |
                TVS_INFOTIP;
    return CTreeView::PreCreateWindow(cs);
}

/**
 * \brief On Create
 *
 * Creates image list and attaches the same to the
 * tree view.
 */
int CMsgSgTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CTreeView::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    m_omImageList.Create( IDR_BMP_MSGSGDB,
                          16,
                          1,
                          WHITE_COLOR);

    if ( m_omImageList != 0 )
    {
        GetTreeCtrl().SetImageList (&m_omImageList, TVSIL_NORMAL);
    }

    /*  m_pomToolTip = new CToolTipCtrl;

        if ( m_pomToolTip != NULL && m_pomToolTip->Create( this ))
        {
            // Associate tooltip to the tree ctrl
            GetTreeCtrl().SetToolTips( m_pomToolTip);
        }
    */
    return 0;
}

/**
 * \brief On Initial Update
 *
 * Called by the frame work to update the view.
 * This again calls vPopulateTree function
 * and sets tree view ptr defined in the mainframe
 */
void CMsgSgTreeView::OnInitialUpdate()
{
    CTreeView::OnInitialUpdate();
    // Update this pointer to the mainframe for future reference
    CMainFrame* pMainFrame = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);
    pMainFrame->podSetMsgSgTreeView( this, m_sDbParams.m_eBus );
    // Populate tree with message names
    bPopulateTree();
}

/**
 * \brief On Item Expanding
 *
 * Called by the frame work when an item is expandede.
 * Displays images appropriately for the nodes and subnodes.
 */
void CMsgSgTreeView::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    // Get reference to tree control
    CTreeCtrl& om_tree = GetTreeCtrl();
    // Get selected item
    HTREEITEM hSelectedItem = pNMTreeView->itemNew.hItem;

    if(pNMTreeView->action == TVE_EXPAND && hSelectedItem != NULL )
    {
        // if the item has child
        if (om_tree.ItemHasChildren(hSelectedItem))
        {
            // Item is a root item
            om_tree.SetItemImage( hSelectedItem, 0, 0 );
        }
        else
        {
            HTREEITEM hChildItem = GetTreeCtrl().GetChildItem( hSelectedItem );
            // Items are messages
            om_tree.SetItemImage( hChildItem, 1, 1 );
        }
    }

    *pResult = 0;
}

/**
 * \brief On Left Button Down
 *
 * Called by the frame work when left mouse button
 * is clicked. Updates the click point
 */
void CMsgSgTreeView::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_omLeftCLickPoint = point;
    CTreeView::OnLButtonDown(nFlags, point);
}

/**
 * \brief Set Message Name
 *
 * Sets the name of the edited item on the tree view
 * If new item is created, then it will be inserted
 * under root.
 */
void CMsgSgTreeView::vSetMessageName(CString omStrMsgName)
{
    CTreeCtrl& om_tree = GetTreeCtrl();
    HTREEITEM hItem = om_tree.GetSelectedItem();

    if ( hItem != NULL )
    {
        // for new message, insert the new message name under root
        if ( hItem == om_tree.GetRootItem() )
        {
            HTREEITEM hNewItem = om_tree.InsertItem( omStrMsgName, hItem );
            // select new message
            om_tree.SelectItem(hNewItem);
            // set its image
            om_tree.SetItemImage( hNewItem, 1, 1);
        }
        else
        {
            om_tree.SetItemText( hItem, omStrMsgName );
        }
    }
}

/**
 * \brief On Right Click Tree Item
 *
 * Displays pop up menu depending on the selected item
 */
void CMsgSgTreeView::OnRightClickTreeItem(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    UINT uFlags = 0;
    CTreeCtrl& om_Tree = GetTreeCtrl();
    CMenu* m_pomContextMenu = new CMenu;
    HTREEITEM hSelectedItem = om_Tree.HitTest( m_omRightClickPoint, &uFlags );
    CMsgSgDetView* pMsgSgDetView = NULL;
    sMESSAGE* pMsg = NULL;
    CMainFrame* pMainFrm = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

    if ( pMainFrm != NULL )
    {
        pMsgSgDetView =
            pMainFrm->podGetMsgSgDetView(m_sDbParams.m_eBus);
    }

    if ( uFlags != TVHT_ONITEMICON && uFlags != TVHT_ONITEMLABEL )
    {
        hSelectedItem = NULL ;
    }
    else
    {
        om_Tree.SelectItem( hSelectedItem );
        // Display the selected message details in the
        // right view
        CMsgSignal* pTempMsgSg = NULL;
        pTempMsgSg = *((CMsgSignal**)m_sDbParams.m_ppvActiveDB);

        if ( !om_Tree.ItemHasChildren(hSelectedItem))// only for message names
        {
            CString omStrMsgName = om_Tree.GetItemText( hSelectedItem );
            m_omSelectedItemText = omStrMsgName;

            // Get message pointer from the data structure
            // corresponding to the message selected name
            if ( pMsgSgDetView != NULL )
            {
                pMsg =
                    pTempMsgSg->psGetMessagePointerInactive( omStrMsgName );
            }

            if ( pMsg != NULL )
            {
                // check if the controls in the form view are hidden
                if ( pMsgSgDetView->bGetControlStatus() == TRUE )//  TRUE - Hidden
                {
                    // Controls are hidden, unhide
                    pMsgSgDetView->vHideControls(SW_SHOW);
                }

                // Fill the msg signals details for this message
                pMsgSgDetView->vDisplayMsgSgInformation( pMsg );
            }
        }
        else
        {
            // Item is root
            // Hide all controls
            pMsgSgDetView->vHideControls(SW_HIDE);
        }

        // Get the parent of selected item to identify
        // whether it is a Message name or the root
        HTREEITEM hParentItem = om_Tree.GetParentItem( hSelectedItem );
        // Load the Menu from the resource
        m_pomContextMenu->DestroyMenu();
        m_pomContextMenu->LoadMenu(IDM_MSG_OPN_MENU);
        CMenu* pomSubMenu = m_pomContextMenu->GetSubMenu(0);

        if ( hParentItem == NULL && m_pomContextMenu != NULL ) // Selected item corresponds to root
        {
            pomSubMenu->EnableMenuItem(IDM_NEW_MSG, MF_BYCOMMAND | MF_ENABLED   );
            pomSubMenu->EnableMenuItem(IDM_DELETE_MSG, MF_BYCOMMAND | MF_DISABLED |MF_GRAYED );
            pomSubMenu->EnableMenuItem(IDM_EDIT_MSG, MF_BYCOMMAND | MF_DISABLED |MF_GRAYED );
        }
        else
        {
            pomSubMenu->EnableMenuItem(IDM_NEW_MSG, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
            pomSubMenu->EnableMenuItem(IDM_DELETE_MSG, MF_BYCOMMAND | MF_ENABLED);
            pomSubMenu->EnableMenuItem(IDM_EDIT_MSG, MF_BYCOMMAND | MF_ENABLED);
        }

        ClientToScreen(&m_omRightClickPoint);
        pomSubMenu->TrackPopupMenu( TPM_LEFTALIGN |TPM_RIGHTBUTTON,
                                    m_omRightClickPoint.x,
                                    m_omRightClickPoint.y,
                                    this,
                                    NULL);
    }

    if (m_pomContextMenu != NULL )
    {
        delete m_pomContextMenu;
    }

    *pResult = 0;
}

/**
 * \brief On Right Button Down
 *
 * Called by the frame work when right mouse button
 * is clicked. Updates the click point
 */
void CMsgSgTreeView::OnRButtonDown(UINT nFlags, CPoint point)
{
    m_omRightClickPoint = point;
    CTreeView::OnRButtonDown(nFlags, point);
}

/**
 * \brief On Delete Message
 *
 * Deletes the selected message from the tree view and
 * from the data structure.
 */
void CMsgSgTreeView::OnDeleteMessage()
{
    CTreeCtrl& om_tree = GetTreeCtrl();
    HTREEITEM hItem = om_tree.GetSelectedItem();

    if ( hItem != NULL )
    {
        int nReturnVal = AfxMessageBox(MSG_DELETE_CONFMN, MB_YESNO|MB_ICONQUESTION);

        if ( nReturnVal == IDYES )
        {
            // Delete the message from the data structure
            CString omStrSelecetedText = om_tree.GetItemText(hItem);
            // Get appropriate msg pinter
            CMsgSignal* pTempMsgSg = NULL;
            //CMsgSgDetView* pMsgSgDetView = NULL;
            pTempMsgSg = *((CMsgSignal**)m_sDbParams.m_ppvActiveDB);

            // delete
            if ( !pTempMsgSg->bDeleteMsg( omStrSelecetedText ))
            {
                AfxMessageBox("Could not delete seleceted message!", MB_OK|MB_ICONINFORMATION);
            }
            else
            {
                m_hTreeItem = NULL;
                // Get previous sibbling item
                HTREEITEM hPrevItem = om_tree.GetPrevSiblingItem( hItem );
                // Remove the item from the tree
                om_tree.DeleteItem( hItem );
                CMainFrame* pMainFrm = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);
                if (pMainFrm != NULL)
                {
                    // Get form view pointer
                    CMsgSgDetView* pMsgSgDetView = pMainFrm->podGetMsgSgDetView(m_sDbParams.m_eBus);

                    if (hPrevItem != NULL )
                    {
                        // Select prev item
                        om_tree.SelectItem( hPrevItem );
                        CString omStrPrev = om_tree.GetItemText( hPrevItem );
                        // Get message pointer
                        sMESSAGE* pMsg = pTempMsgSg->
                                         psGetMessagePointerInactive(omStrPrev);

                        if ( pMsg != NULL && pMsgSgDetView != NULL )
                        {
                            pMsgSgDetView->vDisplayMsgSgInformation( pMsg );
                        }
                    }// no message left
                    else
                    {
                        // Select root item
                        om_tree.SelectItem(om_tree.GetRootItem());

                        // check if the controls in the form view are hidden
                        if ( pMsgSgDetView->bGetControlStatus() == FALSE )//  TRUE - Hidden
                        {
                            // Controls are not hidden, hide
                            pMsgSgDetView->vHideControls(SW_HIDE);
                        }
                    }
                }
            }
        }
    }
}

/**
 * \brief On New Message
 *
 * New message is created and added tio the data structure
 */
void CMsgSgTreeView::OnNewMessage()
{
    vAddEditMessage(FALSE);
}

/**
 * \brief Set Root Item Text
 *
 * Set Root Item Text
 */
void CMsgSgTreeView::vSetRootItemText(CString omStrRootitemText)
{
    CTreeCtrl& om_tree = GetTreeCtrl();

    if ( !omStrRootitemText.IsEmpty())
    {
        om_tree.SetItemText( om_tree.GetRootItem(), omStrRootitemText );
    }
}

/**
 * \brief Set Text Bold
 *
 * Sets the item to bold
 */
void CMsgSgTreeView::vSetTextBold()
{
    // Show the selected text in BOLD
    CTreeCtrl& om_tree = GetTreeCtrl();
    HTREEITEM hItem = om_tree.GetSelectedItem();

    if ( hItem != NULL )
    {
        om_tree.SetItemState( hItem, TVIS_BOLD, TVIS_BOLD );
    }
}

/**
 * \brief Set All Items Normal
 *
 * Set All Items Normal
 */
void CMsgSgTreeView::vSetAllItemsNormal()
{
    CTreeCtrl& om_tree = GetTreeCtrl();
    // Show all of the visible items in NORMAL FONT.
    HTREEITEM hItem = om_tree.GetFirstVisibleItem();

    while (hItem != NULL)
    {
        om_tree.SetItemState(hItem, FALSE, TVIS_BOLD);
        hItem = om_tree.GetNextVisibleItem(hItem);
    }
}

/**
 * \brief On Tool Tip Info
 *
 * Displays message ID's as tool-tips for every message
 */
void CMsgSgTreeView::OnInfoToolTip( NMHDR* /*pNMHDR*/, LRESULT* pResult )
{
    /*  CTreeCtrl& omTree = GetTreeCtrl();

        CToolTipCtrl* omTips = omTree.GetToolTips();

        CToolInfo omInfo;
        CMainFrame* podFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
        CWnd* pomWnd = NULL;

        if (podFrm)
        {
            pomWnd = podFrm->podGetMsgSgTreeView()->GetDC()->GetWindow();
        }

        omTips->GetToolInfo( omInfo, pomWnd);
        omInfo.lpszText ="MY TEST";
        omTips->SetToolInfo( &omInfo);
    */
    pResult = 0;
}

/**
 * \brief On Edit Message
 *
 * For Editing selected message
 */
void CMsgSgTreeView::OnEditMsg()
{
    vAddEditMessage(TRUE);
}

/**
 * \brief Add Edit Message
 *
 * Displays message detials dialog to add new message
 * or edit selecetd message base on the parameter passed.
 */
void CMsgSgTreeView::vAddEditMessage(BOOL bMode)
{
    CMsgSignal* pTempMsgSg = NULL;
    CMsgSgDetView* pMsgSgDetView = NULL;
    pTempMsgSg = *((CMsgSignal**)m_sDbParams.m_ppvActiveDB);
    sMESSAGE* pMsg =    NULL;

    // For Edit mode
    if ( bMode == TRUE )
    {
        CTreeCtrl& omTree = GetTreeCtrl();
        HTREEITEM hItem =
            omTree.GetSelectedItem();

        if (hItem != NULL)
        {
            CString omStr = omTree.GetItemText(hItem);
            pMsg =
                pTempMsgSg->psGetMessagePointerInactive( omStr );
        }
    }

    // End Edit mode
    CMessageDetailsDlg odMsgDlg(m_sDbParams, pMsg,NULL);

    if (odMsgDlg.DoModal() == IDOK )
    {
        vSetMessageName(odMsgDlg.m_omStrMessageName);
        vSetTextBold();
        // Get message pointer
        sMESSAGE* pMsg =
            pTempMsgSg->psGetMessagePointerInactive(odMsgDlg.m_omStrMessageName);
        CMainFrame* pMainFrm = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

        if ( pMainFrm != NULL )
        {
            pMsgSgDetView =
                pMainFrm->podGetMsgSgDetView(m_sDbParams.m_eBus);

            if ( pMsg != NULL && pMsgSgDetView != NULL )
            {
                // check if the controls in the form view are hidden
                if ( pMsgSgDetView->bGetControlStatus() == TRUE )//  TRUE - Hidden
                {
                    // Controls are hidden, unhide
                    pMsgSgDetView->vHideControls(SW_SHOW);
                }

                pMsgSgDetView->vDisplayMsgSgInformation( pMsg );
            }
        }
    }
}

/**
 * \brief On Selection Change
 *
 * Called by the frame work when an item selection is
 * changed. Updates message information on the right pane
 * depending on the selected item
 */
void CMsgSgTreeView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    // TODO: Add your control notification handler code here
    *pResult = 0;
    // check if any data in the form view
    // is missing. If yes,
    // don't let the user to select any item
    // from the tree view
    CMsgSgDetView* pMsgSgDetView = NULL;
    CTreeCtrl& om_tree = GetTreeCtrl();
    sMESSAGE* pMsg = NULL;
    CMsgSignal* pTempMsgSg = NULL;
    pTempMsgSg = *((CMsgSignal**)m_sDbParams.m_ppvActiveDB);
    CMainFrame* pMainFrm = static_cast<CMainFrame*> (AfxGetApp()->m_pMainWnd);

    if ( pMainFrm != NULL )
    {
        pMsgSgDetView =
            pMainFrm->podGetMsgSgDetView(m_sDbParams.m_eBus);

        if ( pMsgSgDetView != NULL)
        {
            if ( pMsgSgDetView->bGetControlStatus() == FALSE )//  TRUE - Hidden
            {
                if (pMsgSgDetView->
                        GetDlgItem(IDC_EDIT_MSG_NAME)->
                        GetWindowTextLength() == 0 )
                {
                    pMsgSgDetView->m_omStrMessageName
                        = m_omSelectedItemText;
                    om_tree.SetItemText( m_hTreeItem,m_omSelectedItemText );
                    pMsgSgDetView->UpdateData(TRUE);
                }

                if ( pMsgSgDetView->
                        GetDlgItem(IDC_EDIT_MSGCODE)->
                        GetWindowTextLength() == 0 )
                {
                    pMsg = pTempMsgSg->psGetMessagePointerInactive(
                               pMsgSgDetView->m_omStrMessageName );

                    if (pMsg != NULL)
                        pMsgSgDetView->m_omStrMessageCode.
                        Format( "%x", pMsg->m_unMessageCode);

                    pMsgSgDetView->UpdateData(TRUE);
                }
            }
        }
    }

    // Get handle to selected item
    HTREEITEM hSelectedItem = pNMTreeView->itemNew.hItem;

    if ( (hSelectedItem != NULL) && (pMsgSgDetView != NULL) )
    {
        if ( !om_tree.ItemHasChildren(hSelectedItem))// only for message names
        {
            CString omStrMsgName = om_tree.GetItemText( hSelectedItem );
            m_omSelectedItemText = omStrMsgName;
            // Get message pointer from the data structure
            // corresponding to the message selected name
            sMESSAGE* pMsg =
                pTempMsgSg->psGetMessagePointerInactive( omStrMsgName );

            if ( pMsg != NULL )
            {
                // check if the controls in the form view are hidden
                if ( pMsgSgDetView->bGetControlStatus() == TRUE )//  TRUE - Hidden
                {
                    // Controls are hidden, unhide
                    pMsgSgDetView->vHideControls(SW_SHOW);
                }

                // Fill the msg signals details for this message
                pMsgSgDetView->vDisplayMsgSgInformation( pMsg );
            }
        }
        else
        {
            // Item is root
            // Hide all controls
            pMsgSgDetView->vHideControls(SW_HIDE);
        }
    }
}
