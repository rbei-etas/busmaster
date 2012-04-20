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
 * \file      LogToExcelConverter.cpp
 * \brief     Definition of CLogToExcelConverter class
 * \authors   Pradeep Kadoor, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Defines the initialization routines for the DLL.
 */

/* Project includes */
#include "ExportLogFileDlg.h"
#include "LogToExcelConverter.h"

#ifdef _MANAGED
#error Please read instructions in LogToExcelConverter.cpp to compile with /clr
// If you want to add /clr to your project you must do the following:
//	1. Remove the above include for afxdllx.h
//	2. Add a .cpp file to your project that does not have /clr thrown and has
//	   Precompiled headers disabled, with the following text:
//			#include <afxwin.h>
//			#include <afxdllx.h>
#endif

static AFX_EXTENSION_MODULE LogToExcelConverterDLL = { NULL, NULL };

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
		TRACE0("LogToExcelConverter.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(LogToExcelConverterDLL, hInstance))
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

		new CDynLinkLibrary(LogToExcelConverterDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("LogToExcelConverter.DLL Terminating!\n");

		// Terminate the library before destructors are called
		AfxTermExtensionModule(LogToExcelConverterDLL);
	}
	return 1;   // ok
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

extern "C" __declspec(dllexport) HRESULT GetBaseConverter(CBaseConverter*& pouConverter)
{
    pouConverter = new CLogToExcelConverter();
    return S_OK;
}

CLogToExcelConverter::CLogToExcelConverter(void)
{
}

CLogToExcelConverter::~CLogToExcelConverter(void)
{
};

HRESULT CLogToExcelConverter::GetHelpText(string&)
{
    return S_FALSE;
}
HRESULT CLogToExcelConverter::GetConverterName(string&)
{
    return S_FALSE;
}
HRESULT CLogToExcelConverter::GetErrorStatus(HRESULT hResult, string& omstrStatus)
{
    return S_FALSE;
}
HRESULT CLogToExcelConverter::GetInputFileFilters(string& pchInputDefFilters, string& pchInputFilters)
{
    return S_FALSE;
}
HRESULT CLogToExcelConverter::GetLastConversionStatus(HRESULT& hResult, string& omstrStatus)
{
    return S_FALSE;
}

HRESULT CLogToExcelConverter::GetOutputFileFilters(string& pchOutputDefFilters, string& pchOutputFilters)
{
    return S_FALSE;
}
HRESULT CLogToExcelConverter::ConvertFile(string& chInputFile, string& chOutputFile)
{
    return S_FALSE;
}
BOOL CLogToExcelConverter::bHaveOwnWindow()
{
    return TRUE;
}

HRESULT CLogToExcelConverter::GetPropertyPage(CPropertyPage*& pPage)
{
    pPage = new CExportLogFileDlg();
    return S_FALSE;
};

