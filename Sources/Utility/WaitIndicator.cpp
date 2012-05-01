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
 * \file      WaitIndicator.cpp
 * \brief     This is the implementation file for CWaitIndicator class.
 * \author    Venkatanarayana Makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This is the implementation file for CWaitIndicator class.
 */

#include "Utils_stdafx.h"         // For standard include
#include "WaitIndicator.h"


// CWaitIndicator

IMPLEMENT_DYNAMIC(CWaitIndicator, CWnd)

BEGIN_MESSAGE_MAP(CWaitIndicator, CWnd)
    ON_WM_PAINT()
    ON_WM_CREATE()
END_MESSAGE_MAP()

/******************************************************************************
Function Name   : CWaitIndicator
Input(s)        : -
Output          : -
Functionality   : Constructor of Wait Indicator.
Member of       : CWaitIndicator
Friend of       : -
Author(s)       : Venkatanarayana Makam
Date Created    :
Modifications   :
******************************************************************************/
CWaitIndicator::CWaitIndicator()
{
    //Register Custom window class name
    RegisterWindowClass();
}

/******************************************************************************
Function Name   : ~CWaitIndicator
Input(s)        : -
Output          : -
Functionality   : Destrutor of Wait Indicator.
Member of       : CWaitIndicator
Friend of       : -
Author(s)       : Venkatanarayana Makam
Date Created    :
Modifications   :
******************************************************************************/
CWaitIndicator::~CWaitIndicator()
{
}

/******************************************************************************
Function Name   : DisplayWindow
Input(s)        : 1. strTitle - Contain the title that has to diaplay on
                  Wait indicator window
                  2. pParentWnd - contains the pointer to the parent window
                  3. nID - ID of the control.Default to Zero

Output          : BOOL - Nonzero if successful, otherwise 0.
Functionality   : Creates and Displays the indicator window.
Member of       : CWaitIndicator
Friend of       : -
Author(s)       : Venkatanarayana Makam
Date Created    :
Modifications   :
******************************************************************************/
BOOL CWaitIndicator::DisplayWindow(CString strTitle, CWnd* pParentWnd, UINT nID )
{
    //save the Title
    m_strTitle = strTitle;
    //Setting window properties.
    DWORD dwStyle = WS_CHILD | SS_CENTER | WS_VISIBLE;
    //Create window with zero size.
    CWnd::Create(INDICATOR_CLASSNAME, "", dwStyle, CRect(0,0,0,0), pParentWnd, nID, NULL);
    Redraw();
    return TRUE;
}

/******************************************************************************
Function Name   : OnPaint
Input(s)        : -
Output          : void
Functionality   : Draws the Indicators client window
Member of       : CWaitIndicator
Friend of       : -
Author(s)       : Venkatanarayana Makam
Date Created    :
Modifications   :
******************************************************************************/
void CWaitIndicator::OnPaint(void)
{
    // Device context for painting
    CPaintDC dc(this);
    /*CRect ClientRect;

    // Client Rect Painting
    GetClientRect(&ClientRect);
    CBrush omBrush((COLORREF) GetSysColor(COLOR_ACTIVECAPTION));
    dc.FrameRect(&ClientRect, &omBrush);

    // Load Font Deatils
    dc.SetBkMode(TRANSPARENT);

    // Finally Set The text;
    dc.DrawText(m_strTitle, &ClientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);*/
    PaintWindow(dc);
}

/******************************************************************************
Function Name   : OnCreate
Input(s)        : lpCreateStruct - Contains the details of window such as
                  width, height, window style.
Output          : int
Functionality   : Resizes and Moves the window to the center of its parent
Member of       : CWaitIndicator
Friend of       : -
Author(s)       : Venkatanarayana Makam
Date Created    :
Modifications   :
******************************************************************************/
int CWaitIndicator::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    //To resize the window and to move the window to the center of parent
    MoveToCenter();
    return 0;
}

/******************************************************************************
Function Name   : MoveToCenter
Input(s)        : -
Output          : void
Functionality   : Moves the Indicator Window to center of its parents
Member of       : CWaitIndicator
Friend of       : -
Author(s)       : Venkatanarayana Makam
Date Created    :
Modifications   :
******************************************************************************/
void CWaitIndicator::MoveToCenter(void)
{
    CPoint   Point;
    CRect    ParentRect;
    int      nWidth;
    int      nHeight;
    CWnd*     MainWindow;
    MainWindow = GetParent();

    if (MainWindow != NULL)
    {
        MainWindow->GetWindowRect(ParentRect);
    }

    CClientDC dc(this);
    CSize TxtSize = dc.GetTextExtent(m_strTitle);
    // 1.1 Times the Text Length;
    nWidth =  (int) (1.1 * TxtSize.cx);
    // Fixed.Height is always Fixed;
    nHeight = 45;
    // Find the center point and convert to screen coordinates.
    Point.x = ParentRect.Width() / 2;
    Point.y = ParentRect.Height() / 2;
    Point.x -= nWidth /2;
    Point.y -= nHeight;
    MoveWindow(Point.x, Point.y, nWidth, nHeight, TRUE);
}

/******************************************************************************
Function Name   : RegisterWindowClass
Input(s)        : -
Output          : BOOL
Functionality   : Register the Indicator window as a custom Window control.
Member of       : CWaitIndicator
Friend of       : -
Author(s)       : Venkatanarayana Makam
Date Created    :
Modifications   :
******************************************************************************/
BOOL CWaitIndicator::RegisterWindowClass(void)
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, INDICATOR_CLASSNAME, &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW ;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_ACTIVEBORDER + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = INDICATOR_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

/******************************************************************************
Function Name   : CloseWindow
Input(s)        : -
Output          : void
Functionality   : Closes the indicator window if its open.
Member of       : CWaitIndicator
Friend of       : -
Author(s)       : Venkatanarayana Makam
Date Created    :
Modifications   :
******************************************************************************/
void CWaitIndicator::CloseWindow(void)
{
    DestroyWindow();
}

/******************************************************************************
Function Name   : Redraw
Input(s)        : -
Output          : void
Functionality   : Redraws the window.
Member of       : CWaitIndicator
Friend of       : -
Author(s)       : Venkatanarayana Makam
Date Created    :
Modifications   :
******************************************************************************/
void CWaitIndicator::Redraw(void)
{
    CClientDC dc(this);
    PaintWindow(dc);
    PaintWindow(dc);
}
void CWaitIndicator::SetWindowText(TCHAR* acText)
{
    m_strTitle = acText;
    Redraw();
}
void CWaitIndicator::PaintWindow(CDC& dc)
{
    CRect ClientRect;
    // Client Rect Painting
    GetClientRect(&ClientRect);
    CBrush omBrush((COLORREF) GetSysColor(COLOR_ACTIVECAPTION));
    // Load Font Deatils
    dc.SetBkMode(TRANSPARENT);
    CBrush omBkgBrush((COLORREF)GetSysColor(COLOR_ACTIVEBORDER));
    dc.FillRect(ClientRect, &omBkgBrush);
    dc.FrameRect(&ClientRect, &omBrush);
    // Finally Set The text;
    dc.DrawText(m_strTitle, &ClientRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}