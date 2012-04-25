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
 * \file      DatabaseDissociateDlg.cpp
 * \brief     Contains the description of the class CDatabaseDissociateDlg
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Contains the description of the class CDatabaseDissociateDlg
 */
#include "stdafx.h"
#include "BUSMASTER.h"
#include "DatabaseDissociateDlg.h"
#include "MainFrm.h"

extern CCANMonitorApp theApp;

// CDatabaseDissociateDlg dialog

IMPLEMENT_DYNAMIC(CDatabaseDissociateDlg, CDialog)
CDatabaseDissociateDlg::CDatabaseDissociateDlg(const SDBPARAMS& sDbParams, CWnd* pParent /*=NULL*/)
	: CDialog(CDatabaseDissociateDlg::IDD, pParent)
{
    m_sDbParams = sDbParams;
}

CDatabaseDissociateDlg::~CDatabaseDissociateDlg()
{
}

void CDatabaseDissociateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LSTB_DISSOCIATE_DBNAMES, m_omDissociateDbLst);
}


BEGIN_MESSAGE_MAP(CDatabaseDissociateDlg, CDialog)
	ON_BN_CLICKED(IDC_CBTN_DISSOCIATE, OnBnClickedCbtnDissociate)
END_MESSAGE_MAP()


/******************************************************************************
  Function Name    :  OnInitDialog
                                                                            
  Input(s)         :  
  Output           :                                                    
  Functionality    :  Initialze the contents of List box 
  Member of        :  CDatabaseDissociateDlg                                        
  Friend of        :      -                                                 
                                                                            
  Author(s)        :  Anish Kumar                                      
  Date Created     :  06.12.2006                                            
  Modifications    :  
******************************************************************************/
BOOL CDatabaseDissociateDlg::OnInitDialog() 
{
    CDialog::OnInitDialog();
	//do initialization below
	m_omDissociateDbLst.ResetContent();
	CStringArray aomstrTempDBFiles;
    (*(CMsgSignal**)(m_sDbParams.m_ppvImportedDBs))->vGetDataBaseNames(&aomstrTempDBFiles);
	int nFileCount = aomstrTempDBFiles.GetSize();
	CSize   sz;
	CString omStrText;
	CDC*  pDC = m_omDissociateDbLst.GetDC();
	int nDx = 0;
	for(int nCount = 0;nCount < nFileCount;nCount++)
	{
		omStrText = aomstrTempDBFiles.GetAt(nCount);
		m_omDissociateDbLst.AddString(omStrText);
		sz = pDC->GetTextExtent(omStrText);

		if (sz.cx > nDx)
			nDx = sz.cx;

	}
	m_omDissociateDbLst.ReleaseDC(pDC);
	// Set the horizontal extent so every character of all strings 
	// can be scrolled to.
	m_omDissociateDbLst.SetHorizontalExtent(nDx);
	return TRUE;
}

/******************************************************************************
  Function Name    :  OnBnClickedCbtnDissociate
                                                                            
  Input(s)         :  
  Output           :                                                    
  Functionality    :  Call the functions to remove the selected Databases 
  Member of        :  CDatabaseDissociateDlg                                        
  Friend of        :      -                                                 
                                                                            
  Author(s)        :  Anish Kumar                                      
  Date Created     :  06.12.2006                                            
  Modifications    :  
******************************************************************************/
void CDatabaseDissociateDlg::OnBnClickedCbtnDissociate()
{
	//TO store the path of files dissociated
	CStringArray aomStrFilesDissociated;
	CMainFrame* pMainFrame = static_cast<CMainFrame*> (theApp.m_pMainWnd);
	// Get the indexes of all the selected items.
	int nCount = m_omDissociateDbLst.GetSelCount();
	if(nCount > 0)
	{
		// Array of selected item's position
		CArray<int,int> aomListBoxSel;
		aomListBoxSel.SetSize(nCount);
		//Pass the array pointer to get the selected item's positions
		m_omDissociateDbLst.GetSelItems(nCount, aomListBoxSel.GetData());
		aomStrFilesDissociated.RemoveAll();
		for(int nTempCnt = 0 ; nTempCnt < nCount ; nTempCnt++)
		{
			BOOL bDBDeleted = FALSE;
			CString omstrDBPath ;
			//Selected file's index
			int nSelectedPos = aomListBoxSel.GetAt(nTempCnt);
			//Find the length of string to pass the buffer to have the selected File path
			int nBufferSize = m_omDissociateDbLst.GetTextLen(nSelectedPos);
			m_omDissociateDbLst.GetText(nSelectedPos,omstrDBPath.GetBuffer(nBufferSize));
			bDBDeleted = (*(CMsgSignal**)(m_sDbParams.m_ppvImportedDBs))->bDeAllocateMemory(omstrDBPath.GetBuffer(0));
			if(TRUE == bDBDeleted)
			{
				aomStrFilesDissociated.Add(omstrDBPath.GetBuffer(0));
			}
		}
		//To remove from theApp class
		CStringArray aomstrDBFiles;
		(*(CMsgSignal**)(m_sDbParams.m_ppvImportedDBs))->vGetDataBaseNames(&aomstrDBFiles);
		//Delete the file path from the List box
		int nTotalCount = aomStrFilesDissociated.GetSize();
		CString omStrTempFile;
		for(int nCount=0 ; nCount<nTotalCount ; nCount++)
		{
			omStrTempFile = aomStrFilesDissociated.GetAt(nCount);
			int nIndex = 0;
			if( (nIndex = m_omDissociateDbLst.FindString(0,
				omStrTempFile)) != LB_ERR )
			{
				//Delete the file path from the list box
				m_omDissociateDbLst.DeleteString(nIndex);
				int nStoredFile = aomstrDBFiles.GetSize();
				CString omStrTemp;
				BOOL bRemoved = FALSE;
				for(int nTemp = 0 ; nTemp < nStoredFile && bRemoved != TRUE; nTemp++)
				{
					omStrTemp = aomstrDBFiles.GetAt(nTemp);
					if(!(omStrTemp.Compare(omStrTempFile)))
					{
						aomstrDBFiles.RemoveAt(nTemp);
						bRemoved = TRUE;
					}
				}
			}
		}
		//Set the new file name array
        (*(CMsgSignal**)(m_sDbParams.m_ppvImportedDBs))->vSetDataBaseNames(&aomstrDBFiles);
		// Send a message to Tx Window about database change
		if( pMainFrame != NULL)
		{
			eUSERSELCTION eUserSel = eDATABASEIMPORTCMD;
			pMainFrame->m_objTxHandler.vPostMessageToTxWnd(WM_USER_CMD, (WPARAM)eUserSel,0);
		}
		////Delete Signal watch list and Graph window list
		//// Check for Signal Watch & DLL load Condition
		//
        BOOL bUserOption = FALSE;
		if(pMainFrame->m_psSignalWatchList != NULL)
		{
			if(theApp.m_bFromAutomation == FALSE)
			bUserOption = AfxMessageBox(defIMPORT_WARNING,
				MB_YESNO | MB_DEFBUTTON1 | MB_ICONQUESTION) ==
				IDYES;			
			// If user wants to clear
			if(bUserOption == TRUE )
			{
				// Clear Signal Watch List
				pMainFrame->vUpdateSWList();
			}
		}
		//Added by Arun to update Data Handler Main entry list.		
		pMainFrame->vUpdateMainEntryListInWaveDataHandler();
		pMainFrame->vClearSignalInfoList();
		if(!pMainFrame->m_ouWaveTransmitter.bIsBlockEnabled())
			theApp.pouGetFlagsPtr()->vSetFlagStatus( SEND_SIGNAL_MSG, FALSE );

		//Update Message windows
		pMainFrame->vUpdateAllMsgWndInterpretStatus(FALSE);

		// Check for Graph list
		for(register int nBusID = CAN; nBusID < AVAILABLE_PROTOCOLS; nBusID++)
		{
			if( pMainFrame->m_odGraphList[nBusID].m_omElementList.GetSize() > 0 )
			{
				// Get the delete confirmation from the user
				if(theApp.m_bFromAutomation == FALSE)
					bUserOption = AfxMessageBox(defIMPORT_WARNING_GRAPH,
					MB_YESNO | MB_DEFBUTTON1 | MB_ICONQUESTION) ==
					IDYES;

				// If user wants to clear
				if(bUserOption == TRUE )
				{
					// Clear Graph List for all buses.
					for(register int nID = CAN; nID < AVAILABLE_PROTOCOLS; nID++)
						pMainFrame->m_odGraphList[nID].m_omElementList.RemoveAll();

					// Send the Message to the Left View to Update List for all buses
					if( pMainFrame != NULL )
					{
						pMainFrame->vPostConfigChangeCmdToSigGrphWnds();
					}
				}
				break;
			}
		}
	}
}
