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
 * \file      TSEditorGUI_PropertyView.h
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "TSEditorGUI_resource.h"
#include "afxcmn.h"
#include "ListCtrlEx.h"

// CPropertyView form view
class CTSEditorChildFrame;
class CPropertyView : public CFormView
{
    //Member Variables
    DECLARE_DYNCREATE(CPropertyView)
    CRect m_rectList;
    CRect m_rectHelp;
    CRect m_rectCancel;
    CRect m_rectConfirm;
    BOOL m_bSizeAvailable;
    CImageList m_omImageList;
    CListCtrlEx m_omPropertyList;
public:
    enum { IDD = IDD_PROPERTYVIEW };

    //Member Functions
public:
    void vShowHelpInfo(CString& omHelpString);

#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    virtual void OnInitialUpdate();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedConfirm();
    afx_msg void OnLvnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);

protected:
    CPropertyView();           // protected constructor used by dynamic creation
    virtual ~CPropertyView();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    DECLARE_MESSAGE_MAP()

private:
    CTSEditorChildFrame* GetEditorWindow();
    CString m_omstrHelpString;
};
