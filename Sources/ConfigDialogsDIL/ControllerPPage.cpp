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
 * \file      ControllerPPage.cpp
 * \brief     Source file for CControllerPPage class.
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Source file for CControllerPPage class.
 */


// ControllerPPage.cpp : implementation file
//

#include "CongDlg_stdafx.h"
#include "ControllerPPage.h"


// CControllerPPage dialog

IMPLEMENT_DYNAMIC(CControllerPPage, CPropertyPage)
CControllerPPage::CControllerPPage()
	: CPropertyPage(CControllerPPage::IDD)
{
}

CControllerPPage::~CControllerPPage()
{
}

void CControllerPPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}
void CControllerPPage::OnOK()
{

}

BEGIN_MESSAGE_MAP(CControllerPPage, CPropertyPage)
END_MESSAGE_MAP()


// CControllerPPage message handlers
