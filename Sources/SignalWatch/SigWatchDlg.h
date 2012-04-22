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
 * \file      SigWatchDlg.h
 * \brief     Interface file for CSigWatchDlg class
 * \authors   Ratnadip Choudhury, Amarnath Shastri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CSigWatchDlg class
 */

#pragma once

// Interface file for CSignalWatchListBox
#include "SignalWatchListBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSigWatchDlg dialog

struct sSIGENTRY
{
    CString m_omMsgName;
    CString m_omSigName;
    CString m_omRawValue;
    CString m_omPhyValue;
    INT    m_nEntryIndex;
    BOOL operator == (const sSIGENTRY& RefObj) const
    {
        return ((m_omMsgName == RefObj.m_omMsgName)&&(m_omSigName == RefObj.m_omSigName));
    }
    sSIGENTRY()
    {
        m_nEntryIndex = -1;
    }
};

typedef CList<sSIGENTRY, sSIGENTRY&> CSigEntryList;
class CSigWatchDlg : public CDialog
{
public:
    void vInitSignalWatchList();
    LRESULT vRemoveSignalFromMap(WPARAM wParam, LPARAM lParam);

// Dialog Data
    //{{AFX_DATA(CSigWatchDlg)
    enum { IDD = IDD_DLG_SIGNAL_WATCH };
    CSignalWatchListBox m_omSignalList;
    //}}AFX_DATA

// Construction
    CSigWatchDlg(CWnd* pParent = NULL);   // standard constructor

// Operation
    void vAddMsgSigIntoList(const CString& omStrMsgName,
                            const CStringArray& omSASignals,
                            const CStringArray& omSARaw,
                            const CStringArray& omSAPhysical,
                            BOOL bIntptrDone);
    //void vUpdateWndCo_Ords();
    void vSaveDefaultWinStatus( );
    void vSetDefaultWinStatus( );
    //void vUpdateWinStatus();
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSigWatchDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CSigWatchDlg)
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnCancel();
    afx_msg void OnOK();
    afx_msg void OnClose();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    virtual BOOL OnInitDialog();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    bool m_bEscape;
    CSigEntryList m_odSigEntryList;
    // For Wnd Co-ordinates
    WINDOWPLACEMENT m_sWinCurrStatus;
    INT m_anColWidth[defSW_LIST_COLUMN_COUNT];
    CImageList m_omSigImageList;
private:
    void vDisplayMsgSigList(void);
    CCriticalSection m_omCSDispEntry;
    UINT m_unTimerId;
public:
    afx_msg void OnTimer(UINT nIDEvent);
};
