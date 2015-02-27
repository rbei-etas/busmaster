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
*  DESCRIPTION:        Implements tab of acceptance mask options for flexi settings dialog.
*
*********************************************************************************************************************/

/** --- CHANGE LOG ---
*  V1.0.0.0        First reviewed version
*
*/


#include "stdafx.h"
#include "FLEXRAY_GIGATRONIK_FLEXI.h"
#include "TAB_ACCEPTANCE_MASK.h"



/**
* ctor, calls base class ctor
*/
CTAB_ACCEPTANCE_MASK::CTAB_ACCEPTANCE_MASK(CWnd* pParent /*=NULL*/)
    : CDialog(CTAB_ACCEPTANCE_MASK::IDD, pParent)
{

}

/**
* dector, nothing to do
*/
CTAB_ACCEPTANCE_MASK::~CTAB_ACCEPTANCE_MASK()
{
}


/**
* This function initializes the Dialog and its elements
*/
BOOL CTAB_ACCEPTANCE_MASK::OnInitDialog()
{
    CDialog::OnInitDialog();

    /* init graphic elements with default settings */
    button_check_filter0.EnableWindow(TRUE);
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

    return TRUE;
}


/**
* This function will generally be called to update the data.
* In this case it sets a relation between the constants of the
* graphical elements and the element's instances.
*/
void CTAB_ACCEPTANCE_MASK::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_FLEXI_CHECK_FIFO_0, button_check_filter0);
    DDX_Control(pDX, IDC_FLEXI_CHECK_FIFO_1, button_check_filter1);
    DDX_Control(pDX, IDC_FLEXI_CHECK_FIFO_2, button_check_filter2);
    DDX_Control(pDX, IDC_FLEXI_CHECK_FIFO_3, button_check_filter3);
    DDX_Control(pDX, IDC_FLEXI_LOW_LIMIT_0, edit_filter0_lower);
    DDX_Control(pDX, IDC_FLEXI_LOW_LIMIT_1, edit_filter1_lower);
    DDX_Control(pDX, IDC_FLEXI_LOW_LIMIT_2, edit_filter2_lower);
    DDX_Control(pDX, IDC_FLEXI_LOW_LIMIT_3, edit_filter3_lower);
    DDX_Control(pDX, IDC_FLEXI_UPPER_LIMIT_0, edit_filter0_upper);
    DDX_Control(pDX, IDC_FLEXI_UPPER_LIMIT_1, edit_filter1_upper);
    DDX_Control(pDX, IDC_FLEXI_UPPER_LIMIT_2, edit_filter2_upper);
    DDX_Control(pDX, IDC_FLEXI_UPPER_LIMIT_3, edit_filter3_upper);
    DDX_Control(pDX, IDC_FLEXI_ACCEPT_0, button_filter0_accept);
    DDX_Control(pDX, IDC_FLEXI_ACCEPT_1, button_filter1_accept);
    DDX_Control(pDX, IDC_FLEXI_ACCEPT_2, button_filter2_accept);
    DDX_Control(pDX, IDC_FLEXI_ACCEPT_3, button_filter3_accept);
    DDX_Control(pDX, IDC_FLEXI_REJECT_0, button_filter0_reject);
    DDX_Control(pDX, IDC_FLEXI_REJECT_1, button_filter1_reject);
    DDX_Control(pDX, IDC_FLEXI_REJECT_2, button_filter2_reject);
    DDX_Control(pDX, IDC_FLEXI_REJECT_3, button_filter3_reject);
}


/*---- add here macro entries for message-handler functions ----*/
BEGIN_MESSAGE_MAP(CTAB_ACCEPTANCE_MASK, CDialog)
    ON_BN_CLICKED(IDC_FLEXI_CHECK_FIFO_0, OnButtonClicked_Filter0)
    ON_BN_CLICKED(IDC_FLEXI_CHECK_FIFO_1, OnButtonClicked_Filter1)
    ON_BN_CLICKED(IDC_FLEXI_CHECK_FIFO_2, OnButtonClicked_Filter2)
    ON_BN_CLICKED(IDC_FLEXI_CHECK_FIFO_3, OnButtonClicked_Filter3)
END_MESSAGE_MAP()






/**
* This function reacts when the checkbox of the filter 0
* gets activated/deactivated. It enables/disables the
* corresponding graphical elements
*/
void CTAB_ACCEPTANCE_MASK::OnButtonClicked_Filter0()
{

    /* check if filter 0 is selected to activate options
    * depending on it, the corresponding graphical elements will be enable or disabled
    */
    if (button_check_filter0.GetCheck() == BST_CHECKED)
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

/**
* This function reacts when the checkbox of the filter 1
* gets activated/deactivated. It enables/disables the
* corresponding graphical elements
*/
void CTAB_ACCEPTANCE_MASK::OnButtonClicked_Filter1()
{

    /* check if filter 1 is selected to activate options
    * depending on it, the corresponding graphical elements will be enable or disabled
    */
    if (button_check_filter1.GetCheck() == BST_CHECKED)
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

/**
* This function reacts when the checkbox of the filter 2
* gets activated/deactivated. It enables/disables the
* corresponding graphical elements
*/
void CTAB_ACCEPTANCE_MASK::OnButtonClicked_Filter2()
{

    /* check if filter 2 is selected to activate options
    * depending on it, the corresponding graphical elements will be enable or disabled
    */
    if (button_check_filter2.GetCheck() == BST_CHECKED)
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

/**
* This function reacts when the checkbox of the filter 3
* gets activated/deactivated. It enables/disables the
* corresponding graphical elements
*/
void CTAB_ACCEPTANCE_MASK::OnButtonClicked_Filter3()
{

    /* check if filter 3 is selected to activate options
    * depending on it, the corresponding graphical elements will be enable or disabled
    */
    if (button_check_filter3.GetCheck() == BST_CHECKED)
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

