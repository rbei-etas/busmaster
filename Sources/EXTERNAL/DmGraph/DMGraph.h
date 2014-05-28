

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Wed May 28 09:52:23 2014
 */
/* Compiler settings for DMGraph.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __DMGraph_h__
#define __DMGraph_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDMGraphElement_FWD_DEFINED__
#define __IDMGraphElement_FWD_DEFINED__
typedef interface IDMGraphElement IDMGraphElement;
#endif 	/* __IDMGraphElement_FWD_DEFINED__ */


#ifndef __IDMGraphAnnotation_FWD_DEFINED__
#define __IDMGraphAnnotation_FWD_DEFINED__
typedef interface IDMGraphAnnotation IDMGraphAnnotation;
#endif 	/* __IDMGraphAnnotation_FWD_DEFINED__ */


#ifndef __IDMGraphCursor_FWD_DEFINED__
#define __IDMGraphCursor_FWD_DEFINED__
typedef interface IDMGraphCursor IDMGraphCursor;
#endif 	/* __IDMGraphCursor_FWD_DEFINED__ */


#ifndef __IDMGraphAxis_FWD_DEFINED__
#define __IDMGraphAxis_FWD_DEFINED__
typedef interface IDMGraphAxis IDMGraphAxis;
#endif 	/* __IDMGraphAxis_FWD_DEFINED__ */


#ifndef __IDMGraphCollection_FWD_DEFINED__
#define __IDMGraphCollection_FWD_DEFINED__
typedef interface IDMGraphCollection IDMGraphCollection;
#endif 	/* __IDMGraphCollection_FWD_DEFINED__ */


#ifndef __IDMGraphCtrl_FWD_DEFINED__
#define __IDMGraphCtrl_FWD_DEFINED__
typedef interface IDMGraphCtrl IDMGraphCtrl;
#endif 	/* __IDMGraphCtrl_FWD_DEFINED__ */


#ifndef __IDMGraphElement_FWD_DEFINED__
#define __IDMGraphElement_FWD_DEFINED__
typedef interface IDMGraphElement IDMGraphElement;
#endif 	/* __IDMGraphElement_FWD_DEFINED__ */


#ifndef __IDMGraphAnnotation_FWD_DEFINED__
#define __IDMGraphAnnotation_FWD_DEFINED__
typedef interface IDMGraphAnnotation IDMGraphAnnotation;
#endif 	/* __IDMGraphAnnotation_FWD_DEFINED__ */


#ifndef __IDMGraphCursor_FWD_DEFINED__
#define __IDMGraphCursor_FWD_DEFINED__
typedef interface IDMGraphCursor IDMGraphCursor;
#endif 	/* __IDMGraphCursor_FWD_DEFINED__ */


#ifndef __IDMGraphAxis_FWD_DEFINED__
#define __IDMGraphAxis_FWD_DEFINED__
typedef interface IDMGraphAxis IDMGraphAxis;
#endif 	/* __IDMGraphAxis_FWD_DEFINED__ */


#ifndef __IDMGraphCollection_FWD_DEFINED__
#define __IDMGraphCollection_FWD_DEFINED__
typedef interface IDMGraphCollection IDMGraphCollection;
#endif 	/* __IDMGraphCollection_FWD_DEFINED__ */


#ifndef ___IDMGraphCtrlEvents_FWD_DEFINED__
#define ___IDMGraphCtrlEvents_FWD_DEFINED__
typedef interface _IDMGraphCtrlEvents _IDMGraphCtrlEvents;
#endif 	/* ___IDMGraphCtrlEvents_FWD_DEFINED__ */


#ifndef __DMGraphCtrl_FWD_DEFINED__
#define __DMGraphCtrl_FWD_DEFINED__

#ifdef __cplusplus
typedef class DMGraphCtrl DMGraphCtrl;
#else
typedef struct DMGraphCtrl DMGraphCtrl;
#endif /* __cplusplus */

#endif 	/* __DMGraphCtrl_FWD_DEFINED__ */


#ifndef __ElementPropPage_FWD_DEFINED__
#define __ElementPropPage_FWD_DEFINED__

#ifdef __cplusplus
typedef class ElementPropPage ElementPropPage;
#else
typedef struct ElementPropPage ElementPropPage;
#endif /* __cplusplus */

#endif 	/* __ElementPropPage_FWD_DEFINED__ */


#ifndef __AnnoPropPage_FWD_DEFINED__
#define __AnnoPropPage_FWD_DEFINED__

#ifdef __cplusplus
typedef class AnnoPropPage AnnoPropPage;
#else
typedef struct AnnoPropPage AnnoPropPage;
#endif /* __cplusplus */

#endif 	/* __AnnoPropPage_FWD_DEFINED__ */


#ifndef __CursorPropPage_FWD_DEFINED__
#define __CursorPropPage_FWD_DEFINED__

#ifdef __cplusplus
typedef class CursorPropPage CursorPropPage;
#else
typedef struct CursorPropPage CursorPropPage;
#endif /* __cplusplus */

#endif 	/* __CursorPropPage_FWD_DEFINED__ */


#ifndef __FormatPropPage_FWD_DEFINED__
#define __FormatPropPage_FWD_DEFINED__

#ifdef __cplusplus
typedef class FormatPropPage FormatPropPage;
#else
typedef struct FormatPropPage FormatPropPage;
#endif /* __cplusplus */

#endif 	/* __FormatPropPage_FWD_DEFINED__ */


#ifndef __DMGraphPropPage_FWD_DEFINED__
#define __DMGraphPropPage_FWD_DEFINED__

#ifdef __cplusplus
typedef class DMGraphPropPage DMGraphPropPage;
#else
typedef struct DMGraphPropPage DMGraphPropPage;
#endif /* __cplusplus */

#endif 	/* __DMGraphPropPage_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_DMGraph_0000_0000 */
/* [local] */ 

typedef /* [public][public][public][helpstring][uuid] */  DECLSPEC_UUID("865EDFD4-9E4D-4752-907A-033D45C1C7F5") 
enum __MIDL___MIDL_itf_DMGraph_0000_0000_0001
    {	Solid	= 0,
	Dash	= ( Solid + 1 ) ,
	Dot	= ( Dash + 1 ) ,
	DashDot	= ( Dot + 1 ) ,
	DashDotDot	= ( DashDot + 1 ) ,
	Null	= ( DashDotDot + 1 ) ,
	XYStep	= ( Null + 1 ) ,
	YXStep	= ( XYStep + 1 ) ,
	Bars	= ( YXStep + 1 ) ,
	Stick	= ( Bars + 1 ) 
    } 	LineType;

typedef /* [public][public][public][helpstring][uuid] */  DECLSPEC_UUID("A096E330-15BA-42C1-BD53-2C7E0B96CF77") 
enum __MIDL___MIDL_itf_DMGraph_0000_0000_0002
    {	Nosym	= 0,
	Dots	= ( Nosym + 1 ) ,
	Rectangles	= ( Dots + 1 ) ,
	Diamonds	= ( Rectangles + 1 ) ,
	Asterisk	= ( Diamonds + 1 ) ,
	DownTriangles	= ( Asterisk + 1 ) ,
	UpTriangles	= ( DownTriangles + 1 ) ,
	LeftTriangles	= ( UpTriangles + 1 ) ,
	RightTriangles	= ( LeftTriangles + 1 ) 
    } 	SymbolType;

typedef /* [public][public][public][helpstring][uuid] */  DECLSPEC_UUID("77D6CAFB-62A7-4916-A188-9A76D6194870") 
enum __MIDL___MIDL_itf_DMGraph_0000_0000_0003
    {	Flat	= 0,
	Scope	= ( Flat + 1 ) ,
	Bitmap	= ( Scope + 1 ) 
    } 	FrameStyle;

typedef /* [public][public][public][public][helpstring][uuid] */  DECLSPEC_UUID("7F2E3118-D790-42AE-81C9-FA14B54FC817") 
enum __MIDL___MIDL_itf_DMGraph_0000_0000_0004
    {	None	= 0,
	TrackCursor	= ( None + 1 ) ,
	TrackElement	= ( TrackCursor + 1 ) ,
	Zoom	= ( TrackElement + 1 ) ,
	ZoomX	= ( Zoom + 1 ) ,
	ZoomY	= ( ZoomX + 1 ) ,
	PanXY	= ( ZoomY + 1 ) ,
	PanX	= ( PanXY + 1 ) ,
	PanY	= ( PanX + 1 ) 
    } 	TrackModeState;

typedef /* [public][public][public][helpstring][uuid] */  DECLSPEC_UUID("5075d5fa-ebc1-4023-9c64-e576e7c58854") 
enum __MIDL___MIDL_itf_DMGraph_0000_0000_0005
    {	Fixed	= 0,
	Floating	= ( Fixed + 1 ) ,
	Snap	= ( Floating + 1 ) 
    } 	CursorMode;

typedef /* [public][public][public][helpstring][uuid] */  DECLSPEC_UUID("c2fe71fe-475c-4cfe-a33a-12eba6ffef37") 
enum __MIDL___MIDL_itf_DMGraph_0000_0000_0006
    {	XY	= 0,
	X	= ( XY + 1 ) ,
	Y	= ( X + 1 ) 
    } 	Crosshair;

typedef /* [public][public][helpstring][uuid] */  DECLSPEC_UUID("61a2b721-ecb0-49e8-9cdb-0509a62011a7") 
enum __MIDL___MIDL_itf_DMGraph_0000_0000_0007
    {	HorizontalX	= 0,
	VerticalY	= ( HorizontalX + 1 ) 
    } 	Orientation;



extern RPC_IF_HANDLE __MIDL_itf_DMGraph_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_DMGraph_0000_0000_v0_0_s_ifspec;

#ifndef __IDMGraphElement_INTERFACE_DEFINED__
#define __IDMGraphElement_INTERFACE_DEFINED__

/* interface IDMGraphElement */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IDMGraphElement;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4ca52ab5-b6ce-41af-9a09-284d00714f8e")
    IDMGraphElement : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LineColor( 
            /* [retval][out] */ OLE_COLOR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_LineColor( 
            /* [in] */ OLE_COLOR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PointColor( 
            /* [retval][out] */ OLE_COLOR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_PointColor( 
            /* [in] */ OLE_COLOR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Linetype( 
            /* [retval][out] */ LineType *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Linetype( 
            /* [in] */ LineType newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LineWidth( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_LineWidth( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PointSymbol( 
            /* [retval][out] */ SymbolType *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_PointSymbol( 
            /* [in] */ SymbolType newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SolidPoint( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SolidPoint( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PointSize( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_PointSize( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Show( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Show( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Name( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_XValue( 
            /* [in] */ long index,
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_XValue( 
            /* [in] */ long index,
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_YValue( 
            /* [in] */ long index,
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_YValue( 
            /* [in] */ long index,
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PlotXY( 
            /* [in] */ double X,
            /* [in] */ double Y) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PlotY( 
            /* [in] */ double Y) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Plot( 
            /* [in] */ VARIANT newXVals,
            /* [in] */ VARIANT newYVals) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDMGraphElementVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDMGraphElement * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDMGraphElement * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDMGraphElement * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDMGraphElement * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDMGraphElement * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDMGraphElement * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDMGraphElement * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LineColor )( 
            IDMGraphElement * This,
            /* [retval][out] */ OLE_COLOR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_LineColor )( 
            IDMGraphElement * This,
            /* [in] */ OLE_COLOR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PointColor )( 
            IDMGraphElement * This,
            /* [retval][out] */ OLE_COLOR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_PointColor )( 
            IDMGraphElement * This,
            /* [in] */ OLE_COLOR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Linetype )( 
            IDMGraphElement * This,
            /* [retval][out] */ LineType *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Linetype )( 
            IDMGraphElement * This,
            /* [in] */ LineType newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LineWidth )( 
            IDMGraphElement * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_LineWidth )( 
            IDMGraphElement * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PointSymbol )( 
            IDMGraphElement * This,
            /* [retval][out] */ SymbolType *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_PointSymbol )( 
            IDMGraphElement * This,
            /* [in] */ SymbolType newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SolidPoint )( 
            IDMGraphElement * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SolidPoint )( 
            IDMGraphElement * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PointSize )( 
            IDMGraphElement * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_PointSize )( 
            IDMGraphElement * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Show )( 
            IDMGraphElement * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Show )( 
            IDMGraphElement * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IDMGraphElement * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Name )( 
            IDMGraphElement * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IDMGraphElement * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_XValue )( 
            IDMGraphElement * This,
            /* [in] */ long index,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_XValue )( 
            IDMGraphElement * This,
            /* [in] */ long index,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_YValue )( 
            IDMGraphElement * This,
            /* [in] */ long index,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_YValue )( 
            IDMGraphElement * This,
            /* [in] */ long index,
            /* [in] */ double newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PlotXY )( 
            IDMGraphElement * This,
            /* [in] */ double X,
            /* [in] */ double Y);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PlotY )( 
            IDMGraphElement * This,
            /* [in] */ double Y);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Plot )( 
            IDMGraphElement * This,
            /* [in] */ VARIANT newXVals,
            /* [in] */ VARIANT newYVals);
        
        END_INTERFACE
    } IDMGraphElementVtbl;

    interface IDMGraphElement
    {
        CONST_VTBL struct IDMGraphElementVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDMGraphElement_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDMGraphElement_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDMGraphElement_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDMGraphElement_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDMGraphElement_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDMGraphElement_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDMGraphElement_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDMGraphElement_get_LineColor(This,pVal)	\
    ( (This)->lpVtbl -> get_LineColor(This,pVal) ) 

#define IDMGraphElement_put_LineColor(This,newVal)	\
    ( (This)->lpVtbl -> put_LineColor(This,newVal) ) 

#define IDMGraphElement_get_PointColor(This,pVal)	\
    ( (This)->lpVtbl -> get_PointColor(This,pVal) ) 

#define IDMGraphElement_put_PointColor(This,newVal)	\
    ( (This)->lpVtbl -> put_PointColor(This,newVal) ) 

#define IDMGraphElement_get_Linetype(This,pVal)	\
    ( (This)->lpVtbl -> get_Linetype(This,pVal) ) 

#define IDMGraphElement_put_Linetype(This,newVal)	\
    ( (This)->lpVtbl -> put_Linetype(This,newVal) ) 

#define IDMGraphElement_get_LineWidth(This,pVal)	\
    ( (This)->lpVtbl -> get_LineWidth(This,pVal) ) 

#define IDMGraphElement_put_LineWidth(This,newVal)	\
    ( (This)->lpVtbl -> put_LineWidth(This,newVal) ) 

#define IDMGraphElement_get_PointSymbol(This,pVal)	\
    ( (This)->lpVtbl -> get_PointSymbol(This,pVal) ) 

#define IDMGraphElement_put_PointSymbol(This,newVal)	\
    ( (This)->lpVtbl -> put_PointSymbol(This,newVal) ) 

#define IDMGraphElement_get_SolidPoint(This,pVal)	\
    ( (This)->lpVtbl -> get_SolidPoint(This,pVal) ) 

#define IDMGraphElement_put_SolidPoint(This,newVal)	\
    ( (This)->lpVtbl -> put_SolidPoint(This,newVal) ) 

#define IDMGraphElement_get_PointSize(This,pVal)	\
    ( (This)->lpVtbl -> get_PointSize(This,pVal) ) 

#define IDMGraphElement_put_PointSize(This,newVal)	\
    ( (This)->lpVtbl -> put_PointSize(This,newVal) ) 

#define IDMGraphElement_get_Show(This,pVal)	\
    ( (This)->lpVtbl -> get_Show(This,pVal) ) 

#define IDMGraphElement_put_Show(This,newVal)	\
    ( (This)->lpVtbl -> put_Show(This,newVal) ) 

#define IDMGraphElement_get_Name(This,pVal)	\
    ( (This)->lpVtbl -> get_Name(This,pVal) ) 

#define IDMGraphElement_put_Name(This,newVal)	\
    ( (This)->lpVtbl -> put_Name(This,newVal) ) 

#define IDMGraphElement_get_Count(This,pVal)	\
    ( (This)->lpVtbl -> get_Count(This,pVal) ) 

#define IDMGraphElement_get_XValue(This,index,pVal)	\
    ( (This)->lpVtbl -> get_XValue(This,index,pVal) ) 

#define IDMGraphElement_put_XValue(This,index,newVal)	\
    ( (This)->lpVtbl -> put_XValue(This,index,newVal) ) 

#define IDMGraphElement_get_YValue(This,index,pVal)	\
    ( (This)->lpVtbl -> get_YValue(This,index,pVal) ) 

#define IDMGraphElement_put_YValue(This,index,newVal)	\
    ( (This)->lpVtbl -> put_YValue(This,index,newVal) ) 

#define IDMGraphElement_PlotXY(This,X,Y)	\
    ( (This)->lpVtbl -> PlotXY(This,X,Y) ) 

#define IDMGraphElement_PlotY(This,Y)	\
    ( (This)->lpVtbl -> PlotY(This,Y) ) 

#define IDMGraphElement_Plot(This,newXVals,newYVals)	\
    ( (This)->lpVtbl -> Plot(This,newXVals,newYVals) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDMGraphElement_INTERFACE_DEFINED__ */


#ifndef __IDMGraphAnnotation_INTERFACE_DEFINED__
#define __IDMGraphAnnotation_INTERFACE_DEFINED__

/* interface IDMGraphAnnotation */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IDMGraphAnnotation;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7d965dbb-74cd-42d2-a9c6-53ed80188b35")
    IDMGraphAnnotation : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LabelCaption( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_LabelCaption( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LabelX( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_LabelX( 
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LabelY( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_LabelY( 
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LabelColor( 
            /* [retval][out] */ OLE_COLOR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_LabelColor( 
            /* [in] */ OLE_COLOR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LabelHorizontal( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_LabelHorizontal( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Visible( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Visible( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LabelBkColor( 
            /* [retval][out] */ OLE_COLOR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_LabelBkColor( 
            /* [in] */ OLE_COLOR newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDMGraphAnnotationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDMGraphAnnotation * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDMGraphAnnotation * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDMGraphAnnotation * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDMGraphAnnotation * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDMGraphAnnotation * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDMGraphAnnotation * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDMGraphAnnotation * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LabelCaption )( 
            IDMGraphAnnotation * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_LabelCaption )( 
            IDMGraphAnnotation * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LabelX )( 
            IDMGraphAnnotation * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_LabelX )( 
            IDMGraphAnnotation * This,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LabelY )( 
            IDMGraphAnnotation * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_LabelY )( 
            IDMGraphAnnotation * This,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LabelColor )( 
            IDMGraphAnnotation * This,
            /* [retval][out] */ OLE_COLOR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_LabelColor )( 
            IDMGraphAnnotation * This,
            /* [in] */ OLE_COLOR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LabelHorizontal )( 
            IDMGraphAnnotation * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_LabelHorizontal )( 
            IDMGraphAnnotation * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IDMGraphAnnotation * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IDMGraphAnnotation * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LabelBkColor )( 
            IDMGraphAnnotation * This,
            /* [retval][out] */ OLE_COLOR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_LabelBkColor )( 
            IDMGraphAnnotation * This,
            /* [in] */ OLE_COLOR newVal);
        
        END_INTERFACE
    } IDMGraphAnnotationVtbl;

    interface IDMGraphAnnotation
    {
        CONST_VTBL struct IDMGraphAnnotationVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDMGraphAnnotation_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDMGraphAnnotation_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDMGraphAnnotation_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDMGraphAnnotation_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDMGraphAnnotation_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDMGraphAnnotation_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDMGraphAnnotation_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDMGraphAnnotation_get_LabelCaption(This,pVal)	\
    ( (This)->lpVtbl -> get_LabelCaption(This,pVal) ) 

#define IDMGraphAnnotation_put_LabelCaption(This,newVal)	\
    ( (This)->lpVtbl -> put_LabelCaption(This,newVal) ) 

#define IDMGraphAnnotation_get_LabelX(This,pVal)	\
    ( (This)->lpVtbl -> get_LabelX(This,pVal) ) 

#define IDMGraphAnnotation_put_LabelX(This,newVal)	\
    ( (This)->lpVtbl -> put_LabelX(This,newVal) ) 

#define IDMGraphAnnotation_get_LabelY(This,pVal)	\
    ( (This)->lpVtbl -> get_LabelY(This,pVal) ) 

#define IDMGraphAnnotation_put_LabelY(This,newVal)	\
    ( (This)->lpVtbl -> put_LabelY(This,newVal) ) 

#define IDMGraphAnnotation_get_LabelColor(This,pVal)	\
    ( (This)->lpVtbl -> get_LabelColor(This,pVal) ) 

#define IDMGraphAnnotation_put_LabelColor(This,newVal)	\
    ( (This)->lpVtbl -> put_LabelColor(This,newVal) ) 

#define IDMGraphAnnotation_get_LabelHorizontal(This,pVal)	\
    ( (This)->lpVtbl -> get_LabelHorizontal(This,pVal) ) 

#define IDMGraphAnnotation_put_LabelHorizontal(This,newVal)	\
    ( (This)->lpVtbl -> put_LabelHorizontal(This,newVal) ) 

#define IDMGraphAnnotation_get_Visible(This,pVal)	\
    ( (This)->lpVtbl -> get_Visible(This,pVal) ) 

#define IDMGraphAnnotation_put_Visible(This,newVal)	\
    ( (This)->lpVtbl -> put_Visible(This,newVal) ) 

#define IDMGraphAnnotation_get_LabelBkColor(This,pVal)	\
    ( (This)->lpVtbl -> get_LabelBkColor(This,pVal) ) 

#define IDMGraphAnnotation_put_LabelBkColor(This,newVal)	\
    ( (This)->lpVtbl -> put_LabelBkColor(This,newVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDMGraphAnnotation_INTERFACE_DEFINED__ */


#ifndef __IDMGraphCursor_INTERFACE_DEFINED__
#define __IDMGraphCursor_INTERFACE_DEFINED__

/* interface IDMGraphCursor */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IDMGraphCursor;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("fb9b14b4-6acc-4bda-94a4-b75a3c349623")
    IDMGraphCursor : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Color( 
            /* [retval][out] */ OLE_COLOR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Color( 
            /* [in] */ OLE_COLOR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_X( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_X( 
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Y( 
            /* [retval][out] */ double *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Y( 
            /* [in] */ double newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Style( 
            /* [retval][out] */ Crosshair *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Style( 
            /* [in] */ Crosshair newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Visible( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Visible( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Mode( 
            /* [retval][out] */ CursorMode *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Mode( 
            /* [in] */ CursorMode newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Name( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CursorID( 
            /* [retval][out] */ SHORT *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CursorID( 
            /* [in] */ SHORT newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDMGraphCursorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDMGraphCursor * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDMGraphCursor * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDMGraphCursor * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDMGraphCursor * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDMGraphCursor * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDMGraphCursor * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDMGraphCursor * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Color )( 
            IDMGraphCursor * This,
            /* [retval][out] */ OLE_COLOR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Color )( 
            IDMGraphCursor * This,
            /* [in] */ OLE_COLOR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_X )( 
            IDMGraphCursor * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_X )( 
            IDMGraphCursor * This,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Y )( 
            IDMGraphCursor * This,
            /* [retval][out] */ double *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Y )( 
            IDMGraphCursor * This,
            /* [in] */ double newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Style )( 
            IDMGraphCursor * This,
            /* [retval][out] */ Crosshair *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Style )( 
            IDMGraphCursor * This,
            /* [in] */ Crosshair newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IDMGraphCursor * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IDMGraphCursor * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Mode )( 
            IDMGraphCursor * This,
            /* [retval][out] */ CursorMode *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Mode )( 
            IDMGraphCursor * This,
            /* [in] */ CursorMode newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IDMGraphCursor * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Name )( 
            IDMGraphCursor * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CursorID )( 
            IDMGraphCursor * This,
            /* [retval][out] */ SHORT *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CursorID )( 
            IDMGraphCursor * This,
            /* [in] */ SHORT newVal);
        
        END_INTERFACE
    } IDMGraphCursorVtbl;

    interface IDMGraphCursor
    {
        CONST_VTBL struct IDMGraphCursorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDMGraphCursor_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDMGraphCursor_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDMGraphCursor_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDMGraphCursor_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDMGraphCursor_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDMGraphCursor_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDMGraphCursor_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDMGraphCursor_get_Color(This,pVal)	\
    ( (This)->lpVtbl -> get_Color(This,pVal) ) 

#define IDMGraphCursor_put_Color(This,newVal)	\
    ( (This)->lpVtbl -> put_Color(This,newVal) ) 

#define IDMGraphCursor_get_X(This,pVal)	\
    ( (This)->lpVtbl -> get_X(This,pVal) ) 

#define IDMGraphCursor_put_X(This,newVal)	\
    ( (This)->lpVtbl -> put_X(This,newVal) ) 

#define IDMGraphCursor_get_Y(This,pVal)	\
    ( (This)->lpVtbl -> get_Y(This,pVal) ) 

#define IDMGraphCursor_put_Y(This,newVal)	\
    ( (This)->lpVtbl -> put_Y(This,newVal) ) 

#define IDMGraphCursor_get_Style(This,pVal)	\
    ( (This)->lpVtbl -> get_Style(This,pVal) ) 

#define IDMGraphCursor_put_Style(This,newVal)	\
    ( (This)->lpVtbl -> put_Style(This,newVal) ) 

#define IDMGraphCursor_get_Visible(This,pVal)	\
    ( (This)->lpVtbl -> get_Visible(This,pVal) ) 

#define IDMGraphCursor_put_Visible(This,newVal)	\
    ( (This)->lpVtbl -> put_Visible(This,newVal) ) 

#define IDMGraphCursor_get_Mode(This,pVal)	\
    ( (This)->lpVtbl -> get_Mode(This,pVal) ) 

#define IDMGraphCursor_put_Mode(This,newVal)	\
    ( (This)->lpVtbl -> put_Mode(This,newVal) ) 

#define IDMGraphCursor_get_Name(This,pVal)	\
    ( (This)->lpVtbl -> get_Name(This,pVal) ) 

#define IDMGraphCursor_put_Name(This,newVal)	\
    ( (This)->lpVtbl -> put_Name(This,newVal) ) 

#define IDMGraphCursor_get_CursorID(This,pVal)	\
    ( (This)->lpVtbl -> get_CursorID(This,pVal) ) 

#define IDMGraphCursor_put_CursorID(This,newVal)	\
    ( (This)->lpVtbl -> put_CursorID(This,newVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDMGraphCursor_INTERFACE_DEFINED__ */


#ifndef __IDMGraphAxis_INTERFACE_DEFINED__
#define __IDMGraphAxis_INTERFACE_DEFINED__

/* interface IDMGraphAxis */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IDMGraphAxis;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("c735264e-70bb-48bb-ac9c-93188a12c254")
    IDMGraphAxis : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_GridNumber( 
            /* [retval][out] */ short *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_GridNumber( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Label( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Label( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Log( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Log( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Time( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Time( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Format( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Format( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ShowGrid( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ShowGrid( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDMGraphAxisVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDMGraphAxis * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDMGraphAxis * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDMGraphAxis * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDMGraphAxis * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDMGraphAxis * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDMGraphAxis * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDMGraphAxis * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GridNumber )( 
            IDMGraphAxis * This,
            /* [retval][out] */ short *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_GridNumber )( 
            IDMGraphAxis * This,
            /* [in] */ short newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Label )( 
            IDMGraphAxis * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Label )( 
            IDMGraphAxis * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Log )( 
            IDMGraphAxis * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Log )( 
            IDMGraphAxis * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Time )( 
            IDMGraphAxis * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Time )( 
            IDMGraphAxis * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Format )( 
            IDMGraphAxis * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Format )( 
            IDMGraphAxis * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ShowGrid )( 
            IDMGraphAxis * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ShowGrid )( 
            IDMGraphAxis * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        END_INTERFACE
    } IDMGraphAxisVtbl;

    interface IDMGraphAxis
    {
        CONST_VTBL struct IDMGraphAxisVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDMGraphAxis_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDMGraphAxis_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDMGraphAxis_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDMGraphAxis_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDMGraphAxis_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDMGraphAxis_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDMGraphAxis_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDMGraphAxis_get_GridNumber(This,pVal)	\
    ( (This)->lpVtbl -> get_GridNumber(This,pVal) ) 

#define IDMGraphAxis_put_GridNumber(This,newVal)	\
    ( (This)->lpVtbl -> put_GridNumber(This,newVal) ) 

#define IDMGraphAxis_get_Label(This,pVal)	\
    ( (This)->lpVtbl -> get_Label(This,pVal) ) 

#define IDMGraphAxis_put_Label(This,newVal)	\
    ( (This)->lpVtbl -> put_Label(This,newVal) ) 

#define IDMGraphAxis_get_Log(This,pVal)	\
    ( (This)->lpVtbl -> get_Log(This,pVal) ) 

#define IDMGraphAxis_put_Log(This,newVal)	\
    ( (This)->lpVtbl -> put_Log(This,newVal) ) 

#define IDMGraphAxis_get_Time(This,pVal)	\
    ( (This)->lpVtbl -> get_Time(This,pVal) ) 

#define IDMGraphAxis_put_Time(This,newVal)	\
    ( (This)->lpVtbl -> put_Time(This,newVal) ) 

#define IDMGraphAxis_get_Format(This,pVal)	\
    ( (This)->lpVtbl -> get_Format(This,pVal) ) 

#define IDMGraphAxis_put_Format(This,newVal)	\
    ( (This)->lpVtbl -> put_Format(This,newVal) ) 

#define IDMGraphAxis_get_ShowGrid(This,pVal)	\
    ( (This)->lpVtbl -> get_ShowGrid(This,pVal) ) 

#define IDMGraphAxis_put_ShowGrid(This,newVal)	\
    ( (This)->lpVtbl -> put_ShowGrid(This,newVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDMGraphAxis_INTERFACE_DEFINED__ */


#ifndef __IDMGraphCollection_INTERFACE_DEFINED__
#define __IDMGraphCollection_INTERFACE_DEFINED__

/* interface IDMGraphCollection */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IDMGraphCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("022e739f-00bb-40be-bbba-8492e4443493")
    IDMGraphCollection : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ long index,
            /* [retval][out] */ IDispatch **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Selected( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Selected( 
            /* [in] */ long index) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [retval][out] */ IDispatch **pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Delete( 
            /* [in] */ long index) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDMGraphCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDMGraphCollection * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDMGraphCollection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDMGraphCollection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDMGraphCollection * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDMGraphCollection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDMGraphCollection * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDMGraphCollection * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IDMGraphCollection * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IDMGraphCollection * This,
            /* [in] */ long index,
            /* [retval][out] */ IDispatch **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Selected )( 
            IDMGraphCollection * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Selected )( 
            IDMGraphCollection * This,
            /* [in] */ long index);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            IDMGraphCollection * This,
            /* [retval][out] */ IDispatch **pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Delete )( 
            IDMGraphCollection * This,
            /* [in] */ long index);
        
        END_INTERFACE
    } IDMGraphCollectionVtbl;

    interface IDMGraphCollection
    {
        CONST_VTBL struct IDMGraphCollectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDMGraphCollection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDMGraphCollection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDMGraphCollection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDMGraphCollection_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDMGraphCollection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDMGraphCollection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDMGraphCollection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDMGraphCollection_get_Count(This,pVal)	\
    ( (This)->lpVtbl -> get_Count(This,pVal) ) 

#define IDMGraphCollection_get_Item(This,index,pVal)	\
    ( (This)->lpVtbl -> get_Item(This,index,pVal) ) 

#define IDMGraphCollection_get_Selected(This,pVal)	\
    ( (This)->lpVtbl -> get_Selected(This,pVal) ) 

#define IDMGraphCollection_put_Selected(This,index)	\
    ( (This)->lpVtbl -> put_Selected(This,index) ) 

#define IDMGraphCollection_Add(This,pVal)	\
    ( (This)->lpVtbl -> Add(This,pVal) ) 

#define IDMGraphCollection_Delete(This,index)	\
    ( (This)->lpVtbl -> Delete(This,index) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDMGraphCollection_INTERFACE_DEFINED__ */


#ifndef __IDMGraphCtrl_INTERFACE_DEFINED__
#define __IDMGraphCtrl_INTERFACE_DEFINED__

/* interface IDMGraphCtrl */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IDMGraphCtrl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("2DC9C043-D85A-45B3-8867-9E3D035DEC7F")
    IDMGraphCtrl : public IDispatch
    {
    public:
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Enabled( 
            /* [in] */ VARIANT_BOOL vbool) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Enabled( 
            /* [retval][out] */ VARIANT_BOOL *pbool) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Caption( 
            /* [in] */ BSTR strCaption) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Caption( 
            /* [retval][out] */ BSTR *pstrCaption) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Appearance( 
            /* [in] */ short appearance) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Appearance( 
            /* [retval][out] */ short *pappearance) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AxisColor( 
            /* [retval][out] */ OLE_COLOR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_AxisColor( 
            /* [in] */ OLE_COLOR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_GridColor( 
            /* [retval][out] */ OLE_COLOR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_GridColor( 
            /* [in] */ OLE_COLOR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LabelColor( 
            /* [retval][out] */ OLE_COLOR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_LabelColor( 
            /* [in] */ OLE_COLOR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_LabelFont( 
            /* [retval][out] */ IFontDisp **pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_LabelFont( 
            /* [in] */ IFontDisp *newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TickFont( 
            /* [retval][out] */ IFontDisp **pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TickFont( 
            /* [in] */ IFontDisp *newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TitleFont( 
            /* [retval][out] */ IFontDisp **pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TitleFont( 
            /* [in] */ IFontDisp *newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IdentFont( 
            /* [retval][out] */ IFontDisp **pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_IdentFont( 
            /* [in] */ IFontDisp *newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ElementIdentify( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ElementIdentify( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_TrackMode( 
            /* [retval][out] */ TrackModeState *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_TrackMode( 
            /* [in] */ TrackModeState newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ControlFramePicture( 
            /* [retval][out] */ IPictureDisp **pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ControlFramePicture( 
            /* [in] */ IPictureDisp *newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PlotAreaPicture( 
            /* [retval][out] */ IPictureDisp **pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_PlotAreaPicture( 
            /* [in] */ IPictureDisp *newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ControlFrameColor( 
            /* [retval][out] */ OLE_COLOR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_ControlFrameColor( 
            /* [in] */ OLE_COLOR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PlotAreaColor( 
            /* [retval][out] */ OLE_COLOR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_PlotAreaColor( 
            /* [in] */ OLE_COLOR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FrameStyle( 
            /* [retval][out] */ FrameStyle *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_FrameStyle( 
            /* [in] */ FrameStyle newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Annotations( 
            /* [retval][out] */ IDMGraphCollection **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Elements( 
            /* [retval][out] */ IDMGraphCollection **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Cursors( 
            /* [retval][out] */ IDMGraphCollection **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Axis( 
            /* [in] */ Orientation eOrientation,
            /* [retval][out] */ IDMGraphAxis **pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SetRange( 
            /* [in] */ double xmin,
            /* [in] */ double xmax,
            /* [in] */ double ymin,
            /* [in] */ double ymax) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetRange( 
            /* [out] */ double *xmin,
            /* [out] */ double *xmax,
            /* [out] */ double *ymin,
            /* [out] */ double *ymax) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AutoRange( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetAutoRange( 
            /* [out] */ double *xmin,
            /* [out] */ double *xmax,
            /* [out] */ double *ymin,
            /* [out] */ double *ymax) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CopyToClipboard( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PrintGraph( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ClearGraph( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ShowProperties( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveAs( 
            /* [in] */ BSTR szFilename) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AboutBox( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ShiftDisplay( 
            SHORT xShift,
            SHORT yShift) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GenerateCSVReport( 
            LPCTSTR pFilename) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE GetElementValueAtCursor( 
            /* [in] */ SHORT shElementID,
            /* [in] */ DOUBLE dblCursorVal,
            /* [out] */ DOUBLE *dblElemVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDMGraphCtrlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDMGraphCtrl * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDMGraphCtrl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDMGraphCtrl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDMGraphCtrl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDMGraphCtrl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDMGraphCtrl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDMGraphCtrl * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            IDMGraphCtrl * This,
            /* [in] */ VARIANT_BOOL vbool);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IDMGraphCtrl * This,
            /* [retval][out] */ VARIANT_BOOL *pbool);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Caption )( 
            IDMGraphCtrl * This,
            /* [in] */ BSTR strCaption);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Caption )( 
            IDMGraphCtrl * This,
            /* [retval][out] */ BSTR *pstrCaption);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Appearance )( 
            IDMGraphCtrl * This,
            /* [in] */ short appearance);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Appearance )( 
            IDMGraphCtrl * This,
            /* [retval][out] */ short *pappearance);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AxisColor )( 
            IDMGraphCtrl * This,
            /* [retval][out] */ OLE_COLOR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_AxisColor )( 
            IDMGraphCtrl * This,
            /* [in] */ OLE_COLOR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_GridColor )( 
            IDMGraphCtrl * This,
            /* [retval][out] */ OLE_COLOR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_GridColor )( 
            IDMGraphCtrl * This,
            /* [in] */ OLE_COLOR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LabelColor )( 
            IDMGraphCtrl * This,
            /* [retval][out] */ OLE_COLOR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_LabelColor )( 
            IDMGraphCtrl * This,
            /* [in] */ OLE_COLOR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_LabelFont )( 
            IDMGraphCtrl * This,
            /* [retval][out] */ IFontDisp **pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_LabelFont )( 
            IDMGraphCtrl * This,
            /* [in] */ IFontDisp *newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TickFont )( 
            IDMGraphCtrl * This,
            /* [retval][out] */ IFontDisp **pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TickFont )( 
            IDMGraphCtrl * This,
            /* [in] */ IFontDisp *newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TitleFont )( 
            IDMGraphCtrl * This,
            /* [retval][out] */ IFontDisp **pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TitleFont )( 
            IDMGraphCtrl * This,
            /* [in] */ IFontDisp *newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IdentFont )( 
            IDMGraphCtrl * This,
            /* [retval][out] */ IFontDisp **pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_IdentFont )( 
            IDMGraphCtrl * This,
            /* [in] */ IFontDisp *newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ElementIdentify )( 
            IDMGraphCtrl * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ElementIdentify )( 
            IDMGraphCtrl * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_TrackMode )( 
            IDMGraphCtrl * This,
            /* [retval][out] */ TrackModeState *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_TrackMode )( 
            IDMGraphCtrl * This,
            /* [in] */ TrackModeState newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ControlFramePicture )( 
            IDMGraphCtrl * This,
            /* [retval][out] */ IPictureDisp **pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ControlFramePicture )( 
            IDMGraphCtrl * This,
            /* [in] */ IPictureDisp *newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PlotAreaPicture )( 
            IDMGraphCtrl * This,
            /* [retval][out] */ IPictureDisp **pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_PlotAreaPicture )( 
            IDMGraphCtrl * This,
            /* [in] */ IPictureDisp *newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ControlFrameColor )( 
            IDMGraphCtrl * This,
            /* [retval][out] */ OLE_COLOR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_ControlFrameColor )( 
            IDMGraphCtrl * This,
            /* [in] */ OLE_COLOR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PlotAreaColor )( 
            IDMGraphCtrl * This,
            /* [retval][out] */ OLE_COLOR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_PlotAreaColor )( 
            IDMGraphCtrl * This,
            /* [in] */ OLE_COLOR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FrameStyle )( 
            IDMGraphCtrl * This,
            /* [retval][out] */ FrameStyle *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_FrameStyle )( 
            IDMGraphCtrl * This,
            /* [in] */ FrameStyle newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Annotations )( 
            IDMGraphCtrl * This,
            /* [retval][out] */ IDMGraphCollection **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Elements )( 
            IDMGraphCtrl * This,
            /* [retval][out] */ IDMGraphCollection **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Cursors )( 
            IDMGraphCtrl * This,
            /* [retval][out] */ IDMGraphCollection **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Axis )( 
            IDMGraphCtrl * This,
            /* [in] */ Orientation eOrientation,
            /* [retval][out] */ IDMGraphAxis **pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SetRange )( 
            IDMGraphCtrl * This,
            /* [in] */ double xmin,
            /* [in] */ double xmax,
            /* [in] */ double ymin,
            /* [in] */ double ymax);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetRange )( 
            IDMGraphCtrl * This,
            /* [out] */ double *xmin,
            /* [out] */ double *xmax,
            /* [out] */ double *ymin,
            /* [out] */ double *ymax);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AutoRange )( 
            IDMGraphCtrl * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetAutoRange )( 
            IDMGraphCtrl * This,
            /* [out] */ double *xmin,
            /* [out] */ double *xmax,
            /* [out] */ double *ymin,
            /* [out] */ double *ymax);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CopyToClipboard )( 
            IDMGraphCtrl * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PrintGraph )( 
            IDMGraphCtrl * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ClearGraph )( 
            IDMGraphCtrl * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ShowProperties )( 
            IDMGraphCtrl * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SaveAs )( 
            IDMGraphCtrl * This,
            /* [in] */ BSTR szFilename);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *AboutBox )( 
            IDMGraphCtrl * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ShiftDisplay )( 
            IDMGraphCtrl * This,
            SHORT xShift,
            SHORT yShift);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GenerateCSVReport )( 
            IDMGraphCtrl * This,
            LPCTSTR pFilename);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *GetElementValueAtCursor )( 
            IDMGraphCtrl * This,
            /* [in] */ SHORT shElementID,
            /* [in] */ DOUBLE dblCursorVal,
            /* [out] */ DOUBLE *dblElemVal);
        
        END_INTERFACE
    } IDMGraphCtrlVtbl;

    interface IDMGraphCtrl
    {
        CONST_VTBL struct IDMGraphCtrlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDMGraphCtrl_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDMGraphCtrl_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDMGraphCtrl_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDMGraphCtrl_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDMGraphCtrl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDMGraphCtrl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDMGraphCtrl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IDMGraphCtrl_put_Enabled(This,vbool)	\
    ( (This)->lpVtbl -> put_Enabled(This,vbool) ) 

#define IDMGraphCtrl_get_Enabled(This,pbool)	\
    ( (This)->lpVtbl -> get_Enabled(This,pbool) ) 

#define IDMGraphCtrl_put_Caption(This,strCaption)	\
    ( (This)->lpVtbl -> put_Caption(This,strCaption) ) 

#define IDMGraphCtrl_get_Caption(This,pstrCaption)	\
    ( (This)->lpVtbl -> get_Caption(This,pstrCaption) ) 

#define IDMGraphCtrl_put_Appearance(This,appearance)	\
    ( (This)->lpVtbl -> put_Appearance(This,appearance) ) 

#define IDMGraphCtrl_get_Appearance(This,pappearance)	\
    ( (This)->lpVtbl -> get_Appearance(This,pappearance) ) 

#define IDMGraphCtrl_get_AxisColor(This,pVal)	\
    ( (This)->lpVtbl -> get_AxisColor(This,pVal) ) 

#define IDMGraphCtrl_put_AxisColor(This,newVal)	\
    ( (This)->lpVtbl -> put_AxisColor(This,newVal) ) 

#define IDMGraphCtrl_get_GridColor(This,pVal)	\
    ( (This)->lpVtbl -> get_GridColor(This,pVal) ) 

#define IDMGraphCtrl_put_GridColor(This,newVal)	\
    ( (This)->lpVtbl -> put_GridColor(This,newVal) ) 

#define IDMGraphCtrl_get_LabelColor(This,pVal)	\
    ( (This)->lpVtbl -> get_LabelColor(This,pVal) ) 

#define IDMGraphCtrl_put_LabelColor(This,newVal)	\
    ( (This)->lpVtbl -> put_LabelColor(This,newVal) ) 

#define IDMGraphCtrl_get_LabelFont(This,pVal)	\
    ( (This)->lpVtbl -> get_LabelFont(This,pVal) ) 

#define IDMGraphCtrl_put_LabelFont(This,newVal)	\
    ( (This)->lpVtbl -> put_LabelFont(This,newVal) ) 

#define IDMGraphCtrl_get_TickFont(This,pVal)	\
    ( (This)->lpVtbl -> get_TickFont(This,pVal) ) 

#define IDMGraphCtrl_put_TickFont(This,newVal)	\
    ( (This)->lpVtbl -> put_TickFont(This,newVal) ) 

#define IDMGraphCtrl_get_TitleFont(This,pVal)	\
    ( (This)->lpVtbl -> get_TitleFont(This,pVal) ) 

#define IDMGraphCtrl_put_TitleFont(This,newVal)	\
    ( (This)->lpVtbl -> put_TitleFont(This,newVal) ) 

#define IDMGraphCtrl_get_IdentFont(This,pVal)	\
    ( (This)->lpVtbl -> get_IdentFont(This,pVal) ) 

#define IDMGraphCtrl_put_IdentFont(This,newVal)	\
    ( (This)->lpVtbl -> put_IdentFont(This,newVal) ) 

#define IDMGraphCtrl_get_ElementIdentify(This,pVal)	\
    ( (This)->lpVtbl -> get_ElementIdentify(This,pVal) ) 

#define IDMGraphCtrl_put_ElementIdentify(This,newVal)	\
    ( (This)->lpVtbl -> put_ElementIdentify(This,newVal) ) 

#define IDMGraphCtrl_get_TrackMode(This,pVal)	\
    ( (This)->lpVtbl -> get_TrackMode(This,pVal) ) 

#define IDMGraphCtrl_put_TrackMode(This,newVal)	\
    ( (This)->lpVtbl -> put_TrackMode(This,newVal) ) 

#define IDMGraphCtrl_get_ControlFramePicture(This,pVal)	\
    ( (This)->lpVtbl -> get_ControlFramePicture(This,pVal) ) 

#define IDMGraphCtrl_put_ControlFramePicture(This,newVal)	\
    ( (This)->lpVtbl -> put_ControlFramePicture(This,newVal) ) 

#define IDMGraphCtrl_get_PlotAreaPicture(This,pVal)	\
    ( (This)->lpVtbl -> get_PlotAreaPicture(This,pVal) ) 

#define IDMGraphCtrl_put_PlotAreaPicture(This,newVal)	\
    ( (This)->lpVtbl -> put_PlotAreaPicture(This,newVal) ) 

#define IDMGraphCtrl_get_ControlFrameColor(This,pVal)	\
    ( (This)->lpVtbl -> get_ControlFrameColor(This,pVal) ) 

#define IDMGraphCtrl_put_ControlFrameColor(This,newVal)	\
    ( (This)->lpVtbl -> put_ControlFrameColor(This,newVal) ) 

#define IDMGraphCtrl_get_PlotAreaColor(This,pVal)	\
    ( (This)->lpVtbl -> get_PlotAreaColor(This,pVal) ) 

#define IDMGraphCtrl_put_PlotAreaColor(This,newVal)	\
    ( (This)->lpVtbl -> put_PlotAreaColor(This,newVal) ) 

#define IDMGraphCtrl_get_FrameStyle(This,pVal)	\
    ( (This)->lpVtbl -> get_FrameStyle(This,pVal) ) 

#define IDMGraphCtrl_put_FrameStyle(This,newVal)	\
    ( (This)->lpVtbl -> put_FrameStyle(This,newVal) ) 

#define IDMGraphCtrl_get_Annotations(This,pVal)	\
    ( (This)->lpVtbl -> get_Annotations(This,pVal) ) 

#define IDMGraphCtrl_get_Elements(This,pVal)	\
    ( (This)->lpVtbl -> get_Elements(This,pVal) ) 

#define IDMGraphCtrl_get_Cursors(This,pVal)	\
    ( (This)->lpVtbl -> get_Cursors(This,pVal) ) 

#define IDMGraphCtrl_get_Axis(This,eOrientation,pVal)	\
    ( (This)->lpVtbl -> get_Axis(This,eOrientation,pVal) ) 

#define IDMGraphCtrl_SetRange(This,xmin,xmax,ymin,ymax)	\
    ( (This)->lpVtbl -> SetRange(This,xmin,xmax,ymin,ymax) ) 

#define IDMGraphCtrl_GetRange(This,xmin,xmax,ymin,ymax)	\
    ( (This)->lpVtbl -> GetRange(This,xmin,xmax,ymin,ymax) ) 

#define IDMGraphCtrl_AutoRange(This)	\
    ( (This)->lpVtbl -> AutoRange(This) ) 

#define IDMGraphCtrl_GetAutoRange(This,xmin,xmax,ymin,ymax)	\
    ( (This)->lpVtbl -> GetAutoRange(This,xmin,xmax,ymin,ymax) ) 

#define IDMGraphCtrl_CopyToClipboard(This)	\
    ( (This)->lpVtbl -> CopyToClipboard(This) ) 

#define IDMGraphCtrl_PrintGraph(This)	\
    ( (This)->lpVtbl -> PrintGraph(This) ) 

#define IDMGraphCtrl_ClearGraph(This)	\
    ( (This)->lpVtbl -> ClearGraph(This) ) 

#define IDMGraphCtrl_ShowProperties(This)	\
    ( (This)->lpVtbl -> ShowProperties(This) ) 

#define IDMGraphCtrl_SaveAs(This,szFilename)	\
    ( (This)->lpVtbl -> SaveAs(This,szFilename) ) 

#define IDMGraphCtrl_AboutBox(This)	\
    ( (This)->lpVtbl -> AboutBox(This) ) 

#define IDMGraphCtrl_ShiftDisplay(This,xShift,yShift)	\
    ( (This)->lpVtbl -> ShiftDisplay(This,xShift,yShift) ) 

#define IDMGraphCtrl_GenerateCSVReport(This,pFilename)	\
    ( (This)->lpVtbl -> GenerateCSVReport(This,pFilename) ) 

#define IDMGraphCtrl_GetElementValueAtCursor(This,shElementID,dblCursorVal,dblElemVal)	\
    ( (This)->lpVtbl -> GetElementValueAtCursor(This,shElementID,dblCursorVal,dblElemVal) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDMGraphCtrl_INTERFACE_DEFINED__ */



#ifndef __DMGraphLib_LIBRARY_DEFINED__
#define __DMGraphLib_LIBRARY_DEFINED__

/* library DMGraphLib */
/* [helpstring][helpfile][version][uuid] */ 







EXTERN_C const IID LIBID_DMGraphLib;

#ifndef ___IDMGraphCtrlEvents_DISPINTERFACE_DEFINED__
#define ___IDMGraphCtrlEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IDMGraphCtrlEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IDMGraphCtrlEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("9392E24C-430F-467D-99A2-69FA98CBCB78")
    _IDMGraphCtrlEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IDMGraphCtrlEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IDMGraphCtrlEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IDMGraphCtrlEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IDMGraphCtrlEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IDMGraphCtrlEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IDMGraphCtrlEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IDMGraphCtrlEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IDMGraphCtrlEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IDMGraphCtrlEventsVtbl;

    interface _IDMGraphCtrlEvents
    {
        CONST_VTBL struct _IDMGraphCtrlEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IDMGraphCtrlEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IDMGraphCtrlEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IDMGraphCtrlEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IDMGraphCtrlEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IDMGraphCtrlEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IDMGraphCtrlEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IDMGraphCtrlEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IDMGraphCtrlEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_DMGraphCtrl;

#ifdef __cplusplus

class DECLSPEC_UUID("AAF89A51-7FC0-43B0-9F81-FFEFF6A8DB43")
DMGraphCtrl;
#endif

EXTERN_C const CLSID CLSID_ElementPropPage;

#ifdef __cplusplus

class DECLSPEC_UUID("14A14BAD-2949-4186-9529-BBB2CC7F9699")
ElementPropPage;
#endif

EXTERN_C const CLSID CLSID_AnnoPropPage;

#ifdef __cplusplus

class DECLSPEC_UUID("273DB50A-E76D-4B95-8E1A-0993E14FFEF0")
AnnoPropPage;
#endif

EXTERN_C const CLSID CLSID_CursorPropPage;

#ifdef __cplusplus

class DECLSPEC_UUID("5DF8A597-C334-4ED5-8D45-8884D506044D")
CursorPropPage;
#endif

EXTERN_C const CLSID CLSID_FormatPropPage;

#ifdef __cplusplus

class DECLSPEC_UUID("9EE3E072-2B3F-47AB-8612-B6ED7F0CDAE6")
FormatPropPage;
#endif

EXTERN_C const CLSID CLSID_DMGraphPropPage;

#ifdef __cplusplus

class DECLSPEC_UUID("cb32b59d-7aaa-44b5-acdf-a64b9380e48c")
DMGraphPropPage;
#endif
#endif /* __DMGraphLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


