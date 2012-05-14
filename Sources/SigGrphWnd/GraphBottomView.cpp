/*******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  GraphBottomView.cpp
  Description   :  Implementation file for CGraphBottomView class
  $Log:   X:/Archive/Sources/SigGrphWnd/GraphBottomView.cpv  $

  Author(s)     :  Raja N
  Date Created  :  10/12/2004
  Modified By   :
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved
*******************************************************************************/

// For Standard Includes
#include "SigGrphWnd_stdafx.h"
// For definition of resource symbols
#include "SigGrphWnd_resource.h"
#include "..\Application\CGCtrl.h"
// For CGraphBottomView class definition
#include "GraphBottomView.h"
// For Export Dialog interface
#include "GraphExportDlg.h"
// For Color picker Dialog interface
#include "Utility/ColorSelector.h"
// For Graph configuration Dialog interface
#include "GraphConfiguration.h"
#include "SigGrphWnd_Defines.h"
#include "GraphChildFrame.h"
#include ".\graphbottomview.h"
#include <math.h>
#include "GraphRightView.h"

IMPLEMENT_DYNCREATE(CGraphBottomView, CFormView)

/*******************************************************************************
  Function Name  : CGraphBottomView
  Description    : Standard default constructor
  Member of      : CGraphBottomView
  Functionality  : This will initialise local variables
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : Raja N on 14.12.2004, Implemented review comments. Modified
                   init value of the timer handle to 0 from -1
*******************************************************************************/
CGraphBottomView::CGraphBottomView()
    : CFormView(CGraphBottomView::IDD)
    , m_dblDeltaTime(0)
{
    //{{AFX_DATA_INIT(CGraphBottomView)
    m_nAxis = -1;
    m_nAction = -1;
    m_dRangeFrom = 0.0;
    m_dRangeTo = 0.0;
    //}}AFX_DATA_INIT
    // Initialise Graph control interface pointer
    m_pDMGraphCtrl = NULL;
    // Set update timer Id to invalid
    nTimerID = 0;
    m_dblarrTime[0] = 0;
    m_dblarrTime[1] = 0;
}

/*******************************************************************************
  Function Name  : ~CGraphBottomView
  Description    : Standard Destructor
  Member of      : CGraphBottomView
  Functionality  : -
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
CGraphBottomView::~CGraphBottomView()
{
}

/*******************************************************************************
  Function Name  : DoDataExchange
  Input(s)       : pDX - Pointer to DDX object
  Output         : -
  Functionality  : This function will map DDX object with UI control for data
                   exchange
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphBottomView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CGraphBottomView)
    DDX_Control(pDX, IDC_BTN_RIGHT, m_omBtnRight);
    DDX_Control(pDX, IDC_BTN_LEFT, m_omBtnLeft);
    DDX_Control(pDX, IDC_BTN_DOWN, m_omBtnDown);
    DDX_Control(pDX, IDC_BTN_UP, m_omBtnUp);
    DDX_Radio(pDX, IDC_RADIO_AXIS_X, m_nAxis);
    DDX_Radio(pDX, IDC_RADIO_ACTION_MOVE, m_nAction);
    DDX_Text(pDX, IDC_EDIT_RANGE_FROM, m_dRangeFrom);
    DDX_Text(pDX, IDC_EDIT_RANGE_TO, m_dRangeTo);
    //}}AFX_DATA_MAP
    DDX_Text(pDX, IDC_EDIT_T1, m_dblarrTime[0]);
    DDX_Text(pDX, IDC_EDIT_T2, m_dblarrTime[1]);
    DDX_Text(pDX, IDC_EDIT_DELTA_T, m_dblDeltaTime);
    DDX_Control(pDX, IDC_LIST_SIG_VALUES, m_lstSignalDetails);
}


BEGIN_MESSAGE_MAP(CGraphBottomView, CFormView)
    //{{AFX_MSG_MAP(CGraphBottomView)
    ON_BN_CLICKED(IDC_BTN_EXPORT, OnBtnExport)
    ON_BN_CLICKED(IDC_BTN_AUTO_FIT, OnBtnAutoFit)
    ON_BN_CLICKED(IDC_BTN_CONFIGURE, OnBtnConfigure)
    ON_BN_CLICKED(IDC_BTN_COPY, OnBtnCopy)
    ON_BN_CLICKED(IDC_BTN_GRID, OnBtnGrid)
    ON_BN_CLICKED(IDC_BTN_SET, OnBtnSet)
    ON_BN_CLICKED(IDC_RADIO_ACTION_MOVE, OnRadioActionMove)
    ON_BN_CLICKED(IDC_RADIO_ACTION_NONE, OnRadioActionNone)
    ON_BN_CLICKED(IDC_RADIO_ACTION_TRACK, OnRadioActionTrack)
    ON_BN_CLICKED(IDC_RADIO_ACTION_ZOOM, OnRadioActionZoom)
    ON_BN_CLICKED(IDC_RADIO_AXIS_X, OnRadioAxisX)
    ON_BN_CLICKED(IDC_RADIO_AXIS_XY, OnRadioAxisXy)
    ON_BN_CLICKED(IDC_RADIO_AXIS_Y, OnRadioAxisY)
    ON_BN_CLICKED(IDC_BTN_FIT_ALL, OnBtnFitAll)
    ON_BN_CLICKED(IDC_BTN_UP, OnBtnUp)
    ON_BN_CLICKED(IDC_BTN_LEFT, OnBtnLeft)
    ON_BN_CLICKED(IDC_BTN_DOWN, OnBtnDown)
    ON_BN_CLICKED(IDC_BTN_RIGHT, OnBtnRight)
    ON_WM_TIMER()
    ON_MESSAGE(WM_CONFIG_CHANGE, vHandleConfigFileChange)

    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


#ifdef _DEBUG
void CGraphBottomView::AssertValid() const
{
    CFormView::AssertValid();
}

void CGraphBottomView::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}
#endif //_DEBUG

/*******************************************************************************
  Function Name  : OnInitialUpdate
  Input(s)       : -
  Output         : FALSE - If focus is set to Any UI control explicitly
  Functionality  : Initialises dialog's UI components
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : Raja N on 15.12.2004, Added code to check graph control
                   proper load by checking the window handle
*******************************************************************************/
void CGraphBottomView::OnInitialUpdate()
{
    // Call Parent windows Update
    CFormView::OnInitialUpdate();
    // Load Icons from direction buttons
    m_omBtnUp.SetIcon(AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDI_ICON_UP)));
    m_omBtnDown.SetIcon(AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDI_ICON_DOWN)));
    m_omBtnLeft.SetIcon(AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDI_ICON_LEFT)));
    m_omBtnRight.SetIcon(AfxGetApp()->LoadIcon(MAKEINTRESOURCE(IDI_ICON_RIGHT)));
    //CGraphList * podList = NULL;
    CGraphChildFrame* pParentWnd = NULL;
    pParentWnd = (CGraphChildFrame*)pomGetParentWindow();
    pParentWnd->m_pomBottomView = this;
    // Get Graph Control Pointer
    m_pDMGraphCtrl = pParentWnd->m_pDMGraphCtrl;
    // If window handle is invalid then init pointer with NULL
    /*if( m_opDMGraphCtrl != NULL &&
        IsWindow(m_opDMGraphCtrl->m_hWnd) == FALSE )
    {
        m_opDMGraphCtrl = NULL;
    }*/
    //Update initial values
    m_dblarrTime[0] = dRound(m_dblarrTime[0], 3);
    m_dblarrTime[1] = dRound(m_dblarrTime[1], 3);
    m_dblDeltaTime  = dRound(m_dblDeltaTime , 3);
    // Create List Control
    m_lstSignalDetails.DeleteAllItems();
    //Set List Control styles
    m_lstSignalDetails.SetExtendedStyle
    (LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT  );
    //Create Columns
    vCreateColumns();
}

/*******************************************************************************
  Function Name  : vCreateColumns
  Input(s)       : -
  Output         : -
  Functionality  : Creates list control columns.
  Member of      : CGraphBottomView
  Author(s)      : Arunkumar K
  Date Created   : 22-12-2010
  Modifications  :
*******************************************************************************/
void CGraphBottomView::vCreateColumns()
{
    //Now create all the coloumns
    LVCOLUMN lvcolumn;
    memset(&lvcolumn, 0, sizeof(lvcolumn));
    lvcolumn.mask =  LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
    lvcolumn.fmt = LVCFMT_LEFT;
    CString omTmpColTitle;
    // add columns
    omTmpColTitle = _T("Category");
    lvcolumn.iSubItem = 0;
    lvcolumn.pszText = omTmpColTitle.GetBuffer(MAX_PATH);
    lvcolumn.cchTextMax = (int)_tcslen(omTmpColTitle.GetBuffer(MAX_PATH))*2;
    lvcolumn.cx = nGetListCtrlTextExtent(omTmpColTitle)*2;
    m_lstSignalDetails.InsertColumn(0, &lvcolumn);
    omTmpColTitle = _T("Element");
    lvcolumn.iSubItem = 1;
    lvcolumn.pszText = omTmpColTitle.GetBuffer(MAX_PATH);
    lvcolumn.cchTextMax = (int)_tcslen(omTmpColTitle.GetBuffer(MAX_PATH))*2;
    lvcolumn.cx = nGetListCtrlTextExtent(omTmpColTitle)*2;
    m_lstSignalDetails.InsertColumn(1, &lvcolumn);
    omTmpColTitle = _T("Type");
    lvcolumn.iSubItem = 2;
    lvcolumn.pszText = omTmpColTitle.GetBuffer(MAX_PATH);
    lvcolumn.cchTextMax = (int)_tcslen(omTmpColTitle.GetBuffer(MAX_PATH))*2;
    lvcolumn.cx = nGetListCtrlTextExtent(omTmpColTitle)*2;
    m_lstSignalDetails.InsertColumn(2, &lvcolumn);
    omTmpColTitle = _T("Cursor1");
    lvcolumn.iSubItem = 3;
    lvcolumn.pszText = omTmpColTitle.GetBuffer(MAX_PATH);
    lvcolumn.cchTextMax = (int)_tcslen(omTmpColTitle.GetBuffer(MAX_PATH));
    lvcolumn.cx = nGetListCtrlTextExtent(omTmpColTitle);
    m_lstSignalDetails.InsertColumn(3, &lvcolumn);
    omTmpColTitle = _T("Cursor2");
    lvcolumn.iSubItem = 4;
    lvcolumn.pszText = omTmpColTitle.GetBuffer(MAX_PATH);
    lvcolumn.cchTextMax = (int)_tcslen(omTmpColTitle.GetBuffer(MAX_PATH));
    lvcolumn.cx = nGetListCtrlTextExtent(omTmpColTitle);
    m_lstSignalDetails.InsertColumn(4, &lvcolumn);
    omTmpColTitle = _T("Difference");
    lvcolumn.iSubItem = 5;
    lvcolumn.pszText = omTmpColTitle.GetBuffer(MAX_PATH);
    lvcolumn.cchTextMax = (int)_tcslen(omTmpColTitle.GetBuffer(MAX_PATH));
    lvcolumn.cx = nGetListCtrlTextExtent(omTmpColTitle);
    m_lstSignalDetails.InsertColumn(5, &lvcolumn);
}

/*******************************************************************************
  Function Name  : vInsertSignalData
  Input(s)       : -
  Output         : -
  Functionality  : This function will read global element list and will insert
                   signal names into list control.
  Member of      : CGraphBottomView
  Author(s)      : ArunKumar K
  Date Created   : 22/12/2010
  Modifications  :
*******************************************************************************/
void CGraphBottomView::vInsertSignalData()
{
    //Clear all previous entries
    m_lstSignalDetails.DeleteAllItems();
    // Get Element List
    CGraphList* podList = NULL;
    CGraphChildFrame* pParentWnd = NULL;
    pParentWnd = (CGraphChildFrame*)pomGetParentWindow();

    if(pParentWnd != NULL)
    {
        podList = pParentWnd->pGetSignalListDetails();
    }
    else
    {
        return;
    }

    CGraphElement odTemp;

    if( podList != NULL )
    {
        UINT unMsgID = 0;
        CString omStrName;
        int nItemCount  = (int)podList->m_omElementList.GetSize();

        // Add items to the list
        for( int nIndex = 0; nIndex < nItemCount; nIndex++ )
        {
            odTemp = podList->m_omElementList[ nIndex ];

            // Statistics Parameter
            if( odTemp.m_nValueType != eSTAT_PARAM )
            {
                unMsgID = odTemp.m_nMsgID;
                // Get Message Name
                omStrName = odTemp.m_strMsgName;
                // Add Element Catogory
                m_lstSignalDetails.InsertItem( nIndex, omStrName );
                // Add Element Name
                m_lstSignalDetails.SetItemText( nIndex, 1,
                                                odTemp.m_omStrElementName );
                // Set Color of the entry
                m_lstSignalDetails.SetItemData( nIndex, odTemp.m_nLineColor );

                // Add Value Type
                if( odTemp.m_nValueType == eRAW_VALUE )
                {
                    m_lstSignalDetails.SetItemText( nIndex, 2, defSTR_RAW_VALUE );
                }
                else
                {
                    m_lstSignalDetails.SetItemText( nIndex, 2, defSTR_PHY_VALUE );
                }
            }
            else
            {
                // Insert Category
                m_lstSignalDetails.InsertItem( nIndex, defSTR_STATISTICS_NAME);
                // Insert element name
                m_lstSignalDetails.SetItemText( nIndex, 1,
                                                odTemp.m_omStrElementName );
                // Insert Channel String
                CString omStrChannel;
                omStrChannel.Format( defSTR_CHANNEL_NAME_FORMAT,
                                     defSTR_CHANNEL_NAME,
                                     odTemp.m_nFrameFormat );
                m_lstSignalDetails.SetItemText( nIndex, 2, omStrChannel );
                // Set Color of the entry
                m_lstSignalDetails.SetItemData( nIndex, odTemp.m_nLineColor );
            }
        }
    }
}

/*******************************************************************************
  Function Name  : vUpdateSignalData
  Input(s)       : -
  Output         : -
  Functionality  : This function will update signal data based on cursor details.
  Member of      : CGraphBottomView
  Author(s)      : ArunKumar K
  Date Created   : 22/12/2010
  Modifications  : ArunKumar K, 24/12/2010
                   Made the dblDiff to contain only modulus of difference.
*******************************************************************************/
void CGraphBottomView::vUpdateSignalData()
{
    if ( m_pDMGraphCtrl == NULL )
    {
        return;
    }

    CGraphChildFrame* pParentWnd = NULL;
    pParentWnd = (CGraphChildFrame*)pomGetParentWindow();
    double dblValue1 = 0, dblValue2 = 0, dblDiff;
    CString strValue1, strValue2, strDiffVal;

    if( m_dblarrTime[0] == 0 && m_dblarrTime[1] == 0 )
    {
        for( int nIndex = 0; nIndex < m_lstSignalDetails.GetItemCount(); nIndex++)
        {
            m_lstSignalDetails.SetItemText( nIndex, 3, STR_EMPTY);  //Cursor 1 value
            m_lstSignalDetails.SetItemText( nIndex, 4, STR_EMPTY);  //Cursor 2 value
            m_lstSignalDetails.SetItemText( nIndex, 5, STR_EMPTY); //Difference value
        }

        return;
    }

    for( int nIndex = 0; nIndex < m_lstSignalDetails.GetItemCount(); nIndex++)
    {
        dblValue1 = 0;
        dblValue2 = 0;
        m_pDMGraphCtrl->GetElementValueAtCursor( (short)nIndex, m_dblarrTime[0], &dblValue1);
        m_pDMGraphCtrl->GetElementValueAtCursor( (short)nIndex, m_dblarrTime[1], &dblValue2);
        dblDiff = dblValue2 - dblValue1;

        if( dblDiff < 0)
        {
            dblDiff = -dblDiff;
        }

        dRound(dblValue1, 3);
        dRound(dblValue2, 3);
        dRound(dblDiff, 3);
        strValue1.Format( _T("%.3lf") ,dblValue1);
        strValue2.Format( _T("%.3lf") ,dblValue2);
        strDiffVal.Format(_T("%.3lf") ,dblDiff);
        m_lstSignalDetails.SetItemText( nIndex, 3, strValue1);  //Cursor 1 value
        m_lstSignalDetails.SetItemText( nIndex, 4, strValue2);  //Cursor 2 value
        m_lstSignalDetails.SetItemText( nIndex, 5, strDiffVal); //Difference value
    }
}

/*******************************************************************************
  Function Name  : nGetListCtrlTextExtent
  Input(s)       : omColTitle
  Output         : Returns the Text Extent.
  Functionality  : Gets the Text Extent of omColTitle and returns the same.
  Member of      : CGraphBottomView
  Author(s)      : Arunkumar K
  Date Created   : 22-12-2010
  Modifications  :
*******************************************************************************/
int CGraphBottomView::nGetListCtrlTextExtent(CString omColTitle)
{
    CSize sz;
    TEXTMETRIC tm;
    int nDx = 0;
    CDC*  pDC = m_lstSignalDetails.GetDC();
    pDC->GetTextMetrics(&tm);
    sz = pDC->GetTextExtent(omColTitle);
    //Add a char avg. width to remove wrapping
    nDx = tm.tmAveCharWidth;
    nDx += sz.cx;
    return nDx;
}

/*******************************************************************************
  Function Name  : OnBtnExport
  Input(s)       : -
  Output         : -
  Functionality  : This function will display Export dialog for data export
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphBottomView::OnBtnExport()
{
    CGraphExportDlg odDlg;
    odDlg.m_pDMGraphCtrl = m_pDMGraphCtrl;
    odDlg.DoModal();
}

/*******************************************************************************
  Function Name  : OnBtnAutoFit
  Input(s)       : -
  Output         : -
  Functionality  : This function will call graph control's AutoRange method to
                   find optimal range of all elements
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphBottomView::OnBtnAutoFit()
{
    m_pDMGraphCtrl->AutoRange();
}

/*******************************************************************************
  Function Name  : OnBtnConfigure
  Input(s)       : -
  Output         : -
  Functionality  : This function will shoe element configuration dialog to show
                   and change view style parameters.
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphBottomView::OnBtnConfigure()
{
    // Create configuration dialog and show it
    CGraphConfiguration omGraphConf;
    CGraphList* podList = NULL;
    CGraphChildFrame* pParentWnd = NULL;
    pParentWnd = (CGraphChildFrame*)pomGetParentWindow();

    if(pParentWnd != NULL)
    {
        podList = pParentWnd->pGetSignalListDetails();
    }
    else
    {
        return;
    }

    CGraphParameters par;
    //omGraphConf.vSetValues(podList->m_odGraphParameters);
    omGraphConf.m_pGraphList = podList;
    omGraphConf.m_pDMGraphCtrl = m_pDMGraphCtrl;
    omGraphConf.DoModal();
}

/*******************************************************************************
  Function Name  : pomGetParentWindow
  Input(s)       : -
  Output         : CWnd * - Pointer to CGraphChildFrame
  Functionality  : This Function will return parent window pointer. That is
                   pointer to CGraphChildFrame. This will return NULL incase of
                   failure
  Member of      : CGraphBottomView
  Author(s)      : ArunKumar K
  Date Created   : 04.11.2010
  Modifications  :
*******************************************************************************/
CWnd* CGraphBottomView::pomGetParentWindow() const
{
    CWnd* pWnd = NULL;
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

/*******************************************************************************
  Function Name  : OnBtnCopy
  Input(s)       : -
  Output         : -
  Functionality  : This function will call graph control's copy function to copy
                   the graph image in to the clipboard
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphBottomView::OnBtnCopy()
{
    if( m_pDMGraphCtrl != NULL )
    {
        m_pDMGraphCtrl->CopyToClipboard();
    }
}

/*******************************************************************************
  Function Name  : OnBtnGrid
  Input(s)       : -
  Output         : -
  Functionality  : This function will toggle grid lines. This will use graph
                   control's ShowGrid property.
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : Raja N on 14.12.2004, Review comments implemented
*******************************************************************************/
void CGraphBottomView::OnBtnGrid()
{
    CGraphList* podList = NULL;
    CGraphChildFrame* pParentWnd = NULL;
    pParentWnd = (CGraphChildFrame*)pomGetParentWindow();

    if(pParentWnd != NULL)
    {
        podList = pParentWnd->pGetSignalListDetails();
    }
    else
    {
        return;
    }

    if( podList != NULL)
    {
        CGraphParameters* pEnv =
            &(podList->m_odGraphParameters);

        if( pEnv != NULL )
        {
            // Toggle grid property
            pEnv->m_bShowGrid = !pEnv->m_bShowGrid;
            // Update Graph Control

            if (  m_pDMGraphCtrl !=NULL )
            {
                CComPtr<IDMGraphAxis> spAxisX;
                m_pDMGraphCtrl->get_Axis( HorizontalX, &spAxisX);
                spAxisX->put_ShowGrid(pEnv->m_bShowGrid ? VARIANT_TRUE : VARIANT_FALSE);
                CComPtr<IDMGraphAxis> spAxisY;
                m_pDMGraphCtrl->get_Axis( VerticalY, &spAxisY);
                spAxisY->put_ShowGrid(pEnv->m_bShowGrid ? VARIANT_TRUE : VARIANT_FALSE);
            }

            // Update Button Text
            vSetShowButtonState( pEnv->m_bShowGrid );
            // Pass Some Pointer to avoid assertion
            //theApp.bSetData( SIGNAL_GRAPH_LIST, pEnv);
        }
    }
}

/*******************************************************************************
  Function Name  : OnBtnSet
  Input(s)       : -
  Output         : -
  Functionality  : This function will validate user entered range and will set
                   the graph range if it is valid
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphBottomView::OnBtnSet()
{
    BOOL bDataValid = TRUE;
    UpdateData();

    if( m_pDMGraphCtrl != NULL )
    {
        // Check for validity
        if( m_dRangeFrom < 0 || m_dRangeTo < 0 )
        {
            bDataValid = FALSE;
        }
        else if( m_dRangeTo <= m_dRangeFrom )
        {
            bDataValid = FALSE;
        }

        // If data valid then update graph control
        if( bDataValid == TRUE )
        {
            double dXMin,dXMax, dYMin, dYMax;
            m_pDMGraphCtrl->GetRange(&dXMin, &dXMax, &dYMin, &dYMax);
            m_pDMGraphCtrl->SetRange(m_dRangeFrom, m_dRangeTo, dYMin, dYMax);
        }
        else
        {
            // Show error message
            AfxMessageBox( defSTR_INVALID_RANGE );
        }
    }
}

/*******************************************************************************
  Function Name  : OnRadioActionMove
  Input(s)       : -
  Output         : -
  Functionality  : This function will set the track action to Move and will also
                   update configuration module
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphBottomView::OnRadioActionMove()
{
    vSetAction();
    // Save Changes
    vSaveChangedInToConfig();
}

/*******************************************************************************
  Function Name  : OnRadioActionNone
  Input(s)       : -
  Output         : -
  Functionality  : This function will set the track action to None and will also
                   update configuration module
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphBottomView::OnRadioActionNone()
{
    vSetAction();
    // Save Changes
    vSaveChangedInToConfig();
}

/*******************************************************************************
  Function Name  : OnRadioActionTrack
  Input(s)       : -
  Output         : -
  Functionality  : This function will set the track action to Track and will
                   also update configuration module
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphBottomView::OnRadioActionTrack()
{
    vSetAction();
    // Save Changes
    vSaveChangedInToConfig();
}

/*******************************************************************************
  Function Name  : OnRadioActionMove
  Input(s)       : -
  Output         : -
  Functionality  : This function will set the track action to Zoom and will also
                   update configuration module
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphBottomView::OnRadioActionZoom()
{
    vSetAction();
    // Save Changes
    vSaveChangedInToConfig();
}

/*******************************************************************************
  Function Name  : OnRadioActionMove
  Input(s)       : -
  Output         : -
  Functionality  : This function will set the active axis to X and will also
                   update configuration module
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphBottomView::OnRadioAxisX()
{
    vSetAction();
    // Save Changes
    vSaveChangedInToConfig();
}

/*******************************************************************************
  Function Name  : OnRadioAxisXy
  Input(s)       : -
  Output         : -
  Functionality  : This function will set the active axis to X and will also
                   update configuration module
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphBottomView::OnRadioAxisXy()
{
    vSetAction();
    // Save Changes
    vSaveChangedInToConfig();
}

/*******************************************************************************
  Function Name  : OnRadioAxisY
  Input(s)       : -
  Output         : -
  Functionality  : This function will set the active axis to X and will also
                   update configuration module
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphBottomView::OnRadioAxisY()
{
    vSetAction();
    // Save Changes
    vSaveChangedInToConfig();
}

/*******************************************************************************
  Function Name  : vInitGraphControl
  Input(s)       : -
  Output         : -
  Functionality  : This function will initialise graph control. This will set
                   viewstyle parameters of the graph control
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphBottomView::vInitGraphControl()
{
    CGraphList* podList = NULL;
    CGraphChildFrame* pParentWnd = NULL;
    pParentWnd = (CGraphChildFrame*)pomGetParentWindow();

    if(pParentWnd != NULL)
    {
        podList = pParentWnd->pGetSignalListDetails();
    }
    else
    {
        return;
    }

    if( podList != NULL )
    {
        CGraphParameters* pEnv = &podList->m_odGraphParameters;

        if( pEnv != NULL )
        {
            // Init Active Axis
            m_nAxis = pEnv->m_nActiveAxis;
            // Init Active Action
            m_nAction = pEnv->m_nAction;
            // Set Show Grid Action
            vSetShowButtonState( pEnv->m_bShowGrid );

            // Set Graph Params
            if( m_pDMGraphCtrl != NULL )
            {
                // Set Axis Color
                m_pDMGraphCtrl->put_AxisColor((OLE_COLOR)pEnv->m_nAxisColor);
                // Set Plot Area Color
                m_pDMGraphCtrl->put_PlotAreaColor((OLE_COLOR)pEnv->m_nPlotAreaColor);
                // Set Grid Color
                m_pDMGraphCtrl->put_GridColor((OLE_COLOR)pEnv->m_nGridColor);
                // Set Frame Style
                m_pDMGraphCtrl->put_FrameStyle((FrameStyle)pEnv->m_nFrameStyle);
                // Set Frame Color
                m_pDMGraphCtrl->put_ControlFrameColor((OLE_COLOR)pEnv->m_nFrameColor);
                // Set Grid Lines Count
                CComPtr<IDMGraphAxis> spAxisX;
                m_pDMGraphCtrl->get_Axis( HorizontalX, &spAxisX);
                spAxisX->put_GridNumber((short)pEnv->m_nXGridLines);
                CComPtr<IDMGraphAxis> spAxisY;
                m_pDMGraphCtrl->get_Axis( VerticalY, &spAxisY);
                spAxisY->put_GridNumber((short)pEnv->m_nYGridLines);
                // Set Show Grid
                spAxisX->put_ShowGrid(pEnv->m_bShowGrid ? VARIANT_TRUE : VARIANT_FALSE);
                spAxisY->put_ShowGrid(pEnv->m_bShowGrid ? VARIANT_TRUE : VARIANT_FALSE);
                // Set Format String
                spAxisX->put_Format( CComBSTR(defSTR_X_AXIS_FORMAT) );
                spAxisY->put_Format( CComBSTR(defSTR_Y_AXIS_FORMAT) );
                // Set X,Y Range
                double dXMin,dXMax, dYMin, dYMax;
                m_pDMGraphCtrl->GetRange(&dXMin, &dXMax, &dYMin, &dYMax);
                dYMax = 100.0;
                m_pDMGraphCtrl->SetRange(defDEFAULT_XRANGE_MIN, defDEFAULT_XRANGE_MAX, dYMin, dYMax);
                //Set the caption
                BSTR bstrCaption(L"Graph Display - CAN");
                m_pDMGraphCtrl->put_Caption(bstrCaption);
            }

            // Update Local DDX Variables
            UpdateData( FALSE );
            // Update Axis and action
            vSetAction();
        }
    }
}

/*******************************************************************************
  Function Name  : vSetShowButtonState
  Input(s)       : bShow - TRUE to set button text to "Show". FALSE to set
                   "Hide" as button text
  Output         : -
  Functionality  : This function will set the button text of "Show" button based
                   on the bShow parameter value
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphBottomView::vSetShowButtonState(BOOL bShow)
{
    CWnd* pWnd = NULL;
    pWnd = GetDlgItem( IDC_BTN_GRID );

    if( pWnd != NULL )
    {
        if( bShow == TRUE )
        {
            pWnd->SetWindowText( defSTR_HIDE_GRID);
        }
        else
        {
            pWnd->SetWindowText( defSTR_SHOW_GRID);
        }
    }
}

/*******************************************************************************
  Function Name  : vSetAction
  Input(s)       : -
  Output         : -
  Functionality  : This function update the active axis and action values as per
                   the button setting. This will be called from Axis and Action.
                   radio button handlers.
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphBottomView::vSetAction()
{
    // Update Axis Value
    if (  m_pDMGraphCtrl == NULL )
    {
        return;
    }

    UpdateData( );

    switch( m_nAction )
    {
        case 0: //move
        {
            vEnableAxisControls( TRUE );

            // switch Axis
            switch( m_nAxis )
            {
                case 0: // X Axis
                    m_pDMGraphCtrl->put_TrackMode(PanX);
                    break;

                case 1: // Y Axis Only
                    m_pDMGraphCtrl->put_TrackMode(PanY);
                    break;

                case 2: // XY Axis
                    m_pDMGraphCtrl->put_TrackMode(PanXY);
                    break;

                default:
                    ASSERT( FALSE );
            }

            break;
        }

        case 1: //Zoom
        {
            vEnableAxisControls( TRUE );

            // switch Axis
            switch( m_nAxis )
            {
                case 0: // X Axis
                    m_pDMGraphCtrl->put_TrackMode(ZoomX);
                    break;

                case 1: // Y Axis Only
                    m_pDMGraphCtrl->put_TrackMode(ZoomY);
                    break;

                case 2:
                    m_pDMGraphCtrl->put_TrackMode(Zoom);
                    break;

                default:
                    ASSERT( FALSE );
            }

            break;
        }

        case 2: // Track
            // Disable axis controls
            vEnableAxisControls( FALSE );
            m_pDMGraphCtrl->put_TrackMode(None);
            break;

        case 3: // None
            // Disable axis controls
            vEnableAxisControls( FALSE );
            m_pDMGraphCtrl->put_TrackMode(None);
            break;

        default:
            ASSERT( FALSE );
    }
}

/*******************************************************************************
  Function Name  : OnBtnFitAll
  Input(s)       : -
  Output         : -
  Functionality  : This function will call graph control function FitAll to set
                   all elements to a special independent line.
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphBottomView::OnBtnFitAll()
{
    //m_pDMGraphCtrl->FitAll();
}

/*******************************************************************************
  Function Name  : OnBtnUp
  Input(s)       : -
  Output         : -
  Functionality  : This function will shift graph display up by one Y grid
                   position
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphBottomView::OnBtnUp()
{
    if( m_pDMGraphCtrl != NULL )
    {
        m_pDMGraphCtrl->ShiftDisplay(0,1);
    }
}

/*******************************************************************************
  Function Name  : OnBtnLeft
  Input(s)       : -
  Output         : -
  Functionality  : This function will shift graph display left by one X grid
                   position
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphBottomView::OnBtnLeft()
{
    if( m_pDMGraphCtrl != NULL )
    {
        m_pDMGraphCtrl->ShiftDisplay(-1, 0);
    }
}

/*******************************************************************************
  Function Name  : OnBtnDown
  Input(s)       : -
  Output         : -
  Functionality  : This function will shift graph display down by one Y grid
                   position
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphBottomView::OnBtnDown()
{
    if( m_pDMGraphCtrl != NULL )
    {
        m_pDMGraphCtrl->ShiftDisplay(0, -1);
    }
}

/*******************************************************************************
  Function Name  : OnBtnRight
  Input(s)       : -
  Output         : -
  Functionality  : This function will shift graph display right by one X grid
                   position
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphBottomView::OnBtnRight()
{
    if( m_pDMGraphCtrl != NULL )
    {
        m_pDMGraphCtrl->ShiftDisplay(1, 0);
    }
}

/*******************************************************************************
  Function Name  : vEnableAxisControls
  Input(s)       : bEnable - TRUE to enable Axis controls, FALSE to disable
  Output         : -
  Functionality  : This function will enable/Disable axis controls. This will
                   be called from action  radio button handlers to enable or
                   disable based on action type.
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphBottomView::vEnableAxisControls(BOOL bEnable)
{
    CWnd* pWnd = NULL;
    // Get X Axis button
    pWnd = GetDlgItem( IDC_RADIO_AXIS_X );

    if( pWnd != NULL )
    {
        pWnd->EnableWindow( bEnable );
    }

    // Get Y Axis button
    pWnd = GetDlgItem( IDC_RADIO_AXIS_Y );

    if( pWnd != NULL )
    {
        pWnd->EnableWindow( bEnable );
    }

    // Get XY Button
    pWnd = GetDlgItem( IDC_RADIO_AXIS_XY );

    if( pWnd != NULL )
    {
        pWnd->EnableWindow( bEnable );
    }
}

/*******************************************************************************
  Function Name  : vEnableActionControls
  Input(s)       : bEnable - TRUE to Enable, FALSE to Disable
  Output         : -
  Functionality  : This function will enable or disable actin radio button group
                   This group will be disabled if the tool is connected
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphBottomView::vEnableActionControls(BOOL bEnable)
{
    CWnd* pWnd = NULL;
    // Get Move button
    pWnd = GetDlgItem( IDC_RADIO_ACTION_MOVE );

    if( pWnd != NULL )
    {
        pWnd->EnableWindow( bEnable );
    }

    // Get Zoom button
    pWnd = GetDlgItem( IDC_RADIO_ACTION_ZOOM );

    if( pWnd != NULL )
    {
        pWnd->EnableWindow( bEnable );
    }

    // Get Track Button
    pWnd = GetDlgItem( IDC_RADIO_ACTION_TRACK );

    if( pWnd != NULL )
    {
        pWnd->EnableWindow( bEnable );
    }

    // Get None Button
    pWnd = GetDlgItem( IDC_RADIO_ACTION_NONE );

    if( pWnd != NULL )
    {
        pWnd->EnableWindow( bEnable );
    }
}

/*******************************************************************************
  Function Name  : vHandleConnectionStatusChange
  Input(s)       : bConnectionStatus - Status of the tool connection. TRUE if
                   the tool is going to connected state
  Output         : -
  Functionality  : This function will handle connect event. This will disable
                   not supported UI controls if the tool is going to connected
                   state. This will also set the graph to Run mode. The update
                   timer will be started during connect state. During disconnect
                   this function will bring back the graph control to normal
                   mode and will kill the update timer.
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : Raja N on 14.12.2004, Implemented review comments
*******************************************************************************/
void CGraphBottomView::vHandleConnectionStatusChange(BOOL bConnectStatus)
{
    // Tool is going to connect state
    vEnableDisableButtons( bConnectStatus );
    CGraphChildFrame* pParentWnd = NULL;
    pParentWnd = (CGraphChildFrame*)pomGetParentWindow();
    //Update Graph right view
    CGraphRightView* pRightView = (CGraphRightView*)pParentWnd->pomGetRightTopViewPointer();
    pRightView->vHandleConnectionStatusChange(bConnectStatus);

    // Create/ Delete Update Timer
    if( bConnectStatus == TRUE )
    {
        // Go To Run Mode
        //m_podGraphControl->GoToRunMode();
        CGraphList* podList = NULL;

        if(pParentWnd != NULL)
        {
            pParentWnd->m_pomBottomView = this;
        }

        if(pParentWnd != NULL)
        {
            podList = pParentWnd->pGetSignalListDetails();
        }
        else
        {
            return;
        }

        //int nTimeDelay = podList->m_odGraphParameters.m_nRefreshRate;
        //nTimerID = SetTimer( defUPDATE_TIMER_ID , nTimeDelay, NULL );
    }
    else
    {
        // Go To Normal Mode
        //m_podGraphControl->GoToNormalMode();
        // Check for valid timer handle
        if( nTimerID != 0 )
        {
            // Kill Display Update Timer
            KillTimer( nTimerID );
        }
    }
}

/*******************************************************************************
  Function Name  : vEnableDisableButtons
  Input(s)       : bConnect - TRUE if the Tool is connected. FALSE otherwise
  Output         : -
  Functionality  : This function will Enable/Disable UI controls based on
                   connection status given by the parameter bConnect.
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphBottomView::vEnableDisableButtons(BOOL bConnect)
{
    BOOL bEnable = !bConnect;
    // Enable / Disable Axis controls
    vEnableAxisControls( bEnable );
    // Enable / Disable Action controls
    vEnableActionControls( bEnable );
    // Enable / Disable Export Section
    // Export Button
    CWnd* pWnd = NULL;
    pWnd = GetDlgItem( IDC_BTN_EXPORT );

    if( pWnd != NULL )
    {
        pWnd->EnableWindow( bEnable );
    }

    // Configure Button
    pWnd = GetDlgItem( IDC_BTN_CONFIGURE );

    if( pWnd != NULL )
    {
        pWnd->EnableWindow( bEnable );
    }
}

/*******************************************************************************
  Function Name  : OnTimer
  Input(s)       : nIDEvent - Timer ID
  Output         : -
  Functionality  : Framework calls this function if any settime value elapses.
                   This timer function will update graph display. This will also
                   scroll the display if the XMax range value is less then the
                   current absolute time.
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphBottomView::OnTimer(UINT nIDEvent)
{
    CFormView::OnTimer(nIDEvent);
}


/*******************************************************************************
  Function Name  : vSaveChangedInToConfig
  Input(s)       : -
  Output         : -
  Functionality  : Updated Global Env attributes and save the changes in config
                   module
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CGraphBottomView::vSaveChangedInToConfig()
{
    CGraphList* podList = NULL;
    CGraphChildFrame* pParentWnd = NULL;
    pParentWnd = (CGraphChildFrame*)pomGetParentWindow();

    if(pParentWnd != NULL)
    {
        podList = pParentWnd->pGetSignalListDetails();
    }
    else
    {
        return;
    }

    CGraphParameters* pEnv = &(podList->m_odGraphParameters);

    if( pEnv != NULL )
    {
        pEnv->m_nAction = m_nAction;
        pEnv->m_nActiveAxis = m_nAxis;
    }
}

/*******************************************************************************
  Function Name  : vHandleConfigFileChange
  Input(s)       : -
  Output         : -
  Functionality  : This function will update graph control view style params
  Member of      : CGraphBottomView
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
LRESULT CGraphBottomView::vHandleConfigFileChange( WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    // Initialise graph control
    vInitGraphControl();
    return 0;
}

/*******************************************************************************
  Function Name  : vUpdateCursordetails
  Input(s)       : X, Y, shCursorID
  Output         : -
  Functionality  : Cusor details updater
  Member of      : CGraphBottomView
  Author(s)      : ArunKumar K
  Date Created   : 22/12/2010
  Modifications  :
*******************************************************************************/
void CGraphBottomView::vUpdateCursordetails(double X, double /*Y*/, short shCursorID)
{
    m_dblarrTime[shCursorID - 1] = X;

    if( m_dblarrTime[1] != 0)
    {
        m_dblDeltaTime = m_dblarrTime[1] - m_dblarrTime[0];
        vUpdateSignalData();
    }

    if( m_dblarrTime[0] == 0 && m_dblarrTime[1] == 0 )
    {
        vUpdateSignalData();
    }

    if( m_dblarrTime[0] == 0)
    {
        m_dblDeltaTime = 0;
    }

    m_dblarrTime[0] = dRound(m_dblarrTime[0], 3);
    m_dblarrTime[1] = dRound(m_dblarrTime[1], 3);
    m_dblDeltaTime  = dRound(m_dblDeltaTime , 3);
    UpdateData(FALSE);
}

/*******************************************************************************
  Function Name  : dRound
  Input(s)       : val, decimals
  Output         : double(Rounded value)
  Functionality  : Round off funtion for double values
  Member of      : CGraphBottomView
  Author(s)      : ArunKumar K
  Date Created   : 22/12/2010
  Modifications  :
*******************************************************************************/
double CGraphBottomView::dRound(double val, unsigned int decimals)
{
    if(val!=0)
    {
        double sign = fabs(val)/val;//we obtain the sign to calculate positive always
        double tempval = fabs(val*pow((double)10, (double)decimals));//shift decimal places
        unsigned int tempint = (unsigned int)tempval;
        double decimalpart = tempval-tempint;//obtain just the decimal part

        if(decimalpart>=0.5)//next integer number if greater or equal to 0.5
        {
            tempval = ceil(tempval);
        }
        else
        {
            tempval = floor(tempval);    //otherwise stay in the current interger part
        }

        return (tempval*pow((double)10,-(double)decimals))*sign;//shift again to the normal decimal places
    }
    else
    {
        return 0;
    }
}
