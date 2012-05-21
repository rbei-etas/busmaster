/////////////////////////////////////////////////////////////////////////////
// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//      GraphAnno.h: Declaration of the CGraphAnnotation class
//
// CLASS NAME
//      CGraphAnnotation
//
// DESCRIPTION
//
// MODIFICATIONS
//      01-Dec-2011 MSamoila major re-design of old CGraphAnnotation
//
#ifndef __GRAPH_ANNO__H__
#define __GRAPH_ANNO__H__

#include "ElementPoint.h"
#include "GraphItem.h"

///////////////////////////////////////////////////////////
// Declaration of the CGraphAnnotation class.
class CGraphAnnotation :
    public CComObjectRootEx<CComSingleThreadModel>,
    public IDispatchImpl<IDMGraphAnnotation, &IID_IDMGraphAnnotation, &LIBID_DMGraphLib>,
    public CGraphItem
{
public:
    BEGIN_COM_MAP(CGraphAnnotation)
    COM_INTERFACE_ENTRY(IDMGraphAnnotation)
    COM_INTERFACE_ENTRY(IDispatch)
    END_COM_MAP()

    CGraphAnnotation ()
    {
        m_Caption       = L"";
        m_Color         = RGB(255,255,255);
        m_BkColor       = RGB(120,120,120);
        m_bHorizontal   = VARIANT_TRUE;
        m_bVisible      = VARIANT_TRUE;
        place           = CElementPoint(0.5,0.5);
    }

    virtual void AttachToGraph(CDMGraphCtrl* pCtrl, int id);

    // IDMGraphAnnotation

    STDMETHOD(get_LabelCaption)(/*[out, retval]*/ BSTR* pVal);
    STDMETHOD(put_LabelCaption)(/*[in]*/ BSTR newVal);
    STDMETHOD(get_LabelX)(/*[out, retval]*/ double* pVal);
    STDMETHOD(put_LabelX)(/*[in]*/ double newVal);
    STDMETHOD(get_LabelY)(/*[out, retval]*/ double* pVal);
    STDMETHOD(put_LabelY)(/*[in]*/ double newVal);
    STDMETHOD(get_LabelColor)(/*[out, retval]*/ OLE_COLOR* pVal);
    STDMETHOD(put_LabelColor)(/*[in]*/ OLE_COLOR newVal);
    STDMETHOD(get_LabelBkColor)(/*[out, retval]*/ OLE_COLOR* pVal);
    STDMETHOD(put_LabelBkColor)(/*[in]*/ OLE_COLOR newVal);
    STDMETHOD(get_Visible)(/*[out, retval]*/ VARIANT_BOOL* pVal);
    STDMETHOD(put_Visible)(/*[in]*/ VARIANT_BOOL newVal);
    STDMETHOD(get_LabelHorizontal)(/*[out, retval]*/ VARIANT_BOOL* pVal);
    STDMETHOD(put_LabelHorizontal)(/*[in]*/ VARIANT_BOOL newVal);
public:
    CComBSTR        m_Caption;
    COLORREF        m_Color, m_BkColor;
    BOOL            m_bHorizontal;
    BOOL            m_bVisible;
    CElementPoint   place;
};

#endif // __GRAPH_ANNO__H__