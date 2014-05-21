

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Mon Oct 28 12:00:37 2013
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


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IDMGraphElement,0x4ca52ab5,0xb6ce,0x41af,0x9a,0x09,0x28,0x4d,0x00,0x71,0x4f,0x8e);


MIDL_DEFINE_GUID(IID, IID_IDMGraphAnnotation,0x7d965dbb,0x74cd,0x42d2,0xa9,0xc6,0x53,0xed,0x80,0x18,0x8b,0x35);


MIDL_DEFINE_GUID(IID, IID_IDMGraphCursor,0xfb9b14b4,0x6acc,0x4bda,0x94,0xa4,0xb7,0x5a,0x3c,0x34,0x96,0x23);


MIDL_DEFINE_GUID(IID, IID_IDMGraphAxis,0xc735264e,0x70bb,0x48bb,0xac,0x9c,0x93,0x18,0x8a,0x12,0xc2,0x54);


MIDL_DEFINE_GUID(IID, IID_IDMGraphCollection,0x022e739f,0x00bb,0x40be,0xbb,0xba,0x84,0x92,0xe4,0x44,0x34,0x93);


MIDL_DEFINE_GUID(IID, IID_IDMGraphCtrl,0x2DC9C043,0xD85A,0x45B3,0x88,0x67,0x9E,0x3D,0x03,0x5D,0xEC,0x7F);


MIDL_DEFINE_GUID(IID, LIBID_DMGraphLib,0xa5e18713,0x3023,0x4fd2,0x8d,0xcb,0xe8,0x2b,0x6f,0x33,0x0f,0x48);


MIDL_DEFINE_GUID(IID, DIID__IDMGraphCtrlEvents,0x9392E24C,0x430F,0x467D,0x99,0xA2,0x69,0xFA,0x98,0xCB,0xCB,0x78);


MIDL_DEFINE_GUID(CLSID, CLSID_DMGraphCtrl,0xAAF89A51,0x7FC0,0x43B0,0x9F,0x81,0xFF,0xEF,0xF6,0xA8,0xDB,0x43);


MIDL_DEFINE_GUID(CLSID, CLSID_ElementPropPage,0x14A14BAD,0x2949,0x4186,0x95,0x29,0xBB,0xB2,0xCC,0x7F,0x96,0x99);


MIDL_DEFINE_GUID(CLSID, CLSID_AnnoPropPage,0x273DB50A,0xE76D,0x4B95,0x8E,0x1A,0x09,0x93,0xE1,0x4F,0xFE,0xF0);


MIDL_DEFINE_GUID(CLSID, CLSID_CursorPropPage,0x5DF8A597,0xC334,0x4ED5,0x8D,0x45,0x88,0x84,0xD5,0x06,0x04,0x4D);


MIDL_DEFINE_GUID(CLSID, CLSID_FormatPropPage,0x9EE3E072,0x2B3F,0x47AB,0x86,0x12,0xB6,0xED,0x7F,0x0C,0xDA,0xE6);


MIDL_DEFINE_GUID(CLSID, CLSID_DMGraphPropPage,0xcb32b59d,0x7aaa,0x44b5,0xac,0xdf,0xa6,0x4b,0x93,0x80,0xe4,0x8c);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



