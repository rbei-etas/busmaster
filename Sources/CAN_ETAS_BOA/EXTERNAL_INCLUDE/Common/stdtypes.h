#ifndef ETAS_STDTYPES_H
#define ETAS_STDTYPES_H

/**
* @file       stdtypes.h
* @brief      Header file for declaration of standard data types.
* 
* Implements a standardized definition of the basic numerical and logical data types
* in order to avoid the usage of the build-in C-types (except char for strings).
* Advantageous is the explicit description of sign and width.
*
* Name convention:  signed-types begin with letter      's'
*                   unsigned-types begin with letter    'u'
*                   types are written in lower-case
*
* @copyright  Copyright (c) 2007-2008 ETAS GmbH. All rights reserved.
*
* $Revision: 4509 $
*/

/**
* @addtogroup GROUP_BOA_GLOBAL_TYPES Global Types
*
* @{
*
*/

#if ( defined _MSC_VER && !defined STRICT )
   #pragma message (__FILE__ ": You should #define STRICT to keep all functions and classes behave correct in all circumstances.")
#endif

typedef signed char              sint8;
typedef unsigned char            uint8;
typedef signed char              int8;
typedef short                    sint16;
typedef unsigned short           uint16;
typedef short                    int16;              
typedef long                     sint32;
typedef unsigned long            uint32;
typedef long                     int32;
typedef long long                sint64;
typedef unsigned long long       uint64;
typedef long long                int64;


/* Machine dependent integer types */

typedef int                      sint;
typedef unsigned int             uint;

/* Type definition for Byte */

typedef unsigned char            byte;
typedef unsigned short           word;
typedef unsigned long            dword;

/* Floating point types */

typedef float                    real32;
typedef double                   real64;

/* Type definition for Boolean */

#ifndef __cplusplus
typedef int                      bool;
#define false ((bool)0)
#define true  ((bool)1)
#endif

/** 
* @} 
*/

#endif

