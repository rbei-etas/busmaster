/*********************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  MessageList.h
  Description   :  Interface file for CMessageList class
  $Log:   X:/Archive/Sources/SigGrphWnd/MessageList.h_v  $
 * 
 *    Rev 1.1   15 Apr 2011 19:43:34   rac2kor
 * Inserted RBEI Copyright information text into the file header.
 * 
 *    Rev 1.0   13 Dec 2010 22:00:36   CANMNTTM
 *  
 * 
 *    Rev 1.0   16 Aug 2010 21:20:36   rac2kor
 *  

  Author(s)     :  Ratnadip Choudhury
  Date Created  :  31.01.2004
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
 *********************************************************************/
#if !defined(AFX_MESSAGELIST_H__5E48819F_A933_4E00_A57F_BE7B97361541__INCLUDED_)
#define AFX_MESSAGELIST_H__5E48819F_A933_4E00_A57F_BE7B97361541__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MessageList.h : header file
//

// Definition of Flicker Free List Control
#include "Utility/FFListCtrl.h"

class CMessageList : public CFFListCtrl
{
private:
    // Row String
    CString m_omStrInARow;
    // GDI Objects for background and selection brush
    HBRUSH m_hWhiteBrush, m_hBlueBrush;

// Construction
public:
    CMessageList();

// Attributes
public:

// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMessageList)
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CMessageList();

    // Generated message map functions
protected:
    // Overwride to implement custom draw
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

    //{{AFX_MSG(CMessageList)
    afx_msg void OnDestroy();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESSAGELIST_H__5E48819F_A933_4E00_A57F_BE7B97361541__INCLUDED_)