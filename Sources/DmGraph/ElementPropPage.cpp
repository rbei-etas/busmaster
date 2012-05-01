// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//      ElementPropPage.cpp: implementation for the CElementPropPage class
//
// CLASS NAME
//      CElementPropPage
//
// DESCRIPTION
//
// MODIFICATIONS
//      01-Dec-2011 MSamoila major re-design of old CElementPropPage
//
#include "stdafx.h"
#include "DMGraph.h"
#include "ElementPropPage.h"

extern void WINAPI FillCombo(HWND hWndCombo, UINT id, int idx);

/////////////////////////////////////////////////////////////////////////////
// CElementPropPage

HRESULT CElementPropPage::GetCurrentElement(IDMGraphCtrl* pGraphCtrl, IDMGraphElement** ppGraphElement)
{
    ATLASSERT(pGraphCtrl);
    CComPtr<IDMGraphCollection> spElements;
    long nSelected = 0;
    HRESULT hr;
    hr = pGraphCtrl->get_Elements(&spElements);

    if(FAILED(hr))
    {
        return hr;
    }

    hr = spElements->get_Selected(&nSelected);

    if(FAILED(hr))
    {
        return hr;
    }

    if(nSelected <0 )
    {
        return S_FALSE;
    }

    CComPtr<IDispatch> spDispatch;
    hr = spElements->get_Item(nSelected, &spDispatch);

    if(FAILED(hr))
    {
        return hr;
    }

    return spDispatch.QueryInterface(ppGraphElement);
}


LRESULT CElementPropPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    m_btnLineColPicker.SubclassWindow(GetDlgItem(IDC_LINE_COLOR));
    m_btnPointColPicker.SubclassWindow(GetDlgItem(IDC_POINT_COLOR));
    FillCombo(GetDlgItem(IDC_COMBO_TYPE), IDS_ELEM_LINE_TYPE, -1);
    FillCombo(GetDlgItem(IDC_COMBO_SYMBOL), IDS_ELEM_POINT_TYPE, -1);
    SendDlgItemMessage(IDC_SPIN_LINE_WIDTH, UDM_SETRANGE, 0, MAKELONG(5, 0));
    SendDlgItemMessage(IDC_SPIN_POINT_WIDTH, UDM_SETRANGE, 0, MAKELONG(5, 0));
    CComPtr<IDMGraphCtrl> spGraph;

    if(FAILED(GetGraphCtrl(&spGraph)))
    {
        return 0;
    }

    HRESULT hr;
    CComPtr<IDMGraphCollection> spGraphCollection;
    long count = 0, nSelected = 0;
    hr = spGraph->get_Elements(&spGraphCollection);

    if(FAILED(hr))
    {
        return hr;
    }

    InitCollectionList(spGraphCollection);
    //Update controls
    UpdateControls(spGraph);
    m_bInited = TRUE;
    return 1;
}

void CElementPropPage::UpdateControls(IDMGraphCtrl* pGraphCtrl)
{
    ATLASSERT(pGraphCtrl);
    HRESULT hr;
    CComPtr<IDMGraphElement> spElement;
    hr = GetCurrentElement(pGraphCtrl, &spElement);
    BOOL bEnabled = spElement != NULL;
    const UINT idCtrls[] = { IDC_LINE_COLOR, IDC_POINT_COLOR, IDC_LINE_WIDTH, IDC_POINT_WIDTH, IDC_COMBO_TYPE, IDC_COMBO_SYMBOL,
                             IDC_CHECK_VISIBLE, IDC_CHECK_SOLID, IDC_EDIT_NAME, IDC_DELBUTTON,
                             IDC_TXT_E1, IDC_TXT_E2, IDC_TXT_E3, IDC_TXT_E4, IDC_TXT_E5, IDC_TXT_E6, IDC_TXT_E7, IDC_TXT_E8, 0
                           };
    int i;

    for(i=0; idCtrls[i]; ::EnableWindow(GetDlgItem(idCtrls[i++]), bEnabled));

    if(bEnabled == FALSE)
    {
        return;
    }

    OLE_COLOR color;
    COLORREF col;
    hr = spElement->get_LineColor(&color);
    hr = OleTranslateColor(color, NULL, &col);
    m_btnLineColPicker.SetColor(col);
    hr = spElement->get_PointColor(&color);
    hr = OleTranslateColor(color, NULL, &col);
    m_btnPointColPicker.SetColor(col);
    long width;
    hr = spElement->get_LineWidth(&width);
    SetDlgItemInt(IDC_LINE_WIDTH,width);
    hr = spElement->get_PointSize(&width);
    SetDlgItemInt(IDC_POINT_WIDTH,width);
    LineType type;
    hr = spElement->get_Linetype(&type);
    SendDlgItemMessage(IDC_COMBO_TYPE, CB_SETCURSEL, type);
    SymbolType symbol;
    hr = spElement->get_PointSymbol(&symbol);
    SendDlgItemMessage(IDC_COMBO_SYMBOL, CB_SETCURSEL, symbol);
    VARIANT_BOOL visible;
    hr = spElement->get_Show(&visible);
    CheckDlgButton(IDC_CHECK_VISIBLE, visible ? BST_CHECKED : BST_UNCHECKED);
    VARIANT_BOOL solid;
    hr = spElement->get_SolidPoint(&solid);
    CheckDlgButton(IDC_CHECK_SOLID, visible ? BST_CHECKED : BST_UNCHECKED);
    CComBSTR ids;
    hr = spElement->get_Name(&ids);
    ::SetDlgItemTextW(m_hWnd, IDC_EDIT_NAME, ids);
}

LRESULT CElementPropPage::OnColChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    HRESULT hr;

    if(m_bInited == FALSE)
    {
        return 0;    //ignore notificatin messages sent before WM_INITDIALOG
    }

    CComPtr<IDMGraphCtrl> spGraph;

    if(FAILED(GetGraphCtrl(&spGraph)))
    {
        return 0;
    }

    CComPtr<IDMGraphElement> spElement;
    hr = GetCurrentElement(spGraph, &spElement);

    if(FAILED(hr) || spElement == NULL)
    {
        return 0;
    }

    OLE_COLOR color = (OLE_COLOR)wParam;

    switch(lParam)
    {
        case IDC_LINE_COLOR:
            hr = spElement->put_LineColor(color);
            break;

        case IDC_POINT_COLOR:
            hr = spElement->put_PointColor(color);
            break;

        default:
            ATLASSERT(FALSE);
    }

    return 0;
}

LRESULT CElementPropPage::OnClickedAddbutton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    ATLTRACE(_T("OnClickedAddbutton\n"));
    HRESULT hr;
    CComPtr<IDMGraphCtrl> spGraph;

    if(FAILED(GetGraphCtrl(&spGraph)))
    {
        return 0;
    }

    CComPtr<IDMGraphCollection> spGraphCollection;
    hr = spGraph->get_Elements(&spGraphCollection);

    if(FAILED(hr))
    {
        return hr;
    }

    AppendNewItem(spGraphCollection);
    UpdateControls(spGraph);
    return 0;
}

LRESULT CElementPropPage::OnClickedDelbutton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    ATLTRACE(_T("OnClickedDelbutton\n"));
    HRESULT hr;
    CComPtr<IDMGraphCtrl> spGraph;

    if(FAILED(GetGraphCtrl(&spGraph)))
    {
        return 0;
    }

    CComPtr<IDMGraphCollection> spGraphCollection;
    hr = spGraph->get_Elements(&spGraphCollection);

    if(FAILED(hr))
    {
        return 0;
    }

    DeleteItem(spGraphCollection);
    UpdateControls(spGraph);
    return 0;
}

LRESULT CElementPropPage::OnClickedRemoveSelbutton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    HRESULT hr;
    CComPtr<IDMGraphCtrl> spGraph;

    if(FAILED(GetGraphCtrl(&spGraph)))
    {
        return 0;
    }

    CComPtr<IDMGraphCollection> spGraphCollection;
    hr = spGraph->get_Elements(&spGraphCollection);
    RemoveSelection(spGraphCollection);
    UpdateControls(spGraph);
    return 0;
}

LRESULT CElementPropPage::OnItemchangedElement_list(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
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
    hr = spGraph->get_Elements(&spGraphCollection);

    if(FAILED(hr))
    {
        return 0;
    }

    hr = spGraphCollection->put_Selected(pHmLv->iItem);
    UpdateControls(spGraph);
    return 0;
}

LRESULT CElementPropPage::OnGetdispinfoElement_list(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
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
        hr = spGraph->get_Elements(&spGraphCollection);

        if(FAILED(hr))
        {
            return 0;
        }

        CComPtr<IDispatch> spDispatch;
        CComPtr<IDMGraphElement> spItem;
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

LRESULT CElementPropPage::OnChangeLine_width(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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

    CComPtr<IDMGraphElement> spElement;
    hr = GetCurrentElement(spGraph, &spElement);

    if(FAILED(hr) || spElement == NULL)
    {
        return 0;
    }

    short width = GetDlgItemInt(IDC_LINE_WIDTH);
    hr = spElement->put_LineWidth(width);
    return 0;
}

LRESULT CElementPropPage::OnDeltaposSpin_line_width(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
    OnChangeLine_width(0, 0, NULL, bHandled);
    return 0;
}

LRESULT CElementPropPage::OnChangePoint_width(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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

    CComPtr<IDMGraphElement> spElement;
    hr = GetCurrentElement(spGraph, &spElement);

    if(FAILED(hr) || spElement == NULL)
    {
        return 0;
    }

    short width = GetDlgItemInt(IDC_POINT_WIDTH);
    hr = spElement->put_PointSize(width);
    return 0;
}

LRESULT CElementPropPage::OnDeltaposSpin_point_width(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
    OnChangePoint_width(0, 0, NULL, bHandled);
    return 0;
}

LRESULT CElementPropPage::OnClickedCheck_solid(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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

    CComPtr<IDMGraphElement> spElement;
    hr = GetCurrentElement(spGraph, &spElement);

    if(FAILED(hr) || spElement == NULL)
    {
        return 0;
    }

    BOOL b = IsDlgButtonChecked(IDC_CHECK_SOLID) ==  BST_CHECKED;
    hr = spElement->put_SolidPoint(b ? VARIANT_TRUE : VARIANT_FALSE);
    return 0;
}

LRESULT CElementPropPage::OnClickedCheck_visible(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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

    CComPtr<IDMGraphElement> spElement;
    hr = GetCurrentElement(spGraph, &spElement);

    if(FAILED(hr) || spElement == NULL)
    {
        return 0;
    }

    BOOL b = IsDlgButtonChecked(IDC_CHECK_VISIBLE) ==  BST_CHECKED;
    hr = spElement->put_Show(b ? VARIANT_TRUE : VARIANT_FALSE);
    return 0;
}

LRESULT CElementPropPage::OnCloseupCombo_symbol(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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

    CComPtr<IDMGraphElement> spElement;
    hr = GetCurrentElement(spGraph, &spElement);

    if(FAILED(hr) || spElement == NULL)
    {
        return 0;
    }

    int symbol = SendDlgItemMessage(IDC_COMBO_SYMBOL, CB_GETCURSEL);
    hr = spElement->put_PointSymbol((SymbolType)symbol);
    return 0;
}

LRESULT CElementPropPage::OnCloseupCombo_type(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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

    CComPtr<IDMGraphElement> spElement;
    hr = GetCurrentElement(spGraph, &spElement);

    if(FAILED(hr) || spElement == NULL)
    {
        return 0;
    }

    int type = SendDlgItemMessage(IDC_COMBO_TYPE, CB_GETCURSEL);
    hr = spElement->put_Linetype((LineType)type);
    return 0;
}

LRESULT CElementPropPage::OnChangeEdit_name(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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

    CComPtr<IDMGraphElement> spElement;
    hr = GetCurrentElement(spGraph, &spElement);

    if(FAILED(hr) || spElement == NULL)
    {
        return 0;
    }

    CComBSTR str;
    GetDlgItemText(IDC_EDIT_NAME, str.m_str);
    hr = spElement->put_Name(str);
    //this is the reason we use list view control and not listbox control
    //we need to update text for items in list
    ::InvalidateRect(GetDlgItem(IDC_COLLECTION_LIST), NULL, FALSE);
    return 0;
}

STDMETHODIMP CElementPropPage::Help(LPCOLESTR pszHelpDir)
{
    DmOpenHelp(GetParent(), pszHelpDir);
    return S_OK;
}
