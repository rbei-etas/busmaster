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
 * @brief Interface file for CTxMsgBlocksView class
 * @author Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CTxMsgBlocksView class
 */

#pragma once

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "afxwin.h"
#include "TxWindow_resource.h"
#include "Utility/RadixEdit.h"          // For the RAdix Edit control definition
#include "Utility/FFListCtrl.h"

class CTxMsgBlocksView : public CFormView
{
public:
    enum { IDD = IDD_DLG_TX_MSG_BLOCKS };

    /** used to check whether its new block is added or not */
    bool m_bNewBlock;

    /**
     * this will store the value of delay between blocks locally,
     * this is needed to keep a back up of the value in case when
     * user doesnt want to save the data, this should not be used anywhere
     * other than to restore the value to global variables on selection of close and not saving
     */
    bool m_bDelayBtnBlocks;

    CFFListCtrl m_omLctrMsgBlockName;
    CString m_omStrMsgBlockName;
    CString m_omStrTimeIntervalVal;
    CString m_omStrKeyVal;
    CString m_omDelayBtnBlocks;
    CEdit m_omEditMsgBlockName;
    CRadixEdit m_omEditTrgTimeIntervalVal;
    CRadixEdit m_omEditTrgKeyVal;
    CButton m_omButtonKeyTrigger;
    CButton m_omButtonTimeTrigger;
    CButton m_omButtonAddMsgBlock;
    CButton m_omButtonDeleteMsgBlock;
    CButton m_omDelayBtwnBlocks;
    CRadixEdit m_omTimeDelayBtwnBlocks;
    BOOL m_bListItemChange;

    /** To denote modifications after connect */
    BOOL m_bModified;

    /** To store selected message block index */
    int m_nSelectedMsgBlockIndex;

    /** To Keep local copy of Tx Msg List */
    PSMSGBLOCKLIST m_psMsgBlockList;

    /**
     * To denote programmed modification of block list so that onchange list item
     * handler will not be exccuted
     */
    BOOL m_bInitDlg;

    /** To keep track of message block count */
    UINT m_unMsgBlockCount;

    /**
     * @brief To get message block pointer denoted by the index from the given list of blocks
     * @param[in] nIndexCurBlock Index of message block in the list
     * @param[in] psCurrentMsgBlockList header of msg block pointer
     *
     * This function will return message block pointer for
     * node at index nIndexCurBlock passed as parameter from
     * the link list of message block.
     */
    SMSGBLOCKLIST* psGetMsgBlockPointer(INT nIndexCurBlock,
                                        SMSGBLOCKLIST* psMsgBlockList);

    /**
     * @brief To update given block information in the UI
     *
     * Update the message block information entered by user
     *
     * @param[in] psCurrentMsgBlock current block pointer
     */
    void vUpdateMsgBlockDetials(SMSGBLOCKLIST* psCurrentMsgBlock);

    /**
     * @brief To enable or disable list of controls
     *
     * This function will enable or disable the controls
     * needed depending upon the flag bEnable condition.
     */
    void vEnableControls(BOOL bEnable);

    /**
     * @brief To display given message block information in UI
     *
     * This function will display the message block details
     * in this function
     *
     * @param[in] psMsgBlock current msg block pointer
     */
    void vDisplayMsgBlockDetails(SMSGBLOCKLIST* psMsgBlock);

    /**
     * @brief To set control properties like limit text, base etc.
     *
     * This function changed the control properties for hex
     * and decimal settings.
     */
    void vSetControlProperties();

    /** Updates the contents of listview to list */
    void UpdateList(NM_LISTVIEW* pNMListView);

    /**
     * This function will be called by the framework during initial
     * show of this view. This function will register iteself in to
     * parent window class so that other views shall access it. This
     * will initialise message block list control and oter edit
     * boxes.
     */
    virtual void OnInitialUpdate();

    afx_msg void OnBnClickedRadiomonoshot();

    afx_msg void OnBnClickedRadiocyclic();

    CComboBox m_omComboAllMsgs;

    afx_msg void OnCbnSelchangeComboMsgs();

    /**
     * This function will be called when any of the EditBox
     * in this view looses focus. We need to update the
     * global structure each time
     */
    afx_msg void AutoUpdateChanges();

    afx_msg void OnBnClickedCheckMsgBlockDelay();

    afx_msg void OnEnUpdateEditBlockTrgTimerVal();

protected:
    /** protected constructor used by dynamic creation */
    CTxMsgBlocksView();
    virtual ~CTxMsgBlocksView();

    /**
     * @brief DDX/DDV support
     *
     * This function will map DDX object with UI control for data
     * exchange
     *
     * @param[in] pDX Pointer to DDX object
     */
    virtual void DoDataExchange(CDataExchange* pDX);

    DECLARE_DYNCREATE(CTxMsgBlocksView)

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    /**
     * @brief On Add Msg Block
     * @req RS_17_01 - Addition of a transmission block
     *
     * This function will called when user selects Add button
     * or menu. This will add a new message block and
     * initialise it.
     */
    afx_msg void OnAddMsgBlock();

    /**
     * @brief On Delete Selected Msg Block
     * @req RS_17_02 - Deletion of a transmission block
     *
     * This function will be called by frame work when delete
     * button or menu is selected. This will be called for
     * message block deletion. It will delete the whole block
     * by removing it from list and also all message frame in
     * the block will be deleted. Memory will be deallocated
     */
    afx_msg void OnDeleteSelectedMsgBlock();

    /**
     * This function will be called by frame work when
     * selection is changed in message blocks list control.
     */
    afx_msg void OnItemchangedLstcMsgBlocksName(NMHDR* pNMHDR, LRESULT* pResult);

    /**
     * This function will be called by frame work when user
     * changes edit box "Message Block name"
     */
    afx_msg void OnChangeEditMsgBlockName();
 
    /**
     * This function is called by framework whenever user
     * selected check button for trigger type.
     */
    afx_msg void OnChkbTriggerType();

    /**
     * This function is will update trigger flag and appropriate UI
     * components
     */
    afx_msg void OnChkbOnTimeTrigger();

    /**
     * This function is will update trigger flag and appropriate UI
     * components
     */
    afx_msg void OnChkbOnKeyTrigger();

    /**
     * This function will be called by frame work when user
     * changes is updated in edit box "time".
     */
    afx_msg void OnUpdateEditTrgTimeVal();

    /**
     * This function will be called by frame work when user
     * changes is updated in edit box "key value"
     */
    afx_msg void OnUpdateEditTrgKeyVal();

    /**
     * This function is called by the framework when user
     * clicks "All Messages"  check box. It will enable the
     * Apply button.
     */
    afx_msg void OnChkbTxAllFrame();

    /** This function will call bDisplayPopMenu. */
    afx_msg void OnRclickLstcMsgBlocksName(NMHDR* pNMHDR, LRESULT* pResult);

    DECLARE_MESSAGE_MAP()

private:
    /**  To keep the block deletion flag */
    BOOL m_bMsgBlockDeleted;

    /** To store trigger type of message block */
    unsigned char m_nRBTNTriggerType;

    /** To retain the previous value of time delay between messges */
    int m_nTimeDelMsg;

    /** To retain the previous value of time delay between blocks */
    int m_nTimeDelBlocks;

    /**
     * @brief To add a message block
     * This function will add message block at the end in the
     * message block list. The message block will be
     * initailised.
     *
     * @return TRUE or FALSE
     */
    BOOL bAddBlock(SMSGBLOCKLIST*& psMsgCurrentBlock);

    /**
     * @brief To initialise message block with default values
     *
     * This function will initialise the message block
     * element with default values
     *
     * @param[in] psMsgBlockList current msg block pointer
     */
    void vInitMsgBlockInfo(SMSGBLOCKLIST* psMsgBlockList);

    /**
     * @brief To delete selected message block
     *
     * This function will delete a message block and its msg
     * list after removing node from the list. It will also
     * delete the memory, after removing all message list and
     * memory for message list.
     *
     * @return TRUE or FALSE
     */
    BOOL bDeleteBlock();

    /**
     * @brief To get parent window pointer
     *
     * This Function will return parent window pointer. That is
     * pointer to CTxMsgChildFrame. This will return nullptr incase of
     * failure
     *
     * @return Pointer to CTxMsgChildFrame
     */
    CWnd* pomGetParentWindow() const;

    /**
     * @brief To get CTxMsgListView pointer
     *
     * This function will return CTxMsgListView pointer. This will
     * get child window pointer to get view pointer.
     *
     * @return Pointer to CTxMsgListView or nullptr incase of failure
     */
    CWnd* pomGetListViewPointer() const;

    /**
     * @brief To get CTxFunctionsView Pointer
     *
     * This function will return CTxFunctionsView pointer. This
     * will get child window pointer to get view pointer.
     *
     * @return Pointer to CTxFunctionsView or nullptr incase of failure
     */
    CWnd* pomGetFunctionsViewPointer() const;

    /**
     * @brief To get CTxMsgDetailsView Pointer
     *
     * This function will return CTxMsgDetailsView pointer. This
     * will get child window pointer to get view pointer.
     *
     * @return Pointer to CTxMsgDetailsView or nullptr incase of failure
     */
    CWnd* pomGetDetailsViewPointer() const;

    /**
     * @brief To show popup memu
     *
     * This function will display pop-up menus if selection
     * is valid and chooses the appropriate menu for display
     *
     * @return TRUE or FALSE
     */
    BOOL bDisplayPopMenu(CListCtrl& omList, UINT nIDResource);

    /** Save the current msg block */
    void vSaveCurrentBlockFirst(void);
};
