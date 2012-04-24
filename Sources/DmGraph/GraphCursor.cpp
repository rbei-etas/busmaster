// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//		GraphCursor.cpp: implementation for the CGraphCursor class
//
// CLASS NAME
//		CGraphCursor
//
// DESCRIPTION
// 
// MODIFICATIONS
//		01-Dec-2011 MSamoila major re-design of old CGraphCursor

#include "stdafx.h"
#include "DMGraph.h"
#include "GraphCursor.h"
#include "GraphElement.h"
#include "DMGraphCtrl.h"

void CGraphCursor::AttachToGraph(CDMGraphCtrl* pCtrl, int id)
{
	CGraphItem::AttachToGraph(pCtrl, id);

	const int TXT_LEN = 128;
	OLECHAR szText[ TXT_LEN ];
	sprintf_s((char *) szText, TXT_LEN, "Cursor-%d", m_nID);
	m_strName = szText;
}

STDMETHODIMP CGraphCursor::get_Color(OLE_COLOR *pVal)
{
	if(pVal == NULL)
		return E_POINTER;
	*pVal = m_Color;
	return S_OK;
}

STDMETHODIMP CGraphCursor::put_Color(OLE_COLOR newVal)
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

STDMETHODIMP CGraphCursor::get_X(double *pVal)
{
	if(pVal == NULL)
		return E_POINTER;
	*pVal = position.x;
	return S_OK;
}

STDMETHODIMP CGraphCursor::put_X(double newVal)
{
	if(position.x == newVal)
		return S_FALSE;

	position.x = newVal;

	if(m_pCtrl)
		m_pCtrl->Refresh(FALSE);
	return S_OK;
}

STDMETHODIMP CGraphCursor::get_Y(double *pVal)
{
	if(pVal == NULL)
		return E_POINTER;
	*pVal = position.y;
	return S_OK;
}

STDMETHODIMP CGraphCursor::put_Y(double newVal)
{
	if(position.y == newVal)
		return S_FALSE;

	position.y = newVal;

	if(m_pCtrl)
		m_pCtrl->Refresh(FALSE);
	return S_OK;
}

STDMETHODIMP CGraphCursor::get_Style(Crosshair *pVal)
{
	if(pVal == NULL)
		return E_POINTER;
	*pVal = m_nStyle;
	return S_OK;
}

STDMETHODIMP CGraphCursor::put_Style(Crosshair newVal)
{
	if(m_nStyle == newVal)
		return S_FALSE;

	m_nStyle = newVal;

	if(m_pCtrl)
		m_pCtrl->Refresh(FALSE);
	return S_OK;
}

STDMETHODIMP CGraphCursor::get_Visible(VARIANT_BOOL *pVal)
{
	if(pVal == NULL)
		return E_POINTER;
	*pVal = m_bVisible;
	return S_OK;
}

STDMETHODIMP CGraphCursor::put_Visible(VARIANT_BOOL newVal)
{
	if(m_bVisible == newVal)
		return S_FALSE;

	m_bVisible = newVal;

	if(m_pCtrl)
		m_pCtrl->Refresh(FALSE);
	return S_OK;
}

STDMETHODIMP CGraphCursor::get_Mode(CursorMode *pVal)
{
	if(pVal == NULL)
		return E_POINTER;
	*pVal = m_nMode;
	return S_OK;
}

STDMETHODIMP CGraphCursor::put_Mode(CursorMode newVal)
{
	if(m_nMode == newVal)
		return S_FALSE;

	m_nMode = newVal;
	if(m_nMode == Snap && m_pCtrl && m_pCtrl->m_pElementList->GetSize() > 0)
	{
		//adjust the current cursor position to snap
		CElementPoint pt;
		m_pCtrl->FindPoint(position.x, position.y, &pt);

		position.x = pt.x;
		position.y = pt.y;
	}

	if(m_pCtrl)
		m_pCtrl->Refresh(FALSE);
	return S_OK;
}

STDMETHODIMP CGraphCursor::get_Name(BSTR *pVal)
{
	return m_strName.CopyTo(pVal);
}

STDMETHODIMP CGraphCursor::put_Name(BSTR newVal)
{
	if(wcscmp(m_strName, newVal) == 0)
		return S_FALSE;

	m_strName = newVal;

	if(m_pCtrl)
		m_pCtrl->Refresh(TRUE);
	return S_OK;
}
STDMETHODIMP CGraphCursor::get_CursorID(SHORT* pVal)
{	
	if(pVal == NULL)
		return E_POINTER;
	*pVal = m_shCursorID;
	
	return S_OK;
}

STDMETHODIMP CGraphCursor::put_CursorID(SHORT newVal)
{
	m_shCursorID = newVal;

	return S_OK;
}