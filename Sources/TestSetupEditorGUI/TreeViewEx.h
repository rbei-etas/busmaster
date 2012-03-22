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
 * \file      TreeViewEx.h
 * \author    Venkatanarayana Makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#if !defined(_TSEDITORGUI_TREEVIEW_H_INCLUDED_)
#define _TSEDITORGUI_TREEVIEW_H_INCLUDED_

class CTSEditorChildFrame;
#define def_COLOR_TREE_BKG      RGB(237, 239, 254)
#define def_COLOR_TREE_TEXT     RGB(0, 0, 0);

class CTreeViewEx : public CTreeView
{
	DECLARE_DYNCREATE(CTreeViewEx)
//Member Variables
private:
    COLORREF m_omBkColor;
    COLORREF m_omTextColor;
    BOOL m_bDragging;
    BOOL m_bEditing;
    HTREEITEM m_hDraggingItemgedItem;
    HCURSOR m_hCurMoveDown;
    HCURSOR m_hCurMoveUp;
    HCURSOR m_hOrigCursor;
    HCURSOR m_hCurNoDrop;
    CImageList *m_pomImageList;
    CImageList *m_pomDragImageList;
    typedef enum eTYPE_DROPPING 
    {
        DROPPING_BELOW,
		DROPPING_ABOVE,
		NO_DROPPING 
    };
//Methods
private:
    CTSEditorChildFrame* GetEditorWindow();
    int unGetIndex(HTREEITEM hTreeItem);
protected:
	DECLARE_MESSAGE_MAP()
    CTreeViewEx();           // protected constructor used by dynamic creation
    virtual ~CTreeViewEx();


    BOOL bIsItemChecked(HTREEITEM hItem);
    BOOL bAnscestor(HTREEITEM hItem, HTREEITEM hCheck);
    BOOL bSuccessor(HTREEITEM hItem, HTREEITEM hCheck);

    virtual BOOL bItemCanDragged(HTREEITEM hItem);
    virtual BOOL IsItemCanDropOn(HTREEITEM hSource, HTREEITEM hTarget);

//To handle Chechbox
    void vSetCheck(HTREEITEM hti, BOOL bCheck);
    void vSetCheckChildren(HTREEITEM hItem, BOOL fCheck);
    void vSetCheckParent(HTREEITEM hItem);

//Item drag
    void DragMove(CPoint point);
    void DragEnd();
    HTREEITEM DragMoveItem(HTREEITEM hDraggingItem, HTREEITEM hDroppingItem, eTYPE_DROPPING eDropping);
    void SetDraggingCursor(eTYPE_DROPPING eDropping);    

//tree Item expansion handlers
    void vCollapseFull();
    void vCollapseTreeBranch( HTREEITEM hItem );
    void vExpandFull();
    void OnBKGColor();
    void vExpandTreeBranch( HTREEITEM hItem);

//Tree item update handlers
    void DeleteItem( HTREEITEM hItem);
    HTREEITEM CopyTreeItem( HTREEITEM hItem, HTREEITEM hNewParentItem,HTREEITEM hAfterItem );
    HTREEITEM CopyTotalBranch( HTREEITEM hBranchItem, HTREEITEM hNewParentItem, HTREEITEM hAfterItem );
    HTREEITEM GetDropItem(eTYPE_DROPPING & hint);

    eTYPE_DROPPING GetDroppingPosition(UINT flags);
 
public:

//Over-written menthods    
    virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
    virtual void OnInitialUpdate();
    virtual BOOL PreTranslateMessage(MSG* pMsg);

//Message Handlers
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
//    afx_msg void OnInsertTreeItem(){MessageBox(_T("Hi"));};
    afx_msg void OnAddEntityItem(UINT nId);
    afx_msg void vOnDeleteItem();
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
    afx_msg void OnTvnSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnTvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);

//Services
    HTREEITEM InsertTreeItem(HTREEITEM hParent, CString csItemName, HTREEITEM hInsAfter, 
	                  int iSelImage ,int iNonSelImage, long lParam);
    INT SetImageList(CImageList *pomImageListNormal, CImageList *pomImageListState);
    void ShowCheckBoxes(BOOL bShow);
    void vDeleteChildItems(HTREEITEM hItem);

//Debug Functions
public:
    #ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
    #endif

    afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
    virtual BOOL CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, LPVOID lpParam = NULL);
    VOID vSetTreeCtrlColor(COLORREF omBkColor, COLORREF omTextColor);
    VOID vGetTreeCtrlColor(COLORREF& omBkColor, COLORREF& omTextColor);
    VOID vSetDefaultColors();
};

#endif