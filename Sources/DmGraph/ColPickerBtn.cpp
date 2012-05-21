// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//      ColPickerBtn.cpp: implementation for the CColPickerBtn class
//
// CLASS NAME
//      CColPickerBtn
//
// DESCRIPTION
//
// MODIFICATIONS
//      01-Dec-2011 MSamoila major re-design of old ColourPicker
//
//
#include "stdafx.h"
#include "ColPickerBtn.h"


/////////////////////////////////////////////////////////////////////////////
// CColPickerBtn

CColPickerBtn::CColPickerBtn()
{
    m_col = GetSysColor(COLOR_3DFACE);
}

LRESULT CColPickerBtn::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if(HIWORD(wParam) != BN_CLICKED)
    {
        return 0;
    }

    HWND hWndParent = GetParent();
    CHOOSECOLOR cc = { 0 };
    static COLORREF colCust[16] = { 0 };
    cc.lStructSize = sizeof(CHOOSECOLOR);
    cc.hwndOwner = hWndParent;
    cc.rgbResult = m_col;
    cc.lpCustColors = colCust;
    cc.Flags = CC_RGBINIT;

    if(ChooseColor(&cc))
    {
        m_col = cc.rgbResult;
        Invalidate(FALSE);
        ::PostMessage(hWndParent, WM_COL_CHANGED, m_col, (LPARAM)GetWindowLong(GWL_ID));
    }

    return 0;
}

LRESULT CColPickerBtn::OnDrawItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    LPDRAWITEMSTRUCT lpDrawItemStruct = (LPDRAWITEMSTRUCT)lParam;
    ATLASSERT(lpDrawItemStruct);
    RECT rcBtn, rcTxt, rcCol;
    memcpy(&rcBtn, &lpDrawItemStruct->rcItem, sizeof(RECT));
    memcpy(&rcTxt, &lpDrawItemStruct->rcItem, sizeof(RECT));
    SIZE offsetFocusRect = { GetSystemMetrics(SM_CXFRAME)/2, GetSystemMetrics(SM_CYFRAME)/2 };
    SIZE offsetText = { GetSystemMetrics(SM_CXEDGE)/2, GetSystemMetrics(SM_CXEDGE)/2 };
    CComBSTR bsText;
    GetWindowText(bsText.m_str);

    if(bsText.Length())
    {
        //write the text on the 2/3 left side
        rcTxt.right     = 2 * (rcBtn.right  - rcBtn.left) / 3;
        //write the color on the 1/3 right side
        rcCol.left      = 2 * (rcBtn.right  - rcBtn.left) / 3;
        rcCol.top       =     (rcBtn.bottom - rcBtn.top)  / 4;
        rcCol.bottom    = 3 * (rcBtn.bottom - rcBtn.top)  / 4;
        rcCol.right     =      rcBtn.right - offsetFocusRect.cx;

        if ((lpDrawItemStruct->itemState & ODS_SELECTED) == 0)
        {
            OffsetRect(&rcTxt, -offsetText.cx, -offsetText.cy);
            OffsetRect(&rcCol, -offsetText.cx, -offsetText.cy);
        }

        if(lpDrawItemStruct->itemState & ODS_DISABLED)
        {
            SetTextColor(lpDrawItemStruct->hDC, GetSysColor(COLOR_ACTIVEBORDER));
        }

        DrawTextW(lpDrawItemStruct->hDC, bsText, -1, &rcTxt, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
    }
    else
    {
        rcCol.left      =       rcBtn.left + offsetFocusRect.cx;
        rcCol.top       =      (rcBtn.bottom - rcBtn.top)  / 4;
        rcCol.bottom    = 3 *  (rcBtn.bottom - rcBtn.top)  / 4;
        rcCol.right     =       rcBtn.right - offsetFocusRect.cx;

        if ((lpDrawItemStruct->itemState & ODS_SELECTED) == 0)
        {
            OffsetRect(&rcCol, -offsetText.cx, -offsetText.cy);
        }
    }

    //write the color on the 1/3 right side
    DrawEdge(lpDrawItemStruct->hDC, &rcCol, lpDrawItemStruct->itemState & ODS_SELECTED ? EDGE_SUNKEN : EDGE_RAISED, BF_RECT);
    COLORREF oldCol = ::SetBkColor(lpDrawItemStruct->hDC, m_col);
    InflateRect(&rcCol, -offsetText.cx*2, -offsetText.cy*2);
    ::ExtTextOut(lpDrawItemStruct->hDC, 0, 0, ETO_OPAQUE, &rcCol, NULL, 0, NULL);
    ::SetBkColor(lpDrawItemStruct->hDC, oldCol);
    DrawEdge(lpDrawItemStruct->hDC, &rcBtn, lpDrawItemStruct->itemState & ODS_SELECTED ? EDGE_SUNKEN : EDGE_RAISED, BF_RECT);

    // Draw focus rect
    if (lpDrawItemStruct->itemState & ODS_FOCUS)
    {
        InflateRect(&rcBtn, -offsetFocusRect.cx, -offsetFocusRect.cy);
        DrawFocusRect(lpDrawItemStruct->hDC, &rcBtn);
    }

    return 0;
}

