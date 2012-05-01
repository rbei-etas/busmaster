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
 * \file      MessageDetailsDlg.h
 * \brief     Interface file for MessageDetailsDlg class
 * \author    Amarnath Shastry
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for MessageDetailsDlg class
 */

#pragma once

#include "MsgSignal.h"
#include "Utility/AlphanumiricEdit.h"
#include "CFilesDefs_CAN.h"
class CMessageDetailsDlg : public CDialog
{
    // Construction
public:
    CMessageDetailsDlg(const SDBPARAMS& sDbParams, sMESSAGE* pMsg = NULL,CWnd* pParent = NULL );   // standard constructor

    // Dialog Data
    //{{AFX_DATA(CMessageDetailsDlg)
    enum { IDD = IDD_MESSAGE_DIALOG };
    CSpinButtonCtrl m_omSpinMsgLen;
    CString m_omStrMessageName;
    CString m_omStrMessageCode;
    UINT    m_unMessageLength;
    int     m_nFrameFormat;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMessageDetailsDlg)
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CMessageDetailsDlg)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    BOOL bDeleteRedundentSignals();
    void vConvertToStandard();
    void vConvertToExtended();
    sMESSAGE* m_psMsgStuctPtr;
    CAlphanumiricEdit m_odMsgNumericEdit;
    SDBPARAMS m_sDbParams;
};
