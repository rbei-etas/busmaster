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
 * \file      TxMsgListView.cpp
 * \brief     Implementation file for CTxMsgListView class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CTxMsgListView class
 */

#include "TxWindow_stdafx.h"             // For standard include
#include "TxWindow_resource.h"  
#include "Utility/SignalMatrix.h"       // For Signal Matrix Class Definition
#include "Utility/ComboItem.h"          // For Custom Combobox Implementation
#include "Utility/EditItem.h"           // For Custom Editbox Implementation
#include "Utility/RadixEdit.h"          // For the RAdix Edit control definition
#include "Utility/NumSpinCtrl.h"        // For the custom spin control
#include "Utility/NumEdit.h"            // For Custom Numeric Edit control Impl
#include "Utility/FFListctrl.h"         // For Flicker Free List class definition
#include "FlexListCtrl.h"       // For editable list control implementation
#include "TxMsgListView.h"      // For CTxMsgListView class definition
#include "TxMsgChildFrame.h"    // For Tx Child Window definition

// For Glodal Application Object
//extern CCANMonitorApp theApp;
// For global stop flag for Message Transmission
extern BOOL g_bStopSelectedMsgTx;

IMPLEMENT_DYNCREATE(CTxMsgListView, CFormView)

/*******************************************************************************
  Function Name  : CTxMsgListView
  Description    : Standard default constructor
  Member of      : CTxMsgListView
  Functionality  : This will initialise local variables
  Author(s)      : Raja N
  Date Created   : 20.4.2005
  Modifications  :
*******************************************************************************/
CTxMsgListView::CTxMsgListView()
    : CFormView(CTxMsgListView::IDD)
{
    //{{AFX_DATA_INIT(CTxMsgListView)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_nSelectedMsgIndex  = -1;
    m_bInitDlg = FALSE;
}

/*******************************************************************************
  Function Name  : ~CTxMsgListView
  Description    : Standard Destructor
  Member of      : CTxMsgListView
  Functionality  : -
  Author(s)      : Raja N
  Date Created   : 20.4.2005
  Modifications  :
*******************************************************************************/
CTxMsgListView::~CTxMsgListView()
{
}

/*******************************************************************************
  Function Name  : DoDataExchange
  Input(s)       : pDX - Pointer to DDX object
  Output         : -
  Functionality  : This function will map DDX object with UI control for data
                   exchange
  Member of      : CTxMsgListView
  Author(s)      : Raja N
  Date Created   : 27.4.2005
  Modifications  : 
*******************************************************************************/
void CTxMsgListView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTxMsgListView)
    DDX_Control(pDX, IDC_LSTC_MSG_DETAILS, m_omLctrMsgList);
    DDX_Control(pDX, IDC_CBTN_DELETE_ALL_MSG, m_omButtonDeleteAllMsg);
    DDX_Control(pDX, IDC_CBTN_DELETE_SEL_MSG, m_omButtonDeleteSelMsg);
    DDX_Control(pDX, IDC_CBTN_SEND_MSG, m_omButtonSendMsg);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTxMsgListView, CFormView)
    //{{AFX_MSG_MAP(CTxMsgListView)
    ON_NOTIFY(LVN_COLUMNCLICK, IDC_LSTC_MSG_DETAILS, OnColumnclickLstcMsgDetails)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSTC_MSG_DETAILS, OnItemchangedLstcMsgDetails)
    ON_BN_CLICKED(IDC_CBTN_SEND_MSG, OnSendSelectedMsg)
    ON_BN_CLICKED(IDC_CBTN_DELETE_SEL_MSG, OnDeleteSelectedMsg)
    ON_BN_CLICKED(IDC_CBTN_DELETE_ALL_MSG, OnDeleteAllMsg)
    ON_COMMAND(IDM_DELETE_ALL_MSG, OnDeleteAllMsg)
    ON_COMMAND(IDM_SEND_SEL_MSG, OnSendSelectedMsg)
    ON_COMMAND(IDM_DELETE_SEL_MSG, OnDeleteSelectedMsg)
    ON_NOTIFY(NM_RCLICK, IDC_LSTC_MSG_DETAILS, OnRightClickMsgDetails)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


#ifdef _DEBUG
void CTxMsgListView::AssertValid() const
{
    CFormView::AssertValid();
}

void CTxMsgListView::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}
#endif //_DEBUG


/*******************************************************************************
  Function Name  : OnInitialUpdate
  Input(s)       : -
  Output         : -
  Functionality  : This function will be called by the framework during initial
                   show of this view. This function will register iteself in to
                   parent window class so that other views shall access it. And
                   this function will initialise signal list, message ID/Name
                   combobox and other UI components
  Member of      : CTxMsgListView
  Author(s)      : Raja N
  Date Created   : 27.4.2005
  Modifications  : 
*******************************************************************************/
void CTxMsgListView::OnInitialUpdate() 
{
    CFormView::OnInitialUpdate();
    // Initialise window pointer in the Tx child window
    CTxMsgChildFrame * pomChildFrame =
                        (CTxMsgChildFrame * )pomGetParentWindow();
    // Update View Pointer
    if( pomChildFrame != NULL )
    {
        pomChildFrame->vSetTxMsgViewPointers( eTxMsgMessageListView, this );
    }
    // set Init flag to TRUE
    m_bInitDlg = TRUE;

    // Init Message List Control
    CRect rListCtrlRect;
    CHAR caColumnName[defMESSAGE_FRAME_COLUMN][defSTRING_SIZE] = {
            defMESSAGE_ID,
            defSTR_CHANNEL_NAME,
            defMESSAGE_TYPE,
            defMESSSAGE_DLC,
            defMESSAGE_DATA_BYTES };
    //Calculate the total size of all column header   
    m_omLctrMsgList.GetWindowRect( &rListCtrlRect);
    int nTotalColunmSize     = rListCtrlRect.right - rListCtrlRect.left;
    int nTotalStrLengthPixel = 0;
    
    int i;  //i declared outside the for loop
    for( i=0; i<defMESSAGE_FRAME_COLUMN;i++)
    {
         nTotalStrLengthPixel += 
             m_omLctrMsgList.GetStringWidth(caColumnName[i]) ;
    }
    //Insert each column name after calculating the size for the same.
    INT nFormat = 0;
    for(i=0; i<defMESSAGE_FRAME_COLUMN;i++)
    {
        int nColumnSize  = m_omLctrMsgList.GetStringWidth(caColumnName[i]) ;
        nColumnSize += 
            (nTotalColunmSize-nTotalStrLengthPixel)/defMESSAGE_FRAME_COLUMN;
        nFormat = LVCFMT_CENTER;
        // Switch Column Index
        switch( i )
        {
            case defMESSAGE_FRAME_COLUMN - 1 : // Data Bytes Column
                nColumnSize += static_cast <INT>(4.25*defDATA_BYTES_EXTRA);
                nFormat = LVCFMT_LEFT;
                break;
            case 0: // Message ID / Name Column. Don't alter this column
                break;
            case 1: // Channels Column
                nColumnSize -= static_cast <INT>(2.2*defDATA_BYTES_EXTRA);
                break;            
            default: // Others
                nColumnSize -= static_cast <INT>(1.1*defDATA_BYTES_EXTRA );
        }
        // Insert the column in to the list
        m_omLctrMsgList.InsertColumn(i,caColumnName[i],
                                         nFormat, nColumnSize);
    }
    // Set extended property
    // Enable Check box
    m_omLctrMsgList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES );

    // Associate image list to the list item
    // Create only onece
    if( m_omImageList.m_hImageList == NULL )
    {
        m_omImageList.Create( IDR_BMP_MSGSGDB,
                                defSIGNAL_ICON_SIZE,
                                1,
                                WHITE_COLOR );
    }
    // Set the Image List
    // Only if it is sucessfully created
    if( m_omImageList.m_hImageList != NULL )
    {
        m_omLctrMsgList.SetImageList( &m_omImageList, LVSIL_SMALL);
    }
    // Associate Header control Image List
    if( m_omHeaderImageList.m_hImageList == NULL )
    {
        m_omHeaderImageList.Create( IDR_BMP_CHECKBOX,
                                defSIGNAL_ICON_SIZE,
                                1,
                                BLUE_COLOR );
    }
    // Set the Image List
    // Only if it is sucessfully created
    if( m_omHeaderImageList.m_hImageList != NULL )
    {
        CHeaderCtrl *pHeader = m_omLctrMsgList.GetHeaderCtrl();
        if( pHeader != NULL )
        {
            pHeader->SetImageList( &m_omHeaderImageList );
            HDITEM hditem;
            hditem.mask = HDI_IMAGE | HDI_FORMAT;
            if( pHeader->GetItem(0, &hditem ) == TRUE )
            {
                hditem.fmt |=  HDF_IMAGE;
                hditem.iImage = 0;
                pHeader->SetItem(0, &hditem );
            }
        }
    }

    m_omButtonSendMsg.EnableWindow(FALSE);
    m_omButtonDeleteSelMsg.EnableWindow(FALSE);
    
    // Set the selection to the first item
    m_omLctrMsgList.SetItemState( 0,
                                  LVIS_SELECTED | LVIS_FOCUSED,
                                  LVIS_SELECTED | LVIS_FOCUSED );
	m_bInitDlg = FALSE;
}

/*******************************************************************************
  Function Name  : OnColumnclickLstcMsgDetails
  Input(s)       : pNMHDR - Pointer to List Item
                   pResule - Pointer to Result
  Output         : -
  Functionality  : This function will be called by the framework when user click
                   on List control header. This function will handle this event
                   only if first column is clicked. In this condition this will
                   toggle column 0 image to show checked/unchecked checkbox
                   image and will call vSetMessageCheckValue to update message
                   details
  Member of      : CTxMsgListView
  Author(s)      : Raja N
  Date Created   : 27.4.2005
  Modifications  : 
*******************************************************************************/
void CTxMsgListView::OnColumnclickLstcMsgDetails(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    // If it is first column 
    if( pNMListView->iSubItem == 0 )
    {
        // Get the status from Image Index
        BOOL bToBeChecked = FALSE;
        // Change header Control Image Index
        CHeaderCtrl *pHeader = m_omLctrMsgList.GetHeaderCtrl();
        if( pHeader != NULL )
        {
            // Get Current Image Index
            HDITEM hditem;
            hditem.mask = HDI_IMAGE | HDI_FORMAT;
            if( pHeader->GetItem(0, &hditem ) == TRUE )
            {
                // Image Index To be Set
                int nNewImageIndex = 0;
                // Toggle Image Index
                if( hditem.iImage == 0 )
                {
                    bToBeChecked = TRUE;
                    nNewImageIndex = 1;
                }
            
                // Update Image Index
                hditem.fmt |=  HDF_IMAGE;
                hditem.iImage = nNewImageIndex;
                pHeader->SetItem(0, &hditem );
                // Update Message Check Value
                vSetMessageCheckValue( bToBeChecked );
            }
        }
    }
    *pResult = 0;
}

/*******************************************************************************
  Function Name  : vSetMessageCheckValue
  Input(s)       : bCheck - TRUE to Enable message, FALSE to disable message for
                   transmission
  Output         : -
  Functionality  : This function will update selected message with the check
                   value. This value will be assigned to m_bEnabled member of
                   message detail.
  Member of      : CTxMsgListView
  Author(s)      : Raja N
  Date Created   : 27.4.2005
  Modifications  : 
*******************************************************************************/
void CTxMsgListView::vSetMessageCheckValue(BOOL bCheck)
{
    // Get Other View Pointers
    // Blocks View
    CTxMsgBlocksView * pomBlocksView = NULL;
    pomBlocksView = ( CTxMsgBlocksView *)pomGetBlocksViewPointer();
    // Message Details View
    CTxMsgDetailsView *pomDetailsView = NULL;
    pomDetailsView = (CTxMsgDetailsView *)pomGetDetailsViewPointer();
    // Functions view
    CTxFunctionsView * pomFunctionsView = NULL;
    pomFunctionsView = (CTxFunctionsView *)pomGetFunctionsViewPointer();
    // If all pointers are valid
    if( pomBlocksView != NULL && pomDetailsView != NULL && 
                pomFunctionsView != NULL )
    {
        // If selected message block index is valid
        if(pomBlocksView->m_nSelectedMsgBlockIndex != -1 )
        {
            PSMSGBLOCKLIST psCurrentMsgBlock = NULL;
            // Get current block pointer
            psCurrentMsgBlock = pomBlocksView->psGetMsgBlockPointer(
                                    pomBlocksView->m_nSelectedMsgBlockIndex,
                                    pomBlocksView->m_psMsgBlockList );
            // Get the message list
            PSTXCANMSGLIST psMsgList = psCurrentMsgBlock->m_psTxCANMsgList;
            int nIndex = 0;
            // Set programmed UI update to TRUE
            BOOL bModified = FALSE;
			m_bInitDlg = TRUE;

            // Update check box status of all messages in this list
            while(psMsgList != NULL )
            {
                if( psMsgList->m_sTxMsgDetails.m_bEnabled != bCheck )
                {
                    // Update message list
                    psMsgList->m_sTxMsgDetails.m_bEnabled = bCheck;
                    // Update UI Control
                    m_omLctrMsgList.SetCheck( nIndex, bCheck );
                    // Update Modified flag
                    bModified = TRUE;
                }
                // Go to Next Node
                psMsgList = psMsgList->m_psNextMsgDetails;
                // Increment list item index count
                nIndex++;
            }
            // Set programmed UI update to FALSE
	         m_bInitDlg = FALSE;
            // Enable Update button only if data got modified
            if( bModified == TRUE )
            {
                // If data is modified then update apply button
                pomFunctionsView->m_omButtonApply.EnableWindow();
            }
        }
    }
}

/******************************************************************************/
/*  Function Name    :  OnItemchangedLstcMsgDetails                           */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function will be called by frame work when       */
/*                      selection is changed in message details list control. */
/*                                                                            */
/*  Member of        :  CTxMsgListView                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  22.07.2004, Added update of signal list on selection  */
/*                      change                                                */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  02.08.2004, Added function call vEnableAddButton to   */
/*                      update add button status with respect to other        */
/*                      dependent  information                                */
/******************************************************************************/
void CTxMsgListView::OnItemchangedLstcMsgDetails(NMHDR* pNMHDR, 
                                                   LRESULT* pResult) 
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    CTxMsgBlocksView * pomBlocksView = NULL;
    pomBlocksView = ( CTxMsgBlocksView *)pomGetBlocksViewPointer();
    if( pomBlocksView != NULL && m_bInitDlg == FALSE )
    {
        // Get new state of the list control items
        // and check for if it is selected and focused.
        UINT unCurrentState = pNMListView->uNewState & defMASK_LIST_CTRL;
        if(pNMListView->uNewState & (LVIS_FOCUSED|LVIS_SELECTED) )
        {
            if((TRUE == CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_CONNECTED)) && (pNMListView->uNewState & LVIS_SELECTED))
                //&& (pNMListView->uNewState&LVIS_FOCUSED))
            {
                m_omButtonSendMsg.EnableWindow(TRUE);
            }
            if(FALSE == CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG))
            {                
                m_omButtonDeleteSelMsg.EnableWindow(!CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG));
                m_omButtonDeleteAllMsg.EnableWindow(!CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG));
                
            }
        }
        else if(pNMListView->uChanged == LVIF_STATE)
        {
            
            //Check for the count
            UINT unCount = m_omLctrMsgList.GetItemCount();
            if ( unCount <= 0)
            {
                m_omButtonDeleteAllMsg.EnableWindow(FALSE);
            }
            if(!(pNMListView->uNewState & defMASK_CHECK_UNCHECK))
            {
                m_omButtonSendMsg.EnableWindow(FALSE);
                m_omButtonDeleteSelMsg.EnableWindow(FALSE);
            }
        }
        if( pNMListView->uChanged  == LVIF_STATE )
        {
            PSTXCANMSGLIST   psTxMsgList = NULL ;
            PSMSGBLOCKLIST    psMsgBlock = NULL ;
            // check if the item is focused and selected.
            unCurrentState =
                pNMListView->uNewState &(LVIS_FOCUSED |LVIS_SELECTED);
            psMsgBlock = pomBlocksView->psGetMsgBlockPointer(
                                    pomBlocksView->m_nSelectedMsgBlockIndex,
                                    pomBlocksView->m_psMsgBlockList );
            // If yes, update message information for newly selected item.
            if(unCurrentState != FALSE)
            {
                psTxMsgList = psGetMsgDetailPointer( pNMListView->iItem,
                                                     psMsgBlock );
                m_nSelectedMsgIndex = pNMListView->iItem;
                if(psTxMsgList != NULL )
                {
                    CTxMsgDetailsView * pView = (CTxMsgDetailsView *)
                                            pomGetDetailsViewPointer();
                    if( pView != NULL )
                    {
                        pView->vSetValues(&(psTxMsgList->m_sTxMsgDetails));
                        // Insert Signal List Update Code Here
                        pView->bUpdateSignalList(
                                        psTxMsgList->m_sTxMsgDetails.m_sTxMsg );
                        pView->vEnableAddButton( TRUE );
                        // Clear error message if any
                        pView->bSetStatusText(STR_EMPTY);
                    }
                }
            }
            else
            {
                m_nSelectedMsgIndex = -1;
            }
        }
        if( pNMListView->uNewState & defMASK_CHECK_UNCHECK )
        {
            PSTXCANMSGLIST   psTxMsgList = NULL ;
            PSMSGBLOCKLIST    psMsgBlock = NULL ;
            
            psMsgBlock = pomBlocksView->psGetMsgBlockPointer(
                                    pomBlocksView->m_nSelectedMsgBlockIndex,
                                    pomBlocksView->m_psMsgBlockList );

            psTxMsgList = psGetMsgDetailPointer(pNMListView->iItem,psMsgBlock);
            m_nSelectedMsgIndex = pNMListView->iItem;
            if(psTxMsgList != NULL )
            {
                int nCurrentState = 
                    m_omLctrMsgList.GetCheck(pNMListView->iItem);
                CTxFunctionsView * pView = (CTxFunctionsView *)
                                                pomGetFunctionsViewPointer();
                if( pView != NULL )
                {
                    if( nCurrentState != 
                            psTxMsgList->m_sTxMsgDetails.m_bEnabled )
                    {
                        psTxMsgList->m_sTxMsgDetails.m_bEnabled = nCurrentState;
                        // Enable Update Button
                        pView->m_omButtonApply.EnableWindow();
                    }
                }
                else
                {
                    m_omLctrMsgList.SetCheck( pNMListView->iItem,
                                    psTxMsgList->m_sTxMsgDetails.m_bEnabled );
                }
            }

        }

    }
    *pResult = 0;    
}

/*******************************************************************************
  Function Name  : pomGetParentWindow
  Input(s)       : -
  Output         : CWnd * - Pointer to CTxMsgChildFrame
  Functionality  : This Function will return parent window pointer. That is
                   pointer to CTxMsgChildFrame. This will return NULL incase of
                   failure
  Member of      : CTxMsgListView
  Author(s)      : Raja N
  Date Created   : 25.4.2005
  Modifications  : 
*******************************************************************************/
CWnd * CTxMsgListView::pomGetParentWindow() const
{
    CWnd * pWnd = NULL;
    // Get Splitter window pointer
    pWnd = GetParent();
    // Get Tx Msg Child Window pointer from Splitter window pointer
    // At fourth Level
    // Splitter 3 -> Splitter 2 -> Splitter 1 -> Child Frame
    if( pWnd != NULL )
    {
        pWnd = pWnd->GetParent();
    }
    if( pWnd != NULL )
    {
        pWnd = pWnd->GetParent();
    }

    if( pWnd != NULL )
    {
        pWnd = pWnd->GetParent();
    }

    // Return Tx Msg Child window pointer or NULL incase of failure
    return pWnd;
}


/*******************************************************************************
  Function Name  : pomGetBlocksViewPointer
  Input(s)       : -
  Output         : CWnd * - Pointer to CTxMsgBlocksView or NULL incase of
                   failure
  Functionality  : This function will return CTxMsgBlocksView pointer. This will
                   get child window pointer to get view pointer.
  Member of      : CTxMsgListView
  Author(s)      : Raja N
  Date Created   : 25.4.2005
  Modifications  : 
*******************************************************************************/
CWnd * CTxMsgListView::pomGetBlocksViewPointer() const
{
    CWnd * pView = NULL;
    // Get Child Frame Pointer
    CWnd * pWnd = NULL;
    pWnd = pomGetParentWindow();
    // Get View Pointer
    if( pWnd != NULL )
    {
        pView = ((CTxMsgChildFrame *)pWnd)->pomGetTxMsgViewPointers( 
                                                        eTxMsgBlocksView );
    }
    // Return View pointer
    return pView;
}

/*******************************************************************************
  Function Name  : pomGetDetailsViewPointer
  Input(s)       : -
  Output         : CWnd * - Pointer to CTxMsgDetailsView or NULL incase of
                   failure
  Functionality  : This function will return CTxMsgDetailsView pointer. This
                   will get child window pointer to get view pointer.
  Member of      : CTxMsgListView
  Author(s)      : Raja N
  Date Created   : 25.4.2005
  Modifications  : 
*******************************************************************************/
CWnd * CTxMsgListView::pomGetDetailsViewPointer() const
{
    CWnd * pView = NULL;
    // Get Child Frame Pointer
    CWnd * pWnd = NULL;
    pWnd = pomGetParentWindow();
    // Get View Pointer
    if( pWnd != NULL )
    {
        pView = ((CTxMsgChildFrame *)pWnd)->pomGetTxMsgViewPointers(
                                                    eTxMsgMessageDetailsView );
    }
    // Return View pointer
    return pView;
}

/*******************************************************************************
  Function Name  : pomGetFunctionsViewPointer
  Input(s)       : -
  Output         : CWnd * - Pointer to CTxFunctionsView or NULL incase of
                   failure
  Functionality  : This function will return CTxFunctionsView pointer. This
                   will get child window pointer to get view pointer.
  Member of      : CTxMsgListView
  Author(s)      : Raja N
  Date Created   : 25.4.2005
  Modifications  : 
*******************************************************************************/
CWnd * CTxMsgListView::pomGetFunctionsViewPointer() const
{
    CWnd * pView = NULL;
    // Get Child Frame Pointer
    CWnd * pWnd = NULL;
    pWnd = pomGetParentWindow();
    // Get View Pointer
    if( pWnd != NULL )
    {
        pView = ((CTxMsgChildFrame *)pWnd)->pomGetTxMsgViewPointers( 
                                                        eTxMsgFunctionsView );
    }
    // Return View pointer
    return pView;
}

/******************************************************************************/
/*  Function Name    :  psGetMsgDetailPointer                                 */
/*  Input(s)         :  nIndex : Index of message in the list                 */
/*                      psCurrentMsgBlockList : current msg block pointer     */
/*  Output           :                                                        */
/*  Functionality    :  This function will return message list pointer for    */
/*                      node at index nIndex passed as parameter from the link*/
/*                      of message frame list from the current message block. */
/*                                                                            */
/*  Member of        :  CTxMsgListView                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
PSTXCANMSGLIST CTxMsgListView::psGetMsgDetailPointer(INT nIndex,
                                          SMSGBLOCKLIST* psCurrentMsgBlockList)
{
    PSTXCANMSGLIST psTxMsgList = NULL;
    if(nIndex != -1 && psCurrentMsgBlockList != NULL )
    {
	    INT nCurrentIndex = 0;
        psTxMsgList = psCurrentMsgBlockList->m_psTxCANMsgList;
        while(nIndex != nCurrentIndex)
        {
            if(psTxMsgList != NULL )
            {
                psTxMsgList = psTxMsgList->m_psNextMsgDetails;
            }
            nCurrentIndex++;
        }
    }
    return psTxMsgList;
}

/******************************************************************************/
/*  Function Name    :  OnSendSelectedMsg                                     */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by frame work when user wants */
/*                      to Transmit selected message                          */
/*                                                                            */
/*  Member of        :  CTxMsgListView                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */    
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
void CTxMsgListView::OnSendSelectedMsg() 
{
    PSTXSELMSGDATA psTxCanMsg = NULL;
    UINT unTotalSelection = m_omLctrMsgList.GetSelectedCount();

    CTxMsgBlocksView * pomBlocksView = NULL;
    pomBlocksView = ( CTxMsgBlocksView *)pomGetBlocksViewPointer();
    if( pomBlocksView != NULL )
    {
        // Get the total selected count.
        if(unTotalSelection > 0 )
        {
            // create the pointer and check if it has created successfully.
            psTxCanMsg  = new STXSELMSGDATA;
            if(psTxCanMsg != NULL )
            {
                // Fill the structure and create memory for array of message
                // frame to be transmitted.
                psTxCanMsg->m_unCount = unTotalSelection;
                psTxCanMsg->m_psTxMsg = new STCAN_MSG[unTotalSelection];
                if(psTxCanMsg->m_psTxMsg != NULL )
                {
                    // get the current message block
                    PSMSGBLOCKLIST psMsgBlock = NULL; 
                    psMsgBlock = pomBlocksView->psGetMsgBlockPointer(
                                        pomBlocksView->m_nSelectedMsgBlockIndex,
                                        pomBlocksView->m_psMsgBlockList );
                    if(psMsgBlock != NULL )
                    {
                        PSTXCANMSGLIST  psTxMsgList = NULL;
                        INT nCurrentIndex           = -1;
                        // Get the message which is selected for transmission
                        // Since selection may be random so get the selection
                        // index one by one and get the pointer for that message
                        // frame . After that copy it to the structure.
                        for(UINT i =0 ; i<unTotalSelection;i++)
                        {
                            nCurrentIndex = m_omLctrMsgList.GetNextItem(
                                                            nCurrentIndex,
                                                            LVNI_SELECTED );
                            // If failure to get the index then assign Invalid
                            // message ID to avoid any problem in transmission.
                            if(nCurrentIndex != -1 )
                            {
                                psTxMsgList = psGetMsgDetailPointer (
                                                                nCurrentIndex,
                                                                psMsgBlock );
                                if(psTxMsgList != NULL )
                                {
                                    memcpy( &(psTxCanMsg->m_psTxMsg[i]),
                                       &(psTxMsgList->m_sTxMsgDetails.m_sTxMsg),
                                       sizeof(STCAN_MSG) );
                                }
                                else
                                {
                                    psTxCanMsg->m_psTxMsg[i].m_unMsgID =
                                                static_cast <UINT> (-1);
                                }
                            }
                            else
                            {
                                psTxCanMsg->m_psTxMsg[i].m_unMsgID = 
                                                static_cast <UINT> (-1 );
                            }
                        }
                        g_bStopSelectedMsgTx = FALSE;
                        // Get handle of thread and assign it to pulic data
                        // member in app class. This will be used to terminate
                        // the thread.
                        CWinThread *pomThread = NULL ;
                        pomThread = 
                            AfxBeginThread( CTxMsgManager::s_unSendSelectedMsg,
                                            psTxCanMsg );
                        if(pomThread != NULL  && 
                            CTxMsgManager::s_sUtilThread.m_hThread
                                                    == NULL )
                        {
                           CTxMsgManager::s_sUtilThread.m_hThread
                                                    = pomThread->m_hThread;
                        }
                    }
                }
            }
        }
    }
}

/******************************************************************************/
/*  Function Name    :  OnDeleteSelectedMsg                                   */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    : This function will delete the selected message frame if*/ 
/*                      the user confirms the deletion action.                */
/*                                                                            */
/*  Member of        :  CTxMsgListView                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  30.01.2004, confirmation message changed.             */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  02.08.2004, Added code to enable add button and to    */
/*                      set the focus to the next message in the list         */
/******************************************************************************/
void CTxMsgListView::OnDeleteSelectedMsg() 
{
    CTxMsgBlocksView * pomBlocksView = NULL;
    pomBlocksView = ( CTxMsgBlocksView *)pomGetBlocksViewPointer();

    CTxMsgDetailsView *pomDetailsView = NULL;
    pomDetailsView = (CTxMsgDetailsView *)pomGetDetailsViewPointer();

    CTxFunctionsView * pomFunctionsView = NULL;
    pomFunctionsView = (CTxFunctionsView *)pomGetFunctionsViewPointer();

    if( pomBlocksView != NULL && pomDetailsView != NULL && 
                pomFunctionsView != NULL )
    {
        if ( AfxMessageBox( defDEL_SEL_MSG_FRAME, 
                                MB_YESNO|MB_ICONQUESTION) == IDYES)
        {
            if(m_nSelectedMsgIndex != -1)
            {
                if(pomBlocksView->m_nSelectedMsgBlockIndex != -1 )
                {
                    PSMSGBLOCKLIST psCurrentMsgBlock = NULL;
                    psCurrentMsgBlock = pomBlocksView->psGetMsgBlockPointer(
                                        pomBlocksView->m_nSelectedMsgBlockIndex,
                                        pomBlocksView->m_psMsgBlockList);

                    if(psCurrentMsgBlock != NULL )
                    {
                        BOOL bReturn = FALSE;
                        bReturn = bDeleteMsgFromBlock(psCurrentMsgBlock);
                        if(bReturn == TRUE )
                        {
                            pomFunctionsView->m_omButtonApply.
                                                        EnableWindow(TRUE);

                            pomDetailsView->vEnableAddButton( TRUE );
                            // Update Modified Flag
                            pomBlocksView->m_bModified = TRUE;
                        }
                        // Set the focus to list control back if it is not empty
                        // Shift the selection to item under fucus as selection
                        // deos not move up if an selected item is deleted.
                        m_nSelectedMsgIndex = 
                          m_omLctrMsgList.GetNextItem(-1,LVIS_FOCUSED);
                        if(m_nSelectedMsgIndex != -1 )
                        {
                            m_omLctrMsgList.SetItemState(
                                                       m_nSelectedMsgIndex,
                                                       LVIS_SELECTED,
                                                       LVIS_SELECTED);
                        }
                    }
                }
            }
        }
    }
}


/******************************************************************************/
/*  Function Name    :  bDeleteMsgFromBlock                                   */
/*  Input(s)         :  psMsgCurrentBlock : Current message block pointer     */
/*  Output           :  TRUE  or FALSE                                        */
/*  Functionality    :  This function will delete messages selected by user   */
/*                      from currently selected message block. Also the node  */
/*                      for link list will be removed and the list will be    */
/*                      updated.                                              */
/*                                                                            */
/*  Member of        :  CTxMsgListView                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
BOOL CTxMsgListView::bDeleteMsgFromBlock(SMSGBLOCKLIST* psMsgCurrentBlock)
{
    BOOL bReturn = FALSE;
    // Check for current message block pointer to be valid.
    if(psMsgCurrentBlock != NULL )
    {
        PSTXCANMSGLIST psDelTxCANMsgList = NULL;
        PSTXCANMSGLIST psTxCANMsgList = NULL;
        // Get the total selected items in message details list control.
        UINT  unSelectedCount = m_omLctrMsgList.GetSelectedCount();
        // If there is selection then delete it one by one.
        if (unSelectedCount > 0)
        {
            for (UINT i=0;i < unSelectedCount;i++)
            {
                // Get the first selection from the begining. Search for 
                // selection is always from start to ensure that selected
                // index is matching with the nodes in the list.
                int nItem = m_omLctrMsgList.GetNextItem(-1, LVNI_SELECTED);
                if(nItem != -1)
                {
                    // Get the current pointer
                    psDelTxCANMsgList = 
                        psGetMsgDetailPointer(nItem,psMsgCurrentBlock);
                    // Get the previous pointer.
                    psTxCANMsgList    = 
                        psGetMsgDetailPointer(nItem-1,psMsgCurrentBlock);
                    // If it is valid the next element in both node is updated
                    // so that the current selected node is removed from list.
                    // If both the pointers are not null i.e. the node is 
                    // in between the list.
                    //  If psTxCANMsgList is null then the node is at the start
                    // and if the both are null it is invalid pointers.
                    if(psDelTxCANMsgList != NULL && psTxCANMsgList != NULL )
                    {
                        psTxCANMsgList->m_psNextMsgDetails = 
                            psDelTxCANMsgList->m_psNextMsgDetails;
                        bReturn = TRUE;
                    }
                    else if(psDelTxCANMsgList != NULL && 
                            psDelTxCANMsgList ==
                                    psMsgCurrentBlock->m_psTxCANMsgList )
                    {
                        psMsgCurrentBlock->m_psTxCANMsgList =
                                    psDelTxCANMsgList->m_psNextMsgDetails;
                        bReturn = TRUE;
                    }
                    // if node  it is successfully removed then delete the 
                    // memory for that node.
                    if(bReturn == TRUE)
                    {
                        delete psDelTxCANMsgList;
                        psDelTxCANMsgList = NULL;
                        m_omLctrMsgList.DeleteItem(nItem);
                        //Decrement Message Count
                        psMsgCurrentBlock->m_unMsgCount--;
                        //Disable Delete All if the list is empty
                        UINT unCount =  m_omLctrMsgList.GetItemCount();
                        if( unCount <= 0 )
                        {
                            m_omButtonDeleteAllMsg.EnableWindow(FALSE);
                        }
                    }
                }
            }// for
        }
    }
    if(bReturn == TRUE )
    {
        m_nSelectedMsgIndex = -1;
    }
    return bReturn;
}

/******************************************************************************/
/*  Function Name    :  OnDeleteAllMsg                                        */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function will be called when user selects to     */
/*                      delete all message using menu or button.              */
/*                                                                            */
/*  Member of        :  CTxMsgListView                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modification on  :  30.01.2004, confirmation message changed.             */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  02.08.2004, Added code to update Add button status    */
/*  Modification By  :  Anish                                                 */
/*  Modification on  :  09.01.2007, Added code to disable send button status  */
/******************************************************************************/
void CTxMsgListView::OnDeleteAllMsg() 
{
    CTxMsgBlocksView * pomBlocksView = NULL;
    pomBlocksView = ( CTxMsgBlocksView *)pomGetBlocksViewPointer();

    CTxMsgDetailsView *pomDetailsView = NULL;
    pomDetailsView = (CTxMsgDetailsView *)pomGetDetailsViewPointer();

    CTxFunctionsView * pomFunctionsView = NULL;
    pomFunctionsView = (CTxFunctionsView *)pomGetFunctionsViewPointer();

    if( pomBlocksView != NULL && pomDetailsView != NULL && 
                pomFunctionsView != NULL )
    {
        // give a warning message before deleting it.
        if ( AfxMessageBox( defDEL_ALL_MSG_FRAME, 
        MB_YESNO|MB_ICONQUESTION) == IDYES)
        {
            // check for valid message block selection index.
            if( pomBlocksView->m_nSelectedMsgBlockIndex != -1)
            {
                BOOL bDeleted = FALSE;
                PSMSGBLOCKLIST psMsgCurrentBlock = NULL;
                // Get the current message block pointer.
                psMsgCurrentBlock = pomBlocksView->psGetMsgBlockPointer(
                        pomBlocksView->m_nSelectedMsgBlockIndex,
                        pomBlocksView->m_psMsgBlockList );
                if(psMsgCurrentBlock != NULL )
                {
                    // Call function to delete all message and clear the list
                    // control if the delete is successfull.
                    bDeleted = bDeleteAllMsgFromBlock(psMsgCurrentBlock);
                    if(bDeleted == TRUE )
                    {
                        m_omLctrMsgList.DeleteAllItems();
                        //Disable Delete All button
                        m_omButtonDeleteAllMsg.EnableWindow(FALSE);
                        m_omButtonDeleteSelMsg.EnableWindow(FALSE);
                        
                        pomFunctionsView->m_omButtonApply.EnableWindow(TRUE);
                        // Update Add button status
                        pomDetailsView->vEnableAddButton( TRUE );
                        // Update Modified Flag
                        pomBlocksView->m_bModified = TRUE;
                    }
					m_omButtonSendMsg.EnableWindow(FALSE);
                }
            }
        }
    }
}

/******************************************************************************/
/*  Function Name    :  bDeleteAllMsgFromBlock                                */
/*  Input(s)         :  psMsgCurrentBlock : pointer of current block          */
/*  Output           :  TRUE or FALSE                                         */
/*  Functionality    :  This function will delete all memory allocated in the */
/*                      current message block and return TRUE on success.     */
/*                                                                            */
/*  Member of        :  CTxMsgListView                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */    
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  11.05.2004 Added init of index variable to -1 to      */
/*                      indicate invalid selection after deleting all items   */
/******************************************************************************/
BOOL CTxMsgListView::bDeleteAllMsgFromBlock(SMSGBLOCKLIST* psMsgCurrentBlock)
{
    BOOL bReturn = TRUE;
    if(psMsgCurrentBlock != NULL )
    {
        PSTXCANMSGLIST psTxCANMsgList = NULL;
        PSTXCANMSGLIST psTxCANMsgListDel = NULL;
        psTxCANMsgList = psMsgCurrentBlock->m_psTxCANMsgList;
        while(psTxCANMsgList != NULL )
        {
            psTxCANMsgListDel = psTxCANMsgList;
            psTxCANMsgList    = psTxCANMsgList->m_psNextMsgDetails;
            delete psTxCANMsgListDel;
            psTxCANMsgListDel = NULL;
        }
        psMsgCurrentBlock->m_psTxCANMsgList = NULL;
        psMsgCurrentBlock->m_unMsgCount = 0;
        // Set index to Invalid
        m_nSelectedMsgIndex = -1;
    }
    else
    {
        bReturn = FALSE;
    }    
    return bReturn;
}


/******************************************************************************/
/*  Function Name    :  vUpdateMsgListDisplay                                 */
/*  Input(s)         :  sTXCANMSGDETAILS sMsgDetail: message details struct   */
/*                      INT nCurrentIndex : Selected Index or -1 (new entry ) */
/*  Output           :                                                        */
/*  Functionality    :  This function will update the list cotrol of message  */
/*                      list at the selected item or as new item.             */
/*                                                                            */
/*  Member of        :  CTxMsgListView                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  22.07.2004, Updated list with DB message name         */
/******************************************************************************/
VOID CTxMsgListView::vUpdateMsgListDisplay(sTXCANMSGDETAILS sMsgDetail,
                                             INT nCurrentIndex)
{

    CString omStrMsgID( STR_EMPTY );
    CString omStrMsgData( STR_EMPTY );
    CString omStrMsgType( STR_EMPTY );
    CString omStrDLC( STR_EMPTY );
    CString omStrChannel( STR_EMPTY );
    INT nIndex           = -1;
    UINT unImageID       = 0;
    CString omStrFormat( STR_EMPTY );
    CString omStrMsgName( STR_EMPTY );;

    // Set Edit flag to TRUE
    m_bInitDlg = TRUE;

    // format the message data length
    // Get the base and accordingly change the format for Format function of
    // CString class.
    if( TRUE == CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_HEX) )
    {
        omStrMsgID.Format(defFORMAT_MSGID_HEX,sMsgDetail.m_sTxMsg.m_unMsgID);
        omStrFormat = defFORMAT_DATA_HEX;
    }
    else
    {
        omStrMsgID.Format( defFORMAT_MSGID_DECIMAL,
                           sMsgDetail.m_sTxMsg.m_unMsgID );
        omStrFormat = defFORMAT_DATA_DECIMAL;
    }    // Message Details View
    CTxMsgDetailsView *pomDetailsView = NULL;
     CMsgSignal *pDBptr = NULL;
    pomDetailsView = (CTxMsgDetailsView *)pomGetDetailsViewPointer();
    if (NULL != pomDetailsView)
    {
        pDBptr =  pomDetailsView->m_pouDBPtr;
    }
    // See if the msg ID is in the database
    // If yes load appropriate image
    if (NULL != pDBptr)
    {
        omStrMsgName = 
            pDBptr->omStrGetMessageNameFromMsgCode(sMsgDetail.m_sTxMsg.m_unMsgID);
    }
    if ( omStrMsgName.IsEmpty() == FALSE )
    {
        unImageID = 1;// Database Image
        // Add message Name with the entry
        omStrMsgID += " [" + 
                      pDBptr->omStrGetMessageNameFromMsgCode(sMsgDetail.m_sTxMsg.m_unMsgID )
                        + "]";
    }
    else
    {
        unImageID = 2;// Non-Database image
    }
    // Format channel ID
    omStrChannel.Format("%d", sMsgDetail.m_sTxMsg.m_ucChannel );

    // Format Message Type
    omStrFormat.Insert(omStrFormat.GetLength(),defEMPTY_CHAR);

    CString omStrTemp = _T("");
    for(INT i=0; i<sMsgDetail.m_sTxMsg.m_ucDataLen;i++)
    {
       omStrTemp.Format(omStrFormat,sMsgDetail.m_sTxMsg.m_ucData[i]);
       omStrMsgData +=  omStrTemp;
    }
    // Format the Message type
    if(sMsgDetail.m_sTxMsg.m_ucEXTENDED == TRUE)
    {
        omStrMsgType = defMSGID_EXTENDED;
    }
    else
    {
        omStrMsgType = defMSGID_STD; 
    }
    if(sMsgDetail.m_sTxMsg.m_ucRTR == TRUE)
    {
        omStrMsgType += defMSGID_RTR;
    }
    // Format the DLC
    omStrDLC.Format("%d",sMsgDetail.m_sTxMsg.m_ucDataLen);
    // Get the current count if this is new items and insert it 
    // as new item. Otherwise set the current item text.
    if(nCurrentIndex == -1)
    {
        INT nCount = m_omLctrMsgList.GetItemCount();
        if(nCount != -1 )
        {
            nIndex = m_omLctrMsgList.
                                    InsertItem(nCount,omStrMsgID,unImageID);
        }
    }
    else
    {
        LVITEM sItem;
        sItem.mask      = LVIF_IMAGE;
        sItem.iItem     = nCurrentIndex;
        sItem.iSubItem  = defMAIN_ITEM;
        sItem.iImage    = unImageID;
        m_omLctrMsgList.
                SetItemText(nCurrentIndex,defMAIN_ITEM,omStrMsgID);
        m_omLctrMsgList.SetItem(&sItem);
        nIndex = nCurrentIndex;
    }
    // Set the list control item with string formatted for each column.
    if(nIndex != -1 )
    {
        // Update Channel ID
        m_omLctrMsgList.SetItemText( nIndex,
                                     defSUBITEM_MSGDETAILS_CHANNEL_ID,
                                     omStrChannel );
        // Update Message Type
        m_omLctrMsgList.SetItemText( nIndex,
                                    defSUBITEM_MSGDETAILS_TYPE,
                                    omStrMsgType );
        // Update Message Length
        m_omLctrMsgList.SetItemText( nIndex,
                                    defSUBITEM_MSGDETAILS_DLC,
                                    omStrDLC );
        // Update Data Bytes with Dirty Flag
        if( sMsgDetail.m_bIsMsgDirty == TRUE && 
            unImageID == 1 &&
            sMsgDetail.m_sTxMsg.m_ucDataLen > 0 )
        {
            omStrMsgData +=defASSETRIC;
        }
        m_omLctrMsgList.SetItemText( nIndex,
                                     defSUBITEM_MSGDETAILS_DATA,
                                     omStrMsgData);
        // Update Check box status
        m_omLctrMsgList.SetCheck( nIndex, sMsgDetail.m_bEnabled );
    }

    // Set Edit flag to FALSE
    m_bInitDlg = FALSE;
}

/******************************************************************************/
/*  Function Name    :  OnRightClickMsgDetails                                */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function will call bDisplayPopMenu.              */
/*                                                                            */
/*  Member of        :  CTxMsgListView                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
void CTxMsgListView::OnRightClickMsgDetails(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
    bDisplayPopMenu(m_omLctrMsgList,IDM_SEND_OPRNS);
    *pResult = 0;
}

/******************************************************************************/
/*  Function Name    :  bDisplayPopMenu                                       */
/*  Input(s)         :                                                        */
/*  Output           :  TRUE or FALSE                                         */
/*  Functionality    :  This function will display pop-up menus if selection  */
/*                      is valid and chooses the appropriate menu for display */
/*                                                                            */
/*  Member of        :  CTxMsgListView                                        */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  06.08.2004, Added checks to disable Add and Delete All*/
/*                      Menu Items                                            */
/******************************************************************************/
BOOL CTxMsgListView::bDisplayPopMenu(CListCtrl& omList,UINT nIDResource )
{
    BOOL bReturn = FALSE;
    // Get selected item's index
    INT nIndex = 
        omList.GetNextItem( -1, LVNI_SELECTED );
    // create menu
    CMenu* pMainMenu = new CMenu;
    // Assert
    if ( pMainMenu != NULL )
    {
        // Load menu
        if ( pMainMenu->LoadMenu( nIDResource ))
        {
            // Get submenu
            CMenu* pSubMenu = pMainMenu->GetSubMenu( 0 );
            // Assert
            if ( pSubMenu != NULL )
            {
                POINT point;
                // Get cursor position wrt screen co-ord
                GetCursorPos(&point);
                // If it is from Message List
                // Disable Delete All menu items if the the item count is 0
                if( nIDResource == IDM_SEND_OPRNS )
                {
                    UINT unCount = m_omLctrMsgList.GetItemCount();
                    if ( unCount <= 0)
                    {
                        pSubMenu->EnableMenuItem( IDM_DELETE_ALL_MSG,
                                    MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
                    }
                }
                
                if(nIndex == -1 )
                {
                    // if selection is not valid, disble the menu.
                    pSubMenu->EnableMenuItem(IDM_SEND_SEL_MSG,
                                       MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
                    pSubMenu->EnableMenuItem(IDM_DELETE_SEL_MSG,
                                       MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
                }
                else
                {
                    // If selection is valid and transmission is on. disable
                    // Delete and delete all menu.
                    if(TRUE == CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG))
                    {
                        pSubMenu->EnableMenuItem(IDM_DELETE_SEL_MSG,
                                        MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
                        pSubMenu->EnableMenuItem(IDM_DELETE_ALL_MSG,
                                        MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
                    }
                    // If tool is not connected disable Send menu.
                    if(FALSE == CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_CONNECTED))
                    {
                        pSubMenu->EnableMenuItem(IDM_SEND_SEL_MSG,
                                        MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
                    }
                }
                // Display menu
                pSubMenu->TrackPopupMenu( 
                    TPM_LEFTALIGN |TPM_RIGHTBUTTON,
                    point.x,
                    point.y,
                    this,
                    NULL);
                // Clean up
                pMainMenu->Detach();
                pMainMenu->DestroyMenu();
                delete pMainMenu;
                pMainMenu = NULL;
                bReturn = TRUE;
            }
        }
    }
    return bReturn;
}