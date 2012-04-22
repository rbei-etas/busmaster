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
 * \file      EditItem.cpp
 * \brief     This file contains CEditItem class definition.
 * \authors   Zafir Anjum
  *
 * This file contains CEditItem class definition.
 */

/*	Code contained in this file is taken from codeguru article 
	http://www.codeguru.com/Cpp/controls/listview/editingitemsandsubitem/article.php/c923 
	Written by Zafir Anjum	*/

#include "Utils_stdafx.h"         // For standard include
#include "EditItem.h"       // Definition of class

/////////////////////////////////////////////////////////////////////////////
// CEditItem
/*******************************************************************************
 Function Name    : CEditItem
 Input(s)         : nItem - Item Index
                    nSubItem - Sub item index
                    sContent - constant refetence to the text
 Output           :
 Functionality    : Constructor
 Member of        : CEditItem
 Friend of        :  -
 Author(s)        : Raja N
 Date Created     : 21.07.2004                                          
 Modifications    :
*******************************************************************************/
CEditItem::CEditItem(int nItem, int nSubItem, const CString &sContent )
{
    // Init member variables
    m_nItem = nItem;
    m_nSubItem = nSubItem;
    m_sContent = sContent;
    m_bVK_ESCAPE = FALSE; 
}

/*******************************************************************************
 Function Name    : ~CEditItem
 Input(s)         : 
 Output           :
 Functionality    : Destructor
 Member of        : CEditItem
 Friend of        :  -
 Author(s)        : Raja N
 Date Created     : 21.07.2004                                          
 Modifications    :
*******************************************************************************/
CEditItem::~CEditItem()
{
}


BEGIN_MESSAGE_MAP(CEditItem, CEdit)
    //{{AFX_MSG_MAP(CEditItem)
    ON_WM_CHAR()
    ON_WM_NCDESTROY()
    ON_WM_KILLFOCUS()
    ON_WM_CREATE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditItem message handlers

/*******************************************************************************
 Function Name    : PreTranslateMessage
                                                                           
 Input(s)         : MSG* pMsg
 Output           : BOOL
 Functionality    : To avoid processing special keys
 Member of        : CEditItem
 Friend of        :   -
                                                                           
 Author(s)        : Raja N
 Date Created     : 21.07.2004
 Modifications    : 
*******************************************************************************/
BOOL CEditItem::PreTranslateMessage(MSG* pMsg) 
{
    // TODO: Add your specialized code here and/or call the base class
    if( pMsg->message == WM_KEYDOWN )   
    {       
        if( pMsg->wParam == VK_RETURN || 
            pMsg->wParam == VK_DELETE ||
            pMsg->wParam == VK_ESCAPE ||
            pMsg->wParam == VK_UP ||
            GetKeyState( VK_CONTROL))
        {           
            ::TranslateMessage(pMsg);
            ::DispatchMessage(pMsg);
            return 1;
        }
        if(pMsg->wParam == VK_TAB)
            MessageBeep(0);
    }

    return CEdit::PreTranslateMessage(pMsg);
}

/*******************************************************************************
 Function Name  : OnChar
 Input(s)       :
 Output         :  -
 Functionality  : Called by the frame work on keying a character. This function
                  hides the control on press of Enter or Escape.This will resize
                  the edit control if the text is hidden.
 Member of      : CEditItem
 Friend of      :
 Author(s)      : Raja N
 Date Created   : 21.07.2004
 Modifications  :
*******************************************************************************/
void CEditItem::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    // TODO: Add your message handler code here and/or call default
    if( nChar == VK_ESCAPE ||
        nChar == VK_RETURN) 
    {       
        if( nChar == VK_ESCAPE)
            m_bVK_ESCAPE = 1;       
        GetParent()->SetFocus();
        return; 
    }

    CEdit::OnChar(nChar, nRepCnt, nFlags);  
    // Resize edit control if needed
    // Get text extent  
    CString str;    
    GetWindowText( str );   

    CWindowDC dc(this);
    CFont *pFont = GetParent()->GetFont();
    CFont *pFontDC = dc.SelectObject(pFont);
    CSize size = dc.GetTextExtent(str); 
    dc.SelectObject(pFontDC);
    size.cx += 5; // add some extra buffer  
    // Get client rect
	CRect rect, parentrect;
    GetClientRect(&rect);
	GetParent()->GetClientRect( &parentrect );
    // Transform rect to parent coordinates 
    ClientToScreen(&rect);
    GetParent()->ScreenToClient(&rect);
    // Check whether control needs to be resized
    // and whether there is space to grow   
    if(size.cx > rect.Width())  
    {
		if( size.cx + rect.left < parentrect.right )
            rect.right = rect.left + size.cx;       
        else            
			rect.right = parentrect.right;
        MoveWindow(&rect);  
    }
    
//  CEdit::OnChar(nChar, nRepCnt, nFlags);
}

/*******************************************************************************
 Function Name  : OnNcDestroy
 Input(s)       :
 Output         :  -
 Functionality  : This will be called after destroying the window. This will
                  delete the memory used by the control.
 Member of      : CEditItem
 Friend of      :
 Author(s)      : Raja N
 Date Created   : 21.07.2004
 Modifications  :
*******************************************************************************/
void CEditItem::OnNcDestroy() 
{
    CEdit::OnNcDestroy();
    
    // TODO: Add your message handler code here
    delete this;
}


/*******************************************************************************
 Function Name  : OnKillFocus
 Input(s)       : pNewWnd - Pointer to the window that got focus
 Output         :  -
 Functionality  : This will be called if the edit control losses its focus. This
                  function will post a message to parent list control to inform
                  that list data has been editied. If the loss of focus is
                  because of Escape key this will restore the old value.
 Member of      : CEditItem
 Friend of      :
 Author(s)      : Raja N
 Date Created   : 21.07.2004
 Modifications  :
*******************************************************************************/
void CEditItem::OnKillFocus(CWnd* pNewWnd) 
{
    CEdit::OnKillFocus(pNewWnd);
    
    // TODO: Add your message handler code here
    if( pNewWnd // NULL condition 
        && pNewWnd != GetParent()->GetParent() //For Dialog Close using X Button
        && pNewWnd->GetDlgCtrlID() != IDCANCEL ) // For Cancel condition
    {
        CString str;    GetWindowText(str);
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
    // Destroy the window
    DestroyWindow();    
}

/*******************************************************************************
 Function Name    : OnCreate
 Input(s)         : LPCREATESTRUCT
 Output           : int
 Functionality    : Called by the framework while a window is being created.
                    This will set the text, font and the selection.
 Member of        : CEditItem
 Friend of        : -
 Author(s)        : Raja N
 Date Created     : 21.07.2004
*******************************************************************************/
int CEditItem::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CEdit::OnCreate(lpCreateStruct) == -1)
        return -1;  

    CFont* font = GetParent()->GetFont();
    SetFont(font);
    SetWindowText(m_sContent);
    SetFocus(); 
    SetSel(0, m_sContent.GetLength());
    return 0;
}