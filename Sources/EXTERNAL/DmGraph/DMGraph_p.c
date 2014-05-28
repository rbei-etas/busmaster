

/* this ALWAYS GENERATED file contains the proxy stub code */


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

#if !defined(_M_IA64) && !defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */
#pragma warning( disable: 4100 ) /* unreferenced arguments in x86 call */

#pragma optimize("", off ) 

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif /* __RPCPROXY_H_VERSION__ */


#include "DMGraph.h"

#define TYPE_FORMAT_STRING_SIZE   1175                              
#define PROC_FORMAT_STRING_SIZE   3757                              
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   2            

typedef struct _DMGraph_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } DMGraph_MIDL_TYPE_FORMAT_STRING;

typedef struct _DMGraph_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } DMGraph_MIDL_PROC_FORMAT_STRING;

typedef struct _DMGraph_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } DMGraph_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const DMGraph_MIDL_TYPE_FORMAT_STRING DMGraph__MIDL_TypeFormatString;
extern const DMGraph_MIDL_PROC_FORMAT_STRING DMGraph__MIDL_ProcFormatString;
extern const DMGraph_MIDL_EXPR_FORMAT_STRING DMGraph__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IDMGraphElement_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IDMGraphElement_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IDMGraphAnnotation_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IDMGraphAnnotation_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IDMGraphCursor_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IDMGraphCursor_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IDMGraphAxis_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IDMGraphAxis_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IDMGraphCollection_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IDMGraphCollection_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO IDMGraphCtrl_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IDMGraphCtrl_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

#if !(TARGET_IS_NT50_OR_LATER)
#error You need Windows 2000 or later to run this stub because it uses these features:
#error   /robust command line switch.
#error However, your C/C++ compilation flags indicate you intend to run this app on earlier systems.
#error This app will fail with the RPC_X_WRONG_STUB_VERSION error.
#endif


static const DMGraph_MIDL_PROC_FORMAT_STRING DMGraph__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure get_Count */


	/* Procedure get_Color */


	/* Procedure get_LineColor */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
/*  8 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 10 */	NdrFcShort( 0x0 ),	/* 0 */
/* 12 */	NdrFcShort( 0x24 ),	/* 36 */
/* 14 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 16 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x0 ),	/* 0 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */


	/* Parameter pVal */


	/* Parameter pVal */

/* 24 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 26 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 28 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */


	/* Return value */

/* 30 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 32 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 34 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Color */


	/* Procedure put_LineColor */

/* 36 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 38 */	NdrFcLong( 0x0 ),	/* 0 */
/* 42 */	NdrFcShort( 0x8 ),	/* 8 */
/* 44 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 46 */	NdrFcShort( 0x8 ),	/* 8 */
/* 48 */	NdrFcShort( 0x8 ),	/* 8 */
/* 50 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 52 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 54 */	NdrFcShort( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0x0 ),	/* 0 */
/* 58 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */


	/* Parameter newVal */

/* 60 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 62 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 64 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 66 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 68 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 70 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Selected */


	/* Procedure get_PointColor */

/* 72 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 74 */	NdrFcLong( 0x0 ),	/* 0 */
/* 78 */	NdrFcShort( 0x9 ),	/* 9 */
/* 80 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 82 */	NdrFcShort( 0x0 ),	/* 0 */
/* 84 */	NdrFcShort( 0x24 ),	/* 36 */
/* 86 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 88 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 90 */	NdrFcShort( 0x0 ),	/* 0 */
/* 92 */	NdrFcShort( 0x0 ),	/* 0 */
/* 94 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */


	/* Parameter pVal */

/* 96 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 98 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 100 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 102 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 104 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 106 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Selected */


	/* Procedure put_PointColor */

/* 108 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 110 */	NdrFcLong( 0x0 ),	/* 0 */
/* 114 */	NdrFcShort( 0xa ),	/* 10 */
/* 116 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 118 */	NdrFcShort( 0x8 ),	/* 8 */
/* 120 */	NdrFcShort( 0x8 ),	/* 8 */
/* 122 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 124 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 126 */	NdrFcShort( 0x0 ),	/* 0 */
/* 128 */	NdrFcShort( 0x0 ),	/* 0 */
/* 130 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */


	/* Parameter newVal */

/* 132 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 134 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 136 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 138 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 140 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 142 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Linetype */

/* 144 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 146 */	NdrFcLong( 0x0 ),	/* 0 */
/* 150 */	NdrFcShort( 0xb ),	/* 11 */
/* 152 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 154 */	NdrFcShort( 0x0 ),	/* 0 */
/* 156 */	NdrFcShort( 0x22 ),	/* 34 */
/* 158 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 160 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 162 */	NdrFcShort( 0x0 ),	/* 0 */
/* 164 */	NdrFcShort( 0x0 ),	/* 0 */
/* 166 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 168 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 170 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 172 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Return value */

/* 174 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 176 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 178 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Linetype */

/* 180 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 182 */	NdrFcLong( 0x0 ),	/* 0 */
/* 186 */	NdrFcShort( 0xc ),	/* 12 */
/* 188 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 190 */	NdrFcShort( 0x6 ),	/* 6 */
/* 192 */	NdrFcShort( 0x8 ),	/* 8 */
/* 194 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 196 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 198 */	NdrFcShort( 0x0 ),	/* 0 */
/* 200 */	NdrFcShort( 0x0 ),	/* 0 */
/* 202 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 204 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 206 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 208 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Return value */

/* 210 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 212 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 214 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_AxisColor */


	/* Procedure get_LabelColor */


	/* Procedure get_LineWidth */

/* 216 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 218 */	NdrFcLong( 0x0 ),	/* 0 */
/* 222 */	NdrFcShort( 0xd ),	/* 13 */
/* 224 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 226 */	NdrFcShort( 0x0 ),	/* 0 */
/* 228 */	NdrFcShort( 0x24 ),	/* 36 */
/* 230 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 232 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 234 */	NdrFcShort( 0x0 ),	/* 0 */
/* 236 */	NdrFcShort( 0x0 ),	/* 0 */
/* 238 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */


	/* Parameter pVal */


	/* Parameter pVal */

/* 240 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 242 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 244 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */


	/* Return value */

/* 246 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 248 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 250 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_AxisColor */


	/* Procedure put_LabelColor */


	/* Procedure put_LineWidth */

/* 252 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 254 */	NdrFcLong( 0x0 ),	/* 0 */
/* 258 */	NdrFcShort( 0xe ),	/* 14 */
/* 260 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 262 */	NdrFcShort( 0x8 ),	/* 8 */
/* 264 */	NdrFcShort( 0x8 ),	/* 8 */
/* 266 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 268 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 270 */	NdrFcShort( 0x0 ),	/* 0 */
/* 272 */	NdrFcShort( 0x0 ),	/* 0 */
/* 274 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */


	/* Parameter newVal */


	/* Parameter newVal */

/* 276 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 278 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 280 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */


	/* Return value */

/* 282 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 284 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 286 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_PointSymbol */

/* 288 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 290 */	NdrFcLong( 0x0 ),	/* 0 */
/* 294 */	NdrFcShort( 0xf ),	/* 15 */
/* 296 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 298 */	NdrFcShort( 0x0 ),	/* 0 */
/* 300 */	NdrFcShort( 0x22 ),	/* 34 */
/* 302 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 304 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 306 */	NdrFcShort( 0x0 ),	/* 0 */
/* 308 */	NdrFcShort( 0x0 ),	/* 0 */
/* 310 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 312 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 314 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 316 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Return value */

/* 318 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 320 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 322 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_PointSymbol */

/* 324 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 326 */	NdrFcLong( 0x0 ),	/* 0 */
/* 330 */	NdrFcShort( 0x10 ),	/* 16 */
/* 332 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 334 */	NdrFcShort( 0x6 ),	/* 6 */
/* 336 */	NdrFcShort( 0x8 ),	/* 8 */
/* 338 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 340 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 342 */	NdrFcShort( 0x0 ),	/* 0 */
/* 344 */	NdrFcShort( 0x0 ),	/* 0 */
/* 346 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 348 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 350 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 352 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Return value */

/* 354 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 356 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 358 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_ShowGrid */


	/* Procedure get_Visible */


	/* Procedure get_SolidPoint */

/* 360 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 362 */	NdrFcLong( 0x0 ),	/* 0 */
/* 366 */	NdrFcShort( 0x11 ),	/* 17 */
/* 368 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 370 */	NdrFcShort( 0x0 ),	/* 0 */
/* 372 */	NdrFcShort( 0x22 ),	/* 34 */
/* 374 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 376 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 378 */	NdrFcShort( 0x0 ),	/* 0 */
/* 380 */	NdrFcShort( 0x0 ),	/* 0 */
/* 382 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */


	/* Parameter pVal */


	/* Parameter pVal */

/* 384 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 386 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 388 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */


	/* Return value */

/* 390 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 392 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 394 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_ShowGrid */


	/* Procedure put_Visible */


	/* Procedure put_SolidPoint */

/* 396 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 398 */	NdrFcLong( 0x0 ),	/* 0 */
/* 402 */	NdrFcShort( 0x12 ),	/* 18 */
/* 404 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 406 */	NdrFcShort( 0x6 ),	/* 6 */
/* 408 */	NdrFcShort( 0x8 ),	/* 8 */
/* 410 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 412 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 414 */	NdrFcShort( 0x0 ),	/* 0 */
/* 416 */	NdrFcShort( 0x0 ),	/* 0 */
/* 418 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */


	/* Parameter newVal */


	/* Parameter newVal */

/* 420 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 422 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 424 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */


	/* Return value */

/* 426 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 428 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 430 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_LabelBkColor */


	/* Procedure get_PointSize */

/* 432 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 434 */	NdrFcLong( 0x0 ),	/* 0 */
/* 438 */	NdrFcShort( 0x13 ),	/* 19 */
/* 440 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 442 */	NdrFcShort( 0x0 ),	/* 0 */
/* 444 */	NdrFcShort( 0x24 ),	/* 36 */
/* 446 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 448 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 450 */	NdrFcShort( 0x0 ),	/* 0 */
/* 452 */	NdrFcShort( 0x0 ),	/* 0 */
/* 454 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */


	/* Parameter pVal */

/* 456 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 458 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 460 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 462 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 464 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 466 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_LabelBkColor */


	/* Procedure put_PointSize */

/* 468 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 470 */	NdrFcLong( 0x0 ),	/* 0 */
/* 474 */	NdrFcShort( 0x14 ),	/* 20 */
/* 476 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 478 */	NdrFcShort( 0x8 ),	/* 8 */
/* 480 */	NdrFcShort( 0x8 ),	/* 8 */
/* 482 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 484 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 486 */	NdrFcShort( 0x0 ),	/* 0 */
/* 488 */	NdrFcShort( 0x0 ),	/* 0 */
/* 490 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */


	/* Parameter newVal */

/* 492 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 494 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 496 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 498 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 500 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 502 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_CursorID */


	/* Procedure get_Show */

/* 504 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 506 */	NdrFcLong( 0x0 ),	/* 0 */
/* 510 */	NdrFcShort( 0x15 ),	/* 21 */
/* 512 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 514 */	NdrFcShort( 0x0 ),	/* 0 */
/* 516 */	NdrFcShort( 0x22 ),	/* 34 */
/* 518 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 520 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 522 */	NdrFcShort( 0x0 ),	/* 0 */
/* 524 */	NdrFcShort( 0x0 ),	/* 0 */
/* 526 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */


	/* Parameter pVal */

/* 528 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 530 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 532 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 534 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 536 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 538 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_CursorID */


	/* Procedure put_Show */

/* 540 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 542 */	NdrFcLong( 0x0 ),	/* 0 */
/* 546 */	NdrFcShort( 0x16 ),	/* 22 */
/* 548 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 550 */	NdrFcShort( 0x6 ),	/* 6 */
/* 552 */	NdrFcShort( 0x8 ),	/* 8 */
/* 554 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 556 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 558 */	NdrFcShort( 0x0 ),	/* 0 */
/* 560 */	NdrFcShort( 0x0 ),	/* 0 */
/* 562 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */


	/* Parameter newVal */

/* 564 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 566 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 568 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 570 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 572 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 574 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Name */

/* 576 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 578 */	NdrFcLong( 0x0 ),	/* 0 */
/* 582 */	NdrFcShort( 0x17 ),	/* 23 */
/* 584 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 586 */	NdrFcShort( 0x0 ),	/* 0 */
/* 588 */	NdrFcShort( 0x8 ),	/* 8 */
/* 590 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 592 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 594 */	NdrFcShort( 0x1 ),	/* 1 */
/* 596 */	NdrFcShort( 0x0 ),	/* 0 */
/* 598 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 600 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 602 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 604 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Return value */

/* 606 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 608 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 610 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Name */

/* 612 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 614 */	NdrFcLong( 0x0 ),	/* 0 */
/* 618 */	NdrFcShort( 0x18 ),	/* 24 */
/* 620 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 622 */	NdrFcShort( 0x0 ),	/* 0 */
/* 624 */	NdrFcShort( 0x8 ),	/* 8 */
/* 626 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 628 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 630 */	NdrFcShort( 0x0 ),	/* 0 */
/* 632 */	NdrFcShort( 0x1 ),	/* 1 */
/* 634 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 636 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 638 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 640 */	NdrFcShort( 0x3a ),	/* Type Offset=58 */

	/* Return value */

/* 642 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 644 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 646 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Count */

/* 648 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 650 */	NdrFcLong( 0x0 ),	/* 0 */
/* 654 */	NdrFcShort( 0x19 ),	/* 25 */
/* 656 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 658 */	NdrFcShort( 0x0 ),	/* 0 */
/* 660 */	NdrFcShort( 0x24 ),	/* 36 */
/* 662 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 664 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 666 */	NdrFcShort( 0x0 ),	/* 0 */
/* 668 */	NdrFcShort( 0x0 ),	/* 0 */
/* 670 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 672 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 674 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 676 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 678 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 680 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 682 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_XValue */

/* 684 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 686 */	NdrFcLong( 0x0 ),	/* 0 */
/* 690 */	NdrFcShort( 0x1a ),	/* 26 */
/* 692 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 694 */	NdrFcShort( 0x8 ),	/* 8 */
/* 696 */	NdrFcShort( 0x2c ),	/* 44 */
/* 698 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 700 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 702 */	NdrFcShort( 0x0 ),	/* 0 */
/* 704 */	NdrFcShort( 0x0 ),	/* 0 */
/* 706 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 708 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 710 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 712 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pVal */

/* 714 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 716 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 718 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */

/* 720 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 722 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 724 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_XValue */

/* 726 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 728 */	NdrFcLong( 0x0 ),	/* 0 */
/* 732 */	NdrFcShort( 0x1b ),	/* 27 */
/* 734 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 736 */	NdrFcShort( 0x18 ),	/* 24 */
/* 738 */	NdrFcShort( 0x8 ),	/* 8 */
/* 740 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 742 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 744 */	NdrFcShort( 0x0 ),	/* 0 */
/* 746 */	NdrFcShort( 0x0 ),	/* 0 */
/* 748 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 750 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 752 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 754 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter newVal */

/* 756 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 758 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 760 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */

/* 762 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 764 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 766 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_YValue */

/* 768 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 770 */	NdrFcLong( 0x0 ),	/* 0 */
/* 774 */	NdrFcShort( 0x1c ),	/* 28 */
/* 776 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 778 */	NdrFcShort( 0x8 ),	/* 8 */
/* 780 */	NdrFcShort( 0x2c ),	/* 44 */
/* 782 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 784 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 786 */	NdrFcShort( 0x0 ),	/* 0 */
/* 788 */	NdrFcShort( 0x0 ),	/* 0 */
/* 790 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 792 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 794 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 796 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pVal */

/* 798 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 800 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 802 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */

/* 804 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 806 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 808 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_YValue */

/* 810 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 812 */	NdrFcLong( 0x0 ),	/* 0 */
/* 816 */	NdrFcShort( 0x1d ),	/* 29 */
/* 818 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 820 */	NdrFcShort( 0x18 ),	/* 24 */
/* 822 */	NdrFcShort( 0x8 ),	/* 8 */
/* 824 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 826 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 828 */	NdrFcShort( 0x0 ),	/* 0 */
/* 830 */	NdrFcShort( 0x0 ),	/* 0 */
/* 832 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 834 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 836 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 838 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter newVal */

/* 840 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 842 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 844 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */

/* 846 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 848 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 850 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PlotXY */

/* 852 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 854 */	NdrFcLong( 0x0 ),	/* 0 */
/* 858 */	NdrFcShort( 0x1e ),	/* 30 */
/* 860 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 862 */	NdrFcShort( 0x20 ),	/* 32 */
/* 864 */	NdrFcShort( 0x8 ),	/* 8 */
/* 866 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 868 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 870 */	NdrFcShort( 0x0 ),	/* 0 */
/* 872 */	NdrFcShort( 0x0 ),	/* 0 */
/* 874 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter X */

/* 876 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 878 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 880 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter Y */

/* 882 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 884 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 886 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */

/* 888 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 890 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 892 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PlotY */

/* 894 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 896 */	NdrFcLong( 0x0 ),	/* 0 */
/* 900 */	NdrFcShort( 0x1f ),	/* 31 */
/* 902 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 904 */	NdrFcShort( 0x10 ),	/* 16 */
/* 906 */	NdrFcShort( 0x8 ),	/* 8 */
/* 908 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 910 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 912 */	NdrFcShort( 0x0 ),	/* 0 */
/* 914 */	NdrFcShort( 0x0 ),	/* 0 */
/* 916 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter Y */

/* 918 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 920 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 922 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */

/* 924 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 926 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 928 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Plot */

/* 930 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 932 */	NdrFcLong( 0x0 ),	/* 0 */
/* 936 */	NdrFcShort( 0x20 ),	/* 32 */
/* 938 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 940 */	NdrFcShort( 0x0 ),	/* 0 */
/* 942 */	NdrFcShort( 0x8 ),	/* 8 */
/* 944 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 946 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 948 */	NdrFcShort( 0x0 ),	/* 0 */
/* 950 */	NdrFcShort( 0x1 ),	/* 1 */
/* 952 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newXVals */

/* 954 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 956 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 958 */	NdrFcShort( 0x42c ),	/* Type Offset=1068 */

	/* Parameter newYVals */

/* 960 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 962 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 964 */	NdrFcShort( 0x42c ),	/* Type Offset=1068 */

	/* Return value */

/* 966 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 968 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 970 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_LabelCaption */

/* 972 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 974 */	NdrFcLong( 0x0 ),	/* 0 */
/* 978 */	NdrFcShort( 0x7 ),	/* 7 */
/* 980 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 982 */	NdrFcShort( 0x0 ),	/* 0 */
/* 984 */	NdrFcShort( 0x8 ),	/* 8 */
/* 986 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 988 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 990 */	NdrFcShort( 0x1 ),	/* 1 */
/* 992 */	NdrFcShort( 0x0 ),	/* 0 */
/* 994 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 996 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 998 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1000 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Return value */

/* 1002 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1004 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1006 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_LabelCaption */

/* 1008 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1010 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1014 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1016 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1018 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1020 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1022 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1024 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1026 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1028 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1030 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 1032 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1034 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1036 */	NdrFcShort( 0x3a ),	/* Type Offset=58 */

	/* Return value */

/* 1038 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1040 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1042 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_X */


	/* Procedure get_LabelX */

/* 1044 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1046 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1050 */	NdrFcShort( 0x9 ),	/* 9 */
/* 1052 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1054 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1056 */	NdrFcShort( 0x2c ),	/* 44 */
/* 1058 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1060 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1062 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1064 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1066 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */


	/* Parameter pVal */

/* 1068 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1070 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1072 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 1074 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1076 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1078 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_X */


	/* Procedure put_LabelX */

/* 1080 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1082 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1086 */	NdrFcShort( 0xa ),	/* 10 */
/* 1088 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1090 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1092 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1094 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1096 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1098 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1100 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1102 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */


	/* Parameter newVal */

/* 1104 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1106 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1108 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 1110 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1112 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1114 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Y */


	/* Procedure get_LabelY */

/* 1116 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1118 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1122 */	NdrFcShort( 0xb ),	/* 11 */
/* 1124 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1126 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1128 */	NdrFcShort( 0x2c ),	/* 44 */
/* 1130 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1132 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1134 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1136 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1138 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */


	/* Parameter pVal */

/* 1140 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1142 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1144 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 1146 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1148 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1150 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Y */


	/* Procedure put_LabelY */

/* 1152 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1154 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1158 */	NdrFcShort( 0xc ),	/* 12 */
/* 1160 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1162 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1164 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1166 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1168 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1170 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1172 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1174 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */


	/* Parameter newVal */

/* 1176 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1178 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1180 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 1182 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1184 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1186 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Visible */


	/* Procedure get_LabelHorizontal */

/* 1188 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1190 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1194 */	NdrFcShort( 0xf ),	/* 15 */
/* 1196 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1198 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1200 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1202 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1204 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1206 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1208 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1210 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */


	/* Parameter pVal */

/* 1212 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1214 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1216 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 1218 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1220 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1222 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Visible */


	/* Procedure put_LabelHorizontal */

/* 1224 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1226 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1230 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1232 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1234 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1236 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1238 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1240 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1242 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1244 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1246 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */


	/* Parameter newVal */

/* 1248 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1250 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1252 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */


	/* Return value */

/* 1254 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1256 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1258 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Style */

/* 1260 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1262 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1266 */	NdrFcShort( 0xd ),	/* 13 */
/* 1268 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1270 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1272 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1274 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1276 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1278 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1280 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1282 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 1284 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 1286 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1288 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Return value */

/* 1290 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1292 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1294 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Style */

/* 1296 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1298 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1302 */	NdrFcShort( 0xe ),	/* 14 */
/* 1304 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1306 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1308 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1310 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1312 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1314 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1316 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1318 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 1320 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1322 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1324 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Return value */

/* 1326 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1328 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1330 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Mode */

/* 1332 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1334 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1338 */	NdrFcShort( 0x11 ),	/* 17 */
/* 1340 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1342 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1344 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1346 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1348 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1350 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1352 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1354 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 1356 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 1358 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1360 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Return value */

/* 1362 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1364 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1366 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Mode */

/* 1368 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1370 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1374 */	NdrFcShort( 0x12 ),	/* 18 */
/* 1376 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1378 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1380 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1382 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1384 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1386 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1388 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1390 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 1392 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1394 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1396 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Return value */

/* 1398 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1400 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1402 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Name */

/* 1404 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1406 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1410 */	NdrFcShort( 0x13 ),	/* 19 */
/* 1412 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1414 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1416 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1418 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1420 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 1422 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1424 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1426 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 1428 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 1430 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1432 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Return value */

/* 1434 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1436 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1438 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Name */

/* 1440 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1442 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1446 */	NdrFcShort( 0x14 ),	/* 20 */
/* 1448 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1450 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1452 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1454 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1456 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1458 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1460 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1462 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 1464 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1466 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1468 */	NdrFcShort( 0x3a ),	/* Type Offset=58 */

	/* Return value */

/* 1470 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1472 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1474 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_GridNumber */

/* 1476 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1478 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1482 */	NdrFcShort( 0x7 ),	/* 7 */
/* 1484 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1486 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1488 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1490 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1492 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1494 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1496 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1498 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 1500 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1502 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1504 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1506 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1508 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1510 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_GridNumber */

/* 1512 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1514 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1518 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1520 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1522 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1524 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1526 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1528 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1530 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1532 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1534 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 1536 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1538 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1540 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1542 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1544 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1546 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Label */

/* 1548 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1550 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1554 */	NdrFcShort( 0x9 ),	/* 9 */
/* 1556 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1558 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1560 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1562 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1564 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 1566 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1568 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1570 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 1572 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 1574 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1576 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Return value */

/* 1578 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1580 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1582 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Label */

/* 1584 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1586 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1590 */	NdrFcShort( 0xa ),	/* 10 */
/* 1592 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1594 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1596 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1598 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1600 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1602 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1604 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1606 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 1608 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1610 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1612 */	NdrFcShort( 0x3a ),	/* Type Offset=58 */

	/* Return value */

/* 1614 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1616 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1618 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Log */

/* 1620 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1622 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1626 */	NdrFcShort( 0xb ),	/* 11 */
/* 1628 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1630 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1632 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1634 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1636 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1638 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1640 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1642 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 1644 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1646 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1648 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1650 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1652 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1654 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Log */

/* 1656 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1658 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1662 */	NdrFcShort( 0xc ),	/* 12 */
/* 1664 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1666 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1668 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1670 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1672 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1674 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1676 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1678 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 1680 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1682 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1684 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1686 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1688 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1690 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Time */

/* 1692 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1694 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1698 */	NdrFcShort( 0xd ),	/* 13 */
/* 1700 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1702 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1704 */	NdrFcShort( 0x22 ),	/* 34 */
/* 1706 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1708 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1710 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1712 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1714 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 1716 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 1718 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1720 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1722 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1724 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1726 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Time */

/* 1728 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1730 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1734 */	NdrFcShort( 0xe ),	/* 14 */
/* 1736 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1738 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1740 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1742 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1744 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1746 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1748 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1750 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 1752 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1754 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1756 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1758 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1760 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1762 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Format */

/* 1764 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1766 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1770 */	NdrFcShort( 0xf ),	/* 15 */
/* 1772 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1774 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1776 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1778 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1780 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 1782 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1784 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1786 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 1788 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 1790 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1792 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Return value */

/* 1794 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1796 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1798 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Format */

/* 1800 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1802 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1806 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1808 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1810 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1812 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1814 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 1816 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 1818 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1820 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1822 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 1824 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 1826 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1828 */	NdrFcShort( 0x3a ),	/* Type Offset=58 */

	/* Return value */

/* 1830 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1832 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1834 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Item */

/* 1836 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1838 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1842 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1844 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 1846 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1848 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1850 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 1852 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1854 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1856 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1858 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 1860 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1862 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1864 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter pVal */

/* 1866 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1868 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1870 */	NdrFcShort( 0x436 ),	/* Type Offset=1078 */

	/* Return value */

/* 1872 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1874 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1876 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Add */

/* 1878 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1880 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1884 */	NdrFcShort( 0xb ),	/* 11 */
/* 1886 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1888 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1890 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1892 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 1894 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1896 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1898 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1900 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 1902 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 1904 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1906 */	NdrFcShort( 0x436 ),	/* Type Offset=1078 */

	/* Return value */

/* 1908 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1910 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1912 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Delete */

/* 1914 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1916 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1920 */	NdrFcShort( 0xc ),	/* 12 */
/* 1922 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1924 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1926 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1928 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1930 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1932 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1934 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1936 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter index */

/* 1938 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1940 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1942 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 1944 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1946 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1948 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Enabled */

/* 1950 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1952 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1956 */	NdrFcShort( 0x7 ),	/* 7 */
/* 1958 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1960 */	NdrFcShort( 0x6 ),	/* 6 */
/* 1962 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1964 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 1966 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 1968 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1970 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1972 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter vbool */

/* 1974 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 1976 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 1978 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 1980 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 1982 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 1984 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Enabled */

/* 1986 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 1988 */	NdrFcLong( 0x0 ),	/* 0 */
/* 1992 */	NdrFcShort( 0x8 ),	/* 8 */
/* 1994 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 1996 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1998 */	NdrFcShort( 0x22 ),	/* 34 */
/* 2000 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2002 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2004 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2006 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2008 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pbool */

/* 2010 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2012 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2014 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2016 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2018 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2020 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Caption */

/* 2022 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2024 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2028 */	NdrFcShort( 0x9 ),	/* 9 */
/* 2030 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2032 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2034 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2036 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 2038 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 2040 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2042 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2044 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter strCaption */

/* 2046 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 2048 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2050 */	NdrFcShort( 0x3a ),	/* Type Offset=58 */

	/* Return value */

/* 2052 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2054 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2056 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Caption */

/* 2058 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2060 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2064 */	NdrFcShort( 0xa ),	/* 10 */
/* 2066 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2068 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2070 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2072 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2074 */	0x8,		/* 8 */
			0x3,		/* Ext Flags:  new corr desc, clt corr check, */
/* 2076 */	NdrFcShort( 0x1 ),	/* 1 */
/* 2078 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2080 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pstrCaption */

/* 2082 */	NdrFcShort( 0x2113 ),	/* Flags:  must size, must free, out, simple ref, srv alloc size=8 */
/* 2084 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2086 */	NdrFcShort( 0x2c ),	/* Type Offset=44 */

	/* Return value */

/* 2088 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2090 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2092 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_Appearance */

/* 2094 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2096 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2100 */	NdrFcShort( 0xb ),	/* 11 */
/* 2102 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2104 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2106 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2108 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2110 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2112 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2114 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2116 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter appearance */

/* 2118 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2120 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2122 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2124 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2126 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2128 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Appearance */

/* 2130 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2132 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2136 */	NdrFcShort( 0xc ),	/* 12 */
/* 2138 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2140 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2142 */	NdrFcShort( 0x22 ),	/* 34 */
/* 2144 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2146 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2148 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2150 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2152 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pappearance */

/* 2154 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2156 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2158 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2160 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2162 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2164 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_GridColor */

/* 2166 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2168 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2172 */	NdrFcShort( 0xf ),	/* 15 */
/* 2174 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2176 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2178 */	NdrFcShort( 0x24 ),	/* 36 */
/* 2180 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2182 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2184 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2186 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2188 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 2190 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2192 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2194 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2196 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2198 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2200 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_GridColor */

/* 2202 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2204 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2208 */	NdrFcShort( 0x10 ),	/* 16 */
/* 2210 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2212 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2214 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2216 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2218 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2220 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2222 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2224 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 2226 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2228 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2230 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2232 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2234 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2236 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_LabelColor */

/* 2238 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2240 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2244 */	NdrFcShort( 0x11 ),	/* 17 */
/* 2246 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2248 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2250 */	NdrFcShort( 0x24 ),	/* 36 */
/* 2252 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2254 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2256 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2258 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2260 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 2262 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2264 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2266 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2268 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2270 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2272 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_LabelColor */

/* 2274 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2276 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2280 */	NdrFcShort( 0x12 ),	/* 18 */
/* 2282 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2284 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2286 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2288 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2290 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2292 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2294 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2296 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 2298 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2300 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2302 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2304 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2306 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2308 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_LabelFont */

/* 2310 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2312 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2316 */	NdrFcShort( 0x13 ),	/* 19 */
/* 2318 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2320 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2322 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2324 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2326 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2328 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2330 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2332 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 2334 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2336 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2338 */	NdrFcShort( 0x43a ),	/* Type Offset=1082 */

	/* Return value */

/* 2340 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2342 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2344 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_LabelFont */

/* 2346 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2348 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2352 */	NdrFcShort( 0x14 ),	/* 20 */
/* 2354 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2356 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2358 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2360 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 2362 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2364 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2366 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2368 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 2370 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2372 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2374 */	NdrFcShort( 0x43e ),	/* Type Offset=1086 */

	/* Return value */

/* 2376 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2378 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2380 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_TickFont */

/* 2382 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2384 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2388 */	NdrFcShort( 0x15 ),	/* 21 */
/* 2390 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2392 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2394 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2396 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2398 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2400 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2402 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2404 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 2406 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2408 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2410 */	NdrFcShort( 0x43a ),	/* Type Offset=1082 */

	/* Return value */

/* 2412 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2414 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2416 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_TickFont */

/* 2418 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2420 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2424 */	NdrFcShort( 0x16 ),	/* 22 */
/* 2426 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2428 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2430 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2432 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 2434 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2436 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2438 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2440 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 2442 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2444 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2446 */	NdrFcShort( 0x43e ),	/* Type Offset=1086 */

	/* Return value */

/* 2448 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2450 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2452 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_TitleFont */

/* 2454 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2456 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2460 */	NdrFcShort( 0x17 ),	/* 23 */
/* 2462 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2464 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2466 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2468 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2470 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2472 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2474 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2476 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 2478 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2480 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2482 */	NdrFcShort( 0x43a ),	/* Type Offset=1082 */

	/* Return value */

/* 2484 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2486 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2488 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_TitleFont */

/* 2490 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2492 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2496 */	NdrFcShort( 0x18 ),	/* 24 */
/* 2498 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2500 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2502 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2504 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 2506 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2508 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2510 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2512 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 2514 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2516 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2518 */	NdrFcShort( 0x43e ),	/* Type Offset=1086 */

	/* Return value */

/* 2520 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2522 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2524 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_IdentFont */

/* 2526 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2528 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2532 */	NdrFcShort( 0x19 ),	/* 25 */
/* 2534 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2536 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2538 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2540 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2542 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2544 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2546 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2548 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 2550 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2552 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2554 */	NdrFcShort( 0x43a ),	/* Type Offset=1082 */

	/* Return value */

/* 2556 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2558 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2560 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_IdentFont */

/* 2562 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2564 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2568 */	NdrFcShort( 0x1a ),	/* 26 */
/* 2570 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2572 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2574 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2576 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 2578 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2580 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2582 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2584 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 2586 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2588 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2590 */	NdrFcShort( 0x43e ),	/* Type Offset=1086 */

	/* Return value */

/* 2592 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2594 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2596 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_ElementIdentify */

/* 2598 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2600 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2604 */	NdrFcShort( 0x1b ),	/* 27 */
/* 2606 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2608 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2610 */	NdrFcShort( 0x22 ),	/* 34 */
/* 2612 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2614 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2616 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2618 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2620 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 2622 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2624 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2626 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2628 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2630 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2632 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_ElementIdentify */

/* 2634 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2636 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2640 */	NdrFcShort( 0x1c ),	/* 28 */
/* 2642 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2644 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2646 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2648 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2650 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2652 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2654 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2656 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 2658 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2660 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2662 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 2664 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2666 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2668 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_TrackMode */

/* 2670 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2672 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2676 */	NdrFcShort( 0x1d ),	/* 29 */
/* 2678 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2680 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2682 */	NdrFcShort( 0x22 ),	/* 34 */
/* 2684 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2686 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2688 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2690 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2692 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 2694 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 2696 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2698 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Return value */

/* 2700 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2702 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2704 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_TrackMode */

/* 2706 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2708 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2712 */	NdrFcShort( 0x1e ),	/* 30 */
/* 2714 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2716 */	NdrFcShort( 0x6 ),	/* 6 */
/* 2718 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2720 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2722 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2724 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2726 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2728 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 2730 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2732 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2734 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Return value */

/* 2736 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2738 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2740 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_ControlFramePicture */

/* 2742 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2744 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2748 */	NdrFcShort( 0x1f ),	/* 31 */
/* 2750 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2752 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2754 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2756 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2758 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2760 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2762 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2764 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 2766 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2768 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2770 */	NdrFcShort( 0x450 ),	/* Type Offset=1104 */

	/* Return value */

/* 2772 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2774 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2776 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_ControlFramePicture */

/* 2778 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2780 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2784 */	NdrFcShort( 0x20 ),	/* 32 */
/* 2786 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2788 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2790 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2792 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 2794 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2796 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2798 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2800 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 2802 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2804 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2806 */	NdrFcShort( 0x454 ),	/* Type Offset=1108 */

	/* Return value */

/* 2808 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2810 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2812 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_PlotAreaPicture */

/* 2814 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2816 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2820 */	NdrFcShort( 0x21 ),	/* 33 */
/* 2822 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2824 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2826 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2828 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 2830 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2832 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2834 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2836 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 2838 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 2840 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2842 */	NdrFcShort( 0x450 ),	/* Type Offset=1104 */

	/* Return value */

/* 2844 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2846 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2848 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_PlotAreaPicture */

/* 2850 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2852 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2856 */	NdrFcShort( 0x22 ),	/* 34 */
/* 2858 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2860 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2862 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2864 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 2866 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2868 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2870 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2872 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 2874 */	NdrFcShort( 0xb ),	/* Flags:  must size, must free, in, */
/* 2876 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2878 */	NdrFcShort( 0x454 ),	/* Type Offset=1108 */

	/* Return value */

/* 2880 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2882 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2884 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_ControlFrameColor */

/* 2886 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2888 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2892 */	NdrFcShort( 0x23 ),	/* 35 */
/* 2894 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2896 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2898 */	NdrFcShort( 0x24 ),	/* 36 */
/* 2900 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2902 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2904 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2906 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2908 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 2910 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2912 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2914 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2916 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2918 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2920 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_ControlFrameColor */

/* 2922 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2924 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2928 */	NdrFcShort( 0x24 ),	/* 36 */
/* 2930 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2932 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2934 */	NdrFcShort( 0x8 ),	/* 8 */
/* 2936 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2938 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2940 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2942 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2944 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 2946 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 2948 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2950 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2952 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2954 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2956 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_PlotAreaColor */

/* 2958 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2960 */	NdrFcLong( 0x0 ),	/* 0 */
/* 2964 */	NdrFcShort( 0x25 ),	/* 37 */
/* 2966 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 2968 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2970 */	NdrFcShort( 0x24 ),	/* 36 */
/* 2972 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 2974 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 2976 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2978 */	NdrFcShort( 0x0 ),	/* 0 */
/* 2980 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 2982 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 2984 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 2986 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 2988 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 2990 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 2992 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_PlotAreaColor */

/* 2994 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 2996 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3000 */	NdrFcShort( 0x26 ),	/* 38 */
/* 3002 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3004 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3006 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3008 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 3010 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3012 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3014 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3016 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 3018 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3020 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3022 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 3024 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3026 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3028 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_FrameStyle */

/* 3030 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3032 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3036 */	NdrFcShort( 0x27 ),	/* 39 */
/* 3038 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3040 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3042 */	NdrFcShort( 0x22 ),	/* 34 */
/* 3044 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 3046 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3048 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3050 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3052 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 3054 */	NdrFcShort( 0x2010 ),	/* Flags:  out, srv alloc size=8 */
/* 3056 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3058 */	NdrFcShort( 0x6 ),	/* Type Offset=6 */

	/* Return value */

/* 3060 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3062 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3064 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure put_FrameStyle */

/* 3066 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3068 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3072 */	NdrFcShort( 0x28 ),	/* 40 */
/* 3074 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3076 */	NdrFcShort( 0x6 ),	/* 6 */
/* 3078 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3080 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 3082 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3084 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3086 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3088 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter newVal */

/* 3090 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3092 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3094 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Return value */

/* 3096 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3098 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3100 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Annotations */

/* 3102 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3104 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3108 */	NdrFcShort( 0x29 ),	/* 41 */
/* 3110 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3112 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3114 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3116 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 3118 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3120 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3122 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3124 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 3126 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 3128 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3130 */	NdrFcShort( 0x466 ),	/* Type Offset=1126 */

	/* Return value */

/* 3132 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3134 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3136 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Elements */

/* 3138 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3140 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3144 */	NdrFcShort( 0x2a ),	/* 42 */
/* 3146 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3148 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3150 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3152 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 3154 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3156 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3158 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3160 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 3162 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 3164 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3166 */	NdrFcShort( 0x466 ),	/* Type Offset=1126 */

	/* Return value */

/* 3168 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3170 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3172 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Cursors */

/* 3174 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3176 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3180 */	NdrFcShort( 0x2b ),	/* 43 */
/* 3182 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3184 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3186 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3188 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x2,		/* 2 */
/* 3190 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3192 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3194 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3196 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pVal */

/* 3198 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 3200 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3202 */	NdrFcShort( 0x466 ),	/* Type Offset=1126 */

	/* Return value */

/* 3204 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3206 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3208 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure get_Axis */

/* 3210 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3212 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3216 */	NdrFcShort( 0x2c ),	/* 44 */
/* 3218 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3220 */	NdrFcShort( 0x6 ),	/* 6 */
/* 3222 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3224 */	0x45,		/* Oi2 Flags:  srv must size, has return, has ext, */
			0x3,		/* 3 */
/* 3226 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3228 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3230 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3232 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter eOrientation */

/* 3234 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3236 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3238 */	0xd,		/* FC_ENUM16 */
			0x0,		/* 0 */

	/* Parameter pVal */

/* 3240 */	NdrFcShort( 0x13 ),	/* Flags:  must size, must free, out, */
/* 3242 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3244 */	NdrFcShort( 0x47c ),	/* Type Offset=1148 */

	/* Return value */

/* 3246 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3248 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3250 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SetRange */

/* 3252 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3254 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3258 */	NdrFcShort( 0x2d ),	/* 45 */
/* 3260 */	NdrFcShort( 0x28 ),	/* x86 Stack size/offset = 40 */
/* 3262 */	NdrFcShort( 0x40 ),	/* 64 */
/* 3264 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3266 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x5,		/* 5 */
/* 3268 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3270 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3272 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3274 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter xmin */

/* 3276 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3278 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3280 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter xmax */

/* 3282 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3284 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3286 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter ymin */

/* 3288 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3290 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3292 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter ymax */

/* 3294 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3296 */	NdrFcShort( 0x1c ),	/* x86 Stack size/offset = 28 */
/* 3298 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */

/* 3300 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3302 */	NdrFcShort( 0x24 ),	/* x86 Stack size/offset = 36 */
/* 3304 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetRange */

/* 3306 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3308 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3312 */	NdrFcShort( 0x2e ),	/* 46 */
/* 3314 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 3316 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3318 */	NdrFcShort( 0x98 ),	/* 152 */
/* 3320 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x5,		/* 5 */
/* 3322 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3324 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3326 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3328 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter xmin */

/* 3330 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 3332 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3334 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter xmax */

/* 3336 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 3338 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3340 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter ymin */

/* 3342 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 3344 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3346 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter ymax */

/* 3348 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 3350 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3352 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */

/* 3354 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3356 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3358 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AutoRange */

/* 3360 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3362 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3366 */	NdrFcShort( 0x2f ),	/* 47 */
/* 3368 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3370 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3372 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3374 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 3376 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3378 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3380 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3382 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 3384 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3386 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3388 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetAutoRange */

/* 3390 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3392 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3396 */	NdrFcShort( 0x30 ),	/* 48 */
/* 3398 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 3400 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3402 */	NdrFcShort( 0x98 ),	/* 152 */
/* 3404 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x5,		/* 5 */
/* 3406 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3408 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3410 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3412 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter xmin */

/* 3414 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 3416 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3418 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter xmax */

/* 3420 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 3422 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3424 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter ymin */

/* 3426 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 3428 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3430 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter ymax */

/* 3432 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 3434 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3436 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */

/* 3438 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3440 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3442 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure CopyToClipboard */

/* 3444 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3446 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3450 */	NdrFcShort( 0x31 ),	/* 49 */
/* 3452 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3454 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3456 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3458 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 3460 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3462 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3464 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3466 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 3468 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3470 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3472 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PrintGraph */

/* 3474 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3476 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3480 */	NdrFcShort( 0x32 ),	/* 50 */
/* 3482 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3484 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3486 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3488 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 3490 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3492 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3494 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3496 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 3498 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3500 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3502 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ClearGraph */

/* 3504 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3506 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3510 */	NdrFcShort( 0x33 ),	/* 51 */
/* 3512 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3514 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3516 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3518 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 3520 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3522 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3524 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3526 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 3528 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3530 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3532 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ShowProperties */

/* 3534 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3536 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3540 */	NdrFcShort( 0x34 ),	/* 52 */
/* 3542 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3544 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3546 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3548 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 3550 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3552 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3554 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3556 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 3558 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3560 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3562 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SaveAs */

/* 3564 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3566 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3570 */	NdrFcShort( 0x35 ),	/* 53 */
/* 3572 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3574 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3576 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3578 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 3580 */	0x8,		/* 8 */
			0x5,		/* Ext Flags:  new corr desc, srv corr check, */
/* 3582 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3584 */	NdrFcShort( 0x1 ),	/* 1 */
/* 3586 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter szFilename */

/* 3588 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 3590 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3592 */	NdrFcShort( 0x3a ),	/* Type Offset=58 */

	/* Return value */

/* 3594 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3596 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3598 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AboutBox */

/* 3600 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3602 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3606 */	NdrFcShort( 0x36 ),	/* 54 */
/* 3608 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3610 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3612 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3614 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x1,		/* 1 */
/* 3616 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3618 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3620 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3622 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Return value */

/* 3624 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3626 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3628 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ShiftDisplay */

/* 3630 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3632 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3636 */	NdrFcShort( 0x37 ),	/* 55 */
/* 3638 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3640 */	NdrFcShort( 0xc ),	/* 12 */
/* 3642 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3644 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x3,		/* 3 */
/* 3646 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3648 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3650 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3652 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter xShift */

/* 3654 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3656 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3658 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter yShift */

/* 3660 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3662 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3664 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Return value */

/* 3666 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3668 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3670 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GenerateCSVReport */

/* 3672 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3674 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3678 */	NdrFcShort( 0x38 ),	/* 56 */
/* 3680 */	NdrFcShort( 0xc ),	/* x86 Stack size/offset = 12 */
/* 3682 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3684 */	NdrFcShort( 0x8 ),	/* 8 */
/* 3686 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x2,		/* 2 */
/* 3688 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3690 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3692 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3694 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter pFilename */

/* 3696 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 3698 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3700 */	NdrFcShort( 0x494 ),	/* Type Offset=1172 */

	/* Return value */

/* 3702 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3704 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3706 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure GetElementValueAtCursor */

/* 3708 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 3710 */	NdrFcLong( 0x0 ),	/* 0 */
/* 3714 */	NdrFcShort( 0x39 ),	/* 57 */
/* 3716 */	NdrFcShort( 0x18 ),	/* x86 Stack size/offset = 24 */
/* 3718 */	NdrFcShort( 0x16 ),	/* 22 */
/* 3720 */	NdrFcShort( 0x2c ),	/* 44 */
/* 3722 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x4,		/* 4 */
/* 3724 */	0x8,		/* 8 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 3726 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3728 */	NdrFcShort( 0x0 ),	/* 0 */
/* 3730 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter shElementID */

/* 3732 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3734 */	NdrFcShort( 0x4 ),	/* x86 Stack size/offset = 4 */
/* 3736 */	0x6,		/* FC_SHORT */
			0x0,		/* 0 */

	/* Parameter dblCursorVal */

/* 3738 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 3740 */	NdrFcShort( 0x8 ),	/* x86 Stack size/offset = 8 */
/* 3742 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Parameter dblElemVal */

/* 3744 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 3746 */	NdrFcShort( 0x10 ),	/* x86 Stack size/offset = 16 */
/* 3748 */	0xc,		/* FC_DOUBLE */
			0x0,		/* 0 */

	/* Return value */

/* 3750 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 3752 */	NdrFcShort( 0x14 ),	/* x86 Stack size/offset = 20 */
/* 3754 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const DMGraph_MIDL_TYPE_FORMAT_STRING DMGraph__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/*  4 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/*  6 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/*  8 */	0xd,		/* FC_ENUM16 */
			0x5c,		/* FC_PAD */
/* 10 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 12 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 14 */	
			0x11, 0x4,	/* FC_RP [alloced_on_stack] */
/* 16 */	NdrFcShort( 0x1c ),	/* Offset= 28 (44) */
/* 18 */	
			0x13, 0x0,	/* FC_OP */
/* 20 */	NdrFcShort( 0xe ),	/* Offset= 14 (34) */
/* 22 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 24 */	NdrFcShort( 0x2 ),	/* 2 */
/* 26 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 28 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 30 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 32 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 34 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 36 */	NdrFcShort( 0x8 ),	/* 8 */
/* 38 */	NdrFcShort( 0xfff0 ),	/* Offset= -16 (22) */
/* 40 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 42 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 44 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 46 */	NdrFcShort( 0x0 ),	/* 0 */
/* 48 */	NdrFcShort( 0x4 ),	/* 4 */
/* 50 */	NdrFcShort( 0x0 ),	/* 0 */
/* 52 */	NdrFcShort( 0xffde ),	/* Offset= -34 (18) */
/* 54 */	
			0x12, 0x0,	/* FC_UP */
/* 56 */	NdrFcShort( 0xffea ),	/* Offset= -22 (34) */
/* 58 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 60 */	NdrFcShort( 0x0 ),	/* 0 */
/* 62 */	NdrFcShort( 0x4 ),	/* 4 */
/* 64 */	NdrFcShort( 0x0 ),	/* 0 */
/* 66 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (54) */
/* 68 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 70 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 72 */	
			0x12, 0x0,	/* FC_UP */
/* 74 */	NdrFcShort( 0x3ce ),	/* Offset= 974 (1048) */
/* 76 */	
			0x2b,		/* FC_NON_ENCAPSULATED_UNION */
			0x9,		/* FC_ULONG */
/* 78 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 80 */	NdrFcShort( 0xfff8 ),	/* -8 */
/* 82 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 84 */	NdrFcShort( 0x2 ),	/* Offset= 2 (86) */
/* 86 */	NdrFcShort( 0x10 ),	/* 16 */
/* 88 */	NdrFcShort( 0x2f ),	/* 47 */
/* 90 */	NdrFcLong( 0x14 ),	/* 20 */
/* 94 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 96 */	NdrFcLong( 0x3 ),	/* 3 */
/* 100 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 102 */	NdrFcLong( 0x11 ),	/* 17 */
/* 106 */	NdrFcShort( 0x8001 ),	/* Simple arm type: FC_BYTE */
/* 108 */	NdrFcLong( 0x2 ),	/* 2 */
/* 112 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 114 */	NdrFcLong( 0x4 ),	/* 4 */
/* 118 */	NdrFcShort( 0x800a ),	/* Simple arm type: FC_FLOAT */
/* 120 */	NdrFcLong( 0x5 ),	/* 5 */
/* 124 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 126 */	NdrFcLong( 0xb ),	/* 11 */
/* 130 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 132 */	NdrFcLong( 0xa ),	/* 10 */
/* 136 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 138 */	NdrFcLong( 0x6 ),	/* 6 */
/* 142 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (374) */
/* 144 */	NdrFcLong( 0x7 ),	/* 7 */
/* 148 */	NdrFcShort( 0x800c ),	/* Simple arm type: FC_DOUBLE */
/* 150 */	NdrFcLong( 0x8 ),	/* 8 */
/* 154 */	NdrFcShort( 0xff9c ),	/* Offset= -100 (54) */
/* 156 */	NdrFcLong( 0xd ),	/* 13 */
/* 160 */	NdrFcShort( 0xdc ),	/* Offset= 220 (380) */
/* 162 */	NdrFcLong( 0x9 ),	/* 9 */
/* 166 */	NdrFcShort( 0xe8 ),	/* Offset= 232 (398) */
/* 168 */	NdrFcLong( 0x2000 ),	/* 8192 */
/* 172 */	NdrFcShort( 0xf4 ),	/* Offset= 244 (416) */
/* 174 */	NdrFcLong( 0x24 ),	/* 36 */
/* 178 */	NdrFcShort( 0x31c ),	/* Offset= 796 (974) */
/* 180 */	NdrFcLong( 0x4024 ),	/* 16420 */
/* 184 */	NdrFcShort( 0x316 ),	/* Offset= 790 (974) */
/* 186 */	NdrFcLong( 0x4011 ),	/* 16401 */
/* 190 */	NdrFcShort( 0x314 ),	/* Offset= 788 (978) */
/* 192 */	NdrFcLong( 0x4002 ),	/* 16386 */
/* 196 */	NdrFcShort( 0x312 ),	/* Offset= 786 (982) */
/* 198 */	NdrFcLong( 0x4003 ),	/* 16387 */
/* 202 */	NdrFcShort( 0x310 ),	/* Offset= 784 (986) */
/* 204 */	NdrFcLong( 0x4014 ),	/* 16404 */
/* 208 */	NdrFcShort( 0x30e ),	/* Offset= 782 (990) */
/* 210 */	NdrFcLong( 0x4004 ),	/* 16388 */
/* 214 */	NdrFcShort( 0x30c ),	/* Offset= 780 (994) */
/* 216 */	NdrFcLong( 0x4005 ),	/* 16389 */
/* 220 */	NdrFcShort( 0x30a ),	/* Offset= 778 (998) */
/* 222 */	NdrFcLong( 0x400b ),	/* 16395 */
/* 226 */	NdrFcShort( 0x2f4 ),	/* Offset= 756 (982) */
/* 228 */	NdrFcLong( 0x400a ),	/* 16394 */
/* 232 */	NdrFcShort( 0x2f2 ),	/* Offset= 754 (986) */
/* 234 */	NdrFcLong( 0x4006 ),	/* 16390 */
/* 238 */	NdrFcShort( 0x2fc ),	/* Offset= 764 (1002) */
/* 240 */	NdrFcLong( 0x4007 ),	/* 16391 */
/* 244 */	NdrFcShort( 0x2f2 ),	/* Offset= 754 (998) */
/* 246 */	NdrFcLong( 0x4008 ),	/* 16392 */
/* 250 */	NdrFcShort( 0x2f4 ),	/* Offset= 756 (1006) */
/* 252 */	NdrFcLong( 0x400d ),	/* 16397 */
/* 256 */	NdrFcShort( 0x2f2 ),	/* Offset= 754 (1010) */
/* 258 */	NdrFcLong( 0x4009 ),	/* 16393 */
/* 262 */	NdrFcShort( 0x2f0 ),	/* Offset= 752 (1014) */
/* 264 */	NdrFcLong( 0x6000 ),	/* 24576 */
/* 268 */	NdrFcShort( 0x2ee ),	/* Offset= 750 (1018) */
/* 270 */	NdrFcLong( 0x400c ),	/* 16396 */
/* 274 */	NdrFcShort( 0x2ec ),	/* Offset= 748 (1022) */
/* 276 */	NdrFcLong( 0x10 ),	/* 16 */
/* 280 */	NdrFcShort( 0x8002 ),	/* Simple arm type: FC_CHAR */
/* 282 */	NdrFcLong( 0x12 ),	/* 18 */
/* 286 */	NdrFcShort( 0x8006 ),	/* Simple arm type: FC_SHORT */
/* 288 */	NdrFcLong( 0x13 ),	/* 19 */
/* 292 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 294 */	NdrFcLong( 0x15 ),	/* 21 */
/* 298 */	NdrFcShort( 0x800b ),	/* Simple arm type: FC_HYPER */
/* 300 */	NdrFcLong( 0x16 ),	/* 22 */
/* 304 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 306 */	NdrFcLong( 0x17 ),	/* 23 */
/* 310 */	NdrFcShort( 0x8008 ),	/* Simple arm type: FC_LONG */
/* 312 */	NdrFcLong( 0xe ),	/* 14 */
/* 316 */	NdrFcShort( 0x2ca ),	/* Offset= 714 (1030) */
/* 318 */	NdrFcLong( 0x400e ),	/* 16398 */
/* 322 */	NdrFcShort( 0x2ce ),	/* Offset= 718 (1040) */
/* 324 */	NdrFcLong( 0x4010 ),	/* 16400 */
/* 328 */	NdrFcShort( 0x2cc ),	/* Offset= 716 (1044) */
/* 330 */	NdrFcLong( 0x4012 ),	/* 16402 */
/* 334 */	NdrFcShort( 0x288 ),	/* Offset= 648 (982) */
/* 336 */	NdrFcLong( 0x4013 ),	/* 16403 */
/* 340 */	NdrFcShort( 0x286 ),	/* Offset= 646 (986) */
/* 342 */	NdrFcLong( 0x4015 ),	/* 16405 */
/* 346 */	NdrFcShort( 0x284 ),	/* Offset= 644 (990) */
/* 348 */	NdrFcLong( 0x4016 ),	/* 16406 */
/* 352 */	NdrFcShort( 0x27a ),	/* Offset= 634 (986) */
/* 354 */	NdrFcLong( 0x4017 ),	/* 16407 */
/* 358 */	NdrFcShort( 0x274 ),	/* Offset= 628 (986) */
/* 360 */	NdrFcLong( 0x0 ),	/* 0 */
/* 364 */	NdrFcShort( 0x0 ),	/* Offset= 0 (364) */
/* 366 */	NdrFcLong( 0x1 ),	/* 1 */
/* 370 */	NdrFcShort( 0x0 ),	/* Offset= 0 (370) */
/* 372 */	NdrFcShort( 0xffff ),	/* Offset= -1 (371) */
/* 374 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 376 */	NdrFcShort( 0x8 ),	/* 8 */
/* 378 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 380 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 382 */	NdrFcLong( 0x0 ),	/* 0 */
/* 386 */	NdrFcShort( 0x0 ),	/* 0 */
/* 388 */	NdrFcShort( 0x0 ),	/* 0 */
/* 390 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 392 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 394 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 396 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 398 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 400 */	NdrFcLong( 0x20400 ),	/* 132096 */
/* 404 */	NdrFcShort( 0x0 ),	/* 0 */
/* 406 */	NdrFcShort( 0x0 ),	/* 0 */
/* 408 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 410 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 412 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 414 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 416 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 418 */	NdrFcShort( 0x2 ),	/* Offset= 2 (420) */
/* 420 */	
			0x12, 0x0,	/* FC_UP */
/* 422 */	NdrFcShort( 0x216 ),	/* Offset= 534 (956) */
/* 424 */	
			0x2a,		/* FC_ENCAPSULATED_UNION */
			0x49,		/* 73 */
/* 426 */	NdrFcShort( 0x18 ),	/* 24 */
/* 428 */	NdrFcShort( 0xa ),	/* 10 */
/* 430 */	NdrFcLong( 0x8 ),	/* 8 */
/* 434 */	NdrFcShort( 0x5a ),	/* Offset= 90 (524) */
/* 436 */	NdrFcLong( 0xd ),	/* 13 */
/* 440 */	NdrFcShort( 0x7e ),	/* Offset= 126 (566) */
/* 442 */	NdrFcLong( 0x9 ),	/* 9 */
/* 446 */	NdrFcShort( 0x9e ),	/* Offset= 158 (604) */
/* 448 */	NdrFcLong( 0xc ),	/* 12 */
/* 452 */	NdrFcShort( 0xc8 ),	/* Offset= 200 (652) */
/* 454 */	NdrFcLong( 0x24 ),	/* 36 */
/* 458 */	NdrFcShort( 0x124 ),	/* Offset= 292 (750) */
/* 460 */	NdrFcLong( 0x800d ),	/* 32781 */
/* 464 */	NdrFcShort( 0x140 ),	/* Offset= 320 (784) */
/* 466 */	NdrFcLong( 0x10 ),	/* 16 */
/* 470 */	NdrFcShort( 0x15a ),	/* Offset= 346 (816) */
/* 472 */	NdrFcLong( 0x2 ),	/* 2 */
/* 476 */	NdrFcShort( 0x174 ),	/* Offset= 372 (848) */
/* 478 */	NdrFcLong( 0x3 ),	/* 3 */
/* 482 */	NdrFcShort( 0x18e ),	/* Offset= 398 (880) */
/* 484 */	NdrFcLong( 0x14 ),	/* 20 */
/* 488 */	NdrFcShort( 0x1a8 ),	/* Offset= 424 (912) */
/* 490 */	NdrFcShort( 0xffff ),	/* Offset= -1 (489) */
/* 492 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 494 */	NdrFcShort( 0x4 ),	/* 4 */
/* 496 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 498 */	NdrFcShort( 0x0 ),	/* 0 */
/* 500 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 502 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 504 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 506 */	NdrFcShort( 0x4 ),	/* 4 */
/* 508 */	NdrFcShort( 0x0 ),	/* 0 */
/* 510 */	NdrFcShort( 0x1 ),	/* 1 */
/* 512 */	NdrFcShort( 0x0 ),	/* 0 */
/* 514 */	NdrFcShort( 0x0 ),	/* 0 */
/* 516 */	0x12, 0x0,	/* FC_UP */
/* 518 */	NdrFcShort( 0xfe1c ),	/* Offset= -484 (34) */
/* 520 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 522 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 524 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 526 */	NdrFcShort( 0x8 ),	/* 8 */
/* 528 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 530 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 532 */	NdrFcShort( 0x4 ),	/* 4 */
/* 534 */	NdrFcShort( 0x4 ),	/* 4 */
/* 536 */	0x11, 0x0,	/* FC_RP */
/* 538 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (492) */
/* 540 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 542 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 544 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 546 */	NdrFcShort( 0x0 ),	/* 0 */
/* 548 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 550 */	NdrFcShort( 0x0 ),	/* 0 */
/* 552 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 554 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 558 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 560 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 562 */	NdrFcShort( 0xff4a ),	/* Offset= -182 (380) */
/* 564 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 566 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 568 */	NdrFcShort( 0x8 ),	/* 8 */
/* 570 */	NdrFcShort( 0x0 ),	/* 0 */
/* 572 */	NdrFcShort( 0x6 ),	/* Offset= 6 (578) */
/* 574 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 576 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 578 */	
			0x11, 0x0,	/* FC_RP */
/* 580 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (544) */
/* 582 */	
			0x21,		/* FC_BOGUS_ARRAY */
			0x3,		/* 3 */
/* 584 */	NdrFcShort( 0x0 ),	/* 0 */
/* 586 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 588 */	NdrFcShort( 0x0 ),	/* 0 */
/* 590 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 592 */	NdrFcLong( 0xffffffff ),	/* -1 */
/* 596 */	NdrFcShort( 0x0 ),	/* Corr flags:  */
/* 598 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 600 */	NdrFcShort( 0xff36 ),	/* Offset= -202 (398) */
/* 602 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 604 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 606 */	NdrFcShort( 0x8 ),	/* 8 */
/* 608 */	NdrFcShort( 0x0 ),	/* 0 */
/* 610 */	NdrFcShort( 0x6 ),	/* Offset= 6 (616) */
/* 612 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 614 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 616 */	
			0x11, 0x0,	/* FC_RP */
/* 618 */	NdrFcShort( 0xffdc ),	/* Offset= -36 (582) */
/* 620 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 622 */	NdrFcShort( 0x4 ),	/* 4 */
/* 624 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 626 */	NdrFcShort( 0x0 ),	/* 0 */
/* 628 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 630 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 632 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 634 */	NdrFcShort( 0x4 ),	/* 4 */
/* 636 */	NdrFcShort( 0x0 ),	/* 0 */
/* 638 */	NdrFcShort( 0x1 ),	/* 1 */
/* 640 */	NdrFcShort( 0x0 ),	/* 0 */
/* 642 */	NdrFcShort( 0x0 ),	/* 0 */
/* 644 */	0x12, 0x0,	/* FC_UP */
/* 646 */	NdrFcShort( 0x192 ),	/* Offset= 402 (1048) */
/* 648 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 650 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 652 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 654 */	NdrFcShort( 0x8 ),	/* 8 */
/* 656 */	NdrFcShort( 0x0 ),	/* 0 */
/* 658 */	NdrFcShort( 0x6 ),	/* Offset= 6 (664) */
/* 660 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 662 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 664 */	
			0x11, 0x0,	/* FC_RP */
/* 666 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (620) */
/* 668 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 670 */	NdrFcLong( 0x2f ),	/* 47 */
/* 674 */	NdrFcShort( 0x0 ),	/* 0 */
/* 676 */	NdrFcShort( 0x0 ),	/* 0 */
/* 678 */	0xc0,		/* 192 */
			0x0,		/* 0 */
/* 680 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 682 */	0x0,		/* 0 */
			0x0,		/* 0 */
/* 684 */	0x0,		/* 0 */
			0x46,		/* 70 */
/* 686 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 688 */	NdrFcShort( 0x1 ),	/* 1 */
/* 690 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 692 */	NdrFcShort( 0x4 ),	/* 4 */
/* 694 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 696 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 698 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 700 */	NdrFcShort( 0x10 ),	/* 16 */
/* 702 */	NdrFcShort( 0x0 ),	/* 0 */
/* 704 */	NdrFcShort( 0xa ),	/* Offset= 10 (714) */
/* 706 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 708 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 710 */	NdrFcShort( 0xffd6 ),	/* Offset= -42 (668) */
/* 712 */	0x36,		/* FC_POINTER */
			0x5b,		/* FC_END */
/* 714 */	
			0x12, 0x0,	/* FC_UP */
/* 716 */	NdrFcShort( 0xffe2 ),	/* Offset= -30 (686) */
/* 718 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 720 */	NdrFcShort( 0x4 ),	/* 4 */
/* 722 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 724 */	NdrFcShort( 0x0 ),	/* 0 */
/* 726 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 728 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 730 */	
			0x48,		/* FC_VARIABLE_REPEAT */
			0x49,		/* FC_FIXED_OFFSET */
/* 732 */	NdrFcShort( 0x4 ),	/* 4 */
/* 734 */	NdrFcShort( 0x0 ),	/* 0 */
/* 736 */	NdrFcShort( 0x1 ),	/* 1 */
/* 738 */	NdrFcShort( 0x0 ),	/* 0 */
/* 740 */	NdrFcShort( 0x0 ),	/* 0 */
/* 742 */	0x12, 0x0,	/* FC_UP */
/* 744 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (698) */
/* 746 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 748 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 750 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 752 */	NdrFcShort( 0x8 ),	/* 8 */
/* 754 */	NdrFcShort( 0x0 ),	/* 0 */
/* 756 */	NdrFcShort( 0x6 ),	/* Offset= 6 (762) */
/* 758 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 760 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 762 */	
			0x11, 0x0,	/* FC_RP */
/* 764 */	NdrFcShort( 0xffd2 ),	/* Offset= -46 (718) */
/* 766 */	
			0x1d,		/* FC_SMFARRAY */
			0x0,		/* 0 */
/* 768 */	NdrFcShort( 0x8 ),	/* 8 */
/* 770 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 772 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 774 */	NdrFcShort( 0x10 ),	/* 16 */
/* 776 */	0x8,		/* FC_LONG */
			0x6,		/* FC_SHORT */
/* 778 */	0x6,		/* FC_SHORT */
			0x4c,		/* FC_EMBEDDED_COMPLEX */
/* 780 */	0x0,		/* 0 */
			NdrFcShort( 0xfff1 ),	/* Offset= -15 (766) */
			0x5b,		/* FC_END */
/* 784 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 786 */	NdrFcShort( 0x18 ),	/* 24 */
/* 788 */	NdrFcShort( 0x0 ),	/* 0 */
/* 790 */	NdrFcShort( 0xa ),	/* Offset= 10 (800) */
/* 792 */	0x8,		/* FC_LONG */
			0x36,		/* FC_POINTER */
/* 794 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 796 */	NdrFcShort( 0xffe8 ),	/* Offset= -24 (772) */
/* 798 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 800 */	
			0x11, 0x0,	/* FC_RP */
/* 802 */	NdrFcShort( 0xfefe ),	/* Offset= -258 (544) */
/* 804 */	
			0x1b,		/* FC_CARRAY */
			0x0,		/* 0 */
/* 806 */	NdrFcShort( 0x1 ),	/* 1 */
/* 808 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 810 */	NdrFcShort( 0x0 ),	/* 0 */
/* 812 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 814 */	0x1,		/* FC_BYTE */
			0x5b,		/* FC_END */
/* 816 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 818 */	NdrFcShort( 0x8 ),	/* 8 */
/* 820 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 822 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 824 */	NdrFcShort( 0x4 ),	/* 4 */
/* 826 */	NdrFcShort( 0x4 ),	/* 4 */
/* 828 */	0x12, 0x0,	/* FC_UP */
/* 830 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (804) */
/* 832 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 834 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 836 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/* 838 */	NdrFcShort( 0x2 ),	/* 2 */
/* 840 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 842 */	NdrFcShort( 0x0 ),	/* 0 */
/* 844 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 846 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 848 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 850 */	NdrFcShort( 0x8 ),	/* 8 */
/* 852 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 854 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 856 */	NdrFcShort( 0x4 ),	/* 4 */
/* 858 */	NdrFcShort( 0x4 ),	/* 4 */
/* 860 */	0x12, 0x0,	/* FC_UP */
/* 862 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (836) */
/* 864 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 866 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 868 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 870 */	NdrFcShort( 0x4 ),	/* 4 */
/* 872 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 874 */	NdrFcShort( 0x0 ),	/* 0 */
/* 876 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 878 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 880 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 882 */	NdrFcShort( 0x8 ),	/* 8 */
/* 884 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 886 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 888 */	NdrFcShort( 0x4 ),	/* 4 */
/* 890 */	NdrFcShort( 0x4 ),	/* 4 */
/* 892 */	0x12, 0x0,	/* FC_UP */
/* 894 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (868) */
/* 896 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 898 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 900 */	
			0x1b,		/* FC_CARRAY */
			0x7,		/* 7 */
/* 902 */	NdrFcShort( 0x8 ),	/* 8 */
/* 904 */	0x19,		/* Corr desc:  field pointer, FC_ULONG */
			0x0,		/*  */
/* 906 */	NdrFcShort( 0x0 ),	/* 0 */
/* 908 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 910 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 912 */	
			0x16,		/* FC_PSTRUCT */
			0x3,		/* 3 */
/* 914 */	NdrFcShort( 0x8 ),	/* 8 */
/* 916 */	
			0x4b,		/* FC_PP */
			0x5c,		/* FC_PAD */
/* 918 */	
			0x46,		/* FC_NO_REPEAT */
			0x5c,		/* FC_PAD */
/* 920 */	NdrFcShort( 0x4 ),	/* 4 */
/* 922 */	NdrFcShort( 0x4 ),	/* 4 */
/* 924 */	0x12, 0x0,	/* FC_UP */
/* 926 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (900) */
/* 928 */	
			0x5b,		/* FC_END */

			0x8,		/* FC_LONG */
/* 930 */	0x8,		/* FC_LONG */
			0x5b,		/* FC_END */
/* 932 */	
			0x15,		/* FC_STRUCT */
			0x3,		/* 3 */
/* 934 */	NdrFcShort( 0x8 ),	/* 8 */
/* 936 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 938 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 940 */	
			0x1b,		/* FC_CARRAY */
			0x3,		/* 3 */
/* 942 */	NdrFcShort( 0x8 ),	/* 8 */
/* 944 */	0x7,		/* Corr desc: FC_USHORT */
			0x0,		/*  */
/* 946 */	NdrFcShort( 0xffd8 ),	/* -40 */
/* 948 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 950 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 952 */	NdrFcShort( 0xffec ),	/* Offset= -20 (932) */
/* 954 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 956 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x3,		/* 3 */
/* 958 */	NdrFcShort( 0x28 ),	/* 40 */
/* 960 */	NdrFcShort( 0xffec ),	/* Offset= -20 (940) */
/* 962 */	NdrFcShort( 0x0 ),	/* Offset= 0 (962) */
/* 964 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 966 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 968 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 970 */	NdrFcShort( 0xfdde ),	/* Offset= -546 (424) */
/* 972 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 974 */	
			0x12, 0x0,	/* FC_UP */
/* 976 */	NdrFcShort( 0xfeea ),	/* Offset= -278 (698) */
/* 978 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 980 */	0x1,		/* FC_BYTE */
			0x5c,		/* FC_PAD */
/* 982 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 984 */	0x6,		/* FC_SHORT */
			0x5c,		/* FC_PAD */
/* 986 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 988 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */
/* 990 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 992 */	0xb,		/* FC_HYPER */
			0x5c,		/* FC_PAD */
/* 994 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 996 */	0xa,		/* FC_FLOAT */
			0x5c,		/* FC_PAD */
/* 998 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1000 */	0xc,		/* FC_DOUBLE */
			0x5c,		/* FC_PAD */
/* 1002 */	
			0x12, 0x0,	/* FC_UP */
/* 1004 */	NdrFcShort( 0xfd8a ),	/* Offset= -630 (374) */
/* 1006 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 1008 */	NdrFcShort( 0xfc46 ),	/* Offset= -954 (54) */
/* 1010 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 1012 */	NdrFcShort( 0xfd88 ),	/* Offset= -632 (380) */
/* 1014 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 1016 */	NdrFcShort( 0xfd96 ),	/* Offset= -618 (398) */
/* 1018 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 1020 */	NdrFcShort( 0xfda4 ),	/* Offset= -604 (416) */
/* 1022 */	
			0x12, 0x10,	/* FC_UP [pointer_deref] */
/* 1024 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1026) */
/* 1026 */	
			0x12, 0x0,	/* FC_UP */
/* 1028 */	NdrFcShort( 0x14 ),	/* Offset= 20 (1048) */
/* 1030 */	
			0x15,		/* FC_STRUCT */
			0x7,		/* 7 */
/* 1032 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1034 */	0x6,		/* FC_SHORT */
			0x1,		/* FC_BYTE */
/* 1036 */	0x1,		/* FC_BYTE */
			0x8,		/* FC_LONG */
/* 1038 */	0xb,		/* FC_HYPER */
			0x5b,		/* FC_END */
/* 1040 */	
			0x12, 0x0,	/* FC_UP */
/* 1042 */	NdrFcShort( 0xfff4 ),	/* Offset= -12 (1030) */
/* 1044 */	
			0x12, 0x8,	/* FC_UP [simple_pointer] */
/* 1046 */	0x2,		/* FC_CHAR */
			0x5c,		/* FC_PAD */
/* 1048 */	
			0x1a,		/* FC_BOGUS_STRUCT */
			0x7,		/* 7 */
/* 1050 */	NdrFcShort( 0x20 ),	/* 32 */
/* 1052 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1054 */	NdrFcShort( 0x0 ),	/* Offset= 0 (1054) */
/* 1056 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 1058 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1060 */	0x6,		/* FC_SHORT */
			0x6,		/* FC_SHORT */
/* 1062 */	0x4c,		/* FC_EMBEDDED_COMPLEX */
			0x0,		/* 0 */
/* 1064 */	NdrFcShort( 0xfc24 ),	/* Offset= -988 (76) */
/* 1066 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 1068 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 1070 */	NdrFcShort( 0x1 ),	/* 1 */
/* 1072 */	NdrFcShort( 0x10 ),	/* 16 */
/* 1074 */	NdrFcShort( 0x0 ),	/* 0 */
/* 1076 */	NdrFcShort( 0xfc14 ),	/* Offset= -1004 (72) */
/* 1078 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 1080 */	NdrFcShort( 0xfd56 ),	/* Offset= -682 (398) */
/* 1082 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 1084 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1086) */
/* 1086 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1088 */	NdrFcLong( 0xbef6e003 ),	/* -1091117053 */
/* 1092 */	NdrFcShort( 0xa874 ),	/* -22412 */
/* 1094 */	NdrFcShort( 0x101a ),	/* 4122 */
/* 1096 */	0x8b,		/* 139 */
			0xba,		/* 186 */
/* 1098 */	0x0,		/* 0 */
			0xaa,		/* 170 */
/* 1100 */	0x0,		/* 0 */
			0x30,		/* 48 */
/* 1102 */	0xc,		/* 12 */
			0xab,		/* 171 */
/* 1104 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 1106 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1108) */
/* 1108 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1110 */	NdrFcLong( 0x7bf80981 ),	/* 2079852929 */
/* 1114 */	NdrFcShort( 0xbf32 ),	/* -16590 */
/* 1116 */	NdrFcShort( 0x101a ),	/* 4122 */
/* 1118 */	0x8b,		/* 139 */
			0xbb,		/* 187 */
/* 1120 */	0x0,		/* 0 */
			0xaa,		/* 170 */
/* 1122 */	0x0,		/* 0 */
			0x30,		/* 48 */
/* 1124 */	0xc,		/* 12 */
			0xab,		/* 171 */
/* 1126 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 1128 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1130) */
/* 1130 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1132 */	NdrFcLong( 0x22e739f ),	/* 36598687 */
/* 1136 */	NdrFcShort( 0xbb ),	/* 187 */
/* 1138 */	NdrFcShort( 0x40be ),	/* 16574 */
/* 1140 */	0xbb,		/* 187 */
			0xba,		/* 186 */
/* 1142 */	0x84,		/* 132 */
			0x92,		/* 146 */
/* 1144 */	0xe4,		/* 228 */
			0x44,		/* 68 */
/* 1146 */	0x34,		/* 52 */
			0x93,		/* 147 */
/* 1148 */	
			0x11, 0x10,	/* FC_RP [pointer_deref] */
/* 1150 */	NdrFcShort( 0x2 ),	/* Offset= 2 (1152) */
/* 1152 */	
			0x2f,		/* FC_IP */
			0x5a,		/* FC_CONSTANT_IID */
/* 1154 */	NdrFcLong( 0xc735264e ),	/* -952818098 */
/* 1158 */	NdrFcShort( 0x70bb ),	/* 28859 */
/* 1160 */	NdrFcShort( 0x48bb ),	/* 18619 */
/* 1162 */	0xac,		/* 172 */
			0x9c,		/* 156 */
/* 1164 */	0x93,		/* 147 */
			0x18,		/* 24 */
/* 1166 */	0x8a,		/* 138 */
			0x12,		/* 18 */
/* 1168 */	0xc2,		/* 194 */
			0x54,		/* 84 */
/* 1170 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/* 1172 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */

			0x0
        }
    };

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            BSTR_UserSize
            ,BSTR_UserMarshal
            ,BSTR_UserUnmarshal
            ,BSTR_UserFree
            },
            {
            VARIANT_UserSize
            ,VARIANT_UserMarshal
            ,VARIANT_UserUnmarshal
            ,VARIANT_UserFree
            }

        };



/* Standard interface: __MIDL_itf_DMGraph_0000_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDMGraphElement, ver. 0.0,
   GUID={0x4ca52ab5,0xb6ce,0x41af,{0x9a,0x09,0x28,0x4d,0x00,0x71,0x4f,0x8e}} */

#pragma code_seg(".orpc")
static const unsigned short IDMGraphElement_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    36,
    72,
    108,
    144,
    180,
    216,
    252,
    288,
    324,
    360,
    396,
    432,
    468,
    504,
    540,
    576,
    612,
    648,
    684,
    726,
    768,
    810,
    852,
    894,
    930
    };

static const MIDL_STUBLESS_PROXY_INFO IDMGraphElement_ProxyInfo =
    {
    &Object_StubDesc,
    DMGraph__MIDL_ProcFormatString.Format,
    &IDMGraphElement_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IDMGraphElement_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    DMGraph__MIDL_ProcFormatString.Format,
    &IDMGraphElement_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(33) _IDMGraphElementProxyVtbl = 
{
    &IDMGraphElement_ProxyInfo,
    &IID_IDMGraphElement,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IDMGraphElement::get_LineColor */ ,
    (void *) (INT_PTR) -1 /* IDMGraphElement::put_LineColor */ ,
    (void *) (INT_PTR) -1 /* IDMGraphElement::get_PointColor */ ,
    (void *) (INT_PTR) -1 /* IDMGraphElement::put_PointColor */ ,
    (void *) (INT_PTR) -1 /* IDMGraphElement::get_Linetype */ ,
    (void *) (INT_PTR) -1 /* IDMGraphElement::put_Linetype */ ,
    (void *) (INT_PTR) -1 /* IDMGraphElement::get_LineWidth */ ,
    (void *) (INT_PTR) -1 /* IDMGraphElement::put_LineWidth */ ,
    (void *) (INT_PTR) -1 /* IDMGraphElement::get_PointSymbol */ ,
    (void *) (INT_PTR) -1 /* IDMGraphElement::put_PointSymbol */ ,
    (void *) (INT_PTR) -1 /* IDMGraphElement::get_SolidPoint */ ,
    (void *) (INT_PTR) -1 /* IDMGraphElement::put_SolidPoint */ ,
    (void *) (INT_PTR) -1 /* IDMGraphElement::get_PointSize */ ,
    (void *) (INT_PTR) -1 /* IDMGraphElement::put_PointSize */ ,
    (void *) (INT_PTR) -1 /* IDMGraphElement::get_Show */ ,
    (void *) (INT_PTR) -1 /* IDMGraphElement::put_Show */ ,
    (void *) (INT_PTR) -1 /* IDMGraphElement::get_Name */ ,
    (void *) (INT_PTR) -1 /* IDMGraphElement::put_Name */ ,
    (void *) (INT_PTR) -1 /* IDMGraphElement::get_Count */ ,
    (void *) (INT_PTR) -1 /* IDMGraphElement::get_XValue */ ,
    (void *) (INT_PTR) -1 /* IDMGraphElement::put_XValue */ ,
    (void *) (INT_PTR) -1 /* IDMGraphElement::get_YValue */ ,
    (void *) (INT_PTR) -1 /* IDMGraphElement::put_YValue */ ,
    (void *) (INT_PTR) -1 /* IDMGraphElement::PlotXY */ ,
    (void *) (INT_PTR) -1 /* IDMGraphElement::PlotY */ ,
    (void *) (INT_PTR) -1 /* IDMGraphElement::Plot */
};


static const PRPC_STUB_FUNCTION IDMGraphElement_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IDMGraphElementStubVtbl =
{
    &IID_IDMGraphElement,
    &IDMGraphElement_ServerInfo,
    33,
    &IDMGraphElement_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IDMGraphAnnotation, ver. 0.0,
   GUID={0x7d965dbb,0x74cd,0x42d2,{0xa9,0xc6,0x53,0xed,0x80,0x18,0x8b,0x35}} */

#pragma code_seg(".orpc")
static const unsigned short IDMGraphAnnotation_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    972,
    1008,
    1044,
    1080,
    1116,
    1152,
    216,
    252,
    1188,
    1224,
    360,
    396,
    432,
    468
    };

static const MIDL_STUBLESS_PROXY_INFO IDMGraphAnnotation_ProxyInfo =
    {
    &Object_StubDesc,
    DMGraph__MIDL_ProcFormatString.Format,
    &IDMGraphAnnotation_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IDMGraphAnnotation_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    DMGraph__MIDL_ProcFormatString.Format,
    &IDMGraphAnnotation_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(21) _IDMGraphAnnotationProxyVtbl = 
{
    &IDMGraphAnnotation_ProxyInfo,
    &IID_IDMGraphAnnotation,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IDMGraphAnnotation::get_LabelCaption */ ,
    (void *) (INT_PTR) -1 /* IDMGraphAnnotation::put_LabelCaption */ ,
    (void *) (INT_PTR) -1 /* IDMGraphAnnotation::get_LabelX */ ,
    (void *) (INT_PTR) -1 /* IDMGraphAnnotation::put_LabelX */ ,
    (void *) (INT_PTR) -1 /* IDMGraphAnnotation::get_LabelY */ ,
    (void *) (INT_PTR) -1 /* IDMGraphAnnotation::put_LabelY */ ,
    (void *) (INT_PTR) -1 /* IDMGraphAnnotation::get_LabelColor */ ,
    (void *) (INT_PTR) -1 /* IDMGraphAnnotation::put_LabelColor */ ,
    (void *) (INT_PTR) -1 /* IDMGraphAnnotation::get_LabelHorizontal */ ,
    (void *) (INT_PTR) -1 /* IDMGraphAnnotation::put_LabelHorizontal */ ,
    (void *) (INT_PTR) -1 /* IDMGraphAnnotation::get_Visible */ ,
    (void *) (INT_PTR) -1 /* IDMGraphAnnotation::put_Visible */ ,
    (void *) (INT_PTR) -1 /* IDMGraphAnnotation::get_LabelBkColor */ ,
    (void *) (INT_PTR) -1 /* IDMGraphAnnotation::put_LabelBkColor */
};


static const PRPC_STUB_FUNCTION IDMGraphAnnotation_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IDMGraphAnnotationStubVtbl =
{
    &IID_IDMGraphAnnotation,
    &IDMGraphAnnotation_ServerInfo,
    21,
    &IDMGraphAnnotation_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IDMGraphCursor, ver. 0.0,
   GUID={0xfb9b14b4,0x6acc,0x4bda,{0x94,0xa4,0xb7,0x5a,0x3c,0x34,0x96,0x23}} */

#pragma code_seg(".orpc")
static const unsigned short IDMGraphCursor_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    36,
    1044,
    1080,
    1116,
    1152,
    1260,
    1296,
    1188,
    1224,
    1332,
    1368,
    1404,
    1440,
    504,
    540
    };

static const MIDL_STUBLESS_PROXY_INFO IDMGraphCursor_ProxyInfo =
    {
    &Object_StubDesc,
    DMGraph__MIDL_ProcFormatString.Format,
    &IDMGraphCursor_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IDMGraphCursor_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    DMGraph__MIDL_ProcFormatString.Format,
    &IDMGraphCursor_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(23) _IDMGraphCursorProxyVtbl = 
{
    &IDMGraphCursor_ProxyInfo,
    &IID_IDMGraphCursor,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCursor::get_Color */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCursor::put_Color */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCursor::get_X */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCursor::put_X */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCursor::get_Y */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCursor::put_Y */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCursor::get_Style */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCursor::put_Style */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCursor::get_Visible */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCursor::put_Visible */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCursor::get_Mode */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCursor::put_Mode */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCursor::get_Name */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCursor::put_Name */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCursor::get_CursorID */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCursor::put_CursorID */
};


static const PRPC_STUB_FUNCTION IDMGraphCursor_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IDMGraphCursorStubVtbl =
{
    &IID_IDMGraphCursor,
    &IDMGraphCursor_ServerInfo,
    23,
    &IDMGraphCursor_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IDMGraphAxis, ver. 0.0,
   GUID={0xc735264e,0x70bb,0x48bb,{0xac,0x9c,0x93,0x18,0x8a,0x12,0xc2,0x54}} */

#pragma code_seg(".orpc")
static const unsigned short IDMGraphAxis_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    1476,
    1512,
    1548,
    1584,
    1620,
    1656,
    1692,
    1728,
    1764,
    1800,
    360,
    396
    };

static const MIDL_STUBLESS_PROXY_INFO IDMGraphAxis_ProxyInfo =
    {
    &Object_StubDesc,
    DMGraph__MIDL_ProcFormatString.Format,
    &IDMGraphAxis_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IDMGraphAxis_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    DMGraph__MIDL_ProcFormatString.Format,
    &IDMGraphAxis_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(19) _IDMGraphAxisProxyVtbl = 
{
    &IDMGraphAxis_ProxyInfo,
    &IID_IDMGraphAxis,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IDMGraphAxis::get_GridNumber */ ,
    (void *) (INT_PTR) -1 /* IDMGraphAxis::put_GridNumber */ ,
    (void *) (INT_PTR) -1 /* IDMGraphAxis::get_Label */ ,
    (void *) (INT_PTR) -1 /* IDMGraphAxis::put_Label */ ,
    (void *) (INT_PTR) -1 /* IDMGraphAxis::get_Log */ ,
    (void *) (INT_PTR) -1 /* IDMGraphAxis::put_Log */ ,
    (void *) (INT_PTR) -1 /* IDMGraphAxis::get_Time */ ,
    (void *) (INT_PTR) -1 /* IDMGraphAxis::put_Time */ ,
    (void *) (INT_PTR) -1 /* IDMGraphAxis::get_Format */ ,
    (void *) (INT_PTR) -1 /* IDMGraphAxis::put_Format */ ,
    (void *) (INT_PTR) -1 /* IDMGraphAxis::get_ShowGrid */ ,
    (void *) (INT_PTR) -1 /* IDMGraphAxis::put_ShowGrid */
};


static const PRPC_STUB_FUNCTION IDMGraphAxis_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IDMGraphAxisStubVtbl =
{
    &IID_IDMGraphAxis,
    &IDMGraphAxis_ServerInfo,
    19,
    &IDMGraphAxis_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IDMGraphCollection, ver. 0.0,
   GUID={0x022e739f,0x00bb,0x40be,{0xbb,0xba,0x84,0x92,0xe4,0x44,0x34,0x93}} */

#pragma code_seg(".orpc")
static const unsigned short IDMGraphCollection_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    1836,
    72,
    108,
    1878,
    1914
    };

static const MIDL_STUBLESS_PROXY_INFO IDMGraphCollection_ProxyInfo =
    {
    &Object_StubDesc,
    DMGraph__MIDL_ProcFormatString.Format,
    &IDMGraphCollection_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IDMGraphCollection_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    DMGraph__MIDL_ProcFormatString.Format,
    &IDMGraphCollection_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(13) _IDMGraphCollectionProxyVtbl = 
{
    &IDMGraphCollection_ProxyInfo,
    &IID_IDMGraphCollection,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCollection::get_Count */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCollection::get_Item */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCollection::get_Selected */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCollection::put_Selected */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCollection::Add */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCollection::Delete */
};


static const PRPC_STUB_FUNCTION IDMGraphCollection_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IDMGraphCollectionStubVtbl =
{
    &IID_IDMGraphCollection,
    &IDMGraphCollection_ServerInfo,
    13,
    &IDMGraphCollection_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: IDMGraphCtrl, ver. 0.0,
   GUID={0x2DC9C043,0xD85A,0x45B3,{0x88,0x67,0x9E,0x3D,0x03,0x5D,0xEC,0x7F}} */

#pragma code_seg(".orpc")
static const unsigned short IDMGraphCtrl_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    1950,
    1986,
    2022,
    2058,
    2094,
    2130,
    216,
    252,
    2166,
    2202,
    2238,
    2274,
    2310,
    2346,
    2382,
    2418,
    2454,
    2490,
    2526,
    2562,
    2598,
    2634,
    2670,
    2706,
    2742,
    2778,
    2814,
    2850,
    2886,
    2922,
    2958,
    2994,
    3030,
    3066,
    3102,
    3138,
    3174,
    3210,
    3252,
    3306,
    3360,
    3390,
    3444,
    3474,
    3504,
    3534,
    3564,
    3600,
    3630,
    3672,
    3708
    };

static const MIDL_STUBLESS_PROXY_INFO IDMGraphCtrl_ProxyInfo =
    {
    &Object_StubDesc,
    DMGraph__MIDL_ProcFormatString.Format,
    &IDMGraphCtrl_FormatStringOffsetTable[-3],
    0,
    0,
    0
    };


static const MIDL_SERVER_INFO IDMGraphCtrl_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    DMGraph__MIDL_ProcFormatString.Format,
    &IDMGraphCtrl_FormatStringOffsetTable[-3],
    0,
    0,
    0,
    0};
CINTERFACE_PROXY_VTABLE(58) _IDMGraphCtrlProxyVtbl = 
{
    &IDMGraphCtrl_ProxyInfo,
    &IID_IDMGraphCtrl,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::put_Enabled */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::get_Enabled */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::put_Caption */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::get_Caption */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::put_Appearance */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::get_Appearance */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::get_AxisColor */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::put_AxisColor */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::get_GridColor */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::put_GridColor */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::get_LabelColor */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::put_LabelColor */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::get_LabelFont */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::put_LabelFont */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::get_TickFont */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::put_TickFont */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::get_TitleFont */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::put_TitleFont */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::get_IdentFont */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::put_IdentFont */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::get_ElementIdentify */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::put_ElementIdentify */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::get_TrackMode */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::put_TrackMode */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::get_ControlFramePicture */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::put_ControlFramePicture */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::get_PlotAreaPicture */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::put_PlotAreaPicture */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::get_ControlFrameColor */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::put_ControlFrameColor */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::get_PlotAreaColor */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::put_PlotAreaColor */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::get_FrameStyle */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::put_FrameStyle */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::get_Annotations */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::get_Elements */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::get_Cursors */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::get_Axis */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::SetRange */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::GetRange */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::AutoRange */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::GetAutoRange */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::CopyToClipboard */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::PrintGraph */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::ClearGraph */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::ShowProperties */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::SaveAs */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::AboutBox */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::ShiftDisplay */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::GenerateCSVReport */ ,
    (void *) (INT_PTR) -1 /* IDMGraphCtrl::GetElementValueAtCursor */
};


static const PRPC_STUB_FUNCTION IDMGraphCtrl_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2,
    NdrStubCall2
};

CInterfaceStubVtbl _IDMGraphCtrlStubVtbl =
{
    &IID_IDMGraphCtrl,
    &IDMGraphCtrl_ServerInfo,
    58,
    &IDMGraphCtrl_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    DMGraph__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x700022b, /* MIDL Version 7.0.555 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };

const CInterfaceProxyVtbl * const _DMGraph_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IDMGraphCtrlProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IDMGraphAxisProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IDMGraphCollectionProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IDMGraphCursorProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IDMGraphElementProxyVtbl,
    ( CInterfaceProxyVtbl *) &_IDMGraphAnnotationProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _DMGraph_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IDMGraphCtrlStubVtbl,
    ( CInterfaceStubVtbl *) &_IDMGraphAxisStubVtbl,
    ( CInterfaceStubVtbl *) &_IDMGraphCollectionStubVtbl,
    ( CInterfaceStubVtbl *) &_IDMGraphCursorStubVtbl,
    ( CInterfaceStubVtbl *) &_IDMGraphElementStubVtbl,
    ( CInterfaceStubVtbl *) &_IDMGraphAnnotationStubVtbl,
    0
};

PCInterfaceName const _DMGraph_InterfaceNamesList[] = 
{
    "IDMGraphCtrl",
    "IDMGraphAxis",
    "IDMGraphCollection",
    "IDMGraphCursor",
    "IDMGraphElement",
    "IDMGraphAnnotation",
    0
};

const IID *  const _DMGraph_BaseIIDList[] = 
{
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    &IID_IDispatch,
    0
};


#define _DMGraph_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _DMGraph, pIID, n)

int __stdcall _DMGraph_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _DMGraph, 6, 4 )
    IID_BS_LOOKUP_NEXT_TEST( _DMGraph, 2 )
    IID_BS_LOOKUP_NEXT_TEST( _DMGraph, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _DMGraph, 6, *pIndex )
    
}

const ExtendedProxyFileInfo DMGraph_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _DMGraph_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _DMGraph_StubVtblList,
    (const PCInterfaceName * ) & _DMGraph_InterfaceNamesList,
    (const IID ** ) & _DMGraph_BaseIIDList,
    & _DMGraph_IID_Lookup, 
    6,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#pragma optimize("", on )
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* !defined(_M_IA64) && !defined(_M_AMD64)*/

