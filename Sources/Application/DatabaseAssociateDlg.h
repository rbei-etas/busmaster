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
 * \file      DatabaseAssociateDlg.h
 * \brief     This header file contains the defintion of class CDatabaseAssociateDlg
 * \author    GT-Derka
 * \copyright 
 *
 * This header file contains the defintion of class CDatabaseAssociateDlg
 */
#pragma once
#include "Datatypes/MsgSignal_Datatypes.h"

// CChannelDatabaseAssociationDlg dialog

class CChannelDatabaseAssociationDlg : public CDialog
{
public:
    CChannelDatabaseAssociationDlg(const SDBPARAMS& sDbParams, CStringArray* strFilePathArray, CFileDialog* fileDlg, INT iNumberOfActiveChannels);

    // Dialog Data
    //{{AFX_DATA(CChannelDatabaseAssociationDlg)
    enum { IDD = IDD_DLG_CHANNEL_DATABASE_ASSOCIATION };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CChannelDatabaseAssociationDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    //{{AFX_MSG(CChannelDatabaseAssociationDlg)
    virtual BOOL OnInitDialog();
	INT m_iNumberOfActiveChannels;
    //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
private:
    CListCtrl m_channelDatabaseAssociationListCtrl;
	CStringArray* m_strFilePathArray;
	CFileDialog* m_strFileDialog;
    SDBPARAMS m_sDbParams;
public:
	afx_msg void OnNMDblclkChannelDatabaseAssociationList(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLvnKeydownChannelDatabaseAssociationList(NMHDR *pNMHDR, LRESULT *pResult);
};