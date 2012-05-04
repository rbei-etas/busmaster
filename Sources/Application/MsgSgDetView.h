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
 * \file      MsgSgDetView.h
 * \brief     This header file contains the defintion of class
 * \author    Amarnath Shastry
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the defintion of class
 */

#pragma once

#include "Datatypes/MsgSignal_Datatypes.h"
#include "MsgSignal.h"
/////////////////////////////////////////////////////////////////////////////
// CMsgSgDetView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CMsgSgDetView : public CFormView
{
protected:
    CMsgSgDetView();           // protected constructor used by dynamic creation
    DECLARE_DYNCREATE(CMsgSgDetView)

    // Form Data
public:
    //{{AFX_DATA(CMsgSgDetView)
    enum { IDD = IDD_DLG_MSGSGDETVIEW };
    CSpinButtonCtrl m_omSpinMsgLen;
    CListCtrl   m_omListCtrlSgIdVal;
    CListCtrl   m_omListCtrlSignal;
    CString m_omStrMessageName;
    UINT    m_unMessageLength;
    CString m_omStrMessageCode;
    UINT    m_unNoOfSgs;
    //int       m_nFrameFormat;
    //int       m_nDataFormat;
    //}}AFX_DATA

    // Attributes
private:
    SDBPARAMS m_sDbParams;
public:
    static SDBPARAMS sm_sDbParams;
    // Operations
public:
    void vEnableButtons (BOOL bEnable );
    BOOL bUpdateEditedMesageDetails();
    BOOL bGetControlStatus();
    void vHideControls(UINT unEnableHide);
    void vSetDefaultValues();
    void vDisplayMsgSgInformation(sMESSAGE* pMsg);

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMsgSgDetView)
public:
    virtual void OnInitialUpdate();
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void PostNcDestroy();
    //}}AFX_VIRTUAL

    // Implementation
protected:
    virtual ~CMsgSgDetView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

    // Generated message map functions
    //{{AFX_MSG(CMsgSgDetView)
    afx_msg void OnClickListSignal(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnRclickLstSignalDetails(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDeleteSignal();
    afx_msg void OnKillfocusEditMsgName();
    afx_msg void OnChangeEditMsgName();
    afx_msg void OnKillfocusEditMsgcode();
    afx_msg void OnKillfocusEditMsgLength();
    afx_msg void OnSetfocusEditMsgName();
    afx_msg void OnSignalNew();
    afx_msg void OnSignalEdit();
    afx_msg void OnRclickListSignal_Desc_Val(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSignal_Desc_Delete();
    afx_msg void OnSignal_Desc_Edit();
    afx_msg void OnSignal_Desc_New();
    afx_msg void OnChangeEditMsgcode();
    afx_msg void OnChangeEditMsgLength();
    afx_msg void OnDblclkLstcSignalDetails(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnButtonDeletesignal();
    afx_msg void OnButtonEditsignal();
    afx_msg void OnButtonNewsignal();
    afx_msg void OnButtonAddDesc();
    afx_msg void OnButtonDeleteDesc();
    afx_msg void OnButtonEditDesc();
    afx_msg void OnClickSignalDescVal(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnItemchangedLstcSignalDetails(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnLvnKeydownLstcSignalDetails(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnKeydownLstcSgidval(NMHDR *pNMHDR, LRESULT *pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    void vEnableNewDescButton(CString );
    void vEnableControls(CString OmStr);
    void vAddItemToSignalList( int nRow,
                               sMESSAGE* pMsg,
                               sSIGNALS* pSg );
    CString m_omStrPrevMsgName;
    BOOL m_bAreControlsHidden;
    void vEnableSignalDetails( BOOL bIsEnabled );
    void vUpDownArrowKeySelection(int nSel);
    int     m_nFrameFormat;
    int     m_nDataFormat;

};
