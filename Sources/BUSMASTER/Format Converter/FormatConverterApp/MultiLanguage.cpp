//////////////////////////////////////////////////////////////////////
/* Multiple language support class: CMultiLanguage
    Copyright (c) 2005 - Herbert Yu
    yuhaian@hotmail.com
    http://www.codeproject.com/useritems/MultiLangSupportMFCExtDLL.asp
License:
    You may use this code in any commersial or non-commersial application,
    and you may redistribute this file (and even modify it if you wish) as
    long as you keep this notice untouched in any version you redistribute.
    Do NOT distribute this file after you modify it without send an email to
    auther. When send email to me put [MultiLangSupportMFCExtDLL] in the
    subject line of email, and attach your changes in the email. So I can put
    it to main distribution at codeproject.com.
Modification History:
    07/15/2005  Herbert Yu  First release
Usage:
  - Insert following into InitInstance() of your application
    // Begin of Multiple Language support
    if ( CMultiLanguage::m_nLocales <= 0 )  // Not detected yet
    {
        CMultiLanguage::DetectLangID();     // Detect language as user locale
        CMultiLanguage::DetectUILanguage(); // Detect language in MUI OS
    }
    TCHAR szModuleFileName[MAX_PATH];       // Get Module File Name and path
    int ret = ::GetModuleFileName(theApp.m_hInstance, szModuleFileName, MAX_PATH);
    if ( ret == 0 || ret == MAX_PATH )
        ASSERT(FALSE);
    // Load resource-only language DLL. It will use the languages
    // detected above, take first available language,
    // or you can specify another language as second parameter to
    // LoadLangResourceDLL. And try that first.
    HINSTANCE hLangInst = CMultiLanguage::LoadLangResourceDLL( szModuleFileName );
    if (hLangInst)
        AfxSetResourceHandle( hLangInst );
    // End of Multiple Language support
  - Insert following into your DllMain(...) function just before new CDynLinkLibrary(...)
        // Begin of Multiple Language support
        if ( CMultiLanguage::m_nLocales <= 0 )  // Not detected yet
        {
            CMultiLanguage::DetectLangID();     // Detect language as user locale
            CMultiLanguage::DetectUILanguage(); // Detect language in MUI OS
        }
        TCHAR szModuleFileName[MAX_PATH];       // Get Module File Name and path
        int ret = ::GetModuleFileName(hInstance, szModuleFileName, MAX_PATH);
        if ( ret == 0 || ret == MAX_PATH )
            ASSERT(FALSE);
        // Load resource-only language DLL. It will use the languages
        // detected above, take first available language,
        // or you can specify another language as second parameter to
        // LoadLangResourceDLL. And try that first.
        HINSTANCE hLangInst = CMultiLanguage::LoadLangResourceDLL( szModuleFileName );
        if (hLangInst)
            SubDLLDLL.hResource = hLangInst;
        // End of Multiple Language support
  - Include MultiLanguage.cpp/.h in your project
  - FreeLibrary when ExitInstance() or in DLL_PROCESS_DETACH process.
  - Insert resource-only dll files for each dll/exe and each new language.
*/
// MultiLanguage.cpp: implementation of the CMultiLanguage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MultiLanguage.h"

typedef LANGID (WINAPI* PFNGETUSERDEFAULTUILANGUAGE)();
typedef LANGID (WINAPI* PFNGETSYSTEMDEFAULTUILANGUAGE)();

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

LCID    CMultiLanguage::m_alcidSearch[MAX_NUM_LCID] = { 0*MAX_NUM_LCID };
int     CMultiLanguage::m_nLocales = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMultiLanguage::CMultiLanguage()
{

}

CMultiLanguage::~CMultiLanguage()
{

}


LCID    CMultiLanguage::AddLangId( LANGID langidIn )
{
    LCID lcidIn = MAKELCID(langidIn, SORT_DEFAULT);
    return AddLocale( lcidIn );
}

LCID    CMultiLanguage::AddLocale( LCID lcidIn )
{
    for ( int iLocale = 0; iLocale < m_nLocales; iLocale++ )
    {
        if ( m_alcidSearch[iLocale] == lcidIn )
        {
            // Do not store duplicated lcid.
            return lcidIn;
        }
    }
    if ( m_nLocales < MAX_NUM_LCID )
    {
        LANGID langid = LANGIDFROMLCID(lcidIn);
        int nPrimaryLang = PRIMARYLANGID(langid);
        //int nSubLang = SUBLANGID(langid);
        LCID lcid = lcidIn;
        m_alcidSearch[m_nLocales] = ::ConvertDefaultLocale(lcid);
        m_nLocales++;

        if ( m_nLocales < MAX_NUM_LCID )
        {
            lcid = MAKELCID(MAKELANGID(nPrimaryLang, SUBLANG_NEUTRAL), SORT_DEFAULT);
            m_alcidSearch[m_nLocales] = ::ConvertDefaultLocale(lcid);
            m_nLocales++;
        }
        else
        {
            // Failed. Too may locale added
            return MAKELCID(MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), SORT_DEFAULT);
        }
        // Success return
        return MAKELCID(langid, SORT_DEFAULT);
    }
    else
    {
        // Failed. Too may locale added
        return MAKELCID(MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), SORT_DEFAULT);
    }
}

//  Based on the code from C:\Program Files\Microsoft Visual Studio .NET 2003\Vc7\atlmfc\src\mfc\appcore.cpp
//      with modification for our multiple lanugage support in VC6.
LANGID CMultiLanguage::DetectUILanguage()
{
    LANGID langid = 0;
    int nPrimaryLang = 0;
    int nSubLang = 0;
    LCID lcid = 0;
    PFNGETUSERDEFAULTUILANGUAGE pfnGetUserDefaultUILanguage;
    PFNGETSYSTEMDEFAULTUILANGUAGE pfnGetSystemDefaultUILanguage;
    HINSTANCE hKernel32;

    hKernel32 = ::GetModuleHandle(_T("kernel32.dll"));
    ASSERT(hKernel32 != NULL);
    pfnGetUserDefaultUILanguage = (PFNGETUSERDEFAULTUILANGUAGE)::GetProcAddress(
                                      hKernel32, "GetUserDefaultUILanguage");
    if(pfnGetUserDefaultUILanguage != NULL)
    {
        // First, try the user's UI language
        langid = pfnGetUserDefaultUILanguage();
        AddLangId( langid );
        TRACE(_T("CMultiLanguage::DetectUILanguage() 1st/2nd = %04X\n"), langid );

        // Then, try the system's default UI language
        pfnGetSystemDefaultUILanguage = (PFNGETSYSTEMDEFAULTUILANGUAGE)::GetProcAddress(
                                            hKernel32, "GetSystemDefaultUILanguage");
        ASSERT( pfnGetSystemDefaultUILanguage != NULL );

        langid = pfnGetSystemDefaultUILanguage();
        AddLangId( langid );
        TRACE(_T("CMultiLanguage::DetectUILanguage() 3rd/4th = %04X\n"), langid );
    }
    else
    {
        // We're not on an MUI-capable system.
        if (::GetVersion()&0x80000000)
        {
            // We're on Windows 9x, so look in the registry for the UI language
            HKEY hKey = NULL;
            LONG nResult = ::RegOpenKeyEx(HKEY_CURRENT_USER,
                                          _T( "Control Panel\\Desktop\\ResourceLocale" ), 0, KEY_READ, &hKey);
            if (nResult == ERROR_SUCCESS)
            {
                DWORD dwType;
                TCHAR szValue[16] = "";
                ULONG nBytes = sizeof( szValue );
                nResult = ::RegQueryValueEx(hKey, NULL, NULL, &dwType,
                                            LPBYTE( szValue ), &nBytes );
                if ((nResult == ERROR_SUCCESS) && (dwType == REG_SZ))
                {
                    DWORD dwLangID;
                    int nFields = _stscanf( szValue, _T( "%lu" ), &dwLangID );
                    if( nFields == 1 )
                    {
                        langid = LANGID( dwLangID );
                        AddLangId( langid );
                        TRACE(_T("CMultiLanguage::DetectUILanguage() 9X1st/2nd = %04X\n"),
                              langid );
                    }
                }
                ::RegCloseKey(hKey);
            }
        }
        else
        {
            // We're on NT 4.  The UI language is the same as the language of the
            // version resource in ntdll.dll
            HMODULE hNTDLL = ::GetModuleHandle( _T( "ntdll.dll" ) );
            if (hNTDLL != NULL)
            {
                langid = 0;
                ::EnumResourceLanguages( hNTDLL, RT_VERSION, MAKEINTRESOURCE( 1 ),
                                         _AfxEnumResLangProc, reinterpret_cast< LONG_PTR >( &langid ) );
                if (langid != 0)
                {
                    AddLangId( langid );
                    TRACE(_T("CMultiLanguage::DetectUILanguage() NT1st/2nd = %04X\n"),
                          langid );
                }
            }
        }
    }

    if ( m_nLocales < MAX_NUM_LCID )
    {
        m_alcidSearch[m_nLocales] = LOCALE_SYSTEM_DEFAULT;
        m_nLocales++;
    }
    else
    {
        m_alcidSearch[MAX_NUM_LCID-1] = LOCALE_SYSTEM_DEFAULT;
        m_nLocales = MAX_NUM_LCID;
    }

    return LANGIDFROMLCID(m_alcidSearch[0]);
}

BOOL CALLBACK CMultiLanguage::_AfxEnumResLangProc(HMODULE /*hModule*/, LPCTSTR /*pszType*/,
        LPCTSTR /*pszName*/, WORD langid, LONG_PTR lParam)
{
    if(lParam == NULL)
    {
        return FALSE;
    }

    LANGID* plangid = reinterpret_cast< LANGID* >( lParam );
    *plangid = langid;

    return TRUE;
}

HINSTANCE CMultiLanguage::LoadLangResourceDLL(LPCTSTR szModuleName, LANGID langUpdateId)
{
    TCHAR       szResDLLName[_MAX_PATH+14];
    HINSTANCE   hLangDLL = NULL;
    LCID        alcid[MAX_NUM_LCID+1];
    TCHAR       szLangCode[4];
    //LPTSTR    pszExtension;
    int         nNoExtension;
    LCID        lcid;
    int         nLocales = 0;

    //pszExtension = ::PathFindExtension(szModuleName);
    //nNoExtension = pszExtension - szModuleName;   temp. for ".exe"
    nNoExtension = lstrlen(szModuleName) - 3 ;  // Quick and kind of dirty way to take ".exe"/".dll" away.
    if ( langUpdateId != MAKELANGID(LANG_NEUTRAL,SUBLANG_NEUTRAL) )
    {
        alcid[nLocales] = MAKELCID(langUpdateId, SORT_DEFAULT);
        nLocales++;
    }
    for ( int iLocale = 0; iLocale < m_nLocales; iLocale++ )
    {
        if ( m_alcidSearch[iLocale] != 0 )
        {
            alcid[nLocales] = m_alcidSearch[iLocale];
            nLocales++;
        }
    }
    for ( int iLocale = 0; iLocale < nLocales; iLocale++ )
    {
        lcid = alcid[iLocale];
        if (lcid == LOCALE_SYSTEM_DEFAULT)
        {
            lstrcpy(szLangCode, _T("LOC"));
        }
        else
        {
            int nResult = ::GetLocaleInfo(lcid, LOCALE_SABBREVLANGNAME, szLangCode, 4);
            ASSERT( nResult == 4 );
            if ( nResult == 0 )
            {
                return NULL;
            }
        }

        if ( nNoExtension + 3 + 4 + 1 < _MAX_PATH+14 )
        {
            // append "ENU.DLL" to moduleName
            lstrcpyn(szResDLLName, szModuleName, nNoExtension);
            lstrcat(szResDLLName, szLangCode);
            lstrcat(szResDLLName, _T(".DLL"));
        }
        else
        {
            ASSERT(FALSE);  // No enough space to hold language resource dll name path.
            return NULL;
        }
        hLangDLL = ::LoadLibrary(szResDLLName);
        if(hLangDLL != NULL)
        {
            return hLangDLL;    // Successful return
        }
    }

    return hLangDLL;
}

LANGID CMultiLanguage::DetectLangID()
{
    LANGID langid = 0;
    int nPrimaryLang = 0;
    int nSubLang = 0;
    LCID lcid = 0;
    int nLocales = 0;

    langid = GetUserDefaultLangID();    // WinNT3.1/95 and later
    AddLangId( langid );
    TRACE(_T("CMultiLanguage::GetUserDefaultLangID() 1st/2nd = %0X\n"),
          langid );

    LANGID langSysid = GetSystemDefaultLangID();    // WinNT3.1/95 and later
    AddLangId( langSysid );
    TRACE(_T("CMultiLanguage::GetSystemDefaultLangID() 3rd/4th = %0X\n"),
          langid );

    return langid;
}

int CMultiLanguage::ResetDetectedLanguage()
{
    m_nLocales = 0;
    for ( int iLocale = 0; iLocale < MAX_NUM_LCID; iLocale++ )
    {
        m_alcidSearch[iLocale] = 0;
    }
    return TRUE;
}

#ifdef  _DEBUG
int CMultiLanguage::PrintThreeLetterLanguageCodeList( void )
{
    LANGID  id[] =
    {
        0x0436,0x041c,0x0401,0x0801,0x0c01,0x1001,0x1401,0x1801,0x1c01,0x2001,0x2401,0x2801,
        0x2c01,0x3001,0x3401,0x3801,0x3c01,0x4001,0x042b,0x042c,0x082c,0x042d,0x0423,0x0445,
        0x141a,0x0402,0x0455,0x0403,0x0404,0x0804,0x0c04,0x1004,0x1404,0x041a,0x101a,0x0405,
        0x0406,0x0465,0x0413,0x0813,0x0409,0x0809,0x0c09,0x1009,0x1409,0x1809,0x1c09,0x2009,
        0x2409,0x2809,0x2c09,0x3009,0x3409,0x0425,0x0438,0x0429,0x040b,0x040c,0x080c,0x0c0c,
        0x100c,0x140c,0x180c,0x0456,0x0437,0x0407,0x0807,0x0c07,0x1007,0x1407,0x0408,0x0447,
        0x040d,0x0439,0x040e,0x040f,0x0421,0x0434,0x0435,0x0410,0x0810,0x0411,0x044b,0x0457,
        0x0412,0x0812,0x0440,0x0426,0x0427,0x0827,0x042f,0x043e,0x083e,0x044c,0x0481,0x043a,
        0x044e,0x0450,0x0414,0x0814,0x0415,0x0416,0x0816,0x0446,0x046b,0x086b,0x0c6b,0x0418,
        0x0419,0x044f,0x043b,0x083b,0x0c3b,0x103b,0x143b,0x183b,0x1c3b,0x203b,0x243b,0x0c1a,
        0x1c1a,0x081a,0x181a,0x046c,0x0432,0x041b,0x0424,0x040a,0x080a,0x0c0a,0x100a,0x140a,
        0x180a,0x1c0a,0x200a,0x240a,0x280a,0x2c0a,0x300a,0x340a,0x380a,0x3c0a,0x400a,0x440a,
        0x480a,0x4c0a,0x500a,0x0430,0x0441,0x041d,0x081d,0x045a,0x0449,0x0444,0x044a,0x041e,
        0x041f,0x0422,0x0420,0x0820,0x0443,0x0843,0x042a,0x0452
    };

    LCID    lcid, lcidMain;
    int     nResult;
    TCHAR   szLangCode[4];
    TCHAR   szLangCodeMain[4];

    TRACE(_T("Identifier\tThree-letter Abbrev. lang. Name\n"));
    for ( int i=0; i< sizeof(id)/sizeof(id[0]); i++ )
    {
        lcid = MAKELCID( id[i], SORT_DEFAULT );
        LANGID langid = id[i];      // Without sub language
        int nPrimaryLang = PRIMARYLANGID(langid);
        int nSubLang = SUBLANGID(langid);
        lcidMain = MAKELCID(MAKELANGID(nPrimaryLang, SUBLANG_NEUTRAL), SORT_DEFAULT);
        lstrcpy(szLangCode, _T("==="));
        lstrcpy(szLangCodeMain, _T("==="));
        nResult = ::GetLocaleInfo(lcid, LOCALE_SABBREVLANGNAME, szLangCode, 4);
        nResult = ::GetLocaleInfo(lcidMain, LOCALE_SABBREVLANGNAME, szLangCodeMain, 4);
        TRACE(_T("0x%04X,\t%s,\t%s\n"), lcid, szLangCode, szLangCodeMain );
    }
    return 0;
}
#endif
/* From Window XP Professional SP2 Build 2600 050301-1519
0x0436  AFK     AFK  Afrikaans
0x041c  SQI     SQI  Albanian
0x0401  ARA     ARA  Arabic (Saudi Arabia)
0x0801  ARI     ARA  Arabic (Iraq)
0x0c01  ARE     ARA  Arabic (Egypt)
0x1001  ARL     ARA  Arabic (Libya)
0x1401  ARG     ARA  Arabic (Algeria)
0x1801  ARM     ARA  Arabic (Morocco)
0x1c01  ART     ARA  Arabic (Tunisia)
0x2001  ARO     ARA  Arabic (Oman)
0x2401  ARY     ARA  Arabic (Yemen)
0x2801  ARS     ARA  Arabic (Syria)
0x2c01  ARJ     ARA  Arabic (Jordan)
0x3001  ARB     ARA  Arabic (Lebanon)
0x3401  ARK     ARA  Arabic (Kuwait)
0x3801  ARU     ARA  Arabic (U.A.E.)
0x3c01  ARH     ARA  Arabic (Bahrain)
0x4001  ARQ     ARA  Arabic (Qatar)
0x042b  HYE     HYE  Windows 2000/XP: Armenian. This is Unicode only.
0x042c  AZE     AZE  Azeri (Latin)
0x082c  AZE     AZE  Azeri (Cyrillic)
0x042d  EUQ     EUQ  Basque
0x0423  BEL     BEL  Belarusian
0x0445  BNG     BNG  Bengali (India)
0x141a  BSB     HRV  Bosnian (Bosnia and Herzegovina)
0x0402  BGR     BGR  Bulgarian
0x0455  ===     ===  Burmese
0x0403  CAT     CAT  Catalan
0x0404  CHT     CHT  Chinese (Taiwan)
0x0804  CHS     CHT  Chinese (PRC)
0x0c04  ZHH     CHT  Chinese (Hong Kong SAR, PRC)
0x1004  ZHI     CHT  Chinese (Singapore)
0x1404  ZHM     CHT  Windows 98/Me, Windows 2000/XP: Chinese (Macao SAR)
0x041a  HRV     HRV  Croatian
0x101a  HRB     HRV  Croatian (Bosnia and Herzegovina)
0x0405  CSY     CSY  Czech
0x0406  DAN     DAN  Danish
0x0465  DIV     DIV  Windows XP: Divehi. This is Unicode only.
0x0413  NLD     NLD  Dutch (Netherlands)
0x0813  NLB     NLD  Dutch (Belgium)
0x0409  ENU     ENU  English (United States)
0x0809  ENG     ENU  English (United Kingdom)
0x0c09  ENA     ENU  English (Australian)
0x1009  ENC     ENU  English (Canadian)
0x1409  ENZ     ENU  English (New Zealand)
0x1809  ENI     ENU  English (Ireland)
0x1c09  ENS     ENU  English (South Africa)
0x2009  ENJ     ENU  English (Jamaica)
0x2409  ENB     ENU  English (Caribbean)
0x2809  ENL     ENU  English (Belize)
0x2c09  ENT     ENU  English (Trinidad)
0x3009  ENW     ENU  Windows 98/Me, Windows 2000/XP: English (Zimbabwe)
0x3409  ENP     ENU  Windows 98/Me, Windows 2000/XP: English (Philippines)
0x0425  ETI     ETI  Estonian
0x0438  FOS     FOS  Faeroese
0x0429  FAR     FAR  Farsi
0x040b  FIN     FIN  Finnish
0x040c  FRA     FRA  French (Standard)
0x080c  FRB     FRA  French (Belgian)
0x0c0c  FRC     FRA  French (Canadian)
0x100c  FRS     FRA  French (Switzerland)
0x140c  FRL     FRA  French (Luxembourg)
0x180c  FRM     FRA  Windows 98/Me, Windows 2000/XP: French (Monaco)
0x0456  GLC     GLC  Windows XP: Galician
0x0437  KAT     KAT  Windows 2000/XP: Georgian. This is Unicode only.
0x0407  DEU     DEU  German (Standard)
0x0807  DES     DEU  German (Switzerland)
0x0c07  DEA     DEU  German (Austria)
0x1007  DEL     DEU  German (Luxembourg)
0x1407  DEC     DEU  German (Liechtenstein)
0x0408  ELL     ELL  Greek
0x0447  GUJ     GUJ  Windows XP: Gujarati. This is Unicode only.
0x040d  HEB     HEB  Hebrew
0x0439  HIN     HIN  Windows 2000/XP: Hindi. This is Unicode only.
0x040e  HUN     HUN  Hungarian
0x040f  ISL     ISL  Icelandic
0x0421  IND     IND  Indonesian
0x0434  XHO     XHO  isiXhosa/Xhosa (South Africa)
0x0435  ZUL     ZUL  isiZulu/Zulu (South Africa)
0x0410  ITA     ITA  Italian (Standard)
0x0810  ITS     ITA  Italian (Switzerland)
0x0411  JPN     JPN  Japanese
0x044b  KAN     KAN  Windows XP: Kannada. This is Unicode only.
0x0457  KNK     KNK  Windows 2000/XP: Konkani. This is Unicode only.
0x0412  KOR     KOR  Korean
0x0812  ===     KOR  Windows 95, Windows NT 4.0 only: Korean (Johab)
0x0440  KYR     KYR  Windows XP: Kyrgyz.
0x0426  LVI     LVI  Latvian
0x0427  LTH     LTH  Lithuanian
0x0827  ===     LTH  Windows 98 only: Lithuanian (Classic)
0x042f  MKI     MKI  Macedonian (FYROM)
0x043e  MSL     MSL  Malay (Malaysian)
0x083e  MSB     MSL  Malay (Brunei Darussalam)
0x044c  MYM     MYM  Malayalam (India)
0x0481  MRI     MRI  Maori (New Zealand)
0x043a  MLT     MLT  Maltese (Malta)
0x044e  MAR     MAR  Windows 2000/XP: Marathi. This is Unicode only.
0x0450  MON     MON  Windows XP: Mongolian
0x0414  NOR     NOR  Norwegian (Bokmal)
0x0814  NON     NOR  Norwegian (Nynorsk)
0x0415  PLK     PLK  Polish
0x0416  PTB     PTB  Portuguese (Brazil)
0x0816  PTG     PTB  Portuguese (Portugal)
0x0446  PAN     PAN  Windows XP: Punjabi. This is Unicode only.
0x046b  QUB     QUB  Quechua (Bolivia)
0x086b  QUE     QUB  Quechua (Ecuador)
0x0c6b  QUP     QUB  Quechua (Peru)
0x0418  ROM     ROM  Romanian
0x0419  RUS     RUS  Russian
0x044f  SAN     SAN  Windows 2000/XP: Sanskrit. This is Unicode only.
0x043b  SME     SME  Sami, Northern (Norway)
0x083b  SMF     SME  Sami, Northern (Sweden)
0x0c3b  SMG     SME  Sami, Northern (Finland)
0x103b  SMJ     SME  Sami, Lule (Norway)
0x143b  SMK     SME  Sami, Lule (Sweden)
0x183b  SMA     SME  Sami, Southern (Norway)
0x1c3b  SMB     SME  Sami, Southern (Sweden)
0x203b  SMS     SME  Sami, Skolt (Finland)
0x243b  SMN     SME  Sami, Inari (Finland)
0x0c1a  SRB     HRV  Serbian (Cyrillic)
0x1c1a  SRN     HRV  Serbian (Cyrillic, Bosnia, and Herzegovina)
0x081a  SRL     HRV  Serbian (Latin)
0x181a  SRS     HRV  Serbian (Latin, Bosnia, and Herzegovina)
0x046c  NSO     NSO  Sesotho sa Leboa/Northern Sotho (South Africa)
0x0432  TSN     TSN  Setswana/Tswana (South Africa)
0x041b  SKY     SKY  Slovak
0x0424  SLV     SLV  Slovenian
0x040a  ESP     ESP  Spanish (Spain, Traditional Sort)
0x080a  ESM     ESP  Spanish (Mexican)
0x0c0a  ESN     ESP  Spanish (Spain, Modern Sort)
0x100a  ESG     ESP  Spanish (Guatemala)
0x140a  ESC     ESP  Spanish (Costa Rica)
0x180a  ESA     ESP  Spanish (Panama)
0x1c0a  ESD     ESP  Spanish (Dominican Republic)
0x200a  ESV     ESP  Spanish (Venezuela)
0x240a  ESO     ESP  Spanish (Colombia)
0x280a  ESR     ESP  Spanish (Peru)
0x2c0a  ESS     ESP  Spanish (Argentina)
0x300a  ESF     ESP  Spanish (Ecuador)
0x340a  ESL     ESP  Spanish (Chile)
0x380a  ESY     ESP  Spanish (Uruguay)
0x3c0a  ESZ     ESP  Spanish (Paraguay)
0x400a  ESB     ESP  Spanish (Bolivia)
0x440a  ESE     ESP  Spanish (El Salvador)
0x480a  ESH     ESP  Spanish (Honduras)
0x4c0a  ESI     ESP  Spanish (Nicaragua)
0x500a  ESU     ESP  Spanish (Puerto Rico)
0x0430  ===     ===  Sutu
0x0441  SWK     SWK  Swahili (Kenya)
0x041d  SVE     SVE  Swedish
0x081d  SVF     SVE  Swedish (Finland)
0x045a  SYR     SYR  Windows XP: Syriac. This is Unicode only.
0x0449  TAM     TAM  Windows 2000/XP: Tamil. This is Unicode only.
0x0444  TTT     TTT  Tatar (Tatarstan)
0x044a  TEL     TEL  Windows XP: Telugu. This is Unicode only.
0x041e  THA     THA  Thai
0x041f  TRK     TRK  Turkish
0x0422  UKR     UKR  Ukrainian
0x0420  URD     URD  Windows 98/Me, Windows 2000/XP: Urdu (Pakistan)
0x0820  ===     URD  Urdu (India)
0x0443  UZB     UZB  Uzbek (Latin)
0x0843  UZB     UZB  Uzbek (Cyrillic)
0x042a  VIT     VIT  Windows 98/Me, Windows NT 4.0 and later: Vietnamese
0x0452  CYM     CYM  Welsh (United Kingdom)
*/