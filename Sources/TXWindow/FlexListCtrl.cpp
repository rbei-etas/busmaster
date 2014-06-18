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
 * @brief Implementation file for CFlexListCtrl
 * @author Raja N
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for CFlexListCtrl
 */

// For all standard header file include
#include "TxWindow_stdafx.h"
#include "Utility/ComboItem.h"              // For Custom Combobox Implementation
#include "Utility/EditItem.h"               // For Custom Editbox Implementation
#include "Utility/Utility_Structs.h"                 // For data struct definition
#include "Utility/RadixEdit.h"             // For the RAdix Edit control definition
#include "Utility/numSpinCtrl.h"            // For the custom spin control
#include "Utility/FFListctrl.h"             // For Flicker Free List class definition
#include "Utility/NumEdit.h"                // For Custom Numeric Edit control Impl
#include "FlexListCtrl.h"           // Interface file for Flex List Control
#include "Utility/MultiLanguageSupport.h"

/** ID for Combobox */
#define IDC_CONTROL 0x12345

CFlexListCtrl::CFlexListCtrl() :
    m_bSingleClickActivate(FALSE)
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
}

CFlexListCtrl::~CFlexListCtrl()
{
}

BEGIN_MESSAGE_MAP(CFlexListCtrl, CListCtrl)
    ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
    ON_NOTIFY_REFLECT(NM_DBLCLK, OnDoubleClick)
    ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
END_MESSAGE_MAP()

void CFlexListCtrl::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{
    LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
    // Get the item index
    LV_ITEM* plvItem = &pDispInfo->item;
    // Proceed only for valid item change
    if( plvItem->iItem != -1 &&  // valid item
            plvItem->pszText != nullptr)       // valid text
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
        }
    }
    *pResult = 0;
}

void CFlexListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // Set the focus to list control. This will hide any controls that
    // are all visible at this time
    if( GetFocus() != this)
    {
        SetFocus();
    }

    CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CFlexListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // Set the focus to list control. This will hide any controls that
    // are all visible at this time
    if( pScrollBar == nullptr ) // Form the (list )window
    {
        if( GetFocus() != this)
        {
            SetFocus();
        }
    }
    CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CFlexListCtrl::OnDoubleClick(NMHDR* pNMHDR, LRESULT* pResult)
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
        lvDispInfo.item.pszText = nullptr;
        lvDispInfo.item.cchTextMax = 0;
        CWnd* pWnd = GetParent();
        if( pWnd != nullptr )
        {
            pWnd->SendMessage( WM_NOTIFY, GetDlgCtrlID(),
                               (LPARAM)&lvDispInfo );
        }
        // Call Handler Function with required parameters
        vShowControl(pNMListView->iItem, pNMListView->iSubItem);
    }
    *pResult = 0;
}

void CFlexListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult)
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

void CFlexListCtrl::vSetColumnInfo(int nRow, int nColunm, SLISTINFO sInfo)
{
    // Get the unique map index
    int nIndex = lGetMapID(nRow, nColunm );
    // Update the map with the type information
    m_omListItemType[ nIndex ] = sInfo;
}

SLISTINFO CFlexListCtrl::sGetColumnInfo(int nRow, int nColunm)
{
    return m_omListItemType [ lGetMapID(nRow, nColunm) ];
}

long CFlexListCtrl::lGetMapID(int nRow, int nCol)
{
    // Calculate the unique value
    return nRow * m_nVirtualColumnCount + nCol;
}

void CFlexListCtrl::vShowControl(int nItem, int nSubItem)
{
    // Proceed only for a valid entry
    if( nItem >= 0 && nSubItem >= 0)
    {
        SLISTINFO sInfo;
        SNUMERICINFO    sNumInfo;
        SUSERPROGINFO   sProgInfo;
        CString omStr = "";
        // Got the entry type from the CMap
        if( m_omListItemType.Lookup(
                    lGetMapID(nItem, nSubItem) , sInfo) == TRUE )
        {
            // Begining of Controls creation
            m_bCreating = TRUE;
            switch( sInfo.m_eType)
            {
                    // Numeric Edit box with or with out Spin Control
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
                default:
                    // Unknown control type
                    ASSERT( FALSE );
            }
            // End of Controls
            m_bCreating = FALSE;
        }
    }
}

CComboItem* CFlexListCtrl::pomComboItem(int nItem,
                                        int nSubItem,
                                        const CStringArray& omList)
{
    // Get the item text from the list control
    CString strFind = GetItemText(nItem, nSubItem);

    //basic code start
    CRect omRect;
    // Make sure that the item is visible
    if( !EnsureVisible(nItem, TRUE))
    {
        return nullptr;
    }

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
    CComboItem* pomCBox = nullptr;
    // Create the control
    pomCBox = new CComboItem( nItem,        // Item Index
                              nSubItem,     // Sub Item Index
                              omList,       // Lsit of strings
                              strFind,      // Selected Text
                              FALSE);       // Editing is FALSE
    if( pomCBox != nullptr )
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
        omStrErr.Format(_(defFLC_CREATE_FAILED),defCOMBO_ITEM);
        AfxMessageBox( omStrErr );
    }
    // The return the pointer just for reference
    // Destroy will delete this memory. So this should not be deleted outside
    return pomCBox;
}

CComboItem* CFlexListCtrl::pomComboList( int nItem,
        int nSubItem,
        const CStringArray& omList)
{
    // Get the item text from the list control
    CString strFind = GetItemText(nItem, nSubItem);

    //basic code start
    CRect omRect;
    // Make sure that the item is visible
    if( !EnsureVisible(nItem, TRUE))
    {
        return nullptr;
    }

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

    CComboItem* pomCBox = nullptr;
    pomCBox = new CComboItem(nItem, nSubItem, omList, strFind, TRUE);

    if( pomCBox != nullptr )
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
        omStrErr.Format(_(defFLC_CREATE_FAILED),defCOMBO_LIST);
        AfxMessageBox( omStrErr );
    }

    // The returned pointer should not be saved
    return pomCBox;
}

CEdit* CFlexListCtrl::pomEditItem(int nItem, int nSubItem)
{
    // Item rect and Client rect
    CRect omRect, omClientRect;
    // Set the item to be visible
    if(!EnsureVisible(nItem, TRUE))
    {
        return nullptr;
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
    CEdit* pomEdit = nullptr;
    pomEdit = new CEditItem(nItem, nSubItem, omStrText);
    if( pomEdit != nullptr )
    {
        pomEdit->Create(dwStyle, omRect, this, IDC_CONTROL);
    }
    else
    {
        CString omStrErr;
        omStrErr.Format( _(defFLC_CREATE_FAILED), defEDIT_ITEM );
        AfxMessageBox( omStrErr );
    }
    // Retrun the window pointer
    return pomEdit;
}

CNumEdit* CFlexListCtrl::pomNumItem( int nItem, int nSubItem,
                                     const SNUMERICINFO& sInfo)
{
    CRect omRect;
    // Set the item to be visible
    if(!EnsureVisible(nItem, TRUE))
    {
        return nullptr;
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
    CNumEdit* pomEdit = nullptr;
    pomEdit = new CNumEdit(nItem, nSubItem, omStrText, sInfo);
    if( pomEdit != nullptr )
    {
        pomEdit->Create(dwStyle, omRect, this, IDC_CONTROL);
    }
    else
    {
        CString omStrErr;
        omStrErr.Format( _(defFLC_CREATE_FAILED), defNUM_ITEM );
        AfxMessageBox( omStrErr );
    }
    // Return the window pointer
    return pomEdit;
}

void CFlexListCtrl::vSetNumericInfo( int nRow, int nColunm,
                                     const SNUMERICINFO& sInfo)
{
    // Get the Unique Index
    int nIndex = lGetMapID(nRow, nColunm);
    // Update the map data
    m_omNumDetails[ nIndex ] = sInfo;
}

void CFlexListCtrl::vSetUserProgInfo( int nRow, int nColunm,
                                      const SUSERPROGINFO& sUSerProgInfo)
{
    // Get the Unique Index
    int nIndex = lGetMapID(nRow, nColunm);
    // Update the map data
    m_omUserProg[ nIndex ] = sUSerProgInfo;
}

BOOL CFlexListCtrl::sGetModificationStructure(LV_DISPINFO& rs_DispInfo)
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

void CFlexListCtrl::vGoToEditMode(int nItem, int nSubItem)
{
    // Show Appropriate UI controls
    vShowControl(nItem, nSubItem);
}

void CFlexListCtrl::vSetMapColumnCount(int nColumn)
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
