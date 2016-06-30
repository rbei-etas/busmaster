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
 * \file      SimSysTreeView.cpp
 * \brief     This file contain definition of all function of
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of
 */

#include "NodeSimEx_stdafx.h" // Contains standard include header files.
#include "SimSysTreeView.h" // Class defintion included here
#include "HashDefines.h"    // All hash defines are here
#include "NodeDetailsDlg.h" // Dialog class for new node
#include "SimSysManager.h"
#include "FunctionEditorDoc.h"
#include "ExecuteManager.h"
#include "GlobalObj.h"
#include "Include/BaseDefs.h"
//#include "DIL_Interface_extern.h"
#include "BaseDIL_CAN.h"
#include "ConvertCpp.h"
#include "AppServices.h"

//const BYTE MSGBUF_ADD = 0x01;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TOOLBAR_BUTTON_SIZE 36
#define TOOLBAR_ICON_SIZE 32
#define TOOLBAR_ENABLE_DISABLE_ITEM_INDEX 8 //Index of the "Enable/Disable" Node button in toolbar.
//IMPLEMENT_DYNCREATE(CSimSysTreeView, CTreeView)


/******************************************************************************/
/*  Function Name    :  CSimSysTreeView                                       */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Constructor
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  16.12.2005                                            */
/*  Modifications    :
/******************************************************************************/
CSimSysTreeView::CSimSysTreeView(ETYPE_BUS eBusType, CSimSysManager* pSimSysMgr, CExecuteManager* pExecuteMgr
                                 , CGlobalObj* pGlobalObj, CString omstrBusName)
{
    m_eBus = eBusType; //CSimSysTreeView::sm_eBus;
    m_pomSubMenu = nullptr;
    m_pomContextMenu = nullptr;
    m_hSelectedTreeItem     = nullptr;
    mpSimSysManager = pSimSysMgr;
    mpExecuteMgr = pExecuteMgr;
    mpGlobalObj = pGlobalObj;
    mstrBusName = omstrBusName;
    mpSimSysManager->podSetSimSysTreeView(this);
}

/******************************************************************************/
/*  Function Name    :  ~CSimSysTreeView
/*
/*  Input(s)         :
/*  Output           :
/*  Functionality    :  Destructor
/*  Member of        :  CSimSysTreeView
/*  Friend of        :      -
/*
/*  Author(s)        :  Harika M
/*  Date Created     :  16.12.2005
/*  Modifications    :
/******************************************************************************/
CSimSysTreeView::~CSimSysTreeView()
{
    if( m_pomContextMenu != nullptr)
    {
        delete m_pomContextMenu;
        m_pomContextMenu = nullptr;
    }
    mpSimSysManager->podSetSimSysTreeView(nullptr);

    //free Advanced UI library used to change the icons of the toolbar.
    if (m_hModAdvancedUILib )
    {
        ::FreeLibrary(m_hModAdvancedUILib);
        m_hModAdvancedUILib = nullptr;
    }
}


BEGIN_MESSAGE_MAP(CSimSysTreeView, CTreeView)
    //{{AFX_MSG_MAP(CSimSysTreeView)
    ON_WM_PAINT()
    ON_WM_CREATE()
    ON_WM_LBUTTONDOWN()
    ON_WM_RBUTTONDOWN()

    ON_WM_KEYDOWN()

    ON_NOTIFY_REFLECT(NM_RCLICK, OnTreeViewRightclick)
    ON_NOTIFY_REFLECT(NM_DBLCLK, OnTreeViewDblclick)
    ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
    ON_COMMAND(IDM_ADD_NODE, OnAddNode)
    ON_UPDATE_COMMAND_UI(IDM_ADD_NODE, OnUpdateAddNode)
    ON_COMMAND(IDM_EDIT_NODE, OnEditNode)
    ON_UPDATE_COMMAND_UI(IDM_EDIT_NODE, OnUpdateEditNode)
    ON_COMMAND(IDM_CONFIG_NODE, OnConfigNode)
    ON_UPDATE_COMMAND_UI(IDM_CONFIG_NODE, OnUpdateConfigNode)
    ON_COMMAND(IDM_BUILD_NODE, OnBuildNode)
    ON_UPDATE_COMMAND_UI(IDM_BUILD_NODE, OnUpdateBuildNode)
    ON_COMMAND(IDM_REMOVE_NODE, OnRemoveNode)
    ON_UPDATE_COMMAND_UI(IDM_REMOVE_NODE, OnUpdateRemoveNode)
    ON_NOTIFY_REFLECT(TVN_GETINFOTIP, OnTreeGetInfoTip)
    //}}AFX_MSG_MAP
    //    ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimSysTreeView drawing

void CSimSysTreeView::OnDraw(CDC* )
{
    GetDocument();
    // TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CSimSysTreeView diagnostics

#ifdef _DEBUG
void CSimSysTreeView::AssertValid() const
{
    CTreeView::AssertValid();
}

void CSimSysTreeView::Dump(CDumpContext& dc) const
{
    CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSimSysTreeView message handlers
/******************************************************************************/
/*  Function Name    :  OnCreate                                              */
/*                                                                            */
/*  Input(s)         :  LPCREATESTRUCT lpCreateStruct                         */
/*  Output           :  int                                                   */
/*  Functionality    :  Creates the image list and associates the same
                        to the tree view.
/*  Member of        :  CFnsTreeView                                          */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  06.03.2002                                            */
/*  Modifications    :
/******************************************************************************/
int CSimSysTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    // Base class call
    if (CTreeView::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    // Create image list
    m_omImageList.Create( IDB_NODE_SIMULATION_TREEVIEW,
                          16,
                          1,
                          WHITE_COLOR);

    // Associate the image list to the tree
    if ( m_omImageList != 0 )
    {
        GetTreeCtrl().SetImageList (&m_omImageList, TVSIL_NORMAL);
    }

    return 0;
}

CNodeDetailsDlg* CSimSysTreeView::getNodeDetailsDlgObj(CGlobalObj* mpGlobalObj, CSimSysManager* mpSimSysManager, sNODEINFO* psNodeInfo)
{
    CNodeDetailsDlg* odNodeDlg = new CNodeDetailsDlg(m_eBus, mpGlobalObj, mpSimSysManager, psNodeInfo, nullptr);
    return odNodeDlg;
}

/******************************************************************************/
/*  Function Name    :  bPopulateTree
/*
/*  Input(s)         :
/*  Output           :
/*  Functionality    :  This function will populate the tree with simulated
/*                      systems, nodes and the dlls under it.
/*  Member of        :  CSimSysTreeView
/*  Friend of        :      -
/*
/*  Author(s)        :  Anish Kr.
/*  Date Created     :
/*  Modification     : Robin G.K.
                       23.10.14, Removed sSIMSYSINFO struct.
/******************************************************************************/

BOOL CSimSysTreeView::bPopulateTree()
{
    BOOL bReturnValue = TRUE;
    // Get reference to the tree control
    CTreeCtrl& om_tree = GetTreeCtrl();
    CString omSelectedItem = "";
    omSelectedItem = om_tree.GetItemText(om_tree.GetSelectedItem());

    om_tree.DeleteAllItems();
    // Insert root text
    //om_tree.SetTextColor( BLUE_COLOR );

    HTREEITEM hRootItem = om_tree.InsertItem( mstrBusName,0,0,TVI_ROOT,TVI_LAST);
    om_tree.SelectItem(hRootItem);

    CSimSysNodeInfo* pSimSysNodeInfo =
        mpSimSysManager->pomGetSimSysNodeInfo();

    if (pSimSysNodeInfo != nullptr)
    {
        PSNODELIST pNodeInfo = pSimSysNodeInfo->m_psNodesList;
        while(pNodeInfo != nullptr)
        {
            CString omStrNodeName = pNodeInfo->m_sNodeInfo->m_omStrNodeName;
            HTREEITEM hNode;
            switch(pNodeInfo->m_sNodeInfo->m_eNodeState)
            {
                case NODE_REQ_CLEAN_BUILT:
                case NODE_NOT_BUILT:
                    hNode = om_tree.InsertItem(omStrNodeName,1,1,hRootItem,TVI_LAST);
                    break;
                case NODE_BUILD_SUCCESS:
                    hNode = om_tree.InsertItem(omStrNodeName,2,2,hRootItem,TVI_LAST);
                    break;
                case NODE_BUILD_FAIL:
                    hNode = om_tree.InsertItem(omStrNodeName,3,3,hRootItem,TVI_LAST);
                    break;
            }

            if(omStrNodeName == omSelectedItem)
            {
                om_tree.SelectItem(hNode);
            }
            //Set Check on node based on enabled/disabled.
            om_tree.SetCheck(hNode,FALSE);
            //Set the text of Node name to bold if it is enabled.
            if(pNodeInfo->m_sNodeInfo->m_bIsNodeEnabled)
            {
                //om_tree.SetItemState( hNode, TVIS_BOLD, TVIS_BOLD );
                om_tree.SetCheck(hNode,TRUE);
                om_tree.SetCheck(hRootItem,TRUE);
            }
            CNodeDetailsDlg* odNodeDlg = getNodeDetailsDlgObj(mpGlobalObj, mpSimSysManager, (pNodeInfo->m_sNodeInfo));
            //CNodeDetailsDlg odNodeDlg(m_eBus, mpGlobalObj, mpSimSysManager, (pNodeInfo->m_sNodeInfo), nullptr);
            pNodeInfo = pNodeInfo->m_psNextNode;
        }

        om_tree.Expand( hRootItem, TVE_EXPAND );


    }
    //paintTree();
    return (bReturnValue);
}
/**************************************************************************************
    Function Name    :  bSetNodeState
    Input(s)         :  const PSNODEINFO psNodeInfo, eNODE_STATE eNodeState
    Output           :  bool
    Functionality    :  Set Node state to NODE_NOT_BUILT/NODE_BUILD_SUCCESS/NODE_BUILD_FAIL
                        and corresponding icons.
    Member of        :  CSimSysTreeView
    Author(s)        :  Robin G.K.
    Date Created     :  13.10.2015
    Requirement ID   :
***************************************************************************************/
bool CSimSysTreeView::bSetNodeState(const PSNODEINFO psNodeInfo, eNODE_STATE eNodeState)
{
    bool bReturn = false;
    if(nullptr!=psNodeInfo)
    {
        CString omStrNodeName = psNodeInfo->m_omStrNodeName;
        HTREEITEM hNode = FindHTreeItem(omStrNodeName);
        CTreeCtrl& om_tree = GetTreeCtrl();
        bReturn = true;
        switch(eNodeState)
        {
            case NODE_REQ_CLEAN_BUILT:
            case NODE_NOT_BUILT:
                psNodeInfo->m_eNodeState = NODE_NOT_BUILT;
                om_tree.SetItemImage(hNode,1,1);
                break;
            case NODE_BUILD_SUCCESS:
                psNodeInfo->m_eNodeState = NODE_BUILD_SUCCESS;
                om_tree.SetItemImage(hNode,2,2);
                break;
            case NODE_BUILD_FAIL:
                psNodeInfo->m_eNodeState = NODE_BUILD_FAIL;
                om_tree.SetItemImage(hNode,3,3);
                break;
        }
    }
    return bReturn;
}

/******************************************************************************/
/*  Function Name    :  PreCreateWindow                                       */
/*                                                                            */
/*  Input(s)         :  CREATESTRUCT& cs                                      */
/*  Output           :                                                        */
/*  Functionality    :  Window style specified here                           */
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  19.12.2005                                            */
/*  Modifications    :
/******************************************************************************/

BOOL CSimSysTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style |= WS_VISIBLE          |
                WS_TABSTOP          |
                WS_CHILD            |
                WS_BORDER           |
                TVS_HASBUTTONS      |
                TVS_LINESATROOT     |
                TVS_HASLINES        |
                TVS_DISABLEDRAGDROP |
                TVS_CHECKBOXES      |
                TVS_SHOWSELALWAYS   |
                TVS_FULLROWSELECT   |
                TVS_INFOTIP ;
    cs.dwExStyle = WS_EX_CONTEXTHELP;

    return CTreeView::PreCreateWindow(cs);
}

/******************************************************************************/
/*  Function Name    :  OnLButtonDown                                         */
/*                                                                            */
/*  Input(s)         :  UINT nFlags, CPoint point                             */
/*  Output           :                                                        */
/*  Functionality    :  Called by the frame work when left mouse button
                        is clicked. Updates the click point
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  19.12.2005                                            */
/*  Modifications    :
/******************************************************************************/
void CSimSysTreeView::OnLButtonDown(UINT nFlags, CPoint point)
{
    UINT uFlags = 0;
    CTreeCtrl& omTree = GetTreeCtrl();
    HTREEITEM hSelItem = omTree.HitTest( point, &uFlags );
    if((hSelItem != nullptr) && (TVHT_ONITEMSTATEICON & uFlags))
    {
        BOOL bCheck = omTree.GetCheck(m_hSelectedTreeItem);
        if(mpExecuteMgr->m_eBusStatus == BUS_DISCONNECTED)
        {
            m_hSelectedTreeItem = hSelItem;
            OnEnableDisableNode();
        }
        /*else
        {
            omTree.SetCheck(hSelItem,bCheck);
        }*/
    }
    else
    {
        CTreeView::OnLButtonDown(nFlags, point);
    }

}
/******************************************************************************/
/*  Function Name    :  OnRButtonDown                                         */
/*                                                                            */
/*  Input(s)         :  UINT nFlags, CPoint point                             */
/*  Output           :                                                        */
/*  Functionality    :  Called by the frame work when right mouse button
                        is clicked. Updates the click point
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  19.12.2005                                            */
/*  Modifications    :
/******************************************************************************/
void CSimSysTreeView::OnRButtonDown(UINT nFlags, CPoint point)
{
    m_omRightClickPoint = point;
    CTreeView::OnRButtonDown(nFlags, point);
}

/******************************************************************************/
/*  Function Name    :  vDisplayRootMenu
/*
/*  Input(s)         :
/*  Output           :
/*  Functionality    :  Display Root Menu of <Protocol>_BUS
/*  Member of        :  CSimSysTreeView
/*  Friend of        :      -
/*
/*  Author(s)        :  Robin G.K.
/*  Date Created     :  23.10.14
/*  Modifications    :
/******************************************************************************/
void CSimSysTreeView::vDisplayRootMenu()
{

    CTreeCtrl& om_Tree = GetTreeCtrl();
    if(m_pomContextMenu != nullptr)
    {
        m_pomContextMenu->DestroyMenu();
        m_pomContextMenu->LoadMenu(IDM_ROOT_MENU);
        m_pomSubMenu = m_pomContextMenu->GetSubMenu(0);
    }

    if(m_pomSubMenu != nullptr)
    {
        //Add Node is enabled if Bus is disconnected.
        if ( mpExecuteMgr->m_eBusStatus == BUS_CONNECTED )
        {
            m_pomSubMenu->EnableMenuItem(IDM_ADD_NODE,  MF_BYCOMMAND | MF_DISABLED | MF_GRAYED );
        }
        else
        {
            m_pomSubMenu->EnableMenuItem(IDM_ADD_NODE, MF_BYCOMMAND | MF_ENABLED );
        }

        //Remove All, Build All, Enable All, Disable All are initialised to disabled.
        m_pomSubMenu->EnableMenuItem(IDM_BUILD_NODE, MF_BYCOMMAND |
                                     MF_DISABLED | MF_GRAYED );
        m_pomSubMenu->EnableMenuItem(IDM_REMOVE_NODE, MF_BYCOMMAND |
                                     MF_DISABLED | MF_GRAYED );
        //if Bus is disconnected and number of nodes is > 1
        if(om_Tree.GetCount() > 1 && mpExecuteMgr->m_eBusStatus == BUS_DISCONNECTED)
        {

            CSimSysNodeInfo* pSimSysNodeInfo =
                mpSimSysManager->pomGetSimSysNodeInfo();
            //Enable "Disable All" if valid
            //Enable "Build All" if valid
            if(pSimSysNodeInfo->bIsBuildAllNodesValid())
            {
                m_pomSubMenu->EnableMenuItem(IDM_BUILD_NODE, MF_BYCOMMAND |
                                             MF_ENABLED );
            }
            m_pomSubMenu->EnableMenuItem(IDM_REMOVE_NODE, MF_BYCOMMAND |
                                         MF_ENABLED );
        }


        ClientToScreen(&m_omRightClickPoint);
        m_pomSubMenu->TrackPopupMenu( TPM_LEFTALIGN |TPM_RIGHTBUTTON,
                                      m_omRightClickPoint.x,
                                      m_omRightClickPoint.y,
                                      this,
                                      nullptr);
    }
}

/******************************************************************************/
/*  Function Name    :  vDisplayNodeMenu
/*
/*  Input(s)         :
/*  Output           :
/*  Functionality    :  Display menu of the Node
/*  Member of        :  CSimSysTreeView
/*  Friend of        :      -
/*
/*  Author(s)        :  Robin G.K.
/*  Date Created     :  23.10.14
/*  Modifications    :
/******************************************************************************/

void CSimSysTreeView::vDisplayNodeMenu()
{
    PSNODEINFO psNodeInfo = GetNodeInfo(m_hSelectedTreeItem);
    if(m_pomContextMenu != nullptr)
    {
        m_pomContextMenu->DestroyMenu();
        m_pomContextMenu->LoadMenu(IDM_NODE_MENU);
        m_pomSubMenu = m_pomContextMenu->GetSubMenu(0);
    }

    if(m_pomSubMenu != nullptr)
    {
        //Initialise "Edit", "Build", "Enable/Disable", "Configure", "Remove" node to Enabled state.
        m_pomSubMenu->EnableMenuItem(IDM_EDIT_NODE, MF_BYCOMMAND |
                                     MF_ENABLED );
        m_pomSubMenu->EnableMenuItem(IDM_BUILD_NODE, MF_BYCOMMAND |
                                     MF_ENABLED );
        m_pomSubMenu->EnableMenuItem(IDM_CONFIG_NODE, MF_BYCOMMAND |
                                     MF_ENABLED );
        m_pomSubMenu->EnableMenuItem(IDM_REMOVE_NODE, MF_BYCOMMAND |
                                     MF_ENABLED );

        //if there is no c or .cpp file disable "Edit" and "Build" node
        if(psNodeInfo != nullptr && psNodeInfo->m_omStrCFileName.IsEmpty())
        {
            m_pomSubMenu->EnableMenuItem(IDM_EDIT_NODE, MF_BYCOMMAND |
                                         MF_DISABLED | MF_GRAYED );

            m_pomSubMenu->EnableMenuItem(IDM_BUILD_NODE, MF_BYCOMMAND |
                                         MF_DISABLED | MF_GRAYED );
        }

        // if bus is connnected disable "build","Enable/Disable", "Configure", "Remove" node.
        if( mpExecuteMgr->m_eBusStatus == BUS_CONNECTED)
        {
            m_pomSubMenu->EnableMenuItem(IDM_BUILD_NODE, MF_BYCOMMAND |
                                         MF_DISABLED | MF_GRAYED );
            m_pomSubMenu->EnableMenuItem(IDM_CONFIG_NODE, MF_BYCOMMAND |
                                         MF_DISABLED | MF_GRAYED );
            m_pomSubMenu->EnableMenuItem(IDM_REMOVE_NODE, MF_BYCOMMAND |
                                         MF_DISABLED | MF_GRAYED );
        }

        ClientToScreen(&m_omRightClickPoint);
        m_pomSubMenu->TrackPopupMenu( TPM_LEFTALIGN |TPM_RIGHTBUTTON,
                                      m_omRightClickPoint.x,
                                      m_omRightClickPoint.y,
                                      this,
                                      nullptr);
    }
}
/******************************************************************************/
/*  Function Name    :  OnTreeViewRightclick
/*
/*  Input(s)         :  NMHDR* pNMHDR, LRESULT* pResult
/*  Output           :
/*  Functionality    :  Displays pop up menu depending on the selected item

/*  Member of        :  CSimSysTreeView
/*  Friend of        :      -
/*
/*  Author(s)        :
/*  Date Created     :
/*  Modification     :  Robin G.K.
                        23.10.14, Restructured.
/******************************************************************************/
void CSimSysTreeView::OnTreeViewRightclick(NMHDR* , LRESULT* pResult)
{
    UINT uFlags = 0;
    CTreeCtrl& omTree = GetTreeCtrl();

    if(omTree.HitTest( m_omRightClickPoint, &uFlags ) != nullptr)
    {
        m_hSelectedTreeItem = omTree.HitTest( m_omRightClickPoint, &uFlags );
        if(m_pomContextMenu == nullptr )
        {
            m_pomContextMenu = new CMenu;
        }

        if(m_hSelectedTreeItem == omTree.GetRootItem())
        {
            vDisplayRootMenu();
        }
        else
        {
            vDisplayNodeMenu();
        }
    }
    *pResult = 0;
}


/******************************************************************************/
/*  Function Name    :  OnSelchanged
/*
/*  Input(s)         :  NMHDR* pNMHDR, LRESULT* pResult
/*  Output           :
/*  Functionality    :  Updates the toolbar icons based on the selected Node.

/*  Member of        :  CSimSysTreeView
/*  Friend of        :      -
/*
/*  Author(s)        :
/*  Date Created     :
/*  Modification     :  Robin G.K.
                        23.10.14, Restructured.
/******************************************************************************/
void CSimSysTreeView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

    CTreeCtrl& omTree = GetTreeCtrl();

    m_hSelectedTreeItem = pNMTreeView->itemNew.hItem;

    *pResult = 0;
}
/******************************************************************************/
/*  Function Name    :  OnTreeViewDblclick
/*
/*  Input(s)         :  NMHDR* pNMHDR, LRESULT* pResult
/*  Output           :
/*  Functionality    :  Opens Function editor if .cpp file is associated with a
                        node else opens Node Configuration Dialog.
/*  Member of        :  CSimSysTreeView
/*  Friend of        :      -
/*
/*  Author(s)        :  Robin G.K.
/*  Date Created     :  23.10.14
/*  Modification     :
/******************************************************************************/
void CSimSysTreeView::OnTreeViewDblclick(NMHDR* , LRESULT* pResult)
{
    OnEditNode();
}


/******************************************************************************/
/*  Function Name    :  OnInitialUpdate                                       */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the frame work to update the view.
                        This again calls bPopulateTree function
                        and sets tree view ptr defined in the mainframe
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  19.12.2005                                            */
/*  Modifications    :
/******************************************************************************/
void CSimSysTreeView::OnInitialUpdate()
{
    CTreeView::OnInitialUpdate();
}

/******************************************************************************/
/*  Function Name    :  OnRemoveNode                                          */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Removes a node from the tree view and Node List.
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Robin G.K.                                            */
/*  Date Created     :  23.10.14                                              */
/*  Modification     :
/******************************************************************************/
void CSimSysTreeView::OnRemoveNode()
{
    CSimSysNodeInfo* pSimSysNodeInfo =
        mpSimSysManager->pomGetSimSysNodeInfo();

    CTreeCtrl& omTree = GetTreeCtrl();

    if(pSimSysNodeInfo != nullptr)
    {
        HTREEITEM m_hRootItem = omTree.GetRootItem();
        if (m_hSelectedTreeItem == m_hRootItem)
        {
            if (TRUE == omTree.ItemHasChildren(m_hRootItem))
            {
                int nReturnVal = AfxMessageBox(ALL_NODE_DELETE_CONFMN, MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2);
                if (nReturnVal == IDYES)
                {

                    pSimSysNodeInfo->bDeleteNodeFromSimSys("");
                }
                omTree.DeleteAllItems();
            }
        }
        else
        {
            int nReturnVal = AfxMessageBox(NODE_DELETE_CONFMN, MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2);
            if (nReturnVal == IDYES)
            {
                if(pSimSysNodeInfo->bDeleteNodeFromSimSys(omTree.GetItemText(m_hSelectedTreeItem)))
                {
                    omTree.DeleteItem(m_hSelectedTreeItem);
                }
            }
        }
    }
    bPopulateTree();
}
/******************************************************************************/
/*  Function Name    :  OnConfigNode                                          */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  For Configuring a Node.
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Robin G.K.                                            */
/*  Date Created     :  23.10.14                                              */
/*  Modifications    :
/*
*****************************************************************************/
void CSimSysTreeView::OnConfigNode()
{
    PSNODEINFO psNodeInfo = GetNodeInfo(m_hSelectedTreeItem);
    if (psNodeInfo != nullptr)
    {
        //CNodeDetailsDlg odNodeDlg(m_eBus, mpGlobalObj, mpSimSysManager, psNodeInfo , nullptr);
        CNodeDetailsDlg* odNodeDlg = getNodeDetailsDlgObj(mpGlobalObj, mpSimSysManager, psNodeInfo);
        odNodeDlg->m_bEdit = TRUE;
        if(odNodeDlg->DoModal() == IDOK)
        {
            if (odNodeDlg->m_bIsNodeModified == TRUE)
            {
                CTreeCtrl& omTree = GetTreeCtrl();
                omTree.SetItemText(m_hSelectedTreeItem, psNodeInfo->m_omStrNodeName);
            }

            if(psNodeInfo->m_eNodeFileType == NODE_FILE_DLL)
            {
                if (psNodeInfo->m_omStrDllName.IsEmpty() == false)
                {
                    CConvertCpp objConvert;
                    if (false == objConvert.bCheckDllVersion(std::string(psNodeInfo->m_omStrDllName)))
                    {
                        char ch[500];
                        sprintf(ch, "dll file %s loaded is created using old Node Simulation version. please recompile .cpp file.", psNodeInfo->m_omStrDllName);
                        gbSendStrToTrace(ch);
                    }
                }
                psNodeInfo->m_eNodeState = NODE_BUILD_SUCCESS;
            }
            else if(psNodeInfo->m_eNodeFileType == NODE_FILE_C_CPP)
            {
                psNodeInfo->m_eNodeState = NODE_NOT_BUILT;
            }
            bPopulateTree();
        }

    }
}







/******************************************************************************/
/*  Function Name    :  OnAddnode                                             */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  New node is created and added to the data structure
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  27.12.2005                                            */
/*  Modification     :
/******************************************************************************/
void CSimSysTreeView::OnAddNode()
{
    CTreeCtrl& omTree = GetTreeCtrl();
    CSimSysNodeInfo* pSimSysNodeInfo =
        mpSimSysManager->pomGetSimSysNodeInfo();
    if(omTree.GetCount() > 15)
    {
        AfxMessageBox("Maximum Supported Nodes is 15", MB_ICONEXCLAMATION | MB_OK );
        return;
    }
    PSNODELIST pNodeList = new sNODELIST(m_eBus, mpGlobalObj);
    sNODEINFO* ousNodeInfo = new sNODEINFO(m_eBus, mpGlobalObj);
    //PSNODEINFO ousNodeInfo = new sNODEINFO(m_eBus, mpGlobalObj);
    pNodeList->vCreateNodeInfoPtr(ousNodeInfo);
    PSNODEINFO psNodeInfo = (pNodeList->m_sNodeInfo);
    if (psNodeInfo != nullptr)
    {
        //CNodeDetailsDlg odNodeDlg(m_eBus, mpGlobalObj, mpSimSysManager, psNodeInfo, nullptr);
        CNodeDetailsDlg* odNodeDlg = getNodeDetailsDlgObj(mpGlobalObj, mpSimSysManager, psNodeInfo);
        if ( odNodeDlg->DoModal() == IDOK )
        {
            pSimSysNodeInfo->vAddNodeToList(pNodeList);
            mpGlobalObj->RegisterNodeToDIL(TRUE, psNodeInfo);

            //Adding Node.
            HTREEITEM hNode = omTree.InsertItem(psNodeInfo->m_omStrNodeName,1,1,omTree.GetRootItem(),TVI_LAST);
            omTree.SelectItem(hNode);
            /*omTree.SetCheck(hNode,TRUE);
            omTree.SetCheck(omTree.GetRootItem(),TRUE);*/
            if(psNodeInfo->m_eNodeFileType == NODE_FILE_DLL)
            {
                psNodeInfo->m_eNodeState = NODE_BUILD_SUCCESS;
            }
            else
            {
                psNodeInfo->m_eNodeState = NODE_NOT_BUILT;
                mpSimSysManager->GenerateMakeFile(psNodeInfo->m_omStrCFileName.GetBuffer(0));
            }
            psNodeInfo->m_bIsNodeEnabled = true;
            bPopulateTree();
        }
        else
        {
            delete pNodeList;
        }
    }

}





/******************************************************************************/
/*  Function Name    :  OnEditNode
/*
/*  Input(s)         :
/*  Output           :
/*  Functionality    :  Opens Function Editor
/*  Member of        :  CSimSysTreeView
/*  Friend of        :      -
/*  Author(s)        :  Robin G.K.
/*  Date Created     :  23.10.14
/******************************************************************************/

void CSimSysTreeView::OnEditNode()
{
    PSNODEINFO psNodeInfo = GetNodeInfo(m_hSelectedTreeItem);
    if(m_eBus == J1939)
    {
        if(mpGlobalObj->bJ1939Activated == false)
        {
            AfxMessageBox("J1939 is not Activated, activate it and then try.");
            return;
        }
    }

    if(psNodeInfo != nullptr)
    {
        if(psNodeInfo->m_omStrCFileName.IsEmpty() && mpExecuteMgr->m_eBusStatus == BUS_DISCONNECTED)
        {
            OnConfigNode(); // Opens Node config dialog
        }
        else if(!psNodeInfo->m_omStrCFileName.IsEmpty() && psNodeInfo->m_eNodeFileType == NODE_FILE_C_CPP)
        {
            CFunctionEditorDoc* pDoc = mpGlobalObj->pGetDocPtrOfFile(psNodeInfo->m_omStrCFileName);
            if (pDoc != nullptr)
            {
                //If file is opened then get its frame and activate it
                {
                    POSITION pos = pDoc->GetFirstViewPosition();
                    if (pos)
                    {
                        pDoc->GetNextView(pos)->GetParentFrame()->ActivateFrame();
                    }
                }
            }
            else
            {
                //If file is not opened then open it
                if ( !mpGlobalObj->bOpenFunctioneditorfile(psNodeInfo->m_omStrCFileName) )
                {
                    AfxMessageBox("Specified filename not found!",
                                  MB_OK|MB_ICONINFORMATION);
                }
            }
        }
    }
}
/******************************************************************************/
/*  Function Name    :  OnEnableDisableNode
/*
/*  Input(s)         :
/*  Output           :
/*  Functionality    :  Changes Toolbar enable/disable icon and highlights the
                        the node name based on m_bIsNodeEnabled flag.
/*  Member of        :  CSimSysTreeView
/*  Friend of        :      -
/*  Author(s)        :  Robin G.K.
/*  Date Created     :  23.10.14
/******************************************************************************/
void CSimSysTreeView::OnEnableDisableNode()
{
    PSNODEINFO psNodeInfo = GetNodeInfo(m_hSelectedTreeItem);
    CTreeCtrl& omTree = GetTreeCtrl();
    if(omTree.GetItemText(m_hSelectedTreeItem).CompareNoCase(mstrBusName) == 0)
    {
        CSimSysNodeInfo* pSimSysNodeInfo =
            mpSimSysManager->pomGetSimSysNodeInfo();
        if(pSimSysNodeInfo != nullptr)
        {
            PSNODELIST pTempNode = pSimSysNodeInfo->m_psNodesList;
            bool bCheck = omTree.GetCheck(m_hSelectedTreeItem);
            while(pTempNode != nullptr)
            {
                pTempNode->m_sNodeInfo->m_bIsNodeEnabled = !bCheck;
                pTempNode = pTempNode->m_psNextNode;
            }
        }
    }
    else if(psNodeInfo != nullptr)
    {
        psNodeInfo->m_bIsNodeEnabled = !psNodeInfo->m_bIsNodeEnabled;
        vModifyToolbarIcon(TOOLBAR_ENABLE_DISABLE_ITEM_INDEX, psNodeInfo->m_bIsNodeEnabled, IDI_ICON_NODE_ENABLED, IDI_ICON_NODE_DISABLED);
    }
    bPopulateTree();
}

/******************************************************************************/
/*  Function Name    :  OnBuildNode
/*  Input(s)         :  -
/*  Output           :  -
/*  Functionality    :  Builds the selected node
/*  Member of        :  CSimSysTreeView
/*  Friend of        :      -
/*
/*  Author(s)        :  Robin G.K.
/*  Date Created     :  23.10.14
/*****************************************************************************/
void CSimSysTreeView::OnBuildNode()
{
    CTreeCtrl& omTree = GetTreeCtrl();
    if(m_eBus == J1939)
    {
        if(mpGlobalObj->bJ1939Activated == false)
        {
            AfxMessageBox("J1939 is not Activated, activate it and then try.");
            return;
        }
    }
    if(m_hSelectedTreeItem == omTree.GetRootItem())
    {
        CSimSysNodeInfo* pSimSysNodeInfo =
            mpSimSysManager->pomGetSimSysNodeInfo();
        //Save All open Documents
        if(pSimSysNodeInfo != nullptr)
        {
            PSNODELIST pTempNode = pSimSysNodeInfo->m_psNodesList;
            while(pTempNode != nullptr)
            {
                CFunctionEditorDoc* pDoc = mpGlobalObj->pGetDocPtrOfFile(pTempNode->m_sNodeInfo->m_omStrCFileName);
                if (pDoc)
                {

                    pDoc->SaveModified();
                }
                pTempNode = pTempNode->m_psNextNode;
            }
        }

        //Build all nodes.
        mpExecuteMgr->bDLLBuildAll();
    }
    else
    {
        PSNODEINFO psNodeInfo = GetNodeInfo(m_hSelectedTreeItem);

        BOOL bSuccess = FALSE;

        if( psNodeInfo != nullptr )
        {
            //If file is opened get its doc pointer and save it before building
            CFunctionEditorDoc* pDoc = mpGlobalObj->pGetDocPtrOfFile(psNodeInfo->m_omStrCFileName);
            if (pDoc)
            {
                pDoc->SaveModified();
            }
            //Clear Output Window.
            mpExecuteMgr->vClearOutputWnd();
            //Build the file
            mpExecuteMgr->bExecuteDllBuild(psNodeInfo,TRUE);
        }
    }
    bPopulateTree();
}
/******************************************************************************/
/*  Function Name    :  OnTreeGetInfoTip
/*  Input(s)         :  NMHDR *pNMHDR, LRESULT *pResult
/*  Output           :  -
/*  Functionality    :  Shows InfoTip of the File and Dll associated with the node.
/*  Member of        :  CSimSysTreeView
/*  Friend of        :      -
/*  Author(s)        :  Robin G.K.
/*  Date Created     :  23.10.14
/*****************************************************************************/
void CSimSysTreeView::OnTreeGetInfoTip(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMTVGETINFOTIP pGetInfoTip = (LPNMTVGETINFOTIP)pNMHDR;
    CTreeCtrl& om_tree = GetTreeCtrl();
    CString omStrItemTxt;
    CSimSysNodeInfo* pSimSysNodeInfo =
        mpSimSysManager->pomGetSimSysNodeInfo();
    *pResult = -1;
    if(pSimSysNodeInfo != nullptr && pGetInfoTip != nullptr)
    {
        PSNODEINFO psNodeInfo = pSimSysNodeInfo->psGetSimSysNodePointer(om_tree.GetItemText(pGetInfoTip->hItem));
        if(psNodeInfo != nullptr)
        {
            if(psNodeInfo->m_eNodeFileType == NODE_FILE_DLL)
            {
                omStrItemTxt = "File : "+psNodeInfo->m_omStrDllName;
            }
            else
            {
                omStrItemTxt = "File : "+psNodeInfo->m_omStrCFileName;
            }
            strcpy(pGetInfoTip->pszText, omStrItemTxt);
            *pResult = 0;
        }
    }

}
/******************************************************************************/
/*  Function Name    :  OnUpdateAddNode
/*  Input(s)         :  CCmdUI* pCmdUI
/*  Output           :  -
/*  Functionality    :  Called by the framework.
                        Enables/Disables "Add" node button in Toolbar
/*  Member of        :  CSimSysTreeView
/*  Friend of        :      -
/*  Author(s)        :  Robin G.K.
/*  Date Created     :  23.10.14
/*****************************************************************************/
void CSimSysTreeView::OnUpdateAddNode(CCmdUI* pCmdUI)
{
    if (mpExecuteMgr->m_eBusStatus == BUS_DISCONNECTED )
    {
        pCmdUI->Enable(TRUE);
    }
    else
    {
        pCmdUI->Enable(FALSE);
    }
}


/******************************************************************************/
/*  Function Name    :  OnUpdateEditNode
/*  Input(s)         :  CCmdUI* pCmdUI
/*  Output           :  -
/*  Functionality    :  Called by the framework.
                        Enables/Disables "Edit" Node button in Toolbar
/*  Member of        :  CSimSysTreeView
/*  Friend of        :      -
/*  Author(s)        :  Robin G.K.
/*  Date Created     :  23.10.14
/*****************************************************************************/
void CSimSysTreeView::OnUpdateEditNode(CCmdUI* pCmdUI)
{
    CTreeCtrl& omTree = GetTreeCtrl();
    PSNODEINFO psNodeInfo = GetNodeInfo(m_hSelectedTreeItem);

    if(m_hSelectedTreeItem == omTree.GetRootItem() || (psNodeInfo!=nullptr && psNodeInfo->m_omStrCFileName.IsEmpty())
            || (psNodeInfo!=nullptr && psNodeInfo->m_eNodeFileType == NODE_FILE_DLL))
    {
        pCmdUI->Enable(FALSE);
    }
    else
    {
        pCmdUI->Enable(TRUE);
    }
}
/******************************************************************************/
/*  Function Name    :  OnUpdateConfigNode
/*  Input(s)         :  CCmdUI* pCmdUI
/*  Output           :  -
/*  Functionality    :  Called by the framework.
                        Enables/Disables "Configure" node button in Toolbar
/*  Member of        :  CSimSysTreeView
/*  Friend of        :      -
/*  Author(s)        :  Robin G.K.
/*  Date Created     :  23.10.14
/*****************************************************************************/
void CSimSysTreeView::OnUpdateConfigNode(CCmdUI* pCmdUI)
{
    PSNODEINFO psNodeInfo = GetNodeInfo(m_hSelectedTreeItem);
    if(psNodeInfo == nullptr || mpExecuteMgr->m_eBusStatus == BUS_CONNECTED)
    {
        pCmdUI->Enable(FALSE);
    }
    else
    {
        pCmdUI->Enable(TRUE);
    }
}

/******************************************************************************/
/*  Function Name    :  OnUpdateBuildNode
/*  Input(s)         :  CCmdUI* pCmdUI
/*  Output           :  -
/*  Functionality    :  Called by the framework.
                        Enables/Disables "Build" node button in Toolbar
/*  Member of        :  CSimSysTreeView
/*  Friend of        :      -
/*  Author(s)        :  Robin G.K.
/*  Date Created     :  23.10.14
/*****************************************************************************/
void CSimSysTreeView::OnUpdateBuildNode(CCmdUI* pCmdUI)
{
    CTreeCtrl& omTree = GetTreeCtrl();


    CSimSysNodeInfo* pSimSysNodeInfo =
        mpSimSysManager->pomGetSimSysNodeInfo();



    PSNODEINFO psNodeInfo = GetNodeInfo(m_hSelectedTreeItem);
    HTREEITEM hRootItem = omTree.GetRootItem();
    if(mpExecuteMgr->m_eBusStatus == BUS_DISCONNECTED)
    {
        if(hRootItem == m_hSelectedTreeItem && omTree.ItemHasChildren(hRootItem) == FALSE)
        {
            pCmdUI->Enable(FALSE);
        }
        else if(hRootItem == m_hSelectedTreeItem && pSimSysNodeInfo!=nullptr)
        {
            pCmdUI->Enable(pSimSysNodeInfo->bIsBuildAllNodesValid());
        }
        else if(psNodeInfo != nullptr && psNodeInfo->m_eNodeFileType == NODE_FILE_C_CPP)
        {
            pCmdUI->Enable(!psNodeInfo->m_omStrCFileName.IsEmpty());
        }
        else if(psNodeInfo != nullptr && psNodeInfo->m_eNodeFileType == NODE_FILE_DLL)
        {
            pCmdUI->Enable(FALSE);
        }
    }
    else
    {
        pCmdUI->Enable(FALSE);
    }

}


/******************************************************************************/
/*  Function Name    :  OnUpdateRemoveAllNodes
/*  Input(s)         :  CCmdUI* pCmdUI
/*  Output           :  -
/*  Functionality    :  Called by the framework.
                        Enables/Disables "Remove All" button in Toolbar.
/*  Member of        :  CSimSysTreeView
/*  Friend of        :      -
/*  Author(s)        :  Robin G.K.
/*  Date Created     :  23.10.14
/*****************************************************************************/
void CSimSysTreeView::OnUpdateRemoveNode(CCmdUI* pCmdUI)
{
    CTreeCtrl& omTree = GetTreeCtrl();
    HTREEITEM hRootItem = omTree.GetRootItem();
    if(mpExecuteMgr->m_eBusStatus == BUS_CONNECTED)
    {
        pCmdUI->Enable(FALSE);
    }
    else if(hRootItem == m_hSelectedTreeItem && omTree.ItemHasChildren(hRootItem) == FALSE)
    {
        pCmdUI->Enable(FALSE);
    }
    else
    {
        pCmdUI->Enable(TRUE);
    }
}
/******************************************************************************/
/*  Function Name    :  vModifyToolbarIcon
/*  Input(s)         :  BYTE bytItemIndex, bool bItemON, UINT nTBIDON, UINT nTBIDOFF
/*  Output           :  -
/*  Functionality    :  To change the icon of "Enable/Disable" node button in toolbar.
/*  Member of        :  CSimSysTreeView
/*  Friend of        :      -
/*  Author(s)        :  Robin G.K.
/*  Date Created     :  23.10.14
/*****************************************************************************/
void CSimSysTreeView::vModifyToolbarIcon(BYTE bytItemIndex, bool bItemON, UINT nTBIDON, UINT nTBIDOFF)
{

    int Result = 0;
    m_hModAdvancedUILib = nullptr;
    CToolBarCtrl& ToolbarCtrl = mpGlobalObj->m_pomSimSysWnd->m_toolbar.GetToolBarCtrl();
    m_hModAdvancedUILib = ::LoadLibrary("AdvancedUIPlugIn.dll");


    /* Perform this only if resource dll is available */
    if ( m_hModAdvancedUILib )
    {

        /* Get image list objects */
        CImageList* pList           = ToolbarCtrl.GetImageList();
        CImageList* pListHot        = ToolbarCtrl.GetHotImageList();
        CImageList* pListDisabled   = ToolbarCtrl.GetDisabledImageList();

        /* Load the icon based on resource ID */
        HICON hIcon;
        if ( bItemON )
        {
            hIcon = LoadIcon(m_hModAdvancedUILib, MAKEINTRESOURCE(nTBIDON));
        }
        else
        {
            hIcon = LoadIcon(m_hModAdvancedUILib, MAKEINTRESOURCE(nTBIDOFF));
        }

        /* Replace the respective Icon images if icon is successfully loaded */
        if ( hIcon )
        {
            pList->Replace(bytItemIndex, hIcon);
            pListHot->Replace(bytItemIndex, hIcon);
            mpGlobalObj->m_pomSimSysWnd->m_toolbar.Invalidate();
        }
    }

}

void CSimSysTreeView::OnPaint()
{
    CTreeCtrl& om_Tree = GetTreeCtrl();
    CPaintDC dc(this);

    // Create a memory DC compatible with the paint DC
    CDC memDC;
    memDC.CreateCompatibleDC( &dc );

    CRect rcClip, rcClient;
    dc.GetClipBox( &rcClip );
    om_Tree.GetClientRect(&rcClient);

    // Select a compatible bitmap into the memory DC
    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap( &dc, rcClient.Width(), rcClient.Height() );
    memDC.SelectObject( &bitmap );
    // Set clip region to be same as that in paint DC
    CRgn rgn;
    rgn.CreateRectRgnIndirect( &rcClip );
    memDC.SelectClipRgn(&rgn);
    rgn.DeleteObject();


    // First let the control do its default drawing.
    CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );


    HTREEITEM hItem = om_Tree.GetFirstVisibleItem();

    int n = om_Tree.GetVisibleCount()+1;
    while( hItem && n--)
    {

        CRect rect;

        // Do not meddle with selected items or drop highlighted items
        UINT selflag = TVIS_DROPHILITED | TVIS_SELECTED;

        if ( !(om_Tree.GetItemState( hItem, selflag ) & selflag ))
        {
            CFont font;
            LOGFONT logfont;
            om_Tree.GetFont()->GetLogFont( &logfont );


            logfont.lfHeight = 100;

            if(om_Tree.GetCheck(hItem))
            {
                logfont.lfWeight = 400;
                memDC.SetTextColor(RGB(0, 0, 0));
            }
            else
            {
                logfont.lfItalic = TRUE;
                memDC.SetTextColor(RGB(120, 120, 120));
            }
            font.CreatePointFontIndirect(&logfont);
            CFont* pFont = memDC.SelectObject(&font);


            om_Tree.GetItemRect( hItem, &rect, TRUE );
            memDC.TextOut( rect.left, rect.top, om_Tree.GetItemText(hItem) );

            memDC.SelectObject(pFont);
            font.DeleteObject();


        }
        hItem = om_Tree.GetNextVisibleItem( hItem );
    }


    dc.BitBlt( rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), &memDC,
               rcClip.left, rcClip.top, SRCCOPY );
}

void CSimSysTreeView::OnKeyDown(UINT ch,UINT Count,UINT Flags)
{

    switch(ch)
    {
        case VK_SPACE://Space Bar
            if(mpExecuteMgr->m_eBusStatus == BUS_DISCONNECTED)
            {
                OnEnableDisableNode();
                CTreeView::OnKeyDown(ch,Count,Flags);
            }
            else
            {
                return;
            }
            break;
        case VK_INSERT: //INS key.
            if(mpExecuteMgr->m_eBusStatus == BUS_DISCONNECTED)
            {
                OnAddNode();
            }
            break;
        case VK_DELETE: //Del Key
            if(mpExecuteMgr->m_eBusStatus == BUS_DISCONNECTED)
            {
                OnRemoveNode();
            }
            break;
        case VK_RETURN: //Enter Key
            OnEditNode();
            break;
    }
    CTreeView::OnKeyDown(ch,Count,Flags);
}
PSNODEINFO CSimSysTreeView::GetNodeInfo(HTREEITEM hTreeItem)
{
    CSimSysNodeInfo* pSimSysNodeInfo =
        mpSimSysManager->pomGetSimSysNodeInfo();
    CString omstrSelectedItemText = GetTreeCtrl().GetItemText(hTreeItem);
    PSNODEINFO psNodeInfo = nullptr;
    if(pSimSysNodeInfo != nullptr)
    {
        //Set the selected node to member variable.
        if(omstrSelectedItemText.CompareNoCase(mstrBusName) != 0)
        {
            psNodeInfo = pSimSysNodeInfo->psGetSimSysNodePointer(omstrSelectedItemText);
        }
    }
    return psNodeInfo;
}
HTREEITEM CSimSysTreeView::FindHTreeItem(const CString& omStrNodeName)
{
    CTreeCtrl& om_Tree = GetTreeCtrl();
    HTREEITEM hRootItem = om_Tree.GetRootItem();
    HTREEITEM hSub = om_Tree.GetChildItem(hRootItem);
    while (hSub)
    {
        CString omStrText = om_Tree.GetItemText(hSub);
        if (omStrText.Compare(omStrNodeName) == 0)
        {
            return hSub;
        }
        hSub = om_Tree.GetNextSiblingItem(hSub);
    }
    return nullptr;
}