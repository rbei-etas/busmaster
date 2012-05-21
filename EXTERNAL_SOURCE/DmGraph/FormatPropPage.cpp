// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//		FormatPropPage.cpp: implementation for the CFormatPropPage class
//
// CLASS NAME
//		FormatPropPage
//
// DESCRIPTION
// 
// MODIFICATIONS
//		01-Dec-2011 MSamoila major re-design of old CFormatPropPage
//
#include "stdafx.h"
#include "DMGraph.h"
#include "FormatPropPage.h"

extern void WINAPI FillCombo(HWND hWndCombo, UINT id, int idx);

/////////////////////////////////////////////////////////////////////////////
// CFormatPropPage

LRESULT CFormatPropPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	FillCombo(GetDlgItem(IDC_COMBO_AXIS), IDS_FORMAT_AXIS, 0);
	FillCombo(GetDlgItem(IDC_COMBO_TYPE), IDS_FORMAT_TYPE, -1);	

	m_bInited = TRUE;

	CComPtr<IDMGraphCtrl> spGraph;
	if(FAILED(GetGraphCtrl(&spGraph)))
		return 0;

	//Update controls
	UpdateControls(spGraph);
	return 0;
}

void CFormatPropPage::UpdateControls(IDMGraphCtrl* pGraphCtrl)
{
	ATLASSERT(pGraphCtrl);

	HRESULT hr;

	CComBSTR str;
	VARIANT_BOOL time;

	CComPtr<IDMGraphAxis> spAxis; 
	hr = GetSelectedAxis(pGraphCtrl, &spAxis);
	if(FAILED(hr) || spAxis == NULL)
		return;

	hr = spAxis->get_Time(&time);
	hr = spAxis->get_Format(&str);

	int idxType = SendDlgItemMessage(IDC_COMBO_TYPE, CB_GETCURSEL);
	if(time && idxType < 3)
	{	//if this axis is a date/time axis but the format is not a date/time then then adjust the type as well
		SendDlgItemMessage(IDC_COMBO_TYPE, CB_SETCURSEL, 3);
		BOOL bHandled;
		OnSelChangeCombo_type(0, 0, NULL, bHandled);
	}
	if(time == VARIANT_FALSE && idxType >= 3)
	{	//if this axis is NOT a date/time axis but the format is a date/time then adjust the type as well
		SendDlgItemMessage(IDC_COMBO_TYPE, CB_SETCURSEL, 0);
		BOOL bHandled;
		OnSelChangeCombo_type(0, 0, NULL, bHandled);
	}


	::SetDlgItemTextW(m_hWnd, IDC_EDIT_FORMAT, str);
}

HRESULT CFormatPropPage::GetSelectedAxis(IDMGraphCtrl* pGraphCtrl, IDMGraphAxis** ppAxis)
{
	ATLASSERT(ppAxis);
	*ppAxis = NULL;
	int axis = SendDlgItemMessage(IDC_COMBO_AXIS, CB_GETCURSEL);
	if(axis == CB_ERR)
		return E_UNEXPECTED;
	
	ATLASSERT(axis == HorizontalX || axis == VerticalY);
	return pGraphCtrl->get_Axis( (Orientation)axis, ppAxis);
}


LRESULT CFormatPropPage::OnChangeEdit_format(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(m_bInited == FALSE)
		return 0;//ignore notificatin messages sent before WM_INITDIALOG

	HRESULT hr;
	CComPtr<IDMGraphCtrl> spGraph;
	if(FAILED(GetGraphCtrl(&spGraph)))
		return 0;

	CComBSTR str ;
	GetDlgItemText(IDC_EDIT_FORMAT, str.m_str);
	
	CComPtr<IDMGraphAxis> spAxis; 
	hr = GetSelectedAxis(spGraph, &spAxis);
	if(FAILED(hr) || spAxis == NULL)
		return 0;

	hr = spAxis->put_Format(str);
	return 0;
}

LRESULT CFormatPropPage::OnClickedButton_default(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CComPtr<IDMGraphCtrl> spGraph;
	if(FAILED(GetGraphCtrl(&spGraph)))
		return 0;

	SetDlgItemText(IDC_EDIT_FORMAT, _T("%g"));
	SendDlgItemMessage(IDC_COMBO_TYPE, CB_SETCURSEL, 0);
	OnSelChangeCombo_type(0, 0, NULL, bHandled);

	UpdateControls(spGraph);
	return 0;
}
LRESULT CFormatPropPage::OnSelChangeCombo_type(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(m_bInited == FALSE)
		return 0;//ignore notificatin messages sent before WM_INITDIALOG

	int type = SendDlgItemMessage(IDC_COMBO_TYPE, CB_GETCURSEL);
	if(type == CB_ERR)
		return 0;
	LPCTSTR szFmtTypes0[] = 
		{
			_T("."),
			_T(".#"),
			_T(".##"),
			_T(".###"),
			_T(".####"),
			NULL
		};
	LPCTSTR szFmtTypes1[] = 
		{
			_T("e"),
			_T(".#e"),
			_T(".##e"),
			_T(".###e"),
			_T(".####e"),
			_T("E"),
			_T(".#E"),
			_T(".##E"),
			_T(".###E"),			
			_T(".####E"),			
			NULL
		};
	LPCTSTR szFmtTypes2[] = 
		{
			_T("V"),
			_T("A"),		
			_T("Hz"),		
			_T("g"),		
			_T("Deg"),		
			_T(". %"),		
			_T(".# %"),		
			_T(".## %"),		
			_T("$ ."),		
			_T("$ .#"),		
			_T("$ .##"),		
			NULL
		};
	LPCTSTR szFmtTypes3[] = 
		{
			_T("ddd/mmm/yyy"),
			_T("dd/mm/yy"),
			_T("d/m/y"),
			_T("m/y"),
			_T("d/m"),
			NULL
		};
	LPCTSTR szFmtTypes4[] = 
		{
			_T("h:m:s"),
			_T("hh:m"),
			_T("h:m"),
			_T("m:s"),
			NULL
		};

	LPCTSTR* szFmtTypes[] = { szFmtTypes0, szFmtTypes1, szFmtTypes2, szFmtTypes3, szFmtTypes4 };
	VARIANT_BOOL time[] = { VARIANT_FALSE, VARIANT_FALSE, VARIANT_FALSE, VARIANT_TRUE, VARIANT_TRUE };

	SetAxisTimeFormat( time[ type ] );

	SendDlgItemMessage(IDC_FORMAT_LIST, LB_RESETCONTENT);
	int i;
	for(i=0; szFmtTypes[type][i]; i++)
	{
		SendDlgItemMessage(IDC_FORMAT_LIST, LB_ADDSTRING, 0, (LPARAM)szFmtTypes[type][i]);
	}

	return 0;
}
LRESULT CFormatPropPage::OnSelChangeCombo_axis(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(m_bInited == FALSE)
		return 0;//ignore notificatin messages sent before WM_INITDIALOG

	CComPtr<IDMGraphCtrl> spGraph;
	if(FAILED(GetGraphCtrl(&spGraph)))
		return 0;

	UpdateControls(spGraph);
	return 0;
}
LRESULT CFormatPropPage::OnSelchangeFormat_list(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(m_bInited == FALSE)
		return 0;//ignore notificatin messages sent before WM_INITDIALOG

	CComBSTR str;
	int type = SendDlgItemMessage(IDC_COMBO_TYPE, CB_GETCURSEL);
	int idx = SendDlgItemMessage(IDC_FORMAT_LIST, LB_GETCARETINDEX);

	LPCTSTR szFmt0[] = 
	{
		_T("%g"),
		_T("%.1f"),
		_T("%.2f"),
		_T("%.3f"),
		_T("%.4f"),
		NULL
	};
	LPCTSTR szFmt1[] = 
	{
		_T("%.e"),
		_T("%.1e"),
		_T("%.2e"),	
		_T("%.3e"),		
		_T("%.4e"),		
		_T("%.E"),		
		_T("%.1E"),		
		_T("%.2E"),		
		_T("%.3E"),		
		_T("%.4E"),		
		NULL
	};
	LPCTSTR szFmt2[] = 
	{
		_T("%g V"),
		_T("%g A"),
		_T("%g Hz"),
		_T("%g g"),
		_T("%g Deg"),
		_T("%g %%"),
		_T("%.1f %%"),
		_T("%.2f %%"),
		_T("$ %g"),
		_T("$ %.1f"),
		_T("$ %.2f"),
		NULL
	};
	LPCTSTR szFmt3[] = 
	{
		_T("%d %a/%B/%Y"),
		_T("%d/%b/%Y"),
		_T("%d/%m/%Y"),
		_T("%b/%Y"),
		_T("%d/%B"),
		NULL
	};
	LPCTSTR szFmt4[] = 
	{
		_T("%H:%M:%S"),
		_T("%H %p/%M"),
		_T("%H:%M"),
		_T("%M:%S"),
		NULL
	};
	LPCTSTR* szFmt[] = { szFmt0, szFmt1, szFmt2, szFmt3, szFmt4 };

	SetDlgItemText(IDC_EDIT_FORMAT, szFmt[ type ][ idx ]);
	return 0;
}

void CFormatPropPage::SetAxisTimeFormat(VARIANT_BOOL b)
{
	HRESULT hr;
	CComPtr<IDMGraphCtrl> spGraph;
	if(FAILED(GetGraphCtrl(&spGraph)))
		return;

	CComPtr<IDMGraphAxis> spAxis; 
	hr = GetSelectedAxis(spGraph, &spAxis);
	if(FAILED(hr) || spAxis == NULL)
		return;

	hr = spAxis->put_Time(b);
}

STDMETHODIMP CFormatPropPage::Help(LPCOLESTR pszHelpDir)
{
	DmOpenHelp(GetParent(), pszHelpDir);
	return S_OK;
}
