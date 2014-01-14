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
 * \brief     Implementation file for CAlphanumiricEdit class
 * \author    Anish Kumar
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CAlphanumiricEdit class
 */

#include "Utils_stdafx.h"
#include "AlphanumiricEdit.h"
#include "RadixEdit.h" //for backspace definintion


// CAlphanumiricEdit

IMPLEMENT_DYNAMIC(CAlphanumiricEdit, CEdit)
CAlphanumiricEdit::CAlphanumiricEdit(int nItem, int nSubItem)
{
    m_nRow = nItem;
    m_nColumn = nSubItem;
    m_bVK_ESCAPE = FALSE;
}

CAlphanumiricEdit::~CAlphanumiricEdit()
{
}


BEGIN_MESSAGE_MAP(CAlphanumiricEdit, CEdit)
    ON_WM_CHAR()
    ON_WM_KILLFOCUS()
END_MESSAGE_MAP()



// CAlphanumiricEdit message handlers
/******************************************************************************/
/*  Function Name    :  OnChar                                                */
/*  Input(s)         :  character code value of the key,repeat count,flag     */
/*  Output           :                                                        */
/*  Functionality    :  This prevent an user to insert special characters in  */
/*                      message ar signal name.                               */
/*  Member of        :  CAlphanumiricEdit                                     */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Anish kumar                                           */
/*  Date Created     :  01.09.2006                                            */
/*  Modifications    :  Raja N on 12.12.2004                                  */
/******************************************************************************/

void CAlphanumiricEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: Add your message handler code here and/or call default

    if(nChar=='_' || isalnum(nChar)!= 0 || nChar == BACK_SPACE || (nChar>=0 && nChar <=32) )
    {
        if( nChar == VK_ESCAPE)
        {
            m_bVK_ESCAPE = TRUE;
        }
        CEdit::OnChar(nChar, nRepCnt, nFlags);
    }
}

void CAlphanumiricEdit::OnKillFocus(CWnd* pNewWnd)
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
            m_bVK_ESCAPE ? LPTSTR((LPCTSTR)omStr) : LPTSTR((LPCTSTR)omStr);
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
