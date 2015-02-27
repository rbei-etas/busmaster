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
*  FILENAME:           TAB_MESSAGE_VIEW.h
*
*  COPYRIGHT:          Gigatronik Ingolstadt GmbH
*  AUTHOR, DPT:        M. Morgott, KE
*
*  DATE OF CREATION:   09.12.2014
*
*  DESCRIPTION:        Defines tab of message view options for flexi settings dialog.
*
*********************************************************************************************************************/

/** --- CHANGE LOG ---
*  V1.0.0.0        First reviewed version
*
*/

#pragma once

#include "resource1.h"
#include "afxwin.h"


class CTAB_MESSAGE_VIEW : public CDialog
{

public:
    CTAB_MESSAGE_VIEW(CWnd* pParent = NULL);
    virtual ~CTAB_MESSAGE_VIEW();

    enum { IDD = IDD_TAB_MESSAGE_VIEW };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    BOOL OnInitDialog();

    DECLARE_MESSAGE_MAP()
public:
    CButton button_show_null_frames;
    CButton button_show_startup_sync_frames;
};
