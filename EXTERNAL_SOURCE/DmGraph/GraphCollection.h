/////////////////////////////////////////////////////////////////////////////
// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, 2011
//
// FILE NAME
//		GraphCollection.h: Declaration of the CGraphCollection class
//
// CLASS NAME
//		CGraphCollection
//
// DESCRIPTION
// 
// MODIFICATIONS
//		01-Dec-2011 MSamoila created
//
#ifndef __GRAPH_COLLECTION__H__
#define __GRAPH_COLLECTION__H__

#include "GraphItem.h"

class CDMGraphCtrl;

///////////////////////////////////////////////////////////
// Declaration of the CGraphCollection class.
template <class T>
class CGraphCollection :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IDMGraphCollection, &IID_IDMGraphCollection, &LIBID_DMGraphLib>,
	public CSimpleValArray<T*>,
	public CGraphItem
{
public:
BEGIN_COM_MAP(CGraphCollection)
	COM_INTERFACE_ENTRY(IDMGraphCollection)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()

	CGraphCollection () 
	{
		m_nSelectedIdx	= -1;
	}
	~CGraphCollection ()
	{
		RemoveAll();
	}
	void RemoveAll()
	{
		int i;
		for(i=0; i<GetSize(); i++)
		{
			ATLASSERT(m_aT[i]);
			if(m_pCtrl)
				m_aT[i]->DetachFromGraph();
			m_aT[i]->Release();
		}
		CSimpleValArray<T*>::RemoveAll();
		m_nSelectedIdx	= -1;
	}

// IDMGraphCollection
	
	STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal)
	{
		if(pVal == NULL)
			return E_POINTER;
		*pVal = GetSize();
		return S_OK;
	}
	STDMETHOD(get_Item)(/*[in]*/ long index, /*[out, retval]*/ IDispatch* *pVal)
	{
		if(pVal == NULL)
			return E_POINTER;
		*pVal = NULL;
		if(index < 0 || index >= GetSize())
			return E_INVALIDARG;

		return m_aT[ index ]->QueryInterface(IID_IDispatch, (void**)pVal);
	}
	STDMETHOD(get_Selected)(/*[out, retval]*/ long *pVal)
	{
		if(pVal == NULL)
			return E_POINTER;
		*pVal = m_nSelectedIdx;
		return S_OK;
	}
	STDMETHOD(put_Selected)(/*[out, retval]*/ long newVal)
	{
		if(m_nSelectedIdx == newVal)
			return S_FALSE;
		m_nSelectedIdx = newVal;
		return S_OK;
	}
	STDMETHOD(Add)(/*[out, retval]*/ IDispatch* *pVal)
	{
		if(pVal == NULL)
			return E_POINTER;
		*pVal = NULL;
		CComObject<T>* newItem;
		HRESULT hr = CComObject<T>::CreateInstance(&newItem);
		if(newItem == NULL)
			return E_OUTOFMEMORY;
		if(!CSimpleValArray<T*>::Add(newItem))
		{
			delete newItem;
			return E_OUTOFMEMORY;
		}
		newItem->AddRef();
		m_nSelectedIdx = GetSize() - 1;
		if(m_pCtrl)
		{
			newItem->AttachToGraph(m_pCtrl, m_nSelectedIdx);
			m_pCtrl->Refresh(FALSE);
		}
		return newItem->QueryInterface(IID_IDispatch, (void**)pVal);
	}
	STDMETHOD(Delete)(/*[in]*/ long index)
	{
		if(index <0 || index >= GetSize())
			return E_INVALIDARG;
		T* item = m_aT[ index ];
		ATLASSERT(item);
		RemoveAt( index );
		item->DetachFromGraph();
		item->Release();

		if(m_nSelectedIdx >= index)
			m_nSelectedIdx--;
		if(m_nSelectedIdx < 0  && GetSize())
			m_nSelectedIdx = 0;
		
		if(m_pCtrl)
			m_pCtrl->Refresh(FALSE);
		return S_OK;
	}

	virtual void AttachToGraph(CDMGraphCtrl* pCtrl, int /*id*/)
	{
		CGraphItem::AttachToGraph(pCtrl, 0);
		int i;
		for(i=0; i<GetSize(); i++)
		{
			ATLASSERT(m_aT[i]);
			m_aT[i]->AttachToGraph(pCtrl, i);
		}
	}
	virtual void DetachFromGraph()
	{
		CGraphItem::DetachFromGraph();
		int i;
		for(i=0; i<GetSize(); i++)
		{
			ATLASSERT(m_aT[i]);
			m_aT[i]->DetachFromGraph();
		}
	}

	long				m_nSelectedIdx;
};

#endif // __GRAPH_COLLECTION__H__