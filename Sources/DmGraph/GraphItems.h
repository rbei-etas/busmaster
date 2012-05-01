#ifndef __GRAPH__ITEMS__H__
#define __GRAPH__ITEMS__H__

#include "ElementPoint.h"


///////////////////////////////////////////////////////////
// Declaration of the CGraphAnnotation class.
class CGraphCursor
{

public:
    COLORREF    m_Color;
    BOOL        m_bVisible;
    CursorMode  m_nMode;
    CElementPoint position;
    Crosshair   m_nStyle;

    CGraphCursor ()
    {
        m_Color = RGB(255,255,255);
        m_bVisible = TRUE;
        position = CElementPoint(0,0);
        m_nStyle  = XY;
        m_nMode = Fixed;
    }
};

///////////////////////////////////////////////////////////
// Declaration of the CGraphAnnotation class.
class CGraphAnnotation
{
public:
    CComBSTR        m_Caption;
    COLORREF        m_Color, m_BkColor;
    BOOL            m_bHorizontal;
    BOOL            m_bVisible;
    CElementPoint   place;

    CGraphAnnotation ()
    {
        m_Caption = L"";
        m_Color = RGB(255,255,255);
        m_BkColor = RGB(120,120,120);
        m_bHorizontal = TRUE;
        m_bVisible = TRUE;
        place = CElementPoint(0.5,0.5);
    }
};

///////////////////////////////////////////////////////////
// Declaration of the CGraphElement class.
class CGraphElement
{
public:
    CGraphElement(int id=0);

    BOOL        m_bShow;
    BOOL        bIsPlotAvailable ;
    COLORREF    m_LineColor ;
    COLORREF    m_PointColor ;
    LineType    m_nType;
    long        m_nWidth ;
    SymbolType  m_nSymbol;
    BOOL        m_bSolid;
    long        m_nSize;
    CComBSTR    m_strName;
    CElementPoint min, max;
    CSimpleArray<CElementPoint> m_PointList;
};


#endif // __GRAPH__ITEMS__H__