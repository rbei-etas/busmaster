/*
* Basic Open API - Open Link Interface
* Version 1.3
*
* Copyright (c) ETAS GmbH. All rights reserved.
*
* $Revision: 4722 $
*/

/** 
* @file
* @brief  Global preprocessor definitions.
* @remark The header structure of the OLI may change
*         in future releases. Don't include this
*         header directly. Use @ref OLI.h instead.
*/

#ifndef __OLI_DEFS_H
#define __OLI_DEFS_H

// safety check
#if defined(OLL_DLL) || defined(OLI_CALL)
#	error "Do not define any of the macros OLL_DLL or OLI_CALL!"
#endif

// open ETAS::OLI namespace

#include "BeginNamespace.h"

#ifdef _DOXYGEN
namespace ETAS {namespace OLI {
#endif

/** 
* @ingroup GROUP_OLI_COMMON_BASE
* @brief  API version constants.
*
* When creating a @ref ILink "link" object, the desired interface
* version number must be specified. It selects the available interfaces
* as well as their behavior.
*
* @since  BOA 1.3
* @see    ICANLink::Create,  ILINLink::Create, IFlexRayLink::Create
*/

enum 
{
    /** Identifies OLI 1.3.0.0 . */
    OLI_1_3 = 0x01030000,

    /** Identifies OLI 1.4.0.0 . */
    OLI_1_4 = 0x01040000,

    /** The current OLI version. */
    VERSION = OLI_1_4
};

// close ETAS::OLI namespace

#ifdef _DOXYGEN
}}
#endif

#include "EndNamespace.h"

/** 
* @ingroup GROUP_OLI_COMMON_BASE
* @brief   Calling convention for all OLI functions and methods 
* @coding  This definition is intended for use within the OLI only.
*          Do not use this in client application code.
* @since   BOA 1.0
*/
#define OLI_CALL __stdcall

/** 
* @ingroup GROUP_OLI_COMMON_BASE
* @brief   Automatic import from / export to DLL of C-style functions
* @coding  This definition is intended for use within the OLI only.
*          Do not use this in client application code.
* @since   BOA 1.0
*/
#ifdef OLI_DLLEXPORT
#	define OLL_API extern "C" __declspec(dllexport)
#else
#	define OLL_API extern "C" __declspec(dllimport)
#endif

/** 
* @ingroup GROUP_OLI_COMMON_BASE
* @brief   Optimized VTable generation, if available
* @coding  This definition is intended for use within the OLI only.
*          Do not use this in client application code.
* @since   BOA 1.3
*/
#if defined(_MSC_VER)
#	define OLI_NOVTABLE __declspec(novtable)
#else 
#	define OLI_NOVTABLE
#endif

/** 
* @ingroup GROUP_OLI_COMMON_BASE
* @brief   Indicate that a method or free function will never 
*          throw an exception
* @coding  This definition is intended for use within the OLI only.
*          Do not use this in client application code.
* @since   BOA 1.3
*/
#if defined(_MSC_VER)
#	define OLI_NOTHROW throw()
#else 
#	define OLI_NOTHROW
#endif

/** 
* @ingroup GROUP_OLI_COMMON_BASE
* @brief   Combined definition used with all OLI interface classes
* @coding  This definition is intended for use within the OLI only.
*          Do not use this in client application code.
* @since   BOA 1.3
*/
#define OLI_INTERFACE class OLI_NOVTABLE

#endif
