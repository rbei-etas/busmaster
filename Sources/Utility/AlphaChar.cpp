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
 * \file      AlphanumiricEdit.cpp
 * \brief     Implementation file for CAlphaCharEdit class
 * \author    Anish Kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CAlphaCharEdit class
 */

#include "Utils_stdafx.h"
#include "AlphaChar.h"
#include "RadixEdit.h" //for backspace definintion


// CAlphaCharEdit

IMPLEMENT_DYNAMIC(CAlphaCharEdit, CEdit)
CAlphaCharEdit::CAlphaCharEdit(int nItem, int nSubItem, CString& strContent)
{
    m_strContent = strContent;
    m_nRow = nItem;
    m_nColumn = nSubItem;
    m_bVK_ESCAPE = FALSE;
}

CAlphaCharEdit::~CAlphaCharEdit()
{
}


BEGIN_MESSAGE_MAP(CAlphaCharEdit, CEdit)
    ON_WM_CHAR()
    ON_WM_KILLFOCUS()
    ON_WM_CREATE()
END_MESSAGE_MAP()



// CAlphaCharEdit message handlers
/******************************************************************************/
/*  Function Name    :  OnChar                                                */
/*  Input(s)         :  character code value of the key,repeat count,flag     */
/*  Output           :                                                        */
/*  Functionality    :  This prevent an user to insert special characters in  */
/*                      message ar signal name.                               */
/*  Member of        :  CAlphaCharEdit                                     */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Anish kumar                                           */
/*  Date Created     :  01.09.2006                                            */
/*  Modifications    :  Raja N on 12.12.2004                                  */
/******************************************************************************/

void CAlphaCharEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: Add your message handler code here and/or call default
    CString str;
    GetWindowText( str );
    CWindowDC dc(this);
    CFont* pFont = GetParent()->GetFont();
    CFont* pFontDC = dc.SelectObject(pFont);
    CSize size = dc.GetTextExtent(str);
    dc.SelectObject(pFontDC);

    if(isalnum(nChar)!= 0 || nChar == BACK_SPACE || (nChar>=0 && nChar <=32) )
    {

        if( nChar == VK_ESCAPE ||
                nChar == VK_RETURN)
        {
            if( nChar == VK_ESCAPE)
            {
                // Set the Escape flag so that the initial text will be
                // restored
                m_bVK_ESCAPE = TRUE;
            }
            // Set the focus to the parent
            GetParent()->SetFocus();
            return;
        }


        CEdit::OnChar(nChar, nRepCnt, nFlags);
    }
}

void CAlphaCharEdit::OnKillFocus(CWnd* pNewWnd)
{

    CEdit::OnKillFocus(pNewWnd);


    if( m_nRow != -1 || m_nColumn != -1 )//For Dialog Close using X Button
    {
        CString omStr;
        // As it is non editable Get the window text to get the selected
        // item text
        GetWindowText(omStr);
        // Send Notification to parent of ListView ctrl
        LV_DISPINFO lvDispinfo;
        lvDispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
        lvDispinfo.hdr.idFrom = GetDlgCtrlID();//that's us
        lvDispinfo.hdr.code = LVN_ENDLABELEDIT;
        lvDispinfo.item.mask = LVIF_TEXT | LVIF_PARAM;
        lvDispinfo.item.iItem = m_nRow;
        lvDispinfo.item.iSubItem = m_nColumn;
        lvDispinfo.item.pszText =
            m_bVK_ESCAPE ? nullptr : LPTSTR((LPCTSTR)omStr);
        lvDispinfo.item.cchTextMax = omStr.GetLength();

        // For non editable the selection should not be -1
        PostMessage(WM_CLOSE);
        GetParent()->GetParent()->SendMessage( WM_NOTIFY,
                                               GetParent()->GetDlgCtrlID(),
                                               (LPARAM)&lvDispinfo);
        GetParent()->SendMessage( WM_NOTIFY,
                                  GetParent()->GetDlgCtrlID(),
                                  (LPARAM)&lvDispinfo);
    }
}


int CAlphaCharEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CEdit::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    CFont* font = GetParent()->GetFont();
    SetFont(font);
    SetWindowText(m_strContent);
    SetFocus();

    SetSel(0, m_strContent.GetLength());
    return 0;
}

BOOL CAlphaCharEdit::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if( pMsg->message == WM_KEYDOWN )
    {
        if( pMsg->wParam == VK_RETURN ||
                pMsg->wParam == VK_ESCAPE )
        {
            ::TranslateMessage(pMsg);
            ::DispatchMessage(pMsg);
            return 1;
        }
        if(pMsg->wParam == VK_TAB)
        {
            MessageBeep(0);
        }
    }

    return CEdit::PreTranslateMessage(pMsg);
}