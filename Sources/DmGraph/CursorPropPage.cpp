// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//      CursorPropPage.cpp: implementation for the CCursorPropPage class
//
// CLASS NAME
//      CCursorPropPage
//
// DESCRIPTION
//
// MODIFICATIONS
//      01-Dec-2011 MSamoila major re-design of old CCursorPropPage
//
//
#include "stdafx.h"
#include "DMGraph.h"
#include "CursorPropPage.h"

static HRESULT WINAPI GetSelectedCursor(IDMGraphCtrl* pGraphCtrl, IDMGraphCursor** pCursor);

void WINAPI FillCombo(HWND hWndCombo, UINT id, int idx)
{
    CComBSTR bsItems;
    bsItems.LoadString(id);
    LPOLESTR ptr;
    ATLASSERT(::IsWindow(hWndCombo));
    int j;

    for(ptr = wcstok(bsItems, L"\n"); ptr; ptr = wcstok(NULL, L"\n"))
    {
        j = ::SendMessageW(hWndCombo, CB_ADDSTRING, 0, (LPARAM)ptr);

        if(j == idx)
        {
            ::SendMessageW(hWndCombo, CB_SETCURSEL, j, 0);
            ::SetWindowTextW(hWndCombo, ptr);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// CCursorPropPage

LRESULT CCursorPropPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    m_btnColPicker.SubclassWindow(GetDlgItem(IDC_CURSOR_COLOR));
    FillCombo(GetDlgItem(IDC_COMBO_STYLE), IDS_CURSOR_CROSSHAIR_STYLE, -1);
    FillCombo(GetDlgItem(IDC_COMBO_SNAP), IDS_CURSOR_SNAP_MODE, -1);
    HRESULT hr;
    CComPtr<IDMGraphCtrl> spGraph;

    if(FAILED(GetGraphCtrl(&spGraph)))
    {
        return 0;
    }

    CComPtr<IDMGraphCollection> spGraphCollection;
    hr = spGraph->get_Cursors(&spGraphCollection);
    InitCollectionList(spGraphCollection);
    //Update controls
    UpdateControls(spGraph);
    m_bInited = TRUE;
    return 1;
}

void CCursorPropPage::UpdateControls(IDMGraphCtrl* pGraphCtrl)
{
    ATLASSERT(pGraphCtrl);
    CComPtr<IDMGraphCursor> spCursor;
    GetSelectedCursor(pGraphCtrl, &spCursor);
    BOOL bEnabled = spCursor != NULL;
    const UINT idCtrls[] = { IDC_COMBO_STYLE, IDC_EDIT_X, IDC_EDIT_Y, IDC_COMBO_SNAP, IDC_CURSOR_COLOR, IDC_EDIT_NAME,
                             IDC_CHECK_VISIBLE,
                             IDC_TXT_C1, IDC_TXT_C2, IDC_TXT_C3, IDC_TXT_C4, IDC_TXT_C5, IDC_TXT_C6, 0
                           };
    int i;

    for(i=0; idCtrls[i]; ::EnableWindow(GetDlgItem(idCtrls[i++]), bEnabled));

    if(bEnabled == FALSE)
    {
        return;
    }

    HRESULT hr;
    const int TXT_LEN = 128;
    TCHAR szText[ TXT_LEN ];
    double x,y;
    hr = spCursor->get_X(&x);
    hr = spCursor->get_Y(&y);
    sprintf_s((char*) szText, sizeof(szText), "%g", x);
    SetDlgItemText (IDC_EDIT_X, szText);
    sprintf_s((char*) szText, sizeof(szText), "%g", y);
    SetDlgItemText (IDC_EDIT_Y, szText);
    OLE_COLOR color;
    COLORREF col;
    hr = spCursor->get_Color(&color);
    hr = OleTranslateColor(color, NULL, &col);
    m_btnColPicker.SetColor(col);
    Crosshair style;
    hr = spCursor->get_Style(&style);
    SendDlgItemMessage(IDC_COMBO_STYLE, CB_SETCURSEL, style);
    CursorMode snap;
    hr = spCursor->get_Mode(&snap);
    SendDlgItemMessage(IDC_COMBO_SNAP, CB_SETCURSEL, snap);
    CComBSTR bsName;
    hr = spCursor->get_Name(&bsName);
    ::SetDlgItemTextW(m_hWnd, IDC_EDIT_NAME, bsName);
    VARIANT_BOOL visible;
    hr = spCursor->get_Visible(&visible);
    CheckDlgButton(IDC_CHECK_VISIBLE, visible ? BST_CHECKED : BST_UNCHECKED);
    long count = 0;
    CComPtr<IDMGraphCollection> spGraphCollection;
    hr = pGraphCtrl->get_Cursors(&spGraphCollection);
    hr = spGraphCollection->get_Count(&count);
    HWND hWnd = GetDlgItem(IDC_DELBUTTON);
    ATLASSERT(::IsWindow(hWnd));
    ::EnableWindow(hWnd, count > 0);
}

LRESULT CCursorPropPage::OnColChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if(m_bInited == FALSE)
    {
        return 0;    //ignore notificatin messages sent before WM_INITDIALOG
    }

    HRESULT hr;
    CComPtr<IDMGraphCtrl> spGraph;

    if(FAILED(GetGraphCtrl(&spGraph)))
    {
        return 0;
    }

    CComPtr<IDMGraphCursor> spCursor;
    GetSelectedCursor(spGraph, &spCursor);

    if(spCursor == NULL)
    {
        return 0;
    }

    OLE_COLOR color = (OLE_COLOR)wParam;
    ATLASSERT(lParam == IDC_CURSOR_COLOR);
    hr = spCursor->put_Color(color);
    return 0;
}

LRESULT CCursorPropPage::OnClickedAddbutton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    ATLTRACE(_T("OnClickedAddbutton\n"));
    HRESULT hr;
    CComPtr<IDMGraphCtrl> spGraph;

    if(FAILED(GetGraphCtrl(&spGraph)))
    {
        return 0;
    }

    CComPtr<IDMGraphCollection> spGraphCollection;
    hr = spGraph->get_Cursors(&spGraphCollection);
    AppendNewItem(spGraphCollection);
    UpdateControls(spGraph);
    return 0;
}

LRESULT CCursorPropPage::OnClickedDelbutton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    ATLTRACE(_T("OnClickedDelbutton\n"));
    HRESULT hr;
    CComPtr<IDMGraphCtrl> spGraph;

    if(FAILED(GetGraphCtrl(&spGraph)))
    {
        return 0;
    }

    CComPtr<IDMGraphCollection> spGraphCollection;
    hr = spGraph->get_Cursors(&spGraphCollection);
    DeleteItem(spGraphCollection);
    UpdateControls(spGraph);
    return 0;
}

LRESULT CCursorPropPage::OnClickedRemoveSelbutton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    HRESULT hr;
    CComPtr<IDMGraphCtrl> spGraph;

    if(FAILED(GetGraphCtrl(&spGraph)))
    {
        return 0;
    }

    CComPtr<IDMGraphCollection> spGraphCollection;
    hr = spGraph->get_Cursors(&spGraphCollection);
    RemoveSelection(spGraphCollection);
    UpdateControls(spGraph);
    return 0;
}

LRESULT CCursorPropPage::OnItemchangedElement_list(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
    if(m_bInited == FALSE)
    {
        return 0;    //ignore notificatin messages sent before WM_INITDIALOG
    }

    BOOL bIsSelected = FALSE;
    const LPNMLISTVIEW pHmLv = (const LPNMLISTVIEW)pnmh;

    if(pHmLv->uChanged & LVIF_STATE)
    {
        BOOL bSelNew = pHmLv->uNewState & LVIS_SELECTED;
        BOOL bSelOld = pHmLv->uOldState & LVIS_SELECTED;

        if(bSelNew && !bSelOld)
        {
            bIsSelected = TRUE;
        }
    }

    if(bIsSelected == FALSE)
    {
        return 0;
    }

    HRESULT hr;
    CComPtr<IDMGraphCtrl> spGraph;

    if(FAILED(GetGraphCtrl(&spGraph)))
    {
        return 0;
    }

    CComPtr<IDMGraphCollection> spGraphCollection;
    hr = spGraph->get_Cursors(&spGraphCollection);

    if(FAILED(hr))
    {
        return 0;
    }

    hr = spGraphCollection->put_Selected(pHmLv->iItem);
    UpdateControls(spGraph);
    return 0;
}

LRESULT CCursorPropPage::OnGetdispinfoElement_list(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
    NMLVDISPINFO* pHmLv = (NMLVDISPINFO*)pnmh;

    if(pHmLv->item.mask & LVIF_TEXT)
    {
        HRESULT hr;
        CComPtr<IDMGraphCtrl> spGraph;

        if(FAILED(GetGraphCtrl(&spGraph)))
        {
            return 0;
        }

        CComPtr<IDMGraphCollection> spGraphCollection;
        hr = spGraph->get_Cursors(&spGraphCollection);

        if(FAILED(hr))
        {
            return 0;
        }

        CComPtr<IDispatch> spDispatch;
        CComPtr<IDMGraphCursor> spItem;
        hr = spGraphCollection->get_Item(pHmLv->item.iItem, &spDispatch);

        if(FAILED(hr))
        {
            return hr;
        }

        hr = spDispatch.QueryInterface(&spItem);

        if(FAILED(hr))
        {
            return hr;
        }

        CComBSTR bsName;
        hr = spItem->get_Name(&bsName);

        if(FAILED(hr))
        {
            return hr;
        }

        USES_CONVERSION;
        sprintf_s((char*) pHmLv->item.pszText, pHmLv->item.cchTextMax, "%s", OLE2T(bsName));
    }

    return 0;
}

LRESULT CCursorPropPage::OnChangeEdit_x(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    if(m_bInited == FALSE)
    {
        return 0;    //ignore notificatin messages sent before WM_INITDIALOG
    }

    HRESULT hr;
    CComPtr<IDMGraphCtrl> spGraph;

    if(FAILED(GetGraphCtrl(&spGraph)))
    {
        return 0;
    }

    CComPtr<IDMGraphCursor> spCursor;
    GetSelectedCursor(spGraph, &spCursor);

    if(spCursor == NULL)
    {
        return 0;
    }

    USES_CONVERSION;
    CComBSTR str;
    GetDlgItemText(IDC_EDIT_X, str.m_str);
    hr = spCursor->put_X(atof(OLE2A(str)));
    return 0;
}

LRESULT CCursorPropPage::OnChangeEdit_y(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    if(m_bInited == FALSE)
    {
        return 0;    //ignore notificatin messages sent before WM_INITDIALOG
    }

    HRESULT hr;
    CComPtr<IDMGraphCtrl> spGraph;

    if(FAILED(GetGraphCtrl(&spGraph)))
    {
        return 0;
    }

    CComPtr<IDMGraphCursor> spCursor;
    GetSelectedCursor(spGraph, &spCursor);

    if(spCursor == NULL)
    {
        return 0;
    }

    USES_CONVERSION;
    CComBSTR str;
    GetDlgItemText(IDC_EDIT_Y, str.m_str);
    hr = spCursor->put_Y(atof(OLE2A(str)));
    return 0;
}

LRESULT CCursorPropPage::OnChangeEdit_name(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    if(m_bInited == FALSE)
    {
        return 0;    //ignore notificatin messages sent before WM_INITDIALOG
    }

    HRESULT hr;
    CComPtr<IDMGraphCtrl> spGraph;

    if(FAILED(GetGraphCtrl(&spGraph)))
    {
        return 0;
    }

    CComPtr<IDMGraphCursor> spCursor;
    GetSelectedCursor(spGraph, &spCursor);

    if(spCursor == NULL)
    {
        return 0;
    }

    USES_CONVERSION;
    CComBSTR str;
    GetDlgItemText(IDC_EDIT_NAME, str.m_str);
    hr = spCursor->put_Name(str);
    //this is the reason we use list view control and not listbox control
    //we need to update text for items in list
    ::InvalidateRect(GetDlgItem(IDC_COLLECTION_LIST), NULL, FALSE);
    return 0;
}

LRESULT CCursorPropPage::OnClickedCheck_visible(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    if(m_bInited == FALSE)
    {
        return 0;    //ignore notificatin messages sent before WM_INITDIALOG
    }

    HRESULT hr;
    CComPtr<IDMGraphCtrl> spGraph;

    if(FAILED(GetGraphCtrl(&spGraph)))
    {
        return 0;
    }

    CComPtr<IDMGraphCursor> spCursor;
    GetSelectedCursor(spGraph, &spCursor);

    if(spCursor == NULL)
    {
        return 0;
    }

    BOOL b = IsDlgButtonChecked(IDC_CHECK_VISIBLE) ==  BST_CHECKED;
    hr = spCursor->put_Visible(b ? VARIANT_TRUE : VARIANT_FALSE);
    return 0;
}

LRESULT CCursorPropPage::OnCloseupCombo_style(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    if(m_bInited == FALSE)
    {
        return 0;    //ignore notificatin messages sent before WM_INITDIALOG
    }

    HRESULT hr;
    CComPtr<IDMGraphCtrl> spGraph;

    if(FAILED(GetGraphCtrl(&spGraph)))
    {
        return 0;
    }

    CComPtr<IDMGraphCursor> spCursor;
    GetSelectedCursor(spGraph, &spCursor);

    if(spCursor == NULL)
    {
        return 0;
    }

    Crosshair style = (Crosshair)SendDlgItemMessage(IDC_COMBO_STYLE, CB_GETCURSEL);
    hr = spCursor->put_Style(style);
    return 0;
}

LRESULT CCursorPropPage::OnCloseupCombo_snap(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    if(m_bInited == FALSE)
    {
        return 0;    //ignore notificatin messages sent before WM_INITDIALOG
    }

    HRESULT hr;
    CComPtr<IDMGraphCtrl> spGraph;

    if(FAILED(GetGraphCtrl(&spGraph)))
    {
        return 0;
    }

    CComPtr<IDMGraphCursor> spCursor;
    GetSelectedCursor(spGraph, &spCursor);

    if(spCursor == NULL)
    {
        return 0;
    }

    CursorMode snap = (CursorMode)SendDlgItemMessage(IDC_COMBO_SNAP, CB_GETCURSEL);
    hr = spCursor->put_Mode(snap);
    return 0;
}

STDMETHODIMP CCursorPropPage::Help(LPCOLESTR pszHelpDir)
{
    DmOpenHelp(GetParent(), pszHelpDir);
    return S_OK;
}

////////////////////////////////
// helpers

HRESULT WINAPI GetSelectedCursor(IDMGraphCtrl* pGraphCtrl, IDMGraphCursor** pCursor)
{
    HRESULT hr;
    ATLASSERT(pGraphCtrl);
    ATLASSERT(pCursor);
    *pCursor = NULL;
    CComPtr<IDMGraphCollection> spGraphCollection;
    hr = pGraphCtrl->get_Cursors(&spGraphCollection);
    long selected = 0;
    CComPtr<IDispatch> spItem;
    hr = spGraphCollection->get_Selected(&selected);

    if(selected<0)
    {
        return S_FALSE;
    }

    hr = spGraphCollection->get_Item(selected, &spItem);
    return spItem.QueryInterface(pCursor);
}
