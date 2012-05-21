/*********************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  GraphElement.h
  Description   :  Interface file for CGraphElement class
  $Log:   X:/Archive/Sources/SigGrphWnd/GraphElement.h_v  $
 * 
 *    Rev 1.1   15 Apr 2011 19:43:30   rac2kor
 * Inserted RBEI Copyright information text into the file header.
 * 
 *    Rev 1.0   13 Dec 2010 22:00:34   CANMNTTM
 *  
 * 
 *    Rev 1.0   16 Aug 2010 21:20:32   rac2kor
 *  

  Author(s)     :  Raja N
  Date Created  :  01.12.2004
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
 *********************************************************************/

#if !defined(AFX_GRAPHELEMENT_H__36E59C90_39E0_404A_A291_7AF5129AA682__INCLUDED_)
#define AFX_GRAPHELEMENT_H__36E59C90_39E0_404A_A291_7AF5129AA682__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SigGrphWnd_Defines.h"

class CGraphElement
{
public:
// Variables
    // Message ID Variable
    int     m_nMsgID;
    // Message ID Name
    CString     m_strMsgName;
    // Frame Format - Standard or Extended
    short   m_nFrameFormat;
    // Element Name String
    CString m_omStrElementName;

    // Type of the element value
    // 0 - network statistics parameter
    // 1 - Raw Value of the Signal
    // 2 - Physical Value of the signal
    int     m_nValueType;
    // Line type of the element
    int     m_nLineType;
    // Line Color of the element
    int     m_nLineColor;
    // Sample point symbol type
    int     m_nPointType;
    // Sample point symbol color
    int     m_nPointColor;
    // Visible or Not
    BOOL    m_bVisible;
    // Enabled or not
    BOOL    m_bEnabled;
	//display type for signal
	eDISPLAY_TYPE m_eDisplayType;	

// Functions
    // To serialize this class
    int nSerialize(CArchive& omArch);
    BYTE* pbySetConfigData(BYTE* pbyTrgtData);
    BYTE* pbyGetConfigData(BYTE* pbyTrgtData);
    // Default Constructor & destructor
    CGraphElement();
    virtual ~CGraphElement();

};

#endif // !defined(AFX_GRAPHELEMENT_H__36E59C90_39E0_404A_A291_7AF5129AA682__INCLUDED_)
