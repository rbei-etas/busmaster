#ifndef ETAS_CSI_DEFS_H
#define ETAS_CSI_DEFS_H

#ifdef CSI_ESS_DLLEXPORT
#define CSI_ESS_DECLSPEC __declspec(dllexport)
#define CSI_ESS_IMPLSPEC __declspec(dllexport)
#else 
#define CSI_ESS_DECLSPEC __declspec(dllimport)
#define CSI_ESS_IMPLSPEC __declspec(dllimport)
#endif

#ifdef CSI_APP_DLLEXPORT
#define CSI_APP_DECLSPEC __declspec(dllexport)
#define CSI_APP_IMPLSPEC __declspec(dllexport)
#else 
#define CSI_APP_DECLSPEC __declspec(dllimport)
#define CSI_APP_IMPLSPEC __declspec(dllimport)
#endif





/**
* @file       csidefs.h
* @brief      Global defines for the Connection Service Interface (CSI).
* @copyright  Copyright (c) 2008 ETAS GmbH. All rights reserved.
*
* $Revision: 4640 $
*/

/**
* @anchor ANCHOR_CSI_INTERFACE_DEFINITION.
* @name   Interface definition and implementation.
* @brief  Macros for interface definition and implementation.
* @{
*/

/**
* API version management
*/

#define CSI_VERSION 100


/**
* @def CSI_STATICLIB  
* Statically-linked library (LIB)
*
* @def CSI_STATICDLL  
* Dynamic-link library (DLL) with automatic import/export.
*
* @def CSI_DYNAMICDLL 
* Dynamic-link library (DLL).
*/

#ifdef CSI_DLLEXPORT
#undef CSI_STATICLIB
#undef CSI_STATICDLL
#undef CSI_DYNAMICDLL
#define CSI_STATICDLL
#endif

/*
Set default to CSI_STATICLIB
*/

#ifndef CSI_STATICDLL
#ifndef CSI_DYNAMICDLL
#undef CSI_STATICLIB
#define CSI_STATICLIB
#endif
#endif

#define CSI_EXTERN extern

/**
* @def CSI_CALLBACK
* Calling convention of all callback functions.
* @todo  Define CSI_CALLBACK (and CSI_CALL) for all platforms and 
*        compilers.
*/

#if defined (_MSC_VER)
#define CSI_CALLBACK __cdecl
#elif defined(__GNUC__) && defined (__QNX__)
#define CSI_CALLBACK
#define CSI_DECLSPEC __attribute__ ((visibility("default")))
#define CSI_CALL 
#elif defined(__GNUC__) && defined (__LINUX__)
#define CSI_CALLBACK
#define CSI_DECLSPEC __attribute__ ((visibility("default")))
#define CSI_CALL 
#else
#error Unsupported platform
#endif

/**
* @def CSI_CALL
* Calling convention of all CSI functions.
*/

#ifndef CSI_STATICLIB
#define CSI_CALL __cdecl
#ifndef CSI_DYNAMICDLL

/**
* @def CSI_DLLEXPORT
* Specify the API dynamic-link library (DLL) with automatic import 
* and export. 
*
* This is the standard way of creating macros which make exporting 
* from a DLL simpler. All files within this DLL are compiled with 
* the @ref CSI_DLLEXPORT symbol defined on the command line. This 
* symbol should not be defined on any project that uses this DLL. 
* This way any other project whose source files include this file see 
* @ref CSI_DECLSPEC functions as being imported from a DLL, whereas 
* this DLL sees symbols defined with this macro as being exported.
*
* @def CSI_DECLSPEC
* @copydoc CSI_DLLEXPORT
* @def CSI_IMPLSPEC
* @copydoc CSI_DLLEXPORT
*/
#ifdef CSI_DLLEXPORT
#define CSI_DECLSPEC __declspec(dllexport)
#define CSI_IMPLSPEC __declspec(dllexport)
#else 
#define CSI_DECLSPEC __declspec(dllimport)
#define CSI_IMPLSPEC __declspec(dllimport)
#endif
#endif /* CSI_DYNAMICDLL */

#else /* CSI_STATICLIB */

#if defined(_MSC_VER)
/*
Specify the API for statically-linked library (LIB) 
*/
#define CSI_CALL
#define CSI_DECLSPEC
#define CSI_IMPLSPEC

#endif /* CSI_STATICLIB */
#endif

/** @} */

#endif
