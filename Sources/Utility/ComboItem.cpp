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
 * \file      ComboItem.cpp
 * \brief     This file contain definition of all function of
 * \author    Raja N
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of
 */

#include "Utils_stdafx.h"       // For standard include
#include "ComboItem.h"          // Definition of class

/////////////////////////////////////////////////////////////////////////////
// CComboItem

/*******************************************************************************
 Function Name    : CComboItem
 Input(s)         : nItem           - Item Index
                    nSubItem        - Sub item index
                    romList         - List of strings to populate list box
                    romStrTExt      - reference to Selected text
 Output           :
 Functionality    : Constructor
 Member of        : CComboItem
 Friend of        :  -
 Author(s)        : Raja N
 Date Created     : 22.07.2004
 Modifications    :
*******************************************************************************/
CComboItem::CComboItem( int nItem,
                        int nSubItem,
                        const CStringArray& romList,
                        const CString& romStrText,
                        BOOL bIsEditable)
{
    m_nItem     = nItem;
    m_nSubItem  = nSubItem;
    m_sList.Append( romList );
    m_bVK_ESCAPE = FALSE;
    m_bIsEditable = bIsEditable;
    omStrText = romStrText;
}

/*******************************************************************************
 Function Name    : ~CComboItem
 Input(s)         :
 Output           :
 Functionality    : Destructor
 Member of        : CComboItem
 Friend of        :  -
 Author(s)        : Raja N
 Date Created     : 22.07.2004
 Modifications    :
*******************************************************************************/
CComboItem::~CComboItem()
{
}


BEGIN_MESSAGE_MAP(CComboItem, CComboBox)
    //{{AFX_MSG_MAP(CComboItem)
    ON_WM_NCDESTROY()
    ON_WM_CHAR()
    ON_WM_KILLFOCUS()
    ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
    ON_WM_CREATE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboItem message handlers

/*******************************************************************************
 Function Name    : PreTranslateMessage

 Input(s)         : MSG* pMsg
 Output           : BOOL
 Functionality    : To avoid processing special keys
 Member of        : CComboItem
 Friend of        :   -

 Author(s)        : Raja N
 Date Created     : 21.07.2004
 Modifications    :
*******************************************************************************/
BOOL CComboItem::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    if( pMsg->message == WM_KEYDOWN )
    {
        if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
        {
            if( pMsg->wParam == VK_ESCAPE)
            {
                m_bVK_ESCAPE = TRUE;
            }

            ::TranslateMessage(pMsg);
            ::DispatchMessage(pMsg);
            return 1;
        }
    }

    return CComboBox::PreTranslateMessage(pMsg);
}

/*******************************************************************************
 Function Name  : OnNcDestroy
 Input(s)       :
 Output         :  -
 Functionality  : This will be called after destroying the window. This will
                  delete the memory used by the control.
 Member of      : CComboItem
 Friend of      :
 Author(s)      : Raja N
 Date Created   : 21.07.2004
 Modifications  :
*******************************************************************************/
void CComboItem::OnNcDestroy()
{
    CComboBox::OnNcDestroy();
    // Life time is over. Just delete the object created dynamically.
    delete this;
}

/*******************************************************************************
 Function Name  : OnChar
 Input(s)       :
 Output         :  -
 Functionality  : Called by the frame work on keying a character. This function
                  hides the control on press of Enter or Escape by setting focus
                  to the list control. The Kill focus will hide the control.
 Member of      : CComboItem
 Friend of      :
 Author(s)      : Raja N
 Date Created   : 21.07.2004
 Modifications  :
*******************************************************************************/
void CComboItem::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // TODO: Add your message handler code here and/or call default
    if(nChar == VK_ESCAPE || nChar == VK_RETURN)
    {
        if( nChar == VK_ESCAPE)
        {
            m_bVK_ESCAPE = 1;
        }

        GetParent()->SetFocus();
        return;
    }

    CComboBox::OnChar(nChar, nRepCnt, nFlags);
}

/*******************************************************************************
 Function Name  : OnKillFocus
 Input(s)       : pNewWnd - Pointer to the window that got focus
 Output         :  -
 Functionality  : This will be called if ComboItem losses its focus. This
                  function will post a message to parent list control to inform
                  that list data has been editied. If the loss of focus is
                  because of Escape key this will restore the old value. These
                  will be executed only if it is non- editable control
 Member of      : CComboItem
 Friend of      :
 Author(s)      : Raja N
 Date Created   : 21.07.2004
 Modifications  : Raja N on 30.07.2004, Implemented code review comments
*******************************************************************************/
void CComboItem::OnKillFocus(CWnd* pNewWnd)
{
    CComboBox::OnKillFocus(pNewWnd);
    int nIndex = GetCurSel();

    if( m_bIsEditable == FALSE && pNewWnd // NULL condition
            && pNewWnd != GetParent()->GetParent() )//For Dialog Close using X Button
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
        lvDispinfo.item.iItem = m_nItem;
        lvDispinfo.item.iSubItem = m_nSubItem;
        lvDispinfo.item.pszText =
            m_bVK_ESCAPE ? LPTSTR((LPCTSTR)omStrText) : LPTSTR((LPCTSTR)omStr);
        lvDispinfo.item.cchTextMax = omStr.GetLength();
        lvDispinfo.item.lParam = GetItemData(GetCurSel());
        // For non editable the selection should not be -1
        PostMessage(WM_CLOSE);

        if( nIndex != CB_ERR )
        {
            // Send the End Label Edit Message
            GetParent()->GetParent()->SendMessage( WM_NOTIFY,
                                                   GetParent()->GetDlgCtrlID(),
                                                   (LPARAM)&lvDispinfo);
        }
    }
}

/*******************************************************************************
 Function Name  : OnCloseup
 Input(s)       :  -
 Output         :  -
 Functionality  : This will be called while the listbox of ComboItem getting
                  closed. If it is editable combobox this function will update
                  the data and post message to list control. For non-editable
                  type this will post destroy message and set the focus to the
                  list control
 Member of      : CComboItem
 Friend of      :
 Author(s)      : Raja N
 Date Created   : 21.07.2004
 Modifications  : Raja N on 30.07.2004, Implemented code review comments
*******************************************************************************/
void CComboItem::OnCloseup()
{
    // If it is non-editable then call kill focus
    // For editable list box kill focus is not getting called at the end
    // So process the message here iteself and destroy the window
    // To update the typed text or selected text
    if( m_bIsEditable == TRUE)
    {
        // Get the current selection
        int nIndex = GetCurSel();
        CString omStr;

        // If the selection is invalid, the the user has typed some text in the
        // text box so use GetWindowText.
        if( nIndex != -1 )
        {
            GetLBText( nIndex, omStr);
        }
        else
        {
            GetWindowText(omStr);
        }

        // Send Notification to parent of ListView ctrl
        LV_DISPINFO lvDispinfo;
        lvDispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
        lvDispinfo.hdr.idFrom = GetDlgCtrlID();//that's us
        lvDispinfo.hdr.code = LVN_ENDLABELEDIT;
        lvDispinfo.item.mask = LVIF_TEXT | LVIF_PARAM;
        lvDispinfo.item.iItem = m_nItem;
        lvDispinfo.item.iSubItem = m_nSubItem;
        lvDispinfo.item.pszText =
            m_bVK_ESCAPE ? LPTSTR((LPCTSTR)omStrText) : LPTSTR((LPCTSTR)omStr);
        lvDispinfo.item.cchTextMax = omStr.GetLength();
        lvDispinfo.item.lParam = GetItemData(GetCurSel());
        // Post close before posting send message
        PostMessage(WM_CLOSE);
        // Set the focus to the list item
        GetParent()->SetFocus();
        // Send the End Label Edit Message
        GetParent()->GetParent()->SendMessage( WM_NOTIFY,
                                               GetParent()->GetDlgCtrlID(),
                                               (LPARAM)&lvDispinfo);
    }
    else
    {
        // Set the focus to the list control
        PostMessage(WM_CLOSE);
        // Set the focus to the list item
        GetParent()->SetFocus();
    }
}

/*******************************************************************************
 Function Name    : OnCreate
 Input(s)         : LPCREATESTRUCT
 Output           : int
 Functionality    : Called by the framework while a window is being created.
                    This will set the text, font and the selection. This will
                    add list entries to the combobox
 Member of        : CEditItem
 Friend of        : -
 Author(s)        : Raja N
 Date Created     : 21.07.2004
*******************************************************************************/
int CComboItem::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CComboBox::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    CFont* font = GetParent()->GetFont();
    SetFont(font);
    //add the items from CStringlist
    INT nItems = (INT)m_sList.GetSize();

    for( int index = 0; index < nItems; index++)
    {
        AddString(m_sList.GetAt( index ));
    }

    SetFocus();
    return 0;
}
