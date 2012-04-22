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
 * \file      TSEditorGUI_SettingsDlg.h
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

#include "TSEditorGUI_resource.h"
#include "Utility/ColourPicker.h"
#include "afxwin.h"

class CTSEditorSettingsDlg : public CDialog
{
	DECLARE_DYNCREATE(CTSEditorSettingsDlg)
public:
    CColourPicker m_ouBkColorBtn;
    CColourPicker m_ouTxtColorBtn;
    BOOL m_bQueryConfirm;
    CColourPicker m_Row1Color;
    CColourPicker m_Row2Color;
    CButton m_omDefaultVal;
    
    // Dialog Data
	enum { IDD = IDD_SETTINGS};
public:
	CTSEditorSettingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTSEditorSettingsDlg();
    afx_msg void OnBnClickedDefault();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
