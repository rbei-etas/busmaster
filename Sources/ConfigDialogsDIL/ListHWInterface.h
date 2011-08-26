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
 * \file      ListHWInterface.h
 * \brief     Definition file for CListHWInterface class.
 * \authors   Ratnadip Choudhury, Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition file for CListHWInterface class.
 */

#pragma once
 #include <afxwin.h>
 #include "DataTypes/DIL_Datatypes.h"

// CDatabaseDissociateDlg dialog

class CListHWInterface : public CDialog
{
	DECLARE_DYNAMIC(CListHWInterface)

public:
	CListHWInterface(CWnd* pParent = NULL, int pCount = 0, INTERFACE_HW* pInterfaceList = NULL);   // standard constructor
	virtual ~CListHWInterface();    

// Dialog Data
	enum { IDD = IDD_LIST_HW_INTRS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 
    DECLARE_MESSAGE_MAP()

    int m_nSelIndex;
    int m_nCount;
    void vUpdateControlsWithCurrSel(void);

public:
    afx_msg void OnBnClickedOk();
    virtual BOOL OnInitDialog();

    CListBox m_omListBox;
    CString m_omCardName;
    CString m_omVendor;
    CString m_omDescription;
    INTERFACE_HW *m_pInterfaceList;

    afx_msg void OnLbnSelchangeList3();
    int GetSelIndex();
};
