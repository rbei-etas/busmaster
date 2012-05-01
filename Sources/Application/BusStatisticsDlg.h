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

// information of CBusStatisticsDlg serialization data
struct sBUSSTATTISTICSDATA
{
    UINT m_nColumnCount;
    bool m_arrbColumnVisible[CHANNEL_CAN_MAX+1];
    int  m_arrnOrder[CHANNEL_CAN_MAX+1];
    int  m_arrnColWidth[CHANNEL_CAN_MAX+1];
    WINDOWPLACEMENT    m_sBusStatsDlgCoOrd;      //  CBusStatisticsDlg coords
    WINDOWPLACEMENT    m_sDefaultBusStatsDlgCoOrd;
    /* constructor */
    sBUSSTATTISTICSDATA();
    void vLoadDefaults();

    bool m_bIsDirty;
};

//BusStatistics Class
class CBusStatisticsDlg : public CDialog
{
    // Construction
public:
    // standard constructor
    CBusStatisticsDlg(CBaseBusStatisticCAN*, CWnd* pParent = NULL, int nChannelCount = defNO_OF_CHANNELS);

    static sBUSSTATTISTICSDATA sm_sBusSerializationData;
    HRESULT GetConfigData(BYTE* pvDataStream);
    HRESULT SetConfigData(BYTE* pvDataStream);
    UINT nGetBusStatsDlgConfigSize();
    void vLoadDefaultValues();
    void vLoadDataFromStore();
    static void vSaveDataToStore(BYTE* pvDataStream);
    static void vGetDataFromStore(BYTE** pvDataStream, UINT& nSize);
    static void vSetDefaultsToStore();
    void vUpdateChannelCountInfo(int nChannelCount);

    // Dialog Data
    //{{AFX_DATA(CBusStatisticsDlg)
    enum { IDD = IDD_DLG_NETWORK_STATISTICS };
    CMessageList m_omStatList;
    //}}AFX_DATA


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
    CBaseBusStatisticCAN* m_pouBSCAN;
    // String objects to format bus load related parameters
    CString m_omStrBusLoad;
    CString m_omStrPeakBusLoad;
    CString m_omStrAvgBusLoad;
    int m_nChannelCount;
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
};
