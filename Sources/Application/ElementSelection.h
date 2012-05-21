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
 * \file      ElementSelection.h
 * \brief     Interface file for CElementSelection class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CElementSelection class
 */

#pragma once

// For Main Frame Definition
#include "MainFrm.h"

class CElementSelection : public CDialog
{
    // Construction
public:
    // Pointer to Main Frame
    CMainFrame* m_pMainFrame;
    eTYPE_BUS m_eBusType;

    // standard constructor
    CElementSelection(UINT nHardware = defNO_OF_CHANNELS, CWnd* pParent = NULL);
    // Dialog Data
    //{{AFX_DATA(CElementSelection)
    enum { IDD = IDD_DLG_MSG_LIST };
    CListCtrl   m_omElementList;
    CTreeCtrl   m_omTreeEntries;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CElementSelection)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CElementSelection)
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    afx_msg void OnDblclkTreeSignal(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnBtnAdd();
    afx_msg void OnBtnDelete();
    afx_msg void OnBtnDeleteAll();
    afx_msg void OnDblclkLstcGraphElements(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnClickTreeSignal(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnClickLstcGraphElements(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnItemchangedLstcGraphElements(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSelchangedTreeSignal(NMHDR* pNMHDR, LRESULT* pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    // Get the statistics channel handle
    HTREEITEM hGetChannelHandle(const CString& omStrChannel);
    // To create channel specific statistics tree
    void vCreateChannelTree(HTREEITEM hStatRoot);
    // Handle to the Statistics root
    HTREEITEM m_hStatRoot;
    // Handle to the Message root
    HTREEITEM m_hMessageRoot;

    // To Enable / Disable Buttons
    void vEnableDisableButtons();
    // To Get the tree item of the signal
    HTREEITEM hGetElementHandle( const CString& omStrMsg,
                                 const CString& omStrSig, BOOL bCreate);
    // To Add the signal to the list
    void vAddToElementList( CString omStrMsgName,
                            CString omStrSigName,
                            int nValType );
    // To Add Statistics element to the list
    void vAddStatToElementList(CString omStatParamName,  CString omChannel );
    // To Add the selected item
    void vAddSelectedItem();
    // To Populate Element List
    void vPopulateElementList(
        const CArray<CGraphElement,CGraphElement&>& odElementList,
        CListCtrl& omListCtrl,
        int nStatImageIndex,
        int nRawValImgIndex,
        int nPhyValImgIndex );
    // To Populate Tree contorl with DB messages
    void vPopulateDBTree(
        CTreeCtrl& omTree, HTREEITEM hMessageRoot,
        const CArray<CGraphElement,CGraphElement&>& odElementList,
        int nMsgImageIndex, int nSigImageIndex,
        int nRawImgIndex, int nPhyImgIndex );
    // To populate Statistics items in the tree control
    void vPopulateStatTree(
        CTreeCtrl& omTree, HTREEITEM hStatRoot, int nChannel,
        const CArray<CGraphElement,CGraphElement&>& odElementList,
        int nStatImageIndex );
    // Image List used by List control and tree control
    CImageList m_omImageList;
    // Graph element array
    CArray<CGraphElement, CGraphElement&> m_odElementList;
    UINT m_nHardware;

public:
    afx_msg void OnBnClickedOk();
};
