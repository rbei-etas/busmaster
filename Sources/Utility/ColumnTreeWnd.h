/*********************************************************************
* Multi-Column Tree View, version 1.4 (July 7, 2005)
* Copyright (C) 2003-2005 Michal Mecinski.
*
* You may freely use and modify this code, but don't remove
* this copyright note.
*
* THERE IS NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, FOR
* THIS CODE. THE AUTHOR DOES NOT TAKE THE RESPONSIBILITY
* FOR ANY DAMAGE RESULTING FROM THE USE OF IT.
*
* E-mail: mimec@mimec.org
* WWW: http://www.mimec.org
********************************************************************/

#pragma once

#include "ColumnTreeCtrl.h"


class CColumnTreeWnd : public CWnd
{
public:
    CColumnTreeWnd();
    virtual ~CColumnTreeWnd();

    enum ChildrenIDs { HeaderID = 1, TreeID = 2 };

    void UpdateColumns();
    void AdjustColumnWidth(int nColumn, BOOL bIgnoreCollapsed);

    CTreeCtrl& GetTreeCtrl()
    {
        return m_Tree;
    }

    CHeaderCtrl& GetHeaderCtrl()
    {
        return m_Header;
    }

    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

protected:
    virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

    void UpdateScroller();
    void RepositionControls();
    int GetMaxColumnWidth(HTREEITEM hItem, int nColumn, int nDepth, BOOL bIgnoreCollapsed);

    CColumnTreeCtrl m_Tree;
    CHeaderCtrl m_Header;
    int m_cyHeader;
    int m_cxTotal;
    int m_xPos;
    int m_arrColWidths[16];
    int m_xOffset;

    DECLARE_MESSAGE_MAP()

    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnHeaderItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnHeaderDividerDblClick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnTreeCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);

private:
    DECLARE_DYNAMIC(CColumnTreeWnd)
};
