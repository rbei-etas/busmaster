// CAPL2CConverter.cpp : Defines the initialization routines for the DLL.
//

#include "CAPL2CConverter_stdafx.h"
#include <afxdllx.h>
#include "CAPL2CConverter.h"
#ifdef _MANAGED
#error Please read instructions in CAPL2CConverter.cpp to compile with /clr
// If you want to add /clr to your project you must do the following:
//  1. Remove the above include for afxdllx.h
//  2. Add a .cpp file to your project that does not have /clr thrown and has
//     Precompiled headers disabled, with the following text:
//          #include <afxwin.h>
//          #include <afxdllx.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//
#include "..\FormatConverterApp\MultiLanguage.h"


static AFX_EXTENSION_MODULE CAPL2CConverterDLL = { NULL, NULL };

#ifdef _MANAGED
#pragma managed(push, off)
#endif


extern "C" int APIENTRY

DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    static HINSTANCE shLangInst=NULL;

    // Remove this if you use lpReserved
    UNREFERENCED_PARAMETER(lpReserved);

    if (dwReason == DLL_PROCESS_ATTACH)
    {
        TRACE0("CAPL2CConverter.DLL Initializing!\n");

        // Extension DLL one-time initialization
        if (!AfxInitExtensionModule(CAPL2CConverterDLL, hInstance))
        {
            return 0;
        }

        // Insert this DLL into the resource chain
        // NOTE: If this Extension DLL is being implicitly linked to by
        //  an MFC Regular DLL (such as an ActiveX Control)
        //  instead of an MFC application, then you will want to
        //  remove this line from DllMain and put it in a separate
        //  function exported from this Extension DLL.  The Regular DLL
        //  that uses this Extension DLL should then explicitly call that
        //  function to initialize this Extension DLL.  Otherwise,
        //  the CDynLinkLibrary object will not be attached to the
        //  Regular DLL's resource chain, and serious problems will
        //  result.


        // Begin of Multiple Language support
        if ( CMultiLanguage::m_nLocales <= 0 )
            // Not detected yet
        {
            CMultiLanguage::DetectLangID(); // Detect language as user locale
            CMultiLanguage::DetectUILanguage(); // Detect language in MUI OS
        }
        TCHAR szModuleFileName[MAX_PATH]; // Get Module File Name and path
        int ret = ::GetModuleFileName(hInstance, szModuleFileName, MAX_PATH);
        if ( ret == 0 || ret == MAX_PATH )
        {
            ASSERT(FALSE);
        }
        // Load resource-only language DLL. It will use the languages
        // detected above, take first available language,
        // or you can specify another language as second parameter to
        // LoadLangResourceDLL. And try that first.
        shLangInst =  CMultiLanguage::LoadLangResourceDLL( szModuleFileName );
        if (shLangInst)
        {
            CAPL2CConverterDLL.hResource = shLangInst;
        }
        // End of Multiple Language support

        new CDynLinkLibrary(CAPL2CConverterDLL);

    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        TRACE0("CAPL2CConverter.DLL Terminating!\n");

        // Terminate the library before destructors are called
        AfxTermExtensionModule(CAPL2CConverterDLL);
    }
    return 1;   // ok
}

#ifdef _MANAGED
#pragma managed(pop)
#endif



extern "C" __declspec(dllexport) HRESULT GetBaseConverter(CBaseConverter*& pouConverter)
{
    pouConverter = new CCAPL2CConverter();
    return S_OK;
}