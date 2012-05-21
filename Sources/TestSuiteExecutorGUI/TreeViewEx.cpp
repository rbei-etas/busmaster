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
 * \file      TreeViewEx.cpp
 * \brief     Implementation file for the class CTSExecutorChildFrame
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for the class CTSExecutorChildFrame
 */


// TreeViewEx.cpp : implementation file
//

#include "TSExecutorGUI_stdafx.h"
#include <afxtempl.h>
#include "TSExecutorGUI_resource.h"
#include "TSExecutorGUI_ChildFrame.h"
//#include "TreeViewEx.h"


//TODO::Move to Definition File
#define def_ID_TESTSUITE            -1
IMPLEMENT_DYNCREATE(CTreeViewEx, CTreeView)

CTreeViewEx::CTreeViewEx()
{
    m_bDragging = FALSE;
    m_bEditing = FALSE;
    m_pomMsgTarget = NULL;
    m_hCurNoDrop = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_NO));
    m_hOrigCursor = ::GetCursor();
}

CTreeViewEx::~CTreeViewEx()
{
}


BEGIN_MESSAGE_MAP(CTreeViewEx, CTreeView)
    ON_WM_LBUTTONDOWN()
    ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnTvnBegindrag)
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_NOTIFY_REFLECT(NM_RCLICK, OnNMRclick)
    ON_COMMAND(IDM_COLLAPSE_ALL, vCollapseFull)
    ON_COMMAND(IDM_EXPAND_ALL, vExpandFull)
    ON_COMMAND_RANGE(IDM_TESTSUITE_ADD, IDM_TESTSETUP_MODIFY, OnCommandRange)
    ON_UPDATE_COMMAND_UI(IDM_TESTSUITE_EXECUTE, onUpdateExecute)
    ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnTvnSelchanged)
    ON_WM_CTLCOLOR_REFLECT()
    ON_NOTIFY_REFLECT(TVN_SELCHANGING, OnTvnSelchanging)
    ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnTvnBeginlabeledit)
    ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnTvnEndlabeledit)
END_MESSAGE_MAP()


// CTreeViewEx diagnostics

#ifdef _DEBUG
void CTreeViewEx::AssertValid() const
{
    CTreeView::AssertValid();
}

void CTreeViewEx::Dump(CDumpContext& dc) const
{
    CTreeView::Dump(dc);
}
#endif //_DEBUG


void CTreeViewEx::OnInitialUpdate()
{
    CTreeView::OnInitialUpdate();
    GetTreeCtrl().SetBkColor(def_COLOR_TREE_BKG);
    CDC* pDC =  GetTreeCtrl().GetDC();
    pDC->SetBkMode(TRANSPARENT);
}

HTREEITEM CTreeViewEx::InsertTreeItem(HTREEITEM hParent, CString omstrItemName, HTREEITEM hTreeInsAfter,
                                      int nSelectedImage ,int nNonSelectedImage, long lParam)
{
    TV_ITEM tvItem;
    TV_INSERTSTRUCT tvInsertItem;
    tvItem.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE;
    tvItem.cchTextMax = omstrItemName.GetLength();
    tvItem.pszText = omstrItemName.GetBuffer(tvItem.cchTextMax);
    tvItem.lParam = lParam;
    tvItem.iImage = nNonSelectedImage;
    tvItem.iSelectedImage = nSelectedImage;
    tvInsertItem.item = tvItem;
    tvInsertItem.hInsertAfter = hTreeInsAfter;
    tvInsertItem.hParent = hParent;
    return GetTreeCtrl().InsertItem(&tvInsertItem);
}

BOOL CTreeViewEx::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
    dwStyle = dwStyle| WS_BORDER | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_EDITLABELS| TVS_SHOWSELALWAYS |TVS_CHECKBOXES;
    return CTreeView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CTreeViewEx::OnLButtonDown(UINT nFlags, CPoint point)
{
    UINT nHitTestFlags = 0;
    HTREEITEM hItem = GetTreeCtrl().HitTest(point, &nHitTestFlags);

    if((hItem != NULL) && (TVHT_ONITEMSTATEICON & nHitTestFlags))
    {
        BOOL bCheck = bIsItemChecked(hItem);
        vSetCheck(hItem, !bCheck);
        // vSetCheckChildren(hItem, !bCheck);
        //  vSetCheckParent(hItem);
        return;
    }

    CTreeView::OnLButtonDown(nFlags, point);
}

BOOL CTreeViewEx::bIsItemChecked(HTREEITEM hItem)
{
    if(hItem == NULL)
    {
        return FALSE;
    }

    CTreeCtrl& omTreeCtrl = GetTreeCtrl();
    return omTreeCtrl.GetItemState( hItem, TVIS_STATEIMAGEMASK )>>12 == 2;
}

void CTreeViewEx::vSetCheck(HTREEITEM hItem, BOOL bCheck)
{
    if(hItem == NULL)
    {
        return;
    }

    if (hItem != NULL)
    {
        int nState = (bCheck == TRUE) ? 2 : 1;
        GetTreeCtrl().SetItemState( hItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
        GetExecutorWindow()->vEnableItem((DWORD)GetTreeCtrl().GetItemData(hItem), bCheck);
    }

    return;
}
void CTreeViewEx::vSetCheckChildren(HTREEITEM hItem, BOOL fCheck)
{
    if(hItem == NULL)
    {
        return;
    }

    CTreeCtrl& omTreeCtrl = GetTreeCtrl();
    HTREEITEM hNext = omTreeCtrl.GetChildItem(hItem);
    vSetCheck(hItem, fCheck);

    // loop to set item state for children
    if ( omTreeCtrl.ItemHasChildren(hItem))
    {
        HTREEITEM htiChild = omTreeCtrl.GetChildItem (hItem);

        if (htiChild == NULL)
        {
            return;
        }
        else
        {
            vSetCheckChildren(htiChild, fCheck);
        }

        HTREEITEM htiSibling = omTreeCtrl.GetNextSiblingItem (htiChild);

        while (htiSibling )
        {
            vSetCheckChildren(htiSibling, fCheck);
            htiSibling = omTreeCtrl.GetNextSiblingItem(htiSibling);
        }
    }

    return;
}
void CTreeViewEx::vSetCheckParent(HTREEITEM hItem)
{
    if(hItem == NULL)
    {
        return;
    }

    CTreeCtrl& omTreeCtrl = GetTreeCtrl();
    HTREEITEM hParentItem = omTreeCtrl.GetParentItem(hItem);
    HTREEITEM hChildItem;
    BOOL bAllChecked = TRUE;

    if( omTreeCtrl.ItemHasChildren(hParentItem))
    {
        hChildItem = omTreeCtrl.GetChildItem(hParentItem);

        while(hChildItem)
        {
            if(!bIsItemChecked(hChildItem))
            {
                bAllChecked = FALSE;
                break;
            }

            hChildItem = omTreeCtrl.GetNextSiblingItem(hChildItem);
        }
    }

    vSetCheck(hParentItem, bAllChecked);
    vSetCheckParent(hParentItem);
    return;
}

void CTreeViewEx::OnTvnBegindrag(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

    if (!m_bDragging)
    {
        if (bItemCanDragged(pNMTreeView->itemNew.hItem))
        {
            CTreeCtrl& omTreeCtrl = GetTreeCtrl();
            m_bDragging = TRUE;
            m_hDraggingItemgedItem = pNMTreeView->itemNew.hItem;
            omTreeCtrl.Select(m_hDraggingItemgedItem, TVGN_CARET);
            SetCapture();
        }
    }

    *pResult = 0;
}


BOOL CTreeViewEx::bItemCanDragged(HTREEITEM hItem)
{
    HTREEITEM hParent = GetTreeCtrl().GetParentItem(hItem);

    if(GetTreeCtrl().GetItemData(hParent) == def_ID_TESTSUITE)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL CTreeViewEx::IsItemCanDropOn(HTREEITEM hSource, HTREEITEM hTarget)
{
    CTreeCtrl& omTreeCtrl = GetTreeCtrl();
    BOOL bRetValue = FALSE;

    if(hSource != NULL || hTarget != NULL)
    {
        HTREEITEM hSourceParent = omTreeCtrl.GetParentItem(hSource);
        HTREEITEM hTargetParent = omTreeCtrl.GetParentItem(hTarget);

        if(hSourceParent == hTargetParent)
        {
            DWORD dwId = (DWORD)omTreeCtrl.GetItemData(hSourceParent);

            if(dwId == def_ID_TESTSUITE)
            {
                bRetValue = TRUE;
            }
        }
    }

    return bRetValue;
}


void CTreeViewEx::OnLButtonUp(UINT nFlags, CPoint point)
{
    if(m_bDragging == TRUE)
    {
        CTreeCtrl& omTreeCtrl = GetTreeCtrl();
        m_bDragging = FALSE;
        DragEnd();
        omTreeCtrl.SelectDropTarget(0);
        SetCursor(m_hOrigCursor);
        GetTreeCtrl().SetInsertMark(0, 0);
        ::ReleaseCapture();
    }

    CTreeView::OnLButtonUp(nFlags, point);
}
void CTreeViewEx::DragEnd()
{
    eTYPE_DROPPING eDropping;
    HTREEITEM hDroppingItem = GetDropItem(eDropping);

    if (hDroppingItem == NULL)
    {
        return;
    }

    CTreeCtrl& omTempTreeCtrl = GetTreeCtrl();
    UINT unFirstIndex = unGetIndex(m_hDraggingItemgedItem);
    HTREEITEM hInsertAfter = DragMoveItem(m_hDraggingItemgedItem, hDroppingItem, eDropping);

    if(hInsertAfter == NULL)
    {
        return;
    }

    DWORD_PTR dwRepositionItemID = omTempTreeCtrl.GetItemData(m_hDraggingItemgedItem);
    DWORD_PTR dwInsertAfterItemID;

    if(hInsertAfter == TVI_FIRST)
    {
        dwInsertAfterItemID = (DWORD)-1;
    }
    else
    {
        dwInsertAfterItemID = omTempTreeCtrl.GetItemData(hInsertAfter);
    }

    UINT unNewID = GetExecutorWindow()->unRepisitonEntry((DWORD)dwRepositionItemID, (DWORD)dwInsertAfterItemID);
    //omTempTreeCtrl.SetItemData(m_hDraggingItemgedItem, unNewID);
}
UINT CTreeViewEx::unGetIndex(HTREEITEM hTreeItem)
{
    CTreeCtrl& omTempTreeCtrl = GetTreeCtrl();
    HTREEITEM hParent = omTempTreeCtrl.GetParentItem(hTreeItem);

    if (omTempTreeCtrl.ItemHasChildren(hParent))
    {
        HTREEITEM hChildItemItem = omTempTreeCtrl.GetChildItem(hParent);
        int i = -1;

        while (hChildItemItem != NULL)
        {
            i++;

            if(hChildItemItem == hTreeItem)
            {
                return i;
            }

            hChildItemItem = omTempTreeCtrl.GetNextItem(hChildItemItem, TVGN_NEXT);
        }
    }

    return (UINT)-1;
}
HTREEITEM CTreeViewEx::DragMoveItem(HTREEITEM hDraggingItem, HTREEITEM hDroppingItem, eTYPE_DROPPING eDropping)
{
    CTreeCtrl& omTreeCtrl = GetTreeCtrl();

    if(eDropping == NO_DROPPING)
    {
        return NULL;
    }

    if(bAnscestor(hDraggingItem, hDroppingItem) || !IsItemCanDropOn(hDraggingItem, hDroppingItem))
    {
        return NULL;
    }

    if(bSuccessor(hDraggingItem, hDroppingItem) && (eDropping == DROPPING_ABOVE))
    {
        return NULL;
    }

    if(hDraggingItem == hDroppingItem)
    {
        return NULL;
    }

    if(!hDroppingItem && eDropping == DROPPING_BELOW)
        for(hDroppingItem = omTreeCtrl.GetRootItem(); omTreeCtrl.GetNextSiblingItem(hDroppingItem) != 0; hDroppingItem = omTreeCtrl.GetNextSiblingItem(hDroppingItem));

    // Setup insertion parameters
    HTREEITEM hInsertAfter = 0;
    HTREEITEM hParent = 0;

    switch(eDropping)
    {
        case DROPPING_BELOW:
            hInsertAfter = hDroppingItem;
            hParent = omTreeCtrl.GetParentItem(hDroppingItem);
            break;

        case DROPPING_ABOVE:
            hInsertAfter = omTreeCtrl.GetPrevSiblingItem(hDroppingItem);

            if(!hInsertAfter)
            {
                hInsertAfter = TVI_FIRST;
            }

            hParent = omTreeCtrl.GetParentItem(hDroppingItem);
            break;

        default:
            ASSERT(false);
            break;
    }

    m_hDraggingItemgedItem = CopyTotalBranch(hDraggingItem, hParent, hInsertAfter);
    omTreeCtrl.DeleteItem(hDraggingItem);
    return hInsertAfter;
}
HTREEITEM CTreeViewEx::CopyTotalBranch( HTREEITEM hBranchItem, HTREEITEM hNewParentItem, HTREEITEM hAfterItem /*= TVI_LAST*/ )
{
    HTREEITEM hChildItem;
    CTreeCtrl& omTreeCtrl = GetTreeCtrl();
    HTREEITEM hNewItem = CopyTreeItem( hBranchItem, hNewParentItem, hAfterItem );
    hChildItem = omTreeCtrl.GetChildItem(hBranchItem);

    while( hChildItem != NULL)
    {
        // recursively transfer all the items
        CopyTotalBranch(hChildItem, hNewItem, TVI_LAST);
        hChildItem = omTreeCtrl.GetNextSiblingItem( hChildItem );
    }

    return hNewItem;
}

HTREEITEM CTreeViewEx::CopyTreeItem( HTREEITEM hItem, HTREEITEM hNewParentItem,HTREEITEM hAfterItem )
{
    TV_INSERTSTRUCT tvstruct;
    HTREEITEM   hNewItem;
    CString sText;
    CTreeCtrl& omTreeCtrl = GetTreeCtrl();
    // get information of the source item
    tvstruct.item.hItem = hItem;
    tvstruct.item.mask = TVIF_CHILDREN | TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE ;
    omTreeCtrl.GetItem(&tvstruct.item);
    sText = omTreeCtrl.GetItemText( hItem );
    tvstruct.item.cchTextMax = sText.GetLength();
    tvstruct.item.pszText = sText.LockBuffer();
    // Insert the item at proper location
    tvstruct.hParent = hNewParentItem;
    tvstruct.hInsertAfter = hAfterItem;
    tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
    hNewItem = omTreeCtrl.InsertItem(&tvstruct);
    sText.ReleaseBuffer();
    // Now copy item data and item state.
    omTreeCtrl.SetItemData( hNewItem, omTreeCtrl.GetItemData( hItem ));
    omTreeCtrl.SetItemState( hNewItem, omTreeCtrl.GetItemState( hItem, TVIS_STATEIMAGEMASK ),
                             TVIS_STATEIMAGEMASK );
    return hNewItem;
}


void CTreeViewEx::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bDragging)
    {
        DragMove(point);
        m_pomDragImageList->DragShowNolock(TRUE);
    }

    CTreeView::OnMouseMove(nFlags, point);
}
void CTreeViewEx::DragMove(CPoint /*point*/)
{
    eTYPE_DROPPING eDropping;
    HTREEITEM hDroppingItem = GetDropItem(eDropping);
    GetTreeCtrl().SetInsertMark(0, 0);

    if(eDropping == DROPPING_ABOVE)
    {
        GetTreeCtrl().SetInsertMark(hDroppingItem, FALSE);
    }

    if(eDropping == DROPPING_BELOW)
    {
        GetTreeCtrl().SetInsertMark(hDroppingItem);
    }

    SetDraggingCursor(eDropping);
}
void CTreeViewEx::SetDraggingCursor(eTYPE_DROPPING eDropping)
{
    if(eDropping == NO_DROPPING)
    {
        SetCursor(m_hCurNoDrop);
    }
    else
    {
        SetCursor(m_hOrigCursor);
    }
}
HTREEITEM CTreeViewEx::GetDropItem(eTYPE_DROPPING& eDroppingPos)
{
    CTreeCtrl& omTreeCtrl = GetTreeCtrl();
    CPoint omCurrentPoint;
    GetCursorPos(&omCurrentPoint);
    ScreenToClient(&omCurrentPoint);
    UINT flags;
    HTREEITEM hDroppingItem = omTreeCtrl.HitTest(omCurrentPoint, &flags);
    eDroppingPos = GetDroppingPosition(flags);

    if(IsItemCanDropOn(m_hDraggingItemgedItem, hDroppingItem) == FALSE)
    {
        eDroppingPos = NO_DROPPING;
    }

    if(hDroppingItem)
    {
        m_pomDragImageList->DragLeave(&omTreeCtrl);
        omTreeCtrl.SelectDropTarget(hDroppingItem);
        // Make sure the surrounding items are visible, too
        // This will scroll the tree if necessary.
        HTREEITEM hPrev = omTreeCtrl.GetPrevVisibleItem(hDroppingItem);

        if(hPrev)
        {
            omTreeCtrl.EnsureVisible(hPrev);
        }

        HTREEITEM hNext = omTreeCtrl.GetNextVisibleItem(hDroppingItem);

        if(hNext)
        {
            omTreeCtrl.EnsureVisible(hNext);
        }

        CPoint omCurPoint;
        GetCursorPos(&omCurPoint);
        ScreenToClient(&omCurPoint);
        m_pomDragImageList->DragEnter(&omTreeCtrl, omCurPoint);
    }
    else if(eDroppingPos != NO_DROPPING && !IsItemCanDropOn(m_hDraggingItemgedItem, hDroppingItem))
    {
        eDroppingPos = NO_DROPPING;
    }

    return hDroppingItem;
}
BOOL CTreeViewEx::bAnscestor(HTREEITEM hItem, HTREEITEM hCheck)
{
    CTreeCtrl& omTreeCtrl = GetTreeCtrl();

    for(HTREEITEM hParent = hCheck; hParent != 0; hParent = omTreeCtrl.GetParentItem(hParent))
    {
        if(hParent == hItem)
        {
            return TRUE;
        }
    }

    return FALSE;
}
BOOL CTreeViewEx::bSuccessor(HTREEITEM hItem, HTREEITEM hCheck)
{
    CTreeCtrl& omTreeCtrl = GetTreeCtrl();
    return (hCheck == omTreeCtrl.GetNextItem(hItem, TVGN_NEXT));
}
CTreeViewEx::eTYPE_DROPPING CTreeViewEx::GetDroppingPosition(UINT flags)
{
    CTreeCtrl& omTreeCtrl = GetTreeCtrl();
    eTYPE_DROPPING eDroppingPos = NO_DROPPING;

    if((flags & TVHT_ONITEMRIGHT))
    {
        eDroppingPos = NO_DROPPING;
    }
    else if(flags & TVHT_ONITEM)
    {
        // check whether we should drop below or above
        // the item
        CRect omItemRect;

        if(omTreeCtrl.GetItemRect(omTreeCtrl.GetDropHilightItem(), omItemRect, false))
        {
            CPoint omCurrentPoint;
            ::GetCursorPos(&omCurrentPoint);
            ScreenToClient(&omCurrentPoint);

            if((omCurrentPoint.y - omItemRect.top) > (omItemRect.bottom - omCurrentPoint.y))
            {
                eDroppingPos = DROPPING_BELOW;
            }
            else
            {
                eDroppingPos = DROPPING_ABOVE;
            }
        }
        else
        {
            eDroppingPos = DROPPING_ABOVE;
        }
    }
    else if((flags & TVHT_NOWHERE))
    {
        // below the last item
        eDroppingPos = DROPPING_BELOW;
    }

    return eDroppingPos;
}

//If Image List Is NULL the Image List will be removed.
INT CTreeViewEx::SetImageList(CImageList* pomImageListNormal, CImageList* pomImageListState)
{
    CImageList* pomOldImageList;
    pomOldImageList = GetTreeCtrl().SetImageList(pomImageListNormal, TVSIL_NORMAL);

    if( pomOldImageList != NULL )
    {
        delete pomOldImageList;
    }

    if(pomImageListNormal != NULL)
    {
        pomOldImageList = GetTreeCtrl().SetImageList(pomImageListState, TVSIL_STATE);
    }

    if( pomOldImageList != NULL )
    {
        delete pomOldImageList;
    }

    return 0;
}

BOOL CTreeViewEx::PreTranslateMessage(MSG* pMsg)
{
    // TODO: Add your specialized code here and/or call the base class
    BOOL bDrag = (pMsg->message == WM_PAINT) && m_bDragging;

    if(bDrag)
    {
        m_pomDragImageList->DragShowNolock(FALSE);
        return 0;
    }

    if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE && m_bDragging)
    {
        m_bDragging = FALSE;
        CImageList::DragLeave(NULL);
        CImageList::EndDrag();
        ReleaseCapture();
        GetTreeCtrl().SelectDropTarget(NULL);
        delete m_pomDragImageList;
        return TRUE;        // DO NOT process further
    }

    if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F2 )
    {
        CTreeCtrl& omTreeCtrl = GetTreeCtrl();
        HTREEITEM hSelectedItem = omTreeCtrl.GetSelectedItem();
        PostMessage ( TVM_EDITLABEL, 0, (LPARAM)hSelectedItem );    //if hSelectedItem is also no problem
        return TRUE;
    }

    if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_DELETE )
    {
        if(m_bEditing == FALSE)
        {
            CTreeCtrl& omTreeCtrl = GetTreeCtrl();
            HTREEITEM hSelectedItem = omTreeCtrl.GetSelectedItem();
            vOnDeleteItem();
            return TRUE;
        }
    }

    return CTreeView::PreTranslateMessage(pMsg);
}



void CTreeViewEx::OnNMRclick(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    CPoint point;
    GetCursorPos(&point);
    CPoint omCurrentPoint(point);
    ScreenToClient(&omCurrentPoint);
    UINT flags;
    HTREEITEM hItem = GetTreeCtrl().HitTest(omCurrentPoint, &flags);
    CTreeCtrl& omTempTreeCtrl = GetTreeCtrl();

    if(hItem && (flags & TVHT_ONITEM) && !(flags & TVHT_ONITEMRIGHT))
    {
        omTempTreeCtrl.SelectItem(hItem);
        CMenu omContextMenu;
        VERIFY(omContextMenu.CreatePopupMenu());

        if(omTempTreeCtrl.GetItemData(hItem) == def_ID_TESTSUITE)
        {
            VERIFY(omContextMenu.AppendMenu(MF_STRING, IDM_TESTSUITE_ADD, _T("Add...")));

            if(GetExecutorWindow()->bGetBusStatus() == TRUE)
            {
                VERIFY(omContextMenu.AppendMenu(MF_STRING, IDM_TESTSUITE_EXECUTE, _T("Execute")));
            }
            else
            {
                VERIFY(omContextMenu.AppendMenu(MF_STRING|MF_GRAYED, IDM_TESTSUITE_EXECUTE, _T("Execute")));
            }
        }
        else
        {
            HTREEITEM hParentItem = omTempTreeCtrl.GetParentItem(hItem);

            if(omTempTreeCtrl.GetItemData(hParentItem) == def_ID_TESTSUITE)
            {
                VERIFY(omContextMenu.AppendMenu(MF_STRING, IDM_TESTSETUP_DELETE, _T("Delete")));
                VERIFY(omContextMenu.AppendMenu(MF_STRING, IDM_TESTSETUP_MODIFY, _T("Reload File")));
            }
        }

        omContextMenu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
        *pResult = 0;
    }
    else        //On Tree View Create Collapse All and Expand All
    {
        CMenu omContextMenu;
        VERIFY(omContextMenu.CreatePopupMenu());
        VERIFY(omContextMenu.AppendMenu(MF_STRING, IDM_COLLAPSE_ALL, _T("Collapse All")));
        VERIFY(omContextMenu.AppendMenu(MF_STRING, IDM_EXPAND_ALL, _T("Expand All")));
        omContextMenu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
        *pResult = 0;
    }
}
void CTreeViewEx::vCollapseTreeBranch( HTREEITEM hTreeItem)
{
    CTreeCtrl& omTreeCtrl = GetTreeCtrl();

    if( omTreeCtrl.ItemHasChildren( hTreeItem ) )
    {
        omTreeCtrl.Expand( hTreeItem, TVE_COLLAPSE );
        hTreeItem = omTreeCtrl.GetChildItem( hTreeItem );

        while(hTreeItem != NULL)
        {
            vCollapseTreeBranch( hTreeItem );
            hTreeItem = omTreeCtrl.GetNextSiblingItem( hTreeItem );
        }
    }
}
void CTreeViewEx::vCollapseFull()
{
    CTreeCtrl& omTreeCtrl = GetTreeCtrl();
    HTREEITEM hTreeItem = omTreeCtrl.GetRootItem();

    while(hTreeItem != NULL)
    {
        vCollapseTreeBranch( hTreeItem );
        hTreeItem = omTreeCtrl.GetNextSiblingItem( hTreeItem );
    }
}

void CTreeViewEx::vExpandTreeBranch( HTREEITEM hTreeItem)
{
    CTreeCtrl& omTreeCtrl = GetTreeCtrl();

    if( omTreeCtrl.ItemHasChildren( hTreeItem ) )
    {
        omTreeCtrl.Expand( hTreeItem, TVE_EXPAND );
        hTreeItem = omTreeCtrl.GetChildItem( hTreeItem );

        while(hTreeItem != NULL)
        {
            vExpandTreeBranch( hTreeItem );
            hTreeItem = omTreeCtrl.GetNextSiblingItem( hTreeItem );
        }
    }
}
void CTreeViewEx::vExpandFull()
{
    CTreeCtrl& omTreeCtrl = GetTreeCtrl();
    HTREEITEM hTreeItem = omTreeCtrl.GetRootItem();

    while(hTreeItem != NULL)
    {
        vExpandTreeBranch( hTreeItem );
        hTreeItem = omTreeCtrl.GetNextSiblingItem( hTreeItem );
    }
}
void CTreeViewEx::DeleteItem( HTREEITEM hItem)
{
    GetTreeCtrl().DeleteItem(hItem);
}
void CTreeViewEx::vDeleteChildItems(HTREEITEM hItem)
{
    CTreeCtrl& omTempTreeCtrl = GetTreeCtrl();

    if (omTempTreeCtrl.ItemHasChildren(hItem))
    {
        HTREEITEM hNextItem;
        HTREEITEM hChildItemItem = omTempTreeCtrl.GetChildItem(hItem);

        while (hChildItemItem != NULL)
        {
            hNextItem = omTempTreeCtrl.GetNextItem(hChildItemItem, TVGN_NEXT);
            omTempTreeCtrl.DeleteItem(hChildItemItem);
            hChildItemItem = hNextItem;
        }
    }
}


void CTreeViewEx::ShowCheckBoxes(BOOL bShow)
{
    DWORD lStyle = GetWindowLong(GetSafeHwnd(), GWL_STYLE);

    if(bShow == TRUE)
    {
        lStyle = lStyle | TVS_CHECKBOXES;
    }
    else
    {
        lStyle = lStyle | ~TVS_CHECKBOXES;
    }

    SetWindowLong(GetSafeHwnd(), GWL_STYLE, lStyle);
}

void CTreeViewEx::OnTvnSelchanged(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    *pResult = 0;
}

BOOL CTreeViewEx::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam)
{
    dwStyle = dwStyle | WS_EX_CLIENTEDGE|WS_EX_STATICEDGE;
    return CTreeView::CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, lpParam);
}


HBRUSH CTreeViewEx::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/)
{
    CBrush brBackColor(def_COLOR_TREE_BKG);
    return brBackColor;
}


void CTreeViewEx::OnTvnSelchanging(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
    *pResult = FALSE;
}

void CTreeViewEx::OnTvnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
    m_bEditing = TRUE;

    if(GetTreeCtrl().GetItemData(pTVDispInfo->item.hItem) == def_ID_TESTSUITE)
    {
        *pResult = 0;
    }
    else
    {
        *pResult = 1;
    }
}
void CTreeViewEx::OnTvnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
    LPTSTR pszText = pTVDispInfo->item.pszText;
    *pResult = ( pszText && (*pszText != _T('\0')));
    CString omstrItemNewName(pszText);

    if(GetTreeCtrl().GetItemData(pTVDispInfo->item.hItem) == def_ID_TESTSUITE)  //Just Confirm
    {
        GetExecutorWindow()->m_ouTSExecutor.SetTestsuiteName(omstrItemNewName);
    }

    m_bEditing = FALSE;
}

/**
 * \brief Execute current test suite
 * \req RS_27_30 - Execute current test suite
 *
 * Execute current test suite
 */
void CTreeViewEx::onUpdateExecute(CCmdUI* pCmdUI)
{
    if(GetExecutorWindow() != NULL)
    {
        pCmdUI->Enable(GetExecutorWindow()->bGetBusStatus());
    }
}
void CTreeViewEx::vOnDeleteItem()
{
    CTreeCtrl& omTempTreeCtrl = GetTreeCtrl();
    HTREEITEM  hDeleteItem = omTempTreeCtrl.GetSelectedItem();
    HTREEITEM hParentItem = omTempTreeCtrl.GetParentItem(hDeleteItem);
}


void CTreeViewEx::OnCommandRange(UINT unCommandID)
{
    CTSExecutorChildFrame* ouExecutorWnd = GetExecutorWindow();

    if(ouExecutorWnd != NULL)
    {
        CTreeCtrl& omTempTreeCtrl = GetTreeCtrl();
        HTREEITEM hSelectedItem = omTempTreeCtrl.GetSelectedItem();
        DWORD_PTR dwID = omTempTreeCtrl.GetItemData(hSelectedItem);
        HRESULT hResult = (HRESULT)ouExecutorWnd->SendMessage(unCommandID, dwID, 0);

        if(unCommandID == IDM_TESTSETUP_DELETE && hResult == S_OK)
        {
            omTempTreeCtrl.DeleteItem(hSelectedItem);
        }
    }
}
CTSExecutorChildFrame* CTreeViewEx::GetExecutorWindow()
{
    CWnd* wndParent;

    if((wndParent = GetParent())!= NULL)
    {
        wndParent = wndParent->GetParent();

        if(wndParent != NULL)
        {
            return ((CTSExecutorChildFrame*)wndParent);
        }
    }

    return NULL;
}