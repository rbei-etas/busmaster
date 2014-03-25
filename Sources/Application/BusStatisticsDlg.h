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
 * \file      BusStatisticsDlg.h
 * \brief     This file contain the declaration of CBusStatisticsDlg class
 * \author    Amitesh Bharti
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the declaration of CBusStatisticsDlg class
 */

#pragma once

// For Color ListBox
#include "MessageList.h"
#include "BaseBusStatisticCAN.h"
#include "Utility/XMLUtils.h"
#include "resource.h"
#include "NetworkStatistics.h"

//BusStatistics Class
class CBusStatisticsDlg : public CCommonStatistics
{
   DECLARE_DYNCREATE(CBusStatisticsDlg)
	// Construction
public:
	enum { IDD = IDD_DLG_NETWORK_STATISTICS };
	CBusStatisticsDlg():CCommonStatistics(CBusStatisticsDlg::IDD, CAN)
	{
		
	}
    // standard constructor
    CBusStatisticsDlg(CBaseBusStatisticCAN*, CWnd* pParent = NULL, int nChannelCount = defNO_OF_CHANNELS);
	CBusStatisticsDlg(CBaseBusStatisticCAN*, int nChannelCount = defNO_OF_CHANNELS);
   
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CBusStatisticsDlg)
    virtual BOOL OnInitDialog();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    afx_msg LRESULT vUpdateFields(WPARAM wParam, LPARAM lParam);
private:
    CBaseBusStatisticCAN* m_pouBSCAN;
    // String objects to format bus load related parameters
    CString m_omStrBusLoad;
    CString m_omStrPeakBusLoad;
    CString m_omStrAvgBusLoad;
    
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
};
