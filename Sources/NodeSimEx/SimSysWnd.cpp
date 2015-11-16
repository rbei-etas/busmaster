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
 * \file      SimSysWnd.cpp
 * \brief     This file contain definition of all function of
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of
 */

#include "NodeSimEx_stdafx.h"   // Contains standard include header files.
#include "HashDefines.h"
#include "SimSysWnd.h"          // Class defintion included here
#include "SimSysManager.h"

#define TOOLBAR_BUTTON_SIZE 36
#define TOOLBAR_ICON_SIZE 32
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ETYPE_BUS CSimSysTreeView::sm_eBus = CAN;
/////////////////////////////////////////////////////////////////////////////
// CSimSysWnd

IMPLEMENT_DYNCREATE(CSimSysWnd, CMDIChildBase)
/******************************************************************************/
/*  Function Name    :  CSimSysWnd                                            */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Constructor
/*  Member of        :  CSimSysWnd                                            */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika.M                                              */
/*  Date Created     :  15.12.2005                                            */
/*  Modifications    :
/******************************************************************************/
CSimSysWnd::CSimSysWnd(ETYPE_BUS eBus) : CMDIChildBase( SIMSYS_WND_PLACEMENT )
{
    m_bSplitWndCreated = FALSE; // Splitter not created yet!
    m_eBus  = eBus;
}
/******************************************************************************/
/*  Function Name    :  ~CSimSysWnd                                           */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Destructor
/*  Member of        :  CSimSysWnd                                            */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika.M                                              */
/*  Date Created     :  15.12.2005                                            */
/*  Modifications    :
/******************************************************************************/
CSimSysWnd::~CSimSysWnd()
{
    //Free Advanced UI Library used to load icons for toolbar.
    if (m_hModAdvancedUILib )
    {
        ::FreeLibrary(m_hModAdvancedUILib);
        m_hModAdvancedUILib = nullptr;
    }
}


BEGIN_MESSAGE_MAP(CSimSysWnd, CMDIChildWnd)
    //{{AFX_MSG_MAP(CSimSysWnd)
    ON_WM_CLOSE()
    ON_WM_DESTROY()
    //}}AFX_MSG_MAP
    ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimSysWnd message handlers
/***********************************************************************************/
/*  Function Name    :  PreCreateWindow
/*
/*  Input(s)         :  cs - Reference to the create parameter structure                                      */
/*  Output           :
/*  Functionality    :  This function will be called before creating the window. This
function will change the create parameters if required.
/*  Member of        :  CSimSysWnd
/*  Friend of        :      -
/*
/*  Author(s)        :  Harika.M
/*  Date Created     :  15.02.2005
/*  Modifications    :
/************************************************************************************/

BOOL CSimSysWnd::PreCreateWindow(CREATESTRUCT& cs)
{
    BOOL bCreate = FALSE;
    bCreate = CMDIChildBase::PreCreateWindow(cs);
    // Return the result
    return bCreate;
}
/******************************************************************************/
/*  Function Name    :  OnCreateClient                                        */
/*                                                                            */
/*  Input(s)         :  LPCREATESTRUCT lpcs, CCreateContext* pContext         */
/*  Output           :  BOOL, True or False                                   */
/*  Functionality    :  This function will create a vertical splitter
                        and associates 2 views for each pane.
/*  Member of        :  CSimSysWnd                                            */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Harika.M                                              */
/*  Date Created     :  15.12.2005                                            */
/*  Modifications    :
/******************************************************************************/
BOOL CSimSysWnd::OnCreateClient(LPCREATESTRUCT , CCreateContext* pContext)
{
    // Create Static Splitter Window with 1 ROW and 2 COLUMNS
    m_bSplitWndCreated  =
        m_omSplitterWnd.CreateStatic(   this,                   // Parent Frame Window
                                        SPLT_ONE_ROWS,          // #Rows
                                        SPLT_TWO_COLS,          // #Columns -- Its Single column
                                        WS_CHILD    |
                                        WS_VISIBLE |
                                        WS_BORDER,              // Window Style
                                        AFX_IDW_PANE_FIRST);    // Splitter NOT Nested


    // Get size of Frame wnd
    CSize om_Size(0,0);



    if ( TRUE == m_bSplitWndCreated )
    {
        // Create the Left Pane for static splitter window
        CSimSysTreeView::sm_eBus = m_eBus;
        m_bSplitWndCreated  =
            m_omSplitterWnd.CreateView( 0,                      // #Row
                                        0,                      // #Column
                                        RUNTIME_CLASS(CSimSysTreeView), // View associated
                                        om_Size,             //CSize( 350,100 )Sizeof Pane
                                        pContext);
    }

    // Create Toolbar
    nCreateToolbar();
    return m_bSplitWndCreated;
}
/******************************************************************************/
/*  Function Name    :  OnClose                                               */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This will just hide the window so that it will be shown
                        later on request. It will be destroyed on close of
                        application

/*  Member of        :  CSimSysWnd                                            */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Anish
/*  Date Created     :
/*  Modification     :                                                        */
/******************************************************************************/

void CSimSysWnd::OnClose()
{
    CMDIChildBase::ShowWindow(SW_HIDE);
}


/*******************************************************************************
Function Name  : vUpdateWndCo_Ords
Input(s)       : -
Output         : -
Functionality  : This function will save window postion in to configuration
module using parent class function & will also save splitter
window postion.
Member of      : CSimSysWnd
Author(s)      : Harika M
Date Created   : 19.12.2005
Modifications  :
*******************************************************************************/
void CSimSysWnd::vUpdateWndCo_Ords()
{
    // Call parent to save window postion
    CMDIChildBase::vUpdateWndCo_Ords();
}

/*******************************************************************************
Function Name  : vUpdateWinStatus
Input(s)       : -
Output         : -
Functionality  : This function will set window postion and splitter postion.
For setting window postion this will use parent class
function and then will set splitter position.
Member of      : CSimSysWnd
Author(s)      : Harika M
Date Created   : 19.12.2005
Modifications  :
*******************************************************************************/
void CSimSysWnd::vUpdateWinStatus()
{
    // Call parent window function for setting window status
    CMDIChildBase::vUpdateWinStatus();
}


void CSimSysWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CMDIChildBase::OnShowWindow(bShow, nStatus);
    if (bShow)
    {
        CSimSysTreeView* pTempSimSysTreeView =
            (CSimSysTreeView*)m_omSplitterWnd.GetPane(0,0);
        if (pTempSimSysTreeView->IsKindOf(RUNTIME_CLASS(CSimSysTreeView)))
        {
            pTempSimSysTreeView->bPopulateTree();
        }
    }
}

/*******************************************************************************
Function Name  : nCreateToolbar
Input(s)       : -
Output         : -
Functionality  : Creates toolbar for Simulated System Configuration Window.
Member of      : CSimSysWnd
Author(s)      : Robin G.K.
Date Created   : 23.10.14
Modifications  :
*******************************************************************************/
int CSimSysWnd::nCreateToolbar()
{
    int Result = 0;
    m_hModAdvancedUILib = nullptr;
    if (!m_toolbar.CreateEx(this,TBSTYLE_FLAT,WS_BORDER | WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER |
                            CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(0,0,0,0), IDR_NODE_SIMULATION) ||
            !m_toolbar.LoadToolBar(IDR_NODE_SIMULATION))
    {
        TRACE0("Failed to create toolbar\n");
        Result =  -1;      // fail to create
    }
    // Set the toolbar button size
    CToolBarCtrl& Toolbar = m_toolbar.GetToolBarCtrl();
    m_hModAdvancedUILib = ::LoadLibrary("AdvancedUIPlugIn.dll");
    BOOL B = FALSE;
    if (m_hModAdvancedUILib)
    {
        CSize objSize;
        //creates toolbar with button size 36 pixel and icon size 32 pixel
        objSize.cx = TOOLBAR_BUTTON_SIZE;
        objSize.cy = TOOLBAR_BUTTON_SIZE;
        Toolbar.SetButtonSize(objSize);
        m_toolbar.bLoadCNVTCToolBar(TOOLBAR_ICON_SIZE, IDB_NODE_SIMULATION,IDB_NODE_SIMULATION_HOT, IDB_NODE_SIMULATION_DISABLED);
    }

    return Result;
}
