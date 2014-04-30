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
 * \authors   RBIN/EMC2 - Ratnadip Choudhury, Amarnath Shastri
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
        bool bRet = false;

        bRet = ((m_omMsgName == RefObj.m_omMsgName)&&(m_omSigName == RefObj.m_omSigName));
        return bRet;
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
    LRESULT vConfigureSignals(WPARAM wParam, LPARAM lParam);
    void OnRButtonDown(WPARAM wParam, LPARAM lParam);
    // Dialog Data
    //{{AFX_DATA(CSigWatchDlg)
    enum { IDD = IDD_DLG_SIGNAL_WATCH };
    CSignalWatchListBox m_omSignalList;
    //}}AFX_DATA

    // Construction
    CSigWatchDlg(CWnd* pParent = NULL, ETYPE_BUS eBus = CAN);   // standard constructor
    void vAddMsgToWnd(SSignalInfoArray* sSingnalinfo,BOOL bIntptrDone,map<int,list<string>>* m_mapDetails,unsigned char mID);
    map<int,list<string>>* m_mapMsgIDtoSignallst;
    // Operation
    void vAddMsgSigIntoList(const CString& omStrMsgName,
                            const CStringArray& omSASignals,
                            const CStringArray& omSARaw,
                            const CStringArray& omSAPhysical,
                            BOOL bIntptrDone);
    //void vUpdateWndCo_Ords();
    void vSaveDefaultWinStatus( );
    void vSetDefaultWinStatus( );
    /*void vConfigureSignals();*/
    void vUpdateMainWndHandle(HWND hMainWnd);
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
    afx_msg LRESULT OnReceiveKeyBoardData(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnReceiveKeyDown(WPARAM wParam, LPARAM lParam);


    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    bool m_bEscape;
    ETYPE_BUS m_eBus;
    CSigEntryList m_odSigEntryList;
    CSigEntryList m_odSigEntryListLIN;


    // For Wnd Co-ordinates
    WINDOWPLACEMENT m_sWinCurrStatus;
    INT m_anColWidth[defSW_LIST_COLUMN_COUNT];
    CImageList m_omSigImageList;
    CWnd* m_pParent;
    HWND m_hMainWnd;
private:
    void vDisplayMsgSigList(void);
    CCriticalSection m_omCSDispEntry;
    UINT m_unTimerId;
public:
    afx_msg void OnTimer(UINT nIDEvent);
};
