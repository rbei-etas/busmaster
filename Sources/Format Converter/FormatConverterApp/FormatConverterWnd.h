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
 * \file      FormatConverterWnd.h
 * \brief     Definition of CFormatConverterWnd class
 * \authors   Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of CFormatConverterWnd class
 */

#pragma once

#include "PluginManagerDll.h"
#include "DefConverterPage.h"

class CFormatConverterWnd : public CPropertySheet
{
    DECLARE_DYNAMIC(CFormatConverterWnd)
    CPluginManagerBase* m_pouPluginManager;
    CDefConverterPage* m_pomDefConverterPage;
public:
    CFormatConverterWnd(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
    CFormatConverterWnd(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
    virtual ~CFormatConverterWnd();
    HRESULT LoadConverters();
protected:
    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    virtual BOOL Create(CWnd* pParentWnd = NULL, DWORD dwStyle = (DWORD)-1, DWORD dwExStyle = 0);
protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


