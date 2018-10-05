/*****************************************************************************
 HMS Technology Center Ravensburg GmbH
******************************************************************************

 File    : STDTYPE.H
 Summary : Common constants, basic data types and useful macros.

 Date    : 2001-05-24
 Author  : Hartmut Heim

 Compiler: MSVC, Borland C, LabWindows/CVI

******************************************************************************
 all rights reserved
*****************************************************************************/

#ifndef _STDTYPE_H_
#define _STDTYPE_H_

/*****************************************************************************
 * compiler definitions
 ****************************************************************************/

#ifdef _COMPILER_
  #undef _COMPILER_
#endif

#ifdef _MSC_VER
  #define MICROSOFT_C _MSC_VER
  #define _COMPILER_ MICROSOFT_C
#endif

#ifdef __BORLANDC__
  #define BORLAND_C __BORLANDC__
  #define _COMPILER_ BORLAND_C
#endif

#ifdef _CVI_
  #define NILABWIN_C _CVI_
  #define _COMPILER_ NILABWIN_C
#endif

#ifndef _COMPILER_
  #error "Compiler not supported"
#endif


/*****************************************************************************
 * machine specific defines
 ****************************************************************************/

#define _LITTLE_ENDIAN  1
#define _BIG_ENDIAN     2
#define _BI_ENDIAN      3

#if defined(MICROSOFT_C)
  #if !defined(_M_IX86_) && defined(_M_IX86)
    #define _M_BITS 32
    #define _M_IX86_ _M_IX86
    #define _M_BYTE_ORDER _LITTLE_ENDIAN
  #elif !defined(_M_MIPS_) && defined(_M_MRX000)
    #define _M_BITS 32
    #define _M_MIPS_ _M_MRX000
    #define _M_BYTE_ORDER _BI_ENDIAN
  #elif !defined(_M_ALPHA_) && defined(_M_ALPHA)
    #define _M_BITS 32
    #define _M_ALPHA_ _M_ALPHA
    #define _M_BYTE_ORDER _LITTLE_ENDIAN
  #elif !defined(_M_PPC_) && defined(_M_PPC)
    #define _M_BITS 32
    #define _M_PPC_ _M_PPC
    #define _M_BYTE_ORDER _BI_ENDIAN
  #elif !defined(_M_M68K_) && defined(_M_M68K)
    #define _M_BITS 32
    #define _M_M68K_ _M_M68K
    #define _M_BYTE_ORDER _BIG_ENDIAN
  #elif !defined(_M_MPPC_) && defined(_M_MPPC)
    #define _M_BITS 32
    #define _M_MPPC_ _M_MPPC
    #define _M_BYTE_ORDER _BI_ENDIAN
  #elif !defined(_M_IA64_) && defined(_M_IA64)
    #define _M_BITS 64
    #define _M_IA64_ _M_IA64
    #define _M_BYTE_ORDER _BI_ENDIAN
  #elif !defined(_M_X64_) && defined(_M_X64)
    #define _M_BITS 64
    #define _M_X64_ _M_X64
    #define _M_BYTE_ORDER _LITTLE_ENDIAN
  #elif !defined(_M_AMD64_) && defined(_M_AMD64)
    #define _M_BITS 64
    #define _M_AMD64_ _M_AMD64
    #define _M_BYTE_ORDER _LITTLE_ENDIAN
  #endif
#elif defined(BORLAND_C)
  #if !defined(_M_IX86_) && defined(_M_IX86)
    #define _M_BITS 32
    #define _M_IX86_ _M_IX86
    #define _M_BYTE_ORDER _LITTLE_ENDIAN
  #endif
#elif defined(NILABWIN_C)
  #if !defined(_M_IX86_) && defined(_M_IX86)
    #define _M_BITS 32
    #define _M_IX86_ _M_IX86
    #define _M_BYTE_ORDER _LITTLE_ENDIAN
  #endif
#endif

#ifndef _M_BITS
#error "Target architecture not defined"
#endif


/*****************************************************************************
 * fundamental data types
 ****************************************************************************/

//------------------------------------------------------------------------
// 8 bit integer
//------------------------------------------------------------------------

#undef __INT8   // default
#undef __SINT8  // signed
#undef __UINT8  // unsigned

//
// Microsoft C/C++ compiler
//
#ifdef MICROSOFT_C
  #if (MICROSOFT_C < 1300)
  #define __INT8              char
  #else
  #define __INT8            __int8
  #endif
  #define __SINT8  signed   __INT8
  #define __UINT8  unsigned __INT8
#endif

//
// Borland C/C++ compiler
//
#ifdef BORLAND_C
  #define __INT8            __int8
  #define __SINT8  signed   __INT8
  #define __UINT8  unsigned __INT8
#endif

//
// National Instruments LabWindows/CVI compiler
//
#ifdef NILABWIN_C
  #define __INT8              char
  #define __SINT8  signed   __INT8
  #define __UINT8  unsigned __INT8
#endif


//------------------------------------------------------------------------
// 16 bit integer
//------------------------------------------------------------------------

#undef __INT16  // default
#undef __SINT16 // signed
#undef __UINT16 // unsigned

//
// Microsoft C/C++ compiler
//
#ifdef MICROSOFT_C
  #if (MICROSOFT_C < 1300)
  #define __INT16             short
  #else
  #define __INT16           __int16
  #endif
  #define __SINT16 signed   __INT16
  #define __UINT16 unsigned __INT16
#endif

//
// Borland C/C++ compiler
//
#ifdef BORLAND_C
  #define __INT16           __int16
  #define __SINT16 signed   __INT16
  #define __UINT16 unsigned __INT16
#endif

//
// National Instruments LabWindows/CVI compiler
//
#ifdef NILABWIN_C
  #define __INT16             short
  #define __SINT16 signed   __INT16
  #define __UINT16 unsigned __INT16
#endif


//------------------------------------------------------------------------
// 32 bit integer
//------------------------------------------------------------------------

#undef __INT32  // default
#undef __SINT32 // signed
#undef __UINT32 // unsigned

//
// Microsoft C/C++ compiler
//
#ifdef MICROSOFT_C
  #if (MICROSOFT_C < 1300)
  #define __INT32             long
  #else
  #define __INT32           __int32
  #endif
  #define __SINT32 signed   __INT32
  #define __UINT32 unsigned __INT32
#endif

//
// Borland C/C++ compiler
//
#ifdef BORLAND_C
  #define __INT32           __int32
  #define __SINT32 signed   __INT32
  #define __UINT32 unsigned __INT32
#endif

//
// National Instruments LabWindows/CVI compiler
//
#ifdef NILABWIN_C
  #define __INT32             long
  #define __SINT32 signed   __INT32
  #define __UINT32 unsigned __INT32
#endif


//------------------------------------------------------------------------
// 64 bit integer
//------------------------------------------------------------------------

#undef __INT64  // default
#undef __SINT64 // signed
#undef __UINT64 // unsigned

//
// Microsoft C/C++ compiler
//
#ifdef MICROSOFT_C
  #if (MICROSOFT_C < 1300)
  #define __INT64             double
  #define __SINT64            double
  #define __UINT64            double
  #else
  #define __INT64           __int64
  #define __SINT64 signed   __INT64
  #define __UINT64 unsigned __INT64
  #endif
#endif

//
// Borland C/C++ compiler
//
#ifdef BORLAND_C
  #define __INT64           __int64
  #define __SINT64 signed   __INT64
  #define __UINT64 unsigned __INT64
#endif

//
// National Instruments LabWindows/CVI compiler
//
#ifdef NILABWIN_C
  #if (NILABWIN_C <= 600)
  #define __INT64             double
  #define __SINT64            double
  #define __UINT64            double
  #else
  #define __INT64           __int64
  #define __SINT64 signed   __INT64
  #define __UINT64 unsigned __INT64
  #endif
#endif


/*****************************************************************************
 * alignment definitions
 ****************************************************************************/

//
// default alignment
//
#ifndef ALIGN
  #if (MICROSOFT_C >= 1300)
    #if (_M_BITS == 32)
      #define ALIGN __declspec(align(4))
    #elif (_M_BITS == 64)
      #define ALIGN __declspec(align(8))
    #else
      #define ALIGN __declspec(align(1))
    #endif
  #else
    #define ALIGN
  #endif
#endif

//
// 8 bit alignment
//
#ifndef ALIGN1
  #if (MICROSOFT_C >= 1300)
    #define ALIGN1 __declspec(align(1))
  #else
    #define ALIGN1
  #endif
#endif

//
// 16 bit alignment
//
#ifndef ALIGN2
  #if (MICROSOFT_C >= 1300)
    #define ALIGN2 __declspec(align(2))
  #else
    #define ALIGN2
  #endif
#endif

//
// 32 bit alignment
//
#ifndef ALIGN4
  #if (MICROSOFT_C >= 1300)
    #define ALIGN4 __declspec(align(4))
  #else
    #define ALIGN4
  #endif
#endif

//
// 64 bit alignment
//
#ifndef ALIGN8
  #if (MICROSOFT_C >= 1300)
    #define ALIGN8 __declspec(align(8))
  #else
    #define ALIGN8
  #endif
#endif

//
// 128 bit alignment
//
#ifndef ALIGN16
  #if (MICROSOFT_C >= 1300)
    #define ALIGN16 __declspec(align(16))
  #else
    #define ALIGN16
  #endif
#endif

//
// 256 bit alignment
//
#ifndef ALIGN32
  #if (MICROSOFT_C >= 1300)
    #define ALIGN32 __declspec(align(32))
  #else
    #define ALIGN32
  #endif
#endif


/*****************************************************************************
 * common definitions
 ****************************************************************************/

#ifndef EXTERN_C
  #ifdef __cplusplus
    #define EXTERN_C extern "C"
  #else
    #define EXTERN_C extern
  #endif
#endif


#ifndef DECLSPEC_ALLOC
  #if (MICROSOFT_C >= 1100)
    #define DECLSPEC_ALLOC(x) __declspec(allocate(x))
  #else
    #define DECLSPEC_ALLOC(x)
  #endif
#endif


#ifndef DECLSPEC_NAKED
  #if ((MICROSOFT_C >= 1100) || (BORLAND_C >= 0x550)) && defined (_M_IX86)
    #define DECLSPEC_NAKED __declspec(naked)
  #else
    #define DECLSPEC_NAKED
  #endif
#endif

#define NAKED DECLSPEC_NAKED


#ifndef DECLSPEC_SELECTANY
  #if (MICROSOFT_C >= 1100) || (BORLAND_C >= 0x550)
    #define DECLSPEC_SELECTANY __declspec(selectany)
  #else
    #define DECLSPEC_SELECTANY
  #endif
#endif

#define SELECTANY DECLSPEC_SELECTANY


#ifndef DECLSPEC_NOALIAS
  #if (_MSC_VER >= 1400)
    #define DECLSPEC_NOALIAS __declspec(noalias)
  #else
    #define DECLSPEC_NOALIAS
  #endif
#endif

#define NOALIAS DECLSPEC_NOALIAS


#ifndef DECLSPEC_RESTRICT
  #if (_MSC_VER >= 1400)
    #define DECLSPEC_RESTRICT __declspec(restrict)
  #else
    #define DECLSPEC_RESTRICT
  #endif
#endif

#define RESTRICT DECLSPEC_RESTRICT


/*****************************************************************************
 * calling conventions
 ****************************************************************************/

#ifndef SCALL
#define SCALL __stdcall
#endif

#ifndef CCALL
#define CCALL __cdecl
#endif

#ifndef FCALL
#define FCALL __fastcall
#endif

#ifndef FORCEINLINE
#if (MICROSOFT_C >= 1200)
#define FORCEINLINE __forceinline
#elif defined (MICROSOFT_C) 
#define FORCEINLINE __inline
#else
#define FORCEINLINE inline
#endif
#endif


/*****************************************************************************
 * common constants
 ****************************************************************************/

#ifndef IN
#define IN  // in parameter
#endif

#ifndef OUT
#define OUT // out parameter
#endif

#ifndef OPT
#define OPT // optional parameter
#endif

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
  #ifdef __cplusplus
    #define NULL 0
  #else
    #define NULL ((void*)0)
  #endif
#endif


/*****************************************************************************
 * basic data types
 ****************************************************************************/

//
// generic pointer types
//

#ifndef VOID
#define VOID             void
#endif

typedef VOID*            PVOID;
typedef PVOID*           PPVOID;

typedef void             (CCALL *PVFV)(void);
typedef void             (CCALL *PCCALLVFV)(void);
typedef void             (SCALL *PSCALLVFV)(void);

typedef int              (CCALL *PIFV)(void);
typedef int              (CCALL *PCCALLIFV)(void);
typedef int              (SCALL *PSCALLIFV)(void);


//
// boolean types
//

#if !defined(__cplusplus)
typedef int              bool;
#endif

typedef int              BOOL;
typedef BOOL*            PBOOL;

typedef __INT8           BOOL8;
typedef BOOL8*           PBOOL8;

typedef __INT16          BOOL16;
typedef BOOL16*          PBOOL16;

typedef __INT32          BOOL32;
typedef BOOL32*          PBOOL32;


//
// generic numeric types
//

typedef int              INT;
typedef INT*             PINT;

typedef unsigned int     UINT;
typedef UINT*            PUINT;

#if !defined(_SIZE_T_DEFINED)
#define _SIZE_T_DEFINED
#if defined(_WIN64)
typedef __UINT64         size_t;
#else
typedef __UINT32         size_t;
#endif
#endif

//
// 8-bit signed types
//

typedef char             CHAR;
typedef CHAR*            PCHAR;

typedef __SINT8          INT8;
typedef INT8*            PINT8;

//
// 8-bit unsigned types
//

typedef unsigned char    UCHAR;
typedef UCHAR*           PUCHAR;

typedef __UINT8          UINT8;
typedef UINT8*           PUINT8;

typedef UCHAR            BYTE;
typedef BYTE*            PBYTE;

//
// 16-bit signed types
//

typedef short            SHORT;
typedef SHORT*           PSHORT;

typedef __SINT16         INT16;
typedef INT16*           PINT16;

//
// 16-bit unsigned types
//

#if !defined(_WCHAR_T_DEFINED) && !defined(_NATIVE_WCHAR_T_DEFINED)
#define _WCHAR_T_DEFINED
#define _NATIVE_WCHAR_T_DEFINED
typedef unsigned short   wchar_t;
#endif

typedef wchar_t          WCHAR;
typedef WCHAR           *PWCHAR;


typedef unsigned short   USHORT;
typedef USHORT*          PUSHORT;

typedef __UINT16         UINT16;
typedef UINT16*          PUINT16;

typedef USHORT           WORD;
typedef WORD*            PWORD;

//
// 32-bit signed types
//

typedef long             LONG;
typedef LONG*            PLONG;

typedef __SINT32         INT32;
typedef INT32*           PINT32;

//
// 32-bit unsigned types
//

typedef unsigned long    ULONG;
typedef ULONG*           PULONG;

typedef __UINT32         UINT32;
typedef UINT32*          PUINT32;

typedef ULONG            DWORD;
typedef DWORD*           PDWORD;

//
// 64-bit signed types
//

typedef __SINT64         INT64;
typedef INT64*           PINT64;

typedef INT64            LONGLONG;
typedef LONGLONG*        PLONGLONG;

//
// 64-bit unsigned types
//
typedef __UINT64         UINT64;
typedef UINT64*          PUINT64;

typedef UINT64           ULONGLONG;
typedef ULONGLONG*       PULONGLONG;

typedef UINT64           QWORD;
typedef QWORD*           PQWORD;


//
// 32-bit floating point types
//

typedef float            FLOAT;
typedef FLOAT*           PFLOAT;

//
// 64-bit floating point types
//

typedef double           DOUBLE;
typedef DOUBLE*          PDOUBLE;

//
// 80-bit floating point types
//

typedef long double      EXTENDED;
typedef EXTENDED*        PEXTENDED;

//
// pointer value types
//
// The SPTRVAL is guaranteed to be the same size as a pointer.
// It's size change with pointer size (32/64). It should be
// used anywhere that a pointer is cast to an integer type.
// UPTRVAL is the unsigned variation.
//

#if defined(_WIN64)
typedef INT64            SPTRVAL, *PSPTRVAL;
typedef UINT64           UPTRVAL, *PUPTRVAL;
#else
typedef LONG             SPTRVAL, *PSPTRVAL;
typedef ULONG            UPTRVAL, *PUPTRVAL;
#endif

typedef void*            POINTER;
typedef UINT32           POINTER32;
typedef UINT64           POINTER64;

//
// handle to an object
//

#ifdef STRICT
typedef void*            HANDLE;
#else
typedef PVOID            HANDLE;
#endif

typedef HANDLE*          PHANDLE;

typedef UINT32           HANDLE32;
typedef UINT64           HANDLE64;

//
// Status values are 32 bit values layed out as follows:
//
//  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
// +---+-+-------------------------+-------------------------------+
// |Sev|C|       Facility          |               Code            |
// +---+-+-------------------------+-------------------------------+
//
// where
//
//     Sev - is the severity code
//
//         00 - Success
//         01 - Informational
//         10 - Warning
//         11 - Error
//
//     C - is the Customer code flag
//
//     Facility - is the facility code
//
//     Code - is the facility's status code
//
// HRESULT and NTSTATUS are exchangeable
//
typedef LONG             ERESULT;
typedef ERESULT*         PERESULT;

#ifndef HRESULT
#define HRESULT          LONG
#endif

#ifndef NTSTATUS
#define NTSTATUS         LONG
#endif

//
// 128-bit globally unique IDs
//
#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct _GUID
{
  DWORD Data1;
  WORD  Data2;
  WORD  Data3;
  BYTE  Data4[8];
} GUID;
#endif // GUID_DEFINED

typedef GUID*            PGUID;
typedef const GUID*      PCGUID;

typedef GUID             CLSID;
typedef CLSID*           PCLSID;
typedef const CLSID*     PCCLSID;

#ifndef _REFGUID_DEFINED
#define _REFGUID_DEFINED
#ifdef __cplusplus
#define REFGUID const GUID &
#else
#define REFGUID const GUID *
#endif
#endif

#ifndef DEFINE_GUID
#ifdef INITGUID
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
  EXTERN_C const GUID SELECTANY name={l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}
#else
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
  EXTERN_C const GUID name
#endif // INITGUID
#endif // DEFINE_GUID


/*****************************************************************************
 * common macros
 ****************************************************************************/

//------------------------------------------------------------------------
// Macro:
//  MAKEWORD
//
// Description:
//  This macro creates a 16-bit value by concatenating the specified
//  8-bit values. 
//
// Arguments:
//  l -> Specifies the low-order 8 bits of the new value. 
//  h -> Specifies the high-order 8 bits of the new value. 
//
// Results:
//  Returns the new 16-bit value.
//------------------------------------------------------------------------
#ifndef MAKEWORD
#define MAKEWORD(l,h) ((WORD)(((BYTE)(l))|(((WORD)((BYTE)(h)))<<8)))
#endif

//------------------------------------------------------------------------
// Macro:
//  MAKELONG
//
// Description:
//  This macro creates a 32-bit value by concatenating the specified
//  16-bit values. 
//
// Arguments:
//  l -> Specifies the low-order 16 bits of the new value. 
//  h -> Specifies the high-order 16 bits of the new value. 
//
// Results:
//  Returns the new 32-bit value.
//------------------------------------------------------------------------
#ifndef MAKELONG
#define MAKELONG(l,h) ((LONG)(((WORD)(l))|(((DWORD)((WORD)(h)))<<16)))
#endif

//------------------------------------------------------------------------
// Macro:
//  MAKEDWORD
//
// Description:
//  This macro creates a 32-bit value by concatenating the specified
//  16-bit values. 
//
// Arguments:
//  l -> Specifies the low-order 16 bits of the new value. 
//  h -> Specifies the high-order 16 bits of the new value. 
//
// Results:
//  Returns the new 32-bit value.
//------------------------------------------------------------------------
#ifndef MAKEDWORD
#define MAKEDWORD(l,h) ((DWORD)(((WORD)(l))|(((DWORD)((WORD)(h)))<<16)))
#endif

//------------------------------------------------------------------------
// Macro:
//  MAKEQWORD
//
// Description:
//  This macro creates a 64-bit value by concatenating the specified
//  32-bit values.
//
// Arguments:
//  l -> Specifies the low-order 32 bits of the new value.
//  h -> Specifies the high-order 32 bits of the new value.
//
// Results:
//  Returns the new 32-bit value.
//------------------------------------------------------------------------
#ifndef MAKEQWORD
#define MAKEQWORD(l,h) ((QWORD)(((DWORD)(l))|(((QWORD)((DWORD)(h)))<<32)))
#endif

//------------------------------------------------------------------------
// Macro:
//  PTROFFSET
//
// Description:
//  This macro computes the byte offset of a pointer relative
//  to a base pointer.
//
// Arguments:
//  p -> Pointer
//  b -> Base pointer
//
// Results:
//  Byte offset of <p> relative to <b>.
//------------------------------------------------------------------------
#ifndef PTROFFSET
#define PTROFFSET(p,b) ((UPTRVAL)(p) - (UPTRVAL)(b))
#endif

#ifndef PTRBYTEDIFF
#define PTRBYTEDIFF PTROFFSET
#endif

//------------------------------------------------------------------------
// Macro:
//  PTRADDBYTE
//
// Description:
//  This macro adds an byte offset to the specified base pointer.
//
// Arguments:
//  p -> Pointer
//  i -> byte offset to add
//
// Results:
//  The macro returns (PUINT8) p + i.
//------------------------------------------------------------------------
#ifndef PTRADDBYTE
#define PTRADDBYTE(p,i) (PVOID) ((UPTRVAL)(p) + (UPTRVAL)(i))
#endif

//------------------------------------------------------------------------
// Macro:
//  PTRSUBBYTE
//
// Description:
//  This macro subtracts an byte offset from a specified base pointer.
//
// Arguments:
//  p -> Pointer
//  i -> byte offset to subtract
//
// Results:
//  The macro returns (PUINT8) p - i.
//------------------------------------------------------------------------
#ifndef PTRSUBBYTE
#define PTRSUBBYTE(p,i) (PVOID) ((UPTRVAL)(p) - (UPTRVAL)(i))
#endif

//------------------------------------------------------------------------
// Macro:
//  FIELDOFFSET
//
// Description:
//  This macro computes the offset of a member variable within the
//  specified data structure.
//
// Arguments:
//  type  -> type of the data structure
//  field -> name of the member variable
//
// Results:
//  Returns the offset of the member variable within the specified
//  data structure.
//------------------------------------------------------------------------
#ifndef FIELDOFFSET
#define FIELDOFFSET(type,field) ((UPTRVAL)(&((type*)0)->field))
#endif

//------------------------------------------------------------------------
// Macro:
//  FIELDSIZE
//
// Description:
//  This macro computes the size in bytes of a member variable within
//  the specified data structure.
//
// Arguments:
//  type  -> type of the data structure
//  field -> name of the member variable
//
// Results:
//  Returns the size in bytes of the member variable within the specified
//  data structure.
//------------------------------------------------------------------------
#ifndef FIELDSIZE
#define FIELDSIZE(type, field) sizeof(((type*)0)->field)
#endif

//------------------------------------------------------------------------
// Macro:
//  HOSTOBJECT
//
// Description:
//  This macro retrieve a reference to the host object.
//
// Arguments:
//  t -> Type of the host object
//  m -> Member variable which represents the inner object
//
// Results:
//  Reference to the host object.
//------------------------------------------------------------------------
#ifdef __cplusplus
#ifndef HOSTOBJECT
#define HOSTOBJECT(t,m) ((t&) *((PCHAR) this - (UPTRVAL)(&((t*)0)->m)))
#endif
#endif

//------------------------------------------------------------------------
// Macro:
//  OUTEROBJECT
//
// Description:
//  This macro retrieve a pointer to the outer object.
//
// Arguments:
//  i -> Pointer to the inner object
//  m -> Member variable which represents the inner object
//  t -> Type of the outer object
//
// Results:
//  Pointer to the outer object.
//------------------------------------------------------------------------
#ifndef OUTEROBJECT
#define OUTEROBJECT(i,m,t) ((t*) ((PCHAR)(i) - (UPTRVAL)(&((t*)0)->m)))
#endif

//------------------------------------------------------------------------
// Macro:
//  NUMELEM
//
// Description:
//  This macro retrieves the number of elements within the specified array.
//
// Arguments:
//  a -> array for which to retrieve the number of elements
//
// Results:
//  Returns the number of elements within the specified array.
//------------------------------------------------------------------------
#ifndef NUMELEM
#define NUMELEM(a) (sizeof(a) / sizeof(a[0]))
#endif

//------------------------------------------------------------------------
// Macro:
//  min
//
// Description:
//  This macro retrieves the smaller of two values.
//
// Arguments:
//  a -> value 1 to be compared
//  b -> value 2 to be compared
//
// Results:
//  Returns the smaller of the two arguments.
//------------------------------------------------------------------------
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

//------------------------------------------------------------------------
// Macro:
//  max
//
// Description:
//  This macro retrieves the larger of two values.
//
// Arguments:
//  a -> value 1 to be compared
//  b -> value 2 to be compared
//
// Results:
//  Returns the larger of the two arguments.
//------------------------------------------------------------------------
#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

//------------------------------------------------------------------------
// Macro:
//  LONIBBLE
//
// Description:
//  This macro retrieves the lower 4 bits of the given 8-bit value.
//
// Arguments:
//  b -> 8-bit value
//
// Results:
//  Low-order nibble (bit 0..3) of the specified value.
//------------------------------------------------------------------------
#ifndef LONIBBLE
#define LONIBBLE(b) ( (BYTE) (b) & 0x0F )
#endif

//------------------------------------------------------------------------
// Macro:
//  HINIBBLE
//
// Description:
//  This macro retrieves the upper 4 bits of the given 8-bit value.
//
// Arguments:
//  b -> 8-bit value
//
// Results:
//  High-order nibble (bit 4..7) of the specified value.
//------------------------------------------------------------------------
#ifndef HINIBBLE
#define HINIBBLE(b) ( (BYTE) (b) >> 4 )
#endif

//------------------------------------------------------------------------
// Macro:
//  LOBYTE
//
// Description:
//  This macro retrieves the lower 8 bits of the given 16-bit value.
//
// Arguments:
//  w -> 16-bit value
//
// Results:
//  Low-order byte (bit 0..7) of the specified value.
//------------------------------------------------------------------------
#ifndef LOBYTE
#define LOBYTE(w) ( (BYTE) (w) )
#endif

//------------------------------------------------------------------------
// Macro:
//  HIBYTE
//
// Description:
//  This macro retrieves the upper 8 bits of the given 16-bit value.
//
// Arguments:
//  w -> 16-bit value
//
// Results:
//  High-order byte (bit 8..15) of the specified value.
//------------------------------------------------------------------------
#ifndef HIBYTE
#define HIBYTE(w) ( (BYTE) ((WORD)(w) >> 8) )
#endif

//------------------------------------------------------------------------
// Macro:
//  LOWORD
//
// Description:
//  This macro retrieves the lower 16 bits of the given 32-bit value.
//
// Arguments:
//  l -> 32-bit value
//
// Results:
//  Low-order word (bit 0..15) of the specified value.
//------------------------------------------------------------------------
#ifndef LOWORD
#define LOWORD(l) ( (WORD) (l) )
#endif

//------------------------------------------------------------------------
// Macro:
//  HIWORD
//
// Description:
//  This macro retrieves the upper 16 bits of the given 32-bit value.
//
// Arguments:
//  l -> 32-bit value
//
// Results:
//  High-order word (bit 8..15) of the specified value.
//------------------------------------------------------------------------
#ifndef HIWORD
#define HIWORD(l) ( (WORD) ((DWORD)(l) >> 16) )
#endif

//------------------------------------------------------------------------
// Macro:
//  LODWORD
//
// Description:
//  This macro retrieves the lower 32 bits of the given 64-bit value.
//
// Arguments:
//  l -> 64-bit value
//
// Results:
//  Low-order dword (bit 0..31) of the specified value.
//------------------------------------------------------------------------
#ifndef LODWORD
#define LODWORD(l) ( (DWORD) (l) )
#endif

//------------------------------------------------------------------------
// Macro:
//  HIDWORD
//
// Description:
//  This macro retrieves the upper 32 bits of the given 64-bit value.
//
// Arguments:
//  l -> 64-bit value
//
// Results:
//  High-order dword (bit 32..63) of the specified value.
//------------------------------------------------------------------------
#ifndef HIDWORD
#define HIDWORD(l) ( (DWORD) ((QWORD)(l) >> 32) )
#endif

//------------------------------------------------------------------------
// Macro:
//  SETPVAR
//
// Description:
//  This macro stores a value in the specified variable.
//
// Arguments:
//  p -> points to the variable
//  v -> value to store in the variable
//
// Results:
//  none
//------------------------------------------------------------------------
#ifndef SETPVAR
#define SETPVAR(p,v) ( (p) ? (*(p) = (v)) : 0 )
#endif

//------------------------------------------------------------------------
// Macro:
//  BYTE_CAST, WORD_CAST, DWORD_CAST, QWORD_CAST
//
// Description:
//  This macro casts the specified variable to a unsigned 8-, 16-,
//  32- or 64- bit value.
//
// Arguments:
//  v -> variable to be casted to a 8-, 16-, 32 or 64- bit value.
//
// Results:
//  none
//------------------------------------------------------------------------
#ifndef BYTE_CAST
#define BYTE_CAST(v) *((PBYTE)(&v))
#endif
#ifndef WORD_CAST
#define WORD_CAST(v) *((PWORD)(&v))
#endif
#ifndef DWORD_CAST
#define DWORD_CAST(v) *((PDWORD)(&v))
#endif
#ifndef QWORD_CAST
#define QWORD_CAST(v) *((PQWORD)(&v))
#endif

//------------------------------------------------------------------------
// Macro:
//  XCHG
//
// Description:
//  This macro exchange two variables.
//
// Arguments:
//  type -> data type of the variables
//  a    -> variable 1
//  b    -> variable 2
//
// Results:
//  none
//------------------------------------------------------------------------
#ifndef XCHG
#define XCHG(type,a,b) {type x=(type)(a);(type)(a)=(type)(b);(type)(b)=x;}
#endif

//------------------------------------------------------------------------
// Macro:
//  XCHG8, XCHG16, XCHG32, XCHG64
//
// Description:
//  This macros exchange two variables of type BYTE, WORD or DWORD.
//
// Arguments:
//  a -> variable 1
//  b -> variable 2
//
// Results:
//  none
//------------------------------------------------------------------------
#ifndef XCHG8
#define XCHG8(a,b) {BYTE x=(BYTE)(a);(BYTE)(a)=(BYTE)(b);(BYTE)(b)=x;}
#endif
#ifndef XCHG16
#define XCHG16(a,b) {WORD x=(WORD)(a);(WORD)(a)=(WORD)(b);(WORD)(b)=x;}
#endif
#ifndef XCHG32
#define XCHG32(a,b) {DWORD x=(DWORD)(a);(DWORD)(a)=(DWORD)(b);(DWORD)(b)=x;}
#endif
#ifndef XCHG64
#define XCHG64(a,b) {QWORD x=(QWORD)(a);(QWORD)(a)=(QWORD)(b);(QWORD)(b)=x;}
#endif

//------------------------------------------------------------------------
// Macro:
//  ROUND_UP
//
// Description:
//  This macro returns a number rounded up to the nearest multiple
//  of the specified base value.
//
// Arguments:
//  v -> value to round up
//  b -> unsigned base value
//
// Results:
//  Rounded value.
//------------------------------------------------------------------------
#ifndef ROUND_UP
#define ROUND_UP(v,b) ( ((v) + (b) - 1) &~ ((b) - 1) )
#endif

//------------------------------------------------------------------------
// Macro:
//  RNDUP8, RNDUP16, RNDUP32, RNDUP64
//
// Description:
//  This macro returns a BYTE, WORD, DWORD or QWORD number rounded up
//  to the nearest multiple of the specified base value.
//
// Arguments:
//  v -> value to round up
//  b -> unsigned base value
//
// Results:
//  Rounded value.
//------------------------------------------------------------------------
#ifndef RNDUP8
#define RNDUP8(v,b) ( ((BYTE)(v) + (b) - 1) &~ ((b) - 1) )
#endif
#ifndef RNDUP16
#define RNDUP16(v,b) ( ((WORD)(v) + (b) - 1) &~ ((b) - 1) )
#endif
#ifndef RNDUP32
#define RNDUP32(v,b) ( ((DWORD)(v) + (b) - 1) &~ ((b) - 1) )
#endif
#ifndef RNDUP64
#define RNDUP64(v,b) ( ((QWORD)(v) + (b) - 1) &~ ((b) - 1) )
#endif

//------------------------------------------------------------------------
// Macro:
//  ROUND_DN
//
// Description:
//  This macro returns a number rounded down to the nearest
//  multiple of the specified base value.
//
// Arguments:
//  v -> value to round down
//  b -> unsigned base value
//
// Results:
//  Rounded value.
//------------------------------------------------------------------------
#ifndef ROUND_DN
#define ROUND_DN(v,b) ( (v) &~ ((b) - 1) )
#endif

//------------------------------------------------------------------------
// Macro:
//  RNDDN8, RNDDN16, RNDDN32, RNDDN64
//
// Description:
//  This macro returns a BYTE, WORD DWORD or QWORD number rounded
//  down to the nearest multiple of the specified base value.
//
// Arguments:
//  v -> value to round down
//  b -> unsigned base value
//
// Results:
//  Rounded value.
//------------------------------------------------------------------------
#ifndef RNDDN8
#define RNDDN8(v,b) ( (BYTE)(v) &~ ((b) - 1) )
#endif
#ifndef RNDDN16
#define RNDDN16(v,b) ( (WORD)(v) &~ ((b) - 1) )
#endif
#ifndef RNDDN32
#define RNDDN32(v,b) ( (DWORD)(v) &~ ((b) - 1) )
#endif
#ifndef RNDDN64
#define RNDDN64(v,b) ( (QWORD)(v) &~ ((b) - 1) )
#endif

#endif //_STDTYPE_H_
