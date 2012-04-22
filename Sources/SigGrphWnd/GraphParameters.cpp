/*********************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  GraphParameters.cpp
  Description   :  Implementation file for CGraphParameters class
  $Log:   X:/Archive/Sources/SigGrphWnd/GraphParameters.cpv  $
   
      Rev 1.1   15 Apr 2011 19:43:32   rac2kor
   Inserted RBEI Copyright information text into the file header.
   
      Rev 1.0   13 Dec 2010 22:00:36   CANMNTTM
    
   
      Rev 1.1   18 Aug 2010 19:01:06   CANMNTTM
   Default configuration function has been implemented
   
      Rev 1.0   16 Aug 2010 21:20:32   rac2kor
    

  Author(s)     :  Raja N
  Date Created  :  01.12.2004
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved
 *********************************************************************/

// For standard Headers
#include "SigGrphWnd_stdafx.h"
// For CGraphParameters class declaration
#include "GraphParameters.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*******************************************************************************
 Function Name  : CGraphParameters
 Input(s)       : -
 Output         : -
 Functionality  : This function will Update global structure with the user
                  selected values.
 Member of      : CGraphParameters
 Author(s)      : Raja N
 Date Created   : 01.12.2004
 Modifications  : 
*******************************************************************************/
CGraphParameters::CGraphParameters()
{
    vInitialize();
}
void CGraphParameters::vInitialize(void)
{
    // Set the default values
    m_nBufferSize = defDEFAULT_BUFFER_SIZE;
    // Display refresh rate
    m_nRefreshRate = defDEFAULT_UPDATE_RATE;
    // View Style Configuration
    // Frame Color
    m_nFrameColor = defDEFAULT_FRAME_COLOR;
    // Frame Style
    m_nFrameStyle = defDEFAULT_FRAME_TYPE;
    // Plot Area Color
    m_nPlotAreaColor = defDEFAULT_PLOT_AREA_COLOR;
    // Grid Color
    m_nGridColor = defDEFAULT_GRID_COLOR;
    // Axis Color
    m_nAxisColor = defDEFAULT_AXIS_COLOR;
    // X Grid Lines
    m_nXGridLines = defDEFAULT_X_GRID_LINES; // 10
    // Y Grid Lines
    m_nYGridLines = defDEFAULT_Y_GRID_LINES; // 5
    // User Selected Active Axis
    m_nActiveAxis = defZOOM_XY_AXIS; // XY
    // User Selected Action
    m_nAction = defTRACK_MODE_NONE; // None
    // Grid Setting
    m_bShowGrid = TRUE;
	//Graph Line Display
	m_eDisplayType = eDISPLAY_NORMAL;	
}
/*******************************************************************************
 Function Name  : ~CGraphParameters
 Input(s)       : -
 Output         : -
 Functionality  : This function will Update global structure with the user
                  selected values.
 Member of      : CGraphParameters
 Author(s)      : Raja N
 Date Created   : 01.12.2004
 Modifications  : 
*******************************************************************************/
CGraphParameters::~CGraphParameters()
{

}
BYTE* CGraphParameters::pbyGetConfigData(BYTE* pbyTrgtData)
{
    BYTE* pbyTemp = pbyTrgtData;
    if (pbyTemp != NULL)
    {
        // Set the def
        COPY_DATA(pbyTemp, &m_nBufferSize, sizeof(int));
        // Display ref
        COPY_DATA(pbyTemp, &m_nRefreshRate, sizeof(int));
        // View Style 
        // Frame Color
        COPY_DATA(pbyTemp, &m_nFrameColor, sizeof(int));
        // Frame Style
        COPY_DATA(pbyTemp, &m_nFrameStyle, sizeof(int));
        // Plot Area C
        COPY_DATA(pbyTemp, &m_nPlotAreaColor, sizeof(int));
        // Grid Color
        COPY_DATA(pbyTemp, &m_nGridColor, sizeof(int));
        // Axis Color
        COPY_DATA(pbyTemp, &m_nAxisColor, sizeof(int));
        // X Grid Line
        COPY_DATA(pbyTemp, &m_nXGridLines, sizeof(int));
        // Y Grid Line
        COPY_DATA(pbyTemp, &m_nYGridLines, sizeof(int));
        // User Select
        COPY_DATA(pbyTemp, &m_nActiveAxis, sizeof(int));
        // User Select
        COPY_DATA(pbyTemp, &m_nAction, sizeof(int));
        // Grid Setting
        COPY_DATA(pbyTemp, &m_bShowGrid, sizeof(BOOL));
		//Line Display type
		COPY_DATA(pbyTemp, &m_eDisplayType, sizeof(eDISPLAY_TYPE));		
    }
    return pbyTemp;
}
BYTE* CGraphParameters::pbySetConfigData(BYTE* pbyTrgtData)
{
    BYTE* pbyTemp = pbyTrgtData;
    if (pbyTemp != NULL)
    {
        // Set the def
        COPY_DATA_2(&m_nBufferSize, pbyTemp, sizeof(int));
        // Display ref
        COPY_DATA_2(&m_nRefreshRate, pbyTemp, sizeof(int));
        // View Style 
        // Frame Color
        COPY_DATA_2(&m_nFrameColor, pbyTemp, sizeof(int));
        // Frame Style
        COPY_DATA_2(&m_nFrameStyle, pbyTemp, sizeof(int));
        // Plot Area C
        COPY_DATA_2(&m_nPlotAreaColor, pbyTemp, sizeof(int));
        // Grid Color
        COPY_DATA_2(&m_nGridColor, pbyTemp, sizeof(int));
        // Axis Color
        COPY_DATA_2(&m_nAxisColor, pbyTemp, sizeof(int));
        // X Grid Line
        COPY_DATA_2(&m_nXGridLines, pbyTemp, sizeof(int));
        // Y Grid Line
        COPY_DATA_2(&m_nYGridLines, pbyTemp, sizeof(int));
        // User Select
        COPY_DATA_2(&m_nActiveAxis, pbyTemp, sizeof(int));
        // User Select
        COPY_DATA_2(&m_nAction, pbyTemp, sizeof(int));
        // Grid Setting
        COPY_DATA_2(&m_bShowGrid, pbyTemp, sizeof(BOOL));
		//Line Display type
		COPY_DATA_2(&m_eDisplayType, pbyTemp, sizeof(eDISPLAY_TYPE));	
    }
    return pbyTemp;
}
/*******************************************************************************
 Function Name  : nSerialize
 Input(s)       : omArchive - CArchive class. Serialisation stream
 Output         : -
 Functionality  : This function will serialise this class using CArchive stream
 Member of      : CGraphParameters
 Author(s)      : Raja N
 Date Created   : 01.12.2004
 Modifications  : 
*******************************************************************************/
int CGraphParameters::nSerialize(CArchive& omArchive)
{
    int nReturn = 0;
    try
    {
        // If it is storing
        if( omArchive.IsStoring() )
        {
            // Set the def
            omArchive << m_nBufferSize;
            // Display ref
            omArchive << m_nRefreshRate;
            // View Style 
            // Frame Color
            omArchive << m_nFrameColor;
            // Frame Style
            omArchive << m_nFrameStyle;
            // Plot Area C
            omArchive << m_nPlotAreaColor;
            // Grid Color
            omArchive << m_nGridColor;
            // Axis Color
            omArchive << m_nAxisColor;
            // X Grid Line
            omArchive << m_nXGridLines;
            // Y Grid Line
            omArchive << m_nYGridLines;
            // User Select
            omArchive << m_nActiveAxis;
            // User Select
            omArchive << m_nAction ;
            // Grid Settin
            omArchive << m_bShowGrid;
        }
        // This is for loading
        else
        {
            // Set the def
            omArchive >> m_nBufferSize;
            // Display ref
            omArchive >> m_nRefreshRate;
            // View Style 
            // Frame Color
            omArchive >> m_nFrameColor;
            // Frame Style
            omArchive >> m_nFrameStyle;
            // Plot Area C
            omArchive >> m_nPlotAreaColor;
            // Grid Color
            omArchive >> m_nGridColor;
            // Axis Color
            omArchive >> m_nAxisColor;
            // X Grid Line
            omArchive >> m_nXGridLines;
            // Y Grid Line
            omArchive >> m_nYGridLines;
            // User Select
            omArchive >> m_nActiveAxis;
            // User Select
            omArchive >> m_nAction ;
            // Grid Settin
            omArchive >> m_bShowGrid;
        }
    }
    catch(CArchiveException* poArchExcep)
    {
        // Get the Error Code and delete dynamic object
        nReturn = poArchExcep->m_cause;
        poArchExcep->Delete();
    }
    // Return the result
    return nReturn;
}
