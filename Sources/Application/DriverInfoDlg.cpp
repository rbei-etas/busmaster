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
 * \file      DriverInfoDlg.cpp
 * \brief     This file contain the definition all member function of
 * \author    Amarnath Shastry
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition all member function of
 */
#include "stdafx.h"
#include "BUSMASTER.h"
#include "DriverInfoDlg.h"

// For DIL Interface Class
#include "Include/BaseDefs.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_Datatypes.h"
#include "DataTypes/DIL_Datatypes.h"
#include "DIL_Interface/BaseDIL_CAN.h"
// DIL CAN interface
extern CBaseDIL_CAN* g_pouDIL_CAN_Interface;

/////////////////////////////////////////////////////////////////////////////
// CDriverInfoDlg dialog

/*******************************************************************************
 Function Name    :  CDriverInfoDlg
 Input(s)         :  CWnd* pParent: Pointer to parent class
 Output           :  -
 Functionality    :  Constructor is called when user create an object of
                     this class. Initialisation of all data members
 Member of        :  CDriverInfoDlg
 Friend of        :      -
 Author(s)        :  Raja N
 Date Created     :  07.09.2004
 Modifications    :  Modified this function to refer HI layer to get information
                     about the driver based on the device selection.
*******************************************************************************/
CDriverInfoDlg::CDriverInfoDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CDriverInfoDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDriverInfoDlg)
    m_omStrDriverInfo = STR_EMPTY;
    //}}AFX_DATA_INIT
    CString omStrFormatDriverInfo;
    m_omStrDriverInfo = _T("");
}


/*******************************************************************************
  Function Name    :  DoDataExchange
  Input(s)         :
  Output           :
  Functionality    :  Called by the framework to exchange and validate dialog
                      data
  Member of        :  CDriverInfoDlg
  Friend of        :   -
  Author(s)        :  Raja N
  Date Created     :  07.09.2004
  Modifications    :
*******************************************************************************/
void CDriverInfoDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDriverInfoDlg)
    DDX_Text(pDX, IDC_STAT_DRIVER_VERSION, m_omStrDriverInfo);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDriverInfoDlg, CDialog)
    //{{AFX_MSG_MAP(CDriverInfoDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDriverInfoDlg message handlers
/*******************************************************************************
  Function Name    :  OnInitDialog
  Input(s)         :    -
  Output           :    -
  Functionality    :  Called by the framework to when the dialog control is
                      initialised. The control will be initialised with last
                      used value or default value.
  Member of        :  CDriverInfoDlg
  Friend of        :      -
  Author(s)        :  Raja N
  Date Created     :  07.09.2004
  Modifications    :
*******************************************************************************/
BOOL CDriverInfoDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    // Resize the dialog in USB build
    // Pointer to components
    CWnd* pomWnd = NULL;
    // Hide Copywright message
    pomWnd = GetDlgItem(IDC_STAT_COPYRIGHT);

    if( pomWnd != NULL )
    {
        //pomWnd->SetWindowText("USB driver from Peak Systems");
        pomWnd->ShowWindow ( SW_HIDE );
        // Invalidate
        pomWnd = NULL;
    }

    // Store current control rect and dialog rect
    CRect omRect, omWndRect;
    // Get Dialog Size
    GetWindowRect( &omWndRect );
    // Get the Bottom line object
    pomWnd = GetDlgItem( IDC_STAT_LINE3 );

    if( pomWnd != NULL )
    {
        // Get the position
        pomWnd->GetWindowRect(&omRect);
        omWndRect.bottom = omRect.bottom;
        // Resize the dialog upto botton line
        MoveWindow( omWndRect.left, omWndRect.top, omWndRect.Width(),
                    omWndRect.Height() );
        // Update dialog size
        GetWindowRect( &omWndRect );
        // Invalidate
        pomWnd = NULL;
    }

    // Offset is 1/4th of dialog size
    int nOffset = omWndRect.Height() / 4;
    // Center the Version Label
    pomWnd = GetDlgItem( IDC_STAT_DRIVER_VERSION );

    if( pomWnd != NULL )
    {
        int nWidth = 0, nHeight = 0;
        pomWnd->CenterWindow();
        pomWnd->GetWindowRect( omRect );
        ScreenToClient(omRect);
        nWidth = omRect.Width();
        nHeight = omRect.Height();
        // Move the top to First quarter
        omRect.top =  nOffset - nHeight / 2;
        // Move the window to the new position
        pomWnd->MoveWindow( omRect.left, omRect.top, nWidth, nHeight );
        pomWnd = NULL;
    }

    // Change the position of OK Button
    pomWnd = GetDlgItem ( IDOK );

    if( pomWnd != NULL )
    {
        // Move the button to the center of the dialog
        pomWnd->CenterWindow();
        pomWnd->GetWindowRect( omRect );
        ScreenToClient(omRect);
        int nWidth = 0, nHeight = 0;
        nWidth = omRect.Width();
        nHeight = omRect.Height();
        // Move it to the thired Quarter
        omRect.top = nOffset * 2 - nHeight / 2;
        // Move the window to its new location
        pomWnd->MoveWindow( omRect.left, omRect.top, nWidth, nHeight, TRUE );
        pomWnd = NULL;
    }

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}
