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
 * \file      LINBusStatisticsDlg.h
 * \brief     This file contain the declaration of CBusStatisticsDlg class
 * \author    Shashank Vernekar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the declaration of CEthernetBusStatisticsDlg class
 */

#pragma once

// For Color ListBox
#include "MessageList.h"
#include "Utility/XMLUtils.h"
#include "PPageMessage.h"
#include "resource.h"
#include "BaseBusStatisticEthernet.h"
#include "NetworkStatistics.h"

//BusStatistics Class
class CEthernetBusStatisticsDlg : public CCommonStatistics
{
    DECLARE_DYNCREATE(CEthernetBusStatisticsDlg)
    // Construction
public:
    enum { IDD = IDD_DLG_NETWORK_STATISTICS_ETHERNET };
    // standard constructor
    CEthernetBusStatisticsDlg():CCommonStatistics(CEthernetBusStatisticsDlg::IDD, LIN)
    {

    }

    CEthernetBusStatisticsDlg(CBaseBusStatisticEthernet* pouBSEth,CWnd* pParent = NULL,int nChannelCount = defNO_OF_LIN_CHANNELS);

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CBusStatisticsDlg)
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
	CBaseBusStatisticEthernet* m_pouBSEthernet;
    HWND m_hMsgWnd;
    // String objects to format bus load related parameters
    CString m_omStrBusLoad;
    CString m_omStrPeakBusLoad;
    CString m_omStrAvgBusLoad;
    /*int m_nChannelCount;*/
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
};
