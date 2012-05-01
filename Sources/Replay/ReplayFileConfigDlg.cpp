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
 * \file      ReplayFileConfigDlg.cpp
 * \brief     Implementation file for CReplayFileConfigDlg class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CReplayFileConfigDlg class
 */

#include "Replay_stdafx.h"          // For standard includes
#include "ReplayFile.h"             // For Replay File class declaration
#include "MsgReplayWnd.h"           // For message Window class declaration
#include "ReplayManager.h"          // For Replay Manager class declaration
#include "Utility/RadixEdit.h"      // For Radix edit control used in Cfg dialog
#include "ReplayFileConfigDlg.h"    // For Replay File Configuration dialog
#include "ReplayProcess.h"          // For Replay Process class declaration
#include "Filter/Filter_extern.h"


#define defREPLAY_FILE_IMAGE_INDEX          3
#define defREPLAY_FILE_COL_WIDTH_PROPOTION  0.95

#define defSTR_REPLAY_FILE_COL_NAME             _T("Replay File")
#define defSTR_DELETE_CONFORMATION          _T("Do you want to remove selected replay file?")
#define defSTR_REPALY_FILTER_DLG_TITLE      _T("Configure Filter for Replay File: %s")

/**
 * Standard default constructor.
 * This will initialise local variables
 */
CReplayFileConfigDlg::CReplayFileConfigDlg( CReplayManager& rouManager,
        const SFILTERAPPLIED_CAN* psFilterConfigured,
        CWnd* pParent /*=NULL*/)
    : CDialog(CReplayFileConfigDlg::IDD, pParent),
      m_rouManager( rouManager ),
      m_psFilterConfigured(psFilterConfigured)
{
    //{{AFX_DATA_INIT(CReplayFileConfigDlg)
    m_nReplayMode = 0;
    //}}AFX_DATA_INIT
    m_bUpdating = FALSE;
    m_nSelecetedNamedLogIndex = -1;
}

/**
 * \param[in] pDX Pointer to data exchange object
 *
 * This will map UI controls will mapped data members. This will
 * be called during UpdateData funtion call for data update
 */
void CReplayFileConfigDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CReplayFileConfigDlg)
    DDX_Control(pDX, IDC_CHK_INTERACTIVE, m_omChkInteractive);
    DDX_Control(pDX, IDC_EDIT_MSG_DELAY, m_omEditMsgDelay);
    DDX_Control(pDX, IDC_EDIT_CYCLE_DELAY, m_omEditCycleDelay);
    DDX_Control(pDX, IDC_CHK_RETAIN_RECORDED_DELAY, m_omChkRetainDelay);
    DDX_Control(pDX, IDC_EDIT_REPLAY_FILE_NAME, m_omEditReplayFileName);
    DDX_Control(pDX, IDC_LIST_REPALY_FILES, m_omLstcReplayFiles);
    DDX_Radio(pDX, IDC_RADIO_REPLAY_MODE_MONO, m_nReplayMode);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CReplayFileConfigDlg, CDialog)
    //{{AFX_MSG_MAP(CReplayFileConfigDlg)
    ON_NOTIFY(NM_CLICK, IDC_LIST_REPALY_FILES, OnClickListRepalyFiles)
    ON_NOTIFY(NM_DBLCLK, IDC_LIST_REPALY_FILES, OnDblclkListRepalyFiles)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_REPALY_FILES, OnItemchangedListRepalyFiles)
    ON_BN_CLICKED(IDC_BTN_BROWSE, OnBtnBrowse)
    ON_BN_CLICKED(IDC_CHK_RETAIN_RECORDED_DELAY, OnChkRetainRecordedDelay)
    ON_EN_UPDATE(IDC_EDIT_MSG_DELAY, OnUpdateEditMsgDelay)
    ON_BN_CLICKED(IDC_RADIO_REPLAY_MODE_MONO, OnRadioReplayModeMono)
    ON_EN_UPDATE(IDC_EDIT_CYCLE_DELAY, OnUpdateEditCycleDelay)
    ON_BN_CLICKED(IDC_CHK_INTERACTIVE, OnChkInteractive)
    ON_BN_CLICKED(IDC_BTN_ADD_FILE, OnBtnAddFile)
    ON_BN_CLICKED(IDC_BTN_DELETE_FILE, OnBtnDeleteFile)
    ON_BN_CLICKED(IDC_RADIO_REPLAY_MODE_CYCLIC, OnRadioReplayModeMono)
    ON_BN_CLICKED(IDC_BTN_FILTER, OnBtnFilter)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/**
 * \return FALSE - If focus is set to Any UI control explicitly
 *
 * Initialises dialog's UI components
 */
BOOL CReplayFileConfigDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    // Create Image List used in UI List
    bCreateImageList();
    // Create Replay Files UI List
    vCreateReplayFileList();
    // To Create Replay Components
    vCreateReplayCopms();
    // Init Replay Components
    vInitReplayCopms();
    // Init Replay List
    vInitReplayFileList();
    // Update Button Status
    vEnableDisableButtons();
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

/**
 * \return TRUE if success, FALSE otherwise
 *
 * To create image lists used in the dialog
 */
BOOL CReplayFileConfigDlg::bCreateImageList()
{
    // Flag to indicate result
    BOOL bReturn = FALSE;

    // Create Image List
    if( m_omImageList.Create( IDB_BMP_PROPERTIES, defICON_SIZE,
                              defICON_GROW, defCOLOR_WHITE ) == TRUE )
    {
        // If successful set the result
        bReturn = TRUE;
    }

    // Return the result
    return bReturn;
}

/**
 * To create replay file list control with required columns
 */
VOID CReplayFileConfigDlg::vCreateReplayFileList()
{
    // Insert Log File Name Column
    m_omLstcReplayFiles.InsertColumn(0, defSTR_REPLAY_FILE_COL_NAME );
    // Set the width to occupy the whole list
    CRect omRect;
    m_omLstcReplayFiles.GetWindowRect( &omRect );
    int nWidth =
        static_cast<int>( omRect.Width() * defREPLAY_FILE_COL_WIDTH_PROPOTION );
    // Set Col Width
    m_omLstcReplayFiles.SetColumnWidth( 0, nWidth );

    // Set Image List
    if( m_omImageList.m_hImageList != NULL )
    {
        m_omLstcReplayFiles.SetImageList( &m_omImageList, LVSIL_SMALL );
    }

    // Set Extended Property
    m_omLstcReplayFiles.SetExtendedStyle( LVS_EX_CHECKBOXES );
}

/**
 * Create replay components. There is no component now which
 * requires creation. For future usage.
 */
VOID CReplayFileConfigDlg::vCreateReplayCopms()
{
    // To do any creation
}

/**
 * To initialise replay details UI components
 */
VOID CReplayFileConfigDlg::vInitReplayCopms()
{
    // Update Message Delay Edit box
    m_omEditMsgDelay.vSetSigned( FALSE );
    m_omEditMsgDelay.vSetBase( BASE_DECIMAL );
    // Update Cycle Delay Edit Box
    m_omEditCycleDelay.vSetSigned( FALSE );
    m_omEditCycleDelay.vSetBase( BASE_DECIMAL );
}

/**
 * To initialise replay list UI control with the list of replay files
 */
VOID CReplayFileConfigDlg::vInitReplayFileList()
{
    // Get the list of files and populate the list control
    // Switch off update
    m_bUpdating = TRUE;
    // Delete all items from the list
    m_omLstcReplayFiles.DeleteAllItems();
    // Get the size and iterate through the list
    int nSize = (int)m_rouManager.m_omReplayFiles.GetSize();

    for( int nIndex = 0; nIndex < nSize; nIndex++ )
    {
        const CReplayFile& ouFile =
            m_rouManager.m_omReplayFiles.ElementAt( nIndex );
        m_omLstcReplayFiles.InsertItem( nIndex,
                                        ouFile.m_omStrFileName,
                                        defREPLAY_FILE_IMAGE_INDEX );
        //Update Check button status
        m_omLstcReplayFiles.SetCheck( nIndex, ouFile.m_bEnabled );
    }

    // Enable update
    m_bUpdating = FALSE;
    // Set the focus to the first item
    m_omLstcReplayFiles.SetItemState( 0, LVIS_SELECTED | LVIS_FOCUSED,
                                      LVIS_SELECTED | LVIS_FOCUSED );
}

/**
 * To update UI controls with respection to selection and other
 * related conditions
 */
VOID CReplayFileConfigDlg::vEnableDisableButtons()
{
    // Get the list item count
    int nSize = m_omLstcReplayFiles.GetItemCount();
    // If list is empty then disable replay comps
    BOOL bEnable = nSize > 0;
    vEnableReplayComps( bEnable );
    // Disable Enable/Delete button
    CWnd* pWnd = GetDlgItem(IDC_BTN_DELETE_FILE);

    if( pWnd != NULL )
    {
        pWnd->EnableWindow( bEnable );
    }
}

/**
 * This function will be called wher user clicks the list
 * control. This will update the selection will set the
 * selection to the last item if nothing is selected by the user
 */
void CReplayFileConfigDlg::OnClickListRepalyFiles(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    // Create selection mask
    UINT unItemStateMask = LVNI_SELECTED | LVNI_FOCUSED;
    // Get current selection
    int nSel = m_omLstcReplayFiles.GetNextItem( -1, LVNI_SELECTED );

    // If nothing got selected restore last selection
    if(nSel == -1)
    {
        m_omLstcReplayFiles.SetItemState( m_nSelecetedNamedLogIndex,
                                          unItemStateMask,
                                          unItemStateMask );
    }

    if( pResult != NULL )
    {
        *pResult = 0;
    }
}

/**
 * This function will be called wher user double clicks the list
 * control. This will update the selection will set the
 * selection to the last item if nothing is selected by the user
 */
void CReplayFileConfigDlg::OnDblclkListRepalyFiles(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    // Create selection mask
    UINT unItemStateMask = LVNI_SELECTED | LVNI_FOCUSED;
    // Get current selection
    int nSel = m_omLstcReplayFiles.GetNextItem( -1, LVNI_SELECTED );

    // If nothing got selected restore last selection
    if(nSel == -1)
    {
        m_omLstcReplayFiles.SetItemState( m_nSelecetedNamedLogIndex,
                                          unItemStateMask,
                                          unItemStateMask );
    }

    if( pResult != NULL )
    {
        *pResult = 0;
    }
}

/**
 * This function will be called wher user changes the selection
 * in the replay list. This will update UI with the selected
 * replay file details. This will also be called during check
 * box press and will update the replay file enable flag.
 */
void CReplayFileConfigDlg::OnItemchangedListRepalyFiles( NMHDR* pNMHDR,
        LRESULT* pResult )
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    // Update Log File Components
    if(pNMListView->uChanged != LVIF_TEXT && m_bUpdating == FALSE )
    {
        // Selected & Focused
        if( pNMListView->uNewState == (LVIS_FOCUSED | LVIS_SELECTED) )
        {
            // Update selection
            m_nSelecetedNamedLogIndex = pNMListView->iItem;
            // Update selected Log file details
            vUpdateReplayFileDetails( pNMListView->iItem);
        }

        if( pNMListView->uNewState & defMASK_CHECK_UNCHECK )
        {
            int nCurrentState =
                m_omLstcReplayFiles.GetCheck(pNMListView->iItem);
            // Update Data
            CReplayFile& ouFile =
                m_rouManager.m_omReplayFiles.ElementAt( pNMListView->iItem );
            ouFile.m_bEnabled = nCurrentState;
        }
    }

    if( pResult != NULL )
    {
        *pResult = 0;
    }
}

/**
 * \param[in] nSelectedIndex UI Index of the replay file in the list
 *
 * Update the replay information in the UI with the replay file
 * pointed by the index
 */
VOID CReplayFileConfigDlg::vUpdateReplayFileDetails( int nSelectedIndex )
{
    // Check for valid index
    if( nSelectedIndex != -1 &&
            nSelectedIndex < m_rouManager.m_omReplayFiles.GetSize() )
    {
        // Get the details from the data array
        const CReplayFile& odLog =
            m_rouManager.m_omReplayFiles.ElementAt( nSelectedIndex );
        // Update Log File Components with this details
        vUpdateReplayFileComps( odLog );
    }
}

/**
 * \param[in] rouFile Reference to replay file
 *
 * Updates UI with the information given by rouFile.
 */
VOID CReplayFileConfigDlg::vUpdateReplayFileComps( const CReplayFile& rouFile )
{
    // File Path
    m_omEditReplayFileName.SetWindowText( rouFile.m_omStrFileName );

    // Message Time Mode
    switch( rouFile.m_nTimeMode )
    {
        case defREPLAY_RETAIN_DELAY:
        {
            // Enable Retain Delay Checkbox and check the item
            m_omChkRetainDelay.SetCheck( TRUE );
            // Disable Specific Delay option
            m_omEditMsgDelay.EnableWindow( FALSE );
        }
        break;

        case defREPLAY_SPECIFIC_DELAY:
        {
            // Disable Retain Delay Checkbox and check the item
            m_omChkRetainDelay.SetCheck( FALSE );
            // Enable Specific Delay option
            m_omEditMsgDelay.EnableWindow( TRUE );
            // Set the value
            m_omEditMsgDelay.vSetValue( rouFile.m_unMsgTimeDelay );
        }
        break;

        default:
            // Invalid value
            ASSERT( FALSE );
    }

    // Replay Mode
    // Assign the mode value
    m_nReplayMode = rouFile.m_nReplayMode;

    // Update Cyclic time delay edit box
    if( rouFile.m_nReplayMode == defREPLAY_MODE_CYCLIC )
    {
        // Enable Cyclic Delay
        m_omEditCycleDelay.EnableWindow( TRUE );
        m_omEditCycleDelay.vSetValue( rouFile.m_unCycleTimeDelay );
    }
    else
    {
        // Disable Cyclic Delay Editbox
        m_omEditCycleDelay.EnableWindow( FALSE );
    }

    // Update Interactive Replay Option
    m_omChkInteractive.SetCheck( rouFile.m_bInteractive );
    // Update DDX data with UI
    UpdateData( FALSE );
}

/**
 * This function will be called when user selects Browse button.
 * This function will show file selection dialog and will update
 * replay file data if user selects any file.
 */
void CReplayFileConfigDlg::OnBtnBrowse()
{
    if( m_nSelecetedNamedLogIndex != -1 &&
            m_nSelecetedNamedLogIndex < m_rouManager.m_omReplayFiles.GetSize() )
    {
        // Get Selected Item Details
        CReplayFile& ouFile =
            m_rouManager.m_omReplayFiles.ElementAt( m_nSelecetedNamedLogIndex );
        DWORD dwFlags   = 0;
        dwFlags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_EXTENSIONDIFFERENT;
        // Show File Selection Dialog to select Log File
        CFileDialog omFileDlg( TRUE,
                               defSTR_LOG_FILE_EXTENSION,
                               ouFile.m_omStrFileName,
                               dwFlags,
                               defLOG_FILTER,
                               NULL );
        //Set the caption
        omFileDlg.m_ofn.lpstrTitle = defSTR_REPLAY_FILE_SELECTION_TITLE;

        // Show File open dialog
        if( omFileDlg.DoModal() == IDOK )
        {
            // Get Path from File Selection Dialog
            ouFile.m_omStrFileName = omFileDlg.GetPathName();
            // Update List Item Text
            m_omLstcReplayFiles.SetItemText( m_nSelecetedNamedLogIndex, 0,
                                             ouFile.m_omStrFileName );
            // Set File Name in the editbox
            m_omEditReplayFileName.SetWindowText( ouFile.m_omStrFileName );
        }
    }
}

/**
 * This function will be called when user check/uncheck the
 * retain reconded delay option. This will update replay file
 * details
 */
void CReplayFileConfigDlg::OnChkRetainRecordedDelay()
{
    if( m_nSelecetedNamedLogIndex != -1 &&
            m_nSelecetedNamedLogIndex < m_rouManager.m_omReplayFiles.GetSize() )
    {
        // Get Selected Item Details
        CReplayFile& ouFile =
            m_rouManager.m_omReplayFiles.ElementAt( m_nSelecetedNamedLogIndex );
        // Get the check value
        BOOL bValue = m_omChkRetainDelay.GetCheck();

        if( bValue == TRUE )
        {
            // Disable the Msg Delay edit control
            m_omEditMsgDelay.EnableWindow( FALSE );
        }
        else
        {
            // Enable Msg Delay Edit control and assign the value from
            // data
            m_omEditMsgDelay.EnableWindow( TRUE );
            m_omEditMsgDelay.vSetValue( ouFile.m_unMsgTimeDelay );
        }

        // Update type in the data
        ouFile.m_nTimeMode = !bValue;
    }
}

/**
 * This function will be called during message delay editbox
 * change. This will update message delay value of replay file
 */
void CReplayFileConfigDlg::OnUpdateEditMsgDelay()
{
    if( m_nSelecetedNamedLogIndex != -1 &&
            m_nSelecetedNamedLogIndex < m_rouManager.m_omReplayFiles.GetSize() )
    {
        // Get Selected Item Details
        CReplayFile& ouFile =
            m_rouManager.m_omReplayFiles.ElementAt( m_nSelecetedNamedLogIndex );
        // Get the delay Value
        UINT unValue = static_cast<UINT>( m_omEditMsgDelay.lGetValue() );
        // Update type in the data
        ouFile.m_unMsgTimeDelay = unValue;
    }
}

/**
 * This function will be called when user changes the replay
 * mode radio button. This will update replay file and will
 * enable cyclic delay editbox appropriatly
 */
void CReplayFileConfigDlg::OnRadioReplayModeMono()
{
    if( m_nSelecetedNamedLogIndex != -1 &&
            m_nSelecetedNamedLogIndex < m_rouManager.m_omReplayFiles.GetSize() )
    {
        // Get Selected Item Details
        CReplayFile& ouFile =
            m_rouManager.m_omReplayFiles.ElementAt( m_nSelecetedNamedLogIndex );
        // Get replay Type
        UpdateData();

        if( m_nReplayMode == 0 ) // Monoshot
        {
            // Disable Cyclic Time Edit control
            m_omEditCycleDelay.EnableWindow( FALSE );
        }
        else
        {
            m_omEditCycleDelay.EnableWindow( TRUE );
            m_omEditCycleDelay.vSetValue( ouFile.m_unCycleTimeDelay );
        }

        // Update type in the data
        ouFile.m_nReplayMode = m_nReplayMode;
    }
}

/**
 * This function will be called during cycle delay editbox
 * change. This will update cycle delay value of replay file
 */
void CReplayFileConfigDlg::OnUpdateEditCycleDelay()
{
    if( m_nSelecetedNamedLogIndex != -1 &&
            m_nSelecetedNamedLogIndex < m_rouManager.m_omReplayFiles.GetSize() )
    {
        // Get Selected Item Details
        CReplayFile& ouFile =
            m_rouManager.m_omReplayFiles.ElementAt( m_nSelecetedNamedLogIndex );
        // Get the delay Value
        UINT unValue = static_cast<UINT>( m_omEditCycleDelay.lGetValue() );
        // Update type in the data
        ouFile.m_unCycleTimeDelay = unValue;
    }
}

/**
 * This function will update interactive option of replay file
 * with the user selection
 */
void CReplayFileConfigDlg::OnChkInteractive()
{
    if( m_nSelecetedNamedLogIndex != -1 &&
            m_nSelecetedNamedLogIndex < m_rouManager.m_omReplayFiles.GetSize() )
    {
        // Get Selected Item Details
        CReplayFile& ouFile =
            m_rouManager.m_omReplayFiles.ElementAt( m_nSelecetedNamedLogIndex );
        // Get the delay Value
        BOOL bInteractive = m_omChkInteractive.GetCheck();
        // Update type in the data
        ouFile.m_bInteractive = bInteractive;
    }
}

/**
 * \req RS_19_03 It shall be possible to add / remove any log file from the input data source set
 *
 * This function will be called when user selects Add button.
 * This will show file selection dialog to select replay file
 * and if the selection is valid this will add the selected file
 * in to the replay file list.
 */
void CReplayFileConfigDlg::OnBtnAddFile()
{
    // Throw File selection dialog to choose replay log file
    DWORD dwFlags =
        OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_EXTENSIONDIFFERENT;
    CFileDialog omFileDlg( TRUE,
                           defSTR_LOG_FILE_EXTENSION,
                           NULL,
                           dwFlags,
                           defLOG_FILTER,
                           NULL );
    //Set the caption
    omFileDlg.m_ofn.lpstrTitle = defSTR_REPLAY_FILE_SELECTION_TITLE;

    // Show File open dialog
    if( omFileDlg.DoModal() == IDOK )
    {
        CReplayFile ouNewReplayFile;
        // Assign the file name and leave the rest to default
        ouNewReplayFile.m_omStrFileName = omFileDlg.GetPathName();
        // Add the data in to the replay file data list
        m_rouManager.m_omReplayFiles.Add( ouNewReplayFile );
        // Insert this new item in to the list
        // Avoid UI update
        m_bUpdating = TRUE;
        // Get the size of the list
        int nIndex = m_omLstcReplayFiles.GetItemCount();
        // Insert at the end
        m_omLstcReplayFiles.InsertItem( nIndex, ouNewReplayFile.m_omStrFileName,
                                        defREPLAY_FILE_IMAGE_INDEX );
        // Update the checkbox status
        m_omLstcReplayFiles.SetCheck( nIndex, ouNewReplayFile.m_bEnabled );

        // This is the first item then enable Replay File components
        if( nIndex == 0 )
        {
            vEnableDisableButtons();
        }

        // Set the selection to this new item
        // Enable UI Update
        m_bUpdating = FALSE;
        m_omLstcReplayFiles.SetItemState( nIndex,
                                          LVIS_SELECTED | LVIS_FOCUSED,
                                          LVIS_SELECTED | LVIS_FOCUSED );
    }
}

/**
 * \req RS_19_03 It shall be possible to add / remove any log file from the input data source set
 *
 * This functuion will handle Delete button event. This will get
 * delete conformation from the user and will remove the
 * selected replay file from the list if user conforms.
 */
void CReplayFileConfigDlg::OnBtnDeleteFile()
{
    if( m_nSelecetedNamedLogIndex != -1 &&
            m_nSelecetedNamedLogIndex < m_rouManager.m_omReplayFiles.GetSize() )
    {
        // Ask user about file delete
        int nResult = AfxMessageBox( defSTR_DELETE_CONFORMATION,
                                     MB_YESNO|MB_ICONQUESTION ) ;

        if ( nResult == IDYES )
        {
            // Avoid UI update
            m_bUpdating = TRUE;
            // Remove the item from the list
            m_omLstcReplayFiles.DeleteItem( m_nSelecetedNamedLogIndex );
            // Remove the item from data list
            m_rouManager.m_omReplayFiles.RemoveAt( m_nSelecetedNamedLogIndex );
            // Set the focus to the next available item
            int nSize = m_omLstcReplayFiles.GetItemCount();

            if( m_nSelecetedNamedLogIndex > ( nSize - 1 ) )
            {
                m_nSelecetedNamedLogIndex = nSize - 1;
            }

            // Enable UI Update
            m_bUpdating = TRUE;

            if( m_nSelecetedNamedLogIndex != -1 )
            {
                // Set the selection
                m_omLstcReplayFiles.SetItemState( m_nSelecetedNamedLogIndex,
                                                  LVIS_SELECTED | LVIS_FOCUSED,
                                                  LVIS_SELECTED | LVIS_FOCUSED );
            }
            else
            {
                // Update Button Status
                vEnableDisableButtons();
            }
        }
    }
}

/**
 * \param bEnable TRUE to enable and FALSE to disable
 *
 * To enable/disable replay UI components
 */
VOID CReplayFileConfigDlg::vEnableReplayComps( BOOL bEnable )
{
    // File Name
    m_omEditReplayFileName.EnableWindow( bEnable );
    // Time Mode
    m_omChkRetainDelay.EnableWindow( bEnable );

    // User Specific Msg Delay
    // Retain Delay is enabled. So Disable this edit control
    if( m_omChkRetainDelay.GetCheck() == TRUE )
    {
        m_omEditMsgDelay.EnableWindow( FALSE );
    }
    else
    {
        m_omEditMsgDelay.EnableWindow( bEnable );
    }

    // Delay Between Cycles
    if( m_nReplayMode == 0 )
    {
        m_omEditCycleDelay.EnableWindow( FALSE );
    }
    else
    {
        m_omEditCycleDelay.EnableWindow( bEnable );
    }

    // Interactive Replay Option
    m_omChkInteractive.EnableWindow( bEnable );
    // Repolay Mode Option buttons
    CWnd* pWnd = NULL;
    // Monoshot
    pWnd = GetDlgItem( IDC_RADIO_REPLAY_MODE_MONO );

    if( pWnd != NULL )
    {
        pWnd->EnableWindow( bEnable );
    }

    // Cyclic
    pWnd = GetDlgItem( IDC_RADIO_REPLAY_MODE_CYCLIC );

    if( pWnd != NULL )
    {
        pWnd->EnableWindow( bEnable );
    }

    // Filter Button
    pWnd = GetDlgItem( IDC_BTN_FILTER );

    if( pWnd != NULL )
    {
        pWnd->EnableWindow( bEnable );
    }
}

static void vPopulateMainSubList(CMainEntryList& DestList, const SFILTERAPPLIED_CAN* psFilterConfigured,
                                 const SFILTERAPPLIED_CAN* psFilterApplied)
{
    ASSERT(psFilterConfigured != NULL);
    DestList.RemoveAll();
    SMAINENTRY sMainEntry;
    sMainEntry.m_omMainEntryName = _T("CAN");

    if (psFilterApplied == NULL)
    {
        SMAINENTRY sMainEntry;
        sMainEntry.m_omMainEntryName = _T("FILTER_SELECTION_CAN");

        for (INT i = 0; i < psFilterConfigured->m_ushTotal; i++)
        {
            SSUBENTRY sSubEntry;
            sSubEntry.m_omSubEntryName.Format("%s",
                                              psFilterConfigured->m_psFilters[i].m_sFilterName.m_acFilterName);
            sMainEntry.m_odUnSelEntryList.AddTail(sSubEntry);
        }
    }
    else
    {
        for (INT i = 0; i < psFilterConfigured->m_ushTotal; i++)
        {
            SSUBENTRY sSubEntry;
            sSubEntry.m_omSubEntryName.Format("%s",
                                              psFilterConfigured->m_psFilters[i].m_sFilterName.m_acFilterName);

            if (SFILTERSET::psGetFilterSetPointer(psFilterApplied->m_psFilters,
                                                  psFilterApplied->m_ushTotal,
                                                  sSubEntry.m_omSubEntryName.GetBuffer(MAX_PATH)) != NULL)
            {
                sMainEntry.m_odSelEntryList.AddTail(sSubEntry);
            }
            else
            {
                sMainEntry.m_odUnSelEntryList.AddTail(sSubEntry);
            }
        }
    }

    DestList.AddTail(sMainEntry);
}

static void vPopulateFilterApplied(const SFILTERAPPLIED_CAN* psFilterConfigured, SFILTERAPPLIED_CAN& sFilterApplied, CMainEntryList& SrcList)
{
    const SMAINENTRY& sMainEntry = SrcList.GetHead();
    int nCount  = (int)sMainEntry.m_odSelEntryList.GetCount();
    sFilterApplied.vClear();
    sFilterApplied.m_psFilters = new SFILTERSET[nCount];
    POSITION pos = sMainEntry.m_odSelEntryList.GetHeadPosition();

    while (pos)
    {
        SSUBENTRY sSubEntry = sMainEntry.m_odSelEntryList.GetNext(pos);
        const PSFILTERSET psTemp = SFILTERSET::psGetFilterSetPointer(psFilterConfigured->m_psFilters,
                                   psFilterConfigured->m_ushTotal, sSubEntry.m_omSubEntryName.GetBuffer(MAX_PATH));
        ASSERT (psTemp != NULL);
        sFilterApplied.m_psFilters[sFilterApplied.m_ushTotal].bClone(*psTemp);
        sFilterApplied.m_ushTotal++;
    }
}

/**
 * This is filter button handler. This will invoke filter
 * configuation dialog and will update filter list if user
 * modifies the filter list of the replay file.
 */
void CReplayFileConfigDlg::OnBtnFilter()
{
    CStringArray omStrFilers;
    // Get List of Filters from Filter Manager
    //CFilterManager::ouGetFilterManager().vGetFilterNameList( omStrFilers );

    if( m_nSelecetedNamedLogIndex != -1 )
    {
        CReplayFile ouReplayFile =
            m_rouManager.m_omReplayFiles[ m_nSelecetedNamedLogIndex ];
        CString omStrTitle;
        omStrTitle.Format( defSTR_REPALY_FILTER_DLG_TITLE,
                           ouReplayFile.m_omStrFileName );
        CMainEntryList DestList;
        vPopulateMainSubList(DestList, m_psFilterConfigured, &(ouReplayFile.m_sFilterApplied) );

        //Show dialog
        if (Filter_ShowSelDlg(this, &DestList) == IDOK)
        {
            vPopulateFilterApplied(m_psFilterConfigured, ouReplayFile.m_sFilterApplied, DestList);
            ouReplayFile.m_sFilterApplied.m_bEnabled = 1;
            m_rouManager.m_omReplayFiles[ m_nSelecetedNamedLogIndex ] = ouReplayFile;
        }
    }
}