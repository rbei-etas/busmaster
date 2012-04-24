// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//		GraphAxis.cpp: implementation for the CGraphAxis class
//
// CLASS NAME
//		CGraphAxis
//
// DESCRIPTION
// 
// MODIFICATIONS
//		01-Dec-2011 MSamoila created
#include "stdafx.h"
#include "DmGraph.h"
#include "GraphAxis.h"
#include "DMGraphCtrl.h"

#define ROUND(x) 	((int)((x) + (((x)<0.0) ? -0.5 : 0.5)))

static BOOL WINAPI _TmFromOleDate(DATE dtSrc, struct tm* tmDest);

STDMETHODIMP CGraphAxis::get_GridNumber(short *pVal)
{
	if(pVal == NULL)
		return E_POINTER;
	*pVal = m_nGrid;
	return S_OK;
}

STDMETHODIMP CGraphAxis::put_GridNumber(short newVal)
{
	if(m_nGrid == newVal)
		return S_FALSE;
	m_nGrid = newVal;

	if(m_pCtrl)
		m_pCtrl->Refresh(FALSE);
	return S_OK;
}

STDMETHODIMP CGraphAxis::get_Label(BSTR *pVal)
{
	return m_bsLabel.CopyTo(pVal);
}

STDMETHODIMP CGraphAxis::put_Label(BSTR newVal)
{
	if(wcscmp(m_bsLabel, newVal) == 0)
		return S_FALSE;
	m_bsLabel = newVal;

	if(m_pCtrl)
		m_pCtrl->Refresh(FALSE);
	return S_OK;
}

STDMETHODIMP CGraphAxis::get_Log(VARIANT_BOOL *pVal)
{
	if(pVal == NULL)
		return E_POINTER;
	*pVal = m_bLog;
	return S_OK;
}

STDMETHODIMP CGraphAxis::put_Log(VARIANT_BOOL newVal)
{
	if(m_bLog == newVal)
		return S_FALSE;
	m_bLog = newVal;

	if(m_pCtrl)
		m_pCtrl->Refresh(FALSE);
	return S_OK;
}

STDMETHODIMP CGraphAxis::get_Time(VARIANT_BOOL *pVal)
{
	if(pVal == NULL)
		return E_POINTER;
	*pVal = m_bTime;
	return S_OK;
}

STDMETHODIMP CGraphAxis::put_Time(VARIANT_BOOL newVal)
{
	if(m_bTime == newVal)
		return S_FALSE;

	m_bTime = newVal;
	if(m_pCtrl)
		m_pCtrl->Refresh(FALSE);
	return S_OK;
}

STDMETHODIMP CGraphAxis::get_Format(BSTR *pVal)
{
	return m_bsFormat.CopyTo(pVal);
}

STDMETHODIMP CGraphAxis::put_Format(BSTR newVal)
{
	if(wcscmp(m_bsFormat, newVal) == 0)
		return S_FALSE;
	m_bsFormat = newVal;

	if(m_pCtrl)
		m_pCtrl->Refresh(FALSE);
	return S_OK;
}

STDMETHODIMP CGraphAxis::get_ShowGrid(VARIANT_BOOL *pVal)
{
	if(pVal == NULL)
		return E_POINTER;
	*pVal = m_bShowGrid;
	return S_OK;
}

STDMETHODIMP CGraphAxis::put_ShowGrid(VARIANT_BOOL newVal)
{
	if(m_bShowGrid == newVal)
		return S_FALSE;
	m_bShowGrid = newVal;

	if(m_pCtrl)
		m_pCtrl->Refresh(FALSE);
	return S_OK;
}


void CGraphAxis::FormatAxisLabel(double data, BSTR* pbsString)
{
	OLECHAR buffer[128];
	ATLASSERT(pbsString);
	*pbsString = NULL;

	DWORD dwCode;
	if(m_bLog)
		sprintf_s((char *) buffer, sizeof(buffer), "1E%d", ROUND(data)); // Log Scale
	else if (m_bTime)
	{
		tm tmTemp;
		if(_TmFromOleDate(data, &tmTemp) == FALSE)
			return;
		size_t ret = 0;
		__try
		{
			ret = wcsftime(buffer, 128, m_bsFormat, &tmTemp);
		}
		__except(dwCode = GetExceptionCode(), EXCEPTION_EXECUTE_HANDLER)
		{
			OutputDebugString(_T("Exception caught during date/time format\n"));
		}
		if( ret == 0 )
			return;
	}
	else
	{
		__try
		{
			sprintf_s((char *) buffer, sizeof(buffer), (char *) m_bsFormat.m_str, data);
		}
		__except(dwCode = GetExceptionCode(), EXCEPTION_EXECUTE_HANDLER)
		{
			OutputDebugString(_T("Exception caught during format\n"));
		}
	}

	ATLASSERT(pbsString);
	*pbsString = SysAllocString(buffer);
}


static BOOL WINAPI _TmFromOleDate(DATE dtSrc, struct tm* tmDest)
{
	SYSTEMTIME st;
	ATLASSERT(tmDest);
	if(VariantTimeToSystemTime(dtSrc, &st) == FALSE)
	{
		ATLTRACE(_T("Failed to convert %g date to system time\n"), dtSrc);
		return FALSE;
	}
	ZeroMemory(tmDest, sizeof(tm));

	tmDest->tm_year = st.wYear - 1900;	// year is based on 1900
	tmDest->tm_mon = st.wMonth - 1;		// month of year is 0-based
	tmDest->tm_mday = st.wDay;
	tmDest->tm_wday = st.wDayOfWeek; 

	tmDest->tm_hour = st.wHour;
	tmDest->tm_min = st.wMinute;
	tmDest->tm_sec = st.wSecond;

	return TRUE;
}

