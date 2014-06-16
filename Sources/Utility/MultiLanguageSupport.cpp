#include "MultiLanguageSupport.h"

#include <windows.h>

/* reference to libintl.dll */
static HMODULE g_hLibIntl;

/* get text */
typedef char * (*PSGETTEXT)(const char * __msgid);
static PSGETTEXT g_pfGetText;

/* text domain */
typedef char * (*PSTEXTDOMAIN)(const char * __domainname);
static PSTEXTDOMAIN g_pfTextDomain;

/* bind text domain */
typedef char * (*PSBINDTEXTDOMAIN)(const char * __domainname, const char * __dirname);
static PSBINDTEXTDOMAIN g_pfBindTextDomain;

void loadInternationalizationLibrary()
{
    if (g_hLibIntl == nullptr)
    {
        g_hLibIntl = LoadLibrary("intl.dll");
    }
}

void unloadInternationalizationLibrary()
{
    if (g_hLibIntl != nullptr)
    {
        FreeLibrary(g_hLibIntl);
    }
}

char * gettext(char * __msgid)
{
    /* get the function pointer */
    if (g_pfGetText == nullptr)
    {
        loadInternationalizationLibrary();

        if (g_hLibIntl != nullptr)
        {
            g_pfGetText = (PSGETTEXT)GetProcAddress(g_hLibIntl, "gettext");
        }
    }

    /* call function */
    if(g_pfGetText != nullptr)
    {
        char * pReturn = g_pfGetText(__msgid);
        return pReturn;
    }
    else
    {
        return __msgid;
    }
}

char * textdomain(const char * __domainname)
{
    /* get the function pointer */
    if (g_pfTextDomain == nullptr)
    {
        loadInternationalizationLibrary();

        if (g_hLibIntl != nullptr)
        {
            g_pfTextDomain = (PSTEXTDOMAIN)GetProcAddress(g_hLibIntl, "textdomain");
        }
    }

    /* call function */
    if (g_pfTextDomain != nullptr)
    {
        char * pReturn = g_pfTextDomain(__domainname);
        return pReturn;
    }
    else
    {
        return (char *) __domainname;
    }
}

char * bindtextdomain(const char * __domainname, const char * __dirname)
{
    /* get the function pointer */
    if (g_pfBindTextDomain == nullptr)
    {
        loadInternationalizationLibrary();

        if (g_hLibIntl != nullptr)
        {
            g_pfBindTextDomain = (PSBINDTEXTDOMAIN)GetProcAddress(g_hLibIntl, "bindtextdomain");
        }
    }

    /* call function */
    if (g_pfBindTextDomain != nullptr)
    {
        char * pReturn = g_pfBindTextDomain(__domainname, __dirname);
        return pReturn;
    }
    else
    {
        return (char *) __domainname;
    }
}
