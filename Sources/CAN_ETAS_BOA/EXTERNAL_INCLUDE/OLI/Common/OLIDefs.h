#ifndef __OLI_DEFS_H
#define __OLI_DEFS_H

/// @todo update doxygen comments

#if defined(OLL_DLL) || defined(OLI_CALL)
#	error "Do not define any of the macros OLL_DLL or OLI_CALL!"
#endif

/// version info
#include "BeginNamespace.h"
enum 
{
    OLI_1_3 = 0x01030000,
    VERSION = OLI_1_3
};
#include "EndNamespace.h"

/// Calling convention for OLI non-static member functions
#define OLI_CALL __stdcall

/// automatic import from / export to DLL
#ifdef OLI_DLLEXPORT
#	define OLL_DLL __declspec(dllexport)
#else
#	define OLL_DLL __declspec(dllimport)
#endif

/// automatic import from / export to DLL of C-style functions.
#ifdef OLI_DLLEXPORT
#	define OLL_API extern "C" __declspec(dllexport)
#else
#	define OLL_API extern "C" __declspec(dllimport)
#endif

/// optimize VTable generation when possible
#if defined(_MSC_VER)
#	define OLI_NOVTABLE __declspec(novtable)
#else 
#	define OLI_NOVTABLE
#endif

/// support for non-standard compliant compilers
#if defined(_MSC_VER)
#	define OLI_NOTHROW throw()
#else 
#	define OLI_NOTHROW
#endif

/// combined definitions
#define OLI_INTERFACE class OLI_NOVTABLE

#endif
