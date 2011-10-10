#ifndef ETAS_OCI_DEFS_H
#define ETAS_OCI_DEFS_H

/**
* @file       ocidefs.h
* @brief      Global defines for the Open Controller Interface (OCI) API.
* @copyright  Copyright (c) 2007-2008 ETAS GmbH. All rights reserved.
*
* $Revision: 4640 $
*/

/**
* @anchor ANCHOR_OCI_INTERFACE_DEFINITION.
* @name   Interface definition and implementation.
* @brief  Marcos for interface definition and implementation.
* @{
*/

/**
* API version management
*/

#define OCI_VERSION 100

/**
* @def OCI_INCLUDE_UNSUPPORTED_API
* Set the following define to disable preprocessor API version checks.
* This is useful when generating a documentation, for instance.
*/
/*
#define OCI_INCLUDE_UNSUPPORTED_API
*/

/**
* @def OCI_STATICLIB
* Statically-linked library (LIB)
*
* @def OCI_STATICDLL
* Dynamic-link library (DLL) with automatic import/export.
*
* @def OCI_DYNAMICDLL
* Dynamic-link library (DLL).
*/

#ifdef OCI_DLLEXPORT
#undef OCI_STATICLIB
#undef OCI_STATICDLL
#undef OCI_DYNAMICDLL
#define OCI_STATICDLL
#endif

/*
Set default to OCI_STATICLIB
*/

#ifndef OCI_STATICDLL
#ifndef OCI_DYNAMICDLL
#undef OCI_STATICLIB
#define OCI_STATICLIB
#endif
#endif

#define OCI_EXTERN extern

/**
* @def OCI_CALLBACK
* Calling convention of all callback functions.
* @todo  Define OCI_CALLBACK (and OCI_CALL) for all platforms and
*        compilers.
*/

#if defined (_MSC_VER)
#define OCI_CALLBACK __cdecl
#elif defined(__GNUC__) && defined (__QNX__)
#define OCI_CALLBACK
#elif defined(__GNUC__) && defined(ALT_RELEASE)
/* ALT_RELEASE define is used by default from NIOS IDE */
#define OCI_CALLBACK
#elif defined (__GNUC__) && (__WIN32__)
#define OCI_CALLBACK __cdecl
#elif defined (__GNUC__) && (__LINUX__)
#define OCI_CALLBACK 
#else
#error Unsupported platform
#endif

/**
* @def OCI_CALL
* Calling convention of all OCI functions.
*/
#if defined (_MSC_VER)
#ifndef OCI_STATICLIB
#define OCI_CALL __cdecl
#ifndef OCI_DYNAMICDLL

/**
* @def OCI_DLLEXPORT
* Specify the API dynamic-link library (DLL) with automatic import
* and export.
*
* This is the standard way of creating macros which make exporting
* from a DLL simpler. All files within this DLL are compiled with
* the @ref OCI_DLLEXPORT symbol defined on the command line. This
* symbol should not be defined on any project that uses this DLL.
* This way any other project whose source files include this file see
* @ref OCI_DECLSPEC functions as being imported from a DLL, whereas
* this DLL sees symbols defined with this macro as being exported.
*
* @def OCI_DECLSPEC
* @copydoc OCI_DLLEXPORT
* @def OCI_IMPLSPEC
* @copydoc OCI_DLLEXPORT
*/

#ifdef OCI_DLLEXPORT
#define OCI_DECLSPEC __declspec(dllexport)
#define OCI_IMPLSPEC __declspec(dllexport)
#else
#define OCI_DECLSPEC __declspec(dllimport)
#define OCI_IMPLSPEC __declspec(dllimport)
#endif

#endif /* OCI_DYNAMICDLL */

#else /* OCI_STATICLIB */

/*
Specify the API for statically-linked library (LIB)
*/
#define OCI_CALL
#define OCI_DECLSPEC
#define OCI_IMPLSPEC

#endif /* OCI_STATICLIB */
#elif defined(__GNUC__)
# define OCI_DECLSPEC __attribute__((visibility("default")))
# define OCI_CALL
#endif

/** @} */

#endif
