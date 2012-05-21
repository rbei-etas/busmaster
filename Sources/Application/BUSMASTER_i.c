

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 7.00.0500 */
/* at Tue Aug 16 23:18:30 2011
 */
/* Compiler settings for .\BUSMASTER.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data
    VC __declspec() decoration level:
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C" {
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

    MIDL_DEFINE_GUID(IID, LIBID_CAN_MonitorApp,0xC3F9B41A,0xD3AD,0x4b36,0xBA,0x65,0xC5,0xC2,0xF1,0xA3,0x14,0xD9);


    MIDL_DEFINE_GUID(IID, IID_IApplication,0xB3DBF7E2,0x93DD,0x4c0c,0xA2,0x37,0x0E,0x8E,0x46,0xD3,0x54,0xC6);


    MIDL_DEFINE_GUID(CLSID, CLSID_Application,0x92D435C1,0xA552,0x4435,0xAD,0x1E,0x46,0x8B,0x4C,0x17,0xBD,0xC7);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



