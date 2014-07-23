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
 * @brief Interface file for CTxMsgListView class
 * @author Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CTxMsgListView class
 */

#pragma once

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "TxWindow_resource.h"

class CTxMsgListView : public CFormView
{
public:
    enum { IDD = IDD_DLG_TX_MSG_BLOCK_DETAILS };
    CListCtrl m_omLctrMsgList;
    CButton m_omButtonDeleteAllMsg;
    CButton m_omButtonDeleteSelMsg;
    CButton m_omButtonSendMsg;

    /**
     * To denote programmed modification of list item to avoid
     * item change handler execution
     */
    BOOL m_bInitDlg;

    /** Selected Message Index */
    int m_nSelectedMsgIndex;

    /** Enable/disable header check */
    void vCheckHeaderCtrl(bool bCheck);

    /**
     * @brief To clear message block
     *
     * This function will delete all memory allocated in the
     * current message block and return TRUE on success.
     *
     * @param[in] psMsgCurrentBlock pointer of current block
     * @return TRUE or FALSE
     */
    BOOL bDeleteAllMsgFromBlock(SMSGBLOCKLIST* psMsgCurrentBlock);

    /**
     * @brief To get Message pointer from block pointer
     *
     * This function will return message list pointer for
     * node at index nIndex passed as parameter from the link
     * of message frame list from the current message block.
     *
     * @param[in] nIndex Index of message in the list
     * @param[in] psCurrentMsgBlockList current msg block pointer
     */
    PSTXCANMSGLIST psGetMsgDetailPointer(INT nIndex,
                                         SMSGBLOCKLIST* psCurrentMsgBlockList);

    /**
     * @brief To update list with given details
     *
     * This function will update the list cotrol of message
     * list at the selected item or as new item.
     *
     * @param[in] sMsgDetail message details struct
     * @param[in] nCurrentIndex Selected Index or -1 (new entry)
     */
    void vUpdateMsgListDisplay(sTXCANMSGDETAILS sMsgDetail, INT nCurrentIndex);

    virtual void OnInitialUpdate();

protected:
    /**
     * @brief DDX/DDV support
     *
     * This function will map DDX object with UI control for data
     * exchange
     *
     * @param[in] pDX Pointer to DDX object
     */
    virtual void DoDataExchange(CDataExchange* pDX);

    virtual ~CTxMsgListView();

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    /**
     * This function will be called by the framework when user click
     * on List control header. This function will handle this event
     * only if first column is clicked. In this condition this will
     * toggle column 0 image to show checked/unchecked checkbox
     * image and will call vSetMessageCheckValue to update message
     * details
     *
     * @param[in] pNMHDR Pointer to List Item
     * @param[in] pResult Pointer to Result
     */
    afx_msg void OnColumnclickLstcMsgDetails(NMHDR* pNMHDR, LRESULT* pResult);

    /**
     * This function will be called by frame work when
     * selection is changed in message details list control.
     */
    afx_msg void OnItemchangedLstcMsgDetails(NMHDR* pNMHDR, LRESULT* pResult);

    /**
     * This function is called by frame work when user wants
     * to Transmit selected message
     */
    afx_msg void OnSendSelectedMsg();

    /**
     * This function will delete the selected message frame if
     * the user confirms the deletion action.
     */
    afx_msg void OnDeleteSelectedMsg();

    /**
     * This function will be called when user selects to
     * delete all message using menu or button.
     */
    afx_msg void OnDeleteAllMsg();

    /** This function will call bDisplayPopMenu. */
    afx_msg void OnRightClickMsgDetails(NMHDR* pNMHDR, LRESULT* pResult);

    DECLARE_MESSAGE_MAP()

    /* protected constructor used by dynamic creation */
    CTxMsgListView();

    DECLARE_DYNCREATE(CTxMsgListView)

private:
    /**
     * @brief To show popup menu item
     *
     * This function will display pop-up menus if selection
     * is valid and chooses the appropriate menu for display
     *
     * @return TRUE or FALSE
     */
    BOOL bDisplayPopMenu(CListCtrl& omList, UINT nIDResource);

    /**
     * @brief To set or reset Check box of selected message
     * This function will update selected message with the check
     * value. This value will be assigned to m_bEnabled member of
     * message detail.
     *
     * @param[in] bCheck TRUE to Enable message, FALSE to disable message for transmission
     */
    void vSetMessageCheckValue(BOOL bCheck);

    /**
     * @brief To Get Parent Window Pointer
     *
     * This Function will return parent window pointer. That is
     * pointer to CTxMsgChildFrame. This will return nullptr incase of
     * failure
     *
     * @return Pointer to CTxMsgChildFrame
     */
    CWnd* pomGetParentWindow() const;

    /**
     * @brief To get Block View Pointer
     *
     * This function will return CTxMsgBlocksView pointer. This will
     * get child window pointer to get view pointer.
     *
     * @return Pointer to CTxMsgBlocksView or nullptr incase of failure
     */
    CWnd* pomGetBlocksViewPointer() const;

    /**
     * @brief To get Functions view pointer
     *
     * This function will return CTxFunctionsView pointer. This
     * will get child window pointer to get view pointer.
     *
     * @return Pointer to CTxFunctionsView or nullptr incase of failure
     */
    CWnd* pomGetFunctionsViewPointer() const;

    /**
     * @brief To get Details view pointer
     * This function will return CTxMsgDetailsView pointer. This
     * will get child window pointer to get view pointer.
     *
     * @return Pointer to CTxMsgDetailsView or nullptr incase of failure
     */
    CWnd* pomGetDetailsViewPointer() const;

    /**
     * @brief To delete selected message from the block
     *
     * This function will delete messages selected by user
     * from currently selected message block. Also the node
     * for link list will be removed and the list will be
     * updated.
     *
     * @param[in] psMsgCurrentBlock Current message block pointer
     * @return TRUE or FALSE
     */
    BOOL bDeleteMsgFromBlock(SMSGBLOCKLIST* psMsgCurrentBlock);

    /** For Message List control */
    CImageList m_omImageList;

    /** For Header control image list */
    CImageList m_omHeaderImageList;
};
