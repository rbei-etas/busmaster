/**
 * \file      ColourPicker.cpp
 * \brief     Implementation file for CColourPicker class
 * \authors   Chris Maunder (chrismaunder@codeguru.com)
  *
 * Implementation file for CColourPicker class
 */

// ColourPicker.cpp : implementation file
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

// For standard includes
#include "Utils_stdafx.h"
// For Color Popup dialog interface
#include "ColourPopup.h"
// For Color Picker dialog interface
#include "ColourPicker.h"
// For standard definitions
#include "Utility_Structs.h"

#define STR_DEFAULT_TEXT         _T("Automatic")
#define STR_CUSTOM_TEXT          _T("More Colours...")


/*******************************************************************************
  Function Name  : DDX_ColourPicker
  Input(s)       : pDX - Pointer to Data exchange object
                   crColour - Color variable associated with DDX
  Output         : -
  Functionality  : This DDX interface function to set/get color value using DDX
  Member of      : CColourPicker
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void AFXAPI DDX_ColourPicker(CDataExchange* pDX, int nIDC, COLORREF& crColour)
{
    HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
    ASSERT (hWndCtrl != NULL);
    CColourPicker* pColourPicker = (CColourPicker*) CWnd::FromHandle(hWndCtrl);

    // To get Color
    if (pDX->m_bSaveAndValidate)
    {
        crColour = pColourPicker->GetColour();
    }
    // To set Color
    else
    {
        pColourPicker->SetColour(crColour);
    }
}

/*******************************************************************************
  Function Name  : CColourPicker
  Description    : Standard default constructor
  Member of      : CColourPicker
  Functionality  : This will initialise local variables
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
CColourPicker::CColourPicker()
{
    SetBkColour(GetSysColor(COLOR_3DFACE));
    SetTextColour(GetSysColor(COLOR_BTNTEXT));
    m_bTrackSelection = FALSE;
    m_nSelectionMode = CP_MODE_BK;
    m_bActive = FALSE;
    m_strDefaultText = STR_DEFAULT_TEXT;
    m_strCustomText  = STR_CUSTOM_TEXT;
}

/*******************************************************************************
  Function Name  : ~CColourPicker
  Description    : Standard Destructor
  Member of      : CColourPicker
  Functionality  : -
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
CColourPicker::~CColourPicker()
{
}

IMPLEMENT_DYNCREATE(CColourPicker, CButton)

BEGIN_MESSAGE_MAP(CColourPicker, CButton)
    //{{AFX_MSG_MAP(CColourPicker)
    ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
    ON_WM_CREATE()
    //}}AFX_MSG_MAP
    ON_MESSAGE(WM_CPN_SELENDOK,     OnSelEndOK)
    ON_MESSAGE(WM_CPN_SELENDCANCEL, OnSelEndCancel)
    ON_MESSAGE(WM_CPN_SELCHANGE,    OnSelChange)
END_MESSAGE_MAP()

/*******************************************************************************
  Function Name  : OnSelEndOK
  Input(s)       : -
  Output         : -
  Functionality  : This will be called if user selects OK in color popup dialog
  Member of      : CColourPicker
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
LONG CColourPicker::OnSelEndOK(UINT lParam, LONG /*wParam*/)
{
    COLORREF crNewColour = (COLORREF) lParam;
    m_bActive = FALSE;
    // Update Color
    SetColour(crNewColour);
    // Update Parent
    CWnd* pParent = GetParent();

    if (pParent != NULL )
    {
        pParent->SendMessage(WM_CPN_CLOSEUP, lParam, (WPARAM) GetDlgCtrlID());
        pParent->SendMessage(WM_CPN_SELENDOK, lParam, (WPARAM) GetDlgCtrlID());
    }

    // Send Change Notification
    if (crNewColour != GetColour())
    {
        if (pParent != NULL)
        {
            pParent->SendMessage( WM_CPN_SELCHANGE, lParam,
                                  (WPARAM) GetDlgCtrlID() );
        }
    }

    return TRUE;
}

/*******************************************************************************
  Function Name  : OnSelEndOK
  Input(s)       : -
  Output         : -
  Functionality  : This will be called if user selects Cancel in color popup
                   dialog
  Member of      : CColourPicker
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
LONG CColourPicker::OnSelEndCancel(UINT lParam, LONG /*wParam*/)
{
    m_bActive = FALSE;
    // Update Color
    SetColour((COLORREF) lParam);
    // Notify parent
    CWnd* pParent = GetParent();

    if (pParent != NULL)
    {
        pParent->SendMessage( WM_CPN_CLOSEUP, lParam, (WPARAM) GetDlgCtrlID());
        pParent->SendMessage( WM_CPN_SELENDCANCEL, lParam,
                              (WPARAM) GetDlgCtrlID());
    }

    return TRUE;
}

/*******************************************************************************
  Function Name  : OnSelChange
  Input(s)       : -
  Output         : -
  Functionality  : Notifys parent on track change
  Member of      : CColourPicker
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
LONG CColourPicker::OnSelChange(UINT lParam, LONG /*wParam*/)
{
    // If Tracking is enabled
    if ( m_bTrackSelection == TRUE )
    {
        SetColour((COLORREF) lParam);
    }

    // Update Parent
    CWnd* pParent = GetParent();

    if (pParent != NULL)
    {
        pParent->SendMessage(WM_CPN_SELCHANGE, lParam, (WPARAM) GetDlgCtrlID());
    }

    return TRUE;
}

/*******************************************************************************
  Function Name  : OnCreate
  Input(s)       : lpCreateStruct - Create Structure
  Output         : -
  Functionality  : Creates window by default method and resizes the window.
  Member of      : CColourPicker
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
int CColourPicker::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CButton::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    // resize appropriately
    SetWindowSize();
    return 0;
}

/*******************************************************************************
  Function Name  : OnClicked
  Input(s)       : -
  Output         : -
  Functionality  : On mouse click, create and show a CColourPopup window for
                   colour selection
  Member of      : CColourPicker
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
BOOL CColourPicker::OnClicked()
{
    m_bActive = TRUE;
    CWnd* pParent = GetParent();

    if (pParent)
    {
        pParent->SendMessage(WM_CPN_DROPDOWN, (LPARAM)GetColour(), (WPARAM) GetDlgCtrlID());
    }

    // Get current point
    CRect rect;
    GetWindowRect(rect);
    // Create new instance. This will be deleted automatically on destroy of
    // window
    new CColourPopup ( CPoint(rect.left, rect.bottom), // Point to display popup
                       GetColour(),                   // Selected colour
                       this,                          // parent
                       m_strDefaultText,              // Default text removed
                       m_strCustomText);              // Custom Text
    return TRUE;
}

/*******************************************************************************
  Function Name  : DrawItem
  Input(s)       : lpDrawItemStruct - Draw Structure
  Output         : -
  Functionality  : This function draws the button with down scrollbar button
  Member of      : CColourPicker
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CColourPicker::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    ASSERT(lpDrawItemStruct);
    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect rect    = lpDrawItemStruct->rcItem;
    UINT state   = lpDrawItemStruct->itemState;
    CString m_strText;
    // Get Margins from the system
    CSize Margins(::GetSystemMetrics(SM_CXEDGE), ::GetSystemMetrics(SM_CYEDGE));

    // Draw arrow
    // Draw Selected button
    if (m_bActive == TRUE )
    {
        state |= ODS_SELECTED;
    }

    // Draw scroll down button
    pDC->DrawFrameControl(&m_ArrowRect, DFC_SCROLL, DFCS_SCROLLDOWN  |
                          ((state & ODS_SELECTED) ? DFCS_PUSHED : 0) |
                          ((state & ODS_DISABLED) ? DFCS_INACTIVE : 0));
    // Draw 3d Rectangle
    pDC->DrawEdge(rect, EDGE_RAISED, BF_RECT);
    // Must reduce the size of the "client" area of the button
    // due to edge thickness.
    rect.DeflateRect( Margins.cx + 3, Margins.cy + 2 );
    // Fill remaining area with colour
    rect.right -= m_ArrowRect.Width();
    // Create Backrgound Brush
    ULONG ulBkColor = 0;

    // If diabled or selected default color use 3D Face system color
    if( state& ODS_DISABLED || m_crColourBk == CLR_DEFAULT)
    {
        ulBkColor = ::GetSysColor(COLOR_3DFACE);
    }
    // Use selected color
    else
    {
        ulBkColor = m_crColourBk;
    }

    // Create backgroung brush
    CBrush brush( ulBkColor );
    // Save old brush for restore
    CBrush* pOldBrush = (CBrush*) pDC->SelectObject(&brush);
    // Select black pen
    pDC->SelectStockObject(BLACK_PEN);
    // Braw boundary rect
    pDC->Rectangle(rect);
    // Select Old Brush
    pDC->SelectObject(pOldBrush);
    // Draw the window text (if any)
    GetWindowText(m_strText);

    if (m_strText.GetLength())
    {
        pDC->SetBkMode(TRANSPARENT);

        // If the button is disabled
        if (state & ODS_DISABLED)
        {
            // Draw disabled text. Text folled by shadow
            rect.OffsetRect(1,1);
            pDC->SetTextColor(::GetSysColor(COLOR_3DHILIGHT));
            pDC->DrawText(m_strText, rect, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
            rect.OffsetRect(-1,-1);
            pDC->SetTextColor(::GetSysColor(COLOR_3DSHADOW));
            pDC->DrawText(m_strText, rect, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
        }
        // Draw normal text
        else
        {
            pDC->SetTextColor((m_crColourText == CLR_DEFAULT)? 0 : m_crColourText);
            pDC->DrawText(m_strText, rect, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
        }
    }

    // Draw focus rect
    if (state & ODS_FOCUS)
    {
        rect.DeflateRect(1,1);
        pDC->DrawFocusRect(rect);
    }
}

/*******************************************************************************
  Function Name  : PreSubclassWindow
  Input(s)       : -
  Output         : -
  Functionality  : This overwridden function will change the style to ownerdraw
                   so that custom draw method will be called.
  Member of      : CColourPicker
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CColourPicker::PreSubclassWindow()
{
    ModifyStyle(0, BS_OWNERDRAW);        // Make it owner drawn
    CButton::PreSubclassWindow();
    SetWindowSize();                     // resize appropriately
}

/*******************************************************************************
  Function Name  : GetColour
  Input(s)       : -
  Output         : COLORREF - Selected Color
  Functionality  : This function returns selected color
  Member of      : CColourPicker
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
COLORREF CColourPicker::GetColour()
{
    return (m_nSelectionMode == CP_MODE_TEXT)?
           GetTextColour(): GetBkColour();
}

/*******************************************************************************
  Function Name  : SetColour
  Input(s)       : COLORREF - Color to set
  Output         : -
  Functionality  : This function sets the color of the control
  Member of      : CColourPicker
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CColourPicker::SetColour(COLORREF crColour)
{
    (m_nSelectionMode == CP_MODE_TEXT)?
    SetTextColour(crColour): SetBkColour(crColour);
}

/*******************************************************************************
  Function Name  : SetBkColour
  Input(s)       : COLORREF - Background Color to set
  Output         : -
  Functionality  : This function sets the background color of the control
  Member of      : CColourPicker
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CColourPicker::SetBkColour(COLORREF crColourBk)
{
    m_crColourBk = crColourBk;

    if (IsWindow(m_hWnd))
    {
        RedrawWindow();
    }
}

/*******************************************************************************
  Function Name  : SetTextColour
  Input(s)       : COLORREF - Text Color to set
  Output         : -
  Functionality  : This function sets the text color of the control
  Member of      : CColourPicker
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CColourPicker::SetTextColour(COLORREF crColourText)
{
    m_crColourText = crColourText;

    if (IsWindow(m_hWnd))
    {
        RedrawWindow();
    }
}

/*******************************************************************************
  Function Name  : SetDefaultText
  Input(s)       : szDefaultText - Default Text to set
  Output         : -
  Functionality  : This function sets the default text.
  Member of      : CColourPicker
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CColourPicker::SetDefaultText(LPCTSTR szDefaultText)
{
    m_strDefaultText = (szDefaultText)? szDefaultText : "";
}

/*******************************************************************************
  Function Name  : SetCustomText
  Input(s)       : szCustomText - Custom Text to set
  Output         : -
  Functionality  : This function sets the custom text.
  Member of      : CColourPicker
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CColourPicker::SetCustomText(LPCTSTR szCustomText)
{
    m_strCustomText = (szCustomText)? szCustomText : STR_EMPTY;
}

/*******************************************************************************
  Function Name  : SetWindowSize
  Input(s)       : -
  Output         : -
  Functionality  : This function calculates the scrollbar button size
  Member of      : CColourPicker
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CColourPicker::SetWindowSize()
{
    // Get size dimensions of edges
    CSize MarginSize(::GetSystemMetrics(SM_CXEDGE), ::GetSystemMetrics(SM_CYEDGE));
    // Get size of dropdown arrow
    int nArrowWidth = max(::GetSystemMetrics(SM_CXHTHUMB), 5*MarginSize.cx);
    int nArrowHeight = max(::GetSystemMetrics(SM_CYVTHUMB), 5*MarginSize.cy);
    CSize ArrowSize(max(nArrowWidth, nArrowHeight), max(nArrowWidth, nArrowHeight));
    // Get window size
    CRect rect;
    GetWindowRect(rect);
    CWnd* pParent = GetParent();

    if (pParent)
    {
        pParent->ScreenToClient(rect);
    }

    // Set window size at least as wide as 2 arrows, and as high as arrow + margins
    int nWidth = max(rect.Width(), 2*ArrowSize.cx + 2*MarginSize.cx);
    MoveWindow(rect.left, rect.top, nWidth, ArrowSize.cy+2*MarginSize.cy, TRUE);
    // Get the new coords of this window
    GetWindowRect(rect);
    ScreenToClient(rect);
    // Get the rect where the arrow goes, and convert to client coords.
    m_ArrowRect.SetRect(rect.right - ArrowSize.cx - MarginSize.cx,
                        rect.top + MarginSize.cy, rect.right - MarginSize.cx,
                        rect.bottom - MarginSize.cy);
}
