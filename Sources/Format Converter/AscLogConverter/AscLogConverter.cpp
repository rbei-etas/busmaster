// AscLogConverter.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
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
HRESULT CAscLogConverter::GetHelpText(TCHAR* pchHelpText)
{
    if(pchHelpText != NULL)
    {
        _tcscpy(pchHelpText, _T("Converts the CANoe log file(.asc) to BUSMASTER log file(.log)"));
        return S_OK;
    }
    return S_FALSE;
}

HRESULT CAscLogConverter::GetConverterName(TCHAR* strConverterName)
{
    if(strConverterName != NULL)
    {
        strcpy(strConverterName, _T("ASC TO LOG Conversion"));
    }
    return S_OK;
}
HRESULT CAscLogConverter::GetErrorStatus(HRESULT hResult, CString& omstrStatus)
{
    switch( hResult )
    {
        /*case ERR_OUTPUT_FILE_NOTFOUND:
            m_omstrConversionStatus = _T("Output File path is not found");
            break;
        case ERR_INPUT_FILE_NOTFOUND:
            m_omstrConversionStatus = _T("Input File path is not found");
            break;*/
        case S_OK:
            m_omstrConversionStatus = _T("Conversion success");
            break;
        case S_FALSE:
            m_omstrConversionStatus = _T("Conversion failed");
            break;
        default:
            m_omstrConversionStatus = _T("Unknown Error");
            break;
    }   
    return S_OK;
}
HRESULT CAscLogConverter::GetInputFileFilters(TCHAR* pchInputDefFilters, TCHAR* pchInputFilters)
{
    HRESULT hResult = S_FALSE;
    if(NULL != pchInputDefFilters)
    {
        _tcscpy(pchInputDefFilters, _T("asc"));
        hResult = S_OK;
    }
    else
    {
        hResult = S_FALSE;
    }
    if(NULL != pchInputFilters)
    {
        _tcscpy(pchInputFilters, _T("CANoe Log File(s) (*.asc)|*.asc||"));
        hResult = S_OK;
    }
    return hResult;
}
HRESULT CAscLogConverter::GetLastConversionStatus(HRESULT& hResult, CString& omstrStatus)
{
    hResult = m_hResult;
    omstrStatus = m_omstrConversionStatus;
    return S_OK;
}

HRESULT CAscLogConverter::GetOutputFileFilters(TCHAR* pchOutputDefFilters, TCHAR* pchOutputFilters)
{
    HRESULT hResult = S_FALSE;
    if(NULL != pchOutputDefFilters)
    {
        _tcscpy(pchOutputDefFilters, _T("log"));
        hResult = S_OK;
    }
    else
    {
        hResult = S_FALSE;
    }
    if(NULL != pchOutputFilters)
    {
        _tcscpy(pchOutputFilters, _T("BUSMASTER Log File(s) (*.log)|*.log||"));
        hResult = S_OK;
    }
    return hResult;
}
HRESULT CAscLogConverter::ConvertFile(TCHAR* chInputFile, TCHAR* chOutputFile)
{
    HRESULT hResult = S_OK;
    /*extern FILE*  yyin;
	extern FILE*  yyout;
    */
    FILE* fpInputFile = NULL;
    FILE* fpOutputFile = NULL;
    fpInputFile = _tfopen(chInputFile, _T("r"));
    if(NULL != fpInputFile)
    {
        fpOutputFile = _tfopen(chOutputFile, _T("w"));
        if(NULL != fpOutputFile)
        {
            //yydebug = 1;
            fprintf(fpOutputFile, "%s\n", "***BUSMASTER Ver 1.4.0***");
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