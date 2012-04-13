// AscLogConverter.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
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
 * \file      AscLogConverter.cpp
 * \brief     Implementation of the AscLogConverter class and DLLMain Function.
 * \author    Venkatanarayana Makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the AscLogConverter class.
 */
#include <afxdllx.h>
#include "AscLogConverter.h"

#ifdef _MANAGED
#error Please read instructions in AscLogConverter.cpp to compile with /clr
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


static AFX_EXTENSION_MODULE AscLogConverterDLL = { NULL, NULL };

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
		TRACE0("AscLogConverter.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(AscLogConverterDLL, hInstance))
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

		new CDynLinkLibrary(AscLogConverterDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("AscLogConverter.DLL Terminating!\n");

		// Terminate the library before destructors are called
		AfxTermExtensionModule(AscLogConverterDLL);
	}
	return 1;   // ok
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

extern "C" int nConvertFile(FILE* fpInputFile, FILE* fpOutputFile);
extern "C" __declspec(dllexport) HRESULT GetBaseConverter(CBaseConverter*& pouConverter)
{
    pouConverter = new CAscLogConverter();
    return S_OK;
}

CAscLogConverter::CAscLogConverter(void)
{
}

HRESULT CAscLogConverter::GetHelpText(string& pchHelpText)
{
    pchHelpText = "Converts the CANoe log file(.asc) to BUSMASTER log file(.log)";
    return S_OK;
}

HRESULT CAscLogConverter::GetConverterName(string& strConverterName)
{
	strConverterName = "ASC TO LOG Conversion";
    return S_OK;
}

HRESULT CAscLogConverter::GetErrorStatus(HRESULT hResult, string& omstrStatus)
{
    switch( hResult )
    {
        /*case ERR_OUTPUT_FILE_NOTFOUND:
            m_omstrConversionStatus = "Output File path is not found";
            break;
        case ERR_INPUT_FILE_NOTFOUND:
            m_omstrConversionStatus = "Input File path is not found";
            break;*/
        case S_OK:
            m_omstrConversionStatus = "Conversion success";
            break;
        case S_FALSE:
            m_omstrConversionStatus = "Conversion failed";
            break;
        default:
            m_omstrConversionStatus = "Unknown Error";
            break;
    }   
    return S_OK;
}

HRESULT CAscLogConverter::GetInputFileFilters(string& pchInputDefFilters, string& pchInputFilters)
{
    pchInputDefFilters = "asc";
    pchInputFilters = "CANoe Log File(s) (*.asc)|*.asc||";
    return S_OK;
}

HRESULT CAscLogConverter::GetLastConversionStatus(HRESULT& hResult, string& omstrStatus)
{
    hResult = m_hResult;
    omstrStatus = m_omstrConversionStatus;
    return S_OK;
}

HRESULT CAscLogConverter::GetOutputFileFilters(string& pchOutputDefFilters, string& pchOutputFilters)
{
    pchOutputDefFilters = "log";
    pchOutputFilters = "BUSMASTER Log File(s) (*.log)|*.log||";
    return S_OK;
}

HRESULT CAscLogConverter::ConvertFile(string& chInputFile, string& chOutputFile)
{
    HRESULT hResult = S_OK;
    FILE* fpInputFile = NULL;
    FILE* fpOutputFile = NULL;
    fpInputFile = _tfopen(chInputFile.c_str(), _T("r"));
    if(NULL != fpInputFile)
    {
        fpOutputFile = _tfopen(chOutputFile.c_str(), _T("w"));
        if(NULL != fpOutputFile)
        {
            //yydebug = 1;
            fprintf(fpOutputFile, "%s\n", "***BUSMASTER Ver 1.6.2***");
            fprintf(fpOutputFile, "%s\n", "***NOTE: PLEASE DO NOT EDIT THIS DOCUMENT***");
            fprintf(fpOutputFile, "%s\n", "***[START LOGGING SESSION]***");
            fprintf(fpOutputFile, "%s", "***START DATE AND TIME ");
            nConvertFile(fpInputFile, fpOutputFile);
            m_omstrConversionStatus = _T("Conversion Completed Successfully"); 
            fclose(fpInputFile);
            fclose(fpOutputFile);
        }
        else
        {
            m_omstrConversionStatus = _T("Output File path is not found");
            hResult = ERR_OUTPUT_FILE_NOTFOUND;
        }
    }
    else
    {
         m_omstrConversionStatus = _T("Input File path is not found");
        hResult = ERR_INPUT_FILE_NOTFOUND;
    }
    m_hResult = hResult;
    return hResult;
}

BOOL CAscLogConverter::bHaveOwnWindow()
{
    return FALSE;
}

CAscLogConverter::~CAscLogConverter(void)
{
};