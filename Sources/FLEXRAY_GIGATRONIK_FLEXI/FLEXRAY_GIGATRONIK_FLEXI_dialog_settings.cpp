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
 *  FILENAME:           FLEXRAY_GIGATRONIK_dialog_settings.cpp
 *
 *  COPYRIGHT:          Gigatronik Ingolstadt GmbH
 *  AUTHOR, DPT:        M. Morgott, KE
 *
 *  DATE OF CREATION:   28.11.2014
 *
 *  DESCRIPTION:        Implements methods of flex-i settings dialog.
 *
 *********************************************************************************************************************/

/** --- CHANGE LOG ---
 *
 *
 */

/* Project includes */
#include "FLEXRAY_GIGATRONIK_FLEXI_dialog_settings.h"
#include "FLEXRAY_GIGATRONIK_FLEXI.h"
#include "FLEXRAY_GIGATRONIK_FLEXI_extern.h"


#include "temp_debug.h"


/*---- add here macro entries for message-handler functions ----*/
BEGIN_MESSAGE_MAP(CflexiSettingsListing, CDialog)
    ON_BN_CLICKED(IDC_FLEXI_UPDATE, OnUpdate)
    ON_BN_CLICKED(IDC_FLEXI_CHECK_FIFO_0, OnButtonClicked_Filter0)
    ON_BN_CLICKED(IDC_FLEXI_CHECK_FIFO_1, OnButtonClicked_Filter1)
    ON_BN_CLICKED(IDC_FLEXI_CHECK_FIFO_2, OnButtonClicked_Filter2)
    ON_BN_CLICKED(IDC_FLEXI_CHECK_FIFO_3, OnButtonClicked_Filter3)
END_MESSAGE_MAP()


/**
 * ctor, calls base class ctor
 */
CflexiSettingsListing::CflexiSettingsListing() : CDialog(CflexiSettingsListing::IDD)
{
    INITIALISE_DATA(m_flexi_Settings);
}


/**
 * This function initialize the Dialog and its elements
 */
BOOL CflexiSettingsListing::OnInitDialog()
{

    CDialog::OnInitDialog();

    /* init graphic elements with default settings */
    button_update.EnableWindow(FALSE);

    button_check_filter1.EnableWindow(TRUE);
    button_check_filter2.EnableWindow(TRUE);
    button_check_filter3.EnableWindow(TRUE);

    /* just allow 4 digits */
    edit_filter0_lower.SetLimitText(4);
    edit_filter1_lower.SetLimitText(4);
    edit_filter2_lower.SetLimitText(4);
    edit_filter3_lower.SetLimitText(4);
    edit_filter0_upper.SetLimitText(4);
    edit_filter1_upper.SetLimitText(4);
    edit_filter2_upper.SetLimitText(4);
    edit_filter3_upper.SetLimitText(4);

    edit_filter0_lower.EnableWindow(FALSE);
    edit_filter0_upper.EnableWindow(FALSE);
    edit_filter1_lower.EnableWindow(FALSE);
    edit_filter1_upper.EnableWindow(FALSE);
    edit_filter2_lower.EnableWindow(FALSE);
    edit_filter2_upper.EnableWindow(FALSE);
    edit_filter3_lower.EnableWindow(FALSE);
    edit_filter3_upper.EnableWindow(FALSE);

    button_filter0_accept.EnableWindow(FALSE);
    button_filter0_reject.EnableWindow(FALSE);
    button_filter1_accept.EnableWindow(FALSE);
    button_filter1_reject.EnableWindow(FALSE);
    button_filter2_accept.EnableWindow(FALSE);
    button_filter2_reject.EnableWindow(FALSE);
    button_filter3_accept.EnableWindow(FALSE);
    button_filter3_reject.EnableWindow(FALSE);

    button_filter0_accept.SetCheck(BST_CHECKED);
    button_filter0_reject.SetCheck(BST_UNCHECKED);
    button_filter1_accept.SetCheck(BST_CHECKED);
    button_filter1_reject.SetCheck(BST_UNCHECKED);
    button_filter2_accept.SetCheck(BST_CHECKED);
    button_filter2_reject.SetCheck(BST_UNCHECKED);
    button_filter3_accept.SetCheck(BST_CHECKED);
    button_filter3_reject.SetCheck(BST_UNCHECKED);

    button_continuous_dyn.SetCheck(BST_CHECKED);
    button_continuous_static.SetCheck(BST_CHECKED);

    return TRUE;
}


/**
 * This function react, when the checkbox of the filter 0
 * get activated/deactivated. It enables/disables the
 * corresponding graphical elements
 */
void CflexiSettingsListing::OnButtonClicked_Filter0()
{

    /* check if filter 0 is selected to activate options
     * depending on it, the corresponding graphical elements will be enable or disabled
     */
    if(button_check_filter0.GetCheck() == BST_CHECKED)
    {
        edit_filter0_lower.EnableWindow(TRUE);
        edit_filter0_upper.EnableWindow(TRUE);
        button_filter0_accept.EnableWindow(TRUE);
        button_filter0_reject.EnableWindow(TRUE);
    }
    else
    {
        edit_filter0_lower.EnableWindow(FALSE);
        edit_filter0_upper.EnableWindow(FALSE);
        button_filter0_accept.EnableWindow(FALSE);
        button_filter0_reject.EnableWindow(FALSE);
    }
}

void CflexiSettingsListing::OnButtonClicked_Filter1()
{

    /* check if filter 1 is selected to activate options
     * depending on it, the corresponding graphical elements will be enable or disabled
     */
    if(button_check_filter1.GetCheck() == BST_CHECKED)
    {
        edit_filter1_lower.EnableWindow(TRUE);
        edit_filter1_upper.EnableWindow(TRUE);
        button_filter1_accept.EnableWindow(TRUE);
        button_filter1_reject.EnableWindow(TRUE);
    }
    else
    {
        edit_filter1_lower.EnableWindow(FALSE);
        edit_filter1_upper.EnableWindow(FALSE);
        button_filter1_accept.EnableWindow(FALSE);
        button_filter1_reject.EnableWindow(FALSE);
    }
}

void CflexiSettingsListing::OnButtonClicked_Filter2()
{

    /* check if filter 2 is selected to activate options
     * depending on it, the corresponding graphical elements will be enable or disabled
     */
    if(button_check_filter2.GetCheck() == BST_CHECKED)
    {
        edit_filter2_lower.EnableWindow(TRUE);
        edit_filter2_upper.EnableWindow(TRUE);
        button_filter2_accept.EnableWindow(TRUE);
        button_filter2_reject.EnableWindow(TRUE);
    }
    else
    {
        edit_filter2_lower.EnableWindow(FALSE);
        edit_filter2_upper.EnableWindow(FALSE);
        button_filter2_accept.EnableWindow(FALSE);
        button_filter2_reject.EnableWindow(FALSE);
    }
}

void CflexiSettingsListing::OnButtonClicked_Filter3()
{

    /* check if filter 3 is selected to activate options
     * depending on it, the corresponding graphical elements will be enable or disabled
     */
    if(button_check_filter3.GetCheck() == BST_CHECKED)
    {
        edit_filter3_lower.EnableWindow(TRUE);
        edit_filter3_upper.EnableWindow(TRUE);
        button_filter3_accept.EnableWindow(TRUE);
        button_filter3_reject.EnableWindow(TRUE);
    }
    else
    {
        edit_filter3_lower.EnableWindow(FALSE);
        edit_filter3_upper.EnableWindow(FALSE);
        button_filter3_accept.EnableWindow(FALSE);
        button_filter3_reject.EnableWindow(FALSE);
    }
}


/**
 * This function will be called if the update button gets pushed.
 * At first the function get the current configuration, verify it
 * and call the corresponding callback function.
 */
void CflexiSettingsListing::OnUpdate()
{
    /* define variables */
    flexi_Settings new_Settings;

    /* get current settings */
    new_Settings = getSettings();

    /* check consistency */
    if(verifyFifoFilter(new_Settings) == true)
    {
        /* call callback to update new settings */
        g_pouDIL_FLEXRAY_GIGATRONIK_FLEXI->callback_Update_flexi_Settings(new_Settings);
    }
}


/**
 * This function verify the transmitted FIFO filter settings.
 * If there is a invalid configuration create a message box and
 * return false. Otherwise return true if the configurations are valid.
 */
bool CflexiSettingsListing::verifyFifoFilter(flexi_Settings new_Settings)
{
    /* check if lower and upper limit of the fifo filters contains valid content */
    if(new_Settings.FifoFilterActive[0] == true &&
            (!(new_Settings.FifoFilter[0].lowerFrameId > 0) ||
             !(new_Settings.FifoFilter[0].lowerFrameId <= new_Settings.FifoFilter[0].upperFrameId) ||
             !(new_Settings.FifoFilter[0].upperFrameId < GTFR_MAX_ALLOWED_FRAME_ID) ))
    {
        MessageBox("The FIFO filter 0 contains invalid content!\nThe limit value has to be between 1 and 1023.", "Error FIFO filter 0", MB_OK);
    }
    else if(new_Settings.FifoFilterActive[1] == true &&
            (!(new_Settings.FifoFilter[1].lowerFrameId > 0) ||
             !(new_Settings.FifoFilter[1].lowerFrameId <= new_Settings.FifoFilter[1].upperFrameId) ||
             !(new_Settings.FifoFilter[1].upperFrameId < GTFR_MAX_ALLOWED_FRAME_ID) ))
    {
        MessageBox("The FIFO filter 1 contains invalid content!\nThe limit value has to be between 1 and 1023.", "Error FIFO filter 1", MB_OK);
    }
    else if(new_Settings.FifoFilterActive[2] == true &&
            (!(new_Settings.FifoFilter[2].lowerFrameId > 0) ||
             !(new_Settings.FifoFilter[2].lowerFrameId <= new_Settings.FifoFilter[2].upperFrameId) ||
             !(new_Settings.FifoFilter[2].upperFrameId < GTFR_MAX_ALLOWED_FRAME_ID) ))
    {
        MessageBox("The FIFO filter 2 contains invalid content!\nThe limit value has to be between 1 and 1023.", "Error FIFO filter 2", MB_OK);
    }
    else if(new_Settings.FifoFilterActive[3] == true &&
            (!(new_Settings.FifoFilter[3].lowerFrameId > 0) ||
             !(new_Settings.FifoFilter[3].lowerFrameId <= new_Settings.FifoFilter[3].upperFrameId) ||
             !(new_Settings.FifoFilter[3].upperFrameId < GTFR_MAX_ALLOWED_FRAME_ID) ))
    {
        MessageBox("The FIFO filter 3 contains invalid content!\nThe limit value has to be between 1 and 1023.", "Error FIFO filter 3", MB_OK);
    }
    else
    {
        return true;
    }

    return false;
}


/**
 * This function enables the update button if the
 * application is ready.
 */
void CflexiSettingsListing::readyToUpdate()
{
    /* after getting default settings, enable Update button: */
    button_update.EnableWindow(TRUE);
}


/**
 * This function reads the current settings, store it in the proper
 * data structure and return it.
 */
flexi_Settings CflexiSettingsListing::getSettings()
{
    /* define variables */
    CString string_FrameID;
    unsigned long ulong_FramdID;

    /* set flag if show null frames */
    if(button_show_null_frames.GetCheck() == BST_CHECKED)
    {
        m_flexi_Settings.showNullFrames = TRUE;
    }
    else
    {
        m_flexi_Settings.showNullFrames = FALSE;
    }

    /* set flag if show startup/sync frames */
    if(button_show_startup_sync_frames.GetCheck() == BST_CHECKED)
    {
        m_flexi_Settings.showSyncStartupFrames = TRUE;
    }
    else
    {
        m_flexi_Settings.showSyncStartupFrames = FALSE;
    }

    /* transmission mode dynamic segment */
    if(button_continuous_dyn.GetCheck() == BST_CHECKED)
    {
        m_flexi_Settings.transmissionMode_dynSeg = GTFR_TX_MSGBUF_CONTINOUS;
    }
    else
    {
        m_flexi_Settings.transmissionMode_dynSeg = GTFR_TX_MSGBUF_SINGLE_SHOT;
    }

    /* transmission mode static segment */
    if(button_continuous_static.GetCheck() == BST_CHECKED)
    {
        m_flexi_Settings.transmissionMode_staticSeg = GTFR_TX_MSGBUF_CONTINOUS;
    }
    else
    {
        m_flexi_Settings.transmissionMode_staticSeg = GTFR_TX_MSGBUF_SINGLE_SHOT;
    }

    /* FIFO Filter 0 */
    if(button_check_filter0.GetCheck() == BST_CHECKED)
    {
        m_flexi_Settings.FifoFilterActive[0] = TRUE;

        m_flexi_Settings.FifoFilter[0].ctrlIdx = 0;
        m_flexi_Settings.FifoFilter[0].filterId = 0;
        m_flexi_Settings.FifoFilter[0].isActive = true;

        /* transmission mode static segment */
        if(button_filter0_accept.GetCheck() == BST_CHECKED)
        {
            m_flexi_Settings.FifoFilter[0].filterTag = FIFO_FID_ACCEPTANCE_FILTER;
        }
        else
        {
            m_flexi_Settings.FifoFilter[0].filterTag = FIFO_FID_REJECTION_FILTER;
        }

        /* get content of CEdit and store it in proper data structure */
        edit_filter0_lower.GetWindowText(string_FrameID);;
        ulong_FramdID = strtoul(string_FrameID, nullptr, 10);
        m_flexi_Settings.FifoFilter[0].lowerFrameId = (uint16) ulong_FramdID;

        edit_filter0_upper.GetWindowText(string_FrameID);;
        ulong_FramdID = strtoul(string_FrameID, nullptr, 10);
        m_flexi_Settings.FifoFilter[0].upperFrameId = (uint16) ulong_FramdID;
    }
    else
    {
        m_flexi_Settings.FifoFilterActive[0] = FALSE;
    }



    /* FIFO Filter 1 */
    if(button_check_filter1.GetCheck() == BST_CHECKED)
    {
        m_flexi_Settings.FifoFilterActive[1] = TRUE;

        m_flexi_Settings.FifoFilter[1].ctrlIdx = 0;
        m_flexi_Settings.FifoFilter[1].filterId = 1;
        m_flexi_Settings.FifoFilter[1].isActive = true;

        /* transmission mode static segment */
        if(button_filter1_accept.GetCheck() == BST_CHECKED)
        {
            m_flexi_Settings.FifoFilter[1].filterTag = FIFO_FID_ACCEPTANCE_FILTER;
        }
        else
        {
            m_flexi_Settings.FifoFilter[1].filterTag = FIFO_FID_REJECTION_FILTER;
        }

        /* get content of CEdit and store it in proper data structure */
        edit_filter1_lower.GetWindowText(string_FrameID);;
        ulong_FramdID = strtoul(string_FrameID, nullptr, 10);
        m_flexi_Settings.FifoFilter[1].lowerFrameId = (uint16) ulong_FramdID;

        edit_filter1_upper.GetWindowText(string_FrameID);;
        ulong_FramdID = strtoul(string_FrameID, nullptr, 10);
        m_flexi_Settings.FifoFilter[1].upperFrameId = (uint16) ulong_FramdID;
    }
    else
    {
        m_flexi_Settings.FifoFilterActive[1] = FALSE;
    }


    /* FIFO Filter 2 */
    if(button_check_filter2.GetCheck() == BST_CHECKED)
    {
        m_flexi_Settings.FifoFilterActive[2] = TRUE;

        m_flexi_Settings.FifoFilter[2].ctrlIdx = 0;
        m_flexi_Settings.FifoFilter[2].filterId = 2;
        m_flexi_Settings.FifoFilter[2].isActive = true;

        /* transmission mode static segment */
        if(button_filter2_accept.GetCheck() == BST_CHECKED)
        {
            m_flexi_Settings.FifoFilter[2].filterTag = FIFO_FID_ACCEPTANCE_FILTER;
        }
        else
        {
            m_flexi_Settings.FifoFilter[2].filterTag = FIFO_FID_REJECTION_FILTER;
        }

        /* get content of CEdit and store it in proper data structure */
        edit_filter2_lower.GetWindowText(string_FrameID);;
        ulong_FramdID = strtoul(string_FrameID, nullptr, 10);
        m_flexi_Settings.FifoFilter[2].lowerFrameId = (uint16) ulong_FramdID;

        edit_filter2_upper.GetWindowText(string_FrameID);;
        ulong_FramdID = strtoul(string_FrameID, nullptr, 10);
        m_flexi_Settings.FifoFilter[2].upperFrameId = (uint16) ulong_FramdID;
    }
    else
    {
        m_flexi_Settings.FifoFilterActive[2] = FALSE;
    }



    /* FIFO Filter 3 */
    if(button_check_filter3.GetCheck() == BST_CHECKED)
    {
        m_flexi_Settings.FifoFilterActive[3] = TRUE;

        m_flexi_Settings.FifoFilter[3].ctrlIdx = 0;
        m_flexi_Settings.FifoFilter[3].filterId = 3;
        m_flexi_Settings.FifoFilter[3].isActive = true;

        /* transmission mode static segment */
        if(button_filter3_accept.GetCheck() == BST_CHECKED)
        {
            m_flexi_Settings.FifoFilter[3].filterTag = FIFO_FID_ACCEPTANCE_FILTER;
        }
        else
        {
            m_flexi_Settings.FifoFilter[3].filterTag = FIFO_FID_REJECTION_FILTER;
        }

        /* get content of CEdit and store it in proper data structure */
        edit_filter3_lower.GetWindowText(string_FrameID);;
        ulong_FramdID = strtoul(string_FrameID, nullptr, 10);
        m_flexi_Settings.FifoFilter[3].lowerFrameId = (uint16) ulong_FramdID;

        edit_filter3_upper.GetWindowText(string_FrameID);;
        ulong_FramdID = strtoul(string_FrameID, nullptr, 10);
        m_flexi_Settings.FifoFilter[3].upperFrameId = (uint16) ulong_FramdID;
    }
    else
    {
        m_flexi_Settings.FifoFilterActive[3] = FALSE;
    }


    return m_flexi_Settings;
}



/**
 * This function will generally be called to update the data.
 * In this case it set a relation between the constants of the
 * graphical elements and the element's instances.
 */
void CflexiSettingsListing::DoDataExchange(CDataExchange* pDX)
{

    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CHardwareListing)
    DDX_Control(pDX, IDC_FLEXI_UPDATE, button_update);
    DDX_Control(pDX, IDC_FLEXI_CONTINUOUS_STATIC, button_continuous_static);
    DDX_Control(pDX, IDC_FLEXI_SINGLE_STATIC, button_single_shot_static);
    DDX_Control(pDX, IDC_FLEXI_CONTINUOUS_DYN, button_continuous_dyn);
    DDX_Control(pDX, IDC_FLEXI_SINGLE_DYN, button_single_shot_dyn);
    DDX_Control(pDX, IDC_FLEXI_NULL, button_show_null_frames);
    DDX_Control(pDX, IDC_FLEXI_STARTUP, button_show_startup_sync_frames);
    DDX_Control(pDX, IDC_FLEXI_CHECK_FIFO_0, button_check_filter0);
    DDX_Control(pDX, IDC_FLEXI_CHECK_FIFO_1, button_check_filter1);
    DDX_Control(pDX, IDC_FLEXI_CHECK_FIFO_2, button_check_filter2);
    DDX_Control(pDX, IDC_FLEXI_CHECK_FIFO_3, button_check_filter3);
    DDX_Control(pDX, IDC_FLEXI_LOW_LIMIT_0, edit_filter0_lower);
    DDX_Control(pDX, IDC_FLEXI_UPPER_LIMIT_0, edit_filter0_upper);
    DDX_Control(pDX, IDC_FLEXI_LOW_LIMIT_1, edit_filter1_lower);
    DDX_Control(pDX, IDC_FLEXI_UPPER_LIMIT_1, edit_filter1_upper);
    DDX_Control(pDX, IDC_FLEXI_LOW_LIMIT_2, edit_filter2_lower);
    DDX_Control(pDX, IDC_FLEXI_UPPER_LIMIT_2, edit_filter2_upper);
    DDX_Control(pDX, IDC_FLEXI_LOW_LIMIT_3, edit_filter3_lower);
    DDX_Control(pDX, IDC_FLEXI_UPPER_LIMIT_3, edit_filter3_upper);
    DDX_Control(pDX, IDC_FLEXI_ACCEPT_0, button_filter0_accept);
    DDX_Control(pDX, IDC_FLEXI_REJECT_0, button_filter0_reject);
    DDX_Control(pDX, IDC_FLEXI_ACCEPT_1, button_filter1_accept);
    DDX_Control(pDX, IDC_FLEXI_REJECT_1, button_filter1_reject);
    DDX_Control(pDX, IDC_FLEXI_ACCEPT_2, button_filter2_accept);
    DDX_Control(pDX, IDC_FLEXI_REJECT_2, button_filter2_reject);
    DDX_Control(pDX, IDC_FLEXI_ACCEPT_3, button_filter3_accept);
    DDX_Control(pDX, IDC_FLEXI_REJECT_3, button_filter3_reject);
    //}}AFX_DATA_MAP
}


