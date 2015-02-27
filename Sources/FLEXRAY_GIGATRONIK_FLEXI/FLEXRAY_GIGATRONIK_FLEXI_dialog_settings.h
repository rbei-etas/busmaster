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
 *  FILENAME:           FLEXRAY_GIGATRONIK_FLEXI_dialog_settings.h
 *
 *  COPYRIGHT:          Gigatronik Ingolstadt GmbH
 *  AUTHOR, DPT:        M. Morgott, KE
 *
 *  DATE OF CREATION:   28.11.2014
 *
 *  DESCRIPTION:        Defines class of GIGABOX flex-i settings dialog.
 *
 *********************************************************************************************************************/

/** --- CHANGE LOG ---
 *
 *
 */

#pragma once


#include "FLEXRAY_GIGATRONIK_FLEXI_GtFr_NativeInterface.h"
#include "FLEXRAY_GIGATRONIK_FLEXI_resource.h"

/* include afxwin.h to avoid windows.h inclusion error */
#include <afxwin.h>
#include <afxcmn.h>


///* struct to store flex-i specific settings */
//typedef struct s_flexi_Settings
//{
//    GtFr_MsgBufTxModeType transmissionMode_staticSeg;
//    GtFr_MsgBufTxModeType transmissionMode_dynSeg;
//    bool showNullFrames;
//    bool showSyncStartupFrames;
//    bool FifoFilterActive[4];
//    GtFr_ExtRxFifoFilterCfgType FifoFilter[4];
//}flexi_Settings;


/* class definition of the dialog for flexi specific settings */
class CflexiSettingsListing : public CDialog
{
public:
    /* ctor */
    CflexiSettingsListing();

public:
    /* definition public methods */
    void readyToUpdate();
    flexi_Settings getSettings();

private:
    /* definition private methods */
    bool verifyFifoFilter(flexi_Settings new_Settings);

protected:
    /* definition protected methods */
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    void OnButtonClicked_Filter0();
    void OnButtonClicked_Filter1();
    void OnButtonClicked_Filter2();
    void OnButtonClicked_Filter3();
    void OnUpdate();

protected:
    /* IDD represents used identifier of used dialog resource */
    enum { IDD = IDD_DIALOG_FLEXI_SETTINGS };

    /* definition of graphical elements */
    CButton     button_update;

    CButton     button_continuous_static;
    CButton     button_single_shot_static;
    CButton     button_continuous_dyn;
    CButton     button_single_shot_dyn;

    CButton     button_show_null_frames;
    CButton     button_show_startup_sync_frames;

    CButton     button_check_filter0;
    CButton     button_check_filter1;
    CButton     button_check_filter2;
    CButton     button_check_filter3;

    CEdit       edit_filter0_lower;
    CEdit       edit_filter0_upper;
    CEdit       edit_filter1_lower;
    CEdit       edit_filter1_upper;
    CEdit       edit_filter2_lower;
    CEdit       edit_filter2_upper;
    CEdit       edit_filter3_lower;
    CEdit       edit_filter3_upper;

    CButton     button_filter0_accept;
    CButton     button_filter0_reject;
    CButton     button_filter1_accept;
    CButton     button_filter1_reject;
    CButton     button_filter2_accept;
    CButton     button_filter2_reject;
    CButton     button_filter3_accept;
    CButton     button_filter3_reject;

protected:
    /* stores data structure for the flexi settings */
    flexi_Settings m_flexi_Settings;

    /* this macro declares that the class defines a message map */
    DECLARE_MESSAGE_MAP()
};

