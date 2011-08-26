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

/**
 * \file      ControllerPPage.h
 * \brief     Definition file for CControllerPPage class.
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for CControllerPPage class.
 */


#pragma once

#include "ConfigDlg_resource.h"	
// CControllerPPage dialog

class CControllerPPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CControllerPPage)

public:
	CControllerPPage();
	virtual ~CControllerPPage();

// Dialog Data
	enum { IDD = IDD_PPAGE_CONTROLLER };
    virtual void OnOK();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
