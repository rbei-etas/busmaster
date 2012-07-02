
#include "stdafx.h"
#include "BusmasterDump.h"

string CBusmasterDump::m_strAppName;

CBusmasterDump::CBusmasterDump( string strAppName )
{
    // Two instances of BUSMASTER dump is not allowed
    ASSERT( m_strAppName.length() > 0 );
    m_strAppName = strAppName;
    ::SetUnhandledExceptionFilter( ExceptionFilter );
}

LONG CBusmasterDump::ExceptionFilter( struct _EXCEPTION_POINTERS* pExceptionInfo )
{
    LONG lRetval = EXCEPTION_CONTINUE_SEARCH;
    HWND hParent = NULL;                        // find a better value for your app

    HMODULE hDll = NULL;
    char szDbgHelpPath[_MAX_PATH];

    if (GetModuleFileName( NULL, szDbgHelpPath, _MAX_PATH ))
    {
        string strDbgHelpPath = szDbgHelpPath;
        strDbgHelpPath = strDbgHelpPath.find_last_of("\\");

        if (strDbgHelpPath.length() > 0)
        {
            strDbgHelpPath += "DBGHELP.DLL";
            hDll = ::LoadLibrary( strDbgHelpPath.c_str() );
        }
    }

    if (hDll==NULL)
    {
        // Load the default from windows path
        hDll = ::LoadLibrary( "DBGHELP.DLL" );
    }

    string strResult = "";

    if (hDll)
    {
        MINIDUMPWRITEDUMP pDump = (MINIDUMPWRITEDUMP)::GetProcAddress( hDll, "MiniDumpWriteDump" );
        if (pDump)
        {
            char szDumpPath[_MAX_PATH];
            char szScratch [_MAX_PATH];
            string strDumpPath;
            string strDumpMsg;

            // Place the dump file in Temp folder
            if (!GetTempPath( _MAX_PATH, szDumpPath ))
            {
                strDumpPath = "c:\\temp\\";
            }

            strDumpPath = szDumpPath;
            strDumpPath += m_strAppName;
            strDumpPath += ".dmp";

            // Prompt the user for saving the busmaster dump file
            if (::MessageBox( NULL, "Unhandled exception in BUSMASTER. Would you like to save a dump file?",
                              m_strAppName.c_str(), MB_YESNO ) == IDYES)
            {
                // Create the dump file as per path given
                HANDLE hFile = ::CreateFile( strDumpPath.c_str (), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
                                             FILE_ATTRIBUTE_NORMAL, NULL );

                if (hFile!=INVALID_HANDLE_VALUE)
                {
                    _MINIDUMP_EXCEPTION_INFORMATION ExInfo;

                    ExInfo.ThreadId = ::GetCurrentThreadId();
                    ExInfo.ExceptionPointers = pExceptionInfo;
                    ExInfo.ClientPointers = NULL;

                    // Write the crash dump
                    BOOL bRetValue = pDump( GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL );
                    if (bRetValue)
                    {
                        strDumpMsg = "Saved dump file to " + strDumpPath;
                        lRetval = EXCEPTION_EXECUTE_HANDLER;
                    }
                    else
                    {
                        strDumpMsg = "Failed to save dump file to " + strDumpPath;
                    }

                    strResult = strDumpMsg;
                    ::CloseHandle(hFile);
                }
                else
                {
                    strDumpMsg = "Failed to create dump file " + strDumpPath;
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
        ::MessageBox( NULL, strResult.c_str(), m_strAppName.c_str(), MB_OK );
    }

    return lRetval;
}
