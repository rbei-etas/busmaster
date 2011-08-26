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
 * \file      CANDBConverterDlg.h
 * \brief     CCANDBConverterDlg class.
 * \author    RBIN/EBS1 - Mahesh B S
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * CCANDBConverterDlg class.
 */
/**
* \file       CANDBConverterDlg.h
* \brief      CCANDBConverterDlg class.
* \authors    Mahesh B S
* \date       17.11.2004
* \copyright  Copyright &copy; 2011 Robert Bosch Engineering and Business Solutions.  All rights reserved.
*/
#if !defined(AFX_CANDBCONVERTERDLG_H__BB35E6F0_48F4_4CC8_9F24_473ACBF35E24__INCLUDED_)
#define AFX_CANDBCONVERTERDLG_H__BB35E6F0_48F4_4CC8_9F24_473ACBF35E24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCANDBConverterDlg dialog


class CCANDBConverterDlg : public CDialog
{
    // Construction
public:
    CCANDBConverterDlg(CWnd* pParent = NULL);	// standard constructor

    // Dialog Data
    //{{AFX_DATA(CCANDBConverterDlg)
    enum { IDD = IDD_CANDBCONVERTER_DIALOG };
    // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CCANDBConverterDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CCANDBConverterDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnCanoeBrowseBtn();
    afx_msg void OnCanmonBrowseBtn();
    afx_msg void OnConvertBtn();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CANDBCONVERTERDLG_H__BB35E6F0_48F4_4CC8_9F24_473ACBF35E24__INCLUDED_)
