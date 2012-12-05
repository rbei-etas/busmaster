#pragma once
#include <afxdtctl.h>       // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>         // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxcview.h>
#include <afxtempl.h>
#include <afxmt.h>
#include <mmsystem.h>
#include <math.h>
#include <IO.H>             // i/0 header definitions
#include <stdlib.h>         // standard library definitions
#include <windows.h>
#import <msxml6.dll>

#include <WinDef.h>
#include <locale.h>



typedef char* (__stdcall*  PSGETTEXT)(char* __msgid);
static PSGETTEXT g_pfGetText;
//#define _(string) g_pfGetText (string)

//typedef __declspec(dllimport)char* (* PSGETTEXT)(char* __msgid);
//PSGETTEXT g_pfGetText;
static HMODULE g_hLibIntl;

//Getting the function pointer for multilanguage support
static char* chGetText(char* __msgid)
{
	if(g_pfGetText == NULL)
	{
		
		//hLibIntl = GetModuleHandle("intl.dll");
		g_hLibIntl = LoadLibrary("intl.dll");
		g_pfGetText = (PSGETTEXT)GetProcAddress(g_hLibIntl, "gettext");
	}
	if(g_pfGetText != NULL)
	{
		char* pReturn = g_pfGetText(__msgid);
		return pReturn;
	}
	else
	{
		return __msgid;
	}
}

#define _(string) chGetText (string)