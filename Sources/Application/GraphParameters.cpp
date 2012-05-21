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
 * \file      GraphParameters.cpp
 * \brief     Implementation file for CGraphParameters class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CGraphParameters class
 */

// For standard Headers
#include "stdafx.h"
//SGW Code commented by Arun 21-10-2010
// For App declaration
#include "BUSMASTER.h"
//SGW Code commented by Arun 21-10-2010
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
BYTE* CGraphParameters::pbyGetConfigData(BYTE* pbyTrgtData, BYTE byVersion)
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
        // Grid Settin
        COPY_DATA(pbyTemp, &m_bShowGrid, sizeof(BOOL));

        if(byVersion == 2)
        {
            //Line Display type
            COPY_DATA(pbyTemp, &m_eDisplayType, sizeof(eDISPLAY_TYPE));
        }
    }

    return pbyTemp;
}
BYTE* CGraphParameters::pbySetConfigData(BYTE* pbyTrgtData, BYTE byVersion)
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
        // Grid Settin
        COPY_DATA_2(&m_bShowGrid, pbyTemp, sizeof(BOOL));

        if(byVersion == 2)
        {
            //Line Display type
            COPY_DATA_2(&m_eDisplayType, pbyTemp, sizeof(eDISPLAY_TYPE));
        }
        else if(byVersion == 1)
        {
            m_eDisplayType = eDISPLAY_NORMAL;
        }
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

/*******************************************************************************
 Function Name  : unGetConfigSize
 Input(s)       : byVersion
 Output         : -
 Functionality  : This function will get the size of the class data members.
 Member of      : CGraphParameters
 Author(s)      : ArunKumar K
 Date Created   : 16.12.2010
 Modifications  :
*******************************************************************************/
UINT CGraphParameters::unGetConfigSize(BYTE byVersion)
{
    UINT unSize = 0;
    // Display perf conf.
    // Buffer size
    unSize += sizeof(int);
    // Display refresh rate
    unSize += sizeof(int);
    // View Style Configuration
    // Frame Color
    unSize += sizeof(int);
    // Frame Style
    unSize += sizeof(int);
    // Plot Area Color
    unSize += sizeof(int);
    // Grid Color
    unSize += sizeof(int);
    // Axis Color
    unSize += sizeof(int);
    // X Grid Lines
    unSize += sizeof(int);
    // Y Grid Lines
    unSize += sizeof(int);
    // User Selected Active Axis
    unSize += sizeof(int);
    // User Selected Action
    unSize += sizeof(int);
    // Grid Setting
    unSize += sizeof(BOOL);

    if(byVersion == 2)
    {
        // Line Display
        unSize += sizeof(int);
    }

    return unSize;
}
