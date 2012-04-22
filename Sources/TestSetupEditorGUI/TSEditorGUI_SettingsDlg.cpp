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
 * \file      TSEditorGUI_SettingsDlg.cpp
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "TSEditorGUI_stdafx.h"
#include "TSEditorGUI_SettingsDlg.h"
#include "TSEditorGUI_Definitions.h"
#include ".\tseditorgui_settingsdlg.h"

IMPLEMENT_DYNCREATE(CTSEditorSettingsDlg, CDialog)

BEGIN_MESSAGE_MAP(CTSEditorSettingsDlg, CDialog)
    ON_BN_CLICKED(ID_DEFAULT, OnBnClickedDefault)    
END_MESSAGE_MAP()
/******************************************************************************
Function Name  :  CTSEditorSettingsDlg
Input(s)       :  
Output         :  -
Functionality  :   
Member of      :  CTSEditorSettingsDlg
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
CTSEditorSettingsDlg::CTSEditorSettingsDlg(CWnd* /*pParent*/ /*=NULL*/)
	: CDialog(CTSEditorSettingsDlg::IDD)
    , m_bQueryConfirm(FALSE)    
{
}

/******************************************************************************
Function Name  :  ~CTSEditorSettingsDlg
Input(s)       :  
Output         :  -
Functionality  :   
Member of      :  CTSEditorSettingsDlg
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
CTSEditorSettingsDlg::~CTSEditorSettingsDlg()
{
}

/******************************************************************************
Function Name  :  DoDataExchange
Input(s)       :  
Output         :  void
Functionality  :   
Member of      :  CTSEditorSettingsDlg
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
void CTSEditorSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BTN_BKG, m_ouBkColorBtn);
    DDX_Control(pDX, IDC_BTN_TXT, m_ouTxtColorBtn);
    DDX_Check(pDX, IDC_AUTOSAVE, m_bQueryConfirm);
    DDX_Control(pDX, IDC_ROW1_COL, m_Row1Color);
    DDX_Control(pDX, IDC_ROW2_COL, m_Row2Color);
    DDX_Control(pDX, ID_DEFAULT, m_omDefaultVal);
}

/******************************************************************************
Function Name  :  OnInitDialog
Input(s)       :  
Output         :  BOOL
Functionality  :   
Member of      :  CTSEditorSettingsDlg
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
BOOL CTSEditorSettingsDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    return TRUE;  // return TRUE  unless you set the focus to a control
}
/******************************************************************************
Function Name  :  OnBnClickedDefault
Input(s)       :  
Output         :  void
Functionality  :   
Member of      :  CTSEditorSettingsDlg
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
void CTSEditorSettingsDlg::OnBnClickedDefault()
{
    m_ouBkColorBtn.SetColour(def_COLOR_TREE_BKG);
    m_ouTxtColorBtn.SetColour(RGB(0,0,0));
    m_Row1Color.SetColour(def_COLOR_FIRSTROW);
    m_Row2Color.SetColour(def_COLOR_SECONDROW);
    m_bQueryConfirm = TRUE;
}