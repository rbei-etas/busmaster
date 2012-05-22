/*********************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  GraphList.h
  Description   :  Interface file for CGraphList class
  $Log:   X:/Archive/Sources/SigGrphWnd/GraphList.h_v  $
 * 
 *    Rev 1.1   15 Apr 2011 19:43:32   rac2kor
 * Inserted RBEI Copyright information text into the file header.
 * 
 *    Rev 1.0   13 Dec 2010 22:00:36   CANMNTTM
 *  
 * 
 *    Rev 1.0   16 Aug 2010 21:20:32   rac2kor
 *  

  Author        :  Raja N
  Date Created  :  01.12.2004
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
 *********************************************************************/

#if !defined(AFX_GRAPHLIST_H__C6AC1275_DE31_4030_AF09_EB1388DE879D__INCLUDED_)
#define AFX_GRAPHLIST_H__C6AC1275_DE31_4030_AF09_EB1388DE879D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// For Graph Parameter Class
#include "GraphParameters.h"
// For Graph Element class
#include "GraphElement.h"

class CGraphList
{
public:
    int nSerialize(CArchive& omArch);
    BYTE* pbyGetConfigData(BYTE* pbyTrgtData);
    BYTE* pbySetConfigData(BYTE* pbyTrgtData);
    UINT unGetConfigSize(void);
    // Default Constructor & destructor
    CGraphList();
    virtual ~CGraphList();

    // Variables
    // Graph control parameters
    CGraphParameters m_odGraphParameters;
    // Graph element array
    CArray<CGraphElement, CGraphElement&> m_omElementList;
};

#endif // !defined(AFX_GRAPHLIST_H__C6AC1275_DE31_4030_AF09_EB1388DE879D__INCLUDED_)
