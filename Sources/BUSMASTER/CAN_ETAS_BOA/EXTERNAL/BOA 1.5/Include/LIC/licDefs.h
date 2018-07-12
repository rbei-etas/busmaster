#ifndef ETAS_LIC_DEFS_H
#define ETAS_LIC_DEFS_H

/**
* @file
* @brief      Global defines for the Licensing (LIC) API.
* @copyright  Copyright (c) 2011 ETAS GmbH. All rights reserved.
*
* $Revision: 5986 $
*/

/**
* @anchor ANCHOR_LIC_INTERFACE_DEFINITION.
* @name   Interface definition and implementation.
* @brief  Marcos for interface definition and implementation.
* @{
*/

/**
* @def LIC_STATICLIB
* Statically-linked library (LIB)
*
* @def LIC_STATICDLL
* Dynamic-link library (DLL) with automatic import/export.
*
* @def LIC_DYNAMICDLL
* Dynamic-link library (DLL).
*/

#ifdef LIC_DLLEXPORT
#undef LIC_STATICLIB
#undef LIC_STATICDLL
#undef LIC_DYNAMICDLL
#define LIC_STATICDLL
#endif

/*
Set default to LIC_STATICLIB
*/

#ifndef LIC_STATICDLL
#ifndef LIC_DYNAMICDLL
#undef LIC_STATICLIB
#define LIC_STATICLIB
#endif
#endif

#define LIC_EXTERN extern

/**
* @def LIC_CALLBACK
* Calling convention of all callback functions.
*/

#if defined (_MSC_VER)
#define LIC_CALLBACK __cdecl
#elif defined(__GNUC__) && defined (__QNX__)
#define LIC_CALLBACK
#elif defined(__GNUC__) && defined(ALT_RELEASE)
/* ALT_RELEASE define is used by default from NIOS IDE */
#define LIC_CALLBACK
#elif defined (__GNUC__) && (__WIN32__)
#define LIC_CALLBACK __cdecl
#elif defined (__GNUC__) && (__LINUX__)
#define LIC_CALLBACK
#else
#error Unsupported platform
#endif

/**
* @def LIC_CALL
* Calling convention of all OCI functions.
*/
#if defined (_MSC_VER)
#ifndef LIC_STATICLIB
#define LIC_CALL __cdecl
#ifndef LIC_DYNAMICDLL

/**
* @def LIC_DLLEXPORT
* Specify the API dynamic-link library (DLL) with automatic import
* and export.
*
* This is the standard way of creating macros which make exporting
* from a DLL simpler. All files within this DLL are compiled with
* the @ref LIC_DLLEXPORT symbol defined on the command line. This
* symbol should not be defined on any project that uses this DLL.
* This way any other project whose source files include this file see
* @ref LIC_DECLSPEC functions as being imported from a DLL, whereas
* this DLL sees symbols defined with this macro as being exported.
*
* @def LIC_DECLSPEC
* @copydoc LIC_DLLEXPORT
* @def LIC_IMPLSPEC
* @copydoc LIC_DLLEXPORT
*/

#ifdef LIC_DLLEXPORT
#define LIC_DECLSPEC __declspec(dllexport)
#define LIC_IMPLSPEC __declspec(dllexport)
#else
#define LIC_DECLSPEC __declspec(dllimport)
#define LIC_IMPLSPEC __declspec(dllimport)
#endif

#endif /* LIC_DYNAMICDLL */

#else /* LIC_STATICLIB */

/*
Specify the API for statically-linked library (LIB)
*/
#define LIC_CALL
#define LIC_DECLSPEC
#define LIC_IMPLSPEC

#endif /* LIC_STATICLIB */
#elif defined(__GNUC__)
# define LIC_DECLSPEC __attribute__((visibility("default")))
# define LIC_CALL
#endif

/** @} */

#endif
