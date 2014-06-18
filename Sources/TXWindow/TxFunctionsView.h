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
 * @brief Interface file for CTxFunctionsView class
 * @author Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CTxFunctionsView class
 */

#pragma once

#include "TxWindow_resource.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CTxFunctionsView : public CFormView
{
public:
    enum { IDD = IDD_DLG_TX_FUNCTIONS };

    CButton m_omButtonApply;

    /**
     * This function will be called by the framework during initial
     * show of this view. This function will register iteself in to
     * parent window class so that other views shall access it.
     */
    virtual void OnInitialUpdate();

    void OnInvokeClose();

    void vAccessButtonApply();

    CButton m_CheckBoxAutoUpdate;

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

protected:
    /** protected constructor used by dynamic creation */
    CTxFunctionsView();
    virtual ~CTxFunctionsView();

    DECLARE_DYNCREATE(CTxFunctionsView)

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    /**
    * This function will be called by the framework during sizing
    * of the window. This function will set the buttons position at
    * the center of the dialog
    *
    * @param[in] nType Sizing mode
    * @param[in] cx New X value
    * @param[in] cy New Y Value
    */
    afx_msg void OnSize(UINT nType, int cx, int cy);

    /**
     * This function is called when user press Apply button.
     * The flags will be set and global list will be updated.
     */
    afx_msg void OnButtonApply();

    /**
     * This button handler will be called during Close button press.
     * This function will send WM_CLOSE message to close Tx Child
     * window.
     */
    afx_msg void OnBtnClose();

    DECLARE_MESSAGE_MAP()

private:
    /**
     * @brief To Update Configuration module with the changes done
     *
     * This function is used to apply changes in the global shared
     * memory
     */
    void vApplyChanges();

    /**
     * @brief To reload TxMsg window with the configuration module Tx data
     *
     * This function will get the Tx block data from configuration
     * and will update the UI. This function will be called if the
     * UI block information is not saved before transmission and
     * the user wants to apply the changes with Tx on which would
     * cause access violation in case of delete. To avoid that the
     * data from configuration is reloaded.
     */
    void vReloadData();

    /**
     * @brief To Get Parent window pointer. That is pointer of CTxMsgChildFrame
     *
     * This Function will return parent window pointer. That is
     * pointer to CTxMsgChildFrame. This will return nullptr incase of
     * failure
     *
     * @return Pointer to CTxMsgChildFrame
     */
    CWnd* pomGetParentWindow() const;

    /**
     * @brief To Get CTxMsgBlocksView Pointer
     *
     * This function will return CTxMsgBlocksView pointer. This will
     * get child window pointer to get view pointer.
     *
     * @return Pointer to CTxMsgBlocksView or nullptr incase of failure
     */
    CWnd* pomGetBlocksViewPointer() const;

    /**
     * @brief To Get CTxMsgListView Pointer
     *
     * This function will return CTxMsgListView pointer. This will
     * get child window pointer to get view pointer.
     *
     * @return Pointer to CTxMsgListView or nullptr incase of failure
     */
    CWnd* pomGetListViewPointer() const;

    /**
     * @brief To Get CTxMsgDetailsView Pointer
     *
     * This function will return CTxMsgDetailsView pointer. This
     * will get child window pointer to get view pointer.
     *
     * @return Pointer to CTxMsgDetailsView or nullptr incase of failure
     */
    CWnd* pomGetDetailsViewPointer() const;

    void SetUpdateBtnChanges();

    /**
     * This checkbox updates the content automaticallyan stores it in
     * global store
     */
    afx_msg void OnBnClickedCheckAutoUpdate();
};
