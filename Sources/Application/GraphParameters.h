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
 * \file      GraphParameters.h
 * \brief     Interface file for CGraphParameters class
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Interface file for CGraphParameters class
 */

#pragma once

typedef enum eDISPLAY_TYPE
{
	eDISPLAY_NORMAL,
	eDISPLAY_STEPMODE_XY,
	eDISPLAY_STEPMODE_YX
};

class CGraphParameters
{
public:
    // To serialize class members
    void vInitialize(void);
	UINT unGetConfigSize(BYTE byVersion);
    int nSerialize(CArchive& omArchive);
    BYTE* pbySetConfigData(BYTE* pbyTrgtData, BYTE byVersion);
    BYTE* pbyGetConfigData(BYTE* pbyTrgtData, BYTE byVersion);
    // Default constructor and destructor.
    CGraphParameters();
    virtual ~CGraphParameters();

    // Display perf conf.
    // Buffer size
    int m_nBufferSize;
    // Display refresh rate
    int m_nRefreshRate;
    // View Style Configuration
    // Frame Color
    int m_nFrameColor;
    // Frame Style
    int m_nFrameStyle;
    // Plot Area Color
    int m_nPlotAreaColor;
    // Grid Color
    int m_nGridColor;
    // Axis Color
    int m_nAxisColor;
    // X Grid Lines
    int m_nXGridLines;
    // Y Grid Lines
    int m_nYGridLines;
    // User Selected Active Axis
    int m_nActiveAxis;
    // User Selected Action
    int m_nAction;
    // Grid Setting
    BOOL m_bShowGrid;
	//Graph Line Display
	eDISPLAY_TYPE m_eDisplayType;	
};
