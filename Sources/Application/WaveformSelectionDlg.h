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
 * \file      WaveformSelectionDlg.h
 * \brief     Interface file for CWaveformSelectionDlg class
 * \author    Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CWaveformSelectionDlg class
 */

#if !defined(AFX_SIGWATCHADDDELDLG_H__4DC6FFE9_566B_47FB_850E_D746C0B37B40__INCLUDED_)
#define AFX_SIGWATCHADDDELDLG_H__4DC6FFE9_566B_47FB_850E_D746C0B37B40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaveformSelectionDlg.h : header file

#include "Resource.h"
#include "WaveFormDataHandler.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CWaveformSelectionDlg dialog

class CWaveformSelectionDlg : public CDialog
{
// Construction
public:
    CWaveformSelectionDlg(CWnd* pParent, CWaveFormDataHandler* pWaveDataHandler, UINT nHardware);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CWaveformSelectionDlg)
    enum { IDD = IDD_DLG_WAVEFORM_CONFIG };
    CListCtrl   m_omListCtrlSignalWatch;
    CListCtrl   m_omListCtrlSignal;
    CComboBox   m_omCombMessage;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CWaveformSelectionDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CWaveformSelectionDlg)
    afx_msg void OnDestroy();
    virtual BOOL OnInitDialog();
    afx_msg void OnSelChangeMessageName();
    afx_msg void OnBtnAddSubEntries();
    afx_msg void OnBtnDelSubEntires();
    afx_msg void OnClickUnSelSubEntryList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDblclkUnSelSubEntryList(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnClickSelSubEntryList(NMHDR* pNMHDR, LRESULT* pResult);        
    afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
	//Pointer to CWaveFormDataHandler object
	CWaveFormDataHandler* m_pWaveDataHandler;
    // Pointer to Temp List
    CMainEntryList* m_podTempCallerList;
    CMainEntryList* m_podMainCallerList;
    // Image list
    CImageList m_omImageList;
    // CString Array for Data link
    CStringArray m_omCSAData;

	UINT m_nHardware;

    //Functions
    // To update the signal list with signals of message ID unMsgID
    void vPopulateUnSelSubEntryList(UINT unMainEntryID);
    // Add/Add All the signals selected in to the temp list
    void vAddSelSubEntries(BOOL bAllSignals);
    // Delete/Delete All the signals selected from the temp list
    void vDelSelSubEntries(BOOL bAllSignals);
    // Get the selected message ID
    UINT unGetSelectedMainEntryID();
    // Delete temp list. Called from Close and Delete All
    BOOL bDeleteTempList();
    // Fun to get Msg and Signal name from Signal Watch List
    BOOL bGetMainSubName(const CString &omMsgStr, 
                           CString &omMsg, 
                           CString &omSig); 
    // For deleting a signal from the Signal Watch List
    BOOL bDeleteSubEntry(UINT unMsgID, const CString& omSignal);
    // For enabling/ diabling buttons
    void vEnableDisableButtons();
    // GUI Interface - To get selected Signals to Add
    void vUpdateUnSelSubEntryList(BOOL bAllSignals);
    // GUI Interface - To get selected Watch Signals to delete
    void vUpdateSelSubEntryList(BOOL bAllSignals);
    // To populate signal watch list
    void vPopulateSelSubEntryList();
    //Extract Id from Message name [MsgName0xID]
    UINT unGetMainEntryIDFromName(CString omMsgName);
	void DefineUpdateWave(CListCtrl* pListCtrl,  UINT nMsgID, 
						  CString strSignalName,sWaveformInfo& objWaveInfo);
	void vInterPretSignalNameMsgID(CString strInterpretData, CString& strSigName, UINT& nMsgID);
public:
	afx_msg void OnLvnItemchangedLstcSignal(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkLstcSignalWatch(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	float m_fDefAmplitude;
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEditDefaultSignalValue();
	CComboBox m_omMsgChannel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIGWATCHADDDELDLG_H__4DC6FFE9_566B_47FB_850E_D746C0B37B40__INCLUDED_)
