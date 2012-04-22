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
 * \file      ListCtrlEx.cpp
 * \brief     Implementation file for CListCtrlEx
 * \author    Venkatanarayana Makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CListCtrlEx
 */

// For all standard header file include
#include "TSEditorGUI_stdafx.h"
#include "Utility/ComboItem.h"              // For Custom Combobox Implementation
#include "Utility/EditItem.h"               // For Custom Editbox Implementation
#include "Utility/Utility_Structs.h"                 // For data struct definition
#include "Utility/RadixEdit.h"             // For the RAdix Edit control definition
#include "Utility/numSpinCtrl.h"            // For the custom spin control
#include "Utility/ButtonItem.h"
#include "Utility/NumEdit.h"                // For Custom Numeric Edit control Impl
#include "ListCtrlEx.h"           // Interface file for Flex List Control
#include "TSEditorGUI_ChildFrame.h"
#include "TSEditorGUI_Definitions.h"
#include ".\listctrlex.h"

// ID for Combobox
#define IDC_CONTROL         0x12345
#define def_HEIGHT_BUTTON       27
#define def_WIDTH_BUTTON        27
/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx
/******************************************************************************
 Function Name  :   CListCtrlEx

 Description    :   Standard default constructor
 Member of      :   CListCtrlEx
 Functionality  :   Initialises data members

 Author(s)      :   Raja N
 Date Created   :   22.07.2004
 Modifications  :   Raja N o n 30.07.2004, Code review comments implemented
******************************************************************************/
CListCtrlEx::CListCtrlEx() : m_bSingleClickActivate(FALSE)
{
    // Init List item type has table
    // Init with some prime number to have an effient search
    // The number of items expected in this list is more
    // So set bigger value
    m_omListItemType.InitHashTable(defLIST_ITEM_TYPE_TABLE_SIZE);
    // Init Num Details Hastable
    // The size could be comparatively less
    m_omNumDetails.InitHashTable(defLIST_NUM_DETAILS_TABLE_SIZE);
    memset(&m_sModifiedInfo, 0, sizeof(LV_DISPINFO));
    // Flag to stop validation while inserting the item
    m_bCreating = FALSE;
    // Set the default virtual column count
    m_nVirtualColumnCount = defVIRTUAL_COL_COUNT;

    m_colRow2 =     def_COLOR_SECONDROW;
    m_colRow1 =     def_COLOR_FIRSTROW;
}

/******************************************************************************
 Function Name  :   ~CListCtrlEx

 Description    :   Standard destructor
 Member of      :   CListCtrlEx

 Author(s)      :   Raja N
 Date Created   :   22.07.2004
 Modifications  :
******************************************************************************/
CListCtrlEx::~CListCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
    //{{AFX_MSG_MAP(CListCtrlEx)
    ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
    ON_NOTIFY_REFLECT(NM_DBLCLK, OnDoubleClick)
    ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
    //}}AFX_MSG_MAP
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListCtrlEx message handlers

/******************************************************************************
 Function Name  : OnEndlabeledit
 Description    : Notification Handler. This handler will be called after once
                  the item editing is complete. This will update the list item
                  text and will copy the DISPINFO data. This will set col 0 val
                  to invoke item changed event.
 Member of      : CListCtrlEx
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  :
******************************************************************************/
void CListCtrlEx::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
    LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
    // Get the item index
    LV_ITEM *plvItem = &pDispInfo->item;
    // Proceed only for valid item change
    if( plvItem->iItem != -1 &&  // valid item
        plvItem->pszText != NULL)       // valid text
    {
        // Copy the change information. This is required to validate the
        // data from OnItemChanged.
        memcpy(&m_sModifiedInfo, pDispInfo, sizeof(LV_DISPINFO));
        // Update the list text
        SetItemText( plvItem->iItem, plvItem->iSubItem, plvItem->pszText);
        // Copy Col 0 item to invoke ItemChanged Event
        if( plvItem->iSubItem != 0)
        {
            // this will invoke an ItemChanged handler in parent
            CString cs = GetItemText( plvItem->iItem,0);
            SetItemText( plvItem->iItem, 0, cs);
            CWnd* wndParent = GetParent()->GetParent()->GetParent();   
            ((CTSEditorChildFrame*)wndParent)->vSetModifiedFlag(TRUE);
        }
    }
    *pResult = 0;
}

/*******************************************************************************
 Function Name  : OnHScroll
 Description    : Event Handler. This handler will hide the controls if
                  anytihng is displayed for editing. This is done by setting the
                  focus to the list control.
 Member of      : CListCtrlEx
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  :
*******************************************************************************/
void CListCtrlEx::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    // Set the focus to list control. This will hide any controls that
    // are all visible at this time
    if( GetFocus() != this)
        SetFocus();
    CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

/*******************************************************************************
 Function Name  : OnVScroll
 Description    : Event Handler. This handler will hide the controls if
                  anytihng is displayed for editing. This is done by setting the
                  focus to the list control.
 Member of      : CListCtrlEx
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  :
*******************************************************************************/
void CListCtrlEx::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
    // Set the focus to list control. This will hide any controls that
    // are all visible at this time
    if( pScrollBar == NULL ) // Form the (list )window
    {
        if( GetFocus() != this)
        {
            SetFocus();
        }
    }
    CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

/*******************************************************************************
 Function Name  : OnDoubleClick
 Description    : Event Handler. This handler will show the controls to edit the
                  selected data from the list control. This will call
                  ShowControl function with the item and subitem indices.
 Member of      : CListCtrlEx
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  : Raja N on 01.08.2004, Modified the function name ShowControl
                  as vShowControl.
 Modifications  : Raja N on 08.07.2005, Modified the function send
                  LVN_BEGINLABELEDIT before showing the UI control.
*******************************************************************************/
void CListCtrlEx::OnDoubleClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
    // TODO: Add your control notification handler code here
    if( m_bSingleClickActivate == FALSE )
    {
        // Set the focus to the list control
        if( GetFocus() != this)
        {
            SetFocus();
        }
        // Send Notification to Parent so that Begin Label Edit
        // Handler will be getting called
        NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
        
        // Send Notification to parent of ListView ctrl 
        LV_DISPINFO lvDispInfo;
        lvDispInfo.hdr.hwndFrom = m_hWnd;
        lvDispInfo.hdr.idFrom = GetDlgCtrlID();
        lvDispInfo.hdr.code = LVN_BEGINLABELEDIT;
        lvDispInfo.item.mask = LVIF_TEXT;
        lvDispInfo.item.iItem = pNMListView->iItem;
        lvDispInfo.item.iSubItem = pNMListView->iSubItem;
        lvDispInfo.item.pszText = NULL;
        lvDispInfo.item.cchTextMax = 0;
        CWnd * pWnd = GetParent();
        if( pWnd != NULL )
        {
            pWnd->SendMessage( WM_NOTIFY, GetDlgCtrlID(),
                                  (LPARAM)&lvDispInfo );
        }
        // Call Handler Function with required parameters
        vShowControl(pNMListView->iItem, pNMListView->iSubItem);
    }
    *pResult = 0;
}

/*******************************************************************************
 Function Name  : OnClick
 Description    : Event Handler. This handler will show the controls to edit the
                  selected data from the list control only if signle click
                  activate property is set. This will make list control editable
                  with a single click.
 Member of      : CListCtrlEx
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  : Raja N on 01.08.2004, Modified the function name ShowControl
                  as vShowControl.
*******************************************************************************/
void CListCtrlEx::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
    if( m_bSingleClickActivate == TRUE )
    {
        // Set the focus to the list control
        if( GetFocus() != this)
        {
            SetFocus();
        }
        
        NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
        // Call Handler Function with required parameters
        vShowControl(pNMListView->iItem, pNMListView->iSubItem);
    }
    
    *pResult = 0;
}

/*******************************************************************************
 Function Name  : vSetColumnInfo
 Inputs         : nRow          - Row Index
                  nColumn       - Column Index
                  sInfo         - List info structure
 Output         :   -
 Description    : This function will be called to set the list item type. This
                  will call lGetMapID to derive a unique number from row-column
                  index. This will update the CMap with the type information.
 Member of      : CListCtrlEx
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  :
*******************************************************************************/
void CListCtrlEx::vSetColumnInfo(int nRow, int nColunm, SLISTINFO sInfo)
{
    // Get the unique map index
    int nIndex = lGetMapID(nRow, nColunm );
    // Update the map with the type information
    m_omListItemType[ nIndex ] = sInfo;
}

/*******************************************************************************
 Function Name  : sGetColumnInfo
 Inputs         : nRow      - Index of the intrested Row
                  nColumn   - Index of the intrested Column
 Output         : SLISTINFO - Column Type info
 Description    : This function will be called to get the list item type. This
                  will call lGetMapID to derive a unique number from row-column
                  index. This will return the value stored in the CMap
 Member of      : CListCtrlEx
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  :
*******************************************************************************/
SLISTINFO CListCtrlEx::sGetColumnInfo(int nRow, int nColunm)
{
    return m_omListItemType [ lGetMapID(nRow, nColunm) ];
}

/*******************************************************************************
 Function Name  : lGetMapID
 Inputs         : nRow      - Index of the Row
                  nColumn   - Index of the Column
 Output         : long      - Unique map index derived from Row & Col val
 Description    : This function will return the unique map index. This will use
                  virtual column count to make the 2d number to linear.
 Member of      : CListCtrlEx
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  : Modifications as per code review comment. A new member is
                  added to the class to keep the virtual column count. This can
                  be dynamically changed.
*******************************************************************************/
long CListCtrlEx::lGetMapID(int nRow, int nCol)
{
    // Calculate the unique value
    return nRow * m_nVirtualColumnCount + nCol;
}

/*******************************************************************************
 Function Name  : vShowControl
 Inputs         : nRow      - Index of the Row
                  nColumn   - Index of the Column
 Output         :   -
 Description    : This function will show the UI control to change the list item
                  text. This will get the type information from the Cmap and
                  will call approp. function to create and show that control.
 Member of      : CListCtrlEx
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  : Raja N on 30.07.2004, Code review comments implemented
*******************************************************************************/
void CListCtrlEx::vShowControl(int nItem, int nSubItem)
{
    // Proceed only for a valid entry
    if( nItem >= 0 && nSubItem >= 0)
    {
        SLISTINFO sInfo;
        SNUMERICINFO    sNumInfo;
        SUSERPROGINFO   sProgInfo;
        CString omStr = STR_EMPTY;
        // Got the entry type from the CMap
        if( m_omListItemType.Lookup(
                lGetMapID(nItem, nSubItem) , sInfo) == TRUE )
        {
            // Begining of Controls creation
            m_bCreating = TRUE;
            switch( sInfo.m_eType)
            {
                case eBrowser:
                    pomBrowserIem(nItem, nSubItem, sInfo.m_omEntries);
                    break;
                case eNumber:
                case eBuddy:
                    // Get the numeric control parameters
                    if( m_omNumDetails.Lookup( lGetMapID(nItem, nSubItem),
                                                sNumInfo ) == TRUE )
                    {
                        pomNumItem(nItem, nSubItem, sNumInfo);
                    }
                    else
                    {
                        // Numeric info is not set
                        ASSERT( FALSE );
                        // Call with default value
                        pomNumItem(nItem, nSubItem, sNumInfo);
                    }
                    break;
                // General Edit control
                case eText:
                    pomEditItem(nItem, nSubItem);
                    break;
                // Editalble Combo Box
                case eComboList:
                    pomComboList(nItem, nSubItem, sInfo.m_omEntries);
                    break;
                // Non - Editable combo box
                case eComboItem:
                    pomComboItem(nItem, nSubItem, sInfo.m_omEntries);
                    break;
                // User function will be executed
                case eUser:
                    // Get the user program pointer and parameter details
                    if( m_omUserProg.Lookup( lGetMapID(nItem, nSubItem),
                                                sProgInfo ) == TRUE )
                    {
                        sProgInfo.m_pfHandler( this,
                                               nItem,
                                               nSubItem,
                                               sProgInfo.m_pUserParam);
                    }
                    else
                    {
                        // User program information is not set
                        ASSERT( FALSE );
                    }
                    break;
                // Toggling type control
                case eBool:
                    // Get the current text
                    omStr = GetItemText(nItem, nSubItem);
                    // Compare with the first item
                    if( sInfo.m_omEntries.GetAt(0).Compare(omStr) == 0 )
                    {
                        // Toggle the first with the second item text.
                        omStr = sInfo.m_omEntries.GetAt(1);
                    }
                    // Compare with the Second item
                    else if( sInfo.m_omEntries.GetAt(1).Compare(omStr) == 0 )
                    {
                        // Replace with the first item
                        omStr = sInfo.m_omEntries.GetAt(0);
                    }
                    // If it is not matching with these two items nothing will
                    // happen. This could be used to disable the control

                    // For boolean type this is the end of Controls creation
                    m_bCreating = FALSE;
                    
                    // For boolean send the EndLAbleEdit message here itself
                    LV_DISPINFO lvDispInfo;
                    lvDispInfo.hdr.hwndFrom = m_hWnd;
                    lvDispInfo.hdr.idFrom = GetDlgCtrlID(); 
                    lvDispInfo.hdr.code = LVN_ENDLABELEDIT;
                    lvDispInfo.item.mask = LVIF_TEXT;   
                    lvDispInfo.item.iItem = nItem;
                    lvDispInfo.item.iSubItem = nSubItem;
                    lvDispInfo.item.pszText = LPTSTR((LPCTSTR)omStr);
                    lvDispInfo.item.cchTextMax = omStr.GetLength();
                    SendMessage( WM_NOTIFY, GetDlgCtrlID(),(LPARAM)&lvDispInfo);
                    break;
            case eNoControl:
                    break;
                default:
                    // Unknown control type
                    ASSERT( FALSE );
            }
            // End of Controls
            m_bCreating = FALSE;
        }
    }
}

/*******************************************************************************
 Function Name  : pomComboItem
 Inputs         : nRow      - Index of the Row
                  nColumn   - Index of the Column
                  omList    - List of strings
 Output         : CComboItem* - Pointer to the control
 Description    : This function will create and show non editable combo box.This
                  control will be placed over the list item. The combobox will
                  be filled with the data given the omList.
 Member of      : CListCtrlEx
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  :
*******************************************************************************/
CComboItem * CListCtrlEx::pomComboItem(int nItem,
                                      int nSubItem,
                                      const CStringArray& omList)
{
    // Get the item text from the list control
    CString strFind = GetItemText(nItem, nSubItem);

    //basic code start
    CRect omRect;
    // Make sure that the item is visible
    if( !EnsureVisible(nItem, TRUE)) 
        return NULL;
    // Get the size of the list item
    GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, omRect);
    // Now scroll if we need to expose the column
    CRect omClientRect;
    // Get the list rect
    GetClientRect(omClientRect);
    // Check for scrolling
    if( omRect.left < 0 || omRect.left > omClientRect.right )
    {
        CSize size;
        size.cx = omRect.left;
        size.cy = 0;
        Scroll(size);
        omRect.left -= size.cx;
    }

    omRect.right = omRect.left + GetColumnWidth(nSubItem);
    // Reduce the size of the control if the list item is not completely
    // Visible
    if(omRect.right > omClientRect.right)
    {
        omRect.right = omClientRect.right;
    }
    //basic code end
    
    //dropdown area
    omRect.bottom += 100;
    
    // Set the standard style and combobox type
    DWORD dwStyle =  WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
                     CBS_DROPDOWNLIST;
    // Create the non editable combobox
    CComboItem *pomCBox = NULL;
    // Create the control
    pomCBox = new CComboItem( nItem,        // Item Index
                              nSubItem,     // Sub Item Index
                              omList,       // Lsit of strings
                              strFind,      // Selected Text
                              FALSE);       // Editing is FALSE
    if( pomCBox != NULL )
    {
        // Create the UI
        pomCBox->Create(dwStyle, omRect, this, IDC_CONTROL);
        // Set the text
        pomCBox->SetWindowText(strFind);
        // Select the text from the list
        pomCBox->SelectString(-1 , strFind.GetBuffer(1)); 
        // Show the drop-down list
        pomCBox->ShowDropDown();
    }
    else
    {
        CString omStrErr;
        omStrErr.Format(defFLC_CREATE_FAILED,defCOMBO_ITEM);
        AfxMessageBox( omStrErr );
    }    
    // The return the pointer just for reference
    // Destroy will delete this memory. So this should not be deleted outside
    return pomCBox;
}


/*******************************************************************************
 Function Name  : pomComboList
 Inputs         : nRow      - Index of the Row
                  nColumn   - Index of the Column
                  omList    - List of strings
 Output         : CComboItem* - Pointer to the control
 Description    : This function will create and show editable combo box.This
                  control will be placed over the list item. The combobox will
                  be filled with the data given the omList.
 Member of      : CListCtrlEx
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  :
*******************************************************************************/
CComboItem * CListCtrlEx::pomComboList( int nItem,
                                          int nSubItem,
                                          const CStringArray& omList)
{
    // Get the item text from the list control
    CString strFind = GetItemText(nItem, nSubItem);

    //basic code start
    CRect omRect;
    // Make sure that the item is visible
    if( !EnsureVisible(nItem, TRUE)) 
        return NULL;
    // Get the size of the list item
    GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, omRect);
    // Now scroll if we need to expose the column
    CRect omClientRect;
    // Get the list rect
    GetClientRect(omClientRect);
    // Check for scrolling
    if( omRect.left < 0 || omRect.left > omClientRect.right )
    {
        CSize size;
        size.cx = omRect.left;
        size.cy = 0;
        Scroll(size);
        omRect.left -= size.cx;
    }

    omRect.right = omRect.left + GetColumnWidth(nSubItem);
    // Reduce the size of the control if the list item is not completely
    // Visible
    if(omRect.right > omClientRect.right)
    {
        omRect.right = omClientRect.right;
    }
    //basic code end

    //dropdown area
    omRect.bottom += 100;

    // Set the standard style and combobox type
    DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
                    CBS_DROPDOWN;

    CComboItem *pomCBox = NULL;
    pomCBox = new CComboItem(nItem, nSubItem, omList, strFind, TRUE);
    
    if( pomCBox != NULL )
    {
        // Create the UI control
        pomCBox->Create(dwStyle, omRect, this, IDC_CONTROL);
        // Show the Drop-down list
        pomCBox->ShowDropDown();
        // Set the seleted item text
        pomCBox->SetWindowText(strFind);
        // Select the item from the list. If it is not available the
        // Set the text with out any selection
        if (pomCBox->SelectString(-1, strFind.GetBuffer(1)) == CB_ERR )
        {
            // Set the with out any selection from the list
            pomCBox->SetWindowText(strFind);
        }
    }
    else
    {
        CString omStrErr;
        omStrErr.Format(defFLC_CREATE_FAILED,defCOMBO_LIST);
        AfxMessageBox( omStrErr );
    }
    
    // The returned pointer should not be saved
    return pomCBox;
}

/*******************************************************************************
 Function Name  : pomEditItem
 Inputs         : nRow      - Index of the Row
                  nColumn   - Index of the Column
 Output         : CEdit * - Pointer to the control
 Description    : This function will create and show a editbox.
 Member of      : CListCtrlEx
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  :
*******************************************************************************/
CEdit *CListCtrlEx::pomEditItem(int nItem, int nSubItem)
{
    // Item rect and Client rect
    CRect omRect, omClientRect;
    // Set the item to be visible
    if(!EnsureVisible(nItem, TRUE))
    { 
        return NULL;    
    }
    // Get the item rect
    GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, omRect);
    // Now scroll if we need to expose the column
    GetClientRect(omClientRect);
    if( omRect.left < 0 || omRect.left > omClientRect.right )
    {
        CSize size(omRect.left,0);      
        Scroll(size);
        omRect.left -= size.cx;
    }

    omRect.right = omRect.left + GetColumnWidth(nSubItem);
    // If the size is bigger then the client size then resizes
    if(omRect.right > omClientRect.right)
    {
       omRect.right = omClientRect.right;
    }

    // Get Column alignment 
    LV_COLUMN lvcol;
    lvcol.mask = LVCF_FMT;
    GetColumn(nSubItem, &lvcol);

    DWORD dwStyle;
    // Get the justification style of the list item
    if((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
    {
        dwStyle = ES_LEFT;
    }
    else if((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
    {
        dwStyle = ES_RIGHT;
    }
    else
    {
        dwStyle = ES_CENTER;
    }
    // Include standard styles
    dwStyle |=WS_BORDER|WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;
    // Get the item text
    CString omStrText = GetItemText(nItem, nSubItem);
    // Create the control now
    CEdit *pomEdit = NULL;
    pomEdit = new CEditItem(nItem, nSubItem, omStrText);
    if( pomEdit != NULL )
    {
        pomEdit->Create(dwStyle, omRect, this, IDC_CONTROL);
    }
    else
    {
        CString omStrErr;
        omStrErr.Format( defFLC_CREATE_FAILED, defEDIT_ITEM );
        AfxMessageBox( omStrErr );
    }
    // Retrun the window pointer
    return pomEdit;
}

/*******************************************************************************
 Function Name  : pomNumItem
 Inputs         : nRow      - Index of the Row
                  nColumn   - Index of the Column
                  sInfo     - Numeric info like Min val, Max val, Base...
                  Please see the structure definition to get all field info
 Output         : CEdit * - Pointer to the control
 Description    : This function will create and show a numeric edit control with
                  or without spin control
 Member of      : CListCtrlEx
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  :
*******************************************************************************/
CNumEdit *CListCtrlEx::pomNumItem( int nItem, int nSubItem,
                                     const SNUMERICINFO& sInfo)
{
    CRect omRect;
    // Set the item to be visible
    if(!EnsureVisible(nItem, TRUE))
    { 
        return NULL;    
    }
    // Get the item rect
    GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, omRect);
    // Now scroll if we need to expose the column
    CRect omClientRect;
    GetClientRect(omClientRect);
    if( omRect.left < 0 || omRect.left > omClientRect.right )
    {
        CSize size( omRect.left, 0 );     
        Scroll( size );
        omRect.left -= size.cx;
    }
    omRect.right = omRect.left + GetColumnWidth(nSubItem);
    if(omRect.right > omClientRect.right)
    {
        omRect.right = omClientRect.right;
    }

    // Get Column alignment 
    LV_COLUMN lvcol;
    lvcol.mask = LVCF_FMT;
    GetColumn(nSubItem, &lvcol);

    DWORD dwStyle;
    if((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
    {
        dwStyle = ES_LEFT;
    }
    else if((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
    {
        dwStyle = ES_RIGHT;
    }
    else 
    {
        dwStyle = ES_CENTER;
    }
    // Set the standard windows style 
    dwStyle |=WS_BORDER|WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;
    // Get the selected item text
    CString omStrText = GetItemText(nItem, nSubItem);
    // Create the control
    CNumEdit *pomEdit = NULL;
    pomEdit = new CNumEdit(nItem, nSubItem, omStrText, sInfo);
    if( pomEdit != NULL )
    {
        pomEdit->Create(dwStyle, omRect, this, IDC_CONTROL);
    }
    else
    {
        CString omStrErr;
        omStrErr.Format( defFLC_CREATE_FAILED, defNUM_ITEM );
        AfxMessageBox( omStrErr );
    }
    // Return the window pointer
    return pomEdit;
}

/*******************************************************************************
 Function Name  : vSetNumericInfo
 Inputs         : nRow          - Row Index
                  nColumn       - Column Index
                  sInfo         - Numeric info structure
 Output         :   -
 Description    : This function will be called to set the list item numeric
                  information. This will set the value in the numeric CMap
 Member of      : CListCtrlEx
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  :
*******************************************************************************/
void CListCtrlEx::vSetNumericInfo( int nRow, int nColunm, 
                                     const SNUMERICINFO& sInfo)
{
    // Get the Unique Index
    int nIndex = lGetMapID(nRow, nColunm);
    // Update the map data
    m_omNumDetails[ nIndex ] = sInfo;
}

/*******************************************************************************
 Function Name  : vSetUserProgInfo
 Inputs         : nRow          - Row Index
                  nColumn       - Column Index
                  sInfo         - User Program info structure
 Output         :   -
 Description    : This function will be called to set the list item user program
                  information. This will set the value in the user prog CMap
 Member of      : CListCtrlEx
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  :
*******************************************************************************/
void CListCtrlEx::vSetUserProgInfo( int nRow, int nColunm, 
                                     const SUSERPROGINFO& sUSerProgInfo)
{
    // Get the Unique Index
    int nIndex = lGetMapID(nRow, nColunm);
    // Update the map data
    m_omUserProg[ nIndex ] = sUSerProgInfo;
}

/*******************************************************************************
 Function Name  : vSetUserProgInfo
 Inputs         : rs_DispInfo   - Reference to Display Information Structure
                  sInfo         - User Program info structure
 Output         : BOOL - Success or Failure
 Description    : This function will return the last edit item details. This is
                  required to get the Row & Col information from the
                  OnItemChanged handler. To avoid validation during insert it
                  will return false telling that data is invalid.
 Member of      : CListCtrlEx
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  :
*******************************************************************************/
BOOL CListCtrlEx::sGetModificationStructure(LV_DISPINFO& rs_DispInfo)
{
    BOOL bSuccess = TRUE;
    // If insertion is in progress don't copy the invalid data
    if( m_bCreating == FALSE )
    {
        // Copy the information
        rs_DispInfo = m_sModifiedInfo;
    }
    else
    {
        bSuccess = FALSE;
    }
    // Return the result
    return bSuccess;
}

/*******************************************************************************
 Function Name  : vGoToEditMode
 Inputs         : nRow          - Row Index
                  nColumn       - Column Index
 Output         :   -
 Description    : This function will be called to show the control to edit the
                  item. This will be useful to show the control during invalid
                  input entered by the user
 Member of      : CListCtrlEx
 Author(s)      : Raja N
 Date Created   : 22.07.2004
 Modifications  : Raja N on 30.07.2004, Renamed the function name as per code
                  review comments.
*******************************************************************************/
void CListCtrlEx::vGoToEditMode(int nItem, int nSubItem)
{
    // Show Appropriate UI controls
    vShowControl(nItem, nSubItem);
}

/*******************************************************************************
 Function Name  : vSetMapColumnCount
 Inputs         : nColumn       - No. of expected Columns
 Output         :   -
 Description    : This function will set the virtual column count that is used
                  to derive the lineat key from row-column index. This will
                  clear all cmap's content as the unique key generator got
                  modified.
 Member of      : CListCtrlEx
 Author(s)      : Raja N
 Date Created   : 30.07.2004
 Modifications  :
*******************************************************************************/
void CListCtrlEx::vSetMapColumnCount(int nColumn)
{
    // Set the virtual column count
    m_nVirtualColumnCount = nColumn;
    // Clear item type map
    m_omListItemType.RemoveAll();
    // Clear numeric information map
    m_omNumDetails.RemoveAll();
    // Clear user program map
    m_omUserProg.RemoveAll();
}

//Added by venkat

/******************************************************************************
Function Name  :  pomBrowserIem
Input(s)       :  
Output         :  CWnd
Functionality  :   
Member of      :  *CListCtrlEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
CWnd *CListCtrlEx::pomBrowserIem(int nItem, int nSubItem, CStringArray& omList)
{
    // Item rect and Client rect
    CRect omRect, omClientRect;
    // Set the item to be visible
    if(!EnsureVisible(nItem, TRUE))
    { 
        return NULL;    
    }
    // Get the item rect
    GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, omRect);
    // Now scroll if we need to expose the column
    GetClientRect(omClientRect);
    if( omRect.left < 0 || omRect.left > omClientRect.right )
    {
        CSize size(omRect.left,0);      
        Scroll(size);
        omRect.left -= size.cx;
    }

    omRect.right = omRect.left + GetColumnWidth(nSubItem);
    // If the size is bigger then the client size then resizes
    if(omRect.right > omClientRect.right)
    {
       omRect.right = omClientRect.right;
    }

    // Get Column alignment 
    LV_COLUMN lvcol;
    lvcol.mask = LVCF_FMT;
    GetColumn(nSubItem, &lvcol);

    DWORD dwStyle;
    // Get the justification style of the list item
    if((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
    {
        dwStyle = ES_LEFT;
    }
    else if((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
    {
        dwStyle = ES_RIGHT;
    }
    else
    {
        dwStyle = ES_CENTER;
    }
    // Include standard styles
    dwStyle |=WS_BORDER|WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;
    // Get the item text
    CString omStrText = GetItemText(nItem, nSubItem);
    // Create the control now
    CRect omEditRect = omRect;
    CRect omButtonRect = omRect;

    omButtonRect.left = omRect.right - def_WIDTH_BUTTON;
    omEditRect.right = omRect.right - def_HEIGHT_BUTTON;

	if(omList.GetSize() >=2)
    {
        CButtonItem *pomButton  = new CButtonItem(omList.GetAt(0), omList.GetAt(1));
        CBrowseEditItem *pomEdit = new CBrowseEditItem(nItem, nSubItem, omStrText, pomButton);
        pomButton->vSetEditItem(pomEdit);
       
        if( pomEdit != NULL )
        {
            pomEdit->Create(dwStyle, omEditRect, this, IDC_CONTROL);
        }
        if(pomButton != NULL)
        {
            dwStyle = WS_CHILD|WS_VISIBLE|BS_TEXT|BS_BOTTOM;
            //ID is not required
            pomButton->Create("...", dwStyle, omButtonRect, this, 0); 
        }
        //Edit box will have the focus first
        pomEdit->SetFocus();        
    }
    return NULL;
}
/******************************************************************************
Function Name  :  OnEraseBkgnd
Input(s)       :  CDC* - The pointer of Client DC
Output         :  BOOL - Nonzero if it erases the background; otherwise 0.
Functionality  :   
Member of      :  CListCtrlEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
BOOL CListCtrlEx::OnEraseBkgnd(CDC* pDC)
{
    CRect omClientRect;
    POINT omItemPos;  

    int nTotalItems =  GetCountPerPage();
    GetClientRect(omClientRect);

    for(int i=0;i<nTotalItems+2;i++)
    {
        GetItemPosition(i,&omItemPos);
        omClientRect.top=omItemPos.y ;
        GetItemPosition(i+1,&omItemPos);
        omClientRect.bottom =omItemPos.y;
        CBrush omBrush((i % 2) ? m_colRow1 : m_colRow2);
        pDC->FillRect(&omClientRect, &omBrush);
        omBrush.DeleteObject();
    }
    //Nonzero if it erases the background; otherwise 0.
    return TRUE;
}

/******************************************************************************
Function Name  :  OnNMCustomdraw
Input(s)       :  NMHDR* - Contains information about a notification message. 
                  LRESULT* - Output value
Output         :  -
Functionality  :  Paints The ListControl rows with the specified colors 
Member of      :  CListCtrlEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
void CListCtrlEx::OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
{
    NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
    *pResult = 0;
    
    if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
    {
        *pResult = CDRF_NOTIFYITEMDRAW;         //Use Default Drawing
    }
    else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
    {
        *pResult = CDRF_NOTIFYSUBITEMDRAW;      //Use Default Repainting
    }
    else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
    {
        COLORREF omcrText, omcrBkgnd;
        if((pLVCD->nmcd.dwItemSpec % 2) == 0)       
        {
            omcrBkgnd = m_colRow2;
            omcrText = def_COLOR_ROW1_TEXT;
        }
        else
        {
            omcrBkgnd = m_colRow1;
            omcrText = def_COLOR_ROW2_TEXT;

        }
        pLVCD->clrText = omcrText;
        pLVCD->clrTextBk = omcrBkgnd;
       
        *pResult = CDRF_DODEFAULT;
    }
}

/******************************************************************************
Function Name  :  vSetRowColors
Input(s)       :  COLORREF crRow1 - even number row color
                  COLORREF crRow2 - odd number row color
Output         :  -
Functionality  :  Sets the Row1 and Row2 Colors 
Member of      :  CListCtrlEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
VOID CListCtrlEx::vSetRowColors(COLORREF crRow1, COLORREF crRow2)
{
    m_colRow1 =  crRow1;
    m_colRow2 = crRow2;
    RedrawWindow();
}

/******************************************************************************
Function Name  :  vGetRowColors
Input(s)       :  COLORREF crRow1 - even number row color
                  COLORREF crRow2 - odd number row color
Output         :  -
Functionality  :  Returns the Row1 and Row2 colors in crRow1, crRow2 
Member of      :  CListCtrlEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
VOID CListCtrlEx::vGetRowColors(COLORREF& colRow1, COLORREF& colRow2)
{
    colRow1 = m_colRow1;
    colRow2 = m_colRow2;
}

/******************************************************************************
Function Name  :  vSetDefaultColors
Input(s)       :  -
Output         :  - 
Functionality  :  Assigns default colors to the even and odd rows 
Member of      :  CListCtrlEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
VOID CListCtrlEx::vSetDefaultColors(void)
{
    m_colRow2 =     def_COLOR_SECONDROW;
    m_colRow1 =     def_COLOR_FIRSTROW;
}

BOOL CListCtrlEx::PreTranslateMessage(MSG* pMsg)
{
// TODO: Add your specialized code here and/or call the base class
    if(pMsg->message == WM_KEYDOWN)
    {
        if(GetKeyState(VK_TAB)<0)
        {
            AfxTrace(_T("I got the message\n"));
        }
    }
    return CFFListCtrl::PreTranslateMessage(pMsg);
}
 