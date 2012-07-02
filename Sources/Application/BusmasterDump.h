
#include "dbghelp.h"

// Mindump signature from MSDN
typedef BOOL (WINAPI* MINIDUMPWRITEDUMP)
(HANDLE hProcess, DWORD dwProcId, HANDLE hFile, MINIDUMP_TYPE dumpType,
 CONST PMINIDUMP_EXCEPTION_INFORMATION exceptionParams,
 CONST PMINIDUMP_USER_STREAM_INFORMATION sserStreamParams,
 CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam );

class CBusmasterDump
{
private:
    static string m_strAppName;

    static LONG WINAPI ExceptionFilter( struct _EXCEPTION_POINTERS* pExceptionInfo );

public:
    CBusmasterDump( string strAppName );
};
