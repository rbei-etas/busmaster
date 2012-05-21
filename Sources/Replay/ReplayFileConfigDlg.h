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
 * \file      ReplayFileConfigDlg.h
 * \brief     Interface file for CReplayFileConfigDlg class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CReplayFileConfigDlg class
 */

#pragma once

#include "Replay_resource.h"

class CReplayFileConfigDlg : public CDialog
{
    // Construction
public:
    // Constructor
    CReplayFileConfigDlg( CReplayManager& rouManager,
                          const SFILTERAPPLIED_CAN* psFilterConfigured,
                          CWnd* pParent = NULL);

    // Dialog Data
    //{{AFX_DATA(CReplayFileConfigDlg)
    enum { IDD = IDD_DLG_REPLAY };
    CButton m_omChkInteractive;
    CRadixEdit  m_omEditMsgDelay;
    CRadixEdit  m_omEditCycleDelay;
    CButton m_omChkRetainDelay;
    CEdit   m_omEditReplayFileName;
    CListCtrl   m_omLstcReplayFiles;
    int     m_nReplayMode;
    //}}AFX_DATA


    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CReplayFileConfigDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CReplayFileConfigDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnClickListRepalyFiles(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDblclkListRepalyFiles(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnItemchangedListRepalyFiles(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnBtnBrowse();
    afx_msg void OnChkRetainRecordedDelay();
    afx_msg void OnUpdateEditMsgDelay();
    afx_msg void OnRadioReplayModeMono();
    afx_msg void OnUpdateEditCycleDelay();
    afx_msg void OnChkInteractive();
    afx_msg void OnBtnAddFile();
    afx_msg void OnBtnDeleteFile();
    afx_msg void OnBtnFilter();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
private:
    // Methods
    // To Create Image Lists used in this dialog
    BOOL bCreateImageList();
    // To create Replay UI List
    VOID vCreateReplayFileList();
    // To create Replay UI Components
    VOID vCreateReplayCopms();
    // To Initialise Replay UI Components
    VOID vInitReplayCopms();
    // To Initialise Replay UI List
    VOID vInitReplayFileList();
    // To Update Buttons Status
    VOID vEnableDisableButtons();
    // To update selected filter details
    VOID vUpdateReplayFileDetails( int nSelectedItem );
    // To update Replay Components withthe passed replay file details
    VOID vUpdateReplayFileComps( const CReplayFile& rouFile );
    // Enable/Disable Replay File Components
    VOID vEnableReplayComps( BOOL bEnable );

private:
    // Members
    //filter applied
    const SFILTERAPPLIED_CAN* m_psFilterConfigured;
    // Reference to Replay Manager
    CReplayManager& m_rouManager;
    // Image list Object
    CImageList      m_omImageList;
    // To denote Programmed Update
    BOOL            m_bUpdating;
    // To keep track of selected item
    int             m_nSelecetedNamedLogIndex;

};
