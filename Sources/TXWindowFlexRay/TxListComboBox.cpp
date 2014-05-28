// ListComboBox.cpp : implementation file
/**********************************************************************************
Project         :   Frame
FileName        :   ListComboBox.cpp
Description     :   Implementation of CListComboBox class, to display the combo
                    box in the list control
Directory       :
Version         :   V1.0
Authors         :   Ravi D
Date Created    :   11/10/2006
Date Modified   :
Modifications   :
**********************************************************************************/

#include "TxWindowFlexRay_stdafx.h"
//#include "FRAME_BaseUI.h"
#include "TxListComboBox.h"


// CListComboBox

IMPLEMENT_DYNAMIC(CListComboBox, CComboBox)

CListComboBox::CListComboBox( int nItem,
                              int nSubItem,
                              const CStringArray& psList,
                              const CString& romStrText,
                              BOOL bIsEditable)
{
    m_nItem     = nItem;
    m_nSubItem  = nSubItem;
    m_sList.Append( psList );
    m_bVK_ESCAPE = FALSE;
    m_bIsEditable = bIsEditable;
    omStrText = romStrText;
}

/**********************************************************************************
Function Name   :   ~CListComboBox
Input(s)        :   -
Output          :   -
Functionality   :   Destructor
Member of       :   CListComboBox
Friend of       :   -
Authors         :   Ravi D
Date Created    :   11/10/2006
Modifications   :   -
************************************************************************************/
CListComboBox::~CListComboBox()
{
}


BEGIN_MESSAGE_MAP(CListComboBox, CComboBox)
    ON_WM_NCDESTROY()
    ON_WM_CHAR()
    ON_WM_CREATE()
    ON_WM_KILLFOCUS()
    ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCbnCloseup)
END_MESSAGE_MAP()



// CListComboBox message handlers
/**********************************************************************************
Function Name   :   PreTranslateMessage
Input(s)        :   pMsg - Pointer to the message structure
Output          :   true - message was translated and not dispatched
                    false -  message was not translated and should be dispatched
Functionality   :   To translate window messages before they are dispatched
Member of       :   CListComboBox
Friend of       :   -
Authors         :   Ravi D
Date Created    :   11/10/2006
Modifications   :   -
************************************************************************************/
BOOL CListComboBox::PreTranslateMessage(MSG* pMsg)
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

/**********************************************************************************
Function Name   :   OnNcDestroy
Input(s)        :   -
Output          :   -
Functionality   :   to perform your own cleanup,  when the nonclient area is being
                    destroyed
Member of       :   CListComboBox
Friend of       :   -
Authors         :   Ravi D
Date Created    :   11/10/2006
Modifications   :   -
************************************************************************************/
void CListComboBox::OnNcDestroy()
{
    CComboBox::OnNcDestroy();

    // TODO: Add your message handler code here
    delete this;
}

/**********************************************************************************
Function Name   :   OnChar
Input(s)        :   nChar - character code value of the key
                    nRepCnt -  the number of times the keystroke is repeated
                    nFlags -  scan code, key-transition code, previous key state,
                    and context code
Output          :   -
Functionality   :   Translate a keystroke to a nonsystem character
Member of       :   CListComboBox
Friend of       :   -
Authors         :   Ravi D
Date Created    :   11/10/2006
Modifications   :   -
************************************************************************************/
void CListComboBox::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
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

/**********************************************************************************
Function Name   :   OnCreate
Input(s)        :   lpCreateStruct - structure that contains information about the
                    CWnd object being created
Output          :   -1 - the window will be destroyed
                    0 - continue the creation of the CWnd object
Functionality   :   any needed initialization of a derived class.
Member of       :   CListComboBox
Friend of       :   -
Authors         :   Ravi D
Date Created    :   11/10/2006
Modifications   :   -
************************************************************************************/
int CListComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CComboBox::OnCreate(lpCreateStruct) == -1)
    {
        return -1;
    }

    // TODO:  Add your specialized creation code here
    CFont* font = GetParent()->GetFont();
    SetFont(font);
    //add the items from CStringlist
    INT_PTR nItems = m_sList.GetSize();
    for( int index = 0; index < nItems; index++)
    {
        AddString(m_sList.GetAt( index ));
    }
    SetFocus();

    return 0;
}

/**********************************************************************************
Function Name   :   OnKillFocus
Input(s)        :   pNewWnd - pointer to the window that receives the input focus
Output          :   -
Functionality   :   This function is called before the window is loosing the input focus
Member of       :   CListComboBox
Friend of       :   -
Authors         :   Ravi D
Date Created    :   11/10/2006
Modifications   :   -
************************************************************************************/
void CListComboBox::OnKillFocus(CWnd* pNewWnd)
{

    CComboBox::OnKillFocus(pNewWnd);

    int nIndex = GetCurSel();
    if( m_bIsEditable == FALSE && pNewWnd // nullptr condition
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

/**********************************************************************************
Function Name   :   OnCbnCloseup
Input(s)        :   -
Output          :   -
Functionality   :   This function is executed when the list box of a combo box has
                    been closed
Member of       :   CListComboBox
Friend of       :   -
Authors         :   Ravi D
Date Created    :   11/10/2006
Modifications   :   -
************************************************************************************/
void CListComboBox::OnCbnCloseup()
{
    // TODO: Add your control notification handler code here
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