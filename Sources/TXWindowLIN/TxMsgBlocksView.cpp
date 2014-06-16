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

#include "TxWindow_stdafx.h"            // For standard includes
#include "Utility/SignalMatrix.h"       // For Signal Matrix Class Definition
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
#include "Utility\MultiLanguageSupport.h"
//#include "../Application/GettextBusmaster.h"

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
    m_omStrMsgBlockName = "";
    m_omStrTimeIntervalVal = "";
    m_omStrKeyVal = "";
    //m_bIsMonoshot = FALSE;
    //m_bTriggerType = FALSE;
    //}}AFX_DATA_INIT
    // Number of message blocks in the Tx List
    m_unMsgBlockCount = 0;
    // Tx Message List Pointer
    m_psMsgBlockList = nullptr;
    // Selected Message Block index
    m_nSelectedMsgBlockIndex = -1;
    m_bMsgBlockDeleted = FALSE;

    m_bInitDlg = FALSE;
    m_nRBTNTriggerType = 0;
    m_bModified = FALSE;
    m_bNewBlock = false;
    m_bListItemChange = false;

    // Set the time trigger value as unsigned since time cannot be negative
    m_omTimeDelayBtwnBlocks.vSetSigned(false);
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
    if(m_psMsgBlockList != nullptr )
    {
        CTxWndDataStore::ouGetTxWndDataStoreObj().vReleaseMultiMsgInfo( m_psMsgBlockList );
        m_psMsgBlockList = nullptr;
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
    //DDX_Control(pDX, IDC_CHECK_MSG_BLOCK_DELAY, m_omDelayBtwnBlocks);
    //DDX_Control(pDX, IDC_EDIT_BLOCK_TRG_TIMER_VAL, m_omTimeDelayBtwnBlocks);
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
    ON_BN_CLICKED(IDC_RADIOMONOSHOT, OnBnClickedRadiomonoshot)
    ON_BN_CLICKED(IDC_RADIOCYCLIC, OnBnClickedRadiocyclic)
    ON_CBN_SELCHANGE(IDC_COMBO_MSGS, OnCbnSelchangeComboMsgs)
    ON_BN_CLICKED(IDC_CHECK_MSG_BLOCK_DELAY, OnBnClickedCheckMsgBlockDelay)
    ON_EN_UPDATE(IDC_EDIT_BLOCK_TRG_TIMER_VAL, OnEnUpdateEditBlockTrgTimerVal)
    ON_EN_KILLFOCUS(IDC_EDIT_MSG_BLOCK_NAME, /*&CTxMsgBlocksView::*/AutoUpdateChanges)
    ON_EN_KILLFOCUS(IDC_EDIT_TRG_TIME_VAL, /*&CTxMsgBlocksView::*/AutoUpdateChanges)
    ON_EN_KILLFOCUS(IDC_EDIT_TRG_KEY_VAL, /*&CTxMsgBlocksView::*/AutoUpdateChanges)
    ON_EN_KILLFOCUS(IDC_EDIT_BLOCK_TRG_TIMER_VAL, /*&CTxMsgBlocksView::*/AutoUpdateChanges)
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
    CWnd* pWnd = nullptr;
    // Get Parent window pointer
    pWnd = pomGetParentWindow();
    if( pWnd != nullptr )
    {
        // Register view pointer
        ((CTxMsgChildFrame*)pWnd)->vSetTxMsgViewPointers( eTxMsgBlocksView,
                this );
    }
    // Get Message List View Pointer
    CTxMsgListView* pomListView = nullptr;
    pomListView = (CTxMsgListView*)pomGetListViewPointer();
    // Populate List Control
    CHAR caColumnName[defMESSAGE_FRAME_COLUMN][defSTRING_SIZE] =
    {
        defMESSAGE_BLOCK_NAME,
        defMESSAGE_BLOCK_TRIGGER,
        defMESSAGE_BLOCK_TRIG_VAL,
        defMESSAGE_DATA_BYTES
    };
    RECT rListCtrlRect;
    INT nTotalColunmSize = 0;
    INT nTotalStrLengthPixel = 0;
    INT nColumnSize = 0;
    //Calculate the total size of all column header
    m_omLctrMsgBlockName.GetWindowRect( &rListCtrlRect);
    nTotalColunmSize     = rListCtrlRect.right - rListCtrlRect.left;
    nTotalStrLengthPixel = 0;
    UINT i;     //i declared outside loop
    for(i=0; i<defMESSAGE_BLOCK_COLUMN; i++)
    {
        nTotalStrLengthPixel +=
            m_omLctrMsgBlockName.GetStringWidth(_(caColumnName[i])) ;
    }
    //Insert each column name after calculating the size for the same.
    for(i=0; i<defMESSAGE_BLOCK_COLUMN; i++)
    {
        nColumnSize  = m_omLctrMsgBlockName.GetStringWidth(_(caColumnName[i])) ;
        nColumnSize +=
            (nTotalColunmSize-nTotalStrLengthPixel)/defMESSAGE_BLOCK_COLUMN;
        m_omLctrMsgBlockName.InsertColumn(i,_(caColumnName[i]),
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
        m_nSelectedMsgBlockIndex = 0;
    }

    // Diable controls if there is no message blocks
    if(m_unMsgBlockCount == 0)
    {
        vEnableControls(FALSE);
    }
    // Update Message List view
    if( pomListView != nullptr )
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

    PSMSGBLOCKLIST psCurrentMsgBlock = nullptr;
    CString omStr = "";
    psCurrentMsgBlock = m_psMsgBlockList;

    // Disable AllTxMessage check box
    // This will be updated in the function vDisplayMsgBlockDetails
    // Disabling it here will avoid enabling the control when no blocks or
    // messages are present
    m_omComboAllMsgs.InsertString(0, _("All"));
    m_omComboAllMsgs.InsertString(1, _("Single"));
    m_omComboAllMsgs.SelectString(0, _("Single"));
    m_omComboAllMsgs.EnableWindow( FALSE );
    /*m_omButtonTxAllFrame.EnableWindow( FALSE );*/

    for(i = 0; i<m_unMsgBlockCount; i++)
    {
        if(psCurrentMsgBlock != nullptr )
        {
            m_omLctrMsgBlockName.InsertItem( i,
                                             psCurrentMsgBlock->m_acStrBlockName);
            if(psCurrentMsgBlock->m_bType == TRUE)
            {
                m_omLctrMsgBlockName.SetItemText( i,defSUBITEM_MSGBLOCK_TYPE,
                                                  _(defMSG_CYCLIC));
            }
            else
            {
                m_omLctrMsgBlockName.SetItemText( i,defSUBITEM_MSGBLOCK_TYPE,
                                                  _(defMSG_MONOSHOT));
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
                m_omComboAllMsgs.EnableWindow();
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
    // Set the focus to the first entry
    m_omLctrMsgBlockName.SetItemState(0,LVIS_SELECTED|LVIS_FOCUSED,
                                      LVIS_SELECTED|LVIS_FOCUSED);
    // Disable Update Button as this is the initial view
    CTxFunctionsView* pomFunctionsView = (CTxFunctionsView*)
                                         pomGetFunctionsViewPointer();
    if( pomFunctionsView != nullptr )
    {
        if(pomFunctionsView->m_CheckBoxAutoUpdate.GetCheck() == BST_UNCHECKED)
        {
            pomFunctionsView->m_omButtonApply.EnableWindow( FALSE );
        }
    }
    //Initiate All msgs combo box
    /*m_omComboAllMsgs.AddString("Single");
    m_omComboAllMsgs.AddString("All");
    m_omComboAllMsgs.SelectString(0, "Single");
    m_omComboAllMsgs.EnableWindow(FALSE);*/
    // AUC - changes for delay between blocks **************************************
    CString         csData;
    csData.Format("%ld", CTxMsgManager::s_unTimeDelayBtnMsgBlocks);
    m_omTimeDelayBtwnBlocks.SetWindowTextA(csData);
    m_bDelayBtnBlocks = CTxMsgManager::s_bDelayBetweenBlocksOnly;
    if(CTxMsgManager::s_bDelayBetweenBlocksOnly)
    {
        m_omDelayBtwnBlocks.SetCheck(BST_CHECKED);

        OnBnClickedCheckMsgBlockDelay();
    }
    else
    {
        m_omDelayBtwnBlocks.SetCheck(BST_UNCHECKED);
        m_omTimeDelayBtwnBlocks.EnableWindow(FALSE);
    }
    if(CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG) == TRUE)
    {
        m_omButtonAddMsgBlock.EnableWindow(false);
        m_omDelayBtwnBlocks.EnableWindow(false);
        m_omTimeDelayBtwnBlocks.EnableWindow(false);
        m_omEditMsgBlockName.EnableWindow(false);
    }
    //(GetDlgItem(IDC_EDIT_BLOCK_TRG_TIMER_VAL))->EnableWindow(FALSE);
    //******************************************************************************
    // Set Init flag to false
    m_bInitDlg = FALSE;
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
    PSMSGBLOCKLIST psCurrentMsgBlock  = nullptr;
    BOOL bAdd = TRUE;
    // Get Message List View Pointer
    CTxMsgListView* pomListView = nullptr;
    pomListView = (CTxMsgListView*)pomGetListViewPointer();
    // Call function to add message block
    bAdd = bAddBlock(psCurrentMsgBlock);
    // Once add is successfull, the message details will be updated in control
    // fields.
    if(psCurrentMsgBlock != nullptr && bAdd == TRUE)
    {
        // Check/Uncheck based on Tx status
        if( FALSE == CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG) )
        {
            psCurrentMsgBlock->m_bActive = TRUE;
        }

        CString omStr = "";

        if( pomListView != nullptr )
        {
            pomListView->m_omLctrMsgList.DeleteAllItems();
        }
        m_unMsgBlockCount++;

        INT nTotalCount = m_omLctrMsgBlockName.GetItemCount();
        psCurrentMsgBlock->m_unIndex = nTotalCount;
        psCurrentMsgBlock->m_bModified = true;
        m_bNewBlock = true;
        m_bInitDlg = TRUE;
        // Insert the new Item
        m_omLctrMsgBlockName.InsertItem(nTotalCount,
                                        psCurrentMsgBlock->m_acStrBlockName);
        m_omLctrMsgBlockName.SetItemText(nTotalCount,
                                         defSUBITEM_MSGBLOCK_TYPE,
                                         _(defMSG_CYCLIC));
        // Update Checkbox status
        m_omLctrMsgBlockName.SetCheck( nTotalCount,
                                       psCurrentMsgBlock->m_bActive );
        //Update cyclic radio button
        CButton* pRadioCyclic = (CButton*)GetDlgItem(IDC_RADIOCYCLIC);
        if (pRadioCyclic != nullptr)
        {
            pRadioCyclic->SetCheck(psCurrentMsgBlock->m_bType);
        }
        // Assign the local variable too
        m_nRBTNTriggerType = psCurrentMsgBlock->m_ucTrigger;

        omStr.Format( defSTR_TIMER_VAL_FMT_SPECIFIER,
                      psCurrentMsgBlock->m_unTimeInterval );
        omStr += defMESSAGE_TRIG_MS;
        if(CTxWndDataStore::ouGetTxWndDataStoreObj().m_bDelayBetweenMsgBlocks)
        {
            if(m_omButtonAddMsgBlock.IsWindowEnabled() == TRUE)
            {
                m_omTimeDelayBtwnBlocks.EnableWindow(TRUE);
            }
            omStr = "";    //don't display if delay between block is checked
        }
        m_omLctrMsgBlockName.SetItemText(nTotalCount,
                                         defSUBITEM_MSGBLOCK_TRG_VAL,
                                         omStr);
        m_bInitDlg = FALSE;
        // Selection will be shifted to at the newly added message block in
        // list.
        m_omLctrMsgBlockName.SetItemState(nTotalCount,
                                          LVIS_SELECTED|LVIS_FOCUSED,
                                          LVIS_SELECTED|LVIS_FOCUSED);
        CTxFunctionsView* pView =
            (CTxFunctionsView* )pomGetFunctionsViewPointer();
        if( pView != nullptr )
        {
            if(pView->m_CheckBoxAutoUpdate.GetCheck() == BST_UNCHECKED)
            {
                pView->m_omButtonApply.EnableWindow(TRUE);
            }
        }
        if(m_unMsgBlockCount >= defMAX_MSGBLOCK )
        {
            m_omButtonAddMsgBlock.EnableWindow(FALSE);
        }

        //update the global list for storing the changed data
        AutoUpdateChanges();
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
BOOL CTxMsgBlocksView::bAddBlock(SMSGBLOCKLIST*& psMsgCurrentBlock)
{
    BOOL bReturn              = TRUE;
    PSMSGBLOCKLIST psMsgBlock = new SMSGBLOCKLIST;
    PSMSGBLOCKLIST psMsgNextBlock = nullptr;
    // Check if memory allocation is successful.
    if(psMsgBlock != nullptr )
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
        if(psMsgNextBlock != nullptr )
        {
            do
            {
                if(psMsgNextBlock->m_psNextMsgBlocksList == nullptr )
                {
                    psMsgNextBlock->m_psNextMsgBlocksList = psMsgBlock;
                    psMsgNextBlock = psMsgBlock->m_psNextMsgBlocksList;
                }
                else
                {
                    psMsgNextBlock = psMsgNextBlock->m_psNextMsgBlocksList;
                }
            }
            while(psMsgNextBlock != nullptr );
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
    if(psMsgBlockList != nullptr )
    {
        psMsgBlockList->m_psNextMsgBlocksList = nullptr;
        psMsgBlockList->m_unMsgCount          = 0;
        psMsgBlockList->m_bActive             = FALSE;
        psMsgBlockList->m_bType               = TRUE;
        psMsgBlockList->m_bTxAllFrame         = FALSE;
        psMsgBlockList->m_ucTrigger           = defTIME_TRIGGER;
        psMsgBlockList->m_psTxLINMsgList      = nullptr;
        psMsgBlockList->m_ucKeyValue          = defDEFAULT_KEY_VAL;
        psMsgBlockList->m_unTimeInterval      = defDEFAULT_TIME_VAL;
        strcpy_s( psMsgBlockList->m_acStrBlockName, defBLOCKNAME_SIZE, _(defDEFAULT_MSG_BLOCK_NAME));
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
    if ( AfxMessageBox( _(defDEL_MSG_BLOCK),
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
                    // And block value control needs to be disabled
                    m_omTimeDelayBtwnBlocks.EnableWindow(FALSE);

                    vEnableControls(FALSE);
                    // Disable Signal List and related component components
                    CTxMsgDetailsView* pView = (CTxMsgDetailsView* )
                                               pomGetDetailsViewPointer();
                    if( pView != nullptr )
                    {
                        pView->vDisableSignalComponents();
                        pView->vSetValues(nullptr);
                    }
                }

                CTxFunctionsView* pView = (CTxFunctionsView* )
                                          pomGetFunctionsViewPointer();
                if( pView != nullptr )
                {
                    if(pView->m_CheckBoxAutoUpdate.GetCheck() == BST_UNCHECKED)
                    {
                        pView->m_omButtonApply.EnableWindow(TRUE);
                    }
                }
                // Update Modified Flag
                m_bModified = TRUE;

                //update the global list for storing the changed data
                AutoUpdateChanges();
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
    CTxMsgListView* pomListView = (CTxMsgListView* )pomGetListViewPointer();
    if( pomListView != nullptr )
    {
        bReturn = TRUE;

        PSMSGBLOCKLIST psMsgCurrentBlock = nullptr;
        PSMSGBLOCKLIST psDelMsgBlock     = nullptr;
        psDelMsgBlock =
            psGetMsgBlockPointer(m_nSelectedMsgBlockIndex,m_psMsgBlockList);
        bReturn = pomListView->bDeleteAllMsgFromBlock(psDelMsgBlock);
        if(bReturn == TRUE )
        {
            if(psDelMsgBlock != nullptr)
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
                    if(psMsgCurrentBlock != nullptr )
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
                    psDelMsgBlock = nullptr;
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
    if(psMsgBlockList != nullptr && nIndexCurBlock != -1)
    {
        INT nIndex = 0;
        // Get the message block pointer in the list at the index
        // nIndexCurBlock
        while(nIndex != nIndexCurBlock)
        {
            if( psMsgBlockList != nullptr )
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
        PSMSGBLOCKLIST psMsgBlock = nullptr ;
        // Get Message List View Pointer
        CTxMsgListView* pomListView = nullptr;
        pomListView = (CTxMsgListView*)pomGetListViewPointer();
        CTxFunctionsView* pomFunctionView = nullptr;
        pomFunctionView = (CTxFunctionsView*)pomGetFunctionsViewPointer();
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
                //update the global list for storing the changed data
                UpdateList(pNMListView);                //AUC
                m_bListItemChange = true;
                m_nSelectedMsgBlockIndex = pNMListView->iItem;
                psMsgBlock = psGetMsgBlockPointer(pNMListView->iItem,
                                                  m_psMsgBlockList);
                if( pomListView != nullptr )
                {
                    pomListView->m_omLctrMsgList.DeleteAllItems();
                    pomListView->m_omButtonSendMsg.EnableWindow(FALSE);
                    pomListView->m_nSelectedMsgIndex = -1;
                }
                if(psMsgBlock != nullptr )
                {

                    vDisplayMsgBlockDetails(psMsgBlock);
                    vEnableControls(TRUE);
                    // Enavle Add Button
                    CTxMsgDetailsView* pView = (CTxMsgDetailsView* )
                                               pomGetDetailsViewPointer();
                    if( pView != nullptr )
                    {
                        pView->vEnableAddButton( TRUE );
                    }
                    if( pomListView != nullptr )
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
                        //AUC
                        if(m_omDelayBtwnBlocks.GetCheck() == BST_CHECKED)
                        {
                            m_omButtonTimeTrigger.SetCheck(BST_UNCHECKED);
                            m_omEditTrgTimeIntervalVal.EnableWindow(FALSE);
                            //OnChkbOnTimeTrigger();
                            m_omButtonTimeTrigger.EnableWindow(FALSE);
                        }
                        //update monoshot and cyclic radio button
                        CButton* pMonoRadioButton = (CButton*)GetDlgItem(IDC_RADIOMONOSHOT);
                        if (pMonoRadioButton != nullptr)
                        {
                            pMonoRadioButton->EnableWindow(!CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG));
                        }
                        CButton* pRadioCyclic = (CButton*)GetDlgItem(IDC_RADIOCYCLIC);
                        if (pRadioCyclic != nullptr)
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
                if( pomFunctionView != nullptr )
                {
                    if(pomFunctionView->m_CheckBoxAutoUpdate.GetCheck() == BST_UNCHECKED)
                    {
                        pomFunctionView->m_omButtonApply.EnableWindow(TRUE);
                    }
                }

                //update the global list for storing the changed data
                psMsgBlock = psGetMsgBlockPointer(pNMListView->iItem,
                                                  m_psMsgBlockList);
                if(psMsgBlock)
                {
                    psMsgBlock->m_bModified = true;
                }
                AutoUpdateChanges();

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
            psMsgBlock = nullptr ;
            psMsgBlock = psGetMsgBlockPointer(pNMListView->iItem,
                                              m_psMsgBlockList);
            // Save the changes if items is deselected and not deleted.
            if(psMsgBlock != nullptr && m_bMsgBlockDeleted == FALSE )
            {
                // vUpdateMsgBlockDetials(psMsgBlock);
                m_omButtonDeleteMsgBlock.EnableWindow(FALSE);
                //Disable Delete All Button
                if( pomListView != nullptr )
                {
                    pomListView->m_omButtonDeleteAllMsg.EnableWindow(FALSE);
                }
            }
        }
    }
    *pResult = 0;
}

void CTxMsgBlocksView::UpdateList(NM_LISTVIEW* /* pNMListView */)
{
    PSMSGBLOCKLIST psMsgBlock = nullptr ;
    for(int nItem =0 ; nItem < m_omLctrMsgBlockName.GetItemCount(); nItem++)
    {
        CString        csName = m_omLctrMsgBlockName.GetItemText(nItem, 0);
        psMsgBlock = psGetMsgBlockPointer(nItem,m_psMsgBlockList);
        if(psMsgBlock == nullptr)
        {
            return;
        }
        if(strcmp(psMsgBlock->m_acStrBlockName, csName.GetBuffer(0)) != 0)
        {
            strcpy(psMsgBlock->m_acStrBlockName, csName.GetBuffer(0));
        }
    }
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
void CTxMsgBlocksView::vDisplayMsgBlockDetails(SMSGBLOCKLIST* psMsgBlock)
{
    if(psMsgBlock != nullptr )
    {
        m_omStrMsgBlockName = psMsgBlock->m_acStrBlockName;
        m_omEditMsgBlockName.SetWindowText(m_omStrMsgBlockName);
        // TRUE -> CYCLIC so uncheck the check box "Monoshot"
        // FALSE -> MONOSHOT so check the check box "Monoshot"
        CButton* pRadioCyclic = (CButton*)GetDlgItem(IDC_RADIOCYCLIC);
        CButton* pRadioMonoShot = (CButton*)GetDlgItem(IDC_RADIOMONOSHOT);
        CButton* pRadioSlave = (CButton*)GetDlgItem(IDC_RADIO_SLAVE);

        if ((pRadioSlave != nullptr))
        {
            pRadioSlave->SetCheck(BST_CHECKED);
        }

        if ((pRadioCyclic != nullptr) && (pRadioMonoShot != nullptr))
        {
            pRadioCyclic->SetCheck(psMsgBlock->m_bType);
            pRadioMonoShot->SetCheck(!psMsgBlock->m_bType);
        }
        if (psMsgBlock->m_bType == TRUE)
        {
            SetDlgItemText(IDC_GROUPBOX_TRIGGER, _("Trigger (Cyclic) on event"));
        }
        else
        {
            SetDlgItemText(IDC_GROUPBOX_TRIGGER, _("Trigger (Monoshot) on event"));
        }
        //m_omButtonTriggerType.SetCheck(!psMsgBlock->m_bType);

        if (psMsgBlock->m_bTxAllFrame)
        {
            m_omComboAllMsgs.SelectString(0, _("All"));
        }
        else
        {
            m_omComboAllMsgs.SelectString(0, _("Single"));
        }
        /*m_omButtonTxAllFrame.SetCheck(psMsgBlock->m_bTxAllFrame);*/
        if(m_omDelayBtwnBlocks.GetCheck() == BST_UNCHECKED) //AUC
        {
            m_nRBTNTriggerType = psMsgBlock->m_ucTrigger;
        }
        // Update Time Trigger Values
        if(IS_TIME_TRIGGERED( psMsgBlock->m_ucTrigger ))
        {
            CButton* pomOnTime = (CButton*)GetDlgItem(IDC_CHKB_ON_TIME_TRIGGER);
            if( pomOnTime != nullptr )
            {
                if( !CTxWndDataStore::ouGetTxWndDataStoreObj().m_bDelayBetweenMsgBlocks)
                {
                    pomOnTime->SetCheck( TRUE );
                }
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
            if( pomOnTime != nullptr )
            {
                pomOnTime->SetCheck( FALSE );
            }
            //if(m_omDelayBtwnBlocks.GetCheck() == BST_UNCHECKED) //AUC
            {
                CString strTrgVal = defDEFAULT_TIME_INTERVAL;
                UINT unDefTimeIntr = atol(defDEFAULT_TIME_INTERVAL);
                if ( psMsgBlock->m_unTimeInterval != unDefTimeIntr)
                {
                    strTrgVal.Format( defSTR_TIMER_VAL_FMT_SPECIFIER,
                                      psMsgBlock->m_unTimeInterval);
                }
                m_omEditTrgTimeIntervalVal.SetWindowText(strTrgVal);
            }
            m_omEditTrgTimeIntervalVal.EnableWindow( FALSE );
        }

        // Update Key Trigger Values
        if( IS_KEY_TRIGGERED( psMsgBlock->m_ucTrigger ) )
        {
            CButton* pomOnTime = (CButton*)GetDlgItem(IDC_CHKB_ON_KEY_TRIGGER);
            if( pomOnTime != nullptr )
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
            if( pomOnTime != nullptr )
            {
                pomOnTime->SetCheck( FALSE );
            }
            m_omComboAllMsgs.SelectString(0, _("Single"));
            m_omComboAllMsgs.EnableWindow(FALSE);
            /*m_omButtonTxAllFrame.SetCheck( FALSE );
            /*m_omButtonTxAllFrame.EnableWindow( FALSE );*/
            CString omStrCurrent = defDEFAULT_KEY_VAL_STR;
            if (psMsgBlock->m_ucKeyValue != defDEFAULT_KEY_VAL)
            {
                omStrCurrent = psMsgBlock->m_ucKeyValue;
            }

            m_omEditTrgKeyVal.SetWindowText(omStrCurrent);
            m_omEditTrgKeyVal.EnableWindow( FALSE );
        }

        // Update message details list for currently selected message
        // block.
        if(psMsgBlock->m_unMsgCount > 0)
        {
            int  nChecked= 0;
            int  nUnCkecked = 0;
            CTxMsgListView* pomListView = ( CTxMsgListView* )
                                          pomGetListViewPointer();
            if( pomListView != nullptr )
            {
                PSTXLINMSGLIST psTxMsgList = nullptr;
                psTxMsgList = pomListView->psGetMsgDetailPointer(0,psMsgBlock);
                for(UINT i =0; i<psMsgBlock->m_unMsgCount; i++)
                {
                    if(psTxMsgList != nullptr )
                    {
                        pomListView->vUpdateMsgListDisplay(
                            psTxMsgList->m_sTxMsgDetails,-1 );
                        psTxMsgList = psTxMsgList->m_psNextMsgDetails;
                    }
                    //added by ashwin for checkin the header listctrl of list view
                    if(psMsgBlock->m_psTxLINMsgList->m_sTxMsgDetails.m_bEnabled == TRUE)
                    {
                        nChecked++;
                    }
                    else if(psMsgBlock->m_psTxLINMsgList->m_sTxMsgDetails.m_bEnabled == FALSE)
                    {
                        nUnCkecked++;
                    }
                }
                if(nChecked == psMsgBlock->m_unMsgCount)
                {
                    pomListView->vCheckHeaderCtrl(true);
                }
                else if(nUnCkecked == psMsgBlock->m_unMsgCount)
                {
                    pomListView->vCheckHeaderCtrl(false);
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
void CTxMsgBlocksView::vUpdateMsgBlockDetials(SMSGBLOCKLIST* psCurrentMsgBlock)
{
    UpdateData(TRUE);
    if(psCurrentMsgBlock != nullptr )
    {
        //check the All Msgs state
        int nCurSel = m_omComboAllMsgs.GetCurSel();
        CString omSelString;
        m_omComboAllMsgs.GetLBText(nCurSel, omSelString);
        if (omSelString == _("All"))
        {
            psCurrentMsgBlock->m_bTxAllFrame = TRUE;
        }
        else
        {
            psCurrentMsgBlock->m_bTxAllFrame = FALSE;
        }

        strcpy_s(psCurrentMsgBlock->m_acStrBlockName, defBLOCKNAME_SIZE, m_omStrMsgBlockName.GetBuffer(MAX_PATH));
        //check the Monoshot radio button state
        BOOL bMonoshot = FALSE;
        CButton* pRadioMonoshot = (CButton*)GetDlgItem(IDC_RADIOMONOSHOT);

        if (pRadioMonoshot != nullptr)
        {
            bMonoshot = pRadioMonoshot->GetCheck();
        }

        psCurrentMsgBlock->m_bType = !bMonoshot;//!m_bTriggerType;
        if(m_omDelayBtwnBlocks.GetCheck() == BST_UNCHECKED) //AUC
        {
            psCurrentMsgBlock->m_ucTrigger = m_nRBTNTriggerType;
        }

        // if the trigger type is on time else it is on key
        if( IS_TIME_TRIGGERED (psCurrentMsgBlock->m_ucTrigger) )
        {
            CHAR* pcChar = nullptr;
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
            if (psCurrentMsgBlock->m_unTimeInterval == defDEFAULT_TIME_VAL)
            {
                psCurrentMsgBlock->m_unTimeInterval = defDEFAULT_TIME_VAL;
            }
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
    CString omStrMsgBlockName = "";
    UpdateData(TRUE);
    // Update the item in message block list control having selections.
    m_omLctrMsgBlockName.SetItemText(m_nSelectedMsgBlockIndex,
                                     defMAIN_ITEM,
                                     m_omStrMsgBlockName);
    CTxFunctionsView* pomFunctionView = ( CTxFunctionsView*)
                                        pomGetFunctionsViewPointer();
    if(m_bListItemChange == FALSE && m_bInitDlg == FALSE)           //if the function is not called from list item change only then save the changes
    {
        if( pomFunctionView != nullptr )
        {
            if(pomFunctionView->m_CheckBoxAutoUpdate.GetCheck() == BST_UNCHECKED)
            {
                pomFunctionView->m_omButtonApply.EnableWindow(TRUE);
            }
        }
        PSMSGBLOCKLIST psMsgCurrentBlock =
            psGetMsgBlockPointer(m_nSelectedMsgBlockIndex,m_psMsgBlockList);
        if(psMsgCurrentBlock != nullptr)
        {
            if(strcmp(psMsgCurrentBlock->m_acStrBlockName,m_omStrMsgBlockName.GetBuffer(0)) != 0)
            {
                psMsgCurrentBlock->m_bModified = true;
                strcpy(psMsgCurrentBlock->m_acStrBlockName, m_omStrMsgBlockName.GetBuffer(0));
            }
        }
    }
    else
    {
        m_bListItemChange = false;      //reset the flag to false
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
        if (pButton != nullptr)
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
                                             _(defMSG_CYCLIC));
        }
        else
        {
            m_omLctrMsgBlockName.SetItemText(m_nSelectedMsgBlockIndex,
                                             defSUBITEM_MSGBLOCK_TYPE,
                                             defMSG_MONOSHOT);
        }
        CTxFunctionsView* pomFunctionView = ( CTxFunctionsView*)
                                            pomGetFunctionsViewPointer();
        if( pomFunctionView != nullptr )
        {
            if(pomFunctionView->m_CheckBoxAutoUpdate.GetCheck() == BST_UNCHECKED)
            {
                pomFunctionView->m_omButtonApply.EnableWindow(TRUE);
            }
        }
        // Update Modified Flag
        PSMSGBLOCKLIST psMsgCurrentBlock = nullptr;
        psMsgCurrentBlock =
            psGetMsgBlockPointer(m_nSelectedMsgBlockIndex,m_psMsgBlockList);
        if (psMsgCurrentBlock != nullptr)
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
    PSMSGBLOCKLIST psMsgCurrentBlock = nullptr;
    psMsgCurrentBlock =
        psGetMsgBlockPointer(m_nSelectedMsgBlockIndex,m_psMsgBlockList);
    if (psMsgCurrentBlock != nullptr)
    {
        psMsgCurrentBlock->m_ucTrigger ^= defTIME_TRIGGER;
        if( IS_TIME_TRIGGERED( psMsgCurrentBlock->m_ucTrigger ))
        {
            m_omEditTrgTimeIntervalVal.EnableWindow();
            omStrCurrent = m_omStrTimeIntervalVal + defMESSAGE_TRIG_MS;
            if (psMsgCurrentBlock != nullptr)
            {
                psMsgCurrentBlock->m_ucTrigger |= defTIME_TRIGGER;
            }
            // Add "|" only if key is enabled
            // This check we cannot prevent!!
            if( IS_KEY_TRIGGERED (  psMsgCurrentBlock->m_ucTrigger ) )
            {
                omStrCurrent += defSTR_MSG_BLOCK_TRIGGER_SEPRATOR;
                /*if (psMsgCurrentBlock != nullptr)
                {
                    psMsgCurrentBlock->m_ucTrigger |= defKEY_TRIGGER;
                }*/
            }
        }
        else
        {
            // Disable Related Controls
            m_omEditTrgTimeIntervalVal.EnableWindow( FALSE );
            if (psMsgCurrentBlock != nullptr)
            {
                psMsgCurrentBlock->m_ucTrigger &= MASK_DISABLE_TIME_TRIGGER;
            }
        }
        // If Key is also enabled then add Key value
        if(  IS_KEY_TRIGGERED (  psMsgCurrentBlock->m_ucTrigger ) )
        {
            omStrCurrent += m_omStrKeyVal;
            /*if (psMsgCurrentBlock != nullptr)
            {
                psMsgCurrentBlock->m_ucTrigger |= defKEY_TRIGGER;
            }*/
        }
    }
    // update the value in list control for message block having current
    // selection
    m_omLctrMsgBlockName.SetItemText( m_nSelectedMsgBlockIndex,
                                      defSUBITEM_MSGBLOCK_TRG_VAL,
                                      omStrCurrent );
    CTxFunctionsView* pomFunctionView = ( CTxFunctionsView*)
                                        pomGetFunctionsViewPointer();
    if( pomFunctionView != nullptr )
    {
        if(pomFunctionView->m_CheckBoxAutoUpdate.GetCheck() == BST_UNCHECKED)
        {
            pomFunctionView->m_omButtonApply.EnableWindow(TRUE);
        }
    }

    // Update Modified Flag

    if( psMsgCurrentBlock->m_bActive == TRUE )
    {
        m_bModified = TRUE;
    }
    if(psMsgCurrentBlock)
    {
        psMsgCurrentBlock->m_bModified = true;
    }
    //update the global list for storing the changed data
    AutoUpdateChanges();
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
    PSMSGBLOCKLIST psMsgCurrentBlock = nullptr;
    psMsgCurrentBlock =
        psGetMsgBlockPointer(m_nSelectedMsgBlockIndex,m_psMsgBlockList);
    psMsgCurrentBlock->m_ucTrigger ^= defKEY_TRIGGER;
    // If Timer is also enabled then add Time value in to list item text
    if( IS_TIME_TRIGGERED ( /*m_nRBTNTriggerType*/ psMsgCurrentBlock->m_ucTrigger) )
    {
        if(CTxWndDataStore::ouGetTxWndDataStoreObj().m_bDelayBetweenMsgBlocks)
        {
            omStrCurrent = "";    //don't display if delay between block is checked
        }
        else
        {
            omStrCurrent = m_omStrTimeIntervalVal + defMESSAGE_TRIG_MS;
            // Add "|" only if key is enabled
            // This check we cannot prevent!!
            /*if (psMsgCurrentBlock != nullptr)
            {
                psMsgCurrentBlock->m_ucTrigger |= defTIME_TRIGGER;
            }*/
            if( IS_KEY_TRIGGERED( /*m_nRBTNTriggerType*/ psMsgCurrentBlock->m_ucTrigger) )
            {
                omStrCurrent += defSTR_MSG_BLOCK_TRIGGER_SEPRATOR;
            }
        }
    }

    if( IS_KEY_TRIGGERED( /*m_nRBTNTriggerType*/ psMsgCurrentBlock->m_ucTrigger))
    {
        m_omComboAllMsgs.EnableWindow();
        /*m_omButtonTxAllFrame.EnableWindow();*/
        m_omEditTrgKeyVal.EnableWindow();
        omStrCurrent += m_omStrKeyVal;
        /*if (psMsgCurrentBlock != nullptr)
        {
            psMsgCurrentBlock->m_ucTrigger |= defKEY_TRIGGER;
        }*/
    }
    else
    {
        /*m_omButtonTxAllFrame.EnableWindow( FALSE );*/
        m_omComboAllMsgs.EnableWindow(FALSE);
        m_omEditTrgKeyVal.EnableWindow( FALSE );
        /* if (psMsgCurrentBlock != nullptr)
         {
             psMsgCurrentBlock->m_ucTrigger &= MASK_DISABLE_KEY_TRIGGER;
         }*/
    }
    // update the value in list control for message block having current
    // selection
    m_omLctrMsgBlockName.SetItemText( m_nSelectedMsgBlockIndex,
                                      defSUBITEM_MSGBLOCK_TRG_VAL,
                                      omStrCurrent );
    CTxFunctionsView* pomFunctionView = ( CTxFunctionsView*)
                                        pomGetFunctionsViewPointer();
    if( pomFunctionView != nullptr )
    {
        if(pomFunctionView->m_CheckBoxAutoUpdate.GetCheck() == BST_UNCHECKED)
        {
            pomFunctionView->m_omButtonApply.EnableWindow(TRUE);
        }
    }

    // Update Modified Flag

    if( psMsgCurrentBlock != nullptr && psMsgCurrentBlock->m_bActive == TRUE )
    {
        m_bModified = TRUE;
    }

    //update the global list for storing the changed data
    if (psMsgCurrentBlock != nullptr)
    {
        psMsgCurrentBlock->m_bModified = true;
    }

    AutoUpdateChanges();
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
    CString omStrCurrent       = "";
    CHAR* pcChar               = nullptr;
    m_omEditTrgTimeIntervalVal.GetWindowText(omStrCurrent);
    // Get the time value
    unTimeInterVal = static_cast<UINT>(strtol(omStrCurrent,&pcChar,BASE_10));
    // If it is valid
    PSMSGBLOCKLIST psMsgCurrentBlock =
        psGetMsgBlockPointer(m_nSelectedMsgBlockIndex,m_psMsgBlockList);
    if(unTimeInterVal > defMAX_TIME_DELAY)
    {
        AfxMessageBox("Time Interval cannot be more than 60000 milliseconds");
        // Update the repvious valid value
        //m_omStrTimeIntervalVal = m_omStrTimeIntervalVal.Left(m_omStrTimeIntervalVal.GetLength()-1);
        m_omStrTimeIntervalVal.Format("%d", m_nTimeDelMsg);
        m_omEditTrgTimeIntervalVal.SetWindowText(m_omStrTimeIntervalVal);
        m_omEditTrgTimeIntervalVal.SetSel( 1,m_omStrTimeIntervalVal.GetLength() );
        return;
    }
    if(unTimeInterVal > 0 )
    {
        // Update the time valie
        m_omStrTimeIntervalVal = omStrCurrent;
        // Enable apply window
        CTxFunctionsView* pomFunctionView = ( CTxFunctionsView*)
                                            pomGetFunctionsViewPointer();
        if( pomFunctionView != nullptr )
        {
            if(pomFunctionView->m_CheckBoxAutoUpdate.GetCheck() == BST_UNCHECKED)
            {
                pomFunctionView->m_omButtonApply.EnableWindow(TRUE);
            }
        }

        // Check whether it is enabled and then
        // Add Unit to the time value
        omStrCurrent = "";
        if( IS_TIME_TRIGGERED ( psMsgCurrentBlock->m_ucTrigger ) )
        {
            omStrCurrent = m_omStrTimeIntervalVal + defMESSAGE_TRIG_MS;
            if( IS_KEY_TRIGGERED (  psMsgCurrentBlock->m_ucTrigger ) )
            {
                omStrCurrent += defSTR_MSG_BLOCK_TRIGGER_SEPRATOR;
            }
            // update this information into the block
            CHAR* pcChar = nullptr;
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
        if( IS_KEY_TRIGGERED ( psMsgCurrentBlock->m_ucTrigger  )  )
        {
            //omStrCurrent += m_omStrKeyVal;
            if(psMsgCurrentBlock != nullptr)
            {
                m_omStrKeyVal.Format( defSTR_KEY_VAL_FMT_SPECIFIER,
                                      psMsgCurrentBlock->m_ucKeyValue );
            }
            if(CTxWndDataStore::ouGetTxWndDataStoreObj().m_bDelayBetweenMsgBlocks)
            {
                omStrCurrent = m_omStrKeyVal;    //don't display if delay between block is checked
            }
            else
            {
                omStrCurrent += m_omStrKeyVal;
            }
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
        CWnd* pomWnd = nullptr;
        pomWnd = GetFocus();
        if( pomWnd != nullptr &&
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
        m_omEditTrgTimeIntervalVal.SetSel( 0,m_omStrTimeIntervalVal.GetLength() );
    }
    if(psMsgCurrentBlock)
    {
        UINT unCurrTimeInterval = atoi(m_omStrTimeIntervalVal.GetBuffer(0));
        if(psMsgCurrentBlock->m_unTimeInterval != unCurrTimeInterval)
        {
            psMsgCurrentBlock->m_bModified = true;
        }
    }
    int nCurrSel = m_omLctrMsgBlockName.GetSelectionMark();
    //if(!m_bNewBlock && !CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG))        //call update only for editing and not while adding a new block
    //if Tx is on dont save
    //    AutoUpdateChanges();
    //}
    m_bNewBlock = false;
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
    CString omStrCurrent = "";
    // Get the Key Value
    m_omEditTrgKeyVal.GetWindowText(omStrCurrent);
    unTextLength = omStrCurrent.GetLength();
    // If it is valid
    if(unTextLength  == 1 )
    {
        PSMSGBLOCKLIST psMsgBlock = psGetMsgBlockPointer(m_nSelectedMsgBlockIndex,
                                    m_psMsgBlockList);
        // Update the value and enable Apply Button
        if (psMsgBlock->m_ucKeyValue != defDEFAULT_KEY_VAL &&
                psMsgBlock->m_ucKeyValue == omStrCurrent)
        {
            omStrCurrent = psMsgBlock->m_ucKeyValue;
        }
        m_omStrKeyVal = omStrCurrent;
        // Enable Apply Button
        CTxFunctionsView* pomFunctionView = ( CTxFunctionsView*)
                                            pomGetFunctionsViewPointer();
        if( pomFunctionView != nullptr )
        {
            if(pomFunctionView->m_CheckBoxAutoUpdate.GetCheck() == BST_UNCHECKED)
            {
                pomFunctionView->m_omButtonApply.EnableWindow(TRUE);
            }
        }

        // Check whether it is enabled and then
        // Add Unit to the time value
        omStrCurrent = "";
        if( IS_TIME_TRIGGERED ( psMsgBlock->m_ucTrigger ) )
        {
            if(CTxMsgManager::s_bDelayBetweenBlocksOnly)
            {
                omStrCurrent = "";
            }
            else
            {
                omStrCurrent = m_omStrTimeIntervalVal + defMESSAGE_TRIG_MS;
            }
            if( IS_KEY_TRIGGERED (  psMsgBlock->m_ucTrigger ) )
            {
                omStrCurrent += defSTR_MSG_BLOCK_TRIGGER_SEPRATOR;
            }
        }
        // If Key is also enabled then add key part
        if( IS_KEY_TRIGGERED ( psMsgBlock->m_ucTrigger  )  )
        {

            psMsgBlock->m_ucKeyValue = m_omStrKeyVal.GetAt(0);
            if(CTxWndDataStore::ouGetTxWndDataStoreObj().m_bDelayBetweenMsgBlocks)
            {
                omStrCurrent = m_omStrKeyVal;    //don't display if delay between block is checked
            }
            else
            {
                omStrCurrent += m_omStrKeyVal;
            }
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
        CWnd* pomWnd = nullptr;
        pomWnd = GetFocus();
        // If the focus is in the Edit box then proceed
        if( pomWnd != nullptr &&
                (pomWnd->m_hWnd == m_omEditTrgTimeIntervalVal.m_hWnd ||
                 pomWnd->m_hWnd == m_omEditTrgKeyVal.m_hWnd))
        {
            //PSMSGBLOCKLIST psMsgCurrentBlock = nullptr;
            //psMsgCurrentBlock =
            //    psGetMsgBlockPointer(m_nSelectedMsgBlockIndex,m_psMsgBlockList);
            if( psMsgBlock->m_bActive == TRUE )
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
    CTxFunctionsView* pomFunctionView = ( CTxFunctionsView*)
                                        pomGetFunctionsViewPointer();
    if( pomFunctionView != nullptr )
    {
        if(pomFunctionView->m_CheckBoxAutoUpdate.GetCheck() == BST_UNCHECKED)
        {
            pomFunctionView->m_omButtonApply.EnableWindow(TRUE);
        }
    }
    // Update Modified Flag
    PSMSGBLOCKLIST psMsgCurrentBlock = nullptr;
    psMsgCurrentBlock =
        psGetMsgBlockPointer(m_nSelectedMsgBlockIndex,m_psMsgBlockList);
    if (psMsgCurrentBlock != nullptr)
    {
        int nCutSel = m_omComboAllMsgs.GetCurSel();
        CString omSelString;
        m_omComboAllMsgs.GetLBText(nCutSel, omSelString);
        if (omSelString == _("All"))
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
        if(psMsgCurrentBlock)
        {
            psMsgCurrentBlock->m_bModified = true;
        }
        AutoUpdateChanges();
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
    CWnd* pomWnd = nullptr;
    /*CButton* pomOnTime = (CButton*)GetDlgItem(IDC_RBTN_ON_TIME);
    if(pomOnTime != nullptr )
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
    if(CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG) == TRUE)
    {
        m_omEditTrgTimeIntervalVal.EnableWindow( FALSE );
    }
    if(CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG) == FALSE)
    {
        m_omDelayBtwnBlocks.EnableWindow(bEnable);
    }
    // Key edit box
    // Check whether Key is enabled or not
    if(CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG) == FALSE)
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
    CTxMsgDetailsView* pomDetailsView = ( CTxMsgDetailsView* )
                                        pomGetDetailsViewPointer();
    if( pomDetailsView != nullptr )
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
        //      pomDetailsView->m_omComboChannelID.EnableWindow(bEnable);
        //      // Include Standard/Extended radion
        //      // Header Radio Button
        //      pomWnd = pomDetailsView->GetDlgItem(IDC_RBTN_MSGTYPE_HDR);
        //      if( pomWnd != nullptr )
        //      {
        //          pomWnd->EnableWindow( bEnable );
        //      }
        //      // Response Radio Button
        //      pomWnd = pomDetailsView->GetDlgItem(IDC_RBTN_MSGTYPE_RESP);
        //      if( pomWnd != nullptr )
        //      {
        //          pomWnd->EnableWindow( bEnable );
        //      }

        //// Include Checksum radion
        //      // Classic Checksum Radio Button
        //      pomWnd = pomDetailsView->GetDlgItem(IDC_RBTN_MSGTYPE_CLASSIC);
        //      if( pomWnd != nullptr )
        //      {
        //          pomWnd->EnableWindow( bEnable );
        //      }
        //      // Enhanced Checksum Radio Button
        //      pomWnd = pomDetailsView->GetDlgItem(IDC_RBTN_MSGTYPE_ENHANCED);
        //      if( pomWnd != nullptr )
        //      {
        //          pomWnd->EnableWindow( bEnable );
        //      }

    }
    // Update List View
    CTxMsgListView* pomListView = ( CTxMsgListView* )
                                  pomGetListViewPointer();
    if( pomListView != nullptr )
    {
        // Message List control
        pomListView->m_omLctrMsgList.EnableWindow(bEnable);
    }

    // Timer checkbox
    m_omButtonTimeTrigger.EnableWindow(bEnable);
    if(CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG) == TRUE)
    {
        m_omButtonTimeTrigger.EnableWindow(FALSE);
    }
    // Key Checkbox
    m_omButtonKeyTrigger.EnableWindow(bEnable);
    if(CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG) == TRUE)
    {
        m_omButtonKeyTrigger.EnableWindow(FALSE);
    }
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

    if(bEnable == TRUE && CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG) == FALSE )
    {
        m_omButtonDeleteMsgBlock.EnableWindow(TRUE);
        m_omComboAllMsgs.EnableWindow( bChkEnable );
    }
    else
    {
        m_omButtonDeleteMsgBlock.EnableWindow(FALSE);
    }
    // Update
    CButton* pMonoRadioButton = (CButton*)GetDlgItem(IDC_RADIOMONOSHOT);
    if (pMonoRadioButton != nullptr)
    {
        pMonoRadioButton->EnableWindow(bEnable);
        if(CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG) == TRUE)
        {
            pMonoRadioButton->EnableWindow(FALSE);
        }
    }
    CButton* pRadioCyclic = (CButton*)GetDlgItem(IDC_RADIOCYCLIC);
    if (pRadioCyclic != nullptr)
    {
        pRadioCyclic->EnableWindow(bEnable);
        if(CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG) == TRUE)
        {
            pRadioCyclic->EnableWindow(FALSE);
        }
    }
    //m_omButtonTriggerType.EnableWindow(bEnable);
    if(CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG) == FALSE)
    {
        m_omEditMsgBlockName.EnableWindow(bEnable);
    }
    // Include Standard/Extended radion
    // Header Radio Button
    pomWnd = pomDetailsView->GetDlgItem(IDC_RBTN_MSGTYPE_HDR);
    if( pomWnd != nullptr )
    {
        pomWnd->EnableWindow( bEnable );
    }
    // Response Radio Button
    pomWnd = pomDetailsView->GetDlgItem(IDC_RBTN_MSGTYPE_RESP);
    if( pomWnd != nullptr )
    {
        pomWnd->EnableWindow( bEnable );
    }


    // Include Checksum radion
    // Classic Checksum Radio Button
    pomWnd = pomDetailsView->GetDlgItem(IDC_RBTN_MSGTYPE_CLASSIC);
    if( pomWnd != nullptr )
    {
        pomWnd->EnableWindow( bEnable );
    }
    // Enhanced Checksum Radio Button
    pomWnd = pomDetailsView->GetDlgItem(IDC_RBTN_MSGTYPE_ENHANCED);
    if( pomWnd != nullptr )
    {
        pomWnd->EnableWindow( bEnable );
    }
}


/*******************************************************************************
  Function Name  : pomGetParentWindow
  Input(s)       : -
  Output         : CWnd * - Pointer to CTxMsgChildFrame
  Functionality  : This Function will return parent window pointer. That is
                   pointer to CTxMsgChildFrame. This will return nullptr incase of
                   failure
  Member of      : CTxMsgBlocksView
  Author(s)      : Raja N
  Date Created   : 25.4.2005
  Modifications  :
*******************************************************************************/
CWnd* CTxMsgBlocksView::pomGetParentWindow() const
{
    CWnd* pWnd = nullptr;
    // Get Splitter window pointer
    pWnd = GetParent();
    // Get Tx Msg Child Window pointer from Splitter window pointer
    if( pWnd != nullptr )
    {
        pWnd = pWnd->GetParent();
    }
    if( pWnd != nullptr )
    {
        pWnd = pWnd->GetParent();
    }

    // Return Tx Msg Child window pointer or nullptr incase of failure
    return pWnd;
}

/*******************************************************************************
  Function Name  : pomGetListViewPointer
  Input(s)       : -
  Output         : CWnd * - Pointer to CTxMsgListView or nullptr incase of
                   failure
  Functionality  : This function will return CTxMsgListView pointer. This will
                   get child window pointer to get view pointer.
  Member of      : CTxMsgBlocksView
  Author(s)      : Raja N
  Date Created   : 25.4.2005
  Modifications  :
*******************************************************************************/
CWnd* CTxMsgBlocksView::pomGetListViewPointer() const
{
    CWnd* pView = nullptr;
    // Get Child Frame Pointer
    CWnd* pWnd = nullptr;
    pWnd = pomGetParentWindow();
    // Get View Pointer
    if( pWnd != nullptr )
    {
        pView = ((CTxMsgChildFrame*)pWnd)->pomGetTxMsgViewPointers(
                    eTxMsgMessageListView );
    }
    // Return view pointer
    return pView;
}

/*******************************************************************************
  Function Name  : pomGetDetailsViewPointer
  Input(s)       : -
  Output         : CWnd * - Pointer to CTxMsgDetailsView or nullptr incase of
                   failure
  Functionality  : This function will return CTxMsgDetailsView pointer. This
                   will get child window pointer to get view pointer.
  Member of      : CTxMsgBlocksView
  Author(s)      : Raja N
  Date Created   : 25.4.2005
  Modifications  :
*******************************************************************************/
CWnd* CTxMsgBlocksView::pomGetDetailsViewPointer() const
{
    CWnd* pView = nullptr;
    // Get Child Frame Pointer
    CWnd* pWnd = nullptr;
    pWnd = pomGetParentWindow();
    // Get View Pointer
    if( pWnd != nullptr )
    {
        pView = ((CTxMsgChildFrame*)pWnd)->pomGetTxMsgViewPointers(
                    eTxMsgMessageDetailsView );
    }
    // Return view pointer
    return pView;
}

/*******************************************************************************
  Function Name  : pomGetFunctionsViewPointer
  Input(s)       : -
  Output         : CWnd * - Pointer to CTxFunctionsView or nullptr incase of
                   failure
  Functionality  : This function will return CTxFunctionsView pointer. This
                   will get child window pointer to get view pointer.
  Member of      : CTxMsgBlocksView
  Author(s)      : Raja N
  Date Created   : 25.4.2005
  Modifications  :
*******************************************************************************/
CWnd* CTxMsgBlocksView::pomGetFunctionsViewPointer() const
{
    CWnd* pView = nullptr;
    // Get Child Frame Pointer
    CWnd* pWnd = nullptr;
    pWnd = pomGetParentWindow();
    // Get View Pointer
    if( pWnd != nullptr )
    {
        pView = ((CTxMsgChildFrame*)pWnd)->pomGetTxMsgViewPointers(
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
    if ( pMainMenu != nullptr )
    {
        // Load menu
        if ( pMainMenu->LoadMenu( nIDResource ))
        {
            // Get submenu
            CMenu* pSubMenu = pMainMenu->GetSubMenu( 0 );
            // Assert
            if ( pSubMenu != nullptr )
            {
                POINT point;
                // Get cursor position wrt screen co-ord
                GetCursorPos(&point);
                pSubMenu->EnableMenuItem(IDC_RADIO_MASTER, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
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
                if(CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG) == TRUE )
                {
                    pSubMenu->EnableMenuItem( IDM_ADD_MSG_BLOCK,
                                              MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
                }
                // Display menu
                pSubMenu->TrackPopupMenu(
                    TPM_LEFTALIGN |TPM_RIGHTBUTTON,
                    point.x,
                    point.y,
                    this,
                    nullptr);
                // Clean up
                pMainMenu->Detach();
                pMainMenu->DestroyMenu();
                delete pMainMenu;
                pMainMenu = nullptr;
                bReturn = TRUE;
            }
        }
    }
    return bReturn;
}
void CTxMsgBlocksView::OnBnClickedRadiomonoshot()
{
    SetDlgItemText(IDC_GROUPBOX_TRIGGER, _("Trigger (Monoshot) on event"));
    OnChkbTriggerType();
    //update the global list for storing the changed data
    PSMSGBLOCKLIST psMsgCurrentBlock = nullptr;
    psMsgCurrentBlock =
        psGetMsgBlockPointer(m_nSelectedMsgBlockIndex,m_psMsgBlockList);
    if(psMsgCurrentBlock)
    {
        psMsgCurrentBlock->m_bModified = true;
    }
    AutoUpdateChanges();
}

void CTxMsgBlocksView::OnBnClickedRadiocyclic()
{
    SetDlgItemText(IDC_GROUPBOX_TRIGGER, _("Trigger (Cyclic) on event"));
    OnChkbTriggerType();
    //update the global list for storing the changed data
    PSMSGBLOCKLIST psMsgCurrentBlock = nullptr;
    psMsgCurrentBlock =
        psGetMsgBlockPointer(m_nSelectedMsgBlockIndex,m_psMsgBlockList);
    if(psMsgCurrentBlock)
    {
        psMsgCurrentBlock->m_bModified = true;
    }
    AutoUpdateChanges();
}

void CTxMsgBlocksView::OnCbnSelchangeComboMsgs()
{
    OnChkbTxAllFrame();
}
void CTxMsgBlocksView::vSaveCurrentBlockFirst()
{
    PSMSGBLOCKLIST psCurrentMsgBlock = nullptr;
    psCurrentMsgBlock =
        psGetMsgBlockPointer(m_nSelectedMsgBlockIndex,m_psMsgBlockList);
    if(psCurrentMsgBlock != nullptr )
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

        strcpy_s(psCurrentMsgBlock->m_acStrBlockName, defBLOCKNAME_SIZE, m_omStrMsgBlockName.GetBuffer(MAX_PATH));
        //check the Monoshot radio button state
        BOOL bMonoshot = FALSE;
        CButton* pRadioMonoshot = (CButton*)GetDlgItem(IDC_RADIOMONOSHOT);
        if (pRadioMonoshot != nullptr)
        {
            bMonoshot = pRadioMonoshot->GetCheck();
        }
        psCurrentMsgBlock->m_bType = !bMonoshot;//!m_bTriggerType;
        psCurrentMsgBlock->m_ucTrigger = m_nRBTNTriggerType;
        // if the trigger type is on time else it is on key
        if( IS_TIME_TRIGGERED (psCurrentMsgBlock->m_ucTrigger) )
        {
            CHAR* pcChar = nullptr;
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
        if(psCurrentMsgBlock)
        {
            psCurrentMsgBlock->m_bModified = true;
        }
    }

    //update the global list for storing the changed data
    AutoUpdateChanges();
}

/******************************************************************************/
/*  Function Name    :  OnEnKillfocusEditMsgBlockName                         */
/*  Input(s)         :                                                        */
/*  Output           :  void                                                  */
/*  Functionality    :  This function will be called when any of the EditBox  */
/*                      in this view looses focus. We need to update the      */
/*                      global structure each time                            */
/*  Member of        :  CTxMsgBlocksView                                      */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Ashwin. R.Uchil                                       */
/*  Date Created     :  25-5-2012                                             */
/*                                                                            */
/******************************************************************************/
void CTxMsgBlocksView::AutoUpdateChanges()
{
    CTxFunctionsView* pomFunctionView = ( CTxFunctionsView*)
                                        pomGetFunctionsViewPointer();

    if (nullptr != pomFunctionView)
    {
        if(pomFunctionView->m_CheckBoxAutoUpdate.GetCheck() == BST_CHECKED)
        {
            pomFunctionView->vAccessButtonApply();
        }

        CWnd* pWnd = GetFocus();
        if (nullptr != pWnd)
        {
            pWnd->SetFocus();
        }
    }

    // Store the previous valid values of the time delay between messages and blocks
    // This will be used to set as default values when invalid values are given as input
    CString omStrCurrent;
    CHAR* pcChar = nullptr;
    UINT unTimeVal = 0;
    m_omEditTrgTimeIntervalVal.GetWindowText(omStrCurrent);
    unTimeVal = static_cast<UINT>(strtol(omStrCurrent,&pcChar,BASE_10));
    if (unTimeVal <= 60000)
    {
        m_nTimeDelMsg = unTimeVal;
    }

    GetDlgItemText(IDC_EDIT_BLOCK_TRG_TIMER_VAL, omStrCurrent);
    unTimeVal = (UINT)atol(omStrCurrent.GetBuffer(0));
    if (unTimeVal <= 60000)
    {
        m_nTimeDelBlocks = unTimeVal;
    }
}
void CTxMsgBlocksView::OnBnClickedCheckMsgBlockDelay()
{
    CTxFunctionsView* pomFunctionView = ( CTxFunctionsView*)
                                        pomGetFunctionsViewPointer();
    int nCurrentSel = m_omLctrMsgBlockName.GetSelectionMark();
    if(m_omDelayBtwnBlocks.GetCheck() == BST_CHECKED)
    {
        CString                 omStrKeyVal;
        PSMSGBLOCKLIST          psMsgBlock = nullptr;

        ((CButton*)GetDlgItem(IDC_CHKB_ON_TIME_TRIGGER))->SetCheck(BST_UNCHECKED);
        (GetDlgItem(IDC_CHKB_ON_TIME_TRIGGER))->EnableWindow(FALSE);
        (GetDlgItem(IDC_EDIT_TRG_TIME_VAL))->EnableWindow(FALSE);
        m_omTimeDelayBtwnBlocks.EnableWindow(TRUE);
        m_bDelayBtnBlocks = true;
        if(m_omDelayBtwnBlocks.IsWindowEnabled() == FALSE)
        {
            m_omTimeDelayBtwnBlocks.EnableWindow(FALSE);
        }
        CString csData;
        m_omTimeDelayBtwnBlocks.GetWindowTextA(csData);
        if(csData == "")
        {
            m_omTimeDelayBtwnBlocks.SetWindowTextA("100");
        }

        if(CTxWndDataStore::ouGetTxWndDataStoreObj().m_bAutoSavedEnabled)
        {
            UINT                    unTimerVal;
            CString                 csTimerVal;
            GetDlgItemText(IDC_EDIT_BLOCK_TRG_TIMER_VAL, csTimerVal);
            unTimerVal = (UINT)atol(csTimerVal.GetBuffer(0));
            CTxWndDataStore::ouGetTxWndDataStoreObj().m_unTimeDelayBtwnMsgBlocks = unTimerVal;
            CTxMsgManager::s_unTimeDelayBtnMsgBlocks = unTimerVal;
            CTxWndDataStore::ouGetTxWndDataStoreObj().m_bDelayBetweenMsgBlocks = true;  //update the global variable
            CTxMsgManager::s_bDelayBetweenBlocksOnly = true;
        }
        for(int nCnt =0; nCnt < m_omLctrMsgBlockName.GetItemCount(); nCnt++)
        {
            psMsgBlock = psGetMsgBlockPointer(nCnt, m_psMsgBlockList);
            if( IS_KEY_TRIGGERED(psMsgBlock->m_ucTrigger) )             //if key triggered then display the key else nullptr
            {
                omStrKeyVal.Format( defSTR_KEY_VAL_FMT_SPECIFIER,
                                    psMsgBlock->m_ucKeyValue );
            }
            else
            {
                omStrKeyVal = "";
            }
            m_omLctrMsgBlockName.SetItemText(nCnt,defSUBITEM_MSGBLOCK_TRG_VAL,
                                             omStrKeyVal);
        }
        psMsgBlock = psGetMsgBlockPointer(m_nSelectedMsgBlockIndex, m_psMsgBlockList);
        if(psMsgBlock != nullptr)
        {
            if( IS_TIME_TRIGGERED(psMsgBlock->m_ucTrigger) )
            {
                m_nRBTNTriggerType &= MASK_DISABLE_TIME_TRIGGER;
            }
        }
    }
    else if(m_omDelayBtwnBlocks.GetCheck() == BST_UNCHECKED)
    {
        CString                 omStr;
        PSMSGBLOCKLIST          psMsgBlock;

        m_omTimeDelayBtwnBlocks.EnableWindow(FALSE);
        (GetDlgItem(IDC_CHKB_ON_TIME_TRIGGER))->EnableWindow(TRUE);
        m_bDelayBtnBlocks = false;

        if(CTxWndDataStore::ouGetTxWndDataStoreObj().m_bAutoSavedEnabled)
        {
            CTxWndDataStore::ouGetTxWndDataStoreObj().m_bDelayBetweenMsgBlocks = false;
            CTxMsgManager::s_bDelayBetweenBlocksOnly = false;
        }
        for(int nCnt = 0; nCnt < m_omLctrMsgBlockName.GetItemCount(); nCnt++)
        {
            psMsgBlock = psGetMsgBlockPointer(nCnt, m_psMsgBlockList);
            if(psMsgBlock == nullptr)
            {
                break;
            }
            if( IS_TIME_TRIGGERED(psMsgBlock->m_ucTrigger) )
            {
                omStr.Format( defSTR_TIMER_VAL_FMT_SPECIFIER,
                              psMsgBlock->m_unTimeInterval );
                omStr += defMESSAGE_TRIG_MS;
                (GetDlgItem(IDC_EDIT_TRG_TIME_VAL))->EnableWindow(TRUE);
                if( IS_KEY_TRIGGERED(psMsgBlock->m_ucTrigger) )
                {
                    omStr += defSTR_MSG_BLOCK_TRIGGER_SEPRATOR;
                }
            }
            else
            {
                omStr = "";
                /*omStr.Format( defSTR_TIMER_VAL_FMT_SPECIFIER,
                              psMsgBlock->m_unTimeInterval );*/
                // Disable the time trigger value edit control if the check box is unchecked
                (GetDlgItem(IDC_EDIT_TRG_TIME_VAL))->EnableWindow(FALSE);

            }
            if( IS_KEY_TRIGGERED(psMsgBlock->m_ucTrigger) )
            {
                CString omStrKeyVal;
                omStrKeyVal.Format( defSTR_KEY_VAL_FMT_SPECIFIER,
                                    psMsgBlock->m_ucKeyValue );
                omStr += omStrKeyVal;
            }
            m_omLctrMsgBlockName.SetItemText(nCnt,defSUBITEM_MSGBLOCK_TRG_VAL,
                                             omStr);
        }
        psMsgBlock = psGetMsgBlockPointer(m_nSelectedMsgBlockIndex, m_psMsgBlockList);
        if(psMsgBlock != nullptr)
        {
            if( IS_TIME_TRIGGERED(psMsgBlock->m_ucTrigger) )
            {
                ((CButton*)GetDlgItem(IDC_CHKB_ON_TIME_TRIGGER))->SetCheck(BST_CHECKED);
                m_nRBTNTriggerType |= defTIME_TRIGGER;    //set this to status of surrent clicked block
            }
        }
    }

    if(pomFunctionView != nullptr)
    {
        if(pomFunctionView->m_CheckBoxAutoUpdate.GetCheck() == BST_UNCHECKED)
        {
            pomFunctionView->m_omButtonApply.EnableWindow(TRUE);
        }
    }
}
void CTxMsgBlocksView::OnEnUpdateEditBlockTrgTimerVal()
{
    UINT                    unTimerVal;
    CString                 csTimerVal;
    GetDlgItemText(IDC_EDIT_BLOCK_TRG_TIMER_VAL, csTimerVal);
    unTimerVal = (UINT)atol(csTimerVal.GetBuffer(0));

    CTxFunctionsView* pomFunctionView = ( CTxFunctionsView*)
                                        pomGetFunctionsViewPointer();
    if(unTimerVal > defMAX_TIME_DELAY)
    {
        AfxMessageBox("Block interval cannot be more than 60000 milliseconds");
        // Update the repvious valid value
        CString omstrTime;
        omstrTime.Format("%d", m_nTimeDelBlocks);
        /*omstrTime.Format("%d", CTxWndDataStore::ouGetTxWndDataStoreObj().m_unTimeDelayBtwnMsgBlocks/10);*/
        SetDlgItemText(IDC_EDIT_BLOCK_TRG_TIMER_VAL, omstrTime);
        ((CEdit*)GetDlgItem(IDC_EDIT_BLOCK_TRG_TIMER_VAL))->SetSel(1,omstrTime.GetLength());
        return;
    }
    if(unTimerVal <=0)
    {
        unTimerVal = atoi(m_omStrTimeIntervalVal.GetBuffer(0));
        if(unTimerVal == 0)
        {
            unTimerVal = 0;
            m_omStrTimeIntervalVal = "100";
        }
        SetDlgItemText(IDC_EDIT_BLOCK_TRG_TIMER_VAL, m_omStrTimeIntervalVal);
        m_omTimeDelayBtwnBlocks.SetSel( 0,m_omStrTimeIntervalVal.GetLength() );
    }
    if(CTxWndDataStore::ouGetTxWndDataStoreObj().m_bAutoSavedEnabled)
    {
        CTxWndDataStore::ouGetTxWndDataStoreObj().m_unTimeDelayBtwnMsgBlocks = unTimerVal;
        CTxMsgManager::s_unTimeDelayBtnMsgBlocks = unTimerVal;
        //AutoUpdateChanges();
    }
    m_omStrTimeIntervalVal.Format("%u", unTimerVal);

    if(pomFunctionView != nullptr)
    {
        if(pomFunctionView->m_CheckBoxAutoUpdate.GetCheck() == BST_UNCHECKED)
        {
            pomFunctionView->m_omButtonApply.EnableWindow(TRUE);
        }
    }
}