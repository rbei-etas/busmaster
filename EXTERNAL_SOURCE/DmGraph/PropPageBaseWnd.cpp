// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//		PropPageBaseWnd.cpp: implementation for the CPropPageBaseWnd class
//
// CLASS NAME
//		CPropPageBaseWnd
//
// DESCRIPTION
// 
// MODIFICATIONS
//		01-Dec-2011 MSamoila created
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DMGraph.h"
#include "PropPageBaseWnd.h"
#include "resource.h"

HRESULT CPropPageBaseWnd::InitCollectionList(IDMGraphCollection* pGraphCollection)
{
	ATLASSERT(pGraphCollection);

	HRESULT hr;
	long count = 0, nSelected = 0;
    
	hr = pGraphCollection->get_Count(&count);
	if(FAILED(hr))
		return hr;
	hr = pGraphCollection->get_Selected(&nSelected);
	if(FAILED(hr))
		return hr;

	LVCOLUMN lvCol;
	RECT rc;
	::GetClientRect(GetDlgItem(IDC_COLLECTION_LIST), &rc);

	lvCol.mask		= LVCF_SUBITEM | LVCF_WIDTH ;
	lvCol.cx		= rc.right-rc.left;
	lvCol.iSubItem	= 0;
	SendDlgItemMessage(IDC_COLLECTION_LIST, LVM_INSERTCOLUMN, 0, (LPARAM)&lvCol);
	SendDlgItemMessage(IDC_COLLECTION_LIST, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT|LVS_EX_LABELTIP, LVS_EX_FULLROWSELECT|LVS_EX_LABELTIP);

	SendDlgItemMessage(IDC_COLLECTION_LIST, LVM_DELETEALLITEMS);

	LVITEM lvItem;
	lvItem.pszText		= LPSTR_TEXTCALLBACK;
	lvItem.iSubItem = 0;
	lvItem.mask			= LVIF_TEXT;

	for (int i=0; i < count; i++) 
	{
		lvItem.iItem	= i;
		SendDlgItemMessage(IDC_COLLECTION_LIST, LVM_INSERTITEM, 0, (LPARAM)&lvItem);

		if(nSelected == i)
		{
			lvItem.stateMask	= LVIS_SELECTED|LVIS_FOCUSED;
			lvItem.state		= LVIS_SELECTED|LVIS_FOCUSED;
			SendDlgItemMessage(IDC_COLLECTION_LIST, LVM_SETITEMSTATE, i, (LPARAM)&lvItem);
		}
	}

	return S_OK;
}

HRESULT CPropPageBaseWnd::AppendNewItem(IDMGraphCollection* pGraphCollection)
{
	ATLASSERT(pGraphCollection);

	HRESULT hr;
	CComPtr<IDispatch> spDispatch;
	hr = pGraphCollection->Add(&spDispatch);	//adds to the end and makes it selected
	if(FAILED(hr))
		return hr;

	long nIndex = 0;
	pGraphCollection->get_Count(&nIndex);

	long nSelected = 0;
	hr = pGraphCollection->get_Selected(&nSelected);
	if(FAILED(hr))
		return hr;
	
	LVITEM lvItem;
	lvItem.pszText		= LPSTR_TEXTCALLBACK;
	lvItem.iSubItem = 0;
	lvItem.mask			= LVIF_TEXT;

	lvItem.iItem	= --nIndex;
	SendDlgItemMessage(IDC_COLLECTION_LIST, LVM_INSERTITEM, 0, (LPARAM)&lvItem);

	if(nIndex == nSelected)	//it should always be
	{
		lvItem.stateMask	= LVIS_SELECTED|LVIS_FOCUSED;
		lvItem.state		= LVIS_SELECTED|LVIS_FOCUSED;
		SendDlgItemMessage(IDC_COLLECTION_LIST, LVM_SETITEMSTATE, nSelected, (LPARAM)&lvItem);
	}
	return S_OK;
}

HRESULT CPropPageBaseWnd::DeleteItem(IDMGraphCollection* pGraphCollection)
{
	ATLASSERT(pGraphCollection);
	HRESULT hr;
	int index = (long)SendDlgItemMessage(IDC_COLLECTION_LIST, LVM_GETNEXTITEM, -1, MAKELPARAM(LVNI_SELECTED, 0));

	hr = pGraphCollection->Delete(index);
	if(FAILED(hr))
		return hr;

	long nSelected = 0;
	hr = pGraphCollection->get_Selected(&nSelected);
	if(FAILED(hr))
		return hr;
	
	SendDlgItemMessage(IDC_COLLECTION_LIST, LVM_DELETEITEM, index);

	LVITEM lvItem;
	lvItem.stateMask	= LVIS_SELECTED|LVIS_FOCUSED;
	lvItem.state		= LVIS_SELECTED|LVIS_FOCUSED;
	SendDlgItemMessage(IDC_COLLECTION_LIST, LVM_SETITEMSTATE, nSelected, (LPARAM)&lvItem);

	return S_OK;
}

HRESULT CPropPageBaseWnd::RemoveSelection(IDMGraphCollection* pGraphCollection)
{
	ATLASSERT(pGraphCollection);
	HRESULT hr;
	hr = pGraphCollection->put_Selected(-1);
	if(FAILED(hr))
		return hr;

	LVITEM lvItem;
	lvItem.stateMask	= LVIS_SELECTED|LVIS_FOCUSED;
	lvItem.state		= 0;
	SendDlgItemMessage(IDC_COLLECTION_LIST, LVM_SETITEMSTATE, -1, (LPARAM)&lvItem);

	return S_OK;
}
