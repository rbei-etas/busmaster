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
 * @brief Minidump support
 * @copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This header file contains the CBusmasterDump class.
 */

#pragma once

#include "dbghelp.h"

/** Minidump signature from MSDN */
typedef BOOL (WINAPI* MINIDUMPWRITEDUMP)
(HANDLE hProcess, DWORD dwProcId, HANDLE hFile, MINIDUMP_TYPE dumpType,
 CONST PMINIDUMP_EXCEPTION_INFORMATION exceptionParams,
 CONST PMINIDUMP_USER_STREAM_INFORMATION sserStreamParams,
 CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam );

class CBusmasterDump
{
public:
    CBusmasterDump(std::string strAppName);

private:
    static std::string m_strAppName;

    static LONG WINAPI ExceptionFilter( struct _EXCEPTION_POINTERS* pExceptionInfo );
};
