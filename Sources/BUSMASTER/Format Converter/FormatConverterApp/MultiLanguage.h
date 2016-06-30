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
// MultiLanguage.h: interface for the CMultiLanguage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTILANGUAGE_H__327ABDCD_358B_4F65_8FA7_D999878B4574__INCLUDED_)
#define AFX_MULTILANGUAGE_H__327ABDCD_358B_4F65_8FA7_D999878B4574__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// LONG_PTR Signed long type for pointer precision.
// Use when casting a pointer to a long to perform pointer arithmetic.
typedef signed long LONG_PTR;
#define     MAX_NUM_LCID    10

class CMultiLanguage
{
public:
    CMultiLanguage();
    virtual ~CMultiLanguage();

    // Attributes
public:
    static  LCID    m_alcidSearch[MAX_NUM_LCID];
    static  int     m_nLocales;


    // Operations
public:
    static HINSTANCE    LoadLangResourceDLL( LPCTSTR ModuleName, LANGID langUpdateId =
                MAKELANGID( LANG_NEUTRAL, SUBLANG_NEUTRAL ) );
    static LANGID       DetectUILanguage( void );
    static LANGID       DetectLangID( void );
    static int          ResetDetectedLanguage();
#ifdef  _DEBUG
    static int  PrintThreeLetterLanguageCodeList( void );
#endif


    // Implementation
protected:
    static BOOL CALLBACK _AfxEnumResLangProc(HMODULE /*hModule*/, LPCTSTR /*pszType*/,
            LPCTSTR /*pszName*/, WORD langid, LONG_PTR lParam);
    static LCID AddLocale( LCID lcid );
    static LCID AddLangId( LANGID langid );
};

#endif // !defined(AFX_MULTILANGUAGE_H__327ABDCD_358B_4F65_8FA7_D999878B4574__INCLUDED_)
