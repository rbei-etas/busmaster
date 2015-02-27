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
 *  FILENAME:           FLEXRAY_GIGATRONIK_FLEXI_dialog_settings_tab.h
 *
 *  COPYRIGHT:          Gigatronik Ingolstadt GmbH
 *  AUTHOR, DPT:        M. Morgott, KE
 *
 *  DATE OF CREATION:   09.12.2014
 *
 *  DESCRIPTION:        Defines class of keyslot dialog.
 *
 *********************************************************************************************************************/

/** --- CHANGE LOG ---
 *  V1.0.0.0        First reviewed version
 *
 */

#pragma once

#include "resource1.h"
#include "afxcmn.h"

#include "FLEXRAY_GIGATRONIK_FLEXI_GtFr_NativeInterface.h"

#include "TAB_ACCEPTANCE_MASK.h"
#include "TAB_MESSAGE_VIEW.h"
#include "TAB_TRANSMISSION_MODE.h"
#include "afxwin.h"



/* struct to store flex-i specific settings */
typedef struct s_flexi_Settings
{
    GtFr_MsgBufTxModeType transmissionMode_staticSeg;
    GtFr_MsgBufTxModeType transmissionMode_dynSeg;
    bool showNullFrames;
    bool showSyncStartupFrames;
    bool FifoFilterActive[4];
    GtFr_ExtRxFifoFilterCfgType FifoFilter[4];
} flexi_Settings;


/* class definition of the dialog for flexi specific settings */
class CFLEXRAY_GIGATRONIK_FLEXI_dialog_settings_tab : public CDialog
{

public:
    CFLEXRAY_GIGATRONIK_FLEXI_dialog_settings_tab(CWnd* pParent = NULL);
    virtual ~CFLEXRAY_GIGATRONIK_FLEXI_dialog_settings_tab();

    BOOL OnInitDialog();

    /* IDD represents used identifier of used dialog resource */
    enum { IDD = IDD_DIALOG_FLEXI_SETTINGS_TAB };

private:
    /* definition private methods */
    bool verifyFifoFilter(flexi_Settings new_Settings);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    void OnCancel();


    /* stores data structure for the flexi settings */
    flexi_Settings m_flexi_Settings;

    /* this macro declares that the class defines a message map */
    DECLARE_MESSAGE_MAP()
public:
    CTabCtrl m_tabcontrol;
    CButton button_update;

    CTAB_ACCEPTANCE_MASK m_tab_acceptance;
    CTAB_MESSAGE_VIEW m_tab_message_view;
    CTAB_TRANSMISSION_MODE m_tab_transmission;


    /**
    * This function will be called if the tab selecetion get changed.
    * Examine the selected tab and show the corresponding content.
    */
    afx_msg void OnTcnSelchangeTabcontrol(NMHDR* pNMHDR, LRESULT* pResult);

    /**
    * This function enables the update button if the
    * application is ready.
    */
    void readyToUpdate();

    /**
    * This function reads the current settings, store it in the proper
    * data structure and return it.
    */
    flexi_Settings getSettings();

    /**
    * This function will be called if the update button gets pushed.
    * At first the function get the current configuration, verify it
    * and call the corresponding callback function.
    */
    void OnUpdate();

};

