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
 * @file  
 *  CANSelectDlg.cpp
 *
 * @brief 
 *  This class is not used! The files can be removed one day if
 *  it really will not needed for controller selection.
 * 
 *  A MFC based dialog for later use to select IXXAT CAN controller
 *  from different boards. 
 *  The reason why it is not used: this dialog will be removed and 
 *  a new dialog in the application itself will be used.
 */


#include "CAN_IXXAT_VCI_stdafx.h"
#include "CAN_IXXAT_VCI.h"
#include "CANSelectDlg.h"



IMPLEMENT_DYNAMIC(CCANSelectDlg, CDialog)


BEGIN_MESSAGE_MAP(CCANSelectDlg, CDialog)
  ON_BN_CLICKED(IDOK, &CCANSelectDlg::OnBnClickedOk)
END_MESSAGE_MAP()


/**
 * @brief 
 *  Constructor.
 *
 * @param [in,out]  pParent
 *  If non-null, the parent windows.
 */
CCANSelectDlg::CCANSelectDlg(CWnd* pParent /*=NULL*/)
  : CDialog(CCANSelectDlg::IDD, pParent)
{

}

/**
 *
 * @brief
 *  Destructor.
 *
 */
CCANSelectDlg::~CCANSelectDlg()
{
}

/**
 * @brief 
 *  Exchanges data to/from the controls in this dialog.
 *
 * @param [in,out]  pDX
 *  If non-null, an object that manages the data exchange operation.
 *
 */
void CCANSelectDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
}


/**
 * @brief 
 *  Called when the user clicks the OK button. This
 *  will close the dialog and return IDOK.
 *
 */
void CCANSelectDlg::OnBnClickedOk()
{
  OnOK();
}
