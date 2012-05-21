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
 * \file      ButtonItem.cpp
 * \author    Venkatanarayana Makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#include "Utils_stdafx.h"         // For standard include
#include "buttonitem.h"

CBrowseEditItem::CBrowseEditItem(int nItem, int nSubItem, CString& sContent, CButtonItem* pomButtonItem)
    :CEditItem(nItem, nSubItem, sContent)
{
    m_pomButton = pomButtonItem;
    m_bIsButtonValid = TRUE;
    m_bKillFocus = TRUE;
}
CBrowseEditItem::~CBrowseEditItem()
{
}
BEGIN_MESSAGE_MAP(CBrowseEditItem, CEditItem)
    ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

void CBrowseEditItem::OnKillFocus(CWnd* pNewWnd)
{
    CEdit::OnKillFocus(pNewWnd);

    // TODO: Add your message handler code here
    if( pNewWnd // NULL condition
            && pNewWnd != GetParent()->GetParent() //For Dialog Close using X Button
            && pNewWnd->GetDlgCtrlID() != IDCANCEL ) // For Cancel condition
    {
        AfxTrace("In if Condition\n");
        CString str;
        GetWindowText(str);
        // Send Notification to parent of ListView ctrl
        LV_DISPINFO lvDispInfo;
        lvDispInfo.hdr.hwndFrom = GetParent()->m_hWnd;
        lvDispInfo.hdr.idFrom = GetDlgCtrlID();
        lvDispInfo.hdr.code = LVN_ENDLABELEDIT;
        lvDispInfo.item.mask = LVIF_TEXT;
        lvDispInfo.item.iItem = m_nItem;
        lvDispInfo.item.iSubItem = m_nSubItem;
        // Restore the old text on press of ESCAPE key
        lvDispInfo.item.pszText =
            m_bVK_ESCAPE ? LPTSTR((LPCTSTR)m_sContent) : LPTSTR((LPCTSTR)str);
        lvDispInfo.item.cchTextMax = str.GetLength();
        // Send the notification message to the parent
        GetParent()->GetParent()->SendMessage( WM_NOTIFY,
                                               GetParent()->GetDlgCtrlID(),
                                               (LPARAM)&lvDispInfo);
    }

    //HWND hNewWnd = 0;
    if(pNewWnd != NULL)
    {
        if(m_pomButton->GetSafeHwnd() == pNewWnd->GetSafeHwnd())
        {
            return;
        }
    }

    if((((CButtonItem*)m_pomButton)->m_bButtonclicked == FALSE) && (m_bKillFocus ==  TRUE))
    {
        ((CButtonItem*)m_pomButton)->m_bIsEditValid = FALSE;

        if(m_bIsButtonValid == TRUE)
        {
            ((CButtonItem*)m_pomButton)->m_bKillFocus = TRUE;
            m_pomButton->SendMessage(WM_KILLFOCUS, 0, 0);
        }

        DestroyWindow();
    }
}

CButtonItem::CButtonItem(CString omStrDefExt, CString omStrFilter)
{
    m_bKillFocus = TRUE;
    m_bIsEditValid = TRUE;
    m_bButtonclicked = FALSE;
    m_omStrDefExt = omStrDefExt;
    m_omStrFilter = omStrFilter;
}

CButtonItem::~CButtonItem(void)
{
}
void CButtonItem::vSetEditItem(CBrowseEditItem* pomEditItem)
{
    m_pomEditItem = pomEditItem;
}
BEGIN_MESSAGE_MAP(CButtonItem, CButton)
    ON_WM_KILLFOCUS()
    ON_CONTROL_REFLECT(BN_CLICKED, OnBnClicked)
END_MESSAGE_MAP()

void CButtonItem::OnKillFocus(CWnd* pNewWnd)
{
    CButton::OnKillFocus(pNewWnd);

    //HWND hNewWnd = 0;
    if(pNewWnd != NULL)
    {
        if(m_pomEditItem->GetSafeHwnd() == pNewWnd->GetSafeHwnd())
        {
            return;
        }
    }

    if((m_bButtonclicked == FALSE)&&(m_bKillFocus == TRUE))
    {
        ((CBrowseEditItem*)m_pomEditItem)->m_bIsButtonValid = FALSE;

        if((m_bIsEditValid == TRUE))
        {
            ((CBrowseEditItem*)m_pomEditItem)->m_bKillFocus = TRUE;
            m_pomEditItem->SendMessage(WM_KILLFOCUS, 0, 0);
        }

        DestroyWindow();
    }
}

void CButtonItem::OnBnClicked()
{
    // TODO: Add your control notification handler code here
    m_bKillFocus = FALSE;
    m_bButtonclicked = TRUE;
    CFileDialog omFileDlg(TRUE, m_omStrDefExt, 0, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, m_omStrFilter);

    if( IDOK == omFileDlg.DoModal())
    {
        CString omStrFilePath = omFileDlg.GetPathName();
        m_pomEditItem->SetWindowText(omStrFilePath);
    }

    m_pomEditItem->SetFocus();
    m_bKillFocus = TRUE;
    ((CBrowseEditItem*)m_pomEditItem)->m_bKillFocus = TRUE;
    m_bButtonclicked = FALSE;
}
