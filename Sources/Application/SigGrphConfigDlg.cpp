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
 * \file      SigGrphConfigDlg.cpp
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
// SigGrphConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BUSMASTER.h"
#include "SigGrphConfigDlg.h"
// For Color ListCtrl class definition
#include "MessageList.h"
#include "ElementSelection.h"
#include "GraphConfiguration.h"
#include ".\siggrphconfigdlg.h"

// Appliction global object
extern CCANMonitorApp theApp;

// Local definitions
#define defFROM_LINE_COLOR        0
#define defFROM_LINE_TYPE         1
#define defFROM_POINT_COLOR       2
#define defFROM_POINT_TYPE        3
#define defFROM_VISIBLE           4
#define defFROM_ENABLE            5
#define defFRM_ELEMENT_SELECTION  6

// CSigGrphConfigDlg dialog

IMPLEMENT_DYNAMIC(CSigGrphConfigDlg, CDialog)
CSigGrphConfigDlg::CSigGrphConfigDlg(UINT nHardware , CWnd* pParent /*=NULL*/)
    : CDialog(CSigGrphConfigDlg::IDD, pParent)
    , m_nSymbolType(0)
    , m_nLineType(0)
{
    m_pMainFrame = NULL;
    m_nHardware  = nHardware;
}

CSigGrphConfigDlg::~CSigGrphConfigDlg()
{
}

void CSigGrphConfigDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LINE_COLOR, m_omLineColor);
    DDX_Control(pDX, IDC_POINT_COLOR, m_omPointColor);
    DDX_Control(pDX, IDC_BTN_VISIBLE, m_omBtnVisible);
    DDX_Control(pDX, IDC_BTN_ENABLE, m_omBtnEnable);
    DDX_Control(pDX, IDC_LIST_SIGNALS, m_omSignalList);
    DDX_Control(pDX, IDC_COMBO_BUS_TYPE, m_omCmbBusType);
    DDX_CBIndex(pDX, IDC_COMBO_SYMBOL, m_nSymbolType);
    DDX_CBIndex(pDX, IDC_COMBO_TYPE, m_nLineType);
}


BEGIN_MESSAGE_MAP(CSigGrphConfigDlg, CDialog)
    ON_BN_CLICKED(IDC_BTN_VISIBLE, OnBnClickedBtnVisible)
    ON_BN_CLICKED(IDC_BTN_ENABLE, OnBnClickedBtnEnable)
    ON_BN_CLICKED(IDC_BTN_CONFIGURE, OnBnClickedBtnConfigure)
    ON_CBN_SELCHANGE(IDC_COMBO_BUS_TYPE, OnCbnSelchangeComboBusType)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SIGNALS, OnLvnItemchangedListSignals)
    ON_BN_CLICKED(IDC_BUTTON_GRP_DISP_CONFIG, OnBnClickedButtonGrpDispConfig)
    ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnCbnSelchangeComboType)
    ON_CBN_SELCHANGE(IDC_COMBO_SYMBOL, OnCbnSelchangeComboSymbol)
    ON_MESSAGE( WM_CPN_SELENDOK, OnColorChange)
END_MESSAGE_MAP()


// CSigGrphConfigDlg message handlers

BOOL CSigGrphConfigDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
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
    //Add the list of available buses.
    m_omCmbBusType.AddString(_T("CAN"));
    //m_omCmbBusType.AddString(_T("MCNET"));
    //m_omCmbBusType.AddString(_T("J1939"));
    //m_omCmbBusType.AddString(_T("FLEXRAY"));
    m_omCmbBusType.SetCurSel(0);
    vEnableControls(FALSE);
    vPopulateElementList();
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

/*******************************************************************************
  Function Name  : OnBnClickedBtnVisible
  Input(s)       : -
  Output         : -
  Functionality  : This function will toggle the Visible property of the element
                   This will also update global list
  Member of      : CSigGrphConfigDlg
  Author(s)      : ArunKumar K
  Date Created   : 27-10-2010
  Modifications  :
*******************************************************************************/
void CSigGrphConfigDlg::OnBnClickedBtnVisible()
{
    CGraphElement odSelectedElement;
    // Get handle to selected message item
    int hSelItem = m_omSignalList.GetNextItem(-1, LVNI_SELECTED);

    if ( hSelItem != -1 )
    {
        // Get Graph List from main frame
        CGraphList* podList = NULL;
        podList = &(m_pMainFrame->m_odGraphList[m_omCmbBusType.GetCurSel()]);

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
  Function Name  : OnBnClickedBtnEnable
  Input(s)       : -
  Output         : -
  Functionality  : This function will toggle the Enable property of the element.
                   This will also update global list
  Member of      : CSigGrphConfigDlg
  Author(s)      : ArunKumar K
  Date Created   : 27-10-2010
  Modifications  :
*******************************************************************************/
void CSigGrphConfigDlg::OnBnClickedBtnEnable()
{
    CGraphElement odSelectedElement;
    // Get handle to selected message item
    int hSelItem = m_omSignalList.GetNextItem(-1, LVNI_SELECTED);

    if ( hSelItem != -1 )
    {
        // Get graph list from main frame
        CGraphList* podList = NULL;
        podList = &(m_pMainFrame->m_odGraphList[m_omCmbBusType.GetCurSel()]);

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
  Function Name  : OnBnClickedBtnConfigure
  Input(s)       : -
  Output         : -
  Functionality  : This function handles the button click event for Configure
                   button IDC_BTN_CONFIGURE.
  Member of      : CSigGrphConfigDlg
  Author(s)      : ArunKumar K
  Date Created   : 27-10-2010
  Modifications  :
*******************************************************************************/
void CSigGrphConfigDlg::OnBnClickedBtnConfigure()
{
    CElementSelection omConfig(m_nHardware);
    omConfig.m_pMainFrame = m_pMainFrame;
    omConfig.m_eBusType = (eTYPE_BUS)m_omCmbBusType.GetCurSel();
    //int nElemCount =
    m_pMainFrame->m_odGraphList[omConfig.m_eBusType].m_omElementList.GetSize();

    if( omConfig.DoModal() == IDOK )
    {
        // Update UI with changes
        vPopulateElementList();
        // Update Controls
        vEnableDisableControls();
    }
}

/*******************************************************************************
  Function Name  : vPopulateElementList
  Input(s)       : -
  Output         : -
  Functionality  : This function will read global element list and will update
                   element UI list control
  Member of      : CSigGrphConfigDlg
  Author(s)      : ArunKumar K
  Date Created   : 27-10-2010
  Modifications  :
*******************************************************************************/
void CSigGrphConfigDlg::vPopulateElementList()
{
    // Get Element List from Main Frame
    CGraphList* podList = NULL;
    // Remove All items in the list
    m_omSignalList.DeleteAllItems();
    // Get List pointer
    podList = &(m_pMainFrame->m_odGraphList[m_omCmbBusType.GetCurSel()]);
    // Get Database Pointer
    CMsgSignal* pomDatabase = theApp.m_pouMsgSignal;
    CGraphElement odTemp;

    if( podList != NULL && pomDatabase != NULL )
    {
        UINT unMsgID = 0;
        CString omStrName;
        int nItemCount  = podList->m_omElementList.GetSize();

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
                omStrName =
                    pomDatabase->omStrGetMessageNameFromMsgCode( unMsgID );
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
  Function Name  : vEnableDisableControls
  Input(s)       : -
  Output         : -
  Functionality  : This function will update UI control as per the selection in
                   the element UI list and connection status
  Member of      : CSigGrphConfigDlg
  Author(s)      : ArunKumar K
  Date Created   : 26-10-2010
  Modifications  :
*******************************************************************************/
void CSigGrphConfigDlg::vEnableDisableControls()
{
    CGraphList* podList = NULL;
    podList = &(m_pMainFrame->m_odGraphList[m_omCmbBusType.GetCurSel()]);

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
}

/*******************************************************************************
  Function Name  : vEnableControls
  Input(s)       : bEnable - TRUE to Enable UI controls, FALSE to Disable
  Output         : -
  Functionality  : This fuction Enable/Disable UI controls as per input param
                   and based on tool connection status
  Member of      : CSigGrphConfigDlg
  Author(s)      : ArunKumar K
  Date Created   : 26-10-2010
  Modifications  :
*******************************************************************************/
void CSigGrphConfigDlg::vEnableControls(BOOL bEnable)
{
    // Enable only if it is not connected
    BOOL bEnableCtrl = bEnable;
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
  Function Name  : OnCbnSelchangeComboBusType
  Input(s)       : -
  Output         : -
  Functionality  : This function handles the selection change event for
                   Combobox IDC_COMBO_BUS_TYPE.
  Member of      : CSigGrphConfigDlg
  Author(s)      : ArunKumar K
  Date Created   : 27-10-2010
  Modifications  :
*******************************************************************************/
void CSigGrphConfigDlg::OnCbnSelchangeComboBusType()
{
    // Update UI with changes
    vPopulateElementList();
    // Update UI Controls
    vEnableDisableControls();
}

/*******************************************************************************
  Function Name  : OnLvnItemchangedListSignals
  Input(s)       : NMHDR *pNMHDR, LRESULT *pResult
  Output         : -
  Functionality  : This function handles the LVN_ITEMCHANGED event for
                   signals list control IDC_LIST_SIGNALS
  Member of      : CSigGrphConfigDlg
  Author(s)      : ArunKumar K
  Date Created   : 27-10-2010
  Modifications  :
*******************************************************************************/
void CSigGrphConfigDlg::OnLvnItemchangedListSignals(NMHDR* pNMHDR, LRESULT* pResult)
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
            podList = &(m_pMainFrame->m_odGraphList[m_omCmbBusType.GetCurSel()]);

            // Update Element Details
            if( podList != NULL )
            {
                if( hSelItem < podList->m_omElementList.GetSize() )
                {
                    vSetElementDetails(podList->m_omElementList[ hSelItem ] );
                }
                else
                {
                    AfxMessageBox( defSTR_ELEMENT_NOT_FOUND, MB_ICONSTOP);
                }
            }
        }
    }

    // Update UI Controls
    vEnableDisableControls();
    *pResult = 0;
}

/*******************************************************************************
  Function Name  : vSetElementDetails
  Input(s)       : odElement - CGraphElement object for which the UI will be
                   updated
  Output         : -
  Functionality  : This function updated UI controls to reflect the properties
                   of the given CGraphElement object
  Member of      : CSigGrphConfigDlg
  Author(s)      : ArunKumar K
  Date Created   : 27-10-2010
  Modifications  :
*******************************************************************************/
void CSigGrphConfigDlg::vSetElementDetails( CGraphElement odElement )
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

    // Update DDX data members
    UpdateData( FALSE );
}

/*******************************************************************************
  Function Name  : OnBnClickedButtonGrpDispConfig
  Input(s)       : -
  Output         : -
  Functionality  : This function handles the Graph display configure click event
  Member of      : CSigGrphConfigDlg
  Author(s)      : ArunKumar K
  Date Created   : 27-10-2010
  Modifications  :
*******************************************************************************/
void CSigGrphConfigDlg::OnBnClickedButtonGrpDispConfig()
{
    // Create configuration dialog and show it
    CGraphConfiguration omGraphConf;
    omGraphConf.m_eBusType = (eTYPE_BUS)m_omCmbBusType.GetCurSel();
    //Store previous Line display type
    eDISPLAY_TYPE ePrevDisplay =
        m_pMainFrame->m_odGraphList[m_omCmbBusType.GetCurSel()].m_odGraphParameters.m_eDisplayType;
    //Show Configuration dialog
    omGraphConf.DoModal();
    eDISPLAY_TYPE eCurrDisplay =
        m_pMainFrame->m_odGraphList[m_omCmbBusType.GetCurSel()].m_odGraphParameters.m_eDisplayType;

    //if line display type changed, update all Graph elements.
    if( ePrevDisplay != eCurrDisplay)
    {
        vUpdateLineDisplayOfElements(eCurrDisplay);
    }
}


/*******************************************************************************
  Function Name  : vUpdateLineDisplayOfElements
  Input(s)       : eDISPLAY_TYPE
  Output         : -
  Functionality  : This function will update the line display of all the
                   elements in currently selected bus.
  Member of      : CSigGrphConfigDlg
  Author(s)      : ArunKumar K
  Date Created   : 28-10-2010
  Modifications  :
*******************************************************************************/
void CSigGrphConfigDlg::vUpdateLineDisplayOfElements(eDISPLAY_TYPE eCurrDisplay)
{
    CGraphList* podList = NULL;
    CGraphElement odSelectedElement;
    podList = &(m_pMainFrame->m_odGraphList[m_omCmbBusType.GetCurSel()]);
    int nCounter = podList->m_omElementList.GetSize();

    for(int nIter = 0 ; nIter < nCounter; nIter++)
    {
        odSelectedElement = podList->m_omElementList[nIter];
        odSelectedElement.m_eDisplayType = eCurrDisplay;
        podList->m_omElementList[ nIter ] = odSelectedElement;
        int nCurrItem =m_omSignalList.GetSelectionMark();

        if(nCurrItem!=-1)
        {
            m_omSignalList.SetItemState( nCurrItem ,
                                         LVIS_SELECTED | LVIS_FOCUSED,
                                         LVIS_SELECTED | LVIS_FOCUSED);
            vSetElementDetails(podList->m_omElementList[ nCurrItem ] );
        }
    }
}

/*******************************************************************************
  Function Name  : OnCbnSelchangeComboType
  Input(s)       : -
  Output         : -
  Functionality  : This function will be called by the framework if the item
                   selection in the combobox got changed. This will update the
                   selection in the global list.
  Member of      : CSigGrphConfigDlg
  Author(s)      : ArunKumar K
  Date Created   : 27-10-2010
  Modifications  :
*******************************************************************************/
void CSigGrphConfigDlg::OnCbnSelchangeComboType()
{
    UpdateData();
    CGraphElement odSelectedElement;
    // Get handle to selected message item
    int hSelItem = m_omSignalList.GetNextItem(-1, LVNI_SELECTED);

    if ( hSelItem != -1 )
    {
        CGraphList* podList = NULL;
        podList = &(m_pMainFrame->m_odGraphList[m_omCmbBusType.GetCurSel()]);

        // Update Element Details
        if( podList != NULL )
        {
            if( hSelItem < podList->m_omElementList.GetSize() )
            {
                odSelectedElement = podList->m_omElementList[ hSelItem ];
                odSelectedElement.m_nLineType = m_nLineType;
                podList->m_omElementList[ hSelItem ] = odSelectedElement;
            }
            else
            {
                AfxMessageBox( defSTR_ELEMENT_NOT_FOUND, MB_ICONSTOP);
            }
        }
    }
}

/*******************************************************************************
  Function Name  : OnCbnSelchangeComboSymbol
  Input(s)       : -
  Output         : -
  Functionality  : This function will be called by the framework if the item
                   selection in the combobox got changed. This will update the
                   selection in the global list.
  Member of      : CSigGrphConfigDlg
  Author(s)      : ArunKumar K
  Date Created   : 27-10-2010
  Modifications  :
*******************************************************************************/
void CSigGrphConfigDlg::OnCbnSelchangeComboSymbol()
{
    UpdateData();
    CGraphElement odSelectedElement;
    // Get handle to selected message item
    int hSelItem = m_omSignalList.GetNextItem(-1, LVNI_SELECTED);

    if ( hSelItem != -1 )
    {
        CGraphList* podList = NULL;
        podList = &(m_pMainFrame->m_odGraphList[m_omCmbBusType.GetCurSel()]);

        // Update Element Details
        if( podList != NULL )
        {
            odSelectedElement = podList->m_omElementList[ hSelItem ];
            odSelectedElement.m_nPointType = m_nSymbolType;
            podList->m_omElementList[ hSelItem ] = odSelectedElement;
        }
        else
        {
            AfxMessageBox( defSTR_ELEMENT_NOT_FOUND, MB_ICONSTOP);
        }
    }
}

/*******************************************************************************
  Function Name  : OnColorChange
  Input(s)       : -
  Output         : -
  Functionality  : This function processes Color change event posted from the
                   CColorPopup dialog. This function updates global list value
                   as per the current selection
  Member of      : CSigGrphConfigDlg
  Author(s)      : ArunKumar K
  Date Created   : 02-11-2010
  Modifications  :
*******************************************************************************/
LRESULT CSigGrphConfigDlg::OnColorChange(WPARAM /*wparam*/, LPARAM lparam)
{
    CGraphElement odSelectedElement;
    // Get handle to selected message item
    int hSelItem = m_omSignalList.GetNextItem(-1, LVNI_SELECTED);

    if ( hSelItem != -1 )
    {
        CGraphList* podList = NULL;
        podList = &(m_pMainFrame->m_odGraphList[m_omCmbBusType.GetCurSel()]);

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
                        break;

                        // Update Sample point color
                    case IDC_POINT_COLOR:
                        odSelectedElement.m_nPointColor =
                            m_omPointColor.GetColour();
                        // Update list member
                        podList->m_omElementList[ hSelItem ] =
                            odSelectedElement;
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
