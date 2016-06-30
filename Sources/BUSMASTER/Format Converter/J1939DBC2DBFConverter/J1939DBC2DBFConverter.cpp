// DBC2DBFConverter.cpp : Defines the initialization routines for the DLL.
//

#include "J1939DBC2DBFConverter_stdafx.h"
#include <afxdllx.h>
#include "J1939DBC2DBFConverter.h"
//#include <locale.h>
//
//#include <libintl.h>
//
//#define _(string) gettext (string)


#ifdef _MANAGED
#error Please read instructions in DBC2DBFConverter.cpp to compile with /clr
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


static AFX_EXTENSION_MODULE J1939DBC2DBFConverterDLL = { NULL, NULL };

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
        TRACE0("DBC2DBFConverter.DLL Initializing!\n");

        // Extension DLL one-time initialization
        if (!AfxInitExtensionModule(J1939DBC2DBFConverterDLL, hInstance))
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

        new CDynLinkLibrary(J1939DBC2DBFConverterDLL);

    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        TRACE0("DBC2DBFConverter.DLL Terminating!\n");

        // Terminate the library before destructors are called
        AfxTermExtensionModule(J1939DBC2DBFConverterDLL);
    }
    return 1;   // ok
}

#ifdef _MANAGED
#pragma managed(pop)
#endif


HRESULT CJ1939DBC2DBFConverter::GettextBusmaster(void)
{
    setlocale(LC_ALL,"");
    bindtextdomain("BUSMASTER", getenv("LOCALDIR") );
    textdomain("BUSMASTER");
    return S_OK;
}


CJ1939DBC2DBFConverter::CJ1939DBC2DBFConverter(void)
{
    m_hDLLModule = NULL;
    m_pouDBC2DBFConverter = NULL;
}
HRESULT CJ1939DBC2DBFConverter::GetHelpText(CString& pchHelpText)
{
    pchHelpText = _("Converts the CANoe J1939 Database(.dbc) file to BUSMASTER J1939 Database(.dbf) file");
    return S_OK;
}

/**
 * \brief      Get converter name
 * \param[out] strConverterName Converter Name
 * \return     Result code
 *
 * Returns strConverterName containing the converter name.
 */
HRESULT CJ1939DBC2DBFConverter::GetConverterName(string& strConverterName)
{
    strConverterName = _("J1939 DBC TO DBF Conversion");
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
HRESULT CJ1939DBC2DBFConverter::GetErrorStatus(HRESULT hResult, string& omstrStatus)
{
    switch( hResult )
    {
        case S_OK:
            m_omstrConversionStatus = _("Conversion success");
            break;

        case S_FALSE:
            m_omstrConversionStatus = _("Conversion failed");
            break;

        default:
            m_omstrConversionStatus = _("Unknown");
            break;
    }

    return S_OK;
}
HRESULT CJ1939DBC2DBFConverter::GetInputFileFilters(string& pchInputDefFilters, string& pchInputFilters)
{
    pchInputDefFilters = "dbc";
    pchInputFilters = _("CANoe Database File(s) (*.dbc)|*.dbc||");
    return S_OK;
}
HRESULT CJ1939DBC2DBFConverter::GetLastConversionStatus(HRESULT& hResult, string& omstrStatus)
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
HRESULT CJ1939DBC2DBFConverter::GetOutputFileFilters(string& pchOutputDefFilters, string& pchOutputFilters)
{
    pchOutputDefFilters = "dbf";
    pchOutputFilters = _("BUSMASTER Database File(s) (*.dbf)|*.dbf||");
    return S_OK;
}

HRESULT CJ1939DBC2DBFConverter::ConvertFile(string& chInputFile, string& chOutputFile)
{
    HRESULT hResult = S_OK;

    if(NULL == m_hDLLModule)
    {
        m_hDLLModule = LoadLibrary("DBC2DBFConverterLibrary.dll");
        m_pfGetConverter = (GETCONVERTER)GetProcAddress(m_hDLLModule, "GetDBCConverter");
        if( NULL != m_pfGetConverter)
        {
            m_pfGetConverter(m_pouDBC2DBFConverter, J1939);
        }
    }
    m_pouDBC2DBFConverter->ClearMsgList();
    INT nRetVal = m_pouDBC2DBFConverter->LoadDBCFile(chInputFile.c_str());
    if( S_OK == nRetVal )
    {
        m_pouDBC2DBFConverter->ConvertFile(chOutputFile.c_str());
    }
    // display final result
    char chResult[1024];
    m_pouDBC2DBFConverter->GetResultString(chResult);
    m_omstrConversionStatus = chResult;
    if(nRetVal != 0)
    {
        //m_omstrConversionStatus += ouConverter.m_omLogFilePath.data();
        m_hResult = nRetVal;
    }
    return hResult;
}

BOOL CJ1939DBC2DBFConverter::bHaveOwnWindow()
{
    return FALSE;
}
CJ1939DBC2DBFConverter::~CJ1939DBC2DBFConverter(void)
{
    if( NULL != m_pouDBC2DBFConverter )
    {
        delete m_pouDBC2DBFConverter;
    }
    if( NULL != m_hDLLModule)
    {
        FreeLibrary(m_hDLLModule);
    }
};



//Exported Funtions

extern "C" __declspec(dllexport) HRESULT GetBaseConverter(CBaseConverter*& pouConverter)
{
    pouConverter = new CJ1939DBC2DBFConverter();
    return S_OK;
}
