#include "stdafx.h"
#include "DMGraph.h"
#include "GraphItems.h"


CGraphElement::CGraphElement(int id/*=0*/)
{
    bIsPlotAvailable = FALSE ;
    m_LineColor = m_PointColor = RGB(0,0,0);
    m_nType     = Solid;
    m_nWidth    = 0 ;
    m_nSymbol   = Nosym;
    m_bSolid    = TRUE;
    m_nSize     = 0 ;
    m_bShow     = TRUE;      // Added by A. Hofmann
    const int TXT_LEN = 128;
    OLECHAR szText[ TXT_LEN ];
    _snwprintf(szText, TXT_LEN, L"Element-%d",id);
    m_strName = szText;
    min = max = CElementPoint(0,0);
}

