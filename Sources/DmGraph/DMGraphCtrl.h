/////////////////////////////////////////////////////////////////////////////
// (c) Copyright The Code Project Open License (CPOL)
//  http://www.codeproject.com/info/cpol10.aspx
//
//  Marius Samoila, Nikolai Teofilov, 2011
//
// FILE NAME
//      DMGraphCtrl.h: Declaration of the CDMGraphCtrl class
//
// CLASS NAME
//      CDMGraphCtrl
//
// DESCRIPTION
//
// MODIFICATIONS
//      01-Dec-2011 MSamoila major re-design of old CNTGraphCtrl
//

#ifndef __DMGRAPHCTRL_H_
#define __DMGRAPHCTRL_H_

#include "resource.h"       // main symbols
#include <atlctl.h>
#include "DMGraphCP.h"
#include "GraphCollection.h"
#include "ElementPoint.h"
#include "DMGraphCursorState.h"

#define TOOLTIP_TXT_LEN 128

class CGraphElement;
class CGraphAnnotation;
class CGraphCursor;
class CGraphAxis;

/////////////////////////////////////////////////////////////////////////////
// CDMGraphCtrl
class ATL_NO_VTABLE CDMGraphCtrl :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CStockPropImpl<CDMGraphCtrl, IDMGraphCtrl, &IID_IDMGraphCtrl, &LIBID_DMGraphLib>,
    public CComControl<CDMGraphCtrl>,
    public IPersistStreamInitImpl<CDMGraphCtrl>,
    public IOleControlImpl<CDMGraphCtrl>,
    public IOleObjectImpl<CDMGraphCtrl>,
    public IOleInPlaceActiveObjectImpl<CDMGraphCtrl>,
    public IViewObjectExImpl<CDMGraphCtrl>,
    public IOleInPlaceObjectWindowlessImpl<CDMGraphCtrl>,
    public ISupportErrorInfo,
    public IConnectionPointContainerImpl<CDMGraphCtrl>,
    public IPersistStorageImpl<CDMGraphCtrl>,
    public ISpecifyPropertyPagesImpl<CDMGraphCtrl>,
    public IQuickActivateImpl<CDMGraphCtrl>,
    public IDataObjectImpl<CDMGraphCtrl>,
    public IProvideClassInfo2Impl<&CLSID_DMGraphCtrl, &DIID__IDMGraphCtrlEvents, &LIBID_DMGraphLib>,
    public IPropertyNotifySinkCP<CDMGraphCtrl>,
    public CComCoClass<CDMGraphCtrl, &CLSID_DMGraphCtrl>,
    public IObjectSafetyImpl<CDMGraphCtrl, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
    public CProxy_IDMGraphCtrlEvents< CDMGraphCtrl >
{
public:
    CDMGraphCtrl();
    ~CDMGraphCtrl();

    HRESULT FinalConstruct();

    DECLARE_REGISTRY_RESOURCEID(IDR_DMGRAPHCTRL)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    DECLARE_WND_CLASS(_T("DmGraphCtrl_class"))

    BEGIN_COM_MAP(CDMGraphCtrl)
    COM_INTERFACE_ENTRY(IDMGraphCtrl)
    COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IViewObjectEx)
    COM_INTERFACE_ENTRY(IViewObject2)
    COM_INTERFACE_ENTRY(IViewObject)
    COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
    COM_INTERFACE_ENTRY(IOleInPlaceObject)
    COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
    COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
    COM_INTERFACE_ENTRY(IOleControl)
    COM_INTERFACE_ENTRY(IOleObject)
    COM_INTERFACE_ENTRY(IPersistStreamInit)
    COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
    COM_INTERFACE_ENTRY(ISupportErrorInfo)
    COM_INTERFACE_ENTRY(IConnectionPointContainer)
    COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
    COM_INTERFACE_ENTRY(IQuickActivate)
    COM_INTERFACE_ENTRY(IPersistStorage)
    COM_INTERFACE_ENTRY(IDataObject)
    COM_INTERFACE_ENTRY(IProvideClassInfo)
    COM_INTERFACE_ENTRY(IProvideClassInfo2)
    COM_INTERFACE_ENTRY(IObjectSafety)
    COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
    END_COM_MAP()

    BEGIN_PROP_MAP(CDMGraphCtrl)
    PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
    PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
    PROP_ENTRY_TYPE("Appearance", DISPID_APPEARANCE, CLSID_NULL, VT_I4)
    PROP_ENTRY_TYPE("Caption", DISPID_CAPTION, CLSID_NULL, VT_BSTR)
    PROP_ENTRY_TYPE("Enabled", DISPID_ENABLED, CLSID_NULL, VT_BOOL)
    PROP_PAGE(CLSID_ElementPropPage)
    PROP_PAGE(CLSID_DMGraphPropPage)
    PROP_PAGE(CLSID_AnnoPropPage)
    PROP_PAGE(CLSID_CursorPropPage)
    PROP_PAGE(CLSID_FormatPropPage)
    // Example entries
    // PROP_ENTRY("Property Description", dispid, clsid)
    // PROP_PAGE(CLSID_StockColorPage)
    END_PROP_MAP()

    BEGIN_CONNECTION_POINT_MAP(CDMGraphCtrl)
    CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
    CONNECTION_POINT_ENTRY(DIID__IDMGraphCtrlEvents)
    END_CONNECTION_POINT_MAP()

    BEGIN_MSG_MAP(CDMGraphCtrl)
    MESSAGE_HANDLER(WM_RBUTTONDBLCLK, OnRButtonDblClk)
    CHAIN_MSG_MAP(CComControl<CDMGraphCtrl>)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
    MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
    MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
    MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
    MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRButtonDown)
    MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
    DEFAULT_REFLECTION_HANDLER()
    END_MSG_MAP()
    // Handler prototypes:
    //  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    //  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    //  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        return TRUE; //prevent system default erasing (avoid flickering)
    }


    // ISupportsErrorInfo
    STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
    {
        static const IID* arr[] =
        {
            &IID_IDMGraphCtrl,
        };

        for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
        {
            if (::InlineIsEqualGUID(*arr[i], riid))
            {
                return S_OK;
            }
        }

        return S_FALSE;
    }

    // IViewObjectEx
    DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

    // IDMGraphCtrl
public:
    STDMETHOD(SaveAs)(/*[in]*/ BSTR szFilename);
    STDMETHOD(ShowProperties)();
    STDMETHOD(ClearGraph)();
    STDMETHOD(PrintGraph)();
    STDMETHOD(CopyToClipboard)();
    STDMETHOD(AutoRange)();
    STDMETHOD(SetRange)(/*[in]*/ double xmin, /*[in]*/ double xmax, /*[in]*/ double ymin, /*[in]*/ double ymax);
    STDMETHOD(GetRange)(/*[out]*/ double* xmin, /*[out]*/ double* xmax, /*[out]*/ double* ymin, /*[out]*/ double* ymax);
    STDMETHOD(GetAutoRange)(/*[out]*/ double* xmin, /*[out]*/ double* xmax, /*[out]*/ double* ymin, /*[out]*/ double* ymax);
    STDMETHOD(get_Cursors)(/*[out, retval]*/ IDMGraphCollection* *pVal);
    STDMETHOD(get_Elements)(/*[out, retval]*/ IDMGraphCollection* *pVal);
    STDMETHOD(get_Annotations)(/*[out, retval]*/ IDMGraphCollection* *pVal);
    STDMETHOD(get_Axis)(/*[in]*/ Orientation eOrientation, /*[out, retval]*/ IDMGraphAxis* *pVal);
    STDMETHOD(get_FrameStyle)(/*[out, retval]*/ FrameStyle* pVal);
    STDMETHOD(put_FrameStyle)(/*[in]*/ FrameStyle newVal);
    STDMETHOD(get_PlotAreaColor)(/*[out, retval]*/ OLE_COLOR* pVal);
    STDMETHOD(put_PlotAreaColor)(/*[in]*/ OLE_COLOR newVal);
    STDMETHOD(get_ControlFrameColor)(/*[out, retval]*/ OLE_COLOR* pVal);
    STDMETHOD(put_ControlFrameColor)(/*[in]*/ OLE_COLOR newVal);
    STDMETHOD(get_PlotAreaPicture)(/*[out, retval]*/ IPictureDisp* *pVal);
    STDMETHOD(put_PlotAreaPicture)(/*[in]*/ IPictureDisp* newVal);
    STDMETHOD(get_ControlFramePicture)(/*[out, retval]*/ IPictureDisp* *pVal);
    STDMETHOD(put_ControlFramePicture)(/*[in]*/ IPictureDisp* newVal);
    STDMETHOD(get_TrackMode)(/*[out, retval]*/ TrackModeState* pVal);
    STDMETHOD(put_TrackMode)(/*[in]*/ TrackModeState newVal);
    STDMETHOD(get_ElementIdentify)(/*[out, retval]*/ VARIANT_BOOL* pVal);
    STDMETHOD(put_ElementIdentify)(/*[in]*/ VARIANT_BOOL newVal);
    STDMETHOD(get_IdentFont)(/*[out, retval]*/ IFontDisp* *pVal);
    STDMETHOD(put_IdentFont)(/*[in]*/ IFontDisp* newVal);
    STDMETHOD(get_TitleFont)(/*[out, retval]*/ IFontDisp* *pVal);
    STDMETHOD(put_TitleFont)(/*[in]*/ IFontDisp* newVal);
    STDMETHOD(get_TickFont)(/*[out, retval]*/ IFontDisp* *pVal);
    STDMETHOD(put_TickFont)(/*[in]*/ IFontDisp* newVal);
    STDMETHOD(get_LabelFont)(/*[out, retval]*/ IFontDisp* *pVal);
    STDMETHOD(put_LabelFont)(/*[in]*/ IFontDisp* newVal);
    STDMETHOD(get_LabelColor)(/*[out, retval]*/ OLE_COLOR* pVal);
    STDMETHOD(put_LabelColor)(/*[in]*/ OLE_COLOR newVal);
    STDMETHOD(get_GridColor)(/*[out, retval]*/ OLE_COLOR* pVal);
    STDMETHOD(put_GridColor)(/*[in]*/ OLE_COLOR newVal);
    STDMETHOD(get_AxisColor)(/*[out, retval]*/ OLE_COLOR* pVal);
    STDMETHOD(put_AxisColor)(/*[in]*/ OLE_COLOR newVal);
    STDMETHOD(AboutBox)();
    STDMETHOD(ShiftDisplay)(/*[in]*/SHORT xShift,/*[in]*/SHORT yShift);
    STDMETHOD(GenerateCSVReport)(/*[in]*/LPCTSTR pFilename);
    STDMETHOD(GetElementValueAtCursor)(/*[in]*/ SHORT shElementID,
            /*[in]*/ DOUBLE dblCursorVal, /*[out]*/ DOUBLE* dblElemVal);

    HRESULT OnDraw(ATL_DRAWINFO& di)
    {
        m_rcBounds = *(RECT*)di.prcBounds;
        DrawGraphOffScreen(di.hdcDraw, &m_rcBounds, di.bOptimize);
        return S_OK;
    }

    // IPersistStreamInit
    STDMETHOD(InitNew)();

    CComBSTR            m_bstrCaption;
    COLORREF            m_colAxis,
                        m_colGrid,
                        m_colLabel;

    CComPtr<IFont>      m_fontTick,
            m_fontTitle,
            m_fontLabel,
            m_fontIdent;


    TrackModeState      m_eTrackMode;
    VARIANT_BOOL        m_bElementIdentify;

    CComPtr<IPicture>   m_spControlFramePicture,
            m_spPlotAreaPicture,
            m_spPicFrame;

    COLORREF            m_colControlFrameColor,
                        m_colPlotAreaColor;
    FrameStyle          m_eFrameStyle;

    void Refresh(BOOL withPenAndBrushes);
    void UpdateAutoRange(double X, double Y, BOOL bFirstPoint);
    void OnPlotDeleted(); //updates m_nAvailablePlots when a graph with at least one point was deleted

    // Implementation
private:
    void DrawGraphOffScreen(HDC hDC, LPCRECT prcBounds, BOOL bOptimized);
    void DrawGraph(HDC hDC, LPCRECT prcBounds, BOOL bOptimized);
    void DrawControlFrame(HDC hDC, BOOL bOptimized);
    void DrawCursor(HDC hDC, BOOL bOptimized);
    void DrawTitle(HDC hDC);
    void DrawGrid(HDC hDC, BOOL bOptimized);
    void DrawAxis(HDC hDC, BOOL bOptimized);
    void DrawTicks(HDC hDC, BOOL bOptimized);
    void DrawAxisLabel(HDC hDC);
    void DrawXLogGrid(HDC hDC, BOOL bOptimized);
    void DrawYLogGrid(HDC hDC, BOOL bOptimized);
    void PlotElement(HDC hDC, BOOL bOptimized);
    void DrawElementLabel(HDC hDC, CGraphElement* pElement, int index);
    void PlotElementLines(HDC hDC, CGraphElement* pElement, BOOL bOptimized);
    void PlotElementPoints(HDC hDC, CGraphElement* pElement, BOOL bOptimized);
    void CursorPosition(const LPPOINT point);
    void Annotate(HDC hDC);
    void ResCalc();
    void CalcRect(HDC hDC);
    void CreateFont(HDC hDC);
    void Corrdinate(double x, double y, LPPOINT pt);
    void Corrdinate(CElementPoint ptElem, LPPOINT pt);
public:
    BOOL FindPoint(double x, double y, CElementPoint* pResult);
private:
    void PrepareForDrawing(HDC hDC, LPCRECT prect);
    void FillToolInfo(TOOLINFO* ti);
    void DoPan(const LPPOINT point, long mode);
    void DoZoom(UINT nFlags, const LPPOINT point);
    void RelayEvent(UINT message, WPARAM wParam, LPARAM lParam);
    HRESULT CreateBMPFile(LPCOLESTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC);
    HRESULT CreateBitmapInfoStruct(HBITMAP hBmp, PBITMAPINFO* pbi);

    void vCursorEventHandler(EDMGRAPHEVENT eEvent, long x = 0, long y = 0);
    void vUpdateCursorSelection(POINT point);
    void vSendCursorDetails();

    HFONT SelectFontObject(HDC hdc, IFont* pFont);
    void OnFontChanged();
    //update tool tip text and position
    void UpdateToolTip(const LPPOINT pt, WPARAM wParam);
    //Helper funtion to get the Y value for element at a Given X
    double DetermineY(CElementPoint point1, CElementPoint point2, double XValue);


    RECT    m_rcBounds,                   // Bounding rectangle for drawing
            m_ctlRect,
            m_axisRect;

    HFONT m_fontY, m_fontVLabel;

    double dResY,           dResX ;
    double dRangeY[2],      dRangeX[2];
    double dOldRangeX[2],   dOldRangeY[2];
    double dAutoRangeX[2],  dAutoRangeY[2];

    HPEN    m_axisPen, m_gridPen, m_cursorPen;
    HBRUSH  m_brushPlot, m_brushFrame;

    HPEN    m_linePen,  m_pointPen;
    HBRUSH  m_lineBrush, m_pointBrush, m_annoBrush;

    CComPtr<IPicture>   m_picControlFrame,
            m_picPlotArea,
            m_picFrame; // Default bitmap frame (FrameStyle = Bitmap)

    POINT   m_panPoint;
    BOOL    m_bUnzoom;
    long    m_nAvailablePlots; //non zero if we have at least one point in one element to draw
    double RoundUp(double val, int ex);

    CWindow     m_ttip;
    TOOLINFO    m_ti;
    char       m_ToolTipText[ TOOLTIP_TXT_LEN ];

    short m_nCursorCount;
    CDMGraphCursorState m_objCursorStMachine;

public:
    CGraphCollection<CGraphElement>* m_pElementList; // Element List
private:
    CGraphCollection<CGraphAnnotation>* m_pAnnotationList;

    CGraphCollection<CGraphCursor>* m_pCursorList;

    CGraphAxis*             m_pAxis[2]; //the X and Y axis
    LRESULT OnRButtonDblClk(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};

#endif //__DMGRAPHCTRL_H_
