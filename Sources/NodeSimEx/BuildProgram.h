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
 * \file      BuildProgram.h
 * \brief     This file contain the definition of CBuildProgram class
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition of CBuildProgram class
 */

#pragma once

#include"OutWnd.h"
#include"SimSysNodeInfo.h"
class CBuildProgram
{
public:
    // Build the DLL from the source file and call function to display the output
    BOOL bBuildProgram(PSNODEINFO psNodeInfo,BOOL bLoadDLL);
    // Default contructor
    CBuildProgram(ETYPE_BUS eBus, HMODULE hModuleHandle =NULL);
    // Default destructor
    virtual ~CBuildProgram();
    // Pointer to output window
    COutWnd* m_podOutWnd;
    //Clear the m_omStrArray
    void vClearArray();
    // Add strings to output window;s list box
    BOOL bAddString(CStringArray& omStrArray );

private:
    ETYPE_BUS m_eBus;
    DWORD dwConvertShortPathName(CONST CString& omStrToConvert,
                                 char* pcStrShortPath);
    // Create output window
    VOID vCreateOutputWindow();
    // Get strings from file and add it to string array
    BOOL bAddStrToArrayFromFile (CString& omStrTextFileName,
                                 CStringArray& omStrArray );
    // Create a make file from makefile template
    BOOL bCreateMakeFile(CString& omStrMakeFileTemplateName,
                         CString& omStrMakeFileName);
    // Source file name to be built to DLL
    CString m_omStrSourceFilename;
    // Module handle for DLL
    HMODULE m_hModuleHandle;
    // CString array to hold string to be displayed on output window
    CStringArray m_omStrArray;
};
