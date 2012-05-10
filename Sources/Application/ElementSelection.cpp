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
 * \file      ElementSelection.cpp
 * \brief     Implementation file for CElementSelection class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CElementSelection class
 */

// For standard Headers
#include "stdafx.h"
// For App declaration
#include "BUSMASTER.h"
// For CElementSelection class declaration
#include "ElementSelection.h"
// Signal Details Dlg
#include "SignalDetailsDlg.h"
// For resource ID Definition
#include "resource.h"
// For DIL Interface Class
#include "Include/BaseDefs.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_Datatypes.h"
#include "DataTypes/DIL_Datatypes.h"
#include "DIL_Interface/BaseDIL_CAN.h"

// DIL CAN interface
extern CBaseDIL_CAN* g_pouDIL_CAN_Interface;
extern UINT unGetMsgIDFromName(CString omMsgName);

// Application object Declaration
extern CCANMonitorApp theApp;

// Types of Tree Nodes
typedef enum
{
    eROOT = 1,
    eMESSAGE,
    eSIGNAL,
    eVAL_TYPE_RAW,
    eVAL_TYPE_PHY,
    eSTAT_VAL,
    eCHANNEL
} eTREE_NODE_TYPES;

// Color Table for global assignment
COLORREF g_dColorTable[] =
{
    RGB(0, 0, 255),
    RGB(0, 255, 0),
    RGB(0, 255, 255),
    RGB(255, 0, 0),

    RGB(255, 0, 255),
    RGB(255, 255, 0),
    RGB(128, 255, 128),
    RGB(128, 128, 0),

    RGB(0, 128, 128),
    RGB(128, 0, 128)
};
// Maximun points types supported
#define defAVAILABLE_POINT_TYPES        8

// Image Indices
#define defMSG_IMG_INDEX        0
#define defSIG_IMG_INDEX        1
#define defSTAT_IMG_INDEX       2
#define defRAW_IMG_INDEX        3
#define defPHY_IMG_INDEX        4
#define defDBC_IMG_INDEX        5


/*******************************************************************************
  Function Name  : CElementSelection
  Description    : Standard default constructor
  Member of      : CElementSelection
  Functionality  : This will initialise local variables
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
CElementSelection::CElementSelection(UINT nHardware, CWnd* pParent /*=NULL*/)
    : CDialog(CElementSelection::IDD, pParent)
{
    //{{AFX_DATA_INIT(CElementSelection)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_pMainFrame = NULL;
    m_hMessageRoot = m_hStatRoot = NULL;
    m_eBusType = CAN;
    m_nHardware  = nHardware;
}

/*******************************************************************************
  Function Name  : DoDataExchange
  Input(s)       : pDX - Pointer to DDX object
  Output         : -
  Functionality  : Maps member variables to controls and vice versa
  Member of      : CElementSelection
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CElementSelection::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CElementSelection)
    DDX_Control(pDX, IDC_LSTC_GRAPH_ELEMENTS, m_omElementList);
    DDX_Control(pDX, IDC_TREE_SIGNAL, m_omTreeEntries);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CElementSelection, CDialog)
    //{{AFX_MSG_MAP(CElementSelection)
    ON_NOTIFY(NM_DBLCLK, IDC_TREE_SIGNAL, OnDblclkTreeSignal)
    ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
    ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
    ON_BN_CLICKED(IDC_BTN_DELETE_ALL, OnBtnDeleteAll)
    ON_NOTIFY(NM_DBLCLK, IDC_LSTC_GRAPH_ELEMENTS, OnDblclkLstcGraphElements)
    ON_NOTIFY(NM_CLICK, IDC_TREE_SIGNAL, OnClickTreeSignal)
    ON_NOTIFY(NM_CLICK, IDC_LSTC_GRAPH_ELEMENTS, OnClickLstcGraphElements)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSTC_GRAPH_ELEMENTS, OnItemchangedLstcGraphElements)
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_SIGNAL, OnSelchangedTreeSignal)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/*******************************************************************************
  Function Name  : OnOK
  Input(s)       : -
  Output         : -
  Functionality  : This function will be called if the user seelcts OK or Enter.
                   This function will replace the global list with the user
                   modified list.
  Member of      : CElementSelection
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CElementSelection::OnOK()
{
    // Update List
    if( m_pMainFrame != NULL )
    {
        m_pMainFrame->m_odGraphList[m_eBusType].m_omElementList.Copy( m_odElementList );
    }

    // Call Parent function to close model loop
    CDialog::OnOK();
}

/*******************************************************************************
  Function Name  : OnInitDialog
  Input(s)       : -
  Output         : FALSE - If focus is set to Any UI control explicitly
  Functionality  : Initialises dialog's UI components
  Member of      : CElementSelection
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : Raja N on 14.12.2004, Implemented review comments
  Modifications  : Raja N on 14.3.2005, Modified to support multi channel
                   artecture. This will populate statistics tree with channel ID
*******************************************************************************/
BOOL CElementSelection::OnInitDialog()
{
    // Create Result flag
    BOOL m_bCreateSuccess = TRUE;
    CDialog::OnInitDialog();
    // Copy Element List Locally
    m_odElementList.Copy( m_pMainFrame->m_odGraphList[m_eBusType].m_omElementList );

    // Create Image List
    if( m_omImageList.Create( IDB_BMP_GRAPH_ELEMENTS,
                              defSIGNAL_ICON_SIZE, 1, WHITE_COLOR) == FALSE )
    {
        m_bCreateSuccess = FALSE;
    }
    else
    {
        m_omTreeEntries.SetImageList(&m_omImageList, TVSIL_NORMAL);
        // Create Message Root
        m_hMessageRoot = m_omTreeEntries.InsertItem( defSTR_DB_MSG_NAME,
                         defDBC_IMG_INDEX, defDBC_IMG_INDEX );
        m_omTreeEntries.SetItemData( m_hMessageRoot, eROOT );
        // Create Statistics Root
        m_hStatRoot = m_omTreeEntries.InsertItem( defSTR_STATISTICS_NAME,
                      defSTAT_IMG_INDEX, defSTAT_IMG_INDEX);
        m_omTreeEntries.SetItemData( m_hStatRoot, eROOT );
        // Insert Message
        vPopulateDBTree( m_omTreeEntries, m_hMessageRoot,
                         m_odElementList,
                         defMSG_IMG_INDEX, defSIG_IMG_INDEX,
                         defRAW_IMG_INDEX, defPHY_IMG_INDEX );
        // Create Statistics infomation for all available channels
        vCreateChannelTree( m_hStatRoot );
        // Set the focus to the tree control
        m_omTreeEntries.SetFocus();

        //SGW Code commented by Arun 21-10-2010
        // Populate List Control
        if( m_pMainFrame != NULL )
        {
            // Init List Control
            m_omElementList.SetImageList( &m_omImageList, LVSIL_SMALL );
            // Insert Columns
            // Insert category column ( For Message Names and Stat type)
            m_omElementList.InsertColumn(
                defCOL_CATOGORY_INDEX, defSTR_LIST_HEADING_CATOGORY );
            m_omElementList.SetColumnWidth(
                defCOL_CATOGORY_INDEX, defELEMENT_SELECTION_CATOGORY_WIDTH);
            // Insert Section column ( For Signal Names and Stat Param Names)
            m_omElementList.InsertColumn(
                defCOL_ELEMENT_INDEX, defSTR_LIST_HEADING_ELEMENT );
            m_omElementList.SetColumnWidth(
                defCOL_ELEMENT_INDEX, defELEMENT_SELECTION_ELEMENT_WIDTH);
            // Insert Type Column ( Only For Raw or Phy type)
            m_omElementList.InsertColumn(
                defCOL_TYPE_INDEX, defSTR_LIST_HEADING_TYPE );
            m_omElementList.SetColumnWidth(
                defCOL_TYPE_INDEX, defELEMENT_SELECTION_TYPE_WIDTH);
            // Set Additional Styles
            m_omElementList.SetExtendedStyle(
                LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
            // Populate Element list
            vPopulateElementList( m_odElementList,  // UI List Control
                                  m_omElementList,  // List of Elements
                                  defSTAT_IMG_INDEX,// Stat Img Index
                                  defRAW_IMG_INDEX, // Raw Img index
                                  defPHY_IMG_INDEX);// Phy Img index
        }
    }

    // Display the dialog
    ShowWindow( SW_SHOW );
    // Enable/disable Add,Delete & Delete All controls as per
    // the list item status
    vEnableDisableButtons();
    return FALSE;
}

/*******************************************************************************
  Function Name  : vPopulateDBTree
  Input(s)       : omTree - Object to Tree Control
                   hMessageRoot - Handle to the Root Node
                   odElemementList - Element List Reference
                   nMsgImageIndex - Index of the image for Message Node
                   nSigImageIndex - Index of the image for Signal Node
                   nRawImgIndex - Index of the image for Raw Value Node
                   nPhyImgIndex - Index of the image for Phy Value Node
  Output         : -
  Functionality  : Populates tree with the data base messages and signals
                   excluding the elements that are already added in to the
                   element list
  Member of      : CElementSelection
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : Anish
                   11.12.2006,Changed code to remove message details search
                   from name to ID,To display ID with name in the tree
*******************************************************************************/
void CElementSelection::vPopulateDBTree( CTreeCtrl& omTree,
        HTREEITEM hMessageRoot,
        const CArray< CGraphElement, CGraphElement&>& odElementList,
        int nMsgImageIndex, int nSigImageIndex,
        int nRawImgIndex, int nPhyImgIndex )
{
    // Get Database Pointer
    CMsgSignal* pomDatabase = NULL;
    pomDatabase = theApp.m_pouMsgSignal;

    if(pomDatabase != NULL)
    {
        CString omStr;
        UINT unNoOfMsgs = pomDatabase->unGetNumerOfMessages();
        UINT* pIDArray = new UINT[unNoOfMsgs];

        //// Get List of database Messages
        //CStringList omMsgList;
        //pomDatabase->omStrListGetMessageNames( omMsgList );
        if (pIDArray != NULL )
        {
            pomDatabase->unListGetMessageIDs( pIDArray );
            // Get the list iterator
            /*POSITION psPos = NULL;
            psPos = omMsgList.GetHeadPosition();*/
            sMESSAGE* pMessage = NULL;
            sSIGNALS* pSignals = NULL;
            // Init helping variables
            HTREEITEM hMessage = NULL;
            int nElementCount = 0;
            // Get the element list size
            nElementCount = odElementList.GetSize();
            int nEliminate = 0;
            CGraphElement odTemp;

            // Get all messages from the database
            for(UINT nCount=0 ; nCount<unNoOfMsgs ; nCount++)
            {
                // Init both pointers at the begining of each iteration
                pMessage = NULL;
                pSignals = NULL;
                // Get next message
                /*omStr = omMsgList.GetNext( psPos );*/
                pMessage = pomDatabase->psGetMessagePointer( pIDArray[nCount] );

                if(pMessage != NULL)
                {
                    omStr = pMessage->m_omStrMessageName;
                    //omStr += defUNDERSCORE;
                    CString omStrMsgId;
                    omStrMsgId.Format(defSTR_MSG_ID_IN_HEX,pMessage->m_unMessageCode);
                    //omStrMsgId.MakeUpper();
                    omStr += omStrMsgId;
                    // Inset the message Name
                    // Use SORT filter to have sorted tree items
                    hMessage = omTree.InsertItem( omStr, nMsgImageIndex,
                                                  nMsgImageIndex, hMessageRoot, TVI_SORT );
                    // Set the node type to Message
                    omTree.SetItemData( hMessage, eMESSAGE );
                    // Get Signal Details
                    pSignals = pMessage->m_psSignals;
                }

                // Insert Signals of this message
                while( pSignals != NULL )
                {
                    // This will be set if found in the element list
                    // Values
                    // 0 - Not Found
                    // 1 - Raw Value Found
                    // 2 - Phy Value Found
                    // 3 - Both entries found and skip insertion of this signal
                    nEliminate = 0;

                    // Iterate through element list
                    for( int nIndex = 0; nIndex < nElementCount; nIndex ++ )
                    {
                        odTemp = odElementList[ nIndex ];

                        // Check for the presence
                        if( odTemp.m_nValueType != eSTAT_PARAM &&
                                odTemp.m_nMsgID == (INT)pMessage->m_unMessageCode &&
                                odTemp.m_omStrElementName == pSignals->m_omStrSignalName)
                        {
                            nEliminate += odElementList[ nIndex ].m_nValueType;
                        }
                    }

                    // If both values are not added in to the list then add the signal
                    // Node
                    if( nEliminate < ( eRAW_VALUE + ePHY_VALUE ))
                    {
                        // Insert Signal Name
                        HTREEITEM hSignal =
                            omTree.InsertItem( pSignals->m_omStrSignalName,
                                               nSigImageIndex, nSigImageIndex, hMessage, TVI_SORT );
                        omTree.SetItemData( hSignal, eSIGNAL );

                        // Insert Raw & Phy Entries
                        if( nEliminate != eRAW_VALUE )
                        {
                            HTREEITEM hRawVal = omTree.InsertItem( defSTR_RAW_VALUE,
                                                                   nRawImgIndex,
                                                                   nRawImgIndex,
                                                                   hSignal, TVI_SORT );
                            omTree.SetItemData( hRawVal, eVAL_TYPE_RAW );
                        }

                        if( nEliminate != ePHY_VALUE )
                        {
                            HTREEITEM hPhyVal = omTree.InsertItem( defSTR_PHY_VALUE,
                                                                   nPhyImgIndex,
                                                                   nPhyImgIndex,
                                                                   hSignal, TVI_SORT );
                            omTree.SetItemData( hPhyVal, eVAL_TYPE_PHY );
                        }
                    }

                    // Get next Signal from the list
                    pSignals = pSignals->m_psNextSignalList;
                }
            }

            delete [] pIDArray;
            pIDArray = NULL;
        }
    }
}

/*******************************************************************************
  Function Name  : vPopulateStatTree
  Input(s)       : omTree - Object to Tree Control
                   hStatRoot - Handle to the Root Node
                   odElemementList - Element List Reference
                   nStatImageIndex - Index of the image for Statistics Node
  Output         : -
  Functionality  : Populates tree with statistics parameters excluding the
                   elements that are already added in to the element list
  Member of      : CElementSelection
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : Raja N on 14.3.2005, Added support for channel based
                   statistics parameters listing.
*******************************************************************************/
void CElementSelection::vPopulateStatTree(
    CTreeCtrl& omTree, HTREEITEM hStatRoot, int nChannel,
    const CArray<CGraphElement,CGraphElement&>& odElementList,
    int nStatImageIndex )

{
    // Get the List of Strings from the global stat parameter array
    int nStatParamCount = defSTAT_PARAMETERS_COUNT;
    int nListItemCount = odElementList.GetSize();
    HTREEITEM hItem = NULL;
    CString omStrEntry;
    BOOL bSkip;

    // Iterate through list of Stat params
    for( int nIndex = 0; nIndex < nStatParamCount; nIndex ++ )
    {
        bSkip = FALSE;

        for( int nItem = 0; nItem < nListItemCount; nItem++ )
        {
            // If it is stat item &
            // if the channel is same &
            // Same parameter then skip
            if( odElementList[ nItem ].m_nValueType == eSTAT_PARAM &&
                    odElementList[ nItem ].m_nFrameFormat == nChannel &&
                    odElementList[ nItem ].m_nMsgID == nIndex )
            {
                bSkip = TRUE;
            }
        }

        // If not found in the element list then add it
        if( bSkip == FALSE )
        {
            // Insert the item now
            hItem = omTree.InsertItem( acStatParams[ nIndex ],
                                       nStatImageIndex, nStatImageIndex,
                                       hStatRoot, TVI_SORT );
            // Set the item type
            omTree.SetItemData( hItem, eSTAT_VAL );
        }
    }
}

/*******************************************************************************
  Function Name  : vPopulateElementList
  Input(s)       : odElemementList - Element List Reference
                   omListCtrl - Reference to List Control Object
                   nStatImageIndex - Index of the image for Statistics Node
                   nRawValImageIndex - Index of the image for Raw Value Node
                   nPhyValImageIndex - Index of the image for Physical Val Node
  Output         : -
  Functionality  : Populates the list control with the list of elements found in
                   global graph list.
  Member of      : CElementSelection
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : Raja N on 14.3.2005, Added code to display channel ID for
                   statistics parameters for the selected elements.
*******************************************************************************/
void CElementSelection::vPopulateElementList(
    const CArray<CGraphElement, CGraphElement&> &odElementList,
    CListCtrl& omListCtrl, int nStatImageIndex,
    int nRawValImgIndex, int nPhyValImgIndex)
{
    // Get the Database Pointer to get Message Name
    CMsgSignal* pomDatabase = NULL;
    pomDatabase = theApp.m_pouMsgSignal;
    // Get element list
    int nCount = odElementList.GetSize();
    CGraphElement odTemp;

    // Iterate through list of graph elements
    for( int nIndex = 0; nIndex < nCount; nIndex++ )
    {
        odTemp = odElementList[ nIndex ];

        // Insert Stat Param
        if( odTemp.m_nValueType == eSTAT_PARAM )
        {
            if( odTemp.m_nMsgID < defSTAT_PARAMETERS_COUNT )
            {
                // Insert Catogory
                omListCtrl.InsertItem( nIndex,
                                       defSTR_STATISTICS_NAME,
                                       nStatImageIndex );
                // Insert Parameter Name
                omListCtrl.SetItemText( nIndex, 1,
                                        acStatParams[ odTemp.m_nMsgID ] );
                // Insert Channel String
                CString omStrChannel;
                omStrChannel.Format( defSTR_CHANNEL_NAME_FORMAT,
                                     defSTR_CHANNEL_NAME,
                                     odTemp.m_nFrameFormat );
                omListCtrl.SetItemText( nIndex, 2, omStrChannel );
                // Set Entry Type
                omListCtrl.SetItemData( nIndex, eSTAT_PARAM );
            }
        }
        // Insert Signal
        else
        {
            // Get Message Name from the database
            if( pomDatabase != NULL )
            {
                // Find the image index
                int nImageIndex = odTemp.m_nValueType == eRAW_VALUE ?
                                  nRawValImgIndex : nPhyValImgIndex;
                CString omStrMsgName = pomDatabase->
                                       omStrGetMessageNameFromMsgCode( odTemp.m_nMsgID );
                // Insert Message Name
                omListCtrl.InsertItem( nIndex, omStrMsgName, nImageIndex );
                // Insert signal Name
                omListCtrl.SetItemText( nIndex, 1, odTemp.m_omStrElementName );

                // Insert Value Type
                if( odTemp.m_nValueType == eRAW_VALUE )
                {
                    omListCtrl.SetItemText( nIndex, 2, defSTR_RAW_VALUE );
                }
                else
                {
                    omListCtrl.SetItemText( nIndex, 2, defSTR_PHY_VALUE );
                }

                // Set Item type
                omListCtrl.SetItemData( nIndex, odTemp.m_nValueType );
            }
        }
    }
}

/*******************************************************************************
  Function Name  : OnDblclkTreeSignal
  Input(s)       : pNMHDR - Pointer to the tree item struct
                   pResult - Pointer to the operation result
  Output         : -
  Functionality  : This function will be called by the framework if the user
                   double clicks the tree node. This will add the selected item
                   in to the element list
  Member of      : CElementSelection
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CElementSelection::OnDblclkTreeSignal(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    // Add the Selected item in to the list
    vAddSelectedItem();
    // Update UI Button status
    vEnableDisableButtons();
    // Denote success
    *pResult = 0;
}

/*******************************************************************************
  Function Name  : OnBtnAdd
  Input(s)       : -
  Output         : -
  Functionality  : This function will add the selected item in to the element
                   list
  Member of      : CElementSelection
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CElementSelection::OnBtnAdd()
{
    // Add the Selected item in to the list
    vAddSelectedItem();
    // Update UI Button status
    vEnableDisableButtons();
}

/*******************************************************************************
  Function Name  : vAddSelectedItem
  Input(s)       : -
  Output         : -
  Functionality  : This function finds the selected item and type from the tree
                   control and adds the item in to the element list. This will
                   also remove the item from the tree control.
  Member of      : CElementSelection
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : Raja N on 14.12.2004, Implemented review comments
  Modifications  : Raja N on 14.03.2005, Added code to include channel ID for
                   statistics parameters when user adds stat parameters.
*******************************************************************************/
void CElementSelection::vAddSelectedItem()
{
    // Get selected item handle
    HTREEITEM hValType = m_omTreeEntries.GetSelectedItem();

    if( hValType != NULL )
    {
        // Check whether user has clocked only on value type or Stat param
        switch( m_omTreeEntries.GetItemData( hValType ) )
        {
                // User clicked on Root nodes. Ignore now
            case eROOT:
                break;

                // User clicked on Message Name. Ignore now
            case eMESSAGE:
                break;

                // User clicked on Signal name. Ignore now
            case eSIGNAL:
                break;

                // User cliecked on Channel root. Ignore now
            case eCHANNEL:
                break;

                // User clicked on Raw Value of Signal.
            case eVAL_TYPE_RAW:
            {
                if( m_omElementList.GetItemCount() < defMAX_ELEMENTS_COUNT )
                {
                    // Get Catogory, Element Name
                    HTREEITEM hCatogory = NULL, hElement = NULL;
                    hElement = m_omTreeEntries.GetParentItem( hValType );

                    if( hElement != NULL )
                    {
                        hCatogory =
                            m_omTreeEntries.GetParentItem( hElement );
                    }

                    if( hElement != NULL && hCatogory != NULL )
                    {
                        int nIndex = m_omElementList.GetItemCount();
                        CString omStrMsg, omStrSig;
                        // Insert Message Name
                        omStrMsg = m_omTreeEntries.GetItemText( hCatogory);
                        m_omElementList.InsertItem( nIndex, omStrMsg, 3);
                        // Insert Signal Name
                        omStrSig = m_omTreeEntries.GetItemText( hElement );
                        m_omElementList.SetItemText( nIndex, 1, omStrSig);
                        m_omElementList.SetItemText( nIndex, 2,
                                                     m_omTreeEntries.GetItemText( hValType ));
                        // Set entry type
                        m_omElementList.SetItemData( nIndex, eRAW_VALUE );
                        // Add the selected element to the list
                        vAddToElementList( omStrMsg, omStrSig, eRAW_VALUE );
                        // Remove the element from the tree
                        // Remove the node if this is the only element
                        m_omTreeEntries.DeleteItem( hValType );

                        if( m_omTreeEntries.ItemHasChildren( hElement ) ==
                                FALSE )
                        {
                            m_omTreeEntries.DeleteItem( hElement );
                        }
                    }
                }
                else
                {
                    // Show exelemt exceed the maximun allowed count message
                    AfxMessageBox( defSTR_MAX_ELEMENTS_EXCEEDED_MSG );
                }
            }
            break;

            // User clicked on Phy Value of Signal.
            case eVAL_TYPE_PHY:
            {
                if( m_omElementList.GetItemCount() < defMAX_ELEMENTS_COUNT )
                {
                    // Get Catogory, Element Name
                    HTREEITEM hCatogory = NULL, hElement = NULL;
                    hElement = m_omTreeEntries.GetParentItem( hValType );

                    if( hElement != NULL )
                    {
                        hCatogory =
                            m_omTreeEntries.GetParentItem( hElement );
                    }

                    if( hElement != NULL && hCatogory != NULL )
                    {
                        int nIndex = m_omElementList.GetItemCount();
                        CString omStrMsg, omStrSig;
                        // Insert Message Name
                        omStrMsg = m_omTreeEntries.GetItemText( hCatogory);
                        m_omElementList.InsertItem( nIndex, omStrMsg, 4);
                        // Insert Signal Name
                        omStrSig = m_omTreeEntries.GetItemText( hElement );
                        m_omElementList.SetItemText( nIndex, 1, omStrSig);
                        m_omElementList.SetItemText( nIndex, 2,
                                                     m_omTreeEntries.GetItemText( hValType ));
                        // Set entry type
                        m_omElementList.SetItemData( nIndex, ePHY_VALUE );
                        // Add the selected element to the list
                        vAddToElementList( omStrMsg, omStrSig, ePHY_VALUE );
                        // Remove the element from the tree
                        // Remove the node if this is the only element
                        m_omTreeEntries.DeleteItem( hValType );

                        if( m_omTreeEntries.ItemHasChildren( hElement ) ==
                                FALSE )
                        {
                            m_omTreeEntries.DeleteItem( hElement );
                        }
                    }
                }
                else
                {
                    // Show exelemt exceed the maximun allowed count message
                    AfxMessageBox( defSTR_MAX_ELEMENTS_EXCEEDED_MSG );
                }
            }
            break;

            case eSTAT_VAL:
            {
                if( m_omElementList.GetItemCount() < defMAX_ELEMENTS_COUNT )
                {
                    // Get Catogory, Element Name
                    HTREEITEM hCatogory = NULL;
                    HTREEITEM hChannel = NULL;
                    // Get channel Index
                    hChannel = m_omTreeEntries.GetParentItem( hValType );

                    if( hChannel != NULL )
                    {
                        // Get catogory
                        hCatogory =
                            m_omTreeEntries.GetParentItem( hChannel );
                    }

                    if( hChannel != NULL && hCatogory != NULL )
                    {
                        int nIndex = m_omElementList.GetItemCount();
                        // Insert Statistics parameter
                        m_omElementList.InsertItem( nIndex,
                                                    m_omTreeEntries.GetItemText( hCatogory), 2);
                        // Set the parameter name
                        m_omElementList.SetItemText( nIndex, 1,
                                                     m_omTreeEntries.GetItemText( hValType ) );
                        // Set the channel index
                        m_omElementList.SetItemText( nIndex, 2,
                                                     m_omTreeEntries.GetItemText( hChannel ) );
                        // Set entry type
                        m_omElementList.SetItemData( nIndex, eSTAT_PARAM );
                        // Add the selected value to the list
                        vAddStatToElementList(
                            m_omTreeEntries.GetItemText( hValType ),
                            m_omTreeEntries.GetItemText( hChannel ));
                        // Remove the element from the list
                        m_omTreeEntries.DeleteItem( hValType );
                    }
                }
                else
                {
                    // Show exelemt exceed the maximun allowed count message
                    AfxMessageBox( defSTR_MAX_ELEMENTS_EXCEEDED_MSG );
                }
            }
            break;

            // Invalid value for Item data.
            default:
                ASSERT( FALSE );
        }
    }
}

/*******************************************************************************
  Function Name  : vAddToElementList
  Input(s)       : omStrMsgName - Message Name
                   omStrSigName - Signal Name
                   nValType - Value Type : Raw or Phy.
  Output         : -
  Functionality  : This function adds the signal in to the graph list. This will
                   also assign auto color to line and sample point. This will
                   also add the auto symbol type
  Member of      : CElementSelection
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : ArunKumar K
                   03.11.2010
                   Added the code for message name storage
                   ArunKumar K
                   28.10.2010
                   Updated the code for assigning default line display type.
*******************************************************************************/
void CElementSelection::vAddToElementList( CString omStrMsgName,
        CString omStrSigName,
        int nValType )
{
    // Get Database Pointer
    CMsgSignal* pomDatabase = NULL;
    pomDatabase = theApp.m_pouMsgSignal;

    // Add Element to the list
    if( pomDatabase != NULL )
    {
        CGraphElement odNewElement;
        int nMsgID = unGetMsgIDFromName( omStrMsgName );
        odNewElement.m_nMsgID = nMsgID;
        odNewElement.m_omStrElementName = omStrSigName;
        odNewElement.m_strMsgName = pomDatabase->omStrGetMessageNameFromMsgCode(nMsgID);
        odNewElement.m_nValueType = nValType;
        // Auto assignment of Color
        int nCount = m_odElementList.GetSize();
        // Assign next color index to this element
        odNewElement.m_nLineColor =
            g_dColorTable[ nCount % defMAX_ELEMENTS_COUNT ];
        // Assign symbol to the next index
        odNewElement.m_nPointType = nCount + 1;

        if( odNewElement.m_nPointType > defAVAILABLE_POINT_TYPES )
        {
            odNewElement.m_nPointType -= defAVAILABLE_POINT_TYPES;
        }

        // Assign symbol Color
        int nReverseIndex = defMAX_ELEMENTS_COUNT - nCount - 1;

        if( nReverseIndex < 0 )
        {
            nReverseIndex = 0;
        }

        odNewElement.m_nPointColor = g_dColorTable[ nReverseIndex ];
        odNewElement.m_eDisplayType =
            m_pMainFrame->m_odGraphList[m_eBusType].m_odGraphParameters.m_eDisplayType;
        // Add the element to the list
        m_odElementList.Add( odNewElement );
    }
}

/*******************************************************************************
  Function Name  : vAddStatToElementList
  Input(s)       : omStatParamName - Statistics parameter Name
                   omChannel - Channel ID.
  Output         : -
  Functionality  : This function adds the statistics parameter in to the graph
                   list. This will also assign auto color to line and sample
                   point. This will also add the auto symbol type.
  Member of      : CElementSelection
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : Raja N on 14.03.2005, Added parameter Channel ID to support
                   multi channel
*******************************************************************************/
void CElementSelection::vAddStatToElementList( CString omStatParamName,
        CString omChannel )
{
    CGraphElement odNewElement;
    // Extract Channel information
    int nPos = omChannel.ReverseFind( defEMPTY_CHAR );
    // Set the channel to default
    int nChannel = 1;

    // Get the Channel Number
    if( nPos != -1 )
    {
        // Get the channal numeric value
        nChannel = atoi( omChannel.Mid( nPos + 1 ) );
    }

    for( int nIndex = 0; nIndex < defSTAT_PARAMETERS_COUNT; nIndex++)
    {
        // Search for the name
        if( acStatParams[ nIndex ] == omStatParamName )
        {
            // If Found then Add it in to the list
            // Save the parameter index
            odNewElement.m_nMsgID = nIndex;
            // Set the parameter name
            odNewElement.m_omStrElementName = omStatParamName;
            // Set the channel ID
            odNewElement.m_nFrameFormat = (short)nChannel;
            // Set the parameter type
            odNewElement.m_nValueType = eSTAT_PARAM;
            // Auto assignment of Color
            int nCount = m_odElementList.GetSize();
            // Assign next color index to this element
            odNewElement.m_nLineColor =
                g_dColorTable[ nCount % defMAX_ELEMENTS_COUNT ];
            // Assign symbol to the next index
            odNewElement.m_nPointType = nCount + 1;

            if( odNewElement.m_nPointType > defAVAILABLE_POINT_TYPES )
            {
                odNewElement.m_nPointType -= defAVAILABLE_POINT_TYPES;
            }

            // Assign symbol Color
            int nReverseIndex = defMAX_ELEMENTS_COUNT - nCount - 1;

            if( nReverseIndex < 0 )
            {
                nReverseIndex = 0;
            }

            odNewElement.m_nPointColor = g_dColorTable[ nReverseIndex ];
            // Add the Element to the list
            m_odElementList.Add( odNewElement );
            // Break the loop
            nIndex = defSTAT_PARAMETERS_COUNT;
        }
    }
}

/*******************************************************************************
  Function Name  : OnBtnDelete
  Input(s)       : -
  Output         : -
  Functionality  : This function will delete the selected item from the element
                   list. This will also update list control and tree control to
                   be in sync with the global element list.
  Member of      : CElementSelection
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : Raja N on 14.12.2004, Implemented review comments
  Modifications  : Raja N on 14.03.2005, Added parameter Channel ID to support
                   multi channel
*******************************************************************************/
void CElementSelection::OnBtnDelete()
{
    // Get selected item
    int nSelItem = m_omElementList.GetNextItem(-1, LVNI_SELECTED);

    // If selection is valid
    if( nSelItem != -1 )
    {
        // Get the type of entry
        int nEntryType = m_omElementList.GetItemData( nSelItem );

        // Switch the item type
        switch( nEntryType )
        {
                // Statistics parameter
            case eSTAT_PARAM:
            {
                // Remove the element and add it to the tree view
                // Order is not important
                // Get the Parameter Name
                CString omStrParam = m_omElementList.GetItemText( nSelItem, 1 );
                // Get the parameter type. That is Channel Id
                CString omStrChannel =
                    m_omElementList.GetItemText( nSelItem, 2 );
                // Flag to indicate item delete in case of missing channel
                BOOL bDeleteListItem = TRUE;
                // Now Insert the new item in to the stat tree
                HTREEITEM hChannel = hGetChannelHandle( omStrChannel );

                if( hChannel != NULL )
                {
                    // Insert the new item
                    HTREEITEM hItem = m_omTreeEntries.InsertItem( omStrParam,
                                      defSTAT_IMG_INDEX, defSTAT_IMG_INDEX,
                                      hChannel, TVI_SORT );
                    // Set the item property
                    m_omTreeEntries.SetItemData( hItem, eSTAT_VAL );
                }
                else
                {
                    // The channel is not found in the tree
                    // Alert the user about this
                    if ( AfxMessageBox( defSTR_INVALID_CHANNEL, MB_YESNO )
                            != IDYES )
                    {
                        // User wants to delete this item
                        bDeleteListItem = FALSE;
                    }
                }

                // If deletion is required
                if( bDeleteListItem == TRUE )
                {
                    // remove the entry from UI List
                    m_omElementList.DeleteItem( nSelItem );
                    // Remove the node from the element list
                    m_odElementList.RemoveAt(nSelItem);
                }
            }
            break;

            // Signal Raw Value
            case eRAW_VALUE:
            {
                // Remove the element and add it to the tree view
                // Order is not important
                CString omStrMsg = m_omElementList.GetItemText( nSelItem, 0 );
                CString omStrSig = m_omElementList.GetItemText( nSelItem, 1 );

                //Remove the Hexadecimal value in braces
                if(omStrMsg.Find('[') != -1)
                {
                    omStrMsg = omStrMsg.Left(omStrMsg.Find('['));
                }

                // remove the entry from UI List
                m_omElementList.DeleteItem( nSelItem );
                // Remove the node from the element list
                m_odElementList.RemoveAt(nSelItem);
                // Get the signal handle. This will create signal if
                // message found and signal not found
                HTREEITEM hSignal =
                    hGetElementHandle( omStrMsg, omStrSig, TRUE );

                if( hSignal != NULL )
                {
                    // Now Insert the new item in to the stat tree
                    HTREEITEM hRawVal = m_omTreeEntries.InsertItem(
                                            defSTR_RAW_VALUE, defRAW_IMG_INDEX,
                                            defRAW_IMG_INDEX, hSignal, TVI_SORT );
                    // Set the item property
                    m_omTreeEntries.SetItemData( hRawVal, eVAL_TYPE_RAW );
                    // Expand the tree item
                    m_omTreeEntries.Expand( hSignal, TVE_EXPAND );
                }
                else
                {
                    // Display dead signal message. User has to remove
                    // this entry from the list
                    AfxMessageBox( defSTR_ELEMENT_NOT_FOUND, MB_ICONSTOP );
                }
            }
            break;

            // Signal Physical Value
            case ePHY_VALUE:
            {
                // Remove the element and add it to the tree view
                // Order is not important
                CString omStrMsg = m_omElementList.GetItemText( nSelItem, 0 );
                CString omStrSig = m_omElementList.GetItemText( nSelItem, 1 );

                //Remove the Hexadecimal value in braces
                if(omStrMsg.Find('[') != -1)
                {
                    omStrMsg = omStrMsg.Left(omStrMsg.Find('['));
                }

                // remove the entry from UI List
                m_omElementList.DeleteItem( nSelItem );
                // Remove the node from the element list
                m_odElementList.RemoveAt(nSelItem);
                // Get the signal handle. This will create signal if
                // message found and signal not found
                HTREEITEM hSignal =
                    hGetElementHandle( omStrMsg, omStrSig, TRUE );

                if( hSignal != NULL )
                {
                    // Now Insert the new item in to the stat tree
                    HTREEITEM hPhyVal = m_omTreeEntries.InsertItem(
                                            defSTR_PHY_VALUE, defPHY_IMG_INDEX,
                                            defPHY_IMG_INDEX, hSignal, TVI_SORT );
                    // Set the item property
                    m_omTreeEntries.SetItemData( hPhyVal, eVAL_TYPE_PHY );
                    // Expand the signal
                    m_omTreeEntries.Expand( hSignal, TVE_EXPAND );
                }
                else
                {
                    // Show dead signal message. User has to remove this
                    // dead message/signal fromthe list
                    AfxMessageBox( defSTR_ELEMENT_NOT_FOUND, MB_ICONSTOP );
                }
            }
            break;

            default:
                // Invalid type data found
                // Reason could be the data type is not set for the selected
                // item using SetItemData. Use SetItemData with the element
                // value type
                ASSERT( FALSE );
        }
    }

    // Update Button status after delete
    vEnableDisableButtons();
}

/*******************************************************************************
  Function Name  : hGetElementHandle
  Input(s)       : omStrMsg - Message Name
                   omStrSig - Signal Name
                   bCreate - Flag to indicate auto creation of tree node if not
                   found. TRUE to do auto create.
  Output         : -
  Functionality  : This function searches the tree control to find the entry of
                   the message and signal specified. If the signal not found
                   this will create the signal node if bCreate flag is TRUE.
  Member of      : CElementSelection
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
HTREEITEM CElementSelection::hGetElementHandle( const CString& omStrMsg,
        const CString& omStrSig,
        BOOL bCreate )
{
    // Get First Child node of Message
    HTREEITEM hCurrent =
        m_omTreeEntries.GetNextItem( m_hMessageRoot, TVGN_CHILD);
    HTREEITEM hMessage = NULL;

    while( hCurrent != NULL )
    {
        CString omStrEntry = m_omTreeEntries.GetItemText( hCurrent );

        //Remove the Hexadecimal value in braces for comparision only
        if(omStrEntry.Find('[') != -1)
        {
            omStrEntry = omStrEntry.Left(omStrEntry.Find('['));
        }

        if( omStrEntry == omStrMsg )
        {
            hMessage = hCurrent;
            // Find for signal
            hCurrent = m_omTreeEntries.GetNextItem( hCurrent, TVGN_CHILD);

            while ( hCurrent != NULL )
            {
                omStrEntry = m_omTreeEntries.GetItemText( hCurrent );

                // Found signal and return the same
                if( omStrEntry == omStrSig )
                {
                    return hCurrent;
                }

                hCurrent = m_omTreeEntries.GetNextItem( hCurrent, TVGN_NEXT);
            }

            if( bCreate == TRUE )
            {
                // Insert Signal Name
                HTREEITEM hSignal = m_omTreeEntries.InsertItem( omStrSig,
                                    defSIG_IMG_INDEX, defSIG_IMG_INDEX, hMessage, TVI_SORT );
                // Set the signal node type
                m_omTreeEntries.SetItemData( hSignal, eSIGNAL );
                // Return the created node handle
                return hSignal;
            }
        }
        else
        {
            hCurrent = m_omTreeEntries.GetNextItem( hCurrent, TVGN_NEXT);
        }
    }

    // Return NULL to indicate Failure
    return NULL;
}

/*******************************************************************************
  Function Name  : OnBtnDeleteAll
  Input(s)       : -
  Output         : -
  Functionality  : Clears graph list and refreshes the tree control for update.
  Member of      : CElementSelection
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : Raja N on 14.03.2005, Added code to support multi channel
*******************************************************************************/
void CElementSelection::OnBtnDeleteAll()
{
    // Remove All entries from the UI List
    m_omElementList.DeleteAllItems();
    // Empty Element List
    m_odElementList.RemoveAll();
    // Delete all elements from the tree control
    m_omTreeEntries.DeleteItem( m_hMessageRoot );
    m_omTreeEntries.DeleteItem( m_hStatRoot );
    // Populate Tree View
    // Create Message Root
    m_hMessageRoot = m_omTreeEntries.InsertItem( defSTR_DB_MSG_NAME,
                     defDBC_IMG_INDEX, defDBC_IMG_INDEX );
    m_omTreeEntries.SetItemData( m_hMessageRoot, eROOT );
    // Create Statistics Root
    m_hStatRoot = m_omTreeEntries.InsertItem( defSTR_STATISTICS_NAME,
                  defSTAT_IMG_INDEX, defSTAT_IMG_INDEX);
    m_omTreeEntries.SetItemData( m_hStatRoot, eROOT );
    // Insert Message
    vPopulateDBTree( m_omTreeEntries, m_hMessageRoot,
                     m_odElementList, defMSG_IMG_INDEX, defSIG_IMG_INDEX,
                     defRAW_IMG_INDEX, defPHY_IMG_INDEX );
    // Insert Statistics parameters
    vCreateChannelTree( m_hStatRoot );
    // Update UI Button Status
    vEnableDisableButtons();
}

/*******************************************************************************
  Function Name  : OnDblclkLstcGraphElements
  Input(s)       : pNMHDR - Pointer to the List item struct
                   pResult - Pointer to the operation result
  Output         : -
  Functionality  : This function will show the details of the signal. If the
                   selected item is not a signal, this will ignore the call.
  Member of      : CElementSelection
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : Anish
                   12.12.2006,Change code to get Msg ID from the name and search
                   pointer to the Msg struct using ID instead of name
*******************************************************************************/
void CElementSelection::OnDblclkLstcGraphElements(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    // Get Selected Item
    int nSelItem = m_omElementList.GetNextItem(-1, LVNI_SELECTED);

    if( nSelItem != -1 )
    {
        CString omStrMsg, omStrSig;

        switch( m_omElementList.GetItemData( nSelItem ))
        {
            case eSTAT_PARAM:
                // Nothing to show for statistics parameters
                break;

                // For Database Messages
            case eRAW_VALUE:
            case ePHY_VALUE:
            {
                // Get Message and Signal Name
                CString omStrMsg = m_omElementList.GetItemText( nSelItem, 0 );
                CString omStrSig = m_omElementList.GetItemText( nSelItem, 1 );
                // Get Signal Pointer from the database module
                CMsgSignal* pomDatabase = theApp.m_pouMsgSignal;

                if( pomDatabase != NULL )
                {
                    //To find out the ID from the displayed name
                    UINT unMsgID = unGetMsgIDFromName(omStrMsg);
    				if (unMsgID == -1)
					{
						unMsgID = m_odElementList.GetAt(nSelItem).m_nMsgID;
					}
                    sMESSAGE* psMsg =
                        pomDatabase->psGetMessagePointer( unMsgID );

                    if( psMsg != NULL )
                    {
                        // Get the Signal Details
                        sSIGNALS* psSig = psMsg->m_psSignals;

                        while( psSig != NULL )
                        {
                            if( psSig->m_omStrSignalName == omStrSig )
                            {
                                // Create Signal Details Dialog
                                CSignalDetailsDlg
                                omDetails( MD_READ_ONLY, psSig, this );
                                // Show Details
                                omDetails.DoModal();
                                // Break the loop
                                psSig = NULL;
                            }
                            else
                            {
                                psSig = psSig->m_psNextSignalList;
                            }
                        }
                    }
                }
            }
            break;

            default:
                // Invalid type data found
                // Reason could be the data type is not set for the selected
                // item using SetItemData. Use SetItemData with the element
                // value type
                ASSERT( FALSE );
        }
    }

    // Update Buttons Status
    vEnableDisableButtons();
    // Update the result
    *pResult = 0;
}

/*******************************************************************************
  Function Name  : vEnableDisableButtons
  Input(s)       : -
  Output         : -
  Functionality  : Updates UI button status based on UI contols status
  Member of      : CElementSelection
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : To check which item is selected and update the Add button status
*******************************************************************************/
void CElementSelection::vEnableDisableButtons()
{
    CWnd* pWnd = NULL;
    BOOL bAddEnable, bDeleteEnable, bDeleteAllEnable;
    bAddEnable = bDeleteEnable = bDeleteAllEnable = FALSE;
    // Get Add Enable condition
    HTREEITEM hValType = m_omTreeEntries.GetSelectedItem();

    if( hValType != NULL )
    {
        // Check whether user has clocked only on value type or Stat param
        switch( m_omTreeEntries.GetItemData( hValType ) )
        {
                // User clicked on Root nodes. Ignore now
            case eVAL_TYPE_RAW:

                // User clicked on Message Name. Ignore now
            case eVAL_TYPE_PHY:

                // User clicked on Signal name. Ignore now
            case eSTAT_VAL:
                bAddEnable =
                    m_omElementList.GetItemCount() < defMAX_ELEMENTS_COUNT ? TRUE : FALSE;
                break;
        }
    }

    // Get Delete & Delete All Button Enable Condition
    if( m_omElementList.GetItemCount() > 0 )
    {
        bDeleteAllEnable = TRUE;

        if(  m_omElementList.GetSelectedCount() > 0 )
        {
            bDeleteEnable = TRUE;
        }
    }

    // Update Buttons
    // Enable Disable Add Button
    pWnd = GetDlgItem( IDC_BTN_ADD );

    if( pWnd != NULL )
    {
        pWnd->EnableWindow( bAddEnable );
    }

    // Enable Disable Delete Button
    pWnd = GetDlgItem( IDC_BTN_DELETE );

    if( pWnd != NULL )
    {
        pWnd->EnableWindow( bDeleteEnable );
    }

    // Enable Delete Delete All
    pWnd = GetDlgItem( IDC_BTN_DELETE_ALL );

    if( pWnd != NULL )
    {
        pWnd->EnableWindow( bDeleteAllEnable );
    }
}


/*******************************************************************************
  Function Name  : OnClickTreeSignal
  Input(s)       : pNMHDR - Pointer to the tree item struct
                   pResult - Pointer to the operation result
  Output         : -
  Functionality  : Updates the UI with respect to the selection change
  Member of      : CElementSelection
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CElementSelection::OnClickTreeSignal(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    // Update Button Status
    vEnableDisableButtons();
    *pResult = 0;
}

/*******************************************************************************
  Function Name  : OnClickLstcGraphElements
  Input(s)       : pNMHDR - Pointer to the List item struct
                   pResult - Pointer to the operation result
  Output         : -
  Functionality  : Updates the UI with respect to the selection change
  Member of      : CElementSelection
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CElementSelection::OnClickLstcGraphElements(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    // Update Button Status
    vEnableDisableButtons();
    *pResult = 0;
}

/*******************************************************************************
  Function Name  : OnItemchangedLstcGraphElements
  Input(s)       : pNMHDR - Pointer to the List item struct
                   pResult - Pointer to the operation result
  Output         : -
  Functionality  : Updates the UI with respect to the selection change
  Member of      : CElementSelection
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
void CElementSelection::OnItemchangedLstcGraphElements(NMHDR* /*pNMHDR*/,
        LRESULT* pResult)
{
    // Update Button Status
    vEnableDisableButtons();
    *pResult = 0;
}

/*******************************************************************************
  Function Name  : vCreateChannelTree
  Input(s)       : hStatRoot - Root of the Channel statistics information
  Output         : -
  Functionality  :
  Member of      : CElementSelection
  Author(s)      : Raja N
  Date Created   : 23.2.2005
  Modifications  : ArunKumar K, 24.3.2011,
                   Replaced defNO_OF_CHANNELS with m_nHardware.
*******************************************************************************/
void CElementSelection::vCreateChannelTree(HTREEITEM hStatRoot)
{
    // Insert Root for each channel
    int nChannels = m_nHardware;

    // Insert channel root for each item
    for( int nIndex = 0; nIndex < nChannels; nIndex++ )
    {
        CString omStrChannel;
        omStrChannel.Format( defSTR_CHANNEL_NAME_FORMAT,
                             defSTR_CHANNEL_NAME,
                             nIndex + 1);
        // Insert Channel Root
        HTREEITEM hChannel = m_omTreeEntries.InsertItem( omStrChannel,
                             defSTAT_IMG_INDEX, defSTAT_IMG_INDEX,
                             hStatRoot );
        // Set the type as channel node
        m_omTreeEntries.SetItemData( hChannel, eCHANNEL );
        // Insert Statistics parameters
        vPopulateStatTree( m_omTreeEntries, hChannel, nIndex + 1,
                           m_odElementList, defSTAT_IMG_INDEX );
    }
}

/*******************************************************************************
  Function Name  : hGetChannelHandle
  Input(s)       : omStrMsg - Message Name
                   omStrSig - Signal Name
  Output         : Handle of the statistics channel
  Functionality  : This function searches the tree control to find the entry of
                   the statistics channel. If the element found this will return
                   the handle otherwise NULL.
  Member of      : CElementSelection
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  :
*******************************************************************************/
HTREEITEM CElementSelection::hGetChannelHandle(const CString& omStrChannel)
{
    HTREEITEM hChannel = NULL;
    // Get First Child node of Message
    HTREEITEM hCurrent =
        m_omTreeEntries.GetNextItem( m_hStatRoot, TVGN_CHILD);

    // If it is a valid root handle
    while( hCurrent != NULL )
    {
        // Get next item in the tree
        CString omStrEntry = m_omTreeEntries.GetItemText( hCurrent );

        // Check with the required handle's text
        if( omStrEntry == omStrChannel )
        {
            hChannel = hCurrent;
            // Break the loop
            hCurrent = NULL;
        }
        else
        {
            hCurrent = m_omTreeEntries.GetNextItem( hCurrent, TVGN_NEXT);
        }
    }

    // Return the searched handle if found otherwise the initial value NULL
    return hChannel;
}
/*******************************************************************************
  Function Name  : OnSelchangedTreeSignal
  Input(s)       : pNMHDR - Pointer to the List item struct
                   pResult - Pointer to the operation result
  Output         : -
  Functionality  : Updates the UI with respect to the selection change
  Member of      : CElementSelection
  Author(s)      : Harika M
  Date Created   : 27/02/2006
  Modifications  :
*******************************************************************************/
void CElementSelection::OnSelchangedTreeSignal(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    //NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    // Update Button Status
    vEnableDisableButtons();
    *pResult = 0;
}
