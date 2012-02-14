/*********************************************************************
  Project       :  CAN-Monitor
  FileName      :  GraphParameters.h
  Description   :  Interface file for CGraphParameters class
  $Log:   X:/Archive/Sources/SigGrphWnd/GraphParameters.h_v  $
 * 
 *    Rev 1.1   15 Apr 2011 19:43:32   rac2kor
 * Inserted RBEI Copyright information text into the file header.
 * 
 *    Rev 1.0   13 Dec 2010 22:00:36   CANMNTTM
 *  
 * 
 *    Rev 1.1   18 Aug 2010 19:03:08   CANMNTTM
 * Graph related configuration 
 * saving functions have been implemented
 * 
 *    Rev 1.0   16 Aug 2010 21:20:32   rac2kor
 *  

  Author        :  Raja N
  Date Created  :  01.12.2004
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
 *********************************************************************/

#if !defined(AFX_GRAPHPARAMETERS_H__2CC21E72_BC35_4502_9FB7_DB030798AC4A__INCLUDED_)
#define AFX_GRAPHPARAMETERS_H__2CC21E72_BC35_4502_9FB7_DB030798AC4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SigGrphWnd_Defines.h"

class CGraphParameters
{
public:
    // To serialize class members
    void vInitialize(void);
    int nSerialize(CArchive& omArchive);
    BYTE* pbySetConfigData(BYTE* pbyTrgtData);
    BYTE* pbyGetConfigData(BYTE* pbyTrgtData);
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

#endif // !defined(AFX_GRAPHPARAMETERS_H__2CC21E72_BC35_4502_9FB7_DB030798AC4A__INCLUDED_)
