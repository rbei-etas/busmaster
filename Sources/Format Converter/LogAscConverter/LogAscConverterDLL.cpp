// LogAscConverter.cpp : Defines the initialization routines for the DLL.
//

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		/* Exclude rarely-used stuff from Windows headers */
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				/* Allow use of features specific to Windows XP or later. */
#define WINVER 0x0501		/* Change this to the appropriate value to target other versions of Windows. */
#endif

#ifndef _WIN32_WINNT		/* Allow use of features specific to Windows XP or later. */
#define _WIN32_WINNT 0x0501	/* Change this to the appropriate value to target other versions of Windows. */
#endif						

#ifndef _WIN32_WINDOWS		/* Allow use of features specific to Windows 98 or later. */
#define _WIN32_WINDOWS 0x0410 /* Change this to the appropriate value to target Windows Me or later. */
#endif

#ifndef _WIN32_IE			/* Allow use of features specific to IE 6.0 or later. */
#define _WIN32_IE 0x0600	/* Change this to the appropriate value to target other versions of IE. */
#endif

#include <afxwin.h>         /* MFC core and standard components */
#include <afxext.h>         /* MFC extensions */

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         /* MFC OLE classes */
#include <afxodlgs.h>       /* MFC OLE dialog classes */
#include <afxdisp.h>        /* MFC Automation classes */
#endif /* _AFX_NO_OLE_SUPPORT */

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			/* MFC ODBC database classes */
#endif /* _AFX_NO_DB_SUPPORT */

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			/* MFC DAO database classes */
#endif /* _AFX_NO_DAO_SUPPORT */

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		/* MFC support for Internet Explorer 4 Common Controls */
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			/* MFC support for Windows Common Controls */
#endif /* _AFX_NO_AFXCMN_SUPPORT */
#include <afxdllx.h>

/* Project includes */
#include "LogAscConverter.h"

#ifdef _MANAGED
#error Please read instructions in LogAscConverter.cpp to compile with /clr
// If you want to add /clr to your project you must do the following:
//	1. Remove the above include for afxdllx.h
//	2. Add a .cpp file to your project that does not have /clr thrown and has
//	   Precompiled headers disabled, with the following text:
//			#include <afxwin.h>
//			#include <afxdllx.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
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
			return 0;

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
