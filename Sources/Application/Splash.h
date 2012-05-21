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
 * \file      Splash.h
 * \author    Arunkumar K.
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

class CSplashScreen : public CWnd
{
    // Construction
protected:
    CSplashScreen();

    // Attributes:
public:
    CBitmap m_ouBitMapImage;

    // Operations
public:
    static void ActivateSplashScreen(BOOL bActivate = TRUE);
    static void DisplaySplashScreen(CWnd* pParentWnd = NULL, INT nShow = SW_SHOW);
    static BOOL PreTranslateAppMessage(MSG* pMsg);

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSplashWnd)
    //}}AFX_VIRTUAL

    // Implementation
public:
    ~CSplashScreen();
    virtual void PostNcDestroy();

protected:
    BOOL Create(CWnd* pParentWnd = NULL);
    static BOOL sm_bShowScreen;
    static CSplashScreen* sm_pouSplashScreen;

    // Generated message map functions
protected:
    //{{AFX_MSG(CSplashWnd)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT nIDEvent);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};
