#pragma once
//#include <afxdtctl.h>       // MFC support for Internet Explorer 4 Common Controls
//#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>         // MFC support for Windows Common Controls
//#endif // _AFX_NO_AFXCMN_SUPPORT
//#include <afxcview.h>
//#include <afxtempl.h>
//#include <afxmt.h>
//#include <mmsystem.h>
#include <math.h>
#include <IO.H>             // i/0 header definitions
#include <stdlib.h>         // standard library definitions
//#include <windows.h>
#import <msxml6.dll>

#include <WinDef.h>
#include <locale.h>



typedef char* (*PSGETTEXT)(const char* __msgid);
static PSGETTEXT g_pfGetText;

typedef char* (*PSTEXTDOMAIN)(const char* __domainname);
static PSTEXTDOMAIN g_pfTextDomain;

typedef char* (*PSBINDTEXTDOMAIN)(const char* __domainname, const char* __dirname);
static PSBINDTEXTDOMAIN g_pfBindTextDomain;

//#define _(string) g_pfGetText (string)

//typedef __declspec(dllimport)char* (* PSGETTEXT)(char* __msgid);
//PSGETTEXT g_pfGetText;
static HMODULE g_hLibIntl;

//Getting the function pointer for multilanguage support
char* chGetText(char* __msgid);

char* textdomain(const char* __domainname);
char* bindtextdomain(const char* __domainname, const char* __dirname);

#define _(string) chGetText (string)
