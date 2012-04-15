/**
 * \file      ColourPopup.h
 * \brief     Interface file for CColourPopup class
 * \author    Chris Maunder (chrismaunder@codeguru.com)
 *
 * Interface file for CColourPopup class
 */

// ColourPopup.h : header file
//
// Written by Chris Maunder (chrismaunder@codeguru.com)
// Extended by Alexander Bischofberger (bischofb@informatik.tu-muenchen.de)
// Copyright (c) 1998.
//
// Updated 30 May 1998 to allow any number of colours, and to
//                     make the appearance closer to Office 97. 
//                     Also added "Default" text area.         (CJM)
//
//         13 June 1998 Fixed change of focus bug (CJM)
//         30 June 1998 Fixed bug caused by focus bug fix (D'oh!!)
//                      Solution suggested by Paul Wilkerson.
//
// ColourPopup is a helper class for the colour picker control
// CColourPicker. Check out the header file or the accompanying 
// HTML doc file for details.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Expect bugs.
// 
// Please use and enjoy. Please let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into this
// file. 

// Reference http://www.codeproject.com/KB/miscctrl/colour_picker.aspx*/
#if !defined(AFX_COLOURPOPUP_H__D0B75902_9830_11D1_9C0F_00A0243D1382__INCLUDED_)
#define AFX_COLOURPOPUP_H__D0B75902_9830_11D1_9C0F_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// forward declaration
class CColourPicker;

// To hold the colours and their names
typedef struct {
    COLORREF crColour;
    TCHAR    *szName;
} ColourTableEntry;

/////////////////////////////////////////////////////////////////////////////
// CColourPopup window

class CColourPopup : public CWnd
{
// Construction
public:
    // Default Constructor
    CColourPopup();
    // Constructor with init values
    CColourPopup(CPoint p, COLORREF crColour, CWnd* pParentWnd,
                 LPCTSTR szDefaultText = NULL, LPCTSTR szCustomText = NULL);
    void Initialise();

// Attributes
public:

// Operations
public:
    // To Create Window
    BOOL Create(CPoint p, COLORREF crColour, CWnd* pParentWnd, 
                LPCTSTR szDefaultText = NULL, LPCTSTR szCustomText = NULL);

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CColourPopup)
    public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    //}}AFX_VIRTUAL

// Implementation
public:
    virtual ~CColourPopup();

protected:
    // To Get boundary of the cell
    BOOL GetCellRect(int nIndex, const LPRECT& rect);
    // To Find the cell from the colour ref.
    void FindCellFromColour(COLORREF crColour);
    // To resize the window approp.
    void SetWindowSize();
    // To Init tooltip object
    void CreateToolTips();
    // To set the selection to a new index
    void ChangeSelection(int nIndex);
    // To close the session
    void EndSelection(int nMessage);
    // Tp plot colour cells
    void DrawCell(CDC* pDC, int nIndex);
    // To get the colour at the index
    COLORREF GetColour(int nIndex);
    // To get tehe name associated with the color
    LPCTSTR GetColourName(int nIndex);
    // To get the color table index from row-col
    int  GetIndex(int row, int col) const;
    // To get only the row from the color table index
    int  GetRow(int nIndex) const;
    // To get only the coloum from the color table index
    int  GetColumn(int nIndex) const;

// protected attributes
protected:
    // Color Table
    static ColourTableEntry m_crColours[];
    // Color table parameters
    int            m_nNumColours;
    int            m_nNumColumns, m_nNumRows;
    // Color box size
    int            m_nBoxSize, m_nMargin;
    // To keep selection
    int            m_nCurrentSel;
    int            m_nChosenColourSel;
    // To keep custom and default text
    CString        m_strDefaultText;
    CString        m_strCustomText;
    // To keep text box boundary
    CRect          m_CustomTextRect, m_DefaultTextRect, m_WindowRect;
    // GDI Objects
    CFont          m_Font;
    CPalette       m_Palette;
    COLORREF       m_crInitialColour, m_crColour;
    // For Color Tool Tip
    CToolTipCtrl   m_ToolTip;
    CWnd*          m_pParent;
    // To keep track of window state
    BOOL           m_bChildWindowVisible;

    // Generated message map functions
protected:
    //{{AFX_MSG(CColourPopup)
    afx_msg void OnNcDestroy();
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg BOOL OnQueryNewPalette();
    afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
#if _MFC_VER <= 0x0600
    afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
#else
    afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
#endif
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLOURPOPUP_H__D0B75902_9830_11D1_9C0F_00A0243D1382__INCLUDED_)
