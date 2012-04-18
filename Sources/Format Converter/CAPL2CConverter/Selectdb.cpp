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
 * \file      Selectdb.cpp
 * \brief     Implementation file for CSelectdb class
 * \authors   Amit Ranjan
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CSelectdb class
 */

/* Project includes */
#include "Selectdb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectdb dialog


CSelectdb::CSelectdb(CWnd* pParent /*=NULL*/)
    : CDialog(CSelectdb::IDD, pParent)
{
    //{{AFX_DATA_INIT(CSelectdb)
    m_option = FALSE;
    //}}AFX_DATA_INIT
    //  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CSelectdb::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSelectdb)
    DDX_Check(pDX, IDC_CHKB_OPTION, m_option);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectdb, CDialog)
    //{{AFX_MSG_MAP(CSelectdb)
    ON_BN_CLICKED(IDC_CBTN_CANOEDB, OnBrowseCANoeDb)
    ON_BN_CLICKED(IDC_CBTN_BUSMASTERDB, OnBrowseBUSMASTERDb)
    ON_BN_CLICKED(IDC_CHKB_OPTION, OnChkbOption)
    ON_EN_CHANGE(IDC_EDIT_ADCANOE, OnChangeEditAdcanoe)
    ON_WM_CLOSE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectdb message handlers

void CSelectdb::SaveSettings()
{
    //this fuction will save the settings of last selected file in the registry.
    CString strSection       = "Files";
    CString strStringItem    = "Database File";
    CWinApp* pApp = AfxGetApp();
    CString omStrDatabaseName;
    CWnd* pCAPL = GetDlgItem(IDC_EDIT_ADCANOE);
    pCAPL->GetWindowText(omStrDatabaseName);
    pApp->WriteProfileString(strSection, strStringItem,omStrDatabaseName);
}





