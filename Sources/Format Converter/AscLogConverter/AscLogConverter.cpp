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
 * \authors   Venkatanarayana Makam, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation of the AscLogConverter class.
 */

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN        /* Exclude rarely-used stuff from Windows headers */
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER              /* Allow use of features specific to Windows XP or later. */
#define WINVER 0x0501       /* Change this to the appropriate value to target other versions of Windows. */
#endif

#ifndef _WIN32_WINNT        /* Allow use of features specific to Windows XP or later. */
#define _WIN32_WINNT 0x0501 /* Change this to the appropriate value to target other versions of Windows. */
#endif

#ifndef _WIN32_WINDOWS      /* Allow use of features specific to Windows 98 or later. */
#define _WIN32_WINDOWS 0x0410 /* Change this to the appropriate value to target Windows Me or later. */
#endif

#ifndef _WIN32_IE           /* Allow use of features specific to IE 6.0 or later. */
#define _WIN32_IE 0x0600    /* Change this to the appropriate value to target other versions of IE. */
#endif

#include <afxwin.h>         /* MFC core and standard components */
#include <afxext.h>         /* MFC extensions */

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         /* MFC OLE classes */
#include <afxodlgs.h>       /* MFC OLE dialog classes */
#include <afxdisp.h>        /* MFC Automation classes */
#endif /* _AFX_NO_OLE_SUPPORT */

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>          /* MFC ODBC database classes */
#endif /* _AFX_NO_DB_SUPPORT */

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>         /* MFC DAO database classes */
#endif /* _AFX_NO_DAO_SUPPORT */

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>       /* MFC support for Internet Explorer 4 Common Controls */
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>         /* MFC support for Windows Common Controls */
#endif /* _AFX_NO_AFXCMN_SUPPORT */
#include <afxdllx.h>

/* Project includes */
#include "AscLogConverter.h"

#ifdef _MANAGED
#error Please read instructions in AscLogConverter.cpp to compile with /clr
// If you want to add /clr to your project you must do the following:
//  1. Remove the above include for afxdllx.h
//  2. Add a .cpp file to your project that does not have /clr thrown and has
//     Precompiled headers disabled, with the following text:
//          #include <afxwin.h>
//          #include <afxdllx.h>
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

/**
 * \brief Constructor
 *
 * Constructor of CAscLogConverter
 */
CAscLogConverter::CAscLogConverter(void)
{
}

/**
 * \brief Destructor
 *
 * Destructor of CAscLogConverter
 */
CAscLogConverter::~CAscLogConverter(void)
{
}

/**
 * \brief      Get help text
 * \param[out] pchHelpText Help Text
 * \return     Result code
 *
 * Returns pchHelpText containing the help text.
 */
HRESULT CAscLogConverter::GetHelpText(string& pchHelpText)
{
    pchHelpText = "Converts the CANoe log file(.asc) to BUSMASTER log file(.log)";
    return S_OK;
}

/**
 * \brief      Get converter name
 * \param[out] strConverterName Converter Name
 * \return     Result code
 *
 * Returns strConverterName containing the converter name.
 */
HRESULT CAscLogConverter::GetConverterName(string& strConverterName)
{
    strConverterName = "ASC TO LOG Conversion";
    return S_OK;
}

/**
 * \brief      Get error status string
 * \param[in]  hResult Error code
 * \param[out] omstrStatus Corresponding error string
 * \return     Result code
 *
 * Returns omstrStatus containing the error string depending on hResult.
 */
HRESULT CAscLogConverter::GetErrorStatus(HRESULT hResult, string& omstrStatus)
{
    switch( hResult )
    {
        case S_OK:
            m_omstrConversionStatus = "Conversion success";
            break;

        case S_FALSE:
            m_omstrConversionStatus = "Conversion failed";
            break;

        default:
            m_omstrConversionStatus = "Unknown";
            break;
    }

    return S_OK;
}

/**
 * \brief      Get input file filter type and name
 * \param[out] pchInputDefFilters file filter types
 * \param[out] pchInputFilters file filter name
 * \return     Result code
 *
 * Returns strings containing the file extensions and a
 * corresponding filter description.
 */
HRESULT CAscLogConverter::GetInputFileFilters(string& pchInputDefFilters, string& pchInputFilters)
{
    pchInputDefFilters = "asc";
    pchInputFilters = "CANoe Log File(s) (*.asc)|*.asc||";
    return S_OK;
}

/**
 * \brief      Get last conversion status
 * \param[out] hResult Last conversion status.
 * \param[out] omstrStatus String describing the last conversion status.
 * \return     Result code
 *
 * Returns a string containing the last conversion status.
 */
HRESULT CAscLogConverter::GetLastConversionStatus(HRESULT& hResult, string& omstrStatus)
{
    hResult = m_hResult;
    omstrStatus = m_omstrConversionStatus;
    return S_OK;
}

/**
 * \brief      Get output file filter type and name
 * \param[out] pchOutputDefFilters file filter types
 * \param[out] pchOutputFilters file filter name
 * \return     Result code
 *
 * Returns strings containing the file extensions and a
 * corresponding filter description.
 */
HRESULT CAscLogConverter::GetOutputFileFilters(string& pchOutputDefFilters, string& pchOutputFilters)
{
    pchOutputDefFilters = "log";
    pchOutputFilters = "BUSMASTER Log File(s) (*.log)|*.log||";
    return S_OK;
}

/**
 * \brief     Conversion function
 * \param[in] chInputFile Input file name to convert from
 * \param[in] chOutputFile Output file name to convert to
 * \return    Result code
 *
 * This is the actual conversion function with input and output file name.
 */
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

/**
 * \brief     Returns if it has an own window
 * \return    True, if it has an own window.
 *
 * This returns true, if the converter has an own window, false otherwise.
 */
BOOL CAscLogConverter::bHaveOwnWindow()
{
    return FALSE;
}
