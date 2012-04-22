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
 * \file      WaveformSelectionDlg.cpp
 * \brief     Implementation file for CWaveformSelectionDlg class
 * \author    Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CWaveformSelectionDlg class
 */
 

#include "stdafx.h"
#include "HashDefines.h"
#include "SignalWatch/SignalWatchDefs.h"
#include "WaveformSelectionDlg.h"
#include "DataTypes/MsgSignal_DataTypes.h"
#include "CSignalGeneration.h"
#include "Utility/WaitIndicator.h"
#include "SignalDefiner/SignalDefiner_Extern.h"
#include <atlconv.h>

/////////////////////////////////////////////////////////////////////////////
// CWaveformSelectionDlg dialog


CWaveformSelectionDlg::CWaveformSelectionDlg(CWnd* pParent, CWaveFormDataHandler* pWaveDataHandler, UINT nHardware)
    : CDialog(CWaveformSelectionDlg::IDD, pParent)
	, m_fDefAmplitude(0)
{
    //{{AFX_DATA_INIT(CWaveformSelectionDlg)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_pWaveDataHandler = pWaveDataHandler;    
	m_nHardware = nHardware;
}


void CWaveformSelectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaveformSelectionDlg)
	DDX_Control(pDX, IDC_LSTC_SIGNAL_WATCH, m_omListCtrlSignalWatch);
	DDX_Control(pDX, IDC_LSTC_SIGNAL, m_omListCtrlSignal);
	DDX_Control(pDX, IDC_COMB_MSGNAME, m_omCombMessage);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_DEFAULT_SIGNAL_VALUE, m_fDefAmplitude);
	DDX_Control(pDX, IDC_COMB_MSG_CHANNEL, m_omMsgChannel);
}


BEGIN_MESSAGE_MAP(CWaveformSelectionDlg, CDialog)
    //{{AFX_MSG_MAP(CWaveformSelectionDlg)
    ON_WM_DESTROY()
    ON_CBN_SELCHANGE(IDC_COMB_MSGNAME, OnSelChangeMessageName)
    ON_BN_CLICKED(IDM_SIGNALDLG_ADD, OnBtnAddSubEntries)
    ON_BN_CLICKED(IDM_SIGNALDLG_DELETE, OnBtnDelSubEntires)
    ON_NOTIFY(NM_CLICK, IDC_LSTC_SIGNAL, OnClickUnSelSubEntryList)
    ON_NOTIFY(NM_DBLCLK, IDC_LSTC_SIGNAL, OnDblclkUnSelSubEntryList)
    ON_NOTIFY(NM_CLICK, IDC_LSTC_SIGNAL_WATCH, OnClickSelSubEntryList)    
    ON_COMMAND(IDM_SIGNALDLG_ADD, OnBtnAddSubEntries)    
    ON_COMMAND(IDM_SIGNALDLG_DELETE, OnBtnDelSubEntires)    
    ON_WM_HELPINFO()
    //}}AFX_MSG_MAP
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSTC_SIGNAL, OnLvnItemchangedLstcSignal)
	ON_NOTIFY(NM_DBLCLK, IDC_LSTC_SIGNAL_WATCH, OnNMDblclkLstcSignalWatch)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_DEFAULT_SIGNAL_VALUE, OnEnChangeEditDefaultSignalValue)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaveformSelectionDlg message handlers

/*******************************************************************************
Function Name    : OnInitDialog
Input(s)         : 
Output           : 
Functionality    : This function will be called during initialization of dialog
                   box. This function will fill respective list controls
Member of        : CWaveformSelectionDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
*******************************************************************************/
BOOL CWaveformSelectionDlg::OnInitDialog() 
{
    // Call parent's init function
    CDialog::OnInitDialog();    

    // Populate the message name combo
    // Clear the content if any
    m_omCombMessage.ResetContent();
    // Create the Image List
    m_omImageList.Create(IDR_BMP_MSG_SIG_WATCH,16,1, defCOLOR_WHITE);

	CStringArray arrMsgIDsList;
	m_pWaveDataHandler->vGetCompleteMsgList(arrMsgIDsList);
	for(int i = 0; i<arrMsgIDsList.GetSize(); i++)
		m_omCombMessage.AddString(arrMsgIDsList.GetAt(i));
	m_omCombMessage.SetCurSel(0);
	m_omCombMessage.SetFocus();	

	m_omListCtrlSignal.SetImageList(&m_omImageList, LVSIL_SMALL);    
    // Create the first column. But this will not be shown
    m_omListCtrlSignal.InsertColumn( 0, STR_EMPTY, LVCFMT_LEFT,
                                        0); //Width is zero as this will 
											//be updated in vUpdateUnSelSubEntryList
	// Populate the list with signal names
	vPopulateUnSelSubEntryList(unGetSelectedMainEntryID());	

	UINT nMsgID = unGetSelectedMainEntryID();
	m_fDefAmplitude = m_pWaveDataHandler->fGetMsgIDDefaultValue(nMsgID);

    // Update Signal Watch List
    // Set the same image list to this control
    m_omListCtrlSignalWatch.SetImageList(&m_omImageList,LVSIL_SMALL);
    // Create Column for Signal Watch Lsit
    m_omListCtrlSignalWatch.InsertColumn( 0,
                                     STR_EMPTY,
                                     LVCFMT_LEFT,
                                     0);//Width is zero as this will 
                                        //be updated in vPopulateSelSubEntryList
    vPopulateSelSubEntryList();
    vEnableDisableButtons();	

	 // Channels
    m_omMsgChannel.ResetContent();
    // Add All Channels as first item
    m_omMsgChannel.AddString(defSTR_SELECTION_ALL);
    for (UINT Index = 1; Index <= m_nHardware; Index++)
    {
        CString omStr;
        // Format Channel ID String Say 1,2,...
        omStr.Format(defFORMAT_MSGID_DECIMAL, Index);
        m_omMsgChannel.AddString(omStr);
    }    
	m_omMsgChannel.SetCurSel(1); //Make Channel 1 default.
	
	//Temporarily disable the channel selection feature.
	m_omMsgChannel.EnableWindow(FALSE);	
	GetDlgItem(IDC_STATIC_CHANNEL)->EnableWindow(FALSE);

	UpdateData(FALSE);
	

    return FALSE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

/*******************************************************************************
Function Name    : vUpdateUnSelSubEntryList
Input(s)         : unMsgID - Message ID
Output           : 
Functionality    : This function will be called to update signal names list when
                   the message name selected got changed or during init to 
                   populate the Signal Names listbox
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modifications    : K ArunKumar
				 : 24.08.2010. Modifications to support data population 
				   from Wave form Data Handler.
				   Raja N
                 : 14.04.2004. Modifications as per Code Review.
*******************************************************************************/
void CWaveformSelectionDlg::vPopulateUnSelSubEntryList(UINT unMainEntryID)
{    
    m_omListCtrlSignal.DeleteAllItems();
	CStringArray arrSignalsList;
	//UINT nMsgID = 
    unGetSelectedMainEntryID();
	m_pWaveDataHandler->vGetAvailableSignalsInMsgID(unMainEntryID, arrSignalsList,true);

	UINT unMaxLen = 0;
	for(int nCount = 0; nCount<arrSignalsList.GetSize(); nCount++)
	{		
		UINT unLen = m_omListCtrlSignal.GetStringWidth(arrSignalsList.GetAt(nCount));
		// Save the length if it is the max.
        if(unMaxLen < unLen )        
            unMaxLen = unLen;
		m_omListCtrlSignal.InsertItem(nCount, arrSignalsList.GetAt(nCount));		
	}
	// Set the column width if it is valid
    if( unMaxLen > 0 )
    {
        m_omListCtrlSignal.SetColumnWidth(0, unMaxLen + defSIZE_OF_IMAGE);
    }
}

/*******************************************************************************
Function Name    : OnDestroy
Input(s)         : 
Output           : 
Functionality    : This function will be called by framework when dialog 
                   is destroyed. It will release the memory allocated
Member of        : CWaveformSelectionDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modification By  : 
Modification on  : 
*******************************************************************************/
void CWaveformSelectionDlg::OnDestroy() 
{
    CDialog::OnDestroy();    
}

/*******************************************************************************
Function Name    : OnSelChangeMessageName
Input(s)         : 
Output           : 
Functionality    : This function will be called by framework when the message 
                   name combo box selection got changed. This will update the 
                   Signal Names List control
Member of        : CWaveformSelectionDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modification By  : 
Modification on  : 
*******************************************************************************/
void CWaveformSelectionDlg::OnSelChangeMessageName() 
{
	UINT nMsgID=unGetSelectedMainEntryID();
	m_fDefAmplitude=m_pWaveDataHandler->fGetMsgIDDefaultValue(nMsgID);
    vPopulateUnSelSubEntryList(unGetSelectedMainEntryID());
    vEnableDisableButtons();
	UpdateData(FALSE);
}

/*******************************************************************************
Function Name    : unGetSelectedMainEntryID
Input(s)         : -
Output           : 
Functionality    : This function will return the message id of the message name
                   selected in the combo box. This will be called to get the id
                   and it will get the id from active database
Member of        : CWaveformSelectionDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modification By  : 
Modification on  : 
*******************************************************************************/
UINT CWaveformSelectionDlg::unGetSelectedMainEntryID()
{
    // Assign init value to an invalid one
    UINT unMainEntryId = (UINT)-1;
    CString omMainEntryName;
    
    // Check for visible window
    if( m_omCombMessage.GetSafeHwnd() != NULL )
    {
        m_omCombMessage.GetWindowText(omMainEntryName);
    }
    // Get the selected message name
    unMainEntryId = unGetMainEntryIDFromName(omMainEntryName);
    return unMainEntryId;

}

/*******************************************************************************
Function Name    : OnBtnAddSubEntries
Input(s)         :  -
Output           :  -
Functionality    : This function will be called by framework when the user 
                   selects "Add" button. This will call "vAddSelSubEntries" to
                   add the selected sub entries
Member of        : CWaveformSelectionDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modifications    : Raja N
                 : 14.04.2004. Modifications as per Code Review.
*******************************************************************************/
void CWaveformSelectionDlg::OnBtnAddSubEntries() 
{
	sWaveformInfo objWaveInfo;
	m_pWaveDataHandler->vGetDefaultSignalWaveValues(objWaveInfo);
	DefineUpdateWave(&m_omListCtrlSignal, unGetSelectedMainEntryID(), 
				     m_omListCtrlSignal.GetItemText(m_omListCtrlSignal.GetSelectionMark() , 0),
					 objWaveInfo);
	vPopulateUnSelSubEntryList(unGetSelectedMainEntryID());
	vPopulateSelSubEntryList();
	// Update button status
	vEnableDisableButtons();
}

/*******************************************************************************
Function Name    : DefineUpdateWave
Input(s)         :  -
Output           :  -
Functionality    : This function defines waveform and updates existing.
Member of        : CWaveformSelectionDlg
Friend of        :  -
Author(s)        : K ArunKumar
Date Created     : 24.08.2010
Modification By  : 
Modification on  : 
*******************************************************************************/
void CWaveformSelectionDlg::DefineUpdateWave(CListCtrl* pListCtrl, UINT nMsgID, 
                             CString strSignalName, sWaveformInfo& objWaveInfo)
{
	if (pListCtrl->GetSelectionMark() != -1)
	{				
		CString strSignalDetails;
		strSignalDetails.Format(_T("0x%x->%s"), nMsgID, strSignalName);
				
		//BSTR bstrName = (BSTR)strSignalDetails.GetBuffer(MAX_PATH);		
		BSTR bstrName(L"");
		
		SignalDefiner_SetSignalDetails(bstrName);		

		SignalDefiner_SetType((SIGNAL_TYPE)objWaveInfo.m_eSignalWaveType);
		SignalDefiner_SetAmplitude(objWaveInfo.m_fAmplitude);
		SignalDefiner_SetFrequency(objWaveInfo.m_fFrequency);
		SignalDefiner_SetSamplingTimePeriod(m_pWaveDataHandler->shGetSamplingTimePeriod());

		if ( SignalDefiner_ShowDlg() == IDOK )
		{
			SIGNAL_TYPE enSelSignalType;
			SignalDefiner_GetType(&enSelSignalType);
			objWaveInfo.m_eSignalWaveType = (eWAVEFORMTYPE)enSelSignalType;
			SignalDefiner_GetAmplitude(&objWaveInfo.m_fAmplitude);
			SignalDefiner_GetFrequency(&objWaveInfo.m_fFrequency);

			if(pListCtrl == &m_omListCtrlSignal)
				m_pWaveDataHandler->bAddSignalToDefinedWavesList(nMsgID, strSignalName, objWaveInfo);
			else
				m_pWaveDataHandler->vSetSignalWavePatternDetails(nMsgID, strSignalName, objWaveInfo);

            // Force a setting of sampling time period.typecast to remove compiler warning
			int nTimePeriod;
			SignalDefiner_GetSamplingTimePeriod(&nTimePeriod);
			m_pWaveDataHandler->vSetSamplingTimePeriod(nTimePeriod);
		}
		//Free the BSTR memory allocated
		::SysFreeString(bstrName);
	}
}

/*******************************************************************************
Function Name    : vInterPretSignalNameMsgID
Input(s)         :  strInterpretData, strSigName, nMsgID
Output           :  -
Functionality    : This function interprets signal name, msgId
Member of        : CWaveformSelectionDlg
Friend of        :  -
Author(s)        : K ArunKumar
Date Created     : 24.08.2010
Modification By  : 
Modification on  : K ArunKumar
                   08.09.2010,
				   Updated the function to interpret data according to format:
				   **** MsgID->Signal Name (Waveform Name) *****
*******************************************************************************/
void CWaveformSelectionDlg::vInterPretSignalNameMsgID(CString strInterpretData, CString& strSigName, UINT& nMsgID)
{
	CString omMsgId;	
	TCHAR* pcStopStr = NULL;
    int nIndex = -1;
	nIndex = strInterpretData.Find(_T("->"));
	if(nIndex != -1)
	{
		omMsgId = strInterpretData.Left(nIndex);
		nMsgID = _tcstol((LPCTSTR )omMsgId,&pcStopStr,16);
		CString strTemp;
		strTemp= strInterpretData.Right(strInterpretData.GetLength()-(nIndex+2));
		//Now remove the Wave form name.
		nIndex = strTemp.Find(_T("("));
		if(nIndex!=-1)
			strSigName = strTemp.Left(nIndex-1);
		else
			strSigName = strTemp;
	}
}

/*******************************************************************************
Function Name    : OnBtnDelSubEntires
Input(s)         :  -
Output           :  -
Functionality    : This function will be called by framework when the user
                   selects "Delete" button. This will call
                   "vDeleteSelectedSignals" to delete the selected signals. If
                   the user selectes all the signals it will do delete all.
Member of        : CWaveformSelectionDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modification By  : 
Modification on  : 
*******************************************************************************/
void CWaveformSelectionDlg::OnBtnDelSubEntires() 
{
    BOOL bDeleteAll = FALSE;
    // Check the selection count and if that is equal to total number just 
    // all. No need to process the data
    if(static_cast<int>(m_omListCtrlSignalWatch.GetSelectedCount()) ==
            m_omListCtrlSignalWatch.GetItemCount())
    {
        bDeleteAll = TRUE;
    }

	CString strSignalName;
	UINT nMsgID = (UINT)-1;			
	vInterPretSignalNameMsgID(m_omListCtrlSignalWatch.GetItemText(
		                   m_omListCtrlSignalWatch.GetSelectionMark() , 0), strSignalName, nMsgID);	
	m_pWaveDataHandler->bRemoveSignalFromDefinedWavesList(nMsgID, strSignalName);
	vPopulateUnSelSubEntryList(unGetSelectedMainEntryID());
	vPopulateSelSubEntryList();    
    vEnableDisableButtons();
}

/*******************************************************************************
Function Name    : OnClickUnSelSubEntryList
Input(s)         : NMHDR* pNMHDR, LRESULT* pResult - Handler parameter
Output           :  -
Functionality    : This function will be called by framework when the user 
                   clicks on Signal Watch box. This will update the status of
                   the Add button as per the current selection
Member of        : CWaveformSelectionDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modification By  : 
Modification on  : 
*******************************************************************************/
void CWaveformSelectionDlg::OnClickUnSelSubEntryList(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
    if(m_omListCtrlSignal.GetSelectedCount() == 0)
    {
        GetDlgItem(IDM_SIGNALDLG_ADD)->EnableWindow(FALSE);
    }
    else
    {
        GetDlgItem(IDM_SIGNALDLG_ADD)->EnableWindow(TRUE);
    }
    *pResult = 0;
}

/*******************************************************************************
Function Name    : OnDblclkUnSelSubEntryList
Input(s)         : NMHDR* pNMHDR, LRESULT* pResult - Handler parameter
Output           :  -
Functionality    : This function will be called by framework when the user 
                   double clicks on Signal List box. This will call 
                   "vAddSelectedSignals" to add the selected entry and will
                   update the button status by calling "vEnableDisableButtons"
Member of        : CWaveformSelectionDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modifications    : Raja N
                 : 14.04.2004. Modifications as per Code Review.
*******************************************************************************/
void CWaveformSelectionDlg::OnDblclkUnSelSubEntryList(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{	
    if(m_omListCtrlSignal.GetSelectedCount() != 0)
    {
		sWaveformInfo objWaveInfo;
		m_pWaveDataHandler->vGetDefaultSignalWaveValues(objWaveInfo);
		DefineUpdateWave(&m_omListCtrlSignal, unGetSelectedMainEntryID(), 
		  		         m_omListCtrlSignal.GetItemText(m_omListCtrlSignal.GetSelectionMark() , 0),
					     objWaveInfo);
		vPopulateUnSelSubEntryList(unGetSelectedMainEntryID());
		vPopulateSelSubEntryList();
		// Update button status
        vEnableDisableButtons();
    }
    *pResult = 0;
}

/*******************************************************************************
Function Name    : OnClickSelSubEntryList
Input(s)         : NMHDR* pNMHDR, LRESULT* pResult - Handler parameter
Output           :  -
Functionality    : This function will be called by framework when the user 
                   clicks on Signal Watch List box. This will update the status
                   of the Delete button as per the current selection
Member of        : CWaveformSelectionDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modification By  : 
Modification on  : 
*******************************************************************************/
void CWaveformSelectionDlg::OnClickSelSubEntryList(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    if(m_omListCtrlSignalWatch.GetSelectedCount() == 0)
    {
        GetDlgItem(IDM_SIGNALDLG_DELETE)->EnableWindow(FALSE);
    }
    else
    {
        GetDlgItem(IDM_SIGNALDLG_DELETE)->EnableWindow(TRUE);
    }

    
    *pResult = 0;
}

/*******************************************************************************
Function Name    : vEnableDisableButtons
Input(s)         :  - 
Output           : 
Functionality    : This UI function will be called to update the status of the
                   buttons in this dialog. This will check the entries in the
                   Signal & Signal Watch List boxes. According to that it will
                   Enable/Disable the buttons. This function will be called from
                   the Init and whenever there is a change in these list boxes.
Member of        : CWaveformSelectionDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modification By  : 
Modification on  : 
*******************************************************************************/
void CWaveformSelectionDlg::vEnableDisableButtons()
{
    // Signal List Operation
    if( m_omListCtrlSignal.GetItemCount() > 0)
    {
        GetDlgItem(IDM_SIGNALDLG_ADDALL)->EnableWindow(TRUE);
        if( m_omListCtrlSignal.GetSelectedCount() > 0)
        {
            GetDlgItem(IDM_SIGNALDLG_ADD)->EnableWindow(TRUE);
        }
        else
        {
            GetDlgItem(IDM_SIGNALDLG_ADD)->EnableWindow(FALSE);
        }
    }
    else
    {
        GetDlgItem(IDM_SIGNALDLG_ADDALL)->EnableWindow(FALSE);
        GetDlgItem(IDM_SIGNALDLG_ADD)->EnableWindow(FALSE);
    }

    // Signal Watch Lsit operation
    if( m_omListCtrlSignalWatch.GetItemCount() > 0)
    {
        GetDlgItem(IDM_SIGNALDLG_DELETEALL)->EnableWindow(TRUE);
        if( m_omListCtrlSignalWatch.GetSelectedCount() > 0)
        {
            GetDlgItem(IDM_SIGNALDLG_DELETE)->EnableWindow(TRUE);
        }
        else
        {
            GetDlgItem(IDM_SIGNALDLG_DELETE)->EnableWindow(FALSE);
        }
    }
    else
    {
        GetDlgItem(IDM_SIGNALDLG_DELETEALL)->EnableWindow(FALSE);
        GetDlgItem(IDM_SIGNALDLG_DELETE)->EnableWindow(FALSE);
    }


}

/*******************************************************************************
Function Name    : vPopulateSelSubEntryList
Input(s)         :  -
Output           :  -
Functionality    : This function will update the signal watch list box with the
                    entries in the signal watch list.
Member of        : CWaveformSelectionDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modifications    : Raja N
                 : 14.04.2004. Modifications as per Code Review.
*******************************************************************************/
void CWaveformSelectionDlg::vPopulateSelSubEntryList()
{
    m_omListCtrlSignalWatch.DeleteAllItems();

	CStringArray arrAllDefinedSignals;	
	m_pWaveDataHandler->vGetAllDefinedSignalsNames(arrAllDefinedSignals);

	UINT unMaxLen = 0;
	for(int nCount = 0; nCount<arrAllDefinedSignals.GetSize(); nCount++)
	{		
		UINT unLen = m_omListCtrlSignalWatch.GetStringWidth(arrAllDefinedSignals.GetAt(nCount));
		// Save the length if it is the max.
        if(unMaxLen < unLen )        
            unMaxLen = unLen;
		m_omListCtrlSignalWatch.InsertItem(nCount, arrAllDefinedSignals.GetAt(nCount));		
	}
	// Set the column width if it is valid
    if( unMaxLen > 0 )
    {
        m_omListCtrlSignalWatch.SetColumnWidth(0, unMaxLen + defSIZE_OF_IMAGE);
    }
}

/*******************************************************************************
Function Name    : OnHelpInfo
Input(s)         : Pointer to object of structure for help request info
Output           :
Functionality    : This function is called by the framework in response
                   to the WM_HELPINFO message. This message is sent to
                   the dialog box during help request from user
Member of        : CWaveformSelectionDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 02/04/2004
Modifications    :
*******************************************************************************/
BOOL CWaveformSelectionDlg::OnHelpInfo(HELPINFO* /*pHelpInfo*/) 
{
    //vSetHelpID(pHelpInfo->dwContextId);
    //return CDialog::OnHelpInfo(pHelpInfo);
    return FALSE;
}

UINT CWaveformSelectionDlg::unGetMainEntryIDFromName(CString omMainEntryName)
{
	CString omMainEntryId;
	UINT unMainEntryID = (UINT)-1;
	TCHAR* pcStopStr = NULL;
    int nIndex = omMainEntryName.Find(defMSGID_EXTENDED);
    int nCloseBraceIndex = omMainEntryName.Find(defMSG_NAME_END_CHAR);
	if((nIndex != -1) && (nCloseBraceIndex != -1))
	{
		omMainEntryId = omMainEntryName.Mid(nIndex + 1, nCloseBraceIndex - (nIndex + 1));
		unMainEntryID = _tcstol((LPCTSTR )omMainEntryId,&pcStopStr,16);
	}
	return unMainEntryID;
}
void CWaveformSelectionDlg::OnLvnItemchangedLstcSignal(NMHDR* /*pNMHDR*/, LRESULT *pResult)
{
	//LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
} 

void CWaveformSelectionDlg::OnNMDblclkLstcSignalWatch(NMHDR* /*pNMHDR*/, LRESULT *pResult)
{
	// Get the selected signal Name
	sWaveformInfo objWaveInfo;
	CString strSignalName;
	UINT nMsgID = (UINT)-1;			
	vInterPretSignalNameMsgID(m_omListCtrlSignalWatch.GetItemText(
		                   m_omListCtrlSignalWatch.GetSelectionMark() , 0), strSignalName, nMsgID);	
	m_pWaveDataHandler->bGetSignalWavePatternDetails(nMsgID, strSignalName, objWaveInfo);

	DefineUpdateWave(&m_omListCtrlSignalWatch, nMsgID, strSignalName, objWaveInfo);
	
	vPopulateUnSelSubEntryList(unGetSelectedMainEntryID());
	vPopulateSelSubEntryList();
	// Update button status
	vEnableDisableButtons();

	*pResult = 0;
}

void CWaveformSelectionDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	m_pWaveDataHandler->vRollback();
	OnCancel();
}

void CWaveformSelectionDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_pWaveDataHandler->vCommit();
	OnOK();
}

void CWaveformSelectionDlg::OnEnChangeEditDefaultSignalValue()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	UpdateData();
	UINT nMsgID=unGetSelectedMainEntryID();
	m_pWaveDataHandler->vSetMsgIDDefaultValue(nMsgID,m_fDefAmplitude);
}
