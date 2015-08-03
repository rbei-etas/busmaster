/*
**                     Copyright 1995,1996 by KVASER AB
**         P.O Box 4076 S-51104 KINNAHULT, SWEDEN. Tel. +46 320 15287
**
** This software is furnished under a license and may be used and copied
** only in accordance with the terms of such license.
**
*/

#pragma message("This file is obsolete")



#ifndef _STD_H_
#define _STD_H_

#define ARGUSED(x)  (void)(x)
#define PRIVATE static
#define PUBLIC

#define COUNTOF(x) ((signed int)(sizeof(x)/sizeof(x[0])))

#include "predef.h"


#ifdef _WINDOWS
   //
   // Building Windows (16/32) version
   //
#  include <windows.h>
#else
   //
   // Building DOS version
   // 
#  define TRUE             1
#  define FALSE            0
#  define BOOL             int

#  define WM_USER          0x0400

typedef unsigned char      BYTE;
typedef unsigned short     WORD;
typedef unsigned long      DWORD;

typedef WORD               WPARAM;
typedef DWORD              LPARAM;

typedef void (*HWND)(WPARAM aMsg, WPARAM aHandle, LPARAM aLparam);

#endif



typedef unsigned long ulong;
typedef unsigned short ushort;
typedef unsigned char uchar;
typedef unsigned int uint;


/*
** Define macros for direct port I/O. (not for Win32)
*/
#if !defined(__WIN32__)
#   if defined(__BORLANDC__)
#      define WRITE_PORT_UCHAR(PORT, VALUE) outportb((unsigned int)(PORT), (VALUE))
#      define READ_PORT_UCHAR(PORT) inportb((unsigned int)(PORT))
#      define DISABLE_INTERRUPTS _disable()
#      define ENABLE_INTERRUPTS  _enable()
#   endif /* __BORLANDC__ */

#   if defined(_MSC_VER)
       /*Prototypes from conio.h, MSVC1.52*/
       int __cdecl _inp(unsigned);
       int __cdecl _outp(unsigned, int);
#      define WRITE_PORT_UCHAR(PORT, VALUE) _outp((unsigned int)(PORT), (int)(VALUE))
#      define READ_PORT_UCHAR(PORT) (unsigned char)_inp((unsigned int)(PORT))
#      define DISABLE_INTERRUPTS _disable()
#      define ENABLE_INTERRUPTS  _enable()
#   endif /* __MSC */
#endif /*!defined(__WIN32__)*/

/*
** Used to convert a major.minor type version number to a 16-bit word.
*/
#define VERSION_NUMBER(x,y) (((x)<<8) + (y))

/*
** A macro for notifying the caller when messages arrives (or something
** else happens.)  If we are running under Windows, we use the standard
** message passing routine PostMessage.  In DOS, we fake messaging by
** calling a user-supplied routine instead - note that it will be called at
** interrupt level.
*/
#if defined(_Windows) || defined(__WIN32__)
/* Win16/32 version */
#     define NOTIFY(hWnd, msg, handle, status) \
                    (void)PostMessage((hWnd), WM__CANLIB, \
                    ((WPARAM)(handle)), \
                    ((LPARAM)(((long)(status))<<16) + (msg)))

#else
/* DOS version */
#  define NOTIFY(hWnd, msg, handle, status) \
                if (hWnd)   \
                (*(hWnd))(WM__CANLIB, \
                ((WPARAM)(handle)), \
                ((LPARAM)(((long)(status))<<16) + (msg)))
#endif


#if defined(__cplusplus) && defined(__BORLANDC__) && (__BORLANDC__ < 0x460)
// These templates are defined in Borland C++ 4.52
       template <class T> inline const T min(const T& a, const T& b) {
           if (a < b)
               return a;
           else
               return b;
       }
       template <class T> inline const T max(const T& a, const T& b) {
           if (a > b)
               return a;
           else
               return b;
       }
#endif

#define minmax(n, a, b) max(a, min(b, n))

#ifndef CompilerAssert
// An assert that is evaluated during compilation.
// If the expression is zero, the compiler will warn that the vector
// _Dummy[] has zero elements, otherwise it is silent.
//
// Lint warning 506 is "Constant value Boolean",
// 762 is "Redundantly declared ... previously declared at line ..."
#define CompilerAssert(e)     \
  /*lint -save -e506 -e762 */ \
  extern char _Dummy[(e)?1:0] \
  /*lint -restore */
#endif

       
#endif
