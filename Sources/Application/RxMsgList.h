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
 * \file      RxMsgList.h
 * \brief     Definition of CRxMsgList class
 * \authors   Ravi D, Arunkumar Karri
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of CRxMsgList class
 */

#pragma once

#include "Utility/flickerfreelistctrl.h"
#include "include/BaseDefs.h"

class CRxMsgList: public CFlickerFreeListCtrl
{
private:
    //BOOL m_bIsAppend;
    //eMode m_eDispMode;
    int m_nIconMinus;
    int m_nIconPlus;
    int m_nIconBlank;
    BOOL m_bDispModeChanged;
    CImageList m_ImageList;
    TCHAR* m_pomDataPtrArr[MAX_MSG_WND_COL_CNT];
    COLORREF m_clrMsg;
    bool* m_pbSortableColumn;
    bool* m_pbAscendingOrder;
    int m_nSortedColumn;
    CPoint m_lClkPoint;
    CFont m_omNewFont;

protected:
    struct ColumnState
    {
        ColumnState()
            :m_Visible(false)
            ,m_OrgWidth(0)
            ,m_OrgPosition(-1)
        {}
        bool m_Visible;
        int  m_OrgWidth;    // Width it had before being hidden
        int  m_OrgPosition; // Position it had before being hidden
    };
    //CSimpleArray<ColumnState> m_ColumnStates;
    CArray<ColumnState, ColumnState>    m_ColumnStates;
    int GetColumnStateCount();
    void InsertColumnState(int nCol, bool bVisible, int nOrgWidth = 0);
    void DeleteColumnState(int nCol);
    ColumnState& GetColumnState(int nCol);

    virtual afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    virtual afx_msg LRESULT OnDeleteColumn(WPARAM wParam, LPARAM lParam);
    virtual afx_msg LRESULT OnInsertColumn(WPARAM wParam, LPARAM lParam);
    virtual afx_msg LRESULT OnSetColumnWidth(WPARAM wParam, LPARAM lParam);
    virtual afx_msg BOOL OnHeaderBeginResize(UINT id, NMHDR* pNmhdr, LRESULT* pResult);
    virtual afx_msg BOOL OnHeaderEndDrag(UINT id, NMHDR* pNmhdr, LRESULT* pResult);
    virtual afx_msg BOOL OnHeaderDividerDblClick(UINT, NMHDR* pNMHDR, LRESULT* pResult);
    virtual afx_msg void OnContextMenu(CWnd*, CPoint point);
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

public:
    HWND m_hParent;
    bool IsColumnVisible(int nCol);
    int GetFirstVisibleColumn();
    BOOL ShowColumn(int nCol, bool bShow);
    BOOL SetColumnWidthAuto(int nCol = -1, bool includeHeader = false);

private:
    class CListHdrCtrl : public CHeaderCtrl
    {
    private:
        void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
        int m_nSortCol;
        BOOL m_bIsAscendingSort;

    public:
        CListHdrCtrl();
        virtual ~CListHdrCtrl() {};
        void SelectSortedColumn(int nCol);
        void SelectSortAscDesc(BOOL bAscending);

        BOOL IsAscendingSort() const;
        int RetreiveSortColID() const;

        void UpdateArrowDirection();
        void ClearArrowDirection();
    };
    bool m_bConnected;

public:
    // Member data
    CListHdrCtrl m_wndHeader;
    CRxMsgList();
    virtual ~CRxMsgList(void);
    void vSetDsipItemDataPtrArr(TCHAR* pomDataPtrArr[MAX_MSG_WND_COL_CNT]);
    void vDoInitialization();
    void vSetMsgColor(COLORREF color);
    void vShowHideBlankcolumn(BOOL bInterpretON);
    void vSetSortableMsgColumns(SMSGWNDHDRCOL& sHdrColStruct, ETYPE_BUS eBusType);
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnLvnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnNMDblclk(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDestroy();
    afx_msg void OnHdnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
protected:
    virtual void PreSubclassWindow();
public:
    afx_msg void OnHdnItemclick(NMHDR* pNMHDR, LRESULT* pResult);
    void SetConnectionStatus(bool bConnected);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
