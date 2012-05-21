/*******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  MDIChildBase.h
  Description   :  Interface file for CMDIChildBase class
  $Log:   X:/Archive/Sources/SigGrphWnd/MDIChildBase.h_v  $
 * 
 *    Rev 1.1   15 Apr 2011 19:43:34   rac2kor
 * Inserted RBEI Copyright information text into the file header.
 * 
 *    Rev 1.0   13 Dec 2010 22:00:36   CANMNTTM
 *  
 * 
 *    Rev 1.0   16 Aug 2010 21:20:34   rac2kor
 *  

  Author(s)     :  Raja N
  Date Created  :  13.4.2005
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
*******************************************************************************/

#if !defined(AFX_MDICHILDBASE_H__B09E3A76_DB04_4D31_A000_150658B2128F__INCLUDED_)
#define AFX_MDICHILDBASE_H__B09E3A76_DB04_4D31_A000_150658B2128F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "Application/hashdefines.h"
#include "SigGrphWnd_Defines.h"

class CMDIChildBase : public CMDIChildWnd
{
    DECLARE_DYNCREATE(CMDIChildBase)
public:
    // Constructor with Window Identity
    CMDIChildBase(eCONFIGDETAILS eWindowIdentity);
    // Default constructor
    CMDIChildBase();
    virtual ~CMDIChildBase();
    // To Update window with co-ordinates from Configuration module
    void vUpdateWndCo_Ords();
    // To save window co-ordinates in to Configuration module
    void vUpdateWinStatus();
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMDIChildBase)
    //}}AFX_VIRTUAL

// Implementation
protected:
    // helping function to Save and loading window postion from
    // configuration module
    void vSaveWinStatus();
    void vGetWinStatus( );
    // Generated message map functions
    //{{AFX_MSG(CMDIChildBase)
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

// Attributes
    // Make members as protected so that derivation of this class shall use
    // if it wants
    // Window placement Attribute
    WINDOWPLACEMENT m_sWindowPlacement;
    // Set the window identity parameter
    eCONFIGDETAILS  m_eWindowIdentity;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDICHILDBASE_H__B09E3A76_DB04_4D31_A000_150658B2128F__INCLUDED_)
