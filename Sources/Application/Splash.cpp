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
 * \file      Splash.cpp
 * \author    Arunkumar K.
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#include "stdafx.h"
#include "resource.h"

#include "Splash.h"

/////////////////////////////////////////////////////////////////////////////
//   Splash Screen class

BOOL CSplashScreen::sm_bShowScreen;
CSplashScreen* CSplashScreen::sm_pouSplashScreen;
CSplashScreen::CSplashScreen()
{
}

CSplashScreen::~CSplashScreen()
{
    sm_pouSplashScreen = NULL;
}

BEGIN_MESSAGE_MAP(CSplashScreen, CWnd)
    //{{AFX_MSG_MAP(CSplashScreen)
    ON_WM_CREATE()
    ON_WM_PAINT()
    ON_WM_TIMER()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSplashScreen::ActivateSplashScreen(BOOL bActivate /*= TRUE*/)
{
    sm_bShowScreen = bActivate;
}

void CSplashScreen::DisplaySplashScreen(CWnd* pParentWnd /*= NULL*/, INT nShow/* = SW_SHOW*/)
{
    if (nShow == SW_SHOW) //if show
    {
        if ((sm_bShowScreen == TRUE) && (sm_pouSplashScreen == NULL))
        {
            sm_pouSplashScreen = new CSplashScreen();

            if (sm_pouSplashScreen->Create(pParentWnd) == TRUE)
            {
                sm_pouSplashScreen->UpdateWindow();
            }
            else
            {
                delete sm_pouSplashScreen;
            }
        }
    }
    else // If Hide
    {
        if (sm_pouSplashScreen != NULL)
        {
            sm_pouSplashScreen->DestroyWindow();
            AfxGetMainWnd()->UpdateWindow();
        }
    }
}

BOOL CSplashScreen::PreTranslateAppMessage(MSG* pMsg)
{
    BOOL bResult = FALSE;

    if (sm_pouSplashScreen != NULL)
    {
        //Hide the screen if mouse clicked or key down
        if ((pMsg->message == WM_LBUTTONDOWN)    ||
                (pMsg->message == WM_RBUTTONDOWN)    ||
                (pMsg->message == WM_NCRBUTTONDOWN)  ||
                (pMsg->message == WM_KEYDOWN)        ||
                (pMsg->message == WM_MBUTTONDOWN)    ||
                (pMsg->message == WM_SYSKEYDOWN)     ||
                (pMsg->message == WM_NCLBUTTONDOWN)  ||
                (pMsg->message == WM_NCMBUTTONDOWN))
        {
            sm_pouSplashScreen->DisplaySplashScreen(NULL, SW_HIDE);
            bResult = TRUE;
        }
    }

    return bResult;
}

BOOL CSplashScreen::Create(CWnd* pParentWnd /*= NULL*/)
{
    BOOL bResult = FALSE;

    if (m_ouBitMapImage.LoadBitmap(IDR_BMP_SPLASH_SCREEN) == TRUE)
    {
        BITMAP BitMap;
        m_ouBitMapImage.GetBitmap(&BitMap);
        bResult = CreateEx(0,
                           AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
                           NULL, WS_POPUP | WS_VISIBLE, 0, 0, BitMap.bmWidth, BitMap.bmHeight, pParentWnd->GetSafeHwnd(), NULL);
    }

    return bResult;
}
void CSplashScreen::PostNcDestroy()
{
    delete this;
}

int CSplashScreen::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    int nResult = -1;

    if (CWnd::OnCreate(lpCreateStruct) != -1)
    {
        CenterWindow(); //Display splash screen in the center
        SetTimer(1, 750, NULL); // Timer interval after splash will dissappear
        nResult = 0;
    }

    return nResult;
}

void CSplashScreen::OnPaint()
{
    CPaintDC omPaintDc(this);
    CDC omDc;

    if (omDc.CreateCompatibleDC(&omPaintDc) == TRUE)
    {
        BITMAP BitMap;
        m_ouBitMapImage.GetBitmap(&BitMap);
        // Paint the image.
        CBitmap* pomBitmapExisting = omDc.SelectObject(&m_ouBitMapImage);
        omPaintDc.BitBlt(0, 0, BitMap.bmWidth, BitMap.bmHeight, &omDc, 0, 0, SRCCOPY);
        omDc.SelectObject(pomBitmapExisting);
    }
}

void CSplashScreen::OnTimer(UINT /*nIDEvent*/)
{
    DisplaySplashScreen(NULL, SW_HIDE);// Hide the splash.
}
