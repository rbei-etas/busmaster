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
 * @brief Implementation file for CTxFunctionsView class
 * @author Raja N
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CTxFunctionsView class
 */

#include "TxWindow_stdafx.h"             // For Standard Includes
#include "Utility/SignalMatrix.h"       // For Signal Matrix Class Definition
#include "Utility/ComboItem.h"          // For Custom Combobox Implementation
#include "Utility/EditItem.h"           // For Custom Editbox Implementation
#include "Utility/RadixEdit.h"          // For the RAdix Edit control definition
#include "Utility/NumSpinCtrl.h"        // For the custom spin control
#include "Utility/NumEdit.h"            // For Custom Numeric Edit control Impl
#include "Utility/FFListctrl.h"         // For Flicker Free List class definition
#include "FlexListCtrl.h"       // For editable list control implementation
#include "TxFunctionsView.h"    // For Tx Function View class declaration
#include "TxMsgChildFrame.h"    // For Parent Window class declaration
#include "Utility/MultiLanguageSupport.h"

IMPLEMENT_DYNCREATE(CTxFunctionsView, CFormView)

CTxFunctionsView::CTxFunctionsView()
    : CFormView(CTxFunctionsView::IDD)
{
}

CTxFunctionsView::~CTxFunctionsView()
{
}

void CTxFunctionsView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BTN_UPDATE, m_omButtonApply);
    DDX_Control(pDX, IDC_CHECK_AUTO_UPDATE, m_CheckBoxAutoUpdate);
}

BEGIN_MESSAGE_MAP(CTxFunctionsView, CFormView)
    ON_WM_SIZE()
    ON_BN_CLICKED(IDC_BTN_UPDATE, OnButtonApply)
    ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
    ON_BN_CLICKED(IDC_CHECK_AUTO_UPDATE, OnBnClickedCheckAutoUpdate)
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

void CTxFunctionsView::OnSize(UINT nType, int cx, int cy)
{
    CFormView::OnSize(nType, cx, cy);
    const double dOffset = 0.75;

    // Get Update Window pointer
    CWnd* pWnd = GetDlgItem( IDC_BTN_UPDATE );
    // If valid
    if( pWnd != nullptr )
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
    if( pWnd != nullptr )
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

void CTxFunctionsView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();

    // Update View pointer in to the child frame
    // Get Child Window Pointer
    CWnd* pWnd = nullptr;

    // Get Parent window pointer
    pWnd = pomGetParentWindow();
    // Update view pointer
    if( pWnd != nullptr )
    {
        // Register this view pointer
        ((CTxMsgChildFrame*)pWnd)->vSetTxMsgViewPointers( eTxMsgFunctionsView,
                this );
    }

    // Disable Update Button
    m_omButtonApply.EnableWindow( FALSE );

    if(CTxWndDataStore::ouGetTxWndDataStoreObj().m_bAutoSavedEnabled)
    {
        m_CheckBoxAutoUpdate.SetCheck(BST_CHECKED);
        GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(FALSE);
        OnBnClickedCheckAutoUpdate();                   //call this function exclusively
    }
    else
    {
        m_CheckBoxAutoUpdate.SetCheck(BST_UNCHECKED);
        GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(TRUE);
    }
}

void CTxFunctionsView::OnButtonApply()
{
    SetUpdateBtnChanges();
    //save the values of delay between blocks
    UINT                    unTimerVal;
    CString                 csTimerVal;
    CTxMsgBlocksView* pomBlockView = nullptr;
    pomBlockView = (CTxMsgBlocksView*)pomGetBlocksViewPointer();
    if(pomBlockView == nullptr)
    {
        return;
    }
    if(pomBlockView)
    {
        pomBlockView->GetDlgItemText(IDC_EDIT_BLOCK_TRG_TIMER_VAL, csTimerVal);

        if(((CButton*) pomBlockView->GetDlgItem(IDC_CHECK_MSG_BLOCK_DELAY))->GetCheck() == BST_CHECKED)
        {
            CTxWndDataStore::ouGetTxWndDataStoreObj().m_bDelayBetweenMsgBlocks = true;
            CTxMsgManager::s_bDelayBetweenBlocksOnly = true;
            pomBlockView->m_bDelayBtnBlocks = true;
        }
        else
        {
            CTxWndDataStore::ouGetTxWndDataStoreObj().m_bDelayBetweenMsgBlocks = false;
            CTxMsgManager::s_bDelayBetweenBlocksOnly = false;
            pomBlockView->m_bDelayBtnBlocks = false;
        }
    }

    unTimerVal = (UINT)atol(csTimerVal.GetBuffer(0));

    CTxWndDataStore::ouGetTxWndDataStoreObj().m_unTimeDelayBtwnMsgBlocks = unTimerVal;
    CTxMsgManager::s_unTimeDelayBtnMsgBlocks = unTimerVal;
    this->SetFocus();
}

void CTxFunctionsView::SetUpdateBtnChanges()
{
    // Apply changed to global Tx Block
    // User is trying to apply late changes
    // Prevent this as some message/block could be deleted after
    // starting the transmission.

    // Get Other view pointers
    CTxMsgBlocksView* pomBlockView = nullptr;
    pomBlockView = (CTxMsgBlocksView*)pomGetBlocksViewPointer();
    if( pomBlockView != nullptr )
    {
        // Message deletion is not allowed runtime from any block. This is only for Update button Scenario
        if((m_CheckBoxAutoUpdate.GetCheck() == BST_UNCHECKED))
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
                m_omButtonApply.EnableWindow(FALSE);
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

void CTxFunctionsView::vApplyChanges()
{
    // Get Other view pointers
    CTxMsgBlocksView* pomBlockView = nullptr;
    CTxMsgListView* pomListView = nullptr;
    pomBlockView = (CTxMsgBlocksView*)pomGetBlocksViewPointer();
    pomListView = (CTxMsgListView* )pomGetListViewPointer();

    if( pomBlockView != nullptr && pomListView != nullptr )
    {
        PSMSGBLOCKLIST psMsgBlock = nullptr;
        psMsgBlock = pomBlockView->psGetMsgBlockPointer(
                         pomBlockView->m_nSelectedMsgBlockIndex,
                         pomBlockView->m_psMsgBlockList );

        // Update the last selected message block details in case user has
        // modified it.
        if(psMsgBlock != nullptr )
        {
            pomBlockView->vUpdateMsgBlockDetials(psMsgBlock);
            psMsgBlock->m_bModified = true;
        }

        // Set the value in configuration class.
        CTxWndDataStore::ouGetTxWndDataStoreObj().bSetTxData( TX_MSG_BLOCK_COUNT,
                (void*)&(pomBlockView->m_unMsgBlockCount) );

        // Data is set only if messagae block count is more then zero.
        if( pomBlockView->m_unMsgBlockCount > 0 )
        {
            CTxWndDataStore::ouGetTxWndDataStoreObj().bSetTxData( TX_MSG_UPDATE,
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
        if( pTxWnd != nullptr )
        {
            // Register this view pointer
            pTxWnd->vUpdateWndCo_Ords();

        }
        if(pomBlockView->m_omDelayBtwnBlocks.GetCheck() == BST_CHECKED)
        {
            CTxWndDataStore::ouGetTxWndDataStoreObj().m_bDelayBetweenMsgBlocks = true;  //update the global variable
            CTxMsgManager::s_bDelayBetweenBlocksOnly = true;

            UINT                    unTimerVal;
            CString                 csTimerVal;
            pomBlockView->GetDlgItemText(IDC_EDIT_BLOCK_TRG_TIMER_VAL, csTimerVal);
            unTimerVal = (UINT)atol(csTimerVal.GetBuffer(0));

            CTxWndDataStore::ouGetTxWndDataStoreObj().m_unTimeDelayBtwnMsgBlocks = unTimerVal;
            CTxMsgManager::s_unTimeDelayBtnMsgBlocks = unTimerVal;
        }
        else
        {
            CTxWndDataStore::ouGetTxWndDataStoreObj().m_bDelayBetweenMsgBlocks = false;
            CTxMsgManager::s_bDelayBetweenBlocksOnly = false;
        }
    }
}

CWnd* CTxFunctionsView::pomGetParentWindow() const
{
    CWnd* pWnd = nullptr;

    // Get Splitter window pointer
    pWnd = GetParent();

    // Get Tx Msg Child Window pointer from Splitter window pointer
    if( pWnd != nullptr )
    {
        pWnd = pWnd->GetParent();
    }

    // Return Tx Msg Child window pointer or nullptr incase of failure
    return pWnd;
}

CWnd* CTxFunctionsView::pomGetBlocksViewPointer() const
{
    CWnd* pView = nullptr;

    // Get Child Frame Pointer
    CWnd* pWnd = nullptr;
    pWnd = pomGetParentWindow();

    // Get View Pointer
    if( pWnd != nullptr )
    {
        pView = ((CTxMsgChildFrame*)pWnd)->pomGetTxMsgViewPointers(
                    eTxMsgBlocksView );
    }

    // Return view pointer
    return pView;
}

CWnd* CTxFunctionsView::pomGetListViewPointer() const
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

CWnd* CTxFunctionsView::pomGetDetailsViewPointer() const
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

void CTxFunctionsView::vReloadData()
{
    // Get Other View Ponters
    CTxMsgBlocksView* pomBlockView = nullptr;
    CTxMsgListView* pomlistView = nullptr;
    CTxMsgDetailsView* pomDetailsView = nullptr;

    // Fetch pointers from Tx Child Frame
    pomBlockView = ( CTxMsgBlocksView* )pomGetBlocksViewPointer();
    pomlistView = ( CTxMsgListView* )pomGetListViewPointer();
    pomDetailsView = ( CTxMsgDetailsView* )pomGetDetailsViewPointer();

    if( pomBlockView != nullptr && pomlistView != nullptr && pomDetailsView != nullptr )
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
        pomBlockView->m_psMsgBlockList = nullptr;

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
            pomBlockView->m_omComboAllMsgs.EnableWindow(FALSE);
            pomlistView->m_omButtonDeleteAllMsg.EnableWindow(FALSE);
            pomlistView->m_omButtonDeleteSelMsg.EnableWindow(FALSE);

        }
        else
        {
            pomlistView->m_omButtonDeleteAllMsg.
            EnableWindow(!CTxMsgManager::s_TxFlags.nGetFlagStatus(TX_SENDMESG) );
        }

        PSMSGBLOCKLIST psCurrentMsgBlock = nullptr;
        CString omStr = "";
        psCurrentMsgBlock = pomBlockView->m_psMsgBlockList;

        // Disable AllTxMessage check box
        // This will be updated in the function vDisplayMsgBlockDetails
        // Disabling it here will avoid enabling the control when no blocks or
        // messages are present
        pomBlockView->m_omComboAllMsgs.EnableWindow(FALSE);

        for(UINT i = 0; i< pomBlockView->m_unMsgBlockCount; i++)
        {
            omStr = "";
            if(psCurrentMsgBlock != nullptr )
            {
                pomBlockView->m_omLctrMsgBlockName.InsertItem( i,
                        psCurrentMsgBlock->m_acStrBlockName);
                if(psCurrentMsgBlock->m_bType == TRUE)
                {
                    pomBlockView->m_omLctrMsgBlockName.SetItemText( i,
                            defSUBITEM_MSGBLOCK_TYPE,
                            _(defMSG_CYCLIC) );
                }
                else
                {
                    pomBlockView->m_omLctrMsgBlockName.SetItemText( i,
                            defSUBITEM_MSGBLOCK_TYPE,
                            _(defMSG_MONOSHOT) );
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

void CTxFunctionsView::OnInvokeClose()
{
    CTxMsgBlocksView* pBlocksView = (CTxMsgBlocksView*)pomGetBlocksViewPointer();
    bool        bUnChanged = true;

    if(m_CheckBoxAutoUpdate.GetCheck() == BST_CHECKED)  //setting will already be saved so exit
    {
        return;
    }
    if(pBlocksView != nullptr)
    {

        PSMSGBLOCKLIST psMsgBlock = nullptr;
        psMsgBlock = pBlocksView->psGetMsgBlockPointer(
                         0,
                         pBlocksView->m_psMsgBlockList );

        if(psMsgBlock != nullptr)
        {
            bUnChanged = (CTxWndDataStore::ouGetTxWndDataStoreObj().nCompareBlockLists(psMsgBlock) == TRUE);
            if(pBlocksView->m_bDelayBtnBlocks != CTxWndDataStore::ouGetTxWndDataStoreObj().m_bDelayBetweenMsgBlocks)
            {
                bUnChanged = false;
            }
            CString  omstrData;
            pBlocksView->m_omTimeDelayBtwnBlocks.GetWindowTextA(omstrData);
            if(atoi(omstrData.GetBuffer(0)) != CTxWndDataStore::ouGetTxWndDataStoreObj().m_unTimeDelayBtwnMsgBlocks)
            {
                bUnChanged = false;
            }
        }
        else if(CTxWndDataStore::ouGetTxWndDataStoreObj().psReturnMsgBlockPointer()!= nullptr)
        {
            bUnChanged = false;
        }
        if(bUnChanged == true)
        {
            return;
        }
    }

    if( bUnChanged == false)            //if there are any changes, then save it.
    {
        if( AfxMessageBox( defSTR_TX_SAVE_CONFIRMATION,
                           MB_YESNO | MB_ICONQUESTION ) == IDYES )
        {
            // Save Changes
            OnButtonApply();
        }
    }
}

void CTxFunctionsView::OnBtnClose()
{
    // Get Child Frame Pointer
    CWnd* pWnd = nullptr;
    pWnd = pomGetParentWindow();
    // Post close message
    if( pWnd != nullptr )
    {
        // This will invoked the InvokeClose function
        ((CTxMsgChildFrame*)pWnd)->PostMessage( WM_CLOSE );
    }
}

void CTxFunctionsView::OnBnClickedCheckAutoUpdate()
{
    CTxMsgBlocksView* pBlocksView = (CTxMsgBlocksView*)pomGetBlocksViewPointer();
    if (nullptr != pBlocksView)
    {
        if (nullptr == pBlocksView->m_psMsgBlockList)
        {
            GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(FALSE);
        }
        else
        {
            GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(TRUE);
        }
    }
    else
    {
        GetDlgItem(IDC_BTN_UPDATE)->EnableWindow(FALSE);
    }

    if((m_CheckBoxAutoUpdate.GetCheck() == BST_CHECKED))
    {
        vAccessButtonApply();
        CTxWndDataStore::ouGetTxWndDataStoreObj().m_bAutoSavedEnabled = true;
        if (nullptr != pBlocksView)
        {
            UINT                    unTimerVal;
            CString                 csTimerVal;
            pBlocksView->GetDlgItemText(IDC_EDIT_BLOCK_TRG_TIMER_VAL, csTimerVal);
            unTimerVal = (UINT)atol(csTimerVal.GetBuffer(0));
            CTxWndDataStore::ouGetTxWndDataStoreObj().m_unTimeDelayBtwnMsgBlocks = unTimerVal;
            CTxMsgManager::s_unTimeDelayBtnMsgBlocks = unTimerVal;
            //set the status of delay between blocks
            if(pBlocksView->m_omDelayBtwnBlocks.GetCheck() == BST_CHECKED)
            {
                CTxMsgManager::s_bDelayBetweenBlocksOnly = true;
                CTxWndDataStore::ouGetTxWndDataStoreObj().m_bDelayBetweenMsgBlocks = true;
                pBlocksView->m_bDelayBtnBlocks = true;
            }
            else
            {
                CTxMsgManager::s_bDelayBetweenBlocksOnly = false;
                CTxWndDataStore::ouGetTxWndDataStoreObj().m_bDelayBetweenMsgBlocks = false;
                pBlocksView->m_bDelayBtnBlocks = false;
            }
        }
    }
    else
    {
        CTxWndDataStore::ouGetTxWndDataStoreObj().m_bAutoSavedEnabled = false;
    }
}

void CTxFunctionsView::vAccessButtonApply()
{
    SetUpdateBtnChanges();
}
