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
 *  FILENAME:           FLEXRAY_GIGATRONIK_dialog_keyslot.cpp
 *
 *  COPYRIGHT:          Gigatronik Ingolstadt GmbH
 *  AUTHOR, DPT:        M. Morgott, KE
 *
 *  DATE OF CREATION:   28.11.2014
 *
 *  DESCRIPTION:        Implements methods of keyslot dialog.
 *
 *********************************************************************************************************************/

/** --- CHANGE LOG ---
 *  V1.0.0.0        First reviewed version
 *
 */

/* Project includes */
#include "FLEXRAY_GIGATRONIK_FLEXI_dialog_keyslot.h"
#include "FLEXRAY_GIGATRONIK_FLEXI_GtFr_NativeInterface.h"
#include "FLEXRAY_GIGATRONIK_FLEXI_configuration.h"

//#include "temp_debug.h"


/*---- add here macro entries for message-handler functions ----*/
BEGIN_MESSAGE_MAP(CflexiKeyslotListing, CDialog)
    ON_BN_CLICKED(IDOK, OnOK)
    ON_BN_CLICKED(IDCANCEL, OnCancel)
    ON_CBN_SELCHANGE(IDC_FLEXI_CC0_PREF, OnSelectionChanged_Combo_CC0)
    ON_CBN_SELCHANGE(IDC_FLEXI_CC1_PREF, OnSelectionChanged_Combo_CC1)
END_MESSAGE_MAP()


/**
 * ctor, calls base class ctor
 */
CflexiKeyslotListing::CflexiKeyslotListing() : CDialog(CflexiKeyslotListing::IDD)
{

}


/**
 * ctor, calls base class ctor; transmit pointer for keyslot settings
 */
CflexiKeyslotListing::CflexiKeyslotListing(flexi_Keyslot_Settings* p_flexi_Keyslot_Settings) : CDialog(CflexiKeyslotListing::IDD)
{
    /* store pointer of data structure to deliver selected settings to calling instance */
    m_flexi_Keyslot_Settings = p_flexi_Keyslot_Settings;
}


/**
 * This function initializes the Dialog and its elements
 */
BOOL CflexiKeyslotListing::OnInitDialog()
{
    CDialog::OnInitDialog();

    /* Init Comboboxes with content and default behaviour
     * The order of adding the content is important and the final evaluation of the comboboxes is adjusted with the current order!
     */
    comboBox_CC0_pref.AddString("Off");
    comboBox_CC0_pref.AddString("automatic");
    comboBox_CC0_pref.AddString("manual");
    comboBox_CC0_pref.SetCurSel(0);

    comboBox_CC1_pref.AddString("Off");
    comboBox_CC1_pref.AddString("automatic");
    comboBox_CC1_pref.AddString("manual");
    comboBox_CC1_pref.SetCurSel(0);
    comboBox_CC1_pref.EnableWindow(FALSE);

    comboBox_CC0_start_sync.AddString("sync node");
    comboBox_CC0_start_sync.AddString("startup node");
    comboBox_CC0_start_sync.SetCurSel(0);
    comboBox_CC0_start_sync.EnableWindow(FALSE);

    comboBox_CC1_start_sync.AddString("sync node");
    comboBox_CC1_start_sync.AddString("startup node");
    comboBox_CC1_start_sync.SetCurSel(0);
    comboBox_CC1_start_sync.EnableWindow(FALSE);

    /* config edit fields, only allow 4 digits */
    edit_CC0_keyslot.SetLimitText(4);
    edit_CC1_keyslot.SetLimitText(4);
    edit_CC0_keyslot.EnableWindow(FALSE);
    edit_CC1_keyslot.EnableWindow(FALSE);

    return TRUE;
}


/**
 * This function modifies the Dialog and its elements, if the keyslot setting selection of the CC0 changed
 */
void CflexiKeyslotListing::OnSelectionChanged_Combo_CC0()
{
    /* if 'off' mode selected: disable corresponding graphical elements */
    if(comboBox_CC0_pref.GetCurSel() == 0)                  /* selection '0' => 'Off' selected */
    {
        comboBox_CC0_start_sync.EnableWindow(FALSE);
        edit_CC0_keyslot.EnableWindow(FALSE);

        comboBox_CC1_pref.SetCurSel(0);
        comboBox_CC1_pref.EnableWindow(FALSE);
        comboBox_CC1_start_sync.EnableWindow(FALSE);
        edit_CC1_keyslot.EnableWindow(FALSE);
    }
    /* if 'automatic' mode selected: disable/enable corresponding graphical elements */
    else if(comboBox_CC0_pref.GetCurSel() == 1)             /* selection '1' => 'automatic' selected */
    {
        comboBox_CC0_start_sync.EnableWindow(TRUE);
        edit_CC0_keyslot.EnableWindow(FALSE);

        comboBox_CC1_pref.EnableWindow(TRUE);
    }
    /* if 'manual' mode selected: disable/enable corresponding graphical elements */
    else if(comboBox_CC0_pref.GetCurSel() == 2)             /* selection '2' => 'manual' selected */
    {
        comboBox_CC0_start_sync.EnableWindow(TRUE);
        edit_CC0_keyslot.EnableWindow(TRUE);

        comboBox_CC1_pref.EnableWindow(TRUE);
    }

}


/**
 * This function modifies the Dialog and its elements, if the keyslot setting selection of the CC0 changed
 */
void CflexiKeyslotListing::OnSelectionChanged_Combo_CC1()
{
    /* if 'off' mode selected: disable corresponding graphical elements */
    if(comboBox_CC1_pref.GetCurSel() == 0)                      /* selection '0' => 'Off' selected */
    {
        comboBox_CC1_start_sync.EnableWindow(FALSE);
        edit_CC1_keyslot.EnableWindow(FALSE);
    }
    /* if 'automatic' mode selected: disable/enable corresponding graphical elements */
    else if(comboBox_CC1_pref.GetCurSel() == 1)                 /* selection '1' => 'automatic' selected */
    {
        comboBox_CC1_start_sync.EnableWindow(TRUE);
        edit_CC1_keyslot.EnableWindow(FALSE);
    }
    /* if 'manual' mode selected: disable/enable corresponding graphical elements */
    else if(comboBox_CC1_pref.GetCurSel() == 2)                 /* selection '2' => 'manual' selected */
    {
        comboBox_CC1_start_sync.EnableWindow(TRUE);
        edit_CC1_keyslot.EnableWindow(TRUE);
    }

}



/**
 * Pushing the OK button calls this function.
 * The current state will be saved and transmitted
 */
void CflexiKeyslotListing::OnOK()
{
    /* declare variable */
    CString string_Keyslot;
    bool valid_data = true;


    /* set configuration CC0 */
    if(comboBox_CC0_pref.GetCurSel() == 0)                  // 'Off' selected
    {
        m_flexi_Keyslot_Settings->CC0_Keyslot_off = true;
        m_flexi_Keyslot_Settings->CC0_Keyslot_automatic = false;
        m_flexi_Keyslot_Settings->CC0_Keyslot_manual = false;
        m_flexi_Keyslot_Settings->CC0_Keyslot_used_for_Sync = false;
        m_flexi_Keyslot_Settings->CC0_Keyslot_used_for_Startup = false;
        m_flexi_Keyslot_Settings->CC0_Keyslot = 0;
    }
    else if(comboBox_CC0_pref.GetCurSel() == 1)             // 'automatic' selected
    {
        m_flexi_Keyslot_Settings->CC0_Keyslot_off = false;
        m_flexi_Keyslot_Settings->CC0_Keyslot_automatic = true;
        m_flexi_Keyslot_Settings->CC0_Keyslot_manual = false;


        if(comboBox_CC0_start_sync.GetCurSel() == 0)        // 'sync node' selected
        {
            m_flexi_Keyslot_Settings->CC0_Keyslot_used_for_Sync = true;
            m_flexi_Keyslot_Settings->CC0_Keyslot_used_for_Startup = false;
        }
        else if(comboBox_CC0_start_sync.GetCurSel() == 1)   // 'startup node' selected
        {
            m_flexi_Keyslot_Settings->CC0_Keyslot_used_for_Sync = true;
            m_flexi_Keyslot_Settings->CC0_Keyslot_used_for_Startup = true;
        }
        m_flexi_Keyslot_Settings->CC0_Keyslot = 0;
    }
    else if(comboBox_CC0_pref.GetCurSel() == 2)             // 'manual' selected
    {
        m_flexi_Keyslot_Settings->CC0_Keyslot_off = false;
        m_flexi_Keyslot_Settings->CC0_Keyslot_automatic = false;
        m_flexi_Keyslot_Settings->CC0_Keyslot_manual = true;

        if(comboBox_CC0_start_sync.GetCurSel() == 0)        // 'sync node' selected
        {
            m_flexi_Keyslot_Settings->CC0_Keyslot_used_for_Sync = true;
            m_flexi_Keyslot_Settings->CC0_Keyslot_used_for_Startup = false;
        }
        else if(comboBox_CC0_start_sync.GetCurSel() == 1)   // 'startup node' selected
        {
            m_flexi_Keyslot_Settings->CC0_Keyslot_used_for_Sync = true;
            m_flexi_Keyslot_Settings->CC0_Keyslot_used_for_Startup = true;
        }

        /* get content of CEdit and store it in proper data structure */
        edit_CC0_keyslot.GetWindowText(string_Keyslot);;
        unsigned long ulong_Keyslot = strtoul(string_Keyslot, nullptr, 10);
        m_flexi_Keyslot_Settings->CC0_Keyslot = (uint16) ulong_Keyslot;

        /* check if frame ID is valid */
        if( !(m_flexi_Keyslot_Settings->CC0_Keyslot > 0) ||
                !(m_flexi_Keyslot_Settings->CC0_Keyslot < GTFR_MAX_ALLOWED_FRAME_ID))
        {
            valid_data = false;
            MessageBox("The manual keyslot of CC0 contains invalid content!\nIt has to be between 1 and 1023.", "Error, manual keyslot of CC0", MB_OK);
        }

    }


    /* set configuration CC1 */
    if(comboBox_CC1_pref.GetCurSel() == 0)                  // 'Off' selected
    {
        m_flexi_Keyslot_Settings->CC1_Keyslot_off = true;
        m_flexi_Keyslot_Settings->CC1_Keyslot_automatic = false;
        m_flexi_Keyslot_Settings->CC1_Keyslot_manual = false;
        m_flexi_Keyslot_Settings->CC1_Keyslot_used_for_Sync = false;
        m_flexi_Keyslot_Settings->CC1_Keyslot_used_for_Startup = false;
        m_flexi_Keyslot_Settings->CC1_Keyslot = 0;
    }
    else if(comboBox_CC1_pref.GetCurSel() == 1)             // 'automatic' selected
    {
        m_flexi_Keyslot_Settings->CC1_Keyslot_off = false;
        m_flexi_Keyslot_Settings->CC1_Keyslot_automatic = true;
        m_flexi_Keyslot_Settings->CC1_Keyslot_manual = false;


        if(comboBox_CC1_start_sync.GetCurSel() == 0)        // 'sync node' selected
        {
            m_flexi_Keyslot_Settings->CC1_Keyslot_used_for_Sync = true;
            m_flexi_Keyslot_Settings->CC1_Keyslot_used_for_Startup = false;
        }
        else if(comboBox_CC1_start_sync.GetCurSel() == 1)   // 'startup node' selected
        {
            m_flexi_Keyslot_Settings->CC1_Keyslot_used_for_Sync = true;
            m_flexi_Keyslot_Settings->CC1_Keyslot_used_for_Startup = true;
        }
        m_flexi_Keyslot_Settings->CC1_Keyslot = 0;
    }
    else if(comboBox_CC1_pref.GetCurSel() == 2)             // 'manual' selected
    {
        m_flexi_Keyslot_Settings->CC1_Keyslot_off = false;
        m_flexi_Keyslot_Settings->CC1_Keyslot_automatic = false;
        m_flexi_Keyslot_Settings->CC1_Keyslot_manual = true;

        if(comboBox_CC1_start_sync.GetCurSel() == 0)        // 'sync node' selected
        {
            m_flexi_Keyslot_Settings->CC1_Keyslot_used_for_Sync = true;
            m_flexi_Keyslot_Settings->CC1_Keyslot_used_for_Startup = false;
        }
        else if(comboBox_CC1_start_sync.GetCurSel() == 1)   // 'startup node' selected
        {
            m_flexi_Keyslot_Settings->CC1_Keyslot_used_for_Sync = true;
            m_flexi_Keyslot_Settings->CC1_Keyslot_used_for_Startup = true;
        }

        /* get content of CEdit and store it in proper data structure */
        edit_CC1_keyslot.GetWindowText(string_Keyslot);;
        unsigned long ulong_Keyslot = strtoul(string_Keyslot, nullptr, 10);
        m_flexi_Keyslot_Settings->CC1_Keyslot = (uint16) ulong_Keyslot;

        /* check if frame ID is valid */
        if( !(m_flexi_Keyslot_Settings->CC1_Keyslot > 0) ||
                !(m_flexi_Keyslot_Settings->CC1_Keyslot < GTFR_MAX_ALLOWED_FRAME_ID))
        {
            valid_data = false;
            MessageBox("The manual keyslot of CC1 contains invalid content!\nIt has to be between 1 and 1023.", "Error, manual keyslot of CC1", MB_OK);
        }
    }

    /* if all data is valid, the dialog gets closed */
    if(valid_data == true)
    {
        CDialog::OnOK();
    }
}



/**
 * Pushing the Cancel button calls this function.
 * A default state will be saved and transmitted.
 */
void CflexiKeyslotListing::OnCancel()
{
    /* use the default settings */
    m_flexi_Keyslot_Settings->CC0_Keyslot_off = true;
    m_flexi_Keyslot_Settings->CC0_Keyslot_automatic = false;
    m_flexi_Keyslot_Settings->CC0_Keyslot_manual = false;
    m_flexi_Keyslot_Settings->CC0_Keyslot_used_for_Sync = false;
    m_flexi_Keyslot_Settings->CC0_Keyslot_used_for_Startup = false;
    m_flexi_Keyslot_Settings->CC0_Keyslot = 0;

    m_flexi_Keyslot_Settings->CC1_Keyslot_off = true;
    m_flexi_Keyslot_Settings->CC1_Keyslot_automatic = false;
    m_flexi_Keyslot_Settings->CC1_Keyslot_manual = false;
    m_flexi_Keyslot_Settings->CC1_Keyslot_used_for_Sync = false;
    m_flexi_Keyslot_Settings->CC1_Keyslot_used_for_Startup = false;
    m_flexi_Keyslot_Settings->CC1_Keyslot = 0;

    /* close dialog */
    CDialog::OnCancel();
}


/**
 * This function will generally be called to update the data.
 * In this case it sets a relation between the constants of the
 * graphical elements and the element's instances.
 */
void CflexiKeyslotListing::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_FLEXI_CC0_PREF, comboBox_CC0_pref);
    DDX_Control(pDX, IDC_FLEXI_CC1_PREF, comboBox_CC1_pref);
    DDX_Control(pDX, IDC_FLEXI_CC0_START_SYNC, comboBox_CC0_start_sync);
    DDX_Control(pDX, IDC_FLEXI_CC1_START_SYNC, comboBox_CC1_start_sync);
    DDX_Control(pDX, IDC_FLEXI_CC0_MANUAL, edit_CC0_keyslot);
    DDX_Control(pDX, IDC_FLEXI_CC1_MANUAL, edit_CC1_keyslot);
}
