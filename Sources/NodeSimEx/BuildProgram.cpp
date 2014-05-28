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
 * \file      BuildProgram.cpp
 * \brief     This file contain the definition all member function of
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain the definition all member function of
 */
// For all standard header file include
#include "NodeSimEx_stdafx.h"
// Definition of CBuildProgram class
#include "BuildProgram.h"
#include "Export_UserDllCAN.h"
#include "SimSysManager.h"
#include "GlobalObj.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
/******************************************************************************/
/*  Function Name    :  CBuildProgram                                         */
/*  Input(s)         :  Module handle                                         */
/*  Output           :                                                        */
/*  Functionality    :  Assign the value in passed in parameter to data member*/
/*                      variable and initialise all other data members        */
/*  Member of        :  CBuildProgram                                         */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  07.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modified On      :  20.03.2002                                            */
/******************************************************************************/
CBuildProgram::CBuildProgram(ETYPE_BUS eBus, HMODULE hModuleHandle /* nullptr*/):
    m_hModuleHandle(hModuleHandle),m_podOutWnd(nullptr)
{
    m_eBus = eBus;
}
/******************************************************************************/
/*  Function Name    :  CBuildProgram                                         */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Default destructor which will destroy the output      */
/*                      if it is already created.                             */
/*  Member of        :  CBuildProgram                                         */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  07.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modified On      :  28.03.2002, Destroy the output window                 */
/******************************************************************************/
CBuildProgram::~CBuildProgram()
{
    if(m_podOutWnd != nullptr)
    {
        m_podOutWnd->DestroyWindow();
    }
}
/******************************************************************************/
/*  Function Name    :  bBuildProgram                                         */
/*  Input(s)         :  PSNODEINFO and flag to load DLL                 */
/*  Output           :  TRUE or FALSE                                         */
/*  Functionality    :  Build the DLL for source file and display output      */
/*  Member of        :  CBuildProgram                                         */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  07.03.2002                                            */
/******************************************************************************/
BOOL CBuildProgram::bBuildProgram(PSNODEINFO psNodeInfo, BOOL bLoadDLL)
{
    BOOL bReturn            = FALSE;
    CString omStrFilePath   = "";
    CString omStrErrorMsg   = "";
    int iMajorVer = 0;
    int iMinorVer = 0;
    BOOL bVersion = FALSE ;
    CString omFileName=psNodeInfo->m_omStrFileName;
    CString omLongFileName=omFileName;
    PROCESS_INFORMATION sProcessInfo;
    STARTUPINFO         sStartInfo;
    SECURITY_ATTRIBUTES sSecurityAttr;
    CWaitCursor omWait;
    // Check if file name passed as parameter is not empty
    if(omFileName.IsEmpty()==FALSE)
    {
        // Remove all items in this string array before adding.
        m_omStrArray.RemoveAll();
        m_omStrArray.Add(" ");
        m_omStrArray.Add(omFileName);
        m_omStrSourceFilename = omFileName;

        LONG lError = 0;
        //HKEY sKey;
        BYTE acGCCPath[1024];

        // Get the installation path for FRAME
        lError = ERROR_SUCCESS;
        /*lError = RegOpenKeyEx(  HKEY_LOCAL_MACHINE,
                 defREGISTRY_LOCATION,
                 0,
                 KEY_READ,
                 &sKey);*/
        // If the registry key open successfully, get the value in "path"
        // sub key
        if(lError==ERROR_SUCCESS)
        {
            HANDLE hStdInput        = nullptr; // Standard input handle of the child process.
            HANDLE hStdOutput       = nullptr; // - Standard output handle of the child process.
            HANDLE hStdError        = nullptr; //- Standard error handle of the child process.

            lError = ERROR_SUCCESS;
            /*lError = RegQueryValueEx(sKey,_T("path"),0, &ulType, acGCCPath,&dwSize);
            RegCloseKey(sKey);*/
            // Check if value is read successfully.
            if(lError !=ERROR_SUCCESS)
            {
                m_omStrArray.Add(defERROR_IN_REGISTRY);
                // Add string to list box attached to output window after
                // displaying it if not displayed already.
                bAddString(m_omStrArray);

                bReturn = FALSE;
                return bReturn;
            }
            // create an output file will be used by create process to send
            // output of process.
            sSecurityAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
            sSecurityAttr.bInheritHandle = TRUE;
            sSecurityAttr.lpSecurityDescriptor = nullptr;

            omStrFilePath   = omFileName.Left(omFileName.ReverseFind('\\'));
            omStrFilePath  += "\\output.txt";

            hStdOutput =  CreateFile(LPCTSTR(omStrFilePath) ,
                                     GENERIC_WRITE|GENERIC_READ, // access mode
                                     FILE_SHARE_WRITE  |FILE_SHARE_WRITE,
                                     &sSecurityAttr,
                                     CREATE_ALWAYS,
                                     FILE_ATTRIBUTE_NORMAL,
                                     nullptr ) ;

            // check if file is open successfully
            if(hStdOutput== INVALID_HANDLE_VALUE)
            {
                DWORD dwError = GetLastError();
                omStrErrorMsg.Format("%s: %d",
                                     "Error in opening output file",
                                     dwError);
                m_omStrArray.Add(omStrErrorMsg);
                // Add string to list box attached to output window after
                // displaying it if not displayed already.
                bAddString(m_omStrArray);

                bReturn = FALSE;
                return bReturn;
            }

            // Create an error file which will be used by create process to put
            // all error message generated.
            sSecurityAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
            sSecurityAttr.bInheritHandle = TRUE;
            sSecurityAttr.lpSecurityDescriptor = nullptr;

            omStrFilePath   = omFileName.Left(omFileName.ReverseFind('\\'));
            omStrFilePath  += "\\error.txt";

            hStdError =  CreateFile(LPCTSTR(omStrFilePath),
                                    GENERIC_WRITE|GENERIC_READ, // access mode
                                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                                    &sSecurityAttr,
                                    CREATE_ALWAYS,
                                    FILE_ATTRIBUTE_NORMAL,
                                    nullptr );

            // check if error file is created successfully
            if(hStdError == INVALID_HANDLE_VALUE)
            {
                DWORD dwError =   GetLastError();
                omStrErrorMsg.Format("%s: %d",
                                     "Error in opening error file",
                                     dwError);
                m_omStrArray.Add(omStrErrorMsg);
                // Add string to list box attached to output window after
                // displaying it if not displayed already.
                bAddString(m_omStrArray);

                bReturn = FALSE;
                // close output if error file is not created
                // successfully
                CloseHandle(hStdOutput);
                return bReturn;
            }

            // create input file which will be used by create process
            sSecurityAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
            sSecurityAttr.bInheritHandle = TRUE;
            sSecurityAttr.lpSecurityDescriptor = nullptr;

            omStrFilePath   = omFileName.Left(omFileName.ReverseFind('\\'));
            omStrFilePath  += "\\input.txt";

            hStdInput =  CreateFile(LPCTSTR(omStrFilePath),
                                    GENERIC_WRITE|GENERIC_READ,    // access mode
                                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                                    &sSecurityAttr,
                                    CREATE_ALWAYS,
                                    FILE_ATTRIBUTE_NORMAL,
                                    nullptr );

            // Check if input file created successfully
            if(hStdInput == INVALID_HANDLE_VALUE)
            {
                DWORD dwError =   GetLastError();
                omStrErrorMsg.Format("%s: %d",
                                     "Error in opening input file",
                                     dwError);
                m_omStrArray.Add(omStrErrorMsg);
                // Add string to list box attached to output window after
                // displaying it if not displayed already.
                bAddString(m_omStrArray);

                // close output and error files if input file is not created
                // successfully
                CloseHandle(hStdError);
                CloseHandle(hStdOutput);
                bReturn = FALSE;
                return bReturn;
            }

            // Create process using input, output and error file handle.
            // Set up the start up info struct.
            ZeroMemory(&sProcessInfo, sizeof(sProcessInfo) );
            ZeroMemory(&sStartInfo,sizeof(STARTUPINFO));
            sStartInfo.cb           = sizeof(STARTUPINFO);
            //to use the specified handles
            sStartInfo.dwFlags      = STARTF_USESTDHANDLES;
            sStartInfo.hStdOutput   = hStdOutput;
            sStartInfo.hStdInput    = hStdInput;
            sStartInfo.hStdError    = hStdError;
            // Use this if you want to hide the child:
            sStartInfo.wShowWindow  = SW_HIDE;

            // Get the make file template path.
            CString omStrMakeFileTemplateName;
            omStrMakeFileTemplateName  = acGCCPath;
            CString omTempGccFile = CGlobalObj::ouGetObj(m_eBus).m_omAppDirectory;
            /*kadoor*/
            omStrMakeFileTemplateName = omTempGccFile;
            CString omGccMakeFileName = "\\GCCDLLMakeTemplate";
            switch (m_eBus)
            {
                case CAN:
                    omGccMakeFileName += "_CAN";
                    break;
                case J1939:
                    omGccMakeFileName += "_J1939";
                    break;
                case LIN:
                    omGccMakeFileName += "_LIN";
                    break;
            }

            omStrMakeFileTemplateName += omGccMakeFileName;

            // Create make file in the path where source file to by compiled is
            // Located
            CString omStrTemp          = omFileName.Left(
                                             omFileName.ReverseFind('\\'));
            CString omStrMakeFileName  = omStrTemp;
            omStrMakeFileName         += "\\DLLMake";
            bReturn = bCreateMakeFile(omStrMakeFileTemplateName,
                                      omStrMakeFileName);
            char acStrShortPath[1024];
            dwConvertShortPathName(omStrMakeFileName,acStrShortPath);

            // check if make file is created successfully.
            if(bReturn == TRUE)
            {
                CString omStrGccMakeFile = "";
                CString omStrGccParamter = "";
                CString omStrGCCPath     = "";
                // Create process after giving the command line input
                omStrGccMakeFile  = acGCCPath;

                /*kadoor*/
                omStrGccMakeFile = omTempGccFile;
                /*omStrGccMakeFile += "\\MinGW\\bin";
                omStrGCCPath      = omStrGccMakeFile;*/
                omStrGccMakeFile = "mingw32-make.exe";
                omStrGccParamter  =  "--silent --file=";
                omStrGccParamter += acStrShortPath;

                char* pucGccMakeFile  = (TCHAR*)omStrGccMakeFile.GetBuffer(
                                            omStrGccMakeFile.GetLength());
                char* pucGccParameter = (TCHAR*)omStrGccParamter.GetBuffer(
                                            omStrGccParamter.GetLength());
                char* pucGCCPath      = (TCHAR*)omStrGCCPath.GetBuffer(
                                            omStrGCCPath.GetLength());
                CString omStrCommandParam = pucGccMakeFile ;
                omStrCommandParam += " ";
                omStrCommandParam += pucGccParameter;
                INT nSuccess = CreateProcess( nullptr, omStrCommandParam.GetBuffer(MAX_PATH),
                                              nullptr, nullptr,
                                              true, CREATE_NO_WINDOW,
                                              nullptr, nullptr,
                                              &sStartInfo, &sProcessInfo);

                // check if process is not created successfully else
                // wait for the process to to complete with time out value
                if (nSuccess==0)
                {
                    DWORD dwError =   GetLastError();
                    LPTSTR pcMessageBuffer;
                    DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
                                          FORMAT_MESSAGE_IGNORE_INSERTS |
                                          FORMAT_MESSAGE_FROM_SYSTEM ;

                    FormatMessage(
                        dwFormatFlags,
                        nullptr, // module to get message from (nullptr == system)
                        dwError,
                        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
                        (LPTSTR) &pcMessageBuffer,
                        0,
                        nullptr
                    );

                    omStrErrorMsg.Format("%s: %s",
                                         "Invoking MinGW gcc compiler Failed",
                                         pcMessageBuffer);
                    LocalFree(pcMessageBuffer);
                    m_omStrArray.Add(omStrErrorMsg);
                    omStrErrorMsg = "Refer \"MinGW Installation\" Section in BUSMASTER Help File for Further Information";
                    m_omStrArray.Add(omStrErrorMsg);
                    // Add string to list box attached to output window after
                    // displaying it if not displayed already.
                    bAddString(m_omStrArray);

                    omStrGccParamter.ReleaseBuffer(omStrGccParamter.GetLength());
                    omStrGccMakeFile.ReleaseBuffer(omStrGccMakeFile.GetLength());
                    omStrGCCPath.ReleaseBuffer(omStrGCCPath.GetLength());

                    // close all three files if create process is not successfull
                    CloseHandle(hStdError);
                    CloseHandle(hStdOutput);
                    CloseHandle(hStdInput);
                    bReturn = FALSE;
                    return bReturn;
                }
                else
                {
                    WaitForSingleObject(sProcessInfo.hProcess, INFINITE );
                }

                omStrGccParamter.ReleaseBuffer(omStrGccParamter.GetLength());
                omStrGccMakeFile.ReleaseBuffer(omStrGccMakeFile.GetLength());
                omStrGCCPath.ReleaseBuffer(omStrGCCPath.GetLength());
                // close the created files
                CloseHandle(hStdError);
                CloseHandle(hStdOutput);
                CloseHandle(hStdInput);

                // Get the output or error message from the corresponding file
                // and add it to CStringArray
                omStrFilePath   = omFileName.Left(omFileName.ReverseFind('\\'));
                omStrFilePath  += "\\error.txt";
                bAddStrToArrayFromFile(omStrFilePath,m_omStrArray);
                // to generate a message that dll is created successfully.
                // If there is no message in error file -> DLL created
                // successfully
                INT nTotalSize = (COMMANINT)m_omStrArray.GetSize();
                CString omString;
                BOOL bSuccess = TRUE;
                for(INT nCount=0; nCount<nTotalSize; nCount++)
                {
                    omString = m_omStrArray.GetAt(nCount);
                    // Find if the omString has keyword "Error"
                    if ( omString.Find("Error") != -1 )
                    {
                        bSuccess = FALSE;
                    }
                }

                omStrFilePath.Replace("error.txt","output.txt");
                bAddStrToArrayFromFile(omStrFilePath,m_omStrArray);

                CString omStrDLLFile = "";
                if(bSuccess == TRUE)
                {
                    CString omStrFileNameInSDLLFile=STR_EMPTY;
                    CString omStrFileNameInLongFileName=STR_EMPTY;
                    CString omStrDLLFile   = "";
                    char acStrShortPath[1000];
                    dwConvertShortPathName(omFileName,acStrShortPath);
                    omFileName    = acStrShortPath;
                    omStrDLLFile  = omFileName.Left(omFileName.ReverseFind('.'));
                    omStrDLLFile += defDOT_DLL;
                    int nLength=omStrDLLFile.GetLength();
                    nLength-=omStrDLLFile.ReverseFind('\\');
                    nLength--;
                    omStrFileNameInSDLLFile=omStrDLLFile.
                                            Right(nLength);
                    int nLastDotPlace=omLongFileName.ReverseFind('.');
                    omLongFileName=omLongFileName.Left(nLastDotPlace);
                    omLongFileName+= defDOT_DLL;
                    nLength=omLongFileName.GetLength();
                    nLength-=omLongFileName.ReverseFind('\\');
                    nLength--;
                    omStrFileNameInLongFileName=omLongFileName.
                                                Right(nLength);
                    if(omStrFileNameInLongFileName.
                            CompareNoCase(omStrFileNameInSDLLFile))
                    {
                        BOOL bCopyFileStatus=COPYFILE(omStrDLLFile,omLongFileName);
                        if(bCopyFileStatus!=FALSE)
                        {
                            DeleteFile(omStrDLLFile);
                        }
                    }
                    psNodeInfo->m_omStrDllName=omLongFileName;
                    omLongFileName +=" created successfully";
                    m_omStrArray.Add(omLongFileName);
                }
                else
                {
                    // Error in building DLL
                    omStrDLLFile  ="Error in creating ";
                    omStrDLLFile += omLongFileName.Left(omLongFileName.ReverseFind('.'));
                    omStrDLLFile += defDOT_DLL;
                    m_omStrArray.Add(omStrDLLFile);
                    bReturn = FALSE;
                }
                // Add all items in CStringArray to list box attached to
                // output window.after displaying it if not displayed
                bAddString(m_omStrArray);
            }
            else
            {
                // close the created files
                CloseHandle(hStdError);
                CloseHandle(hStdOutput);
                CloseHandle(hStdInput);
                bReturn = FALSE;
            }

        }
        else
        {
            m_omStrArray.Add(defERROR_OPEN_REGISTRY);
            // Add string to list box attached to output window
            // after displaying it if not displayed already.
            bAddString(m_omStrArray);
            bReturn = FALSE;
        }
        // Load the DLL if the flag passed as parameter is set
        // Unloading of already loaded library is done in CMainFrame.
        if(bLoadDLL==TRUE && bReturn == TRUE)
        {
            int n=omLongFileName.ReverseFind('.');
            omLongFileName=omLongFileName.Left(n);
            omLongFileName+= defDOT_DLL;
            HMODULE hModuleHandle  = nullptr;

            hModuleHandle = LoadLibrary(omLongFileName);
            if(hModuleHandle!=nullptr)
            {
                //check for the version number
                PFGET_PRG_VER pFGetPrgVer = (PFGET_PRG_VER)
                                            GetProcAddress( hModuleHandle,
                                                    defNAME_FUNC_GET_PRG_VER );
                if( pFGetPrgVer != nullptr )
                {
                    bVersion = pFGetPrgVer(&iMajorVer,&iMinorVer,hModuleHandle);
                    if( bVersion == TRUE )
                    {
                        if( iMajorVer != defMAJOR_VERSION &&
                                iMinorVer != defMINOR_VERSION )
                        {
                            CString omStrErrMsg ;
                            omStrErrMsg.Format( defUNSUPPORTED_VER,
                                                iMajorVer,iMinorVer );
                            bReturn = FALSE ;
                            if( FreeLibrary(hModuleHandle) != 0 )
                            {
                                hModuleHandle = nullptr;
                            }
                            // Remove Key Panel entry
                            /*if (CGlobalObj::g_podNodeToDllMap != nullptr)
                            {
                                CGlobalObj::g_podNodeToDllMap->RemoveKey(psNodeInfo->m_omStrDllName);
                            }*/
                            //AfxMessageBox(omStrErrMsg);
                            m_omStrArray.Add(omStrErrMsg);
                            bAddString(m_omStrArray);
                        }
                        else
                        {
                            /*if (CGlobalObj::g_podNodeToDllMap != nullptr)
                            {
                                CGlobalObj::g_podNodeToDllMap->operator [](psNodeInfo->m_omStrDllName) = hModuleHandle;
                                ::PostMessage(CGlobalObj::hWmdMDIParentFrame,WM_LOAD_UNLOAD, 1, 0);
                            }*/
                        }
                    }
                }
                else
                {
                    CString omStrErrMsg ;
                    omStrErrMsg.Format( defOLD_VER, omLongFileName );
                    bReturn = FALSE ;
                    if( FreeLibrary(hModuleHandle) != 0 )
                    {
                        hModuleHandle = nullptr;
                    }
                    //AfxMessageBox(omStrErrMsg);
                    m_omStrArray.Add(omStrErrMsg);
                    bAddString(m_omStrArray);
                }

                if(bReturn)
                {
                    psNodeInfo->m_omStrDllName=omLongFileName;
                    psNodeInfo->m_hModuleHandle=hModuleHandle;
                    // Set the flag for DLL loaded.
                    CSimSysManager::ouGetSimSysManager(m_eBus).
                    ouGetFlags().vSetFlagStatus( H_DLLLOADED, TRUE );
                    //Local array so that only loading message will be
                    //added to the COutWnd list
                    CStringArray m_omStrArray;
                    m_omStrArray.Add(omLongFileName+ " loaded successfully");
                    // Add string to list box attached to output window
                    // after displaying it if not displayed already.
                    bAddString(m_omStrArray);
                }
            }

        }
    }
    return bReturn;
}
/******************************************************************************/
/*  Function Name    :  bCreateMakeFile                                       */
/*  Input(s)         :  Make file template and make file name with path       */
/*  Output           :  TRUE or FALSE                                         */
/*  Functionality    :  Creates a specific make file from a generic make file */
/*                      template.                                             */
/*  Member of        :  CBuildProgram                                         */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  07.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modified On      :  20.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modified On      :  16.12.2002, solved exception due to passing null      */
/*                      pointer to CFileException object to open function by  */
/*                      replacing it with passing a pointer to existing object*/
/******************************************************************************/
BOOL CBuildProgram::bCreateMakeFile(CString& omStrMakeFileTemplateName,
                                    CString& omStrMakeFileName          )
{
    CStdioFile omStdiofile;
    CFileException omException ;
    CString omStrFile            = "";
    BOOL bReturn                 = FALSE;
    DWORD dwSize                 = 0;
    DWORD dwRead                 = 0;
    BOOL bFileOpen               = FALSE;
    CString omStrErrorMessage     = "";
    char  acErrorMsg[defSIZE_OF_ERROR_BUFFER];
    // Open the make file template.
    TRY
    {
        bFileOpen = omStdiofile.Open(omStrMakeFileTemplateName,
        CFile::modeRead | CFile::typeText, &omException);
        if(bFileOpen!=FALSE)
        {
            CHAR* pcBuff   = nullptr;
            CString omTemp = "";
            // Get the size of file
            dwSize = (DWORD)omStdiofile.GetLength();
            //pcBuff = (char*) new char[dwSize];
            pcBuff = (char*) new char[dwSize+1];

            if(pcBuff!=nullptr)
            {
                memset(pcBuff, 0, dwSize);
                // Read the whole file and put the content to pcBuff;
                dwRead = omStdiofile.Read(pcBuff,dwSize);
                // Add end of string;
                *(pcBuff+dwRead) = '\0';
                // copy the content to CString object;
                omStrFile = pcBuff;
                // Delete the buffer and initialise it to nullptr
                delete []pcBuff;
                pcBuff = nullptr;

                char acStrShortPath[1000] ;
                // Convert long name to short name for GCC compiler
                dwConvertShortPathName(omStrMakeFileTemplateName,acStrShortPath);

                omTemp = acStrShortPath;
                INT nIndex = 0;
                // Get the path of make file temptate in temp object
                nIndex = omStrMakeFileTemplateName.ReverseFind('\\');
                if(nIndex>=0)
                {
                    omTemp = omStrMakeFileTemplateName.Left(nIndex);
                    // Replace all occurrence of "<INSTALLDIR>" string with template path
                    omStrFile.Replace("<INSTALLDIR>",omTemp);
                }
                // Replace all occurence of <SRCDIR> and <FILENAME> with source
                // directory and file name respectively.

                nIndex = omTemp.ReverseFind('\\');
                if(nIndex>=0)
                {
                    nIndex = m_omStrSourceFilename.ReverseFind('.');
                    if(nIndex>=0)
                    {
                        omTemp = m_omStrSourceFilename.Left(nIndex);
                        // Replace all occurrence of "<INSTALLDIR>" string with template path
                        omStrFile.Replace("<INPUTFILE>",omTemp);
                    }
                    omTemp = m_omStrSourceFilename;
                    omTemp.Replace('\\', '/');
                    omTemp.Replace(" ", "\\ ");
                    omStrFile.Replace("<CPPFILE>", omTemp);
                    bReturn = TRUE;
                }
            }
            omStdiofile.Close();
        }
        else
        {
            // Get the exception error message
            omException.GetErrorMessage(acErrorMsg,sizeof(acErrorMsg));
            // make file template open error notification
            // AfxMessageBox(acErrorMsg ,MB_ICONERROR| MB_SYSTEMMODAL|MB_OK,0);
            m_omStrArray.Add(acErrorMsg);
            bAddString(m_omStrArray);

        }
        if(bReturn == TRUE)
        {
            bFileOpen = omStdiofile.Open( omStrMakeFileName,
            CFile::modeCreate | CFile::modeWrite,
            &omException);
            if(bFileOpen!=FALSE)
            {
                //AfxMessageBox(omStrFile);
                omStdiofile.WriteString(omStrFile);
                omStdiofile.Close();
            }
            else
            {
                // Get the exception error message
                omException.GetErrorMessage(acErrorMsg,sizeof(acErrorMsg));
                //AfxMessageBox(acErrorMsg ,
                //            MB_ICONERROR| MB_SYSTEMMODAL|MB_OK,
                //          0);
                m_omStrArray.Add(acErrorMsg);
                bAddString(m_omStrArray);
                bReturn = FALSE;
            }
        }

    }
    CATCH_ALL(pomE)
    {
        if(pomE != nullptr )
        {
            // Get the exception error message
            pomE->GetErrorMessage(acErrorMsg,sizeof(acErrorMsg));
            omStrErrorMessage = acErrorMsg;
            //Display the error
            //AfxMessageBox(omStrErrorMessage ,
            //             MB_ICONERROR| MB_SYSTEMMODAL|MB_OK,0);
            m_omStrArray.Add(acErrorMsg);
            bAddString(m_omStrArray);
            pomE->Delete();
        }
        bReturn = FALSE;
    }
    END_CATCH_ALL

    return bReturn;
}
/******************************************************************************/
/*  Function Name    :  bAddStrToArrayFromFile                                */
/*  Input(s)         :  File name and reference to CStringArray object        */
/*  Output           :  TRUE or FALSE                                         */
/*  Functionality    :  Set the text extent to maximum text length of items   */
/*  Member of        :  CBuildProgram                                         */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  07.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modified On      :  20.03.2002                                            */
/******************************************************************************/
BOOL CBuildProgram::bAddStrToArrayFromFile(CString& omStrTextFileName,
        CStringArray& omStrArray)
{
    //   Local Var.
    std::ifstream    InTextFile;
    CString     omStrMainMessage = "";
    CString     omStrLine        = "" ;
    CHAR        cLine[500];
    BOOL        bReturn          = FALSE;

    TRY
    {
        InTextFile.open(/*T2A*/(omStrTextFileName.GetBuffer(MAX_PATH)), std::ios::in);
        if (!InTextFile.good())
        {

            // Info file open error notification
            omStrMainMessage  ="Input file open error : ";
            omStrMainMessage +=omStrTextFileName;
            //AfxMessageBox(omStrMainMessage ,MB_ICONERROR| MB_SYSTEMMODAL|MB_OK,0);
            m_omStrArray.Add(omStrMainMessage);
            bAddString(m_omStrArray);
            bReturn = FALSE;
        }
        else
        {
            //  Read line by line and add string to List box
            while (!InTextFile.eof())
            {
                InTextFile.getline( cLine, sizeof(cLine));
                omStrLine = cLine;
                omStrLine.TrimLeft();
                omStrLine.TrimRight();
                if(omStrLine.IsEmpty()==FALSE)
                {
                    omStrArray.Add(omStrLine);
                }
            }
            InTextFile.close();
        }
    }
    CATCH_ALL(pomException)
    {

        if(pomException != nullptr )
        {
            char scErrorMsg[255];
            CString cStrErrorMessage;
            // Get the exception error message
            pomException->GetErrorMessage(scErrorMsg,sizeof(scErrorMsg));
            cStrErrorMessage = "Failed to get address of Timer Handlers: ";
            cStrErrorMessage += scErrorMsg;
            //Display the error
            //AfxMessageBox(cStrErrorMessage ,MB_ICONERROR| MB_SYSTEMMODAL|MB_OK,0);
            m_omStrArray.Add(cStrErrorMessage);
            bAddString(m_omStrArray);
            pomException->Delete();
        }
        bReturn = FALSE;
    }
    END_CATCH_ALL
    return TRUE;
}
/******************************************************************************/
/*  Function Name    :  bAddString                                            */
/*  Input(s)         :  Reference to Object of CStringArray                   */
/*  Output           :  TRUE or FALSE                                         */
/*  Functionality    :  Creates output window, if it is not created and calls */
/*                      public member function of COutWnd class to add string */
/*                      to output window.                                     */
/*  Member of        :  CBuildProgram                                         */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  07.03.2002                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
BOOL CBuildProgram::bAddString(CStringArray& omStrArray)
{
    // Create output window if it is not arealdy created.
    if(m_podOutWnd ==nullptr)
    {
        vCreateOutputWindow();
    }
    // If it is created, add string item in the list box attached to it.
    if(m_podOutWnd !=nullptr)
    {
        m_podOutWnd->bAddString(omStrArray);
    }
    return TRUE;
}
/******************************************************************************/
/*  Function Name    :  vCreateOutputWindow                                   */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Create output window, if it is not created. It uses   */
/*                      default or last stored coordinates                    */
/*  Member of        :  CBuildProgram                                         */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  07.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modified On      :  20.03.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modified On      :  18.04.2003, new member function  bCreateOutputWindow  */
/*                                  of COutWnd class is called to create it.  */
/******************************************************************************/
VOID CBuildProgram::vCreateOutputWindow()
{
    m_podOutWnd = new COutWnd(m_eBus);
    // check pointer is not nullptr.
    if(m_podOutWnd != nullptr)
    {
        m_podOutWnd->bCreateOutputWindow();
    }
}
/******************************************************************************/
/*  Function Name    :  dwConvertShortPathName                                */
/*  Input(s)         :  String to be converted,String buffer to store         */
/*                      converted string.                                     */
/*  Output           :  buffer length of converted string                     */
/*  Functionality    :  It convertes the string passed as parameter of        */
/*                      CString object and store the short path name in buffer*/
/*                      passed as other parameter. It return the buffer size  */
/*  Member of        :  CBuildProgram                                         */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  28.03.2002                                            */
/*  Modification By  :                                                        */
/*  Modified On      :                                                        */
/******************************************************************************/
DWORD CBuildProgram::dwConvertShortPathName(CONST CString& omStrToConvert,
        char* pcStrShortPath)
{
    DWORD dwBuffer   = 0;
    DWORD dwReturn   = 0;
    dwBuffer = omStrToConvert.GetLength();
    dwReturn = GetShortPathName(omStrToConvert,pcStrShortPath,dwBuffer);
    if(dwBuffer<dwReturn)
    {
        dwReturn = GetShortPathName(omStrToConvert,pcStrShortPath,dwReturn);
    }
    return dwReturn;
}

/******************************************************************************/
/*  Function Name    :  vClearArray                                           */
/*  Input(s)         :  void                                                  */
/*  Output           :  buffer length of converted string                     */
/*  Functionality    :  It clear the string which contains all the messages   */
/*                      of output window                                      */
/*  Member of        :  CBuildProgram                                         */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Anish Kumar                                           */
/*  Date Created     :  14.01.2006                                            */
/******************************************************************************/
void CBuildProgram::vClearArray()
{
    m_omStrArray.RemoveAll();
    if(m_podOutWnd!=nullptr)
    {
        m_podOutWnd->vResetContent();
    }

}
