
#include "stdafx.h"
#include "BusmasterDump.h"
//#include "GettextBusmaster.h"
#include "Utility\MultiLanguageSupport.h"

std::string CBusmasterDump::m_strAppName;

CBusmasterDump::CBusmasterDump( std::string strAppName )
{
    // Two instances of BUSMASTER dump is not allowed
    m_strAppName = strAppName;
    //ASSERT( m_strAppName.length() > 0 );
    ::SetUnhandledExceptionFilter( ExceptionFilter );
}

LONG CBusmasterDump::ExceptionFilter( struct _EXCEPTION_POINTERS* pExceptionInfo )
{
    LONG lRetval = EXCEPTION_CONTINUE_SEARCH;

    HMODULE hDll = nullptr;
    char szDbgHelpPath[_MAX_PATH];

    if (GetModuleFileName( nullptr, szDbgHelpPath, _MAX_PATH ))
    {
        std::string strDbgHelpPath = szDbgHelpPath;
        strDbgHelpPath = strDbgHelpPath.find_last_of("\\");

        if (!strDbgHelpPath.empty())
        {
            strDbgHelpPath += "DBGHELP.DLL";
            hDll = ::LoadLibrary( strDbgHelpPath.c_str() );
        }
    }

    if (hDll==nullptr)
    {
        // Load the default from windows path
        hDll = ::LoadLibrary( "DBGHELP.DLL" );
    }

    std::string strResult = "";

    if (hDll)
    {
        MINIDUMPWRITEDUMP pDump = (MINIDUMPWRITEDUMP)::GetProcAddress( hDll, "MiniDumpWriteDump" );
        if (pDump)
        {
            char szDumpPath[_MAX_PATH];
            std::string strDumpPath;
            std::string strDumpMsg;

            // Place the dump file in Temp folder
            if (!GetTempPath( _MAX_PATH, szDumpPath ))
            {
                strDumpPath = "c:\\temp\\";
            }

            strDumpPath = szDumpPath;
            strDumpPath += m_strAppName;
            strDumpPath += ".dmp";

            // Prompt the user for saving the busmaster dump file
            if (::MessageBox( nullptr, _("Unhandled exception in BUSMASTER. Would you like to save a dump file?"),
                              m_strAppName.c_str(), MB_YESNO ) == IDYES)
            {
                // Create the dump file as per path given
                HANDLE hFile = ::CreateFile( strDumpPath.c_str (), GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS,
                                             FILE_ATTRIBUTE_NORMAL, nullptr );

                if (hFile!=INVALID_HANDLE_VALUE)
                {
                    _MINIDUMP_EXCEPTION_INFORMATION ExInfo;

                    ExInfo.ThreadId = ::GetCurrentThreadId();
                    ExInfo.ExceptionPointers = pExceptionInfo;
                    ExInfo.ClientPointers = 0;

                    // Write the crash dump
                    BOOL bRetValue = pDump( GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, nullptr, nullptr );
                    if (bRetValue)
                    {
                        strDumpMsg = _("Saved dump file to ") + strDumpPath;
                        lRetval = EXCEPTION_EXECUTE_HANDLER;
                    }
                    else
                    {
                        strDumpMsg = _("Failed to save dump file to ") + strDumpPath;
                    }

                    strResult = strDumpMsg;
                    ::CloseHandle(hFile);
                }
                else
                {
                    strDumpMsg = _("Failed to create dump file ") + strDumpPath;
                    strResult = strDumpMsg;
                }
            }
        }
        else
        {
            strResult = "DBGHELP.DLL too old";
        }
    }
    else
    {
        strResult = "DBGHELP.DLL not found";
    }

    if (strResult.length() > 0)
    {
        ::MessageBox( nullptr, strResult.c_str(), m_strAppName.c_str(), MB_OK );
    }

    return lRetval;
}
