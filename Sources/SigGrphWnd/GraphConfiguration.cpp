/*******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  GraphConfiguration.cpp
  Description   :  Implementation file for CGraphConfiguration class
  $Log:   X:/Archive/Sources/SigGrphWnd/GraphConfiguration.cpv  $
   
      Rev 1.2   06 Jun 2011 11:51:38   CANMNTTM
    
   
      Rev 1.1   15 Apr 2011 19:43:30   rac2kor
   Inserted RBEI Copyright information text into the file header.
   
      Rev 1.0   13 Dec 2010 22:00:34   CANMNTTM
    
   
      Rev 1.0   16 Aug 2010 21:20:32   rac2kor
    

  Author(s)     :  Raja N
  Date Created  :  01/12/2004
  Modified By   : 
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved
*******************************************************************************/

#include "SigGrphWnd_stdafx.h"
// For Resource symbol definition
#include "SigGrphWnd_Resource.h"
// For Color Picker class interface
#include "Utility/ColourPicker.h"
// For Graph Configuration class interface
#include "GraphConfiguration.h"
#include "SigGrphWnd_Defines.h"

//SGW Code commented by Arun 21-10-2010
// App declaration
//extern CCANMonitorApp theApp;
//SGW Code commented by Arun 21-10-2010

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
	m_pGraphList = NULL;
	m_pDMGraphCtrl = NULL;
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
    DDV_MinMaxInt(pDX, m_nBufferSize, 1000, 10000);
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
    if( m_pGraphList != NULL && UpdateData() == TRUE)
    {                        
        m_pGraphList->m_odGraphParameters.m_nBufferSize = m_nBufferSize;
        m_pGraphList->m_odGraphParameters.m_nRefreshRate = m_nUpdateRate;
        m_pGraphList->m_odGraphParameters.m_nXGridLines = m_nXGridCount;
        m_pGraphList->m_odGraphParameters.m_nYGridLines = m_nYGridCount;
        m_pGraphList->m_odGraphParameters.m_nAxisColor = m_omAxisColor.GetColour();
        m_pGraphList->m_odGraphParameters.m_nFrameColor =
                                                m_omFrameColor.GetColour();
        m_pGraphList->m_odGraphParameters.m_nGridColor = m_omGridColor.GetColour();
        m_pGraphList->m_odGraphParameters.m_nPlotAreaColor =
                                                m_omPlotColor.GetColour();
        m_pGraphList->m_odGraphParameters.m_nFrameStyle = m_omFrameType;
		m_pGraphList->m_odGraphParameters.m_eDisplayType = (eDISPLAY_TYPE)m_nLineDisplay;

		// Update Graph Control        
        if( m_pDMGraphCtrl != NULL )
        {
			// Buffer size and Refresh rate will be taken care
			// during connect
			// Set X Y Gridlines
			CComPtr<IDMGraphAxis> spAxisX; 
			m_pDMGraphCtrl->get_Axis( HorizontalX, &spAxisX);
			spAxisX->put_GridNumber((short)m_nXGridCount);

			CComPtr<IDMGraphAxis> spAxisY; 
			m_pDMGraphCtrl->get_Axis( VerticalY, &spAxisY);
			spAxisY->put_GridNumber((short)m_nYGridCount);				
			// Set Axis Color
			m_pDMGraphCtrl->put_AxisColor((OLE_COLOR)m_omAxisColor.GetColour());					
			// Set Frame Color
			m_pDMGraphCtrl->put_ControlFrameColor((OLE_COLOR)m_omFrameColor.GetColour());					
			// Set Frame Style
			m_pDMGraphCtrl->put_FrameStyle((FrameStyle)m_omFrameType);				
			// Set Grid Color
			m_pDMGraphCtrl->put_GridColor((OLE_COLOR)m_omGridColor.GetColour());				
			// Set Plot Area Color
			m_pDMGraphCtrl->put_PlotAreaColor((OLE_COLOR)m_omPlotColor.GetColour());								
        }

        // Call parent's OnOK to close this Modal session
        CDialog::OnOK();        
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
    if( m_pGraphList != NULL )
    {
		vSetValues( m_pGraphList->m_odGraphParameters );
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