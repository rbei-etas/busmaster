/*******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  GraphRightView.cpp
  Description   :  Implementation file for CGraphRightView class
  $Log:   X:/Archive/Sources/SigGrphWnd/GraphRightView.cpv  $
   
      Rev 1.2   15 Apr 2011 19:43:32   rac2kor
   Inserted RBEI Copyright information text into the file header.
   
      Rev 1.1   23 Dec 2010 18:04:44   CANMNTTM
   Updated to support cursors feature in 
   graph control.
   
      Rev 1.0   13 Dec 2010 22:00:36   CANMNTTM
    
   
      Rev 1.0   16 Aug 2010 21:20:32   rac2kor
    

  Author(s)     :  Raja N
  Date Created  :  10/12/2004 
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved
*******************************************************************************/
// For Standard Includes
#include "SigGrphWnd_stdafx.h"
// for Resource Symbol definitions
#include "SigGrphWnd_Resource.h"
// For Right View Definition
#include "GraphRightView.h"
#include ".\graphrightview.h"
#include "GraphBottomView.h"

IMPLEMENT_DYNCREATE(CGraphRightView, CFormView)

/*******************************************************************************
  Function Name  : CGraphRightView
  Description    : Standard default constructor
  Member of      : CGraphRightView
  Functionality  : This will initialise local variables
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
CGraphRightView::CGraphRightView()
    : CFormView(CGraphRightView::IDD)
{
    //{{AFX_DATA_INIT(CGraphRightView)
    //}}AFX_DATA_INIT
	m_pGraphSink = NULL;
	m_dwCookie = 0;
	m_pDMGraphCtrl = NULL;
	m_pWndGraphCtrl = NULL;
}

/*******************************************************************************
  Function Name  : ~CGraphRightView
  Description    : Standard Destructor
  Member of      : CGraphRightView
  Functionality  : -
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
CGraphRightView::~CGraphRightView()
{	
	if (NULL != m_pDMGraphCtrl)
	{
		m_pDMGraphCtrl->Release ();
	}
}

/*******************************************************************************
  Function Name  : DoDataExchange
  Input(s)       : pDX - Pointer to Data Exchange Object
  Output         : -
  Functionality  : This function maps UI controls will associated variables
  Member of      : CGraphRightView
  Author(s)      : Raja N
  Date Created   : 12/12/2004
  Modifications  : 
*******************************************************************************/
void CGraphRightView::DoDataExchange(CDataExchange* pDX)
{
    CWnd * pWnd = NULL;
    pWnd = GetDlgItem(IDC_DMGRAPH_CTRL);
    if( pWnd != NULL )
    {
    CFormView::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CGraphRightView)    
    //}}AFX_DATA_MAP
    }
}


BEGIN_MESSAGE_MAP(CGraphRightView, CFormView)
    //{{AFX_MSG_MAP(CGraphRightView)
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
    //}}AFX_MSG_MAP
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphRightView diagnostics

#ifdef _DEBUG
void CGraphRightView::AssertValid() const
{
    CFormView::AssertValid();
}

void CGraphRightView::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}
#endif //_DEBUG

/*******************************************************************************
  Function Name  : OnSize
  Input(s)       : nType - Specifies the type of resizing requested
                   cx - Specifies the new width of the client area
                   cy - Specifies the new height of the client area
  Output         : -
  Functionality  : The framework calls this member function after the window's
                   size has changed. 
  Member of      : CGraphRightView
  Author(s)      : Raja N
  Date Created   : 12/12/2004
  Modifications  : 
*******************************************************************************/
void CGraphRightView::OnSize(UINT nType, int cx, int cy) 
{
    CFormView::OnSize(nType, cx, cy);
	CWnd * pWnd = NULL;
    pWnd = GetDlgItem(IDC_DMGRAPH_CTRL);
	if ( pWnd && IsWindow(pWnd->m_hWnd) )
	{
        CRect omRect;
        pWnd->GetWindowRect( &omRect );
        // Convert in to logical units
        ScreenToClient( &omRect );
        // Set the width equal to the screen size
        omRect.right = omRect.left + cx;
        omRect.bottom = omRect.top + cy;
        // Set the window boundary
        pWnd->MoveWindow( &omRect );
	}
}

/*******************************************************************************
  Function Name  : OnEraseBkgnd
  Input(s)       : pDC - Pointer to Device Context
  Output         : -
  Functionality  : This function will be called by framewrok to erase the
                   background of the client area. Paint takes care of painting
                   the background so rerutn TRUE.
  Member of      : CGraphRightView
  Author(s)      : Raja N
  Date Created   : 12/12/2004
  Modifications  : 
*******************************************************************************/
BOOL CGraphRightView::OnEraseBkgnd(CDC* /*pDC*/) 
{
    // TODO: Add your message handler code here and/or call default
    return TRUE;
}

/*******************************************************************************
  Function Name  : OnInitialUpdate
  Input(s)       : -
  Output         : -
  Functionality  : Initialises UI components
  Member of      : CGraphRightView
  Author(s)      : Raja N
  Date Created   : 12/12/2004
  Modifications  : 
*******************************************************************************/
void CGraphRightView::OnInitialUpdate() 
{
    // Call parent class handler
    CFormView::OnInitialUpdate();
	AfxEnableControlContainer();

	m_pParentWnd = (CGraphChildFrame*)pomGetParentWindow();

	//Get the CWnd reference to the DMGraph ActiveX control
	m_pWndGraphCtrl = GetDlgItem(IDC_DMGRAPH_CTRL);
	
	LPUNKNOWN pUnk = m_pWndGraphCtrl->GetControlUnknown();	
	pUnk->QueryInterface(IID_IDMGraphCtrl, (void **) &m_pDMGraphCtrl);
	if (  m_pDMGraphCtrl !=NULL )
	{
		m_pParentWnd->m_pDMGraphCtrl = m_pDMGraphCtrl;
	}
	else
	{
		m_pParentWnd->m_pDMGraphCtrl = NULL;
	}
	m_pParentWnd->m_pomRightView = this;
}

/*******************************************************************************
  Function Name  : pomGetParentWindow
  Input(s)       : -
  Output         : CWnd * - Pointer to CGraphChildFrame
  Functionality  : This Function will return parent window pointer. That is
                   pointer to CGraphChildFrame. This will return NULL incase of
                   failure
  Member of      : CGraphRightView
  Author(s)      : ArunKumar K
  Date Created   : 08.11.2010
  Modifications  : 
*******************************************************************************/
CWnd * CGraphRightView::pomGetParentWindow() const
{
    CWnd * pWnd = NULL;
    // Get Splitter window pointer
    pWnd = GetParent();
    // Get CGraphChildFrame pointer from Splitter window pointer
    if( pWnd != NULL )
    {
        pWnd = pWnd->GetParent();
    }
	if( pWnd != NULL )
    {
        pWnd = pWnd->GetParent();
    }

    // Return CGraphChildFrame pointer or NULL incase of failure
    return pWnd;
}
BEGIN_EVENTSINK_MAP(CGraphRightView, CFormView)
END_EVENTSINK_MAP()

void CGraphRightView::CursorPositionGraphCtrl(double X, double Y, short shCursorID)
{
	if(m_pParentWnd)
	{
		((CGraphBottomView*)m_pParentWnd->m_pomBottomView)->
							vUpdateCursordetails(X, Y, shCursorID);
	}
}

void CGraphRightView::OnDestroy()
{
	if ( m_pGraphSink )
	{
		//Get a pointer to sinks IUnknown, no AddRef.
		LPUNKNOWN pUnkSink = m_pGraphSink->GetIDispatch(FALSE);
		
		CWnd * pWnd = NULL;
		pWnd = GetDlgItem(IDC_DMGRAPH_CTRL);
		LPUNKNOWN pUnk = pWnd->GetControlUnknown();
		
		//Terminate a connection between source and sink.
		//m_pUnkSrc is IUnknown of server obtained by CoCreateInstance().
		//m_dwCookie is a value obtained through AfxConnectionAdvise().
		AfxConnectionUnadvise(pUnk, DIID__IDMGraphCtrlEvents, pUnkSink, FALSE, m_dwCookie);

		//Release the sink object
		delete m_pGraphSink;
		m_pGraphSink = NULL;
	}	
	CFormView::OnDestroy();
}

/*******************************************************************************
  Function Name  : vHandleConnectionStatusChange
  Input(s)       : bConnectionStatus - Status of the tool connection. TRUE if
                   the tool is going to connected state
  Output         : -
  Functionality  : This function will handle connect event. Will advise sink class
					on connect and unadvise on disconnect.
  Member of      : CGraphRightView
  Author(s)      : Arunkumar Karri
  Date Created   : 10/12/2012  
*******************************************************************************/
void CGraphRightView::vHandleConnectionStatusChange(BOOL bConnectStatus)
{
	if ( bConnectStatus == TRUE )
	{
		if ( m_pGraphSink == NULL )
		{
			//Create Sink object
			m_pGraphSink = new CGraphSink();

			//Share the Parent window pointer
			m_pGraphSink->m_pParentWnd = m_pParentWnd;
			
			//Get a pointer to sinks IUnknown, no AddRef. CMySink implements only
			//dispinterface and the IUnknown and IDispatch pointers will be same.
			LPUNKNOWN pUnkSink = m_pGraphSink->GetIDispatch(FALSE);	
			
			CWnd * pWnd = NULL;
			pWnd = GetDlgItem(IDC_DMGRAPH_CTRL);
			LPUNKNOWN pUnk = pWnd->GetControlUnknown();

			//Establish a connection between source and sink.
			//m_pUnkSrc is IUnknown of server obtained by CoCreateInstance().
			//m_dwCookie is a cookie identifying the connection, and is needed
			//to terminate the connection.
			AfxConnectionAdvise(pUnk, DIID__IDMGraphCtrlEvents, pUnkSink, FALSE,&m_dwCookie);	
		}
		//Set element point size to 0

		CComPtr<IDMGraphCollection> spGraphCollection;		
		HRESULT hr = m_pDMGraphCtrl->get_Elements(&spGraphCollection);
		if(FAILED(hr))
			return;		
		long lngCount;
		spGraphCollection->get_Count(&lngCount);
        // Add items to the list
        for ( long lngIndex = 0; lngIndex < lngCount; lngIndex++ )
        {            
			CComPtr<IDispatch> spDispatch;
			CComPtr<IDMGraphElement> spGraphElement;
			//Get item at index lngIndex
			spGraphCollection->get_Item(lngIndex, &spDispatch);
			spDispatch.QueryInterface(&spGraphElement);
			spGraphElement->put_PointSize(0);
		}                 
	}
	else
	{
		//Set element point size to 2

		CComPtr<IDMGraphCollection> spGraphCollection;		
		HRESULT hr = m_pDMGraphCtrl->get_Elements(&spGraphCollection);
		if(FAILED(hr))
			return;
		
		long lngCount;
		spGraphCollection->get_Count(&lngCount);
        // Add items to the list
        for ( long lngIndex = 0; lngIndex < lngCount; lngIndex++ )
        {            
			CComPtr<IDispatch> spDispatch;
			CComPtr<IDMGraphElement> spGraphElement;
			//Get item at index lngIndex
			spGraphCollection->get_Item(lngIndex, &spDispatch);
			spDispatch.QueryInterface(&spGraphElement);
			spGraphElement->put_PointSize(2);
		}                  
	}
}
