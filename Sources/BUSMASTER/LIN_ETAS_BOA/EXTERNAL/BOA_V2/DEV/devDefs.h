#ifndef ETAS_DEV_DEFS_H
#define ETAS_DEV_DEFS_H

/**
* @file
* @brief      Global defines for the Licensing (DEV) API.
* @copyright  Copyright (c) 2011 ETAS GmbH. All rights reserved.
*
* $Revision: 6642 $
*/

/**
* @anchor ANCHOR_DEV_INTERFACE_DEFINITION.
* @name   Interface definition and implementation.
* @brief  Marcos for interface definition and implementation.
* @{
*/

/**
* @def DEV_STATICLIB
* Statically-linked library (LIB)
*
* @def DEV_STATICDLL
* Dynamic-link library (DLL) with automatic import/export.
*
* @def DEV_DYNAMICDLL
* Dynamic-link library (DLL).
*/

#ifdef DEV_DLLEXPORT
#undef DEV_STATICLIB
#undef DEV_STATICDLL
#undef DEV_DYNAMICDLL
#define DEV_STATICDLL
#endif

/*
Set default to DEV_STATICLIB
*/

#ifndef DEV_STATICDLL
#ifndef DEV_DYNAMICDLL
#undef DEV_STATICLIB
#define DEV_STATICLIB
#endif
#endif

#define DEV_EXTERN extern

/**
* @def DEV_CALLBACK
* Calling convention of all callback functions.
*/

#if defined (_MSC_VER)
#define DEV_CALLBACK __cdecl
#elif defined(__GNUC__) && defined (__QNX__)
#define DEV_CALLBACK
#elif defined(__GNUC__) && defined(ALT_RELEASE)
/* ALT_RELEASE define is used by default from NIOS IDE */
#define DEV_CALLBACK
#elif defined (__GNUC__) && (__WIN32__)
#define DEV_CALLBACK __cdecl
#elif defined (__GNUC__) && (__LINUX__)
#define DEV_CALLBACK
#else
#error Unsupported platform
#endif

/**
* @def DEV_CALL
* Calling convention of all OCI functions.
*/
#if defined (_MSC_VER)
#ifndef DEV_STATICLIB
#define DEV_CALL __cdecl
#ifndef DEV_DYNAMICDLL

/**
* @def DEV_DLLEXPORT
* Specify the API dynamic-link library (DLL) with automatic import
* and export.
*
* This is the standard way of creating macros which make exporting
* from a DLL simpler. All files within this DLL are compiled with
* the @ref DEV_DLLEXPORT symbol defined on the command line. This
* symbol should not be defined on any project that uses this DLL.
* This way any other project whose source files include this file see
* @ref DEV_DECLSPEC functions as being imported from a DLL, whereas
* this DLL sees symbols defined with this macro as being exported.
*
* @def DEV_DECLSPEC
* @copydoc DEV_DLLEXPORT
* @def DEV_IMPLSPEC
* @copydoc DEV_DLLEXPORT
*/

#ifdef DEV_DLLEXPORT
#define DEV_DECLSPEC __declspec(dllexport)
#define DEV_IMPLSPEC __declspec(dllexport)
#else
#define DEV_DECLSPEC __declspec(dllimport)
#define DEV_IMPLSPEC __declspec(dllimport)
#endif

#endif /* DEV_DYNAMICDLL */

#else /* DEV_STATICLIB */

/*
Specify the API for statically-linked library (LIB)
*/
#define DEV_CALL
#define DEV_DECLSPEC
#define DEV_IMPLSPEC

#endif /* DEV_STATICLIB */
#elif defined(__GNUC__)
# define DEV_DECLSPEC __attribute__((visibility("default")))
# define DEV_CALL
#endif

/** @} */

#endif
