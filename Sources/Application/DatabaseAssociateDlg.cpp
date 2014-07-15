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
 * \file      DatabaseAssociateDlg.cpp
 * \brief     Contains the description of the class CDatabaseAssociateDlg
 * \author    GT-Derka
 * \copyright
 *
 * Contains the description of the class CDatabaseAssociateDlg
 */
#include "stdafx.h"
#include "BUSMASTER.h"
#include "DatabaseAssociateDlg.h"
#include "MainFrm.h"

extern CCANMonitorApp theApp;

CChannelDatabaseAssociationDlg::CChannelDatabaseAssociationDlg(const SDBPARAMS& sDbParams, CStringArray* strFilePathArray, CFileDialog* strFileDlg, INT iNumberOfActiveChannels)
    : CDialog(CChannelDatabaseAssociationDlg::IDD)	, m_strFilePathArray(NULL)
{
    m_sDbParams = sDbParams;
    m_strFilePathArray = strFilePathArray;
    m_strFileDialog = strFileDlg;
	m_iNumberOfActiveChannels = iNumberOfActiveChannels;
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CChannelDatabaseAssociationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChannelDatabaseAssociationDlg)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHANNEL_DATABASE_ASSOCIATION_LIST, m_channelDatabaseAssociationListCtrl);
}

void CChannelDatabaseAssociationDlg::OnNMDblclkChannelDatabaseAssociationList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	if ( IDOK == m_strFileDialog->DoModal() )
	{
		BOOL bDatabaseAlreadyAssociated = false;
		for ( INT i = 0; i < m_channelDatabaseAssociationListCtrl.GetItemCount(); i++)
		{
			if( m_channelDatabaseAssociationListCtrl.GetItemText(i, 1) == m_strFileDialog->GetPathName() )
			{
				bDatabaseAlreadyAssociated = true;
				AfxMessageBox("Database already associated!\nSelect another database or delete previsously associated database.");
				break;
			}
		}
		if( bDatabaseAlreadyAssociated == false )
		{
			m_channelDatabaseAssociationListCtrl.SetItemText(pNMItemActivate->iItem, 1, m_strFileDialog->GetPathName());	// show the database-file
			m_strFilePathArray->SetAt( pNMItemActivate->iItem, m_strFileDialog->GetPathName() );
		}
	}

	*pResult = 0;
}

BOOL CChannelDatabaseAssociationDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

	LVCOLUMN lvColumn;
	LVITEM lvItem;
	BYTE byCol;
	BYTE byItem;

	m_channelDatabaseAssociationListCtrl.SetExtendedStyle( LVS_EX_FULLROWSELECT );

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 55;
	lvColumn.pszText = "Channel";
	byCol = (BYTE)m_channelDatabaseAssociationListCtrl.InsertColumn(0, &lvColumn);

	lvColumn.cx = 567;
	lvColumn.pszText = "Database";
	byCol = (BYTE)m_channelDatabaseAssociationListCtrl.InsertColumn(1, &lvColumn);

	lvItem.mask = LVIF_TEXT;
	lvItem.iSubItem = 0;

    CStringArray aomstrTempDBFiles;
    CStringArray aomstrTempChannels;
    (*(CMsgSignal**)(m_sDbParams.m_ppvImportedDBs))->vGetDataBaseNames(&aomstrTempDBFiles);
    (*(CMsgSignal**)(m_sDbParams.m_ppvImportedDBs))->vGetDatabaseChannels(&aomstrTempChannels);

	for(INT i = 1; i <= m_iNumberOfActiveChannels; i++)
	{
		CString tmpChannelNumber;
		tmpChannelNumber.Format("CAN%d", i);			// Format the Bus-Name and Channel-Number
		lvItem.pszText = tmpChannelNumber.GetBuffer(0);	// Convert to LPSTR
		tmpChannelNumber.ReleaseBuffer();				// Release the buffer directly

		lvItem.iItem = i;								// item-position in table

		byItem = (BYTE)m_channelDatabaseAssociationListCtrl.InsertItem(&lvItem);	// insert the new item
	}

    for(INT i = 0; i < m_iNumberOfActiveChannels; i++)
    {
        if(i > aomstrTempChannels.GetUpperBound())
        {
            break;
        }
        int j = atoi(aomstrTempChannels[i]);
        m_channelDatabaseAssociationListCtrl.SetItemText(atoi(aomstrTempChannels[i]) - 1, 1, aomstrTempDBFiles[i]);
    }



    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CChannelDatabaseAssociationDlg, CDialog)
    //{{AFX_MSG_MAP(CChannelDatabaseAssociationDlg)
    //}}AFX_MSG_MAP
	ON_NOTIFY(NM_DBLCLK, IDC_CHANNEL_DATABASE_ASSOCIATION_LIST, CChannelDatabaseAssociationDlg::OnNMDblclkChannelDatabaseAssociationList)
	ON_NOTIFY(LVN_KEYDOWN, IDC_CHANNEL_DATABASE_ASSOCIATION_LIST, CChannelDatabaseAssociationDlg::OnLvnKeydownChannelDatabaseAssociationList)
END_MESSAGE_MAP()

void CCANMonitorApp::OnChannelDatabaseAssociation(CStringArray* strFilePathArray, CFileDialog* fileDlg, INT iNumberOfActiveChannels)
{
    sg_asDbParams[CAN].m_ppvActiveDB = (void**)&(theApp.m_pouMsgSgInactive);
    sg_asDbParams[CAN].m_ppvImportedDBs = (void**)&(theApp.m_pouMsgSignal);

    CChannelDatabaseAssociationDlg channelDatabaseAssociationDlg(sg_asDbParams[CAN], strFilePathArray, fileDlg, iNumberOfActiveChannels);	// Create Dlg-instance
	
    CStringArray strOldFilePathArray; // copy-funktion
    strOldFilePathArray.RemoveAll();
    strOldFilePathArray.Copy(*strFilePathArray);
    
    if(channelDatabaseAssociationDlg.DoModal() == IDOK)	// Show Dlg
    {
        // Check if dissociation is required
        for(INT i = 0; i < iNumberOfActiveChannels; i++)
        {
            //if( strOldFilePathArray.GetAt(i) == (*strFilePathArray).GetAt(i) )
            //{
            //    // Disscociate current channel    // gelöschte dbs zurückgeben lassen und diese löschen
            //    int j = 0;
            //}
        }
    }
    else // Cancel
    {
        strFilePathArray->RemoveAll();
        strFilePathArray->Copy(strOldFilePathArray);
    }
    // Association itself of new databases is located somewhere else
}

void CChannelDatabaseAssociationDlg::OnLvnKeydownChannelDatabaseAssociationList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);

	if( (pLVKeyDow->wVKey == VK_DELETE) || (pLVKeyDow->wVKey == VK_BACK) )	// Delete or Backspace pressed
	{
        AfxMessageBox("To delete Databases use CAN->Database->Dissociate...");
	/*	int nItem = m_channelDatabaseAssociationListCtrl.GetSelectionMark();
		if(nItem > -1)
		{
			m_channelDatabaseAssociationListCtrl.SetItemText(nItem, 1, "");
			m_strFilePathArray->SetAt( nItem, "");
		}*/
	}
	*pResult = 0;
}
