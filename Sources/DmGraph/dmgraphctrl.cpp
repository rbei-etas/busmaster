// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//      DMGraphCtrl.cpp: implementation for the CDMGraphCtrl class
//
// CLASS NAME
//      CDMGraphCtrl
//
// DESCRIPTION
//
// MODIFICATIONS
//      01-Dec-2011 MSamoila major re-design of old CNTGraphCtrl
// code based on:
//  http://www.codeproject.com/KB/miscctrl/ntgraph_activex.aspx
//

#include "stdafx.h"
#include "DMGraph.h"
#include "DMGraphCtrl.h"
#include "DrawFct.h"
#include "RectTracker.h"
#include "GraphAnno.h"
#include "GraphElement.h"
#include "GraphCursor.h"
#include "GraphAxis.h"
#include "about.h"

#ifndef MAX
#define MAX 1
#endif

#ifndef MIN
#define MIN 0
#endif

#define X_OFFSET 15
#define Y_OFFSET 15

#define PT2DBLX(x) (double)((x - m_axisRect.left)*dResX)+ dRangeX[MIN]
#define PT2DBLY(y) (double)((m_axisRect.bottom - y)*dResY)+ dRangeY[MIN]

//////////////////////////////////////////////////////////////////////////////
// static FONTDESC structure containing default attributes of the fonts
static const FONTDESC _fontdescIdent =
{
    sizeof(FONTDESC), OLESTR("Arial"), FONTSIZE( 8 ), FW_THIN,
    ANSI_CHARSET, FALSE, FALSE, FALSE
};

static const FONTDESC _fontdescTick =
{
    sizeof(FONTDESC), OLESTR("Arial"), FONTSIZE( 8 ), FW_NORMAL,
    ANSI_CHARSET, FALSE, FALSE, FALSE
};

static const FONTDESC _fontdescLabel =
{
    sizeof(FONTDESC), OLESTR("Arial"), FONTSIZE( 10 ), FW_NORMAL,
    ANSI_CHARSET, FALSE, FALSE, FALSE
};

static const FONTDESC _fontdescTitle =
{
    sizeof(FONTDESC), OLESTR("Times New Roman"), FONTSIZE( 12 ), FW_BOLD,
    ANSI_CHARSET, FALSE, FALSE, FALSE
};

/////////////////////////////////////////////////////////////////////////////
// CDMGraphCtrl

CDMGraphCtrl::CDMGraphCtrl()
{
    m_fontY = m_fontVLabel = NULL;
    m_axisPen = m_gridPen = m_cursorPen = NULL;
    m_brushPlot = m_brushFrame = NULL;
    m_linePen = m_pointPen = NULL;
    m_lineBrush = m_pointBrush = m_annoBrush = NULL;
    ZeroMemory(&m_rcBounds, sizeof(RECT));
    m_bWindowOnly = TRUE;
    SIZE sizePixel = { 250, 150 };
    AtlPixelToHiMetric(&sizePixel, &m_sizeExtent);
    m_nAvailablePlots = 0;
    m_bUnzoom = FALSE;
    m_pElementList      = NULL;
    m_pAnnotationList   = NULL;
    m_pCursorList       = NULL;
    ZeroMemory(m_pAxis, 2*sizeof(CGraphAxis*));
    m_eTrackMode = None;
    m_eFrameStyle = Bitmap;
    m_colAxis = RGB( 127, 127, 127 );
    m_colGrid = RGB(  64,  64,  64 );
    m_colLabel= RGB( 255, 255, 255 );
    m_colControlFrameColor  = RGB( 255, 255, 255 );
    m_colPlotAreaColor      = RGB(   0,   0,   0);
    ZeroMemory(&m_ti, sizeof(TOOLINFO));
    m_ti.cbSize = sizeof(TOOLINFO);
    m_ti.lpszText = (LPWSTR) m_ToolTipText;
    m_ToolTipText[0] = '\0';
    m_nCursorCount = 0;
}

HRESULT CDMGraphCtrl::FinalConstruct()
{
    m_pElementList = new CComObject< CGraphCollection< CGraphElement > >();

    if(m_pElementList == NULL)
    {
        return E_OUTOFMEMORY;
    }

    m_pElementList->AttachToGraph(this, 0);
    m_pElementList->AddRef();
    m_pAnnotationList = new CComObject< CGraphCollection< CGraphAnnotation > >();

    if(m_pAnnotationList == NULL)
    {
        return E_OUTOFMEMORY;
    }

    m_pAnnotationList->AttachToGraph(this, 0);
    m_pAnnotationList->AddRef();
    m_pCursorList = new CComObject< CGraphCollection< CGraphCursor > >();

    if(m_pCursorList == NULL)
    {
        return E_OUTOFMEMORY;
    }

    m_pCursorList->AttachToGraph(this, 0);
    m_pCursorList->AddRef();
    int i;

    for(i=0; i<2; i++)
    {
        m_pAxis[i] = new CComObject<CGraphAxis>();

        if(m_pAxis[i] == NULL)
        {
            return E_OUTOFMEMORY;
        }

        m_pAxis[i]->AttachToGraph(this, 0);
        m_pAxis[i]->AddRef();
    }

    SetRange(0.0,1.0,0.0,1.0);
    m_axisRect.left = m_axisRect.bottom = m_axisRect.right = m_axisRect.top = 0;
    Corrdinate(0,0, &m_panPoint);
    m_bElementIdentify = FALSE;
    HBITMAP hBitmap = LoadBitmap(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDB_FRAME));

    if(hBitmap == NULL)
    {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    PICTDESC pdesc;
    pdesc.cbSizeofstruct = sizeof(PICTDESC);
    pdesc.picType = PICTYPE_BITMAP;
    pdesc.bmp.hbitmap = hBitmap;
    pdesc.bmp.hpal = NULL;
    HRESULT hr = OleCreatePictureIndirect(&pdesc, IID_IPicture,
                                          TRUE, (LPVOID*)&m_spPicFrame);

    if(FAILED(hr))
    {
        ATLTRACE(_T("Failed to load bitmap %#x\n"), hr);
        return hr;
    }

    //fill with default values
    hr = ::OleCreateFontIndirect(const_cast<LPFONTDESC>(&_fontdescLabel), IID_IFont,
                                 (LPVOID*)&m_fontLabel);

    if(FAILED(hr))
    {
        return hr;
    }

    hr = ::OleCreateFontIndirect(const_cast<LPFONTDESC>(&_fontdescTick), IID_IFont,
                                 (LPVOID*)&m_fontTick);

    if(FAILED(hr))
    {
        return hr;
    }

    hr = ::OleCreateFontIndirect(const_cast<LPFONTDESC>(&_fontdescTitle), IID_IFont,
                                 (LPVOID*)&m_fontTitle);

    if(FAILED(hr))
    {
        return hr;
    }

    hr = ::OleCreateFontIndirect(const_cast<LPFONTDESC>(&_fontdescIdent), IID_IFont,
                                 (LPVOID*)&m_fontIdent);
    return hr;
}

CDMGraphCtrl::~CDMGraphCtrl()
{
    if(m_fontY)
    {
        DeleteObject(m_fontY);
        m_fontY = NULL;
    }

    if(m_fontVLabel)
    {
        DeleteObject(m_fontVLabel);
        m_fontVLabel = NULL;
    }

    if(m_axisPen)
    {
        DeleteObject(m_axisPen);
        m_axisPen = NULL;
    }

    if(m_gridPen)
    {
        DeleteObject(m_gridPen);
        m_gridPen = NULL;
    }

    if(m_cursorPen)
    {
        DeleteObject(m_cursorPen);
        m_cursorPen = NULL;
    }

    if(m_brushPlot)
    {
        DeleteObject(m_brushPlot);
        m_brushPlot = NULL;
    }

    if(m_brushFrame)
    {
        DeleteObject(m_brushFrame);
        m_brushFrame = NULL;
    }

    if(m_linePen)
    {
        DeleteObject(m_linePen);
        m_linePen = NULL;
    }

    if(m_pointPen)
    {
        DeleteObject(m_pointPen);
        m_pointPen = NULL;
    }

    if(m_lineBrush)
    {
        DeleteObject(m_lineBrush);
        m_lineBrush = NULL;
    }

    if(m_pointBrush)
    {
        DeleteObject(m_pointBrush);
        m_pointBrush = NULL;
    }

    if(m_annoBrush)
    {
        DeleteObject(m_annoBrush);
        m_annoBrush = NULL;
    }

    if(m_pElementList)
    {
        m_pElementList->DetachFromGraph();
        m_pElementList->Release();
        m_pElementList = NULL;
    }

    if(m_pAnnotationList)
    {
        m_pAnnotationList->DetachFromGraph();
        m_pAnnotationList->Release();
        m_pAnnotationList = NULL;
    }

    if(m_pCursorList)
    {
        m_pCursorList->DetachFromGraph();
        m_pCursorList->Release();
        m_pCursorList= NULL;
    }

    int i;

    for(i=0; i<2; i++)
    {
        if(m_pAxis[i])
        {
            m_pAxis[i]->DetachFromGraph();
            m_pAxis[i]->Release();
            m_pAxis[i] = NULL;
        }
    }
}

void CDMGraphCtrl::Refresh(BOOL withPenAndBrushes)
{
    if(withPenAndBrushes)
    {
        if(m_linePen)
        {
            DeleteObject(m_linePen);
            m_linePen = NULL;
        }

        if(m_pointPen)
        {
            DeleteObject(m_pointPen);
            m_pointPen = NULL;
        }

        if(m_lineBrush)
        {
            DeleteObject(m_lineBrush);
            m_lineBrush = NULL;
        }

        if(m_pointBrush)
        {
            DeleteObject(m_pointBrush);
            m_pointBrush = NULL;
        }
    }

    FireViewChange();
    m_bRequiresSave = TRUE;
}

STDMETHODIMP CDMGraphCtrl::AboutBox()
{
    CAboutDlg dlg;
    dlg.DoModal(::GetParent(m_hWnd));
    return S_OK;
}


STDMETHODIMP CDMGraphCtrl::get_AxisColor(OLE_COLOR* pVal)
{
    if(pVal == NULL)
    {
        return E_POINTER;
    }

    *pVal = m_colAxis;
    return S_OK;
}

STDMETHODIMP CDMGraphCtrl::put_AxisColor(OLE_COLOR newVal)
{
    HRESULT hr;
    COLORREF col;
    hr = OleTranslateColor(newVal, NULL, &col);

    if(m_colAxis == col)
    {
        return S_FALSE;
    }

    m_colAxis = col;

    if(m_axisPen)
    {
        DeleteObject(m_axisPen);
        m_axisPen = NULL;
    }

    FireViewChange();
    m_bRequiresSave = TRUE;
    return S_OK;
}

STDMETHODIMP CDMGraphCtrl::get_GridColor(OLE_COLOR* pVal)
{
    if(pVal == NULL)
    {
        return E_POINTER;
    }

    *pVal = m_colGrid;
    return S_OK;
}

STDMETHODIMP CDMGraphCtrl::put_GridColor(OLE_COLOR newVal)
{
    HRESULT hr;
    COLORREF col;
    hr = OleTranslateColor(newVal, NULL, &col);

    if(m_colGrid == col)
    {
        return S_FALSE;
    }

    m_colGrid = col;

    if(m_gridPen)
    {
        DeleteObject(m_gridPen);
        m_gridPen = NULL;
    }

    FireViewChange();
    m_bRequiresSave = TRUE;
    return S_OK;
}

STDMETHODIMP CDMGraphCtrl::get_LabelColor(OLE_COLOR* pVal)
{
    if(pVal == NULL)
    {
        return E_POINTER;
    }

    *pVal = m_colLabel;
    return S_OK;
}

STDMETHODIMP CDMGraphCtrl::put_LabelColor(OLE_COLOR newVal)
{
    HRESULT hr;
    COLORREF col;
    hr = OleTranslateColor(newVal, NULL, &col);

    if(m_colLabel == col)
    {
        return S_FALSE;
    }

    m_colLabel = col;
    FireViewChange();
    m_bRequiresSave = TRUE;
    return S_OK;
}

STDMETHODIMP CDMGraphCtrl::get_LabelFont(IFontDisp** pVal)
{
    if(pVal == NULL)
    {
        return E_POINTER;
    }

    *pVal = NULL;

    if(m_fontLabel == NULL)
    {
        return S_FALSE;
    }

    return m_fontLabel->QueryInterface(IID_IFontDisp, (void**)pVal);
}

STDMETHODIMP CDMGraphCtrl::put_LabelFont(IFontDisp* newVal)
{
    HRESULT hr;
    m_fontLabel.Release();

    if(newVal)
    {
        hr = newVal->QueryInterface(IID_IFont, (void**)&m_fontLabel);

        if(SUCCEEDED(hr))
        {
            return hr;
        }
    }

    hr = ::OleCreateFontIndirect(const_cast<LPFONTDESC>(&_fontdescLabel), IID_IFont,
                                 (LPVOID*)&m_fontLabel);

    if(FAILED(hr))
    {
        return hr;
    }

    OnFontChanged();    //notify any changes
    m_bRequiresSave = TRUE;
    return hr;
}

STDMETHODIMP CDMGraphCtrl::get_TickFont(IFontDisp** pVal)
{
    if(pVal == NULL)
    {
        return E_POINTER;
    }

    *pVal = NULL;

    if(m_fontTick == NULL)
    {
        return S_FALSE;
    }

    return m_fontTick->QueryInterface(IID_IFontDisp, (void**)pVal);
}

STDMETHODIMP CDMGraphCtrl::put_TickFont(IFontDisp* newVal)
{
    HRESULT hr;
    m_fontTick.Release();

    if(newVal)
    {
        hr = newVal->QueryInterface(IID_IFont, (void**)&m_fontTick);

        if(SUCCEEDED(hr))
        {
            return hr;
        }
    }

    hr = ::OleCreateFontIndirect(const_cast<LPFONTDESC>(&_fontdescTick), IID_IFont,
                                 (LPVOID*)&m_fontTick);

    if(FAILED(hr))
    {
        return hr;
    }

    OnFontChanged();    //notify any changes
    m_bRequiresSave = TRUE;
    return hr;
}

STDMETHODIMP CDMGraphCtrl::get_TitleFont(IFontDisp** pVal)
{
    if(pVal == NULL)
    {
        return E_POINTER;
    }

    *pVal = NULL;

    if(m_fontTitle == NULL)
    {
        return S_FALSE;
    }

    return m_fontTitle->QueryInterface(IID_IFontDisp, (void**)pVal);
}

STDMETHODIMP CDMGraphCtrl::put_TitleFont(IFontDisp* newVal)
{
    HRESULT hr;
    m_fontTitle.Release();

    if(newVal)
    {
        hr = newVal->QueryInterface(IID_IFont, (void**)&m_fontTitle);

        if(SUCCEEDED(hr))
        {
            return hr;
        }
    }

    hr = ::OleCreateFontIndirect(const_cast<LPFONTDESC>(&_fontdescTitle), IID_IFont,
                                 (LPVOID*)&m_fontTitle);

    if(FAILED(hr))
    {
        return hr;
    }

    OnFontChanged();    //notify any changes
    m_bRequiresSave = TRUE;
    return hr;
}

STDMETHODIMP CDMGraphCtrl::get_IdentFont(IFontDisp** pVal)
{
    if(pVal == NULL)
    {
        return E_POINTER;
    }

    *pVal = NULL;

    if(m_fontIdent == NULL)
    {
        return S_FALSE;
    }

    return m_fontIdent->QueryInterface(IID_IFontDisp, (void**)pVal);
}

STDMETHODIMP CDMGraphCtrl::put_IdentFont(IFontDisp* newVal)
{
    HRESULT hr;
    m_fontIdent.Release();

    if(newVal)
    {
        hr = newVal->QueryInterface(IID_IFont, (void**)&m_fontIdent);

        if(SUCCEEDED(hr))
        {
            return hr;
        }
    }

    hr = ::OleCreateFontIndirect(const_cast<LPFONTDESC>(&_fontdescIdent), IID_IFont,
                                 (LPVOID*)&m_fontIdent);

    if(FAILED(hr))
    {
        return hr;
    }

    OnFontChanged();    //notify any changes
    m_bRequiresSave = TRUE;
    return hr;
}

STDMETHODIMP CDMGraphCtrl::get_TrackMode(TrackModeState* pVal)
{
    if(pVal == NULL)
    {
        return E_POINTER;
    }

    *pVal = m_eTrackMode;
    return S_OK;
}

STDMETHODIMP CDMGraphCtrl::put_TrackMode(TrackModeState newVal)
{
    if(m_eTrackMode == newVal)
    {
        return S_FALSE;
    }

    m_eTrackMode = newVal;
    Corrdinate(0,0, &m_panPoint);
    m_bUnzoom = TRUE;
    dOldRangeX[MIN]=dRangeX[MIN];
    dOldRangeY[MIN]=dRangeY[MIN];
    dOldRangeX[MAX]=dRangeX[MAX];
    dOldRangeY[MAX]=dRangeY[MAX];
    Fire_TrackModeChanged(newVal);
    m_bRequiresSave = TRUE;
    return S_OK;
}

STDMETHODIMP CDMGraphCtrl::get_ElementIdentify(VARIANT_BOOL* pVal)
{
    if(pVal == NULL)
    {
        return E_POINTER;
    }

    *pVal = m_bElementIdentify;
    return S_OK;
}

STDMETHODIMP CDMGraphCtrl::put_ElementIdentify(VARIANT_BOOL newVal)
{
    m_bElementIdentify = newVal;
    FireViewChange();
    m_bRequiresSave = TRUE;
    return S_OK;
}


STDMETHODIMP CDMGraphCtrl::get_ControlFramePicture(IPictureDisp** pVal)
{
    if(pVal == NULL)
    {
        return E_POINTER;
    }

    *pVal = NULL;

    if(m_spControlFramePicture == NULL)
    {
        return S_FALSE;
    }

    return m_spControlFramePicture->QueryInterface(IID_IPictureDisp, (void**)pVal);
}

STDMETHODIMP CDMGraphCtrl::put_ControlFramePicture(IPictureDisp* newVal)
{
    m_spControlFramePicture.Release();
    HRESULT hr = S_OK;

    if(newVal)
    {
        hr = newVal->QueryInterface(IID_IPicture, (void**)&m_spControlFramePicture);
    }

    FireViewChange();
    m_bRequiresSave = TRUE;
    return hr;
}

STDMETHODIMP CDMGraphCtrl::get_PlotAreaPicture(IPictureDisp** pVal)
{
    if(pVal == NULL)
    {
        return E_POINTER;
    }

    *pVal = NULL;

    if(m_spPlotAreaPicture == NULL)
    {
        return S_FALSE;
    }

    return m_spPlotAreaPicture->QueryInterface(IID_IPictureDisp, (void**)pVal);
}

STDMETHODIMP CDMGraphCtrl::put_PlotAreaPicture(IPictureDisp* newVal)
{
    m_spPlotAreaPicture.Release();
    HRESULT hr = S_OK;

    if(newVal)
    {
        hr = newVal->QueryInterface(IID_IPicture, (void**)&m_spPlotAreaPicture);
    }

    FireViewChange();
    m_bRequiresSave = TRUE;
    return hr;
}

STDMETHODIMP CDMGraphCtrl::get_ControlFrameColor(OLE_COLOR* pVal)
{
    if(pVal == NULL)
    {
        return E_POINTER;
    }

    *pVal = m_colControlFrameColor;
    return S_OK;
}

STDMETHODIMP CDMGraphCtrl::put_ControlFrameColor(OLE_COLOR newVal)
{
    HRESULT hr;
    COLORREF col;
    hr = OleTranslateColor(newVal, NULL, &col);

    if(m_colControlFrameColor == col)
    {
        return S_FALSE;
    }

    m_colControlFrameColor = col;

    if(m_brushFrame)
    {
        DeleteObject(m_brushFrame);
        m_brushFrame = NULL;
    }

    FireViewChange();
    m_bRequiresSave = TRUE;
    return S_OK;
}

STDMETHODIMP CDMGraphCtrl::get_PlotAreaColor(OLE_COLOR* pVal)
{
    if(pVal == NULL)
    {
        return E_POINTER;
    }

    *pVal = m_colPlotAreaColor;
    return S_OK;
}

STDMETHODIMP CDMGraphCtrl::put_PlotAreaColor(OLE_COLOR newVal)
{
    HRESULT hr;
    COLORREF col;
    hr = OleTranslateColor(newVal, NULL, &col);

    if(m_colPlotAreaColor == col)
    {
        return S_FALSE;
    }

    m_colPlotAreaColor = col;

    if(m_brushPlot)
    {
        DeleteObject(m_brushPlot);
        m_brushPlot = NULL;
    }

    FireViewChange();
    m_bRequiresSave = TRUE;
    return S_OK;
}

STDMETHODIMP CDMGraphCtrl::get_FrameStyle(FrameStyle* pVal)
{
    if(pVal == NULL)
    {
        return E_POINTER;
    }

    *pVal = m_eFrameStyle;
    return S_OK;
}

STDMETHODIMP CDMGraphCtrl::put_FrameStyle(FrameStyle newVal)
{
    if(m_eFrameStyle == newVal)
    {
        return S_FALSE;
    }

    m_eFrameStyle = newVal;

    if(m_brushFrame)
    {
        DeleteObject(m_brushFrame);
        m_brushFrame = NULL;
    }

    FireViewChange();
    m_bRequiresSave = TRUE;
    return S_OK;
}


STDMETHODIMP CDMGraphCtrl::get_Annotations(IDMGraphCollection* *pVal)
{
    if(pVal == NULL)
    {
        return E_POINTER;
    }

    *pVal = m_pAnnotationList;

    if(*pVal)
    {
        (*pVal)->AddRef();
    }

    return S_OK;
}

STDMETHODIMP CDMGraphCtrl::get_Elements(IDMGraphCollection* *pVal)
{
    if(pVal == NULL)
    {
        return E_POINTER;
    }

    *pVal = m_pElementList;

    if(*pVal)
    {
        (*pVal)->AddRef();
    }

    return S_OK;
}

STDMETHODIMP CDMGraphCtrl::get_Cursors(IDMGraphCollection* *pVal)
{
    if(pVal == NULL)
    {
        return E_POINTER;
    }

    *pVal = m_pCursorList;

    if(*pVal)
    {
        (*pVal)->AddRef();
    }

    return S_OK;
}

STDMETHODIMP CDMGraphCtrl::get_Axis(Orientation eOrientation, IDMGraphAxis* *pVal)
{
    if(pVal == NULL)
    {
        return E_POINTER;
    }

    if(eOrientation < 0 || eOrientation > 1)
    {
        *pVal = NULL;
        return E_INVALIDARG;
    }

    *pVal = m_pAxis[ eOrientation ];

    if(*pVal)
    {
        (*pVal)->AddRef();
    }

    return S_OK;
}

STDMETHODIMP CDMGraphCtrl::SetRange(double xmin, double xmax, double ymin, double ymax)
{
    if (xmin==xmax || ymin==ymax)
    {
        return S_FALSE;
    }
    else if (xmin>xmax || ymin>ymax)
    {
        return S_FALSE;
    }
    else
    {
        dRangeX[MIN] = xmin ;
        dRangeX[MAX] = xmax ;
        dRangeY[MIN] = ymin ;
        dRangeY[MAX] = ymax ;
        //  m_nExpX = (int)(floor( log10(fabs(dRangeX[MAX])) ) );
        //  m_nExpY = (int)(floor( log10(fabs(dRangeY[MAX])) ) );
    }

    FireViewChange();
    return S_OK;
}

STDMETHODIMP CDMGraphCtrl::GetRange(double* xmin, double* xmax, double* ymin, double* ymax)
{
    if(xmin)
    {
        *xmin = dRangeX[MIN];
    }

    if(xmax)
    {
        *xmax = dRangeX[MAX];
    }

    if(ymin)
    {
        *ymin = dRangeY[MIN];
    }

    if(ymax)
    {
        *ymax = dRangeY[MAX];
    }

    return S_OK;
}

STDMETHODIMP CDMGraphCtrl::GetAutoRange(double* xmin, double* xmax, double* ymin, double* ymax)
{
    if(xmin)
    {
        *xmin = dAutoRangeX[MIN];
    }

    if(xmax)
    {
        *xmax = dAutoRangeX[MAX];
    }

    if(ymin)
    {
        *ymin = dAutoRangeY[MIN];
    }

    if(ymax)
    {
        *ymax = dAutoRangeY[MAX];
    }

    return S_OK;
}

STDMETHODIMP CDMGraphCtrl::AutoRange()
{
    if (m_nAvailablePlots == 0)
    {
        dAutoRangeX[MIN]=dRangeX[MIN];
        dAutoRangeY[MIN]=dRangeY[MIN];
        dAutoRangeX[MAX]=dRangeX[MAX];
        dAutoRangeY[MAX]=dRangeY[MAX];
    }

    return SetRange(dAutoRangeX[MIN],dAutoRangeX[MAX],dAutoRangeY[MIN],dAutoRangeY[MAX]);
}

STDMETHODIMP CDMGraphCtrl::ShowProperties()
{
    return DoVerbProperties(NULL, GetParent());
}

HFONT CDMGraphCtrl::SelectFontObject(HDC hdc, IFont* pFont)
{
    if(pFont == NULL)
    {
        return NULL;
    }

    HRESULT hr;
    hr = pFont->SetRatio(m_rcBounds.bottom - m_rcBounds.top, m_sizeExtent.cy);

    if(FAILED(hr))
    {
        return NULL;
    }

    HFONT hFont = NULL;
    hr = pFont->get_hFont(&hFont);

    if(FAILED(hr))
    {
        return NULL;
    }

    return (HFONT)::SelectObject(hdc, hFont);
}

void CDMGraphCtrl::OnFontChanged()
{
    if(m_fontY)
    {
        DeleteObject(m_fontY);
        m_fontY = NULL;
    }

    if(m_fontVLabel)
    {
        DeleteObject(m_fontVLabel);
        m_fontVLabel = NULL;
    }

    FireViewChange();
}


/////////////////////////////////////////////////////////////////////////////
// Reset control to default state

STDMETHODIMP CDMGraphCtrl::InitNew()
{
    IPersistStreamInitImpl<CDMGraphCtrl>::InitNew();
    HRESULT hr;
    PICTDESC pdesc;
    pdesc.cbSizeofstruct = sizeof(pdesc);
    pdesc.picType = PICTYPE_NONE;
    m_spPlotAreaPicture.Release();
    hr = OleCreatePictureIndirect(&pdesc, IID_IPicture, FALSE,
                                  (LPVOID*)&m_spPlotAreaPicture);

    if(FAILED(hr))
    {
        return hr;
    }

    m_spControlFramePicture.Release();
    hr = OleCreatePictureIndirect(&pdesc, IID_IPicture, FALSE,
                                  (LPVOID*)&m_spControlFramePicture);
    return hr;
}


/////////////////////////////////////////////////////////////////////////////
// CDMGraphCtrl message handlers
void CDMGraphCtrl::CreateFont(HDC hDC)
{
    if (m_fontY == NULL)
    {
        //Create system font
        LOGFONT lf;
        HFONT hOldFont, hLogFont;
        hOldFont  = SelectFontObject(hDC, m_fontLabel);
        hLogFont  = (HFONT)SelectObject(hDC, hOldFont);

        // Retrieve a copy of the LOGFONT structure
        // for a currently selected stock font.
        if (hLogFont)
        {
            GetObject(hLogFont, sizeof(LOGFONT), &lf);
        }

        // Rotate font 90 degree for Y axis label
        SetGraphicsMode( hDC, GM_ADVANCED );
        lf.lfOrientation = 900 ;
        lf.lfEscapement = 900 ;
        m_fontY = CreateFontIndirect(&lf);
        hOldFont  = SelectFontObject(hDC, m_fontIdent);
        hLogFont  = (HFONT)SelectObject(hDC, hOldFont);

        // Retrieve a copy of the LOGFONT structure
        // for a currently selected stock font.
        if (hLogFont)
        {
            GetObject(hLogFont, sizeof(LOGFONT), &lf);
        }

        // Rotate font 90 degree for Y axis label
        lf.lfOrientation = 900 ;
        lf.lfEscapement = 900 ;
        m_fontVLabel = CreateFontIndirect(&lf);
    }
}

void CDMGraphCtrl::PrepareForDrawing(HDC hDC, LPCRECT rect)
{
    ATLASSERT(hDC);
    ATLASSERT(rect);
    SetMapMode(hDC, MM_TEXT) ;
    SetWindowOrgEx(hDC, 0,0, NULL) ;
    SetWindowExtEx(hDC, rect->right , rect->bottom, NULL) ;
    SetViewportOrgEx(hDC, 0,0, NULL) ;
    SetViewportExtEx (hDC, rect->right , rect->bottom, NULL );
    memcpy(&m_ctlRect, rect, sizeof(RECT));
    DPtoLP(hDC, (LPPOINT)&m_ctlRect, 2);
    CreateFont(hDC); // Create system font for Y label
    CalcRect(hDC) ; // Compute rectangle
    ResCalc();  // Compute resolution per dot.
}

void CDMGraphCtrl::CalcRect(HDC hDC)
{
    int offset = 15;
    SIZE txtXLabelSize, txtYLabelSize = {0,0}, txtTitleSize ;
    /*CComBSTR str;*/
    BSTR str;
    HFONT hOldFont, hSaveFont ;
    hOldFont = SelectFontObject(hDC, m_fontTick);
    short   nGridY;
    m_pAxis[ VerticalY ]->get_GridNumber(&nGridY);
    // Calculate Y Label sizes.
    double res = (dRangeY[MAX] - dRangeY[MIN]) / nGridY ;
    SIZE sz = {0,0};

    for (int i = 0 ; i <= nGridY ; i++)
    {
        double y = dRangeY[MIN] + (res * (double)i) ;
        m_pAxis[ VerticalY ]->FormatAxisLabel(y, &str);
        /*GetTextExtentPoint32W(hDC, str, str.Length(), &sz);*/
        GetTextExtentPoint32W(hDC, str, SysStringLen(str), &sz);

        if (txtYLabelSize.cx < sz.cx)
        {
            txtYLabelSize = sz;
        }

        SysFreeString(str);
    }

    //Calculate X Label sizes.
    /*str.Empty();*/
    m_pAxis[ HorizontalX ]->FormatAxisLabel(dRangeX[MAX], &str);
    /*GetTextExtentPoint32W(hDC, str, str.Length(), &txtXLabelSize);*/
    GetTextExtentPoint32W(hDC, str, SysStringLen(str), &txtXLabelSize);
    SelectObject(hDC, hOldFont);
    SysFreeString(str);
    // AT 10.07.2003 original code: pSaveFont = SelectFontObject(hDC,m_fontTick);
    // AT 10.07.2003 Begin
    // the old code selected the wrong font
    // and the wrong String to define the top-border
    hSaveFont = SelectFontObject(hDC, m_fontTitle);
    DWORD dwCaptionLength = m_bstrCaption.Length();

    if(dwCaptionLength)
    {
        GetTextExtentPoint32W(hDC, m_bstrCaption, dwCaptionLength, &txtTitleSize);
    }
    else
    {
        ZeroMemory(&txtTitleSize, sizeof(SIZE));
    }

    // AT 10.07.2003 End
    // AT 10.07.2003 original: txtTitleSize = pDC->GetTextExtent(m_bsXLabel);
    SelectObject(hDC, hOldFont);
    m_axisRect.left     = m_ctlRect.left    + txtTitleSize.cy + txtYLabelSize.cx + offset;
    m_axisRect.right    = m_ctlRect.right   - (txtXLabelSize.cx/2) - offset;
    m_axisRect.top      = m_ctlRect.top     + txtTitleSize.cy + offset;
    m_axisRect.bottom   = m_ctlRect.bottom  - (txtTitleSize.cy + txtXLabelSize.cy) - offset;
}



////////////////////////////////
//Calculate resolution per dot
void CDMGraphCtrl::ResCalc()
{
    double dpixelx, dpixely ;
    dpixelx = (double)(m_axisRect.right - m_axisRect.left);
    dpixely = (double)(m_axisRect.bottom - m_axisRect.top);
    dResY = (dRangeY[MAX] - dRangeY[MIN]) / dpixely ;
    dResX = (dRangeX[MAX] - dRangeX[MIN]) / dpixelx ;
}

void CDMGraphCtrl::Corrdinate(double x, double y, LPPOINT pt)
{
    ATLASSERT(pt);
    double rx , ry ;
    int xPixel , yPixel ;
    rx = x - dRangeX[MIN] ; // Calculate horizontal offset from origin
    ry = y - dRangeY[MIN]; // Calculate vertical offset from origin .
    // Convert offset to be number of pixel on screen .
    xPixel = (int)(rx / dResX) ;
    yPixel = (int)(ry / dResY) ;
    //Calulate point to be drawn .
    pt->x = xPixel + m_axisRect.left ;
    pt->y = m_axisRect.bottom - yPixel;
}

void CDMGraphCtrl::Corrdinate(CElementPoint ptElem, LPPOINT pt)
{
    double rx , ry ;
    int xPixel , yPixel ;
    rx = ptElem.x - dRangeX[MIN] ; // Calculate horizontal offset from origin
    ry = ptElem.y - dRangeY[MIN]; // Calculate vertical offset from origin .
    // Convert offset to be number of pixel on screen .
    xPixel = (int)(rx / dResX) ;
    yPixel = (int)(ry / dResY) ;
    //Calulate point to be drawn .
    pt->x = xPixel + m_axisRect.left ;
    pt->y = m_axisRect.bottom - yPixel;
}

double CDMGraphCtrl::RoundUp( double val, int ex )
{
    int exponent;
    int i;
    exponent = (int)(floor( log10(fabs(val)) ) );

    if (ex==0)
    {
        if( exponent < 0 )
            for(i=exponent; i<0; i++)
            {
                val *= 10.0;
            }
        else
            for(i=0; i<exponent; i++)
            {
                val /= 10.0;
            }
    }
    else
    {
        if( ex < 0 )
            if (exponent<0)
                for(i=ex; i<0; i++)
                {
                    val *= 10.0;
                }
            else
            {
                return val;
            }
        else if (exponent<0)
        {
            return val;
        }
        else
            for(i=0; i<ex; i++)
            {
                val /= 10.0;
            }
    }

    return val;
}
///////////////////////////////////////////////////////////////////////////////////////

void CDMGraphCtrl::DrawGraphOffScreen(HDC hDC, LPCRECT prcBounds, BOOL bOptimized)
{
    ATLASSERT(hDC);
    ATLASSERT(prcBounds);
    HDC dcMem;
    HBITMAP bitOff;
    RECT rcBoundsDP;
    memcpy(&rcBoundsDP, prcBounds, sizeof(RECT)) ;
    // Convert bounds to device units.
    LPtoDP(hDC, (LPPOINT)&rcBoundsDP, 2) ;
    // The bitmap bounds have 0,0 in the upper-left corner.
    RECT rcBitmapBounds = {0,0, rcBoundsDP.right-rcBoundsDP.left, rcBoundsDP.bottom - rcBoundsDP.top };
    // Create a DC that is compatible with the screen.
    //dcMem.CreateCompatibleDC(&screenDC) ;
    dcMem = CreateCompatibleDC(hDC) ;
    // Create a really compatible bitmap.
    bitOff = CreateCompatibleBitmap(hDC,
                                    rcBitmapBounds.right-rcBitmapBounds.left, rcBitmapBounds.bottom-rcBitmapBounds.top);
    // Select the bitmap into the memory DC.
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(dcMem, bitOff) ;
    // Save the memory DC state, since DrawMe might change it.
    int iSavedDC = SaveDC(dcMem);
    // Draw our control on the memory DC.
    DrawGraph(dcMem, &rcBitmapBounds, bOptimized) ;
    // Restore the DC, since DrawMe might have changed mapping modes.
    RestoreDC(dcMem, iSavedDC) ;
    // We don't know what mapping mode pdc is using.
    // BitBlt uses logical coordinates.
    // Easiest thing is to change to MM_TEXT.
    SetMapMode(hDC, MM_TEXT) ;
    SetWindowOrgEx(hDC, 0,0, NULL) ;
    SetViewportOrgEx(hDC, 0,0, NULL) ;
    // Blt the memory device context to the screen.
    BitBlt(hDC, rcBoundsDP.left,
           rcBoundsDP.top,
           rcBoundsDP.right - rcBoundsDP.left,
           rcBoundsDP.bottom - rcBoundsDP.top,
           dcMem,
           0,
           0,
           SRCCOPY) ;
    // Clean up.
    SelectObject(dcMem, hOldBitmap) ;
    DeleteObject(bitOff);
    DeleteDC(dcMem);
}

void CDMGraphCtrl::DrawGraph(HDC hdc, LPCRECT prcBounds, BOOL bOptimized)
{
    ATLASSERT(hdc);
    ATLASSERT(prcBounds);
    // Set control and axis rectangles
    PrepareForDrawing(hdc, prcBounds);
    DrawControlFrame(hdc, bOptimized);
    DrawTitle(hdc);
    VARIANT_BOOL bXLog, bYLog, bShowXGrid, bShowYGrid;
    m_pAxis[ HorizontalX ]->get_Log(&bXLog);
    m_pAxis[ VerticalY  ]->get_Log(&bYLog);
    m_pAxis[ HorizontalX ]->get_ShowGrid(&bShowXGrid);
    m_pAxis[ VerticalY  ]->get_ShowGrid(&bShowYGrid);

    if (bShowXGrid || bShowYGrid)
        if (bXLog || bYLog)
        {
            if(bXLog && bShowXGrid)
            {
                DrawXLogGrid(hdc, bOptimized);
            }

            if(bYLog && bShowYGrid)
            {
                DrawYLogGrid(hdc, bOptimized);
            }

            DrawGrid(hdc, bOptimized);
        }
        else
        {
            DrawGrid(hdc, bOptimized);
        }
    else if (bXLog || bYLog)
    {
        if(bXLog)
        {
            DrawXLogGrid(hdc, bOptimized);
        }

        if(bYLog)
        {
            DrawYLogGrid(hdc, bOptimized);
        }

        DrawTicks(hdc, bOptimized);
    }
    else
    {
        DrawTicks(hdc, bOptimized);
    }

    if(m_eFrameStyle == Flat)
    {
        DrawAxis(hdc, bOptimized);
    }

    DrawAxisLabel(hdc);

    if (m_nAvailablePlots)
    {
        PlotElement(hdc, bOptimized);
    }

    if (m_pCursorList->GetSize() )
    {
        DrawCursor(hdc, bOptimized);
    }

    if(m_pAnnotationList->GetSize() )
    {
        Annotate(hdc);
    }
}
//////////////////////////////////////////////////////////
// Draw Control Frame
void CDMGraphCtrl::DrawControlFrame(HDC hDC, BOOL bOptimized)
{
    if (m_brushFrame == NULL)
    {
        m_brushFrame = CreateSolidBrush(m_colControlFrameColor);
    }

    if (m_brushPlot == NULL)
    {
        m_brushPlot = CreateSolidBrush(m_colPlotAreaColor);
    }

    HBRUSH hBrushSave = (HBRUSH)SelectObject(hDC, m_brushFrame);
    Rectangle(hDC, m_ctlRect.left, m_ctlRect.top, m_ctlRect.right, m_ctlRect.bottom);

    // Draw frame border
    if(m_eFrameStyle == Scope)
    {
        ::DrawEdge( hDC, &m_ctlRect, EDGE_RAISED, BF_RECT);
    }

    HRESULT hr;

    // Draw Bitmap Frame
    // TODO To maket it transparent!
    if(m_eFrameStyle == Bitmap && m_spPicFrame)
    {
        long hmWidth;
        long hmHeight;
        hr = m_spPicFrame->get_Width(&hmWidth);
        hr = m_spPicFrame->get_Height(&hmHeight);
        hr = m_spPicFrame->Render(hDC, m_ctlRect.left, m_ctlRect.top, m_ctlRect.right - m_ctlRect.left, m_ctlRect.bottom - m_ctlRect.top,
                                  0, hmHeight-1, hmWidth, -hmHeight, &m_ctlRect);
    }

    // Render the frame picture
    if(m_spControlFramePicture)
    {
        long hmWidth;
        long hmHeight;
        hr = m_spPicFrame->get_Width(&hmWidth);
        hr = m_spPicFrame->get_Height(&hmHeight);
        hr = m_spControlFramePicture->Render(hDC, m_ctlRect.left, m_ctlRect.top, m_ctlRect.right - m_ctlRect.left, m_ctlRect.bottom - m_ctlRect.top,
                                             0, hmHeight-1, hmWidth, -hmHeight, &m_ctlRect);
    }

    // Draw plot area background
    SelectObject(hDC, m_brushPlot);
    Rectangle(hDC, m_axisRect.left, m_axisRect.top, m_axisRect.right, m_axisRect.bottom);

    // Draw plot area border
    if(m_eFrameStyle != Flat )
    {
        ::DrawEdge( hDC, &m_axisRect, EDGE_SUNKEN, BF_RECT);
    }

    // Render the plot area picture
    if(m_spPlotAreaPicture)
    {
        long hmWidth;
        long hmHeight;
        hr = m_spPicFrame->get_Width(&hmWidth);
        hr = m_spPicFrame->get_Height(&hmHeight);
        hr = m_spPlotAreaPicture->Render(hDC, m_axisRect.left, m_axisRect.top, m_axisRect.right - m_axisRect.left, m_axisRect.bottom - m_axisRect.top,
                                         0, hmHeight-1, hmWidth, -hmHeight, &m_axisRect);
    }

    if (! bOptimized)
    {
        SelectObject(hDC, hBrushSave);
    }
}

void CDMGraphCtrl::DrawAxis(HDC hDC, BOOL bOptimized)
{
    if (m_axisPen == NULL)
    {
        m_axisPen = CreatePen(PS_SOLID, 2, m_colAxis);
    }

    HPEN hPenSave = (HPEN)SelectObject(hDC, m_axisPen);
    POINT pts[8];
    Corrdinate(dRangeX[MIN], dRangeY[MIN], &pts[0]);
    Corrdinate(dRangeX[MAX],dRangeY[MIN], &pts[1]);
    Corrdinate(dRangeX[MIN],dRangeY[MIN], &pts[2]);
    Corrdinate(dRangeX[MIN],dRangeY[MAX], &pts[3]);
    Corrdinate(dRangeX[MAX],dRangeY[MIN], &pts[4]);
    Corrdinate(dRangeX[MAX],dRangeY[MAX], &pts[5]);
    Corrdinate(dRangeX[MIN],dRangeY[MAX], &pts[6]);
    Corrdinate(dRangeX[MAX],dRangeY[MAX], &pts[7]);
    MoveToEx(hDC, pts[0].x, pts[0].y, NULL);
    LineTo(  hDC, pts[1].x, pts[1].y);
    MoveToEx(hDC, pts[2].x, pts[2].y, NULL);
    LineTo(  hDC, pts[3].x, pts[3].y);
    MoveToEx(hDC, pts[4].x, pts[4].y, NULL);
    LineTo(  hDC, pts[5].x, pts[5].y);
    MoveToEx(hDC, pts[6].x, pts[6].y, NULL);
    LineTo(  hDC, pts[7].x, pts[7].y);

    if (! bOptimized)
    {
        SelectObject(hDC, hPenSave);
    }
}


//////////////////////////////////////////
// Draw Y log ticks and grid
void CDMGraphCtrl::DrawYLogGrid(HDC hDC, BOOL bOptimized)
{
    // Change this number for changing number of fine scales.
    const int FINE_SCALE = 5 ;
    POINT m_start, m_stop ;
    int i  ;
    int j ;
    double y ;
    double step ;
    double Ymax , Ymin ;
    Ymax = dRangeY[MAX] ;
    Ymin = dRangeY[MIN] ;
    short   nGridY;
    VARIANT_BOOL bShowYGrid;
    m_pAxis[ VerticalY ]->get_GridNumber(&nGridY);
    m_pAxis[ VerticalY  ]->get_ShowGrid(&bShowYGrid);
    //Remap scale to 0 - nGridY
    dRangeY[MIN] = 0;
    dRangeY[MAX] = nGridY;
    ResCalc();

    if (m_gridPen == NULL)
    {
        m_gridPen = CreatePen(PS_SOLID, 0, m_colGrid);
    }

    HPEN hPenSave = (HPEN)SelectObject(hDC, m_gridPen);

    for( j = (int)dRangeY[MIN] ; j < (int)dRangeY[MAX] ; j ++)
    {
        y = (double)j;
        step = (pow(10,y+1) - pow(10,y)) /(double)FINE_SCALE ;

        for(i = 0 ; i < FINE_SCALE ; i++ )
        {
            y = log10(pow(10,y) + step) ;
            Corrdinate(dRangeX[MIN], y, &m_start) ;
            Corrdinate(dRangeX[MAX], y, &m_stop) ;

            if(!bShowYGrid)
            {
                m_stop = m_start;

                if(m_eFrameStyle == Flat)
                {
                    m_stop.x+=5;    // Draw log ticks only
                }
                else
                {
                    m_stop.x-=5;
                }
            }

            MoveToEx(hDC, m_start.x, m_start.y, NULL);
            LineTo(hDC, m_stop.x, m_stop.y);
        }
    }

    if(m_eFrameStyle == Flat) // Draw log ticks on right axis
    {
        for( j = (int)dRangeY[MIN] ; j < (int)dRangeY[MAX] ; j ++)
        {
            y = (double)j;
            step = (pow(10,y+1) - pow(10,y)) /(double)FINE_SCALE ;

            for(i = 0 ; i < FINE_SCALE ; i++ )
            {
                y = log10(pow(10,y) + step) ;
                Corrdinate(dRangeX[MIN], y, &m_start) ;
                Corrdinate(dRangeX[MAX], y, &m_stop) ;

                if(!bShowYGrid)
                {
                    m_start  = m_stop;
                    m_stop.x -= 5;
                    MoveToEx(hDC, m_start.x, m_start.y, NULL);
                    LineTo(hDC, m_stop.x, m_stop.y);
                }
            }
        }
    }

    dRangeY[MIN] = Ymin;
    dRangeY[MAX] = Ymax;
    ResCalc();

    if (! bOptimized)
    {
        SelectObject(hDC, hPenSave);
    }
}

///////////////////////////////////////////
// Draw X log grid and ticks
void CDMGraphCtrl::DrawXLogGrid(HDC hDC, BOOL bOptimized)
{
    // Change this number for changing number of fine scales.
    const int FINE_SCALE = 5 ;
    POINT m_start, m_stop ;
    int i  ;
    int j ;
    double x ;
    double step ;
    double Xmax , Xmin ;
    Xmax = dRangeX[MAX] ;
    Xmin = dRangeX[MIN] ;
    short   nGridX;
    VARIANT_BOOL bShowXGrid;
    m_pAxis[ HorizontalX ]->get_GridNumber(&nGridX);
    m_pAxis[ HorizontalX ]->get_ShowGrid(&bShowXGrid);
    //Remap scale to 0 - nGridX
    dRangeX[MIN] = 0;
    dRangeX[MAX] = nGridX;
    ResCalc();

    if (m_gridPen == NULL)
    {
        m_gridPen = CreatePen(PS_SOLID, 0, m_colGrid);
    }

    HPEN hPenSave = (HPEN)SelectObject(hDC, m_gridPen);

    for( j = (int)dRangeX[MIN] ; j < (int)dRangeX[MAX] ; j ++)
    {
        x = (double)j;
        step = (pow(10,x+1) - pow(10,x)) /(double)FINE_SCALE ;

        for(i = 0 ; i < FINE_SCALE ; i++ )
        {
            x = log10(pow(10,x) + step) ;
            Corrdinate(x, dRangeY[MIN], &m_start) ;
            Corrdinate(x, dRangeY[MAX], &m_stop) ;

            if(!bShowXGrid)
            {
                m_stop = m_start;

                if(m_eFrameStyle==Flat)
                {
                    m_stop.y-=5 ;    // Draw log ticks only
                }
                else
                {
                    m_stop.y+=5 ;
                }
            }

            MoveToEx(hDC, m_start.x, m_start.y, NULL);
            LineTo(hDC, m_stop.x, m_stop.y);
        }
    }

    if(m_eFrameStyle == Flat) // Draw log ticks on top axis
    {
        for( j = (int)dRangeX[MIN] ; j < (int)dRangeX[MAX] ; j ++)
        {
            x = (double)j;
            step = (pow(10,x+1) - pow(10,x)) /(double)FINE_SCALE ;

            for(i = 0 ; i < FINE_SCALE ; i++ )
            {
                x = log10(pow(10,x) + step) ;
                Corrdinate(x, dRangeY[MIN], &m_start) ;
                Corrdinate(x, dRangeY[MAX], &m_stop) ;

                if(!bShowXGrid)
                {
                    m_start = m_stop;
                    m_stop.y+=5 ;
                    MoveToEx(hDC, m_start.x, m_start.y, NULL);
                    LineTo(hDC, m_stop.x, m_stop.y);
                }
            }
        }
    }

    dRangeX[MIN] = Xmin;
    dRangeX[MAX] = Xmax;
    ResCalc();

    if (! bOptimized)
    {
        SelectObject(hDC, hPenSave);
    }
}

//////////////////////////////////////
// Draw linear scale ticks
void CDMGraphCtrl::DrawTicks(HDC hDC, BOOL bOptimized)
{
    POINT m_start, m_stop ;
    int i  ;
    double x ,y ;
    double step ;

    if (m_gridPen == NULL)
    {
        m_gridPen = CreatePen(PS_SOLID, 0, m_colGrid);
    }

    HPEN hPenSave = (HPEN)SelectObject(hDC, m_gridPen);
    VARIANT_BOOL    bXLog, bYLog;
    m_pAxis[ HorizontalX ]->get_Log(&bXLog);
    m_pAxis[ VerticalY   ]->get_Log(&bYLog);

    // Draw vertical Major and Minor Ticks
    if(!bXLog)
    {
        short           nGridX;
        m_pAxis[ HorizontalX ]->get_GridNumber(&nGridX);
        step = (dRangeX[MAX] - dRangeX[MIN]) / (double)nGridX ;

        for( i = 1 ; i < nGridX ; i++ )     // Major Ticks on Botom
        {
            x = dRangeX[MIN] + (step * (double)i) ;
            Corrdinate(x, dRangeY[MIN], &m_start);
            m_stop = m_start;

            if(m_eFrameStyle==Flat)
            {
                m_stop.y -= 6;
            }
            else
            {
                m_stop.y += 6;
            }

            MoveToEx(hDC, m_start.x, m_start.y, NULL);
            LineTo(hDC, m_stop.x, m_stop.y);
        }

        for( i = 1 ; i < nGridX*3 ; i++ )       // Minor Ticks on Botom
        {
            x = dRangeX[MIN] + (step * (double)i)/3 ;
            Corrdinate(x, dRangeY[MIN], &m_start);
            m_stop = m_start;

            if(m_eFrameStyle==Flat)
            {
                m_stop.y -= 4;
            }
            else
            {
                m_stop.y += 4;
            }

            MoveToEx(hDC, m_start.x, m_start.y, NULL);
            LineTo(hDC, m_stop.x, m_stop.y);
        }

        if(m_eFrameStyle == Flat)
        {
            for( i = 1 ; i < nGridX ; i++ )     // Major Ticks on Top
            {
                x = dRangeX[MIN] + (step * (double)i) ;
                Corrdinate(x, dRangeY[MAX], &m_start);
                m_stop = m_start;
                m_stop.y += 6;
                MoveToEx(hDC, m_start.x, m_start.y, NULL);
                LineTo(hDC, m_stop.x, m_stop.y);
            }

            for( i = 1 ; i < nGridX*3 ; i++ )       // Minor Ticks on Top
            {
                x = dRangeX[MIN] + (step * (double)i)/3 ;
                Corrdinate(x, dRangeY[MAX], &m_start);
                m_stop = m_start;
                m_stop.y += 4;
                MoveToEx(hDC, m_start.x, m_start.y, NULL);
                LineTo(hDC, m_stop.x, m_stop.y);
            }
        }
    }

    // Draw horizontal Ticks
    if(!bYLog)
    {
        short           nGridY;
        m_pAxis[ VerticalY ]->get_GridNumber(&nGridY);
        step = (dRangeY[MAX] - dRangeY[MIN]) / (double)nGridY ;

        for( i = 1 ; i < nGridY ; i++ )     // Major Ticks on Left
        {
            y = dRangeY[MIN] + (step * (double)i) ;
            Corrdinate(dRangeX[MIN], y, &m_start) ;
            m_stop  = m_start;

            if (m_eFrameStyle == Flat)
            {
                m_stop.x += 6;
            }
            else
            {
                m_stop.x -= 6;
            }

            MoveToEx(hDC, m_start.x, m_start.y, NULL);
            LineTo(hDC, m_stop.x, m_stop.y);
        }

        for( i = 1 ; i < nGridY*3 ; i++ )       // Minor Ticks on Left
        {
            y = dRangeY[MIN] + (step * (double)i)/3 ;
            Corrdinate(dRangeX[MIN], y, &m_start) ;
            m_stop  = m_start;

            if (m_eFrameStyle==Flat)
            {
                m_stop.x += 4;
            }
            else
            {
                m_stop.x -= 4;
            }

            MoveToEx(hDC, m_start.x, m_start.y, NULL);
            LineTo(hDC, m_stop.x, m_stop.y);
        }

        if(m_eFrameStyle == Flat) // only for plots
        {
            for( i = 1 ; i < nGridY ; i++ )     // Major Ticks on Right
            {
                y = dRangeY[MIN] + (step * (double)i) ;
                Corrdinate(dRangeX[MAX], y, &m_start) ;
                m_stop  = m_start;
                m_stop.x -= 6;
                MoveToEx(hDC, m_start.x, m_start.y, NULL);
                LineTo(hDC, m_stop.x, m_stop.y);
            }

            for( i = 1 ; i < nGridY*3 ; i++ )       // Minor Ticks on Right
            {
                y = dRangeY[MIN] + (step * (double)i)/3 ;
                Corrdinate(dRangeX[MAX], y, &m_start) ;
                m_stop  = m_start;
                m_stop.x -= 4;
                MoveToEx(hDC, m_start.x, m_start.y, NULL);
                LineTo(hDC, m_stop.x, m_stop.y);
            }
        }
    }

    if (! bOptimized)
    {
        SelectObject(hDC, hPenSave);
    }
}
/////////////////////////////////////
// Draw linear grid
void CDMGraphCtrl::DrawGrid(HDC hDC, BOOL bOptimized)
{
    POINT m_start, m_stop ;
    int i  ;
    double x ,y ;
    double step ;

    if (m_gridPen == NULL)
    {
        m_gridPen = CreatePen(PS_SOLID, 0, m_colGrid);
    }

    HPEN hPenSave = (HPEN)SelectObject(hDC, m_gridPen);
    VARIANT_BOOL    bXLog, bYLog, bShowXGrid, bShowYGrid;
    m_pAxis[ HorizontalX ]->get_Log(&bXLog);
    m_pAxis[ VerticalY ]->get_Log(&bYLog);
    m_pAxis[ HorizontalX ]->get_ShowGrid(&bShowXGrid);
    m_pAxis[ VerticalY  ]->get_ShowGrid(&bShowYGrid);

    // Draw vertical grid
    if(!bXLog && bShowXGrid)
    {
        short nGridX;
        m_pAxis[ HorizontalX ]->get_GridNumber(&nGridX);
        step = (dRangeX[MAX] - dRangeX[MIN]) / (double)nGridX ;

        for( i = 1 ; i < nGridX ; i++ )
        {
            x = dRangeX[MIN] + (step * (double)i) ;
            Corrdinate(x, dRangeY[MIN], &m_start);
            Corrdinate(x, dRangeY[MAX], &m_stop);
            MoveToEx(hDC, m_start.x, m_start.y, NULL);
            LineTo(hDC, m_stop.x, m_stop.y);
        }
    }

    // Draw horizontal grid.
    if(!bYLog && bShowYGrid)
    {
        short nGridY;
        m_pAxis[ VerticalY ]->get_GridNumber(&nGridY);
        step = (dRangeY[MAX] - dRangeY[MIN]) / (double)nGridY ;

        for( i = 1 ; i < nGridY ; i++ )
        {
            y = dRangeY[MIN] + (step * (double)i) ;
            Corrdinate(dRangeX[MIN], y, &m_start) ;
            Corrdinate(dRangeX[MAX], y, &m_stop) ;
            MoveToEx(hDC, m_start.x, m_start.y, NULL);
            LineTo(hDC, m_stop.x, m_stop.y);
        }
    }

    if (! bOptimized)
    {
        SelectObject(hDC, hPenSave);
    }
}

void CDMGraphCtrl::DrawTitle(HDC hDC)
{
    HFONT hOldFont;
    TEXTMETRIC tm;

    if(m_bstrCaption.Length() == 0)
    {
        return;
    }

    hOldFont = SelectFontObject (hDC, m_fontTitle);
    SetTextColor (hDC, m_colLabel);
    GetTextMetrics(hDC, &tm);
    SetTextAlign(hDC, TA_TOP | TA_CENTER);
    SetBkMode(hDC, TRANSPARENT);
    TextOutW(hDC, (m_ctlRect.left + m_ctlRect.right) / 2,
             (m_ctlRect.top+m_axisRect.top - tm.tmHeight) / 2,
             m_bstrCaption, SysStringLen(m_bstrCaption));
    SelectObject(hDC, hOldFont);
}

void CDMGraphCtrl::DrawAxisLabel(HDC hDC)
{
    HFONT hOldFont;
    int i ;
    double res , x, y  ;
    POINT point ;
    SIZE txtSize ;
    CComBSTR str;
    SetTextColor (hDC, m_colLabel);
    SetTextAlign(hDC, TA_RIGHT);
    SetBkMode(hDC, TRANSPARENT);
    ////////////////////////////////////////////////
    hOldFont  = SelectFontObject(hDC, m_fontTick);
    ////////////////////////////////////////////////
    short nGridX, nGridY;
    m_pAxis[ HorizontalX ]->get_GridNumber(&nGridX);
    m_pAxis[ VerticalY ]->get_GridNumber(&nGridY);
    // Draw Y Tick Labels
    res = (dRangeY[MAX] - dRangeY[MIN]) / nGridY ;
    SIZE sz = {0,0};

    for ( i = 0 ; i <= nGridY ; i++)
    {
        y = dRangeY[MIN] + (res * (double)i) ;
        Corrdinate(dRangeX[MIN], y, &point) ;
        m_pAxis[ VerticalY ]->FormatAxisLabel(y, &str);
        GetTextExtentPoint32W(hDC, str, str.Length(), &txtSize);

        if (sz.cx < txtSize.cx)
        {
            sz=txtSize;
        }

        point.x -= 5 ;
        point.y -= txtSize.cy/2 ;
        TextOutW(hDC, point.x, point.y, str, str.Length()) ;
        /*str.Empty();*/
        SysFreeString(str);
    }

    // Draw X Tick Labels
    res = (dRangeX[MAX] - dRangeX[MIN]) / nGridX ;

    for ( i = 0 ; i <= nGridX ; i ++ )
    {
        x = dRangeX[MIN] + ( res * (double)i);
        Corrdinate(x, dRangeY[MIN], &point);
        m_pAxis[ HorizontalX ]->FormatAxisLabel(x, &str);
        GetTextExtentPoint32W(hDC, str, str.Length(), &txtSize);
        point.x += txtSize.cx/2;
        point.y += 5 ;
        TextOutW(hDC, point.x,point.y, str, str.Length());
        /*str.Empty();*/
        SysFreeString(str);
    }

    ////////////////////////////////////////////////
    SelectFontObject(hDC, m_fontLabel);
    ////////////////////////////////////////////////
    CComBSTR bsXLabel, bsYLabel;
    m_pAxis[ HorizontalX ]->get_Label(&bsXLabel);
    m_pAxis[ VerticalY   ]->get_Label(&bsYLabel);
    // Draw X Axis Title
    GetTextExtentPoint32W(hDC, bsXLabel, bsXLabel.Length(), &txtSize);
    point.x = (m_axisRect.left + m_axisRect.right) / 2 + (txtSize.cx / 2);
    point.y = (m_ctlRect.bottom + m_axisRect.bottom)/2;
    point.y -= (txtSize.cy - sz.cy)/2;
    TextOutW(hDC, point.x, point.y, bsXLabel, bsXLabel.Length());
    //////////////////////////////////////////////
    SelectObject(hDC, m_fontY);
    //////////////////////////////////////////////
    // Draw Y Axis Title
    GetTextExtentPoint32W(hDC, bsYLabel, bsYLabel.Length(), &txtSize);
    point.x = (m_ctlRect.left + m_axisRect.left) / 2;
    point.x -= ( txtSize.cy + sz.cx )/2 ;
    point.y = (m_axisRect.top + m_axisRect.bottom)/2  - (txtSize.cx/2);
    TextOutW(hDC, point.x,point.y, bsYLabel, bsYLabel.Length());
    SelectObject(hDC, hOldFont);
}

/////////////////////////////////////////
// Plot all elements
void CDMGraphCtrl::PlotElement(HDC hDC, BOOL bOptimized)
{
    // Prevent plotting of non-existing data
    if (m_nAvailablePlots == 0)
    {
        return;
    }

    int i;

    for(i=0; i<m_pElementList->GetSize(); i++)
    {
        CGraphElement* pElement = (*m_pElementList)[i];
        ATLASSERT(pElement);

        // Added by A.Hofmann
        //////////////////////////////////
        // Check show state of the element
        if (! pElement->m_bShow )
        {
            continue;
        }

        if (pElement->m_nType != Null)
        {
            PlotElementLines( hDC, pElement, bOptimized );
        }

        if (pElement->m_nSymbol != Nosym)
        {
            PlotElementPoints( hDC, pElement, bOptimized );
        }

        if (m_bElementIdentify)
        {
            DrawElementLabel( hDC, pElement, i+1 );    // i+1: offset for annotation strings
        }
    }
}

////////////////////////////////////
// Draw element's annotation
void CDMGraphCtrl::DrawElementLabel(HDC hDC, CGraphElement* pElement, int index)
{
    ATLASSERT(pElement);
    HFONT hOldFont;
    TEXTMETRIC tm;
    CComBSTR strCaption = pElement->m_strName;
    hOldFont = SelectFontObject (hDC, m_fontIdent);
    SetTextColor (hDC, pElement->m_LineColor);
    GetTextMetrics(hDC, &tm);
    SetTextAlign(hDC, TA_TOP | TA_CENTER);
    SetBkMode(hDC, TRANSPARENT);
    RECT rect;
    memcpy(&rect, &m_axisRect, sizeof(RECT));
    rect.left += tm.tmAveCharWidth * strCaption.Length();
    rect.top  += tm.tmHeight * index;
    TextOutW(hDC, rect.left, rect.top, strCaption, strCaption.Length());
    SelectObject(hDC, hOldFont);
}
////////////////////////////////////
// Draw Lines
void CDMGraphCtrl::PlotElementLines(HDC hDC, CGraphElement* pElement, BOOL bOptimized)
{
    ATLASSERT(pElement);

    if(pElement->m_PointList.GetSize() == 0)
    {
        return;
    }

    // Create the new pen as the color of element line
    if (m_linePen == NULL)
        m_linePen = CreatePen (pElement->m_nType,
                               pElement->m_nLineWidth,
                               pElement->m_LineColor);

    HPEN hPenSave = (HPEN)SelectObject(hDC, m_linePen);

    // Create the new pen as the color of element symbol
    if (m_pointPen == NULL)
    {
        m_pointPen = CreatePen (0, 0, pElement->m_PointColor);
    }

    // Create the new brush as the color of element symbol
    if (m_lineBrush == NULL)
    {
        m_lineBrush = CreateSolidBrush (pElement->m_PointColor);
    }

    HBRUSH hBrushSave = (HBRUSH)SelectObject(hDC, m_lineBrush);
    RECT BoundingRect; // Object to store bounding rectangle
    memcpy(&BoundingRect, &m_axisRect, sizeof(RECT)); // Store the bounding rectangle
    // Decrease the rectangle by the pen width
    InflateRect(&BoundingRect, -pElement->m_nLineWidth, -pElement->m_nLineWidth);
    //BoundingRect.NormalizeRect ();
    // Lock the plot access
    EnterCriticalSection(&pElement->m_CritSectionForPlot);
    POINT point;
    CElementPoint pt;
    VARIANT_BOOL bXLog, bYLog;
    POINT oldpt;

    //Proceed only if it contains atleast 2 points
    if ( pElement->m_PointList.GetSize() > 1 )
    {
        pt = pElement->m_PointList[0];
        m_pAxis[ HorizontalX ]->get_Log(&bXLog);
        m_pAxis[ VerticalY  ]->get_Log(&bYLog);

        if(bXLog)
        {
            pt.x = log10(pt.x);
        }

        if(bYLog)
        {
            pt.y = log10(pt.y);
        }

        // calculate the corrdinate of ploting point.
        point;
        Corrdinate(pt, &point);

        // Clip the ploting area if it exceed ranged .
        if (point.x < BoundingRect.left)
        {
            point.x = BoundingRect.left;
        }

        if (point.y < BoundingRect.top)
        {
            point.y = BoundingRect.top;
        }

        if (point.y > BoundingRect.bottom)
        {
            point.y = BoundingRect.bottom;
        }

        MoveToEx (hDC, point.x, point.y, NULL);
        oldpt;
        memcpy(&oldpt, &point, sizeof(POINT));
    }

    //Start plot all available data.
    int i;

    for(i=1; i<pElement->m_PointList.GetSize(); i++)
    {
        //pt = pElement->m_PointList[i];
        pt.x = pElement->m_PointList[i].x;
        pt.y = pElement->m_PointList[i].y;

        if(bXLog)
        {
            pt.x = log10(pt.x);
        }

        if(bYLog)
        {
            pt.y = log10(pt.y);
        }

        Corrdinate(pt, &point);
        POINT p;
        RECT rect;

        // Clip the ploting area if it exceed ranged .
        if (point.x>BoundingRect.right)
        {
            point.x=BoundingRect.right;
        }

        if (point.x<BoundingRect.left)
        {
            point.x=BoundingRect.left;
        }

        if (point.y<BoundingRect.top)
        {
            point.y=BoundingRect.top;
        }

        if (point.y>BoundingRect.bottom)
        {
            point.y=BoundingRect.bottom;
        }

        if(
            (oldpt.x==BoundingRect.right && point.x==BoundingRect.right) ||
            (oldpt.x==BoundingRect.left && point.x==BoundingRect.left) ||
            (oldpt.y==BoundingRect.top && point.y==BoundingRect.top) ||
            (oldpt.y==BoundingRect.bottom && point.y==BoundingRect.bottom)
        )
        {
            switch (pElement->m_nType)
            {
                case Bars:
                    SetRect(&rect, oldpt.x, oldpt.y, point.x, point.y);
                    //fill solid rect
                    ::SetBkColor(hDC, pElement->m_LineColor);
                    ::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
                    MoveToEx (hDC, point.x, point.y, NULL);
                    break;

                case Stick:
                    MoveToEx (hDC, point.x, point.y, NULL);
                    Corrdinate(pt.x, pElement->min.y, &p);
                    p.x = point.x;

                    if (p.y > BoundingRect.bottom)
                    {
                        p.y = BoundingRect.bottom;
                    }

                    if (p.x > BoundingRect.right)
                    {
                        p.y = BoundingRect.right;
                    }

                    LineTo(hDC, p.x, p.y);
                    MoveToEx (hDC, point.x, point.y, NULL);
                    break;

                default:
                    MoveToEx (hDC, point.x, point.y, NULL);
                    oldpt = point;
                    continue;
            }
        }

        switch (pElement->m_nType)
        {
            case XYStep: // Draw XY steps
                p.x = point.x;
                p.y = oldpt.y;
                LineTo(hDC, p.x, p.y);
                LineTo(hDC, point.x, point.y) ;
                break;

            case YXStep: // Draw YX steps
                p.y = point.y;
                p.x = oldpt.x;
                LineTo(hDC, p.x, p.y);
                LineTo(hDC, point.x, point.y) ;
                break;

            case Bars:  // Draw bars
                Corrdinate(pt.x, pElement->min.y, &p);
                p.x = point.x;

                if (p.y < BoundingRect.top)
                {
                    p.y=BoundingRect.top;
                }

                if (p.y > BoundingRect.bottom)
                {
                    p.y=BoundingRect.bottom;
                }

                SetRect(&rect, oldpt.x, oldpt.y ,p.x, p.y);
                ::SetBkColor(hDC, pElement->m_LineColor);
                ::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
                MoveToEx (hDC, point.x, point.y, NULL);
                break;

            case Stick: // Draw sticked points to the base line
                LineTo(hDC, point.x, point.y );
                Corrdinate(pt.x, pElement->min.y, &p);
                p.x = point.x;

                if (p.y > BoundingRect.bottom)
                {
                    p.y=BoundingRect.bottom;
                }

                if (p.x > BoundingRect.right)
                {
                    p.y=BoundingRect.right;
                }

                LineTo(hDC, p.x, p.y);
                MoveToEx (hDC, point.x, point.y, NULL);
                break;

            default:
                LineTo(hDC, point.x, point.y );
                break;
        }

        oldpt = point;
    }

    // Unlock the plot access
    LeaveCriticalSection(&pElement->m_CritSectionForPlot);

    if (! bOptimized)
    {
        SelectObject(hDC, hPenSave);
    }

    if (! bOptimized)
    {
        SelectObject(hDC, hBrushSave);
    }

    DeleteObject(m_linePen);
    m_linePen = NULL;
    DeleteObject(m_lineBrush);
    m_lineBrush = NULL;
}

/////////////////////////////////////
// Draw Points
void CDMGraphCtrl::PlotElementPoints(HDC hDC, CGraphElement* pElement, BOOL bOptimized)
{
    if(pElement->m_PointList.GetSize() == 0)
    {
        return;
    }

    // Create the new pen as the color of element symbol
    if (m_pointPen == NULL)
    {
        m_pointPen = CreatePen (0, 0, pElement->m_PointColor);
    }

    HPEN hPenSave = (HPEN)SelectObject(hDC, m_pointPen);

    // Create the new brush as the color of element symbol
    if (m_pointBrush == NULL)
    {
        m_pointBrush = CreateSolidBrush (pElement->m_PointColor);
    }

    HBRUSH hBrushSave = (HBRUSH)SelectObject(hDC, m_pointBrush);
    RECT BoundingRect; // Object to store bounding rectangle
    BoundingRect = m_axisRect; // Store the bounding rectangle
    // Decrease the rectangle by the pen width
    InflateRect(&BoundingRect, -pElement->m_nLineWidth, -pElement->m_nLineWidth);
    //BoundingRect.NormalizeRect ();
    // Lock the plot access
    EnterCriticalSection(&pElement->m_CritSectionForPlot);
    POINT point;
    CElementPoint pt;
    VARIANT_BOOL bXLog, bYLog;
    POINT oldpt;

    //Proceed only if it contains any points
    if ( pElement->m_PointList.GetSize() > 0 )
    {
        pt = pElement->m_PointList[0];
        m_pAxis[ HorizontalX ]->get_Log(&bXLog);
        m_pAxis[ VerticalY  ]->get_Log(&bYLog);

        if(bXLog)
        {
            pt.x = log10(pt.x);
        }

        if(bYLog)
        {
            pt.y = log10(pt.y);
        }

        // calculate the corrdinate of ploting point.
        Corrdinate(pt, &point);

        // Clip the ploting area if it exceed ranged .
        if (point.x < BoundingRect.left)
        {
            point.x = BoundingRect.left;
        }

        if (point.y < BoundingRect.top)
        {
            point.y = BoundingRect.top;
        }

        if (point.y > BoundingRect.bottom)
        {
            point.y = BoundingRect.bottom;
        }

        MoveToEx (hDC, point.x, point.y, NULL);
        oldpt = point;
    }

    //Start plot all available data.
    for(int i=0; i<pElement->m_PointList.GetSize(); i++)
    {
        //pt = pElement->m_PointList[i];
        pt.x = pElement->m_PointList[i].x;
        pt.y = pElement->m_PointList[i].y;

        if(bXLog)
        {
            pt.x = log10(pt.x);
        }

        if(bYLog)
        {
            pt.y = log10(pt.y);
        }

        Corrdinate(pt, &point);

        // Clip the ploting area if it exceed ranged .
        if (point.x > BoundingRect.right)
        {
            point.x = BoundingRect.right;
        }

        if (point.x < BoundingRect.left)
        {
            point.x = BoundingRect.left;
        }

        if (point.y < BoundingRect.top)
        {
            point.y = BoundingRect.top;
        }

        if (point.y > BoundingRect.bottom)
        {
            point.y = BoundingRect.bottom;
        }

        if(
            (oldpt.x == BoundingRect.right  && point.x == BoundingRect.right ) ||
            (oldpt.x == BoundingRect.left   && point.x == BoundingRect.left ) ||
            (oldpt.y == BoundingRect.top    && point.y == BoundingRect.top ) ||
            (oldpt.y == BoundingRect.bottom && point.y == BoundingRect.bottom )
        )
        {
            MoveToEx(hDC, point.x, point.y, NULL);
            oldpt = point;
            continue;
        }

        int symsz = pElement->m_nPointSize ;

        if (symsz<0)
        {
            symsz = 0;
        }

        RECT rect;
        SetRect(&rect, point.x-symsz, point.y-symsz, point.x+symsz, point.y+symsz);

        if (pElement->m_bSolid == FALSE)
        {
            SelectObject(hDC, hBrushSave);
        }

        if (pElement->m_nSymbol == Dots)    // Draw dots.
        {
            Ellipse(hDC, rect.left, rect.top, rect.right, rect.bottom);
        }

        if (pElement->m_nSymbol == Rectangles)  // Draw rectangles.
        {
            Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
        }

        if (pElement->m_nSymbol == Diamonds) // Draw Diamonds
        {
            DrawDiamond(hDC, &point, symsz);
        }

        if (pElement->m_nSymbol == Asterisk) // Draw Asterisks
        {
            DrawAsterisk(hDC, &point, symsz);
        }

        if (pElement->m_nSymbol == DownTriangles) // Draw Down Triangles
        {
            DrawDownTriangle(hDC, &point, symsz);
        }

        if (pElement->m_nSymbol == RightTriangles) // Draw Right Triangles
        {
            DrawRightTriangle(hDC, &point, symsz);
        }

        if (pElement->m_nSymbol == UpTriangles) // Draw Up Triangles
        {
            DrawUpTriangle(hDC, &point, symsz);
        }

        if (pElement->m_nSymbol == LeftTriangles) // Draw Left Triangles
        {
            DrawLeftTriangle(hDC, &point, symsz);
        }

        oldpt = point;
    }

    // Unlock the plot access
    LeaveCriticalSection(&pElement->m_CritSectionForPlot);

    if (! bOptimized)
    {
        SelectObject(hDC, hPenSave);
    }

    if (! bOptimized)
    {
        SelectObject(hDC, hBrushSave);
    }

    DeleteObject(m_pointPen);
    m_pointPen = NULL;
    DeleteObject(m_pointBrush);
    m_pointBrush = NULL;
}


LRESULT CDMGraphCtrl::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    RECT rcPos = { CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT };

    if (!m_ttip.Create(TOOLTIPS_CLASS, m_hWnd, rcPos, NULL, WS_POPUP|TTS_NOPREFIX|TTS_ALWAYSTIP, WS_EX_TOPMOST))
    {
        ATLTRACE(_T("Error in creating ToolTip %#x\n"), GetLastError());
        return 0;
    }

    m_ti.hwnd = m_hWnd;
    m_ti.uFlags  = TTF_IDISHWND | TTF_TRACK | TTF_ABSOLUTE;
    m_ti.uId = (UINT)m_hWnd;
    m_ttip.SendMessage(TTM_ADDTOOL, 0, (LPARAM)&m_ti);
    m_ttip.SendMessage(TTM_SETDELAYTIME, TTDT_AUTOPOP, MAKELPARAM(SHRT_MAX, 0));   // stop the tooltip coming up automatically
    m_ttip.SendMessage(TTM_SETDELAYTIME, TTDT_INITIAL, 0);
    ::SetClassLongPtr(m_hWnd, GCL_HCURSOR, (LONG_PTR)LoadCursor(NULL, IDC_CROSS));
    return 0;
}

void CDMGraphCtrl::RelayEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
    if (m_ttip.m_hWnd)
    {
        MSG msg;
        m_ttip.SendMessage(TTM_TRACKACTIVATE, TRUE, (LPARAM)&m_ti);
        msg.hwnd    = m_hWnd;
        msg.message = message;
        msg.wParam  = wParam;
        msg.lParam  = lParam;
        msg.time    = 0;
        msg.pt.x    = LOWORD (lParam);
        msg.pt.y    = HIWORD (lParam);
        m_ttip.SendMessage(TTM_RELAYEVENT, 0, (LPARAM)&msg);
    }
}

void CDMGraphCtrl::DoPan(const LPPOINT point, long mode)
{
    ATLASSERT(point);
    double Y1 = PT2DBLY(point->y);
    double Y2 = PT2DBLY(m_panPoint.y);
    double yOffset = Y1 - Y2;
    double X1 =  PT2DBLX(point->x);
    double X2 =  PT2DBLX(m_panPoint.x);
    double xOffset = X1 - X2;

    switch (mode)
    {
        case PanXY:
            SetRange(dRangeX[MIN] - xOffset, dRangeX[MAX] - xOffset,
                     dRangeY[MIN] - yOffset, dRangeY[MAX] - yOffset);
            break;

        case PanX:
            SetRange(dRangeX[MIN] - xOffset, dRangeX[MAX] - xOffset,
                     dRangeY[MIN], dRangeY[MAX]);
            break;

        case PanY:
            SetRange(dRangeX[MIN], dRangeX[MAX],
                     dRangeY[MIN] - yOffset, dRangeY[MAX] - yOffset);
            break;
    }

    m_panPoint = *point;
}

void CDMGraphCtrl::DoZoom(UINT nFlags, const LPPOINT point)
{
    ATLASSERT(point);

    if (m_eTrackMode == Zoom || m_eTrackMode == ZoomX || m_eTrackMode == ZoomY)
    {
        if (m_bUnzoom)
        {
            dOldRangeX[MIN]=dRangeX[MIN];
            dOldRangeY[MIN]=dRangeY[MIN];
            dOldRangeX[MAX]=dRangeX[MAX];
            dOldRangeY[MAX]=dRangeY[MAX];
            m_bUnzoom = FALSE;
        }

        CRectTracker tracker;
        double xmin, xmax, ymin, ymax;
        xmin=dRangeX[MIN];
        xmax=dRangeX[MAX];
        ymin=dRangeY[MIN];
        ymax=dRangeY[MAX];

        if (tracker.TrackRubberBand(m_hWnd, point) )
        {
            POINT pt1, pt2;
            int nTemp;
            tracker.GetPos(&pt1, &pt2);

            if (pt1.x > pt2.x)
            {
                nTemp = pt1.x;
                pt1.x = pt2.x;
                pt2.x = nTemp;
            }

            if (pt1.y > pt2.y)
            {
                nTemp = pt1.y;
                pt1.y = pt2.y;
                pt2.y = nTemp;
            }

            if(m_eTrackMode == Zoom || m_eTrackMode == ZoomX)
            {
                xmin = PT2DBLX(pt1.x);
                xmax = PT2DBLX(pt2.x);
            }

            if(m_eTrackMode == Zoom || m_eTrackMode == ZoomY)
            {
                ymin = PT2DBLY(pt2.y);
                ymax = PT2DBLY(pt1.y);
            }
        }

        SetRange(xmin,xmax,ymin,ymax);
    }
}

LRESULT CDMGraphCtrl::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    POINT point = { (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam) };

    if (PtInRect (&m_axisRect, point))
    {
        if (wParam == MK_LBUTTON)
        {
            switch (m_eTrackMode)
            {
                case TrackCursor:
                case TrackElement:
                    UpdateToolTip(&point, wParam);
                    break;

                case PanXY:
                    DoPan(&point, PanXY);
                    break;

                case PanX:
                    DoPan(&point, PanX);
                    break;

                case PanY:
                    DoPan(&point, PanY);
                    break;

                default:
                    vUpdateCursorSelection(point);
                    CursorPosition(&point);
                    break;
            }
        }
    }

    bHandled = FALSE;
    return 0;
}

LRESULT CDMGraphCtrl::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    POINT point = { (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam) };

    if (PtInRect (&m_axisRect, point))
    {
        SetCursor(LoadCursor(NULL, IDC_ARROW));
        m_panPoint = point;

        //CursorPosition(&point);

        if (m_eTrackMode == Zoom || m_eTrackMode == ZoomX || m_eTrackMode == ZoomY )
        {
            DoZoom(wParam, &point);
        }
        else if ((m_eTrackMode == TrackCursor || m_eTrackMode == TrackElement) && wParam == MK_LBUTTON )
        {
            UpdateToolTip(&point, wParam);
        }
    }

    bHandled = FALSE;
    return 0;
}

LRESULT CDMGraphCtrl::OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    POINT point = { (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam) };

    if (PtInRect (&m_axisRect, point))
    {
        m_ttip.SendMessage(TTM_TRACKACTIVATE, FALSE, (LPARAM)&m_ti);
    }

    bHandled = FALSE;
    return 0;
}

LRESULT CDMGraphCtrl::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    POINT point = { (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam) };

    if(m_eTrackMode == Zoom || m_eTrackMode == ZoomX || m_eTrackMode == ZoomY)
    {
        SetRange(dOldRangeX[MIN],dOldRangeX[MAX],dOldRangeY[MIN],dOldRangeY[MAX]);
    }

    bHandled = FALSE;
    return 0;
}

////////////////////////////////////////////////////////
// display the property pages of the control at runtime.
//
LRESULT CDMGraphCtrl::OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    //DoVerbProperties(NULL, GetParent());
    POINT point = { (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam) };

    if (PtInRect (&m_axisRect, point))
    {
        vCursorEventHandler(EVENT_LDBL_CLK, point.x, point.y);
    }

    bHandled = FALSE;
    return 0;
}



////////////////////////////////////////////////////////
// Copy the control bitmap to the clipboard
// Code Project Article: "Clipboard Copy in a Nutshell"
// by Keith Rule
STDMETHODIMP CDMGraphCtrl::CopyToClipboard()
{
    RECT rect;
    HDC dc = GetDC();
    GetClientRect(&rect);
    HDC memDC;
    HBITMAP bitmap;
    // Create memDC
    memDC = CreateCompatibleDC(dc);
    bitmap = CreateCompatibleBitmap(dc, rect.right - rect.left, rect.bottom-rect.top);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, bitmap);
    // Fill in memDC
    SetBkColor(memDC, GetBkColor(dc));
    ExtTextOut(memDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
    m_rcBounds = rect;
    DrawGraphOffScreen(memDC, &rect, FALSE);
    // Copy contents of memDC to clipboard
    OpenClipboard();
    EmptyClipboard();
    SetClipboardData(CF_BITMAP, bitmap); // CF_METAFILEPICT
    CloseClipboard();
    // Clean up; do not delete bitmap which now belongs to Clipboard
    SelectObject(memDC, hOldBitmap);
    ReleaseDC(dc);
    return S_OK;
}

////////////////////////////////////////////
// Print the control
STDMETHODIMP CDMGraphCtrl::PrintGraph()
{
    HRESULT hr;
    WCHAR pbuf[] = L"DMGraph ActiveX Control";
    // Instantiate a Print Dialog.
    PRINTDLGEX pd;
    ZeroMemory(&pd, sizeof(PRINTDLGEX));
    pd.lStructSize = sizeof(PRINTDLGEX);
    // Initialize some of the fields in PRINTDLG structure.
    pd.hwndOwner = GetParent();
    pd.Flags = PD_ALLPAGES | PD_RETURNDC | PD_NOPAGENUMS;
    pd.nStartPage = START_PAGE_GENERAL;
    // Display Windows print dialog box.
    hr = PrintDlgEx(&pd); //Win2k and later

    if(FAILED(hr))
    {
        return hr;
    }

    if(pd.dwResultAction != PD_RESULT_PRINT)
    {
        //user cancelled or so
        hr = S_FALSE;
        goto $end;
    }

    // Obtain a handle to the device context.
    if (pd.hDC == NULL)
    {
        hr = S_FALSE;
        goto $end;
    }

    DOCINFOW di;
    ZeroMemory(&di, sizeof(DOCINFOW));
    di.cbSize = sizeof(DOCINFOW);
    di.lpszDocName = L"Graph2D";
    StartDocW(pd.hDC, &di); // begin a new print job
    StartPage(pd.hDC); // begin a new page
    TextOutW(pd.hDC, 100, 10, pbuf, wcslen(pbuf));// write the string in pbuf
    POINT pt;
    pt.x = pt.y = 100,100; // position
    // AT 10.07.2003 Begin
    // compare the resolution of the screen and of the printer
    HDC hdcScreen;
    hdcScreen = GetDC();
    int nScreenWidth, nScreenHeight, nWidth, nHeight;
    nScreenWidth = ::GetDeviceCaps(hdcScreen, HORZRES);
    nScreenHeight = ::GetDeviceCaps(hdcScreen, VERTRES);
    nWidth = ::GetDeviceCaps(pd.hDC, HORZRES);
    nHeight = ::GetDeviceCaps(pd.hDC, VERTRES);
    // distinguish between the paper-orientation
    LPDEVMODE lpDevMode;
    lpDevMode = (LPDEVMODE)GlobalLock(pd.hDevMode);

    if( lpDevMode->dmOrientation == DMORIENT_PORTRAIT )
    {
        // Portrait --> 1/3 of the Paper
        nWidth = static_cast<int>(0.9 * nWidth);
        nHeight = static_cast<int>(0.33 * nHeight);
    }
    else // DMORIENT_LANDSCAPE
    {
        nWidth = static_cast<int>(0.9 * nWidth);
        nHeight = static_cast<int>(0.9 * nHeight);
    }

    GlobalUnlock(pd.hDevMode);
    SIZE sz;
    sz.cx = nWidth;
    sz.cy = nHeight;
    // convert the Textsize:
    CY size;
    CY arrFontSize[4];
    // calculate the conversion factor:
    double dHeightRatio, dWidthRatio, dconv;
    dHeightRatio = nHeight/nScreenHeight;
    dWidthRatio = nWidth/nScreenWidth;
    dconv = max(dHeightRatio, dWidthRatio);
    // convert each font and keep the actual font-size
    int i;
    IFont* arrFonts[4];
    arrFonts[0] = m_fontTick;
    arrFonts[1] = m_fontTitle;
    arrFonts[2] = m_fontLabel;
    arrFonts[3] = m_fontIdent;

    for(i=0; i<4; i++)
    {
        hr = arrFonts[i]->get_Size(&size);
        arrFontSize[i] = size;
        ATLTRACE("Size: %d\n",size.Lo);
        size.Lo = static_cast<int>(floor(dconv * size.Lo));
        hr = arrFonts[i]->put_Size(size);
    }

    // end convert
    // AT 10.07.2003 End
    // AT 10.07.2003 original: SIZE sz(500,300); // width, hieght
    RECT rect;
    SetRect(&rect, pt.x, pt.y, pt.x + sz.cx, pt.y + sz.cy);
    SetViewportExtEx(pd.hDC, rect.right , rect.bottom, NULL);
    SetViewportOrgEx(pd.hDC, 50, 50, NULL); // position
    DrawGraph(pd.hDC, &rect, FALSE);
    EndPage(pd.hDC); // end a page
    EndDoc(pd.hDC); // end a print job

    // AT 10.07.2003 reconvert the Font

    for(i=0; i<4; i++)
    {
        hr = arrFonts[i]->get_Size(&size);
        size = arrFontSize[i];
        ATLTRACE("Size: %d\n",size.Lo);
        hr = arrFonts[i]->put_Size(size);
    }

    // AT 10.07.2003 End
    FireViewChange();
$end:

    if (pd.hDevMode)
    {
        GlobalFree(pd.hDevMode);
    }

    if (pd.hDevNames)
    {
        GlobalFree(pd.hDevNames);
    }

    if (pd.lpPageRanges)
    {
        GlobalFree(pd.lpPageRanges);
    }

    if (pd.hDC)
    {
        DeleteDC(pd.hDC);
    }

    return S_OK;
}

//////////////////////////////////////////////
// Delete all elements and allocate one empty
STDMETHODIMP CDMGraphCtrl::ClearGraph()
{
    ATLASSERT(m_pElementList);
    m_pElementList->RemoveAll();
    //this should be cleared by above RemoveAll
    ATLASSERT(m_nAvailablePlots == 0);
    m_pCursorList->RemoveAll();
    m_pAnnotationList->RemoveAll();
    dAutoRangeX[MIN]=0;
    dAutoRangeX[MAX]=0;
    dAutoRangeY[MIN]=0;
    dAutoRangeY[MAX]=0;
    vCursorEventHandler(GRAPH_ELEM_ALL_CLEAR);
    FireViewChange();
    m_bRequiresSave = TRUE;
    return S_OK;
}

void CDMGraphCtrl::UpdateAutoRange(double X, double Y, BOOL bFirstPoint)
{
    if(m_nAvailablePlots)
    {
        if(X < dAutoRangeX[MIN])
        {
            dAutoRangeX[MIN] = floor(X);
        }

        if(Y < dAutoRangeY[MIN])
        {
            dAutoRangeY[MIN] = floor(Y);
        }

        if(X > dAutoRangeX[MAX])
        {
            dAutoRangeX[MAX] = ceil(X);
        }

        if(Y > dAutoRangeY[MAX])
        {
            dAutoRangeY[MAX] = ceil(Y);
        }
    }
    else
    {
        dAutoRangeX[MIN] = floor(X);
        dAutoRangeY[MIN] = floor(Y);
        dAutoRangeX[MAX] = ceil(X);
        dAutoRangeY[MAX] = ceil(Y);
    }

    if(bFirstPoint)
    {
        m_nAvailablePlots++;
    }

    if(m_objCursorStMachine.eGetCurrState() == NO_GRAPH_DRAWN)
    {
        vCursorEventHandler(ADDED_GRAPH_ELEM);
    }
}

//updates m_nAvailablePlots when a graph with at least one point was deleted
void CDMGraphCtrl::OnPlotDeleted()
{
    ATLASSERT(m_nAvailablePlots);
    m_nAvailablePlots--;
}

///////////////////////////////////////////////////////
//
// Added by Robert Harber
// Montag, 14. Juli 2003
//
/****************************************/
/* Method to Save the graph as a bitmap*/
/****************************************/
STDMETHODIMP CDMGraphCtrl::SaveAs(BSTR szFilename)
{
    if(szFilename == NULL)
    {
        return E_POINTER;
    }

    HRESULT hr = S_OK;
    PBITMAPINFO pbi;
    RECT rect;
    HDC dc = GetDC();

    if(dc == NULL)
    {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    GetClientRect(&rect);
    HDC memDC;
    HBITMAP hBitmap;
    // Create memDC
    memDC = CreateCompatibleDC(dc);

    if(memDC == NULL)
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
        goto $end1;
    }

    hBitmap = CreateCompatibleBitmap(dc, rect.right - rect.left, rect.bottom - rect.top);

    if(hBitmap == NULL)
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
        goto $end2;
    }

    HBITMAP hOldBitmap;
    hOldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);
    // Fill in memDC
    SetBkColor(memDC, GetBkColor(dc));
    ExtTextOut(memDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL); //FillSolidRect
    m_rcBounds = rect;
    DrawGraphOffScreen(memDC, &rect, FALSE);
    //Create the bitmap info struct
    hr = CreateBitmapInfoStruct(hBitmap, &pbi);

    if(SUCCEEDED(hr))
    {
        //Create the bitmap
        CreateBMPFile(szFilename, pbi, hBitmap, memDC);
        free(pbi);
    }

    // Clean up
    SelectObject(memDC, hOldBitmap);
    DeleteObject(hBitmap);
$end2:
    DeleteDC(memDC);
$end1:
    ReleaseDC(dc);
    return hr;
}

///////////////////////////////////////////////////////
//
// Added by Arunkumar Karri
// 19 Jan 2012
//
/****************************************/
/* This function will shift the display interms of number of grid line of the graph.*/
/****************************************/
STDMETHODIMP CDMGraphCtrl::ShiftDisplay(SHORT xShift,SHORT yShift)
{
    double dXMin = 0.0, dYMin = 0.0, dXMax = 0.0, dYMax = 0.0;
    GetRange(&dXMin, &dXMax, &dYMin, &dYMax);
    SetRange(dXMin+xShift, dXMax+xShift, dYMin+yShift, dYMax+yShift);
    return S_OK;
}

///////////////////////////////////////////////////////
//
// Added by Arunkumar Karri
// 23 Jan 2012
//
/****************************************/
/* Helper Function to convert LPCTSTR to char array*/
/****************************************/
char* LPCTSTRToCharArray(LPCTSTR val)
{
    char* ansistr = NULL;
    int lenW = _tcslen(val);
    int lenA = ::WideCharToMultiByte(CP_ACP, 0, val, lenW, 0, 0, NULL, NULL);

    if (lenA > 0)
    {
        ansistr = new char[lenA + 1]; // allocate a final null terminator as well
        WideCharToMultiByte(CP_ACP, 0, val, lenW, ansistr, lenA, NULL, NULL);
        ansistr[lenA] = 0; // Set the null terminator yourself
    }

    char* c = new char[lenA + 1];

    if (NULL != ansistr)
    {
        strcpy(c, ansistr);
        //...use the strings, then free their memory:
        delete[] ansistr;
    }

    return c;
}
///////////////////////////////////////////////////////
//
// Added by Arunkumar Karri
// 23 Jan 2012
//
/****************************************/
/* Helper Function to convert BSTR to char array*/
/****************************************/
void* WINAPI BStr2TStr(HWND hwnd, BSTR strIn)
{
    DWORD   size;
    void*    strOut;

    if (!IsWindowUnicode(hwnd))
    {
        // ANSI
        size = WideCharToMultiByte(CP_ACP, 0, (WCHAR*)((char*)strIn), -1, 0, 0, 0, 0);

        if ((strOut = GlobalAlloc(GMEM_FIXED, size)))
        {
            WideCharToMultiByte(CP_ACP, 0, (WCHAR*)((char*)strIn), -1, (char*)strOut, size, 0, 0);
        }
    }
    else
    {
        // UNICODE
        size = (*((short*)strIn) + 1) * sizeof(wchar_t);

        if ((strOut = GlobalAlloc(GMEM_FIXED, size)))
        {
            CopyMemory(strOut, (char*)strIn, size);
        }
    }

    return(strOut);
}

//Linear equation calculations y = Mx + C
double CDMGraphCtrl::DetermineY(CElementPoint point1, CElementPoint point2, double XValue)
{
    double dblSlope = 0, dblC = 0;
    //Calculate slope
    dblSlope = (point2.y - point1.y)/(point2.x - point1.x);
    //calculate C intercept value
    dblC = point1.y - (dblSlope*point1.x);
    //return new Y value
    return ((dblSlope*XValue)+dblC);
}

///////////////////////////////////////////////////////
//
// Added by Arunkumar Karri
// 31 Jan 2012
//
/****************************************/
/* This will return the element value at a cursor location 'dblCursorVal'.*/
/****************************************/
STDMETHODIMP CDMGraphCtrl::GetElementValueAtCursor( SHORT shElementID,
        DOUBLE dblCursorVal, DOUBLE* dblElemVal )
{
    // Pointers to Element
    CGraphElement* pElement = NULL;
    pElement = (*m_pElementList)[ shElementID ];

    if( pElement != NULL )
    {
        // If it is valid then only get the count
        if( pElement != NULL )
        {
            // Get the number of points in the element's ring buffer
            int nCount = pElement->m_PointList.GetSize();

            if(nCount > 0)
            {
                CElementPoint odPoint;

                for(int nIndex = 0; nIndex < nCount; nIndex++ )
                {
                    odPoint = pElement->m_PointList[nIndex];

                    if(odPoint.x > dblCursorVal)
                    {
                        if(nIndex == 0)
                        {
                            *dblElemVal = 0;
                            return S_OK;
                        }

                        LineType enLineType;
                        CElementPoint odPrevPoint;
                        odPrevPoint = pElement->m_PointList[nIndex-1];
                        pElement->get_Linetype(&enLineType);

                        if( enLineType == XYStep || enLineType == Bars)
                        {
                            *dblElemVal = odPrevPoint.y;
                            return S_OK;
                        }
                        else if( enLineType == YXStep)
                        {
                            *dblElemVal = odPoint.y;
                            return S_OK;
                        }
                        else
                        {
                            //Calculate Y using linear equation
                            *dblElemVal = DetermineY(odPrevPoint, odPoint, dblCursorVal);
                            return S_OK;
                        }
                    }
                }
            }
            else
            {
                *dblElemVal = 0;
            }
        }
    }

    return S_OK;
}

///////////////////////////////////////////////////////
//
// Added by Arunkumar Karri
// 20 Jan 2012
//
/****************************************/
/* This will export graph data into CSV file.*/
/****************************************/
STDMETHODIMP CDMGraphCtrl::GenerateCSVReport(LPCTSTR pFilename)
{
    // Check for valid pointer
    if( pFilename != NULL )
    {
        // Create File Pointer
        FILE* pFile = NULL;
        // Open the file in read mode
        char* pchFileName = NULL;
        pchFileName = LPCTSTRToCharArray(pFilename);
        fopen_s(&pFile, pchFileName,"w+");
        delete pchFileName;

        // If successful
        if( pFile != NULL )
        {
            // Pointer to Pointers of Elements
            CGraphElement** pElement = NULL;
            long lElementCount = 0;
            // Get the element count
            m_pElementList->get_Count(&lElementCount);
            // Allocate memory for pointer array
            pElement = new CGraphElement*[ lElementCount ];

            // If enough memory is available then proceed
            if( pElement != NULL )
            {
                int nMaxCount = 0; // To store the number of iterations

                for (int nIndex = 0; nIndex < lElementCount; nIndex++)
                {
                    pElement[ nIndex ] = (*m_pElementList)[ nIndex ];

                    // If it is valid then only get the count
                    if( pElement[ nIndex ] != NULL )
                    {
                        // Get the number of points in the element's ring buffer
                        int nCount =
                            pElement[ nIndex ]->m_PointList.GetSize();

                        // Save if it is greater then the available maximum
                        // value
                        if ( nMaxCount < nCount )
                        {
                            nMaxCount = nCount;
                        }
                    }
                }

                // Now Export the data
                //Write CSV File Header
                fprintf(pFile,"BUSMASTER Generated Report\n\n");

                // Write Signal Names
                for(int nIndex = 0; nIndex < lElementCount; nIndex++ )
                {
                    // Save the signal name
                    BSTR bstrElementName;
                    pElement[ nIndex ]->get_Name(&bstrElementName);
                    //char chElementName[1024];
                    char* pElementName = NULL;
                    pElementName = (char*)BStr2TStr(m_hWnd, bstrElementName);
                    fprintf( pFile,"%s,,", pElementName);
                    GlobalFree(pElementName);
                }

                // Create new Row
                fprintf(pFile, "\n");

                // Write Time and Value Column
                for(int nIndex = 0; nIndex < lElementCount; nIndex++ )
                {
                    fprintf( pFile,"Time( in mSec),Value,");
                }

                fprintf(pFile, "\n");
                // Dump Signal Data
                CElementPoint odPoint;

                for(int nIndex = 0; nIndex < nMaxCount; nIndex++ )
                {
                    for( int nElementIndex = 0;
                            nElementIndex < lElementCount;
                            nElementIndex++ )
                    {
                        if( nIndex < pElement[ nElementIndex ]->
                                m_PointList.GetSize())
                        {
                            odPoint = pElement[ nElementIndex ]->m_PointList[nIndex];
                            fprintf( pFile, "%.4f, %.3f,",
                                     odPoint.x, odPoint.y );
                        }
                    }

                    fprintf(pFile, "\n" );
                }

                // Close opened file
                fflush( pFile );
                fclose( pFile );
            }

            delete[] pElement;
        }
    }

    return S_OK;
}

/*******************************************/
/*Function to create the Bitmap info struct*/
/*******************************************/
HRESULT CDMGraphCtrl::CreateBitmapInfoStruct(HBITMAP hBmp, PBITMAPINFO* pbi)
{
    ATLASSERT(hBmp);
    ATLASSERT(pbi);
    BITMAP bmp;
    WORD cClrBits;
    *pbi = NULL;

    //Retrieve the bitmap colour format, width, and height.
    if(!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp))
    {
        return Error(L"Unable to retrieve colour format, width and height of bitmap", GUID_NULL, HRESULT_FROM_WIN32(GetLastError()));
    }

    //Convert the color format to a count of bits.
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);

    if(cClrBits == 1)
    {
        cClrBits = 1;
    }
    else if(cClrBits <= 4)
    {
        cClrBits = 4;
    }
    else if (cClrBits <= 8)
    {
        cClrBits = 8;
    }
    else if (cClrBits <= 16)
    {
        cClrBits = 16;
    }
    else if (cClrBits <= 24)
    {
        cClrBits = 24;
    }
    else
    {
        cClrBits = 32;
    }

    //Allocate memory for the BITMAPINFO structure. (This structure
    //contains a BITMAPINFOHEADER structure and an array of RGBQUAD
    //data structures.)

    if(cClrBits != 24)
        *pbi = (PBITMAPINFO) malloc(sizeof(BITMAPINFOHEADER) +
                                    sizeof(RGBQUAD) * (1<< cClrBits));
    else //There is no RGBQUAD array for the 24-bit-per-pixel format.
    {
        *pbi = (PBITMAPINFO) malloc(sizeof(BITMAPINFOHEADER));
    }

    if(*pbi == NULL)
    {
        return E_OUTOFMEMORY;
    }

    //Initialize the fields in the BITMAPINFO structure.
    (*pbi)->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    (*pbi)->bmiHeader.biWidth = bmp.bmWidth;
    (*pbi)->bmiHeader.biHeight = bmp.bmHeight;
    (*pbi)->bmiHeader.biPlanes = bmp.bmPlanes;
    (*pbi)->bmiHeader.biBitCount = bmp.bmBitsPixel;

    if(cClrBits < 24)
    {
        (*pbi)->bmiHeader.biClrUsed = (1<<cClrBits);
    }

    //If the bitmap is not compressed, set the BI_RGB flag.
    (*pbi)->bmiHeader.biCompression = BI_RGB;
    //Compute the number of bytes in the array of color
    //indices and store the result in biSizeImage.
    //For Windows NT, the width must be DWORD aligned unless
    //the bitmap is RLE compressed. This example shows this.
    //For Windows 95/98/Me, the width must be WORD aligned unless the
    //bitmap is RLE compressed.
    (*pbi)->bmiHeader.biSizeImage = (((*pbi)->bmiHeader.biWidth * cClrBits +31) & ~31) /8
                                    * (*pbi)->bmiHeader.biHeight;
    //Set biClrImportant to 0, indicating that all of the
    //device colors are important.
    (*pbi)->bmiHeader.biClrImportant = 0;
    return S_OK;
}

/*******************************/
/*Function to create the Bitmap*/
/*******************************/
HRESULT CDMGraphCtrl::CreateBMPFile(LPCOLESTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC)
{
    HRESULT hr = S_OK;
    HANDLE hf = INVALID_HANDLE_VALUE; //file handle
    BITMAPFILEHEADER hdr;     //bitmap file-header
    PBITMAPINFOHEADER pbih;   //bitmap info-header
    LPBYTE lpBits;            //memory pointer
    DWORD dwTotal;            //total count of bytes
    DWORD cb;                 //incremental count of bytes
    BYTE* hp;                 //byte pointer
    DWORD dwTmp;
    ATLASSERT(pszFile);
    pbih = (PBITMAPINFOHEADER) pbi;
    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

    if(!lpBits)
    {
        return E_OUTOFMEMORY;
    }

    //Retrieve the color table (RGBQUAD array) and the bits (array of palette indices) from the DIB.
    if(!GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi, DIB_RGB_COLORS))
    {
        hr = Error(L"Unable to retrieve colour table when saving bitmap", GUID_NULL, HRESULT_FROM_WIN32(GetLastError()));
        goto $end;
    }

    //Create the .BMP file.
    hf = CreateFileW(pszFile,
                     GENERIC_READ | GENERIC_WRITE,
                     (DWORD) 0,
                     NULL,
                     CREATE_ALWAYS,
                     FILE_ATTRIBUTE_NORMAL,
                     (HANDLE) NULL);

    if(hf == INVALID_HANDLE_VALUE)
    {
        hr = Error(L"Unable to open the file when saving bitmap", GUID_NULL, HRESULT_FROM_WIN32(GetLastError()));
        goto $end;
    }

    hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"
    //Compute the size of the entire file.
    hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) +
                          pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD) +
                          pbih->biSizeImage);
    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;
    //Compute the offset to the array of color indices.
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +
                    pbih->biSize + pbih->biClrUsed * sizeof (RGBQUAD);

    //Copy the BITMAPFILEHEADER into the .BMP file.
    if(!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), (LPDWORD) &dwTmp,  NULL))
    {
        hr = Error(L"Unable to write file header when saving bitmap", GUID_NULL, HRESULT_FROM_WIN32(GetLastError()));
        goto $end;
    }

    //Copy the BITMAPINFOHEADER and RGBQUAD array into the file.
    if(!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER)
                  + pbih->biClrUsed * sizeof (RGBQUAD),
                  (LPDWORD) &dwTmp, ( NULL)) )
    {
        hr = Error(L"Unable to write bitmap info", GUID_NULL, HRESULT_FROM_WIN32(GetLastError()));
        goto $end;
    }

    //Copy the array of color indices into the .BMP file.
    dwTotal = cb = pbih->biSizeImage;
    hp = lpBits;

    if(!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL))
    {
        hr = Error(L"Unable to write colour indices when saving bitmap", GUID_NULL, HRESULT_FROM_WIN32(GetLastError()));
    }

$end:

    if(hf != INVALID_HANDLE_VALUE)
    {
        // Close the .BMP file.
        if (!CloseHandle(hf))
        {
            hr = Error(L"Unable to close file handle when saving bitmap", GUID_NULL, HRESULT_FROM_WIN32(GetLastError()));
        }
    }

    // Free memory.
    GlobalFree((HGLOBAL)lpBits);
    return hr;
}


////////////////////////////////////////////////////////
// Draw Annotations
void CDMGraphCtrl::Annotate(HDC hDC)
{
    ATLASSERT(m_pAnnotationList);

    if(m_pAnnotationList->GetSize() == 0)
    {
        return;
    }

    //Start drawing all available labels.
    int i;

    for(i=0; i<m_pAnnotationList->GetSize(); i++)
    {
        CGraphAnnotation* anno = (*m_pAnnotationList)[i];

        if(!anno->m_bVisible)
        {
            continue;
        }

        SetTextColor (hDC, anno->m_Color);
        SetTextAlign(hDC, TA_LEFT|TA_BOTTOM);
        SetBkMode(hDC, TRANSPARENT);
        HFONT hOldFont;

        if(anno->m_bHorizontal)
        {
            hOldFont = SelectFontObject (hDC, m_fontIdent);
        }
        else
        {
            hOldFont = (HFONT)SelectObject(hDC, m_fontVLabel);
        }

        RECT textRect = { 0 };
        DrawTextW(hDC, anno->m_Caption, -1, &textRect, DT_SINGLELINE | DT_CALCRECT);
        ATLTRACE(_T("Width %d Height %d \n"),textRect.right - textRect.left, textRect.bottom - textRect.top);
        // calculate the corrdinate of ploting point.
        POINT point;
        Corrdinate(anno->place, &point);
        SIZE sz;

        if(anno->m_bHorizontal)
        {
            sz.cx = textRect.right - textRect.left;
            sz.cy = -(textRect.bottom - textRect.top);
        }
        else
        {
            sz.cx = -(textRect.bottom - textRect.top);
            sz.cy = -(textRect.right - textRect.left);
            point.x += (textRect.bottom - textRect.top)/2;
        }

        RECT rc;
        SetRect(&rc, point.x, point.y, point.x+sz.cx, point.y+sz.cy);

        // Create the new brush as the color of element symbol
        if (m_annoBrush == NULL)
        {
            m_annoBrush = CreateSolidBrush (anno->m_BkColor);
        }

        HBRUSH hBrushSave = (HBRUSH)SelectObject(hDC, m_annoBrush);
        POINT bottomRight = {rc.right, rc.bottom};
        POINT topLeft = {rc.left, rc.top};

        if (
            PtInRect(&m_axisRect, bottomRight) &&
            PtInRect(&m_axisRect, topLeft)
        )
        {
            FillRect(hDC, &rc, m_annoBrush);
            TextOutW(hDC, point.x, point.y, anno->m_Caption, anno->m_Caption.Length());
        }

        SelectObject(hDC, hOldFont);
        SelectObject(hDC, hBrushSave);

        if(m_annoBrush)
        {
            DeleteObject(m_annoBrush);
            m_annoBrush = NULL;
        }

        if(m_fontVLabel)
        {
            DeleteObject(m_fontVLabel);
            m_fontVLabel = NULL;
        }
    }
}

/////////////////////////////////////////////////////
// Draw Cursors
void CDMGraphCtrl::DrawCursor(HDC hDC, BOOL bOptimized)
{
    int i;
    ATLASSERT(m_pCursorList);

    for(i=0; i < m_pCursorList->GetSize(); i++)
    {
        CGraphCursor* cursor = (*m_pCursorList)[i];

        if(!cursor->m_bVisible)
        {
            continue;
        }

        if (m_cursorPen == NULL)
        {
            m_cursorPen = CreatePen(PS_SOLID, 0, cursor->m_Color);
        }

        HPEN hPenSave = (HPEN)SelectObject(hDC, m_cursorPen);

        if(cursor->m_nStyle == XY || cursor->m_nStyle == X)
        {
            POINT pt[2];
            Corrdinate(dRangeX[MIN], cursor->position.y, &pt[0]);
            Corrdinate(dRangeX[MAX], cursor->position.y, &pt[1]);
            MoveToEx(hDC, pt[0].x, pt[0].y, NULL);
            LineTo(hDC, pt[1].x, pt[1].y);
        }

        if(cursor->m_nStyle == XY || cursor->m_nStyle == Y)
        {
            POINT pt[2];
            Corrdinate(cursor->position.x, dRangeY[MIN], &pt[0]);
            Corrdinate(cursor->position.x, dRangeY[MAX], &pt[1]);
            MoveToEx(hDC, pt[0].x, pt[0].y, NULL);
            LineTo(hDC, pt[1].x, pt[1].y);
            //To show Cursor ID
            char strCursorID[1];
            strCursorID[0] = cursor->m_shCursorID+48;
            RECT rcBox;
            rcBox.left = pt[1].x-10;
            rcBox.top = pt[1].y+14;
            rcBox.right = pt[1].x;
            rcBox.bottom = pt[1].y;
            HBRUSH brBox;
            brBox = CreateSolidBrush(RGB(0,0,125));
            ::FillRect(hDC, &rcBox, brBox);

            if ( brBox )
            {
                DeleteObject(brBox);
            }

            ::TextOut(hDC, pt[1].x, pt[1].y, (LPCWSTR) strCursorID, ARRAYSIZE(strCursorID));
        }

        if (! bOptimized)
        {
            SelectObject(hDC, hPenSave);
        }

        if(m_cursorPen)
        {
            DeleteObject(m_cursorPen);
            m_cursorPen = NULL;
        }
    }
}

void CDMGraphCtrl::CursorPosition(const LPPOINT point)
{
    long nCursorIdx;
    ATLASSERT(m_pCursorList);
    m_pCursorList->get_Selected(&nCursorIdx);

    if(nCursorIdx < 0)
    {
        return;
    }

    CGraphCursor* pCursor = (*m_pCursorList)[ nCursorIdx ];
    ATLASSERT(pCursor);
    double rx,ry;
    rx = PT2DBLX(point->x);
    ry = PT2DBLY(point->y);
    pCursor->position.x = rx;
    pCursor->position.y = ry;
    CElementPoint pt(rx,ry);

    if (PtInRect(&m_axisRect, *point) && m_nCursorCount == 2 )
    {
        if(pCursor->m_nMode > 0)
        {
            pCursor->position.x = rx;
            pCursor->position.y = ry;
            /* Get hold of unselected cursor */
            CGraphCursor* pUnSelCursor;
            long nUnSelCursorIdx;

            if ( nCursorIdx == 0 )
            {
                nUnSelCursorIdx = 1;
                pUnSelCursor= (*m_pCursorList)[ nUnSelCursorIdx ];
            }
            else
            {
                nUnSelCursorIdx = 0;
                pUnSelCursor= (*m_pCursorList)[ nUnSelCursorIdx ];
            }

            /* Check if swap is needed */
            SHORT shSelCursorID, shUnSelCursorID;
            pCursor->get_CursorID(&shSelCursorID);
            pUnSelCursor->get_CursorID(&shUnSelCursorID);

            if ( pUnSelCursor->position.x < pCursor->position.x )
            {
                if ( shUnSelCursorID != 1 )
                {
                    /* Swap */
                    pCursor->put_CursorID(shUnSelCursorID);
                    pUnSelCursor->put_CursorID(shSelCursorID);
                    m_pCursorList->put_Selected(nUnSelCursorIdx);
                }
            }
            else if ( pUnSelCursor->position.x > pCursor->position.x )
            {
                if ( shUnSelCursorID != 2 )
                {
                    /* Swap */
                    pCursor->put_CursorID(shUnSelCursorID);
                    pUnSelCursor->put_CursorID(shSelCursorID);
                    m_pCursorList->put_Selected(nUnSelCursorIdx);
                }
            }
        }
    }

    FireViewChange();
    vSendCursorDetails();
}

BOOL CDMGraphCtrl::FindPoint(double cursor_x, double cursor_y, CElementPoint* pResult)
{
    ATLASSERT(pResult);
    long nElementIdx;
    ATLASSERT(m_pElementList);
    m_pElementList->get_Selected(&nElementIdx);

    if(nElementIdx < 0)
    {
        return FALSE;
    }

    CGraphElement* pElement = (*m_pElementList)[ nElementIdx ];
    ATLASSERT(pElement);
    return pElement->FindPoint(cursor_x, cursor_y, pResult);
}

// helper function
HRESULT WINAPI GetSelectedCursor(IDMGraphCtrl* pGraphCtrl, IDMGraphCursor** pCursor)
{
    HRESULT hr;
    ATLASSERT(pGraphCtrl);
    ATLASSERT(pCursor);
    *pCursor = NULL;
    CComPtr<IDMGraphCollection> spGraphCollection;
    hr = pGraphCtrl->get_Cursors(&spGraphCollection);
    long selected = 0;
    CComPtr<IDispatch> spItem;
    hr = spGraphCollection->get_Selected(&selected);

    if(selected<0)
    {
        return S_FALSE;
    }

    hr = spGraphCollection->get_Item(selected, &spItem);
    return spItem.QueryInterface(pCursor);
}

void CDMGraphCtrl::vSendCursorDetails()
{
    HRESULT hr;
    CComPtr<IDMGraphCollection> spGraphCollection;
    hr = get_Cursors(&spGraphCollection);
    long lngCount = 0;
    spGraphCollection->get_Count(&lngCount);

    for (int nIndex = 0 ; nIndex < lngCount; nIndex++ )
    {
        CComPtr<IDispatch> spItem;
        CComPtr<IDMGraphCursor> spCursor;
        hr = spGraphCollection->get_Item(nIndex, &spItem);
        spItem.QueryInterface(&spCursor);
        /* POINT ptCursor; */
        DOUBLE dLogX,dLogY;
        short shCursorID;
        spCursor->get_X(&dLogX);
        spCursor->get_Y(&dLogY);
        spCursor->get_CursorID(&shCursorID);
        //Send cursor details to client.
        Fire_CursorPosition(dLogX, dLogY, shCursorID);
    }
}

void CDMGraphCtrl::vCursorEventHandler(EDMGRAPHEVENT eEvent, long x, long y)
{
    m_objCursorStMachine.eApplyEvents(eEvent);

    if( m_objCursorStMachine.eGetCurrState() == FIRST_OFFSET_DATA ||
            m_objCursorStMachine.eGetCurrState() == LAST_OFFSET_DATA   )
    {
        if(m_nCursorCount == 2
                && m_objCursorStMachine.eGetCurrState() == LAST_OFFSET_DATA )
        {
            return;
        }

        HRESULT hr;
        CComPtr<IDMGraphCollection> spGraphCollection;
        hr = get_Cursors(&spGraphCollection);
        CComPtr<IDispatch> spDispatch;
        CComPtr<IDMGraphCursor> spCursor;
        hr = spGraphCollection->Add(&spDispatch);   //adds to the end and makes it selected
        hr = GetSelectedCursor(this, &spCursor);
        hr= spGraphCollection->get_Count((long*)&m_nCursorCount);

        if (hr == S_OK)
        {
            spCursor->put_X(PT2DBLX(x));
            spCursor->put_Y(PT2DBLX(y));
            spCursor->put_CursorID((short)m_nCursorCount);
        }
        else
        {
            return;
        }

        vSendCursorDetails();
    }
    else if( eEvent == EVENT_RDBL_CLK )
    {
        m_nCursorCount = 0;
        HRESULT hr;
        CComPtr<IDMGraphCollection> spGraphCollection;
        hr = get_Cursors(&spGraphCollection);
        //Delete both the cursors
        spGraphCollection->Delete(0);
        spGraphCollection->Delete(0);
        Fire_CursorPosition( 0, 0, 1);
        Fire_CursorPosition( 0, 0, 2);
    }
}

void CDMGraphCtrl::vUpdateCursorSelection(POINT point)
{
    if ( !PtInRect (&m_axisRect, point) )
    {
        return;
    }

    HRESULT hr;
    CComPtr<IDMGraphCollection> spGraphCollection;
    hr = get_Cursors(&spGraphCollection);
    long lngCount = 0;
    spGraphCollection->get_Count(&lngCount);

    for (int nIndex = 0 ; nIndex < lngCount; nIndex++ )
    {
        CComPtr<IDispatch> spItem;
        CComPtr<IDMGraphCursor> spCursor;
        hr = spGraphCollection->get_Item(nIndex, &spItem);
        spItem.QueryInterface(&spCursor);
        POINT ptCursor;
        double dLogX,dLogY;
        spCursor->get_X(&dLogX);
        spCursor->get_Y(&dLogY);
        Corrdinate(dLogX, dLogY, &ptCursor);

        if ( ptCursor.x-5 <= point.x &&
                ptCursor.x+5 >= point.x )
        {
            /*HCURSOR hCursor;
            hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE));
            ::SetCursor(hCursor);*/
            spGraphCollection->put_Selected(nIndex);
            break;
        }
    }
}

//update tool tip text and position
void CDMGraphCtrl::UpdateToolTip(const LPPOINT pt, WPARAM wParam)
{
    double rx,ry;
    rx = PT2DBLX(pt->x);
    ry = PT2DBLY(pt->y);

    if(m_eTrackMode == TrackElement)
    {
        ATLASSERT(m_pElementList);

        if(m_pElementList->GetSize() == 0)
        {
            m_ToolTipText[0] = '\0';
        }
        else
        {
            //find the closest point with such values from each element
            CElementPoint closestPoint;
            int i, j, found_i = -1, found_j = -1;
            double dx, dy, dr;

            for(i=0; i<m_pElementList->GetSize(); i++)
            {
                CGraphElement* pElement = (*m_pElementList)[i];
                ATLASSERT(pElement);

                for(j=0; j<pElement->m_PointList.GetSize(); j++)
                {
                    CElementPoint point = (*m_pElementList)[i]->m_PointList[j];
                    dx = fabs(rx - point.x);
                    dy = fabs(ry - point.y);

                    if(i == 0 && j == 0)
                    {
                        dr = sqrt(dx + dy);
                    }
                    else
                    {
                        if (sqrt(dx+dy) < dr )
                        {
                            dr = sqrt(dx+dy);
                            found_i = i;
                            found_j = j;
                        }
                    }
                }
            }

            if(found_i < 0 && found_j < 0 && m_pElementList->GetSize() && (*m_pElementList)[0]->m_PointList.GetSize())
            {
                found_i = found_j = 0; //if we have points and we did not find it then it is the first one
            }

            if(found_i >=0 && found_j >= 0)
            {
                /*CComBSTR bsX, bsY, bsName;*/
                BSTR bsX, bsY, bsName;
                CGraphElement* pElement = (*m_pElementList)[found_i];
                m_pAxis[ HorizontalX ]->FormatAxisLabel(pElement->m_PointList[found_j].x, &bsX);
                m_pAxis[ VerticalY ]->FormatAxisLabel(pElement->m_PointList[found_j].y, &bsY);
                pElement->get_Name(&bsName);
                sprintf_s((char*) m_ToolTipText, TOOLTIP_TXT_LEN, "%s: %s, %s", (char*) bsName, bsX, bsY);
                SysFreeString(bsX);
                SysFreeString(bsY);
                SysFreeString(bsName);
            }
            else
            {
                m_ToolTipText[0] = '\0';
            }
        }
    }
    else
    {
        ATLASSERT(m_eTrackMode == TrackCursor);
        CursorPosition(pt);
        long nCursorIdx;
        ATLASSERT(m_pCursorList);
        m_pCursorList->get_Selected(&nCursorIdx);

        if(nCursorIdx < 0)
        {
            m_ToolTipText[0] = '\0';
        }
        else
        {
            /*CComBSTR bsX, bsY, bsName;*/
            BSTR bsX, bsY, bsName;
            CGraphCursor* pCursor = (*m_pCursorList)[ nCursorIdx ];
            ATLASSERT(pCursor);
            m_pAxis[ HorizontalX ]->FormatAxisLabel(rx, &bsX);
            m_pAxis[ VerticalY ]->FormatAxisLabel(ry, &bsY);
            pCursor->get_Name(&bsName);
            sprintf_s((char*) m_ToolTipText, TOOLTIP_TXT_LEN, "%s: %s, %s", (char*) bsName, bsX, bsY);
            SysFreeString(bsX);
            SysFreeString(bsY);
            SysFreeString(bsName);
        }
    }

    m_ttip.SendMessage(TTM_UPDATETIPTEXT, 0, (LPARAM)&m_ti);
    POINT pointScreen;
    pointScreen.x = pt->x;
    pointScreen.y = pt->y;
    ClientToScreen(&pointScreen);
    m_ttip.SendMessage(TTM_TRACKPOSITION, 0, MAKELPARAM(pointScreen.x + X_OFFSET, pointScreen.y + Y_OFFSET));
    RelayEvent(WM_LBUTTONDOWN, wParam,  MAKELPARAM(LOWORD(pt->x), LOWORD(pt->y)));
}

LRESULT CDMGraphCtrl::OnRButtonDblClk(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
    POINT point = { (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam) };

    if (PtInRect (&m_axisRect, point))
    {
        vCursorEventHandler(EVENT_RDBL_CLK);
    }

    return 0;
}
