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
 * \file      TxMsgBlocksView.cpp
 * \brief     Implementation file for CTxMsgBlocksView class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CTxMsgBlocksView class
 */

#include "TxWindow_stdafx.h"             // For standard includes
#include "SignalMatrix.h"       // For Signal Matrix Class Definition
#include "Utility/ComboItem.h"          // For Custom Combobox Implementation
#include "Utility/EditItem.h"           // For Custom Editbox Implementation
#include "Utility/RadixEdit.h"          // For the RAdix Edit control definition
#include "Utility/NumSpinCtrl.h"        // For the custom spin control
#include "Utility/NumEdit.h"            // For Custom Numeric Edit control Impl
#include "Utility/FFListctrl.h"         // For Flicker Free List class definition
#include "FlexListCtrl.h"       // For editable list control implementation
#include "TxMsgBlocksView.h"    // For CTxMsgBlocksView class definition
#include "TxMsgListView.h"      // For Tx msg List view class declaration
#include "TxMsgChildFrame.h"    // For Parent window class declaration
#include "TxWindow_resource.h"

// Global App object declaration
//extern CCANMonitorApp theApp;

IMPLEMENT_DYNCREATE(CTxMsgBlocksView, CFormView)

/*******************************************************************************
  Function Name  : CTxMsgBlocksView
  Description    : Standard default constructor
  Member of      : CTxMsgBlocksView
  Functionality  : This will initialise local variables
  Author(s)      : Raja N
  Date Created   : 20.4.2005
  Modifications  :
*******************************************************************************/
CTxMsgBlocksView::CTxMsgBlocksView()
    : CFormView(CTxMsgBlocksView::IDD)
{
    //{{AFX_DATA_INIT(CTxMsgBlocksView)
    m_omStrMsgBlockName = STR_EMPTY;
    m_omStrTimeIntervalVal = STR_EMPTY;
    m_omStrKeyVal = STR_EMPTY;
    //m_bIsMonoshot = FALSE;
    //m_bTriggerType = FALSE;
    //}}AFX_DATA_INIT
    // Number of message blocks in the Tx List
    m_unMsgBlockCount = 0;
    // Tx Message List Pointer
    m_psMsgBlockList = NULL;
    // Selected Message Block index
    m_nSelectedMsgBlockIndex = -1;
    m_bMsgBlockDeleted = FALSE;

    m_bInitDlg = FALSE;
    m_nRBTNTriggerType = 0;
    m_bModified = FALSE;
}

/*******************************************************************************
  Function Name  : ~CTxMsgBlocksView
  Description    : Standard Destructor
  Member of      : CTxMsgBlocksView
  Functionality  : -
  Author(s)      : Raja N
  Date Created   : 20.4.2005
  Modifications  :
*******************************************************************************/
CTxMsgBlocksView::~CTxMsgBlocksView()
{
    // Delete the allocated memory for message block
    if(m_psMsgBlockList != NULL )
    {
        CTxWndDataStore::ouGetTxWndDataStoreObj().vReleaseMultiMsgInfo( m_psMsgBlockList );
        m_psMsgBlockList = NULL;
    }
}

/*******************************************************************************
  Function Name  : DoDataExchange
  Input(s)       : pDX - Pointer to DDX object
  Output         : -
  Functionality  : This function will map DDX object with UI control for data
                   exchange
  Member of      : CTxMsgBlocksView
  Author(s)      : Raja N
  Date Created   : 26.4.2005
  Modifications  : 
*******************************************************************************/
void CTxMsgBlocksView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTxMsgBlocksView)
    DDX_Text(pDX, IDC_EDIT_MSG_BLOCK_NAME, m_omStrMsgBlockName);
    DDX_Text(pDX, IDC_EDIT_TRG_TIME_VAL, m_omStrTimeIntervalVal);
    DDX_Control(pDX, IDC_LSTC_MSG_BLOCKS_NAME, m_omLctrMsgBlockName);
    DDX_Control(pDX, IDC_EDIT_MSG_BLOCK_NAME, m_omEditMsgBlockName);
    //DDX_Control(pDX, IDC_CHKB_TRIGGER_TYPE, m_omButtonTriggerType);
    DDX_Control(pDX, IDC_EDIT_TRG_TIME_VAL, m_omEditTrgTimeIntervalVal);
    DDX_Control(pDX, IDC_EDIT_TRG_KEY_VAL, m_omEditTrgKeyVal);
    DDX_Control(pDX, IDC_CHKB_ON_KEY_TRIGGER, m_omButtonKeyTrigger);
    DDX_Control(pDX, IDC_CHKB_ON_TIME_TRIGGER, m_omButtonTimeTrigger);
    DDX_Control(pDX, IDC_CBTN_ADD_MSG_BLOCK, m_omButtonAddMsgBlock);
    //DDX_Control(pDX, IDC_CHKB_TX_ALL_FRAME, m_omButtonTxAllFrame);
    DDX_Control(pDX, IDC_CBTN_DELETE_MSG_BLOCK, m_omButtonDeleteMsgBlock);
    //DDX_Check(pDX, IDC_CHKB_TX_ALL_FRAME, m_bTXAllFrame);
    //DDX_Check(pDX, IDC_CHK_MONO, m_bIsMonoshot);
    //DDX_Check(pDX, IDC_CHKB_TRIGGER_TYPE, m_bTriggerType);
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_COMBO_MSGS, m_omComboAllMsgs);
	DDV_MaxChars(pDX, m_omStrMsgBlockName, (defBLOCKNAME_SIZE - 1));
}


BEGIN_MESSAGE_MAP(CTxMsgBlocksView, CFormView)
    //{{AFX_MSG_MAP(CTxMsgBlocksView)
    ON_BN_CLICKED(IDC_CBTN_ADD_MSG_BLOCK, OnAddMsgBlock)
    ON_BN_CLICKED(IDC_CBTN_DELETE_MSG_BLOCK, OnDeleteSelectedMsgBlock)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSTC_MSG_BLOCKS_NAME, OnItemchangedLstcMsgBlocksName)
    ON_EN_CHANGE(IDC_EDIT_MSG_BLOCK_NAME, OnChangeEditMsgBlockName)
    //ON_BN_CLICKED(IDC_CHKB_TRIGGER_TYPE, OnChkbTriggerType)
    ON_BN_CLICKED(IDC_CHKB_ON_TIME_TRIGGER, OnChkbOnTimeTrigger)
    ON_BN_CLICKED(IDC_CHKB_ON_KEY_TRIGGER, OnChkbOnKeyTrigger)
    ON_EN_UPDATE(IDC_EDIT_TRG_TIME_VAL, OnUpdateEditTrgTimeVal)
    ON_EN_UPDATE(IDC_EDIT_TRG_KEY_VAL, OnUpdateEditTrgKeyVal)
    //ON_BN_CLICKED(IDC_CHKB_TX_ALL_FRAME, OnChkbTxAllFrame)
    ON_NOTIFY(NM_RCLICK, IDC_LSTC_MSG_BLOCKS_NAME, OnRclickLstcMsgBlocksName)
    ON_COMMAND(IDM_ADD_MSG_BLOCK, OnAddMsgBlock)
    ON_COMMAND(IDM_DELETE_SEL_MSG_BLOCK, OnDeleteSelectedMsgBlock)
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_RADIOMONOSHOT, OnBnClickedRadiomonoshot)
    ON_BN_CLICKED(IDC_RADIOCYCLIC, OnBnClickedRadiocyclic)
    ON_CBN_SELCHANGE(IDC_COMBO_MSGS, OnCbnSelchangeComboMsgs)
END_MESSAGE_MAP()


#ifdef _DEBUG
void CTxMsgBlocksView::AssertValid() const
{
    CFormView::AssertValid();
}

void CTxMsgBlocksView::Dump(CDumpContext& dc) const
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
                   parent window class so that other views shall access it. This
                   will initialise message block list control and oter edit
                   boxes.
  Member of      : CTxMsgBlocksView
  Author(s)      : Raja N
  Date Created   : 26.4.2005
  Modifications  : 
*******************************************************************************/
void CTxMsgBlocksView::OnInitialUpdate() 
{
    CFormView::OnInitialUpdate();
    // Set Init Flag to true
    m_bInitDlg = TRUE;
    // Register this to child window
    CWnd * pWnd = NULL;
    // Get Parent window pointer
    pWnd = pomGetParentWindow();
    if( pWnd != NULL )
    {
        // Register view pointer
        ((CTxMsgChildFrame *)pWnd)->vSetTxMsgViewPointers( eTxMsgBlocksView,
                                                             this );
    }
    // Get Message List View Pointer
    CTxMsgListView * pomListView = NULL;
    pomListView = (CTxMsgListView *)pomGetListViewPointer();
    // Populate List Control
    CHAR caColumnName[defMESSAGE_FRAME_COLUMN][defSTRING_SIZE] = {
                                                    defMESSAGE_BLOCK_NAME,
                                                    defMESSAGE_BLOCK_TRIGGER,
                                                    defMESSAGE_BLOCK_TRIG_VAL,
                                                    defMESSAGE_DATA_BYTES };
    RECT rListCtrlRect;
    INT nTotalColunmSize = 0;
    INT nTotalStrLengthPixel = 0;
    INT nColumnSize = 0;
    //Calculate the total size of all column header   
    m_omLctrMsgBlockName.GetWindowRect( &rListCtrlRect);
    nTotalColunmSize     = rListCtrlRect.right - rListCtrlRect.left;
    nTotalStrLengthPixel = 0;
    UINT i;     //i declared outside loop
    for(i=0; i<defMESSAGE_BLOCK_COLUMN;i++)
    {
         nTotalStrLengthPixel += 
             m_omLctrMsgBlockName.GetStringWidth(caColumnName[i]) ;
    }
    //Insert each column name after calculating the size for the same.
    for(i=0; i<defMESSAGE_BLOCK_COLUMN;i++)
    {
         nColumnSize  = m_omLctrMsgBlockName.GetStringWidth(caColumnName[i]) ;
         nColumnSize +=
             (nTotalColunmSize-nTotalStrLengthPixel)/defMESSAGE_BLOCK_COLUMN;
         m_omLctrMsgBlockName.InsertColumn(i,caColumnName[i],
                                        LVCFMT_CENTER, nColumnSize);
    }
    m_omLctrMsgBlockName.SetExtendedStyle(
                                LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
    // Get the message block count. No memory allocation is required.
    UINT* punCount = &m_unMsgBlockCount;
    CTxWndDataStore::ouGetTxWndDataStoreObj().bGetTxData(TX_MSG_BLOCK_COUNT, (void**)&punCount);
    // Make a local copy of the data structure
    // The local copy will be used to manipulate message frame details
    // Memory allocation will be done in the CConfigDetails class.
    // Get the pointer only if there m_unMsgBlockCount>0
    if(m_unMsgBlockCount>0)
    {
        CTxWndDataStore::ouGetTxWndDataStoreObj().bGetTxData(TX_SEND_MULTI_MSGS, (void**)&m_psMsgBlockList);
    }

    // Diable controls if there is no message blocks
    if(m_unMsgBlockCount == 0)
    {
        vEnableControls(FALSE);
    }
    // Update Message List view
    if( pomListView != NULL )
    {
        // If no message Blocks then disable controls
        if(m_unMsgBlockCount == 0)
        {
            pomListView->m_omButtonDeleteAllMsg.EnableWindow(FALSE);
            pomListView->m_omButtonDeleteSelMsg.EnableWindow(FALSE);
            m_omComboAllMsgs.EnableWindow(FALSE);
            /*m_omButtonTxAllFrame.EnableWindow(FALSE);*/
        }
        else
        {
            pomListView->m_omButtonDeleteAllMsg.EnableWindow(!CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG));
        }
    }

    PSMSGBLOCKLIST psCurrentMsgBlock = NULL;
    CString omStr = STR_EMPTY;
    psCurrentMsgBlock = m_psMsgBlockList;

    // Disable AllTxMessage check box
    // This will be updated in the function vDisplayMsgBlockDetails
    // Disabling it here will avoid enabling the control when no blocks or
    // messages are present
    m_omComboAllMsgs.InsertString(0, "All");
    m_omComboAllMsgs.InsertString(1, "Single");
    m_omComboAllMsgs.SelectString(0, "Single");
    m_omComboAllMsgs.EnableWindow( FALSE );
    /*m_omButtonTxAllFrame.EnableWindow( FALSE );*/

    for(i = 0; i<m_unMsgBlockCount;i++)
    {
        if(psCurrentMsgBlock != NULL )
        {
            m_omLctrMsgBlockName.InsertItem( i,
                                          psCurrentMsgBlock->m_acStrBlockName);
            if(psCurrentMsgBlock->m_bType == TRUE)
            {
                m_omLctrMsgBlockName.SetItemText( i,defSUBITEM_MSGBLOCK_TYPE,
                                                  defMSG_CYCLIC);
            }
            else
            {
                m_omLctrMsgBlockName.SetItemText( i,defSUBITEM_MSGBLOCK_TYPE,
                                                  defMSG_MONOSHOT);
            }
            // Clear the previous content
            omStr.Empty();
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
            m_omLctrMsgBlockName.SetItemText(i,defSUBITEM_MSGBLOCK_TRG_VAL,
                                                                        omStr);
            m_omLctrMsgBlockName.SetCheck(i,psCurrentMsgBlock->m_bActive);
            psCurrentMsgBlock = psCurrentMsgBlock->m_psNextMsgBlocksList;
        }
    }
    // Set Limit and Type for edit boxes
    vSetControlProperties();
    // If number of message blocks exceed disable add button
    if(m_unMsgBlockCount >= defMAX_MSGBLOCK )
    {
        m_omButtonAddMsgBlock.EnableWindow(FALSE);
    }
    // Set Init flag to false
    m_bInitDlg = FALSE;
    // Set the focus to the first entry
    m_omLctrMsgBlockName.SetItemState(0,LVIS_SELECTED|LVIS_FOCUSED,
                                                    LVIS_SELECTED|LVIS_FOCUSED);
    // Disable Update Button as this is the initial view
    CTxFunctionsView *pomFunctionsView = (CTxFunctionsView *)
                                    pomGetFunctionsViewPointer();
    if( pomFunctionsView != NULL )
    {
        pomFunctionsView->m_omButtonApply.EnableWindow( FALSE );
    }
    //Initiate All msgs combo box
    /*m_omComboAllMsgs.AddString("Single");
    m_omComboAllMsgs.AddString("All");
    m_omComboAllMsgs.SelectString(0, "Single");
    m_omComboAllMsgs.EnableWindow(FALSE);*/
}

/**
 * \brief On Add Msg Block
 * \req RS_17_01 - Addition of a transmission block
 *
 * This function will called when user selects Add button
 * or menu. This will add a new message block and
 * initialise it.
 */
void CTxMsgBlocksView::OnAddMsgBlock() 
{
    PSMSGBLOCKLIST psCurrentMsgBlock  = NULL;
    BOOL bAdd = TRUE;
    // Get Message List View Pointer
    CTxMsgListView * pomListView = NULL;
    pomListView = (CTxMsgListView *)pomGetListViewPointer();
    // Call function to add message block
    bAdd = bAddBlock(psCurrentMsgBlock);
    // Once add is successfull, the message details will be updated in control
    // fields.
    if(psCurrentMsgBlock != NULL && bAdd == TRUE)
    {
        // Check/Uncheck based on Tx status
        if( FALSE == CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG) )
        {
            psCurrentMsgBlock->m_bActive = TRUE;
        }
        
        CString omStr = _T("");
        if( pomListView != NULL )
        {
            pomListView->m_omLctrMsgList.DeleteAllItems();
        }
        m_unMsgBlockCount++;
        
        INT nTotalCount = m_omLctrMsgBlockName.GetItemCount();
        m_bInitDlg = TRUE;
        // Insert the new Item
        m_omLctrMsgBlockName.InsertItem(nTotalCount,
                                        psCurrentMsgBlock->m_acStrBlockName);
        m_omLctrMsgBlockName.SetItemText(nTotalCount,
                                         defSUBITEM_MSGBLOCK_TYPE,
                                         defMSG_CYCLIC);
        // Update Checkbox status
        m_omLctrMsgBlockName.SetCheck( nTotalCount,
                                       psCurrentMsgBlock->m_bActive );
        //Update cyclic radio button
        CButton *pRadioCyclic = (CButton*)GetDlgItem(IDC_RADIOCYCLIC);
        if (pRadioCyclic != NULL)
        {
            pRadioCyclic->SetCheck(psCurrentMsgBlock->m_bType);
        }
        // Assign the local variable too
        m_nRBTNTriggerType = psCurrentMsgBlock->m_ucTrigger;

        omStr.Format( defSTR_TIMER_VAL_FMT_SPECIFIER,
                      psCurrentMsgBlock->m_unTimeInterval );
        omStr += defMESSAGE_TRIG_MS;
        m_omLctrMsgBlockName.SetItemText(nTotalCount,
                                         defSUBITEM_MSGBLOCK_TRG_VAL,
                                         omStr);
        m_bInitDlg = FALSE;
        // Selection will be shifted to at the newly added message block in
        // list.
        m_omLctrMsgBlockName.SetItemState(nTotalCount,
                                          LVIS_SELECTED|LVIS_FOCUSED,
                                          LVIS_SELECTED|LVIS_FOCUSED);
       CTxFunctionsView * pView =
                (CTxFunctionsView * )pomGetFunctionsViewPointer();
       if( pView != NULL )
       {
           pView->m_omButtonApply.EnableWindow(TRUE);
       }
       if(m_unMsgBlockCount >= defMAX_MSGBLOCK )
       {
            m_omButtonAddMsgBlock.EnableWindow(FALSE);
       }
    }
}


/******************************************************************************/
/*  Function Name    :  bAddBlock                                             */
/*  Input(s)         :                                                        */
/*  Output           :  TRUE or FALSE                                         */
/*  Functionality    :  This function will add message block at the end in the*/
/*                      message block list. The message block will be         */
/*                      initailised.                                          */
/*                                                                            */
/*  Member of        :  CTxMsgBlocksView                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
BOOL CTxMsgBlocksView::bAddBlock(SMSGBLOCKLIST* &psMsgCurrentBlock)
{
    BOOL bReturn              = TRUE;
    PSMSGBLOCKLIST psMsgBlock = new SMSGBLOCKLIST;
    PSMSGBLOCKLIST psMsgNextBlock = NULL;
    // Check if memory allocation is successful.
    if(psMsgBlock != NULL )
    {
        psMsgCurrentBlock = psMsgBlock;
        vInitMsgBlockInfo(psMsgBlock);
    }
    else
    {
        bReturn = FALSE;
    }
    // if it is succssfull add this block in the list at the end.
    // if it is first block then m_psMsgBlockList will be assigned the same
    // memory.
    if(bReturn == TRUE)
    {
        psMsgNextBlock         = m_psMsgBlockList;
        if(psMsgNextBlock != NULL )
        {
            do 
            {
                if(psMsgNextBlock->m_psNextMsgBlocksList == NULL )
                {
                   psMsgNextBlock->m_psNextMsgBlocksList = psMsgBlock;
                   psMsgNextBlock = psMsgBlock->m_psNextMsgBlocksList;
                }
                else
                {
                    psMsgNextBlock = psMsgNextBlock->m_psNextMsgBlocksList;
                }
            }while(psMsgNextBlock != NULL );
        }
        else
        {
            m_psMsgBlockList  = psMsgBlock;
        }
    }
    return bReturn;
}


/******************************************************************************/
/*  Function Name    :  vInitMsgBlockInfo                                     */
/*  Input(s)         : PSMSGBLOCKLIST psMsgBlockList:current msg block pointer*/
/*  Output           :                                                        */
/*  Functionality    :  This function will initialise the message block       */
/*                      element with default values                           */
/*                                                                            */
/*  Member of        :  CTxMsgBlocksView                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */
/******************************************************************************/
void CTxMsgBlocksView::vInitMsgBlockInfo(SMSGBLOCKLIST* psMsgBlockList)
{
    if(psMsgBlockList != NULL )
    {
        psMsgBlockList->m_psNextMsgBlocksList = NULL;
        psMsgBlockList->m_unMsgCount          = 0;
        psMsgBlockList->m_bActive             = FALSE;
        psMsgBlockList->m_bType               = TRUE;
        psMsgBlockList->m_bTxAllFrame         = FALSE;
        psMsgBlockList->m_ucTrigger           = defTIME_TRIGGER;
        psMsgBlockList->m_psTxCANMsgList      = NULL;
        psMsgBlockList->m_ucKeyValue          = defDEFAULT_KEY_VAL;
        psMsgBlockList->m_unTimeInterval      = defDEFAULT_TIME_VAL;
        _tcscpy( psMsgBlockList->m_acStrBlockName, defDEFAULT_MSG_BLOCK_NAME);
    }
}

/**
 * \brief On Delete Selected Msg Block
 * \req RS_17_02 - Deletion of a transmission block
 *
 * This function will be called by frame work when delete
 * button or menu is selected. This will be called for
 * message block deletion. It will delete the whole block
 * by removing it from list and also all message frame in
 * the block will be deleted. Memory will be deallocated
 */
void CTxMsgBlocksView::OnDeleteSelectedMsgBlock()
{
    BOOL bReturn = FALSE;
    if ( AfxMessageBox( defDEL_MSG_BLOCK, 
                        MB_YESNO|MB_ICONQUESTION) == IDYES)
    {
        if(m_nSelectedMsgBlockIndex != -1 )
        {
            m_bMsgBlockDeleted = TRUE;
            bReturn = bDeleteBlock();
            if(bReturn == TRUE )
            {
                m_unMsgBlockCount--;
                m_omLctrMsgBlockName.DeleteItem(m_nSelectedMsgBlockIndex);
                // Shift the selection if there is items in box.
                if(m_unMsgBlockCount < defMAX_MSGBLOCK )
                {
                    m_omButtonAddMsgBlock.EnableWindow(TRUE);
                }

                if(m_unMsgBlockCount >0 )
                {
                    // Shift the selection to item under fucus as selection
                    // deos not move up if an selected item is deleted.
                    m_nSelectedMsgBlockIndex = 
                      m_omLctrMsgBlockName.GetNextItem(-1,LVIS_FOCUSED);
                    if(m_nSelectedMsgBlockIndex != -1 )
                    {
                        m_omLctrMsgBlockName.SetItemState(
                                                   m_nSelectedMsgBlockIndex,
                                                   LVIS_SELECTED,
                                                   LVIS_SELECTED);
                    }
                }
                else
                {
                    //The Message Block List is Empty. Disable (Message Block) Delete Button
                    m_omButtonDeleteMsgBlock.EnableWindow(FALSE);
                    vEnableControls(FALSE);
                    // Disable Signal List and related component components
                    CTxMsgDetailsView * pView = (CTxMsgDetailsView * )
                                                pomGetDetailsViewPointer();
                    if( pView != NULL )
                    {
                        pView->vDisableSignalComponents();
                        pView->vSetValues(NULL);
                    }
                }

                CTxFunctionsView * pView = (CTxFunctionsView * )
                                                pomGetFunctionsViewPointer();
                if( pView != NULL )
                {
                    pView->m_omButtonApply.EnableWindow(TRUE);
                }
                // Update Modified Flag
                m_bModified = TRUE;
            }
            m_bMsgBlockDeleted = FALSE;
        }
    }
}

/******************************************************************************/
/*  Function Name    :  bDeleteBlock                                          */
/*  Input(s)         :                                                        */
/*  Output           :  TRUE or FALSE                                         */
/*  Functionality    :  This function will delete a message block and its msg */
/*                      list after removing node from the list. It will also  */
/*                      delete the memory, after removing all message list and*/
/*                      memory for message list.                              */
/*                                                                            */
/*  Member of        :  CTxMsgBlocksView                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
BOOL CTxMsgBlocksView::bDeleteBlock()
{
    BOOL bReturn = FALSE;
    // Get List view pointer
    CTxMsgListView * pomListView = (CTxMsgListView * )pomGetListViewPointer();
    if( pomListView != NULL )
    {
        bReturn = TRUE;
        
        PSMSGBLOCKLIST psMsgCurrentBlock = NULL;
        PSMSGBLOCKLIST psDelMsgBlock     = NULL;
        psDelMsgBlock = 
            psGetMsgBlockPointer(m_nSelectedMsgBlockIndex,m_psMsgBlockList);
        bReturn = pomListView->bDeleteAllMsgFromBlock(psDelMsgBlock);
        if(bReturn == TRUE )
        {
            if(psDelMsgBlock != NULL)
            {
                // If message block to be deleted is first message block.
                // the take the next block pointer and assign to header.
                if(psDelMsgBlock == m_psMsgBlockList)
                {
                    m_psMsgBlockList = psDelMsgBlock->m_psNextMsgBlocksList;
                } 
                else 
                {
                    // If the block not the first one, then remove the node 
                    // to be deleted from the list.
                    psMsgCurrentBlock = 
                        psGetMsgBlockPointer( m_nSelectedMsgBlockIndex - 1,
                                              m_psMsgBlockList);
                    if(psMsgCurrentBlock != NULL )
                    {
                        psMsgCurrentBlock->m_psNextMsgBlocksList = 
                                        psDelMsgBlock->m_psNextMsgBlocksList;
                    }
                    else
                    {
                        bReturn = FALSE;
                    }
                }
                // if successful then delete the block.
                if(bReturn == TRUE )
                {
                    delete psDelMsgBlock;
                    psDelMsgBlock = NULL;
                    //Clear the List & Disable Buttons
                    // (Required for the First Message Block )
                    pomListView->m_omLctrMsgList.DeleteAllItems();
                    pomListView->m_omButtonDeleteAllMsg.EnableWindow(FALSE);
                    pomListView->m_omButtonDeleteSelMsg.EnableWindow(FALSE);
                }
            }
            else
            {
                bReturn = FALSE;
            }
        }
    }
    return bReturn;
}
/******************************************************************************/
/*  Function Name    :  psGetMsgBlockPointer                                  */
/*  Input(s)         :  nIndexCurBlock : Index of message block in the list   */
/*                      psCurrentMsgBlockList : header of msg block pointer   */
/*  Output           :                                                        */
/*  Functionality    :  This function will return message block pointer for   */
/*                      node at index nIndexCurBlock passed as parameter from */
/*                      the link list of message block.                       */
/*                                                                            */
/*  Member of        :  CTxMsgBlocksView                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */    
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
SMSGBLOCKLIST* CTxMsgBlocksView::psGetMsgBlockPointer(INT nIndexCurBlock,
                                                 SMSGBLOCKLIST* psMsgBlockList)
{
    if(psMsgBlockList != NULL && nIndexCurBlock != -1)
    {
	    INT nIndex = 0;
        // Get the message block pointer in the list at the index 
        // nIndexCurBlock
        while(nIndex != nIndexCurBlock)
        {
            if( psMsgBlockList != NULL )
            {
                psMsgBlockList = psMsgBlockList->m_psNextMsgBlocksList ;
            }
            nIndex++;
        }
    }
    return psMsgBlockList;
}

/******************************************************************************/
/*  Function Name    :  OnItemchangedLstcMsgBlocksName                        */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function will be called by frame work when       */
/*                      selection is changed in message blocks list control.  */
/*                                                                            */
/*  Member of        :  CTxMsgBlocksView                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  22.07.2004, Added Macros to check the trigger type    */
/*  Modification on  :  02.08.2004, Moved Enabling controls function call     */
/*                      after update of message block details. Because that   */
/*                      function refers message block details to update       */
/*                      controls                                              */
/*  Modification By  :  Anish                                                 */
/*  Modification on  :  26.12.2006, Key trigger + update bug fixed            */
/******************************************************************************/
void CTxMsgBlocksView::OnItemchangedLstcMsgBlocksName(NMHDR* pNMHDR, 
                                                      LRESULT* pResult) 
{    
    //vSaveCurrentBlockFirst();
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    if(pNMListView->uChanged != LVIF_TEXT && m_bInitDlg == FALSE)
    {
        UINT unCurrentState = 0;
        PSMSGBLOCKLIST psMsgBlock = NULL ;
        // Get Message List View Pointer
        CTxMsgListView * pomListView = NULL;
        pomListView = (CTxMsgListView *)pomGetListViewPointer();
        CTxFunctionsView * pomFunctionView = NULL;
        pomFunctionView = (CTxFunctionsView *)pomGetFunctionsViewPointer();
        // Get the current state i.e new state of the list control items
        // and check for if it is checked, unchecked, selected and focused.
        unCurrentState = pNMListView->uNewState & defMASK_LIST_CTRL;
        if(  unCurrentState != FALSE
            &&  pNMListView->uChanged  == LVIF_STATE )
        {
            // check if the item is focused and selected.
            unCurrentState =
                pNMListView->uNewState &(LVIS_FOCUSED |LVIS_SELECTED);
            // If yes, update information for newly selected item.
            if(unCurrentState != FALSE)
            {
                m_nSelectedMsgBlockIndex = pNMListView->iItem;
                psMsgBlock = psGetMsgBlockPointer(pNMListView->iItem,
                                                  m_psMsgBlockList);
                if( pomListView != NULL )
                {
                    pomListView->m_omLctrMsgList.DeleteAllItems();
                    pomListView->m_omButtonSendMsg.EnableWindow(FALSE);
                    pomListView->m_nSelectedMsgIndex = -1;
                }
                if(psMsgBlock != NULL )
                {

                    vDisplayMsgBlockDetails(psMsgBlock);
                    vEnableControls(TRUE);
                    // Enavle Add Button
                    CTxMsgDetailsView * pView = (CTxMsgDetailsView * )
                                        pomGetDetailsViewPointer();
                    if( pView != NULL )
                    {
                        pView->vEnableAddButton( TRUE );
                    }
                    if( pomListView != NULL )
                    {
                        if(psMsgBlock->m_unMsgCount > 0)
                        {
                            pomListView->
                                m_omButtonDeleteAllMsg.EnableWindow(!CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG));
                        }
                        else
                        {
                            pomListView->
                                m_omButtonDeleteAllMsg.EnableWindow(FALSE);
                        }
                        // By Default Disable Delete as none of the
                        // items are selected
                        pomListView->m_omButtonDeleteSelMsg.EnableWindow(FALSE);
                    }
                    // if the block is active and then enable or disable 
                    // some button based on if transmission is on or off.
                    if(psMsgBlock->m_bActive == TRUE)
                    {
                        
                        if( IS_KEY_TRIGGERED (psMsgBlock->m_ucTrigger) )
                        {
                            m_omComboAllMsgs.EnableWindow(!CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG));
                            /*m_omButtonTxAllFrame.EnableWindow(!CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG));*/
                            m_omEditTrgKeyVal.EnableWindow(!CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG));
                        }
                        if( IS_TIME_TRIGGERED (psMsgBlock->m_ucTrigger) )
                        {
                            m_omEditTrgTimeIntervalVal.EnableWindow(!CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG));
                        }
                        m_omButtonTimeTrigger.EnableWindow(!CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG));
                        m_omButtonKeyTrigger.EnableWindow(!CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG));
                        //update monoshot and cyclic radio button
                        CButton* pMonoRadioButton = (CButton*)GetDlgItem(IDC_RADIOMONOSHOT);
                        if (pMonoRadioButton != NULL)
                        {
                            pMonoRadioButton->EnableWindow(!CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG));
                        }
                        CButton* pRadioCyclic = (CButton*)GetDlgItem(IDC_RADIOCYCLIC);
                        if (pRadioCyclic != NULL)
                        {
                            pRadioCyclic->EnableWindow(!CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG));
                        }
                        //m_omButtonTriggerType.EnableWindow(!CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG));
                    }

                }
            }
            // if it is checked or unchecked, get the condition of the item
            // and store it in message block pointer for current
            // selected message.
            unCurrentState = pNMListView->uNewState & defMASK_CHECK_UNCHECK;
            psMsgBlock = psGetMsgBlockPointer(pNMListView->iItem,
                                                  m_psMsgBlockList);
            // Update check box selection only if transmission is not on.
            if(unCurrentState != FALSE && CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG) == FALSE)
            {
                psMsgBlock->m_bActive = 
                    m_omLctrMsgBlockName.GetCheck(pNMListView->iItem);
                if( pomFunctionView != NULL )
                {
                    pomFunctionView->m_omButtonApply.EnableWindow(TRUE);
                }
                // Update Modified Flag
                m_bModified = TRUE;
            }
            else if (unCurrentState != FALSE  )
            {
                // Keep the check/uncheck condition as it is if transmission is
                // on
                m_omLctrMsgBlockName.SetCheck(pNMListView->iItem,
                    psMsgBlock->m_bActive );
            }
        }
        else if(pNMListView->uChanged  == LVIF_STATE && 
                pNMListView->uOldState == LVIS_SELECTED )
        {
          psMsgBlock = NULL ;
          psMsgBlock = psGetMsgBlockPointer(pNMListView->iItem,
                                            m_psMsgBlockList);
          // Save the changes if items is deselected and not deleted.
          if(psMsgBlock != NULL && m_bMsgBlockDeleted == FALSE )
          {
             // vUpdateMsgBlockDetials(psMsgBlock);
              m_omButtonDeleteMsgBlock.EnableWindow(FALSE);
              //Disable Delete All Button
              if( pomListView != NULL )
              {
                  pomListView->m_omButtonDeleteAllMsg.EnableWindow(FALSE);
              }
          }
        }
    }
    *pResult = 0;
}

/******************************************************************************/
/*  Function Name    :  vDisplayMsgBlockDetails                               */
/*  Input(s)         :  PSMSGBLOCKLIST psMsgBlock: current msg block pointer  */
/*  Output           :                                                        */
/*  Functionality    :  This function will display the message block details  */
/*                      in this function                                      */
/*                                                                            */
/*  Member of        :  CTxMsgBlocksView                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  22.07.2004, Updated new check box controls and used   */
/*                      macros to find the type of the trigger                */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  02.08.2004, Removed setting type information to timer */
/*                      and key trigger value editboxs. Moved the code in to  */
/*                      vSetControlProperties function                        */
/******************************************************************************/
VOID CTxMsgBlocksView::vDisplayMsgBlockDetails(SMSGBLOCKLIST* psMsgBlock)
{
    if(psMsgBlock != NULL )
    {
        m_omStrMsgBlockName = psMsgBlock->m_acStrBlockName;
        m_omEditMsgBlockName.SetWindowText(m_omStrMsgBlockName);
        // TRUE -> CYCLIC so uncheck the check box "Monoshot"
        // FALSE -> MONOSHOT so check the check box "Monoshot"
        CButton *pRadioCyclic = (CButton*)GetDlgItem(IDC_RADIOCYCLIC);
        CButton *pRadioMonoShot = (CButton*)GetDlgItem(IDC_RADIOMONOSHOT);
        if ((pRadioCyclic != NULL) && (pRadioMonoShot != NULL))
        {
            pRadioCyclic->SetCheck(psMsgBlock->m_bType);
            pRadioMonoShot->SetCheck(!psMsgBlock->m_bType);            
        }
        if (psMsgBlock->m_bType == TRUE)
        {
            SetDlgItemText(IDC_GROUPBOX_TRIGGER, "Trigger (Cyclic) on event");
        }
        else
        {
            SetDlgItemText(IDC_GROUPBOX_TRIGGER, "Trigger (Monoshot) on event");
        }
        //m_omButtonTriggerType.SetCheck(!psMsgBlock->m_bType);
        
        if (psMsgBlock->m_bTxAllFrame)
        {
            m_omComboAllMsgs.SelectString(0, "All");
        }
        else
        {
            m_omComboAllMsgs.SelectString(0, "Single");
        }
        /*m_omButtonTxAllFrame.SetCheck(psMsgBlock->m_bTxAllFrame);*/
        m_nRBTNTriggerType = psMsgBlock->m_ucTrigger;
        // Update Time Trigger Values
        if(IS_TIME_TRIGGERED( m_nRBTNTriggerType ))
        {
            CButton* pomOnTime = (CButton*)GetDlgItem(IDC_CHKB_ON_TIME_TRIGGER);
            if( pomOnTime != NULL )
            {
                pomOnTime->SetCheck( TRUE );
            }
            // Assign only if it is valid
            if( psMsgBlock->m_unTimeInterval != 0 )
            {
                m_omStrTimeIntervalVal.Format( defSTR_TIMER_VAL_FMT_SPECIFIER,
                                               psMsgBlock->m_unTimeInterval);
            }
            m_omEditTrgTimeIntervalVal.SetWindowText(m_omStrTimeIntervalVal);
        }
        else
        {
            // Disable Related Controls
            CButton* pomOnTime = (CButton*)GetDlgItem(IDC_CHKB_ON_TIME_TRIGGER);
            if( pomOnTime != NULL )
            {
                pomOnTime->SetCheck( FALSE );
            }
            m_omEditTrgTimeIntervalVal.SetWindowText(defDEFAULT_TIME_INTERVAL);
            m_omEditTrgTimeIntervalVal.EnableWindow( FALSE );
        }

        // Update Key Trigger Values
        if( IS_KEY_TRIGGERED( m_nRBTNTriggerType ) )
        {
            CButton* pomOnTime = (CButton*)GetDlgItem(IDC_CHKB_ON_KEY_TRIGGER);
            if( pomOnTime != NULL )
            {
                pomOnTime->SetCheck( TRUE );
            }
            m_omStrKeyVal.Format( defSTR_KEY_VAL_FMT_SPECIFIER,
                                  psMsgBlock->m_ucKeyValue);
            m_omEditTrgKeyVal.SetWindowText(m_omStrKeyVal);
            /*m_omComboAllMsgs.EnableWindow();
            if (psMsgBlock->m_bTxAllFrame == TRUE)
            {
                m_omComboAllMsgs.SelectString(0, "All");
            }
            else
            {
                m_omComboAllMsgs.SelectString(0, "Single");
            }*/
        }
        else
        {
            CButton* pomOnTime = (CButton*)GetDlgItem(IDC_CHKB_ON_KEY_TRIGGER);
            if( pomOnTime != NULL )
            {
                pomOnTime->SetCheck( FALSE );
            }
            m_omComboAllMsgs.SelectString(0, "Single");
            m_omComboAllMsgs.EnableWindow(FALSE);
            /*m_omButtonTxAllFrame.SetCheck( FALSE );
            /*m_omButtonTxAllFrame.EnableWindow( FALSE );*/
            m_omEditTrgKeyVal.SetWindowText(defDEFAULT_KEY_VAL_STR);
            m_omEditTrgKeyVal.EnableWindow( FALSE );
        }

        // Update message details list for currently selected message
        // block.
        if(psMsgBlock->m_unMsgCount > 0)
        {
            CTxMsgListView * pomListView = ( CTxMsgListView * )
                                        pomGetListViewPointer();
            if( pomListView != NULL )
            {
                PSTXCANMSGLIST psTxMsgList = NULL;
                psTxMsgList = pomListView->psGetMsgDetailPointer(0,psMsgBlock);
                for(UINT i =0; i<psMsgBlock->m_unMsgCount; i++)
                {
                    if(psTxMsgList != NULL )
                    {
                        pomListView->vUpdateMsgListDisplay(
                                        psTxMsgList->m_sTxMsgDetails,-1 );
                        psTxMsgList = psTxMsgList->m_psNextMsgDetails;
                    }
                }
            }
        }
        UpdateData(TRUE);
    }
}

/******************************************************************************/
/*  Function Name    :  vUpdateMsgBlockDetials                                */
/*  Input(s)         :  PSMSGBLOCKLIST psCurrentMsgBlock:current block pointer*/
/*  Output           :                                                        */
/*  Functionality    :  Update the message block information entered by user  */
/*                                                                            */
/*  Member of        :  CTxMsgBlocksView                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  22.07.2004, Updated new check box controls and used   */
/*                      macros to find the type of the trigger                */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  02.08.2004, Removed Hardcoded values                  */
/******************************************************************************/
VOID CTxMsgBlocksView::vUpdateMsgBlockDetials(SMSGBLOCKLIST* psCurrentMsgBlock)
{
    UpdateData(TRUE);
    if(psCurrentMsgBlock != NULL )
    {
        //check the All Msgs state
        int nCurSel = m_omComboAllMsgs.GetCurSel();
        CString omSelString;
        m_omComboAllMsgs.GetLBText(nCurSel, omSelString);
        if (omSelString == "All")
        {
            psCurrentMsgBlock->m_bTxAllFrame = TRUE;
        }
        else
        {
            psCurrentMsgBlock->m_bTxAllFrame = FALSE;
        }
        
        _tcscpy(psCurrentMsgBlock->m_acStrBlockName, m_omStrMsgBlockName.GetBuffer(MAX_PATH));
      //check the Monoshot radio button state
      BOOL bMonoshot = FALSE;
      CButton *pRadioMonoshot = (CButton*)GetDlgItem(IDC_RADIOMONOSHOT);
      if (pRadioMonoshot != NULL)
      {
        bMonoshot = pRadioMonoshot->GetCheck();
      }
      psCurrentMsgBlock->m_bType = !bMonoshot;//!m_bTriggerType;
      psCurrentMsgBlock->m_ucTrigger = m_nRBTNTriggerType;
      // if the trigger type is on time else it is on key
      if( IS_TIME_TRIGGERED (psCurrentMsgBlock->m_ucTrigger) )
      {
        CHAR* pcChar = NULL;
        LONG lTimeVal = strtol(m_omStrTimeIntervalVal,&pcChar, defBASE_DEC);
        if( lTimeVal > 0 )
        {
            psCurrentMsgBlock->m_unTimeInterval = lTimeVal;
        }
        else
        {
            psCurrentMsgBlock->m_unTimeInterval = defDEFAULT_TIME_VAL;
        }
      }
      else
      {
          psCurrentMsgBlock->m_unTimeInterval = defDEFAULT_TIME_VAL;
      }

      if( IS_KEY_TRIGGERED(psCurrentMsgBlock->m_ucTrigger) )
      {
        if(m_omStrKeyVal.IsEmpty() == FALSE)
        {
            psCurrentMsgBlock->m_ucKeyValue     = m_omStrKeyVal.GetAt(0);
        }
        else
        {
            psCurrentMsgBlock->m_ucKeyValue = defDEFAULT_KEY_VAL;
        }
      }
    }
}

/******************************************************************************/
/*  Function Name    :  OnChangeEditMsgBlockName                              */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function will be called by frame work when user  */
/*                      changes edit box "Message Block name"                 */
/*                                                                            */
/*  Member of        :  CTxMsgBlocksView                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */    
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
void CTxMsgBlocksView::OnChangeEditMsgBlockName() 
{
    CString omStrMsgBlockName = _T("");
    UpdateData(TRUE);
    // Update the item in message block list control having selections.
    m_omLctrMsgBlockName.SetItemText(m_nSelectedMsgBlockIndex,
                                     defMAIN_ITEM,
                                     m_omStrMsgBlockName);
    CTxFunctionsView * pomFunctionView = ( CTxFunctionsView *)
                            pomGetFunctionsViewPointer();
    if( pomFunctionView != NULL )
    {
        pomFunctionView->m_omButtonApply.EnableWindow(TRUE);
    }
}

/******************************************************************************/
/*  Function Name    :  OnChkbTriggerType                                     */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by framework whenever user    */
/*                      selected check button for trigger type.               */
/*                                                                            */
/*  Member of        :  CTxMsgBlocksView                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */    
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  05.08.2004, Added modified flag to indicate change    */
/******************************************************************************/
void CTxMsgBlocksView::OnChkbTriggerType() 
{
    if(m_nSelectedMsgBlockIndex != -1 )
    {
        BOOL bCheck = FALSE;
        CButton* pButton = (CButton*)GetDlgItem(IDC_RADIOMONOSHOT);
        //bCheck = m_omButtonTriggerType.GetCheck();
        if (pButton != NULL)
        {
            bCheck = pButton->GetCheck();
        }
        bCheck = !bCheck;
        // If the button is checked the trigger type is MONOSHOT else
        // it is cyclic.
        // Update the message block list item accordingly.
        if(bCheck == TRUE)
        {
            m_omLctrMsgBlockName.SetItemText(m_nSelectedMsgBlockIndex,
                                             defSUBITEM_MSGBLOCK_TYPE,
                                             defMSG_CYCLIC);
        }
        else
        {
            m_omLctrMsgBlockName.SetItemText(m_nSelectedMsgBlockIndex,
                                             defSUBITEM_MSGBLOCK_TYPE,
                                             defMSG_MONOSHOT);
        }
        CTxFunctionsView * pomFunctionView = ( CTxFunctionsView *)
                            pomGetFunctionsViewPointer();
        if( pomFunctionView != NULL )
        {
            pomFunctionView->m_omButtonApply.EnableWindow(TRUE);
        }        
        // Update Modified Flag
        PSMSGBLOCKLIST psMsgCurrentBlock = NULL;
        psMsgCurrentBlock = 
            psGetMsgBlockPointer(m_nSelectedMsgBlockIndex,m_psMsgBlockList);
        if (psMsgCurrentBlock != NULL)
        {
            // update this information in the current block
            psMsgCurrentBlock->m_bType = bCheck;
            if( psMsgCurrentBlock->m_bActive == TRUE )
            {   
                m_bModified = TRUE;
            }
        }
    }
}

/*******************************************************************************
 Function Name    : OnChkbOnTimeTrigger
 Input(s)         :
 Output           :
 Functionality    : This function is will update trigger flag and appropriate UI
                    components
 Member of        : CTxMsgBlocksView
 Friend of        :  -
 Author(s)        : Raja N
 Date Created     : 19.07.2004
 Modification By  : Raja N
 Modification on  : 02.08.2004, Added code update trigger value in msg block
                    list control
*******************************************************************************/
void CTxMsgBlocksView::OnChkbOnTimeTrigger() 
{
    CString omStrCurrent;
    // Toggle Time Trigger Bit
    m_nRBTNTriggerType ^= defTIME_TRIGGER;
    //get the current block
    PSMSGBLOCKLIST psMsgCurrentBlock = NULL;
    psMsgCurrentBlock = 
        psGetMsgBlockPointer(m_nSelectedMsgBlockIndex,m_psMsgBlockList);
    if( IS_TIME_TRIGGERED( m_nRBTNTriggerType ))
    {
        m_omEditTrgTimeIntervalVal.EnableWindow();
        omStrCurrent = m_omStrTimeIntervalVal + defMESSAGE_TRIG_MS;
        if (psMsgCurrentBlock != NULL)
        {
            psMsgCurrentBlock->m_ucTrigger |= defTIME_TRIGGER;
        }
        // Add "|" only if key is enabled
        // This check we cannot prevent!!
        if( IS_KEY_TRIGGERED ( m_nRBTNTriggerType ) )
        {
            omStrCurrent += defSTR_MSG_BLOCK_TRIGGER_SEPRATOR;
            /*if (psMsgCurrentBlock != NULL)
            {
                psMsgCurrentBlock->m_ucTrigger |= defKEY_TRIGGER;
            }*/
        }
    }
    else
    {
        // Disable Related Controls
        m_omEditTrgTimeIntervalVal.EnableWindow( FALSE );
        if (psMsgCurrentBlock != NULL)
        {
            psMsgCurrentBlock->m_ucTrigger &= MASK_DISABLE_TIME_TRIGGER;
        }
    }
    // If Key is also enabled then add Key value
    if(  IS_KEY_TRIGGERED ( m_nRBTNTriggerType ) )
    {
        omStrCurrent += m_omStrKeyVal;
        /*if (psMsgCurrentBlock != NULL)
        {
            psMsgCurrentBlock->m_ucTrigger |= defKEY_TRIGGER;
        }*/
    }
    // update the value in list control for message block having current
    // selection
    m_omLctrMsgBlockName.SetItemText( m_nSelectedMsgBlockIndex,
                                      defSUBITEM_MSGBLOCK_TRG_VAL,
                                      omStrCurrent );
    CTxFunctionsView * pomFunctionView = ( CTxFunctionsView *)
                            pomGetFunctionsViewPointer();
    if( pomFunctionView != NULL )
    {
        pomFunctionView->m_omButtonApply.EnableWindow(TRUE);
    }        

    // Update Modified Flag
    
    if( psMsgCurrentBlock->m_bActive == TRUE )
    {
        m_bModified = TRUE;
    }
}

/*******************************************************************************
 Function Name    : OnChkbOnKeyTrigger
 Input(s)         :
 Output           :
 Functionality    : This function is will update trigger flag and appropriate UI
                    components
 Member of        : CTxMsgBlocksView
 Friend of        :  -
 Author(s)        : Raja N
 Date Created     : 19.07.2004
 Modification By  : Raja N
 Modification on  : 02.08.2004, Added code update trigger value in msg block
                    list control
*******************************************************************************/
void CTxMsgBlocksView::OnChkbOnKeyTrigger() 
{
    CString omStrCurrent;
    // Toggle Time Trigger Bit
    m_nRBTNTriggerType ^= defKEY_TRIGGER;
    PSMSGBLOCKLIST psMsgCurrentBlock = NULL;
    psMsgCurrentBlock = 
        psGetMsgBlockPointer(m_nSelectedMsgBlockIndex,m_psMsgBlockList);
    // If Timer is also enabled then add Time value in to list item text
    if( IS_TIME_TRIGGERED ( m_nRBTNTriggerType ) )
    {
        omStrCurrent = m_omStrTimeIntervalVal + defMESSAGE_TRIG_MS;
        // Add "|" only if key is enabled
        // This check we cannot prevent!!
        /*if (psMsgCurrentBlock != NULL)
        {
            psMsgCurrentBlock->m_ucTrigger |= defTIME_TRIGGER;
        }*/
        if( IS_KEY_TRIGGERED( m_nRBTNTriggerType ) )
        {
            omStrCurrent += defSTR_MSG_BLOCK_TRIGGER_SEPRATOR;
            /*if (psMsgCurrentBlock != NULL)
            {
                psMsgCurrentBlock->m_ucTrigger |= defKEY_TRIGGER;
            }*/
        }
    }
    
    if( IS_KEY_TRIGGERED( m_nRBTNTriggerType ))
    {
        m_omComboAllMsgs.EnableWindow();
        /*m_omButtonTxAllFrame.EnableWindow();*/
        m_omEditTrgKeyVal.EnableWindow();
        omStrCurrent += m_omStrKeyVal;
        if (psMsgCurrentBlock != NULL)
        {
            psMsgCurrentBlock->m_ucTrigger |= defKEY_TRIGGER;
        }
    }
    else
    {
        /*m_omButtonTxAllFrame.EnableWindow( FALSE );*/
        m_omComboAllMsgs.EnableWindow(FALSE);
        m_omEditTrgKeyVal.EnableWindow( FALSE );
        if (psMsgCurrentBlock != NULL)
        {
            psMsgCurrentBlock->m_ucTrigger &= MASK_DISABLE_KEY_TRIGGER;
        }
    }
    // update the value in list control for message block having current
    // selection
    m_omLctrMsgBlockName.SetItemText( m_nSelectedMsgBlockIndex,
                                      defSUBITEM_MSGBLOCK_TRG_VAL,
                                      omStrCurrent );
    CTxFunctionsView * pomFunctionView = ( CTxFunctionsView *)
                                    pomGetFunctionsViewPointer();
    if( pomFunctionView != NULL )
    {
        pomFunctionView->m_omButtonApply.EnableWindow(TRUE);
    }        

    // Update Modified Flag    
    
    if( psMsgCurrentBlock->m_bActive == TRUE )
    {
        m_bModified = TRUE;
    }
}

/******************************************************************************/
/*  Function Name    :  OnUpdateEditTrgTimeVal                                */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function will be called by frame work when user  */
/*                      changes is updated in edit box "time".                */
/*                                                                            */
/*  Member of        :  CTxMsgBlocksView                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  22.07.2004, Seprated Timer and On Key Handler         */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  02.08.2004, Added code to update message block trigger*/
/*                      information on update of timer value                  */
/******************************************************************************/
void CTxMsgBlocksView::OnUpdateEditTrgTimeVal() 
{
    UINT unTimeInterVal        = 0;
    CString omStrCurrent       = STR_EMPTY;
    CHAR* pcChar               = NULL;
    m_omEditTrgTimeIntervalVal.GetWindowText(omStrCurrent);
    // Get the time value
    unTimeInterVal = static_cast<UINT>(strtol(omStrCurrent,&pcChar,BASE_10));
    // If it is valid
    PSMSGBLOCKLIST psMsgCurrentBlock =
                                psGetMsgBlockPointer(m_nSelectedMsgBlockIndex,m_psMsgBlockList);
    if(unTimeInterVal > 0 )
    {
        // Update the time valie
        m_omStrTimeIntervalVal = omStrCurrent;
        // Enable apply window
        CTxFunctionsView * pomFunctionView = ( CTxFunctionsView *)
                            pomGetFunctionsViewPointer();
        if( pomFunctionView != NULL )
        {
            pomFunctionView->m_omButtonApply.EnableWindow(TRUE);
        }        

        // Check whether it is enabled and then
        // Add Unit to the time value
        omStrCurrent = STR_EMPTY;
        if( IS_TIME_TRIGGERED ( m_nRBTNTriggerType ) )
        {
            omStrCurrent = m_omStrTimeIntervalVal + defMESSAGE_TRIG_MS;
            if( IS_KEY_TRIGGERED ( m_nRBTNTriggerType ) )
            {
                omStrCurrent += defSTR_MSG_BLOCK_TRIGGER_SEPRATOR;
            }
            // update this information into the block
            CHAR* pcChar = NULL;
            LONG lTimeVal = strtol(m_omStrTimeIntervalVal,&pcChar, defBASE_DEC);
            if( lTimeVal > 0 )
            {
                psMsgCurrentBlock->m_unTimeInterval = lTimeVal;
            }
            else
            {
                psMsgCurrentBlock->m_unTimeInterval = defDEFAULT_TIME_VAL;
            }
        }
        // If Key is also enabled the nadd key part
        if( IS_KEY_TRIGGERED ( m_nRBTNTriggerType )  )
        {
             omStrCurrent += m_omStrKeyVal;
        }
        // update the value in list control for message block having current 
        // selection
        m_omLctrMsgBlockName.SetItemText(m_nSelectedMsgBlockIndex,
                                         defSUBITEM_MSGBLOCK_TRG_VAL,
                                         omStrCurrent);
        // Update Modified Flag
        // Update the flag only if the user is changing the text
        // This function will be called even setting the text using
        // SetwindowText or update data.
        CWnd * pomWnd = NULL;
        pomWnd = GetFocus();
        if( pomWnd != NULL &&
            pomWnd->m_hWnd == m_omEditTrgTimeIntervalVal.m_hWnd)
        {
            
            if( psMsgCurrentBlock->m_bActive == TRUE )
            {
                m_bModified = TRUE;
            }
        }
    }
    else 
    {
        // Restore the old value
        m_omEditTrgTimeIntervalVal.SetWindowText(m_omStrTimeIntervalVal);
        m_omEditTrgTimeIntervalVal.SetSel( 0,
                                           m_omStrTimeIntervalVal.GetLength() );
    }
    
}

/******************************************************************************/
/*  Function Name    :  OnUpdateEditTrgKeyVal                                 */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function will be called by frame work when user  */
/*                      changes is updated in edit box "key value"            */
/*                                                                            */
/*  Member of        :  CTxMsgBlocksView                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  22.07.2004, Seprated Timer and On Key Handler         */
/*  Modification on  :  02.08.2004, Added code to update message block trigger*/
/*                      information on update of Key value                    */
/******************************************************************************/
void CTxMsgBlocksView::OnUpdateEditTrgKeyVal() 
{
    UINT unTextLength = 0;
    CString omStrCurrent = STR_EMPTY;
    // Get the Key Value
    m_omEditTrgKeyVal.GetWindowText(omStrCurrent);
    unTextLength = omStrCurrent.GetLength();
    // If it is valid
    if(unTextLength  == 1 )
    {
        // Update the value and enable Apply Button
        m_omStrKeyVal = omStrCurrent;
        // Enable Apply Button
        CTxFunctionsView * pomFunctionView = ( CTxFunctionsView *)
                            pomGetFunctionsViewPointer();
        if( pomFunctionView != NULL )
        {
            pomFunctionView->m_omButtonApply.EnableWindow(TRUE);
        }        

        // Check whether it is enabled and then
        // Add Unit to the time value
        omStrCurrent = STR_EMPTY;
        if( IS_TIME_TRIGGERED ( m_nRBTNTriggerType ) )
        {
            omStrCurrent = m_omStrTimeIntervalVal + defMESSAGE_TRIG_MS;
            if( IS_KEY_TRIGGERED ( m_nRBTNTriggerType ) )
            {
                omStrCurrent += defSTR_MSG_BLOCK_TRIGGER_SEPRATOR;
            }
        }
        // If Key is also enab7led the nadd key part
        if( IS_KEY_TRIGGERED ( m_nRBTNTriggerType )  )
        {
			PSMSGBLOCKLIST psMsgBlock = psGetMsgBlockPointer(m_nSelectedMsgBlockIndex,
                                                  m_psMsgBlockList);
			psMsgBlock->m_ucKeyValue = m_omStrKeyVal.GetAt(0);
            omStrCurrent += m_omStrKeyVal;
        }
        // update the value in list control for message block having current 
        // selection
        m_omLctrMsgBlockName.SetItemText(m_nSelectedMsgBlockIndex,
                                         defSUBITEM_MSGBLOCK_TRG_VAL,
                                         omStrCurrent);
        // Update Modified Flag
        // Update the flag only if the user is changing the text
        // This function will be called even setting the text using
        // SetwindowText or update data.
        CWnd * pomWnd = NULL;
        pomWnd = GetFocus();
        // If the focus is in the Edit box then proceed
        if( pomWnd != NULL &&
            pomWnd->m_hWnd == m_omEditTrgTimeIntervalVal.m_hWnd)
        {
            PSMSGBLOCKLIST psMsgCurrentBlock = NULL;
            psMsgCurrentBlock = 
                psGetMsgBlockPointer(m_nSelectedMsgBlockIndex,m_psMsgBlockList);
            if( psMsgCurrentBlock->m_bActive == TRUE )
            {
                m_bModified = TRUE;
            }
        }
    }
    else 
    {
        // Restore the old value
        m_omEditTrgKeyVal.SetWindowText(m_omStrKeyVal);
    }
    m_omEditTrgKeyVal.SetSel( 0, m_omStrKeyVal.GetLength());
}

/******************************************************************************/
/*  Function Name    :  OnChkbTxAllFrame                                      */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function is called by the framework when user    */
/*                      clicks "All Messages"  check box. It will enable the  */
/*                      Apply button.                                         */
/*  Member of        :  CTxMsgBlocksView                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  15.01.2004                                            */
/*  Modifications    :                                                        */
/*                                                                            */
/******************************************************************************/
void CTxMsgBlocksView::OnChkbTxAllFrame() 
{
    CTxFunctionsView * pomFunctionView = ( CTxFunctionsView *)
                                        pomGetFunctionsViewPointer();
    if( pomFunctionView != NULL )
    {
        pomFunctionView->m_omButtonApply.EnableWindow(TRUE);
    }
    // Update Modified Flag
    PSMSGBLOCKLIST psMsgCurrentBlock = NULL;
    psMsgCurrentBlock = 
        psGetMsgBlockPointer(m_nSelectedMsgBlockIndex,m_psMsgBlockList);
    if (psMsgCurrentBlock != NULL)
    {
        int nCutSel = m_omComboAllMsgs.GetCurSel();
        CString omSelString;
        m_omComboAllMsgs.GetLBText(nCutSel, omSelString);
        if (omSelString == "All")
        {
            psMsgCurrentBlock->m_bTxAllFrame = TRUE;
        }
        else
        {
            psMsgCurrentBlock->m_bTxAllFrame = FALSE;
        }
        if( psMsgCurrentBlock->m_bActive == TRUE )
        {        
            m_bModified = TRUE;
        }
    }
    
}

/******************************************************************************/
/*  Function Name    :  vEnableControls                                       */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function will enable or disable the controls     */
/*                      needed depending upon the flag bEnable condition .    */
/*                                                                            */
/*  Member of        :  CTxMsgBlocksView                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  22.07.2004, Added new controls to the list            */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  02.08.2004, Added RTR control in to the list and added*/
/*                      checks for Timer and Key edit boxes to avoid enabling */
/*                      if the corresponding ckeck box is not selected.       */
/******************************************************************************/
void CTxMsgBlocksView::vEnableControls(BOOL bEnable)
{
    CWnd *pomWnd = NULL;
    /*CButton* pomOnTime = (CButton*)GetDlgItem(IDC_RBTN_ON_TIME);
    if(pomOnTime != NULL )
    {
        pomOnTime->EnableWindow(bEnable);
    }*/
    // Timer edit box
    // Check whether time is enabled or not
    if( m_omButtonTimeTrigger.GetCheck() )
    {
        m_omEditTrgTimeIntervalVal.EnableWindow( bEnable);
    }
    else
    {
        m_omEditTrgTimeIntervalVal.EnableWindow( FALSE );
    }
    // Key edit box
    // Check whether Key is enabled or not
    if( m_omButtonKeyTrigger.GetCheck() )
    {
        m_omEditTrgKeyVal.EnableWindow( bEnable );
        m_omComboAllMsgs.EnableWindow(bEnable);
    }
    else
    {
        m_omEditTrgKeyVal.EnableWindow( FALSE );
        m_omComboAllMsgs.EnableWindow(FALSE);
    }

    // Update Details View Control
    CTxMsgDetailsView * pomDetailsView = ( CTxMsgDetailsView * )
                                            pomGetDetailsViewPointer();
    if( pomDetailsView != NULL )
    {
        // Message ID/Name Combobox
        pomDetailsView->m_omComboMsgIDorName.EnableWindow(bEnable);
        // Enable disable Add Button
        // Enable Add Button only if the other conditions are met
        pomDetailsView->vEnableAddButton(bEnable);
        // DLC Value
        pomDetailsView->m_odDLC.EnableWindow(bEnable);
        // Data Bytes Value
        pomDetailsView->m_odDB8.EnableWindow(bEnable);
        pomDetailsView->m_odDB7.EnableWindow(bEnable);
        pomDetailsView->m_odDB6.EnableWindow(bEnable);
        pomDetailsView->m_odDB5.EnableWindow(bEnable);
        pomDetailsView->m_odDB4.EnableWindow(bEnable);
        pomDetailsView->m_odDB3.EnableWindow(bEnable);
        pomDetailsView->m_odDB2.EnableWindow(bEnable);
        pomDetailsView->m_odDB1.EnableWindow(bEnable);
        // Enable / Disable Channel ID
        pomDetailsView->m_omComboChannelID.EnableWindow(bEnable);
        // Include Standard/Extended radion and RTR checkbox
        // Standard Radio Button
        pomWnd = pomDetailsView->GetDlgItem(IDC_RBTN_MSGTYPE_STD);
        if( pomWnd != NULL )
        {
            pomWnd->EnableWindow( bEnable );
        }
        // Extended Radio Button
        pomWnd = pomDetailsView->GetDlgItem(IDC_RBTN_MSGTYPE_EXTD);
        if( pomWnd != NULL )
        {
            pomWnd->EnableWindow( bEnable );
        }
        // Standard Radio Button
        pomWnd = pomDetailsView->GetDlgItem(IDC_CHKB_MSGTYPE_RTR);
        if( pomWnd != NULL )
        {
            pomWnd->EnableWindow( bEnable );
        }
    }
    // Update List View 
    CTxMsgListView *pomListView = ( CTxMsgListView * )
                                            pomGetListViewPointer();
    if( pomListView != NULL )
    {
        // Message List control
        pomListView->m_omLctrMsgList.EnableWindow(bEnable);
    }

    // Timer checkbox
    m_omButtonTimeTrigger.EnableWindow(bEnable);
    // Key Checkbox
    m_omButtonKeyTrigger.EnableWindow(bEnable);
    // All Messages Check box
    // Check whether Key is enabled or not
    
    BOOL bChkEnable = FALSE;
    if( bEnable == TRUE )
    {
        // Enable only if key is selected
        if( m_omButtonKeyTrigger.GetCheck() )
        {
            bChkEnable = TRUE;
        }
    }
    m_omComboAllMsgs.EnableWindow( bChkEnable );
   
    if(bEnable == TRUE && CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG) == FALSE )
    {
        m_omButtonDeleteMsgBlock.EnableWindow(TRUE);
    }
    else
    {
        m_omButtonDeleteMsgBlock.EnableWindow(FALSE);
    }
    // Update 
    CButton* pMonoRadioButton = (CButton*)GetDlgItem(IDC_RADIOMONOSHOT);
    if (pMonoRadioButton != NULL)
    {   
        pMonoRadioButton->EnableWindow(bEnable);
    }
    CButton* pRadioCyclic = (CButton*)GetDlgItem(IDC_RADIOCYCLIC);
    if (pRadioCyclic != NULL)
    {   
        pRadioCyclic->EnableWindow(bEnable);
    }
    //m_omButtonTriggerType.EnableWindow(bEnable);
    m_omEditMsgBlockName.EnableWindow(bEnable);
    // Include Standard/Extended radion and RTR checkbox
    // Standard Radio Button
    pomWnd = GetDlgItem(IDC_RBTN_MSGTYPE_STD);
    if( pomWnd != NULL )
    {
        pomWnd->EnableWindow( bEnable );
    }
    // Extended Radio Button
    pomWnd = GetDlgItem(IDC_RBTN_MSGTYPE_EXTD);
    if( pomWnd != NULL )
    {
        pomWnd->EnableWindow( bEnable );
    }
    // Standard Radio Button
    pomWnd = GetDlgItem(IDC_CHKB_MSGTYPE_RTR);
    if( pomWnd != NULL )
    {
        pomWnd->EnableWindow( bEnable );
    }
    // Standard Radio Button
}


/*******************************************************************************
  Function Name  : pomGetParentWindow
  Input(s)       : -
  Output         : CWnd * - Pointer to CTxMsgChildFrame
  Functionality  : This Function will return parent window pointer. That is
                   pointer to CTxMsgChildFrame. This will return NULL incase of
                   failure
  Member of      : CTxMsgBlocksView
  Author(s)      : Raja N
  Date Created   : 25.4.2005
  Modifications  : 
*******************************************************************************/
CWnd * CTxMsgBlocksView::pomGetParentWindow() const
{
    CWnd * pWnd = NULL;
    // Get Splitter window pointer
    pWnd = GetParent();
    // Get Tx Msg Child Window pointer from Splitter window pointer
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
  Function Name  : pomGetListViewPointer
  Input(s)       : -
  Output         : CWnd * - Pointer to CTxMsgListView or NULL incase of
                   failure
  Functionality  : This function will return CTxMsgListView pointer. This will
                   get child window pointer to get view pointer.
  Member of      : CTxMsgBlocksView
  Author(s)      : Raja N
  Date Created   : 25.4.2005
  Modifications  : 
*******************************************************************************/
CWnd * CTxMsgBlocksView::pomGetListViewPointer() const
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
  Member of      : CTxMsgBlocksView
  Author(s)      : Raja N
  Date Created   : 25.4.2005
  Modifications  : 
*******************************************************************************/
CWnd * CTxMsgBlocksView::pomGetDetailsViewPointer() const
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
  Function Name  : pomGetFunctionsViewPointer
  Input(s)       : -
  Output         : CWnd * - Pointer to CTxFunctionsView or NULL incase of
                   failure
  Functionality  : This function will return CTxFunctionsView pointer. This
                   will get child window pointer to get view pointer.
  Member of      : CTxMsgBlocksView
  Author(s)      : Raja N
  Date Created   : 25.4.2005
  Modifications  : 
*******************************************************************************/
CWnd * CTxMsgBlocksView::pomGetFunctionsViewPointer() const
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
    // Return view pointer
    return pView;
}

/******************************************************************************/
/*  Function Name    :  vSetControlProperties                                 */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function changed the control properties for hex  */
/*                      and decimal settings.                                 */
/*                                                                            */
/*  Member of        :  CTxMsgBlocksView                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  01.08.2004, Added Timer value and Key value controls  */
/******************************************************************************/
void CTxMsgBlocksView::vSetControlProperties()
{
    // Init Time Value Edit control
    m_omEditTrgTimeIntervalVal.vSetBase(BASE_DECIMAL);
    m_omEditTrgTimeIntervalVal.vSetSigned( FALSE );
    m_omEditTrgTimeIntervalVal.LimitText(0);
    // Init Key Value Edit Control
    m_omEditTrgKeyVal.vSetBase( BASE_ALPHANUMERIC);
    m_omEditTrgKeyVal.vSetSigned( FALSE );
    m_omEditTrgKeyVal.SetLimitText(1);
}

/******************************************************************************/
/*  Function Name    :  OnRclickLstcMsgBlocksName                             */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function will call bDisplayPopMenu.              */
/*                                                                            */
/*  Member of        :  CTxMsgBlocksView                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */
/*  Modification By  :                                                        */
/*  Modification on  :                                                        */
/******************************************************************************/
void CTxMsgBlocksView::OnRclickLstcMsgBlocksName(NMHDR* /*pNMHDR*/,
                                                 LRESULT* pResult) 
{
    bDisplayPopMenu(m_omLctrMsgBlockName,IDM_CONFIG_MSG_BLOCK);
    *pResult = 0;
}

/******************************************************************************/
/*  Function Name    :  bDisplayPopMenu                                       */
/*  Input(s)         :                                                        */
/*  Output           :  TRUE or FALSE                                         */
/*  Functionality    :  This function will display pop-up menus if selection  */
/*                      is valid and chooses the appropriate menu for display */
/*                                                                            */
/*  Member of        :  CTxMsgBlocksView                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  08.01.2004                                            */    
/*  Modification By  :  Raja N                                                */
/*  Modification on  :  06.08.2004, Added checks to disable Add and Delete All*/
/*                      Menu Items                                            */
/******************************************************************************/
BOOL CTxMsgBlocksView::bDisplayPopMenu(CListCtrl& omList,UINT nIDResource )
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
                // Disable Add message block item if the block count is
                // >= Max count
                if( m_unMsgBlockCount >= defMAX_MSGBLOCK )
                {
                    pSubMenu->EnableMenuItem( IDM_ADD_MSG_BLOCK,
                                    MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
                }

                if(nIndex == -1 )
                {
                    // if selection is not valid, disble the menu.
                    pSubMenu->EnableMenuItem(IDM_DELETE_SEL_MSG_BLOCK,
                                   MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

                }
                else
                {
                    // If selection is valid and transmission is on. disable
                    // Delete and delete all menu.
                    if(CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG) == TRUE )
                    {
                        pSubMenu->EnableMenuItem(IDM_DELETE_SEL_MSG_BLOCK,
                                    MF_BYCOMMAND | MF_DISABLED | MF_GRAYED );
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
void CTxMsgBlocksView::OnBnClickedRadiomonoshot()
{
    SetDlgItemText(IDC_GROUPBOX_TRIGGER, "Trigger (Monoshot) on event");
    OnChkbTriggerType();
}

void CTxMsgBlocksView::OnBnClickedRadiocyclic()
{   
    SetDlgItemText(IDC_GROUPBOX_TRIGGER, "Trigger (Cyclic) on event");
    OnChkbTriggerType();
}

void CTxMsgBlocksView::OnCbnSelchangeComboMsgs()
{   
    OnChkbTxAllFrame();
}
void CTxMsgBlocksView::vSaveCurrentBlockFirst()
{
    PSMSGBLOCKLIST psCurrentMsgBlock = NULL;
    psCurrentMsgBlock = 
            psGetMsgBlockPointer(m_nSelectedMsgBlockIndex,m_psMsgBlockList);
    if(psCurrentMsgBlock != NULL )
    {
        //check the All Msgs state
        int nCurSel = m_omComboAllMsgs.GetCurSel();
        CString omSelString;
        m_omComboAllMsgs.GetLBText(nCurSel, omSelString);
        if (omSelString == "All")
        {
            psCurrentMsgBlock->m_bTxAllFrame = TRUE;
        }
        else
        {
            psCurrentMsgBlock->m_bTxAllFrame = FALSE;
        }
        
        _tcscpy(psCurrentMsgBlock->m_acStrBlockName, m_omStrMsgBlockName.GetBuffer(MAX_PATH));
        //check the Monoshot radio button state
        BOOL bMonoshot = FALSE;
        CButton *pRadioMonoshot = (CButton*)GetDlgItem(IDC_RADIOMONOSHOT);
        if (pRadioMonoshot != NULL)
        {
        bMonoshot = pRadioMonoshot->GetCheck();
        }
        psCurrentMsgBlock->m_bType = !bMonoshot;//!m_bTriggerType;
        psCurrentMsgBlock->m_ucTrigger = m_nRBTNTriggerType;
        // if the trigger type is on time else it is on key
        if( IS_TIME_TRIGGERED (psCurrentMsgBlock->m_ucTrigger) )
        {
            CHAR* pcChar = NULL;
            LONG lTimeVal = strtol(m_omStrTimeIntervalVal,&pcChar, defBASE_DEC);
            if( lTimeVal > 0 )
            {
                psCurrentMsgBlock->m_unTimeInterval = lTimeVal;
            }
            else
            {
                psCurrentMsgBlock->m_unTimeInterval = defDEFAULT_TIME_VAL;
            }
        }
        else
        {
            psCurrentMsgBlock->m_unTimeInterval = defDEFAULT_TIME_VAL;
        }

        if( IS_KEY_TRIGGERED(psCurrentMsgBlock->m_ucTrigger) )
        {
            if(m_omStrKeyVal.IsEmpty() == FALSE)
            {
                psCurrentMsgBlock->m_ucKeyValue     = m_omStrKeyVal.GetAt(0);
            }
            else
            {
                psCurrentMsgBlock->m_ucKeyValue = defDEFAULT_KEY_VAL;
            }
        }
    }
}