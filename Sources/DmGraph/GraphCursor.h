/////////////////////////////////////////////////////////////////////////////
// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//      GraphCursor.h: Declaration of the CGraphCursor class
//
// CLASS NAME
//      CGraphCursor
//
// DESCRIPTION
//
// MODIFICATIONS
//      01-Dec-2011 MSamoila major re-design of old CGraphCursor
//
#ifndef __GRAPH__CURSOR__H__
#define __GRAPH__CURSOR__H__

#include "ElementPoint.h"
#include "GraphItem.h"

///////////////////////////////////////////////////////////
// Declaration of the CGraphCursor class.
class CGraphCursor :
    public CComObjectRootEx<CComSingleThreadModel>,
    public IDispatchImpl<IDMGraphCursor, &IID_IDMGraphCursor, &LIBID_DMGraphLib>,
    public CGraphItem
{
public:
    BEGIN_COM_MAP(CGraphCursor)
    COM_INTERFACE_ENTRY(IDMGraphCursor)
    COM_INTERFACE_ENTRY(IDispatch)
    END_COM_MAP()

    CGraphCursor ()
    {
        m_Color = RGB(255,0,0);
        m_bVisible = VARIANT_TRUE;
        position = CElementPoint(0,0);
        m_nStyle  = Y;
        m_nMode = Floating;
        m_shCursorID = 1;
    }

    virtual void AttachToGraph(CDMGraphCtrl* pCtrl, int id);

    STDMETHOD(get_Color)(/*[out, retval]*/ OLE_COLOR* pVal);
    STDMETHOD(put_Color)(/*[in]*/ OLE_COLOR newVal);
    STDMETHOD(get_X)(/*[out, retval]*/ double* pVal);
    STDMETHOD(put_X)(/*[in]*/ double newVal);
    STDMETHOD(get_Y)(/*[out, retval]*/ double* pVal);
    STDMETHOD(put_Y)(/*[in]*/ double newVal);
    STDMETHOD(get_Style)(/*[out, retval]*/ Crosshair* pVal);
    STDMETHOD(put_Style)(/*[in]*/ Crosshair newVal);
    STDMETHOD(get_Visible)(/*[out, retval]*/ VARIANT_BOOL* pVal);
    STDMETHOD(put_Visible)(/*[in]*/ VARIANT_BOOL newVal);
    STDMETHOD(get_Mode)(/*[out, retval]*/ CursorMode* pVal);
    STDMETHOD(put_Mode)(/*[in]*/ CursorMode newVal);
    STDMETHOD(get_Name)(/*[out, retval]*/ BSTR* pVal);
    STDMETHOD(put_Name)(/*[in]*/ BSTR newVal);

public:
    COLORREF    m_Color;
    BOOL        m_bVisible;
    CursorMode  m_nMode;
    CElementPoint position;
    Crosshair   m_nStyle;
    CComBSTR    m_strName;
    SHORT       m_shCursorID;
    BOOL        m_bTracking;
public:
    STDMETHOD(get_CursorID)(SHORT* pVal);
public:
    STDMETHOD(put_CursorID)(SHORT newVal);
};




#endif // __GRAPH__CURSOR__H__