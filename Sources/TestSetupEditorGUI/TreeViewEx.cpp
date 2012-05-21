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
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

#include "TSEditorGUI_stdafx.h"
#include "TreeViewEx.h"
#include "TSEditorGUI_resource.h"
#include "TSEditorGUI_ChildFrame.h"
#include "TSEditorGUI_Definitions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CTreeViewEx, CTreeView)

/******************************************************************************
Function Name  :  CTreeViewEx
Input(s)       :  -
Output         :  -
Functionality  :  Constructor of CTreeViewEx 
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
CTreeViewEx::CTreeViewEx(void)
{
    m_bDragging = FALSE;
    m_bEditing = FALSE;
    m_hCurMoveDown = AfxGetApp()->LoadCursor(MAKEINTRESOURCE(IDC_CURMDOWN));
    m_hCurMoveUp = AfxGetApp()->LoadCursor(MAKEINTRESOURCE(IDC_CURMUP));
    m_hCurNoDrop = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_NO));
    m_hOrigCursor = ::GetCursor();
    m_omBkColor = def_COLOR_TREE_BKG;    
    m_omTextColor = def_COLOR_TREE_TEXT;
}

/******************************************************************************
Function Name  :  ~CTreeViewEx
Input(s)       :  -
Output         :  -
Functionality  :  Destructor of CTreeViewEx 
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
CTreeViewEx::~CTreeViewEx()
{
}

BEGIN_MESSAGE_MAP(CTreeViewEx, CTreeView)
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    
    ON_WM_CTLCOLOR_REFLECT()
    
    ON_COMMAND(IDM_DELETE, vOnDeleteItem)
    ON_COMMAND(IDM_EXPAND_ALL, vExpandFull)
    ON_COMMAND(IDM_SETTINGS_CLR, OnBKGColor)
    ON_COMMAND(IDM_COLLAPSE_ALL, vCollapseFull)
    ON_COMMAND_RANGE(IDM_ADD_TESTCASE, IDM_ADD_REPLAY, OnAddEntityItem)
    
    ON_NOTIFY_REFLECT(NM_RCLICK, OnNMRclick)
    ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnTvnBegindrag)
    ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnTvnSelchanged)
    ON_NOTIFY_REFLECT(TVN_SELCHANGING, OnTvnSelchanging)
    ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnTvnEndlabeledit)
    ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnTvnBeginlabeledit)
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



/******************************************************************************
Function Name  :  OnInitialUpdate
Input(s)       :  -
Output         :  void
Functionality  :  This function does the initial updation 
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
void CTreeViewEx::OnInitialUpdate(void)
{
    CTreeView::OnInitialUpdate();
    GetTreeCtrl().SetBkColor(m_omBkColor);
    GetTreeCtrl().SetTextColor(m_omTextColor);
}

/******************************************************************************
Function Name  :  InsertTreeItem
Input(s)       :  HTREEITEM hParent - parent treeitem handle
                  CString omstrItemName - name of tree item
                  HTREEITEM hTreeInsAfter - handle of the next tree item
                  int nSelectedImage - image of the tree node
                  int nNonSelectedImage - image of the tree node when it is not selected
                  long lParam - 32 bit specific value of tree node
Output         :  HTREEITEM - handle of the newly inserted item
Functionality  :  Insrets a new tree item in the tree view 
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
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


/******************************************************************************
Function Name  :  Create
Input(s)       :  LPCTSTR lpszClassName - Class Name
                  LPCTSTR lpszWindowName - Window Name
                  DWORD dwStyle - window styles
                  RECT& rect - window size
                  CWnd* pParentWnd - parent window
                  UINT nID - ID of the window
                  CCreateContext* pContext - indow context
Output         :  BOOL
Functionality  :  Create the treeview control with the required styles. 
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
BOOL CTreeViewEx::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
    dwStyle = dwStyle| WS_BORDER | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_EDITLABELS| TVS_SHOWSELALWAYS|TVS_FULLROWSELECT;
    return CTreeView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

/******************************************************************************
Function Name  :  OnLButtonDown
Input(s)       :  UINT unFlag - Flags
                  CPoint point - Mouse Pointer point
Output         :  void
Functionality  :  ON_WM_LBUTTON Handler used to toggle the check point 
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
void CTreeViewEx::OnLButtonDown(UINT unFlag, CPoint point)
{
    UINT unHitTestFlags = 0;
    HTREEITEM hItem = GetTreeCtrl().HitTest(point, &unHitTestFlags);

    if((hItem != NULL) && (TVHT_ONITEMSTATEICON & unHitTestFlags))
    {
        BOOL bCheck = bIsItemChecked(hItem);
        vSetCheck(hItem, !bCheck);
        vSetCheckChildren(hItem, !bCheck);
        vSetCheckParent(hItem);
        return;
    }
    CTreeView::OnLButtonDown(unFlag, point);
}

/******************************************************************************
Function Name  :  bIsItemChecked
Input(s)       :  HTREEITEM hItem - hanle of tree item
Output         :  BOOL - True if item is checked else false
Functionality  :  Returns weather the item is checked or not 
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
BOOL CTreeViewEx::bIsItemChecked(HTREEITEM hItem)
{
    if(hItem == NULL)
    {
        return FALSE;
    }
    CTreeCtrl &omTreeCtrl = GetTreeCtrl();
	return omTreeCtrl.GetItemState( hItem, TVIS_STATEIMAGEMASK )>>12 == 2;
}

/******************************************************************************
Function Name  :  vSetCheck
Input(s)       :  HTREEITEM hItem - hanle of tree item whose state has to change
                  BOOL bCheck - state of the item
Output         :  void
Functionality  :  Item state will be changed according to the bCheck Value 
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
void CTreeViewEx::vSetCheck(HTREEITEM hItem, BOOL bCheck)
{
    if (hItem != NULL)
	{
        int nState = (bCheck == TRUE) ? 2 : 1;
        GetTreeCtrl().SetItemState( hItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
	}
	return;
}

/******************************************************************************
Function Name  :  vSetCheckChildren
Input(s)       :  HTREEITEM hItem - hanle of tree item whose Childrens state has to change
                  BOOL bCheck - state of the Children item
Output         :  void
Functionality  :  Item's Children state will be changed according to the bCheck Value  
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
void CTreeViewEx::vSetCheckChildren(HTREEITEM hItem, BOOL bCheck)
{
    if(hItem == NULL)
    {
        return;
    }
    CTreeCtrl &omTreeCtrl = GetTreeCtrl();
	
    //W4 Removal
    //HTREEITEM hNext = omTreeCtrl.GetChildItem(hItem);
    
    vSetCheck(hItem, bCheck);
	
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
            vSetCheckChildren(htiChild, bCheck);
        }
        HTREEITEM htiSibling = omTreeCtrl.GetNextSiblingItem (htiChild);
        while (htiSibling )
        {
            vSetCheckChildren(htiSibling, bCheck);
            htiSibling = omTreeCtrl.GetNextSiblingItem(htiSibling);
        }
    }

	return;
}
/******************************************************************************
Function Name  :  vSetCheckParent
Input(s)       :  
Output         :  void
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
void CTreeViewEx::vSetCheckParent(HTREEITEM hItem)
{
    if(hItem == NULL)
    {
        return;
    }
	CTreeCtrl &omTreeCtrl = GetTreeCtrl();
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

/******************************************************************************
Function Name  :  OnTvnBegindrag
Input(s)       :  
Output         :  void
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
void CTreeViewEx::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
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


/******************************************************************************
Function Name  :  bItemCanDragged
Input(s)       :  
Output         :  BOOL
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
BOOL CTreeViewEx::bItemCanDragged(HTREEITEM /*hItem*/)
{
	return TRUE;
}

/******************************************************************************
Function Name  :  IsItemCanDropOn
Input(s)       :  
Output         :  BOOL
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
BOOL CTreeViewEx::IsItemCanDropOn(HTREEITEM hSource, HTREEITEM hTarget)
{
    CTreeCtrl &omTreeCtrl = GetTreeCtrl();
   
    if(hSource != NULL || hTarget != NULL)
    {
        if(omTreeCtrl.GetParentItem(hSource) == omTreeCtrl.GetParentItem(hTarget))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
}


/******************************************************************************
Function Name  :  OnLButtonUp
Input(s)       :  
Output         :  void
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
void CTreeViewEx::OnLButtonUp(UINT unFlag, CPoint point)
{
    if(m_bDragging == TRUE)
    {
        CTreeCtrl &omTreeCtrl = GetTreeCtrl();
        m_bDragging = FALSE;
        DragEnd();
        omTreeCtrl.SelectDropTarget(0);
        SetCursor(m_hOrigCursor);
        GetTreeCtrl().SetInsertMark(0, 0);
        ::ReleaseCapture();
    }
	CTreeView::OnLButtonUp(unFlag, point);
 }

/******************************************************************************
Function Name  :  DragEnd
Input(s)       :  
Output         :  void
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
void CTreeViewEx::DragEnd(VOID) 
{
 	eTYPE_DROPPING eDropping;
	HTREEITEM hDroppingItem = GetDropItem(eDropping);
    
    if (hDroppingItem == NULL)
    {
        return;
    }
    CTreeCtrl& omTempTreeCtrl = GetTreeCtrl();
    //UINT unFirstIndex = unGetIndex(m_hDraggingItemgedItem);
    HTREEITEM hInsertAfter = DragMoveItem(m_hDraggingItemgedItem, hDroppingItem, eDropping);
    if(hInsertAfter == NULL)
    {
        return;
    }
    //W4 Removal
    DWORD dwRepositionItemID = (DWORD)omTempTreeCtrl.GetItemData(m_hDraggingItemgedItem);
    DWORD dwInsertAfterItemID;
    if(hInsertAfter == TVI_FIRST)
    {
        //W4 Removal
        dwInsertAfterItemID = (DWORD)-1;
    }
    else
    {
        //W4 Removal
        dwInsertAfterItemID = (DWORD)omTempTreeCtrl.GetItemData(hInsertAfter);
    }
    HTREEITEM hParent = omTempTreeCtrl.GetParentItem(m_hDraggingItemgedItem);
    //W4 Removal
    UINT unNewID = GetEditorWindow()->unRepisitonEntry((DWORD)dwRepositionItemID, (DWORD)dwInsertAfterItemID, (DWORD)omTempTreeCtrl.GetItemData(hParent));
    omTempTreeCtrl.SetItemData(m_hDraggingItemgedItem, unNewID);
    GetEditorWindow()->vSetFileSavedFlag(FALSE);
}

/******************************************************************************
Function Name  :  unGetIndex
Input(s)       :  
Output         :  UINT
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
int CTreeViewEx::unGetIndex(HTREEITEM hTreeItem)
{
    CTreeCtrl &omTempTreeCtrl = GetTreeCtrl();
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
    return -1;
}

/******************************************************************************
Function Name  :  DragMoveItem
Input(s)       :  
Output         :  HTREEITEM
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
HTREEITEM CTreeViewEx::DragMoveItem(HTREEITEM hDraggingItem, HTREEITEM hDroppingItem, eTYPE_DROPPING eDropping) 
{
    CTreeCtrl &omTreeCtrl = GetTreeCtrl();
    
    if(eDropping == NO_DROPPING)
		return NULL;
	if(bAnscestor(hDraggingItem, hDroppingItem) || !IsItemCanDropOn(hDraggingItem, hDroppingItem))
		return NULL;
    if(bSuccessor(hDraggingItem, hDroppingItem) && (eDropping == DROPPING_ABOVE))
        return NULL;
    if(hDraggingItem == hDroppingItem)
        return NULL;

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
				hInsertAfter = TVI_FIRST;
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

/******************************************************************************
Function Name  :  CopyTotalBranch
Input(s)       :  
Output         :  HTREEITEM
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
HTREEITEM CTreeViewEx::CopyTotalBranch( HTREEITEM hBranchItem, HTREEITEM hNewParentItem, HTREEITEM hAfterItem /*= TVI_LAST*/ )
{
        HTREEITEM hChildItem;
        CTreeCtrl& omTreeCtrl = GetTreeCtrl();
        HTREEITEM hNewItem = CopyTreeItem( hBranchItem, hNewParentItem, hAfterItem );
        hChildItem = omTreeCtrl.GetChildItem(hBranchItem);
        while( hChildItem != NULL)
        {
            CopyTotalBranch(hChildItem, hNewItem, TVI_LAST);  
            hChildItem = omTreeCtrl.GetNextSiblingItem( hChildItem );
        }
        return hNewItem;
}

/******************************************************************************
Function Name  :  CopyTreeItem
Input(s)       :  
Output         :  HTREEITEM
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
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


/******************************************************************************
Function Name  :  OnMouseMove
Input(s)       :  
Output         :  void
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
void CTreeViewEx::OnMouseMove(UINT unFlag, CPoint point)
{
    if (m_bDragging)
    {
        DragMove(point);
    }
    CTreeView::OnMouseMove(unFlag, point);
}

/******************************************************************************
Function Name  :  DragMove
Input(s)       :  
Output         :  void
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
void CTreeViewEx::DragMove(CPoint /*point*/)        //point is not required currently
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

/******************************************************************************
Function Name  :  SetDraggingCursor
Input(s)       :  
Output         :  void
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
void CTreeViewEx::SetDraggingCursor(eTYPE_DROPPING eDropping) 
{
    if(eDropping == NO_DROPPING)
        SetCursor(m_hCurNoDrop);
    else
        SetCursor(m_hOrigCursor);
}

/******************************************************************************
Function Name  :  GetDropItem
Input(s)       :  
Output         :  HTREEITEM
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
HTREEITEM CTreeViewEx::GetDropItem(eTYPE_DROPPING & eDroppingPos) 
{
	CTreeCtrl &omTreeCtrl = GetTreeCtrl();
	CPoint omCurrentPoint;
	GetCursorPos(&omCurrentPoint);
	ScreenToClient(&omCurrentPoint);

	UINT unFlag;
	HTREEITEM hDroppingItem = omTreeCtrl.HitTest(omCurrentPoint, &unFlag);

	eDroppingPos = GetDroppingPosition(unFlag);
    
    if(IsItemCanDropOn(m_hDraggingItemgedItem, hDroppingItem) == FALSE)
    {
         eDroppingPos = NO_DROPPING;
    }
	if(hDroppingItem) 
    {
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
	} 
    else if(eDroppingPos != NO_DROPPING && !IsItemCanDropOn(m_hDraggingItemgedItem, hDroppingItem))
    {
	    eDroppingPos = NO_DROPPING;
    }

	return hDroppingItem;
}

/******************************************************************************
Function Name  :  bAnscestor
Input(s)       :  
Output         :  BOOL
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
BOOL CTreeViewEx::bAnscestor(HTREEITEM hItem, HTREEITEM hCheck)
{
    CTreeCtrl &omTreeCtrl = GetTreeCtrl();
	for(HTREEITEM hParent = hCheck; hParent != 0; hParent = omTreeCtrl.GetParentItem(hParent))
    {		
        if(hParent == hItem)
			return TRUE;
    }
	return FALSE;
}

/******************************************************************************
Function Name  :  bSuccessor
Input(s)       :  
Output         :  BOOL
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
BOOL CTreeViewEx::bSuccessor(HTREEITEM hItem, HTREEITEM hCheck)
{
    CTreeCtrl &omTreeCtrl = GetTreeCtrl();
    return (hCheck == omTreeCtrl.GetNextItem(hItem, TVGN_NEXT));
}

/******************************************************************************
Function Name  :  GetDroppingPosition
Input(s)       :  
Output         :  eTYPE_DROPPING
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
CTreeViewEx::eTYPE_DROPPING CTreeViewEx::GetDroppingPosition(UINT flags) 
{
    
    CTreeCtrl &omTreeCtrl = GetTreeCtrl();
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
                eDroppingPos = DROPPING_BELOW;
            else
                eDroppingPos = DROPPING_ABOVE;
		} 
        else
			eDroppingPos = DROPPING_ABOVE;

	} 
    else if((flags & TVHT_NOWHERE))
    {
		// below the last item
		eDroppingPos = DROPPING_BELOW;
    }
    return eDroppingPos;
}

//If Image List Is NULL the Image List will be removed.
/******************************************************************************
Function Name  :  SetImageList
Input(s)       :  
Output         :  INT
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
INT CTreeViewEx::SetImageList(CImageList *pomImageListNormal, CImageList *pomImageListState)
{
    CImageList *pomOldImageList;
    
    pomOldImageList = GetTreeCtrl().SetImageList(pomImageListNormal, TVSIL_NORMAL);
    if( pomOldImageList != NULL )
        delete pomOldImageList;
    
    if(pomImageListNormal != NULL)
    pomOldImageList = GetTreeCtrl().SetImageList(pomImageListState, TVSIL_STATE);
        if( pomOldImageList != NULL )
            delete pomOldImageList;
    return 0;
}

/******************************************************************************
Function Name  :  PreTranslateMessage
Input(s)       :  
Output         :  BOOL
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
BOOL CTreeViewEx::PreTranslateMessage(MSG* pMsg)
{
// TODO: Add your specialized code here and/or call the base class
    BOOL bDrag = (pMsg->message == WM_PAINT) && m_bDragging;
    if(bDrag)
    {
        //m_pomDragImageList->DragShowNolock(FALSE);
        return 0;
    }
    if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE && m_bDragging)
	{
		m_bDragging = FALSE;
		CImageList::DragLeave(NULL);
		CImageList::EndDrag();
		ReleaseCapture();
        GetTreeCtrl().SetInsertMark(0, 0);
        GetTreeCtrl().SelectDropTarget(NULL);
		return TRUE;		// DO NOT process further
	}
    if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F2 )
    {
 	    CTreeCtrl &omTreeCtrl = GetTreeCtrl();
        HTREEITEM hSelectedItem = omTreeCtrl.GetSelectedItem();
        PostMessage ( TVM_EDITLABEL, 0, (LPARAM)hSelectedItem );    //if hSelectedItem is also no problem
        return TRUE;
    }
    if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_DELETE )
    {
        if(m_bEditing == FALSE)
        {
            //CTreeCtrl &omTreeCtrl = GetTreeCtrl();
            //HTREEITEM hSelectedItem = omTreeCtrl.GetSelectedItem();
            vOnDeleteItem();
            return TRUE;
        }
        
    }
    
    return CTreeView::PreTranslateMessage(pMsg);
}



/******************************************************************************
Function Name  :  OnNMRclick
Input(s)       :  
Output         :  void
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
void CTreeViewEx::OnNMRclick(NMHDR* /*pNMHDR*/, LRESULT *pResult)
{
    // TODO: Add your control notification handler code here
    CPoint point;
	GetCursorPos(&point);
	CPoint omCurrentPoint(point);
	ScreenToClient(&omCurrentPoint);
	UINT flags;
	HTREEITEM hItem = GetTreeCtrl().HitTest(omCurrentPoint, &flags);
    CTreeCtrl &omTempTreeCtrl = GetTreeCtrl();
    if(hItem && (flags & TVHT_ONITEM) && !(flags & TVHT_ONITEMRIGHT))
    {
        omTempTreeCtrl.SelectItem(hItem);   
        eTYPE_ENTITY eEnityType = GetEditorWindow()->eGetCurrentEntityType();
        CMenu omContextMenu;

        VERIFY(omContextMenu.CreatePopupMenu());
        if(eEnityType == TEST_SETUP)
        {
            VERIFY(omContextMenu.AppendMenu(MF_STRING, IDM_ADD_TESTCASE, _T("New Test Case")));
        }
        else if(eEnityType == TEST_CASE)
        {
            CMenu omSubMenu;
            omSubMenu.CreatePopupMenu();
            omSubMenu.AppendMenu(MF_STRING, IDM_ADD_SEND, _T("Send"));
            omSubMenu.AppendMenu(MF_STRING, IDM_ADD_VERIFY, _T("Verify"));
            omSubMenu.AppendMenu(MF_STRING, IDM_ADD_WAIT, _T("Wait"));
            omSubMenu.AppendMenu(MF_STRING, IDM_ADD_VERIFYRESPONSE, _T("verfiyResponse"));
            //omSubMenu.AppendMenu(MF_STRING, IDM_ADD_REPLAY, _T("Replay"));
            omContextMenu.AppendMenu(MF_POPUP, (UINT_PTR)omSubMenu.m_hMenu, _T("New"));
            omContextMenu.AppendMenu(MF_STRING, IDM_DELETE, _T("Delete"));
        }
        else
        {
            omContextMenu.AppendMenu(MF_STRING, IDM_DELETE, _T("Delete"));
        }
        
        if(omContextMenu.GetMenuItemCount() > 0)
        {
            omContextMenu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
        }
        *pResult = 0;
    }
    
    else        //On Tree View Create Collapse All and Expand All
    {
        CMenu omContextMenu;
	    VERIFY(omContextMenu.CreatePopupMenu());
        VERIFY(omContextMenu.AppendMenu(MF_STRING, IDM_COLLAPSE_ALL, _T("Collapse All")));
		VERIFY(omContextMenu.AppendMenu(MF_STRING, IDM_EXPAND_ALL, _T("Expand All")));
        VERIFY(omContextMenu.AppendMenu(MF_SEPARATOR));
        VERIFY(omContextMenu.AppendMenu(MF_STRING, IDM_SETTINGS_CLR, _T("Background Color")));
	    omContextMenu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
        *pResult = 0;
    }
}

/******************************************************************************
Function Name  :  vCollapseTreeBranch
Input(s)       :  
Output         :  void
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
void CTreeViewEx::vCollapseTreeBranch( HTREEITEM hTreeItem)
{
    CTreeCtrl &omTreeCtrl = GetTreeCtrl();
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

/******************************************************************************
Function Name  :  vCollapseFull
Input(s)       :  
Output         :  void
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
void CTreeViewEx::vCollapseFull()
{
    CTreeCtrl &omTreeCtrl = GetTreeCtrl();
    HTREEITEM hTreeItem = omTreeCtrl.GetRootItem();
    while(hTreeItem != NULL)
    {
        vCollapseTreeBranch( hTreeItem );
        hTreeItem = omTreeCtrl.GetNextSiblingItem( hTreeItem );
    }
}

/******************************************************************************
Function Name  :  vExpandTreeBranch
Input(s)       :  
Output         :  void
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
void CTreeViewEx::vExpandTreeBranch( HTREEITEM hTreeItem)
{
    CTreeCtrl &omTreeCtrl = GetTreeCtrl();
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

/******************************************************************************
Function Name  :  vExpandFull
Input(s)       :  
Output         :  void
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
void CTreeViewEx::vExpandFull()
{
    CTreeCtrl &omTreeCtrl = GetTreeCtrl();
    HTREEITEM hTreeItem = omTreeCtrl.GetRootItem();
    while(hTreeItem != NULL)
    {
        vExpandTreeBranch( hTreeItem );
        hTreeItem = omTreeCtrl.GetNextSiblingItem( hTreeItem );
    }
}

/******************************************************************************
Function Name  :  OnBKGColor
Input(s)       :  
Output         :  void
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
void CTreeViewEx::OnBKGColor()
{
    CColorDialog omClrDlg(m_omBkColor);
    if(omClrDlg.DoModal() == IDOK)
    {
        m_omBkColor = omClrDlg.GetColor();
        GetTreeCtrl().SetBkColor(m_omBkColor);
        (GetTreeCtrl().GetImageList(TVSIL_NORMAL))->SetBkColor(m_omBkColor);
        RedrawWindow();
    }
}

/******************************************************************************
Function Name  :  DeleteItem
Input(s)       :  
Output         :  void
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
void CTreeViewEx::DeleteItem( HTREEITEM hItem)
{
    GetTreeCtrl().DeleteItem(hItem);
}

/******************************************************************************
Function Name  :  vDeleteChildItems
Input(s)       :  
Output         :  void
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
void CTreeViewEx::vDeleteChildItems(HTREEITEM hItem)
{
    CTreeCtrl &omTempTreeCtrl = GetTreeCtrl();
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


/******************************************************************************
Function Name  :  ShowCheckBoxes
Input(s)       :  
Output         :  void
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
void CTreeViewEx::ShowCheckBoxes(BOOL bShow)
{
    //CTreeCtrl &omTreeCtrl = GetTreeCtrl();
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

/******************************************************************************
Function Name  :  OnTvnSelchanged
Input(s)       :  
Output         :  void
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
void CTreeViewEx::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    CWnd *omParentWnd= GetEditorWindow();
    if(omParentWnd != NULL)
    {
        omParentWnd->SendMessage(WM_TS_SELCHANGED, (WPARAM)pNMTreeView , 0);
    }
    *pResult = 0;
}

/******************************************************************************
Function Name  :  CreateEx
Input(s)       :  
Output         :  BOOL
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
BOOL CTreeViewEx::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam)
{
    dwStyle = dwStyle | WS_EX_CLIENTEDGE|WS_EX_STATICEDGE;
    return CTreeView::CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, lpParam);
}


/******************************************************************************
Function Name  :  CtlColor
Input(s)       :  
Output         :  HBRUSH
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
HBRUSH CTreeViewEx::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/)
{
    CBrush brBackColor(m_omBkColor);
    return brBackColor;
}


/******************************************************************************
Function Name  :  OnTvnSelchanging
Input(s)       :  
Output         :  void
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
void CTreeViewEx::OnTvnSelchanging(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    CWnd *omParentWnd= GetEditorWindow();
    if(omParentWnd != NULL)
    {
        *pResult = omParentWnd->SendMessage(WM_TS_SELCHANGING, (WPARAM)pNMTreeView , 0);
    }
}

/******************************************************************************
Function Name  :  GetEditorWindow
Input(s)       :  
Output         :  CTSEditorChildFrame*
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
CTSEditorChildFrame* CTreeViewEx::GetEditorWindow()
{
    CWnd* wndParent;
    if((wndParent = GetParent())!= NULL)
    {
        wndParent = wndParent->GetParent();
        if(wndParent != NULL)
        {
            return ((CTSEditorChildFrame*)wndParent);
        }
    }
    return NULL;
}

/******************************************************************************
Function Name  :  OnTvnBeginlabeledit
Input(s)       :  
Output         :  void
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
void CTreeViewEx::OnTvnBeginlabeledit(NMHDR* /*pNMHDR*/, LRESULT *pResult)
{
//    LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
    m_bEditing = TRUE;
    
    eTYPE_ENTITY eCurrentEntity = GetEditorWindow()->eGetCurrentEntityType();
    *pResult = 1;
    if(eCurrentEntity == TEST_CASE || eCurrentEntity == TEST_SETUP)
    {
        *pResult = 0;
    }
}

/******************************************************************************
Function Name  :  OnTvnEndlabeledit
Input(s)       :  
Output         :  void
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
void CTreeViewEx::OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
    //CTreeCtrl &omTreeCtrl = GetTreeCtrl();
    LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
    LPTSTR pszText = pTVDispInfo->item.pszText;
    *pResult = ( pszText && (*pszText != _T('\0')));
    CString omstrItemNewName(pszText);
    GetEditorWindow()->nChangeEntityTitle(NULL, omstrItemNewName);   
    m_bEditing = FALSE;
}

/**
 * \brief On Add Entity Item
 * \req RS_27_08 - Addition of a test case sub-node
 * \req RS_27_11 - Addition of a send message node
 * \req RS_27_14 - Addition of a verify message node
 *
 * On Add Entity Item
 */
void CTreeViewEx::OnAddEntityItem(UINT nId)
{
    eTYPE_ENTITY eEntityType;
    switch(nId)
    {
        case IDM_ADD_TESTCASE:
            eEntityType = TEST_CASE;
            break;

        case IDM_ADD_SEND:
            eEntityType = SEND;
            break;

        case IDM_ADD_VERIFY:
            eEntityType = VERIFY;
            break;

        case IDM_ADD_REPLAY:
            eEntityType = REPLAY;
            break;

        case IDM_ADD_WAIT:
            eEntityType = WAIT;
            break;
        case IDM_ADD_VERIFYRESPONSE:
            eEntityType = VERIFYRESPONSE;
            break;
        default:
            return;
    }
    CTreeCtrl &omTempTreeCtrl = GetTreeCtrl();
    HTREEITEM hSelectedItem = omTempTreeCtrl.GetSelectedItem();
    //W4 Removal
    GetEditorWindow()->nAddNewEntity((DWORD)omTempTreeCtrl.GetItemData(hSelectedItem), eEntityType);
}

/**
 * \brief On Delete Entity Item
 * \req RS_27_10 - Deletion of a test case sub-node
 * \req RS_27_13 - Deletion of a send message node
 * \req RS_27_16 - Deletion of a verify message node
 *
 * On Add Entity Item
 */
void CTreeViewEx::vOnDeleteItem()
{
    CTreeCtrl &omTempTreeCtrl = GetTreeCtrl();
    HTREEITEM  hDeleteItem = omTempTreeCtrl.GetSelectedItem();
    HTREEITEM hParentItem = omTempTreeCtrl.GetParentItem(hDeleteItem);
    //W4 Removal
    INT nRetVal = GetEditorWindow()->nDeleteItem((DWORD)omTempTreeCtrl.GetItemData(hDeleteItem), (DWORD)omTempTreeCtrl.GetItemData(hParentItem));
    if(nRetVal == 0)
    {
        omTempTreeCtrl.DeleteItem(hDeleteItem);
        GetEditorWindow()->vSetFileSavedFlag(FALSE);
    }
}

/******************************************************************************
Function Name  :  vSetTreeCtrlColor
Input(s)       :  
Output         :  VOID
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
VOID CTreeViewEx::vSetTreeCtrlColor(COLORREF omBkColor, COLORREF omTextColor)
{
    m_omBkColor = omBkColor;
    m_omTextColor = omTextColor;
    GetTreeCtrl().SetBkColor(omBkColor);
    GetTreeCtrl().SetTextColor(omTextColor);
    (GetTreeCtrl().GetImageList(TVSIL_NORMAL))->SetBkColor(omBkColor);
    RedrawWindow();
}

/******************************************************************************
Function Name  :  vGetTreeCtrlColor
Input(s)       :  
Output         :  VOID
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
VOID CTreeViewEx::vGetTreeCtrlColor(COLORREF& omBkColor, COLORREF& omTextColor)
{
    omBkColor = m_omBkColor;
    omTextColor = m_omTextColor;
}

/******************************************************************************
Function Name  :  vSetDefaultColors
Input(s)       :  
Output         :  VOID
Functionality  :   
Member of      :  CTreeViewEx
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  30/03/2011
Modifications  :  
******************************************************************************/
VOID CTreeViewEx::vSetDefaultColors()
{
    m_omBkColor = def_COLOR_TREE_BKG;    
    m_omTextColor = def_COLOR_TREE_TEXT;
}
