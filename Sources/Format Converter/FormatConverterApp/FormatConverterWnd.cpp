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
 * \brief     Implementation of CFormatConverterWnd class
 * \authors   Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of CFormatConverterWnd class
 */

#include "stdafx.h"
#include "FormatConverterWnd.h"
#include "DefConverterPage.h"

IMPLEMENT_DYNAMIC(CFormatConverterWnd, CPropertySheet)

CFormatConverterWnd::CFormatConverterWnd(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
    :CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
    m_pouPluginManager = NULL;
    m_pomDefConverterPage = NULL;
}

CFormatConverterWnd::CFormatConverterWnd(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
    :CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
    m_pouPluginManager = NULL;
    m_pomDefConverterPage = NULL;
}

CFormatConverterWnd::~CFormatConverterWnd()
{
    if( m_pouPluginManager != NULL)
    {
        m_pouPluginManager->UnLoadAllPlugins();
        delete m_pouPluginManager;
    }

    if(m_pomDefConverterPage != NULL)
    {
        delete m_pomDefConverterPage;
    }
}


BEGIN_MESSAGE_MAP(CFormatConverterWnd, CPropertySheet)
    ON_WM_CREATE()
END_MESSAGE_MAP()


// CFormatConverterWnd message handlers

BOOL CFormatConverterWnd::OnInitDialog()
{
    BOOL bResult = CPropertySheet::OnInitDialog();
    // TODO:  Add your specialized code here
    ModifyStyle(0, WS_MINIMIZEBOX);
    CButton* omBtn;
    WINDOWPLACEMENT omWndPlace;
    omBtn = reinterpret_cast<CButton*>(GetDlgItem(ID_APPLY_NOW));
    omBtn->ShowWindow(SW_HIDE);
    omBtn->GetWindowPlacement(&omWndPlace);
    omBtn = reinterpret_cast<CButton*>(GetDlgItem(IDOK));
    omBtn->ShowWindow(SW_HIDE);
    omBtn = reinterpret_cast<CButton*>(GetDlgItem(IDCANCEL));
    omBtn->SetWindowText("Close");
    omBtn->SetWindowPlacement(&omWndPlace);
    return bResult;
}

int CFormatConverterWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    // TODO:  Add your specialized creation code here
    return 0;
}

BOOL CFormatConverterWnd::Create(CWnd* pParentWnd , DWORD dwStyle , DWORD dwExStyle)
{
    // TODO: Add your specialized code here and/or call the base class
    CPropertySheet::Create(pParentWnd, dwStyle, dwExStyle);
    return TRUE;
}


HRESULT CFormatConverterWnd::LoadConverters()
{
    if (m_pouPluginManager != NULL)
        delete m_pouPluginManager;
    m_pouPluginManager = new CPluginManagerDLL();
    m_pouPluginManager->LoadConvertersFromFolder();
    INT nCount = m_pouPluginManager->m_ConverterList.GetCount();
    m_pomDefConverterPage = new CDefConverterPage();
    m_pomDefConverterPage->SetPluginManager(m_pouPluginManager);
    m_pomDefConverterPage->m_psp.dwFlags &= ~PSP_HASHELP;
    //m_pomDefConverterPage->m_psp.dwFlags &= ~PSP_HIDEHEADER;
    AddPage(m_pomDefConverterPage);

    for(INT i = 0; i < nCount; i++)
    {
        POSITION pos = m_pouPluginManager->m_ConverterList.FindIndex(i);
        ConverterInfo& ouConverterInfo = m_pouPluginManager->m_ConverterList.GetAt(pos);

        if(ouConverterInfo.m_pouConverter->bHaveOwnWindow() == TRUE)
        {
            //CPropertyPage *pPage = NULL;
            ouConverterInfo.m_pouConverter->GetPropertyPage(ouConverterInfo.m_pomPage);

            if(ouConverterInfo.m_pomPage != NULL)
            {
                ouConverterInfo.m_pomPage->m_psp.dwFlags &= ~PSP_HASHELP;
                AddPage(ouConverterInfo.m_pomPage);
            }
        }
    }

    return S_OK;
}

BOOL CFormatConverterWnd::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Add your specialized code here and/or call the base class
    cs.dwExStyle = cs.dwExStyle | WS_EX_TOOLWINDOW;
    return CPropertySheet::PreCreateWindow(cs);
}
