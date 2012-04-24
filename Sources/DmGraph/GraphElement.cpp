// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//		GraphElement.cpp: implementation for the CGraphElement class
//
// CLASS NAME
//		CGraphElement
//
// DESCRIPTION
// 
// MODIFICATIONS
//		01-Dec-2011 MSamoila major re-design of old CGraphElement
//
#include "stdafx.h"
#include "DMGraph.h"
#include "GraphElement.h"
#include "DMGraphCtrl.h"

static HRESULT WINAPI GetArrayFromVariant(VARIANT* pVar, SAFEARRAY** pparray);
static HRESULT WINAPI GetDoubleElement(VARTYPE type, SAFEARRAY* pSA, long idx, double* pRetVal);

CGraphElement::CGraphElement()
{
	m_LineColor = m_PointColor = RGB(255, 255, 255);
	m_nType			= Solid;
	m_nLineWidth	= 0 ;
	m_nSymbol		= Nosym;
	m_bSolid		= VARIANT_TRUE;
	m_nPointSize	= 0 ;
	m_bShow			= VARIANT_TRUE;      // Added by A. Hofmann

	min = max = CElementPoint(0,0);		

	InitializeCriticalSection(&m_CritSectionForPlot);
}

CGraphElement::~CGraphElement()
{
	DeleteCriticalSection(&m_CritSectionForPlot);
}

void CGraphElement::AttachToGraph(CDMGraphCtrl* pCtrl, int id)
{
	CGraphItem::AttachToGraph(pCtrl, id);

	const int TXT_LEN = 128;
	OLECHAR szText[ TXT_LEN ];
	sprintf_s((char *) szText, TXT_LEN, "Element-%d", m_nID);
	m_strName = szText;
}

void CGraphElement::DetachFromGraph()
{
	ATLASSERT(m_pCtrl);

	if(m_PointList.GetSize())
		m_pCtrl->OnPlotDeleted();

	CGraphItem::DetachFromGraph();
}

BOOL CGraphElement::FindPoint(double x, double y, CElementPoint* pResult)
{
	ATLASSERT(pResult);
	int i, index = -1;

	double dx, dy, dr;

	for(i=0; i<m_PointList.GetSize(); i++)
	{
	    CElementPoint point = m_PointList[i];

		dx = fabs(x - point.x);
		dy = fabs(y - point.y);

		if(i == 0)
			dr = sqrt(dx + dy);
		else
		{
			if (sqrt(dx+dy) < dr )
			{
				dr = sqrt(dx+dy);
				index = i;
			}
		}
	}
	if(index<0)
		return FALSE;	//not found

	pResult->x = m_PointList[index].x;
	pResult->y = m_PointList[index].y;
	return TRUE;
}

STDMETHODIMP CGraphElement::get_LineColor(OLE_COLOR *pVal)
{
	if(pVal == NULL)
		return E_POINTER;
	
	*pVal = m_LineColor;
	return S_OK;
}

STDMETHODIMP CGraphElement::put_LineColor(OLE_COLOR newVal)
{
	HRESULT hr;
	COLORREF col;
	hr = OleTranslateColor(newVal, NULL, &col);

	if(m_LineColor == col)
		return S_FALSE;

	m_LineColor = col;

	if(m_pCtrl)
		m_pCtrl->Refresh(TRUE);

	return S_OK;
}

STDMETHODIMP CGraphElement::get_PointColor(OLE_COLOR *pVal)
{
	if(pVal == NULL)
		return E_POINTER;
	
	*pVal = m_PointColor;
	return S_OK;
}

STDMETHODIMP CGraphElement::put_PointColor(OLE_COLOR newVal)
{

	HRESULT hr;
	COLORREF col;
	hr = OleTranslateColor(newVal, NULL, &col);

	if(m_PointColor == col)
		return S_FALSE;

	m_PointColor = col;

	if(m_pCtrl)
		m_pCtrl->Refresh(TRUE);
	return S_OK;
}

STDMETHODIMP CGraphElement::get_Linetype(LineType *pVal)
{
	if(pVal == NULL)
		return E_POINTER;
	
	*pVal = m_nType;
	return S_OK;
}

STDMETHODIMP CGraphElement::put_Linetype(LineType newVal)
{
	if(m_nType == newVal)
		return S_FALSE;

	m_nType = newVal;

	if(m_pCtrl)
		m_pCtrl->Refresh(TRUE);
	return S_OK;
}

STDMETHODIMP CGraphElement::get_LineWidth(long *pVal)
{
	if(pVal == NULL)
		return E_POINTER;
	
	*pVal = m_nLineWidth;
	return S_OK;
}

STDMETHODIMP CGraphElement::put_LineWidth(long newVal)
{
	if(m_nLineWidth == newVal)
		return S_FALSE;

	if(newVal > 5)
		newVal = 5;
	if(newVal < 0)
		newVal = 0;

	m_nLineWidth = newVal;

	if(m_pCtrl)
		m_pCtrl->Refresh(TRUE);
	return S_OK;
}

STDMETHODIMP CGraphElement::get_PointSymbol(SymbolType *pVal)
{
	if(pVal == NULL)
		return E_POINTER;

	*pVal = m_nSymbol;
	return S_OK;
}

STDMETHODIMP CGraphElement::put_PointSymbol(SymbolType newVal)
{
	if(m_nSymbol == newVal)
		return S_FALSE;

	m_nSymbol = newVal;

	if(m_pCtrl)
		m_pCtrl->Refresh(TRUE);
	return S_OK;
}

STDMETHODIMP CGraphElement::get_SolidPoint(VARIANT_BOOL *pVal)
{
	if(pVal == NULL)
		return E_POINTER;

	*pVal = m_bSolid;
	return S_OK;
}

STDMETHODIMP CGraphElement::put_SolidPoint(VARIANT_BOOL newVal)
{
	if(m_bSolid == newVal)
		return S_FALSE;

	m_bSolid = newVal;

	if(m_pCtrl)
		m_pCtrl->Refresh(TRUE);
	return S_OK;
}

STDMETHODIMP CGraphElement::get_PointSize(long *pVal)
{
	if(pVal == NULL)
		return E_POINTER;
	
	*pVal = m_nPointSize;
	return S_OK;
}

STDMETHODIMP CGraphElement::put_PointSize(long newVal)
{
	if(m_nPointSize == newVal)
		return S_FALSE;

	if(newVal > 5)
		newVal = 5;
	if(newVal < 0)
		newVal = 0;

	m_nPointSize = newVal;

	if(m_pCtrl)
		m_pCtrl->Refresh(TRUE);
	return S_OK;
}

STDMETHODIMP CGraphElement::get_Show(VARIANT_BOOL *pVal)
{
	if(pVal == NULL)
		return E_POINTER;
	
	*pVal = m_bShow;
	return S_OK;
}

STDMETHODIMP CGraphElement::put_Show(VARIANT_BOOL newVal)
{
	if(m_bShow == newVal)
		return S_FALSE;

	m_bShow = newVal;

	if(m_pCtrl)
		m_pCtrl->Refresh(TRUE);
	return S_OK;
}


STDMETHODIMP CGraphElement::get_Name(BSTR *pVal)
{
	return m_strName.CopyTo(pVal);
}

STDMETHODIMP CGraphElement::put_Name(BSTR newVal)
{
	if(wcscmp(m_strName, newVal) == 0)
		return S_FALSE;

	m_strName = newVal;

	if(m_pCtrl)
		m_pCtrl->Refresh(TRUE);
	return S_OK;
}

STDMETHODIMP CGraphElement::get_Count(/*[out, retval]*/ long *pVal)
{
	if(pVal == NULL)
		return E_POINTER;
	*pVal = m_PointList.GetSize();
	return S_OK;
}

///////////////////////////////////////////////////
// Get X point value at point index 
STDMETHODIMP CGraphElement::get_XValue(/*[in]*/ long index, /*[out, retval]*/ double *pVal)
{
	if(index < 0 || index > m_PointList.GetSize())
		return AtlReportError(CLSID_NULL, L"Point not found!", IID_NULL, E_INVALIDARG);
	if(pVal == NULL)
		return E_POINTER;
	*pVal = m_PointList[index].x;
	return S_OK;
}
//////////////////////////////////////////////////
// Set X point value at point index 
STDMETHODIMP CGraphElement::put_XValue(/*[in]*/ long index, /*[in]*/ double newVal)
{
	if(index < 0 || index > m_PointList.GetSize())
		return AtlReportError(CLSID_NULL, L"Point not found!", IID_NULL, E_INVALIDARG);
	m_PointList[index].x = newVal;
	if(m_pCtrl)
		m_pCtrl->Refresh(FALSE);
	return S_OK;
}

////////////////////////////////////////////////////
// Get Y point value at point index
STDMETHODIMP CGraphElement::get_YValue(/*[in]*/ long index, /*[out, retval]*/ double *pVal)
{
	if(index < 0 || index > m_PointList.GetSize())
		return AtlReportError(CLSID_NULL, L"Point not found!", IID_NULL, E_INVALIDARG);
	if(pVal == NULL)
		return E_POINTER;
	*pVal = m_PointList[index].y;
	return S_OK;
}
///////////////////////////////////////////////////////
// Get Y point value at point index 
STDMETHODIMP CGraphElement::put_YValue(/*[in]*/ long index, /*[in]*/ double newVal)
{
	if(index < 0 || index > m_PointList.GetSize())
		return E_INVALIDARG;
	m_PointList[index].y = newVal;
	if(m_pCtrl)
		m_pCtrl->Refresh(FALSE);
	return S_OK;
}

///////////////////////////
// Set element XY data
STDMETHODIMP CGraphElement::PlotXY(double X, double Y) 
{
	CElementPoint point(X, Y);		

	m_PointList.Add(point);
	
	if(min.x > point.x)
		 min.x = point.x ;
    if(min.y > point.y)
		 min.y = point.y ;
	if(max.x < point.x)
		 max.x = point.x ;
	if(max.y < point.y)
		 max.y = point.y ;

	if(m_pCtrl)
	{
		m_pCtrl->UpdateAutoRange(X, Y, 
			m_PointList.GetSize() == 1 );	//TRUE only for first added point
		
		m_pCtrl->Refresh(FALSE);
	}
	return S_OK;
}

////////////////////////////
// Set element Y data 
STDMETHODIMP CGraphElement::PlotY(double Y) 
{
	int X;
	X = m_PointList.GetSize();

	return PlotXY(X, Y);
}


STDMETHODIMP CGraphElement::Plot(VARIANT newXVals, VARIANT newYVals)
{
	LONG lXBound, uXBound, lYBound, uYBound;
	SAFEARRAY* psaXVals, *psaYVals;
	HRESULT hr;

	//usually from VBScript the arrays are coming packed in by ref variants
	hr = GetArrayFromVariant(&newXVals, &psaXVals);
	if(FAILED(hr))
		return hr;
	hr = GetArrayFromVariant(&newYVals, &psaYVals);
	if(FAILED(hr))
		return hr;

	hr = SafeArrayGetLBound(psaXVals, 1, &lXBound);
	if(FAILED(hr))
		return hr;
	hr = SafeArrayGetUBound(psaYVals, 1, &uXBound);
	if(FAILED(hr))
		return hr;
	hr = SafeArrayGetLBound(psaXVals, 1, &lYBound);
	if(FAILED(hr))
		return hr;
	hr = SafeArrayGetUBound(psaYVals, 1, &uYBound);
	if(FAILED(hr))
		return hr;
	if(uXBound - lXBound != uYBound - lYBound)
		return AtlReportError(GUID_NULL, L"Array with same size expected", IID_NULL, E_INVALIDARG);

	int i, nCount;

	min = max = CElementPoint(0,0);		

	nCount = m_PointList.GetSize();

	EnterCriticalSection(&m_CritSectionForPlot);
	m_PointList.RemoveAll();
	LeaveCriticalSection(&m_CritSectionForPlot);

	if(m_pCtrl && nCount)
		m_pCtrl->OnPlotDeleted(); //current element does not have any points anymore

	nCount = uXBound - lXBound + 1;

	VARTYPE	typeX, typeY;
	SafeArrayGetVartype(psaXVals, &typeX);
	SafeArrayGetVartype(psaYVals, &typeY);

	for(i=0; i< nCount; i++)
	{
		double x ,y;
		hr = GetDoubleElement(typeX, psaXVals, i, &x);
		if(FAILED(hr))
			break;
		hr = GetDoubleElement(typeY, psaYVals, i, &y);
		if(FAILED(hr))
			break;

		CElementPoint point(x, y);
		m_PointList.Add(point);

		if(min.x > point.x)
			 min.x = point.x ;
		if(min.y > point.y)
			 min.y = point.y ;
		if(max.x < point.x)
			 max.x = point.x ;
		if(max.y < point.y)
			 max.y = point.y ;
		
		if(m_pCtrl)
		{
			m_pCtrl->UpdateAutoRange(x, y, 
				i == 0);	//TRUE only for first added point
		}
	}
	if(m_pCtrl)	//redraw once for all added points
		m_pCtrl->Refresh(FALSE);

	return hr;
}

///////////////////////////////////////////////
// helpers

static HRESULT WINAPI GetArrayFromVariant(VARIANT* pVar, SAFEARRAY** pparray)
{
	ATLASSERT(pVar);
	ATLASSERT(pparray);
	
	*pparray = NULL;
	VARIANT* pVarTmp;
	pVarTmp = pVar->vt == (VT_VARIANT|VT_BYREF) ? pVar->pvarVal : pVar; //typical for script clients
	if(pVarTmp == NULL)
		return E_INVALIDARG;
	
	if( (pVarTmp->vt & VT_ARRAY) == 0 )
		return AtlReportError(GUID_NULL, L"Array expected", IID_NULL, E_INVALIDARG);

	*pparray = (pVarTmp->vt & VT_BYREF) ? *(pVarTmp->pparray) : pVarTmp->parray;
	if(*pparray == NULL)
		return E_POINTER;

	if(SafeArrayGetDim(*pparray) != 1)
		return AtlReportError(GUID_NULL, L"Unidimensional array expected", IID_NULL, E_INVALIDARG);

	return S_OK;
}

static HRESULT WINAPI GetDoubleElement(VARTYPE type, SAFEARRAY* pSA, long idx, double* pRetVal)
{
	ATLASSERT(pRetVal);
	ATLASSERT(pSA);

	*pRetVal = 0.;

	HRESULT hr;
	CComVariant v;
	switch(type)
	{
	case VT_R8:
		return SafeArrayGetElement(pSA, &idx, pRetVal);
	case VT_VARIANT:
		hr = SafeArrayGetElement(pSA, &idx, &v);
		if(FAILED(hr))
			return hr;
		hr = v.ChangeType(VT_R8);
		if(FAILED(hr))
			return hr;
		*pRetVal = v.dblVal;
		break;
	case VT_BSTR:
		hr = SafeArrayGetElement(pSA, &idx, &v.bstrVal);
		if(FAILED(hr))
			return hr;
		v.vt = VT_BSTR;
		hr = v.ChangeType(VT_R8);
		if(FAILED(hr))
			return hr;
		*pRetVal = v.dblVal;
		break;
	case VT_I4:
		hr = SafeArrayGetElement(pSA, &idx, &v.lVal);
		if(FAILED(hr))
			return hr;
		v.vt = VT_I4;
		hr = v.ChangeType(VT_R8);
		if(FAILED(hr))
			return hr;
		*pRetVal = v.dblVal;
		break;
	case VT_UI4:
		hr = SafeArrayGetElement(pSA, &idx, &v.ulVal);
		if(FAILED(hr))
			return hr;
		v.vt = VT_I4;
		hr = v.ChangeType(VT_R8);
		if(FAILED(hr))
			return hr;
		*pRetVal = v.dblVal;
		break;
	}
	return S_OK;
}
