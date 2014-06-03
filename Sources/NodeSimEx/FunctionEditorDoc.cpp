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
 * \file      FunctionEditorDoc.cpp
 * \brief     This file contain definition of all function of
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of
 */
#pragma once
#include "NodeSimEx_stdafx.h"
#include "GlobalObj.h"
#include "Application/StdAfx.h"
#include "Export_userdllCAN.h"     // For getting name of a permanent exported 
// function from the user-defined DLL.
#include "FunctionEditorDoc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFunctionEditorDoc

IMPLEMENT_DYNCREATE(CFunctionEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CFunctionEditorDoc, CDocument)
    //{{AFX_MSG_MAP(CFunctionEditorDoc)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/******************************************************************************/
/*  Function Name    :  CFunctionEditorDoc                                    */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Constructor                                           */
/*  Member of        :  CFunctionEditorDoc                                    */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  05.03.2002                                            */
/*  Modifications    :  Amitesh Bharti                                        */
/*                      19.02.2003, Added m_omErrorHandlerArray member varible*/
/*                      element removing code.                                */
/******************************************************************************/

CFunctionEditorDoc::CFunctionEditorDoc()
{
    m_dwSourceCodeLineNo = 0;
    m_omMessageHandlerArray.RemoveAll();
    m_omTimerHandlerArray.RemoveAll();
    m_omEventIndArray.RemoveAll();
    m_omKeyHandlerArray.RemoveAll();
    m_omUtilityFuncArray.RemoveAll();
    m_omGlobalVariableArray.RemoveAll();
    m_omFunctionNameArray.RemoveAll();
    m_omErrorHandlerArray.RemoveAll();
    m_omDLLHandlerArray.RemoveAll();
    m_omBusEventsHandlerArray.RemoveAll();
    m_nMaxLineLength = 0;
    m_lCurrentWarningLineNum = 0;
}
/******************************************************************************/
/*  Function Name    :  ~CFunctionEditorDoc                                   */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Destructor
/*  Member of        :  CFunctionEditorDoc
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  05.03.2002                                            */
/*  Modifications    :
/******************************************************************************/
CFunctionEditorDoc::~CFunctionEditorDoc()
{
}
BOOL CFunctionEditorDoc::bCreateNewDocument(CString& omStrFileName )
{
    BOOL bSuccess = FALSE;
    FILE* pCFile = fopen( /*T2A*/(omStrFileName.GetBuffer(MAX_PATH)), "at");
    CString omStrCFileName = "";
    CString omStr = "";
    // To get the application version
    CString omstrAppVersion = GetApplicationVersion();

    if( pCFile != nullptr )
    {
        bSuccess = TRUE;

        char buffer[2500]= {'\0'};
        // Adding the version and Protocol information to the .c file
        CString omstrCopyWriteInformation = COPYWRITE_INFORMATION;
        CString omstrCFileVersion = C_FILE_VERSION;
        CString omstrBusMasterVersion = BUSMASTER_VERSION;
        CString omstrProtocol = PROTOCOL;

        // Adding Protocol information
        if(m_sBusSpecInfo.m_eBus == CAN)
        {
            omstrProtocol.Replace("PLACE_HOLDER_FOR_PROTOCOL_VALUE", DATABASE_PROTOCOL_CAN);
        }
        else if(m_sBusSpecInfo.m_eBus == J1939)
        {
            omstrProtocol.Replace("PLACE_HOLDER_FOR_PROTOCOL_VALUE", DATABASE_PROTOCOL_J1939);
        }
        else if(m_sBusSpecInfo.m_eBus == LIN)
        {
            omstrProtocol.Replace("PLACE_HOLDER_FOR_PROTOCOL_VALUE", DATABASE_PROTOCOL_LIN);
        }

        // adding application version information
        omstrBusMasterVersion.Replace("PLACE_HOLDER_FOR_BUSMASTER_VERSION", omstrAppVersion);

        omstrCFileVersion.Replace("PLACE_HOLDER_FOR_C_FILE_VERSION", C_FILE_VERSION_VALUE);

        strcat(buffer, (omstrCopyWriteInformation.GetBuffer(MAX_PATH)));
        strcat(buffer, (omstrCFileVersion.GetBuffer(MAX_PATH)));
        strcat(buffer, (omstrBusMasterVersion.GetBuffer(MAX_PATH)));
        strcat(buffer, (omstrProtocol.GetBuffer(MAX_PATH)));
        strcat(buffer, "\n\n");

        CString omStr = BUS_INCLUDE_HDR;
        omStr.Replace("PLACE_HODLER_FOR_BUSNAME", m_sBusSpecInfo.m_omBusName);
        strcat(buffer ,/*T2A*/(omStr.GetBuffer(MAX_PATH)));
        strcat(buffer, "\n#include <Windows.h>");
        if (m_sBusSpecInfo.m_omHeaderFileName.IsEmpty())
        {
            ASSERT(FALSE);
        }

        omStr.Format(STR_INCLUDE_FILE, m_sBusSpecInfo.m_omHeaderFileName);
        strcat(buffer, /*T2A*/(omStr.GetBuffer(MAX_PATH)));

        strcat(buffer, "\n");

        CStringArray* paomstrDBFiles = &(CGlobalObj::ouGetObj(m_sBusSpecInfo.m_eBus).m_omDefinedMsgHeaders);
        int nTotalCount = (COMMANINT)paomstrDBFiles->GetSize();
        for(int nCount = 0 ; nCount < nTotalCount ; nCount++)
        {
            omStr = paomstrDBFiles->GetAt(nCount);
            if ( !omStr.IsEmpty())
            {
                omStr.Insert( 0,"#include \"");
                omStr += "\"";
                strcat(buffer, /*T2A*/(omStr));
                strcat(buffer, "\n");
            }
        }
        strcat(buffer, "\n");
        omStr = BUS_INCLUDE_FOOTER;
        omStr.Replace("PLACE_HODLER_FOR_BUSNAME", m_sBusSpecInfo.m_omBusName);
        strcat(buffer, /*T2A*/(omStr));

        strcat(buffer, "\n\n\n");

        omStr = BUS_VAR_HDR;
        omStr.Replace("PLACE_HODLER_FOR_BUSNAME", m_sBusSpecInfo.m_omBusName);
        strcat(buffer, /*T2A*/(omStr));

        strcat(buffer, "\n\n");

        omStr = BUS_VAR_FOOTER;
        omStr.Replace("PLACE_HODLER_FOR_BUSNAME", m_sBusSpecInfo.m_omBusName);
        strcat(buffer, /*T2A*/(omStr));

        strcat(buffer, "\n\n\n");

        omStr = BUS_FN_PROTOTYPE_HDR;
        omStr.Replace("PLACE_HODLER_FOR_BUSNAME", m_sBusSpecInfo.m_omBusName);
        strcat(buffer, /*T2A*/(omStr));

        strcat(buffer, "\n");

        omStr = BUS_FN_PROTOTYPE_FOOTER;
        omStr.Replace("PLACE_HODLER_FOR_BUSNAME", m_sBusSpecInfo.m_omBusName);
        strcat(buffer, /*T2A*/(omStr));

        strcat(buffer, "\n\n");
        int nLength = (COMMANINT)strlen(buffer);
        fwrite( buffer, sizeof( char ), nLength, pCFile );
        fclose(pCFile);
        pCFile = nullptr;
    }
    else
    {
        AfxMessageBox("Error in creating c file");
    }

    return bSuccess;
}

/******************************************************************************/
/*  Function Name    :  OnNewDocument                                         */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :  BOOL                                                  */
/*  Functionality    :  Constructs the new document and updates the source
                        list.
/*  Member of        :  CFunctionEditorDoc
/*  Friend of        :      -                                                 */
/******************************************************************************/
BOOL CFunctionEditorDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
    {
        return FALSE;
    }

    UINT unCount  = 1;
    BOOL bStop    = FALSE;
    CString omStrCFileName = "";
    CString omStr = "";
    CString strFilePath;
    // Get current working directory, and title
    // and form the file path
    char cBuffer[_MAX_PATH];
    _getcwd( cBuffer, _MAX_PATH );
    // file-attribute information
    struct _tfinddata_t fileinfo;
    // Find if the current directory has some .c file already created.
    // Check if it has name "NewEdn" if yes, the new file name will be
    // NewEdx where value of x = n+1;
    while (bStop == FALSE)
    {
        omStrCFileName = "NewEd";
        omStr = "";
        omStr.Format( "%d", unCount);
        omStr += ".c";
        omStrCFileName += omStr;
        // Search for the file name and if it is not present, set
        // the flag to TRUE to break the loop.
        if (_tfindfirst( omStrCFileName.GetBuffer(MAX_PATH), &fileinfo) == -1L)
        {
            strFilePath = cBuffer ;
            strFilePath += "\\"+ omStrCFileName ;
            if (!CGlobalObj::ouGetObj(m_sBusSpecInfo.m_eBus).pGetDocPtrOfFile(strFilePath))
            {
                //If returned doc pointer is nullptr means a file with same
                //name is not opened for editing
                bStop = TRUE;
            }
        }
        unCount++;
    }
    // Set the root tree view text to file name with path. The path
    // information will be used for build in case user select to build
    // without saving.
    CGlobalObj::ouGetObj(m_sBusSpecInfo.m_eBus).m_omStrSourceFilePathName = strFilePath;
    // Set the title after removing the file extension. This title will be
    // the default file name in save file explorer dialog appended with .c
    omStrCFileName = omStrCFileName.Left(omStrCFileName.ReverseFind('.'));
    SetPathName(CGlobalObj::ouGetObj(m_sBusSpecInfo.m_eBus).m_omStrSourceFilePathName);
    //SetTitle(omStrCFileName);
    // Include windows.h header file.
    m_omSourceCodeTextList.AddTail( BUS_INCLUDE_HDR );
    omStr   = "#include <Windows.h>";
    m_omSourceCodeTextList.AddTail( omStr );
    m_omIncludeFileArray.Add( omStr );

    // Include struct.h header file.
    if (CGlobalObj::ouGetObj(m_sBusSpecInfo.m_eBus).m_omMsgStructFile.IsEmpty())
    {
        ASSERT(FALSE);
    }
    omStr.Format(STR_INCLUDE_FILE, CGlobalObj::ouGetObj(m_sBusSpecInfo.m_eBus).m_omMsgStructFile);
    m_omSourceCodeTextList.AddTail( omStr );
    m_omIncludeFileArray.Add( omStr );
    m_dwSourceCodeLineNo = 6;
    // Get file path of unions.h from app class
    // and if valid insert into the header namespace

    CStringArray* paomstrDBFiles = &(CGlobalObj::ouGetObj(m_sBusSpecInfo.m_eBus).m_omDefinedMsgHeaders);
    int nTotalCount = (COMMANINT)paomstrDBFiles->GetSize();
    for(int nCount = 0 ; nCount < nTotalCount ; nCount++)
    {
        omStr = paomstrDBFiles->GetAt(nCount);
        if ( !omStr.IsEmpty())
        {
            omStr.Insert( 0,"#include \"");
            omStr += "\"";
            m_omSourceCodeTextList.AddTail( omStr);
            m_dwSourceCodeLineNo++ ;
            // Update the list of the header file
            m_omIncludeFileArray.Add( omStr );
        }
    }
    m_omSourceCodeTextList.AddTail( BUS_INCLUDE_FOOTER );
    m_omSourceCodeTextList.AddTail(  "" );
    m_omSourceCodeTextList.AddTail(  "" );
    m_omSourceCodeTextList.AddTail( BUS_VAR_HDR );
    m_omSourceCodeTextList.AddTail(  "" );
    m_omSourceCodeTextList.AddTail( BUS_VAR_FOOTER );
    m_omSourceCodeTextList.AddTail(  "" );
    m_omSourceCodeTextList.AddTail(  "" );
    m_omSourceCodeTextList.AddTail( BUS_FN_PROTOTYPE_HDR );
    m_omSourceCodeTextList.AddTail( BUS_FN_PROTOTYPE_FOOTER );
    m_omSourceCodeTextList.AddTail(  "" );

    return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CFunctionEditorDoc serialization
/******************************************************************************/
/*  Function Name    :  Serialize                                             */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :                                                        */
/*  Functionality    :  Handles loading and unloading of the archive
/*  Member of        :  CFunctionEditorDoc
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  05.03.2002                                            */
/******************************************************************************/

void CFunctionEditorDoc::Serialize(CArchive& ar)
{
    //UINT fileSize;
    if (ar.IsStoring())
    {
        CString omStrLine = "";
        BYTE* pbyFileData = new BYTE[m_nMaxLineLength * m_omSourceCodeTextList.GetCount()];
        memset(pbyFileData, 0, m_nMaxLineLength * m_omSourceCodeTextList.GetCount());
        BYTE pbyFileLine[1024] = {'\0'};
        POSITION pos = m_omSourceCodeTextList.GetHeadPosition();
        int nIndex = 0;
        while ( pos != nullptr )
        {
            omStrLine = m_omSourceCodeTextList.GetNext( pos );
            UINT LineLength = omStrLine.GetLength();
            memcpy(pbyFileLine, /*T2A*/(omStrLine.GetBuffer(MAX_PATH)), LineLength);
            memcpy(pbyFileData + nIndex, pbyFileLine, LineLength);
            nIndex += LineLength;
            pbyFileData[nIndex++] = '\n';
        }
        CFile* pFile = ar.GetFile();
        if (pFile != nullptr)
        {
            pFile->Write(pbyFileData, nIndex);
        }
        delete[] pbyFileData;
    }
    else // Loading
    {
        CString omTextLine = "";
        int nLineLength = 0;
        TRY
        {
            CFile* pFile = ar.GetFile();
            if (pFile != nullptr)
            {
                ULONGLONG FileLength = pFile->GetLength();
                BYTE* pbyFileData = new BYTE[(size_t)FileLength];       //Warning
                pFile->Read(pbyFileData, (COMMANUINT)FileLength);

                UINT nIndex = 0;
                CHAR acFileLine[4000];
                while (ReadString((LPSTR)pbyFileData, (COMMANUINT)FileLength, acFileLine, nIndex))
                {
                    //omTextLine = A2T(acFileLine);
                    omTextLine.Format("%s",acFileLine);
                    nLineLength = omTextLine.GetLength();
                    if(nLineLength > m_nMaxLineLength)
                    {
                        m_nMaxLineLength = nLineLength;
                    }



                    if ( omTextLine.Find( defHASHINCLUDE ) != -1 )
                    {
                        //Back word compatiability of Old BUSMASTER C Files
                        INT nIndex = omTextLine.Find("<struct.h>");
                        if( nIndex >= 0 )
                        {
                            omTextLine.Replace("<struct.h>", "\"CANIncludes.h\"");
                        }
                        else
                        {
                            nIndex = omTextLine.Find("<Struct_J1939.h>");
                            if( nIndex >= 0 )
                            {
                                omTextLine.Replace("<Struct_J1939.h>", "\"J1939Includes.h\"");
                            }
                        }
                        m_omIncludeFileArray.Add( omTextLine );
                    }
                    m_omSourceCodeTextList.AddTail(omTextLine);
                    m_dwSourceCodeLineNo++;

                    BOOL bInsideLoop = TRUE;
                    CString omTemp = BUS_VAR_HDR;
                    omTemp.Replace("PLACE_HODLER_FOR_BUSNAME", m_sBusSpecInfo.m_omBusName);
                    if (omTextLine == omTemp )
                    {
                        while ( bInsideLoop )
                        {
                            ReadString((LPSTR)pbyFileData, (COMMANUINT)FileLength, acFileLine, nIndex);
                            //omTextLine = A2T(acFileLine);
                            omTextLine.Format(_T("%s"),acFileLine);
                            nLineLength = omTextLine.GetLength();
                            if(nLineLength > m_nMaxLineLength)
                            {
                                m_nMaxLineLength = nLineLength;
                            }

                            m_omSourceCodeTextList.AddTail(omTextLine);
                            m_dwSourceCodeLineNo++;
                            omTemp = BUS_VAR_FOOTER;
                            omTemp.Replace("PLACE_HODLER_FOR_BUSNAME", m_sBusSpecInfo.m_omBusName);
                            if ( omTextLine == omTemp )
                            {
                                bInsideLoop = FALSE;
                            }
                            else
                            {
                                // Eliminate ";" OR initialisation from the string
                                int nIndex = omTextLine.Find( _T("=") );

                                if ( nIndex == -1 )
                                {
                                    nIndex = omTextLine.Find( _T(";") );
                                }

                                omTextLine = omTextLine.Left( nIndex );

                                omTextLine.TrimRight();

                                m_omGlobalVariableArray.Add( omTextLine );
                            }
                        }
                    }
                    omTemp = FRAME_FN_PARTIAL_HDR;
                    omTemp.Replace("PLACE_HODLER_FOR_BUSNAME", m_sBusSpecInfo.m_omBusName);
                    if ( omTextLine.Find( omTemp ) != -1 )
                    {
                        ReadString((LPSTR)pbyFileData, (COMMANUINT)FileLength, acFileLine, nIndex);
                        //omTextLine = A2T(acFileLine);
                        omTextLine.Format("%s",acFileLine);
                        //Only for Error Handling functions
                        if(omTextLine.Find("OnError") != -1)
                        {
                            if(omTextLine.Find("SCAN_ERR") == -1 && omTextLine.Find("_LIN") == -1)
                            {
                                if(m_sBusSpecInfo.m_eBus == CAN)
                                {
                                    //Inserting the SCAN_ERR structure as parameter in the error handlers
                                    nLineLength = omTextLine.GetLength();
                                    omTextLine.Insert( ( nLineLength-1 ),"SCAN_ERR ErrorMsg");
                                }
                                else if(m_sBusSpecInfo.m_eBus == LIN)
                                {
                                    vInsertErrorMessageStruct(omTextLine);
                                }
                            }
                        }

                        nLineLength = omTextLine.GetLength();

                        if(nLineLength > m_nMaxLineLength)
                        {
                            m_nMaxLineLength = nLineLength;
                        }

                        m_omSourceCodeTextList.AddTail(omTextLine);
                        m_dwSourceCodeLineNo++;
                        CString omTextLineTemp ="";
                        omTextLineTemp = omTextLine.Left(omTextLine.Find('_')+1);

                        if ( omTextLineTemp.Find(CGlobalObj::omGetBusSpecMsgHndlrName(m_sBusSpecInfo.m_eBus)) != -1 )
                        {
                            // It is a Utility function
                            m_omMessageHandlerArray.Add(omTextLine);
                        }
                        else if ( omTextLineTemp.Find(defKEY_HANDLER) != -1)
                        {
                            // It is a key function
                            m_omKeyHandlerArray.Add(omTextLine);
                        }
                        else if(omTextLineTemp.Find(defTIMER_HANDLER) != -1)
                        {
                            // It is a timer function
                            m_omTimerHandlerArray.Add(omTextLine);
                        }
                        else if(omTextLineTemp.Find(defERROR_HANDLER_FN) != -1)
                        {
                            m_omErrorHandlerArray.Add(omTextLine);
                        }
                        else if ( omTextLineTemp.Find(defEVENT_IND_HANDLER) != -1)
                        {
                            m_omEventIndArray.Add(omTextLine);
                        }
                        else if( omTextLineTemp.Find(defDLL_HANDLER_FN) != -1)
                        {
                            m_omDLLHandlerArray.Add(omTextLine);
                        }
                        else if(omTextLineTemp.Find(defBUSEVE_HANDLER_FN) != -1)
                        {
                            m_omBusEventsHandlerArray.Add(omTextLine);
                        }
                        else
                        {
                            // It is a Utility function
                            m_omUtilityFuncArray.Add(omTextLine);
                        }

                        m_omFunctionNameArray.Add(
                            omStrExtractFunctionNameFromPrototype(omTextLine));
                    }
                    // end of while(bReadSuccess == TRUE)
                }
                //clear File Data Buffer
                delete[] pbyFileData;
            }
        }
        CATCH_ALL(pomE)
        {
            if(pomE != nullptr)
            {
                char acError[1024];
                // Get error
                pomE->GetErrorMessage( acError, sizeof(acError));

                MessageBox( nullptr, acError, nullptr, MB_OK );

                pomE->Delete();
            }
        }
        END_CATCH_ALL
    }
}

void CFunctionEditorDoc::vInsertErrorMessageStruct(CString& omTextLine)
{
    INT nLineLength;
    if(omTextLine.Find(defERR_CHECKSUM_HANDLER_FN) != -1)
    {
        nLineLength = omTextLine.GetLength();
        omTextLine.Insert( ( nLineLength-1 ),"SEVENT_CHECKSUM_LIN ErrorMsg");
    }
    else if(omTextLine.Find(defERR_RX_FRM_HANDLER_FN) != -1)
    {
        nLineLength = omTextLine.GetLength();
        omTextLine.Insert( ( nLineLength-1 ),"SEVENT_RECEIVE_LIN ErrorMsg");
    }
    else if(omTextLine.Find(defERR_SLAVENOANS_HANDLER_FN) != -1)
    {
        nLineLength = omTextLine.GetLength();
        omTextLine.Insert( ( nLineLength-1 ),"SEVENT_SLAVE_NORESP_LIN ErrorMsg");
    }
    else if(omTextLine.Find(defERR_SYNC_HANDLER_FN) != -1)
    {
        nLineLength = omTextLine.GetLength();
        omTextLine.Insert( ( nLineLength-1 ),"SEVENT_SYNC_LIN ErrorMsg");
    }
}

/////////////////////////////////////////////////////////////////////////////
// CFunctionEditorDoc diagnostics

#ifdef _DEBUG
void CFunctionEditorDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CFunctionEditorDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFunctionEditorDoc commands
/******************************************************************************/
/*  Function Name    :  dwGetLineCount                                        */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :  DWORD                                                 */
/*  Functionality    :  Returns string list count
/*  Member of        :  CFunctionEditorDoc
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  05.03.2002                                            */
/*  Modifications    :
/******************************************************************************/
DWORD CFunctionEditorDoc::dwGetLineCount()
{
    return ((DWORD)m_omSourceCodeTextList.GetCount());
}
/******************************************************************************/
/*  Function Name    :  pcGetLine                                             */
/*                                                                            */
/*  Input(s)         :  POSITION &rPosition                                   */
/*  Output           :  const char*                                           */
/*  Functionality    :  Returns next position in the string list
/*  Member of        :  CFunctionEditorDoc
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  05.03.2002                                            */
/*  Modifications    :
/******************************************************************************/
const char* CFunctionEditorDoc::pcGetLine(POSITION& rPosition)
{
    return(m_omSourceCodeTextList.GetNext(rPosition));
}
/******************************************************************************/
/*  Function Name    :  pcGetLine                                             */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :  int                                                   */
/*  Functionality    :  Returns number of lines of source code
/*  Member of        :  CFunctionEditorDoc
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  05.03.2002                                            */
/*  Modifications    :
/******************************************************************************/
int CFunctionEditorDoc::nGetDocumentSize()
{
    return m_dwSourceCodeLineNo;
}
/******************************************************************************/
/*  Function Name    :  SetPosToFirstLine                                     */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :  POSITION                                              */
/*  Functionality    :  Returns the position of head item in the string list
/*  Member of        :  CFunctionEditorDoc
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  05.03.2002                                            */
/*  Modifications    :
/******************************************************************************/
POSITION CFunctionEditorDoc::SetPosToFirstLine()
{
    POSITION Position = m_omSourceCodeTextList.GetHeadPosition();
    return(Position);
}
/******************************************************************************/
/*  Function Name    :  omStrGetMessageHandlerPrototypes                      */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :  CStringArray*                                         */
/*  Functionality    :  Returns an array of message handlers
/*  Member of        :  CFunctionEditorDoc
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  05.03.2002                                            */
/*  Modifications    :
/******************************************************************************/
CStringArray* CFunctionEditorDoc::omStrGetMessageHandlerPrototypes()
{
    return (&m_omMessageHandlerArray);
}
/******************************************************************************/
/*  Function Name    :  omStrGetTimerHandlerPrototypes                        */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :  CStringArray*                                         */
/*  Functionality    :  Returns an array of timer handlers
/*  Member of        :  CFunctionEditorDoc
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  05.03.2002                                            */
/*  Modifications    :
/******************************************************************************/
CStringArray* CFunctionEditorDoc::omStrGetTimerHandlerPrototypes()
{
    return (&m_omTimerHandlerArray);
}
/******************************************************************************/
/*  Function Name    :  omStrGetKeyHandlerPrototypes                          */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :  CStringArray*                                         */
/*  Functionality    :  Returns an array of key handlers
/*  Member of        :  CFunctionEditorDoc
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  05.03.2002                                            */
/*  Modifications    :
/******************************************************************************/
CStringArray* CFunctionEditorDoc::omStrGetKeyHandlerPrototypes()
{
    return (&m_omKeyHandlerArray);
}
/******************************************************************************/
/*  Function Name    :  omStrGetKeyHandlerPrototypes                          */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :  CStringArray*                                         */
/*  Functionality    :  Returns an array of key handlers
/*  Member of        :  CFunctionEditorDoc
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  05.03.2002                                            */
/*  Modifications    :
/******************************************************************************/
CStringArray* CFunctionEditorDoc::omStrGetUtilityFunctionPrototypes()
{
    return (&m_omUtilityFuncArray);
}
/******************************************************************************/
/*  Function Name    :  omStrGetGlobalVariablePrototypes                      */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :  CStringArray*                                         */
/*  Functionality    :  Returns an array of global variable
/*  Member of        :  CFunctionEditorDoc
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  05.03.2002                                            */
/*  Modifications    :
/******************************************************************************/

CStringArray* CFunctionEditorDoc::omStrGetGlobalVariablePrototypes()
{
    return (&m_omGlobalVariableArray);
}
/******************************************************************************/
/*  Function Name    :  omStrGetFunctionNamesForDEF                           */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :  CStringArray*                                         */
/*  Functionality    :  Returns an array of all the function names without
                        prototype
/*  Member of        :  CFunctionEditorDoc
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  05.03.2002                                            */
/*  Modifications    :
/******************************************************************************/
CStringArray* CFunctionEditorDoc::omStrGetFunctionNamesForDEF()
{
    return (&m_omFunctionNameArray);
}
/******************************************************************************/
/*  Function Name    :  omStrExtractFunctionNameFromPrototype                 */
/*                                                                            */
/*  Input(s)         :  CString                                               */
/*  Output           :  CString                                               */
/*  Functionality    :  Extracts function name from the prototype
/*  Member of        :  CFunctionEditorDoc
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  05.03.2002                                            */
/*  Modifications    :
/******************************************************************************/

CString CFunctionEditorDoc::
omStrExtractFunctionNameFromPrototype(CString omStrPrototype)
{
    CString omStrFuncName = "";

    if ( !omStrPrototype.IsEmpty())
    {
        // Find open paranthesis in the defn
        int nIndex = omStrPrototype.Find("(");

        if ( nIndex != -1 )
        {
            // Found, Get left of open paranthesis
            omStrPrototype = omStrPrototype.Left( nIndex );

            // Remove void from "void funcname" from the string
            int nIndex = omStrPrototype.ReverseFind(SPACE);

            omStrPrototype =
                omStrPrototype.Right( omStrPrototype.GetLength() - (nIndex+1));

            omStrPrototype.TrimRight();

            omStrFuncName = omStrPrototype;
        }
    }
    return omStrFuncName;
}
/******************************************************************************/
/*  Function Name    :  vUpdateDEFFile                                        */
/*                                                                            */
/*  Input(s)         :  CString omStrFileName                                 */
/*  Output           :                                                        */
/*  Functionality    :  Updates the .DEF file with all function names
/*  Member of        :  CFunctionEditorDoc
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  05.03.2002                                            */
/******************************************************************************/
void CFunctionEditorDoc::vUpdateDEFFile(CString omStrFileName)
{
    m_omFunctionNameArray.RemoveAll();
    // Collect all the function names from their respective
    // prototypes
    // 1. Msg handlers
    int unCount;    //unCount declared outside
    for( unCount = 0; unCount < m_omMessageHandlerArray.GetSize(); unCount++)
    {
        m_omFunctionNameArray.Add(
            omStrExtractFunctionNameFromPrototype(
                m_omMessageHandlerArray.GetAt( unCount )));
    }
    // 2. Timer handlers
    for ( unCount = 0; unCount < m_omTimerHandlerArray.GetSize(); unCount++)
    {
        CString omStrFunction =
            omStrExtractFunctionNameFromPrototype(
                m_omTimerHandlerArray.GetAt( unCount ));

        // Timer callbacks have __stdcall prefixed to the function name
        // Trim it and get function name
        omStrFunction.TrimLeft();
        omStrFunction.TrimRight();
        int nIndex = omStrFunction.Find( " ");
        if ( nIndex != -1 )
        {
            omStrFunction =
                omStrFunction.Right(
                    omStrFunction.GetLength() - (nIndex+1) );
        }

        m_omFunctionNameArray.Add( omStrFunction );
    }
    // 3. Key handlers
    for ( unCount = 0; unCount < m_omKeyHandlerArray.GetSize(); unCount++)
    {
        m_omFunctionNameArray.Add(
            omStrExtractFunctionNameFromPrototype(
                m_omKeyHandlerArray.GetAt( unCount )));
    }
    // 4. Error handlers
    for ( unCount = 0; unCount < m_omErrorHandlerArray.GetSize(); unCount++)
    {
        m_omFunctionNameArray.Add(
            omStrExtractFunctionNameFromPrototype(
                m_omErrorHandlerArray.GetAt( unCount )));
    }
    // 5. Event Indication handlers
    for ( unCount = 0; unCount < m_omEventIndArray.GetSize(); unCount++)
    {
        m_omFunctionNameArray.Add(
            omStrExtractFunctionNameFromPrototype(
                m_omEventIndArray.GetAt( unCount )));
    }
    // 6. DLL handlers
    for ( unCount = 0; unCount < m_omDLLHandlerArray.GetSize(); unCount++)
    {
        m_omFunctionNameArray.Add(
            omStrExtractFunctionNameFromPrototype(
                m_omDLLHandlerArray.GetAt( unCount )));
    }
    // 7. Bus Event functions
    for ( unCount = 0; unCount < m_omBusEventsHandlerArray.GetSize(); unCount++)
    {
        m_omFunctionNameArray.Add(
            omStrExtractFunctionNameFromPrototype(
                m_omBusEventsHandlerArray.GetAt( unCount )));
    }

    // 8. Utility function
    /*for ( unCount = 0; unCount < m_omUtilityFuncArray.GetSize(); unCount++)
    {
        m_omFunctionNameArray.Add(
            omStrExtractFunctionNameFromPrototype(
                m_omUtilityFuncArray.GetAt( unCount )));
    }*/
    // 8. Global variable
    for ( unCount = 0; unCount < m_omGlobalVariableArray.GetSize(); unCount++)
    {
        //Find the data which the user wants to declare as external global
        CString omStrTemp = m_omGlobalVariableArray.GetAt( unCount );
        int nIndex = omStrTemp.Find(" ");
        if( nIndex != -1 )
        {
            int nLength = omStrTemp.GetLength();
            omStrTemp = omStrTemp.Right(nLength - nIndex - 1);
            int nComaIndex = omStrTemp.ReverseFind(',');
            if(nComaIndex == -1)
            {
                //no other variable is declared
                if(omStrTemp.Find(GLOBAL_VAR_PREFIX) == 0)
                {
                    //if starts with g_ then add
                    m_omFunctionNameArray.Add(omStrTemp);
                }
            }
            else
            {
                nLength = omStrTemp.GetLength();
                //multiple variable declare in one line
                while(nLength > MIN_CHAR_IN_GLOBAL_VAR && nComaIndex != -1)
                {
                    CString strVar = omStrTemp.Right(nLength - nComaIndex - 1);
                    if(strVar.Find(GLOBAL_VAR_PREFIX) == 0)
                    {
                        //if starts with g_ then add
                        m_omFunctionNameArray.Add(strVar);
                    }
                    omStrTemp = omStrTemp.Left(nComaIndex);
                    nComaIndex = omStrTemp.ReverseFind(',');
                    nLength = omStrTemp.GetLength();
                }
                if(omStrTemp.Find(GLOBAL_VAR_PREFIX) == 0)
                {
                    //if starts with g_ then add
                    m_omFunctionNameArray.Add(omStrTemp);
                }
            }
        }
    }


    BOOL bRetVal = FALSE;

    if ( !omStrFileName.IsEmpty())
    {
        omStrFileName.MakeUpper();
        // replace ".c" with ".def"
        INT nIndex = omStrFileName.Find(defDOT_C);
        if( nIndex >= 0 )
        {
            omStrFileName = omStrFileName.Left(nIndex) + defDOT_DEF ;
        }
        //omStrFileName.Replace( defDOT_C, defDOT_DEF );

        // CStdioFile object for .DEF file
        CStdioFile o_DEF_File;

        TRY
        {
            // Open DEF file
            bRetVal = o_DEF_File.Open( omStrFileName,
            CFile::modeCreate|CFile::modeWrite|CFile::typeText);
            if (bRetVal)
            {
                // Write EXPORTS into the file
                o_DEF_File.WriteString("EXPORTS");
                o_DEF_File.WriteString(NEW_LINE);

                /* Enter name of the exported function into the .def file. For
                every application API function there must be a getter that the
                application must call in order to pass addresses of the
                application API functions */
                // Skip the last API
                for (int i = 0; i < CGlobalObj::ouGetObj(m_sBusSpecInfo.m_eBus).m_ArrAPIFuncNames.GetSize(); i++)
                {
                    CString omWriteStr = "vSet";
                    omWriteStr += CGlobalObj::ouGetObj(m_sBusSpecInfo.m_eBus).m_ArrAPIFuncNames.GetAt(i);
                    omWriteStr += "ProcAddress";
                    o_DEF_File.WriteString(omWriteStr);
                    o_DEF_File.WriteString(NEW_LINE);
                }
                //GetProgramVersion is exported to check the validity
                CString omProgVer = _T(defNAME_FUNC_GET_PRG_VER);
                o_DEF_File.WriteString(omProgVer);
                o_DEF_File.WriteString(NEW_LINE);

                if (CAN == m_sBusSpecInfo.m_eBus)
                {
                    omProgVer = _T(NAME_FUNC_KEYPRESSED);
                    o_DEF_File.WriteString(omProgVer);
                    o_DEF_File.WriteString(NEW_LINE);

                    CString omGetMessName = _T(NAME_FUNC_GETMESSAGENAME);
                    o_DEF_File.WriteString(omGetMessName);
                    o_DEF_File.WriteString(NEW_LINE);

                    CString omTimeNow = _T(NAME_FUNC_TIMENOW);
                    o_DEF_File.WriteString(omTimeNow);
                    o_DEF_File.WriteString(NEW_LINE);

                    CString omGetFirstCANdbName = _T(NAME_FUNC_GETFIRSTCANDBNAME);
                    o_DEF_File.WriteString(omGetFirstCANdbName);
                    o_DEF_File.WriteString(NEW_LINE);
                }



                // Write all function names into the DEF file
                for (UINT unCount = 0;
                        unCount < (UINT)m_omFunctionNameArray.GetSize();
                        unCount++)
                {
                    o_DEF_File.WriteString(NEW_LINE);
                    o_DEF_File.WriteString(
                        m_omFunctionNameArray.GetAt(unCount));
                }

                o_DEF_File.Close();
            }
        }
        CATCH_ALL(pomE)
        {
            if(pomE != nullptr)
            {
                char acError[1024];
                // Get error
                pomE->GetErrorMessage( acError, sizeof(acError));

                MessageBox( nullptr, acError, nullptr, MB_OK );

                pomE->Delete();
            }
            // If file is opened, close
            if ( bRetVal )
            {
                o_DEF_File.Close();
            }
        }
        END_CATCH_ALL
    }
    else
    {
        AfxMessageBox( FILENAME_NOT_FOUND, MB_OK|MB_ICONINFORMATION );
    }
}
/******************************************************************************/
/*  Function Name    :  OnSaveDocument                                        */
/*                                                                            */
/*  Input(s)         :  LPCTSTR lpszPathName                                  */
/*  Output           :                                                        */
/*  Functionality    :  Saves the document and
                        Updates the .DEF file with all function names
/*  Member of        :  CFunctionEditorDoc
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :
/*  Date Created     :
/******************************************************************************/
BOOL CFunctionEditorDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
    BOOL nRetVal =
        CDocument::OnSaveDocument(lpszPathName);
    DWORD dwErrorCode = GetLastError();
    // If he user does SaveAs.. the new file name
    // shud be updated to the dta member for
    // future use

    // Update new file name
    CGlobalObj::ouGetObj(m_sBusSpecInfo.m_eBus).m_omStrSourceFilePathName = lpszPathName;
    // Changes in the file name should get
    // reflected to the root of the tree item
    UpdateAllViews(nullptr);
    // Saved
    SetModifiedFlag( FALSE );

    // Find the header in the list
    CString omTemp = BUS_VAR_HDR;
    if (m_sBusSpecInfo.m_omBusName.IsEmpty())
    {
        // initialise default values atleast
        m_sBusSpecInfo.m_omBusName = "BUSMASTER";
        m_sBusSpecInfo.m_omHeaderFileName = CGlobalObj::ouGetObj(m_sBusSpecInfo.m_eBus).m_omMsgStructFile;
    }
    else
    {
        /* Adding Protocol and Application version information */

        // To get the application version
        CString omstrAppVersion = GetApplicationVersion();

        // Adding the version and Protocol information to the .c file
        CString omstrCopyWriteInformation = COPYWRITE_INFORMATION;
        CString omstrCFileVersion = C_FILE_VERSION;
        CString omstrBusMasterVersion = BUSMASTER_VERSION;
        CString omstrProtocol = PROTOCOL;

        // Adding Protocol information
        if(m_sBusSpecInfo.m_eBus == CAN)
        {
            omstrProtocol.Replace("PLACE_HOLDER_FOR_PROTOCOL_VALUE", DATABASE_PROTOCOL_CAN);
        }
        else if(m_sBusSpecInfo.m_eBus == J1939)
        {
            omstrProtocol.Replace("PLACE_HOLDER_FOR_PROTOCOL_VALUE", DATABASE_PROTOCOL_J1939);
        }
        else if(m_sBusSpecInfo.m_eBus == LIN)
        {
            omstrProtocol.Replace("PLACE_HOLDER_FOR_PROTOCOL_VALUE", DATABASE_PROTOCOL_LIN);
        }

        // adding application version information
        omstrBusMasterVersion.Replace("PLACE_HOLDER_FOR_BUSMASTER_VERSION", omstrAppVersion);

        omstrCFileVersion.Replace("PLACE_HOLDER_FOR_C_FILE_VERSION", C_FILE_VERSION_VALUE);
        /* End of Adding Protocol and Application version information */

        POSITION pCopyWritePos = m_omSourceCodeTextList.Find( omstrCopyWriteInformation );

        if(pCopyWritePos == nullptr)
        {
            m_omSourceCodeTextList.AddHead(omstrProtocol);
            m_omSourceCodeTextList.AddHead(omstrBusMasterVersion);
            m_omSourceCodeTextList.AddHead(omstrCFileVersion);
            m_omSourceCodeTextList.AddHead(omstrCopyWriteInformation);
        }
    }
    omTemp.Replace("PLACE_HODLER_FOR_BUSNAME", m_sBusSpecInfo.m_omBusName);
    POSITION posStartGvar = m_omSourceCodeTextList.Find( omTemp  );

    if (nullptr != posStartGvar)
    {
        omTemp = BUS_VAR_FOOTER;
        omTemp.Replace("PLACE_HODLER_FOR_BUSNAME", m_sBusSpecInfo.m_omBusName);
        POSITION posEndGvar = m_omSourceCodeTextList.Find( omTemp  );

        m_omSourceCodeTextList.GetNext( posStartGvar );
        m_omGlobalVariableArray.RemoveAll();
        CString omStr;
        // Copy the function to be deleted to a temp list
        while ( posStartGvar != nullptr && posEndGvar != nullptr && posStartGvar != posEndGvar)
        {
            omStr = m_omSourceCodeTextList.GetNext( posStartGvar );
            //Any initialization
            int nIndex = omStr.Find( "=" );

            if ( nIndex == -1 )
            {
                nIndex = omStr.Find(";" );
            }
            omStr = omStr.Left( nIndex );
            omStr.TrimRight();
            m_omGlobalVariableArray.Add(omStr);
        }
    }

    // Update the DEF file
    vUpdateDEFFile( lpszPathName );

    return nRetVal;
}
/******************************************************************************/
/*  Function Name    :  nGetMaxLineLength                                     */
/*                                                                            */
/*  Input(s)         :
/*  Output           :  int                                                   */
/*  Functionality    :  Returns max line length value
/*  Member of        :  CFunctionEditorDoc
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  06.03.2002                                            */
/*  Modifications    :
/******************************************************************************/
int CFunctionEditorDoc::nGetMaxLineLength()
{
    POSITION pos = m_omSourceCodeTextList.GetHeadPosition();

    CString omStrLine   = "";
    int nLineLength     = 0;

    while ( pos != nullptr )
    {
        // Get each line
        omStrLine =
            m_omSourceCodeTextList.GetNext( pos );

        nLineLength =
            omStrLine.GetLength();

        if ( nLineLength > m_nMaxLineLength)
        {
            m_nMaxLineLength = nLineLength;
        }
    }

    return (m_nMaxLineLength);
}
/******************************************************************************/
/*  Function Name    :  OnOpenDocument                                        */
/*                                                                            */
/*  Input(s)         :
/*  Output           :  bool                                                  */
/*  Functionality    :  Initialises the filename to CMainFrame mem var
                        Creates Unions.h in local directory
/*  Member of        :  CFunctionEditorDoc
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :
/******************************************************************************/
BOOL CFunctionEditorDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    if (!CDocument::OnOpenDocument(lpszPathName))
    {
        return FALSE;
    }

    CGlobalObj::ouGetObj(m_sBusSpecInfo.m_eBus).m_omStrSourceFilePathName = lpszPathName;
    SetPathName(lpszPathName);
    vUpdateDEFFile(lpszPathName);

    return TRUE;
}
/* Read String from ASCII buffer*/
int CFunctionEditorDoc::ReadString(const LPSTR lpBuffer, UINT unSize, LPSTR lpsz, UINT& nIndex)
{
    int nReturn = 0;
    UINT unStop = unSize - nIndex;
    if(lpsz == nullptr)
    {
        return 0;
    }
    CHAR ch;
    UINT unRead = 0;

    while (unRead < unStop)
    {
        ch = (char)lpBuffer[nIndex++];

        // stop and end-of-line (trailing '\n' is ignored)
        if (ch == '\n' || ch == '\r')
        {
            if (ch == '\r')
            {
                ch = '\n';
                nIndex++;
            }
            nReturn = 1;
            break;

        }
        lpsz[unRead++] = ch;
    }
    lpsz[unRead] = '\0';

    return nReturn;
}
/******************************************************************************/
/*  Function Name    :  OnCloseDocument                                       */
/*                                                                            */
/*  Input(s)         :
/*  Output           :  void                                                  */
/*  Functionality    :  Initialises the filename to CMainFrame mem var
                        Deletes Unions.h file if found
/*  Member of        :  CFunctionEditorDoc
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  06.03.2002                                            */
/******************************************************************************/
void CFunctionEditorDoc::OnCloseDocument()
{
    // Set filename to empty
    /* CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
     if ( pMainFrame != nullptr )
     {
         pMainFrame->CGlobalObj::m_omStrSourceFilePathName = "";
     }*/

    CDocument::OnCloseDocument();

    // Check if all the documents were closed
    // so that Build, Build and Load, Stop Build menu option
    // can be disabled

    // Get first document template position
    //POSITION pos = theApp.GetFirstDocTemplatePosition();

    //while ( pos != nullptr )
    //{
    //    // Get Next doc template
    //    CDocTemplate* pDocTemplate =
    //        theApp.GetNextDocTemplate( pos );

    //    if ( pDocTemplate != nullptr )
    //    {
    //        // Get postion to first document
    //        POSITION pos2 = pDocTemplate->GetFirstDocPosition();

    //        if ( pos2 == nullptr )
    //        {
    //            // All doc's are closed
    //            pos = nullptr;
    //        }
    //    }
    //
    //}
}
/******************************************************************************/
/*  Function Name    :  omStrGetIncudeHeaderFiles                             */
/*                                                                            */
/*  Input(s)         :
/*  Output           :  CStringArray*                                         */
/*  Functionality    :  Returns pointer to include header file array
/*  Member of        :  CFunctionEditorDoc
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amarnath Shastry                                      */
/*  Date Created     :  06.03.2002                                            */
/*  Modifications    :  13.03.2002, Amarnath Shastry
/******************************************************************************/
CStringArray* CFunctionEditorDoc::omStrGetIncudeHeaderFiles()
{
    return (&m_omIncludeFileArray);
}
/******************************************************************************/
/*  Function Name    :  pomStrGetErrorHandlerPrototypes                       */
/*  Input(s)         :                                                        */
/*  Output           :  CStringArray*                                         */
/*  Functionality    :  Returns pointer to m_omErrorHandlerArray DLL handler  */
/*  Member of        :  CFunctionEditorDoc                                    */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  20.02.2003                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
CStringArray* CFunctionEditorDoc::pomStrGetErrorHandlerPrototypes()
{
    return (&m_omErrorHandlerArray);
}
/******************************************************************************/
/*  Function Name    :  pomStrGetDLLHandlerPrototypes                         */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :  CStringArray*                                         */
/*  Functionality    :  Returns pointer to include header file array          */
/*  Member of        :  CFunctionEditorDoc                                    */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Ravikumar Patil                                       */
/*  Date Created     :  04.03.2003                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
CStringArray* CFunctionEditorDoc::pomStrGetDLLHandlerPrototypes()
{
    return (&m_omDLLHandlerArray);
}
/******************************************************************************/
/*  Function Name    :  pomStrGetBusEventHandlerPrototypes                    */
/*                                                                            */
/*  Input(s)         :                                                        */
/*  Output           :  CStringArray*                                         */
/*  Functionality    :  Returns pointer to Bus Event header file array        */
/*  Member of        :  CFunctionEditorDoc                                    */
/*  Friend of        :      -                                                 */
/*                                                                            */
/*  Author(s)        :  Venkatanarayana Makam                                 */
/*  Date Created     :  09.01.2012                                            */
/*  Modifications    :                                                        */
/******************************************************************************/
CStringArray* CFunctionEditorDoc::pomStrGetBusEventHandlerPrototypes()
{
    return (&m_omBusEventsHandlerArray);
}
/******************************************************************************/
/*  Function Name    :  omStrGetInitialisedMessage                            */
/*                                                                            */
/*  Input(s)         :  omStrMsgName - Message Name                           */
/*                      omStrVarName - Variable Name                          */
/*                      bInitData    - Init databytes flag                    */
/*  Output           :  Initialised message declaration                       */
/*  Functionality    :  Forms a string initialised with message details       */
/*                      If message not found in the database returns the same */
/*                      string without any init                               */
/*  Member of        :  CFunctionEditorDoc                                    */
/*  Friend of        :      -                                                 */
/******************************************************************************/
CString CFunctionEditorDoc::omStrGetInitialisedMessage(UINT unMsgID,
        const CString&
        omStrMsgName,
        const CString&
        omStrVarName,
        BOOL bInitData,
        UCHAR ucChannelId, INT unChnlSel)
{
    // Copy the input param as init statement
    CString omStrResult = omStrMsgName + defSPACE_STR + omStrVarName +
                          SEMI_COLON;
    BOOL bInitByUser = FALSE;

    //Check for init by User
    if( omStrVarName.Find(defCHAR_EQUALS, 0) != -1)
    {
        bInitByUser = TRUE;
    }
    // Get the message from Active database
    SMSG_NAME_CODE sMsgNameCode;
    POSITION pos = nullptr;
    BOOL bIsMsgFound = FALSE;
    if( bInitByUser == FALSE)
    {
        sMsgNameCode.m_dwMsgCode = unMsgID;

        if(m_sBusSpecInfo.m_eBus == LIN)
        {
            if(CGlobalObj::ouGetObj(m_sBusSpecInfo.m_eBus).m_ouClusterConfig->m_nChannelsConfigured >= unChnlSel)
            {
                std::list<FRAME_STRUCT> lstFrames;
                CGlobalObj::ouGetObj(m_sBusSpecInfo.m_eBus).m_ouClusterConfig->m_ouFlexChannelConfig[unChnlSel-1].GetSelectedECUFrames(lstFrames);

                std::list<FRAME_STRUCT>::iterator itrFrame = lstFrames.begin();
                while(itrFrame != lstFrames.end())
                {
                    short nMsgId = unMsgID;
                    if(itrFrame->m_nSlotId == unMsgID)
                    {
                        sMsgNameCode.m_omMsgName = itrFrame->m_strFrameName.c_str();
                        sMsgNameCode.m_unMsgLen = itrFrame->m_nLength;

                        std::list<Flexray_SSIGNALINFO> lstSignalInfo;
                        unsigned char uchBytes[254];
                        bGetSignalInfo(*itrFrame,uchBytes,  sMsgNameCode.m_unMsgLen, lstSignalInfo);
                        GetSignalNames(lstSignalInfo, sMsgNameCode.m_omSignalNames);
                        bIsMsgFound = TRUE;
                        break;
                    }
                    itrFrame++;
                }
            }
        }
        else
        {
            POSITION        MainPos;
            MainPos =  CGlobalObj::ouGetObj(m_sBusSpecInfo.m_eBus).m_odMsgNameMsgCodeListDb.GetHeadPosition();
            while(MainPos != nullptr)
            {
                SDB_NAME_MSG&  sDbNameMsg = CGlobalObj::ouGetObj(m_sBusSpecInfo.m_eBus).
                                            m_odMsgNameMsgCodeListDb.GetNext(MainPos);
                pos = sDbNameMsg.m_oMsgNameMsgCodeList.Find(sMsgNameCode);
                if(pos != nullptr)         //if present stop searching
                {
                    sMsgNameCode = sDbNameMsg.m_oMsgNameMsgCodeList.GetAt(pos);
                    break;
                }
            }
        }
    }
    // If message found
    if(pos != nullptr || (bIsMsgFound == TRUE && m_sBusSpecInfo.m_eBus == LIN))
    {
        //sMsgNameCode = CGlobalObj::ouGetObj(m_sBusSpecInfo.m_eBus).m_odMsgNameMsgCodeList.GetAt(pos);
        CString omStrTemp;
        vInitialiseBusSpecStructure(omStrTemp, ucChannelId, sMsgNameCode, bInitData);
        omStrResult.Format(defFNS_INIT_FORMAT, omStrMsgName, omStrVarName,
                           omStrTemp);
    }
    return omStrResult;
}

BOOL CFunctionEditorDoc::bInitBusSpecificInfo(SBUS_SPECIFIC_INFO& sBusSpecInfo)
{
    BOOL bReturn = TRUE;
    m_sBusSpecInfo.m_eBus      = sBusSpecInfo.m_eBus;
    m_sBusSpecInfo.m_omBusName = sBusSpecInfo.m_omBusName; // CAN or McNet
    m_sBusSpecInfo.m_omWrapperObjName = sBusSpecInfo.m_omWrapperObjName; // Wrapper.o
    m_sBusSpecInfo.m_omHeaderFileName = sBusSpecInfo.m_omHeaderFileName; // struct_can.h or struct_mcnet.h

    m_sBusSpecInfo.m_odAPIFuncList.RemoveAll();
    m_sBusSpecInfo.m_odAPIFuncList.AddTail(&(sBusSpecInfo.m_odAPIFuncList));
    return bReturn;
}
BOOL CFunctionEditorDoc::bGetBusSpecificInfo(SBUS_SPECIFIC_INFO& sBusSpecInfo)
{
    BOOL bReturn = TRUE;
    sBusSpecInfo.m_eBus = m_sBusSpecInfo.m_eBus;
    sBusSpecInfo.m_omBusName = m_sBusSpecInfo.m_omBusName; // CAN or McNet
    sBusSpecInfo.m_omWrapperObjName = m_sBusSpecInfo.m_omWrapperObjName; // Wrapper.o
    sBusSpecInfo.m_omHeaderFileName = m_sBusSpecInfo.m_omHeaderFileName; // struct_can.h or struct_mcnet.h

    sBusSpecInfo.m_odAPIFuncList.RemoveAll();
    sBusSpecInfo.m_odAPIFuncList.AddTail(&(m_sBusSpecInfo.m_odAPIFuncList));

    return bReturn;

}
static UINT unFormExtIdFromPGN(UINT unPGN)
{
    UNION_29_BIT_ID uExtId;
    uExtId.m_s29BitId.m_uPGN.m_sPGN.m_byPriority = DEFAULT_PRIORITY - 1;
    uExtId.m_s29BitId.m_bySrcAddress = 0x0;
    uExtId.m_s29BitId.m_uPGN.m_unPGN = unPGN;
    return uExtId.m_unExtID;
}
void CFunctionEditorDoc::vInitialiseBusSpecStructure(CString& omStrTemp, UCHAR ucChannel,
        SMSG_NAME_CODE& sMsgNameCode, BOOL bInitData)
{
    switch (m_sBusSpecInfo.m_eBus)
    {
        case CAN:
        {
            // Format Message ID, Format, RTR & DLC
            omStrTemp.Format(defMSG_INIT_FORMAT_CAN,
                             sMsgNameCode.m_dwMsgCode,
                             DATA_FORMAT_INTEL,
                             0,
                             sMsgNameCode.m_unMsgLen, ucChannel);

            // Init data bytes if requested
            if(bInitData == TRUE && sMsgNameCode.m_unMsgLen > 0)
            {
                omStrTemp += defOPEN_PARENTHESIS;
                for( UINT unIndex = 0;
                        unIndex < sMsgNameCode.m_unMsgLen;
                        unIndex++)
                {
                    // Init to Zero
                    omStrTemp += defFNS_INIT_VAL;
                    // Check for Last zero
                    if( unIndex + 1 != sMsgNameCode.m_unMsgLen)
                    {
                        omStrTemp += defFNS_COMMA;
                    }

                }
                omStrTemp += defCLOSE_PARENTHESIS;
            }
            // Now form total init string
            omStrTemp += defFNS_COMMA;
            omStrTemp += ucChannel;
            omStrTemp += defCLOSE_PARENTHESIS;
            omStrTemp += SEMI_COLON;
        }
        break;
        case J1939:
        {
            UINT unExtId = unFormExtIdFromPGN(sMsgNameCode.m_dwMsgCode);

            // Format Message ID, Format, RTR & DLC
            omStrTemp.Format(defMSG_INIT_FORMAT_J1939,
                             0, //timestamp
                             ucChannel, //channel
                             "MSG_TYPE_BROADCAST", //Msg type. 4 - Broadcast
                             "DIR_TX", //Direction TX
                             unExtId, //PGN
                             sMsgNameCode.m_unMsgLen); //MSG LEN

            // Init data bytes if requested
            omStrTemp += defCLOSE_PARENTHESIS;
            omStrTemp += SEMI_COLON;
            omStrTemp += "\n";
            CString omStrTemp2;
            omStrTemp2.Format("sMsgStruct.m_sWhichBit = (%s_*)malloc(sizeof(%s_));\n", sMsgNameCode.m_omMsgName, sMsgNameCode.m_omMsgName);
            omStrTemp += omStrTemp2;
            omStrTemp2.Format("//Move the following line to the end of the function\nfree(sMsgStruct.m_sWhichBit);\n");
            omStrTemp += omStrTemp2;
            /*if(bInitData == TRUE && sMsgNameCode.m_unMsgLen > 0)
            {

                omStrTemp += defOPEN_PARENTHESIS;
                // Init to 0xFF
                omStrTemp += "{0xFF}";
                //for( UINT unIndex = 0;
                //        unIndex < sMsgNameCode.m_unMsgLen;
                //        unIndex++)
                //        {
                //
                //            // Check for Last zero
                //            if( unIndex + 1 != sMsgNameCode.m_unMsgLen)
                //            {
                //                omStrTemp += defFNS_COMMA;
                //            }

                //        }
                omStrTemp += defCLOSE_PARENTHESIS;
            }*/

        }
        break;
        case LIN:
        {
            UINT unMsgId = sMsgNameCode.m_dwMsgCode;

            // Format Message ID, F RTR & DLC
            omStrTemp.Format(defMSG_INIT_FORMAT_LIN,
                             1, // Response
                             0, // Classical
                             sMsgNameCode.m_unMsgLen , //dlc
                             unMsgId// Msg id
                            ); // channel no

            // Init data bytes if requested
            if(bInitData == TRUE && sMsgNameCode.m_unMsgLen > 0)
            {
                omStrTemp += defOPEN_PARENTHESIS;
                for( UINT unIndex = 0;
                        unIndex < sMsgNameCode.m_unMsgLen;
                        unIndex++)
                {
                    // Init to Zero
                    omStrTemp += defFNS_INIT_VAL;
                    // Check for Last zero
                    if( unIndex + 1 != sMsgNameCode.m_unMsgLen)
                    {
                        omStrTemp += defFNS_COMMA;
                    }

                }

                omStrTemp += defCLOSE_PARENTHESIS;
            }
            // Now form total init string
            omStrTemp += defFNS_COMMA;
            omStrTemp += defFNS_INIT_VAL; // Timestamp
            omStrTemp += defFNS_COMMA;
            omStrTemp += ucChannel; // Channel
            omStrTemp += defFNS_COMMA;
            omStrTemp += defFNS_INIT_VAL; // Checksum
            omStrTemp += defCLOSE_PARENTHESIS;
            omStrTemp += SEMI_COLON;

        }
        break;
    }
}
/******************************************************************************
  Function Name    :  omStrConIndPrototypes

  Input(s)         :
  Output           :  CStringArray*
  Functionality    :  Returns an array of Connection indication handlers
  Member of        :  CFunctionEditorDoc
  Friend of        :      -

  Author(s)        :  Anish Kumar
  Date Created     :  05.01.2010
  Modifications    :
******************************************************************************/
CStringArray* CFunctionEditorDoc::omStrGetEventIndPrototypes()
{
    return (&m_omEventIndArray);
}
// To get the BUSMASTER version
CString CFunctionEditorDoc::GetApplicationVersion()
{
    CString strVersion;

    // Application version
    strVersion.Format("%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);

    return strVersion;
}
BOOL CFunctionEditorDoc::bAddFunctionPrototype(CString omStrFuntion, BOOL bGCCExport)
{
    omStrFuntion.TrimLeft(" ");
    omStrFuntion.TrimRight(" ");

    if( bGCCExport == TRUE);
    {
        bAddGCCExportPrototype(omStrFuntion);
    }

    CString omSelectedText = BUS_FN_HDR;

    if ( !omSelectedText.IsEmpty() )
    {
        //Add prototype to the prototype part of the doc
        SBUS_SPECIFIC_INFO sBusSpecInfo;
        bGetBusSpecificInfo(sBusSpecInfo);
        CString omTemp = BUS_FN_PROTOTYPE_FOOTER;
        omTemp.Replace(_T("PLACE_HODLER_FOR_BUSNAME"), sBusSpecInfo.m_omBusName);
        POSITION pos = m_omSourceCodeTextList.Find( omTemp );

        if ( pos != nullptr )
        {
            CString omStrReplace = _T("");
            omStrReplace  = omStrFuntion;
            omStrReplace += SEMI_COLON;
            m_omSourceCodeTextList.InsertBefore(pos,omStrReplace );
        }
    }
    return TRUE;
}
BOOL CFunctionEditorDoc::bAddGCCExportPrototype(CString& omStrFunction)
{
    INT nIndex = -1;
    nIndex = omStrFunction.Find(" ");
    if(nIndex >= 0)
    {
        omStrFunction.Insert(nIndex, " ");
        omStrFunction.Insert(nIndex+1, defGCC_EXPORT);
        omStrFunction = " " + omStrFunction;
        omStrFunction = defGCC_EXTERN + omStrFunction;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}