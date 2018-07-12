// DBC2DBFConverterDll.cpp : Defines the initialization routines for the DLL.
//

#include "DBC2DBFConverterLibrary_stdafx.h"
#include <afxdllx.h>
/*
#include "../DBC2DBFConverterLib/DBCConverterBase.h"
#include "../DBC2DBFConverterLib/Converter.h"
#include "../DBC2DBFConverterLib/message.h"
*/
#include "DBCConverterBase.h"
#include "DBFConverter.h"
#include "DBFMessage.h"
#include "DBFTagDefinitions.h"
#ifdef _MANAGED
#error Please read instructions in DBC2DBFConverterDll.cpp to compile with /clr
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


static AFX_EXTENSION_MODULE DBC2DBFConverterDllDLL = { NULL, NULL };

#ifdef _MANAGED
#pragma managed(push, off)
#endif

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    // Remove this if you use lpReserved
    UNREFERENCED_PARAMETER(lpReserved);

    if (dwReason == DLL_PROCESS_ATTACH)
    {
        TRACE0("DBC2DBFConverterDll.DLL Initializing!\n");

        // Extension DLL one-time initialization
        if (!AfxInitExtensionModule(DBC2DBFConverterDllDLL, hInstance))
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

        new CDynLinkLibrary(DBC2DBFConverterDllDLL);

    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        TRACE0("DBC2DBFConverterDll.DLL Terminating!\n");

        // Terminate the library before destructors are called
        AfxTermExtensionModule(DBC2DBFConverterDllDLL);
    }
    return 1;   // ok
}

#ifdef _MANAGED
#pragma managed(pop)
#endif


GETCONVERTER pfGetConverter;

extern "C" __declspec(dllexport) HRESULT GetDBCConverter(CDBCConverterBase*& ouDBCConverter, ETYPE_BUS eBusType)
{
    ouDBCConverter = new CDBFConverter(eBusType);

    return S_OK;
}