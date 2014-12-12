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
*  DESCRIPTION:        Implements tab of transmission mode options for flexi settings dialog.
*
*********************************************************************************************************************/

/** --- CHANGE LOG ---
*  V1.0.0.0        First reviewed version
*
*/


#include "stdafx.h"
#include "FLEXRAY_GIGATRONIK_FLEXI.h"
#include "TAB_TRANSMISSION_MODE.h"



/*---- add here macro entries for message-handler functions ----*/
BEGIN_MESSAGE_MAP(CTAB_TRANSMISSION_MODE, CDialog)
END_MESSAGE_MAP()


/**
* ctor, calls base class ctor
*/
CTAB_TRANSMISSION_MODE::CTAB_TRANSMISSION_MODE(CWnd* pParent)
    : CDialog(CTAB_TRANSMISSION_MODE::IDD, pParent)
{

}

/**
* dector, nothing to do
*/
CTAB_TRANSMISSION_MODE::~CTAB_TRANSMISSION_MODE()
{
}

/**
* This function initializes the Dialog and its elements
*/
BOOL CTAB_TRANSMISSION_MODE::OnInitDialog()
{
    CDialog::OnInitDialog();

    /* init graphic elements with default settings */
    button_continuous_dyn.SetCheck(BST_CHECKED);
    button_continuous_static.SetCheck(BST_CHECKED);

    return TRUE;
}

/**
* This function will generally be called to update the data.
* In this case it sets a relation between the constants of the
* graphical elements and the element's instances.
*/
void CTAB_TRANSMISSION_MODE::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_FLEXI_CONTINUOUS_STATIC, button_continuous_static);
    DDX_Control(pDX, IDC_FLEXI_CONTINUOUS_DYN, button_continuous_dyn);
    DDX_Control(pDX, IDC_FLEXI_SINGLE_DYN, button_single_shot_dyn);
    DDX_Control(pDX, IDC_FLEXI_SINGLE_STATIC, button_single_shot_static);
}

