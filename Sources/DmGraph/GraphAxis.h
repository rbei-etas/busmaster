/////////////////////////////////////////////////////////////////////////////
// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//      GraphAxis.h: Declaration of the CGraphAxis class
//
// CLASS NAME
//      CGraphAxis
//
// DESCRIPTION
//
// MODIFICATIONS
//      01-Dec-2011 MSamoila created
//
#ifndef __GRAPH_AXIS__H__
#define __GRAPH_AXIS__H__

#include "GraphItem.h"

class CDMGraphCtrl;

///////////////////////////////////////////////////////////
// Declaration of the CGraphAxis class.
class CGraphAxis :
    public CComObjectRootEx<CComSingleThreadModel>,
    public IDispatchImpl<IDMGraphAxis, &IID_IDMGraphAxis, &LIBID_DMGraphLib>,
    public CGraphItem
{
public:
    BEGIN_COM_MAP(CGraphAxis)
    COM_INTERFACE_ENTRY(IDMGraphAxis)
    COM_INTERFACE_ENTRY(IDispatch)
    END_COM_MAP()

    CGraphAxis ()
    {
        m_nGrid     = 5;
        m_bLog      = VARIANT_FALSE;
        m_bTime     = VARIANT_FALSE;
        m_pCtrl     = NULL;
        m_bsFormat  = L"%g";
        m_bShowGrid = VARIANT_TRUE;
    }

    void FormatAxisLabel(double data, BSTR* pbsString);

    // IDMGraphAxis

    STDMETHOD(get_Label)(/*[out, retval]*/ BSTR* pVal);
    STDMETHOD(put_Label)(/*[in]*/ BSTR newVal);
    STDMETHOD(get_Time)(/*[out, retval]*/ VARIANT_BOOL* pVal);
    STDMETHOD(put_Time)(/*[in]*/ VARIANT_BOOL newVal);
    STDMETHOD(get_Log)(/*[out, retval]*/ VARIANT_BOOL* pVal);
    STDMETHOD(put_Log)(/*[in]*/ VARIANT_BOOL newVal);
    STDMETHOD(get_GridNumber)(/*[out, retval]*/ short* pVal);
    STDMETHOD(put_GridNumber)(/*[in]*/ short newVal);
    STDMETHOD(get_Format)(/*[out, retval]*/ BSTR* pVal);
    STDMETHOD(put_Format)(/*[in]*/ BSTR newVal);
    STDMETHOD(get_ShowGrid)(/*[out, retval]*/ VARIANT_BOOL* pVal);
    STDMETHOD(put_ShowGrid)(/*[in]*/ VARIANT_BOOL newVal);

public:
    CComBSTR            m_bsLabel;
    VARIANT_BOOL        m_bLog,
                        m_bTime;

    short               m_nGrid;    //grid number
    CComBSTR            m_bsFormat;

    VARIANT_BOOL        m_bShowGrid;
};

#endif // __GRAPH_AXIS__H__