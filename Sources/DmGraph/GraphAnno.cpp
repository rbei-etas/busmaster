// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//		GraphAnno.cpp: implementation for the CGraphAnnotation class
//
// CLASS NAME
//		CGraphAnnotation
//
// DESCRIPTION
// 
// MODIFICATIONS
//		01-Dec-2011 MSamoila major re-design of old CGraphAnnotation
//
#include "stdafx.h"
#include "DmGraph.h"
#include "GraphAnno.h"
#include "DMGraphCtrl.h"

void CGraphAnnotation::AttachToGraph(CDMGraphCtrl* pCtrl, int id)
{
	CGraphItem::AttachToGraph(pCtrl, id);

	const int TXT_LEN = 128;
	OLECHAR szText[ TXT_LEN ];
	sprintf_s((char *) szText, TXT_LEN, "Annotation-%d", m_nID);
	m_Caption = szText;
}

STDMETHODIMP CGraphAnnotation::get_LabelCaption(BSTR *pVal)
{
	return m_Caption.CopyTo(pVal);
}

STDMETHODIMP CGraphAnnotation::put_LabelCaption(BSTR newVal)
{
	if(wcscmp(m_Caption, newVal) == 0)
		return S_FALSE;

	m_Caption = newVal;
	ATLTRACE(_T("Caption : %s\n"), newVal);

	if(m_pCtrl)
		m_pCtrl->Refresh(FALSE);
	return S_OK;
}

STDMETHODIMP CGraphAnnotation::get_LabelX(double *pVal)
{
	if(pVal == NULL)
		return E_POINTER;
	*pVal = place.x;
	return S_OK;
}

STDMETHODIMP CGraphAnnotation::put_LabelX(double newVal)
{
	if(place.x == newVal)
		return S_FALSE;

	place.x = newVal;
	ATLTRACE(_T("LabelX = %g\n"), newVal);

	if(m_pCtrl)
		m_pCtrl->Refresh(FALSE);
	return S_OK;
}

STDMETHODIMP CGraphAnnotation::get_LabelY(double *pVal)
{
	if(pVal == NULL)
		return E_POINTER;
	*pVal = place.y;
	return S_OK;
}

STDMETHODIMP CGraphAnnotation::put_LabelY(double newVal)
{
	if(place.y == newVal)
		return S_FALSE;

	place.y = newVal;
	ATLTRACE(_T("LabelY = %g\n"), newVal);

	if(m_pCtrl)
		m_pCtrl->Refresh(FALSE);
	return S_OK;
}

STDMETHODIMP CGraphAnnotation::get_LabelColor(OLE_COLOR *pVal)
{
	if(pVal == NULL)
		return E_POINTER;
	*pVal = m_Color;
	return S_OK;
}

STDMETHODIMP CGraphAnnotation::put_LabelColor(OLE_COLOR newVal)
{
	HRESULT hr;
	COLORREF col;
	hr = OleTranslateColor(newVal, NULL, &col);

	if(m_Color == col)
		return S_FALSE;

	m_Color = col;

	if(m_pCtrl)
		m_pCtrl->Refresh(FALSE);
	return S_OK;
}

STDMETHODIMP CGraphAnnotation::get_LabelHorizontal(VARIANT_BOOL *pVal)
{
	if(pVal == NULL)
		return E_POINTER;
	*pVal = m_bHorizontal;
	return S_OK;
}

STDMETHODIMP CGraphAnnotation::put_LabelHorizontal(VARIANT_BOOL newVal)
{
	if(m_bHorizontal == newVal)
		return S_FALSE;

	m_bHorizontal = newVal;

	if(m_pCtrl)
		m_pCtrl->Refresh(FALSE);
	return S_OK;
}

STDMETHODIMP CGraphAnnotation::get_Visible(VARIANT_BOOL *pVal)
{
	if(pVal == NULL)
		return E_POINTER;
	*pVal = m_bVisible;
	return S_OK;
}

STDMETHODIMP CGraphAnnotation::put_Visible(VARIANT_BOOL newVal)
{
	if(m_bVisible == newVal)
		return S_FALSE;

	m_bVisible = newVal;

	if(m_pCtrl)
		m_pCtrl->Refresh(FALSE);
	return S_OK;
}

STDMETHODIMP CGraphAnnotation::get_LabelBkColor(OLE_COLOR *pVal)
{
	if(pVal == NULL)
		return E_POINTER;
	*pVal = m_BkColor;
	return S_OK;
}

STDMETHODIMP CGraphAnnotation::put_LabelBkColor(OLE_COLOR newVal)
{
	HRESULT hr;
	COLORREF col;
	hr = OleTranslateColor(newVal, NULL, &col);

	if(m_BkColor == col)
		return S_FALSE;

	m_BkColor = col;

	if(m_pCtrl)
		m_pCtrl->Refresh(FALSE);
	return S_OK;
}
