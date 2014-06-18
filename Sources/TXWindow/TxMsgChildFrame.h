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
 * @brief Interface file for CTxMsgChildFrame class
 * @author Ratnadip Choudhury
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CTxMsgChildFrame class
 */

#pragma once

#include "Tx_MDIChildBase.h"       // For Common base class defintion
#include "TxFunctionsView.h"    // For CTxFunctionsView class defintions
#include "TxMsgBlocksView.h"    // For CTxFunctionsView class defintions
#include "TxMsgDetailsView.h"   // For CTxMsgDetailsView class defintions
#include "TxMsgListView.h"      // For CTxMsgListView class defintions

class CTxMsgChildFrame : public CMDIChildBase
{
public:
    CTxMsgChildFrame();
    virtual ~CTxMsgChildFrame();

    /**
     * @brief To update window size and splitter after loading a conf file
     *
     * This function will set window postion and splitter postion.
     * For setting window postion this will use parent class
     * function and then will set splitter position.
     */
    void vUpdateWinStatus();

    /**
     * @brief To save window size and splitter postion before unloading a conf file
     *
     * This function will save window postion in to configuration
     * module using parent class function & will also save splitter
     * window postion.
     */
    void vUpdateWndCo_Ords();

    afx_msg void OnClose();

    /**
     * @brief To Set View Pointers
     *
     * This function will set view pointer
     *
     * @param[in] eViewIdentity View Identity parameter
     * @param[in] pomWnd View Pointer
     */
    void vSetTxMsgViewPointers(ETXMSGVIEWTYPE eViewIdentity, CWnd* pomWnd);

    /**
     * @brief To get View pointers
     *
     * This function will return view pointer spefied by the param
     * eViewIdentity
     *
     * @param[in] eViewIdentity View Identity parameter
     */
    CWnd* pomGetTxMsgViewPointers(ETXMSGVIEWTYPE eViewIdentity) const;

    /**
     * Sets the m_pouDBPtr pointer in TxMsgDetailsView to pMsgDB.
     *
     * @param[in] pMsgDB CMsgSignal Pointer
     */
    void vSetMsgDBPtrInDetailsView(void* pMsgDB);

    void vChangeConnectionStatus(BOOL bConnect);

    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
    afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

    void vCallAutoUpdate();

protected:
    /**
     * This function will be called to create window. This function
     * will create splitter window and views.
     *
     * @param[in] lpcs Create Parameters
     * @param[in] pContext Pointer to Create Context
     * @return Create Result
     */
    virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

    /**
     * This function will simply return true to avoid flickering due
     * to background erase
     *
     * @param[in] pDC Pointer to Device context
     */
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnDestroy();

    /**
     * This function is message mapped and will be called
     * when selects, connect/disconnect start/stop tx and
     * hex/dec button/menu. The setting of controls will be
     * changed accordingly.
     *
     * @param[in] wParam enumerator eUSERSELCTION
     * @param[in] lParam : NOT USED
     */
    afx_msg LRESULT vUserCommand(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()

    CSplitterWnd m_omRootSplitter;
    CSplitterWnd m_omLeftViewSplitter;
    CSplitterWnd m_omRightViewSplitter;

private:
    DECLARE_DYNCREATE(CTxMsgChildFrame)

    /** Structure to hold splitter window postion */
    STXMSGSPLITTERDATA m_sSplitterPosition;

    /** Pointers to all views */
    CTxMsgBlocksView* m_pomTxMsgBlocksView;

    /** Message List view Pointer */
    CTxMsgListView* m_pomTxMsgListView;

    /** For functions view poniter */
    CTxFunctionsView* m_pomFunctionsView;

    /** Message Details view pointer */
    CTxMsgDetailsView* m_pomTxMsgDetailsView;

    BOOL m_bInit;

    /**
     * @brief Function to set splitter postion.
     *
     * This function will set splitter postion. This will refer
     * member variable m_sSplitterPostion to get splitter postion
     */
    void vSetSplitterPostion();

    /**
     * @brief Function to get splitter postion from configuration file
     *
     * This function will get splitter information from
     * configuration module
     */
    void vGetSplitterStatus();

    /**
     * @brief To save splitter position from configuration file
     *
     * This function will save splitter postion. This will get
     * current splitter postion and will compare with
     * m_sSplitterPostion. If current position is different it will
     * update member varaible and configuration module.
     */
    void vSaveSplitterPostion();
};
