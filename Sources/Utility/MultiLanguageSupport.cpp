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
#include "MultiLanguageSupport.h"

//Getting the function pointer for multilanguage support
char* chGetText(char* __msgid)
{
    if(g_pfGetText == NULL)
    {

        //hLibIntl = GetModuleHandle("intl.dll");
        if (NULL == g_hLibIntl)
        {
            g_hLibIntl = LoadLibrary("intl.dll");
        }
        g_pfGetText = (PSGETTEXT)GetProcAddress(g_hLibIntl, "gettext");
    }
    if(g_pfGetText != NULL)
    {
        char* pReturn = g_pfGetText(__msgid);
        int a = 0;
        return pReturn;
    }
    else
    {
        return __msgid;
    }
}

char* textdomain(const char* __domainname)
{
    if(g_pfTextDomain == NULL)
    {
        if (NULL == g_hLibIntl)
        {
            g_hLibIntl = LoadLibrary("intl.dll");
        }
        g_pfTextDomain = (PSTEXTDOMAIN)GetProcAddress(g_hLibIntl, "textdomain");
    }
    if(g_pfTextDomain != NULL)
    {
        char* pReturn = g_pfTextDomain(__domainname);
        return pReturn;
    }
    else
    {
        return (char*) __domainname;
    }
}

char* bindtextdomain(const char* __domainname, const char* __dirname)
{
    if(g_pfBindTextDomain == NULL)
    {
        if (NULL == g_hLibIntl)
        {
            g_hLibIntl = LoadLibrary("intl.dll");
        }
        g_pfBindTextDomain = (PSBINDTEXTDOMAIN)GetProcAddress(g_hLibIntl, "bindtextdomain");
    }
    if(g_pfBindTextDomain != NULL)
    {
        char* pReturn = g_pfBindTextDomain(__domainname, __dirname);
        return pReturn;
    }
    else
    {
        return (char*) __domainname;
    }
}