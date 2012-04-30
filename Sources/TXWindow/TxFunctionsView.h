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
 * \file      TxFunctionsView.h
 * \brief     Interface file for CTxFunctionsView class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CTxFunctionsView class
 */

#pragma once

// TxFunctionsView.h : header file
#include "TxWindow_resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTxFunctionsView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CTxFunctionsView : public CFormView
{
    // Form Data
public:
    //{{AFX_DATA(CTxFunctionsView)
    enum { IDD = IDD_DLG_TX_FUNCTIONS };
    CButton m_omButtonApply;
    //}}AFX_DATA

    // Attributes
public:

    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTxFunctionsView)
public:
    virtual void OnInitialUpdate();
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

protected:
    // protected constructor used by dynamic creation
    CTxFunctionsView();
    DECLARE_DYNCREATE(CTxFunctionsView)
    // Implementation
protected:
    virtual ~CTxFunctionsView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // Generated message map functions
    //{{AFX_MSG(CTxFunctionsView)
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnButtonApply();
    afx_msg void OnBtnClose();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    // To Update Configuration module with the changes done
    void vApplyChanges();
    // To reload TxMsg window with the configuration module Tx data
    void vReloadData();
    // To Get Parent window pointer. That is pointer of CTxMsgChildFrame
    CWnd* pomGetParentWindow() const;
    // To Get CTxMsgBlocksView Pointer
    CWnd* pomGetBlocksViewPointer() const;
    // To Get CTxMsgListView Pointer
    CWnd* pomGetListViewPointer() const;
    // To Get CTxMsgDetailsView Pointer
    CWnd* pomGetDetailsViewPointer() const;
};
