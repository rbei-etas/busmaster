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
 * \file      TxFunctionsView.cpp
 * \brief     Implementation file for CTxFunctionsView class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CTxFunctionsView class
 */

#include "TxWindow_stdafx.h"             // For Standard Includes
#include "SignalMatrix.h"       // For Signal Matrix Class Definition
#include "Utility/ComboItem.h"          // For Custom Combobox Implementation
#include "Utility/EditItem.h"           // For Custom Editbox Implementation
#include "Utility/RadixEdit.h"          // For the RAdix Edit control definition
#include "Utility/NumSpinCtrl.h"        // For the custom spin control
#include "Utility/NumEdit.h"            // For Custom Numeric Edit control Impl
#include "Utility/FFListctrl.h"         // For Flicker Free List class definition
#include "FlexListCtrl.h"       // For editable list control implementation
#include "TxFunctionsView.h"    // For Tx Function View class declaration
#include "TxMsgChildFrame.h"    // For Parent Window class declaration

IMPLEMENT_DYNCREATE(CTxFunctionsView, CFormView)

/*******************************************************************************
  Function Name  : CTxFunctionsView
  Description    : Standard default constructor
  Member of      : CTxFunctionsView
  Functionality  : This will initialise local variables
  Author(s)      : Raja N
  Date Created   : 26.4.2005
  Modifications  :
*******************************************************************************/
CTxFunctionsView::CTxFunctionsView()
    : CFormView(CTxFunctionsView::IDD)
{
    //{{AFX_DATA_INIT(CTxFunctionsView)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

/*******************************************************************************
  Function Name  : ~CTxFunctionsView
  Description    : Standard Destructor
  Member of      : CTxFunctionsView
  Functionality  : -
  Author(s)      : Raja N
  Date Created   : 26.4.2005
  Modifications  :
*******************************************************************************/
CTxFunctionsView::~CTxFunctionsView()
{
}

/*******************************************************************************
  Function Name  : DoDataExchange
  Input(s)       : pDX - Pointer to DDX object
  Output         : -
  Functionality  : This function will map DDX object with UI control for data
                   exchange
  Member of      : CTxFunctionsView
  Author(s)      : Raja N
  Date Created   : 26.4.2005
  Modifications  : 
*******************************************************************************/
void CTxFunctionsView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTxFunctionsView)
    DDX_Control(pDX, IDC_BTN_UPDATE, m_omButtonApply);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTxFunctionsView, CFormView)
    //{{AFX_MSG_MAP(CTxFunctionsView)
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_BTN_UPDATE, OnButtonApply)
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


#ifdef _DEBUG
void CTxFunctionsView::AssertValid() const
{
    CFormView::AssertValid();
}

void CTxFunctionsView::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}
#endif //_DEBUG

/*******************************************************************************
  Function Name  : OnSize
  Input(s)       : nType - Sizing mode
                   cx - New X value
                   cy - New Y Value
  Output         : -
  Functionality  : This function will be called by the framework during sizing
                   of the window. This function will set the buttons position at
                   the center of the dialog
  Member of      : CTxFunctionsView
  Author(s)      : Raja N
  Date Created   : 26.4.2005
  Modifications  : 
*******************************************************************************/
void CTxFunctionsView::OnSize(UINT nType, int cx, int cy) 
{
    CFormView::OnSize(nType, cx, cy);
    const double dOffset = 0.75;
    // Get Update Window pointer
    CWnd *pWnd = GetDlgItem( IDC_BTN_UPDATE );
    // If valid
    if( pWnd != NULL )
    {
        CRect omRect;
        // Move the button to center postion
        pWnd->CenterWindow();
        // Now move away from the center
        pWnd->GetWindowRect( &omRect );
        ScreenToClient(&omRect);
        double nWidth = omRect.Width();
        // Shift 75 % of width of the button away from the center
        nWidth *= dOffset;
        omRect.left -= (LONG)nWidth;
        omRect.right -= (LONG)nWidth;
        // Move the window to the new location
        pWnd->MoveWindow(&omRect);
    }

    // Get Close Window pointer
    pWnd = GetDlgItem( IDC_BTN_CLOSE );
    // If valid
    if( pWnd != NULL )
    {
        CRect omRect;
        // Move the button to center postion
        pWnd->CenterWindow();
        // Now move away from the center
        pWnd->GetWindowRect( &omRect );
        ScreenToClient(&omRect);
        double nWidth = omRect.Width();
        // Shift 75 % of width of the button away from the center
        nWidth *= dOffset;
        omRect.left += (LONG)nWidth;
        omRect.right += (LONG)nWidth;
        // Move the window to the new location
        pWnd->MoveWindow(&omRect);
    }
}

/*******************************************************************************
  Function Name  : OnInitialUpdate
  Input(s)       : -
  Output         : -
  Functionality  : This function will be called by the framework during initial
                   show of this view. This function will register iteself in to
                   parent window class so that other views shall access it.
  Member of      : CTxFunctionsView
  Author(s)      : Raja N
  Date Created   : 26.4.2005
  Modifications  : 
*******************************************************************************/
void CTxFunctionsView::OnInitialUpdate() 
{
    CFormView::OnInitialUpdate();
    // Update View pointer in to the child frame
    // Get Child Window Pointer
    CWnd * pWnd = NULL;
    // Get Parent window pointer
    pWnd = pomGetParentWindow();
    // Update view pointer
    if( pWnd != NULL )
    {
        // Register this view pointer
        ((CTxMsgChildFrame *)pWnd)->vSetTxMsgViewPointers( eTxMsgFunctionsView,
                                                           this );
    }
    // Disable Update Button
    m_omButtonApply.EnableWindow( FALSE );
}


/******************************************************************************/
/*  Function Name    :  OnButtonApply                                         */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called when user press Apply button  */
/*                      The flags will be set and global list will be updated */
/*                                                                            */
/*  Member of        :  CTxFunctionsView                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  22.07.2004, Removed the update code and and made it as*/
/*                      a seprate function.                                   */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  05.08.2004, Added code to check the changes during Tx */
/*                      is ON. This will prevent apply and give an option to  */
/*                      the user to reload the data from configuration        */
/******************************************************************************/
void CTxFunctionsView::OnButtonApply() 
{
    // Apply changed to global Tx Block
    // User is trying to apply late changes
    // Prevent this as some message/block could be deleted after
    // starting the transmission.
    
    // Get Other view pointers
    CTxMsgBlocksView * pomBlockView = NULL;
    pomBlockView = (CTxMsgBlocksView *)pomGetBlocksViewPointer();
    if( pomBlockView != NULL )
    {
        if( (pomBlockView->m_bModified == TRUE) && (TRUE == CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG)) )
        {
            if(AfxMessageBox( defSTR_RELOAD_CONFIRMATION,
                          MB_YESNO | MB_DEFBUTTON2) == IDYES )
            {
                // Reload the data from Configuration
                vReloadData();
            }
        }
        else
        {
            vApplyChanges();
            // Disable the button
            m_omButtonApply.EnableWindow(FALSE);
            // Update modified flag
            pomBlockView->m_bModified = FALSE;
        }
    }
}

/*******************************************************************************
 Function Name    : vApplyChanges
 Input(s)         :
 Output           :
 Functionality    : This function is used to apply changes in the global shared
                    memory
 Member of        : CTxFunctionsView
 Friend of        :  -
 Author(s)        : Raja N
 Date Created     : 19.07.2004
 Modification By  :
 Modification on  :
*******************************************************************************/
void CTxFunctionsView::vApplyChanges()
{
    // Get Other view pointers
    CTxMsgBlocksView * pomBlockView = NULL;
    CTxMsgListView * pomListView = NULL;
    pomBlockView = (CTxMsgBlocksView *)pomGetBlocksViewPointer();
    pomListView = (CTxMsgListView * )pomGetListViewPointer();

    if( pomBlockView != NULL && pomListView != NULL )
    {
        PSMSGBLOCKLIST psMsgBlock = NULL;
        psMsgBlock = pomBlockView->psGetMsgBlockPointer(
                                    pomBlockView->m_nSelectedMsgBlockIndex,
                                    pomBlockView->m_psMsgBlockList );
        // Update the last selected message block details in case user has 
        // modified it.
        if(psMsgBlock != NULL )
        {
            pomBlockView->vUpdateMsgBlockDetials(psMsgBlock);
        }

        // Set the value in configuration class.
        CTxWndDataStore::ouGetTxWndDataStoreObj().bSetTxData( TX_MSG_BLOCK_COUNT,
                         (void*)&(pomBlockView->m_unMsgBlockCount) );
        // Data is set only if messagae block count is more then zero.
        if( pomBlockView->m_unMsgBlockCount > 0 )
        {
            CTxWndDataStore::ouGetTxWndDataStoreObj().bSetTxData( TX_SEND_MULTI_MSGS,
                            (void*)(pomBlockView->m_psMsgBlockList) );
            // Allocate memory for globle list
            BOOL bAllocateMemory =
                CTxMsgManager::s_podGetTxMsgManager()->bAllocateMemoryForGlobalTxList();
            // Update Message List
            if(bAllocateMemory == TRUE )
            {
                CTxMsgManager::s_podGetTxMsgManager()->vAssignMsgBlockList();
            }
        }
        //Save Window position
        CTxMsgChildFrame* pTxWnd = (CTxMsgChildFrame*)pomGetParentWindow();
        // Update view pointer
        if( pTxWnd != NULL )
        {
            // Register this view pointer
            pTxWnd->vUpdateWndCo_Ords();
            
        }
    }
}

/*******************************************************************************
  Function Name  : pomGetParentWindow
  Input(s)       : -
  Output         : CWnd * - Pointer to CTxMsgChildFrame
  Functionality  : This Function will return parent window pointer. That is
                   pointer to CTxMsgChildFrame. This will return NULL incase of
                   failure
  Member of      : CTxFunctionsView
  Author(s)      : Raja N
  Date Created   : 25.4.2005
  Modifications  : 
*******************************************************************************/
CWnd * CTxFunctionsView::pomGetParentWindow() const
{
    CWnd * pWnd = NULL;
    // Get Splitter window pointer
    pWnd = GetParent();
    // Get Tx Msg Child Window pointer from Splitter window pointer
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
  Member of      : CTxFunctionsView
  Author(s)      : Raja N
  Date Created   : 25.4.2005
  Modifications  : 
*******************************************************************************/
CWnd * CTxFunctionsView::pomGetBlocksViewPointer() const
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
    // Return view pointer
    return pView;
}

/*******************************************************************************
  Function Name  : pomGetListViewPointer
  Input(s)       : -
  Output         : CWnd * - Pointer to CTxMsgListView or NULL incase of
                   failure
  Functionality  : This function will return CTxMsgListView pointer. This will
                   get child window pointer to get view pointer.
  Member of      : CTxFunctionsView
  Author(s)      : Raja N
  Date Created   : 25.4.2005
  Modifications  : 
*******************************************************************************/
CWnd * CTxFunctionsView::pomGetListViewPointer() const
{
    CWnd * pView = NULL;
    // Get Child Frame Pointer
    CWnd * pWnd = NULL;
    pWnd = pomGetParentWindow();
    // Get View Pointer
    if( pWnd != NULL )
    {
        pView = ((CTxMsgChildFrame *)pWnd)->pomGetTxMsgViewPointers( 
                                                        eTxMsgMessageListView );
    }
    // Return view pointer
    return pView;
}

/*******************************************************************************
  Function Name  : pomGetDetailsViewPointer
  Input(s)       : -
  Output         : CWnd * - Pointer to CTxMsgDetailsView or NULL incase of
                   failure
  Functionality  : This function will return CTxMsgDetailsView pointer. This
                   will get child window pointer to get view pointer.
  Member of      : CTxFunctionsView
  Author(s)      : Raja N
  Date Created   : 25.4.2005
  Modifications  : 
*******************************************************************************/
CWnd * CTxFunctionsView::pomGetDetailsViewPointer() const
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
    // Return view pointer
    return pView;
}

/*******************************************************************************
 Function Name    : vReloadData
 Input(s)         : 
 Output           : 
 Functionality    : This function will get the Tx block data from configuration
                    and will update the UI. This function will be called if the
                    UI block information is not saved before transmission and
                    the user wants to apply the changes with Tx on which would
                    cause access violation in case of delete. To avoid that the
                    data from configuration is reloaded.
 Member of        : CTxFunctionsView
 Friend of        :     -
 Author(s)        : Raja N
 Date Created     : 05.08.2004
 Modification     : Anish,02.02.07
					Bug fixed in display of blocks value if there are only key 
					triggred blocks after a time and key triggere block
/******************************************************************************/
void CTxFunctionsView::vReloadData()
{
    // Get Other View Ponters
    CTxMsgBlocksView * pomBlockView = NULL;
    CTxMsgListView * pomlistView = NULL;
    CTxMsgDetailsView * pomDetailsView = NULL;
    // Fetch pointers from Tx Child Frame
    pomBlockView = ( CTxMsgBlocksView * )pomGetBlocksViewPointer();
    pomlistView = ( CTxMsgListView * )pomGetListViewPointer();
    pomDetailsView = ( CTxMsgDetailsView * )pomGetDetailsViewPointer();

    if( pomBlockView != NULL && pomlistView != NULL && pomDetailsView != NULL )
    {
        // Clear all data
        pomBlockView->m_omLctrMsgBlockName.DeleteAllItems();
        pomlistView->m_omLctrMsgList.DeleteAllItems();
        pomDetailsView->m_omLctrSigList.DeleteAllItems();
        
        // Set this flag to avoid processing item change messages during init
        pomBlockView->m_bInitDlg = TRUE;
        pomlistView->m_bInitDlg = TRUE;
        
        //Clear the existing memory
        CTxWndDataStore::ouGetTxWndDataStoreObj().vReleaseMultiMsgInfo( pomBlockView->m_psMsgBlockList );
        pomBlockView->m_psMsgBlockList = NULL;
        /*vRelease( TX_SEND_MULTI_MSGS,
                         (void**)&(pomBlockView->m_psMsgBlockList) );*/
        // Get the message block count. No memory allocation is required.
        UINT* punCount = &( pomBlockView->m_unMsgBlockCount );
        CTxWndDataStore::ouGetTxWndDataStoreObj().bGetTxData(TX_MSG_BLOCK_COUNT, (void**)&punCount);
        // Make a local copy of the data structure
        // The local copy will be used to manipulate message frame details
        // Memory allocation will be done in the CConfigDetails class.
        // Get the pointer only if there m_unMsgBlockCount>0
        if( pomBlockView->m_unMsgBlockCount > 0 )
        {
            CTxWndDataStore::ouGetTxWndDataStoreObj().bGetTxData( TX_SEND_MULTI_MSGS,
                             (void**)&(pomBlockView->m_psMsgBlockList) );
        }
        
        if( pomBlockView->m_unMsgBlockCount == 0 )
        {
            pomBlockView->vEnableControls(FALSE);
            /*pomBlockView->m_omButtonTxAllFrame.EnableWindow(FALSE);*/
            pomBlockView->m_omComboAllMsgs.EnableWindow(FALSE);

            pomlistView->m_omButtonDeleteAllMsg.EnableWindow(FALSE);
            pomlistView->m_omButtonDeleteSelMsg.EnableWindow(FALSE);
            
        }
        else
        {
            pomlistView->m_omButtonDeleteAllMsg.
                EnableWindow(!CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG) );
        }
        
        PSMSGBLOCKLIST psCurrentMsgBlock = NULL;
        CString omStr = STR_EMPTY;
        psCurrentMsgBlock = pomBlockView->m_psMsgBlockList;
        
        // Disable AllTxMessage check box
        // This will be updated in the function vDisplayMsgBlockDetails
        // Disabling it here will avoid enabling the control when no blocks or
        // messages are present
        pomBlockView->m_omComboAllMsgs.EnableWindow(FALSE);
        /*pomBlockView->m_omButtonTxAllFrame.EnableWindow( FALSE );*/
        
        for(UINT i = 0; i< pomBlockView->m_unMsgBlockCount;i++)
        {
			omStr = STR_EMPTY;
            if(psCurrentMsgBlock != NULL )
            {
                pomBlockView->m_omLctrMsgBlockName.InsertItem( i,
                                          psCurrentMsgBlock->m_acStrBlockName);
                if(psCurrentMsgBlock->m_bType == TRUE)
                {
                    pomBlockView->m_omLctrMsgBlockName.SetItemText( i,
                                            defSUBITEM_MSGBLOCK_TYPE,
                                            defMSG_CYCLIC );
                }
                else
                {
                    pomBlockView->m_omLctrMsgBlockName.SetItemText( i,
                                            defSUBITEM_MSGBLOCK_TYPE,
                                            defMSG_MONOSHOT );
                }
                
                // Use Macros to find the type
                // Timer?
                if( IS_TIME_TRIGGERED(psCurrentMsgBlock->m_ucTrigger) )
                {
                    omStr.Format( defSTR_TIMER_VAL_FMT_SPECIFIER,
                              psCurrentMsgBlock->m_unTimeInterval );
                    omStr += defMESSAGE_TRIG_MS;
                    
                    if( IS_KEY_TRIGGERED(psCurrentMsgBlock->m_ucTrigger) )
                    {
                        omStr += defSTR_MSG_BLOCK_TRIGGER_SEPRATOR;
                    }
                }
                
                // Key ?
                if( IS_KEY_TRIGGERED(psCurrentMsgBlock->m_ucTrigger) )
                {
                    CString omStrKeyVal;
                    omStrKeyVal.Format( defSTR_KEY_VAL_FMT_SPECIFIER,
                                        psCurrentMsgBlock->m_ucKeyValue );
                    omStr += omStrKeyVal;
                }
                pomBlockView->m_omLctrMsgBlockName.SetItemText( i,
                                                defSUBITEM_MSGBLOCK_TRG_VAL,
                                                omStr );
                pomBlockView->m_omLctrMsgBlockName.SetCheck( i,
                                        psCurrentMsgBlock->m_bActive );
                psCurrentMsgBlock = psCurrentMsgBlock->m_psNextMsgBlocksList;
            }
        }
        // Clear Init Flag
        pomBlockView->m_bInitDlg = FALSE;
        pomlistView->m_bInitDlg = FALSE;

        pomBlockView->m_omLctrMsgBlockName.SetItemState(
                                       pomBlockView->m_nSelectedMsgBlockIndex,
                                       LVIS_SELECTED|LVIS_FOCUSED,
                                       LVIS_SELECTED|LVIS_FOCUSED );
        
        // Clear the Modified Flag
        pomBlockView->m_bModified = FALSE;

        // Disable Update Button
        m_omButtonApply.EnableWindow( FALSE );
        
    }
}

/*******************************************************************************
  Function Name  : OnBtnClose
  Input(s)       : -
  Output         : -
  Functionality  : This button handler will be called during Close button press.
                   This function will send WM_CLOSE message to close Tx Child
                   window.
  Member of      : CTxFunctionsView
  Author(s)      : Raja N
  Date Created   : 25.4.2005
  Modifications  : 
*******************************************************************************/
void CTxFunctionsView::OnBtnClose() 
{
    // Check for Modifications
    if( m_omButtonApply.IsWindowEnabled() == TRUE )
    {
        if( AfxMessageBox( defSTR_TX_SAVE_CONFIRMATION,
                           MB_YESNO | MB_ICONQUESTION ) == IDYES )
        {
            // Save Changes
            OnButtonApply();
        }
    }
    
    // Get Child Frame Pointer
    CWnd * pWnd = NULL;
    pWnd = pomGetParentWindow();
    // Post close message
    if( pWnd != NULL )
    {
        ((CTxMsgChildFrame *)pWnd)->PostMessage( WM_CLOSE );
    }
}