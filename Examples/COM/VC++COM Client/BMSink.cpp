
#include "stdafx.h"
#include "BMSink.h"

/* Constructor */
ClientSink::ClientSink()
{
}

/* Distructor */
ClientSink::~ClientSink()
{
}

STDMETHODIMP ClientSink::OnMessageReceived(CAN_MSGS RxMsg)
{
    CString strTemp ("OnMessageReceived");
    AfxMessageBox(strTemp);
    return S_OK;
};

HRESULT STDMETHODCALLTYPE ClientSink::QueryInterface(REFIID iid, void **ppvObject)
{
	HRESULT hr = S_OK;

	if (iid == IID_IUnknown)
	{
		m_dwRefCount++;
		*ppvObject = (IUnknown *)this;
	}
	else if (iid == IID__IAppEvents)
	{   
		m_dwRefCount++;
		*ppvObject = (_IAppEvents *)this;
	}
	else
	{
		*ppvObject = NULL;
		hr = E_NOINTERFACE;
	}
	return hr;
}

ULONG STDMETHODCALLTYPE ClientSink::AddRef()
{
	m_dwRefCount++;
	return m_dwRefCount;
}


ULONG STDMETHODCALLTYPE ClientSink::Release()
{
	ULONG l;
	l  = m_dwRefCount--;
	if ( 0 == m_dwRefCount)
	{
		delete this;
	}
	return l;
}
