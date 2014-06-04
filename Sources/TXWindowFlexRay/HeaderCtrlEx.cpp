//*************************************************************************
// HeaderCtrlEx.cpp : header file
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

#include "TxWindowFlexRay_stdafx.h"
#include ".\HeaderCtrlEx.h"
#include "atlbase.h"
#include "TxWindowFlexRay_resource.h"

// CHeaderCtrlEx

UINT WM_HDN_ENDDRAG = ::RegisterWindowMessage("{8548F4AE-4A04-4c1e-9D0C-51CAEBEEA587}");

CHeaderCtrlEx::CHeaderCtrlEx()
{
    m_nDraggingItem = -1;
    m_nItemWidth = 0;
    m_bDragFullWindow = FALSE;
}

CHeaderCtrlEx::~CHeaderCtrlEx()
{
}


BEGIN_MESSAGE_MAP(CHeaderCtrlEx, CHeaderCtrl)
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_CAPTURECHANGED()
    ON_WM_DESTROY()
    //ON_WM_SETCURSOR()
    ON_WM_LBUTTONDBLCLK()
    ON_REGISTERED_MESSAGE(WM_HDN_ENDDRAG, OnHdnEndDrag)
END_MESSAGE_MAP()



// CHeaderCtrlEx message handlers

BOOL CHeaderCtrlEx::SetItemWidth(int index, int nWidth)
{
    HDITEM hdi;
    memset(&hdi, 0, sizeof(hdi));
    hdi.mask = HDI_WIDTH;
    hdi.cxy = nWidth;
    return SetItem(index, &hdi);
}

int CHeaderCtrlEx::GetItemWidth(int index)
{
    HDITEM hdi;
    memset(&hdi, 0, sizeof(hdi));
    hdi.mask = HDI_WIDTH;
    if (GetItem(index, &hdi))
    {
        return hdi.cxy;
    }
    return 0;
}

BOOL CHeaderCtrlEx::GetWidthArray(int* piArray, int iCount)
{
    HDITEM hdi;
    memset(&hdi, 0, sizeof(hdi));
    hdi.mask = HDI_WIDTH;
    for (int i = 0; i < iCount; i++)
    {
        if (!GetItem(i, &hdi))
        {
            return FALSE;
        }
        piArray[i] = hdi.cxy;
    }

    return TRUE;
}

BOOL CHeaderCtrlEx::SetWidthArray(int iCount, int* piArray)
{
    HDITEM hdi;
    memset(&hdi, 0, sizeof(hdi));
    hdi.mask = HDI_WIDTH;
    for (int i = 0; i < iCount; i++)
    {
        hdi.cxy = piArray[i];
        if (!SetItem(i, &hdi))
        {
            return FALSE;
        }
    }

    return TRUE;
}

BOOL CHeaderCtrlEx::GetVisibleArray(int* piArray, int iCount)
{
    for (int i = 0; i < iCount; i++)
    {
        piArray[i] = GetVisible(i);
    }

    return TRUE;
}

BOOL CHeaderCtrlEx::SetVisibleArray(int iCount, int* piArray)
{
    for (int i = 0; i < iCount; i++)
    {
        SetVisible(i, piArray[i]);
    }
    return TRUE;
}

BOOL CHeaderCtrlEx::GetVisible(int index)
{
    CItemData* pData = (CItemData*)GetItemData(index);
    return pData->m_bVisible;
}

int CHeaderCtrlEx::GetVisibleItemCount()
{
    int count = GetItemCount();
    int visible = 0;
    for (int i = 0; i < count; i++)
        if (GetVisible(i))
        {
            visible++;
        }
    return visible;
}

void CHeaderCtrlEx::SetVisible(int index, BOOL bVisible)
{
    CItemData* pData = (CItemData*)GetItemData(index);
    if (pData->m_bVisible != bVisible)
    {
        pData->m_bVisible = bVisible;

        // get total items
        int count = GetItemCount();
        // get current item's order
        int nOrder = IndexToOrder(index);

        CAutoPtr<int> pCols(new int[count]);
        GetOrderArray(pCols, count);

        if (bVisible)
        {
            // restore item width
            ResetItemWidth(index);

            // move the item to the original position
            int nVisible = GetVisibleItemCount();
            int nTarget;
            if (index > nVisible - 1)
            {
                nTarget = nVisible - 1;
            }
            else
            {
                nTarget = index;
            }

            ASSERT(nTarget <= nOrder);

            for (int i = nOrder; i > nTarget; i--)
            {
                pCols[i] = pCols[i - 1];
            }
            pCols[nTarget] = index;
        }
        else
        {
            // hide item
            SetItemWidth(index, 0);
        }

        SetOrderArray(count, pCols);
    }
}

BOOL CHeaderCtrlEx::SetOrderArray(int iCount, LPINT piArray)
{
    // move all hidden items to the end
    int* piArrayNew = new int[iCount];
    int* p1 = piArrayNew;
    int* p2 = piArrayNew + (iCount - 1);
    for (int i = 0, j = 0; i < iCount; i++)
    {
        int index = piArray[i];
        if (GetVisible(index))
        {
            *p1++ = index;
        }
        else
        {
            *p2-- = index;
        }
    }

    BOOL ret = CHeaderCtrl::SetOrderArray(iCount, piArrayNew);
    delete[] piArrayNew;

    GetParent()->Invalidate();

    return ret;
}

BOOL CHeaderCtrlEx::GetRemovable(int index)
{
    CItemData* pData = (CItemData*)GetItemData(index);
    return pData->m_bRemovable;
}

void CHeaderCtrlEx::SetRemovable(int index, BOOL bRemovable)
{
    CItemData* pData = (CItemData*)GetItemData(index);
    pData->m_bRemovable = bRemovable;
}

void CHeaderCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    HDHITTESTINFO ht;
    memset(&ht, 0, sizeof(HDHITTESTINFO));
    ht.pt = point;
    int index = (int)SendMessage(HDM_HITTEST, 0, (LPARAM)&ht);
    if (index >= 0)
    {
        if (ht.flags == HHT_ONDIVIDER)
        {
            ReleaseCapture();
            m_nDraggingItem = index;
        }
        else if (ht.flags == HHT_ONDIVOPEN)
        {
            ReleaseCapture();
            m_nDraggingItem = FindVisibleItem(index);
        }

        if (m_nDraggingItem >= 0)
        {
            m_ptDragStart = point;
            BeginDragDivider();

            return;
        }
    }

    CHeaderCtrl::OnLButtonDown(nFlags, point);
}

void CHeaderCtrlEx::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    if (m_nDraggingItem >= 0)
    {
        int dx = point.x - m_ptDragStart.x;
        int cxy = max(0, m_nItemWidth + dx);

        if (m_bDragFullWindow)
        {
            SetItemWidth(m_nDraggingItem, cxy);
        }
        else
        {
            DrawDragDivider();
            m_ptDragMove = point;
            DrawDragDivider();
        }
        return;
    }

    CHeaderCtrl::OnMouseMove(nFlags, point);
}

void CHeaderCtrlEx::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    if (m_nDraggingItem >= 0)
    {
        m_ptDragEnd = point;
        EndDragDivider(TRUE);
        return;
    }

    CHeaderCtrl::OnLButtonUp(nFlags, point);
}

void CHeaderCtrlEx::BeginDragDivider()
{
    SetCapture();

    m_nItemWidth = GetItemWidth(m_nDraggingItem);

    // check if dragging full window is enabled
    ::SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0, &m_bDragFullWindow, 0);
    m_ptDragMove = m_ptDragStart;
    if (!m_bDragFullWindow)
    {
        DrawDragDivider();
    }
}

void CHeaderCtrlEx::EndDragDivider(BOOL bSubmit)
{
    if (!m_bDragFullWindow)
    {
        DrawDragDivider();
    }

    if (bSubmit)
    {
        if (!m_bDragFullWindow)
        {
            int dx = m_ptDragEnd.x - m_ptDragStart.x;
            int cxy = max(0, m_nItemWidth + dx);
            SetItemWidth(m_nDraggingItem, cxy);
        }
    }
    else
    {
        // cancel and restore the original width
        if (m_bDragFullWindow)
        {
            SetItemWidth(m_nDraggingItem, m_nItemWidth);
        }
    }

    m_nDraggingItem = -1;
    ReleaseCapture();
}

void CHeaderCtrlEx::DrawDragDivider()
{
    CWnd* pParent = GetParent();

    CDC* pDC = pParent->GetDCEx(nullptr, DCX_WINDOW | DCX_CACHE | DCX_LOCKWINDOWUPDATE);
    int nROP2 = pDC->SetROP2(R2_NOT);

    CPen pen;
    pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    CPen* pPenOld = pDC->SelectObject(&pen);

    CRect rc;
    pParent->GetWindowRect(&rc);
    ScreenToClient(&rc);

    CRect rcItem;
    GetItemRect(m_nDraggingItem, &rcItem);

    int dx = m_ptDragMove.x - m_ptDragStart.x;
    int x = max(rcItem.left, rcItem.right + dx);
    pDC->MoveTo(x, rc.top);
    pDC->LineTo(x, rc.bottom);

    pDC->SelectObject(pPenOld);
    pDC->SetROP2(nROP2);
    pParent->ReleaseDC(pDC);
}

void CHeaderCtrlEx::OnCaptureChanged(CWnd* pWnd)
{
    // TODO: Add your message handler code here

    TRACE("OnCaptureChanged(0x%08x), m_nDraggingItem = %d\n", pWnd, m_nDraggingItem);
    if (pWnd != this && m_nDraggingItem >= 0)
    {
        EndDragDivider(FALSE);
    }

    CHeaderCtrl::OnCaptureChanged(pWnd);
}

int CHeaderCtrlEx::ItemFromPoint(CPoint point)
{
    for (int i = 0; i < GetItemCount(); i++)
    {
        CRect rc;
        GetItemRect(i, &rc);
        if (rc.PtInRect(point))
        {
            return i;
        }
    }
    return -1;
}

DWORD_PTR CHeaderCtrlEx::GetItemData(int index)
{
    HDITEM hdi;
    hdi.mask = HDI_LPARAM;
    if (GetItem(index, &hdi))
    {
        return (DWORD_PTR)hdi.lParam;
    }
    return 0;
}

BOOL CHeaderCtrlEx::SetItemData(int index, DWORD_PTR dwData)
{
    HDITEM hdi;
    hdi.mask = HDI_LPARAM;
    hdi.lParam = (LPARAM)dwData;
    return SetItem(index, &hdi);
}

int CHeaderCtrlEx::IndexToOrder(int index)
{
    int nOrder = -1;
    int count = GetItemCount();
    if (count > 0)
    {
        CAutoPtr <int> pnColOrder(new int[count]);
        GetOrderArray(pnColOrder, count);
        for (int i = 0; i < count; i++)
        {
            if (pnColOrder[i] == index)
            {
                nOrder = i;
                break;
            }
        }
    }
    return nOrder;
}

int CHeaderCtrlEx::FindVisibleItem(int index)
{
    if (GetVisible(index))
    {
        return index;
    }

    int nOrder = IndexToOrder(index);
    while (nOrder > 0)
    {
        index = OrderToIndex(--nOrder);
        if (GetVisible(index))
        {
            return index;
        }
    }
    return -1;
}

void CHeaderCtrlEx::OnDestroy()
{

    for (int i = 0; i < GetItemCount(); i++)
    {
        delete (CItemData*)GetItemData(i);
        SetItemData(i, 0);
    }

    CHeaderCtrl::OnDestroy();

    // TODO: Add your message handler code here
}

void CHeaderCtrlEx::ResetItemWidth(int index)
{
    CItemData* pData = (CItemData*)GetItemData(index);
    SetItemWidth(index, pData->m_nWidth);
}

/*
BOOL CHeaderCtrlEx::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    // TODO: Add your message handler code here and/or call default

    //return CHeaderCtrl::OnSetCursor(pWnd, nHitTest, message);

    HCURSOR hCursor = nullptr;

    HDHITTESTINFO ht;
    ::GetCursorPos(&ht.pt);
    ScreenToClient(&ht.pt);
    int index = (int)SendMessage(HDM_HITTEST, 0, (LPARAM)&ht);
    if (index >= 0)
    {
        if (ht.flags == HHT_ONDIVIDER)
        {
            hCursor = AfxGetApp()->LoadCursor(IDC_HEADER_SIZE);
            ASSERT(hCursor != nullptr);
        }
        else if (ht.flags == HHT_ONDIVOPEN)
        {
            int nItem = FindVisibleItem(index);
            if (nItem >= 0)
            {
                if (GetItemWidth(nItem) > 0)
                    hCursor = AfxGetApp()->LoadCursor(IDC_HEADER_SIZE);
                else
                    hCursor = AfxGetApp()->LoadCursor(IDC_HEADER_OPEN);
            }
        }
    }

    if (hCursor == nullptr)
        hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);

    ::SetCursor(hCursor);

    return TRUE;
}
*/

void CHeaderCtrlEx::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default

    HDHITTESTINFO ht;
    memset(&ht, 0, sizeof(HDHITTESTINFO));
    ::GetCursorPos(&ht.pt);
    ScreenToClient(&ht.pt);
    int index = (int)SendMessage(HDM_HITTEST, 0, (LPARAM)&ht);
    if (index >= 0)
    {
        TRACE("ht.flags = %d\n", ht.flags);
        if (ht.flags == HHT_ONDIVOPEN)
        {
            int nItem = FindVisibleItem(index);
            if (nItem >= 0)
            {
                if (GetItemWidth(nItem) > 0)
                {
                    NMHEADER nm;
                    nm.hdr.code = HDN_DIVIDERDBLCLICK;
                    nm.hdr.hwndFrom = m_hWnd;
                    nm.hdr.idFrom = GetDlgCtrlID();
                    nm.iButton = 0;
                    nm.iItem = nItem;
                    nm.pitem = nullptr;    // not used for HDN_DIVIDERDBLCLICK
                    GetParent()->SendMessage(WM_NOTIFY, (WPARAM)nm.hdr.idFrom, (LPARAM)&nm);
                }
            }
        }
    }

    CHeaderCtrl::OnLButtonDblClk(nFlags, point);
}

LRESULT CHeaderCtrlEx::OnHdnEndDrag(WPARAM wp, LPARAM /* lp */)
{
    int count = GetItemCount();
    int* piArray = new int[count];
    GetOrderArray(piArray, count);
    SetOrderArray(count, piArray);
    delete[] piArray;

    return 1;
}
