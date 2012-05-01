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
// Pointers of this and "CSimSysDetView" defined here
#include "HashDefines.h"    // All hash defines are here
#include "SplFileDlg.h"     // customise CSplFileDlg brower
#include "NodeDetailsDlg.h" // Dialog class for new node
#include "SimSysManager.h"
#include "FunctionEditorDoc.h"
#include "ExecuteManager.h"
#include "GlobalObj.h"
#include "Include/BaseDefs.h"
#include "DIL_Interface/DIL_Interface_extern.h"
#include "DIL_Interface/BaseDIL_CAN.h"

//const BYTE MSGBUF_ADD = 0x01;

#define defSIMSYSCONFIGFILTER   _T("BUSMASTER Simulated system Configuration files(*.sim)|*.sim||")
#define defSIMSYSCONFEXT        _T("sim")
#define defDLGFLAGS             OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST


IMPLEMENT_DYNCREATE(CSimSysTreeView, CTreeView)


//static BOOL GetFileNameFromPath(CString omPathName, CString& omFileName)
//{
//  BOOL bRet = FALSE;
//  int nIndex = -1;
//  if ((nIndex = omPathName.ReverseFind('/')) != -1)
//  {
//      omFileName = omPathName.GetBuffer(MAX_PATH) + (nIndex + 1);
//      bRet = TRUE;
//  }
//  else if ((nIndex = omPathName.ReverseFind('\\')) != -1)
//  {
//      omFileName = omPathName.GetBuffer(MAX_PATH) + (nIndex + 1);
//      bRet = TRUE;
//  }
//  return bRet;
//}
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
CSimSysTreeView::CSimSysTreeView()
{
    m_eBus = CSimSysTreeView::sm_eBus;
    bCallFrmSaveAs = FALSE;
    m_pomSubMenu = NULL;
    m_pomContextMenu = NULL;
    m_omSimsysCount         = 0;
    m_hRootItem = NULL;
    m_bIsNewNode            = FALSE;
    m_omSelectedItemText    = STR_EMPTY;
    m_hTreeItem             = NULL;
    // Clean the list
    m_omSimsysNames.RemoveAll();
    CSimSysManager::ouGetSimSysManager(m_eBus).podSetSimSysTreeView(this);
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
    if( m_pomContextMenu != NULL)
    {
        delete m_pomContextMenu;
        m_pomContextMenu = NULL;
    }

    CSimSysManager::ouGetSimSysManager(m_eBus).podSetSimSysTreeView(NULL);
}


BEGIN_MESSAGE_MAP(CSimSysTreeView, CTreeView)
    //{{AFX_MSG_MAP(CSimSysTreeView)
    ON_WM_LBUTTONDOWN()
    ON_WM_RBUTTONDOWN()
    ON_NOTIFY_REFLECT(NM_RCLICK, OnTreeViewRightclick)
    ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
    ON_COMMAND(IDM_ADD_SIMSYS, OnAddSimsys)
    ON_COMMAND(IDM_ALL_NODE_HANDLERS, OnAllNodeHandlers)
    ON_COMMAND(IDM_DELETE_ALL_SIMSYS, OnDeleteAllSimsys)
    ON_COMMAND(IDM_DELETE_NODE, OnDeleteNode)
    ON_COMMAND(IDM_EDIT_NODE, OnEditNode)
    ON_COMMAND(IDM_NEW_SIMSYS, OnNewSimsys)
    ON_COMMAND(IDM_NODE_ERRORHANDLERS, OnNodeErrorhandlers)
    ON_COMMAND(IDM_NODE_KEYHANDLERS, OnNodeKeyhandlers)
    ON_COMMAND(IDM_NODE_MESSAGEHANDLERS, OnNodeMessagehandlers)
    ON_COMMAND(IDM_NODE_TIMERHANDLERS, OnNodeTimerhandlers)
    ON_COMMAND(IDM_SIMSYS_ADDNODE, OnAddnode)
    ON_COMMAND(IDM_SIMSYS_ALLERRORHANDLERS, OnSimsysAllErrorhandlers)
    ON_COMMAND(IDM_SIMSYS_ALLHANDLERS, OnSimsysAllhandlers)
    ON_COMMAND(IDM_SIMSYS_ALLKEYHANDLERS, OnSimsysAllKeyhandlers)
    ON_COMMAND(IDM_SIMSYS_ALLMSGHANDLERS, OnSimsysAllMsghandlers)
    ON_COMMAND(IDM_SIMSYS_ALLTIMERHANDLERS, OnSimsysAllTimerhandlers)
    ON_COMMAND(IDM_SIMSYS_BUILDALL, OnSimsysBuildall)
    ON_COMMAND(IDM_SIMSYS_DELETEALLNODES, OnDeleteAllNodes)
    ON_COMMAND(IDM_SIMSYS_DELETESIMULATEDSYSTEM, OnDeleteSimulatedsystem)
    ON_COMMAND(IDM_SIMSYS_LOADALL, OnSimsysLoadall)
    ON_COMMAND(IDM_SIMSYS_SAVE, OnSimsysSave)
    ON_COMMAND(IDM_SIMSYS_SAVEAS, OnSimsysSaveAs)
    ON_COMMAND(IDM_SIMSYS_UNLOADALL, OnSimsysUnloadall)
    ON_COMMAND(IDM_SAVE_ALL_SIMSYS, OnSimsysSaveAll)

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
/*  Modification     :
/******************************************************************************/

BOOL CSimSysTreeView::bPopulateTree()
{
    BOOL bReturnValue = TRUE;
    // Get reference to the tree control
    CTreeCtrl& om_tree = GetTreeCtrl();
    om_tree.DeleteAllItems();
    // Insert root text
    om_tree.SetTextColor( BLUE_COLOR );
    m_hRootItem = om_tree.InsertItem( CGlobalObj::omGetBusName(m_eBus));
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();

    if (pSimSysNodeInfo != NULL)
    {
        PSSIMSYSINFO pSimSysInfo = pSimSysNodeInfo->psReturnSimsysInfoListPtr();

        while(pSimSysInfo != NULL)
        {
            HTREEITEM hSimsys = om_tree.InsertItem( pSimSysInfo->m_omStrSimSysName, m_hRootItem  );
            PSNODELIST pNodeInfo = pSimSysInfo->m_psNodesList;

            while(pNodeInfo != NULL)
            {
                CString omStrNodeName = pNodeInfo->m_sNodeInfo.m_omStrNodeName;
                HTREEITEM hNode = om_tree.InsertItem(omStrNodeName, hSimsys);

                if(pNodeInfo->m_sNodeInfo.m_omStrDllName != STR_EMPTY)
                {
                    CString omStrDllPath = pNodeInfo->m_sNodeInfo.m_omStrDllName;
                    HTREEITEM hDll = om_tree.InsertItem(omStrDllPath, hNode);

                    if(pNodeInfo->m_sNodeInfo.m_bIsDllLoaded)
                    {
                        om_tree.SetItemState( hDll, TVIS_BOLD, TVIS_BOLD );
                    }
                }

                pNodeInfo = pNodeInfo->m_psNextNode;
            }

            pSimSysInfo = pSimSysInfo-> m_psSimsysNext;
        }

        // Select the root item
        om_tree.SelectItem( m_hRootItem );
        // Expand the root
        om_tree.Expand( m_hRootItem, TVE_EXPAND );
    }

    return (bReturnValue);
}
/*****************************************************************************
/*  Function Name    :  vPopulateAddedSimSysInfo
/*
/*  Input(s)         :  CString omStrSimSysPath
/*  Output           :
/*  Functionality    :  This function will populate the tree with addded
/*                      simulated systems, nodes and the dlls under it.
/*  Member of        :  CSimSysTreeView
/*  Friend of        :      -
/*
/*  Author(s)        :  Harika M
/*  Date Created     :  19.12.2005
/*  Modification     :
/******************************************************************************/
void CSimSysTreeView::vPopulateAddedSimSysInfo(CString omStrSimSysPath)
{
    // Get reference to the tree control
    CTreeCtrl& om_tree = GetTreeCtrl();
    om_tree.SetTextColor( BLUE_COLOR );
    //Insert the simsys node
    m_hRootItem = om_tree.GetRootItem();
    CString srRoot = om_tree.GetItemText(m_hRootItem);

    if (srRoot != CGlobalObj::omGetBusName(m_eBus))
    {
        om_tree.DeleteAllItems();
        m_hRootItem = om_tree.InsertItem(CGlobalObj::omGetBusName(m_eBus));
    }

    HTREEITEM hSimsys = hInsertAndSelectItem( omStrSimSysPath, m_hRootItem );
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();

    if (pSimSysNodeInfo != NULL)
    {
        PSSIMSYSINFO pSimSysInfo = pSimSysNodeInfo->psGetSimSysPointer(omStrSimSysPath);

        if(pSimSysInfo != NULL)
        {
            PSNODELIST pNodeInfo = pSimSysInfo->m_psNodesList;

            while(pNodeInfo != NULL)
            {
                CString omStrNodeName = pNodeInfo->m_sNodeInfo.m_omStrNodeName;
                HTREEITEM hNode = om_tree.InsertItem(omStrNodeName, hSimsys);

                if(pNodeInfo->m_sNodeInfo.m_omStrDllName != STR_EMPTY)
                {
                    CString omStrDllPath = STR_EMPTY;
                    omStrDllPath = pNodeInfo->m_sNodeInfo.m_omStrDllName;
                    om_tree.InsertItem(omStrDllPath, hNode);
                }

                pNodeInfo = pNodeInfo->m_psNextNode;
            }
        }

        // Expand the root
        om_tree.Expand( hSimsys, TVE_EXPAND );
    }
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
                TVS_SHOWSELALWAYS   |
                TVS_FULLROWSELECT   |
                TVS_INFOTIP ;
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
    m_omLeftCLickPoint = point;
    CTreeView::OnLButtonDown(nFlags, point);
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
/*  Functionality    :  Display menu of the FLEXRAY_BUS
/*  Member of        :  CSimSysTreeView
/*  Friend of        :      -
/*
/*  Author(s)        :  Harika M
/*  Date Created     :  19.12.2005
/*  Modifications    :  Anish Kr., 26.02.09, Restructured
/******************************************************************************/
void CSimSysTreeView::vDisplayRootMenu()
{
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CTreeCtrl& om_Tree = GetTreeCtrl();
    m_pomContextMenu->DestroyMenu();
    m_pomContextMenu->LoadMenu(IDM_SIMSYS_OPRNS);
    m_pomSubMenu = m_pomContextMenu->GetSubMenu(0);
    m_pomSubMenu->EnableMenuItem(IDM_NEW_SIMSYS, MF_BYCOMMAND | MF_ENABLED );
    m_pomSubMenu->EnableMenuItem(IDM_ADD_SIMSYS, MF_BYCOMMAND | MF_ENABLED );
    m_pomSubMenu->EnableMenuItem(IDM_DELETE_ALL_SIMSYS, MF_BYCOMMAND |
                                 MF_DISABLED | MF_GRAYED );
    m_pomSubMenu->EnableMenuItem(IDM_SAVE_ALL_SIMSYS, MF_BYCOMMAND |
                                 MF_DISABLED | MF_GRAYED );

    //if simsys_count >= 1 then enable "Delete All systems"
    //ie. no. of item > 1(first item is Flexray bus)
    if(om_Tree.GetCount() > 1)
    {
        m_pomSubMenu->EnableMenuItem(IDM_DELETE_ALL_SIMSYS, MF_BYCOMMAND |
                                     MF_ENABLED );
        m_pomSubMenu->EnableMenuItem(IDM_SAVE_ALL_SIMSYS, MF_BYCOMMAND |
                                     MF_ENABLED );
    }

    ClientToScreen(&m_omRightClickPoint);
    m_pomSubMenu->TrackPopupMenu( TPM_LEFTALIGN |TPM_RIGHTBUTTON,
                                  m_omRightClickPoint.x,
                                  m_omRightClickPoint.y,
                                  this,
                                  NULL);
}

/******************************************************************************/
/*  Function Name    :  vDisplaySimSysMenu
/*
/*  Input(s)         :
/*  Output           :
/*  Functionality    :  Display menu of the Simulated system
/*  Member of        :  CSimSysTreeView
/*  Friend of        :      -
/*
/*  Author(s)        :  Harika M
/*  Date Created     :  19.12.2005
/*  Modifications    :  Anish Kr., 26.02.09, Restructured
/******************************************************************************/
void CSimSysTreeView::vDisplaySimSysMenu()
{
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    //Get the reqd. data from tree control
    CTreeCtrl& om_Tree = GetTreeCtrl();
    HTREEITEM hSelectedItem = om_Tree.GetSelectedItem();
    CString omStrSimSysName = om_Tree.GetItemText(hSelectedItem);
    //Load the menu item
    m_pomContextMenu->DestroyMenu();
    m_pomContextMenu->LoadMenu(IDM_SIMSYS_CONFIGMENU);
    m_pomSubMenu = m_pomContextMenu->GetSubMenu(0);
    //if simsys_count >= 1 then enable "Delete All nodes"
    const UINT unEnableFlag   = MF_BYCOMMAND | MF_ENABLED;
    const UINT unDiasableFlag = MF_BYCOMMAND | MF_DISABLED | MF_GRAYED;
    m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_SAVE, unEnableFlag );
    m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_SAVEAS, unEnableFlag );
    m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_DELETESIMULATEDSYSTEM, unEnableFlag );
    m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_ADDNODE, unEnableFlag );
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    UINT uCurrMenuFlag;

    //For Delete all nodes
    if (om_Tree.ItemHasChildren(hSelectedItem))
    {
        uCurrMenuFlag = unEnableFlag;
    }
    else
    {
        uCurrMenuFlag = unDiasableFlag;
    }

    m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_DELETEALLNODES, uCurrMenuFlag);

    //For build all
    if(pSimSysNodeInfo->bIsBuildAllValid(omStrSimSysName))
    {
        uCurrMenuFlag = unEnableFlag;
    }
    else
    {
        uCurrMenuFlag = unDiasableFlag;
    }

    m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_BUILDALL, uCurrMenuFlag);

    //For load all
    if(pSimSysNodeInfo->bIsLoadAllValid(omStrSimSysName))
    {
        uCurrMenuFlag = unEnableFlag;
    }
    else
    {
        uCurrMenuFlag = unDiasableFlag;
    }

    m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_LOADALL, uCurrMenuFlag);

    //For unload all
    if(pSimSysNodeInfo->bIsUnLoadAllValid(omStrSimSysName))
    {
        uCurrMenuFlag = unEnableFlag;
    }
    else
    {
        uCurrMenuFlag = unDiasableFlag;
    }

    m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_UNLOADALL, unDiasableFlag );
    // to check if all the handlers are enabled/ disabled
    BOOL bEnabled = pSimSysNodeInfo->bReturnAllHandlersStatus(omStrSimSysName);

    if(bEnabled)
    {
        BOOL bRetVal = ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                                  IDM_SIMSYS_ALLHANDLERS, MF_BYCOMMAND | MF_STRING,
                                  IDM_SIMSYS_ALLHANDLERS, _T("Disable All Handlers"));

        if (bRetVal == FALSE)
        {
            MessageBox(_T("Failed to change the text of the String"));
        }

        ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                   IDM_SIMSYS_ALLKEYHANDLERS, MF_BYCOMMAND | MF_STRING,
                   IDM_SIMSYS_ALLKEYHANDLERS, _T("Disable All Key Handlers"));
        ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                   IDM_SIMSYS_ALLMSGHANDLERS, MF_BYCOMMAND | MF_STRING,
                   IDM_SIMSYS_ALLMSGHANDLERS, _T("Disable All Message Handlers"));
        ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                   IDM_SIMSYS_ALLERRORHANDLERS, MF_BYCOMMAND | MF_STRING,
                   IDM_SIMSYS_ALLERRORHANDLERS, _T("Disable All Error Handlers"));
        ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                   IDM_SIMSYS_ALLTIMERHANDLERS, MF_BYCOMMAND | MF_STRING,
                   IDM_SIMSYS_ALLTIMERHANDLERS, _T("Disable All Timer Handlers"));
    }
    else
    {
        BOOL bRetVal = ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                                  IDM_SIMSYS_ALLHANDLERS, MF_BYCOMMAND | MF_STRING,
                                  IDM_SIMSYS_ALLHANDLERS, _T("Enable All Handlers"));

        if (bRetVal == FALSE)
        {
            MessageBox(_T("Failed to change the text of the String"));
        }

        ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                   IDM_SIMSYS_ALLKEYHANDLERS, MF_BYCOMMAND | MF_STRING,
                   IDM_SIMSYS_ALLKEYHANDLERS, _T("Enable All Key Handlers"));
        ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                   IDM_SIMSYS_ALLMSGHANDLERS, MF_BYCOMMAND | MF_STRING,
                   IDM_SIMSYS_ALLMSGHANDLERS, _T("Enable All Message Handlers"));
        ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                   IDM_SIMSYS_ALLERRORHANDLERS, MF_BYCOMMAND | MF_STRING,
                   IDM_SIMSYS_ALLERRORHANDLERS, _T("Enable All Error Handlers"));
        ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                   IDM_SIMSYS_ALLTIMERHANDLERS, MF_BYCOMMAND | MF_STRING,
                   IDM_SIMSYS_ALLTIMERHANDLERS, _T("Enable All Timer Handlers"));
    }

    // to check if all the key handlers are enabled/ disabled
    BOOL bKeyEnabled = pSimSysNodeInfo->bReturnAllKeyHandlersStatus(
                           omStrSimSysName);

    if(bKeyEnabled)
    {
        BOOL bRetVal = ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                                  IDM_SIMSYS_ALLKEYHANDLERS, MF_BYCOMMAND | MF_STRING,
                                  IDM_SIMSYS_ALLKEYHANDLERS, _T("Disable All Key Handlers"));

        if (bRetVal == FALSE)
        {
            MessageBox(_T("Failed to change the text of the String"));
        }
    }
    else
    {
        ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                   IDM_SIMSYS_ALLKEYHANDLERS, MF_BYCOMMAND | MF_STRING,
                   IDM_SIMSYS_ALLKEYHANDLERS, _T("Enable All Key Handlers"));
    }

    // to check if all the Message handlers are enabled/ disabled
    BOOL bMsgEnabled = pSimSysNodeInfo->bReturnAllMsgHandlersStatus(
                           omStrSimSysName);

    if(bMsgEnabled)
    {
        BOOL bRetVal = ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                                  IDM_SIMSYS_ALLMSGHANDLERS, MF_BYCOMMAND | MF_STRING,
                                  IDM_SIMSYS_ALLMSGHANDLERS, _T("Disable All Message Handlers"));

        if (bRetVal == FALSE)
        {
            MessageBox(_T("Failed to change the text of the String"));
        }
    }
    else
    {
        ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                   IDM_SIMSYS_ALLMSGHANDLERS, MF_BYCOMMAND | MF_STRING,
                   IDM_SIMSYS_ALLMSGHANDLERS, _T("Enable All Message Handlers"));
    }

    // to check if all the Error handlers are enabled/ disabled
    BOOL bErrorEnabled = pSimSysNodeInfo->bReturnAllErrorHandlersStatus(
                             omStrSimSysName);

    if(bErrorEnabled)
    {
        BOOL bRetVal = ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                                  IDM_SIMSYS_ALLERRORHANDLERS, MF_BYCOMMAND | MF_STRING,
                                  IDM_SIMSYS_ALLERRORHANDLERS, _T("Disable All Error Handlers"));

        if (bRetVal == FALSE)
        {
            MessageBox(_T("Failed to change the text of the String"));
        }
    }
    else
    {
        ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                   IDM_SIMSYS_ALLERRORHANDLERS, MF_BYCOMMAND | MF_STRING,
                   IDM_SIMSYS_ALLERRORHANDLERS, _T("Enable All Error Handlers"));
    }

    // to check if all the Timer handlers are enabled/ disabled
    BOOL bTimerEnabled = pSimSysNodeInfo->bReturnAllTimerHandlersStatus(
                             omStrSimSysName);

    if(bTimerEnabled)
    {
        BOOL bRetVal = ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                                  IDM_SIMSYS_ALLTIMERHANDLERS, MF_BYCOMMAND | MF_STRING,
                                  IDM_SIMSYS_ALLTIMERHANDLERS, _T("Disable All Timer Handlers"));

        if (bRetVal == FALSE)
        {
            MessageBox(_T("Failed to change the text of the String"));
        }
    }
    else
    {
        ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                   IDM_SIMSYS_ALLTIMERHANDLERS, MF_BYCOMMAND | MF_STRING,
                   IDM_SIMSYS_ALLTIMERHANDLERS, _T("Enable All Timer Handlers"));
    }

    //Check if the tool is connected
    BOOL bConnected =
        CSimSysManager::ouGetSimSysManager(m_eBus).ouGetFlags().nGetFlagStatus(H_CONNECTED);
    PSSIMSYSINFO pSimsys =
        pSimSysNodeInfo->psGetSimSysPointer(omStrSimSysName);

    if( (pSimsys != NULL) && (pSimsys->m_psNodesList != NULL))
    {
        //     if( pSimsys->m_psNodesList->m_sNodeInfo.m_bIsDllLoaded )
        if(pSimSysNodeInfo->bIsUnLoadAllValid(omStrSimSysName))
        {
            m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_UNLOADALL, MF_BYCOMMAND
                                         | MF_ENABLED );

            if(bConnected == TRUE )
            {
                m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_ALLHANDLERS, MF_BYCOMMAND
                                             | MF_ENABLED );
                m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_ALLKEYHANDLERS, MF_BYCOMMAND
                                             | MF_ENABLED );
                m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_ALLMSGHANDLERS, MF_BYCOMMAND
                                             | MF_ENABLED );
                m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_ALLERRORHANDLERS, MF_BYCOMMAND
                                             | MF_ENABLED );
                m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_ALLTIMERHANDLERS, MF_BYCOMMAND
                                             | MF_ENABLED );
            }
            else
            {
                m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_ALLHANDLERS, MF_BYCOMMAND |
                                             MF_DISABLED | MF_GRAYED );
                m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_ALLKEYHANDLERS, MF_BYCOMMAND |
                                             MF_DISABLED | MF_GRAYED );
                m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_ALLMSGHANDLERS, MF_BYCOMMAND |
                                             MF_DISABLED | MF_GRAYED );
                m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_ALLERRORHANDLERS, MF_BYCOMMAND |
                                             MF_DISABLED | MF_GRAYED );
                m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_ALLTIMERHANDLERS, MF_BYCOMMAND |
                                             MF_DISABLED | MF_GRAYED );
            }
        }
        else
        {
            m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_ALLHANDLERS, MF_BYCOMMAND |
                                         MF_DISABLED | MF_GRAYED );
            m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_ALLKEYHANDLERS, MF_BYCOMMAND |
                                         MF_DISABLED | MF_GRAYED );
            m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_ALLMSGHANDLERS, MF_BYCOMMAND |
                                         MF_DISABLED | MF_GRAYED );
            m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_ALLERRORHANDLERS, MF_BYCOMMAND |
                                         MF_DISABLED | MF_GRAYED );
            m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_ALLTIMERHANDLERS, MF_BYCOMMAND |
                                         MF_DISABLED | MF_GRAYED );
        }
    }
    else
    {
        m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_ALLHANDLERS, MF_BYCOMMAND |
                                     MF_DISABLED | MF_GRAYED );
        m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_ALLKEYHANDLERS, MF_BYCOMMAND |
                                     MF_DISABLED | MF_GRAYED );
        m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_ALLMSGHANDLERS, MF_BYCOMMAND |
                                     MF_DISABLED | MF_GRAYED );
        m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_ALLERRORHANDLERS, MF_BYCOMMAND |
                                     MF_DISABLED | MF_GRAYED );
        m_pomSubMenu->EnableMenuItem(IDM_SIMSYS_ALLTIMERHANDLERS, MF_BYCOMMAND |
                                     MF_DISABLED | MF_GRAYED );
    }

    ClientToScreen(&m_omRightClickPoint);
    m_pomSubMenu->TrackPopupMenu( TPM_LEFTALIGN |TPM_RIGHTBUTTON,
                                  m_omRightClickPoint.x,
                                  m_omRightClickPoint.y,
                                  this,
                                  NULL);
}


void CSimSysTreeView::vDisplayNodeMenu()
{
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CString omStrSimSysName = STR_EMPTY;
    // Get reference to the tree control
    CTreeCtrl& om_tree = GetTreeCtrl();
    HTREEITEM hSelectedItem = om_tree.GetSelectedItem();
    omStrSimSysName = om_tree.GetItemText(om_tree.GetParentItem(hSelectedItem));
    CString omStrNodeName =  om_tree.GetItemText(hSelectedItem);
    // Get node pointer from the data structure
    // corresponding to the simsys under which node is selected
    //Check if the tool is connected
    BOOL bConnected =
        CSimSysManager::ouGetSimSysManager(m_eBus).ouGetFlags().nGetFlagStatus(H_CONNECTED);
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    PSNODEINFO pNode = pSimSysNodeInfo->psGetSimSysNodePointer(omStrSimSysName, omStrNodeName );

    if ( pNode == NULL ) //if it is not node don't do anything
    {
        return;
    }

    m_pomContextMenu->DestroyMenu();
    m_pomContextMenu->LoadMenu(IDM_NODE_OPRNS);
    m_pomSubMenu = m_pomContextMenu->GetSubMenu(0);

    //if simsys_count >= 1 then enable "Delete All systems"
    if (m_eBus == CAN) // Allow only if it is CAN node
    {
        m_pomSubMenu->EnableMenuItem(IDM_EDIT_NODE, MF_BYCOMMAND | MF_ENABLED );
    }
    else
    {
        m_pomSubMenu->EnableMenuItem(IDM_EDIT_NODE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
    }

    m_pomSubMenu->EnableMenuItem(IDM_DELETE_NODE, MF_BYCOMMAND | MF_ENABLED );
    /**************************************************************************/
    // For node
    // to check if all the handlers are enabled/ disabled
    BOOL bEnabled = pSimSysNodeInfo->bReturnNodeAllHandlersStatus(
                        omStrSimSysName , omStrNodeName);

    if(bEnabled)
    {
        BOOL bRetVal = ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                                  IDM_ALL_NODE_HANDLERS, MF_BYCOMMAND | MF_STRING,
                                  IDM_ALL_NODE_HANDLERS, _T("Disable All Handlers"));

        if (bRetVal == FALSE)
        {
            MessageBox(_T("Failed to change the text of the String"));
        }

        ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                   IDM_NODE_KEYHANDLERS, MF_BYCOMMAND | MF_STRING,
                   IDM_NODE_KEYHANDLERS, _T("Disable Key Handlers"));
        ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                   IDM_NODE_MESSAGEHANDLERS, MF_BYCOMMAND | MF_STRING,
                   IDM_NODE_MESSAGEHANDLERS, _T("Disable Message Handlers"));
        ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                   IDM_NODE_ERRORHANDLERS, MF_BYCOMMAND | MF_STRING,
                   IDM_NODE_ERRORHANDLERS, _T("Disable Error Handlers"));
        ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                   IDM_NODE_TIMERHANDLERS, MF_BYCOMMAND | MF_STRING,
                   IDM_NODE_TIMERHANDLERS, _T("Disable Timer Handlers"));
    }
    else
    {
        BOOL bRetVal = ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                                  IDM_ALL_NODE_HANDLERS, MF_BYCOMMAND | MF_STRING,
                                  IDM_ALL_NODE_HANDLERS, _T("Enable All Handlers"));

        if (bRetVal == FALSE)
        {
            MessageBox(_T("Failed to change the text of the String"));
        }

        ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                   IDM_NODE_KEYHANDLERS, MF_BYCOMMAND | MF_STRING,
                   IDM_NODE_KEYHANDLERS, _T("Enable Key Handlers"));
        ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                   IDM_NODE_MESSAGEHANDLERS, MF_BYCOMMAND | MF_STRING,
                   IDM_NODE_MESSAGEHANDLERS, _T("Enable Message Handlers"));
        ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                   IDM_NODE_ERRORHANDLERS, MF_BYCOMMAND | MF_STRING,
                   IDM_NODE_ERRORHANDLERS, _T("Enable Error Handlers"));
        ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                   IDM_NODE_TIMERHANDLERS, MF_BYCOMMAND | MF_STRING,
                   IDM_NODE_TIMERHANDLERS, _T("Enable Timer Handlers"));
    }

    // to check if all the key handlers are enabled/ disabled
    BOOL bKeyEnabled = pSimSysNodeInfo->bReturnNodeKeyHandlersStatus(
                           omStrSimSysName , omStrNodeName );

    if(bKeyEnabled)
    {
        BOOL bRetVal = ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                                  IDM_NODE_KEYHANDLERS, MF_BYCOMMAND | MF_STRING,
                                  IDM_NODE_KEYHANDLERS, _T("Disable Key Handlers"));

        if (bRetVal == FALSE)
        {
            MessageBox(_T("Failed to change the text of the String"));
        }
    }
    else
    {
        ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                   IDM_NODE_KEYHANDLERS, MF_BYCOMMAND | MF_STRING,
                   IDM_NODE_KEYHANDLERS, _T("Enable Key Handlers"));
    }

    // to check if all the Message handlers are enabled/ disabled
    BOOL bMsgEnabled = pSimSysNodeInfo->bReturnNodeMsgHandlersStatus(
                           omStrSimSysName , omStrNodeName );

    if(bMsgEnabled)
    {
        BOOL bRetVal = ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                                  IDM_NODE_MESSAGEHANDLERS, MF_BYCOMMAND | MF_STRING,
                                  IDM_NODE_MESSAGEHANDLERS, _T("Disable Message Handlers"));

        if (bRetVal == FALSE)
        {
            MessageBox(_T("Failed to change the text of the String"));
        }
    }
    else
    {
        ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                   IDM_NODE_MESSAGEHANDLERS, MF_BYCOMMAND | MF_STRING,
                   IDM_NODE_MESSAGEHANDLERS, _T("Enable Message Handlers"));
    }

    // to check if all the Error handlers are enabled/ disabled
    BOOL bErrorEnabled = pSimSysNodeInfo->bReturnNodeErrorHandlersStatus(
                             omStrSimSysName , omStrNodeName );

    if(bErrorEnabled)
    {
        BOOL bRetVal = ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                                  IDM_NODE_ERRORHANDLERS, MF_BYCOMMAND | MF_STRING,
                                  IDM_NODE_ERRORHANDLERS, _T("Disable Error Handlers"));

        if (bRetVal == FALSE)
        {
            MessageBox(_T("Failed to change the text of the String"));
        }
    }
    else
    {
        ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                   IDM_NODE_ERRORHANDLERS, MF_BYCOMMAND | MF_STRING,
                   IDM_NODE_ERRORHANDLERS, _T("Enable Error Handlers"));
    }

    // to check if all the Timer handlers are enabled/ disabled
    BOOL bTimerEnabled = pSimSysNodeInfo->bReturnNodeTimerHandlersStatus(
                             omStrSimSysName , omStrNodeName);

    if(bTimerEnabled)
    {
        BOOL bRetVal = ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                                  IDM_NODE_TIMERHANDLERS, MF_BYCOMMAND | MF_STRING,
                                  IDM_NODE_TIMERHANDLERS, _T("Disable Timer Handlers"));

        if (bRetVal == FALSE)
        {
            MessageBox(_T("Failed to change the text of the String"));
        }
    }
    else
    {
        ModifyMenu(m_pomSubMenu->GetSafeHmenu(),
                   IDM_NODE_TIMERHANDLERS, MF_BYCOMMAND | MF_STRING,
                   IDM_NODE_TIMERHANDLERS, _T("Enable Timer Handlers"));
    }

    if((pNode != NULL) && ( pNode->m_bIsDllLoaded)
            && (bConnected == TRUE ))
    {
        m_pomSubMenu->EnableMenuItem(IDM_ALL_NODE_HANDLERS, MF_BYCOMMAND
                                     | MF_ENABLED );
        m_pomSubMenu->EnableMenuItem(IDM_NODE_KEYHANDLERS, MF_BYCOMMAND
                                     | MF_ENABLED );
        m_pomSubMenu->EnableMenuItem(IDM_NODE_MESSAGEHANDLERS, MF_BYCOMMAND
                                     | MF_ENABLED );
        m_pomSubMenu->EnableMenuItem(IDM_NODE_ERRORHANDLERS, MF_BYCOMMAND
                                     | MF_ENABLED );
        m_pomSubMenu->EnableMenuItem(IDM_NODE_TIMERHANDLERS, MF_BYCOMMAND
                                     | MF_ENABLED );
    }
    else
    {
        m_pomSubMenu->EnableMenuItem(IDM_ALL_NODE_HANDLERS, MF_BYCOMMAND |
                                     MF_DISABLED | MF_GRAYED );
        m_pomSubMenu->EnableMenuItem(IDM_NODE_KEYHANDLERS, MF_BYCOMMAND |
                                     MF_DISABLED | MF_GRAYED );
        m_pomSubMenu->EnableMenuItem(IDM_NODE_MESSAGEHANDLERS, MF_BYCOMMAND |
                                     MF_DISABLED | MF_GRAYED );
        m_pomSubMenu->EnableMenuItem(IDM_NODE_ERRORHANDLERS, MF_BYCOMMAND |
                                     MF_DISABLED | MF_GRAYED );
        m_pomSubMenu->EnableMenuItem(IDM_NODE_TIMERHANDLERS, MF_BYCOMMAND |
                                     MF_DISABLED | MF_GRAYED );
    }

    /**************************************************************************/
    ClientToScreen(&m_omRightClickPoint);
    m_pomSubMenu->TrackPopupMenu( TPM_LEFTALIGN |TPM_RIGHTBUTTON,
                                  m_omRightClickPoint.x,
                                  m_omRightClickPoint.y,
                                  this,
                                  NULL);
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
/*  Modification     :
/******************************************************************************/
void CSimSysTreeView::OnTreeViewRightclick(NMHDR* , LRESULT* pResult)
{
    UINT uFlags = 0;
    CTreeCtrl& om_Tree = GetTreeCtrl();
    HTREEITEM hSelectedItem = om_Tree.HitTest( m_omRightClickPoint, &uFlags );

    if ( uFlags & TVHT_ONITEM )
    {
        //Automatic selection not happening
        om_Tree.SelectItem(hSelectedItem);

        if(m_pomContextMenu == NULL )
        {
            m_pomContextMenu = new CMenu;
        }

        //Check if the tool is connected
        BOOL bConnected =
            CSimSysManager::ouGetSimSysManager(m_eBus).ouGetFlags().nGetFlagStatus(H_CONNECTED);
        // Get the parent item
        HTREEITEM hParentItem =   om_Tree.GetParentItem(hSelectedItem);
        HTREEITEM hGrandParentItem = om_Tree.GetParentItem(hParentItem);

        //// when selected item is root "FlexrayBus"
        if ( hParentItem == NULL )
        {
            vDisplayRootMenu();
        }
        else
        {
            if( hGrandParentItem == NULL)   // for any simsys
            {
                vDisplaySimSysMenu();
            }
            ///for any node
            else
            {
                CString omStrGrandParent = om_Tree.GetItemText(hGrandParentItem);

                if (omStrGrandParent.CompareNoCase(CGlobalObj::omGetBusName(m_eBus)) == 0)
                {
                    //Display menu for any node
                    vDisplayNodeMenu();
                }
            }// else if , for a node
        } // else , for right click on sim sys or node.
    } // else, for right click on any item

    *pResult = 0;
}
/******************************************************************************/
/*  Function Name    :  OnSelchanged                                          */
/*                                                                            */
/*  Input(s)         :  NMHDR* pNMHDR, LRESULT* pResult                       */
/*  Output           :                                                        */
/*  Functionality    :  Called by the frame work when an item selection is    */
/*                      changed. Updates node information on the right pane
                        depending on the selected item

/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  19.12.2005                                            */
/*  Modifications    :
/******************************************************************************/
void CSimSysTreeView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    // check if any data in the form view
    // is missing. If yes,
    // don't let the user to select any item
    // from the tree view
    CSimSysDetView* pSimSysDetView = NULL;
    CSimSysNodeInfo* pTempSimsysNodeInfo = NULL;
    CTreeCtrl& om_tree = GetTreeCtrl();
    // Get handle to selected item
    HTREEITEM hSelectedItem = pNMTreeView->itemNew.hItem;
    pTempSimsysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    pSimSysDetView =
        CSimSysManager::ouGetSimSysManager(m_eBus).podGetSimSysDetView();

    if ( (hSelectedItem != NULL) &&
            (pSimSysDetView != NULL) &&
            (pTempSimsysNodeInfo != NULL))
    {
        HTREEITEM hParentItem = om_tree.GetParentItem(hSelectedItem);
        HTREEITEM hGrandParentItem = om_tree.GetParentItem(hParentItem);
        CString omStrGPText = om_tree.GetItemText(hGrandParentItem);

        if ( omStrGPText == CGlobalObj::omGetBusName(m_eBus) )// only for node names
        {
            CString omStrNodeName = om_tree.GetItemText( hSelectedItem );
            CString omStrSimName = om_tree.GetItemText( hParentItem );
            m_omSelectedItemText = omStrNodeName;
            // Get node pointer from the data structure
            // corresponding to the simulated system
            PSNODEINFO pNode =
                pTempSimsysNodeInfo->psGetSimSysNodePointer(omStrSimName, omStrNodeName);

            if ( pNode != NULL )
            {
                // check if the controls in the form view are hidden
                if ( pSimSysDetView->bGetControlStatus() == TRUE )//  TRUE - Hidden
                {
                    // Controls are hidden, unhide
                    pSimSysDetView->vHideControls(SW_SHOW);
                }

                // Fill the node details for this node
                pSimSysDetView->vDisplayNodeInformation( pNode );
            }
        }
        else
        {
            // Item is root
            // Hide all controls
            pSimSysDetView->vHideControls(SW_HIDE);
        }
    }

    *pResult = 0;
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
/*  Function Name    :  OnAddSimsys                                           */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  New Simulated system is created and added to the data
/*                      structure
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  19.12.2005                                            */
/*  Modifications    :
/******************************************************************************/

void CSimSysTreeView::OnAddSimsys()
{
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CString oCfgFilename = STR_EMPTY;
    CSplFileDlg oCfgFileDlg(TRUE,       // Open dialog as Open File dlg
                            defSIMSYSCONFEXT,  // default extension
                            NULL,       // default file name
                            OFN_HIDEREADONLY |OFN_EXTENSIONDIFFERENT |OFN_FILEMUSTEXIST, // mode
                            defSIMSYSCONFIGFILTER, // filter
                            NULL,        // parent wnd
                            _T("Add"));
    oCfgFileDlg.m_ofn.lpstrTitle = _T("Add Configuration Filename...");

    if(oCfgFileDlg.DoModal() == IDOK)
    {
        oCfgFilename = oCfgFileDlg.GetPathName();
        vAddSimDetFromFile(oCfgFilename);
    }
}


/******************************************************************************/
/*  Function Name    :  void CSimSysTreeView::vAddSimDetFromFile()
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Loads all the information from the .sim file
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Pradeep Kadoor
/*  Date Created     :  19.02.2009                                            */
/*  Modification     :
/******************************************************************************/
void CSimSysTreeView::vAddSimDetFromFile(CString oCfgFilename)
{
    BOOL bIsSimsyspresent = FALSE;
    BOOL bListSucess = FALSE;
    BOOL bDuplicateFound = FALSE;
    BOOL bDupNodeFound = FALSE;
    BOOL bDupCFileFound = FALSE;
    sSIMSYSINFO* pSimsys = NULL;
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    CSimSysConfigDetails* pSimsysConfig =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysConfig();

    if(pSimSysNodeInfo != NULL)
    {
        bIsSimsyspresent = pSimSysNodeInfo->bIsSimSysPresent(oCfgFilename);
    }

    if( (!bIsSimsyspresent) && ( pSimsysConfig != NULL ) )
    {
        int nReturn = pSimsysConfig->nLoadConfiguration(oCfgFilename, pSimsys);

        if((nReturn == defCONFIG_FILE_SUCCESS) && (pSimsys != NULL))
        {
            if (!pSimSysNodeInfo->bIsAnyInfoInSimsysPreExist(pSimsys))
            {
                pSimSysNodeInfo->vAddSimSys(pSimsys);
                vPopulateAddedSimSysInfo(oCfgFilename);
            }
        }
        else if( nReturn == defCONFIG_FILE_CORRUPT)
        {
            AfxMessageBox(_T(" The config file is corrupted. cannot add!"));
        }
    }
}


/******************************************************************************/
/*  Function Name    :  OnAllNodeHandlers                                     */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Enables all the handlers of the dll under the node.
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  28.02.2005                                            */
/*  Modification     :
/******************************************************************************/
void CSimSysTreeView::OnAllNodeHandlers()
{
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    CTreeCtrl& om_Tree = GetTreeCtrl();
    HTREEITEM hSelectedItem = om_Tree.GetSelectedItem();
    CString omStrNodeName = STR_EMPTY;
    CString omStrSimSysName = STR_EMPTY;
    sSIMSYSINFO* pSimsys = NULL;
    sNODEINFO* pNode = NULL;
    HTREEITEM hParent = om_Tree.GetParentItem(hSelectedItem);
    omStrSimSysName = om_Tree.GetItemText(hParent);
    omStrNodeName = om_Tree.GetItemText(hSelectedItem);
    CString omStrMenuText = STR_EMPTY;
    CSimSysDetView* pSimsysDetView = NULL;
    pSimsysDetView =
        CSimSysManager::ouGetSimSysManager(m_eBus).podGetSimSysDetView();
    CSimSysNodeInfo* pTempSimsys = NULL;
    pTempSimsys = CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    m_pomSubMenu->GetMenuString(IDM_ALL_NODE_HANDLERS , omStrMenuText , MF_BYCOMMAND);

    if(omStrMenuText == "Enable All Handlers" )
    {
        // when user wants to enable all handlers
        pSimSysNodeInfo->vSetEnableNodeAllHandlers(omStrSimSysName ,
                omStrNodeName , TRUE);
    }
    else if(omStrMenuText == "Disable All Handlers" )
    {
        pSimSysNodeInfo->vSetEnableNodeAllHandlers(omStrSimSysName ,
                omStrNodeName , FALSE);
    }

    if ( pSimsysDetView != NULL )
    {
        pSimsys = pTempSimsys->psGetSimSysPointer(omStrSimSysName);
        pNode = pTempSimsys->psGetSimSysNodePointer(omStrSimSysName, omStrNodeName );

        if ( pNode != NULL ) //if it is node
        {
            // check if the controls in the form view are hidden
            if ( pSimsysDetView->bGetControlStatus() == TRUE )//  TRUE - Hidden
            {
                // Controls are hidden, unhide
                pSimsysDetView->vHideControls(SW_SHOW);
            }

            // Fill the node details for this node
            pSimsysDetView->vDisplayNodeInformation( pNode );
        }
    }
}
/******************************************************************************/
/*  Function Name    :  OnDeleteAllSimsys
/*
/*  Input(s)         :
/*  Output           :
/*  Functionality    :  Deletes all the simulated systems from the tree view and
                        from the data structure.
/*  Member of        :  CSimSysTreeView
/*  Friend of        :      -
/*
/*  Author(s)        :  Harika M
/*  Date Created     :  28.02.2005
/*  Modification     :  Anish kr., 27.02.09, Logic changed
/******************************************************************************/
void CSimSysTreeView::OnDeleteAllSimsys()
{
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    BOOL bIsDllsLoaded = pSimSysNodeInfo->bIsUnLoadAllValid(STR_EMPTY);

    if ((!bIsDllsLoaded) && (pSimSysNodeInfo != NULL))
    {
        CTreeCtrl& om_tree = GetTreeCtrl();

        if (om_tree.GetCount() > 1) // if Main node has children
        {
            int nReturnVal = AfxMessageBox(ALL_SIMSYS_DELETE_CONFMN, MB_YESNO|MB_ICONQUESTION);

            if (nReturnVal == IDYES)
            {
                if (pSimSysNodeInfo->bIsAnySimSysModified())
                {
                    int nReturn = AfxMessageBox(ASK_SIMSYS_SAVE_PROMPT,
                                                MB_YESNO|MB_ICONQUESTION);

                    if (nReturn == IDYES)
                    {
                        CSimSysManager::ouGetSimSysManager(m_eBus).vSaveAllSimSys();
                    }
                }

                pSimSysNodeInfo->bDeleteSimsysFromInfo( STR_EMPTY );
                om_tree.DeleteAllItems();
                om_tree.SetTextColor( BLUE_COLOR );
                om_tree.InsertItem( CGlobalObj::omGetBusName(m_eBus) );
            }
        }
    }
    else
    {
        AfxMessageBox(_T("Unload all the dlls and try again!!"));
    }
}
/******************************************************************************/
/*  Function Name    :  OnDeleteNode                                          */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Deletes the selected node from the tree view and
                        from the data structure.
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  28.02.2005                                            */
/*  Modification     :
/******************************************************************************/
void CSimSysTreeView::OnDeleteNode()
{
    PSNODEINFO pNode =    NULL;
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    BOOL bSuccess = FALSE;
    BOOL bDataSet = FALSE;
    CTreeCtrl& om_tree = GetTreeCtrl();
    HTREEITEM hNodeItem = om_tree.GetSelectedItem();
    CString omStrNodeName = om_tree. GetItemText(hNodeItem);
    HTREEITEM hSimSysItem =   om_tree.GetParentItem(hNodeItem);
    CString omStrSimSysName = om_tree. GetItemText(hSimSysItem);
    // First check if the node can be deleted or not.
    // only if the dll is not loaded it can be deleted.
    pNode = pSimSysNodeInfo->psGetSimSysNodePointer( omStrSimSysName , omStrNodeName );

    if( pNode != NULL )
    {
        BOOL bIsDllLoaded = pNode->m_bIsDllLoaded;

        if(bIsDllLoaded)
        {
            AfxMessageBox(_T("Unload the dll and try again!!"));
        }
        else
        {
            bSuccess = pSimSysNodeInfo->bDeleteNodeFromSimSys(omStrSimSysName , omStrNodeName);

            if(bSuccess)
            {
                //Now delete from tree window
                if (om_tree.ItemHasChildren(hNodeItem))
                {
                    HTREEITEM  hDllItem = om_tree.GetChildItem(hNodeItem);
                    om_tree.DeleteItem(hDllItem);
                    om_tree.DeleteItem(hNodeItem);
                }
                else
                {
                    om_tree.DeleteItem(hNodeItem);
                }

                // Get previous sibbling item
                HTREEITEM hPrevItem = om_tree.GetPrevSiblingItem(hNodeItem);

                if ( hPrevItem != NULL )
                {
                    // Select prev item
                    om_tree.SelectItem( hPrevItem );
                }
                else
                {
                    om_tree.SelectItem(hSimSysItem);
                }

                //Set the modify flag
                PSSIMSYSINFO pSimsysInfo = pSimSysNodeInfo->psGetSimSysPointer(omStrSimSysName);

                if( pSimsysInfo != NULL )
                {
                    pSimsysInfo->m_bIsSimSysModified = TRUE;
                }
            }
        }
    }
}
/******************************************************************************/
/*  Function Name    :  OnEditNode                                            */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  For Editing selected node
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  20.12.2005                                            */
/*  Modifications    :
/*
*****************************************************************************/
void CSimSysTreeView::OnEditNode()
{
    PSNODEINFO pNode =    NULL;
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    CTreeCtrl& omTree = GetTreeCtrl();
    HTREEITEM hItem = omTree.GetSelectedItem();
    HTREEITEM hParentItem = omTree.GetParentItem(hItem);
    HTREEITEM hGrandParent = omTree.GetParentItem(hParentItem);
    CString omStrSim = omTree.GetItemText(hParentItem);
    CString omStrBus = omTree.GetItemText(hGrandParent);

    if ((omStrBus == CGlobalObj::omGetBusName(m_eBus)) && (pSimSysNodeInfo != NULL))
    {
        CString omStrNode = omTree.GetItemText(hItem);
        pNode = pSimSysNodeInfo->psGetSimSysNodePointer( omStrSim , omStrNode );

        if (pNode)
        {
            if(pNode->m_bIsDllLoaded)
            {
                AfxMessageBox(_T("Unload the dll and try again!"));
            }
            else
            {
                vAddEditNode(TRUE);
            }
        }
    }
}

/******************************************************************************
  Function Name    :  vAddEditNode

  Input(s)         :  BOOL bMode
  Output           :
  Functionality    :  Displays node detials dialog to add new node
                      or edit selecetd node based on the parameter passed.
  Member of        :  CSimSysTreeView
  Friend of        :      -

  Author(s)        :  Harika M
  Date Created     :  27.12.2005
  Modification     :  Anish Kr
  Date             :  26.02.09, Changed logic
/******************************************************************************/
void CSimSysTreeView::vAddEditNode(BOOL bMode)
{
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    BOOL bIsFound = FALSE;
    CTreeCtrl& omTree = GetTreeCtrl();
    HTREEITEM hSelItem = omTree.GetSelectedItem();
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();

    if(pSimSysNodeInfo != NULL)
    {
        //For Node Editing
        if (bMode == TRUE)
        {
            //Parent is Simsys
            HTREEITEM hParentSys  = omTree.GetParentItem(hSelItem);
            CString omNodeName    = omTree.GetItemText(hSelItem);
            CString omSimSysName = omTree.GetItemText(hParentSys);
            PSNODEINFO pNode =  pSimSysNodeInfo->
                                psGetSimSysNodePointer( omSimSysName , omNodeName );

            if (pNode != NULL)
            {
                CNodeDetailsDlg odNodeDlg(m_eBus, pNode , NULL);
                odNodeDlg.DoModal();

                if (odNodeDlg.m_bIsNodeModified == TRUE)
                {
                    PSSIMSYSINFO psSimSys = pSimSysNodeInfo->psGetSimSysPointer(omSimSysName);
                    psSimSys->m_bIsSimSysModified = TRUE;

                    //Update the tree view
                    if ( !pNode->m_omStrNodeName.IsEmpty() )
                    {
                        omTree.SetItemText(hSelItem, pNode->m_omStrNodeName);
                    }

                    if ( !pNode->m_omStrDllName.IsEmpty())
                    {
                        //change Dll name if old name present else add it as child
                        HTREEITEM hChildDll  = omTree.GetChildItem(hSelItem);

                        if (hChildDll != NULL)
                        {
                            omTree.SetItemText(hChildDll, pNode->m_omStrDllName);
                        }
                        else
                        {
                            omTree.InsertItem(pNode->m_omStrDllName, hSelItem);
                        }
                    }

                    //Update det view
                    CSimSysDetView* pSimSysDetView =
                        CSimSysManager::ouGetSimSysManager(m_eBus).podGetSimSysDetView();
                    pSimSysDetView->vDisplayNodeInformation(pNode);
                }
            }
        }
        else
        {
            //Assuming SimSys is selected
            HTREEITEM hSelItem = omTree.GetSelectedItem();
            CString omSimSysName = omTree.GetItemText(hSelItem);
            PSSIMSYSINFO psSimSys = pSimSysNodeInfo->psGetSimSysPointer(omSimSysName);

            if (psSimSys != NULL)
            {
                PSNODELIST pNodeList = new sNODELIST(m_eBus);

                if (pNodeList != NULL)
                {
                    PSNODEINFO pNode = &pNodeList->m_sNodeInfo;
                    CNodeDetailsDlg odNodeDlg(m_eBus, pNode, NULL);

                    if ( odNodeDlg.DoModal() == IDOK )
                    {
                        psSimSys->vAddNodeToList(pNodeList);
                        CGlobalObj::ouGetObj(m_eBus).RegisterNodeToDIL(TRUE, pNode);
                        //Establish Connection with All the nodes which are necessory
                        psSimSys->m_unNumberOfNodesAdded++;
                        psSimSys->m_bIsSimSysModified = TRUE;
                        //Add to the tree view
                        HTREEITEM hNode = omTree.InsertItem(pNode->m_omStrNodeName, hSelItem);
                        //Set node as selected so that its detail can be displayed
                        omTree.SelectItem(hNode);

                        if ( !pNode->m_omStrDllName.IsEmpty())
                        {
                            //change Dll name if old name present else add it as child
                            HTREEITEM hChildDll  = omTree.GetChildItem(hNode);

                            if (hChildDll != NULL)
                            {
                                omTree.SetItemText(hChildDll, pNode->m_omStrDllName);
                            }
                            else
                            {
                                omTree.InsertItem(pNode->m_omStrDllName, hNode);
                            }
                        }
                    }
                    else
                    {
                        delete pNodeList;
                    }
                }
                else
                {
                    AfxMessageBox(_T("Memory Creation Error. Please try again"));
                }
            }
        }
    }

    omTree.Invalidate();
}

/**
 * \brief New Simulated System
 * \req RS_13_1 - Option "New Simulated System"
 *
 * New sim sys is created and added to the data structure
 */
void CSimSysTreeView::OnNewSimsys()
{
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    BOOL bReturn = FALSE;
    CSplFileDlg oCfgFileDlg(FALSE,      // Save  as dialog
                            defSIMSYSCONFEXT,               // default extension
                            NULL,                           // default file name
                            defDLGFLAGS,                    // mode
                            defSIMSYSCONFIGFILTER,          // filter
                            NULL,                           // parent wnd
                            _T("Open"));
    oCfgFileDlg.m_ofn.lpstrTitle = _T("New Simulated system Configuration Filename...");

    if(oCfgFileDlg.DoModal() == IDOK)
    {
        // get the name of the selected file
        CString oCfgFilename = oCfgFileDlg.GetPathName();
        CSimSysNodeInfo* pSimSysNodeInfo =
            CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();

        if (pSimSysNodeInfo != NULL)
        {
            BOOL bIsSimsyspresent = pSimSysNodeInfo->bIsSimSysPresent(oCfgFilename);

            if( !bIsSimsyspresent)
            {
                //Add to the list of simsys
                pSimSysNodeInfo->bAddNewSimSysInfo(oCfgFilename);
                //Add to the tree view
                hInsertAndSelectItem( oCfgFilename, GetTreeCtrl().GetRootItem());
            }
        }
    }
}

/******************************************************************************/
/*  Function Name    :  OnNodeErrorhandlers                                   */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Enables all the error handlers of the dll under the node.
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  27.12.2005                                            */
/*  Modification     :  Anish Kr.,
/******************************************************************************/
void CSimSysTreeView::OnNodeErrorhandlers()
{
    CTreeCtrl& om_Tree = GetTreeCtrl();
    HTREEITEM hSelectedItem = om_Tree.GetSelectedItem();
    HTREEITEM hParent = om_Tree.GetParentItem(hSelectedItem);
    //Assuming a node as selected
    CString omStrSimSysName = om_Tree.GetItemText(hParent);
    CString omStrNodeName = om_Tree.GetItemText(hSelectedItem);
    CString omStrMenuText = _T("");
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();

    if (m_pomSubMenu != NULL)
    {
        m_pomSubMenu->GetMenuString(IDM_NODE_ERRORHANDLERS , omStrMenuText , MF_BYCOMMAND);
    }

    if(omStrMenuText == _T("Enable Error Handlers") )
    {
        // when user wants to enable all handlers
        pSimSysNodeInfo->vSetEnableNodeErrorHandlers(omStrSimSysName ,
                omStrNodeName , TRUE);
    }
    else if (omStrMenuText == "Disable Error Handlers" )
    {
        pSimSysNodeInfo->vSetEnableNodeErrorHandlers(omStrSimSysName ,
                omStrNodeName , FALSE);
    }
    else
    {
        ASSERT(FALSE);
    }
}
/******************************************************************************/
/*  Function Name    :  OnNodeKeyhandlers                                     */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Enables all the key handlers of the dll under the node.
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  27.12.2005                                            */
/*  Modification     :
/******************************************************************************/
void CSimSysTreeView::OnNodeKeyhandlers()
{
    CTreeCtrl& om_Tree = GetTreeCtrl();
    HTREEITEM hSelectedItem = om_Tree.GetSelectedItem();
    HTREEITEM hParent = om_Tree.GetParentItem(hSelectedItem);
    //Assuming a node as selected
    CString omStrSimSysName = om_Tree.GetItemText(hParent);
    CString omStrNodeName = om_Tree.GetItemText(hSelectedItem);
    CString omStrMenuText = _T("");
    CSimSysNodeInfo* pSimsysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();

    if (m_pomSubMenu != NULL)
    {
        m_pomSubMenu->GetMenuString(IDM_NODE_ERRORHANDLERS , omStrMenuText , MF_BYCOMMAND);
    }

    if(omStrMenuText == _T("Enable Key Handlers") )
    {
        // when user wants to enable all handlers
        pSimsysNodeInfo->vSetEnableNodeKeyHandlers(omStrSimSysName ,
                omStrNodeName , TRUE);
    }
    else if(omStrMenuText == _T("Disable Key Handlers") )
    {
        pSimsysNodeInfo->vSetEnableNodeKeyHandlers(omStrSimSysName ,
                omStrNodeName , FALSE);
    }
    else
    {
        ASSERT(FALSE);
    }
}
/******************************************************************************/
/*  Function Name    :  OnNodeMessagehandlers                                 */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Enables all the message handlers of the dll under the node.
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  27.12.2005                                            */
/*  Modification     :
/******************************************************************************/
void CSimSysTreeView::OnNodeMessagehandlers()
{
    CSimSysNodeInfo* pSimSysNodeInfo = CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    CTreeCtrl& om_Tree = GetTreeCtrl();
    HTREEITEM hSelectedItem = om_Tree.GetSelectedItem();
    CString omStrNodeName = STR_EMPTY;
    CString omStrSimSysName = STR_EMPTY;
    HTREEITEM hParent = om_Tree.GetParentItem(hSelectedItem);
    omStrSimSysName = om_Tree.GetItemText(hParent);
    omStrNodeName = om_Tree.GetItemText(hSelectedItem);
    CString omStrMenuText = STR_EMPTY;
    sSIMSYSINFO* pSimsys = NULL;
    sNODEINFO* pNode = NULL;
    CSimSysDetView* pSimsysDetView = NULL;
    pSimsysDetView =
        CSimSysManager::ouGetSimSysManager(m_eBus).podGetSimSysDetView();
    CSimSysNodeInfo* pTempSimsys = NULL;
    pTempSimsys = CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    m_pomSubMenu->GetMenuString(IDM_NODE_MESSAGEHANDLERS , omStrMenuText , MF_BYCOMMAND);

    if(omStrMenuText == "Enable Message Handlers" )
    {
        // when user wants to enable all handlers
        pSimSysNodeInfo->vSetEnableNodeMsgHandlers(omStrSimSysName ,
                omStrNodeName , TRUE);
    }
    else
    {
        pSimSysNodeInfo->vSetEnableNodeMsgHandlers(omStrSimSysName ,
                omStrNodeName , FALSE);
    }

    if ( pSimsysDetView != NULL )
    {
        pSimsys = pTempSimsys->psGetSimSysPointer(omStrSimSysName);
        pNode = pTempSimsys->psGetSimSysNodePointer(omStrSimSysName, omStrNodeName );

        if ( pNode != NULL ) //if it is node
        {
            // check if the controls in the form view are hidden
            if ( pSimsysDetView->bGetControlStatus() == TRUE )//  TRUE - Hidden
            {
                // Controls are hidden, unhide
                pSimsysDetView->vHideControls(SW_SHOW);
            }

            // Fill the node details for this node
            pSimsysDetView->vDisplayNodeInformation( pNode );
        }
    }
}
/******************************************************************************/
/*  Function Name    :  OnNodeTimerhandlers                                   */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Enables all the timer handlers of the dll under the node.
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  27.12.2005                                            */
/*  Modification     :
/******************************************************************************/
void CSimSysTreeView::OnNodeTimerhandlers()
{
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    CTreeCtrl& om_Tree = GetTreeCtrl();
    HTREEITEM hSelectedItem = om_Tree.GetSelectedItem();
    CString omStrNodeName = STR_EMPTY;
    CString omStrSimSysName = STR_EMPTY;
    HTREEITEM hParent = om_Tree.GetParentItem(hSelectedItem);
    omStrSimSysName = om_Tree.GetItemText(hParent);
    omStrNodeName = om_Tree.GetItemText(hSelectedItem);
    CString omStrMenuText = STR_EMPTY;
    sSIMSYSINFO* pSimsys = NULL;
    sNODEINFO* pNode = NULL;
    CSimSysDetView* pSimsysDetView = NULL;
    pSimsysDetView =
        CSimSysManager::ouGetSimSysManager(m_eBus).podGetSimSysDetView();
    CSimSysNodeInfo* pTempSimsys = NULL;
    pTempSimsys = CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    m_pomSubMenu->GetMenuString(IDM_NODE_TIMERHANDLERS , omStrMenuText , MF_BYCOMMAND);

    if(omStrMenuText == _T("Enable Timer Handlers") )
    {
        // when user wants to enable all handlers
        pSimSysNodeInfo->vSetEnableNodeTimerHandlers(omStrSimSysName ,
                omStrNodeName , TRUE);
    }
    else
    {
        pSimSysNodeInfo->vSetEnableNodeTimerHandlers(omStrSimSysName ,
                omStrNodeName , FALSE);
    }

    if ( pSimsysDetView != NULL )
    {
        pSimsys = pTempSimsys->psGetSimSysPointer(omStrSimSysName);
        pNode = pTempSimsys->psGetSimSysNodePointer(omStrSimSysName, omStrNodeName );

        if ( pNode != NULL ) //if it is node
        {
            // check if the controls in the form view are hidden
            if ( pSimsysDetView->bGetControlStatus() == TRUE )//  TRUE - Hidden
            {
                // Controls are hidden, unhide
                pSimsysDetView->vHideControls(SW_SHOW);
            }

            // Fill the node details for this node
            pSimsysDetView->vDisplayNodeInformation( pNode );
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
void CSimSysTreeView::OnAddnode()
{
    vAddEditNode(FALSE);
}
/******************************************************************************/
/*  Function Name    :  OnSimsysAllErrorhandlers                              */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Enables all the error handlers of all the dlls under the
                        simulated system.
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  27.12.2005                                            */
/*  Modification     :
/******************************************************************************/
void CSimSysTreeView::OnSimsysAllErrorhandlers()
{
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    CTreeCtrl& om_Tree = GetTreeCtrl();
    HTREEITEM hSelectedItem = om_Tree.GetSelectedItem();
    CString omStrSimSysName = om_Tree.GetItemText(hSelectedItem);
    CString omStrMenuText = STR_EMPTY;
    m_pomSubMenu->GetMenuString(IDM_SIMSYS_ALLERRORHANDLERS , omStrMenuText
                                , MF_BYCOMMAND);

    if(omStrMenuText == _T("Enable All Error Handlers") )
    {
        // when user wants to enable all handlers
        pSimSysNodeInfo->vSetEnableAllSimSysErrorHandlers(omStrSimSysName , TRUE);
    }
    else
    {
        pSimSysNodeInfo->vSetEnableAllSimSysErrorHandlers(omStrSimSysName , FALSE);
    }
}
/******************************************************************************/
/*  Function Name    :  OnSimsysAllhandlers                                   */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Enables all the handlers of all the dlls under the
                        simulated system.
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  27.12.2005                                            */
/*  Modification     :
/******************************************************************************/
void CSimSysTreeView::OnSimsysAllhandlers()
{
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    CTreeCtrl& om_Tree = GetTreeCtrl();
    HTREEITEM hSelectedItem = om_Tree.GetSelectedItem();
    CString omStrSimSysName = om_Tree.GetItemText(hSelectedItem);
    CString omStrMenuText = STR_EMPTY;
    m_pomSubMenu->GetMenuString(IDM_SIMSYS_ALLHANDLERS , omStrMenuText , MF_BYCOMMAND);

    if(omStrMenuText == _T("Enable All Handlers") )
    {
        // when user wants to enable all handlers
        pSimSysNodeInfo->vSetEnableAllSimSysHandlers(omStrSimSysName , TRUE);
    }
    else
    {
        pSimSysNodeInfo->vSetEnableAllSimSysHandlers(omStrSimSysName , FALSE);
    }
}
/******************************************************************************/
/*  Function Name    :  OnSimsysAllKeyhandlers                                */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Enables all the key handlers of all the dlls under the
                        simulated system.
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  27.12.2005                                            */
/*  Modification     :
/******************************************************************************/
void CSimSysTreeView::OnSimsysAllKeyhandlers()
{
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    CTreeCtrl& om_Tree = GetTreeCtrl();
    HTREEITEM hSelectedItem = om_Tree.GetSelectedItem();
    CString omStrSimSysName = om_Tree.GetItemText(hSelectedItem);
    CString omStrMenuText = STR_EMPTY;
    m_pomSubMenu->GetMenuString(IDM_SIMSYS_ALLKEYHANDLERS , omStrMenuText
                                , MF_BYCOMMAND);

    if(omStrMenuText == "Enable All Key Handlers" )
    {
        // when user wants to enable all handlers
        pSimSysNodeInfo->vSetEnableAllSimSysKeyHandlers(omStrSimSysName , TRUE);
    }
    else
    {
        pSimSysNodeInfo->vSetEnableAllSimSysKeyHandlers(omStrSimSysName , FALSE);
    }
}
/******************************************************************************/
/*  Function Name    :  OnSimsysAllMsghandlers                                */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Enables all the msg handlers of all the dlls under the
                        simulated system.
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  27.12.2005                                            */
/*  Modification     :
/******************************************************************************/
void CSimSysTreeView::OnSimsysAllMsghandlers()
{
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    CTreeCtrl& om_Tree = GetTreeCtrl();
    HTREEITEM hSelectedItem = om_Tree.GetSelectedItem();
    CString omStrSimSysName = om_Tree.GetItemText(hSelectedItem);
    CString omStrMenuText = STR_EMPTY;
    m_pomSubMenu->GetMenuString(IDM_SIMSYS_ALLMSGHANDLERS , omStrMenuText
                                , MF_BYCOMMAND);

    if(omStrMenuText == _T("Enable All Message Handlers") )
    {
        // when user wants to enable all handlers
        pSimSysNodeInfo->vSetEnableAllSimSysMsgHandlers(omStrSimSysName , TRUE);
    }
    else
    {
        pSimSysNodeInfo->vSetEnableAllSimSysMsgHandlers(omStrSimSysName , FALSE);
    }
}
/******************************************************************************/
/*  Function Name    :  OnSimsysAllTimerhandlers                              */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Enables all the timer handlers of the all the dlls
                        under the simulated system.
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  27.12.2005                                            */
/*  Modification     :
/******************************************************************************/
void CSimSysTreeView::OnSimsysAllTimerhandlers()
{
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    CTreeCtrl& om_Tree = GetTreeCtrl();
    HTREEITEM hSelectedItem = om_Tree.GetSelectedItem();
    CString omStrSimSysName = om_Tree.GetItemText(hSelectedItem);
    CString omStrMenuText = STR_EMPTY;
    m_pomSubMenu->GetMenuString(IDM_SIMSYS_ALLTIMERHANDLERS , omStrMenuText
                                , MF_BYCOMMAND);

    if(omStrMenuText == _T("Enable All Timer Handlers") )
    {
        // when user wants to enable all handlers
        pSimSysNodeInfo->vSetEnableAllSimSysTimerHandlers(omStrSimSysName );
    }
    else
    {
        pSimSysNodeInfo->vSetDisableAllSimSysTimerHandlers(omStrSimSysName );
    }
}

/******************************************************************************/
/*  Function Name    :  OnSimsysBuildall                                      */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Builds all the C files under the selected simulated system.
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  27.12.2005                                            */
/*  Modification     :  Anish Kumar
/******************************************************************************/
void CSimSysTreeView::OnSimsysBuildall()
{
    CTreeCtrl& om_Tree = GetTreeCtrl();
    HTREEITEM hSelectedItem = om_Tree.GetSelectedItem();
    CString omStrSimSysName = om_Tree.GetItemText(hSelectedItem);
    CString omStrFileName = STR_EMPTY;
    int nNodeCount = 0;
    int nFailure = 0;
    BOOL bSuccess = FALSE;
    CStringArray omStrBuildFiles;
    omStrBuildFiles.RemoveAll();
    PSSIMSYSINFO pTempSimsys =
        CSimSysManager::ouGetSimSysManager(m_eBus).psReturnSimsysInfoPtr();
    CExecuteManager::ouGetExecuteManager(m_eBus).vClearOutputWnd();

    while(pTempSimsys != NULL)
    {
        nNodeCount += pTempSimsys->m_unNumberOfNodesAdded;

        if(pTempSimsys->m_omStrSimSysName == omStrSimSysName)
        {
            PSNODELIST pTempNode = pTempSimsys->m_psNodesList;

            while(pTempNode != NULL)
            {
                if(pTempNode->m_sNodeInfo.m_omStrFileName != "")
                {
                    CFunctionEditorDoc* pDoc = CGlobalObj::ouGetObj(m_eBus).podGetFunctionEditorDoc();

                    if ( pDoc != NULL)
                    {
                        //TO find which all files are opened through Framewnd
                        CMDIFrameWnd* pMainWnd = (CMDIFrameWnd*)CWnd::FromHandle(CGlobalObj::sm_hWndMDIParentFrame);

                        if(pMainWnd != NULL)
                        {
                            //Get the active child wnd
                            CWnd* pWndTemp = pMainWnd->GetActiveFrame();

                            if(pWndTemp != NULL)
                            {
                                CWnd* pWnd = pWndTemp;

                                do
                                {
                                    CString omStrWndName;
                                    CString omStrFileName = pTempNode->m_sNodeInfo.m_omStrFileName;
                                    int nIndex = omStrFileName.ReverseFind(defCHAR_PATH_SEPRATER);
                                    int nLength = omStrFileName.GetLength();
                                    omStrFileName = omStrFileName.Right(nLength - nIndex -1);
                                    pWnd->GetWindowText(omStrWndName);

                                    //if the file is opened save it
                                    if(!(omStrFileName.Compare(omStrWndName)))
                                    {
                                        CDocument*(pDoc) = CGlobalObj::ouGetObj(m_eBus).m_pEditorDocTemplate->OpenDocumentFile(pTempNode->m_sNodeInfo.m_omStrFileName);

                                        if(pDoc!=NULL)
                                        {
                                            // If file name is not empty generate new def file
                                            pDoc->OnSaveDocument(pTempNode->m_sNodeInfo.m_omStrFileName);
                                        }

                                        break;
                                    }

                                    pWnd = pWnd->GetNextWindow();
                                }
                                while((pWndTemp != pWnd) && pWnd != NULL);
                            }
                        }
                    }

                    bSuccess = CExecuteManager::ouGetExecuteManager(m_eBus).bExecuteDllBuild(&
                               pTempNode->m_sNodeInfo);

                    if(!bSuccess)
                    {
                        nFailure++;
                        omStrBuildFiles.Add(pTempNode->m_sNodeInfo.m_omStrFileName);
                    }
                    else  // if the build is successfull
                    {
                        CString omStrPrevDllName = STR_EMPTY;
                        omStrPrevDllName = pTempNode->m_sNodeInfo.m_omStrDllName;
                        omStrFileName = pTempNode->m_sNodeInfo.m_omStrFileName;
                        omStrFileName.Replace( defDOT_SMALL_C , defDOT_DLL );
                        pTempNode->m_sNodeInfo.m_omStrDllName = omStrFileName;
                        bPopulateTree();
                    }
                }

                pTempNode = pTempNode->m_psNextNode;
            }

            pTempSimsys = pTempSimsys->m_psSimsysNext;
        }
        else
        {
            pTempSimsys = pTempSimsys->m_psSimsysNext;
        }
    }

    if((nFailure == 0) || (nFailure != nNodeCount))
    {
        // if the build is successfull atleast for one.
        bPopulateTree();
    }

    if(nFailure > 0)  // if the build is not successfull
    {
        CString omStrErrorMsg =_T("Following file(s) are not properly build:");
        CString omStrErrorMsgDummy=_T("");

        for(int i = 0 ; i < nFailure; i++)
        {
            omStrErrorMsgDummy.Format(_T("\n%s"), omStrBuildFiles.GetAt(i));
            omStrErrorMsg += omStrErrorMsgDummy;
        }

        AfxMessageBox(omStrErrorMsg);
    }
}
/******************************************************************************/
/*  Function Name    :  OnDeleteAllNodes                                      */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Deletes all the nodes from the tree view and
                        from the data structure under a simulated system.
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  28.02.2005                                            */
/*  Modification     :
/******************************************************************************/
void CSimSysTreeView::OnDeleteAllNodes()
{
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    BOOL bSuccess = FALSE;
    BOOL bDataSet = FALSE;
    CTreeCtrl& om_tree = GetTreeCtrl();
    HTREEITEM hSimSysItem = om_tree.GetSelectedItem();
    CString omStrSimSysName = STR_EMPTY;
    omStrSimSysName = om_tree. GetItemText(hSimSysItem);
    // First check if the nodes can be deleted or not.
    // only if the dlls are not loaded it can be deleted.
    BOOL bIsDelValid = pSimSysNodeInfo->bIsUnLoadAllValid(omStrSimSysName);

    if(bIsDelValid)
    {
        AfxMessageBox(_T("Unload all the dlls and try again!!"));
    }
    else
    {
        int nReturnVal = AfxMessageBox(ALL_SIMSYS_DELETE_CONFMN, MB_YESNO|MB_ICONQUESTION);

        if ( nReturnVal == IDYES )
        {
            bSuccess = pSimSysNodeInfo->bDeleteNodeFromSimSys(omStrSimSysName , STR_EMPTY);

            if(bSuccess)
            {
                while(om_tree.ItemHasChildren(hSimSysItem))
                {
                    HTREEITEM hNodeItem = om_tree.GetChildItem(hSimSysItem);
                    om_tree.DeleteItem(hNodeItem);
                }

                //Set the modify flag
                PSSIMSYSINFO pSimsysInfo = pSimSysNodeInfo->psGetSimSysPointer(omStrSimSysName);

                if( pSimsysInfo != NULL )
                {
                    pSimsysInfo->m_bIsSimSysModified = TRUE;
                }
            }
        }
    }
}
/******************************************************************************/
/*  Function Name    :  OnDeleteSimulatedsystem                               */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Deletes the selected simulated system from the tree view and
                        from the data structure.
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  28.02.2005                                            */
/*  Modification     :
/******************************************************************************/
void CSimSysTreeView::OnDeleteSimulatedsystem()
{
    CTreeCtrl& om_tree = GetTreeCtrl();
    // for simsys array purpose
    CSimSysConfigDetails* pSimsysConfig = CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysConfig();

    if(pSimsysConfig == NULL)
    {
        return;
    }

    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();
    HTREEITEM hItem = om_tree.GetSelectedItem();
    CString omStrSelecetedText = om_tree.GetItemText(hItem);
    BOOL bIsDllsLoaded = pSimSysNodeInfo->bIsUnLoadAllValid(omStrSelecetedText);

    if(bIsDllsLoaded)
    {
        AfxMessageBox(_T("Unload all the dlls and try again!!"));
    }
    else
    {
        if ( hItem != NULL )
        {
            //If there is change in the system then save it
            vSaveSimsSysIfModified(omStrSelecetedText);
            // Delete the simulated system from the data structure
            BOOL bSuccess = pSimSysNodeInfo->bDeleteSimsysFromInfo(omStrSelecetedText);

            // delete
            if (!bSuccess)
            {
                AfxMessageBox(_T("Could not delete seleceted Simsys!"),
                              MB_OK|MB_ICONINFORMATION);
            }
            else
            {
                m_hTreeItem = NULL;
                // Get previous sibbling item
                HTREEITEM hPrevItem = om_tree.GetPrevSiblingItem( hItem );
                omStrSimsysToBeDeleted = om_tree.GetItemText(hItem);
                // Remove the item from the tree
                om_tree.DeleteItem( hItem );

                if ( hPrevItem != NULL )
                {
                    // Select prev item
                    om_tree.SelectItem( hPrevItem );
                    CString omStrPrev = om_tree.GetItemText( hPrevItem );
                }// no simsys left
                else
                {
                    // Select root item
                    om_tree.SelectItem(om_tree.GetRootItem());
                }
            }
        }
    }
}

/******************************************************************************
 Function Name    :

 Input(s)         :
 Output           :
 Functionality    :
 Member of        :  CSimSysTreeView
 Friend of        :      -

 Author(s)        :  Anish Kr.
 Date Created     :  02.03.2009
 Modification By  :
/******************************************************************************/
void CSimSysTreeView::vSaveSimsSysIfModified(CString omSimSysName)
{
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();

    if (pSimSysNodeInfo != NULL)
    {
        PSSIMSYSINFO psSimSys = pSimSysNodeInfo->psGetSimSysPointer(omSimSysName);

        if (psSimSys != NULL)
        {
            if (psSimSys->m_bIsSimSysModified)
            {
                int nReturn = AfxMessageBox(ASK_SIMSYS_SAVE_PROMPT,
                                            MB_YESNO|MB_ICONQUESTION);

                if (nReturn == IDYES)
                {
                    CSimSysConfigDetails* pSimsysConfig =
                        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysConfig();

                    if (pSimsysConfig)
                    {
                        pSimsysConfig->nSaveConfiguration(omSimSysName, psSimSys);
                    }
                }
            }
        }
    }
}
/******************************************************************************/
/*  Function Name    :  OnSimsysLoadall                                       */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Loads all the dlls under the selected simulated system.
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  27.12.2005                                            */
/*  Modification By  :  Anish kr
/*  Date             :  31.10.2006,Remove code for changing Dll name(File's name.dll)
/*                      in node's detail struct on simply loading
/******************************************************************************/
void CSimSysTreeView::OnSimsysLoadall()
{
    CTreeCtrl& om_Tree = GetTreeCtrl();
    HTREEITEM hSelectedItem = om_Tree.GetSelectedItem();
    CString omStrSimSysName = om_Tree.GetItemText(hSelectedItem);
    CString omStrFileName = STR_EMPTY;
    int nNodeCount = 0;
    CStringArray omStrLoadFiles;
    omStrLoadFiles.RemoveAll();
    BOOL bSuccess = FALSE;
    int nFailure = 0;
    PSSIMSYSINFO pTempSimsys =
        CSimSysManager::ouGetSimSysManager(m_eBus).psReturnSimsysInfoPtr();
    CExecuteManager::ouGetExecuteManager(m_eBus).vClearOutputWnd();

    while(pTempSimsys != NULL)
    {
        nNodeCount += pTempSimsys->m_unNumberOfNodesAdded;

        if(pTempSimsys->m_omStrSimSysName == omStrSimSysName)
        {
            PSNODELIST pTempNode = pTempSimsys->m_psNodesList;

            while(pTempNode != NULL)
            {
                if(pTempNode->m_sNodeInfo.m_omStrDllName != "")
                {
                    bSuccess = CExecuteManager::ouGetExecuteManager(m_eBus).bExecuteDllLoad(&
                               pTempNode->m_sNodeInfo);

                    if(!bSuccess)
                    {
                        nFailure++;
                        omStrLoadFiles.Add(pTempNode->m_sNodeInfo.m_omStrDllName);
                    }
                    else
                    {
                        // to make the status of the handlers disabled
                        pTempNode->m_sNodeInfo.m_bDllHandlersEnabled = FALSE;
                        pTempNode->m_sNodeInfo.m_bErrorHandlersEnabled = FALSE;
                        pTempNode->m_sNodeInfo.m_bIsAllHandlersEnabled = FALSE;
                        pTempNode->m_sNodeInfo.m_bKeyHandlersEnabled = FALSE;
                        pTempNode->m_sNodeInfo.m_bMsgHandlersEnabled = FALSE;
                        pTempNode->m_sNodeInfo.m_bTimerHandlersEnabled = FALSE;
                    }
                }

                pTempNode = pTempNode->m_psNextNode;
            }

            pTempSimsys = pTempSimsys->m_psSimsysNext;
        }
        else
        {
            pTempSimsys = pTempSimsys->m_psSimsysNext;
        }
    }

    if((nFailure == 0) || (nFailure != nNodeCount))
    {
        // if the load is successfull atleast for one dll.
        bPopulateTree();
    }

    if(nFailure > 0)  // if the load is not successfull
    {
        CString omStrErrorMsg =_T("Following file(s) are not properly loaded:");
        CString omStrErrorMsgDummy=_T("");

        for(int i = 0 ; i < nFailure; i++)
        {
            omStrErrorMsgDummy.Format(_T("\n%s"), omStrLoadFiles.GetAt(i));
            omStrErrorMsg += omStrErrorMsgDummy;
        }

        AfxMessageBox(omStrErrorMsg);
    }
}
/******************************************************************************/
/*  Function Name    :  OnSimsysSave                                          */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called when the user wants to save the configuration  */
/*                      information into the file. The method calls the       */
/*                      CSimSysConfigDetails nSaveConfiguration(..) method to save
/*                      information.                                          */
/*                      If there is any error, then a message box is displayed*/
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  28.12.2005                                            */
/*  Modifications    :
/******************************************************************************/
void CSimSysTreeView::OnSimsysSave()
{
    CSimSysConfigDetails* pSimsysConfig =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysConfig();
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();

    if((pSimsysConfig != NULL) && (pSimSysNodeInfo))
    {
        CTreeCtrl& omTree = GetTreeCtrl();
        HTREEITEM hItem = omTree.GetSelectedItem();
        // get the name of the loaded config file only if no filename has
        // been specified until now..
        CString omStrPathName = omTree.GetItemText(hItem);
        PSSIMSYSINFO psSimSys = pSimSysNodeInfo->psGetSimSysPointer(omStrPathName);

        if( psSimSys != NULL )
        {
            pSimsysConfig->nSaveConfiguration(omStrPathName, psSimSys );
        }
    }
}
/******************************************************************************/
/*  Function Name    :  OnSimsysSaveAs                                        */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called when the user wants to save the configuration  */
/*                      information into a new file.
/*                      If there is any error, then a message box is displayed*/
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  28.12.2005                                            */
/*  Modifications    :
/******************************************************************************/
void CSimSysTreeView::OnSimsysSaveAs()
{
    //AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CTreeCtrl& omTree = GetTreeCtrl();
    HTREEITEM hItem = omTree.GetSelectedItem();
    CString oCfgFilename = STR_EMPTY;
    // get the name of the loaded config file only if no filename has
    // been specified until now..
    CString omStrPathName = omTree.GetItemText(hItem);
    CSimSysNodeInfo* pSimSysNodeInfo =
        CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();

    if(pSimSysNodeInfo != NULL)
    {
        CFileDialog oCfgFileDlg(FALSE,      // Open dialog as Save as File dlg
                                defSIMSYSCONFEXT,               // default extension
                                NULL,                           // default file name
                                defDLGFLAGS ,                   // mode
                                defSIMSYSCONFIGFILTER,          // filter
                                NULL                            // parent wnd
                               );
        oCfgFileDlg.m_ofn.lpstrTitle = _T("SaveAs Configuration Filename...");

        if(oCfgFileDlg.DoModal() == IDOK)
        {
            // get the name of the selected file
            oCfgFilename = oCfgFileDlg.GetPathName();
            bCallFrmSaveAs = TRUE;
            BOOL bReturn = pSimSysNodeInfo->bIsSimSysPresent( oCfgFilename );

            if(! bReturn )
            {
                if(oCfgFilename.IsEmpty() == FALSE)
                {
                    COPYFILE(omStrPathName, oCfgFilename);
                }
            }
            else
            {
                AfxMessageBox(_T("The simulated system is already present.Cannot replace the file"));
            }
        }
    }
}
/******************************************************************************/
/*  Function Name    :  OnSimsysUnloadall                                     */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  UnLoads all the dlls under the selected simulated system.
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  27.12.2005                                            */
/*  Modification By  :  Anish kr
/*  Date             :  31.10.2006,Remove code for changing Dll name(File's name.dll)
/*                      in node's detail struct on simply unloading
/******************************************************************************/
void CSimSysTreeView::OnSimsysUnloadall()
{
    CTreeCtrl& om_Tree = GetTreeCtrl();
    HTREEITEM hSelectedItem = om_Tree.GetSelectedItem();
    CString omStrSimSysName = om_Tree.GetItemText(hSelectedItem);
    CString omStrFileName = STR_EMPTY;
    int nNodeCount = 0;
    CStringArray omStrUnLoadFiles;
    omStrUnLoadFiles.RemoveAll();
    BOOL bSuccess = FALSE;
    int nFailure = 0;
    PSSIMSYSINFO pTempSimsys =
        CSimSysManager::ouGetSimSysManager(m_eBus).psReturnSimsysInfoPtr();
    CExecuteManager::ouGetExecuteManager(m_eBus).vClearOutputWnd();

    while(pTempSimsys != NULL)
    {
        nNodeCount += pTempSimsys->m_unNumberOfNodesAdded;

        if(pTempSimsys->m_omStrSimSysName == omStrSimSysName)
        {
            PSNODELIST pTempNode = pTempSimsys->m_psNodesList;

            while(pTempNode != NULL)
            {
                if(pTempNode->m_sNodeInfo.m_bIsDllLoaded)  // if any dll is loaded
                {
                    bSuccess = CExecuteManager::ouGetExecuteManager(m_eBus).bExecuteDllUnload(&
                               pTempNode->m_sNodeInfo);

                    if(!bSuccess)
                    {
                        nFailure++;
                        omStrUnLoadFiles.Add(pTempNode->m_sNodeInfo.m_omStrDllName);
                    }
                    else
                    {
                        // to make the status of the handlers disabled
                        pTempNode->m_sNodeInfo.m_bDllHandlersEnabled   = FALSE;
                        pTempNode->m_sNodeInfo.m_bErrorHandlersEnabled = FALSE;
                        pTempNode->m_sNodeInfo.m_bIsAllHandlersEnabled = FALSE;
                        pTempNode->m_sNodeInfo.m_bKeyHandlersEnabled   = FALSE;
                        pTempNode->m_sNodeInfo.m_bMsgHandlersEnabled   = FALSE;
                        pTempNode->m_sNodeInfo.m_bTimerHandlersEnabled = FALSE;
                    }
                }

                pTempNode = pTempNode->m_psNextNode;
            }

            pTempSimsys = pTempSimsys->m_psSimsysNext;
        }
        else
        {
            pTempSimsys = pTempSimsys->m_psSimsysNext;
        }
    }

    if((nFailure == 0) || (nFailure != nNodeCount))
    {
        // if the unload is successfull atleast for one dll.
        bPopulateTree();
    }

    if(nFailure > 0)  // if the load is not successfull
    {
        CString omStrErrorMsg = _T("Following file(s) are not properly unloaded:");
        CString omStrErrorMsgDummy = _T("");

        for(int i = 0 ; i < nFailure; i++)
        {
            omStrErrorMsgDummy.Format(_T("\n%s"), omStrUnLoadFiles.GetAt(i));
            omStrErrorMsg += omStrErrorMsgDummy;
        }

        AfxMessageBox(omStrErrorMsg);
    }
}
/******************************************************************************/
/*  Function Name    :  bSetSimsysName                                        */
/*                                                                            */
/*  Input(s)         :  CString omStrSimsysName                               */
/*  Output           :                                                        */
/*  Functionality    :  Sets the name of the new simsys which is created.
                        under root.
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  28.02.2005                                            */
/*  Modifications    :
/******************************************************************************/
BOOL CSimSysTreeView::bSetSimsysName(CString& omStrSimsysName)
{
    CTreeCtrl& om_tree = GetTreeCtrl();
    int bFlag = TRUE;
    HTREEITEM hChildItem = NULL;
    m_hRootItem = om_tree.GetRootItem();

    if (om_tree.ItemHasChildren(m_hRootItem))
    {
        hChildItem = om_tree.GetChildItem(m_hRootItem);
    }

    HTREEITEM hItem = om_tree.GetSelectedItem();

    if ( hItem != NULL || hChildItem != NULL)
    {
        // for new simsys, insert the new simsys name under root
        if ( hItem == om_tree.GetRootItem() )
        {
            HTREEITEM hNewItem = om_tree.InsertItem( omStrSimsysName, hItem );
            m_omSimsysCount++;
            // select new simsys
            om_tree.SelectItem(hNewItem);
        }
    }
    else
    {
        bFlag = FALSE;
    }

    return bFlag;
}


/******************************************************************************/
/*  Function Name    :  OnSimsysSaveAll                                       */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called when the user wants to save all the sim sys config
/*                      information into the file. The method calls the
/*                      CSimSysConfigDetails nSaveConfiguration(..) method to save           */
/*                      information.                                          */
/*                      If there is any error, then a message box is displayed*/
/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  28.12.2005                                            */
/*  Modifications    :
/******************************************************************************/
void CSimSysTreeView::OnSimsysSaveAll()
{
    CSimSysManager::ouGetSimSysManager(m_eBus).vSaveAllSimSys();
}


/******************************************************************************/
/*  Function Name    :  vSetDllName                                           */
/*                                                                            */
/*  Input(s)         :  CString omStrPrevDllName , CString omStrNewDllName    */
/*  Output           :                                                        */
/*  Functionality    :  Sets the name of the edited item on the tree view

/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika M                                              */
/*  Date Created     :  12.1.2006                                             */
/*  Modifications    :
/******************************************************************************/
void CSimSysTreeView::vSetDllName(CString& omStrPrevDllName , CString& omStrNewDllName )
{
    CTreeCtrl& om_tree = GetTreeCtrl();
    HTREEITEM hItem = om_tree.GetSelectedItem();
    HTREEITEM hChild = om_tree.GetChildItem( hItem );

    if ( (hChild != NULL) && (!omStrNewDllName.IsEmpty()))
    {
        CString omStrDllName = om_tree.GetItemText( hChild );

        if( omStrDllName == omStrPrevDllName )
        {
            om_tree.SetItemText( hChild, omStrNewDllName );
        }
    }
    else if( (hChild == NULL) && (!omStrNewDllName.IsEmpty()))
    {
        // when no dll was associated before
        om_tree.InsertItem( omStrNewDllName , hItem );
    }
    else if ( (hChild != NULL) && (omStrNewDllName.IsEmpty()))
    {
        // when new dll name is empty
        om_tree.DeleteItem( hChild );
    }

    // to set the focus to the tree view so that the changes are visible.
    CSimSysTreeView* pSimSysTreeView =
        CSimSysManager::ouGetSimSysManager(m_eBus).podGetSimSysTreeView();
    pSimSysTreeView->SetFocus();
}
/******************************************************************************/
/*  Function Name    :  vSetSimSysDllName                                     */
/*                                                                            */
/*  Input(s)         :  CString omStrPrevDllName , CString omStrNewDllName    */
/*  Output           :                                                        */
/*  Functionality    :  Sets the name of the dll in Bold if dll is loaded else
                        normal font in the tree view

/*  Member of        :  CSimSysTreeView                                       */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :
/*  Date Created     :
/*  Modifications    :
/******************************************************************************/
void CSimSysTreeView::vSetSimSysDllName(CString& omStrNodeName ,
                                        CString& omStrNewDllName ,
                                        BOOL bIsDllLoaded)
{
    if (!omStrNewDllName.IsEmpty())
    {
        CTreeCtrl& om_tree = GetTreeCtrl();
        HTREEITEM hItem = om_tree.GetSelectedItem();  // Node

        if (hItem)
        {
            CString str = om_tree.GetItemText(hItem);

            if( str == omStrNodeName)
            {
                HTREEITEM hGChild = om_tree.GetChildItem( hItem );

                if ( hGChild != NULL)
                {
                    //If already dll name child present change its name
                    om_tree.SetItemText( hGChild, omStrNewDllName );
                }
                else
                {
                    //If no child add new child item
                    om_tree.InsertItem( omStrNewDllName , hItem );
                }

                //Set the item text as bold if the dll is loaded
                if(bIsDllLoaded)
                {
                    om_tree.SetItemState( hGChild, TVIS_BOLD, TVIS_BOLD );
                }
                else
                {
                    om_tree.SetItemState( hGChild, FALSE, TVIS_BOLD );
                }

                // to set the focus to the tree view so that the changes are visible.
                SetFocus();
            }
        }
    }
}

/******************************************************************************/
/*  Function Name    :  hInsertAndSelectItem
/*
/*  Input(s)         :  HTREEITEM hParent, CString omItemName
/*  Output           :
/*  Functionality    :  Insert an item and set it selected
/*  Member of        :  CSimSysTreeView
/*  Friend of        :      -
/*
/*  Author(s)        :  Anish Kr.
/*  Date Created     :  26.02.09
/******************************************************************************/
HTREEITEM CSimSysTreeView::hInsertAndSelectItem(CString omItemName, HTREEITEM hParent )
{
    HTREEITEM hItem = NULL;
    CTreeCtrl& om_tree = GetTreeCtrl();
    hItem = om_tree.InsertItem(omItemName, hParent);

    if (hItem)
    {
        om_tree.SelectItem(hItem);
    }

    return hItem;
}

/******************************************************************************/
/*  Function Name    :  psGetCurrentSimSysInfo
/*
/*  Input(s)         :
/*  Output           :
/*  Functionality    :  It returns the pointer of simsys under which current
                        selected node is present
/*  Member of        :  CSimSysTreeView
/*  Friend of        :      -
/*
/*  Author(s)        :  Anish Kr.
/*  Date Created     :  26.02.09
/******************************************************************************/
PSSIMSYSINFO CSimSysTreeView::psGetCurrentSimSysInfo()
{
    HTREEITEM hSelItem = NULL;
    PSSIMSYSINFO psCurrentSimsys = NULL;
    CTreeCtrl& om_tree = GetTreeCtrl();
    hSelItem = om_tree.GetSelectedItem();

    if (hSelItem != NULL)
    {
        HTREEITEM hParenSys = om_tree.GetParentItem(hSelItem);

        if (hParenSys != NULL)
        {
            CString omSimSys = om_tree.GetItemText(hParenSys);
            CSimSysNodeInfo* psNodeInfo =
                CSimSysManager::ouGetSimSysManager(m_eBus).pomGetSimSysNodeInfo();

            if (psNodeInfo != NULL)
            {
                psCurrentSimsys =  psNodeInfo->psGetSimSysPointer(omSimSys);
            }
        }
    }

    return psCurrentSimsys;
}

