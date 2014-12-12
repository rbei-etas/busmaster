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
*  FILENAME:           TAB_ACCEPTANCE_MASK.h
*
*  COPYRIGHT:          Gigatronik Ingolstadt GmbH
*  AUTHOR, DPT:        M. Morgott, KE
*
*  DATE OF CREATION:   09.12.2014
*
*  DESCRIPTION:        Defines tab of acceptance mask options for flexi settings dialog.
*
*********************************************************************************************************************/

/** --- CHANGE LOG ---
*  V1.0.0.0        First reviewed version
*
*/

#pragma once

#include "resource1.h"
#include "afxwin.h"


class CTAB_ACCEPTANCE_MASK : public CDialog
{

public:
    CTAB_ACCEPTANCE_MASK(CWnd* pParent = NULL);
    virtual ~CTAB_ACCEPTANCE_MASK();

    enum { IDD = IDD_TAB_ACCEPTANCE };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    BOOL OnInitDialog();

    /* definition of protected methods */
    void OnButtonClicked_Filter0();
    void OnButtonClicked_Filter1();
    void OnButtonClicked_Filter2();
    void OnButtonClicked_Filter3();

    DECLARE_MESSAGE_MAP()
public:

    CButton button_check_filter0;
    CButton button_check_filter1;
    CButton button_check_filter2;
    CButton button_check_filter3;
    CEdit edit_filter0_lower;
    CEdit edit_filter1_lower;
    CEdit edit_filter2_lower;
    CEdit edit_filter3_lower;
    CEdit edit_filter0_upper;
    CEdit edit_filter1_upper;
    CEdit edit_filter2_upper;
    CEdit edit_filter3_upper;
    CButton button_filter0_accept;
    CButton button_filter1_accept;
    CButton button_filter2_accept;
    CButton button_filter3_accept;
    CButton button_filter0_reject;
    CButton button_filter1_reject;
    CButton button_filter2_reject;
    CButton button_filter3_reject;

};
