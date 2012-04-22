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
 * \author    Amitesh Bharti
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
#include "TimeManager.h"
// For DIL Interface Class
#include "Include/BaseDefs.h"
#include "DataTypes/Base_WrapperErrorLogger.h"
#include "DataTypes/MsgBufAll_Datatypes.h"
#include "DataTypes/DIL_Datatypes.h"
#include "DIL_Interface/BaseDIL_CAN.h"

#include "BusStatisticsDlg.h"
#include ".\busstatisticsdlg.h"

/////////////////////////////////////////////////////////////////////////////
// CBusStatisticsDlg dialog
//extern SBUSSTATISTICS g_sBusStatistics[ defNO_OF_CHANNELS];
// For application object
extern CCANMonitorApp theApp;
// DIL CAN interface
extern CBaseDIL_CAN* g_pouDIL_CAN_Interface;

const BYTE BUS_STATS_DLG_VERSION = 0x1;

sBUSSTATTISTICSDATA CBusStatisticsDlg::sm_sBusSerializationData;


/* constructor */
sBUSSTATTISTICSDATA::sBUSSTATTISTICSDATA()
{
	m_bIsDirty = false;
	//One for the first default parameters column
	m_nColumnCount = CHANNEL_CAN_MAX + 1;
}

void sBUSSTATTISTICSDATA::vLoadDefaults()
{
	//For first column 'Parameters'
	m_arrbColumnVisible[0] = true;
	m_arrnOrder[0] = 0;
	m_arrnColWidth[0] = defPARAMETER_COL_WIDTH;


	for(short i = 1; i < CHANNEL_CAN_MAX+1; i++)
	{
		m_arrbColumnVisible[i] = true;
		m_arrnOrder[i] = i;
		m_arrnColWidth[i] = defVALUE_COL_WIDTH;
	}	
	m_sBusStatsDlgCoOrd = m_sDefaultBusStatsDlgCoOrd;
}

/******************************************************************************/
/*  Function Name    :  CBusStatisticsDlg                                     */
/*  Input(s)         :  pParent : Pointer to parent window                    */
/*                      CBaseBusStatisticCAN* Pointer to the BusStatistics    */
/*                      Base class                                            */  
/*  Output           :                                                        */
/*  Functionality    :  This is the constructor of this class. All data member*/
/*                      will be initialised in this class.                    */
/*  Member of        :  CBusStatisticsDlg                                     */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  04.03.2003                                            */
/*  Modifications    :  Raja N on 12.12.2004                                  */
/*                      Removed duplicate initialisation by calling common    */
/*                      init function vClearData                              */
/*  Modifications    :  Raja N on 12.12.2004                                  */
/*                      Removed clearing data as data is moved to the class   */
/*                      CStatistics                                           */
/******************************************************************************/
CBusStatisticsDlg::CBusStatisticsDlg(CBaseBusStatisticCAN* pouBSCAN, CWnd* pParent,int nChannelCount)
    : CDialog(CBusStatisticsDlg::IDD, pParent),
     m_pouBSCAN(pouBSCAN),
     m_omStrBusLoad(STR_EMPTY),
     m_omStrPeakBusLoad(STR_EMPTY),
     m_omStrAvgBusLoad( STR_EMPTY )

{
    //{{AFX_DATA_INIT(CBusStatisticsDlg)
    //}}AFX_DATA_INIT
	m_nChannelCount = nChannelCount;
}

/******************************************************************************/
/*  Function Name    :  DoDataExchange                                        */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Called by the framework to exchange and validate      */
/*                         dialog data                                        */
/*                                                                            */
/*  Member of        :  CBusStatisticsDlg                                     */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  04.03.2003                                            */
/*  Modifications    :  Raja N on 12.12.2004                                  */
/*                      Removed DDX mapping of global statistics members as   */
/*                      UI type got changed to List control from read only    */
/*                      text boxes                                            */
/******************************************************************************/
void CBusStatisticsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CBusStatisticsDlg)
    DDX_Control(pDX, IDC_LIST_STAT, m_omStatList);
    //}}AFX_DATA_MAP
}
    

BEGIN_MESSAGE_MAP(CBusStatisticsDlg, CDialog)
    //{{AFX_MSG_MAP(CBusStatisticsDlg)
    ON_WM_ERASEBKGND()
    //}}AFX_MSG_MAP
    ON_MESSAGE(WM_DISPLAY_MESSAGE, vUpdateFields)
	ON_WM_SIZE()
END_MESSAGE_MAP()

/******************************************************************************/
/*  Function Name    :  vUpdateFields                                         */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  This function will uodate the dialog controls with the*/
/*                      data stored in its corresponding data member. Some    */
/*                      will be calculated before updating.                   */
/*                                                                            */
/*  Member of        :  CBusStatisticsDlg                                     */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  04.03.2003                                            */
/*  Modifications    :  Amitesh Bharti, 13.08.2004                            */
/*                      Bus load and peak bus load is displayed.              */
/*  Modifications    :  Raja N on 12.12.2004, Modified function to compute new*/
/*                      stat parameters and to populate new list control UI.  */
/*                      also added check to update the UI only if the dialog  */
/*                      is visible. Otherwise only do computation             */
/*  Modifications    :  Raja N on 14.03.2005, Removed computation part. Now   */
/*                      CStatistics class will do computation and this function
/*                      will only update UI with the computed values          */
/******************************************************************************/
LRESULT CBusStatisticsDlg::vUpdateFields(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    // Update the list control only if the dialog is visible
    if( IsWindowVisible() == TRUE )
    {
         for( int nChannel = 0; nChannel < m_nChannelCount; nChannel++ )
         {
            SBUSSTATISTICS sBusStatistics;
            m_pouBSCAN->BSC_GetBusStatistics(nChannel, sBusStatistics);
            // Update Window Text
            CString omDispText;
            int nIndex = 0;
            // Message Total
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                sBusStatistics.m_unTotalMsgCount );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Total Message Rate
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                sBusStatistics.m_unMsgPerSecond );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Total Errors
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                sBusStatistics.m_unErrorTotalCount );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
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

            // For Tx Message Title
            nIndex++;

            // Tx Total
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                sBusStatistics.m_unTotalTxMsgCount );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Tx Std Msg
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                sBusStatistics.m_unTxSTDMsgCount);
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Tx Std Msg Rate
            omDispText.Format( defSTR_FORMAT_FLOAT_DATA,
                sBusStatistics.m_dTxSTDMsgRate );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Tx Exd Msg
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                sBusStatistics.m_unTxEXTDMsgCount);
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Tx Exd Msg Rate
            omDispText.Format( defSTR_FORMAT_FLOAT_DATA,
                sBusStatistics.m_dTxEXTMsgRate );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Std. RTR
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                sBusStatistics.m_unTxSTD_RTRMsgCount);
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Exd. RTR
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                sBusStatistics.m_unTxEXTD_RTRMsgCount);
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Tx Error
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                sBusStatistics.m_unErrorTxCount);
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Tx Error Rate
            omDispText.Format( defSTR_FORMAT_FLOAT_DATA,
                sBusStatistics.m_dErrorTxRate );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;

            nIndex++;

            // Rx Total
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                sBusStatistics.m_unTotalRxMsgCount);
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Rx Std Msg
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                sBusStatistics.m_unRxSTDMsgCount);
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Rx Std Msg Rate
            omDispText.Format( defSTR_FORMAT_FLOAT_DATA,
                sBusStatistics.m_dRxSTDMsgRate );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Rx Exd Msg
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                sBusStatistics.m_unRxEXTDMsgCount);
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Rx Exd Msg Rate
            omDispText.Format( defSTR_FORMAT_FLOAT_DATA,
                sBusStatistics.m_dRxEXTMsgRate );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Std. RTR
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                sBusStatistics.m_unRxSTD_RTRMsgCount);
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Exd. RTR
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                sBusStatistics.m_unRxEXTD_RTRMsgCount);
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Rx Error
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                sBusStatistics.m_unErrorRxCount);
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Rx Error Rate
            omDispText.Format( defSTR_FORMAT_FLOAT_DATA,
                sBusStatistics.m_dErrorRxRate );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
            // Increment for the title Status
            nIndex++;

            // Controller status
            switch( sBusStatistics.m_ucStatus )
            {
            case defCONTROLLER_ACTIVE:
                {
                    omDispText = defSTR_ACTIVE_STATE;
                }
                break;
            case defCONTROLLER_PASSIVE:
                {
                    omDispText = defSTR_PASSIVE_STATE;
                }
                break;
            case defCONTROLLER_BUSOFF:
                {
                    omDispText = defSTR_BUSOFF_STATE;
                }
                break;
            default:
                {
                    omDispText = defSTR_UNKNOWN_STATE;
                }
            }
            // Set controller state
            m_omStatList.SetItemText( nIndex, nChannel + 1, omDispText );
            nIndex++;
            // Set Tx Error Counter value
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                sBusStatistics.m_ucTxErrorCounter );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;

            // Set Tx Peak Error Counter value
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                sBusStatistics.m_ucTxPeakErrorCount );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;

            // Set Rx Error Counter value
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                sBusStatistics.m_ucRxErrorCounter );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;

            // Set Rx Peak Error Counter value
            omDispText.Format( defSTR_FORMAT_UINT_DATA,
                sBusStatistics.m_ucRxPeakErrorCount );
            m_omStatList.SetItemText(nIndex, nChannel + 1, omDispText);
            nIndex++;
        }
    }
	return 0;
}

/*******************************************************************************
  Function Name  : OnInitDialog
  Input(s)       : -
  Output         : FALSE - If focus is set to Any UI control explicitly
  Functionality  : Initialises dialog's UI components
  Member of      : CBusStatisticsDlg
  Author(s)      : Raja N
  Date Created   : 12/12/2004
  Modifications  : Raja N on 14.03.2005
                   Modified init to support multi channel information and added
                   rows for new statistics parameters
*******************************************************************************/
BOOL CBusStatisticsDlg::OnInitDialog() 
{
    CDialog::OnInitDialog();
    // Init Item color and Heading color variables
    int nHeadingColor, nItemColor;
    nHeadingColor = defHEADING_COLOR;
    nItemColor = defITEM_COLOR;

    int nIndex = 0;
    
    // Insert Columns
    m_omStatList.SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP );
    m_omStatList.InsertColumn( 0, defSTR_COL_PARAMETER );
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
    CString omStrInitValue = STR_EMPTY;
    // Init Item Index
    nIndex = 0;

    // Total Message count
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_MSG_TOTAL );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Message Rate
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_MSG_RATE );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Errors
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_ERR_TOTAL );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Error Rate
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_ERR_RATE);
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Network Load
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_LOAD );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Peak network load
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_PEAK_LOAD );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Insert Avarage Bus Load
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_AVG_LOAD );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;

    // "Transmitted' heading
    m_omStatList.InsertItem(nIndex, defSTR_TX_HEADING );
    m_omStatList.SetItemData(nIndex, nHeadingColor);
    nIndex++;
    // Total Tx Message
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_TX_TOTAL );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Tx Std Message
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_TX_STD_TOTAL );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Tx Std Message Rate
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_TX_STD_RATE );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Tx Extended Message
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_TX_EXD_TOTAL );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Tx Extended Message Rate
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_TX_EXD_RATE );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Tx stanard RTR Message
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_TX_STD_RTR );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Tx Extended RTR Message
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_TX_EXD_RTR );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Tx Errors Total
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_TX_ERR_TOTAL );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Tx Error Rate
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_TX_ERR_RATE );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;

    // "Received" Heading
    m_omStatList.InsertItem(nIndex, defSTR_RX_HEADING);
    m_omStatList.SetItemData(nIndex, nHeadingColor);
    nIndex++;
    // Total Rx Messages
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_RX_TOTAL );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Rx Standard Messages
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_RX_STD_TOTAL );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Rx Standard Message Rate
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_RX_STD_RATE );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Rx Extended Messages
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_RX_EXD_TOTAL );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Rx Extended Message Rate
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_RX_EXD_RATE );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Rx Standard RTR Messages
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_RX_STD_RTR );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Rx Extended RTR Messages
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_RX_EXD_RTR );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Total Rx Errors
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_RX_ERR_TOTAL );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;
    // Rx Error Rate
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_RX_ERR_RATE );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;

    // Insert Controller Status Heading
    m_omStatList.InsertItem(nIndex, defSTR_STATUS_HEADING );
    m_omStatList.SetItemData(nIndex, nHeadingColor);
    nIndex++;

    // Insert Controller status
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_CONTROLLER_STATUS );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;

    // Insert Tx Error Counter
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_TX_ERROR_COUNT );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;

    // Insert Peak Tx Error Counter
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_PEAK_TX_ERROR_COUNT );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;

    // Insert Rx Error counter
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_RX_ERROR_COUNT );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;

    // Insert Peak Rx Error Counter
    m_omStatList.InsertItem(nIndex, defSTR_PARAMETER_PEAK_RX_ERROR_COUNT );
    m_omStatList.SetItemText(nIndex, 1, omStrInitValue);
    m_omStatList.SetItemData(nIndex, nItemColor);
    nIndex++;

	GetWindowPlacement(&sm_sBusSerializationData.m_sDefaultBusStatsDlgCoOrd);
	if(sm_sBusSerializationData.m_sBusStatsDlgCoOrd.rcNormalPosition.left == 0)
		sm_sBusSerializationData.m_sBusStatsDlgCoOrd = sm_sBusSerializationData.m_sDefaultBusStatsDlgCoOrd;
	
    // Set the focus to the list
    m_omStatList.SetFocus();
    return FALSE;
}

/*******************************************************************************
  Function Name  : OnEraseBkgnd
  Input(s)       : pDC - Device context to the Screen
  Output         : -
  Functionality  : This will return simply as the whole dialog area is occupied
                   by the list control. This will avoid flickering
  Member of      : CBusStatisticsDlg
  Author(s)      : Raja N
  Date Created   : 14.3.2005
  Modifications  : 
*******************************************************************************/
BOOL CBusStatisticsDlg::OnEraseBkgnd(CDC* /*pDC*/) 
{
    return TRUE;
}


/*******************************************************************************
  Function Name  : OnSize
  Input(s)       : UINT nType, int cx, int cy
  Output         : -
  Functionality  : This function Makes the List control to the same size as 
				   that of dialog whenever dialog window is resized.
  Member of      : CBusStatisticsDlg
  Author(s)      : ArunKumar K
  Date Created   : 24.3.2011
  Modifications  : 
*******************************************************************************/
void CBusStatisticsDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(m_omStatList.m_hWnd)
	{
		CRect omClientRect;
        	GetClientRect(&omClientRect);
        	m_omStatList.MoveWindow(&omClientRect);
	}
}


/*******************************************************************************
  Function Name  : GetConfigData
  Input(s)       : ppvDataStream, unLength
  Output         : -
  Functionality  : Returns the Bus stats dialog Configuration in Byte Array.
  Member of      : CBusStatisticsDlg
  Author(s)      : Arunkumar K
  Date Created   : 06-04-2011
  Modifications  : 
*******************************************************************************/
HRESULT CBusStatisticsDlg::GetConfigData(BYTE* pvDataStream) 
{
	BYTE* pByteTrgt = NULL;	
	pByteTrgt = pvDataStream;	
	
	BYTE byVer = BUS_STATS_DLG_VERSION;
	COPY_DATA(pByteTrgt, &byVer , sizeof(BYTE)); //Setting Version.

	UINT nSize= nGetBusStatsDlgConfigSize();
	COPY_DATA(pByteTrgt, &nSize, sizeof(UINT)); //Setting Buffer Size. 
	
	CHeaderCtrl* pHeaderCtrl = m_omStatList.GetHeaderCtrl();
	if (pHeaderCtrl != NULL)	
	{
		int  nColumnCount = pHeaderCtrl->GetItemCount();
		COPY_DATA(pByteTrgt, &nColumnCount, sizeof(UINT)); //Storing column count.

		LPINT pnOrder = (LPINT) malloc(nColumnCount*sizeof(int));
        
		ASSERT(pnOrder != NULL);
		m_omStatList.GetColumnOrderArray(pnOrder, nColumnCount);
    
		for (int i = 0 ; i < nColumnCount; i++)
        {
			//Storing Column Header Positions.
			COPY_DATA(pByteTrgt, &pnOrder[i], sizeof(int)); 				
		
            //Storing Column Header show/Hide.
			bool bColumnVisible = m_omStatList.IsColumnShown(i);
		    COPY_DATA(pByteTrgt, &bColumnVisible, sizeof(bool));
            
			//Storing Column width.
			INT nColWidth = m_omStatList.GetColumnWidth(i);
            COPY_DATA(pByteTrgt, &nColWidth, sizeof(int));

	    }
        free(pnOrder);
        
	}
	GetWindowPlacement(&sm_sBusSerializationData.m_sBusStatsDlgCoOrd);
	COPY_DATA(pByteTrgt, &sm_sBusSerializationData.m_sBusStatsDlgCoOrd, sizeof(WINDOWPLACEMENT));

	return S_OK;
}

/*******************************************************************************
  Function Name  : SetConfigData
  Input(s)       : pvDataStream
  Output         : -
  Functionality  : Sets the Bus stats dialog Configuration from Byte Array.
  Member of      : CBusStatisticsDlg
  Author(s)      : Arunkumar K
  Date Created   : 25-08-2010
  Modifications  :  
*******************************************************************************/
HRESULT CBusStatisticsDlg::SetConfigData(BYTE* pvDataStream)
{
	BYTE* pByteSrc = NULL;	
	pByteSrc = pvDataStream;

	if (pByteSrc != NULL)
	{
		//Reading Version.
		BYTE byVer = 0;
		COPY_DATA_2(&byVer, pByteSrc, sizeof(BYTE));
        UINT nSize= 0;
        //Reading Buffer Size. 
		COPY_DATA_2(&nSize, pByteSrc, sizeof(UINT));
		if ((byVer == BUS_STATS_DLG_VERSION) && (nSize > 0))
		{
			//Reading column Header Positions.
			CHeaderCtrl* pHeaderCtrl = m_omStatList.GetHeaderCtrl();
			if (pHeaderCtrl != NULL)	
			{	
				int  nColumnCount=0;
				//Reading column count.
				COPY_DATA_2(&nColumnCount, pByteSrc, sizeof(UINT));

				//Saved channel count is same as current channel count.
				if(nColumnCount==m_nChannelCount+1)
				{
					LPINT pnOrder = (LPINT) malloc(nColumnCount*sizeof(int));
					for (int i = 0 ; i < nColumnCount; i++)	
					{
						COPY_DATA_2(&pnOrder[i], pByteSrc, sizeof(int));
						bool bColumnVisible = false;
						//Reading visibility
						COPY_DATA_2(&bColumnVisible, pByteSrc, sizeof(bool));			
						m_omStatList.MakeColumnVisible(i, bColumnVisible);

						INT nColWidth = 0;
						//Reading width
						COPY_DATA_2(&nColWidth, pByteSrc, sizeof(int));
						m_omStatList.SetColumnWidth(i, nColWidth);
					}
					m_omStatList.SetColumnOrderArray(nColumnCount, pnOrder);
					free(pnOrder);
				}
				else //Saved channel count differs from current channel count.
					vLoadDefaultValues();

			}
            WINDOWPLACEMENT sMsgWndPlacement;

            COPY_DATA_2(&sMsgWndPlacement, pByteSrc, sizeof(WINDOWPLACEMENT));
            SetWindowPlacement(&sMsgWndPlacement);			
		}
	}
	return S_OK;
}

/*******************************************************************************
  Function Name  : vLoadDefaultValues
  Input(s)       : -
  Output         : -
  Functionality  : Loads the Bus stats dialog Configuration defaults.
  Member of      : CBusStatisticsDlg
  Author(s)      : Arunkumar K
  Date Created   : 07-04-2011
  Modifications  :  
*******************************************************************************/
void CBusStatisticsDlg::vLoadDefaultValues()
{
	sm_sBusSerializationData.vLoadDefaults();

	for (int i = 0 ; i < m_nChannelCount+1; i++)	
	{
		m_omStatList.MakeColumnVisible(i, sm_sBusSerializationData.m_arrbColumnVisible[i]);
		m_omStatList.SetColumnWidth(i, sm_sBusSerializationData.m_arrnColWidth[i]);
	}
	m_omStatList.SetColumnOrderArray(m_nChannelCount+1, (LPINT)sm_sBusSerializationData.m_arrnOrder);		
}

/*******************************************************************************
  Function Name  : vSetDefaultsToStore
  Input(s)       : -
  Output         : -
  Functionality  : Loads the Bus stats dialog default values to store.
  Member of      : CBusStatisticsDlg
  Author(s)      : Arunkumar K
  Date Created   : 07-04-2011
  Modifications  :  
*******************************************************************************/
void CBusStatisticsDlg::vSetDefaultsToStore()
{
	sm_sBusSerializationData.vLoadDefaults();
	sm_sBusSerializationData.m_bIsDirty = true;
}

/*******************************************************************************
  Function Name  : vLoadDataFromStore
  Input(s)       : -
  Output         : -
  Functionality  : Loads the Bus stats dialog Configuration date from store.
  Member of      : CBusStatisticsDlg
  Author(s)      : Arunkumar K
  Date Created   : 07-04-2011
  Modifications  : ArunKumar K, 08-04-2011,
				   Updated for loop MAX index to m_nChannelCount+1 instead of
				   sm_sBusSerializationData.m_nColumnCount.
*******************************************************************************/
void CBusStatisticsDlg::vLoadDataFromStore()
{
	if(sm_sBusSerializationData.m_bIsDirty)
	{
		SetWindowPlacement(&sm_sBusSerializationData.m_sBusStatsDlgCoOrd);
		//Reading column Header Positions.
		CHeaderCtrl* pHeaderCtrl = m_omStatList.GetHeaderCtrl();
		if (pHeaderCtrl != NULL)	
		{	
			if(sm_sBusSerializationData.m_nColumnCount != (UINT)(m_nChannelCount + 1))
			{
				sm_sBusSerializationData.vLoadDefaults();
			}			
			for (int i = 0 ; i < m_nChannelCount+1 ; i++)	
			{
				m_omStatList.MakeColumnVisible(i, sm_sBusSerializationData.m_arrbColumnVisible[i]);
				m_omStatList.SetColumnWidth(i, sm_sBusSerializationData.m_arrnColWidth[i]);
			}
			m_omStatList.SetColumnOrderArray(sm_sBusSerializationData.m_nColumnCount , (LPINT)sm_sBusSerializationData.m_arrnOrder);		
		}		
	}
}

/*******************************************************************************
  Function Name  : vUpdateChannelCountInfo
  Input(s)       : nChannelCount
  Output         : -
  Functionality  : Updates the channel count value when a hardware interface 
                   is updated.
  Member of      : CBusStatisticsDlg
  Author(s)      : Arunkumar K
  Date Created   : 07-04-2011
  Modifications  :  
*******************************************************************************/
void CBusStatisticsDlg::vUpdateChannelCountInfo(int nChannelCount)
{
	if(m_nChannelCount != nChannelCount)
	{
		if(m_nChannelCount > nChannelCount)
		{
			INT nChannelIndex = nChannelCount+1;
			for(int i = nChannelCount+1; i <= m_nChannelCount; i++)
			{
				m_omStatList.DeleteColumn(nChannelIndex);					
			}
		}
		else
		{
			for(int i = m_nChannelCount+1; i <= nChannelCount; i++)
			{
				CString cs;
				cs.Format( defSTR_CHANNEL_NAME_FORMAT, defSTR_CHANNEL_NAME, i);
				m_omStatList.InsertColumn( i , cs, LVCFMT_CENTER );
				m_omStatList.SetColumnWidth( i , defVALUE_COL_WIDTH );
			}
		}

		m_nChannelCount = nChannelCount;
	}
}

/*******************************************************************************
  Function Name  : vSaveDataToStore
  Input(s)       : pvDataStream
  Output         : -
  Functionality  : Save the Bus stats dialog Configuration date to store.
  Member of      : CBusStatisticsDlg
  Author(s)      : Arunkumar K
  Date Created   : 25-08-2010
  Modifications  :  
*******************************************************************************/
void CBusStatisticsDlg::vSaveDataToStore(BYTE* pvDataStream)
{
	BYTE* pByteSrc = NULL;	
	pByteSrc = pvDataStream;

	if (pByteSrc != NULL)
	{
		//Reading Version.
		BYTE byVer = 0;
		COPY_DATA_2(&byVer, pByteSrc, sizeof(BYTE));
        UINT nSize= 0;
        //Reading Buffer Size. 
		COPY_DATA_2(&nSize, pByteSrc, sizeof(UINT));
		if ((byVer == BUS_STATS_DLG_VERSION) && (nSize > 0))
		{
			int  nColumnCount=0;
			//Reading column count.
			COPY_DATA_2(&nColumnCount, pByteSrc, sizeof(UINT));
			sm_sBusSerializationData.m_nColumnCount = nColumnCount;

			LPINT pnOrder = (LPINT) sm_sBusSerializationData.m_arrnOrder;
			for (int i = 0 ; i < nColumnCount; i++)	
            {
				COPY_DATA_2(&pnOrder[i], pByteSrc, sizeof(int));
				//Reading visibility
				COPY_DATA_2(&sm_sBusSerializationData.m_arrbColumnVisible[i], pByteSrc, sizeof(bool));							

                //int nColWidth = 0;
				//Reading width
				COPY_DATA_2(&sm_sBusSerializationData.m_arrnColWidth[i], pByteSrc, sizeof(int));                
			}          

			COPY_DATA_2(&sm_sBusSerializationData.m_sBusStatsDlgCoOrd, pByteSrc, sizeof(WINDOWPLACEMENT));		
		}
		//Mark as Dirty
		sm_sBusSerializationData.m_bIsDirty = true; 
	}	
}

/*******************************************************************************
  Function Name  : vGetDataFromStore
  Input(s)       : -
  Output         : -
  Functionality  : Gets the data from store for comparision.
  Member of      : CBusStatisticsDlg
  Author(s)      : Arunkumar K
  Date Created   : 06-04-2011
  Modifications  : 
*******************************************************************************/
void CBusStatisticsDlg::vGetDataFromStore(BYTE** pvDataStream, UINT& nSize)
{
	if(sm_sBusSerializationData.m_bIsDirty == true)
	{		
		nSize += sizeof(BYTE);  // 1 Byte for Version
		nSize += sizeof(UINT);  // Bytes for Buffer Size
		//Bytes for column Header Positions.
		nSize += sizeof(UINT); //to store the column count	
		UINT unColCount = sm_sBusSerializationData.m_nColumnCount;
		nSize += (sizeof(int) * unColCount); //Column order
		nSize += (sizeof(bool) * unColCount);//Column visibility
		nSize += (sizeof(int) * unColCount); //Column width    

		nSize += sizeof(WINDOWPLACEMENT);//sizeof Bus Statistics dialog placement

		*pvDataStream = new BYTE[nSize]; 

		BYTE* pByteTrgt = NULL;	
		pByteTrgt = *pvDataStream;

		if (pByteTrgt != NULL)
		{			
			BYTE byVer = BUS_STATS_DLG_VERSION;
			COPY_DATA(pByteTrgt, &byVer , sizeof(BYTE)); //Setting Version.		
			COPY_DATA(pByteTrgt, &nSize, sizeof(UINT)); //Setting Buffer Size. 
			COPY_DATA(pByteTrgt, &unColCount, sizeof(UINT)); //Storing column count.
	    
			for (UINT i = 0 ; i < unColCount; i++)
			{
				//Storing Column Header Positions.
				COPY_DATA(pByteTrgt, &sm_sBusSerializationData.m_arrnOrder[i], sizeof(int)); 							
				//Storing Column Header show/Hide.
				COPY_DATA(pByteTrgt, &sm_sBusSerializationData.m_arrbColumnVisible[i], sizeof(bool));	            
				//Storing Column width.
				COPY_DATA(pByteTrgt, &sm_sBusSerializationData.m_arrnColWidth[i], sizeof(int));
			}
			COPY_DATA(pByteTrgt, &sm_sBusSerializationData.m_sBusStatsDlgCoOrd, sizeof(WINDOWPLACEMENT));
		}	
	}
}


/*******************************************************************************
  Function Name  : nGetBusStatsDlgConfigSize
  Input(s)       : -
  Output         : -
  Functionality  : Gets the Bus Statistics dialog Configuration size as UINT.
  Member of      : CBusStatisticsDlg
  Author(s)      : Arunkumar K
  Date Created   : 06-04-2011
  Modifications  : 
*******************************************************************************/
UINT CBusStatisticsDlg::nGetBusStatsDlgConfigSize()
{
	/*
	1. 1 Byte for Version
	2. Bytes for Buffer Size
	3. Bytes to store the column count
	4. Column Header Positions.   : ITEM COUNT
	5. Column Header Show/Hide. : ITEM COUNT
	6. Column Width: ITEM COUNT
	*/
	UINT nSize = 0;
	nSize += sizeof(BYTE);  // 1 Byte for Version
	nSize += sizeof(UINT);  // Bytes for Buffer Size

	//Bytes for column Header Positions.
    nSize += sizeof(UINT); //to store the column count
	CHeaderCtrl* pHeaderCtrl = m_omStatList.GetHeaderCtrl();
	if (pHeaderCtrl != NULL)
    {
        UINT unColCount = pHeaderCtrl->GetItemCount();
		nSize += (sizeof(int) * unColCount); //Column order
        nSize += (sizeof(bool) * unColCount);//Column visibility
        nSize += (sizeof(int) * unColCount); //Column width
    }

    nSize += sizeof(WINDOWPLACEMENT);//sizeof Bus Statistics dialog placement

	return nSize;
}


