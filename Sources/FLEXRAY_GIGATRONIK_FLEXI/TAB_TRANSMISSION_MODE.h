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
*  FILENAME:           TAB_TRANSMISSION_MODE.h
*
*  COPYRIGHT:          Gigatronik Ingolstadt GmbH
*  AUTHOR, DPT:        M. Morgott, KE
*
*  DATE OF CREATION:   09.12.2014
*
*  DESCRIPTION:        Defines tab of transmission mode options for flexi settings dialog.
*
*********************************************************************************************************************/

/** --- CHANGE LOG ---
*  V1.0.0.0        First reviewed version
*
*/


#pragma once

#include "resource1.h"
#include "afxwin.h"


class CTAB_TRANSMISSION_MODE : public CDialog
{

public:
    CTAB_TRANSMISSION_MODE(CWnd* pParent = NULL);
    virtual ~CTAB_TRANSMISSION_MODE();

    enum { IDD = IDD_TAB_TRANSMISSION };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    BOOL OnInitDialog();

    DECLARE_MESSAGE_MAP()
public:
    CButton button_continuous_static;
    CButton button_continuous_dyn;
    CButton button_single_shot_dyn;
    CButton button_single_shot_static;

};
