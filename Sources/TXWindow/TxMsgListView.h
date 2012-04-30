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
 * \file      TxMsgListView.h
 * \brief     Interface file for CTxMsgListView class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CTxMsgListView class
 */

#pragma once

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "TxWindow_resource.h"

//struct SMSGBLOCKLIST;
//struct sTXCANMSGDETAILS;
class CTxMsgListView : public CFormView
{
    // Form Data
public:
    //{{AFX_DATA(CTxMsgListView)
    enum { IDD = IDD_DLG_TX_MSG_BLOCK_DETAILS };
    CListCtrl   m_omLctrMsgList;
    CButton m_omButtonDeleteAllMsg;
    CButton m_omButtonDeleteSelMsg;
    CButton m_omButtonSendMsg;
    //}}AFX_DATA

    // Attributes
public:
    // To denote programmed modification of list item to avoid
    // item change handler execution
    BOOL m_bInitDlg;
    // Selected Message Index
    int m_nSelectedMsgIndex;

    // Operations
public:
    // To clear message block
    BOOL bDeleteAllMsgFromBlock(SMSGBLOCKLIST* psMsgCurrentBlock);
    // To get Message pointer from block pointer
    PSTXCANMSGLIST psGetMsgDetailPointer( INT nIndex,
                                          SMSGBLOCKLIST* psCurrentMsgBlockList);
    // To update list with given details
    VOID vUpdateMsgListDisplay(sTXCANMSGDETAILS sMsgDetail, INT nCurrentIndex);

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTxMsgListView)
public:
    virtual void OnInitialUpdate();
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    virtual ~CTxMsgListView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // Generated message map functions
    //{{AFX_MSG(CTxMsgListView)
    afx_msg void OnColumnclickLstcMsgDetails(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnItemchangedLstcMsgDetails(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSendSelectedMsg();
    afx_msg void OnDeleteSelectedMsg();
    afx_msg void OnDeleteAllMsg();
    afx_msg void OnRightClickMsgDetails(NMHDR* pNMHDR, LRESULT* pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

protected:
    // protected constructor used by dynamic creation
    CTxMsgListView();
    DECLARE_DYNCREATE(CTxMsgListView)

private:
    // To show popup menu item
    BOOL bDisplayPopMenu(CListCtrl& omList,UINT nIDResource );
    // To set or reset Check box of selected message
    void vSetMessageCheckValue(BOOL bCheck);
    // To Get Parent Window Pointer
    CWnd* pomGetParentWindow() const;
    // To get Block View Pointer
    CWnd* pomGetBlocksViewPointer() const;
    // To get Functions view pointer
    CWnd* pomGetFunctionsViewPointer() const;
    // To get Details view pointer
    CWnd* pomGetDetailsViewPointer() const;
    // To delete selected message from the block
    BOOL bDeleteMsgFromBlock(SMSGBLOCKLIST* psMsgCurrentBlock);
    // For Message List control
    CImageList m_omImageList;
    // For Header control image list
    CImageList m_omHeaderImageList;
};
