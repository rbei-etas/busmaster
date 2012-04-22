/*******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  GraphChildFrm.cpp
  Description   :  Implementation file for CGraphChildFrame class
  $Log:   X:/Archive/Sources/SigGrphWnd/GraphChildFrame.cpv  $
   
      Rev 1.6   06 Sep 2011 20:09:22   CANMNTTM
   CANvas Name is changed to BUSMASTER
   
      Rev 1.5   08 Aug 2011 13:06:04   CANMNTTM
   Updated function 'vUserCommand(..)'with following changes:
   1. Improve readability by inserting a space after a keyword, using braces in if .... else statements.
   2. Exlored the two variable usage with if ... else and made more straightforward.
   
      Rev 1.4   25 Jul 2011 11:44:54   CANMNTTM
   Updated to make the graph clearing done only in two scenarios:
   1. When initially connected.
   2. DIsconnected and connected back.
   
      Rev 1.3   15 Apr 2011 19:43:30   rac2kor
   Inserted RBEI Copyright information text into the file header.
   
      Rev 1.2   01 Feb 2011 18:04:58   CANMNTTM
   Updated vHandleConFigChange() with call to
   vInsertSignalData() function in bottom view.
   
      Rev 1.1   14 Dec 2010 17:08:36   CANMNTTM
   Updated functionality to solve the 
   default window positioning issue.
   
      Rev 1.0   13 Dec 2010 22:00:34   CANMNTTM
    
   
      Rev 1.1   18 Aug 2010 18:59:42   CANMNTTM
   Graph related configuration saving functions have been implemented
   
      Rev 1.0   16 Aug 2010 21:20:32   rac2kor
    

  Author(s)     :  Raja N
  Date Created  :  10/12/2004
  Modified By   : 
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved
*******************************************************************************/

// For Standard Includes
#include "SigGrphWnd_stdafx.h"
// For definition of resource symbols
#include "SigGrphWnd_resource.h"
// For CGraphRightView class definition
#include "GraphRightView.h"
// For CGraphBottomView class definition
#include "GraphBottomView.h"
// For CColourPicker class definition
#include "Utility/ColourPicker.h"
// For CMessageList class definition
#include "MessageList.h"
// For CGraphLeftView class definition
#include "GraphLeftView.h"
// For CGraphChildFrame class definition
#include "GraphChildFrame.h"

#include "TimeManager.h"

IMPLEMENT_DYNCREATE(CGraphChildFrame, CMDIChildBase)

BEGIN_MESSAGE_MAP(CGraphChildFrame, CMDIChildBase)
    //{{AFX_MSG_MAP(CGraphChildFrame)
    ON_WM_ERASEBKGND()
    ON_WM_SHOWWINDOW()
    ON_WM_CLOSE()
	ON_MESSAGE(WM_USER_CMD,vUserCommand)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*******************************************************************************
  Function Name  : CGraphChildFrame
  Description    : Standard default constructor
  Member of      : CGraphChildFrame
  Functionality  : This will initialise local variables
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : Raja N on 02.05.2005, Added code to init parent with window
                   identity parameter
*******************************************************************************/
CGraphChildFrame::CGraphChildFrame() : CMDIChildBase( GRAPH_WND_PLACEMENT )
{
    // Init Pointers
    m_pomBottomView = NULL;
    m_pomLeftView = NULL;
    m_pomRightView = NULL;
    m_sSplitterPostion.m_nRootSplitterData[0][0] = -1;
	m_pGraphList = NULL;
	m_pDMGraphCtrl = NULL;
	m_bIsPrevConnected = FALSE;
}

/*******************************************************************************
  Function Name  : ~CGraphChildFrame
  Description    : Standard Destructor
  Member of      : CGraphChildFrame
  Functionality  : -
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
CGraphChildFrame::~CGraphChildFrame()
{
	m_pDMGraphCtrl = NULL;
}

/*******************************************************************************
  Function Name  : PreCreateWindow
  Input(s)       : cs - Reference to the create parameter structure
  Output         : -
  Functionality  : This function will be called before creating the window. This
                   function will change the create parameters if required
  Member of      : CGraphChildFrame
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : Raja N on 02.05.2005, Added code to init parent with window
                   identity parameter
*******************************************************************************/
BOOL CGraphChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    BOOL bCreate = FALSE;
    bCreate = CMDIChildBase::PreCreateWindow(cs);
    // Return the result
    return bCreate;
}

#ifdef _DEBUG
void CGraphChildFrame::AssertValid() const
{
    CMDIChildBase::AssertValid();
}

void CGraphChildFrame::Dump(CDumpContext& dc) const
{
    CMDIChildBase::Dump(dc);
}

#endif //_DEBUG

/*******************************************************************************
  Function Name  : OnCreateClient
  Input(s)       : lpcs - Create Parameters
                   pContext - Pointer to Create Context
  Output         : BOOL - Create Result
  Functionality  : This function will be called to create window. This function
                   will create splitter window and views.
  Member of      : CGraphChildFrame
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : Raja N on 02.05.2005, changed base class to CMDIChildBase
*******************************************************************************/
BOOL CGraphChildFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
    BOOL bReturn = TRUE;
    // Boundary objects
    CSize omSize;
    CRect omRect;
    /*// Update Child Window Pointer
    CMainFrame * pMainFrame = (CMainFrame *)theApp.m_pMainWnd;*/
    // Create Splitter Window
    bReturn = 
        m_wndSplitter.CreateStatic( this,
                            defGRAPH_WINDOW_ROWS, defGRAPH_WINDOW_COLS );
    if( bReturn == TRUE )
    {
        // Get Window Rectangle
        GetWindowRect( &omRect );
        // Make propotionation width
        omSize.cx = (LONG)(omRect.Width() * defLEFT_VIEW_PROPOTION);
        omSize.cy = omRect.Height();

        /*if( pMainFrame != NULL )
        {
            pMainFrame->m_pomGraphChild = this;
        }*/

        // Get Right Column to create sub division
        int nChildID = m_wndSplitter.IdFromRowCol(0, 1);
        // Create sub splitter
        bReturn =  m_wndSecondSplitter.CreateStatic( &m_wndSplitter,
                        defGRAPH_WINDOW_RIGHT_ROWS,
                        defGRAPH_WINDOW_RIGHT_COLS,
                        WS_CHILD | WS_VISIBLE | WS_BORDER,     // Window Style
                        nChildID );
    }
 
    // If second splitter create is success
    if( bReturn == TRUE )
    {
        // Create Graph control view first as other views refers this view
        // Pointer
        bReturn =
            m_wndSecondSplitter.CreateView( 0, 0,
                                        RUNTIME_CLASS(CGraphRightView),
                                        CSize(
                                         (int)(omSize.cx * defRIGHT_VIEW_PROPOTION) ,
                                         (int)(omSize.cy * defGRAPH_VIEW_PROPOSION)) ,
                                        pContext);
    }
    // If Create Graph control view is successful
    if( bReturn == TRUE )
    {
        // Now Create Left View
        bReturn = m_wndSplitter.CreateView( 0, 0, RUNTIME_CLASS(CGraphLeftView),
                                 omSize,pContext);
        if( bReturn == TRUE )
        {
            bReturn = m_wndSecondSplitter.CreateView( 1, 0,
                                        RUNTIME_CLASS(CGraphBottomView),
                                        CSize( 
                                         (int)(omSize.cx * defRIGHT_VIEW_PROPOTION),
                                         (int)(omSize.cy * defBOTTOM_VIEW_PROPOSION) ),
                                        pContext );
        }
    }

    // Check for graph control creation
    //if( bReturn == TRUE && pMainFrame != NULL )
    {
        //if( pMainFrame->m_podGraphControl == NULL ||
        //    IsWindow( pMainFrame->m_podGraphControl->m_hWnd) == FALSE )
        //{
        //    // Show control register message
        //    AfxMessageBox( defSTR_CONTROL_NOT_REGISTERED_MSG, MB_ICONSTOP );
        //    bReturn = FALSE;
        //}
        // Get Splitter information
        vGetSplitterStatus();
        // Set Splitter postion
        vSetSplitterPostion();
    }

    // Return the result
    return bReturn;
}

/*******************************************************************************
  Function Name  :  vUserCommand                                          
  Input(s)       :  WPARAM wParam : enumerator eUSERSELCTION              
                    LPARAM lParam:  NOT USED                              
  Output         :                                                        
  Functionality  :  This function is message mapped and will be called    
					from BUSMASTER application module during connect/disconnect.					  
  Member of      :	CGraphChildFrame
  Author(s)      :	ArunKumar K
  Date Created   :	09.11.2010
  Modifications  :  ArunKumar K, 22.07.2011
					Storing the previous connect state.
*******************************************************************************/
LRESULT CGraphChildFrame::vUserCommand(WPARAM wParam, LPARAM lParam)
{
	if(m_pomBottomView && m_pomLeftView)
	{
		eUSERSELCTION eUserSel = eDATABASEIMPORTCMD;
		eUserSel               = static_cast <eUSERSELCTION>(wParam);
		BOOL bConnect = FALSE;

		switch(eUserSel)
        {
			case eCONNECTCMD:
			{
				bConnect = (BOOL)lParam;
				vHandleConnectChange(bConnect);
				((CGraphBottomView*)m_pomBottomView)->vHandleConnectionStatusChange(bConnect);
				((CGraphLeftView*)m_pomLeftView)->vHandleConnectionStatusChange(bConnect);								

				if ( bConnect )
				{
					if ( !m_bIsPrevConnected )
					{
						// Update Absolute time value
						CTimeManager::vInitAbsoluteTime();
						m_bIsPrevConnected = TRUE;
					}
				}					
				else
				{
					m_bIsPrevConnected = FALSE;
				}
			}
			break;
			case eCONFIGCHANGECMD:
			{
				vHandleConFigChange();
			}
			break;
		}
	}
	return 0;
}
/*******************************************************************************
  Function Name  : OnEraseBkgnd
  Input(s)       : pDC - Pointer to Device context
  Output         : -
  Functionality  : This function will simply return true to avoid flickering due
                   to background erase
  Member of      : CGraphChildFrame
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : 
*******************************************************************************/
BOOL CGraphChildFrame::OnEraseBkgnd(CDC* /*pDC*/) 
{
    return TRUE;
}

/*******************************************************************************
  Function Name  : OnShowWindow
  Input(s)       : bShow - Show or Hide
                   nStatus - Specifies the status of the window being shown
  Output         : -
  Functionality  : This function is Overridden to update main frame to update
                   the status of the graph child window
  Member of      : CGraphChildFrame
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : Raja N on 02.05.2005, changed base class to CMDIChildBase
*******************************************************************************/
void CGraphChildFrame::OnShowWindow(BOOL bShow, UINT nStatus) 
{
    CMDIChildBase::OnShowWindow(bShow, nStatus);
    // Send Notification to the Parent
    GetMDIFrame()->SendMessage(WM_NOTIFICATION_FROM_OTHER, eWNDID_GRAPHWND, bShow);
}

/*******************************************************************************
  Function Name  : OnClose
  Input(s)       : -
  Output         : -
  Functionality  : This function will hide the window instead of destroy. This
                   window will be maintained invisibly and will be shown again
                   on selection of Graph Window from Main Menu
  Member of      : CGraphChildFrame
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : 
*******************************************************************************/
void CGraphChildFrame::OnClose() 
{
    // Don't destroy. But Hide
    ShowWindow( SW_HIDE );
}

/*******************************************************************************
  Function Name  : pomGetLeftViewPointer
  Input(s)       : -
  Output         : CView* - Pointer to the left view
  Functionality  : This function will return left view pointer. Left View
                   pointer will be initialised from CGraphLeftView class after
                   creation
  Member of      : CGraphChildFrame
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : 
*******************************************************************************/
CView* CGraphChildFrame::pomGetLeftViewPointer()
{
    // Return the pointer. Calling function should handle NULL values
    return m_pomLeftView;
}

/*******************************************************************************
  Function Name  : pomGetRightTopViewPointer
  Input(s)       : -
  Output         : CView* - Pointer to the Right view
  Functionality  : This function will return right view pointer. Right View
                   pointer will be initialised from CGraphRightView class after
                   creation
  Member of      : CGraphChildFrame
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : 
*******************************************************************************/
CView* CGraphChildFrame::pomGetRightTopViewPointer()
{
    // Return the pointer. Calling function should handle NULL values
    return m_pomRightView;
}

/*******************************************************************************
  Function Name  : pomGetRightBottomViewPointer
  Input(s)       : -
  Output         : CView* - Pointer to the Bottom view
  Functionality  : This function will return bottom view pointer. Bottom View
                   pointer will be initialised from CGraphBottomView class after
                   creation
  Member of      : CGraphChildFrame
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : 
*******************************************************************************/
CView* CGraphChildFrame::pomGetRightBottomViewPointer()
{
    // Return the pointer. Calling function should handle NULL values
    return m_pomBottomView;
}

/*******************************************************************************
  Function Name  : vHandleConnectChange
  Input(s)       : bConnect - Connection Status
  Output         : -
  Functionality  : This function handles connection change event. This will
                   update all views with the connection status
  Member of      : CGraphChildFrame
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : 
*******************************************************************************/
void CGraphChildFrame::vHandleConnectChange(BOOL bConnect)
{
    // Get Left View Pointer
    CGraphLeftView * pLeftView = ( CGraphLeftView *)
        pomGetLeftViewPointer();
    // Update Left View
    pLeftView->vHandleConnectionStatusChange( bConnect );
    // Get Bottom View Pointer
    CGraphBottomView * pBottomView = (CGraphBottomView *)
        pomGetRightBottomViewPointer();
    // Update Bottom View
    pBottomView->vHandleConnectionStatusChange( bConnect );    }

/*******************************************************************************
  Function Name  : vHandleConFigChange
  Input(s)       : -
  Output         : -
  Functionality  : This function handles config change event. This will inform
                   all views about the change
  Member of      : CGraphChildFrame
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : 
*******************************************************************************/
void CGraphChildFrame::vHandleConFigChange()
{
    // Get Left View Pointer
    CGraphLeftView * pLeftView = ( CGraphLeftView *)
        pomGetLeftViewPointer();
    // Update Left View
    // Use Send Message to avoid assertion because of DDX Update Data
    pLeftView->SendMessage( WM_CONFIG_CHANGE );
    // Get Bottom View Pointer
    CGraphBottomView * pBottomView = (CGraphBottomView *)
        pomGetRightBottomViewPointer();
    // Update Bottom View
    // Use Send Message to avoid assertion because of DDX Update Data
    pBottomView->SendMessage( WM_CONFIG_CHANGE );
	pBottomView->vInsertSignalData();
    // Nothing to do with top view
}

/*******************************************************************************
  Function Name  : vSetSplitterPostion
  Input(s)       : -
  Output         : -
  Functionality  : This function will set splitter postion. This will refer
                   member variable m_sSplitterPostion to get splitter postion
  Member of      : CGraphChildFrame
  Author(s)      : Raja N
  Date Created   : 19.4.2005
  Modifications  : 
*******************************************************************************/
void CGraphChildFrame::vSetSplitterPostion()
{
    // Set Parent Splitter Postion
    // Column 0
    m_wndSplitter.SetColumnInfo( 0,
                    m_sSplitterPostion.m_nRootSplitterData[0][0],
                    m_sSplitterPostion.m_nRootSplitterData[0][1]);
    // Column 1
    m_wndSplitter.SetColumnInfo( 1,
                    m_sSplitterPostion.m_nRootSplitterData[1][0],
                    m_sSplitterPostion.m_nRootSplitterData[1][1]);

    m_wndSplitter.RecalcLayout();
    // Set Right sub division position
    // Row 0
    m_wndSecondSplitter.SetRowInfo( 0,
                    m_sSplitterPostion.m_nRightViewSplitterData[0][0],
                    m_sSplitterPostion.m_nRightViewSplitterData[0][1]);
    // Row 1
    m_wndSecondSplitter.SetRowInfo( 1,
                    m_sSplitterPostion.m_nRightViewSplitterData[1][0],
                    m_sSplitterPostion.m_nRightViewSplitterData[1][1]);

    m_wndSecondSplitter.RecalcLayout();
}

/*******************************************************************************
  Function Name  : vSaveSplitterPostion
  Input(s)       : -
  Output         : -
  Functionality  : This function will save splitter postion. This will get
                   current splitter postion and will compare with
                   m_sSplitterPostion. If current position is different it will
                   update member varaible and configuration module.
  Member of      : CGraphChildFrame
  Author(s)      : Raja N
  Date Created   : 19.4.2005
  Modifications  : 
*******************************************************************************/
void CGraphChildFrame::vSaveSplitterPostion()
{
    // Get Splitter Window Postion and save it
    SGRAPHSPLITTERDATA sPostion;
    m_wndSplitter.GetColumnInfo( 0,
                                 sPostion.m_nRootSplitterData[0][0],
                                 sPostion.m_nRootSplitterData[0][1]);
    // Column 1
    m_wndSplitter.GetColumnInfo( 1,
                                 sPostion.m_nRootSplitterData[1][0],
                                 sPostion.m_nRootSplitterData[1][1]);
    // Set Right sub division position
    // Row 0
    m_wndSecondSplitter.GetRowInfo( 0,
                                 sPostion.m_nRightViewSplitterData[0][0],
                                 sPostion.m_nRightViewSplitterData[0][1]);
    // Row 1
    m_wndSecondSplitter.GetRowInfo( 1,
                                 sPostion.m_nRightViewSplitterData[1][0],
                                 sPostion.m_nRightViewSplitterData[1][1]);
    // Do memory compare before storing the postion
    if( memcmp( &m_sSplitterPostion, 
                &sPostion,
                sizeof(SGRAPHSPLITTERDATA)) != 0)
    {
        // Update Local copy
        m_sSplitterPostion = sPostion;
        // Save it in configuration file
        //theApp.bSetData( GRAPH_WND_SPLITTER_DATA, (void * )&m_sSplitterPostion);
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
void CGraphChildFrame::vUpdateWndCo_Ords()
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
void CGraphChildFrame::vUpdateWinStatus()
{
    // Call parent window function for setting window status
    CMDIChildBase::vUpdateWinStatus();
    // Get Splitter postion from configuration file
    vGetSplitterStatus();
    // Set Splitter Postion
    vSetSplitterPostion();
}
BOOL CGraphChildFrame::bGetDefaultSplitterPostion(CRect& omWndSize, 
                                                  SGRAPHSPLITTERDATA& sGraphSplitterData)
{
    sGraphSplitterData.m_nRootSplitterData[0][0] =
        (int)(omWndSize.Width() * defLEFT_VIEW_PROPOTION);
    sGraphSplitterData.m_nRootSplitterData[0][1] = 0;
    
    sGraphSplitterData.m_nRootSplitterData[1][0] =
        (int)(omWndSize.Width() * defRIGHT_VIEW_PROPOTION);
    sGraphSplitterData.m_nRootSplitterData[1][1] = 0;
    
    sGraphSplitterData.m_nRightViewSplitterData[0][0] =
        (int)(omWndSize.Height() * defGRAPH_VIEW_PROPOSION);
    sGraphSplitterData.m_nRightViewSplitterData[0][1] = 0;
    
    sGraphSplitterData.m_nRightViewSplitterData[1][0] =
        (int)(omWndSize.Height() * defGRAPH_VIEW_PROPOSION);
    sGraphSplitterData.m_nRightViewSplitterData[1][1] = 0;
    return TRUE;
}
/*******************************************************************************
  Function Name  : vGetSplitterStatus
  Input(s)       : -
  Output         : -
  Functionality  : This function will get splitter information from
                   configuration module
  Member of      : CGraphChildFrame
  Author(s)      : Raja N
  Date Created   : 19.4.2005
  Modifications  : 
*******************************************************************************/
void CGraphChildFrame::vGetSplitterStatus()
{
    // Check for initial condition
    if( m_sSplitterPostion.m_nRootSplitterData[0][0] == -1 )
    {
        CRect omWndSize;
        GetWindowRect( omWndSize );
        bGetDefaultSplitterPostion(omWndSize, m_sSplitterPostion );
    }
}

void CGraphChildFrame::vSetSplitterConfig(SGRAPHSPLITTERDATA& sSplitterPostion)
{
    // Check for initial condition
    m_sSplitterPostion = sSplitterPostion;
    if( m_sSplitterPostion.m_nRootSplitterData[0][0] == -1 )
    {
        CRect omWndSize;
        GetWindowRect( omWndSize );
        bGetDefaultSplitterPostion(omWndSize, m_sSplitterPostion );
    }
	vSetSplitterPostion();
}
void CGraphChildFrame::vGetSplitterConfig(SGRAPHSPLITTERDATA& sSplitterPostion)
{
	vUpdateWndCo_Ords();
    vGetSplitterStatus();
    sSplitterPostion = m_sSplitterPostion;
}

/*******************************************************************************
  Function Name  : vSetSignalListDetails
  Input(s)       : pGraphList
  Output         : -
  Functionality  : This function initialize the signal list pointer variable.
  Member of      : CGraphChildFrame
  Author(s)      : ArunKumar K
  Date Created   : 02.11.2010
  Modifications  : 
*******************************************************************************/
void CGraphChildFrame::vSetSignalListDetails(CGraphList* pGraphList)
{
	if(pGraphList!=NULL)
		m_pGraphList = pGraphList;

}

/*******************************************************************************
  Function Name  : pGetSignalListDetails
  Input(s)       : -
  Output         : CGraphList*
  Functionality  : This function gets the signal list details pointer.
  Member of      : CGraphChildFrame
  Author(s)      : ArunKumar K
  Date Created   : 02.11.2010
  Modifications  : 
*******************************************************************************/
CGraphList* CGraphChildFrame::pGetSignalListDetails()
{
	if(m_pGraphList!=NULL)
		return m_pGraphList;
	return NULL;
}