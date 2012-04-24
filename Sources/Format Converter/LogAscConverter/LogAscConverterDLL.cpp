/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      LogAscConverter.cpp
 * \brief     Defines the initialization routines for the DLL.
 * \authors   Venkatanarayana Makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Defines the initialization routines for the DLL.
 */

#define VC_EXTRALEAN        /* Exclude rarely-used stuff from Windows headers */

#include <afxwin.h>         /* MFC core and standard components */
#include <afxext.h>         /* MFC extensions */

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>         /* MFC support for Windows Common Controls */
#endif /* _AFX_NO_AFXCMN_SUPPORT */
#include <afxdllx.h>

/* Project includes */
#include "LogAscConverter.h"

#ifdef _MANAGED
#error Please read instructions in LogAscConverter.cpp to compile with /clr
// If you want to add /clr to your project you must do the following:
//  1. Remove the above include for afxdllx.h
//  2. Add a .cpp file to your project that does not have /clr thrown and has
//     Precompiled headers disabled, with the following text:
//          #include <afxwin.h>
//          #include <afxdllx.h>
#endif

static AFX_EXTENSION_MODULE LogAscConverterDLL = { NULL, NULL };

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
        TRACE0("LogAscConverter.DLL Initializing!\n");

        // Extension DLL one-time initialization
        if (!AfxInitExtensionModule(LogAscConverterDLL, hInstance))
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
        new CDynLinkLibrary(LogAscConverterDLL);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        TRACE0("LogAscConverter.DLL Terminating!\n");
        // Terminate the library before destructors are called
        AfxTermExtensionModule(LogAscConverterDLL);
    }

    return 1;   // ok
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

extern "C" __declspec(dllexport) HRESULT GetBaseConverter(CBaseConverter*& pouConverter)
{
    pouConverter = new CLogAscConverter();
    return S_OK;
}
