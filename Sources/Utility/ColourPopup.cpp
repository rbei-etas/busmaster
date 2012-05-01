/**
 * \file      ColourPopup.cpp
 * \brief     Interface file for CColourPopup.c class
 * \authors   Chris Maunder (chrismaunder@codeguru.com)
  *
 * Interface file for CColourPopup.c class
 */

// ColourPopup.cpp : implementation file
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

// For standard include
#include "Utils_stdafx.h"
// For Color Picker class interface
#include "ColourPicker.h"
// For Color Popup class interface
#include "ColourPopup.h"
// For standard definitions
#include "Utility_Structs.h"

#define DEFAULT_BOX_VALUE       -3
#define CUSTOM_BOX_VALUE        -2
#define INVALID_COLOUR          -1

#define MAX_COLOURS             100

#define BLOCK_COLOR             RGB(0, 0, 0)
#define defPAL_VERSION          0x300

#define defMIN_BOX_SIZE         5
#define defMARGIN_FACTOR        2
#define defCOLOR_BOX_SIZE       18

// Color Table Definition
ColourTableEntry CColourPopup::m_crColours[] =
{
    { RGB(0x00, 0x00, 0x00),    _T("Black")             },
    { RGB(0xA5, 0x2A, 0x00),    _T("Brown")             },
    { RGB(0x00, 0x40, 0x40),    _T("Dark Olive Green")  },
    { RGB(0x00, 0x55, 0x00),    _T("Dark Green")        },
    { RGB(0x00, 0x00, 0x5E),    _T("Dark Teal")         },
    { RGB(0x00, 0x00, 0x8B),    _T("Dark blue")         },
    { RGB(0x4B, 0x00, 0x82),    _T("Indigo")            },
    { RGB(0x28, 0x28, 0x28),    _T("Dark grey")         },

    { RGB(0x8B, 0x00, 0x00),    _T("Dark red")          },
    { RGB(0xFF, 0x68, 0x20),    _T("Orange")            },
    { RGB(0x8B, 0x8B, 0x00),    _T("Dark yellow")       },
    { RGB(0x00, 0x93, 0x00),    _T("Green")             },
    { RGB(0x38, 0x8E, 0x8E),    _T("Teal")              },
    { RGB(0x00, 0x00, 0xFF),    _T("Blue")              },
    { RGB(0x7B, 0x7B, 0xC0),    _T("Blue-grey")         },
    { RGB(0x66, 0x66, 0x66),    _T("Grey - 40")         },

    { RGB(0xFF, 0x00, 0x00),    _T("Red")               },
    { RGB(0xFF, 0xAD, 0x5B),    _T("Light orange")      },
    { RGB(0x32, 0xCD, 0x32),    _T("Lime")              },
    { RGB(0x3C, 0xB3, 0x71),    _T("Sea green")         },
    { RGB(0x7F, 0xFF, 0xD4),    _T("Aqua")              },
    { RGB(0x7D, 0x9E, 0xC0),    _T("Light blue")        },
    { RGB(0x80, 0x00, 0x80),    _T("Violet")            },
    { RGB(0x7F, 0x7F, 0x7F),    _T("Grey - 50")         },

    { RGB(0xFF, 0xC0, 0xCB),    _T("Pink")              },
    { RGB(0xFF, 0xD7, 0x00),    _T("Gold")              },
    { RGB(0xFF, 0xFF, 0x00),    _T("Yellow")            },
    { RGB(0x00, 0xFF, 0x00),    _T("Bright green")      },
    { RGB(0x40, 0xE0, 0xD0),    _T("Turquoise")         },
    { RGB(0xC0, 0xFF, 0xFF),    _T("Skyblue")           },
    { RGB(0x48, 0x00, 0x48),    _T("Plum")              },
    { RGB(0xC0, 0xC0, 0xC0),    _T("Light grey")        },

    { RGB(0xFF, 0xE4, 0xE1),    _T("Rose")              },
    { RGB(0xD2, 0xB4, 0x8C),    _T("Tan")               },
    { RGB(0xFF, 0xFF, 0xE0),    _T("Light yellow")      },
    { RGB(0x98, 0xFB, 0x98),    _T("Pale green ")       },
    { RGB(0xAF, 0xEE, 0xEE),    _T("Pale turquoise")    },
    { RGB(0x68, 0x83, 0x8B),    _T("Pale blue")         },
    { RGB(0xE6, 0xE6, 0xFA),    _T("Lavender")          },
    { RGB(0xFF, 0xFF, 0xFF),    _T("White")             }
};

/*******************************************************************************
  Function Name  : CColourPopup
  Description    : Standard default constructor
  Member of      : CColourPopup
  Functionality  : This will initialise local variables
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
CColourPopup::CColourPopup()
{
    // Initilise default
    Initialise();
}

/*******************************************************************************
  Function Name  : CColourPopup
  Input(s)       : p - Point of show of this dialog
                   crColor - Selected color
                   pParentWnd - Parent window pointer
                   szDefaultText - Default Text Pointer
                   szCustomText - Custom Text Pointer
  Output         : -
  Functionality  : Constructor. Initialises local variables with the passed
                   parameters values
  Member of      : CColourPopup
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
CColourPopup::CColourPopup(CPoint p, COLORREF crColour, CWnd* pParentWnd,
                           LPCTSTR szDefaultText /* = NULL */,
                           LPCTSTR szCustomText  /* = NULL */)
{
    // Initilise default
    Initialise();
    // Assign parameter values now
    m_crColour       = m_crInitialColour = crColour;
    m_pParent        = pParentWnd;
    m_strDefaultText = (szDefaultText)? szDefaultText : STR_EMPTY;
    m_strCustomText  = (szCustomText)?  szCustomText  : STR_EMPTY;
    // Create Control
    CColourPopup::Create(p, crColour, pParentWnd, szDefaultText, szCustomText);
}

/*******************************************************************************
  Function Name  : Initialise
  Input(s)       : -
  Output         : -
  Functionality  : Initialises dialog's UI components
  Member of      : CColourPopup
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CColourPopup::Initialise()
{
    // Calculate number of colors in the pallete
    m_nNumColours       = sizeof(m_crColours)/sizeof(ColourTableEntry);
    // Boundary condition assertion
    ASSERT(m_nNumColours <= MAX_COLOURS);

    if (m_nNumColours > MAX_COLOURS)
    {
        m_nNumColours = MAX_COLOURS;
    }

    // Init draw parameters
    m_nNumColumns       = 0;
    m_nNumRows          = 0;
    m_nBoxSize          = defCOLOR_BOX_SIZE;
    m_nMargin           = ::GetSystemMetrics(SM_CXEDGE);
    m_nCurrentSel       = INVALID_COLOUR;
    m_nChosenColourSel  = INVALID_COLOUR;
    m_pParent           = NULL;
    m_crColour          = m_crInitialColour = BLOCK_COLOR;
    m_bChildWindowVisible = FALSE;

    // Make sure the colour square is at least 5 x 5;
    if( m_nBoxSize - (defMARGIN_FACTOR * m_nMargin) -
            defMARGIN_FACTOR < defMIN_BOX_SIZE )
    {
        m_nBoxSize = defMIN_BOX_SIZE + ( defMARGIN_FACTOR * m_nMargin) +
                     defMARGIN_FACTOR;
    }

    // Create the font
    NONCLIENTMETRICS ncm;
    ncm.cbSize = sizeof(NONCLIENTMETRICS);
    VERIFY(
        SystemParametersInfo( SPI_GETNONCLIENTMETRICS,
                              sizeof(NONCLIENTMETRICS),
                              &ncm, 0) );
    m_Font.CreateFontIndirect(&(ncm.lfMessageFont));
    // Create the palette
    struct
    {
        LOGPALETTE    LogPalette;
        PALETTEENTRY  PalEntry[MAX_COLOURS];
    } pal;
    LOGPALETTE* pLogPalette = (LOGPALETTE*) &pal;
    pLogPalette->palVersion    = defPAL_VERSION;
    pLogPalette->palNumEntries = (WORD) m_nNumColours;

    for (int i = 0; i < m_nNumColours; i++)
    {
        pLogPalette->palPalEntry[i].peRed   = GetRValue(m_crColours[i].crColour);
        pLogPalette->palPalEntry[i].peGreen = GetGValue(m_crColours[i].crColour);
        pLogPalette->palPalEntry[i].peBlue  = GetBValue(m_crColours[i].crColour);
        pLogPalette->palPalEntry[i].peFlags = 0;
    }

    m_Palette.CreatePalette(pLogPalette);
}

/*******************************************************************************
  Function Name  : ~CColourPopup
  Description    : Standard Destructor
  Member of      : CColourPopup
  Functionality  : -
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
CColourPopup::~CColourPopup()
{
    // Delete GDI Objects
    m_Font.DeleteObject();
    m_Palette.DeleteObject();
}

/*******************************************************************************
  Function Name  : CColourPopup
  Input(s)       : p - Point of show of this dialog
                   crColor - Selected color
                   pParentWnd - Parent window pointer
                   szDefaultText - Default Text Pointer
                   szCustomText - Custom Text Pointer
  Output         : BOOL - Success Value. TRUE on success. FALSE otherwise
  Functionality  : Creates colour pallete window
  Member of      : CColourPopup
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
BOOL CColourPopup::Create(CPoint p, COLORREF crColour, CWnd* pParentWnd,
                          LPCTSTR szDefaultText /* = NULL */,
                          LPCTSTR szCustomText  /* = NULL */)
{
    // Check parent window pointer's validity
    ASSERT(pParentWnd && ::IsWindow(pParentWnd->GetSafeHwnd()));
    ASSERT(pParentWnd->IsKindOf(RUNTIME_CLASS(CColourPicker)));
    // Assign it ot member variable
    m_pParent  = pParentWnd;
    m_crColour = m_crInitialColour = crColour;
    // Get the class name and create the window
    CString szClassName =
        AfxRegisterWndClass( CS_CLASSDC | CS_SAVEBITS | CS_HREDRAW | CS_VREDRAW,
                             0, (HBRUSH) (COLOR_BTNFACE+1), 0);

    if (!CWnd::CreateEx(0, szClassName, STR_EMPTY, WS_VISIBLE,
                        p.x, p.y, 100, 100, // Use default size
                        pParentWnd->GetSafeHwnd(), 0, NULL))
    {
        return FALSE;
    }

    // Store the Custom text
    if (szCustomText != NULL)
    {
        m_strCustomText = szCustomText;
    }

    // Store the Default Area text
    if (szDefaultText != NULL)
    {
        m_strDefaultText = szDefaultText;
    }

    // Set the window size
    SetWindowSize();
    // Create the tooltips
    CreateToolTips();
    // Find which cell (if any) corresponds to the initial colour
    FindCellFromColour(crColour);
    // Capture all mouse events for the life of this window
    SetCapture();
    return TRUE;
}

BEGIN_MESSAGE_MAP(CColourPopup, CWnd)
    //{{AFX_MSG_MAP(CColourPopup)
    ON_WM_NCDESTROY()
    ON_WM_LBUTTONUP()
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
    ON_WM_KEYDOWN()
    ON_WM_QUERYNEWPALETTE()
    ON_WM_PALETTECHANGED()
    ON_WM_KILLFOCUS()
    ON_WM_ACTIVATEAPP()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/*******************************************************************************
  Function Name  : PreTranslateMessage
  Input(s)       : pMsg - Pointer to Windows Message Struct
  Output         : -
  Functionality  : This function captures messages to popup tooltip messages
  Member of      : CColourPopup
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
BOOL CColourPopup::PreTranslateMessage(MSG* pMsg)
{
    m_ToolTip.RelayEvent(pMsg);

    // Set the focus with in this window
    if (GetCapture()->GetSafeHwnd() != m_hWnd)
    {
        SetCapture();
    }

    return CWnd::PreTranslateMessage(pMsg);
}

/*******************************************************************************
  Function Name  : OnKeyDown
  Input(s)       : nChar - Key Code, nRepCnt - Repeat Count,
                   nFlags - Ctrl and Alt key status
  Output         : -
  Functionality  : This function moves the selection if arrow keys are pressed
  Member of      : CColourPopup
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CColourPopup::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    int row = GetRow(m_nCurrentSel),
        col = GetColumn(m_nCurrentSel);

    // Move Selection
    if (nChar == VK_DOWN)
    {
        // Wrap to top
        if (row == DEFAULT_BOX_VALUE)
        {
            row = col = 0;
        }
        else if (row == CUSTOM_BOX_VALUE)
        {
            if ( m_strDefaultText.GetLength() > 0 )
            {
                row = col = DEFAULT_BOX_VALUE;
            }
            else
            {
                row = col = 0;
            }
        }
        else
        {
            // Increment row count
            row++;

            if ( GetIndex(row,col) < 0)
            {
                if (m_strCustomText.GetLength() > 0)
                {
                    row = col = CUSTOM_BOX_VALUE;
                }
                else if (m_strDefaultText.GetLength() > 0)
                {
                    row = col = DEFAULT_BOX_VALUE;
                }
                else
                {
                    row = col = 0;
                }
            }
        }

        // Update Selection
        ChangeSelection(GetIndex(row, col));
    }

    // Move One cell Up
    if (nChar == VK_UP)
    {
        if (row == DEFAULT_BOX_VALUE)
        {
            if (m_strCustomText.GetLength() > 0)
            {
                row = col = CUSTOM_BOX_VALUE;
            }
            else
            {
                row = GetRow(m_nNumColours-1);
                col = GetColumn(m_nNumColours-1);
            }
        }
        else if (row == CUSTOM_BOX_VALUE)
        {
            row = GetRow(m_nNumColours-1);
            col = GetColumn(m_nNumColours-1);
        }
        else if (row > 0)
        {
            row--;
        }
        else
        {
            if (m_strDefaultText.GetLength() > 0)
            {
                row = col = DEFAULT_BOX_VALUE;
            }
            else if( m_strCustomText.GetLength() > 0)
            {
                row = col = CUSTOM_BOX_VALUE;
            }
            else
            {
                row = GetRow(m_nNumColours-1);
                col = GetColumn(m_nNumColours-1);
            }
        }

        // Update Selection
        ChangeSelection(GetIndex(row, col));
    }

    // Move One Cell Right
    if (nChar == VK_RIGHT)
    {
        if (row == DEFAULT_BOX_VALUE)
        {
            row = col = 0;
        }
        else if (row == CUSTOM_BOX_VALUE)
        {
            if (m_strDefaultText.GetLength())
            {
                row = col = DEFAULT_BOX_VALUE;
            }
            else
            {
                row = col = 0;
            }
        }
        else if (col < m_nNumColumns - 1)
        {
            col++;
        }
        else
        {
            // Move to Col 0 of next row
            col = 0;
            row++;
        }

        if (GetIndex(row,col) == INVALID_COLOUR)
        {
            if (m_strCustomText.GetLength() > 0)
            {
                row = col = CUSTOM_BOX_VALUE;
            }
            else if ( m_strDefaultText.GetLength() > 0)
            {
                row = col = DEFAULT_BOX_VALUE;
            }
            else
            {
                row = col = 0;
            }
        }

        // Update Selection
        ChangeSelection(GetIndex(row, col));
    }

    // Move one cell left
    if (nChar == VK_LEFT)
    {
        if (row == DEFAULT_BOX_VALUE)
        {
            if (m_strCustomText.GetLength() > 0)
            {
                row = col = CUSTOM_BOX_VALUE;
            }
            else
            {
                row = GetRow(m_nNumColours-1);
                col = GetColumn(m_nNumColours-1);
            }
        }
        else if (row == CUSTOM_BOX_VALUE)
        {
            row = GetRow(m_nNumColours-1);
            col = GetColumn(m_nNumColours-1);
        }
        else if (col > 0)
        {
            col--;
        }
        else  // col == 0
        {
            if (row > 0)
            {
                row--;
                col = m_nNumColumns - 1;
            }
            else
            {
                if (m_strDefaultText.GetLength() > 0)
                {
                    row = col = DEFAULT_BOX_VALUE;
                }
                else if (m_strCustomText.GetLength() > 0)
                {
                    row = col = CUSTOM_BOX_VALUE;
                }
                else
                {
                    row = GetRow(m_nNumColours-1);
                    col = GetColumn(m_nNumColours-1);
                }
            }
        }

        // Update Selection
        ChangeSelection(GetIndex(row, col));
    }

    // Hide This dialog on press of escape key
    if (nChar == VK_ESCAPE)
    {
        // Restore initial color
        m_crColour = m_crInitialColour;
        // Close the session
        EndSelection(WM_CPN_SELENDCANCEL);
        return;
    }

    // On Press of enter or space select the selection and close this
    // dialog
    if (nChar == VK_RETURN || nChar == VK_SPACE)
    {
        EndSelection(WM_CPN_SELENDOK);
        return;
    }

    // Else call default procedure
    CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

/*******************************************************************************
  Function Name  : OnNcDestroy
  Input(s)       : -
  Output         : -
  Functionality  : Deletes the memort allocated for this window
  Member of      : CColourPopup
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CColourPopup::OnNcDestroy()
{
    // Destroy the window first
    CWnd::OnNcDestroy();
    // Delete the memory
    delete this;
}

/*******************************************************************************
  Function Name  : OnPaint
  Input(s)       : -
  Output         : -
  Functionality  : Draws the color pallete, selection and user text.
  Member of      : CColourPopup
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CColourPopup::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    // Draw the Default Area text
    if (m_strDefaultText.GetLength() > 0)
    {
        DrawCell(&dc, DEFAULT_BOX_VALUE);
    }

    // Draw colour cells
    for (int i = 0; i < m_nNumColours; i++)
    {
        DrawCell(&dc, i);
    }

    // Draw custom text
    if (m_strCustomText.GetLength() > 0)
    {
        DrawCell(&dc, CUSTOM_BOX_VALUE);
    }

    // Draw raised window edge
    CRect rect;
    GetClientRect(rect);
    dc.DrawEdge(rect, EDGE_RAISED, BF_RECT);
}

/*******************************************************************************
  Function Name  : OnMouseMove
  Input(s)       : nFlags - Mouse Button Status
                   point - Cursor point
  Output         : -
  Functionality  : Moves the focus rect to the selected point
  Member of      : CColourPopup
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CColourPopup::OnMouseMove(UINT nFlags, CPoint point)
{
    int nNewSelection = INVALID_COLOUR;
    // Translate points to be relative raised window edge
    point.x -= m_nMargin;
    point.y -= m_nMargin;

    // check whether the selection isn't in the text box
    if( m_strCustomText.GetLength() > 0 &&
            m_CustomTextRect.PtInRect(point) )
    {
        nNewSelection = CUSTOM_BOX_VALUE;
    }
    else if( m_strDefaultText.GetLength() > 0 &&
             m_DefaultTextRect.PtInRect(point) )
    {
        nNewSelection = DEFAULT_BOX_VALUE;
    }
    else
    {
        // Take into account text box
        if (m_strDefaultText.GetLength() > 0)
        {
            point.y -= m_DefaultTextRect.Height();
        }

        // Get the row and column
        nNewSelection = GetIndex( point.y / m_nBoxSize,
                                  point.x / m_nBoxSize );

        // If not in the range then call def proc and return
        if (nNewSelection < 0 || nNewSelection >= m_nNumColours)
        {
            CWnd::OnMouseMove(nFlags, point);
            return;
        }
    }

    // Draw focus rect if it is different from current one
    if( nNewSelection != m_nCurrentSel )
    {
        ChangeSelection(nNewSelection);
    }

    CWnd::OnMouseMove(nFlags, point);
}

/*******************************************************************************
  Function Name  : OnLButtonUp
  Input(s)       : nFlags - Mouse Button Status
                   point - Cursor point
  Output         : -
  Functionality  : This function will be called during left muse button up. This
                   will end the selection.
  Member of      : CColourPopup
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CColourPopup::OnLButtonUp(UINT nFlags, CPoint point)
{
    CWnd::OnLButtonUp(nFlags, point);
    DWORD pos = GetMessagePos();
    point = CPoint(LOWORD(pos), HIWORD(pos));

    if (m_WindowRect.PtInRect(point))
    {
        EndSelection(WM_CPN_SELENDOK);
    }
    else
    {
        EndSelection(WM_CPN_SELENDCANCEL);
    }
}

/*******************************************************************************
  Function Name  : GetIndex
  Input(s)       : row, col - matrix index
  Output         : int - Color table index
  Functionality  : This function returns the color table index of the selection
  Member of      : CColourPopup
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
int CColourPopup::GetIndex(int row, int col) const
{
    if ((row == CUSTOM_BOX_VALUE || col == CUSTOM_BOX_VALUE) &&
            m_strCustomText.GetLength() > 0 )
    {
        return CUSTOM_BOX_VALUE;
    }
    else if(( row == DEFAULT_BOX_VALUE || col == DEFAULT_BOX_VALUE) &&
            m_strDefaultText.GetLength() > 0 )
    {
        return DEFAULT_BOX_VALUE;
    }
    else if( row < 0 || col < 0 ||
             row >= m_nNumRows || col >= m_nNumColumns )
    {
        return INVALID_COLOUR;
    }
    else
    {
        if( row* m_nNumColumns + col >= m_nNumColours )
        {
            return INVALID_COLOUR;
        }
        else
        {
            return row * m_nNumColumns + col;
        }
    }
}

/*******************************************************************************
  Function Name  : GetRow
  Input(s)       : nIndex - Index of the color table
  Output         : int - Row corresponds to the color rect
  Functionality  : Returns the Row corresponds to the color rect
  Member of      : CColourPopup
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
int CColourPopup::GetRow(int nIndex) const
{
    if( nIndex == CUSTOM_BOX_VALUE && m_strCustomText.GetLength() > 0 )
    {
        return CUSTOM_BOX_VALUE;
    }
    else if( nIndex == DEFAULT_BOX_VALUE && m_strDefaultText.GetLength() > 0 )
    {
        return DEFAULT_BOX_VALUE;
    }
    else if( nIndex < 0 || nIndex >= m_nNumColours )
    {
        return INVALID_COLOUR;
    }
    else
    {
        return nIndex / m_nNumColumns;
    }
}

/*******************************************************************************
  Function Name  : GetColumn
  Input(s)       : nIndex - Index of the color table
  Output         : int - Col corresponds to the color rect
  Functionality  : Returns the Col corresponds to the color rect
  Member of      : CColourPopup
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
int CColourPopup::GetColumn(int nIndex) const
{
    if( nIndex == CUSTOM_BOX_VALUE && m_strCustomText.GetLength() > 0)
    {
        return CUSTOM_BOX_VALUE;
    }
    else if( nIndex == DEFAULT_BOX_VALUE && m_strDefaultText.GetLength() > 0)
    {
        return DEFAULT_BOX_VALUE;
    }
    else if( nIndex < 0 || nIndex >= m_nNumColours )
    {
        return INVALID_COLOUR;
    }
    else
    {
        return nIndex % m_nNumColumns;
    }
}

/*******************************************************************************
  Function Name  : FindCellFromColour
  Input(s)       : crColour - Color for which cell to be identified
  Output         : -
  Functionality  : Updates the selection row and col from the color
  Member of      : CColourPopup
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CColourPopup::FindCellFromColour(COLORREF crColour)
{
    if (crColour == CLR_DEFAULT && m_strDefaultText.GetLength() > 0)
    {
        m_nChosenColourSel = DEFAULT_BOX_VALUE;
        return;
    }

    // Iterate through list of entries in the color table
    for (int i = 0; i < m_nNumColours; i++)
    {
        if (GetColour(i) == crColour)
        {
            m_nChosenColourSel = i;
            return;
        }
    }

    if( m_strCustomText.GetLength() > 0 )
    {
        m_nChosenColourSel = CUSTOM_BOX_VALUE;
    }
    else
    {
        m_nChosenColourSel = INVALID_COLOUR;
    }
}

/*******************************************************************************
  Function Name  : GetCellRect
  Input(s)       : nIndex - Type of the entry
                   rect - Boundary of the entry
  Output         : BOOL - TRUE on Success
  Functionality  : Gets the dimensions of the colour cell given by (row,col)
  Member of      : CColourPopup
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
BOOL CColourPopup::GetCellRect(int nIndex, const LPRECT& rect)
{
    if (nIndex == CUSTOM_BOX_VALUE)
    {
        ::SetRect(rect,
                  m_CustomTextRect.left,  m_CustomTextRect.top,
                  m_CustomTextRect.right, m_CustomTextRect.bottom);
        return TRUE;
    }
    else if (nIndex == DEFAULT_BOX_VALUE)
    {
        ::SetRect(rect,
                  m_DefaultTextRect.left,  m_DefaultTextRect.top,
                  m_DefaultTextRect.right, m_DefaultTextRect.bottom);
        return TRUE;
    }

    if (nIndex < 0 || nIndex >= m_nNumColours )
    {
        return FALSE;
    }

    rect->left = GetColumn(nIndex) * m_nBoxSize + m_nMargin;
    rect->top  = GetRow(nIndex) * m_nBoxSize + m_nMargin;

    // Move everything down if we are displaying a default text area
    if( m_strDefaultText.GetLength() > 0 )
    {
        rect->top += (m_nMargin + m_DefaultTextRect.Height());
    }

    rect->right = rect->left + m_nBoxSize;
    rect->bottom = rect->top + m_nBoxSize;
    return TRUE;
}

/*******************************************************************************
  Function Name  : SetWindowSize
  Input(s)       : -
  Output         : -
  Functionality  : Calculates appropriate size and position of this window
  Member of      : CColourPopup
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CColourPopup::SetWindowSize()
{
    CSize TextSize;

    // If we are showing a custom or default text area,
    // get the font and text size.
    if ( m_strCustomText.GetLength() > 0 ||
            m_strDefaultText.GetLength() > 0 )
    {
        CClientDC dc(this);
        CFont* pOldFont = (CFont*) dc.SelectObject(&m_Font);
        // Get the size of the custom text (if there IS custom text)
        TextSize = CSize(0,0);

        if( m_strCustomText.GetLength() > 0)
        {
            TextSize = dc.GetTextExtent(m_strCustomText);
        }

        // Get the size of the default text (if there IS default text)
        if( m_strDefaultText.GetLength() > 0 )
        {
            CSize DefaultSize = dc.GetTextExtent(m_strDefaultText);

            if( DefaultSize.cx > TextSize.cx )
            {
                TextSize.cx = DefaultSize.cx;
            }

            if( DefaultSize.cy > TextSize.cy )
            {
                TextSize.cy = DefaultSize.cy;
            }
        }

        // Restore Old Object
        dc.SelectObject(pOldFont);
        TextSize += CSize( 2 * m_nMargin, 2 * m_nMargin );
        // Add even more space to draw the horizontal line
        TextSize.cy += 2*m_nMargin + 2;
    }

    // Get the number of columns and rows
    //m_nNumColumns = (int) sqrt((double)m_nNumColours);
    // for a square window
    m_nNumColumns = 8;
    m_nNumRows = m_nNumColours / m_nNumColumns;

    if (m_nNumColours % m_nNumColumns)
    {
        m_nNumRows++;
    }

    // Get the current window position, and set the new size
    CRect rect;
    GetWindowRect(rect);
    m_WindowRect.SetRect( rect.left, rect.top,
                          rect.left + m_nNumColumns * m_nBoxSize + 2 * m_nMargin,
                          rect.top  + m_nNumRows * m_nBoxSize + 2 * m_nMargin );

    // if custom text, then expand window if necessary, and set text width as
    // window width
    if( m_strDefaultText.GetLength() > 0)
    {
        if( TextSize.cx > m_WindowRect.Width())
        {
            m_WindowRect.right = m_WindowRect.left + TextSize.cx;
        }

        TextSize.cx = m_WindowRect.Width() - 2 * m_nMargin;
        // Work out the text area
        m_DefaultTextRect.SetRect( m_nMargin, m_nMargin,
                                   m_nMargin + TextSize.cx, 2 * m_nMargin + TextSize.cy );
        m_WindowRect.bottom += m_DefaultTextRect.Height() + 2 * m_nMargin;
    }

    // if custom text, then expand window if necessary, and set text width as
    // window width
    if (m_strCustomText.GetLength() > 0)
    {
        if (TextSize.cx > m_WindowRect.Width())
        {
            m_WindowRect.right = m_WindowRect.left + TextSize.cx;
        }

        TextSize.cx = m_WindowRect.Width() - 2 * m_nMargin;
        // Work out the text area
        m_CustomTextRect.SetRect( m_nMargin, m_WindowRect.Height(),
                                  m_nMargin + TextSize.cx,
                                  m_WindowRect.Height() + m_nMargin + TextSize.cy );
        m_WindowRect.bottom += m_CustomTextRect.Height() + 2 * m_nMargin + 25;
        m_WindowRect.right += 5;
    }

    // Check it'll fit on screen:
    CSize ScreenSize(
        ::GetSystemMetrics(SM_CXSCREEN),
        ::GetSystemMetrics(SM_CYSCREEN) );

    if( m_WindowRect.right > ScreenSize.cx )
    {
        // Shift the position
        m_WindowRect.OffsetRect( -( m_WindowRect.right - ScreenSize.cx ), 0);
    }

    // Check for Too far left
    if (m_WindowRect.left < 0)
    {
        m_WindowRect.OffsetRect( -m_WindowRect.left, 0 );
    }

    // Bottom falling out of screen
    if( m_WindowRect.bottom > ScreenSize.cy )
    {
        CRect ParentRect;
        m_pParent->GetWindowRect( ParentRect );
        m_WindowRect.OffsetRect( 0,
                                 -(ParentRect.Height() + m_WindowRect.Height()));
    }

    // Set the window size and position
    MoveWindow(m_WindowRect, TRUE);
}

/*******************************************************************************
  Function Name  : CreateToolTips
  Input(s)       : -
  Output         : -
  Functionality  : Create tool tips for color table entries
  Member of      : CColourPopup
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CColourPopup::CreateToolTips()
{
    // Create the tool tip
    if (!m_ToolTip.Create(this))
    {
        return;
    }

    // Add a tool for each cell
    for (int i = 0; i < m_nNumColours; i++)
    {
        CRect rect;

        if(GetCellRect(i, rect))
        {
            m_ToolTip.AddTool( this, GetColourName(i), rect, 1);
        }
    }
}

/*******************************************************************************
  Function Name  : ChangeSelection
  Input(s)       : nIndex - New Index of the selection
  Output         : -
  Functionality  : Changes the selection to new index
  Member of      : CColourPopup
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CColourPopup::ChangeSelection(int nIndex)
{
    CClientDC dc(this);        // device context for drawing

    if (nIndex > m_nNumColours)
    {
        nIndex = CUSTOM_BOX_VALUE;
    }

    if ((m_nCurrentSel >= 0 && m_nCurrentSel < m_nNumColours) ||
            m_nCurrentSel == CUSTOM_BOX_VALUE || m_nCurrentSel == DEFAULT_BOX_VALUE)
    {
        // Set Current selection as invalid and redraw old selection (this way
        // the old selection will be drawn unselected)
        int OldSel = m_nCurrentSel;
        m_nCurrentSel = INVALID_COLOUR;
        DrawCell(&dc, OldSel);
    }

    // Set the current selection as row/col and draw (it will be drawn selected)
    m_nCurrentSel = nIndex;
    DrawCell(&dc, m_nCurrentSel);

    // Store the current colour
    if (m_nCurrentSel == CUSTOM_BOX_VALUE)
    {
        m_pParent->SendMessage(WM_CPN_SELCHANGE, (WPARAM) m_crInitialColour, 0);
    }
    else if (m_nCurrentSel == DEFAULT_BOX_VALUE)
    {
        m_crColour = CLR_DEFAULT;
        m_pParent->SendMessage(WM_CPN_SELCHANGE, (WPARAM) CLR_DEFAULT, 0);
    }
    else
    {
        m_crColour = GetColour(m_nCurrentSel);
        m_pParent->SendMessage(WM_CPN_SELCHANGE, (WPARAM) m_crColour, 0);
    }
}

/*******************************************************************************
  Function Name  : EndSelection
  Input(s)       : nMessage - Reason for End Selection
  Output         : -
  Functionality  : Complets tracks and closes the session
  Member of      : CColourPopup
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CColourPopup::EndSelection(int nMessage)
{
    ReleaseCapture();

    // If custom text selected, perform a custom colour selection
    if (nMessage != WM_CPN_SELENDCANCEL && m_nCurrentSel == CUSTOM_BOX_VALUE)
    {
        m_bChildWindowVisible = TRUE;
        CColorDialog dlg(m_crInitialColour, CC_FULLOPEN | CC_ANYCOLOR, this);

        if (dlg.DoModal() == IDOK)
        {
            m_crColour = dlg.GetColor();
        }
        else
        {
            nMessage = WM_CPN_SELENDCANCEL;
        }

        m_bChildWindowVisible = FALSE;
    }

    if (nMessage == WM_CPN_SELENDCANCEL)
    {
        m_crColour = m_crInitialColour;
    }

    m_pParent->SendMessage(nMessage, (WPARAM) m_crColour, 0);
    // Set the focus to the parent before kill present window
    m_pParent->SetFocus();

    if (!m_bChildWindowVisible)
    {
        DestroyWindow();
    }
}

/*******************************************************************************
  Function Name  : DrawCell
  Input(s)       : pDC- Pointer to Device Context
                   nIndex - Index of the cell
  Output         : -
  Functionality  : Draws the color cell pointer by the index nIndex
  Member of      : CColourPopup
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CColourPopup::DrawCell(CDC* pDC, int nIndex)
{
    // For the Custom Text area
    if (m_strCustomText.GetLength() && nIndex == CUSTOM_BOX_VALUE)
    {
        // The extent of the actual text button
        CRect TextButtonRect = m_CustomTextRect;
        TextButtonRect.top += 2*m_nMargin;
        // Fill background
        pDC->FillSolidRect( TextButtonRect, ::GetSysColor(COLOR_3DFACE));
        // Draw horizontal line
        pDC->FillSolidRect( m_CustomTextRect.left + 2 * m_nMargin,
                            m_CustomTextRect.top,
                            m_CustomTextRect.Width() - 4 * m_nMargin,
                            1,
                            ::GetSysColor(COLOR_3DSHADOW) );
        pDC->FillSolidRect( m_CustomTextRect.left + 2 * m_nMargin,
                            m_CustomTextRect.top + 1,
                            m_CustomTextRect.Width() - 4 * m_nMargin,
                            1,
                            ::GetSysColor(COLOR_3DHILIGHT) );
        TextButtonRect.DeflateRect(1,1);

        // fill background
        if( m_nChosenColourSel == nIndex &&
                m_nCurrentSel != nIndex )
        {
            pDC->FillSolidRect( TextButtonRect, ::GetSysColor(COLOR_3DLIGHT));
        }
        else
        {
            pDC->FillSolidRect( TextButtonRect, ::GetSysColor(COLOR_3DFACE));
        }

        // Draw button
        if ( m_nCurrentSel == nIndex )
        {
            pDC->DrawEdge(TextButtonRect, BDR_RAISEDINNER, BF_RECT);
        }
        else if( m_nChosenColourSel == nIndex )
        {
            pDC->DrawEdge(TextButtonRect, BDR_SUNKENOUTER, BF_RECT);
        }

        // Draw custom text
        CFont* pOldFont = (CFont*) pDC->SelectObject(&m_Font);
        pDC->SetBkMode(TRANSPARENT);
        pDC->DrawText( m_strCustomText,
                       TextButtonRect,
                       DT_CENTER | DT_VCENTER | DT_SINGLELINE );
        pDC->SelectObject(pOldFont);
        return;
    }

    // For the Default Text area
    if ( m_strDefaultText.GetLength() > 0 &&
            nIndex == DEFAULT_BOX_VALUE )
    {
        // Fill background
        pDC->FillSolidRect( m_DefaultTextRect, ::GetSysColor(COLOR_3DFACE) );
        // The extent of the actual text button
        CRect TextButtonRect = m_DefaultTextRect;
        TextButtonRect.DeflateRect(1,1);

        // fill background
        if ( m_nChosenColourSel == nIndex && m_nCurrentSel != nIndex )
        {
            pDC->FillSolidRect(TextButtonRect, ::GetSysColor(COLOR_3DLIGHT));
        }
        else
        {
            pDC->FillSolidRect(TextButtonRect, ::GetSysColor(COLOR_3DFACE));
        }

        // Draw thin line around text
        CRect LineRect = TextButtonRect;
        LineRect.DeflateRect( 2 * m_nMargin, 2 * m_nMargin );
        CPen pen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
        CPen* pOldPen = pDC->SelectObject(&pen);
        pDC->SelectStockObject(NULL_BRUSH);
        pDC->Rectangle(LineRect);
        pDC->SelectObject(pOldPen);

        // Draw button
        if (m_nCurrentSel == nIndex)
        {
            pDC->DrawEdge(TextButtonRect, BDR_RAISEDINNER, BF_RECT);
        }
        else if (m_nChosenColourSel == nIndex)
        {
            pDC->DrawEdge(TextButtonRect, BDR_SUNKENOUTER, BF_RECT);
        }

        // Draw custom text
        CFont* pOldFont = (CFont*) pDC->SelectObject(&m_Font);
        pDC->SetBkMode(TRANSPARENT);
        pDC->DrawText( m_strDefaultText,
                       TextButtonRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        pDC->SelectObject(pOldFont);
        return;
    }

    CRect rect;

    if (!GetCellRect(nIndex, rect))
    {
        return;
    }

    // Select and realize the palette
    CPalette* pOldPalette = NULL;

    if (pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
    {
        pOldPalette = pDC->SelectPalette(&m_Palette, FALSE);
        pDC->RealizePalette();
    }

    // fill background
    if (m_nChosenColourSel == nIndex && m_nCurrentSel != nIndex)
    {
        pDC->FillSolidRect(rect, ::GetSysColor(COLOR_3DHILIGHT));
    }
    else
    {
        pDC->FillSolidRect(rect, ::GetSysColor(COLOR_3DFACE));
    }

    // Draw button
    if (m_nCurrentSel == nIndex)
    {
        pDC->DrawEdge(rect, BDR_RAISEDINNER, BF_RECT);
    }
    else if (m_nChosenColourSel == nIndex)
    {
        pDC->DrawEdge(rect, BDR_SUNKENOUTER, BF_RECT);
    }

    CBrush brush( PALETTERGB( GetRValue(GetColour(nIndex)),
                              GetGValue(GetColour(nIndex)),
                              GetBValue(GetColour(nIndex)) ));
    CPen   pen;
    pen.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_3DSHADOW));
    CBrush* pOldBrush = (CBrush*) pDC->SelectObject(&brush);
    CPen*   pOldPen   = (CPen*)   pDC->SelectObject(&pen);
    // Draw the cell colour
    rect.DeflateRect(m_nMargin+1, m_nMargin+1);
    pDC->Rectangle(rect);
    // restore DC and cleanup
    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);
    brush.DeleteObject();
    pen.DeleteObject();

    if (pOldPalette && pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
    {
        pDC->SelectPalette(pOldPalette, FALSE);
    }
}

/*******************************************************************************
  Function Name  : OnQueryNewPalette
  Input(s)       : -
  Output         : -
  Functionality  :
  Member of      : CColourPopup
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
BOOL CColourPopup::OnQueryNewPalette()
{
    Invalidate();
    return CWnd::OnQueryNewPalette();
}

/*******************************************************************************
  Function Name  : OnPaletteChanged
  Input(s)       : -
  Output         : -
  Functionality  :
  Member of      : CColourPopup
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CColourPopup::OnPaletteChanged(CWnd* pFocusWnd)
{
    CWnd::OnPaletteChanged(pFocusWnd);

    if (pFocusWnd->GetSafeHwnd() != GetSafeHwnd())
    {
        Invalidate();
    }
}

/*******************************************************************************
  Function Name  : OnKillFocus
  Input(s)       : -
  Output         : -
  Functionality  :
  Member of      : CColourPopup
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
void CColourPopup::OnKillFocus(CWnd* pNewWnd)
{
    CWnd::OnKillFocus(pNewWnd);
    ReleaseCapture();
}

/*******************************************************************************
  Function Name  : OnActivateApp
  Input(s)       : -
  Output         : -
  Functionality  :
  Member of      : CColourPopup
  Author(s)      : Raja N
  Date Created   : 09/12/2004
  Modifications  :
*******************************************************************************/
#if _MFC_VER <= 0x0600
void CColourPopup::OnActivateApp(BOOL bActive, HTASK hTask)
{
    CWnd::OnActivateApp(bActive,hTask);
#else
void CColourPopup::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
    CWnd::OnActivateApp(bActive,dwThreadID);
#endif

    // If Deactivating App, cancel this selection
    if (!bActive)
    {
        EndSelection(WM_CPN_SELENDCANCEL);
    }
}

/*******************************************************************************
  Function Name  : GetColour
  Input(s)       : -
  Output         : -
  Functionality  : To get the colour at the index
  Member of      : CColourPopup
  Author(s)      : Raja N
  Date Created   : 14/12/2004
  Modifications  :
*******************************************************************************/
COLORREF CColourPopup::GetColour(int nIndex)
{
    return m_crColours[nIndex].crColour;
}

/*******************************************************************************
  Function Name  : GetColourName
  Input(s)       : -
  Output         : -
  Functionality  : To get tehe name associated with the color
  Member of      : CColourPopup
  Author(s)      : Raja N
  Date Created   : 14/12/2004
  Modifications  :
*******************************************************************************/
LPCTSTR CColourPopup::GetColourName(int nIndex)
{
    return m_crColours[nIndex].szName;
}
