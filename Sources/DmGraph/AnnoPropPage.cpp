// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//      AnnoPropPage.cpp: implementation for the CAnnoPropPage class
//
// CLASS NAME
//      CAnnoPropPage
//
// DESCRIPTION
//
// MODIFICATIONS
//      01-Dec-2011 MSamoila major re-design of old CAnnoPropPage
//
#include "stdafx.h"
#include "DMGraph.h"
#include "AnnoPropPage.h"

static HRESULT WINAPI GetSelectedAnnotation(IDMGraphCtrl* pGraphCtrl, IDMGraphAnnotation** pAnno);
extern void WINAPI FillCombo(HWND hWndCombo, UINT id, int idx);

/////////////////////////////////////////////////////////////////////////////
// CAnnoPropPage

LRESULT CAnnoPropPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    m_btnLabelTxtColPicker.SubclassWindow(GetDlgItem(IDC_LABEL_COLOR));
    m_btnLabelBkColPicker.SubclassWindow(GetDlgItem(IDC_LABEL_BGCOLOR));
    FillCombo(GetDlgItem(IDC_COMBO_ORIENTATION), IDS_ANNO_ORIENTATION, -1);
    HRESULT hr;
    CComPtr<IDMGraphCtrl> spGraph;

    if(FAILED(GetGraphCtrl(&spGraph)))
    {
        return 0;
    }

    CComPtr<IDMGraphCollection> spGraphCollection;
    hr = spGraph->get_Annotations(&spGraphCollection);
    InitCollectionList(spGraphCollection);
    //Update controls
    UpdateControls(spGraph);
    m_bInited = TRUE;
    return 1;
}

void CAnnoPropPage::UpdateControls(IDMGraphCtrl* pGraphCtrl)
{
    ATLASSERT(pGraphCtrl);
    HRESULT hr;
    CComPtr<IDMGraphAnnotation> spAnnotation;
    GetSelectedAnnotation(pGraphCtrl, &spAnnotation);
    BOOL bEnabled = spAnnotation != NULL;
    const UINT idCtrls[] = { IDC_EDIT_CAPTION, IDC_EDIT_X, IDC_EDIT_Y, IDC_LABEL_COLOR, IDC_LABEL_BGCOLOR,
                             IDC_CHECK_VISIBLE, IDC_COMBO_ORIENTATION,
                             IDC_TXT_A1, IDC_TXT_A2, IDC_TXT_A3, IDC_TXT_A4, IDC_TXT_A5, 0
                           };
    int i;

    for(i=0; idCtrls[i]; ::EnableWindow(GetDlgItem(idCtrls[i++]), bEnabled));

    const int TXT_LEN = 128;
    TCHAR szText[ TXT_LEN ];
    CComBSTR caption;

    if(spAnnotation)
    {
        hr = spAnnotation->get_LabelCaption(&caption);
    }

    ::SetDlgItemTextW(m_hWnd, IDC_EDIT_CAPTION, caption);
    double x = 0., y = 0.;

    if(spAnnotation)
    {
        hr = spAnnotation->get_LabelX(&x);
        hr = spAnnotation->get_LabelY(&y);
    }

    sprintf_s((char*) szText, sizeof(szText), "%g", x);
    SetDlgItemText (IDC_EDIT_X, szText);
    sprintf_s((char*) szText, sizeof(szText), "%g", y);
    SetDlgItemText (IDC_EDIT_Y, szText);

    if(spAnnotation)
    {
        OLE_COLOR color;
        COLORREF col;
        hr = spAnnotation->get_LabelColor(&color);
        hr = OleTranslateColor(color, NULL, &col);
        m_btnLabelTxtColPicker.SetColor(col);
        hr = spAnnotation->get_LabelBkColor(&color);
        hr = OleTranslateColor(color, NULL, &col);
        m_btnLabelBkColPicker.SetColor(col);
        VARIANT_BOOL orientation;
        hr = spAnnotation->get_LabelHorizontal(&orientation);
        SendDlgItemMessage(IDC_COMBO_ORIENTATION, CB_SETCURSEL, orientation ? 1 : 0);
        VARIANT_BOOL visible;
        hr = spAnnotation->get_Visible(&visible);
        CheckDlgButton(IDC_CHECK_VISIBLE, visible ? BST_CHECKED : BST_UNCHECKED);
    }
}

LRESULT CAnnoPropPage::OnColChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

    CComPtr<IDMGraphAnnotation> spAnnotation;
    GetSelectedAnnotation(spGraph, &spAnnotation);

    if(spAnnotation == NULL)
    {
        return 0;
    }

    OLE_COLOR color = (OLE_COLOR)wParam;

    switch(lParam)
    {
        case IDC_LABEL_COLOR:
            hr = spAnnotation->put_LabelColor(color);
            break;

        case IDC_LABEL_BGCOLOR:
            hr = spAnnotation->put_LabelBkColor(color);
            break;

        default:
            ATLASSERT(FALSE);
    }

    return 0;
}

LRESULT CAnnoPropPage::OnClickedAddbutton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    ATLTRACE(_T("OnClickedAddbutton\n"));
    HRESULT hr;
    CComPtr<IDMGraphCtrl> spGraph;

    if(FAILED(GetGraphCtrl(&spGraph)))
    {
        return 0;
    }

    CComPtr<IDMGraphCollection> spGraphCollection;
    hr = spGraph->get_Annotations(&spGraphCollection);
    AppendNewItem(spGraphCollection);
    UpdateControls(spGraph);
    return 0;
}

LRESULT CAnnoPropPage::OnClickedDelbutton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    ATLTRACE(_T("OnClickedDelbutton\n"));
    HRESULT hr;
    CComPtr<IDMGraphCtrl> spGraph;

    if(FAILED(GetGraphCtrl(&spGraph)))
    {
        return 0;
    }

    CComPtr<IDMGraphCollection> spGraphCollection;
    hr = spGraph->get_Annotations(&spGraphCollection);
    DeleteItem(spGraphCollection);
    UpdateControls(spGraph);
    return 0;
}

LRESULT CAnnoPropPage::OnClickedRemoveSelbutton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    HRESULT hr;
    CComPtr<IDMGraphCtrl> spGraph;

    if(FAILED(GetGraphCtrl(&spGraph)))
    {
        return 0;
    }

    CComPtr<IDMGraphCollection> spGraphCollection;
    hr = spGraph->get_Annotations(&spGraphCollection);
    RemoveSelection(spGraphCollection);
    UpdateControls(spGraph);
    return 0;
}

LRESULT CAnnoPropPage::OnItemchangedElement_list(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
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
    hr = spGraph->get_Annotations(&spGraphCollection);

    if(FAILED(hr))
    {
        return 0;
    }

    hr = spGraphCollection->put_Selected(pHmLv->iItem);
    UpdateControls(spGraph);
    return 0;
}

LRESULT CAnnoPropPage::OnGetdispinfoElement_list(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
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
        hr = spGraph->get_Annotations(&spGraphCollection);

        if(FAILED(hr))
        {
            return 0;
        }

        CComPtr<IDispatch> spDispatch;
        CComPtr<IDMGraphAnnotation> spItem;
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
        hr = spItem->get_LabelCaption(&bsName);

        if(FAILED(hr))
        {
            return hr;
        }

        USES_CONVERSION;
        sprintf_s((char*) pHmLv->item.pszText, pHmLv->item.cchTextMax, "%s", OLE2T(bsName));
    }

    return 0;
}

LRESULT CAnnoPropPage::OnChangeEdit_caption(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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

    CComPtr<IDMGraphAnnotation> spAnnotation;
    GetSelectedAnnotation(spGraph, &spAnnotation);

    if(spAnnotation == NULL)
    {
        return 0;
    }

    CComBSTR str;
    GetDlgItemText(IDC_EDIT_CAPTION, str.m_str);
    hr = spAnnotation->put_LabelCaption(str);
    //this is the reason we use list view control and not listbox control
    //we need to update text for items in list
    ::InvalidateRect(GetDlgItem(IDC_COLLECTION_LIST), NULL, FALSE);
    return 0;
}

LRESULT CAnnoPropPage::OnChangeEdit_x(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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

    CComPtr<IDMGraphAnnotation> spAnnotation;
    GetSelectedAnnotation(spGraph, &spAnnotation);

    if(spAnnotation == NULL)
    {
        return 0;
    }

    USES_CONVERSION;
    CComBSTR str;
    GetDlgItemText(IDC_EDIT_X, str.m_str);
    hr = spAnnotation->put_LabelX(atof(OLE2A(str)));
    return 0;
}

LRESULT CAnnoPropPage::OnChangeEdit_y(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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

    CComPtr<IDMGraphAnnotation> spAnnotation;
    GetSelectedAnnotation(spGraph, &spAnnotation);

    if(spAnnotation == NULL)
    {
        return 0;
    }

    USES_CONVERSION;
    CComBSTR str;
    GetDlgItemText(IDC_EDIT_Y, str.m_str);
    hr = spAnnotation->put_LabelY(atof(OLE2A(str)));
    return 0;
}

LRESULT CAnnoPropPage::OnCloseupCombo_orientation(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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

    CComPtr<IDMGraphAnnotation> spAnnotation;
    GetSelectedAnnotation(spGraph, &spAnnotation);

    if(spAnnotation == NULL)
    {
        return 0;
    }

    int horizontal = SendDlgItemMessage(IDC_COMBO_ORIENTATION, CB_SETCURSEL);
    hr = spAnnotation->put_LabelHorizontal(horizontal ? VARIANT_TRUE : VARIANT_FALSE);
    return 0;
}

LRESULT CAnnoPropPage::OnClickedCheck_visible(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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

    CComPtr<IDMGraphAnnotation> spAnnotation;
    GetSelectedAnnotation(spGraph, &spAnnotation);

    if(spAnnotation == NULL)
    {
        return 0;
    }

    BOOL b = IsDlgButtonChecked(IDC_CHECK_VISIBLE) ==  BST_CHECKED;
    hr = spAnnotation->put_Visible(b ? VARIANT_TRUE : VARIANT_FALSE);
    return 0;
}

STDMETHODIMP CAnnoPropPage::Help(LPCOLESTR pszHelpDir)
{
    DmOpenHelp(GetParent(), pszHelpDir);
    return S_OK;
}

////////////////////////////////
// helpers

HRESULT WINAPI GetSelectedAnnotation(IDMGraphCtrl* pGraphCtrl, IDMGraphAnnotation** pAnno)
{
    HRESULT hr;
    ATLASSERT(pGraphCtrl);
    ATLASSERT(pAnno);
    *pAnno = NULL;
    CComPtr<IDMGraphCollection> spGraphCollection;
    hr = pGraphCtrl->get_Annotations(&spGraphCollection);
    long selected = 0;
    CComPtr<IDispatch> spItem;
    hr = spGraphCollection->get_Selected(&selected);

    if(selected<0)
    {
        return S_FALSE;
    }

    hr = spGraphCollection->get_Item(selected, &spItem);
    return spItem.QueryInterface(pAnno);
}
