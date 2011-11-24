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
 * \file      HardwareListing.cpp
 * \brief     Interface file for CHardwareListing class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CHardwareListing class
 */

// Standard include header
#include "CAN_Vector_XL_stdafx.h"

#include "HardwareListing.h"
#include ".\hardwarelisting.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// Macro to find minimum number
#define MIN(a,b)  ((a) < (b) ? (a) : (b))
/* - Hardware Interface Layer - */
#define defSTR_MISSING_HARDWARE _T("Required number of hardware are not present\nRequired: %2d    Available: %2d")

#define defSTR_CHANNEL_NAME                _T("Channel")
#define defSTR_CHANNEL_NAME_FORMAT         _T("%s %d")

#define defSTR_HARDWARE_COL_NAME           _T("Hardware")
#define defSTR_CHANNEL_COL_WIDTH           90
#define defSTR_HARDWARE_COL_WIDTH          75

#define defCHANNEL_COL                     0
#define defHARDWARE_COL                    1

#define defSTR_TEMP_CLIENT_NAME            _T("TEMP_CLIENT")
#define defSTR_TEMP_NET_NAME               _T("TEMP_NET")

#define defSTR_NOT_FULLY_CONFIGURED        _T("Required number of hardware are not configured.\nApplication will use default channel association with hardware")
#define defCONNECTED_IMAGE_INDEX            1
#define defDISCONNECTED_IMAGE_INDEX         0
#define defSTR_HW_DISPLAY_FORMAT            _T("CAN %d")
#define defSIGNAL_ICON_SIZE               16
#define WHITE_COLOR             RGB(255,255,255)

/*******************************************************************************
 Function Name  : CHardwareListing
 Description    : Constructor for dialog box
 Member of      : CHardwareListing
 Functionality  : -
 Author(s)      : Raja N
 Date Created   : 08.09.2004
 Modifications  : Raja N on 17.3.2005
                  Implemented code review comments
*******************************************************************************/
CHardwareListing::CHardwareListing( INTERFACE_HW * psIntrHw,
                                    int nSize,CWnd* pParent /*=NULL*/)
    : CDialog(CHardwareListing::IDD, pParent),
      m_nSize( nSize ),
      m_nSelectedItem( -1)
{
    //{{AFX_DATA_INIT(CHardwareListing)
    //}}AFX_DATA_INIT
    // Create Image List for Hardware
    m_omImageList.Create(IDR_BMP_NET, defSIGNAL_ICON_SIZE, 1, WHITE_COLOR);
    m_psHwInterface = psIntrHw;
}

/*******************************************************************************
 Function Name  : CHardwareListing
 Description    : Standard default constructor for dialog box
 Member of      : CHardwareListing
 Functionality  : -
 Author(s)      : Raja N
 Date Created   : 08.09.2004
 Modifications  : Raja N on 14.03.2005, Prevented implicit access with out
                  enough arguments
*******************************************************************************/
CHardwareListing::CHardwareListing()
        : CDialog(CHardwareListing::IDD, NULL)
{
    // This dialog will not work with out enough constructor parameters
    // Refer previous constructor for the parameter list
    ASSERT( FALSE );
}

/******************************************************************************
 Function Name   : DoDataExchange
 Input(s)        : CDataExchange* pDX
 Output          :  -
 Functionality   : Called by the framework to exchange and validate dialog data
 Member of       : CHardwareListing
 Friend of       : -
 Author(s)       : Raja N
 Date Created    : 13.09.2004
 Modifications   : Raja N on 14.03.2005, Added DDX support for selected hardware
                   list
******************************************************************************/
void CHardwareListing::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CHardwareListing)
    DDX_Control(pDX, IDC_LSTC_SELECTED_HW_LIST, m_omSelectedHwList);
    DDX_Control(pDX, IDC_EDIT_NET_NAME, m_omNetName);
    DDX_Control(pDX, IDC_EDIT_FIRMWARE, m_omFirmware);
    DDX_Control(pDX, IDC_EDIT_DRIVER_ID, m_omDriverID);
    DDX_Control(pDX, IDC_LSTC_HW_LIST, m_omHardwareList);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHardwareListing, CDialog)
    //{{AFX_MSG_MAP(CHardwareListing)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSTC_HW_LIST, OnItemchangedHWList)
    ON_BN_CLICKED(IDC_BUT_BLINK, OnBlinkHw)
    ON_BN_CLICKED(IDC_BUT_SELECT, OnButtonSelect)
    ON_BN_CLICKED(IDC_BUT_REMOVE, OnButtonRemove)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSTC_SELECTED_HW_LIST, OnItemchangedLstcSelectedHwList)
    //}}AFX_MSG_MAP
    ON_NOTIFY(NM_CLICK, IDC_LSTC_HW_LIST, OnNMClickLstcHwList)
    ON_NOTIFY(NM_CLICK, IDC_LSTC_SELECTED_HW_LIST, OnNMClickLstcSelectedHwList)
END_MESSAGE_MAP()


/*******************************************************************************
 Function Name  : vSetHardwareList
 Input(s)       : hHwList - Hardware List
                  nSize   - Size of the Hardware Lsit
 Output         :  -
 Functionality  : This function will init hardware list and will find net name
                  associated with the hardware if any.
 Member of      : CHardwareListing
 Author(s)      : Raja N
 Date Created   : 08.09.2004
 Modifications  : Raja N on 14.03.2005, Create list control to hold selected
                  hardware items
*******************************************************************************/
void CHardwareListing::vSetHardwareList(INTERFACE_HW * /*psHwIntr*/, int nSize)
{
	//all the application related to UI shall be performed only if
	//BUSMASTER is not called from COM
	
	// Clear the lsit
	m_omHardwareList.DeleteAllItems();
	// Remove all columns
	m_omHardwareList.DeleteColumn(0);
	// Set the Image List
	m_omHardwareList.SetImageList(&m_omImageList, LVSIL_NORMAL );
	// Set Image List for selected list
	m_omSelectedHwList.SetImageList(&m_omImageList, LVSIL_SMALL );
	
	// Insert First Columns
	// The style is Icon. So insert Empty column
	m_omHardwareList.InsertColumn(0, STR_EMPTY);
	
	// Create selelected list columns
	m_omSelectedHwList.InsertColumn( defCHANNEL_COL ,
		defSTR_CHANNEL_NAME );
	m_omSelectedHwList.SetColumnWidth( defCHANNEL_COL,
		defSTR_CHANNEL_COL_WIDTH );
	m_omSelectedHwList.InsertColumn( defHARDWARE_COL,
		defSTR_HARDWARE_COL_NAME );
	m_omSelectedHwList.SetColumnWidth( defHARDWARE_COL,
		defSTR_HARDWARE_COL_WIDTH );

	int nImageIndex = 0;
	CString omStrFormat(STR_EMPTY);
	// Add List of Hw in to the CListCtrl
	int index = 0;
	for( ; index < nSize; index++)
	{
		omStrFormat.Format( defSTR_HW_DISPLAY_FORMAT, index + 1);
        nImageIndex = defDISCONNECTED_IMAGE_INDEX;
		// Insert List Item
		m_omHardwareList.InsertItem( index, omStrFormat, nImageIndex);
		// Set the hardware list index as item data
		m_omHardwareList.SetItemData( index, index );
	}
	// Clear selection list
	for( index = 0; index < CHANNEL_ALLOWED; index++ )
	{
		m_anSelectedChannels[ index ] = -1;
	}
	
	// Set the selection to the first row
	m_nSelectedItem = 0;
	m_omHardwareList.SetItemState( m_nSelectedItem,
		LVIS_SELECTED | LVIS_FOCUSED,
		LVIS_SELECTED | LVIS_FOCUSED);
}

/*******************************************************************************
 Function Name  : OnInitDialog
 Input(s)       :  -
 Output         : BOOL - TRUE if the focus isn't modified. FALSE otherwise
 Functionality  : This function will be called by the framework before showing a
                  dialog. This will init GUI components of a dialog
 Member of      : CHardwareListing
 Author(s)      : Raja N
 Date Created   : 08.09.2004
*******************************************************************************/
BOOL CHardwareListing::OnInitDialog() 
{
    CDialog::OnInitDialog();
    // Update Hardware List with network details
    vSetHardwareList(m_psHwInterface, m_nSize);

    return TRUE;
}


/******************************************************************************
  Function Name   : OnItemchangedHWList
  Input(s)        : Address of an NM_LISTVIEW structure that identifies
                    the item that has changed and specifies its previous
                    and new states
  Output          : -
  Functionality   : This function will be called by the framework when the 
                    user selects an item from the Hw List. This will update
                    selected Hw details
  Member of       : CHardwareListing
  Author(s)       : Raja N
  Date Created    : 08.09.2004
  Modifications   : Raja N on 14.03.2005, Modified to get item data for getting
                    actual index.
******************************************************************************/
void CHardwareListing::OnItemchangedHWList(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    // If it is a selection change update the hardware details
    if(pNMListView->uNewState & LVIS_SELECTED)
    {
        // Get the selected Item index
        m_nSelectedItem = pNMListView->iItem;
        // Update selected Hw details
        vUpdateHwDetails( (INT)m_omHardwareList.GetItemData( m_nSelectedItem ) );
    }
    // Update Button Status
    vEnableDisableButtons();
    *pResult = 0;
}

/*******************************************************************************
 Function Name  : vUpdateHwDetails
 Input(s)       : nIndex - Index of the Hw
 Output         :  -
 Functionality  : This function will be called to update the Hw details. This
                  update Driver handle, Firmware version and network name
                  associated that hardware.
 Member of      : CHardwareListing
 Author(s)      : Raja N
 Date Created   : 08.09.2004
*******************************************************************************/
void CHardwareListing::vUpdateHwDetails(int nIndex)
{
    if(nIndex < m_nSize )
    {
		//Driver Id
		CString omStr;
		omStr.Format(_T("%d"), m_psHwInterface[nIndex].m_dwIdInterface);
		m_omDriverID.SetWindowText(omStr);
		//Firmware
		m_omFirmware.SetWindowText(m_psHwInterface[nIndex].m_acDeviceName);
		//Description
        m_omNetName.SetWindowText( m_psHwInterface[nIndex].m_acDescription );
		
    }
}

/*******************************************************************************
 Function Name  : OnBlinkHw
 Input(s)       :  - 
 Output         :  -
 Functionality  : This function will make the selected hardware LED to blink.
                  This is done by sending the CAN_PARAM_USER_LOCATION_INFO
                  string continously. The LED blink will indicate Hw access.
 Member of      : CHardwareListing
 Author(s)      : Raja N
 Date Created   : 08.09.2004
 Modifications  : Raja N on 14.3.2005
                  Modified blinking logic. Now a temp net and client will be
                  created. Temp Client will be connected with temp net so that
                  LED in the hardware will blink and then will be disconnected.
                  Temp resources will be freed then.
 Modifications  : Raja N on 14.3.2005
                  Modified function name to get free net handle
 Modifications  : Raja N on 22.3.2005
                  Modified as per testing. Refered item data for getting Hw
                  index rather then item index.
*******************************************************************************/
void CHardwareListing::OnBlinkHw()
{
}
//{
//    // Get the Hardware index
//    // Get the Item data. Don't refer the index directly as index can be changed
//    // because of add/remove
//    int nIndex = m_omHardwareList.GetItemData( m_nSelectedItem );
//    // If it is valid
//    if( nIndex != -1 )
//    {
//        char cBuff[ defUSB_PARAM_BUFFER_SIZE ];
//
//        SHWNETLIST sSelected = m_sHwNetList[ nIndex ];
//        BOOL bNetCreated = FALSE;
//        // If net is not yet created
//        if( sSelected.m_hNet == 0 )
//        {
//            int nNetHandle = CHardwareInterface::s_podGetSingletonInstance()->
//                            nGetFreeNetHandle();
//            sSelected.m_hNet = static_cast<UCHAR>( nNetHandle );
//            // Create a temporary network
//            CAN_RegisterNet( sSelected.m_hNet ,
//                             defSTR_TEMP_NET_NAME,
//                             sSelected.m_hHw,
//                             defBAUD_RATE );
//            bNetCreated = TRUE;
//        }
//
//        HCANCLIENT hClient;
//        int nReturn;
//        // Create Temp Client
//        nReturn = CAN_RegisterClient( defSTR_TEMP_CLIENT_NAME, 0 , &hClient);
//        // Set the Hardware in Listen only mode
//        CAN_SetHwParam( sSelected.m_hHw, CAN_PARAM_LISTEN_ONLY, TRUE );
//        // Get the network Name
//        CAN_GetNetParam( sSelected.m_hNet, CAN_PARAM_NAME, cBuff,
//                           defUSB_PARAM_BUFFER_SIZE );
//        HCANNET hRetNet;
//        // Connect to the net. Wait for LED to blink and then disconnect
//        CAN_ConnectToNet( hClient, cBuff, &hRetNet );
//        Sleep(250);
//        CAN_DisconnectFromNet( hClient, sSelected.m_hNet );
//
//        // Set the Hardware in Listen only mode
//        CAN_SetHwParam( sSelected.m_hHw, CAN_PARAM_LISTEN_ONLY, FALSE );
//        // Delete the Client
//        CAN_RemoveClient( hClient );
//
//        // Delete Temp net if created
//        if( bNetCreated == TRUE )
//        {
//            CAN_RemoveNet( sSelected.m_hNet );
//        }
//    }
//}

/*******************************************************************************
  Function Name  : OnButtonSelect
  Input(s)       : -
  Output         : -
  Functionality  : This function will add the selected hardware in to the
                   selected hardware list.
  Member of      : CHardwareListing
  Author(s)      : Raja N
  Date Created   : 25.2.2005
  Modifications  : 
*******************************************************************************/
void CHardwareListing::OnButtonSelect() 
{
    int nSelected = m_nSelectedItem;
    // Insert the selected item in to the selected list
    int nItem = m_omSelectedHwList.GetItemCount();
    CString omStrChannel;//(STR_EMPTY);
    CString omStrHardware;
    int nArrayIndex = -1;
    // Get the data
    // Format channel information
    omStrChannel.Format( defSTR_CHANNEL_NAME_FORMAT,
                         defSTR_CHANNEL_NAME,
                         nItem + 1 );
    // Get the Hardware name
    omStrHardware = m_omHardwareList.GetItemText( nSelected, 0 );
    // Get the array index
    nArrayIndex = (INT)m_omHardwareList.GetItemData( nSelected );
    int nImageIndex = defDISCONNECTED_IMAGE_INDEX;
    // Insert the new item in to the selected list
    m_omSelectedHwList.InsertItem( nItem, omStrChannel, nImageIndex );
    // Set the Hardware Name
    m_omSelectedHwList.SetItemText( nItem, 1, omStrHardware );
    // Set the array index
    m_omSelectedHwList.SetItemData( nItem, nArrayIndex );
    
    // Remove the item from the list
    m_omHardwareList.DeleteItem( nSelected );
    // Set the focus to the first item
    m_omHardwareList.SetItemState( 0,
                                   LVIS_SELECTED | LVIS_FOCUSED,
                                   LVIS_SELECTED | LVIS_FOCUSED );
    // Update Button Status
    vEnableDisableButtons();

}

/*******************************************************************************
  Function Name  : OnButtonRemove
  Input(s)       : -
  Output         : -
  Functionality  : This function will remove the selected item from the selected
                   hardware list. This will also insert the same in to the
                   available list
  Member of      : CHardwareListing
  Author(s)      : Raja N
  Date Created   : 21.2.2005
  Modifications  : Raja N on 17.03.2005
                   Added code to change channel ID of rest of items when a
                   channel is deleted from the selected channel list
*******************************************************************************/
void CHardwareListing::OnButtonRemove() 
{
    // Get the selected item from the list
    POSITION sPos = m_omSelectedHwList.GetFirstSelectedItemPosition();
    if( sPos != NULL )
    {
        int nSelectedItem = m_omSelectedHwList.GetNextSelectedItem(sPos);
        int nArrayIndex = (INT)m_omSelectedHwList.GetItemData( nSelectedItem );
        int nImageIndex;
        
        nImageIndex = defDISCONNECTED_IMAGE_INDEX;

        // Insert this item in to the available list
        // Calculate new item index
        int nItemCount = m_omHardwareList.GetItemCount();
        // Insert the new entry
        m_omHardwareList.InsertItem( nItemCount,
                            m_omSelectedHwList.GetItemText( nSelectedItem, 1),
                            nImageIndex );
        // Set the hardware list index as item data.
        m_omHardwareList.SetItemData( nItemCount, nArrayIndex );
        // Remove the item from the selected list
        m_omSelectedHwList.DeleteItem( nSelectedItem );
        // Change the channel text approp.
        int nItem = m_omSelectedHwList.GetItemCount();
        // Check whether update is required or not
        if( nSelectedItem < nItem )
        {
            // Format string
            CString omStrChannel;
            int nItemsToUpdate = nItem - nSelectedItem;
            // Loopt through list of items
            for( int nIndex = 0; nIndex < nItemsToUpdate; nIndex++ )
            {
                // Update format string
                omStrChannel.Format( defSTR_CHANNEL_NAME_FORMAT,
                                     defSTR_CHANNEL_NAME,
                                     nSelectedItem + nIndex + 1 );
                // Update Text 
                m_omSelectedHwList.SetItemText( nSelectedItem + nIndex,
                                                0,
                                                omStrChannel );
            }
        }
        // Update UI button status
        vEnableDisableButtons();
    }
}

/*******************************************************************************
  Function Name  : OnOK
  Input(s)       : -
  Output         : -
  Functionality  : This function will be called when the user selects OK button
                   or on press of Enter key. This will assign channel with
                   hardware handle as per user selection.
  Member of      : CHardwareListing
  Author(s)      : Raja N
  Date Created   : 21.2.2005
  Modifications  : 
*******************************************************************************/
void CHardwareListing::OnOK() 
{
    // Check the number of selections done by the user
    m_nNoOfHwSelected = m_omSelectedHwList.GetItemCount();
    if (m_nNoOfHwSelected < 1)
    {
        AfxMessageBox(_T("Please select atleast one hardware"));
        return;
    }
    // Number of hardware will be used
    int nNumberOfHwUsed = MIN( m_nSize, CHANNEL_ALLOWED );
    if( m_nNoOfHwSelected < nNumberOfHwUsed )
    {
        // Show alert message telling that default association will be
        // used.
        // Assign selected values first
		int nIndex = 0;
        for( ; nIndex < m_nNoOfHwSelected; nIndex++ )
        {
            // Store the index of hardware list.
            m_anSelectedChannels[ nIndex ] =
                   (INT)m_omSelectedHwList.GetItemData( nIndex );
        }
        // Assign remaining items
        for( nIndex = 0; nIndex < ( nNumberOfHwUsed - m_nNoOfHwSelected ); nIndex++ )
        {
            m_anSelectedChannels[ m_nNoOfHwSelected + nIndex ] =
                                    (INT)m_omHardwareList.GetItemData( nIndex );
        }
    }
    // User has associated all available hardware with a channel number
    else if( m_nNoOfHwSelected == nNumberOfHwUsed )
    {
        // Assign index of hardware list
        for( int nIndex = 0; nIndex < m_nNoOfHwSelected; nIndex++ )
        {
            // Store the index of hardware list.
            m_anSelectedChannels[ nIndex ] =
                  (INT)m_omSelectedHwList.GetItemData( nIndex );
        }
    }
    // This is invalid case. There could be some problem in list
    // control manipulation because the selected item count exceeds the
    // number of hardware available!!!!!
    else
    {        
        CString omErr;
        omErr.Format(_T("Please select atmost %d hardwares"),  CHANNEL_ALLOWED);
        AfxMessageBox(omErr);
        return;
    }
    // Call Parent class function to return from DoModal
    CDialog::OnOK();
}

/*******************************************************************************
  Function Name  : OnCancel
  Input(s)       : -
  Output         : -
  Functionality  : This handler will be called when the dialog is cancenled by
                   selecting Cancel button or by pressing Escape. This handler
                   will assign default values for the channel-hardware
                   association.
  Member of      : CHardwareListing
  Author(s)      : Raja N
  Date Created   : 21.2.2005
  Modifications  : Raja N on 22.03.2005
                   Added check to find the minimum value of available hardware
                   Vs channels supported
*******************************************************************************/
void CHardwareListing::OnCancel() 
{
    // Show alert message telling that default association will be
    // used.
    AfxMessageBox( defSTR_NOT_FULLY_CONFIGURED );
    // Assign Default Association
    // only for supported or available channels
    for( int nIndex = 0; (UINT)nIndex < MIN( (UINT)m_nSize, CHANNEL_ALLOWED ); nIndex++ )
    {
        m_anSelectedChannels[ nIndex ] = nIndex;
    }
    m_nNoOfHwSelected = MIN( m_nSize, CHANNEL_ALLOWED );
    // Call Parent class function to return from DoModal
    CDialog::OnCancel();
}

/*******************************************************************************
  Function Name  : vEnableDisableButtons
  Input(s)       : -
  Output         : -
  Functionality  : This function will enable or disable UI buttons as per the
                   selection
  Member of      : CHardwareListing
  Author(s)      : Raja N
  Date Created   : 21.2.2005
  Modifications  : Pradeep Kadoor, 14.12.2010, Disabling of hardware details removed
*******************************************************************************/
void CHardwareListing::vEnableDisableButtons()
{
    BOOL bSelectEnable = FALSE;
    BOOL bRemoveEnable = FALSE;

    // Enable Disable Select Button
    // Get the selected hardware count
    int nSelectedHardware = m_omSelectedHwList.GetItemCount();
    // Check for the minimum of supported channels or available channels
    if( (UINT)nSelectedHardware < MIN( (UINT)m_nSize, CHANNEL_ALLOWED ) )
    {
        if( m_omHardwareList.GetSelectedCount() != 0 )
        {
            bSelectEnable = TRUE;
        }
    }

    // Check the item count in the selected list and enable/disable
    if( nSelectedHardware != 0 &&
        m_omSelectedHwList.GetSelectedCount() != 0 )
    {
        bRemoveEnable = TRUE;
    }

    // Update Window.
    CWnd *pWnd = NULL;
    // Select Button
    pWnd = GetDlgItem( IDC_BUT_SELECT );
    if( pWnd != NULL )
    {
        pWnd->EnableWindow( bSelectEnable );
    }
    // Remove Button
    pWnd = GetDlgItem( IDC_BUT_REMOVE );
    if( pWnd != NULL )
    {
        pWnd->EnableWindow( bRemoveEnable );
    }
    // Enable Hardware Details
    BOOL bHardwareDetailsEnable = TRUE;
    

    // Driver ID
    pWnd = GetDlgItem( IDC_EDIT_DRIVER_ID );
    if( pWnd != NULL )
    {
        pWnd->EnableWindow( bHardwareDetailsEnable );
        // If disabled then clear the text
        if( bHardwareDetailsEnable == FALSE )
        {
            pWnd->SetWindowText( STR_EMPTY );
        }
    }
    // Network Name
    pWnd = GetDlgItem( IDC_EDIT_NET_NAME );
    if( pWnd != NULL )
    {
        pWnd->EnableWindow( bHardwareDetailsEnable );
        // If disabled then clear the text
        if( bHardwareDetailsEnable == FALSE )
        {
            pWnd->SetWindowText( STR_EMPTY );
        }
    }
    // Firmware
    pWnd = GetDlgItem( IDC_EDIT_FIRMWARE );
    if( pWnd != NULL )
    {
        pWnd->EnableWindow( bHardwareDetailsEnable );
        // If disabled then clear the text
        if( bHardwareDetailsEnable == FALSE )
        {
            pWnd->SetWindowText( STR_EMPTY );
        }
    }
    // Blink Button
    pWnd = GetDlgItem( IDC_BUT_BLINK );
    if( pWnd != NULL )
    {
        pWnd->EnableWindow( FALSE);/*Kadoor bHardwareDetailsEnable );*/
    }
}

/******************************************************************************
  Function Name   : OnItemchangedLstcSelectedHwList
  Input(s)        : Address of an NM_LISTVIEW structure that identifies
                    the item that has changed and specifies its previous
                    and new states
  Output          : -
  Functionality   : This function will be called by the framework when the 
                    user selects an item from the selected Hw List. This will
                    UI buttons status based on the current selection
  Member of       : CHardwareListing
  Author(s)       : Raja N
  Date Created    : 08.09.2004
  Modifications   :
******************************************************************************/
void CHardwareListing::OnItemchangedLstcSelectedHwList( NMHDR* pNMHDR,
                                                        LRESULT* pResult)
{
    //NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    // Update UI Buttons
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    // If it is a selection change update the hardware details
    if(pNMListView->uNewState & LVIS_SELECTED)
    {
        // Get the selected Item index
        UINT nSelectedItem = pNMListView->iItem;
        // Update selected Hw details
        vUpdateHwDetails( (INT)m_omSelectedHwList.GetItemData( nSelectedItem ) );
    }
    vEnableDisableButtons();
    *pResult = 0;
}

INT CHardwareListing::nGetSelectedList(int* pnList)
{    
    for (int i = 0; i < m_nNoOfHwSelected; i++)
    {
        pnList[i] = m_anSelectedChannels[i];
    }
    return m_nNoOfHwSelected;
}
void CHardwareListing::OnNMClickLstcHwList(NMHDR *pNMHDR, LRESULT *pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    // If it is a selection change update the hardware details
    INT nSelectedItem = pNMListView->iItem;
    if(nSelectedItem > -1)
    {
        // Get the selected Item index
        m_nSelectedItem = pNMListView->iItem;
        // Update selected Hw details
        vUpdateHwDetails( (INT)m_omHardwareList.GetItemData( m_nSelectedItem ) );
    }
    // Update Button Status
    vEnableDisableButtons();
    *pResult = 0;
}

void CHardwareListing::OnNMClickLstcSelectedHwList(NMHDR *pNMHDR, LRESULT *pResult)
{
    //NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    // Update UI Buttons
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    // If it is a selection change update the hardware details
    INT nSelectedItem = pNMListView->iItem;
    if(nSelectedItem > -1)
    {
        // Update selected Hw details
        vUpdateHwDetails( (INT)m_omSelectedHwList.GetItemData( nSelectedItem ) );
    }
    vEnableDisableButtons();
    *pResult = 0;
}
