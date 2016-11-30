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
 * \brief     Interface file for CHardwareListingCAN class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CHardwareListingCAN class
 */

// Standard include header
//#include "../Application/GettextBusmaster.h"
#include "HardwareListingCAN.h"
#include "Utility\MultiLanguageSupport.h"
#include "utility\Utility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// Macro to find minimum number
#define MIN(a,b)  ((a) < (b) ? (a) : (b))
/* - Hardware Interface Layer - */
#define defSTR_MISSING_HARDWARE _("Required number of hardware are not present\nRequired: %2d    Available: %2d")

#define defSTR_CHANNEL_NAME                _("Channel")
#define defSTR_CHANNEL_NAME_FORMAT         _("%s %d")

#define defSTR_HARDWARE_COL_NAME           _("Hardware")
#define defSTR_CHANNEL_COL_WIDTH           68
#define defSTR_HARDWARE_COL_WIDTH          250
#define defSTR_LISTED_HARDWARE_COL_WIDTH   243	

#define defSTR_CAN_HW_DISPLAY_FORMAT            _("CAN %d")


#define defCHANNEL_COL                     1
#define defHARDWARE_COL                    0

#define defSTR_TEMP_CLIENT_NAME            _("TEMP_CLIENT")
#define defSTR_TEMP_NET_NAME               _("TEMP_NET")

#define defSTR_NOT_FULLY_CONFIGURED        _("Required number of hardware are not configured.\nApplication will use default channel association with hardware")
#define defCONNECTED_IMAGE_INDEX            1
#define defDISCONNECTED_IMAGE_INDEX         0
#define defSTR_CHANNEL_HW_DISPLAY_FORMAT            _("%s %d")
#define defSIGNAL_ICON_SIZE               16
#define WHITE_COLOR             RGB(255,255,255)

#define  defFACT_FREQUENCY               1000000.0
#define  defFACT_BAUD_RATE               1000.0
#define  defVALID_DECIMAL_VALUE          0.004
#define  defMAX_BRP						 64
#define  defMAX_NBT						 25
#define  defMIN_NBT						 8
#define  defmcROUND5(X)        X - (unsigned int)X >= 0.5 ? (unsigned int)(X+0.5):(unsigned int)(X)
#define defSTR_HW_DISPLAY_FORMAT            "CAN %d"
#define  defSET                           1
#define  defRESET                         0
#define  defMIN_BRP                       1
#define  defBAUD_RATE_MESSAGE         "Nearest Valid Baud Rate is %s kBit/s. Do You want to change it ?"
/*******************************************************************************
 Function Name  : CHardwareListingCAN
 Description    : Constructor for dialog box
 Member of      : CHardwareListingCAN
 Functionality  : -
 Author(s)      : Raja N
 Date Created   : 08.09.2004
 Modifications  : Raja N on 17.3.2005
                  Implemented code review comments
*******************************************************************************/
CHardwareListingCAN::CHardwareListingCAN( INTERFACE_HW* psIntrHw,
                                    int nSize, int* pnSelList, ETYPE_BUS eBusType, int nMaxChannels, CWnd* pParent /*=nullptr*/, PSCONTROLLER_DETAILS InitData, IChangeRegisters* pAdvancedSetting)
    : CDialog(CHardwareListingCAN::IDD, pParent),
      m_nSize( nSize ),
      m_nSelectedItem( -1)
	  , m_omStrEditBaudRate(_T(""))
	  , m_omCANFDStrEditBaudRate(_T(""))
	  , m_bCANFD(FALSE)
{
    //{{AFX_DATA_INIT(CHardwareListingCAN)
    //}}AFX_DATA_INIT
    // Create Image List for Hardware
    m_eBus = eBusType;
    m_nMaxChannelAllowed = nMaxChannels;
    m_omImageList.Create(IDR_BMP_NET, defSIGNAL_ICON_SIZE, 1, WHITE_COLOR);
    m_psHwInterface = psIntrHw;
    m_pnSelList = pnSelList;
	m_pControllerDetails = InitData;
	m_pAdvChnlConfig = pAdvancedSetting;
	m_pParent = pParent;
	m_bDialogCancel    = FALSE;
	m_mapChannelToCANFDStatus.clear();
	if (m_pControllerDetails != nullptr)
	{
		for (int i = 0; i < nSize; i++)
		{
			m_mapChannelToCANFDStatus[m_psHwInterface[i].m_acDescription] = m_pControllerDetails[i].m_bSupportCANFD;
			m_pnCANDFDValue[i] = m_pControllerDetails[i].m_bSupportCANFD;
		}
	}
}

/*******************************************************************************
 Function Name  : CHardwareListingCAN
 Description    : Standard default constructor for dialog box
 Member of      : CHardwareListingCAN
 Functionality  : -
 Author(s)      : Raja N
 Date Created   : 08.09.2004
 Modifications  : Raja N on 14.03.2005, Prevented implicit access with out
                  enough arguments
*******************************************************************************/
CHardwareListingCAN::CHardwareListingCAN()
    : CDialog(CHardwareListingCAN::IDD, nullptr)
	, m_omStrEditBaudRate(_T(""))
	, m_omCANFDStrEditBaudRate(_T(""))
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
 Member of       : CHardwareListingCAN
 Friend of       : -
 Author(s)       : Raja N
 Date Created    : 13.09.2004
 Modifications   : Raja N on 14.03.2005, Added DDX support for selected hardware
                   list
******************************************************************************/
void CHardwareListingCAN::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CHardwareListingCAN)
	DDX_Control(pDX, IDC_EDIT_DRIVER_ID, m_omDriverID);
	DDX_Control(pDX, IDC_LSTC_HW_LIST, m_omHardwareList);
    DDX_Control(pDX, IDC_LSTC_SELECTED_HW_LIST, m_omSelectedHwList);
    DDX_Control(pDX, IDC_EDIT_FIRMWARE, m_omFirmware);
	DDX_Control(pDX,IDC_CAN_EDIT_BAUD_RATE, m_omEditBaudRate);
	DDX_Text(pDX, IDC_CAN_EDIT_BAUD_RATE, m_omStrEditBaudRate);
	DDX_Control(pDX, IDC_CANFD_EDIT_BAUD_RATE, m_omCANFDEditBaudRate);
	DDX_Text(pDX, IDC_CANFD_EDIT_BAUD_RATE, m_omCANFDStrEditBaudRate);
	DDX_Control(pDX, IDC_CHECK_CANFD, m_chkCANFD);
	DDX_Check(pDX, IDC_CHECK_CANFD, m_bCANFD);
}


BEGIN_MESSAGE_MAP(CHardwareListingCAN, CDialog)
    //{{AFX_MSG_MAP(CHardwareListingCAN)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSTC_HW_LIST, OnItemchangedHWList)
    ON_BN_CLICKED(IDC_BUT_SELECT, OnButtonSelect)
    ON_BN_CLICKED(IDC_BUT_REMOVE, OnButtonRemove)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSTC_SELECTED_HW_LIST, OnItemchangedLstcSelectedHwList)
    //}}AFX_MSG_MAP
    ON_NOTIFY(NM_CLICK, IDC_LSTC_HW_LIST, OnNMClickLstcHwList)
    ON_NOTIFY(NM_CLICK, IDC_LSTC_SELECTED_HW_LIST, OnNMClickLstcSelectedHwList)
	ON_EN_KILLFOCUS(IDC_CAN_EDIT_BAUD_RATE, CHardwareListingCAN::OnEnKillfocusCanEditBaudRate)
	ON_BN_CLICKED(IDC_BUTTON_ADVANCE, CHardwareListingCAN::OnBnClickedButtonAdvance)
	ON_NOTIFY(NM_DBLCLK, IDC_LSTC_HW_LIST, CHardwareListingCAN::OnNMDblclkLstcHwList)
	ON_NOTIFY(NM_DBLCLK, IDC_LSTC_SELECTED_HW_LIST, CHardwareListingCAN::OnNMDblclkLstcSelectedHwList)
	ON_BN_CLICKED(IDC_CHECK_CANFD, CHardwareListingCAN::OnBnClickedCheckCanfd)
	ON_EN_KILLFOCUS(IDC_CANFD_EDIT_BAUD_RATE, CHardwareListingCAN::OnEnKillfocusCanfdEditBaudRate)
END_MESSAGE_MAP()


/*******************************************************************************
 Function Name  : vSetHardwareList
 Input(s)       : hHwList - Hardware List
                  nSize   - Size of the Hardware Lsit
 Output         :  -
 Functionality  : This function will init hardware list and will find net name
                  associated with the hardware if any.
 Member of      : CHardwareListingCAN
 Author(s)      : Raja N
 Date Created   : 08.09.2004
 Modifications  : Raja N on 14.03.2005, Create list control to hold selected
                  hardware items
*******************************************************************************/
void CHardwareListingCAN::vSetHardwareList(INTERFACE_HW* /*psHwIntr*/, int nSize)
{
    //all the application related to UI shall be performed only if
    //BUSMASTER is not called from COM

    // Clear the lsit
    m_omHardwareList.DeleteAllItems();
    // Remove all columns
    m_omHardwareList.DeleteColumn(0);
    // Set the Image List
    m_omHardwareList.SetImageList(&m_omImageList, LVSIL_SMALL );
    // Set Image List for selected list
    m_omSelectedHwList.SetImageList(&m_omImageList, LVSIL_SMALL );

    // Insert First Columns
    // The style is Icon. So insert Empty column

    // Create selelected list columns
	m_omHardwareList.InsertColumn(  defHARDWARE_COL,
									_(defSTR_HARDWARE_COL_NAME) );
	m_omHardwareList.SetColumnWidth( defHARDWARE_COL,
										defSTR_LISTED_HARDWARE_COL_WIDTH );
    m_omSelectedHwList.InsertColumn( defHARDWARE_COL,
                                     _(defSTR_HARDWARE_COL_NAME) );
    m_omSelectedHwList.SetColumnWidth( defHARDWARE_COL,
                                       defSTR_HARDWARE_COL_WIDTH );
    m_omSelectedHwList.InsertColumn(defCHANNEL_COL,
                                     _(defSTR_CHANNEL_NAME) );
    m_omSelectedHwList.SetColumnWidth( defCHANNEL_COL,
                                       defSTR_CHANNEL_COL_WIDTH );

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
            omStrFormat.Format( getProtocolChannelNameFormat(index).c_str() );
            nImageIndex = defDISCONNECTED_IMAGE_INDEX;
            // Insert List Item
            m_omHardwareList.InsertItem( nItemCount, omStrFormat, nImageIndex);
            // Set the hardware list index as item data
            m_omHardwareList.SetItemData( nItemCount++, index );
        }
    }

    // Set the selection to the first row
    m_nSelectedItem = 0;
    m_omHardwareList.SetItemState( m_nSelectedItem,
                                   LVIS_SELECTED | LVIS_FOCUSED,
                                   LVIS_SELECTED | LVIS_FOCUSED);
}
std::string CHardwareListingCAN::getProtocolChannelNameFormat(int nIndex)
{
    std::string strFormat = "%d";
    switch (m_eBus)
    {
        case CAN:
            strFormat =  m_psHwInterface[nIndex].m_acDescription;
            break;
        default:
            strFormat = "%d";
            break;
    }
    return strFormat;
}



/*******************************************************************************
 Function Name  : OnInitDialog
 Input(s)       :  -
 Output         : BOOL - TRUE if the focus isn't modified. FALSE otherwise
 Functionality  : This function will be called by the framework before showing a
                  dialog. This will init GUI components of a dialog
 Member of      : CHardwareListingCAN
 Author(s)      : Raja N
 Date Created   : 08.09.2004
*******************************************************************************/
BOOL CHardwareListingCAN::OnInitDialog()
{
    CDialog::OnInitDialog();
    // Update Hardware List with network details
    vSetHardwareList(m_psHwInterface, m_nSize);

	CComboBox* pEditBaudRateCAN = (CComboBox*)GetDlgItem(IDC_CAN_EDIT_BAUD_RATE);
	CComboBox* pEditBaudRateCANFD = (CComboBox*)GetDlgItem(IDC_CANFD_EDIT_BAUD_RATE);
	CWnd* pChkCANFD = (CWnd*)GetDlgItem(IDC_CHECK_CANFD);
	if(nullptr != pChkCANFD)
	{
		pChkCANFD->EnableWindow(FALSE);
	}
	if(nullptr!=pEditBaudRateCAN)
	{
		pEditBaudRateCAN ->EnableWindow(FALSE);
	}
	if(nullptr!=pEditBaudRateCANFD)
	{
		pEditBaudRateCANFD ->EnableWindow(FALSE);
	}
	if (m_pControllerDetails != nullptr)
	{
		for (int nIndex = 0; nIndex < m_nMaxChannelAllowed; nIndex++)
		{
			m_asDummyControllerDetails[nIndex] = m_pControllerDetails[nIndex];
		}
	}
	
    //Update the previously selected channel list
    vSetSelectedList();
   	m_nLastSelection = 0;

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
  Member of       : CHardwareListingCAN
  Author(s)       : Raja N
  Date Created    : 08.09.2004
  Modifications   : Raja N on 14.03.2005, Modified to get item data for getting
                    actual index.
******************************************************************************/
void CHardwareListingCAN::OnItemchangedHWList(NMHDR* pNMHDR, LRESULT* pResult)
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
 Member of      : CHardwareListingCAN
 Author(s)      : Raja N
 Date Created   : 08.09.2004
*******************************************************************************/
void CHardwareListingCAN::vUpdateHwDetails(int nIndex)
{
    if(nIndex < m_nSize )
    {
        //Driver Id
        CString omStr;
        omStr.Format(_("%d"), m_psHwInterface[nIndex].m_dwIdInterface);
        m_omDriverID.SetWindowText(omStr);
        //Firmware
        m_omFirmware.SetWindowText(m_psHwInterface[nIndex].m_acDeviceName.c_str());
        //Description
       

    }
}

/*******************************************************************************
  Function Name  : OnButtonSelect
  Input(s)       : -
  Output         : -
  Functionality  : This function will add the selected hardware in to the
                   selected hardware list.
  Member of      : CHardwareListingCAN
  Author(s)      : Raja N
  Date Created   : 25.2.2005
  Modifications  :
*******************************************************************************/
void CHardwareListingCAN::vMoveItemToSelectedList()
{
	POSITION sPos = m_omHardwareList.GetFirstSelectedItemPosition();
    // Insert the selected item in to the selected list
	if( nullptr!=sPos )
	{
		int nSelected = m_omHardwareList.GetNextSelectedItem(sPos);
    int nItem = m_omSelectedHwList.GetItemCount();
    CString omStrChannel;//("");
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
		m_omSelectedHwList.InsertItem( nItem, omStrHardware, nImageIndex );
    // Set the Hardware Name
		m_omSelectedHwList.SetItemText( nItem, 1, omStrChannel);
    // Set the array index
    m_omSelectedHwList.SetItemData( nItem, nArrayIndex );

		InitializeControllerDetails(nItem);
    // Remove the item from the list
    m_omHardwareList.DeleteItem( nSelected );

    //Sort Hardware Items
    vSortHardwareItems();

    // Set the focus to the first item
		m_omHardwareList.SetItemState(nSelected,
                                   LVIS_SELECTED | LVIS_FOCUSED,
                                   LVIS_SELECTED | LVIS_FOCUSED );
		m_omSelectedHwList.SetItemState(nItem,
			LVIS_SELECTED | LVIS_FOCUSED,
			LVIS_SELECTED | LVIS_FOCUSED );
    // Update Button Status
    vEnableDisableButtons();
	}
	else
	{
		m_omHardwareList.SetItemState(0,
			LVIS_SELECTED | LVIS_FOCUSED,
			LVIS_SELECTED | LVIS_FOCUSED );
	}
}

void CHardwareListingCAN::OnButtonSelect()
{
	vMoveItemToSelectedList();
}

/*******************************************************************************
  Function Name  : OnButtonRemove
  Input(s)       : -
  Output         : -
  Functionality  : This function will remove the selected item from the selected
                   hardware list. This will also insert the same in to the
                   available list
  Member of      : CHardwareListingCAN
  Author(s)      : Raja N
  Date Created   : 21.2.2005
  Modifications  : Raja N on 17.03.2005
                   Added code to change channel ID of rest of items when a
                   channel is deleted from the selected channel list
*******************************************************************************/
void CHardwareListingCAN::vMoveItemFromSelectedList()
{
    // Get the selected item from the list
    POSITION sPos = m_omSelectedHwList.GetFirstSelectedItemPosition();
	if( nullptr!=sPos )
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
			m_omSelectedHwList.GetItemText( nSelectedItem, 0),
                                     nImageIndex );
        // Set the hardware list index as item data.
        m_omHardwareList.SetItemData( nItemCount, nArrayIndex );

		int nSelectedItemCount = m_omSelectedHwList.GetItemCount();
        //Sort Hardware Items
        vSortHardwareItems();
		if (m_pControllerDetails != nullptr)
		{
			for (int nIndex = nSelectedItem; nIndex<nSelectedItemCount - 1; nIndex++)
			{
				m_pControllerDetails[nIndex] = m_pControllerDetails[nIndex + 1];
			}
		}
		
		UpdateData(FALSE);
//Set the values over the UI
		if (m_pControllerDetails != nullptr)
		{
			m_omEditBaudRate.SetWindowText(m_pControllerDetails[nSelectedItem].m_omStrBaudrate.c_str());
			CString omCANFDDataBitRate;
			m_chkCANFD.EnableWindow(m_pControllerDetails[nSelectedItem].m_bSupportCANFD);
			m_chkCANFD.SetCheck(m_pControllerDetails[nSelectedItem].m_bcanFDEnabled);
			CButton* pChkCANFD = (CButton*)GetDlgItem(IDC_CHECK_CANFD);
			if (nullptr != pChkCANFD)
			{
				if (pChkCANFD->GetCheck() == BST_CHECKED)
				{
					omCANFDDataBitRate.Format("%u", m_pControllerDetails[nSelectedItem].m_unDataBitRate);
					m_omCANFDEditBaudRate.SetWindowText(omCANFDDataBitRate);
				}
			}
		}
		UpdateData(TRUE);
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
					1,
                                                omStrChannel );
            }
        }
		m_omSelectedHwList.SetItemState( nSelectedItem,
			LVIS_SELECTED | LVIS_FOCUSED,
			LVIS_SELECTED | LVIS_FOCUSED );
        // Update UI button status
        vEnableDisableButtons();
    }
	else
	{
		m_omSelectedHwList.SetItemState( 0,
			LVIS_SELECTED | LVIS_FOCUSED,
			LVIS_SELECTED | LVIS_FOCUSED );
	}
}
void CHardwareListingCAN::OnButtonRemove()
{
	vMoveItemFromSelectedList(); 
}

/*******************************************************************************
  Function Name  : OnOK
  Input(s)       : -
  Output         : -
  Functionality  : This function will be called when the user selects OK button
                   or on press of Enter key. This will assign channel with
                   hardware handle as per user selection.
  Member of      : CHardwareListingCAN
  Author(s)      : Raja N
  Date Created   : 21.2.2005
  Modifications  :
*******************************************************************************/
void CHardwareListingCAN::OnOK()
{
    // Check the number of selections done by the user
	if (nFillChannelDetails() == S_OK)
    {
	//Checking for the Baud Rate entered either for CAN or CANFD
		GetDlgItem(IDC_CAN_EDIT_BAUD_RATE)->GetWindowText(m_omStrEditBaudRate);
		GetDlgItem(IDC_CANFD_EDIT_BAUD_RATE)->GetWindowText(m_omCANFDStrEditBaudRate);

		for (int nIndex=0; nIndex < m_nNoOfHwSelected; nIndex++ )
    	{
			if(nullptr!=m_pControllerDetails)

        	{
				CString omDummy = m_pControllerDetails[ nIndex ].m_omStrBaudrate.c_str();
				if(omDummy.IsEmpty() || m_omStrEditBaudRate.IsEmpty()) //&& m_omCanfdStrEditBaudRate.IsEmpty()) //Checking whether the Combo box is empty
 
       			 {
					AfxMessageBox("Please enter the Baud Rate");
					return;
        		 }
    		}

        }
		vValidateBaudRate();
		vUpdateControllerDetails();
    }
    
	    else
    {
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
  Member of      : CHardwareListingCAN
  Author(s)      : Raja N
  Date Created   : 21.2.2005
  Modifications  : Raja N on 22.03.2005
                   Added check to find the minimum value of available hardware
                   Vs channels supported
*******************************************************************************/
void CHardwareListingCAN::OnCancel()
{
     m_bDialogCancel = TRUE; // to say that Cancel is clicked
    // Show alert message telling that default association will be
    // used.
    //AfxMessageBox( defSTR_NOT_FULLY_CONFIGURED );
    // Assign Default Association
    // only for supported or available channels
	 for (int nIndex = 0; (UINT)nIndex < MIN((UINT)m_nSize, CHANNEL_ALLOWED); nIndex++)
	 {
		 m_anSelectedChannels[nIndex] = nIndex;
		 if (m_pControllerDetails != nullptr)
		 {
			 m_pControllerDetails[nIndex] = m_asDummyControllerDetails[nIndex];
		 }
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
  Member of      : CHardwareListingCAN
  Author(s)      : Raja N
  Date Created   : 21.2.2005
  Modifications  : Pradeep Kadoor, 14.12.2010, Disabling of hardware details removed
*******************************************************************************/
void CHardwareListingCAN::vEnableDisableButtons()
{
    BOOL bSelectEnable = FALSE;
    BOOL bRemoveEnable = FALSE;

    // Enable Disable Select Button
    // Get the selected hardware count
    int nSelectedHardware = m_omSelectedHwList.GetItemCount();
    // Check for the minimum of supported channels or available channels
    if( (UINT)nSelectedHardware < MIN( (UINT)m_nSize, m_nMaxChannelAllowed ) )
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
    CWnd* pWnd = nullptr;
    // Select Button
    pWnd = GetDlgItem( IDC_BUT_SELECT );
    if( nullptr!=pWnd  )
    {
        pWnd->EnableWindow( bSelectEnable );
    }
    // Remove Button
    pWnd = GetDlgItem( IDC_BUT_REMOVE );
    if( nullptr!=pWnd  )
    {
        pWnd->EnableWindow( bRemoveEnable );
    }
    // Enable Hardware Details
    BOOL bHardwareDetailsEnable = TRUE;


    // Driver ID
    pWnd = GetDlgItem( IDC_EDIT_DRIVER_ID );
    if( nullptr!=pWnd )
    {
        pWnd->EnableWindow( bHardwareDetailsEnable );
        // If disabled then clear the text
        if( bHardwareDetailsEnable == FALSE )
        {
            pWnd->SetWindowText( "" );
        }
    }
    // Network Name
    
        // If disabled then clear the text
    // Firmware
    pWnd = GetDlgItem( IDC_EDIT_FIRMWARE );
    if( nullptr!=pWnd )
    {
        pWnd->EnableWindow( bHardwareDetailsEnable );
        // If disabled then clear the text
        if( bHardwareDetailsEnable == FALSE )
        {
            pWnd->SetWindowText( "" );
        }
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
  Member of       : CHardwareListingCAN
  Author(s)       : Raja N
  Date Created    : 08.09.2004
  Modifications   :
******************************************************************************/
void CHardwareListingCAN::OnItemchangedLstcSelectedHwList( NMHDR* pNMHDR,
        LRESULT* pResult)
{
    //NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CComboBox* pEditBaudRateCAN = (CComboBox*)GetDlgItem(IDC_CAN_EDIT_BAUD_RATE);
	CComboBox* pEditBaudRateCANFD = (CComboBox*)GetDlgItem(IDC_CANFD_EDIT_BAUD_RATE);
	if(nullptr!=pEditBaudRateCAN )
	{
		pEditBaudRateCAN ->EnableWindow(TRUE);
	}
    // Update UI Buttons
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    // If it is a selection change update the hardware details
    if(pNMListView->uNewState & LVIS_SELECTED)
    {
        // Get the selected Item index
        UINT nSelectedItem = pNMListView->iItem;
		m_nLastSelection = pNMListView->iItem;
        // Update selected Hw details
        vUpdateHwDetails( (INT)m_omSelectedHwList.GetItemData( nSelectedItem ) );
		std::string omSelectedItem = m_omSelectedHwList.GetItemText( nSelectedItem, 0);
		std::map<std::string, bool>::iterator itr = m_mapChannelToCANFDStatus.find(omSelectedItem);
		if(m_mapChannelToCANFDStatus.end() !=  itr)
		{
			m_chkCANFD.EnableWindow(itr->second);
		}
		vFillControllerConfigDetails();
    }
	else if( pNMListView->uChanged  == LVIF_STATE &&
		pNMListView->uOldState == LVIS_SELECTED )
	{
		UpdateData( TRUE );
		vValidateBaudRate();
		vUpdateControllerDetails();
	}
	if(m_omSelectedHwList.GetSelectedCount() == 0)
	{
		if(nullptr!=pEditBaudRateCAN)
		{
			pEditBaudRateCAN ->EnableWindow(FALSE);
		}
		if(nullptr!=pEditBaudRateCANFD)
		{
			pEditBaudRateCANFD ->EnableWindow(FALSE);
		}
		m_chkCANFD.SetCheck(BST_UNCHECKED);
		m_chkCANFD.EnableWindow(FALSE);
		m_omEditBaudRate.SetSel(0, -1);
		m_omEditBaudRate.Clear();
		m_omCANFDEditBaudRate.SetSel(0, -1);
		m_omCANFDEditBaudRate.Clear();
    }
    vEnableDisableButtons();
    *pResult = 0;
}

INT CHardwareListingCAN::nGetSelectedList(int* pnList)
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
* \brief         This function will set the selected channel list
* \param[in]     pnList, contains the list of selected channels
* \return        void
* \authors       Arunkumar Karri
* \date          13.12.2011 Created
*/
void CHardwareListingCAN::vSetSelectedList()
{
    // Insert the selected item in to the selected list
    int nItem;
    CString omStrChannel;//("");
    CString omStrHardware;
	bool bIsChannelSelected = false; //To check for the Selected List
    for ( int i = 0 ; i < m_nSize && m_pnSelList[i]!=-1 ; i++)
    {
        nItem = m_omSelectedHwList.GetItemCount();
        // Format channel information
        omStrChannel.Format( defSTR_CHANNEL_NAME_FORMAT,
                             defSTR_CHANNEL_NAME,
                             nItem + 1 );
        // Get the Hardware name
        omStrHardware.Format( getProtocolChannelNameFormat(m_pnSelList[i]).c_str() );
        int nImageIndex = defDISCONNECTED_IMAGE_INDEX;
        // Insert the new item in to the selected list
        m_omSelectedHwList.InsertItem( nItem, omStrHardware , nImageIndex );
        // Set the Hardware Name
        m_omSelectedHwList.SetItemText( nItem, 1, omStrChannel );
        // Set the array index
        m_omSelectedHwList.SetItemData( nItem, m_pnSelList[i] );
		bIsChannelSelected = true;
    }
    // Set the focus to the first item
	if (bIsChannelSelected)
	{
		m_omSelectedHwList.SetItemState( 0,
			LVIS_SELECTED | LVIS_FOCUSED,
			LVIS_SELECTED | LVIS_FOCUSED );
		CComboBox* pEditBaudRateCAN = (CComboBox*)GetDlgItem(IDC_CAN_EDIT_BAUD_RATE);
		CComboBox* pEditBaudRateCANFD = (CComboBox*)GetDlgItem(IDC_CANFD_EDIT_BAUD_RATE);
		if(nullptr!=pEditBaudRateCAN)
		{
			pEditBaudRateCAN ->EnableWindow(TRUE);
		}
		CButton* pChkCANFD = (CButton*)GetDlgItem(IDC_CHECK_CANFD);
		bool ulCANFDEnabled = 0;
		if (m_pControllerDetails != nullptr)
		{
			ulCANFDEnabled = m_pControllerDetails[0].m_bcanFDEnabled;
		}
		std::string omSelectedItem = m_omSelectedHwList.GetItemText( 0, 0);
		std::map<std::string, bool>::iterator itr = m_mapChannelToCANFDStatus.find(omSelectedItem);
		if(m_mapChannelToCANFDStatus.end() !=  itr)
		{
			m_chkCANFD.EnableWindow(itr->second);
		}
		if(nullptr != pChkCANFD || nullptr != pEditBaudRateCANFD)
		{
			if(m_chkCANFD.IsWindowEnabled() == TRUE) // If Channel Supports CANFD
			{
				pChkCANFD->EnableWindow(TRUE);
				if(ulCANFDEnabled == 1) //If User has Enabled CANFD Feature 
				{
					pChkCANFD->SetCheck(BST_CHECKED);
					pEditBaudRateCANFD->EnableWindow(TRUE);
				}
				else
				{
					pChkCANFD->SetCheck(BST_UNCHECKED);
				}
			}
			else
			{
				pChkCANFD->EnableWindow(FALSE);
			}
		}
	}
	else
	{
    m_omHardwareList.SetItemState( 0,
                                   LVIS_SELECTED | LVIS_FOCUSED,
                                   LVIS_SELECTED | LVIS_FOCUSED );
	}
}

void CHardwareListingCAN::OnNMClickLstcHwList(NMHDR* pNMHDR, LRESULT* pResult)
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

void CHardwareListingCAN::OnNMClickLstcSelectedHwList(NMHDR* pNMHDR, LRESULT* pResult)
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
	else
	{
		m_omSelectedHwList.SetItemState( m_nLastSelection,
			LVIS_SELECTED | LVIS_FOCUSED,
			LVIS_SELECTED | LVIS_FOCUSED);
	}
    vEnableDisableButtons();
    *pResult = 0;
}
void CHardwareListingCAN::vSortHardwareItems()
{
    // clear map data
    if(mHardwareListMap.size() > 0 )
    {
        mHardwareListMap.clear();
    }

    int nItemCount = m_omHardwareList.GetItemCount();
    for(int nIndex = 0; nIndex < nItemCount; nIndex++)
    {
        m_pouHardwareContainer = new HARDWARE_CONTAINER();
        m_pouHardwareContainer->m_omHardwareName  = m_omHardwareList.GetItemText(nIndex,0);

        // Get the array index
        m_pouHardwareContainer->m_omDriverId  = m_omHardwareList.GetItemData( nIndex );

        // Insert List Item
        mHardwareListMap.insert ( Int_Pair ( m_pouHardwareContainer->m_omDriverId , m_pouHardwareContainer ) );
    }

    m_omHardwareList.DeleteAllItems();
    int iCount = 0;
    for(int nIndex = 0; nIndex < m_nSize; nIndex++)
    {
        m_pIter = mHardwareListMap.find(nIndex);

        if(m_pIter != mHardwareListMap.end() )//Hardware found
        {
            PHARDWARE_CONTAINER pTempHardware = m_pIter->second;

            //insert List Item
            m_omHardwareList.InsertItem( iCount, pTempHardware->m_omHardwareName, 0);
            // Set the hardware list index as item data
            m_omHardwareList.SetItemData( iCount++, pTempHardware->m_omDriverId );

        }
    }

    for ( m_pIter = mHardwareListMap.begin( ) ; m_pIter != mHardwareListMap.end( ) ; m_pIter++ )
    {
        delete(m_pIter->second); //release Memory
        m_pIter->second = nullptr;
    }

    // clear map data
    mHardwareListMap.clear();
}

void CHardwareListingCAN::OnEnKillfocusCanEditBaudRate()
{
	OnKillFocusOfEditBox(&m_omEditBaudRate,m_omStrEditBaudRate,0); //Validation on Kill Focus
	CButton* pomButtonCANFD = (CButton*) GetDlgItem(IDC_CHECK_CANFD);
	CButton* pomButtonWnd = (CButton*) GetFocus();
	if(nullptr != pomButtonCANFD)
	{
		if(TRUE == pomButtonCANFD->IsWindowEnabled())
		{
			/*UINT unButton = pomButtonCANFD->GetState();*/
			if(nullptr != pomButtonWnd && nullptr != pomButtonCANFD && pomButtonCANFD == pomButtonWnd)
			{
				if(BST_CHECKED == m_chkCANFD.GetCheck())
				{
					m_chkCANFD.SetCheck(BST_UNCHECKED);
					m_omCANFDEditBaudRate.EnableWindow(FALSE);
				}
				else
				{
					m_chkCANFD.SetCheck(BST_CHECKED);
					m_omCANFDEditBaudRate.EnableWindow(TRUE);
				}
			}
		}
	}
}
void CHardwareListingCAN::vUpdateControllerDetails()
{ 
	if(nullptr!=m_pControllerDetails)
	{
		UpdateData(TRUE);
		CString omStrBaudRate;
		m_omEditBaudRate.GetWindowText(omStrBaudRate);
		m_pControllerDetails[m_nLastSelection].m_omStrBaudrate = omStrBaudRate;
		CButton* pChkCANFD = (CButton*)GetDlgItem(IDC_CHECK_CANFD);
		if(nullptr != pChkCANFD)
		{
			if(TRUE == m_chkCANFD.IsWindowEnabled()) // If Channel Supports CANFD
			{
				if(pChkCANFD->GetCheck() == BST_CHECKED)
				{
					CString omCANFDStrEditDataRate;
					m_omCANFDEditBaudRate.GetWindowText(omCANFDStrEditDataRate);
					UINT32 ulCANFDDataRate = strtoul(omCANFDStrEditDataRate,NULL,0);
					m_pControllerDetails[ m_nLastSelection ].m_unDataBitRate = ulCANFDDataRate;
					m_pControllerDetails[m_nLastSelection].m_bcanFDEnabled = TRUE;
					m_pControllerDetails[m_nLastSelection].m_bSupportCANFD = TRUE;
				}
				else
				{
					m_pControllerDetails[m_nLastSelection].m_bcanFDEnabled = FALSE;
				}
			}
			else
			{
				pChkCANFD->EnableWindow(FALSE);
				m_pControllerDetails[m_nLastSelection].m_bSupportCANFD = FALSE;
				m_pControllerDetails[m_nLastSelection].m_bcanFDEnabled = FALSE;
			}
		}
	}
}
void CHardwareListingCAN::vFillControllerConfigDetails()
{
	int nIndex = m_nLastSelection;
	if(nullptr!=m_pControllerDetails)
	{
		m_omStrEditBaudRate     = m_pControllerDetails[ nIndex ].m_omStrBaudrate.c_str();
    	unsigned long ulCANFDEnabled = m_pControllerDetails[nIndex].m_bcanFDEnabled;
			if(m_chkCANFD.IsWindowEnabled() == TRUE) // If Channel Supports CANFD
			{
				m_chkCANFD.EnableWindow(TRUE);
				if(ulCANFDEnabled == 1) //If User has Enabled CANFD Feature 
				{
					m_chkCANFD.SetCheck(BST_CHECKED);
					UINT32 unDataBitrate = m_pControllerDetails[ nIndex ].m_unDataBitRate;
					m_omCANFDStrEditBaudRate.Format("%u",unDataBitrate);
					m_omCANFDEditBaudRate.EnableWindow(TRUE);
					m_omCANFDEditBaudRate.SetWindowText(m_omCANFDStrEditBaudRate);
				}
				else
				{
					m_chkCANFD.SetCheck(BST_UNCHECKED);
					m_omCANFDEditBaudRate.EnableWindow(FALSE);
					m_omCANFDEditBaudRate.SetWindowText("");
				}
			}
			else
			{
				m_chkCANFD.SetCheck(BST_UNCHECKED);
				m_chkCANFD.EnableWindow(FALSE);
				m_omCANFDEditBaudRate.EnableWindow(FALSE);
				m_omCANFDEditBaudRate.SetWindowText("");
			}
	}
	GetDlgItem(IDC_CAN_EDIT_BAUD_RATE)->SetWindowText(m_omStrEditBaudRate);
	UpdateData(TRUE);
}
void CHardwareListingCAN::OnBnClickedButtonAdvance()
{
	if (m_pControllerDetails == nullptr)
	{
		return;
	}
	if(nFillChannelDetails() == S_OK)
	{
		for(int nIndex=0; nIndex<m_nNoOfHwSelected; nIndex++)
		{
			m_pControllerDetails[nIndex].m_omHardwareDesc = m_psHwInterface[m_anSelectedChannels[nIndex]].m_acDescription;
			m_pControllerDetails[nIndex].m_bSupportCANFD = m_pnCANDFDValue[m_anSelectedChannels[nIndex]];
		}
		POSITION sPos = m_omSelectedHwList.GetFirstSelectedItemPosition();
		UINT nSelectedHw = -1;
		// Insert the selected item in to the selected list
		if (nullptr != sPos)
		{
			nSelectedHw = m_omSelectedHwList.GetNextSelectedItem(sPos);
		}
		if(nSelectedHw == -1)//If no Item is set in selected List Control, set first Item 
		{
			m_omSelectedHwList.SetItemState( 0,
				LVIS_SELECTED | LVIS_FOCUSED,
				LVIS_SELECTED | LVIS_FOCUSED );
			nSelectedHw = 0;
		}
		vValidateBaudRate();
		vUpdateControllerDetails();
		if(nullptr!=m_pAdvChnlConfig)
		{
			m_pAdvChnlConfig->InvokeAdavancedSettings(m_pControllerDetails, m_nNoOfHwSelected, nSelectedHw);
		}
		
			int nCount = m_omSelectedHwList.GetSelectionMark();
			m_omEditBaudRate.SetWindowText(m_pControllerDetails[nCount].m_omStrBaudrate.c_str());
			CString omstrCANFDDataBitRate;
			CButton* pChkCANFD = (CButton*)GetDlgItem(IDC_CHECK_CANFD);
			if (nullptr != pChkCANFD)
			{
				if (pChkCANFD->GetCheck() == BST_CHECKED)
				{
					omstrCANFDDataBitRate.Format("%d", m_pControllerDetails[nCount].m_unDataBitRate);
					m_omCANFDEditBaudRate.SetWindowText(omstrCANFDDataBitRate);
				}
			}
		
	}
}
void CHardwareListingCAN::vValidateBaudRate()
{
	DOUBLE dBaudrate;
	CString omStrEditBaudRate = "";
	if(nullptr!=m_pAdvChnlConfig)
	{
		m_omEditBaudRate.GetWindowText(omStrEditBaudRate);
		dBaudrate=strtod(omStrEditBaudRate, nullptr);
		int nItemCount = m_omSelectedHwList.GetSelectionMark();
		DOUBLE dBaudRateStr=m_pAdvChnlConfig->vValidateBaudRate(dBaudrate,nItemCount,0); //This is the call to respective ChangeRegisters.cpp
		omStrEditBaudRate.Format(_T("%.0lf"),dBaudRateStr);
	}
	m_omEditBaudRate.SetWindowText(omStrEditBaudRate);
	m_omStrEditBaudRate = omStrEditBaudRate;
}
void CHardwareListingCAN::OnNMDblclkLstcHwList(NMHDR *pNMHDR, LRESULT *pResult)
{
	vMoveItemToSelectedList();
}
void CHardwareListingCAN::OnNMDblclkLstcSelectedHwList(NMHDR *pNMHDR, LRESULT *pResult)
{
	vMoveItemFromSelectedList();
}
int CHardwareListingCAN::nFillChannelDetails()
{
	m_nNoOfHwSelected = m_omSelectedHwList.GetItemCount();
	if (m_nNoOfHwSelected < 1)
	{
		AfxMessageBox(_("Please select atleast one hardware"));
		return S_FALSE;
	}
		// Number of hardware will be used
	int nNumberOfHwUsed = MIN( m_nSize, m_nMaxChannelAllowed );
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
		return S_FALSE;
	}
	return S_OK;
}
void CHardwareListingCAN::OnBnClickedCheckCanfd()
{
	CComboBox* pEditBaudRateCANFD = (CComboBox*)GetDlgItem(IDC_CANFD_EDIT_BAUD_RATE);
	std::string omSelectedItem = m_omSelectedHwList.GetItemText( m_nLastSelection, 0);
	std::map<std::string, bool>::iterator itr = m_mapChannelToCANFDStatus.find(omSelectedItem);
	if(m_mapChannelToCANFDStatus.end() !=  itr)
	{
		m_chkCANFD.EnableWindow(itr->second);
	}
	if(m_chkCANFD.IsWindowEnabled() == TRUE)
	{
		if(m_chkCANFD.GetCheck() == BST_CHECKED)
		{
			if(nullptr!=pEditBaudRateCANFD)
			{
				pEditBaudRateCANFD ->EnableWindow(TRUE);
				UINT32 unDataBitRate  = 2000000; //Set it to Default DataBitRate
				m_omCANFDStrEditBaudRate.Format("%u",unDataBitRate);
				m_omCANFDEditBaudRate.SetWindowText(m_omCANFDStrEditBaudRate);
				m_pControllerDetails[m_nLastSelection].m_bcanFDEnabled = TRUE;
			}
		}
		else
		{
			if(nullptr!=pEditBaudRateCANFD)
			{
				pEditBaudRateCANFD ->EnableWindow(FALSE);
				m_omCANFDEditBaudRate.SetWindowText("");
			}
			m_pControllerDetails[m_nLastSelection].m_bcanFDEnabled = FALSE;
		}
	}
}
void CHardwareListingCAN::OnEnKillfocusCanfdEditBaudRate()
{
	OnKillFocusOfEditBox(&m_omCANFDEditBaudRate,m_omCANFDStrEditBaudRate ,1); //Validation on Kill Focus 
	CButton* pomButtonCANFD = (CButton*) GetDlgItem(IDC_CHECK_CANFD);
	if(pomButtonCANFD != nullptr)
	{
		UINT unButton = pomButtonCANFD->GetState();
		if(1 != unButton )
		{
			m_chkCANFD.SetCheck(BST_UNCHECKED);
			m_omCANFDEditBaudRate.EnableWindow(FALSE);
		}
	}
}
void CHardwareListingCAN::OnKillFocusOfEditBox(CEdit* omEditBaudRate, CString omStrEditBaudRate,bool bchkCANFD)
{
	CString omStrBaudRate   ="";
	CString omStrValid      ="";
	INT     nLength         = 0;
	omEditBaudRate->GetWindowText(omStrBaudRate);
	nLength             = omStrBaudRate.GetLength();
	CButton* pomButtonCancel = (CButton*) GetDlgItem(IDCANCEL);
	// To get the state of CANCEL button. A non zero value if the button is
	// clicked.
	UINT unButtonState;
	if(nullptr!=pomButtonCancel)
	{
		unButtonState       = pomButtonCancel->GetState();
	}
	// Validate only if next command is not ESC Button
	if(m_bDialogCancel != TRUE  )
	{
			// Don't validate if CANCEL button is clicked.
			if(unButtonState ==0 )
			{
				// Validate for empty string and if zero value is entered.
				DOUBLE dBaudRate = (FLOAT) _tstof(omStrBaudRate);
				if(bchkCANFD == 0)
				{
					ValidationOfKillFocus(omEditBaudRate, omStrEditBaudRate, bchkCANFD,1000000);
				}
				else if(bchkCANFD == 1)
				{
					ValidationOfKillFocus(omEditBaudRate, omStrEditBaudRate, bchkCANFD,8000000);
				}
			}
	}
	else
	{
		omEditBaudRate->SetWindowText(m_omStrEditBaudRate);
	}
}
void CHardwareListingCAN::ValidationOfKillFocus( CEdit* omEditBaudRate, CString omStrEditBaudRate, bool bchkCANFD, INT nMaxBitRateAllowed )
{
	CString omStrBaudRate;
	omEditBaudRate->GetWindowText(omStrBaudRate);
	DOUBLE dBaudRate = (FLOAT) _tstof(omStrBaudRate);
	INT nLength             = omStrBaudRate.GetLength();
	if(nLength == 0 || dBaudRate <= 0 || dBaudRate > nMaxBitRateAllowed)
	{
		omEditBaudRate->SetWindowText(omStrEditBaudRate);
		CString omStr;
		omStr.Format("Not a valid Decimal Numerals or out of range. !!! \n\nEnter only positive Decimal Numerals upto %dkbps.",nMaxBitRateAllowed/1000);
		AfxMessageBox(omStr);
		omEditBaudRate->SetFocus();
		omEditBaudRate->SetSel(0, -1,FALSE);
	}
	else
	{
		m_dEditBaudRate     = (FLOAT) _tstof(omStrEditBaudRate);
		// Call if string is valid to validate the baud rate value and
		// suggest  a next valid baud rate
		//Validate only if previous value in edit control is not the
		//  same as the one changed by user
		if(m_dEditBaudRate != dBaudRate && dBaudRate>0
			&& m_dEditBaudRate > 0  )
		{
			if(bchkCANFD == 0) //Validation for CAN BaudRate
			{
				vValidateBaudRate();
			}
			// Update List items only it is from edit box
			//vChangeListBoxValues(-1);
			CButton* pomButtonoK = (CButton*)GetDlgItem(IDOK);
			CButton* pomFocusWnd     = (CButton*)GetFocus();
			if(nullptr!=pomButtonoK && nullptr!=pomFocusWnd)
			{
				if (pomButtonoK ==pomFocusWnd)
				{
					OnOK();
				}
			}
			CButton* pomButtonAdvance = (CButton*) GetDlgItem(IDC_BUTTON_ADVANCE);
			CButton* pomFocusWndAdvance     = (CButton*)GetFocus();
			if(nullptr!=pomButtonAdvance && nullptr!=pomFocusWndAdvance)
			{
				if (pomButtonAdvance ==pomFocusWndAdvance)
				{

					// Open the Configuration dialog if the user
					// has pressed Advanced button
					OnBnClickedButtonAdvance();
				}
			}
		}
	}
}
void CHardwareListingCAN::InitializeControllerDetails(int nItem)
{
	if (m_pControllerDetails == nullptr)
	{
		return;
	}
	m_pControllerDetails[nItem].m_nItemUnderFocus = 64;
	m_pControllerDetails[nItem].m_nBTR0BTR1 = 49210;
	m_pControllerDetails[nItem].m_omStrCNF1 = "7";
	m_pControllerDetails[nItem].m_omStrCNF2 = "B8";
	m_pControllerDetails[nItem].m_omStrCNF3 = "5";
	m_pControllerDetails[nItem].m_omStrBTR0 = "C0";
	m_pControllerDetails[nItem].m_omStrBTR1 = "3A";
	m_pControllerDetails[nItem].m_omStrBaudrate = "500000";
	m_pControllerDetails[nItem].m_omStrClock = "16";
	m_pControllerDetails[nItem].m_omStrSamplePercentage = "75";
	m_pControllerDetails[nItem].m_omStrSampling = "1";
	m_pControllerDetails[nItem].m_omStrWarningLimit = "96";
	m_pControllerDetails[nItem].m_omStrPropagationDelay = "ALL";
	m_pControllerDetails[nItem].m_omStrSjw = "4";
	m_pControllerDetails[nItem].m_omStrAccCodeByte1[0] = "0";
	m_pControllerDetails[nItem].m_omStrAccCodeByte2[0] = "0";
	m_pControllerDetails[nItem].m_omStrAccCodeByte3[0] = "0";
	m_pControllerDetails[nItem].m_omStrAccCodeByte4[0] = "0";
	m_pControllerDetails[nItem].m_omStrAccCodeByte1[1] = "0";
	m_pControllerDetails[nItem].m_omStrAccCodeByte2[1] = "0";
	m_pControllerDetails[nItem].m_omStrAccCodeByte3[1] = "0";
	m_pControllerDetails[nItem].m_omStrAccCodeByte4[1] = "0";
	m_pControllerDetails[nItem].m_omStrAccMaskByte1[0] = "FF";
	m_pControllerDetails[nItem].m_omStrAccMaskByte2[0] = "FF";
	m_pControllerDetails[nItem].m_omStrAccMaskByte3[0] = "FF";
	m_pControllerDetails[nItem].m_omStrAccMaskByte4[0] = "FF";
	m_pControllerDetails[nItem].m_omStrAccMaskByte1[1] = "FF";
	m_pControllerDetails[nItem].m_omStrAccMaskByte2[1] = "FF";
	m_pControllerDetails[nItem].m_omStrAccMaskByte3[1] = "FF";
	m_pControllerDetails[nItem].m_omStrAccMaskByte4[1] = "FF";
	m_pControllerDetails[nItem].m_bAccFilterMode = FALSE;
	m_pControllerDetails[nItem].m_ucControllerMode = 0x1;
	m_pControllerDetails[nItem].m_enmHWFilterType[0] = HW_FILTER_ACCEPT_ALL;
	m_pControllerDetails[nItem].m_enmHWFilterType[1] = HW_FILTER_ACCEPT_ALL;
	m_pControllerDetails[nItem].m_bSelfReception = TRUE;
	m_pControllerDetails[nItem].m_bLowSpeed = FALSE;
	m_pControllerDetails[nItem].m_unBTL_Cycles = 10;
	m_pControllerDetails[nItem].m_bcanFDEnabled                 = FALSE;
	m_pControllerDetails[nItem].m_unDataBitRate                  = 2000000;
	m_pControllerDetails[nItem].m_unDataSamplePoint              = 70;
	m_pControllerDetails[nItem].m_unDataBTL_Cycles               = 10;
	m_pControllerDetails[nItem].m_unDataSJW                      = 03;
	m_pControllerDetails[nItem].m_bTxDelayCompensationControl    = 0;   // OCI_CANFD_TX_DELAY_COMPENSATION_OFF
	m_pControllerDetails[nItem].m_unTxSecondarySamplePointOffset = 0;
	m_pControllerDetails[nItem].m_bytCanRxMode                   = 2;   // OCI_CAN_RXMODE_CAN_FRAMES_USING_CAN_MESSAGE
	m_pControllerDetails[nItem].m_bytCanFdRxMode                 = 4;   // OCI_CANFDRXMODE_CANFD_FRAMES_USING_CANFD_MESSAGE
	m_pControllerDetails[nItem].m_bytCanFdTxConfig               = 4;   //  OCI_CANFDTX_USE_CAN_AND_CANFD_FRAMES
}
