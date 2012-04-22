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
 * \file      EditFrameWnd.cpp
 * \brief     This file contain definition of all function of 
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of 
 */


#include "NodeSimEx_stdafx.h"         // standard header include file
#include "EditFrameWnd.h"   // Class definition file
#include "FileView.h"       // C fileview defintion file
#include "FnsTreeView.h"    // Function tree view defintion file
#include "FunctionView.h"   // Editable function view defintion file
#include "editframewnd.h"

ETYPE_BUS CFunctionView::sm_eBus = CAN;
ETYPE_BUS CFnsTreeView::sm_eBus  = CAN;
IMPLEMENT_DYNCREATE(CEditFrameWnd, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CEditFrameWnd, CMDIChildWnd)
    //{{AFX_MSG_MAP(CEditFrameWnd)
        // NOTE - the ClassWizard will add and remove mapping macros here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG_MAP
    ON_WM_CREATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditFrameWnd construction/destruction
/******************************************************************************/
/*  Function Name    :  CEditFrameWnd                                         */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Constructor
/*  Member of        :  CEditFrameWnd                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  05.03.2002                                            */
/*  Modifications    :  
/******************************************************************************/
CEditFrameWnd::CEditFrameWnd()
{
    m_bIsSplitterCreated = FALSE;
    m_eBus = sm_eBus;
}
/******************************************************************************/
/*  Function Name    :  ~CEditFrameWnd                                        */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Constructor
/*  Member of        :  CEditFrameWnd                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  15.02.2002                                            */
/*  Modifications    :  
/******************************************************************************/
CEditFrameWnd::~CEditFrameWnd()
{
}
/******************************************************************************/
/*  Function Name    :  PreCreateWindow                                       */
/*                                                                            */
/*  Input(s)         :  CREATESTRUCT& cs                                      */
/*  Output           :                                                        */
/*  Functionality    :  Window style is specified
/*  Member of        :  CEditFrameWnd                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  05.03.2002                                            */
/*  Modifications    :  
/******************************************************************************/
BOOL CEditFrameWnd::PreCreateWindow(CREATESTRUCT& cs)
{
    if( !CMDIChildWnd::PreCreateWindow(cs) )
        return FALSE;

    // Child window style
    cs.style =  WS_CHILD            | 
                WS_VISIBLE          | 
                WS_OVERLAPPEDWINDOW | 
                WS_CAPTION          | 
                WS_SYSMENU          | 
                FWS_ADDTOTITLE;

    return TRUE;
}
/******************************************************************************/
/*  Function Name    :  ActivateFrame                                         */
/*                                                                            */
/*  Input(s)         :  int nCmdShow                                          */
/*  Output           :                                                        */
/*  Functionality    :  Maximizes the window initially
/*  Member of        :  CEditFrameWnd                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  05.03.2002                                            */
/*  Modifications    :  14.03.2002, Amarnath Shastry
/******************************************************************************/
void CEditFrameWnd::ActivateFrame(int nCmdShow)
{
    nCmdShow = SW_SHOWNORMAL;
    CMDIChildWnd::ActivateFrame(nCmdShow);
}


#ifdef _DEBUG
void CEditFrameWnd::AssertValid() const
{
    CMDIChildWnd::AssertValid();
}

void CEditFrameWnd::Dump(CDumpContext& dc) const
{
    CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEditFrameWnd message handlers
/******************************************************************************/
/*  Function Name    :  OnCreateClient                                        */
/*                                                                            */
/*  Input(s)         :  LPCREATESTRUCT lpcs, CCreateContext* pContext         */
/*  Output           :                                                        */
/*  Functionality    :  This function will create a vertical and horizontal
                        splitter and associates 3 views for each pane.
/*  Member of        :  CEditFrameWnd                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  05.03.2002                                            */
/*  Modifications    :  22.03.2002 Amarnath Shastry
                        Changed the positon of splitter window                        
/******************************************************************************/
BOOL CEditFrameWnd::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext) 
{
    // Create Static Splitter Window with 1 ROW and 2 COLUMNS
    m_bIsSplitterCreated = 
        m_omSplitterWndOne.CreateStatic(this,           // Parent Frame Window 
                                        SPLT_ONE_ROWS,  // #Rows
                                        SPLT_ONE_COLS); // #Columns
    if ( m_bIsSplitterCreated == TRUE )
    {
        // Get size of Frame wnd
        CSize om_Size(0,0);

        vCalculateSplitterPos( om_Size );
        CFnsTreeView::sm_eBus = m_eBus;
        // Create view for the Right top Pane for static splitter window
        m_bIsSplitterCreated = 
            m_omSplitterWndOne.CreateView(0,    // #Row
                                          0,    // #Column      
                                          RUNTIME_CLASS(CFnsTreeView),
                                          om_Size,      // Sizeof Pane
                                          pContext);

        if ( TRUE == m_bIsSplitterCreated )
        {
            // Get child window ID from 0th row and 1st column
            int nChildID = m_omSplitterWndOne.IdFromRowCol(0, 1);

            m_bIsSplitterCreated = 
                m_omSplitterWndTwo.CreateStatic( &m_omSplitterWndOne,                   // Parent Frame Window 
                                                 SPLT_TWO_ROWS, // #Rows
                                                 SPLT_TWO_COLS, // #Columns,                        // #Columns
                                                 WS_CHILD   | 
                                                 WS_VISIBLE | 
                                                 WS_BORDER,     // Window Style
                                                 nChildID);     // Splitter  Nested 
            
            if ( TRUE == m_bIsSplitterCreated )
            {
                CFileView::sm_eBus = m_eBus;
                CSize om_Size2(0,0);
                om_Size2.cy = ((om_Size.cy) / 2);
                // Create view for the Left Pane for static splitter window 2
                m_bIsSplitterCreated = 
                    m_omSplitterWndTwo.CreateView( 0,// #Row
                                                   0,// #Column
                                                   RUNTIME_CLASS(CFileView),
                                                   om_Size2,// Sizeof Pane
                                                   pContext);

                if ( TRUE == m_bIsSplitterCreated )
                {
                    om_Size2.cy = 0;
                    CFunctionView::sm_eBus = m_eBus;
                    // Create view for the bottom Pane for static splitter window 2
                    m_bIsSplitterCreated    =   
                        m_omSplitterWndTwo.CreateView( 1,// #Row
                                                       0,// #Column     
                                                       RUNTIME_CLASS(CFunctionView),
                                                       om_Size2,// Sizeof Pane
                                                       pContext);
                    if ( m_bIsSplitterCreated != TRUE )
                    {
                        // Failed to create view 2 for splitter 2, 
                        //close child window
                        // return m_bIsSplitterCreated;
                    }
                }
                else
                {
                    // Failed to create view 1 for splitter two, 
                    // close child window
                    // return m_bIsSplitterCreated;
                }
            }
            else
            {
                // Failed to create splitter two, close child window
                // return m_bIsSplitterCreated;
            }
        }
        else
        {
            // Failed to create view for splitter one, close child window
            // return m_bIsSplitterCreated;
        }
    }
    else
    {
        // Failed to create splitter one, close child window
        // return m_bIsSplitterCreated;
    }
    return m_bIsSplitterCreated;
}
/******************************************************************************/
/*  Function Name    :  vCalculateSplitterPos                                 */
/*                                                                            */
/*  Input(s)         :  CSize &size                                           */
/*  Output           :                                                        */
/*  Functionality    :  Calculates the splitter position
/*  Member of        :  CEditFrameWnd                                         */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  05.03.2002                                            */
/*  Modifications    :  22.03.2002, Amarnath Shastry
                        Changes in size.cy
/******************************************************************************/
void CEditFrameWnd::vCalculateSplitterPos(CSize &size)
{
    size.cx = 0;
    size.cy = 0;
    RECT rRect;
    // Get window rectangle co-ordinates
    GetWindowRect( &rRect );
    size.cx = ( rRect.right/5 );
    size.cy = ( rRect.bottom );
}

int CEditFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
        return -1;
    SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_FN_EDITOR), TRUE);

    return 0;
}



