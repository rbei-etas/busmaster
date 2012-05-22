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
 * \file      MainSubListDlg.cpp
 * \brief     Inplementation file for CMainSubListDlg class
 * \author    Raja N.
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Inplementation file for CMainSubListDlg class
 */

#include "Filter_stdafx.h"
#include "Filter_resource.h"
#include "DataTypes/MainSubEntry.h"
#include "FilterWatchDefs.h"
#include "MainSubListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Critical Section for Signal Watch
extern UINT unGetMainEntryIDFromName(CString omMsgName);
/////////////////////////////////////////////////////////////////////////////
// CMainSubListDlg dialog




CMainSubListDlg::CMainSubListDlg(CWnd* pParent, CMainEntryList* psMainFrameSignalWatchList, SGUIPARAMS& sGuiParams)
    : CDialog(CMainSubListDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CMainSubListDlg)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    m_podMainCallerList = psMainFrameSignalWatchList;
    if (m_podMainCallerList != NULL)
    {
        m_podTempCallerList = new CMainEntryList;
        m_podTempCallerList->AddTail(psMainFrameSignalWatchList);
    }
    m_sGuiParams = sGuiParams;
}


void CMainSubListDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMainSubListDlg)
    DDX_Control(pDX, IDC_LSTC_SIGNAL_WATCH, m_omListCtrlSignalWatch);
    DDX_Control(pDX, IDC_LSTC_SIGNAL, m_omListCtrlSignal);
    DDX_Control(pDX, IDC_COMB_MSGNAME, m_omCombMessage);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainSubListDlg, CDialog)
    //{{AFX_MSG_MAP(CMainSubListDlg)
    ON_WM_DESTROY()
    ON_CBN_SELCHANGE(IDC_COMB_MSGNAME, OnSelChangeMessageName)
    ON_BN_CLICKED(IDM_SIGNALDLG_ADD, OnBtnAddSubEntries)
    ON_BN_CLICKED(IDM_SIGNALDLG_ADDALL, OnBtnAddAllSubEntries)
    ON_BN_CLICKED(IDM_SIGNALDLG_DELETE, OnBtnDelSubEntires)
    ON_BN_CLICKED(IDM_SIGNALDLG_DELETEALL, OnBtnDelAllSubEntires)
    ON_NOTIFY(NM_CLICK, IDC_LSTC_SIGNAL, OnClickUnSelSubEntryList)
    ON_NOTIFY(NM_DBLCLK, IDC_LSTC_SIGNAL, OnDblclkUnSelSubEntryList)
    ON_NOTIFY(NM_CLICK, IDC_LSTC_SIGNAL_WATCH, OnClickSelSubEntryList)
    ON_COMMAND(IDM_SIGNALDLG_SIGNALDETAILS, OnSignaldetails)
    ON_NOTIFY(NM_RCLICK, IDC_LSTC_SIGNAL, OnRClickUnSelSubEntryList)
    ON_NOTIFY(NM_RCLICK, IDC_LSTC_SIGNAL_WATCH, OnRClickSelSubEntryList)
    ON_COMMAND(IDM_SIGNALDLG_ADD, OnBtnAddSubEntries)
    ON_COMMAND(IDM_SIGNALDLG_ADDALL, OnBtnAddAllSubEntries)
    ON_COMMAND(IDM_SIGNALDLG_DELETE, OnBtnDelSubEntires)
    ON_COMMAND(IDM_SIGNALDLG_DELETEALL, OnBtnDelAllSubEntires)
    ON_WM_HELPINFO()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainSubListDlg message handlers

/*******************************************************************************
Function Name    : OnInitDialog
Input(s)         : 
Output           : 
Functionality    : This function will be called during initialization of dialog
                   box. This function will fill respective list controls
Member of        : CMainSubListDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
*******************************************************************************/
BOOL CMainSubListDlg::OnInitDialog() 
{
    // Call parent's init function
    CDialog::OnInitDialog();    

    //Display title name
    SetWindowText((LPCTSTR)m_sGuiParams.m_acTitleName);
    //Dislay Main List name
    CWnd* pWnd = GetDlgItem(IDC_EDIT_MAINLIST_NAME);
    if (pWnd != NULL)
    {
        pWnd->SetWindowText((LPCTSTR)m_sGuiParams.m_acMainListName);
    }
    //Dislay Unselected List name
    pWnd = GetDlgItem(IDC_EDIT_UNSELLIST_NAME);
    if (pWnd != NULL)
    {
        pWnd->SetWindowText((LPCTSTR)m_sGuiParams.m_acUnSelListName);
    }
    //Dislay Selected List name
    pWnd = GetDlgItem(IDC_EDIT_SELLIST_NAME);
    if (pWnd != NULL)
    {
        pWnd->SetWindowText((LPCTSTR)m_sGuiParams.m_acSelListName);
    }
    // Populate the message name combo
    // Clear the content if any
    m_omCombMessage.ResetContent();
    if (m_podTempCallerList != NULL)
    {
        UINT unNoOfMainEntries = (UINT)m_podTempCallerList->GetCount();
    
        if ( unNoOfMainEntries > 0 )
        {		
		    // Add every message name into the message list
            POSITION pos = m_podTempCallerList->GetHeadPosition();
            while (pos != NULL)
            {
                SMAINENTRY& sMainEntry = m_podTempCallerList->GetNext(pos);
                CString omMainEntryName = sMainEntry.m_omMainEntryName;
				CString omMainEntryId = _T("");
                if (m_sGuiParams.m_bCombine == TRUE)
                {
                    omMainEntryId.Format(defSTR_MSG_ID_IN_HEX,sMainEntry.m_unMainEntryID);
                    omMainEntryName = omMainEntryId + omMainEntryName;
                }
				
				m_omCombMessage.AddString(omMainEntryName);
            }
            m_omCombMessage.SetCurSel(0);
            
            m_omListCtrlSignal.SetImageList(m_sGuiParams.m_pomImageList, LVSIL_SMALL);
            
            // Create the first column. But this will not be shown
            m_omListCtrlSignal.InsertColumn( 0, 
                                                STR_EMPTY, 
                                                LVCFMT_LEFT,
                                                0); //Width is zero as this will 
                                            //be updated in vUpdateUnSelSubEntryList
            
            // Populate the list with signal names
            vPopulateUnSelSubEntryList(unGetSelectedMainEntryID());
            m_omCombMessage.SetFocus();
        }
    }
    
    // Update Signal Watch List
    // Set the same image list to this control
    m_omListCtrlSignalWatch.SetImageList(m_sGuiParams.m_pomImageList,LVSIL_SMALL);
    // Create Column for Signal Watch Lsit
    m_omListCtrlSignalWatch.InsertColumn( 0,
                                     STR_EMPTY,
                                     LVCFMT_LEFT,
                                     0);//Width is zero as this will 
                                        //be updated in vPopulateSelSubEntryList
    vPopulateSelSubEntryList();
    vEnableDisableButtons();

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
Modifications    : Raja N
                 : 14.04.2004. Modifications as per Code Review.
*******************************************************************************/
void CMainSubListDlg::vPopulateUnSelSubEntryList(UINT unMainEntryID)
{
    //CMainEntryList* psTemp = m_podTempCallerList;
    m_omListCtrlSignal.DeleteAllItems();
    if (m_podTempCallerList != NULL)
    {
        SMAINENTRY sMainEntry;
        // Check for visible window
        if( m_omCombMessage.GetSafeHwnd() != NULL )
        {
            m_omCombMessage.GetWindowText(sMainEntry.m_omMainEntryName);
        }
        sMainEntry.m_unMainEntryID = unMainEntryID;
        POSITION pos = m_podTempCallerList->Find(sMainEntry);
        if ( pos != NULL)
        {
            sMainEntry = m_podTempCallerList->GetAt(pos);
            POSITION SubPos = sMainEntry.m_odUnSelEntryList.GetHeadPosition();
            UINT unCountItem = 0;
            UINT unMaxLen = 0;
            while (SubPos != NULL)
            {
                SSUBENTRY& sSubEntry = sMainEntry.m_odUnSelEntryList.GetNext(SubPos);
                UINT unLen = m_omListCtrlSignal.GetStringWidth(sSubEntry.m_omSubEntryName);
                // Save the length if it is the max.
                if(unMaxLen < unLen )
                {
                    unMaxLen = unLen;
                }
                m_omListCtrlSignal.InsertItem(unCountItem++, sSubEntry.m_omSubEntryName, m_sGuiParams.m_unUnSelIconIndex);
            }
            // Set the column width if it is valid
            if( unMaxLen > 0 )
            {
                m_omListCtrlSignal.SetColumnWidth(0, unMaxLen + defSIZE_OF_IMAGE);
            }
        }
    }
}

/*******************************************************************************
Function Name    : OnDestroy
Input(s)         : 
Output           : 
Functionality    : This function will be called by framework when dialog 
                   is destroyed. It will release the memory allocated
Member of        : CMainSubListDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modification By  : 
Modification on  : 
*******************************************************************************/
void CMainSubListDlg::OnDestroy() 
{
    if (m_podTempCallerList != NULL)
    {
        m_podTempCallerList->RemoveAll();
        delete m_podTempCallerList;
        m_podTempCallerList = NULL;
    }
    CDialog::OnDestroy();    
}

/*******************************************************************************
Function Name    : OnSelChangeMessageName
Input(s)         : 
Output           : 
Functionality    : This function will be called by framework when the message 
                   name combo box selection got changed. This will update the 
                   Signal Names List control
Member of        : CMainSubListDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modification By  : 
Modification on  : 
*******************************************************************************/
void CMainSubListDlg::OnSelChangeMessageName() 
{

    vPopulateUnSelSubEntryList(unGetSelectedMainEntryID());
    vEnableDisableButtons();
}

/*******************************************************************************
Function Name    : vAddSelSubEntries
Input(s)         : bAllSignals - Add All Signals
Output           : 
Functionality    : This function will be called to add the selected signals to
                   the temporary signal list. It will add all messages if the
                   bool bAllSignals is true.
Member of        : CMainSubListDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modifications    : Raja N
                 : 14.04.2004. Modifications as per Code Review.
*******************************************************************************/
void CMainSubListDlg::vAddSelSubEntries(BOOL bAllSignals)
{
    //BOOL bMemoryError = FALSE;
    //UINT unSelMainEntryID = unGetSelectedMainEntryID();

    // Get the selected message list
    vUpdateUnSelSubEntryList(bAllSignals);
    if( (m_omCSAData.GetSize() > 0) && (m_podTempCallerList != NULL))
    {
        
        SMAINENTRY sMainEntry;
        UINT unMainEntryId = unGetSelectedMainEntryID(&(sMainEntry.m_omMainEntryName));
        sMainEntry.m_unMainEntryID = unMainEntryId;
        POSITION pos = m_podTempCallerList->Find(sMainEntry);
        if (pos != NULL)
        {
            SMAINENTRY& sPointEntry = m_podTempCallerList->GetAt(pos);
            for (int i = 0; i < m_omCSAData.GetSize(); i++)
            {
                CString omSubEntryName = m_omCSAData.GetAt(i);
                SSUBENTRY sSubEntry;
                sSubEntry.m_omSubEntryName = omSubEntryName;
                POSITION SubPos = sPointEntry.m_odUnSelEntryList.Find(sSubEntry);
                if (SubPos != NULL)
                {
                    sSubEntry = sPointEntry.m_odUnSelEntryList.GetAt(SubPos);
                    sPointEntry.m_odUnSelEntryList.RemoveAt(SubPos);
                    sPointEntry.m_odSelEntryList.AddTail(sSubEntry);
                }
            }
        }
    }
}

/*******************************************************************************
Function Name    : vDelSelSubEntries
Input(s)         : bAllSignals - Delete All Signals Flag
Output           : 
Functionality    : This function will be called to delete the selected signals
                   from the temporary signal list. It will delete all messages
                   if the bool bAllSignals is true.
Member of        : CMainSubListDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modification By  : 
Modification on  : 
*******************************************************************************/
void CMainSubListDlg::vDelSelSubEntries(BOOL bAllEntries)
{
    // Get the data from GUI interface
    vUpdateSelSubEntryList(bAllEntries);

    if(bAllEntries == FALSE)
    {
        UINT unSelCount = (UINT)m_omCSAData.GetSize();

        for(UINT index = 0; index < unSelCount; index++)
        {
            CString omSelMainSubEntry, omMainEntry, omSubEntry;
            
            omSelMainSubEntry = m_omCSAData.GetAt(index);

            if( bGetMainSubName( omSelMainSubEntry, omMainEntry, omSubEntry) == TRUE )
            {
                UINT unMainEntryID = unGetMainEntryIDFromName(omMainEntry);
                if ( bDeleteSubEntry(unMainEntryID, omMainEntry, omSubEntry) != TRUE)
                {
                    CString omError;
                    omError.Format(defSTR_SW_DELETE_SIG_ERROR, omSubEntry ,omMainEntry);
                    AfxMessageBox(omError);
                }
            }
        }

    }
    else
    {
        if (m_podTempCallerList != NULL)
        {
            POSITION pos = m_podTempCallerList->GetHeadPosition();
            while (pos)
            {
                SMAINENTRY& sMainEntry = m_podTempCallerList->GetNext(pos);
                POSITION SubPos = sMainEntry.m_odSelEntryList.GetHeadPosition();
                while (SubPos != NULL)
                {
                    POSITION TempPos = SubPos;
                    SSUBENTRY& sSubEntry = sMainEntry.m_odSelEntryList.GetNext(SubPos);
                    sMainEntry.m_odUnSelEntryList.AddTail(sSubEntry);
                    sMainEntry.m_odSelEntryList.RemoveAt(TempPos);                    
                }
            }
        }

    }
    vPopulateUnSelSubEntryList(unGetSelectedMainEntryID());
}

/*******************************************************************************
Function Name    : unGetSelectedMainEntryID
Input(s)         : -
Output           : 
Functionality    : This function will return the message id of the message name
                   selected in the combo box. This will be called to get the id
                   and it will get the id from active database
Member of        : CMainSubListDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modification By  : 
Modification on  : 
*******************************************************************************/
UINT CMainSubListDlg::unGetSelectedMainEntryID(CString* pomMainEntryName)
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
    if (pomMainEntryName != NULL)
    {
        *pomMainEntryName = omMainEntryName;
    }
    return unMainEntryId;

}

/*******************************************************************************
Function Name    : OnBtnAddSubEntries
Input(s)         :  -
Output           :  -
Functionality    : This function will be called by framework when the user 
                   selects "Add" button. This will call "vAddSelSubEntries" to
                   add the selected sub entries
Member of        : CMainSubListDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modifications    : Raja N
                 : 14.04.2004. Modifications as per Code Review.
*******************************************************************************/
void CMainSubListDlg::OnBtnAddSubEntries() 
{
    // Add only the selected signals
    vAddSelSubEntries(FALSE);
    // Update button status
    vEnableDisableButtons();
}

/*******************************************************************************
Function Name    : OnBtnAddAllSubEntries
Input(s)         :  -
Output           :  -
Functionality    : This function will be called by framework when the user
                   selects "Add All" button.This will call "vAddSelSubEntries"
                   to add all the signals
Member of        : CMainSubListDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modification By  : 
Modification on  : 
*******************************************************************************/
void CMainSubListDlg::OnBtnAddAllSubEntries() 
{
    // Add only the selected signals
    vAddSelSubEntries(TRUE);
    // Update button status
    vEnableDisableButtons();
}

/*******************************************************************************
Function Name    : OnBtnDelSubEntires
Input(s)         :  -
Output           :  -
Functionality    : This function will be called by framework when the user
                   selects "Delete" button. This will call
                   "vDeleteSelectedSignals" to delete the selected signals. If
                   the user selectes all the signals it will do delete all.
Member of        : CMainSubListDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modification By  : 
Modification on  : 
*******************************************************************************/
void CMainSubListDlg::OnBtnDelSubEntires() 
{
    BOOL bDeleteAll = FALSE;
    // Check the selection count and if that is equal to total number just 
    // all. No need to process the data
    if(static_cast<int>(m_omListCtrlSignalWatch.GetSelectedCount()) ==
            m_omListCtrlSignalWatch.GetItemCount())
    {
        bDeleteAll = TRUE;
    }

    // Now call the delete function
    vDelSelSubEntries(bDeleteAll);
    vEnableDisableButtons();
}

/*******************************************************************************
Function Name    : OnBtnDelAllSignal
Input(s)         :  -
Output           :  -
Functionality    : This function will be called by framework when the user
                   selects "Delete All" button. This will call
                   "vDeleteSelectedSignals" to delete all the signals.
Member of        : CMainSubListDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modification By  : 
Modification on  : 
*******************************************************************************/
void CMainSubListDlg::OnBtnDelAllSubEntires() 
{
    vDelSelSubEntries(TRUE);
    vEnableDisableButtons();
}

/*******************************************************************************
Function Name    : bGetMainSubName
Input(s)         : omMainSubName - Signal Watch List item with Message & Signal Name
                   omMsg - CString reference to store parsed Message Name
                   omSig - CString reference to store parsed Signal Name
Output           : TRUE - success
                   FALSE - Failure
Functionality    : This function will parse the Signal List item and will fill
                   the omMsg and omSig strings. The match string is defined in
                   the file HashDefines.h
Member of        : CMainSubListDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modification By  : 
Modification on  : 
*******************************************************************************/
BOOL CMainSubListDlg::bGetMainSubName( const CString &omMainSubName,
                                                    CString &omMain,
                                                    CString &omSub )
{
    omSub = omMainSubName;

    int nIndex;
    BOOL bSuccess = FALSE;
    nIndex = omMainSubName.Find(defSTR_MSG_SIG_SEPERATER);

    if( nIndex != -1)
    {
        omMain = omMainSubName.Left(nIndex);
        // Length of seprater is 4. use 4 to subract
        omSub = omMainSubName.Right( omMainSubName.GetLength() - nIndex - 4);
        bSuccess = TRUE;
    }
    return bSuccess;
}

/*******************************************************************************
Function Name    : bDeleteSubEntry
Input(s)         : unMsgID - Message ID of the Signal
                   omSignal - Signal Name that needs to be deleted
Output           : TRUE - success
                   FALSE - Failure
Functionality    : This function will delete the signal from temp list. If the
                   message is empty after deleting the signal it will delete the
                   message node also and will remove that from the list.
Member of        : CMainSubListDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modifications    : Raja N
                 : 14.04.2004. Modifications as per Code Review.
*******************************************************************************/
BOOL CMainSubListDlg::bDeleteSubEntry(UINT unMainEntryID,const CString& omMainEntryName,
                                                        const CString& omSubEntry)
{
    BOOL bSuccess = FALSE;
    if (m_podTempCallerList != NULL)
    {
        SMAINENTRY sMainEntry;
        sMainEntry.m_unMainEntryID = unMainEntryID;
        sMainEntry.m_omMainEntryName = omMainEntryName;
        POSITION pos = m_podTempCallerList->Find(sMainEntry);
        if (pos != NULL)
        {
            SMAINENTRY& sPointEntry = m_podTempCallerList->GetAt(pos);
            SSUBENTRY sSubEntry;
            sSubEntry.m_omSubEntryName = omSubEntry;
            POSITION SubPos = sPointEntry.m_odSelEntryList.Find(sSubEntry);
            if (SubPos != NULL)
            {
                bSuccess = TRUE;
                sSubEntry = sPointEntry.m_odSelEntryList.GetAt(SubPos);
                sPointEntry.m_odSelEntryList.RemoveAt(SubPos);
                sPointEntry.m_odUnSelEntryList.AddTail(sSubEntry);
            }

        }
    }
    return bSuccess;
}

/*******************************************************************************
Function Name    : OnClickUnSelSubEntryList
Input(s)         : NMHDR* pNMHDR, LRESULT* pResult - Handler parameter
Output           :  -
Functionality    : This function will be called by framework when the user 
                   clicks on Signal Watch box. This will update the status of
                   the Add button as per the current selection
Member of        : CMainSubListDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modification By  : 
Modification on  : 
*******************************************************************************/
void CMainSubListDlg::OnClickUnSelSubEntryList(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
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
Member of        : CMainSubListDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modifications    : Raja N
                 : 14.04.2004. Modifications as per Code Review.
*******************************************************************************/
void CMainSubListDlg::OnDblclkUnSelSubEntryList(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
    if(m_omListCtrlSignal.GetSelectedCount() != 0)
    {
        // Add Selected Signal
        vAddSelSubEntries(FALSE);
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
Member of        : CMainSubListDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modification By  : 
Modification on  : 
*******************************************************************************/
void CMainSubListDlg::OnClickSelSubEntryList(NMHDR* /*pNMHDR*/, LRESULT* pResult)
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
Member of        : CMainSubListDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modification By  : 
Modification on  : 
*******************************************************************************/
void CMainSubListDlg::vEnableDisableButtons()
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
Function Name    : vUpdateSelSubEntryList
Input(s)         : bAllSignals - Add All Signals
Output           :  - 
Functionality    : This UI function will be called to get the selected signals
                   list. This will populate the private member of this class
                   m_omCSAData. This array will be used by data manipulation
                   functions to update the data list. Any UI modifications are
                   limited to this function only. It will add all the signals if
                   bAllSignals is TRUE.
Member of        : CMainSubListDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modifications    : Raja N
                 : 14.04.2004. Modifications as per Code Review.
*******************************************************************************/
void CMainSubListDlg::vUpdateUnSelSubEntryList(BOOL bAllEntries)
{
    CString omMainEntryName;
    m_omCombMessage.GetWindowText(omMainEntryName);
    m_omCSAData.RemoveAll();
    if( bAllEntries == FALSE )
    {
        // Only Add Selected Signals

        // Get the initial position
        POSITION pos = m_omListCtrlSignal.GetFirstSelectedItemPosition();
        // Store current column width. Subtract image width
        INT unCurrentSize = 
            m_omListCtrlSignalWatch.GetColumnWidth( 0 ) - defSIZE_OF_IMAGE;
        // Assume this is the greatest value
        INT unMaxSize = unCurrentSize;
        
        while(pos != NULL)
        {
            int nSelIndex;  
            CString omSelSubEntry;

            // Get the first selection
            nSelIndex = m_omListCtrlSignal.GetNextSelectedItem(pos);
            CString omStrEntry = omSelSubEntry = m_omListCtrlSignal.GetItemText(nSelIndex, 0);
            // Add the data into CStringArray and into the Watch List
            m_omCSAData.Add(omSelSubEntry);
            omStrEntry.Format(defSTR_FORMAT_SW_LIST,
                                    omMainEntryName,
                                    defSTR_MSG_SIG_SEPERATER,
                                    omSelSubEntry);
            m_omListCtrlSignalWatch.InsertItem(0,
                                               omStrEntry,
                                               m_sGuiParams.m_unSelIconIndex);
            // Resize the the column width
            
            INT unSize = m_omListCtrlSignalWatch.GetStringWidth(omStrEntry);
            if( unMaxSize < unSize )
            {
                unMaxSize = unSize;
            }
            // Delete the item from the Signal List
            m_omListCtrlSignal.DeleteItem(nSelIndex);
            // As an item got deleted get the latest position
            pos = m_omListCtrlSignal.GetFirstSelectedItemPosition();
        }
        if( unMaxSize > unCurrentSize )
        {
            m_omListCtrlSignalWatch.SetColumnWidth( 0 , unMaxSize + 
                                                        defSIZE_OF_IMAGE );
        }
                        
    }
    else
    {
        // Get All items
        UINT unItemCount = m_omListCtrlSignal.GetItemCount();
        
        INT unCurrentSize = 
            m_omListCtrlSignalWatch.GetColumnWidth( 0 ) - defSIZE_OF_IMAGE;
        INT unMaxSize = unCurrentSize;
        
        CString omSelSubEntry;
        for(UINT index = 0; index < unItemCount; index++ )
        {
            CString omStrEntry = omSelSubEntry = m_omListCtrlSignal.GetItemText(index, 0);
            // Add the data into CStringArray and into the Watch List
            m_omCSAData.Add(omSelSubEntry);
            
            omStrEntry.Format(defSTR_FORMAT_SW_LIST,
                                    omMainEntryName,
                                    defSTR_MSG_SIG_SEPERATER,
                                    omSelSubEntry);
            INT unSize = m_omListCtrlSignalWatch.GetStringWidth(omStrEntry);
            
            if( unMaxSize < unSize )
            {
                unMaxSize = unSize;
            }
            m_omListCtrlSignalWatch.InsertItem(0,
                                               omStrEntry,
                                               m_sGuiParams.m_unSelIconIndex);
        }
        if( unMaxSize > unCurrentSize )
        {
            m_omListCtrlSignalWatch.SetColumnWidth( 0 , unMaxSize + 
                                                        defSIZE_OF_IMAGE );
        }
        // Clear the list as all items ware added
        m_omListCtrlSignal.DeleteAllItems();
    }
}

/*******************************************************************************
Function Name    : vUpdateSelSubEntryList
Input(s)         : bAllSignals - Add All Signals
Output           : 
Functionality    : This UI function will be called to get the selected signal
                   watch list entries. This will populate the private member of
                   this class m_omCSAData. This array will be used by data
                   manipulation functions to update the list. Any UI
                   modification changes are limited to this function only. bool
                   bAllSignals is true.
Member of        : CMainSubListDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modification By  : 
Modification on  : 
*******************************************************************************/
void CMainSubListDlg::vUpdateSelSubEntryList(BOOL bAllEntries)
{

    m_omCSAData.RemoveAll();
    if( bAllEntries == FALSE )
    {
        // Only Add Selected Signals
        // Get the initial position
        POSITION pos = m_omListCtrlSignalWatch.GetFirstSelectedItemPosition();

        while(pos != NULL)
        {
            int nSelIndex;  
            CString omSelMainSub;

            // Get the first selection
            nSelIndex = m_omListCtrlSignalWatch.GetNextSelectedItem(pos);
            omSelMainSub = 
                m_omListCtrlSignalWatch.GetItemText(nSelIndex, 0);

            // Add the data into CStringArray and into the Watch List
            m_omCSAData.Add(omSelMainSub);

            // Delete the item from the Signal List
            m_omListCtrlSignalWatch.DeleteItem(nSelIndex);

            // As an item got deleted get the latest position
            pos = m_omListCtrlSignalWatch.GetFirstSelectedItemPosition();
        }

                        
    }
    else
    {
         // Clear the list as all items ware added
        m_omListCtrlSignalWatch.DeleteAllItems();
        // Nothing is required to store in the data array as the whole
        // list will be deleted. The data interface will delete the linked
        // list of signal watch
    }

}

/**
 * \brief Show selected signal details
 * \todo Function needs to be implemented.
 *
 * This function will be called by framework when the user
 * selects "Signal Details" from the popup menu. This will show
 * selected signal details using CSignalDetailsDlg.
 */
void CMainSubListDlg::OnSignaldetails()
{ 
}

/*******************************************************************************
Function Name    : OnRClickSignal
Input(s)         : NMHDR* pNMHDR, LRESULT* pResult - Handler parameter
Output           :  -
Functionality    : This function will be called by framework when the user 
                   clicks the right mouse button in Signal List box. This will
                   popup the menu item. The irrevelent menu items will be
                   disabled. If there is not item in the list box the popup menu
                   will not be displayed.
Member of        : CMainSubListDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modification By  : 
Modification on  : 
*******************************************************************************/
void CMainSubListDlg::OnRClickUnSelSubEntryList(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
    CMenu omCtxMenu;
    CPoint point(0,0);

    // Enable buttons in sysn with menu
    vEnableDisableButtons();

    GetCursorPos(&point);
    // Popup only if there is something in the list
    if( m_omListCtrlSignal.GetItemCount() > 0)
    {
        if (omCtxMenu.LoadMenu(IDM_MENU_SIGNAL_WATCH))
        {
            CMenu* pomCtxMenu = omCtxMenu.GetSubMenu(2);

            if (pomCtxMenu != NULL)
            {
                UINT unSelCount;
                unSelCount = m_omListCtrlSignal.GetSelectedCount();
                // Disable Delete Operations
                pomCtxMenu->EnableMenuItem(IDM_SIGNALDLG_DELETE,
                                            MF_DISABLED | MF_GRAYED);

                pomCtxMenu->EnableMenuItem(IDM_SIGNALDLG_DELETEALL,
                                            MF_DISABLED | MF_GRAYED);
                if(unSelCount == 0)
                {
                    // Disable selection operations
                    pomCtxMenu->EnableMenuItem(IDM_SIGNALDLG_ADD,
                                                MF_DISABLED | MF_GRAYED);
                }
                // Enable Signal Details only if one item got selected
                if(unSelCount == 1)
                {
                    // Copy the Message Name and Signal Name for Signal Details
                    m_omCSAData.RemoveAll();
                    // Message Name First
                    CString omMainEntry;
                    m_omCombMessage.GetWindowText(omMainEntry);
                    m_omCSAData.Add(omMainEntry);
                    // Signal Name Next
                    POSITION pos = 
                        m_omListCtrlSignal.GetFirstSelectedItemPosition();
                    UINT unIndex = m_omListCtrlSignal.GetNextSelectedItem(pos);
            
                    m_omCSAData.Add(m_omListCtrlSignal.GetItemText(unIndex,0));
                }
                // Multiple Selection
                else
                {
                    // Disable Details Menu Item
                    pomCtxMenu->EnableMenuItem(IDM_SIGNALDLG_SIGNALDETAILS,
                                                MF_DISABLED | MF_GRAYED);
                }
                
                pomCtxMenu->TrackPopupMenu(TPM_RIGHTBUTTON | TPM_LEFTALIGN,
                                       point.x, point.y,
                                       this);
            }
        }
    }   
    
    *pResult = 0;
}

/*******************************************************************************
Function Name    : OnRClickSelSubEntryList
Input(s)         : NMHDR* pNMHDR, LRESULT* pResult - Handler parameter
Output           :  -
Functionality    : This function will be called by framework when the user 
                   clicks the right mouse button in Signal Watch List box. This
                   will popup the menu item. The irrevelent menu items will be
                   disabled. If there is not item in the list box the popup menu
                   will not be displayed.
Member of        : CMainSubListDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modification By  : 
Modification on  : 
*******************************************************************************/
void CMainSubListDlg::OnRClickSelSubEntryList(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    CMenu omCtxMenu;
    CPoint point(0,0);

    // Enable buttons in sync with the menu status
    vEnableDisableButtons();

    GetCursorPos(&point);
    // Popup only if there is atleast one entry in the list
    if( m_omListCtrlSignalWatch.GetItemCount() > 0)
    {
        if (omCtxMenu.LoadMenu(IDM_MENU_SIGNAL_WATCH))
        {
            CMenu* pomCtxMenu = omCtxMenu.GetSubMenu(2);

            if (pomCtxMenu != NULL)
            {
                UINT unSelCount;
                unSelCount = m_omListCtrlSignalWatch.GetSelectedCount();
                // Disable Delete Operations
                pomCtxMenu->EnableMenuItem(IDM_SIGNALDLG_ADD,
                                            MF_DISABLED | MF_GRAYED);

                pomCtxMenu->EnableMenuItem(IDM_SIGNALDLG_ADDALL,
                                            MF_DISABLED | MF_GRAYED);
                if(unSelCount == 0)
                {
                    // Disable selection operations
                    pomCtxMenu->EnableMenuItem(IDM_SIGNALDLG_DELETE,
                                                MF_DISABLED | MF_GRAYED);
                }
                // Enable Signal Details only if one item got selected
                if(unSelCount == 1)
                {
                    // Copy the Message Name and Signal Name for Signal Details
                    m_omCSAData.RemoveAll();
                    
                    CString omMainEntry, omSubEntry, omMainSub;
                    POSITION pos = 
                        m_omListCtrlSignalWatch.GetFirstSelectedItemPosition();
                    
                    UINT unIndex = 
                        m_omListCtrlSignalWatch.GetNextSelectedItem(pos);
                    
                    omMainSub =  
                        m_omListCtrlSignalWatch.GetItemText(unIndex,0);
                    
                    if (bGetMainSubName(omMainSub, omMainEntry, omSubEntry) == TRUE)
                    {
                        // Message Name First
                        m_omCSAData.Add(omMainEntry);
                        // Signal Name Next
                        m_omCSAData.Add(omSubEntry);
                    }
                }
                // Multiple Selection
                else
                {
                    // Disable Details Menu Item
                    pomCtxMenu->EnableMenuItem(IDM_SIGNALDLG_SIGNALDETAILS,
                                                MF_DISABLED | MF_GRAYED);
                }
                
                pomCtxMenu->TrackPopupMenu(TPM_RIGHTBUTTON | TPM_LEFTALIGN,
                                       point.x, point.y,
                                       this);
            }
        }
    }
    *pResult = 0;
}

/*******************************************************************************
Function Name    : OnOK
Input(s)         :  -
Output           :  -
Functionality    : This function will be called by framework when the user
                   selects "OK" button. This will update the global pointer to
                   the signal watch list with the latest value.
Member of        : CMainSubListDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modification By  : 
Modification on  : 
*******************************************************************************/
void CMainSubListDlg::OnOK() 
{
    if (m_podMainCallerList != NULL)
    {
        m_podMainCallerList->RemoveAll();
        m_podMainCallerList->AddTail(m_podTempCallerList);
    }
    CDialog::OnOK();
}

/*******************************************************************************
Function Name    : vPopulateSelSubEntryList
Input(s)         :  -
Output           :  -
Functionality    : This function will update the signal watch list box with the
                    entries in the signal watch list.
Member of        : CMainSubListDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 25.03.2004
Modifications    : Raja N
                 : 14.04.2004. Modifications as per Code Review.
*******************************************************************************/
void CMainSubListDlg::vPopulateSelSubEntryList()
{
    // To store the Col Width
    UINT unMaxSize = 0;
    // Get the items and add
    if ( m_podTempCallerList != NULL)
    {
        POSITION pos = m_podTempCallerList->GetHeadPosition();
        while (pos)
        {
            SMAINENTRY& sMainEntry = m_podTempCallerList->GetNext(pos);
            if( sMainEntry.m_omMainEntryName != STR_EMPTY)
            {
                CString omNameWithId = _T("");
                if (m_sGuiParams.m_bCombine == TRUE)
                {
                    omNameWithId.Format(defSTR_MSG_ID_IN_HEX,sMainEntry.m_unMainEntryID);
                }
                omNameWithId = omNameWithId + sMainEntry.m_omMainEntryName;// + omNameWithId;
                CString omStrEntry;
                POSITION SubPos = sMainEntry.m_odSelEntryList.GetHeadPosition();
                while (SubPos != NULL)
                {                    
                    SSUBENTRY& sSubEntry = sMainEntry.m_odSelEntryList.GetNext(SubPos);
                    // Use format instead of multiple operator '+' call
                    
                    omStrEntry.Format(defSTR_FORMAT_SW_LIST,omNameWithId,
                                                defSTR_MSG_SIG_SEPERATER,
                                                sSubEntry.m_omSubEntryName);
                    
                    // Get current entry size
                    UINT unSize = 
                        m_omListCtrlSignalWatch.GetStringWidth( omStrEntry );
                    // Compare with the maximun
                    if( unSize > unMaxSize)
                    {
                        // This is gonna be the maximum
                        unMaxSize = unSize;
                    }
                    m_omListCtrlSignalWatch.InsertItem(0, omStrEntry, m_sGuiParams.m_unSelIconIndex);
                }
            }
        }
    }
    // Set the column width if it is valid
    if( unMaxSize > 0 )
    {
        m_omListCtrlSignalWatch.SetColumnWidth(0, unMaxSize + defSIZE_OF_IMAGE);
    }
}

/*******************************************************************************
Function Name    : OnHelpInfo
Input(s)         : Pointer to object of structure for help request info
Output           :
Functionality    : This function is called by the framework in response
                   to the WM_HELPINFO message. This message is sent to
                   the dialog box during help request from user
Member of        : CMainSubListDlg
Friend of        :  -
Author(s)        : Raja N
Date Created     : 02/04/2004
Modifications    :
*******************************************************************************/
BOOL CMainSubListDlg::OnHelpInfo(HELPINFO* /*pHelpInfo*/) 
{
    //vSetHelpID(pHelpInfo->dwContextId);
    //return CDialog::OnHelpInfo(pHelpInfo);
    return FALSE;
}

UINT CMainSubListDlg::unGetMainEntryIDFromName(CString omMainEntryName)
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
