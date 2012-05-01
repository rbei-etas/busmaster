/**
 * \file      ColourPicker.h
 * \brief     Interface file for CColourPicker class
 * \author    Chris Maunder
 *
 * Interface file for CColourPicker class
 */
// ColourPicker.h : header file
//
// ColourPicker is a drop-in colour picker control. Check out the
// header file or the accompanying HTML doc file for details.
//
// Written by Chris Maunder (chrismaunder@codeguru.com)
// Extended by Alexander Bischofberger (bischofb@informatik.tu-muenchen.de)
// Copyright (c) 1998.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is
// not sold for profit without the authors written consent, and
// providing that this notice and the authors name is included. If
// the source code in  this file is used in any commercial application
// then a simple email would be nice.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to your
// computer, causes your pet cat to fall ill, increases baldness or
// makes you car start emitting strange noises when you start it up.
//
// Expect bugs.
//
// Please use and enjoy. Please let me know of any bugs/mods/improvements
// that you have found/implemented and I will fix/incorporate them into this
// file.
//
// Updated 16 May 1998
//         31 May 1998 - added Default text (CJM)
//          9 Jan 1999 - minor vis update

// Reference http://www.codeproject.com/KB/miscctrl/colour_picker.aspx*/

#pragma once

#define WM_CPN_SELCHANGE            WM_USER + 52 // Colour Picker Selection change
#define WM_CPN_DROPDOWN             WM_USER + 53 // Colour Picker drop down
#define WM_CPN_CLOSEUP              WM_USER + 54 // Colour Picker close up
#define WM_CPN_SELENDOK             WM_USER + 55 // Colour Picker end OK
#define WM_CPN_SELENDCANCEL         WM_USER + 56 // Colour Picker end (cancelled)


// For Color Popup Dialog interface
#include "ColourPopup.h"


void AFXAPI DDX_ColourPicker(CDataExchange* pDX, int nIDC, COLORREF& crColour);


#define CP_MODE_TEXT 1  // edit text colour
#define CP_MODE_BK   2  // edit background colour (default)

class CColourPicker : public CButton
{
public:
    CColourPicker();
    DECLARE_DYNCREATE(CColourPicker);

    // Attributes
public:
    // To Get Selected Color
    COLORREF GetColour();
    // To Set Color
    void SetColour(COLORREF crColour);
    // To Get Default Text
    void SetDefaultText(LPCTSTR szDefaultText);
    // To Set Custom Text
    void SetCustomText(LPCTSTR szCustomText);
    // To Activate/ Deactivate Online Tracking of Color
    void SetTrackSelection(BOOL bTracking = TRUE)
    {
        m_bTrackSelection = bTracking;
    }
    // Get get current track action
    BOOL GetTrackSelection()
    {
        return m_bTrackSelection;
    }
    // To set Selection mode
    void SetSelectionMode(UINT nMode)
    {
        m_nSelectionMode = nMode;
    }
    // To Get current selection mode
    UINT GetSelectionMode()
    {
        return m_nSelectionMode;
    };
    // To Set Background Color
    void SetBkColour(COLORREF crColourBk);
    // To get Current background color
    COLORREF GetBkColour()
    {
        return m_crColourBk;
    }
    // To set Text Color
    void SetTextColour( COLORREF crColourText );
    // To Get Text Color
    COLORREF GetTextColour()
    {
        return m_crColourText;
    }

    // Operations
public:
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CColourPicker)
public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
protected:
    virtual void PreSubclassWindow();
    //}}AFX_VIRTUAL

    // Implementation
public:
    virtual ~CColourPicker();

protected:
    void SetWindowSize();

    // protected attributes
protected:
    // For DropDown Active
    BOOL        m_bActive;
    // For Tracking the selection
    BOOL        m_bTrackSelection;
    // Background color
    COLORREF m_crColourBk;
    // Text Color
    COLORREF m_crColourText;
    // Selection mode
    UINT     m_nSelectionMode;
    // Border Rect
    CRect    m_ArrowRect;
    // Default text
    CString  m_strDefaultText;
    // User set text
    CString  m_strCustomText;

    // Generated message map functions
protected:
    //{{AFX_MSG(CColourPicker)
    afx_msg BOOL OnClicked();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    //}}AFX_MSG
    afx_msg LONG OnSelEndOK(UINT lParam, LONG wParam);
    afx_msg LONG OnSelEndCancel(UINT lParam, LONG wParam);
    afx_msg LONG OnSelChange(UINT lParam, LONG wParam);

    DECLARE_MESSAGE_MAP()
};
