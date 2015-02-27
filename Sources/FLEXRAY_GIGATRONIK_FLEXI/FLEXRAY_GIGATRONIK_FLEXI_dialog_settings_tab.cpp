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

/**********************************************************************************************************************
*
*  FILENAME:           TAB_TRANSMISSION_MODE.cpp
*
*  COPYRIGHT:          Gigatronik Ingolstadt GmbH
*  AUTHOR, DPT:        M. Morgott, KE
*
*  DATE OF CREATION:   09.12.2014
*
*  DESCRIPTION:        Implements tab controller for flexi settings dialog.
*
*********************************************************************************************************************/

/** --- CHANGE LOG ---
*  V1.0.0.0        First reviewed version
*
*/



#include "FLEXRAY_GIGATRONIK_FLEXI.h"
#include "FLEXRAY_GIGATRONIK_FLEXI_extern.h"
#include "FLEXRAY_GIGATRONIK_FLEXI_dialog_settings_tab.h"
#include "FLEXRAY_GIGATRONIK_FLEXI_configuration.h"



/*---- add here macro entries for message-handler functions ----*/
BEGIN_MESSAGE_MAP(CFLEXRAY_GIGATRONIK_FLEXI_dialog_settings_tab, CDialog)
    ON_NOTIFY(TCN_SELCHANGE, IDC_TABCONTROL, OnTcnSelchangeTabcontrol)
    ON_BN_CLICKED(IDC_FLEXI_UPDATE, OnUpdate)
END_MESSAGE_MAP()


/**
* ctor, calls base class ctor
*/
CFLEXRAY_GIGATRONIK_FLEXI_dialog_settings_tab::CFLEXRAY_GIGATRONIK_FLEXI_dialog_settings_tab(CWnd* pParent)
    : CDialog(CFLEXRAY_GIGATRONIK_FLEXI_dialog_settings_tab::IDD, pParent)
{
    INITIALISE_DATA(m_flexi_Settings);
}

/**
* dector, nothing to do
*/
CFLEXRAY_GIGATRONIK_FLEXI_dialog_settings_tab::~CFLEXRAY_GIGATRONIK_FLEXI_dialog_settings_tab()
{

}


/**
* This function will generally be called to update the data.
* In this case it sets a relation between the constants of the
* graphical elements and the element's instances.
*/
void CFLEXRAY_GIGATRONIK_FLEXI_dialog_settings_tab::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_FLEXI_UPDATE, button_update);
    DDX_Control(pDX, IDC_TABCONTROL, m_tabcontrol);
}


/**
* This function initializes the Dialog and its elements
*/
BOOL CFLEXRAY_GIGATRONIK_FLEXI_dialog_settings_tab::OnInitDialog()
{
    CDialog::OnInitDialog();

    CRect rcItem;
    TCITEM item1;

    /* init graphic elements with default settings */
    /* disable update button till the update gets enabled with readyToUpdate() */
    button_update.EnableWindow(FALSE);


    /* create tab of the acceptance mask options and assign it to the corresponding tab */
    m_tab_acceptance.Create(IDD_TAB_ACCEPTANCE, &m_tabcontrol);
    item1.mask = TCIF_TEXT | TCIF_PARAM;
    item1.lParam = (LPARAM)& m_tab_acceptance;
    item1.pszText = _T("Acceptance Mask");
    m_tabcontrol.InsertItem(0, &item1);

    /* positioning dialog */
    m_tabcontrol.GetItemRect(0, &rcItem);
    m_tab_acceptance.SetWindowPos(NULL, rcItem.left, rcItem.bottom + 1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    /* show first Tab */
    m_tab_acceptance.ShowWindow(SW_SHOW);
    m_tabcontrol.SetCurSel(0);


    /* create tab of the message view options and assign it to the corresponding tab */
    m_tab_message_view.Create(IDD_TAB_MESSAGE_VIEW, &m_tabcontrol);
    item1.mask = TCIF_TEXT | TCIF_PARAM;
    item1.lParam = (LPARAM)& m_tab_message_view;
    item1.pszText = _T("Message View Options");
    m_tabcontrol.InsertItem(1, &item1);

    /* positioning dialog */
    m_tab_message_view.SetWindowPos(NULL, rcItem.left, rcItem.bottom + 1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    /* hide tab */
    m_tab_message_view.ShowWindow(SW_HIDE);


    /* create tab of the transmission mode options and assign it to the corresponding tab */
    m_tab_transmission.Create(IDD_TAB_TRANSMISSION, &m_tabcontrol);
    item1.mask = TCIF_TEXT | TCIF_PARAM;
    item1.lParam = (LPARAM)& m_tab_transmission;
    item1.pszText = _T("Transmission Mode");
    m_tabcontrol.InsertItem(2, &item1);

    /* positioning dialog */
    m_tab_transmission.SetWindowPos(NULL, rcItem.left, rcItem.bottom + 1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    /* hide tab */
    m_tab_transmission.ShowWindow(SW_HIDE);

    return TRUE;
}


/**
* This function will be called if the tab selection gets changed.
* Examine the selected tab and show the corresponding content.
*/
void CFLEXRAY_GIGATRONIK_FLEXI_dialog_settings_tab::OnTcnSelchangeTabcontrol(NMHDR* pNMHDR, LRESULT* pResult)
{
    /* get current selected tab to switch the right content*/
    uint16 i = (uint16) m_tabcontrol.GetCurSel();

    m_tab_acceptance.ShowWindow(SW_HIDE);
    m_tab_message_view.ShowWindow(SW_HIDE);
    m_tab_transmission.ShowWindow(SW_HIDE);
    switch (i)
    {
        case 0:
        {
            m_tab_acceptance.ShowWindow(SW_SHOW);
            break;
        }
        case 1:
        {
            m_tab_message_view.ShowWindow(SW_SHOW);
            break;
        }
        case 2:
        {
            m_tab_transmission.ShowWindow(SW_SHOW);
            break;
        }
        default:
        {
            break;
        }
    }
}



/**
* This function will be called if the update button gets pushed.
* At first the function gets the current configuration, verifies it
* and calls the corresponding callback function.
*/
void CFLEXRAY_GIGATRONIK_FLEXI_dialog_settings_tab::OnUpdate()
{
    /* define variables */
    flexi_Settings new_Settings;

    /* get current settings */
    new_Settings = getSettings();

    /* check consistency */
    if (verifyFifoFilter(new_Settings) == true)
    {
        /* call callback to update new settings */
        g_pouDIL_FLEXRAY_GIGATRONIK_FLEXI->callback_Update_flexi_Settings(new_Settings);
    }
}



/**
* This function enables the update button if the
* application is ready.
*/
void CFLEXRAY_GIGATRONIK_FLEXI_dialog_settings_tab::readyToUpdate()
{
    /* after getting default settings, enable Update button */
    button_update.EnableWindow(TRUE);
}




/**
* This function reads the current settings, stores it in the proper
* data structure and returns it.
*/
flexi_Settings CFLEXRAY_GIGATRONIK_FLEXI_dialog_settings_tab::getSettings()
{
    /* define variables */
    CString string_FrameID;
    unsigned long ulong_FramdID;

    /* activation of null frames */
    if (m_tab_message_view.button_show_null_frames.GetCheck() == BST_CHECKED)
    {
        m_flexi_Settings.showNullFrames = TRUE;
    }
    else
    {
        m_flexi_Settings.showNullFrames = FALSE;
    }

    /* activation of startup/sync frames */
    if (m_tab_message_view.button_show_startup_sync_frames.GetCheck() == BST_CHECKED)
    {
        m_flexi_Settings.showSyncStartupFrames = TRUE;
    }
    else
    {
        m_flexi_Settings.showSyncStartupFrames = FALSE;
    }

    /* transmission mode dynamic segment */
    if (m_tab_transmission.button_continuous_dyn.GetCheck() == BST_CHECKED)
    {
        m_flexi_Settings.transmissionMode_dynSeg = GTFR_TX_MSGBUF_CONTINOUS;
    }
    else
    {
        m_flexi_Settings.transmissionMode_dynSeg = GTFR_TX_MSGBUF_SINGLE_SHOT;
    }

    /* transmission mode static segment */
    if (m_tab_transmission.button_continuous_static.GetCheck() == BST_CHECKED)
    {
        m_flexi_Settings.transmissionMode_staticSeg = GTFR_TX_MSGBUF_CONTINOUS;
    }
    else
    {
        m_flexi_Settings.transmissionMode_staticSeg = GTFR_TX_MSGBUF_SINGLE_SHOT;
    }

    /* FIFO Filter 0 */
    if (m_tab_acceptance.button_check_filter0.GetCheck() == BST_CHECKED)
    {
        m_flexi_Settings.FifoFilterActive[0] = TRUE;

        m_flexi_Settings.FifoFilter[0].ctrlIdx = 0;
        m_flexi_Settings.FifoFilter[0].filterId = 0;
        m_flexi_Settings.FifoFilter[0].isActive = true;

        /* transmission mode static segment */
        if (m_tab_acceptance.button_filter0_accept.GetCheck() == BST_CHECKED)
        {
            m_flexi_Settings.FifoFilter[0].filterTag = FIFO_FID_ACCEPTANCE_FILTER;
        }
        else
        {
            m_flexi_Settings.FifoFilter[0].filterTag = FIFO_FID_REJECTION_FILTER;
        }

        /* get content of CEdit and store it in proper data structure */
        m_tab_acceptance.edit_filter0_lower.GetWindowText(string_FrameID);;
        ulong_FramdID = strtoul(string_FrameID, nullptr, 10);
        m_flexi_Settings.FifoFilter[0].lowerFrameId = (uint16)ulong_FramdID;

        m_tab_acceptance.edit_filter0_upper.GetWindowText(string_FrameID);;
        ulong_FramdID = strtoul(string_FrameID, nullptr, 10);
        m_flexi_Settings.FifoFilter[0].upperFrameId = (uint16)ulong_FramdID;
    }
    else
    {
        m_flexi_Settings.FifoFilterActive[0] = FALSE;
    }



    /* FIFO Filter 1 */
    if (m_tab_acceptance.button_check_filter1.GetCheck() == BST_CHECKED)
    {
        m_flexi_Settings.FifoFilterActive[1] = TRUE;

        m_flexi_Settings.FifoFilter[1].ctrlIdx = 0;
        m_flexi_Settings.FifoFilter[1].filterId = 1;
        m_flexi_Settings.FifoFilter[1].isActive = true;

        /* transmission mode static segment */
        if (m_tab_acceptance.button_filter1_accept.GetCheck() == BST_CHECKED)
        {
            m_flexi_Settings.FifoFilter[1].filterTag = FIFO_FID_ACCEPTANCE_FILTER;
        }
        else
        {
            m_flexi_Settings.FifoFilter[1].filterTag = FIFO_FID_REJECTION_FILTER;
        }

        /* get content of CEdit and store it in proper data structure */
        m_tab_acceptance.edit_filter1_lower.GetWindowText(string_FrameID);;
        ulong_FramdID = strtoul(string_FrameID, nullptr, 10);
        m_flexi_Settings.FifoFilter[1].lowerFrameId = (uint16)ulong_FramdID;

        m_tab_acceptance.edit_filter1_upper.GetWindowText(string_FrameID);;
        ulong_FramdID = strtoul(string_FrameID, nullptr, 10);
        m_flexi_Settings.FifoFilter[1].upperFrameId = (uint16)ulong_FramdID;
    }
    else
    {
        m_flexi_Settings.FifoFilterActive[1] = FALSE;
    }


    /* FIFO Filter 2 */
    if (m_tab_acceptance.button_check_filter2.GetCheck() == BST_CHECKED)
    {
        m_flexi_Settings.FifoFilterActive[2] = TRUE;

        m_flexi_Settings.FifoFilter[2].ctrlIdx = 0;
        m_flexi_Settings.FifoFilter[2].filterId = 2;
        m_flexi_Settings.FifoFilter[2].isActive = true;

        /* transmission mode static segment */
        if (m_tab_acceptance.button_filter2_accept.GetCheck() == BST_CHECKED)
        {
            m_flexi_Settings.FifoFilter[2].filterTag = FIFO_FID_ACCEPTANCE_FILTER;
        }
        else
        {
            m_flexi_Settings.FifoFilter[2].filterTag = FIFO_FID_REJECTION_FILTER;
        }

        /* get content of CEdit and store it in proper data structure */
        m_tab_acceptance.edit_filter2_lower.GetWindowText(string_FrameID);;
        ulong_FramdID = strtoul(string_FrameID, nullptr, 10);
        m_flexi_Settings.FifoFilter[2].lowerFrameId = (uint16)ulong_FramdID;

        m_tab_acceptance.edit_filter2_upper.GetWindowText(string_FrameID);;
        ulong_FramdID = strtoul(string_FrameID, nullptr, 10);
        m_flexi_Settings.FifoFilter[2].upperFrameId = (uint16)ulong_FramdID;
    }
    else
    {
        m_flexi_Settings.FifoFilterActive[2] = FALSE;
    }



    /* FIFO Filter 3 */
    if (m_tab_acceptance.button_check_filter3.GetCheck() == BST_CHECKED)
    {
        m_flexi_Settings.FifoFilterActive[3] = TRUE;

        m_flexi_Settings.FifoFilter[3].ctrlIdx = 0;
        m_flexi_Settings.FifoFilter[3].filterId = 3;
        m_flexi_Settings.FifoFilter[3].isActive = true;

        /* transmission mode static segment */
        if (m_tab_acceptance.button_filter3_accept.GetCheck() == BST_CHECKED)
        {
            m_flexi_Settings.FifoFilter[3].filterTag = FIFO_FID_ACCEPTANCE_FILTER;
        }
        else
        {
            m_flexi_Settings.FifoFilter[3].filterTag = FIFO_FID_REJECTION_FILTER;
        }

        /* get content of CEdit and store it in proper data structure */
        m_tab_acceptance.edit_filter3_lower.GetWindowText(string_FrameID);;
        ulong_FramdID = strtoul(string_FrameID, nullptr, 10);
        m_flexi_Settings.FifoFilter[3].lowerFrameId = (uint16)ulong_FramdID;

        m_tab_acceptance.edit_filter3_upper.GetWindowText(string_FrameID);;
        ulong_FramdID = strtoul(string_FrameID, nullptr, 10);
        m_flexi_Settings.FifoFilter[3].upperFrameId = (uint16)ulong_FramdID;
    }
    else
    {
        m_flexi_Settings.FifoFilterActive[3] = FALSE;
    }


    return m_flexi_Settings;
}




/**
* This function verifies the transmitted FIFO filter settings.
* If there is a invalid configuration create a message box and
* return false. Otherwise return true if the configuration is valid.
*/
bool CFLEXRAY_GIGATRONIK_FLEXI_dialog_settings_tab::verifyFifoFilter(flexi_Settings new_Settings)
{
    /* check if lower and upper limit of the fifo filters contains valid content */
    if (new_Settings.FifoFilterActive[0] == true &&
            (!(new_Settings.FifoFilter[0].lowerFrameId > 0) ||
             !(new_Settings.FifoFilter[0].lowerFrameId <= new_Settings.FifoFilter[0].upperFrameId) ||
             !(new_Settings.FifoFilter[0].upperFrameId < GTFR_MAX_ALLOWED_FRAME_ID)))
    {
        MessageBox("The FIFO filter 0 contains invalid content!\nThe limit value has to be between 1 and 1023.", "Error FIFO filter 0", MB_OK);
    }
    else if (new_Settings.FifoFilterActive[1] == true &&
             (!(new_Settings.FifoFilter[1].lowerFrameId > 0) ||
              !(new_Settings.FifoFilter[1].lowerFrameId <= new_Settings.FifoFilter[1].upperFrameId) ||
              !(new_Settings.FifoFilter[1].upperFrameId < GTFR_MAX_ALLOWED_FRAME_ID)))
    {
        MessageBox("The FIFO filter 1 contains invalid content!\nThe limit value has to be between 1 and 1023.", "Error FIFO filter 1", MB_OK);
    }
    else if (new_Settings.FifoFilterActive[2] == true &&
             (!(new_Settings.FifoFilter[2].lowerFrameId > 0) ||
              !(new_Settings.FifoFilter[2].lowerFrameId <= new_Settings.FifoFilter[2].upperFrameId) ||
              !(new_Settings.FifoFilter[2].upperFrameId < GTFR_MAX_ALLOWED_FRAME_ID)))
    {
        MessageBox("The FIFO filter 2 contains invalid content!\nThe limit value has to be between 1 and 1023.", "Error FIFO filter 2", MB_OK);
    }
    else if (new_Settings.FifoFilterActive[3] == true &&
             (!(new_Settings.FifoFilter[3].lowerFrameId > 0) ||
              !(new_Settings.FifoFilter[3].lowerFrameId <= new_Settings.FifoFilter[3].upperFrameId) ||
              !(new_Settings.FifoFilter[3].upperFrameId < GTFR_MAX_ALLOWED_FRAME_ID)))
    {
        MessageBox("The FIFO filter 3 contains invalid content!\nThe limit value has to be between 1 and 1023.", "Error FIFO filter 3", MB_OK);
    }
    else
    {
        return true;
    }

    return false;
}


/* Define this derived method and do not call the method CDialog::OnCancel()
 * to prevent closing the dialog.
 */
void CFLEXRAY_GIGATRONIK_FLEXI_dialog_settings_tab::OnCancel()
{

}