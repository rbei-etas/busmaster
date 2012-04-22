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
 * \file      GraphConfiguration.cpp
 * \brief     Implementation file for CGraphConfiguration class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CGraphConfiguration class
 */

#include "Stdafx.h"
// For App declaration
#include "BUSMASTER.h"
// For Resource symbol definition
#include "Resource.h"
// For Main Frame definition
#include "MainFrm.h"
// For Color Picker class interface
#include "Utility/ColourPicker.h"
// For Graph Configuration class interface
#include "GraphConfiguration.h"

// App declaration
extern CCANMonitorApp theApp;

/*******************************************************************************
  Function Name  : CGraphConfiguration
  Description    : Standard default constructor
  Member of      : CGraphConfiguration
  Functionality  : This will initialise local variables
  Author(s)      : Raja N
  Date Created   : 01/12/2004
  Modifications  :
*******************************************************************************/
CGraphConfiguration::CGraphConfiguration(CWnd* pParent /*=NULL*/)
    : CDialog(CGraphConfiguration::IDD, pParent)
	, m_nLineDisplay(0)
{
    //{{AFX_DATA_INIT(CGraphConfiguration)
    m_nYGridCount = 0;
    m_nXGridCount = 0;
    m_nUpdateRate = 0;
    m_nBufferSize = 0;
    m_omFrameType = -1;
    //}}AFX_DATA_INIT
	m_eBusType = CAN;	
}

/*******************************************************************************
  Function Name  : 
  Input(s)       : pDX - Data Exchange Object
  Output         : -
  Functionality  : 
  Member of      : CGraphConfiguration
  Author         :  Raja N
  Date Created   :  01/12/2004
  Modifications  :
*******************************************************************************/
void CGraphConfiguration::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGraphConfiguration)
	DDX_Control(pDX, IDC_AXIS_COLOR, m_omAxisColor);
	DDX_Control(pDX, IDC_FRAME_COLOR, m_omFrameColor);
	DDX_Control(pDX, IDC_GRID_COLOR, m_omGridColor);
	DDX_Control(pDX, IDC_PLOT_COLOR, m_omPlotColor);
	DDX_Text(pDX, IDC_EDIT_Y_GRIDS, m_nYGridCount);
	DDV_MinMaxInt(pDX, m_nYGridCount, 2, 10);
	DDX_Text(pDX, IDC_EDIT_X_GRIDS, m_nXGridCount);
	DDV_MinMaxInt(pDX, m_nXGridCount, 2, 10);
	DDX_Text(pDX, IDC_EDIT_DISPLAY_UPDATE, m_nUpdateRate);
	DDV_MinMaxInt(pDX, m_nUpdateRate, 1000, 20000);
	DDX_Text(pDX, IDC_EDIT_BUFFER_SIZE, m_nBufferSize);
	DDV_MinMaxInt(pDX, m_nBufferSize, 1000, 50000);
	DDX_CBIndex(pDX, IDC_COMBO_FRAME, m_omFrameType);
	//}}AFX_DATA_MAP
	DDX_CBIndex(pDX, IDC_COMBO_LINE_DISPLAY, m_nLineDisplay);
}


BEGIN_MESSAGE_MAP(CGraphConfiguration, CDialog)
    //{{AFX_MSG_MAP(CGraphConfiguration)
    ON_BN_CLICKED(IDC_CBTN_SET_DEFAULT, OnSetDefault)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGraphConfiguration message handlers

/*******************************************************************************
 Function Name  : OnSetDefault
 Input(s)       : -
 Output         : -
 Functionality  : This function will set all parameters to default value.
 Member of      : CGraphConfiguration
 Author(s)      : Raja N
 Date Created   : 11.29.2004
 Modifications  : 
*******************************************************************************/
void CGraphConfiguration::OnSetDefault() 
{
    // Create Temp object to get default value
    CGraphParameters omTemp;
    // Set the detault values
    vSetValues( omTemp );
}

/*******************************************************************************
 Function Name  : OnOK
 Input(s)       : -
 Output         : -
 Functionality  : This function will Update global structure with the user
                  selected values.
 Member of      : CGraphConfiguration
 Author(s)      : Raja N
 Date Created   : 11.29.2004
 Modifications  : 
*******************************************************************************/
void CGraphConfiguration::OnOK() 
{	
    // Update Global Structure
    CMainFrame *pMainFrame = (CMainFrame *)theApp.m_pMainWnd;
    if( pMainFrame != NULL )
    {
        CGraphList *pList = &(pMainFrame->m_odGraphList[m_eBusType]);
        if( pList != NULL && UpdateData() == TRUE )
        {
            pList->m_odGraphParameters.m_nBufferSize = m_nBufferSize;
            pList->m_odGraphParameters.m_nRefreshRate = m_nUpdateRate;
            pList->m_odGraphParameters.m_nXGridLines = m_nXGridCount;
            pList->m_odGraphParameters.m_nYGridLines = m_nYGridCount;
            pList->m_odGraphParameters.m_nAxisColor = m_omAxisColor.GetColour();
            pList->m_odGraphParameters.m_nFrameColor =
                                                    m_omFrameColor.GetColour();
            pList->m_odGraphParameters.m_nGridColor = m_omGridColor.GetColour();
            pList->m_odGraphParameters.m_nPlotAreaColor =
                                                    m_omPlotColor.GetColour();
            pList->m_odGraphParameters.m_nFrameStyle = m_omFrameType;
			pList->m_odGraphParameters.m_eDisplayType = (eDISPLAY_TYPE)m_nLineDisplay;
	
            // Call parent's OnOK to close this Modal session
            CDialog::OnOK();
        }
    }
}

/*******************************************************************************
  Function Name  : OnInitDialog
  Input(s)       : -
  Output         : FALSE - If focus is set to Any UI control explicitly
  Functionality  : Initialises dialog's UI components
  Member of      : CGraphConfiguration
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : Raja N on 7.3.2005
                   Removed the Automatic tab from the color popup windows
*******************************************************************************/
BOOL CGraphConfiguration::OnInitDialog() 
{
    CDialog::OnInitDialog();	
    // Update Data from Global Structure
    CMainFrame *pMainFrame = (CMainFrame *)theApp.m_pMainWnd;
    if( pMainFrame != NULL )
    {
        CGraphList *pList = &(pMainFrame->m_odGraphList[m_eBusType]);
        if( pList != NULL )
        {
            vSetValues( pList->m_odGraphParameters );
        }
    }	
    // Remove default color option from the buttons
    m_omFrameColor.SetDefaultText( STR_EMPTY );
    m_omPlotColor.SetDefaultText( STR_EMPTY );
    m_omAxisColor.SetDefaultText( STR_EMPTY );
    m_omGridColor.SetDefaultText( STR_EMPTY );

    return TRUE;
}

/*******************************************************************************
  Function Name  : vSetValues
  Input(s)       : omParam - View Style Object
  Output         : -
  Functionality  : This will set the view style parameters to the passed
                   object's value
  Member of      : CGraphConfiguration
  Author(s)      : Raja N
  Date Created   : 10/12/2004
  Modifications  : 
*******************************************************************************/
void CGraphConfiguration::vSetValues(CGraphParameters omParam)
{
    // Set the values
    // buffer size
    m_nBufferSize = omParam.m_nBufferSize;
    // Refresh Rate
    m_nUpdateRate = omParam.m_nRefreshRate;
    // X & Y Grid Lines
    m_nXGridCount = omParam.m_nXGridLines;
    m_nYGridCount = omParam.m_nYGridLines;
    // Colors
        // Axis
    m_omAxisColor.SetColour( omParam.m_nAxisColor );
        // Frame
    m_omFrameColor.SetColour( omParam.m_nFrameColor );
        // Grid
    m_omGridColor.SetColour( omParam.m_nGridColor );
        // Plot Area
    m_omPlotColor.SetColour( omParam.m_nPlotAreaColor );
    // Frame Style
    m_omFrameType = omParam.m_nFrameStyle;
	//Line Display Type
	m_nLineDisplay = omParam.m_eDisplayType;
    // Update data to reflect changes in UI
    UpdateData( FALSE );
}