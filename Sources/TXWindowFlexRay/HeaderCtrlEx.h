//*************************************************************************
// HeaderCtrlEx.h : header file
// Version : 1.0
// Date : June 2005
// Author : Chen Hao
// Email :  ch@sothink.com
// Website : http://www.sothink.com
//
// You are free to use/modify this code but leave this header intact.
// This class is public domain so you are free to use it any of
// your applications (Freeware,Shareware,Commercial). All I ask is
// that you let me know that some of my code is in your app.
//*************************************************************************

#pragma once

// CHeaderCtrlEx

extern UINT WM_HDN_ENDDRAG;

class CHeaderCtrlEx : public CHeaderCtrl
{
public:
    class CItemData
    {
    public:
        CItemData(int nWidth, BOOL bRemovable, BOOL bVisible)
        {
            m_nWidth = nWidth;
            m_bRemovable = bRemovable;
            m_bVisible = bVisible;
        }
        int m_nWidth;
        BOOL m_bRemovable;
        BOOL m_bVisible;
    };

public:
    CHeaderCtrlEx();
    virtual ~CHeaderCtrlEx();

    BOOL SetItemWidth(int index, int nWidth);
    int GetItemWidth(int index);
    BOOL GetWidthArray(int* piArray, int iCount);
    BOOL SetWidthArray(int iCount, int* piArray);
    DWORD_PTR GetItemData(int index);
    BOOL SetItemData(int index, DWORD_PTR dwData);
    int IndexToOrder(int index);
    int FindVisibleItem(int index);
    BOOL GetVisibleArray(int* piArray, int iCount);
    BOOL SetVisibleArray(int iCount, int* piArray);
    BOOL GetVisible(int index);
    void SetVisible(int index, BOOL bVisible);
    BOOL GetRemovable(int index);
    void SetRemovable(int index, BOOL bRemovable);
    void ResetItemWidth(int index);
    BOOL SetOrderArray(int iCount, LPINT piArray);
    int GetVisibleItemCount();

protected:
    DECLARE_MESSAGE_MAP()
    int m_nDraggingItem;
    CPoint m_ptDragStart;
    CPoint m_ptDragMove;
    CPoint m_ptDragEnd;
    BOOL m_nItemWidth;
    BOOL m_bDragFullWindow;

    int ItemFromPoint(CPoint point);
    void BeginDragDivider();
    void EndDragDivider(BOOL bSubmit);
    void DrawDragDivider();

public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnCaptureChanged(CWnd* pWnd);
    afx_msg void OnDestroy();
    //afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg LRESULT OnHdnEndDrag(WPARAM wp, LPARAM lp);
};
