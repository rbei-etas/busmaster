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
 * \file      ExploreMsgSg.h
 * \brief     This header file contains the defintion of class
 * \author    Amarnath Shastry
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the defintion of class
 */

#pragma once

#include "HashDefines.h"    // #define file
#include "GlobalObj.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CExploreMsgSg : public CDialog
{
    // Construction
public:
    // Holds selected message name
    CString m_omStrMessageName;

    // Holds selected item, message or signal
    CString m_omStrSelectedItemText;

    // Overrided constructor
    CExploreMsgSg(CFunctionEditorDoc* pDoc,
                  CMsgNameMsgCodeList& odMsgNameCodelist,
                  BOOL bCheckRequired,
                  eMESSAGEFROM,
                  eSELTYPE,
                  CWnd* pParent = NULL);

    // Dialog Data
    //{{AFX_DATA(CExploreMsgSg)
    enum { IDD = IDD_MSG_SG_DLG };
    CListBox    m_omSignalListBox;
    CListCtrl   m_omMsgList;
    BOOL    m_bWantStructure;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CExploreMsgSg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CExploreMsgSg)
    afx_msg void OnDblclkListMsgs(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDblclkListSignals();
    afx_msg void OnSelect();
    virtual BOOL OnInitDialog();
    afx_msg void OnCbtnCancel();
    afx_msg void OnItemchangedLstcMsgs(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnClickLstcMsgs(NMHDR* pNMHDR, LRESULT* pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    CFunctionEditorDoc* m_pDoc;
    CMsgNameMsgCodeList& m_odMsgNameCodeList;
    // Dialog class may be called by any window
    eMESSAGEFROM m_eWindow;
    // Flag to enable check box option
    BOOL m_bCheckRequired;
    // enum to indicate whether to
    // select message or signal
    eSELTYPE m_eSelectType;
    // Store last selected message item
    int m_nMsgIndex;
    //Get Signal Names from Message Id
    void vGetSigNamesFromMsgCode(DWORD dwMsgCode, CStringList& omSignalNames);
};
