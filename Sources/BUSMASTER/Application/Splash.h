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
 * @author Arunkumar K.
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#pragma once

class CSplashScreen : public CWnd
{
    DECLARE_MESSAGE_MAP()

public:
    ~CSplashScreen();

    virtual void PostNcDestroy();
    static void ActivateSplashScreen(BOOL bActivate = TRUE);
    static void DisplaySplashScreen(CWnd* pParentWnd = nullptr, INT nShow = SW_SHOW);
    static BOOL PreTranslateAppMessage(MSG* pMsg);

    CBitmap m_ouBitMapImage;

protected:
    CSplashScreen();

    BOOL Create(CWnd* pParentWnd = nullptr);
    static BOOL sm_bShowScreen;
    static CSplashScreen* sm_pouSplashScreen;

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT nIDEvent);
};
