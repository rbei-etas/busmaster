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
 *  FILENAME:           FLEXRAY_GIGATRONIK_FLEXI_dialog_keyslot.h
 *
 *  COPYRIGHT:          Gigatronik Ingolstadt GmbH
 *  AUTHOR, DPT:        M. Morgott, KE
 *
 *  DATE OF CREATION:   28.11.2014
 *
 *  DESCRIPTION:        Defines class of keyslot dialog.
 *
 *********************************************************************************************************************/

/** --- CHANGE LOG ---
 *  V1.0.0.0        First reviewed version
 *
 */


#pragma once


/* project includes */
#include "FLEXRAY_GIGATRONIK_FLEXI_GtFr_NativeInterface.h"
#include "FLEXRAY_GIGATRONIK_FLEXI_resource.h"

/* include afxwin.h to avoid windows.h inclusion error */
#include <afxwin.h>
#include <afxcmn.h>


/* struct to store flex-i specific settings */
typedef struct s_flexi_Keyslot_Settings
{
    bool CC0_Keyslot_off;
    bool CC1_Keyslot_off;
    bool CC0_Keyslot_automatic;
    bool CC1_Keyslot_automatic;
    bool CC0_Keyslot_manual;
    bool CC1_Keyslot_manual;
    bool CC0_Keyslot_used_for_Sync;
    bool CC1_Keyslot_used_for_Sync;
    bool CC0_Keyslot_used_for_Startup;
    bool CC1_Keyslot_used_for_Startup;
    uint16 CC0_Keyslot;
    uint16 CC1_Keyslot;
} flexi_Keyslot_Settings;


/* class definition of the dialog for keyslot settings */
class CflexiKeyslotListing : public CDialog
{
public:
    /* ctors */
    CflexiKeyslotListing();
    CflexiKeyslotListing(flexi_Keyslot_Settings* p_flexi_Keyslot_Settings);

    /* IDD represents used identifier of used dialog resource */
    enum { IDD = IDD_DIALOG_FLEXI_KEYSLOT };

    /* definition of graphical elements */
    CComboBox   comboBox_CC0_pref;
    CComboBox   comboBox_CC1_pref;
    CComboBox   comboBox_CC0_start_sync;
    CComboBox   comboBox_CC1_start_sync;
    CEdit       edit_CC0_keyslot;
    CEdit       edit_CC1_keyslot;

protected:
    /* method definition */
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    void OnSelectionChanged_Combo_CC0();
    void OnSelectionChanged_Combo_CC1();

protected:
    /* this macro declares that the class defines a message map */
    DECLARE_MESSAGE_MAP()

    /* stores pointer of the data structure for the keyslot settings of the application */
    flexi_Keyslot_Settings* m_flexi_Keyslot_Settings;
};
