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
 *  CANSelectDlg.h
 *
 * @brief
 *  This class is not used! The files can be removed one day if
 *  it really will not needed for controller selection.
 *  A MFC based dialog for later use to select IXXAT CAN controller
 *  from different boards.
 *  The reason why it is not used: this dialog will be removed and
 *  a new dialog in the application itself will be used.
 */

#pragma once



/**
 * @class CCANSelectDlg
 *
 * @brief
 *  This class is not used! The files can be removed one day if
 *  it really will not needed for controller selection.
 *  A MFC based dialog for later use to select IXXAT CAN controller
 *  from different boards.
 *  The reason why it is not used: this dialog will be removed and
 *  a new dialog in the application itself will be used.
 *
 *
 */
class CCANSelectDlg : public CDialog
{
    DECLARE_DYNAMIC(CCANSelectDlg)

public:
    CCANSelectDlg(CWnd* pParent = NULL);   // standard constructor
    virtual ~CCANSelectDlg();

    // Dialog Data
    enum { IDD = IDD_IXXAT_CAN_SELECTION_DLG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
};
