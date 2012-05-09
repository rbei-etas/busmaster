/*******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  GraphLeftView.cpp
  Description   :  Implementation file for CGraphLeftView class
  $Log:   X:/Archive/Sources/SigGrphWnd/GraphLeftView.cpv  $

  Author(s)     :  Raja N
  Date Created  :  09/12/2004
  Modified By   :
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved
*******************************************************************************/


// For standard Headers
#include "SigGrphWnd_stdafx.h"
// For resource symbol definitions
#include "SigGrphWnd_resource.h"
// For Graph Left View class definition
#include "GraphLeftView.h"
// For Element Selection Dialog definition
//#include "ElementSelection.h"
// For standard definitions
#include "SigGrphWnd_Defines.h"
#include "GraphChildFrame.h"
#include "GraphBottomView.h"
#include ".\graphleftview.h"

// Local definitions
#define defFROM_LINE_COLOR        0
#define defFROM_LINE_TYPE         1
#define defFROM_POINT_COLOR       2
#define defFROM_POINT_TYPE        3
#define defFROM_VISIBLE           4
#define defFROM_ENABLE            5
#define defFRM_ELEMENT_SELECTION  6
#define defFROM_LINE_DISPLAY      7

IMPLEMENT_DYNCREATE(CGraphLeftView, CFormView)

/*******************************************************************************
  Function Name  : CGraphLeftView
  Description    : Standard default constructor
  Member of      : CGraphLeftView
  Functionality  : This will initialise local variables
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
CGraphLeftView::CGraphLeftView()
    : CFormView(CGraphLeftView::IDD)
    , m_nLineDisplay(0)
{
    //{{AFX_DATA_INIT(CGraphLeftView)
    m_nLineType = -1;
    m_nSymbolType = -1;
    //}}AFX_DATA_INIT
    //SGW Code commented by Arun 21-10-2010
    //pMainFrame = NULL;
    //SGW Code commented by Arun 21-10-2010
    m_pDMGraphCtrl = NULL;
    m_bConnectionStatus = FALSE;
}

/*******************************************************************************
  Function Name  : ~CGraphLeftView
  Description    : Standard Destructor
  Member of      : CGraphLeftView
  Functionality  : -
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
CGraphLeftView::~CGraphLeftView()
{
}

void CGraphLeftView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CGraphLeftView)
    DDX_Control(pDX, IDC_BTN_VISIBLE, m_omBtnVisible);
    DDX_Control(pDX, IDC_BTN_ENABLE, m_omBtnEnable);
    DDX_Control(pDX, IDC_LIST_SIGNALS, m_omSignalList);
    DDX_Control(pDX, IDC_POINT_COLOR, m_omPointColor);
    DDX_Control(pDX, IDC_LINE_COLOR, m_omLineColor);
    DDX_CBIndex(pDX, IDC_COMBO_TYPE, m_nLineType);
    DDX_CBIndex(pDX, IDC_COMBO_SYMBOL, m_nSymbolType);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGraphLeftView, CFormView)
    //{{AFX_MSG_MAP(CGraphLeftView)
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
    ON_BN_CLICKED(IDC_BTN_VISIBLE, OnBtnVisible)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SIGNALS, OnItemchangedListSignals)
    ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnSelchangeComboType)
    ON_CBN_SELCHANGE(IDC_COMBO_SYMBOL, OnSelchangeComboSymbol)
    ON_BN_CLICKED(IDC_BTN_ENABLE, OnBtnEnable)
    ON_MESSAGE( WM_CPN_SELENDOK, OnColorChange)
    ON_MESSAGE(WM_CONFIG_CHANGE, vHandleConfigFileChange)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


#ifdef _DEBUG
void CGraphLeftView::AssertValid() const
{
    CFormView::AssertValid();
}

void CGraphLeftView::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}
#endif //_DEBUG

/*******************************************************************************
  Function Name  : OnInitialUpdate
  Input(s)       : -
  Output         : -
  Functionality  : Initialised UI Controls for the first time show
  Member of      : CGraphLeftView
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CGraphLeftView::OnInitialUpdate()
{
    // Call parent class to do init
    CFormView::OnInitialUpdate();
    // Create List Control
    m_omSignalList.DeleteAllItems();
    // Enable grid lines and full row selection
    m_omSignalList.SetExtendedStyle( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
    m_omSignalList.InsertColumn( defCOL_CATOGORY_INDEX,
                                 defSTR_LIST_HEADING_CATOGORY );
    m_omSignalList.SetColumnWidth( defCOL_CATOGORY_INDEX,
                                   defCOL_CATOGORY_WIDTH );
    m_omSignalList.InsertColumn(  defCOL_ELEMENT_INDEX,
                                  defSTR_LIST_HEADING_ELEMENT );
    m_omSignalList.SetColumnWidth( defCOL_ELEMENT_INDEX,
                                   defCOL_ELEMENT_WIDTH);
    m_omSignalList.InsertColumn( defCOL_TYPE_INDEX,
                                 defSTR_LIST_HEADING_TYPE );
    m_omSignalList.SetColumnWidth( defCOL_TYPE_INDEX,
                                   defCOL_TYPE_WIDTH );
    // Remove Automatic Text from the color popup
    m_omLineColor.SetDefaultText( STR_EMPTY );
    m_omPointColor.SetDefaultText( STR_EMPTY );
    // Register this to child window
    CWnd* pWnd = NULL;
    pWnd = pomGetParentWindow();

    if( pWnd != NULL )
    {
        // Register view pointer
        (static_cast<CGraphChildFrame*>(pWnd))->m_pomLeftView = this;
        // Get Graph Control Pointer
        m_pDMGraphCtrl = (static_cast<CGraphChildFrame*>(pWnd))->m_pDMGraphCtrl;
    }
}

/*******************************************************************************
  Function Name  : pomGetParentWindow
  Input(s)       : -
  Output         : CWnd * - Pointer to CGraphChildFrame
  Functionality  : This Function will return parent window pointer. That is
                   pointer to CGraphChildFrame. This will return NULL incase of
                   failure
  Member of      : CGraphLeftView
  Author(s)      : ArunKumar K
  Date Created   : 02.11.2010
  Modifications  :
*******************************************************************************/
CWnd* CGraphLeftView::pomGetParentWindow() const
{
    CWnd* pWnd = NULL;
    // Get Splitter window pointer
    pWnd = GetParent();

    // Get CGraphChildFrame pointer from Splitter window pointer
    if( pWnd != NULL )
    {
        pWnd = pWnd->GetParent();
    }

    // Return CGraphChildFrame pointer or NULL incase of failure
    return pWnd;
}

/*******************************************************************************
  Function Name  : vPopulateElementList
  Input(s)       : -
  Output         : -
  Functionality  : This function will read global element list and will update
                   element UI list control
  Member of      : CGraphLeftView
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  : Raja N on 11.3.2005
                   Added Channel information for statistics parameters in the
                   Type column
*******************************************************************************/
void CGraphLeftView::vPopulateElementList()
{
    // Get Element List
    CGraphList* podList = NULL;
    // Remove All items in the list
    m_omSignalList.DeleteAllItems();
    // Get List pointer
    CGraphChildFrame* pParentWnd = NULL;
    pParentWnd = static_cast<CGraphChildFrame*>(pomGetParentWindow());

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
            if( odTemp.m_nValueType == eSTAT_PARAM )
            {
                // Insert Category
                m_omSignalList.InsertItem( nIndex, defSTR_STATISTICS_NAME);
                // Insert element name
                m_omSignalList.SetItemText( nIndex, 1,
                                            odTemp.m_omStrElementName );
                // Insert Channel String
                CString omStrChannel;
                omStrChannel.Format( defSTR_CHANNEL_NAME_FORMAT,
                                     defSTR_CHANNEL_NAME,
                                     odTemp.m_nFrameFormat );
                m_omSignalList.SetItemText( nIndex, 2, omStrChannel );
                // Set Color of the entry
                m_omSignalList.SetItemData( nIndex, odTemp.m_nLineColor );
            }
            else
            {
                unMsgID = odTemp.m_nMsgID;
                // Get Message Name
                omStrName = odTemp.m_strMsgName;
                // Add Element Catogory
                m_omSignalList.InsertItem( nIndex, omStrName );
                // Add Element Name
                m_omSignalList.SetItemText( nIndex, 1,
                                            odTemp.m_omStrElementName );
                // Set Color of the entry
                m_omSignalList.SetItemData( nIndex, odTemp.m_nLineColor );

                // Add Value Type
                if( odTemp.m_nValueType == eRAW_VALUE )
                {
                    m_omSignalList.SetItemText( nIndex, 2, defSTR_RAW_VALUE );
                }
                else
                {
                    m_omSignalList.SetItemText( nIndex, 2, defSTR_PHY_VALUE );
                }
            }
        }
    }
}

/*******************************************************************************
  Function Name  : OnItemchangedListSignals
  Input(s)       : pNMHDR - Pointer to the list item struct
                   pResult - Pointer to the result value
  Output         : -
  Functionality  : This function will update line type, line color, point type,
                   point color, enable and visible properties of the selected
                   graph element
  Member of      : CGraphLeftView
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CGraphLeftView::OnItemchangedListSignals(NMHDR* pNMHDR, LRESULT* /*pResult*/)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    // Process only selection change message
    if(pNMListView->uNewState == ( LVIS_FOCUSED |LVIS_SELECTED ) )
    {
        // Get handle to selected message item
        int hSelItem = pNMListView->iItem;

        if ( hSelItem != -1 )
        {
            // Update selected element deteils
            CGraphList* podList = NULL;
            CGraphChildFrame* pParentWnd = NULL;
            pParentWnd = static_cast<CGraphChildFrame*>(pomGetParentWindow());

            if(pParentWnd != NULL)
            {
                podList = pParentWnd->pGetSignalListDetails();
            }
            else
            {
                return;
            }

            // Update Element Details
            if( podList != NULL )
            {
                if( hSelItem < podList->m_omElementList.GetSize() )
                {
                    vSetElementDetails(podList->m_omElementList[ hSelItem ] );
                    vUpdateGraphControl( defFRM_ELEMENT_SELECTION, hSelItem );
                }
                else
                {
                    AfxMessageBox( defSTR_ELEMENT_NOT_FOUND, MB_ICONSTOP);
                }
            }

            //SGW Code commented by Arun 21-10-2010
        }
    }

    // Update UI Controls
    vEnableDisableControls();
}

/*******************************************************************************
  Function Name  : vSetElementDetails
  Input(s)       : odElement - CGraphElement object for which the UI will be
                   updated
  Output         : -
  Functionality  : This function updated UI controls to reflect the properties
                   of the given CGraphElement object
  Member of      : CGraphLeftView
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CGraphLeftView::vSetElementDetails( CGraphElement odElement )
{
    // Update Line Type
    m_nLineType = odElement.m_nLineType;
    // Set Line Color
    m_omLineColor.SetColour( odElement.m_nLineColor );
    // Update symbol Type
    m_nSymbolType = odElement.m_nPointType;
    // Set Sample Point Color
    m_omPointColor.SetColour( odElement.m_nPointColor );

    // Update Visible Property
    if( odElement.m_bVisible == TRUE )
    {
        m_omBtnVisible.SetWindowText( defSTR_HIDE );
    }
    else
    {
        m_omBtnVisible.SetWindowText( defSTR_SHOW );
    }

    // Update Enable Property
    if( odElement.m_bEnabled == TRUE )
    {
        m_omBtnEnable.SetWindowText( defSTR_DISABLE );
    }
    else
    {
        m_omBtnEnable.SetWindowText( defSTR_ENABLE );
    }

    //Set Line display type
    m_nLineDisplay = odElement.m_eDisplayType;
    // Update DDX data members
    UpdateData( FALSE );
}

/*******************************************************************************
  Function Name  : OnSelchangeComboType
  Input(s)       : -
  Output         : -
  Functionality  : This function will be called by the framework if the item
                   selection in the combobox got changed. This will update the
                   selection in the global list and inm graph control
  Member of      : CGraphLeftView
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CGraphLeftView::OnSelchangeComboType()
{
    UpdateData();
    CGraphElement odSelectedElement;
    // Get handle to selected message item
    int hSelItem = m_omSignalList.GetNextItem(-1, LVNI_SELECTED);

    if ( hSelItem != -1 )
    {
        CGraphList* podList = NULL;
        CGraphChildFrame* pParentWnd = NULL;
        pParentWnd = static_cast<CGraphChildFrame*>(pomGetParentWindow());

        if(pParentWnd != NULL)
        {
            podList = pParentWnd->pGetSignalListDetails();
        }
        else
        {
            return;
        }

        // Update Element Details
        if( podList != NULL )
        {
            if( hSelItem < podList->m_omElementList.GetSize() )
            {
                odSelectedElement = podList->m_omElementList[ hSelItem ];
                odSelectedElement.m_nLineType = m_nLineType;
                podList->m_omElementList[ hSelItem ] = odSelectedElement;
                vUpdateGraphControl( defFROM_LINE_TYPE, m_nLineType );
            }
            else
            {
                AfxMessageBox( defSTR_ELEMENT_NOT_FOUND, MB_ICONSTOP);
            }
        }
    }
}
/*******************************************************************************
  Function Name  : OnSelchangeComboSymbol
  Input(s)       : -
  Output         : -
  Functionality  : This function will be called by the framework if the item
                   selection in the combobox got changed. This will update the
                   selection in the global list and inm graph control
  Member of      : CGraphLeftView
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CGraphLeftView::OnSelchangeComboSymbol()
{
    UpdateData();
    CGraphElement odSelectedElement;
    // Get handle to selected message item
    int hSelItem = m_omSignalList.GetNextItem(-1, LVNI_SELECTED);

    if ( hSelItem != -1 )
    {
        CGraphList* podList = NULL;
        CGraphChildFrame* pParentWnd = NULL;
        pParentWnd = static_cast<CGraphChildFrame*>(pomGetParentWindow());

        if(pParentWnd != NULL)
        {
            podList = pParentWnd->pGetSignalListDetails();
        }
        else
        {
            return;
        }

        // Update Element Details
        if( podList != NULL )
        {
            odSelectedElement = podList->m_omElementList[ hSelItem ];
            odSelectedElement.m_nPointType = m_nSymbolType;
            podList->m_omElementList[ hSelItem ] = odSelectedElement;
            vUpdateGraphControl( defFROM_POINT_TYPE, m_nSymbolType );
        }
        else
        {
            AfxMessageBox( defSTR_ELEMENT_NOT_FOUND, MB_ICONSTOP);
        }
    }
}

/*******************************************************************************
  Function Name  : OnBtnEnable
  Input(s)       : -
  Output         : -
  Functionality  : This function will toggle the Enable property of the element.
                   This will also update global list
  Member of      : CGraphLeftView
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CGraphLeftView::OnBtnEnable()
{
    CGraphElement odSelectedElement;
    // Get handle to selected message item
    int hSelItem = m_omSignalList.GetNextItem(-1, LVNI_SELECTED);

    if ( hSelItem != -1 )
    {
        // Get graph list from main frame
        CGraphList* podList = NULL;
        CGraphChildFrame* pParentWnd = NULL;
        pParentWnd = static_cast<CGraphChildFrame*>(pomGetParentWindow());

        if(pParentWnd != NULL)
        {
            podList = pParentWnd->pGetSignalListDetails();
        }
        else
        {
            return;
        }

        // Update Element Details
        if( podList != NULL )
        {
            if( hSelItem < podList->m_omElementList.GetSize() )
            {
                odSelectedElement = podList->m_omElementList[ hSelItem ];
                odSelectedElement.m_bEnabled =
                    !odSelectedElement.m_bEnabled;
                podList->m_omElementList[ hSelItem ] = odSelectedElement;

                // Update Configuration Module
                //theApp.bSetData( SIGNAL_GRAPH_LIST, podList);

                // Update Button Text
                if( odSelectedElement.m_bEnabled == TRUE )
                {
                    m_omBtnEnable.SetWindowText( defSTR_DISABLE );
                }
                else
                {
                    m_omBtnEnable.SetWindowText( defSTR_ENABLE );
                }

                // Update Graph Control
                vUpdateGraphControl( defFROM_ENABLE,
                                     odSelectedElement.m_bEnabled );
            }
            else
            {
                // Invalid element found
                AfxMessageBox( defSTR_ELEMENT_NOT_FOUND, MB_ICONSTOP);
            }
        }
    }
}

/*******************************************************************************
  Function Name  : OnBtnVisible
  Input(s)       : -
  Output         : -
  Functionality  : This function will toggle the Visible property of the element
                   This will also update global list
  Member of      : CGraphLeftView
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CGraphLeftView::OnBtnVisible()
{
    CGraphElement odSelectedElement;
    // Get handle to selected message item
    int hSelItem = m_omSignalList.GetNextItem(-1, LVNI_SELECTED);

    if ( hSelItem != -1 )
    {
        // Get Graph List from main frame
        CGraphList* podList = NULL;
        CGraphChildFrame* pParentWnd = NULL;
        pParentWnd = static_cast<CGraphChildFrame*>(pomGetParentWindow());

        if(pParentWnd != NULL)
        {
            podList = pParentWnd->pGetSignalListDetails();
        }
        else
        {
            return;
        }

        // Update Element Details
        if( podList != NULL )
        {
            if( hSelItem < podList->m_omElementList.GetSize() )
            {
                odSelectedElement = podList->m_omElementList[ hSelItem ];
                odSelectedElement.m_bVisible =
                    !odSelectedElement.m_bVisible;
                podList->m_omElementList[ hSelItem ] = odSelectedElement;

                // Update Configuration Module
                //theApp.bSetData( SIGNAL_GRAPH_LIST, podList);

                // Update Button Text
                if( odSelectedElement.m_bVisible == TRUE )
                {
                    m_omBtnVisible.SetWindowText( defSTR_HIDE );
                }
                else
                {
                    m_omBtnVisible.SetWindowText( defSTR_SHOW );
                }

                // Update Graph Control
                vUpdateGraphControl( defFROM_VISIBLE,
                                     odSelectedElement.m_bVisible );
            }
            else
            {
                // Invalid Element.
                AfxMessageBox( defSTR_ELEMENT_NOT_FOUND, MB_ICONSTOP);
            }
        }
    }
}

/*******************************************************************************
  Function Name  : vEnableDisableControls
  Input(s)       : -
  Output         : -
  Functionality  : This function will update UI control as per the selection in
                   the element UI list and connection status
  Member of      : CGraphLeftView
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CGraphLeftView::vEnableDisableControls()
{
    CGraphList* podList = NULL;
    CGraphChildFrame* pParentWnd = NULL;
    pParentWnd = static_cast<CGraphChildFrame*>(pomGetParentWindow());

    if(pParentWnd != NULL)
    {
        podList = pParentWnd->pGetSignalListDetails();
    }
    else
    {
        return;
    }

    // Update UI Elements
    if( podList != NULL )
    {
        if( podList->m_omElementList.GetSize() > 0 &&
                m_omSignalList.GetSelectedCount() > 0 )
        {
            vEnableControls( TRUE );
        }
        else
        {
            vEnableControls( FALSE );
        }
    }

    //SGW Code commented by Arun 21-10-2010
}

/*******************************************************************************
  Function Name  : vEnableControls
  Input(s)       : bEnable - TRUE to Enable UI controls, FALSE to Disable
  Output         : -
  Functionality  : This fuction Enable/Disable UI controls as per input param
                   and based on tool connection status
  Member of      : CGraphLeftView
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CGraphLeftView::vEnableControls(BOOL bEnable)
{
    // Enable only if it is not connected
    BOOL bEnableCtrl = bEnable && !m_bConnectionStatus;
    CWnd* pWnd = NULL;
    // Line Type
    pWnd = GetDlgItem( IDC_COMBO_TYPE );

    if( pWnd != NULL )
    {
        pWnd->EnableWindow( bEnableCtrl );
    }

    // Line Color
    m_omLineColor.EnableWindow( bEnableCtrl );
    // Point Type
    pWnd = GetDlgItem( IDC_COMBO_SYMBOL );

    if( pWnd != NULL )
    {
        pWnd->EnableWindow( bEnableCtrl );
    }

    // Point Color
    m_omPointColor.EnableWindow( bEnableCtrl );
    // Visible Button - Use Enable Variable directly as this is enabled
    // Irrespective of connection status
    pWnd = GetDlgItem( IDC_BTN_VISIBLE );

    if( pWnd != NULL )
    {
        pWnd->EnableWindow( bEnable );
    }

    // Enable Button - Use Enable Variable directly as this is enabled
    // Irrespective of connection status
    pWnd = GetDlgItem( IDC_BTN_ENABLE );

    if( pWnd != NULL )
    {
        pWnd->EnableWindow( bEnable );
    }
}

/*******************************************************************************
  Function Name  : OnColorChange
  Input(s)       : lparam - Not Used
                   wparam - ID of the control
  Output         : -
  Functionality  : This function processes Color change event posted from the
                   CColorPopup dialog. This function updates global list value
                   as per the current selection
  Member of      : CLeftView
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
LRESULT CGraphLeftView::OnColorChange(WPARAM /*wparam*/, LPARAM lparam)
{
    CGraphElement odSelectedElement;
    // Get handle to selected message item
    int hSelItem = m_omSignalList.GetNextItem(-1, LVNI_SELECTED);

    if ( hSelItem != -1 )
    {
        CGraphList* podList = NULL;
        CGraphChildFrame* pParentWnd = NULL;
        pParentWnd = static_cast<CGraphChildFrame*>(pomGetParentWindow());

        if(pParentWnd != NULL)
        {
            podList = pParentWnd->pGetSignalListDetails();
        }
        else
        {
            return 0;
        }

        // Update Element Details
        if( podList != NULL )
        {
            if( hSelItem < podList->m_omElementList.GetSize() )
            {
                odSelectedElement = podList->m_omElementList[ hSelItem ];

                // Switch the Control ID
                switch( lparam )
                {
                        // Update Line Color
                    case IDC_LINE_COLOR:
                        odSelectedElement.m_nLineColor =
                            m_omLineColor.GetColour();
                        // Update List member
                        podList->m_omElementList[ hSelItem ] =
                            odSelectedElement;
                        // Update UI List Color
                        m_omSignalList.SetItemData( hSelItem,
                                                    odSelectedElement.m_nLineColor );
                        // Update Control
                        vUpdateGraphControl( defFROM_LINE_COLOR,
                                             odSelectedElement.m_nLineColor );
                        (static_cast<CGraphBottomView*>(pParentWnd->m_pomBottomView))
                        ->vInsertSignalData();
                        (static_cast<CGraphBottomView*>(pParentWnd->m_pomBottomView))
                        ->vUpdateSignalData();
                        break;

                        // Update Sample point color
                    case IDC_POINT_COLOR:
                        odSelectedElement.m_nPointColor =
                            m_omPointColor.GetColour();
                        // Update list member
                        podList->m_omElementList[ hSelItem ] =
                            odSelectedElement;
                        // Update Control
                        vUpdateGraphControl( defFROM_POINT_COLOR,
                                             odSelectedElement.m_nPointColor );
                        break;

                    default:
                        ASSERT( FALSE );
                }
            }
            else
            {
                AfxMessageBox( defSTR_ELEMENT_NOT_FOUND, MB_ICONSTOP);
            }
        }
    }

    return 0;
}

/*******************************************************************************
  Function Name  : vUpdateGraphControl
  Input(s)       : [in]pGraphCtrl - Pointer to Graph control interface
                   [in]ppGraphElement - Double pointer to Graph element
  Output         : HRESULT indicating success /Failure
  Functionality  : This function will return the currently selected
                    graph element in graph control.
  Member of      : CGraphLeftView
  Author(s)      : MSamoila
  Date Created   : 01-Dec-2011
  Modifications  :
*******************************************************************************/
HRESULT CGraphLeftView::GetCurrentElement(IDMGraphCtrl* pGraphCtrl, IDMGraphElement** ppGraphElement)
{
    ASSERT(pGraphCtrl);
    CComPtr<IDMGraphCollection> spElements;
    long nSelected = 0;
    HRESULT hr;
    hr = pGraphCtrl->get_Elements(&spElements);

    if(FAILED(hr))
    {
        return hr;
    }

    hr = spElements->get_Selected(&nSelected);

    if(FAILED(hr))
    {
        return hr;
    }

    if(nSelected <0 )
    {
        return S_FALSE;
    }

    CComPtr<IDispatch> spDispatch;
    hr = spElements->get_Item(nSelected, &spDispatch);

    if(FAILED(hr))
    {
        return hr;
    }

    return spDispatch.QueryInterface(ppGraphElement);
}

/*******************************************************************************
  Function Name  : vUpdateGraphControl
  Input(s)       : nFromControl - ID of the control that is calling this func
                   lValue - Parameter Value
  Output         : -
  Functionality  : This central function updates the graph control. This
                   function will be called from various UI controls for graph
                   control update.
  Member of      : CGraphLeftView
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CGraphLeftView::vUpdateGraphControl(int nFromControl, long lValue)
{
    if( m_pDMGraphCtrl != NULL )
    {
        CComPtr<IDMGraphElement> spElement;
        HRESULT hr = GetCurrentElement(m_pDMGraphCtrl, &spElement);

        switch( nFromControl )
        {
            case defFROM_LINE_COLOR:
                // Update Line Color
            {
                spElement->put_LineColor( (OLE_COLOR)lValue );
            }
            break;

            case defFROM_LINE_TYPE:
                // Update Line Type
            {
                spElement->put_LineType((LineType)lValue);
            }
            break;

            case defFROM_POINT_COLOR:
                // Update Point Color
            {
                spElement->put_PointColor((OLE_COLOR)lValue);
            }
            break;

            case defFROM_POINT_TYPE:
                // Update Point Type
            {
                spElement->put_PointSymbol((SymbolType)lValue);
            }
            break;

            case defFROM_VISIBLE:
                // Update Visiblity
            {
                spElement->put_Show(lValue ? VARIANT_TRUE : VARIANT_FALSE);
            }
            break;

            case defFROM_ENABLE:
                // Update Enable property
            {
                // Nothing to update in Graph Control
            }
            break;

            case defFRM_ELEMENT_SELECTION:
                // Update Element Selection
            {
                // Sel Old Element's Line width to default (1)
                spElement->put_LineWidth(defDEFAULT_LINE_WIDTH);
                //Store the selected item
                HRESULT hr;
                CComPtr<IDMGraphCollection> spGraphCollection;
                hr = m_pDMGraphCtrl->get_Elements(&spGraphCollection);

                if(FAILED(hr))
                {
                    return;
                }

                // Select New Element
                hr = spGraphCollection->put_Selected(lValue);
                // Set new element width to selected element width (2)
                spElement->put_LineWidth(defSELECTED_LINE_WIDTH);
            }
            break;

            default:
                ASSERT( FALSE );
        }
    }
}

/*******************************************************************************
  Function Name  : vPopulateGraphControl
  Input(s)       : -
  Output         : -
  Functionality  : This function initializes graph control with list of elements
                   This function will be called from init and during connect.
  Member of      : CGraphLeftView
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  : Raja N on 11.03.2005
                   Added channel information to statistics parameters. This will
                   be used in report generation
                   Arunkumar K on 25.07.2011
                   Commented the ClearGraph() function.
*******************************************************************************/
void CGraphLeftView::vPopulateGraphControl()
{
    if( m_pDMGraphCtrl != NULL )
    {
        // Get Element List from Main Frame
        CGraphList* podList = NULL;
        CGraphChildFrame* pParentWnd = NULL;
        pParentWnd = static_cast<CGraphChildFrame*>(pomGetParentWindow());

        if (pParentWnd != NULL)
        {
            podList = pParentWnd->pGetSignalListDetails();
        }
        else
        {
            ASSERT(FALSE);
        }

        CGraphElement odTemp;

        if ( podList != NULL )
        {
            UINT unMsgID = 0;
            CString omStrName;
            int nItemCount  = (int)podList->m_omElementList.GetSize();
            // Get Buffer Size
            int nBufferSize = podList->m_odGraphParameters.m_nBufferSize;
            CComPtr<IDMGraphCollection> spGraphCollection;
            HRESULT hr = m_pDMGraphCtrl->get_Elements(&spGraphCollection);

            if(FAILED(hr))
            {
                return;
            }

            // Add items to the list
            for ( int nIndex = 0; nIndex < nItemCount; nIndex++ )
            {
                odTemp = podList->m_omElementList[ nIndex ];
                CComPtr<IDispatch> spDispatch;
                //adds to the end and makes it selected
                hr = spGraphCollection->Add(&spDispatch);
                CComPtr<IDMGraphElement> spElement;
                hr = GetCurrentElement(m_pDMGraphCtrl, &spElement);

                // Insert element name
                // If this is stat parameter then append the channel ID
                // prefix with the name
                if ( odTemp.m_nValueType == eSTAT_PARAM )
                {
                    CString omStrChannel;
                    omStrChannel.Format( defSTR_CHANNEL_NAME_FORMAT,
                                         defSTR_CHANNEL_NAME,
                                         odTemp.m_nFrameFormat );
                    // Prefix channel information with the stat
                    // parameter
                    omStrChannel += defSPACE_STR + odTemp.m_omStrElementName;
                    _bstr_t bstrChannel = omStrChannel;
                    spElement->put_Name(bstrChannel.GetBSTR());
                }
                else
                {
                    // Add only the Signal name
                    _bstr_t bstrElementName = odTemp.m_omStrElementName;
                    spElement->put_Name(bstrElementName.GetBSTR());
                }

                // Insert Line Color
                spElement->put_LineColor((OLE_COLOR)odTemp.m_nLineColor);
                // Insert Line Type
                spElement->put_LineType((LineType)odTemp.m_nLineType);
                // Sample Point Type
                spElement->put_PointSymbol((SymbolType)odTemp.m_nPointType);
                // Set The Symbol Type to Sold. Hardcoded
                spElement->put_SolidPoint(VARIANT_TRUE);
                // Sample point type
                spElement->put_PointColor((OLE_COLOR)odTemp.m_nPointColor);
                // Visiblity param
                spElement->put_Show(odTemp.m_bVisible  ? VARIANT_TRUE : VARIANT_FALSE);
            }

            // Set the last item as Selected
            m_omSignalList.SetItemState( nItemCount - 1,
                                         LVIS_SELECTED | LVIS_FOCUSED,
                                         LVIS_SELECTED | LVIS_FOCUSED );
        }
    }
}

/*******************************************************************************
  Function Name  : vHandleConnectionStatusChange
  Input(s)       : bConnect - TRUE if the tool is connected, FALSE otherwise
  Output         : -
  Functionality  : Handles connection change event. Initialised graph control
  Member of      : CGraphLeftView
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  : Arunkumar K on 25/07/2011
                   Added ClearGraph() function call.
*******************************************************************************/
void CGraphLeftView::vHandleConnectionStatusChange(BOOL bConnect)
{
    if( bConnect  && m_pDMGraphCtrl)
    {
        // Remove All items in the control
        m_pDMGraphCtrl->ClearGraph();
        // Set X,Y Range
        double dXMin,dXMax, dYMin, dYMax;
        m_pDMGraphCtrl->GetRange(&dXMin, &dXMax, &dYMin, &dYMax);
        dYMin = 0.0;
        dYMax = 100.0;
        m_pDMGraphCtrl->SetRange(defDEFAULT_XRANGE_MIN, defDEFAULT_XRANGE_MAX, dYMin, dYMax);
    }

    BOOL bEnable = !bConnect;
    // Update Local Instance
    m_bConnectionStatus = bConnect;

    // Update Configuration Controls
    if(bEnable)
    {
        CGraphList* podList = NULL;
        CGraphChildFrame* pParentWnd = NULL;
        pParentWnd = static_cast<CGraphChildFrame*>(pomGetParentWindow());

        if(pParentWnd != NULL)
        {
            podList = pParentWnd->pGetSignalListDetails();
        }
        else
        {
            ASSERT(FALSE);
        }

        // Update UI Elements
        if( podList != NULL )
        {
            if( podList->m_omElementList.GetSize() > 0 &&
                    m_omSignalList.GetSelectedCount() > 0 )
            {
                vEnableControls( bEnable );
            }
        }
    }
    else
    {
        vEnableControls( bEnable );
    }

    // Disable Configure button
    CWnd* pWnd = NULL;
    pWnd = GetDlgItem( IDC_BTN_CONFIGURE );

    if( pWnd != NULL )
    {
        pWnd->EnableWindow( bEnable );
    }

    if( bConnect == TRUE )
    {
        // Add Elements in to the graph control
        vPopulateGraphControl();
    }
}

/*******************************************************************************
  Function Name  : vHandleConfigFileChange
  Input(s)       : lParam, wParam - Not Used
  Output         : -
  Functionality  : Handles connection change event. Initialised graph and list
                   control members
  Member of      : CGraphLeftView
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
LRESULT CGraphLeftView::vHandleConfigFileChange( WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    // Update List Control
    vPopulateElementList();
    // Update UI Controls
    vEnableDisableControls();

    if ( m_pDMGraphCtrl )
    {
        // Remove All items in the control
        m_pDMGraphCtrl->ClearGraph();
    }

    // Add Elements in to the graph control
    vPopulateGraphControl();
    return 0;
}