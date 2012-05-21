/////////////////////////////////////////////////////////////////////////////
// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//      GraphElement.h: Declaration of the CGraphElement class
//
// CLASS NAME
//      CGraphElement
//
// DESCRIPTION
//
// MODIFICATIONS
//      23-Jan-2012 Arunkumar Karri: Added LoicalCoordinate(..) API
//      01-Dec-2011 MSamoila major re-design of old CGraphElement
//
#ifndef __GRAPH_ELEMENT__H__
#define __GRAPH_ELEMENT__H__

#include "ElementPoint.h"
#include "GraphItem.h"

class CDMGraphCtrl;

///////////////////////////////////////////////////////////
// Declaration of the CGraphElement class.
class CGraphElement :
    public CComObjectRootEx<CComSingleThreadModel>,
    public IDispatchImpl<IDMGraphElement, &IID_IDMGraphElement, &LIBID_DMGraphLib>,
    public CGraphItem
{
public:
    BEGIN_COM_MAP(CGraphElement)
    COM_INTERFACE_ENTRY(IDMGraphElement)
    COM_INTERFACE_ENTRY(IDispatch)
    END_COM_MAP()

    CGraphElement();
    ~CGraphElement();

    virtual void AttachToGraph(CDMGraphCtrl* pCtrl, int id);
    virtual void DetachFromGraph();

    BOOL FindPoint(double x, double y, CElementPoint* pResult);

    // IDMGraphElement
    STDMETHOD(get_LineColor)(/*[out, retval]*/ OLE_COLOR* pVal);
    STDMETHOD(put_LineColor)(/*[in]*/ OLE_COLOR newVal);
    STDMETHOD(get_PointColor)(/*[out, retval]*/ OLE_COLOR* pVal);
    STDMETHOD(put_PointColor)(/*[in]*/ OLE_COLOR newVal);
    STDMETHOD(get_Linetype)(/*[out, retval]*/ LineType* pVal);
    STDMETHOD(put_Linetype)(/*[in]*/ LineType newVal);
    STDMETHOD(get_LineWidth)(/*[out, retval]*/ long* pVal);
    STDMETHOD(put_LineWidth)(/*[in]*/ long newVal);
    STDMETHOD(get_PointSymbol)(/*[out, retval]*/ SymbolType* pVal);
    STDMETHOD(put_PointSymbol)(/*[in]*/ SymbolType newVal);
    STDMETHOD(get_SolidPoint)(/*[out, retval]*/ VARIANT_BOOL* pVal);
    STDMETHOD(put_SolidPoint)(/*[in]*/ VARIANT_BOOL newVal);
    STDMETHOD(get_PointSize)(/*[out, retval]*/ long* pVal);
    STDMETHOD(put_PointSize)(/*[in]*/ long newVal);
    STDMETHOD(get_Show)(/*[out, retval]*/ VARIANT_BOOL* pVal);
    STDMETHOD(put_Show)(/*[in]*/ VARIANT_BOOL newVal);
    STDMETHOD(get_Name)(/*[out, retval]*/ BSTR* pVal);
    STDMETHOD(put_Name)(/*[in]*/ BSTR newVal);
    STDMETHOD(get_Count)(/*[out, retval]*/ long* pVal);
    STDMETHOD(get_YValue)(/*[in]*/ long index, /*[out, retval]*/ double* pVal);
    STDMETHOD(put_YValue)(/*[in]*/ long index, /*[in]*/ double newVal);
    STDMETHOD(get_XValue)(/*[in]*/ long index, /*[out, retval]*/ double* pVal);
    STDMETHOD(put_XValue)(/*[in]*/ long index, /*[in]*/ double newVal);
    STDMETHOD(PlotXY)(/*[in]*/ double X, /*[in]*/ double Y);
    STDMETHOD(PlotY)(/*[in]*/ double Y);
    STDMETHOD(Plot)(/*[in]*/ VARIANT newXVals, /*[in]*/ VARIANT newYVals);

    VARIANT_BOOL    m_bShow;
    COLORREF        m_LineColor ;
    COLORREF        m_PointColor ;
    LineType        m_nType;
    long            m_nLineWidth ;
    SymbolType      m_nSymbol;
    VARIANT_BOOL    m_bSolid;
    long            m_nPointSize;
    CComBSTR        m_strName;
    CElementPoint   min, max;
    CSimpleArray<CElementPoint> m_PointList;

    CRITICAL_SECTION m_CritSectionForPlot;
};



#endif // __GRAPH_ELEMENT__H__