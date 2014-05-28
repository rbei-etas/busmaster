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
#include "CAN_ETAS_BOA_stdafx.h"

#include "HardwareListing.h"
#include "Utility\MultiLanguageSupport.h"
//#include "../Application/GettextBusmaster.h"

// Macro to find minimum number
#define MIN(a,b)  ((a) < (b) ? (a) : (b))
/* - Hardware Interface Layer - */
#define defSTR_MISSING_HARDWARE "Required number of hardware are not present\nRequired: %2d    Available: %2d"

#define defSTR_CHANNEL_NAME                "Channel"
#define defSTR_CHANNEL_NAME_FORMAT         "%s %d"

#define defSTR_HARDWARE_COL_NAME           "Hardware"
#define defSTR_CHANNEL_COL_WIDTH           90
#define defSTR_HARDWARE_COL_WIDTH          75

#define defCHANNEL_COL                     0
#define defHARDWARE_COL                    1

#define defSTR_TEMP_CLIENT_NAME            "TEMP_CLIENT"
#define defSTR_TEMP_NET_NAME               "TEMP_NET"

#define defSTR_NOT_FULLY_CONFIGURED        "Required number of hardware are not configured.\nApplication will use default channel association with hardware"
#define defCONNECTED_IMAGE_INDEX            1
#define defDISCONNECTED_IMAGE_INDEX         0
#define defSTR_HW_DISPLAY_FORMAT            "CAN %d"
#define defSIGNAL_ICON_SIZE               16
#define WHITE_COLOR             RGB(255,255,255)

CHardwareListing::CHardwareListing( INTERFACE_HW* psIntrHw,
                                    int nSize, int* pnSelList, CWnd* pParent)
    : CDialog(CHardwareListing::IDD, pParent),
      m_nSize( nSize ),
      m_nSelectedItem( -1)
{
    //{{AFX_DATA_INIT(CHardwareListing)
    //}}AFX_DATA_INIT
    // Create Image List for Hardware
    m_omImageList.Create(IDR_BMP_NET, defSIGNAL_ICON_SIZE, 1, WHITE_COLOR);
    m_psHwInterface = psIntrHw;
    m_pnSelList = pnSelList;
}

CHardwareListing::CHardwareListing()
    : CDialog(CHardwareListing::IDD, nullptr)
{
    // This dialog will not work with out enough constructor parameters
    // Refer previous constructor for the parameter list
    ASSERT( FALSE );
}

/**
 * Called by the framework to exchange and validate dialog data
 */
void CHardwareListing::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LSTC_SELECTED_HW_LIST, m_omSelectedHwList);
    DDX_Control(pDX, IDC_EDIT_NET_NAME, m_omNetName);
    DDX_Control(pDX, IDC_EDIT_FIRMWARE, m_omFirmware);
    DDX_Control(pDX, IDC_EDIT_DRIVER_ID, m_omDriverID);
    DDX_Control(pDX, IDC_LSTC_HW_LIST, m_omHardwareList);
}

BEGIN_MESSAGE_MAP(CHardwareListing, CDialog)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSTC_HW_LIST, OnItemchangedHWList)
    ON_BN_CLICKED(IDC_BUT_SELECT, OnButtonSelect)
    ON_BN_CLICKED(IDC_BUT_REMOVE, OnButtonRemove)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSTC_SELECTED_HW_LIST, OnItemchangedLstcSelectedHwList)
    ON_NOTIFY(NM_CLICK, IDC_LSTC_HW_LIST, OnNMClickLstcHwList)
    ON_NOTIFY(NM_CLICK, IDC_LSTC_SELECTED_HW_LIST, OnNMClickLstcSelectedHwList)
END_MESSAGE_MAP()

/**
 * This function will init hardware list and will find net name
 * associated with the hardware if any.
 *
 * @param[in] psHwIntr Hardware List
 * @param[in] nSize Size of the Hardware List
 */
void CHardwareListing::vSetHardwareList(INTERFACE_HW* /*psHwIntr*/, int nSize)
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
    m_omHardwareList.InsertColumn(0, "");

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
    CString omStrFormat("");
    // Add List of unselected Hw in to the CListCtrl
    int index = 0;
    bool bSelItem;
    UINT nItemCount = 0;
    for( ; index < nSize; index++)
    {
        bSelItem = false;
        //check if the channel is already in selected list
        for ( int i = 0 ; i < nSize && m_pnSelList[i]!=-1 ; i++ )
        {
            if ( m_pnSelList[i] == index )
            {
                bSelItem = true;
                break;
            }
        }

        //Add the channel only if its not in selected list
        if ( !bSelItem )
        {
            omStrFormat.Format( defSTR_HW_DISPLAY_FORMAT, index + 1);
            nImageIndex = defDISCONNECTED_IMAGE_INDEX;
            // Insert List Item
            m_omHardwareList.InsertItem( nItemCount, omStrFormat, nImageIndex);
            // Set the hardware list index as item data
            m_omHardwareList.SetItemData( nItemCount++, index );
        }
    }

    /* Set the selection to the first row */
    m_nSelectedItem = 0;
    m_omHardwareList.SetItemState( m_nSelectedItem,
                                   LVIS_SELECTED | LVIS_FOCUSED,
                                   LVIS_SELECTED | LVIS_FOCUSED);
}

/**
 * This function will be called by the framework before showing a
 * dialog. This will init GUI components of a dialog
 *
 * @return TRUE if the focus isn't modified. FALSE otherwise
 */
BOOL CHardwareListing::OnInitDialog()
{
    CDialog::OnInitDialog();

    /* Update Hardware List with network details */
    vSetHardwareList(m_psHwInterface, m_nSize);

    /* Update the previously selected channel list */
    vSetSelectedList();

    return TRUE;
}

/**
 * This function will be called by the framework when the
 * user selects an item from the Hw List. This will update
 * selected Hw details
 *
 * @param pNMHDR
 *   Address of an NM_LISTVIEW structure that identifies
 *   the item that has changed and specifies its previous
 *   and new states
 */
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

/**
 * This function will be called to update the Hw details. This
 * update Driver handle, Firmware version and network name
 * associated that hardware.
 *
 * @param[in] nIndex Index of the Hw
 */
void CHardwareListing::vUpdateHwDetails(int nIndex)
{
    if(nIndex < m_nSize )
    {
        //Driver Id
        CString omStr;
        omStr.Format("%d", m_psHwInterface[nIndex].m_dwIdInterface);
        m_omDriverID.SetWindowText(omStr);
        //Firmware
        m_omFirmware.SetWindowText(m_psHwInterface[nIndex].m_acDeviceName.c_str());
        //Description
        m_omNetName.SetWindowText( m_psHwInterface[nIndex].m_acDescription.c_str() );

    }
}

/**
 * This function will add the selected hardware in to the
 * selected hardware list.
 */
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

    //Sort Hardware Items
    vSortHardwareItems();


    // Set the focus to the first item
    m_omHardwareList.SetItemState( 0,
                                   LVIS_SELECTED | LVIS_FOCUSED,
                                   LVIS_SELECTED | LVIS_FOCUSED );
    // Update Button Status
    vEnableDisableButtons();

}

/**
 * This function will remove the selected item from the selected
 * hardware list. This will also insert the same in to the
 * available list
 */
void CHardwareListing::OnButtonRemove()
{
    // Get the selected item from the list
    POSITION sPos = m_omSelectedHwList.GetFirstSelectedItemPosition();
    if( sPos != nullptr )
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

        //Sort Hardware Items
        vSortHardwareItems();


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

/**
 * This function will be called when the user selects OK button
 * or on press of Enter key. This will assign channel with
 * hardware handle as per user selection.
 */
void CHardwareListing::OnOK()
{
    // Check the number of selections done by the user
    m_nNoOfHwSelected = m_omSelectedHwList.GetItemCount();
    if (m_nNoOfHwSelected < 1)
    {
        AfxMessageBox(_("Please select atleast one hardware"));
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
        omErr.Format(_("Please select atmost %d hardwares"),  CHANNEL_ALLOWED);
        AfxMessageBox(omErr);
        return;
    }

    /* Call Parent class function to return from DoModal */
    CDialog::OnOK();
}

/**
 * This handler will be called when the dialog is cancenled by
 * selecting Cancel button or by pressing Escape. This handler
 * will assign default values for the channel-hardware
 * association.
 */
void CHardwareListing::OnCancel()
{
    /* Assign Default Association only for supported or available channels */
    for( int nIndex = 0; (UINT)nIndex < MIN( (UINT)m_nSize, CHANNEL_ALLOWED ); nIndex++ )
    {
        m_anSelectedChannels[ nIndex ] = nIndex;
    }
    m_nNoOfHwSelected = MIN( m_nSize, CHANNEL_ALLOWED );

    /* Call Parent class function to return from DoModal */
    CDialog::OnCancel();
}

/**
 * This function will enable or disable UI buttons as per the
 * selection
 */
void CHardwareListing::vEnableDisableButtons()
{
    /* Enable Disable Select Button */
    BOOL bSelectEnable = FALSE;
    BOOL bRemoveEnable = FALSE;

    /* Get the selected hardware count */
    int nSelectedHardware = m_omSelectedHwList.GetItemCount();

    /* Check for the minimum of supported channels or available channels */
    if( (UINT)nSelectedHardware < MIN( (UINT)m_nSize, CHANNEL_ALLOWED ) )
    {
        if( m_omHardwareList.GetSelectedCount() != 0 )
            bSelectEnable = TRUE;
    }

    /* Check the item count in the selected list and enable/disable */
    if( nSelectedHardware != 0 &&
            m_omSelectedHwList.GetSelectedCount() != 0 )
        bRemoveEnable = TRUE;

    /* Update Window. */
    CWnd* pWnd = nullptr;

    /* Select Button */
    pWnd = GetDlgItem( IDC_BUT_SELECT );
    if( pWnd != nullptr )
        pWnd->EnableWindow( bSelectEnable );

    /* Remove Button */
    pWnd = GetDlgItem( IDC_BUT_REMOVE );
    if( pWnd != nullptr )
        pWnd->EnableWindow( bRemoveEnable );

    /* Enable Hardware Details */
    BOOL bHardwareDetailsEnable = TRUE;

    /* Driver ID */
    pWnd = GetDlgItem( IDC_EDIT_DRIVER_ID );
    if( pWnd != nullptr )
    {
        pWnd->EnableWindow( bHardwareDetailsEnable );
        
        /* If disabled then clear the text */
        if( bHardwareDetailsEnable == FALSE )
            pWnd->SetWindowText("");
    }

    /* Network Name */
    pWnd = GetDlgItem( IDC_EDIT_NET_NAME );
    if( pWnd != nullptr )
    {
        pWnd->EnableWindow( bHardwareDetailsEnable );

        /* If disabled then clear the text */
        if( bHardwareDetailsEnable == FALSE )
            pWnd->SetWindowText("");
    }

    /* Firmware */
    pWnd = GetDlgItem( IDC_EDIT_FIRMWARE );
    if( pWnd != nullptr )
    {
        pWnd->EnableWindow( bHardwareDetailsEnable );

        /* If disabled then clear the text */
        if( bHardwareDetailsEnable == FALSE )
            pWnd->SetWindowText("");
    }
}

/**
 * This function will be called by the framework when the
 * user selects an item from the selected Hw List. This will
 * UI buttons status based on the current selection
 *
 * @param[in] pNMHDR
 *   Address of an NM_LISTVIEW structure that identifies
 *   the item that has changed and specifies its previous
 *   and new states
 */
void CHardwareListing::OnItemchangedLstcSelectedHwList( NMHDR* pNMHDR,
        LRESULT* pResult)
{
    /* Update UI Buttons */
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    /* If it is a selection change update the hardware details */
    if(pNMListView->uNewState & LVIS_SELECTED)
    {
        /* Get the selected Item index */
        UINT nSelectedItem = pNMListView->iItem;

        /* Update selected Hw details */
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
    for ( int i = m_nNoOfHwSelected; i < m_nSize ; i++)
    {
        pnList[i] = -1;
    }
    return m_nNoOfHwSelected;
}

/**
 * @brief This function will set the selected channel list
 * @param[in] pnList, contains the list of selected channels
 * @author Arunkumar Karri
 */
void CHardwareListing::vSetSelectedList()
{
    /* Insert the selected item in to the selected list */
    int nItem;
    CString omStrChannel;
    CString omStrHardware;

    for ( int i = 0 ; i < m_nSize && m_pnSelList[i]!=-1 ; i++)
    {
        nItem = m_omSelectedHwList.GetItemCount();

        /* Format channel information */
        omStrChannel.Format( defSTR_CHANNEL_NAME_FORMAT,
                             _(defSTR_CHANNEL_NAME),
                             nItem + 1 );

        /* Get the Hardware name */
        omStrHardware.Format( defSTR_HW_DISPLAY_FORMAT, m_pnSelList[i] + 1);
        int nImageIndex = defDISCONNECTED_IMAGE_INDEX;

        /* Insert the new item in to the selected list */
        m_omSelectedHwList.InsertItem( nItem, omStrChannel, nImageIndex );

        /* Set the Hardware Name */
        m_omSelectedHwList.SetItemText( nItem, 1, omStrHardware );

        /* Set the array index */
        m_omSelectedHwList.SetItemData( nItem, m_pnSelList[i] );
    }

    /* Set the focus to the first item */
    m_omHardwareList.SetItemState( 0,
                                   LVIS_SELECTED | LVIS_FOCUSED,
                                   LVIS_SELECTED | LVIS_FOCUSED );
}

void CHardwareListing::OnNMClickLstcHwList(NMHDR* pNMHDR, LRESULT* pResult)
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    /* If it is a selection change update the hardware details */
    INT nSelectedItem = pNMListView->iItem;
    if(nSelectedItem > -1)
    {
        /* Get the selected Item index */
        m_nSelectedItem = pNMListView->iItem;
        /* Update selected Hw details */
        vUpdateHwDetails( (INT)m_omHardwareList.GetItemData( m_nSelectedItem ) );
    }

    /* Update Button Status */
    vEnableDisableButtons();
    *pResult = 0;
}

void CHardwareListing::OnNMClickLstcSelectedHwList(NMHDR* pNMHDR, LRESULT* pResult)
{
    /* Update UI Buttons */
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    /* If it is a selection change update the hardware details */
    INT nSelectedItem = pNMListView->iItem;
    if(nSelectedItem > -1)
    {
        /* Update selected Hw details */
        vUpdateHwDetails( (INT)m_omSelectedHwList.GetItemData( nSelectedItem ) );
    }
    vEnableDisableButtons();
    *pResult = 0;
}
void CHardwareListing::vSortHardwareItems()
{
    /* clear map data */
    if(!mHardwareListMap.empty())
        mHardwareListMap.clear();

    int nItemCount = m_omHardwareList.GetItemCount();
    for(int nIndex = 0; nIndex < nItemCount; nIndex++)
    {
        m_pouHardwareContainer = new HARDWARE_CONTAINER();
        m_pouHardwareContainer->m_omHardwareName  = m_omHardwareList.GetItemText(nIndex,0);

        /* Get the array index */
        m_pouHardwareContainer->m_omDriverId  = m_omHardwareList.GetItemData( nIndex );

        /* Insert List Item */
        mHardwareListMap.insert ( Int_Pair ( m_pouHardwareContainer->m_omDriverId , m_pouHardwareContainer ) );

    }

    m_omHardwareList.DeleteAllItems();
    int iCount = 0;
    for(int nIndex = 0; nIndex < m_nSize; nIndex++)
    {
        m_pIter = mHardwareListMap.find(nIndex);

        /* Hardware found */
        if(m_pIter != mHardwareListMap.end() )
        {
            PHARDWARE_CONTAINER pTempHardware = m_pIter->second;

            /* insert List Item */
            m_omHardwareList.InsertItem( iCount, pTempHardware->m_omHardwareName, 0);
            /* Set the hardware list index as item data */
            m_omHardwareList.SetItemData( iCount++, pTempHardware->m_omDriverId );

        }
    }

    for (m_pIter = mHardwareListMap.begin(); m_pIter != mHardwareListMap.end(); ++m_pIter)
    {
        /* release Memory */
        delete(m_pIter->second);
        m_pIter->second = nullptr;
    }

    /* clear map data */
    mHardwareListMap.clear();
}

