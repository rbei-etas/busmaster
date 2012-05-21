// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//      DMGraphPropPage.cpp: implementation for the CDMGraphPropPage class
//
// CLASS NAME
//      DMGraphPropPage
//
// DESCRIPTION
//
// MODIFICATIONS
//      01-Dec-2011 MSamoila major re-design of old CNTGraphPropPage
//
#include "stdafx.h"
#include "DMGraph.h"
#include "DMGraphPropPage.h"

extern void WINAPI FillCombo(HWND hWndCombo, UINT id, int idx);

/////////////////////////////////////////////////////////////////////////////
// CDMGraphPropPage

LRESULT CDMGraphPropPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    m_btnFrameColPicker.SubclassWindow(GetDlgItem(IDC_FRAME_COLOR));
    m_btnPlotColPicker.SubclassWindow(GetDlgItem(IDC_PLOT_COLOR));
    m_btnAxisColPicker.SubclassWindow(GetDlgItem(IDC_AXIS_COLOR));
    m_btnGridColPicker.SubclassWindow(GetDlgItem(IDC_GRID_COLOR));
    m_btnLabelColPicker.SubclassWindow(GetDlgItem(IDC_LABEL_COLOR));
    FillCombo(GetDlgItem(IDC_COMBO_MODE), IDS_GRAPH_TRACK_MODE, -1);
    FillCombo(GetDlgItem(IDC_COMBO_FRAME), IDS_GRAPH_FRAME, -1);
    CComPtr<IDMGraphCtrl> spGraph;

    if(FAILED(GetGraphCtrl(&spGraph)))
    {
        return 0;
    }

    UpdateControls(spGraph);
    m_bInited = TRUE;
    return 0;
}

void CDMGraphPropPage::UpdateControls(IDMGraphCtrl* pGraphCtrl)
{
    ATLASSERT(pGraphCtrl);
    HRESULT hr;
    CComBSTR caption;
    hr = pGraphCtrl->get_Caption(&caption);
    ::SetDlgItemTextW (m_hWnd, IDC_CAPTION, caption);
    OLE_COLOR color;
    COLORREF col;
    hr = pGraphCtrl->get_AxisColor(&color);
    hr = OleTranslateColor(color, NULL, &col);
    m_btnAxisColPicker.SetColor(color);
    hr = pGraphCtrl->get_ControlFrameColor(&color);
    hr = OleTranslateColor(color, NULL, &col);
    m_btnFrameColPicker.SetColor(color);
    hr = pGraphCtrl->get_GridColor(&color);
    hr = OleTranslateColor(color, NULL, &col);
    m_btnGridColPicker.SetColor(color);
    hr = pGraphCtrl->get_PlotAreaColor(&color);
    hr = OleTranslateColor(color, NULL, &col);
    m_btnPlotColPicker.SetColor(color);
    hr = pGraphCtrl->get_LabelColor(&color);
    hr = OleTranslateColor(color, NULL, &col);
    m_btnLabelColPicker.SetColor(color);
    FrameStyle frame;
    hr = pGraphCtrl->get_FrameStyle(&frame);
    SendDlgItemMessage(IDC_COMBO_FRAME, CB_SETCURSEL, frame);
    TrackModeState mode;
    hr = pGraphCtrl->get_TrackMode(&mode);
    SendDlgItemMessage(IDC_COMBO_MODE, CB_SETCURSEL, mode);
    VARIANT_BOOL bXLog, bYLog;
    VARIANT_BOOL gridX, gridY;
    CComPtr<IDMGraphAxis> spAxisX, spAxisY;
    hr = pGraphCtrl->get_Axis( HorizontalX, &spAxisX);
    hr = pGraphCtrl->get_Axis( VerticalY ,  &spAxisY);
    hr = spAxisX->get_Log(&bXLog);
    hr = spAxisY->get_Log(&bYLog);
    hr = spAxisX->get_ShowGrid(&gridX);
    hr = spAxisY->get_ShowGrid(&gridY);
    CheckDlgButton(IDC_CHECK_XLOG, bXLog ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(IDC_CHECK_YLOG, bYLog ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(IDC_CHECK_SHOWGRID_X, gridX ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(IDC_CHECK_SHOWGRID_Y, gridY ? BST_CHECKED : BST_UNCHECKED);
}

LRESULT CDMGraphPropPage::OnColChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

    OLE_COLOR color = (OLE_COLOR)wParam;

    switch(lParam)
    {
        case IDC_FRAME_COLOR:
            hr = spGraph->put_ControlFrameColor(color);
            break;

        case IDC_PLOT_COLOR:
            hr = spGraph->put_PlotAreaColor(color);
            break;

        case IDC_AXIS_COLOR:
            hr = spGraph->put_AxisColor(color);
            break;

        case IDC_GRID_COLOR:
            hr = spGraph->put_GridColor(color);
            break;

        case IDC_LABEL_COLOR:
            hr = spGraph->put_LabelColor(color);
            break;

        default:
            ATLASSERT(FALSE);
    }

    return 0;
}

LRESULT CDMGraphPropPage::OnChangeCaption(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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

    CComBSTR str;
    GetDlgItemText(IDC_CAPTION, str.m_str);
    hr = spGraph->put_Caption(str);
    return 0;
}

LRESULT CDMGraphPropPage::OnClickedCheck_showgridX(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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

    BOOL b = IsDlgButtonChecked(IDC_CHECK_SHOWGRID_X) ==  BST_CHECKED;
    CComPtr<IDMGraphAxis> spAxisX;
    hr = spGraph->get_Axis( HorizontalX, &spAxisX);
    hr = spAxisX->put_ShowGrid(b ? VARIANT_TRUE : VARIANT_FALSE);
    return 0;
}

LRESULT CDMGraphPropPage::OnClickedCheck_showgridY(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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

    BOOL b = IsDlgButtonChecked(IDC_CHECK_SHOWGRID_Y) ==  BST_CHECKED;
    CComPtr<IDMGraphAxis> spAxisY;
    hr = spGraph->get_Axis( VerticalY, &spAxisY);
    hr = spAxisY->put_ShowGrid(b ? VARIANT_TRUE : VARIANT_FALSE);
    return 0;
}

LRESULT CDMGraphPropPage::OnClickedCheck_xlog(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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

    BOOL b = IsDlgButtonChecked(IDC_CHECK_XLOG) ==  BST_CHECKED;
    CComPtr<IDMGraphAxis> spAxisX;
    hr = spGraph->get_Axis( HorizontalX, &spAxisX);
    hr = spAxisX->put_Log(b ? VARIANT_TRUE : VARIANT_FALSE);
    return 0;
}

LRESULT CDMGraphPropPage::OnClickedCheck_ylog(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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

    BOOL b = IsDlgButtonChecked(IDC_CHECK_YLOG) ==  BST_CHECKED;
    CComPtr<IDMGraphAxis> spAxisY;
    hr = spGraph->get_Axis( VerticalY ,  &spAxisY);
    hr = spAxisY->put_Log(b ? VARIANT_TRUE : VARIANT_FALSE);
    return 0;
}

LRESULT CDMGraphPropPage::OnCloseupCombo_frame(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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

    int i = SendDlgItemMessage(IDC_COMBO_FRAME, CB_GETCURSEL);
    hr = spGraph->put_FrameStyle((FrameStyle)i);
    return 0;
}

LRESULT CDMGraphPropPage::OnCloseupCombo_mode(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
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

    int i = SendDlgItemMessage(IDC_COMBO_MODE, CB_GETCURSEL);
    hr = spGraph->put_TrackMode((TrackModeState)i);
    return 0;
}

STDMETHODIMP CDMGraphPropPage::Help(LPCOLESTR pszHelpDir)
{
    DmOpenHelp(GetParent(), pszHelpDir);
    return S_OK;
}
