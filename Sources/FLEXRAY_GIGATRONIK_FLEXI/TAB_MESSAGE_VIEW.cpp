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
*  FILENAME:           TAB_MESSAGE_VIEW.cpp
*
*  COPYRIGHT:          Gigatronik Ingolstadt GmbH
*  AUTHOR, DPT:        M. Morgott, KE
*
*  DATE OF CREATION:   09.12.2014
*
*  DESCRIPTION:        Implements tab of message view options for flexi settings dialog.
*
*********************************************************************************************************************/

/** --- CHANGE LOG ---
*  V1.0.0.0        First reviewed version
*
*/

#include "stdafx.h"
#include "FLEXRAY_GIGATRONIK_FLEXI.h"
#include "TAB_MESSAGE_VIEW.h"



/**
* ctor, calls base class ctor
*/
CTAB_MESSAGE_VIEW::CTAB_MESSAGE_VIEW(CWnd* pParent /*=NULL*/)
    : CDialog(CTAB_MESSAGE_VIEW::IDD, pParent)
{

}


/**
* dector, nothing to do
*/
CTAB_MESSAGE_VIEW::~CTAB_MESSAGE_VIEW()
{
}

/**
* This function will generally be called to update the data.
* In this case it sets a relation between the constants of the
* graphical elements and the element's instances.
*/
void CTAB_MESSAGE_VIEW::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_FLEXI_NULL, button_show_null_frames);
    DDX_Control(pDX, IDC_FLEXI_STARTUP, button_show_startup_sync_frames);
}


/*---- add here macro entries for message-handler functions ----*/
BEGIN_MESSAGE_MAP(CTAB_MESSAGE_VIEW, CDialog)
END_MESSAGE_MAP()


/**
* This function initializes the Dialog and its elements
*/
BOOL CTAB_MESSAGE_VIEW::OnInitDialog()
{
    CDialog::OnInitDialog();

    return TRUE;
}