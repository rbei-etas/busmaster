/*
**                   Copyright 2007 by KVASER AB, SWEDEN      
**                        WWW: http://www.kvaser.com
**
** This software is furnished under a license and may be used and copied
** only in accordance with the terms of such license.
**
** Description:
** This file removes the differences between BCC and MSC with regards
** to some predefined macros. The other differences remain. Sigh.
** ---------------------------------------------------------------------------
*/

#ifndef _PREDEF_H_
#define _PREDEF_H_

// 1) Win16: Borland defines _Windows
//    MSC defines _WINDOWS
//    We want _WINDOWS (but also _Windows to be sure)
#if defined(_Windows) && !defined(_WINDOWS)
#   define _WINDOWS 1
#endif

#if defined(_WINDOWS) && !defined(_Windows)
#   define _Windows 1
#endif


// 2) Win32: Borland defines _Windows and __WIN32__ and _WIN32
//    MSC defines _WIN32
//    We want __WIN32__ and _WINDOWS and _Windows and _WIN32
#if defined(_WIN32) || defined(__WIN32__)
#   if !defined(__WIN32__)
#       define __WIN32__ 1
#   endif
#   if !defined(_WINDOWS)
#       define _WINDOWS 1
#   endif
#   if !defined(_Windows)
#       define _Windows 1
#   endif
#endif


// 3) DLL: Borland defines __DLL__
//    MSC defines _DLL
//    We want __DLL__
#if defined (_DLL) && !defined(__DLL__)
#   define __DLL__ 1
#endif




//===========================================================================
#ifndef USE_CANLIB39_TYPES
  // New typedefs for CANLIB 4.0 and beyond, partly to take care about
  // the 64 bit ints, partly to alleviate pre3.9 mixing of ints and
  // longs.
  #if __STDC_VERSION__ >= 199901L
    #include <stdint.h>
    #define KVINT32   int32_t
    #define KVINT64   int64_t
    #define KVINT32L  int32_t
    #define KVUINT32   uint32_t
    #define KVUINT64   uint64_t
    #define KVUINT32L  uint32_t
  #elif defined(__GNUC__)
    #define KVINT32   int
    #define KVINT64   long long
    #define KVINT32L  int
    #define KVUINT32   unsigned int
    #define KVUINT64   unsigned long long
    #define KVUINT32L  unsigned int
  #else
    #define KVINT32   int
    #define KVINT64   __int64
    #define KVINT32L  int
    #define KVUINT32   unsigned int
    #define KVUINT64   unsigned __int64
    #define KVUINT32L  unsigned int
  #endif
#else
  // Old typedefs for compability with CANLIB 3.9 and older.
  #ifdef __GNUC__
    #define KVINT32   int
    #define KVINT64   long long
    #define KVINT32L  long
    #define KVUINT32   unsigned int
    #define KVUINT64   unsigned long long
    #define KVUINT32L  unsigned long
  #else
    #define KVINT32   int
    #define KVINT64   __int64
    #define KVINT32L  long
    #define KVUINT32   unsigned int
    #define KVUINT64   unsigned __int64
    #define KVUINT32L  unsigned long
  #endif
#endif

#endif

