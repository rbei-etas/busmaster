/////////////////////////////////////////////////////////////////////////////
// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//		GraphItem.h: Declaration of the CGraphItem class
//
// CLASS NAME
//		CGraphItem
//
// DESCRIPTION
// 
// MODIFICATIONS
//		01-Dec-2011 MSamoila created
//
#ifndef __GRAPH_ITEM__H__
#define __GRAPH_ITEM__H__

class CDMGraphCtrl;

class CGraphItem
{
public:
	CGraphItem()
	{
		m_pCtrl = NULL;
		m_nID = -1;
	}
	virtual void AttachToGraph(CDMGraphCtrl* pCtrl, int id)
	{
		ATLASSERT(m_pCtrl == NULL);
		m_pCtrl = pCtrl;
		m_nID = id;
	}
	virtual void DetachFromGraph()
	{
		ATLASSERT(m_pCtrl);
		m_pCtrl = NULL;
		m_nID = -1;
	}
protected:
	int				m_nID;
	CDMGraphCtrl*	m_pCtrl;
};

#endif // __GRAPH_ITEM__H__