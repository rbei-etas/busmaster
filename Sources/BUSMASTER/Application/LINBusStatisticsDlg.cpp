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
 * \file      BusStatisticsDlg.cpp
 * \brief     This file contain the definition of CBusStatisticsDlg class
 * \author    Shashank Vernekar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition of CBusStatisticsDlg class
 */
// For all standard header file include
#include "stdafx.h"
// Definition of App class
#include "BUSMASTER.h"
// Definition of CBusStatistics class

// For time manager class
#include "Utility\TimeManager.h"
// For DIL Interface Class
#include "Include/BaseDefs.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
//#include "DataTypes/MsgBufAll_Datatypes.h"
//#include "DataTypes/DIL_Datatypes.h"
#include "BaseDIL_LIN.h"

#include "LINBusStatisticsDlg.h"
#include ".\linbusstatisticsdlg.h"
#include "NetworkStatistics.h"
#define  BUS_STATICS_CONFIG_PATH     "//BUSMASTER_CONFIGURATION/Module_Configuration/Bus_Statistics/LIN_Statistics/COLUMN"
/////////////////////////////////////////////////////////////////////////////
// CBusStatisticsDlg dialog

// For application object
extern CCANMonitorApp theApp;
// DIL CAN interface
extern CBaseDIL_LIN* g_pouDIL_LIN_Interface;

const BYTE BUS_STATS_DLG_VERSION = 0x1;

//sLINBUSSTATTISTICSDATA CLINBusStatisticsDlg::sm_sBusSerializationData;

//xmlNodePtr CLINBusStatisticsDlg::m_pxmlNodePtr = nullptr;

CLINBusStatisticsDlg::CLINBusStatisticsDlg(CBaseBusStatisticLIN* pouBSLIN, CWnd* /* pParent */, int nChannelCount)
    : CCommonStatistics(CLINBusStatisticsDlg::IDD, LIN),
      m_pouBSLIN(pouBSLIN),
      m_omStrBusLoad(""),
      m_omStrPeakBusLoad(""),
      m_omStrAvgBusLoad( "" )

{
    //{{AFX_DATA_INIT(CBusStatisticsDlg)
    //}}AFX_DATA_INIT
    m_nChannelCount = nChannelCount;
    for( int nChannel = 0; nChannel < m_nChannelCount; nChannel++ )
    {
        SBUSSTATISTICS_LIN sBusStatistics;
        m_pouBSLIN->BSL_GetBusStatistics(nChannel, sBusStatistics);
    }
}

void CLINBusStatisticsDlg::DoDataExchange(CDataExchange* pDX)
{
    CCommonStatistics::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CBusStatisticsDlg)
    DDX_Control(pDX, IDC_LIST_STAT, m_omStatList);
    //}}AFX_DATA_MAP
}

IMPLEMENT_DYNAMIC(CLINBusStatisticsDlg, CCommonStatistics)
BEGIN_MESSAGE_MAP(CLINBusStatisticsDlg, CCommonStatistics)
    //{{AFX_MSG_MAP(CBusStatisticsDlg)
    ON_WM_ERASEBKGND()
    //}}AFX_MSG_MAP
    ON_MESSAGE(WM_DISPLAY_MESSAGE, vUpdateFields)
    ON_WM_SIZE()
END_MESSAGE_MAP()

LRESULT CLINBusStatisticsDlg::vUpdateFields(WPARAM /* wParam */, LPARAM lParam)
{
    // Update the list control only if the dialog is visible
    BOOL bUpdateSpecific = (BOOL)lParam;
    //  if( IsWindowVisible() == TRUE )
    //   {
    for( int nChannel = 0; nChannel < m_nChannelCount; nChannel++ )
    {
        SBUSSTATISTICS_LIN sBusStatistics;
        m_pouBSLIN->BSL_GetBusStatistics(nChannel, sBusStatistics);
        // Update Window Text
        CString omDispText;
        int nIndex = 0;
        // Message Total
        omDispText.Format( defSTR_FORMAT_UINT_DATA,
                           sBusStatistics.m_unTotalMsgCount );
        m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
        nIndex++;
        if (!bUpdateSpecific) // Do not calculate for disconnected state
        {
            // Total Message Rate
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                               sBusStatistics.m_unMsgPerSecond );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
        }
        nIndex++;
        // Total Errors
        omDispText.Format( defSTR_FORMAT_UINT_DATA,
                           sBusStatistics.m_unErrorTotalCount );
        m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
        nIndex++;
        if (!bUpdateSpecific) // Do not calculate for disconnected state
        {
            // Error Rate
            omDispText.Format( defSTR_FORMAT_FLOAT_DATA,
                               sBusStatistics.m_dErrorRate );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Bus Load
            // Format to show only two decimal digit.
            m_omStrBusLoad.Format( defSTR_FORMAT_BUS_LOAD,
                                   sBusStatistics .m_dBusLoad );
            m_omStatList.SetItemText(nIndex, nChannel + 1, m_omStrBusLoad);
            nIndex++;
            // Peak Load
            m_omStrPeakBusLoad.Format( defSTR_FORMAT_BUS_LOAD,
                                       sBusStatistics.m_dPeakBusLoad );
            m_omStatList.SetItemText(nIndex, nChannel + 1, m_omStrPeakBusLoad);
            nIndex++;
            // Avarage Load
            m_omStrAvgBusLoad.Format( defSTR_FORMAT_BUS_LOAD,
                                      sBusStatistics.m_dAvarageBusLoad );
            m_omStatList.SetItemText(nIndex, nChannel + 1, m_omStrAvgBusLoad);
            nIndex++;
        }
        else
        {
            nIndex += 4; // To accomodate the missed increments because of the above condition
        }
        // For Tx Message Title
        nIndex++;

        // Tx Total
        omDispText.Format( defSTR_FORMAT_UINT_DATA,
                           sBusStatistics.m_unTotalTxMsgCount );
        m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
        nIndex++;

        // Tx Error
        omDispText.Format( defSTR_FORMAT_UINT_DATA,
                           sBusStatistics.m_unErrorTxCount);
        m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
        nIndex++;
        // Tx Error Rate
        if (!bUpdateSpecific) // Do not calculate for disconnected state
        {
            omDispText.Format( defSTR_FORMAT_FLOAT_DATA,
                               sBusStatistics.m_dErrorTxRate );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
        }
        else
        {
            nIndex += 1; // To accomodate the missed increments because of the above condition
        }

        nIndex++;

        // Rx Total
        omDispText.Format( defSTR_FORMAT_UINT_DATA,
                           sBusStatistics.m_unTotalRxMsgCount);
        m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
        nIndex++;

        // Rx Error
        omDispText.Format( defSTR_FORMAT_UINT_DATA,
                           sBusStatistics.m_unErrorRxCount);
        m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
        nIndex++;
        if (!bUpdateSpecific) // Do not calculate for disconnected state
        {
            // Rx Error Rate
            omDispText.Format( defSTR_FORMAT_FLOAT_DATA,
                               sBusStatistics.m_dErrorRxRate );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
        }
        else
        {
            nIndex += 1; // To accomodate the missed increments because of the above condition
        }
        // Increment for the title Status
        nIndex++;

        // Controller status
        switch( sBusStatistics.m_ucStatus )
        {
            case defCONTROLLER_ACTIVE:
            {
                omDispText = _(defSTR_ACTIVE_STATE);
            }
            break;
            case defCONTROLLER_PASSIVE:
            {
                omDispText = _(defSTR_PASSIVE_STATE);
            }
            break;
            case defCONTROLLER_BUSOFF:
            {
                omDispText = _(defSTR_BUSOFF_STATE);
            }
            break;
            default:
            {
                omDispText = _(defSTR_UNKNOWN_STATE);
            }
        }
        // Set controller state
        m_omStatList.SetItemText( nIndex, nChannel + 1, omDispText );
        nIndex++;

        if (!bUpdateSpecific) // Do not calculate for disconnected state
        {
            //wake up rate
            omDispText.Format( defSTR_FORMAT_FLOAT_DATA,
                               (float)(sBusStatistics.m_unTotalWakeUpsRate));
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
        }
        nIndex++;

        //wake up count
        omDispText.Format( defSTR_FORMAT_UINT_DATA,
                           sBusStatistics.m_unTotalWakeUpsCount);
        m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
        nIndex++;
    }
    // }
    return 0;
}

BOOL CLINBusStatisticsDlg::OnInitDialog()
{
    CCommonStatistics::OnInitDialog();

    /* Try to load resource DLL for icons*/
    HMODULE hModAdvancedUILib = ::LoadLibrary("AdvancedUIPlugIn.dll");

    //SSH #issue 438+
    if ( hModAdvancedUILib )
    {
        SetIcon( ::LoadIcon(hModAdvancedUILib, MAKEINTRESOURCE( IDI_ICON_NETWORKSTAT )), TRUE);
        ::FreeLibrary(hModAdvancedUILib);
        hModAdvancedUILib = nullptr;
    }
    else
    {
        SetIcon( AfxGetApp()->LoadIcon(IDI_ICON_NETWORKSTAT), TRUE);
    }

    // Init Item color and Heading color variables
    int nHeadingColor, nItemColor;
    nHeadingColor = defHEADING_COLOR;
    nItemColor = defITEM_COLOR;

    int nIndex = 0;

    // Insert Columns
    m_omStatList.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP );
    m_omStatList.InsertColumn( 0, _(defSTR_COL_PARAMETER) );
    m_omStatList.SetColumnWidth( 0, defPARAMETER_COL_WIDTH );

    for( nIndex = 0; nIndex < m_nChannelCount; nIndex++)
    {
        CString cs;
        cs.Format( defSTR_CHANNEL_NAME_FORMAT, defSTR_CHANNEL_NAME, nIndex + 1);
        m_omStatList.InsertColumn( nIndex + 1, cs, LVCFMT_CENTER );
        m_omStatList.SetColumnWidth( nIndex + 1, defVALUE_COL_WIDTH );
    }

    CRect cr;
    // Get Window Rectangle
    GetWindowRect(&cr);
    // Resize the window.
    // Dialog Width = Width of Parameter Col +
    //                Width of Value * Number of Channels +
    //                Dialog Border offset
    cr.right = cr.left + defPARAMETER_COL_WIDTH + defDIALOG_BORDER_OFFSET +
               ( defVALUE_COL_WIDTH * 2 );
    // Resize the dialog width
    MoveWindow(&cr);
    // Get list boundary
    m_omStatList.GetWindowRect(&cr);
    // Dialog Width = Width of Parameter Col +
    //                Width of Value * Number of Channels +
    //                List Border offset
    cr.right = cr.left + defPARAMETER_COL_WIDTH + defLIST_BORDER_OFFSET +
               ( defVALUE_COL_WIDTH * 2 );
    // Convert to client offset
    ScreenToClient(&cr);
    // Set the boundary for list control
    m_omStatList.MoveWindow(&cr);
    // Insert Rows
    CString omStrInitValue = "";
    // Init Item Index
    nIndex = 0;

    // Total Message count
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_MSG_TOTAL_LIN) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Message Rate
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_MSG_RATE_LIN) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Errors
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_ERR_TOTAL_LIN) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Error Rate
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_ERR_RATE_LIN));
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Network Load
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_LOAD_LIN) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Peak network load
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_PEAK_LOAD_LIN) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Insert Avarage Bus Load
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_AVG_LOAD_LIN) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;

    // "Transmitted' heading
    m_omStatList.InsertItem(nIndex, _(defSTR_TX_HEADING_LIN) );
    m_omStatList.SetItemData(nIndex, nHeadingColor);
    nIndex++;
    // Total Tx Message
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_TX_TOTAL_LIN) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;

    // Total Tx Errors Total
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_TX_ERR_TOTAL_LIN) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Tx Error Rate
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_TX_ERR_RATE_LIN) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;

    // "Received" Heading
    m_omStatList.InsertItem(nIndex, _(defSTR_RX_HEADING_LIN));
    m_omStatList.SetItemData(nIndex, nHeadingColor);
    nIndex++;
    // Total Rx Messages
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_RX_TOTAL_LIN) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Rx Errors
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_RX_ERR_TOTAL_LIN) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Rx Error Rate
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_RX_ERR_RATE_LIN) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;

    // Insert Controller Status Heading
    m_omStatList.InsertItem(nIndex, _(defSTR_STATUS_HEADING_LIN) );
    m_omStatList.SetItemData(nIndex, nHeadingColor);
    nIndex++;

    // Insert Controller status
    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_CONTROLLER_STATUS_LIN) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;

    // Insert Tx Error Counter
    //m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_TX_ERROR_COUNT_LIN) );
    //m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    //m_omStatList.SetItemData(nIndex, nItemColor);
    //nIndex++;

    //// Insert Peak Tx Error Counter
    //m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_PEAK_TX_ERROR_COUNT_LIN) );
    //m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    //m_omStatList.SetItemData(nIndex, nItemColor);
    //nIndex++;

    //// Insert Rx Error counter
    //m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_RX_ERROR_COUNT_LIN) );
    //m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    //m_omStatList.SetItemData(nIndex, nItemColor);
    //nIndex++;

    //// Insert Peak Rx Error Counter
    //m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_PEAK_RX_ERROR_COUNT_LIN) );
    //m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    //m_omStatList.SetItemData(nIndex, nItemColor);
    //nIndex++;

    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_WAKEUPS_COUNT_LIN) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;

    m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_WAKEUPS_RATE_LIN) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;


    /* m_omStatList.InsertItem(nIndex, _(defSTR_PARAMETER_CHIPSTATE_LIN) );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;*/

    GetWindowPlacement(&sm_sBusSerializationData.m_sDefaultBusStatsDlgCoOrd);

    if(sm_sBusSerializationData.m_sBusStatsDlgCoOrd.rcNormalPosition.left == 0)
    {
        sm_sBusSerializationData.m_sBusStatsDlgCoOrd = sm_sBusSerializationData.m_sDefaultBusStatsDlgCoOrd;
    }

    // Set the focus to the list
    m_omStatList.SetFocus();

    hSetConfigData();

    return FALSE;
}

BOOL CLINBusStatisticsDlg::OnEraseBkgnd(CDC* /*pDC*/)
{
    return TRUE;
}


void CLINBusStatisticsDlg::OnSize(UINT nType, int cx, int cy)
{
    CCommonStatistics::OnSize(nType, cx, cy);

    if(m_omStatList.m_hWnd)
    {
        CRect omClientRect;
        GetClientRect(&omClientRect);
        m_omStatList.MoveWindow(&omClientRect);
    }
}
