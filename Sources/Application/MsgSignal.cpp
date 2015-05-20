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
 * \file      MsgSignal.cpp
 * \brief     This file contain definition of all function of CMsgSignal
 * \author    Amarnath Shastry
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * This file contain definition of all function of CMsgSignal
 */

#include "stdafx.h"         // Standard header include file
#include "Struct.h"
#include "Flags.h"
#include "Utility/Utility.h"
#include "Utility/UtilFunctions.h"
#include "MsgSignal.h"      // Class defintion file
#include "MessageAttrib.h"   //Saving contents on dissocation of database
#include "BUSMASTER.h"
#include <algorithm>
#include <map>
#include <list>
#include "FibexClass_extern.h"
static CHAR s_acTraceStr[1024] = {""};

//Trace window ptr
CUIThread* CMsgSignal::m_pUIThread = nullptr;

extern CCANMonitorApp theApp;       // Application object
extern "C" int  nParseLDFFile(std::string fpInputFile, CHANNEL_CONFIG& ouCluster);


/* Helper function to calculate how many bytes the signal consumes */
UINT static nGetNoOfBytesToRead(UINT nBitNum, UINT nSigLen)
{
    ASSERT(nSigLen > 0);
    UINT nBytesToRead = 1; //Consider first byte

    INT nRemainingLength = nSigLen - (defBITS_IN_BYTE - nBitNum);

    if (nRemainingLength > 0)
    {
        // Add te number of bytes totally it consumes.
        nBytesToRead += (INT)(nRemainingLength / defBITS_IN_BYTE);

        // Check for extra bits which traverse to the next byte.
        INT nTotalBitsConsidered = ((nBytesToRead - 1) * defBITS_IN_BYTE) +
                                   (defBITS_IN_BYTE - nBitNum);
        if ((UINT)nTotalBitsConsidered < nSigLen)
        {
            nBytesToRead++;
        }
    }

    return nBytesToRead;
}
/* checks whether signal cross array boundary or not */
BOOL CMsgSignal::bValidateSignal(UINT nDLC, UINT nByteNum, UINT nBitNum,
                                 UINT nLength, EFORMAT_DATA bDataFormat)
{
    BOOL bValid = TRUE;
    UINT nBytesToRead = nGetNoOfBytesToRead(nBitNum, nLength);
    bValid = (bDataFormat == DATA_FORMAT_INTEL)?
             (INT)(nByteNum + nBytesToRead - 1) <= nDLC :
             (INT)(nByteNum - nBytesToRead) >= 0;
    return bValid;
}
/* Helper function to calculate the bit mask of a signal */
BOOL CMsgSignal::bCalcBitMaskForSig(BYTE* pbyMaskByte, UINT unArrayLen,
                                    UINT nByteNum, UINT nBitNum, UINT nLength,
                                    EFORMAT_DATA bDataFormat)
{
    BOOL bValid = TRUE;
    //Reset the Byte array
    memset(pbyMaskByte, 0, sizeof(BYTE) * unArrayLen);

    // Calculate how many bytes the signal occupies
    UINT nBytesToRead = nGetNoOfBytesToRead(nBitNum, nLength);

    UINT CurrBitNum = nBitNum;
    /* If the Byte order is the motorola Bit mask has to be updated in
    the reverse order */
    INT nByteOrder = (bDataFormat == DATA_FORMAT_INTEL) ? 1: -1;
    bValid = bValidateSignal(unArrayLen, nByteNum, nBitNum, nLength, bDataFormat);

    if (bValid == TRUE)
    {
        UINT nBitsRead = 0;

        for (UINT i = 0; i < nBytesToRead; i++)
        {
            BYTE byMsgByteVal = 0xFF; //Mask
            if (CurrBitNum != 0)
            {
                byMsgByteVal <<= CurrBitNum; //Set the bits high after the bit number
            }
            // Check for the extra bits at the end and set them low.
            INT ExtraBits = (defBITS_IN_BYTE - CurrBitNum) - (nLength - nBitsRead);
            BYTE nMask = (BYTE)((ExtraBits > 0) ? 0xFF >> ExtraBits : 0xFF);
            byMsgByteVal &= nMask;

            //Calculate the bits read for each time
            nBitsRead += min (defBITS_IN_BYTE - CurrBitNum, nLength - nBitsRead);
            /*Reset the current bit num since bit number always starts from
            the zero after the first byte has been read*/
            CurrBitNum = 0;
            //Update the mask in the corresponding byte of the array
            pbyMaskByte[(nByteNum - 1) + (i * nByteOrder)] = byMsgByteVal;
        }
    }

    return bValid;
}
/* End of helper function vCalcBitMaskForSig */


/******************************************************************************
  Function Name    :  CMsgSignal

  Input(s)         :
  Output           :
  Functionality    :  Constructor
  Member of        :  CMsgSignal
  Friend of        :      -

  Author(s)        :  Amarnath Shastry
  Date Created     :  15.02.2002
  Modifications    :
******************************************************************************/
CMsgSignal::CMsgSignal(const SDBPARAMS& sDbParams, BOOL bInAutoSrvMode)
{
    m_bAutoServerMode           = bInAutoSrvMode;
    m_sDbParams                 = sDbParams;
    m_strActiveDBFileName       = "";
    m_unMessageCount            = 0;
    m_psMessages                = nullptr;
    m_bIsDatabaseSaved          = TRUE;
    m_bIsDatabaseActive         = FALSE;
    m_psDatbaseStructList       = nullptr;
    // Initialise the hash table. For best performance, the hash table size
    // should be a prime number. To minimize collisions the size should be
    // roughly 20 percent larger than the largest anticipated data set.
    // Hence SIZE_HASH_TABLE
    m_omMsgDetailsIDMap.InitHashTable(SIZE_HASH_TABLE_DBMSGS);
}
/******************************************************************************
  Function Name    :  ~CMsgSignal

  Input(s)         :
  Output           :
  Functionality    :  Destructor
  Member of        :  CMsgSignal
  Friend of        :      -

  Author(s)        :  Amarnath Shastry
  Date Created     :  15.02.2002
  Modifications    :
******************************************************************************/
CMsgSignal::~CMsgSignal()
{

}
/*******************************************************************************
  Function Name    :  bWriteDBHeader
  Input(s)         :    -
  Output           :  TRUE/FALSE
  Functionality    :  Creates a "Unions.h" file and adds union defintions
                      to it.
  Member of        :  CMsgSignal
  Friend of        :      -

  Author(s)        :  Amarnath Shastry
  Date Created     :  15.02.2002
  Modifications    :  Amitesh Bharti, 12.03.2003
                      changes made to create unions.h structures as per
                      CRH0002
  Modifications    :  Raja N 12.02.2004
                      Modified to include Sign check for signals and moved
                      some hardcoded strings to Hashdefines.h
  Modifications    :  Raja N 18.05.2004
                      Added two more members in the union to give word and long
                      access of the data array
  Modifications    :  Raja N 18.05.2004
                      Added two more members in the union to give word and long
                      access of the data array
  Modifications    :  Anish 21.12.2006
                      Added code to have header name as header file name in
                      ifndef condition at the begining of *_UNIONS.h file
*******************************************************************************/
CString CMsgSignal::bWriteDBHeader(CString omStrActiveDataBase)
{
    BOOL bRetVal                = TRUE;
    CString omStrPath           = "";
    //Add header for ifndef condition
    CString omStrHeaderString   = "";
    CString omStrHeaderFileName = defHEADER_FILE_NAME;
    char  acErrorMsg[defSIZE_OF_ERROR_BUFFER];
    CFileException   omException  ;
    CStdioFile omHeaderFile;
    omStrHeaderFileName = omStrActiveDataBase.Left(
                              omStrActiveDataBase.ReverseFind('.') );
    omStrHeaderString = omStrHeaderFileName;
    omStrHeaderFileName += defHEADER_FILE_NAME;
    TRY
    {
        // Open HeaderFile
        bRetVal = omHeaderFile.Open( omStrHeaderFileName,
        CFile::modeCreate|CFile::modeWrite|CFile::typeText,&omException);
        if (bRetVal == FALSE)
        {
            // Get the exception error message
            omException.GetErrorMessage(acErrorMsg,sizeof(acErrorMsg));
            // union.h file  open error notification
            // AfxMessageBox and MessageBox does not work here. So the API
            // call for MessageBox is used.
            if (m_bAutoServerMode == FALSE)
            {
                ::MessageBox(nullptr,acErrorMsg,"BUSMASTER" ,MB_ICONERROR|MB_OK);
            }
        }
        else
        {
            int nIndex = omStrHeaderString.ReverseFind(defCHAR_PATH_SEPRATER);
            int nLength = omStrHeaderString.GetLength();
            omStrHeaderString = omStrHeaderString.Right(nLength - nIndex -1);
            omStrHeaderString.MakeUpper();
            CString omStrTemp;
            omStrTemp.Format(H_FILE_HEADER_START,omStrHeaderString,omStrHeaderString);

            // Add Header "Help information"
            omHeaderFile.WriteString(defHELP_INFO_IN_UNIONS);

            // Add Header "ifndef..."
            omHeaderFile.WriteString(omStrTemp);

            // Add Pragma Pack
            omStrTemp.Format(H_FILE_HEADER_PRAGMA_PACK,omStrHeaderString,omStrHeaderString);
            omHeaderFile.WriteString(omStrTemp);

            CString omStrDLC = "";
            UINT aunSigStartBit[defMAX_SIGNALS] ;
            UINT aunLength[defMAX_SIGNALS] ;
            CStringArray omStrArraySigName;
            omStrArraySigName.RemoveAll();
            BOOL bReturn = FALSE;
            UINT unSigCount = 0;
            CString omStrcommandLine = "";
            CString omStrSigName = "";
            CString omStrdelimiter = "";
            for ( UINT unMsgIndex = 0; unMsgIndex < m_unMessageCount; unMsgIndex++)
            {
                // Get all signal names.
                // signal name will be the variable name
                // of the union of length specified in DB
                unSigCount = 0;

                sSIGNALS* pSg = m_psMessages[unMsgIndex].m_psSignals;
                while  ( pSg != nullptr )
                {
                    UINT nSize = omStrArraySigName.GetSize();
                    aunSigStartBit[unSigCount] =
                    (pSg->m_unStartByte - 1 ) * defBITS_IN_BYTE;
                    aunSigStartBit[unSigCount] += pSg->m_byStartBit;
                    unSigCount++;
                    pSg = pSg->m_psNextSignalList;
                }
                // Check if there is no signal add one signal declaration
                // with no name occuppying whole message length.
                if(m_psMessages[unMsgIndex].m_unNumberOfSignals>0 )
                {
                    bReturn =
                        bSortSignalStartBitAscend(aunSigStartBit,
                                                  m_psMessages[unMsgIndex].m_unNumberOfSignals);
                    if(bReturn == TRUE )
                    {
                        switch (m_sDbParams.m_eBus)
                        {
                            case CAN:
                            {
                                bReturn = bFormSigNameAndLength(aunLength,
                                                                aunSigStartBit,
                                                                omStrArraySigName,
                                                                unMsgIndex);
                            }
                            break;
                            default: //This is a general routine which contructs
                                //structure based on message length and its signal length
                            {
                                bReturn = bFormSigNameAndLengthJ1939(aunSigStartBit,
                                                                     omStrArraySigName,
                                                                     unMsgIndex);
                            }
                            break;
                        }
                    }
                }
                else
                {
                    CString omFormatString;
                    // If message length is more then four byte
                    // define two signal with no name.
                    UINT unTempLen = m_psMessages[unMsgIndex].m_unMessageLength;
                    while (unTempLen > defUINT_SIZE)
                    {
                        omFormatString.Format(defUNION_FORMAT_STRING,
                                              defUNSIGNED_INT,
                                              "",
                                              defUINT_LENGTH);
                        omStrArraySigName.Add(omFormatString);
                        unSigCount++;
                        unTempLen -= defUINT_SIZE;
                    }
                    UINT unLength = m_psMessages[unMsgIndex].m_unMessageLength
                                    * defBITS_IN_BYTE
                                    - ( unSigCount * defUINT_LENGTH );
                    omFormatString.Format(defUNION_FORMAT_STRING,
                                          defUNSIGNED_INTEGER,
                                          "",
                                          unLength);
                    omStrArraySigName.Add(omFormatString);

                    unSigCount = 0;
                    bReturn = TRUE;
                }
                //                INT nIndex = 0;
                if(bReturn == TRUE )
                {
                    bInsertBusSpecStructures(omHeaderFile,
                                             omStrcommandLine,
                                             omStrArraySigName,
                                             &(m_psMessages[unMsgIndex]));
                }
            }

            // add "#endif.."
            //omHeaderFile.WriteString(H_FILE_HEADER_END);

            omStrPath = omStrHeaderFileName;
            // Close opened file
            omHeaderFile.Close();
        }
    }
    CATCH_ALL(pomE)
    {
        if(pomE != nullptr )
        {
            pomE->GetErrorMessage(acErrorMsg ,sizeof(acErrorMsg) );
            bRetVal = FALSE;
            pomE->Delete();
            // union.h file  open error notification
            if (m_bAutoServerMode == FALSE)
            {
                ::MessageBox(nullptr,acErrorMsg,"BUSMASTER" ,MB_ICONERROR|MB_OK);
            }
        }
        // Close opened file
        omHeaderFile.Close();
    }
    END_CATCH_ALL

    return omStrPath;
}
/******************************************************************************
  Function Name    :  bValidateDatabaseFile

  Input(s)         :  CString strFileName
  Output           :  TRUE/FALSE
  Functionality    :  Validates Db file for its extension and header
                      information.
  Member of        :  CMsgSignal
  Friend of        :      -

  Author(s)        :  Amarnath Shastry
  Date Created     :  15.02.2002
  Modifications    :
******************************************************************************/
BOOL CMsgSignal::bValidateDatabaseFile(CString strFileName)
{
    BOOL bReturnValue = FALSE;

    if (strFileName.IsEmpty())
    {
        strcpy_s(s_acTraceStr,1024, _("Database filename is empty."));

        if (m_bAutoServerMode == FALSE)
        {
            //vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM) s_acTraceStr);
        }
    }
    else
    {
        // Verify for ".dbf" extn
        int nIndex = strFileName.ReverseFind(PERIOD);

        if ( nIndex == -1 ) // not found
        {
            strcpy_s(s_acTraceStr, 1024, _("Invalid database filename"));
            if(m_bAutoServerMode == FALSE)
            {
                //vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0,
                //(LPARAM) s_acTraceStr);
            }
        }
        else // found
        {
            CString sFileExtn = strFileName.Right( (strFileName.GetLength() - nIndex) - 1 );

            sFileExtn.MakeUpper();

            if ( sFileExtn != DATABASE_EXTN)
            {
                strcpy_s(s_acTraceStr, 1024, _("Invalid database filename"));
                if(m_bAutoServerMode == FALSE)
                {
                    //vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0,
                    //(LPARAM) s_acTraceStr);
                }
            }
            else
            {
                // file-attribute information
                struct _finddata_t fileinfo;

                // If file doesn't exist, return
                if (_findfirst( strFileName, &fileinfo)== -1)
                {
                    return FALSE;
                }
                else
                {
                    CStdioFile o_File;
                    TRY
                    {
                        // Open File
                        o_File.Open( strFileName, CFile::modeRead|CFile::typeText );

                        CString sFirstLine = "";

                        // Read file line
                        o_File.ReadString( sFirstLine );

                        // First line of the file
                        if ( sFirstLine == DATABASE_HEADER || sFirstLine == DATABASE_HEADER_NEW ||
                        sFirstLine == CANVAS_DATABASE_HEADER || sFirstLine == CANVAS_DATABASE_HEADER_NEW )
                        {
                            bReturnValue = TRUE;
                        }

                        // close File
                        o_File.Close();
                    }
                    CATCH_ALL (pomE)
                    {
                        if(pomE != nullptr )
                        {
                            LPTSTR lpszError = "";
                            // Get error
                            pomE->GetErrorMessage( lpszError, defSIZE_OF_ERROR_BUFFER);
                            strcpy_s(s_acTraceStr, 1024, T2A("lpszError"));
                            if(m_bAutoServerMode == FALSE)
                            {
                                vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0,
                                //(LPARAM) s_acTraceStr);
                            }
                            pomE->Delete();
                        }
                    }
                    END_CATCH_ALL

                }
            }
        }
    }

    return (bReturnValue);
}
/******************************************************************************
  Function Name    :  bAllocateMemory

  Input(s)         :  BOOL nMode
  Output           :  TRUE/FALSE
  Functionality    :  Deletes/Reserves memory for a message deleted/added
  Member of        :  CMsgSignal
  Friend of        :      -

  Author(s)        :  Amarnath Shastry
  Date Created     :  15.02.2002
  Modifications    :
******************************************************************************/
BOOL CMsgSignal::bAllocateMemory(BOOL nMode)
{
    BOOL bReturnValue = TRUE;

    UINT unCount = 0;

    if ( nMode == MD_ADD )
    {
        unCount = ++m_unMessageCount;
    }
    else if ( nMode == MD_DELETE)
    {
        // Already decremented
        unCount = m_unMessageCount;
    }

    // Create a temp array of object with new count
    sMESSAGE* pNewMsgArray = new sMESSAGE[unCount];

    if (pNewMsgArray == nullptr)
    {
        if (!m_bAutoServerMode)
        {
            strcpy_s (s_acTraceStr, 1024,  T2A(MSG_MEMORY_CONSTRAINT));
            vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
        }
        bReturnValue = FALSE;
    }
    else
    {
        // copy all the data into this new pointer
        if ( nMode == MD_ADD )
        {
            for ( UINT i = 0 ; i < (unCount - 1); i++ )
            {
                pNewMsgArray[i] = m_psMessages[i];
            }
        }
        else if ( nMode == MD_DELETE )
        {
            for ( UINT i = 0 ; i < unCount ; i++ )
            {
                pNewMsgArray[i] = m_psMessages[i];
            }
        }

        // delete the global msg pointer
        delete [] m_psMessages;

        m_psMessages = nullptr;

        // reallocate the global pointer to hold new msg info
        m_psMessages = new sMESSAGE[unCount];

        if (m_psMessages != nullptr)
        {
            // copy back all the data from the temp obj
            for ( UINT i = 0; i < unCount; i++)
            {
                m_psMessages[i] = pNewMsgArray[i];
            }

            // delete the temp obj
            delete [] pNewMsgArray;
            pNewMsgArray = nullptr;

            // allocate memory to hold new message and
            // assign defaults
            if ( nMode == MD_ADD )
            {
                m_psMessages[unCount - 1].m_omStrMessageName.Empty();

                m_psMessages[unCount - 1].m_psSignals = nullptr;

                m_psMessages[unCount - 1].m_unMessageCode = 0;

                m_psMessages[unCount - 1].m_unMessageLength = 8;

                m_psMessages[unCount - 1].m_unNumberOfSignals = 0;

                m_psMessages[unCount - 1].m_bMessageFrameFormat = 0;

                m_psMessages[unCount - 1].m_nMsgDataFormat = 1;

                // initialise all the matrix elements
                // to zero for this message
                for ( int nCount = 0; nCount < 1785; nCount++ )
                {
                    m_psMessages[unCount - 1].m_bySignalMatrix[nCount] = 0x00;
                }
            }
        }
        else
        {
            if (!m_bAutoServerMode)
            {
                strcpy_s (s_acTraceStr, 1024, T2A(_(MSG_MEMORY_CONSTRAINT)));
                vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
            }
            bReturnValue = FALSE;
        }

    }

    return bReturnValue;
}


/******************************************************************************
  Function Name    :  bDeAllocateMemory

  Input(s)         :  DataBaseName
  Output           :  TRUE/FALSE
  Functionality    :  Find the the DB struct poiner and call the function to
                      Deletes all memory allocated for the particular DB
  Member of        :  CMsgSignal
  Friend of        :      -

  Author(s)        :  Anish Kumar
  Date Created     :  15.11.2006
  Modifications    :
******************************************************************************/
BOOL CMsgSignal::bDeAllocateMemory(CString strDBName)
{
    m_omDBMapCritSec.Lock();
    BOOL bReturn = FALSE;
    if(m_psDatbaseStructList != nullptr)
    {
        BOOL bFileEmpty = strDBName.IsEmpty();
        // If it is Empty delete all the database structures
        if( bFileEmpty == TRUE )
        {
            sDBFileStruct* psTempDatbaseStructList;
            while( m_psDatbaseStructList != nullptr )
            {
                psTempDatbaseStructList = m_psDatbaseStructList;
                m_psDatbaseStructList = m_psDatbaseStructList->m_psNextDBFileStruct;
                bReturn = bDeAllocateDBMemory(psTempDatbaseStructList);
                delete psTempDatbaseStructList;
                psTempDatbaseStructList = nullptr;
            }
        }
        else
        {
            sDBFileStruct* psCurrDatbaseStructList;
            sDBFileStruct* psPrevDatbaseStructList;
            psCurrDatbaseStructList = m_psDatbaseStructList;
            psPrevDatbaseStructList = m_psDatbaseStructList;
            //If it is first database
            if( !psCurrDatbaseStructList->m_omStrDatabasePath.Compare(strDBName) )
            {
                m_psDatbaseStructList = m_psDatbaseStructList->m_psNextDBFileStruct;
                bReturn = bDeAllocateDBMemory(psCurrDatbaseStructList);
                delete psCurrDatbaseStructList;
                psCurrDatbaseStructList = nullptr;
            }
            //Search the Database struct for the particular DB path
            else
            {
                while(psCurrDatbaseStructList != nullptr)
                {
                    if(!(psCurrDatbaseStructList->m_omStrDatabasePath.Compare(strDBName)))
                    {
                        psPrevDatbaseStructList->m_psNextDBFileStruct =
                            psCurrDatbaseStructList->m_psNextDBFileStruct;

                        bReturn = bDeAllocateDBMemory(psCurrDatbaseStructList);
                        delete psCurrDatbaseStructList;
                        psCurrDatbaseStructList = nullptr;
                    }
                    else
                    {
                        psPrevDatbaseStructList = psCurrDatbaseStructList;
                        psCurrDatbaseStructList =
                            psCurrDatbaseStructList->m_psNextDBFileStruct;
                    }
                }
            }
        }
    }
    //Reset the Map contents
    vResetMsgMapContent();
    //Reset Message attrib contents
    CMessageAttrib::ouGetHandle(m_sDbParams.m_eBus).vSaveMessageAttribData();
    //If it is the last database
    if(m_psDatbaseStructList == nullptr)
    {
        /*CMainFrame* pMainFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;*/
        //No database selected
        if (m_sDbParams.m_eBus == CAN)
        {
            CFlags::ouGetFlagObj().vSetFlagStatus(SELECTDATABASEFILE, FALSE);
            //theApp.pouGetFlagsPtr()->vSetFlagStatus(SELECTDATABASEFILE, FALSE);
        }
        // Clear Message Interpretation Window Content
        //pMainFrm->vClearInterpretationWindow();
        //// Clear Signal Watch List
        //pMainFrm->vEmptySignalWatchList();
        //// Clear Graph List
        //pMainFrm->m_podGraphList->m_omElementList.RemoveAll();
    }
    m_omDBMapCritSec.Unlock();
    return bReturn;
}


/******************************************************************************
  Function Name    :  bDeAllocateDBMemory

  Input(s)         :  DataBase structure pointer
  Output           :  TRUE/FALSE
  Functionality    :  Deletes all memory allocated for the particular DB messages
  Member of        :  CMsgSignal
  Friend of        :      -

  Author(s)        :  Amarnath Shastry
  Date Created     :  15.02.2002
  Modifications    :  Anish Kumar,30.11.2006
                      The function is changed for Multi database requirement
******************************************************************************/
BOOL CMsgSignal::bDeAllocateDBMemory(sDBFileStruct* psDatbaseStructList)
{
    if(psDatbaseStructList != nullptr)
    {
        UINT unMessageCount = psDatbaseStructList->m_unMessageCount;
        sMESSAGE* psMessages = psDatbaseStructList->m_psMessages;
        for ( UINT nMsgIndex = 0; nMsgIndex < unMessageCount; nMsgIndex++ )
        {
            if ( &psMessages[nMsgIndex] != nullptr )
            {
                if ( psMessages[nMsgIndex].m_psSignals != nullptr )
                {
                    sSIGNALS* pDelSg;
                    sSIGNALS* pTemSg;

                    pTemSg = pDelSg =
                                 psMessages[nMsgIndex].m_psSignals;

                    // Delete all signals associated with the message
                    while ( pTemSg != nullptr )
                    {
                        pDelSg = pTemSg;

                        if ( pTemSg->m_oSignalIDVal != nullptr )
                        {
                            CSignalDescVal* pDelIDVal;
                            CSignalDescVal* pTemIDVal;

                            pTemIDVal = pDelIDVal = pTemSg->m_oSignalIDVal;

                            // Delete all associated signal ID's and Val
                            while ( pTemIDVal != nullptr )
                            {
                                pDelIDVal=pTemIDVal;

                                // Traverse to next ID-Val
                                if ( pTemIDVal != nullptr )
                                {
                                    pTemIDVal = pDelIDVal->m_pouNextSignalSignalDescVal;

                                    delete pDelIDVal;

                                    pDelIDVal = nullptr;
                                }

                            }

                            pTemSg->m_oSignalIDVal = nullptr;

                        }// if

                        // Traverse to next signal
                        if ( pTemSg != nullptr )
                        {
                            pTemSg = pDelSg->m_psNextSignalList;

                            delete pDelSg;

                            pDelSg = nullptr;

                        }

                    }// while

                    psMessages[nMsgIndex].m_psSignals = nullptr;

                }// if
            }
        }

        if ( psMessages != nullptr )
        {
            delete [] psMessages;

            unMessageCount = 0;

            psMessages = nullptr;
        }
    }

    return TRUE;
}
/******************************************************************************
  Function Name    :  bDeAllocateMemoryInactive

  Input(s)         :  -
  Output           :  TRUE/FALSE
  Functionality    :  Deletes all memory allocated for DB messages
                      When dealing with Inactive DB
  Member of        :  CMsgSignal
  Friend of        :      -

  Author(s)        :  Amarnath Shastry
  Date Created     :  15.02.2002
  Modifications    :  Anish Kumar,30.11.2006
                      The function is changed for Multi database requirement
  Modifications    :  Anish Kumar,30.01.2007
                      modified to remove memory leak
******************************************************************************/
BOOL CMsgSignal::bDeAllocateMemoryInactive()
{
    // Clearing the Map on close database
    m_omMsgDetailsIDMap.RemoveAll();
    for ( UINT nMsgIndex = 0; nMsgIndex < m_unMessageCount; nMsgIndex++ )
    {
        if ( &m_psMessages[nMsgIndex] != nullptr )
        {
            if ( m_psMessages[nMsgIndex].m_psSignals != nullptr )
            {
                sSIGNALS* pDelSg;
                sSIGNALS* pTemSg;

                pTemSg = pDelSg =
                             m_psMessages[nMsgIndex].m_psSignals;

                // Delete all signals associated with the message
                while ( pTemSg != nullptr )
                {
                    pDelSg = pTemSg;

                    if ( pTemSg->m_oSignalIDVal != nullptr )
                    {
                        CSignalDescVal* pDelIDVal;
                        CSignalDescVal* pTemIDVal;

                        pTemIDVal = pDelIDVal = pTemSg->m_oSignalIDVal;

                        // Delete all associated signal ID's and Val
                        while ( pTemIDVal != nullptr )
                        {
                            pDelIDVal=pTemIDVal;

                            // Traverse to next ID-Val
                            if ( pTemIDVal != nullptr )
                            {
                                pTemIDVal = pDelIDVal->m_pouNextSignalSignalDescVal;

                                delete pDelIDVal;

                                pDelIDVal = nullptr;
                            }

                        }

                        pTemSg->m_oSignalIDVal = nullptr;

                    }// if

                    // Traverse to next signal
                    if ( pTemSg != nullptr )
                    {
                        pTemSg = pDelSg->m_psNextSignalList;

                        delete pDelSg;

                        pDelSg = nullptr;

                    }

                }// while

                m_psMessages[nMsgIndex].m_psSignals = nullptr;

            }// if
        }
    }

    if ( m_psMessages != nullptr )
    {
        delete [] m_psMessages;

        m_unMessageCount = 0;

        m_psMessages = nullptr;

        delete m_psDatbaseStructList;

        m_psDatbaseStructList = nullptr;

    }


    return TRUE;
}
/******************************************************************************
  Function Name    :  nGetMessageCode

  Input(s)         :  CString strMsgName
  Output           :  int
  Functionality    :  Returns valid message code if message is found,
                      otherwise -1.
  Member of        :  CMsgSignal
  Friend of        :      -

  Author(s)        :  Amarnath Shastry
  Date Created     :  15.02.2002
  Modifications    :
******************************************************************************/
int CMsgSignal::nGetMessageCode( CString strMsgName)
{
    int nMsgCode = -1;
    POSITION pos = m_omMsgDetailsIDMap.GetStartPosition();
    sMESSAGE* psMsgStruct = nullptr;
    BOOL bFound = FALSE;
    UINT unKey;
    while (pos != nullptr && bFound != TRUE)
    {
        m_omMsgDetailsIDMap.GetNextAssoc( pos, unKey,psMsgStruct );
        if(!(psMsgStruct->m_omStrMessageName.Compare(strMsgName)))
        {
            nMsgCode = (int)psMsgStruct->m_unMessageCode;
            bFound = TRUE;
        }
    }
    return nMsgCode;
}
/******************************************************************************
  Function Name    :  vGetDataBaseNames

  Input(s)         :
  Output           :  CStringArray of dB names
  Functionality    :  Returns the names of all the databases
  Member of        :  CMsgSignal
  Friend of        :      -

  Author(s)        :  Anish kumar
  Date Created     :  06.12.2006
  Modifications    :
******************************************************************************/
void CMsgSignal::vGetDataBaseNames(CStringArray* pastrDBnames)
{
    if(pastrDBnames != nullptr)
    {
        pastrDBnames->RemoveAll();
        pastrDBnames->Append(m_omDatabaseNames);
    }
}
void CMsgSignal::vGetRelativeDataBaseNames(std::string& omStrBasePath, CStringArray* pastrDBnames)
{
    if(pastrDBnames != nullptr)
    {
        pastrDBnames->RemoveAll();
        for ( int i = 0, n = m_omDatabaseNames.GetSize(); i < n; i++)
        {
            CString omStrTemp = m_omDatabaseNames.GetAt(i);
            std::string omStrRelativePath;
            CUtilFunctions::MakeRelativePath(omStrBasePath.c_str(), omStrTemp.GetBuffer(MAX_PATH), omStrRelativePath);
            pastrDBnames->Add(omStrRelativePath.c_str());
        }
    }
}
void CMsgSignal::vSetDataBaseNames(const CStringArray* pastrDBnames)
{
    if(pastrDBnames != nullptr)
    {
        m_omDatabaseNames.RemoveAll();
        m_omDatabaseNames.Append(*pastrDBnames);
    }
}
void CMsgSignal::bAddDbNameEntry(const CString& omDbFileName)
{
    m_omDatabaseNames.Add(omDbFileName);
}
/******************************************************************************
  Function Name    :  omStrGetMessageNameFromMsgCode

  Input(s)         :  UINT unMsgCode
  Output           :  CString
  Functionality    :  Returns message name from message code if found,
                      otherwise empty string.
  Member of        :  CMsgSignal
  Friend of        :      -

  Author(s)        :  Amarnath Shastry
  Date Created     :  15.02.2002
  Modifications    :  Anish
                      13.12.2006,Changed code for Multiple DB concept
******************************************************************************/
CString CMsgSignal::omStrGetMessageNameFromMsgCode( UINT unMsgCode)
{
    CString strMsgName = "";
    if (unMsgCode >= 0)
    {
        sMESSAGE* psMsgStruct = nullptr;
        m_omMsgDetailsIDMap.Lookup(unMsgCode,psMsgStruct);
        if(psMsgStruct != nullptr)
        {
            strMsgName = psMsgStruct->m_omStrMessageName;
        }
    }
    return strMsgName;
}
/******************************************************************************
  Function Name    :  omStrGetMessageNameFromMsgCodeInactive

  Input(s)         :  UINT unMsgCode
  Output           :  CString
  Functionality    :  Returns message name from message code if found,
                      otherwise empty string.
  Member of        :  CMsgSignal
  Friend of        :      -

  Author(s)        :  Amarnath Shastry
  Date Created     :  15.02.2002
  Modifications    :  Anish
                      13.12.2006,Changed code for Multiple DB concept
******************************************************************************/
CString CMsgSignal::omStrGetMessageNameFromMsgCodeInactive( UINT unMsgCode)
{
    CString strMsgName = "";

    // validate message code
    if (unMsgCode >= 0)
    {
        for ( UINT unMsgCount = 0; unMsgCount < m_unMessageCount; unMsgCount++ )
        {
            if ( m_psMessages[unMsgCount].m_unMessageCode
                    == unMsgCode )
            {
                strMsgName = m_psMessages[unMsgCount].m_omStrMessageName;

                break;
            }
        }
    }

    return (strMsgName);
}
/******************************************************************************
  Function Name    :  bMessageNameFromMsgCode

  Input(s)         :  UINT unMsgCode
  Output           :  TRUE or FALSE
  Functionality    :  Returns TRUE if message name for message code unMsgCode
                      is found,otherwise FALSE. The message name will be
                      returned through omMsgName.
  Member of        :  CMsgSignal
  Friend of        :      -

  Author(s)        :  Amarnath Shastry
  Date Created     :  15.02.2002
  Modifications    :  Anish
                      13.12.2006,Changed code for Multiple DB concept
******************************************************************************/
BOOL CMsgSignal::bMessageNameFromMsgCode(UINT unMsgCode, CString& omMsgName)
{
    BOOL bResult = FALSE;
    if ( m_sDbParams.m_eBus == FLEXRAY && unMsgCode >= 0)
    {
        static CFrameDef ouFrameDef;
        BOOL bRet = m_ouFrameDataSet.Lookup(unMsgCode, ouFrameDef);
        if (bRet)
        {
            omMsgName = ouFrameDef.m_omNameFrame;
            bResult = TRUE;
        }
    }
    else if (unMsgCode >= 0)
    {
        sMESSAGE* psMsgStruct = nullptr;
        m_omMsgDetailsIDMap.Lookup(unMsgCode,psMsgStruct);
        if(psMsgStruct != nullptr)
        {
            omMsgName = psMsgStruct->m_omStrMessageName;
            bResult = TRUE;
        }
    }
    return bResult;
}

BOOL CMsgSignal::bMessageLengthFromMsgCode(UINT unMsgCode, CString& omMsgLength)
{
    BOOL bResult = FALSE;
    if (unMsgCode >= 0)
    {
        sMESSAGE* psMsgStruct = nullptr;
        m_omMsgDetailsIDMap.Lookup(unMsgCode,psMsgStruct);
        if(psMsgStruct != nullptr)
        {
            CString omstrMsgLength = "";

            omstrMsgLength.Format("%d", psMsgStruct->m_unMessageLength);

            omMsgLength = omstrMsgLength;
            bResult = TRUE;
        }
    }
    return bResult;
}
/******************************************************************************
  Function Name    :  bCreateDataBase

  Input(s)         :  CString omStrFilename
  Output           :  BOOL
  Functionality    :  Creates new database file and adds header information
                      to it.
  Member of        :  CMsgSignal
  Friend of        :      -

  Author(s)        :  Amarnath Shastry
  Date Created     :  15.02.2002
  Modifications    :
******************************************************************************/
BOOL CMsgSignal::bCreateDataBase(CString omStrFilename)
{
    BOOL bRetVal = FALSE;
    // For File I/O
    CStdioFile o_File;
    TRY
    {
        // Open File
        if ( o_File.Open( omStrFilename, CFile::modeCreate|CFile::modeWrite|CFile::typeText ))
        {
            // Write Header information

            // Write Header information
            o_File.WriteString( _(DATABASE_HEADER_NEW) );

            // writeVersion info
            CString strBuffer = NEW_LINE;
            strBuffer += NEW_LINE;
            strBuffer += DATABASE_VERSION;
            strBuffer += SPACE;
            strBuffer += DATABASE_VERSION_NO;
            strBuffer += NEW_LINE;
            strBuffer += NEW_LINE;
            // Adding Protocol information in the version 1.3
            strBuffer += DATABASE_PROTOCOL;
            strBuffer += NEW_LINE;
            strBuffer += DATABASE_PROTOCOL_CAN;

            strBuffer += NEW_LINE;
            strBuffer += NEW_LINE;


            // Adding Application version information in the version 1.3
            CString omstrAppVersion = APPLICATION_VERSION_NO;

            CString omstrVersion;

            // Application version
            omstrVersion.Format("%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);

            omstrAppVersion.Replace(APPLICATION_VERSION_PLACE_HOLDER, omstrVersion);

            // Adding Application Version
            strBuffer += APPLICATION_VERSION;
            strBuffer += SPACE;
            strBuffer += omstrAppVersion;

            strBuffer += NEW_LINE;
            strBuffer += NEW_LINE;

            o_File.WriteString( strBuffer );


            // write Number of messages info
            strBuffer = "[";
            strBuffer += NUMBER_OF_MESSAGES;
            strBuffer += "]";
            strBuffer += " ";
            strBuffer += "0";

            o_File.WriteString( strBuffer );

            o_File.Close();

            bRetVal = TRUE;

            omStrFilename += _(" created successfully.");

            if (!m_bAutoServerMode)
            {
                strcpy_s (s_acTraceStr, 1024, T2A(omStrFilename.GetBuffer(MAX_PATH)));
                vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
            }
        }
    }
    CATCH_ALL (pomE)
    {
        if(pomE != nullptr )
        {
            LPTSTR lpszError = "";;
            // Get error
            pomE->GetErrorMessage( lpszError, 255);
            if (!m_bAutoServerMode)
            {
                strcpy_s (s_acTraceStr, 1024,  T2A(lpszError));
                vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
            }
            pomE->Delete();
        }

        bRetVal = FALSE; // Invalid Db file
    }
    END_CATCH_ALL

    return bRetVal;

}
/******************************************************************************
  Function Name    :  nGetMessageIndexFromMsgName

  Input(s)         :  CString strMsgName
  Output           :  int
  Functionality    :  If found, returns index of the message asked for,
                      otherwise returns -1.
  Member of        :  CMsgSignal
  Friend of        :      -

  Author(s)        :  Amarnath Shastry
  Date Created     :  15.02.2002
  Modifications    :
******************************************************************************/
int CMsgSignal::nGetMessageIndexFromMsgName( CString strMsgName)
{
    int nMsgIndex = -1;

    // validate message name
    if (!strMsgName.IsEmpty())
    {
        for ( UINT unMsgCount = 0; unMsgCount < m_unMessageCount; unMsgCount++ )
        {
            if ( m_psMessages[unMsgCount].m_omStrMessageName
                    == strMsgName )
            {
                nMsgIndex = unMsgCount;

                break;
            }
        }
    }
    return (nMsgIndex);
}
/******************************************************************************
  Function Name    :  vGetSignalNames

  Input(s)         :  CString strMsgName,
                      CStringList &strSignalList
  Output           :  CStringList &strSignalList
  Functionality    :  Fills the string list with all the signal names
                      associted with the message name.
  Member of        :  CMsgSignal
  Friend of        :      -

  Author(s)        :  Amarnath Shastry
  Date Created     :  15.02.2002
  Modifications    :
  ******************************************************************************/
void CMsgSignal::vGetSignalNames( UINT unMsgID,
                                  CStringList& strSignalList)
{
    strSignalList.RemoveAll();

    sMESSAGE* p_Msg = psGetMessagePointer( unMsgID );

    if (p_Msg != nullptr)
    {
        sSIGNALS* p_Sgs = p_Msg->m_psSignals;

        while ( p_Sgs != nullptr )
        {
            strSignalList.AddHead( p_Sgs->m_omStrSignalName);

            p_Sgs = p_Sgs->m_psNextSignalList;
        }
    }

}
/******************************************************************************
  Function Name    :  bFillDataStructureFromDatabaseFile
  Input(s)         :  CString strFileName
  Output           :  BOOL
  Functionality    :  Reads the DB file specified and fills the data
                      structure
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  15.02.2002
  Modifications    :  Rajesh Kumar 03.03.2003
                      1. Added reading Frame-Format from DB file
                      Rajesh Kumar 04.03.2003
                      1. changes w.r.t change in sSIGNALS
                      Raja N 10.02.2004
                      Included No. Of Messages check in the while loop to avoid
                      crash during corrupted database file loading
 Modifications     :  Anish Kumar 03.12.2006
                      Changes for new database,made searching NUMBER_OF_MESSAGES
                      generic
 Modifications     :  Anish Kumar 09.02.2007
                      Initialize m_unMessageCount before operating on new DB
                      to solve bug due to invalid DB file
******************************************************************************/
BOOL CMsgSignal::bFillDataStructureFromDatabaseFile( CString strFileName, eProtocol eProtocolName)
{
    USES_CONVERSION;
    BOOL bReturnValue   = TRUE;
    BOOL bIsFileOpen    = FALSE;
    BOOL bIsMessageLengthExceeds = FALSE;
    m_unMessageCount = 0;
    // validate the file
    bReturnValue = bValidateDatabaseFile(strFileName);
    if ( bReturnValue )
    {
        char* charArray = nullptr;
        char szSep;
        GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, &szSep, 1);

        // For File I/O
        CStdioFile o_File;
        FLOAT fDBVerNum = 0;
        FLOAT fCurrDBVer = 0;

        TRY
        {
            // Open File
            bIsFileOpen = o_File.Open(
                strFileName, CFile::modeRead|CFile::typeText );
            if(bIsFileOpen != FALSE )
            {
                CString sFirstLine  = "";
                CString omDBVerLine  = "";
                CString omstrDBVersion = "";
                // read subsequent info from the file
                /* Read Database version number*/
                int nIndex = -1;
                while( nIndex == -1 && o_File.ReadString( omDBVerLine ))
                {
                    nIndex = omDBVerLine.Find(DATABASE_VERSION);
                }
                if (nIndex != -1)
                {
                    int nPlace = omDBVerLine.Find('=');
                    if(nPlace == -1)
                    {
                        nPlace = omDBVerLine.Find(']');
                    }
                    CString omDbVerNo =
                        omDBVerLine.Right( omDBVerLine.GetLength() - (nPlace+2));
                    omstrDBVersion = omDbVerNo;
                    fDBVerNum = (FLOAT)atof(T2A(omDbVerNo.GetBuffer(MAX_PATH)));
                    fCurrDBVer = (FLOAT)atof(T2A(DATABASE_VERSION_NO));
                    if (fDBVerNum < fCurrDBVer)
                        // Updated for checking the dbf version with Previous version
                        if (fDBVerNum < fCurrDBVer && omDbVerNo != DATABASE_PREVIOUS_VERSION_NO)
                        {
                            if (!m_bAutoServerMode)
                            {
                                strcpy_s (s_acTraceStr, 1024, T2A(DATABASE_MISMATCH));
                                vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                                m_bIsDatabaseSaved = FALSE;
                            }
                        }
                    if (fDBVerNum > fCurrDBVer)
                    {
                        if (!m_bAutoServerMode)
                        {
                            strcpy_s(s_acTraceStr, 1024, "DATABASE_CORRUPT");
                            vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                            return FALSE;
                        }
                    }
                }
                /* Reading of database version number ends */
                /*Reading Database Protocol and Application version starts here*/
                if(omstrDBVersion == DATABASE_VERSION_NO)
                {
                    o_File.SeekToBegin();
                    CString strDatabaseProtocol, omstrApplicationVersion;

                    // Reading Database Protocol
                    nIndex = -1;
                    while( nIndex == -1 && o_File.ReadString( strDatabaseProtocol ))
                    {
                        // Getting the PROTOCOL name from the loaded dbf file
                        nIndex = strDatabaseProtocol.Find(DATABASE_PROTOCOL);
                    }

                    if ( nIndex != -1 )
                    {
                        int nPlace = strDatabaseProtocol.Find('=');
                        if(nPlace == -1)
                        {
                            nPlace = strDatabaseProtocol.Find(']');
                        }
                        CString omstrDatabaseProtocol;

                        // Getting the PROTOCOL name
                        omstrDatabaseProtocol = strDatabaseProtocol.Right( strDatabaseProtocol.GetLength() - (nPlace+2));

                        if(omstrDatabaseProtocol.IsEmpty() == FALSE)
                        {
                            // On Open can database
                            if(eProtocolName == PROTOCOL_CAN)
                            {
                                // If the loaded file is not CAN database file
                                if(omstrDatabaseProtocol != DATABASE_PROTOCOL_CAN)
                                {
                                    strcpy(s_acTraceStr, strFileName + _(" is not created for CAN. Please load CAN related dbf file."));
                                    vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                                    return FALSE;
                                }
                            }
                            // On Open J1939 database
                            else if(eProtocolName == PROTOCOL_J1939)
                            {
                                // If the loaded file is not J1939 database file
                                if(omstrDatabaseProtocol != DATABASE_PROTOCOL_J1939)
                                {
                                    strcpy(s_acTraceStr, strFileName + _(" is not created for J1939. Please load J1939 related dbf file."));
                                    vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                                    return FALSE;
                                }
                            }
                        }
                    }

                    // Reading Application version
                    nIndex = -1;
                    while( nIndex == -1 && o_File.ReadString( omstrApplicationVersion ))
                    {
                        // Getting the PROTOCOL name from the loaded dbf file
                        nIndex = omstrApplicationVersion.Find(APPLICATION_VERSION);
                    }

                    int nPlace = omstrApplicationVersion.Find('=');
                    if(nPlace == -1)
                    {
                        nPlace = omstrApplicationVersion.Find(']');
                    }

                    // Getting the PROTOCOL name
                    omstrApplicationVersion = omstrApplicationVersion.Right( omstrApplicationVersion.GetLength() - (nPlace+2));
                }
                /*Reading of database Protocol ends here*/
                //int nCount = 0;

                nIndex = -1;
                // Get number of messages
                while( nIndex == -1 && o_File.ReadString( sFirstLine ))
                {
                    nIndex = sFirstLine.Find(NUMBER_OF_MESSAGES);
                }

                if ( nIndex != -1 )
                {
                    int nPlace = sFirstLine.Find('=');
                    if(nPlace == -1)
                    {
                        nPlace = sFirstLine.Find(']');
                    }
                    CString omstrNoOfMsgs =
                        sFirstLine.Right( sFirstLine.GetLength() - (nPlace+2));

                    // Number of messages updated here
                    m_unMessageCount = atoi( omstrNoOfMsgs );

                    // create message signal structure using no of message
                    m_psMessages = new sMESSAGE[m_unMessageCount];

                    if ( m_psMessages != nullptr )
                    {
                        // Initialise all signal stuff to nullptr to
                        // make sure application will not crash
                        for ( UINT nCount = 0; nCount < m_unMessageCount; nCount++ )
                        {
                            m_psMessages[nCount].m_psSignals = nullptr;
                        }

                        UINT unMsgCount = 0;
                        // To check if the Message length exceeds 8
                        bIsMessageLengthExceeds = FALSE;

                        // start getting message information with in the
                        // message boundary. The message count is included to
                        // avoid crash if the file has more entries
                        while ( unMsgCount < m_unMessageCount
                                && o_File.ReadString( sFirstLine ))
                        {
                            nIndex = sFirstLine.Find( MSG_START_ID, 0);

                            if ( nIndex != -1 )
                            {
                                // initialise all the matrix elements
                                // to zero for this message
                                for ( UINT nCount = 0; nCount < 1785; nCount++ )
                                {
                                    m_psMessages[unMsgCount].m_bySignalMatrix[nCount] = 0x00;
                                }

                                CString sMsgDet = sFirstLine;

                                nIndex = sMsgDet.Find( SPACE );

                                // Find Msg Name
                                if (  nIndex != -1 )
                                {
                                    CString strTmp = sMsgDet.Right(sMsgDet.GetLength() - nIndex);

                                    strTmp.TrimLeft();

                                    nIndex = strTmp.Find( ',' );

                                    if ( nIndex != -1 )
                                    {
                                        // Msg Name
                                        m_psMessages[unMsgCount].m_omStrMessageName = strTmp.Left( nIndex );

                                        sMsgDet = strTmp.Right(strTmp.GetLength() - nIndex - 1);
                                    }
                                    else
                                    {
                                        if (!m_bAutoServerMode)
                                        {
                                            strcpy_s(s_acTraceStr, 1024, _("Database file corrupted"));
                                            vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                                        }
                                        bReturnValue = FALSE;
                                        break;
                                    }

                                    // Find Msg Code

                                    nIndex = sMsgDet.Find( ',' );

                                    if (  nIndex != -1 )
                                    {
                                        strTmp = sMsgDet.Left(nIndex);

                                        // Msg Code
                                        m_psMessages[unMsgCount].m_unMessageCode = strtoul(strTmp.GetBuffer(MAX_PATH), nullptr, 10);
                                        sMsgDet = sMsgDet.Right(sMsgDet.GetLength() - nIndex - 1);
                                    }
                                    else
                                    {
                                        if (!m_bAutoServerMode)
                                        {
                                            strcpy_s(s_acTraceStr, 1024, _("Database file corrupted"));
                                            vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                                        }
                                        bReturnValue = FALSE;
                                        break;
                                    }

                                    // Find Msg Length

                                    nIndex = sMsgDet.Find( ',' );

                                    if (  nIndex != - 1 )
                                    {
                                        strTmp = sMsgDet.Left(nIndex);

                                        // Msg length
                                        m_psMessages[unMsgCount].m_unMessageLength = atoi(strTmp);

                                        if(eProtocolName != PROTOCOL_J1939)
                                        {
                                            // If message length exceeds 8 stop opening the .dbf file
                                            if( m_psMessages[unMsgCount].m_unMessageLength > 8)
                                            {
                                                bIsMessageLengthExceeds = TRUE;

                                                bReturnValue = FALSE;
                                                break;
                                            }
                                        }
                                        sMsgDet = sMsgDet.Right(sMsgDet.GetLength() - nIndex - 1);
                                    }
                                    else
                                    {
                                        if (!m_bAutoServerMode)
                                        {
                                            strcpy_s(s_acTraceStr, 1024,  _("Database file corrupted"));
                                            vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                                        }
                                        bReturnValue = FALSE;
                                        break;
                                    }

                                    nIndex = sMsgDet.Find( ',' );

                                    if (  nIndex != - 1 )
                                    {
                                        strTmp = sMsgDet.Left(nIndex);

                                        // No of Signals
                                        m_psMessages[unMsgCount].m_unNumberOfSignals = (UINT)atoi((const char*) sMsgDet );

                                        sMsgDet = sMsgDet.Right(sMsgDet.GetLength() - nIndex - 1);
                                    }
                                    else
                                    {
                                        if (!m_bAutoServerMode)
                                        {
                                            strcpy_s(s_acTraceStr, 1024, _("Database file corrupted"));
                                            vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                                        }
                                        bReturnValue = FALSE;
                                        break;
                                    }

                                    // rajesh: 03-03-2003: modification begin: Read Data-Format & Frame-Format
                                    nIndex = sMsgDet.Find( ',' );

                                    if (  nIndex != - 1 )
                                    {
                                        strTmp = sMsgDet.Left(nIndex);

                                        // No of Signals
                                        m_psMessages[unMsgCount].m_nMsgDataFormat = 1;//(UINT)atoi((const char*) sMsgDet );

                                        sMsgDet = sMsgDet.Right(sMsgDet.GetLength() - nIndex - 1);
                                    }
                                    else
                                    {
                                        if (!m_bAutoServerMode)
                                        {
                                            strcpy_s(s_acTraceStr, 1024,  _("Database file corrupted"));
                                            vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                                        }
                                        bReturnValue = FALSE;
                                        break;
                                    }

                                    // check that extracted substring is not empty
                                    if(sMsgDet.IsEmpty())
                                    {
                                        if (!m_bAutoServerMode)
                                        {
                                            strcpy_s(s_acTraceStr, 1024, _("Database file corrupted"));
                                            vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                                        }
                                        bReturnValue = FALSE;
                                        break;
                                    }

                                    // if not empty, find frame-format
                                    if(sMsgDet[0] == 'S')
                                    {
                                        m_psMessages[unMsgCount].m_bMessageFrameFormat = 0; // STANDARD
                                    }
                                    else if(sMsgDet[0] == 'X')
                                    {
                                        m_psMessages[unMsgCount].m_bMessageFrameFormat = 1; // EXTENDED
                                    }
                                    else
                                    {
                                        if (!m_bAutoServerMode)
                                        {
                                            strcpy_s(s_acTraceStr, 1024, _("Database file corrupted"));
                                            vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                                        }
                                        bReturnValue = FALSE;
                                        break;
                                    }
                                    // rajesh: 03-03-2003: modification end

                                    if ( m_psMessages[unMsgCount].m_unNumberOfSignals > 0)
                                    {
                                        sSIGNALS* pNext =
                                            m_psMessages[unMsgCount].m_psSignals = nullptr;

                                        // start getting signal information
                                        UINT unNumberofSignalsRead = 0;

                                        BOOL bLineRead = FALSE;

                                        while (unNumberofSignalsRead < m_psMessages[unMsgCount].m_unNumberOfSignals)
                                        {
                                            if (!bLineRead)
                                            {
                                                o_File.ReadString( sFirstLine );
                                            }
                                            int nIndex = sFirstLine.Find( SG_START_ID, 0 );

                                            if ( nIndex != -1)
                                            {
                                                // Allocate memory for signal
                                                sSIGNALS* sTempSg = new sSIGNALS;

                                                if (sTempSg == nullptr)
                                                {
                                                    if (!m_bAutoServerMode)
                                                    {
                                                        strcpy_s(s_acTraceStr, 1024, T2A(_(MSG_MEMORY_CONSTRAINT)));
                                                        vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                                                    }

                                                    bReturnValue = FALSE;
                                                    break;
                                                }
                                                else
                                                {
                                                    sTempSg->m_psNextSignalList = nullptr;
                                                    CString sMsgDet = sFirstLine;
                                                    nIndex = sMsgDet.Find( SPACE );

                                                    // Find Signal Name
                                                    if (  nIndex != -1 )
                                                    {
                                                        CString strTmp = sMsgDet.Right(sMsgDet.GetLength() - nIndex);
                                                        strTmp.TrimLeft();
                                                        sMsgDet = strTmp;
                                                        nIndex = strTmp.Find( ',' );

                                                        // Find and get signal name
                                                        if ( nIndex != -1 )
                                                        {
                                                            CString sTmp = sMsgDet.Left(nIndex);
                                                            sTempSg->m_omStrSignalName = sTmp;
                                                            sMsgDet = sMsgDet.Right(sMsgDet.GetLength() - nIndex - 1);
                                                        }
                                                        else
                                                        {
                                                            if (!m_bAutoServerMode)
                                                            {
                                                                strcpy_s(s_acTraceStr, _("Database file corrupted"));
                                                                vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                                                            }

                                                            bReturnValue = FALSE;
                                                            break;
                                                        }

                                                        // Find and get SIGNAL LENGTH
                                                        nIndex = sMsgDet.Find( ',' );

                                                        if (  nIndex != -1 )
                                                        {
                                                            CString sTmp = sMsgDet.Left(nIndex);
                                                            sTempSg->m_unSignalLength = atoi( (const char*) sTmp );
                                                            sMsgDet = sMsgDet.Right(sMsgDet.GetLength() - nIndex - 1);
                                                        }
                                                        else
                                                        {
                                                            if (!m_bAutoServerMode)
                                                            {
                                                                strcpy_s(s_acTraceStr, 1024, _("Database file corrupted"));
                                                                vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                                                            }
                                                            bReturnValue = FALSE;
                                                            break;
                                                        }

                                                        // Find and get byte info from which signal has started
                                                        nIndex = sMsgDet.Find( ',' );

                                                        if (  nIndex != -1 )
                                                        {
                                                            CString sTmp = sMsgDet.Left(nIndex);
                                                            sTempSg->m_unStartByte =  atoi( (const char*) sTmp );
                                                            sMsgDet = sMsgDet.Right(sMsgDet.GetLength() - nIndex - 1);
                                                        }
                                                        else
                                                        {
                                                            if (!m_bAutoServerMode)
                                                            {
                                                                strcpy_s(s_acTraceStr, 1024, _("Database file corrupted"));
                                                                vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                                                            }

                                                            bReturnValue = FALSE;
                                                            break;
                                                        }

                                                        // Find and get signal start bit
                                                        nIndex = sMsgDet.Find( ',' );

                                                        if (  nIndex != -1 )
                                                        {
                                                            CString sTmp = sMsgDet.Left(nIndex);

                                                            sTempSg->m_byStartBit = (BYTE)atoi( (const char*) sTmp );

                                                            sMsgDet = sMsgDet.Right(sMsgDet.GetLength() - nIndex - 1);
                                                        }
                                                        else
                                                        {
                                                            if (!m_bAutoServerMode)
                                                            {
                                                                strcpy_s(s_acTraceStr, 1024, _("Database file corrupted"));
                                                                vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                                                            }
                                                            bReturnValue = FALSE;
                                                            break;
                                                        }

                                                        // Find and get signal type
                                                        nIndex = sMsgDet.Find( ',' );

                                                        if (  nIndex != -1 )
                                                        {
                                                            CString sTmp = sMsgDet.Left(nIndex);

                                                            const char* ucSgTyp  = (const char*)sTmp;

                                                            sTempSg->m_bySignalType = ucSgTyp[0];

                                                            sMsgDet = sMsgDet.Right(sMsgDet.GetLength() - nIndex - 1);
                                                        }
                                                        else
                                                        {
                                                            if (!m_bAutoServerMode)
                                                            {
                                                                strcpy_s(s_acTraceStr, 1024, _("Database file corrupted"));
                                                                vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                                                            }
                                                            bReturnValue = FALSE;
                                                            break;
                                                        }
                                                        // Find and get signal maximum value

                                                        nIndex = sMsgDet.Find( ',' );

                                                        if (  nIndex != -1 )
                                                        {
                                                            CString sTmp = sMsgDet.Left(nIndex);

                                                            if(sTempSg->m_bySignalType == CHAR_INT)
                                                            {
                                                                sTempSg->m_SignalMaxValue.n64Value =_atoi64(sTmp);
                                                            }
                                                            else
                                                            {
                                                                sTempSg->m_SignalMaxValue.un64Value = _atoi64(sTmp);
                                                            }

                                                            sMsgDet = sMsgDet.Right(sMsgDet.GetLength() - nIndex - 1);
                                                        }
                                                        else
                                                        {
                                                            if (!m_bAutoServerMode)
                                                            {
                                                                strcpy_s(s_acTraceStr, 1024, _("Database file corrupted"));
                                                                vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                                                            }
                                                            bReturnValue = FALSE;
                                                            break;
                                                        }

                                                        // Find and get signal minimum value

                                                        nIndex = sMsgDet.Find( ',' );

                                                        if (  nIndex != -1 )
                                                        {
                                                            CString sTmp = sMsgDet.Left(nIndex);

                                                            if(sTempSg->m_bySignalType == CHAR_INT)
                                                            {
                                                                sTempSg->m_SignalMinValue.n64Value = _atoi64(sTmp);
                                                            }
                                                            else
                                                            {
                                                                sTempSg->m_SignalMinValue.un64Value = _atoi64(sTmp);
                                                            }

                                                            sMsgDet = sMsgDet.Right(sMsgDet.GetLength() - nIndex - 1);
                                                        }
                                                        else
                                                        {
                                                            if (!m_bAutoServerMode)
                                                            {
                                                                strcpy_s(s_acTraceStr, 1024, _("Database file corrupted"));
                                                                vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                                                            }
                                                            bReturnValue = FALSE;
                                                            break;
                                                        }

                                                        // Find and get signal data format

                                                        nIndex = sMsgDet.Find( ',' );

                                                        if (  nIndex != -1 )
                                                        {
                                                            CString sTmp = sMsgDet.Left(nIndex);

                                                            sTempSg->m_eFormat = DATA_FORMAT_MOTOROLA;
                                                            if (atoi(sTmp) == 1) // Forced to use hardcoded value owing to
                                                            {
                                                                // the already existing nonsense code.
                                                                sTempSg->m_eFormat = DATA_FORMAT_INTEL;
                                                            }
                                                            if (fDBVerNum < fCurrDBVer)
                                                            {
                                                                sTempSg->m_eFormat = DATA_FORMAT_INTEL;
                                                            }
                                                            sMsgDet = sMsgDet.Right(sMsgDet.GetLength() - nIndex - 1);
                                                        }
                                                        else
                                                        {
                                                            if (!m_bAutoServerMode)
                                                            {
                                                                strcpy_s(s_acTraceStr, 1024, _("Database file corrupted"));
                                                                vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                                                            }
                                                            bReturnValue = FALSE;
                                                            break;
                                                        }

                                                        // Find and get signal offset value

                                                        nIndex = sMsgDet.Find( ',' );

                                                        if (  nIndex != -1 )
                                                        {
                                                            CString sTmp = sMsgDet.Left(nIndex);
                                                            charArray = T2A(sTmp.GetBuffer(MAX_PATH));
                                                            vReplaceChar(charArray,'.', szSep);

                                                            sTempSg->m_fSignalOffset = (float)atof(sTmp);

                                                            sMsgDet = sMsgDet.Right(sMsgDet.GetLength() - nIndex - 1);
                                                        }
                                                        else
                                                        {
                                                            if (!m_bAutoServerMode)
                                                            {
                                                                strcpy_s(s_acTraceStr, 1024, _("Database file corrupted"));
                                                                vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                                                            }
                                                            bReturnValue = FALSE;
                                                            break;
                                                        }

                                                        // Find and get signal factor value

                                                        nIndex = sMsgDet.Find( ',' );

                                                        if (  nIndex != -1 )
                                                        {
                                                            CString sTmp = sMsgDet.Left(nIndex);
                                                            charArray = T2A(sTmp.GetBuffer(MAX_PATH));
                                                            vReplaceChar(charArray,'.', szSep);

                                                            sTempSg->m_fSignalFactor = (float)atof(sTmp);

                                                            sMsgDet = sMsgDet.Right(sMsgDet.GetLength() - nIndex - 1);
                                                        }
                                                        else
                                                        {
                                                            if (!m_bAutoServerMode)
                                                            {
                                                                strcpy_s(s_acTraceStr, 1024, _("Database file corrupted"));
                                                                vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                                                            }
                                                            bReturnValue = FALSE;
                                                            break;
                                                        }

                                                        // Find and get signal unit
                                                        //in 1.1 version ","will be added to end signal unit field
                                                        //in 1.0 it is ended with space

                                                        nIndex = sMsgDet.Find( ',' );

                                                        if (  nIndex != -1 )
                                                        {
                                                            CString sTmp = sMsgDet.Left(nIndex);

                                                            sTempSg->m_omStrSignalUnit = sTmp;

                                                            sMsgDet = sMsgDet.Right(sMsgDet.GetLength() - nIndex -1 );
                                                        }
                                                        /*else
                                                        {
                                                            MessageBox( nullptr,"Database file corrupted", nullptr, MB_ICONINFORMATION|MB_OK );
                                                            bReturnValue = FALSE;
                                                            break;
                                                        }*/

                                                        // sTempSg->m_omStrSignalUnit = sMsgDet;


                                                        // Update the signal matrix
                                                        BYTE abySigMask[DATA_LENGTH_MAX] = {0};
                                                        bCalcBitMaskForSig(abySigMask,
                                                                           DATA_LENGTH_MAX,
                                                                           sTempSg->m_unStartByte,
                                                                           sTempSg->m_byStartBit,
                                                                           sTempSg->m_unSignalLength,
                                                                           sTempSg->m_eFormat);
                                                        vUpdateSignalMatrix(abySigMask,
                                                                            m_psMessages[unMsgCount].m_bySignalMatrix,
                                                                            DATA_LENGTH_MAX,
                                                                            SET);


                                                        CSignalDescVal* pomSgDescValNext = sTempSg->m_oSignalIDVal = nullptr;

                                                        // Get signal value descriptor and value
                                                        while ( o_File.ReadString( sFirstLine ))
                                                        {
                                                            int nFoundIndex = sFirstLine.Find( SG_DESC_START_ID ,0 );

                                                            if ( nFoundIndex != -1)
                                                            {
                                                                nFoundIndex = sFirstLine.Find( " " );
                                                            }
                                                            if (nFoundIndex != -1 )
                                                            {
                                                                CSignalDescVal* pomSgDescVal =
                                                                    new CSignalDescVal;

                                                                // Find Msg Name
                                                                CString strTmp = sFirstLine.Right(sFirstLine.GetLength() - (nFoundIndex+1));

                                                                strTmp.TrimLeft();

                                                                sFirstLine = strTmp;

                                                                nFoundIndex = sFirstLine.Find( ',' );

                                                                // Find and get signal type
                                                                if (  nFoundIndex != -1 )
                                                                {
                                                                    CString sTmp = sFirstLine.Left(nFoundIndex);

                                                                    // Get ID
                                                                    pomSgDescVal->m_omStrSignalDescriptor = sTmp;

                                                                    sFirstLine = sFirstLine.Right(sFirstLine.GetLength() - nFoundIndex - 1);

                                                                    //                                                                        pomSgDescVal->m_n64SignalVal = _atoi64( (const char*) sFirstLine );
                                                                    pomSgDescVal->m_DescValue.n64Value = _atoi64( (const char*) sFirstLine );

                                                                    pomSgDescVal->m_pouNextSignalSignalDescVal = nullptr;

                                                                }// end if ( finding for comma)
                                                                else
                                                                {
                                                                    if (!m_bAutoServerMode)
                                                                    {
                                                                        strcpy_s(s_acTraceStr, 1024, _("Database file corrupted"));
                                                                        vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                                                                    }
                                                                    bReturnValue = FALSE;
                                                                    break;
                                                                }
                                                                // Attach the signal descand value to the data structure.

                                                                if ( pomSgDescValNext != nullptr)
                                                                {
                                                                    while ( pomSgDescValNext->m_pouNextSignalSignalDescVal != nullptr )
                                                                    {
                                                                        pomSgDescValNext = pomSgDescValNext->m_pouNextSignalSignalDescVal;
                                                                    }
                                                                    pomSgDescValNext->m_pouNextSignalSignalDescVal = pomSgDescVal;
                                                                }
                                                                else
                                                                {
                                                                    pomSgDescValNext = sTempSg->m_oSignalIDVal = pomSgDescVal;

                                                                    pomSgDescValNext->m_pouNextSignalSignalDescVal = nullptr;
                                                                }

                                                            }
                                                            else
                                                            {
                                                                bLineRead = TRUE;

                                                                break;
                                                            }
                                                        }// while
                                                    }//

                                                    // allocate memory for next signal
                                                    if ( pNext != nullptr )
                                                    {
                                                        while( pNext->m_psNextSignalList != nullptr )
                                                        {
                                                            pNext = pNext->m_psNextSignalList;
                                                        }
                                                        pNext->m_psNextSignalList = sTempSg;

                                                    }
                                                    else
                                                    {
                                                        pNext = m_psMessages[unMsgCount].m_psSignals = sTempSg;

                                                        pNext->m_psNextSignalList = nullptr;
                                                    }

                                                    unNumberofSignalsRead++;

                                                }// if (!=nullptr)

                                            }// if signal found
                                        }

                                        if (bReturnValue == FALSE)
                                        {
                                            break;
                                        }
                                    }
                                    else // No signals defined.
                                    {
                                        m_psMessages[unMsgCount].m_psSignals = nullptr;
                                    }

                                    // next message index
                                    unMsgCount++;
                                }

                            }// message not found

                        }// while read message
                        if(bIsMessageLengthExceeds == FALSE)
                        {
                            //Create one DB structure and add it to the list
                            sDBFileStruct* psDBStruct = new sDBFileStruct;
                            if(psDBStruct != nullptr)
                            {
                                psDBStruct->m_omStrDatabasePath = strFileName;
                                psDBStruct->m_unMessageCount = m_unMessageCount;
                                psDBStruct->m_psMessages = m_psMessages;
                                //Add the structure at the begining of list so that while
                                //making the CMap,the message id of first added DB should be present
                                psDBStruct->m_psNextDBFileStruct = m_psDatbaseStructList;
                                m_psDatbaseStructList = psDBStruct;
                            }
                            else
                            {
                                if (!m_bAutoServerMode)
                                {
                                    strcpy_s(s_acTraceStr, 1024, T2A(_(MSG_MEMORY_CONSTRAINT)));
                                    vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                                }
                                bReturnValue = FALSE;
                            }
                        }
                    }
                    else
                    {
                        if (!m_bAutoServerMode)
                        {
                            strcpy_s(s_acTraceStr, 1024, T2A(_(MSG_MEMORY_CONSTRAINT)));
                            vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                        }
                        bReturnValue = FALSE;
                    }

                }
                if (bIsFileOpen == TRUE)
                {
                    o_File.Close();
                }
            }
            else
            {
                CString omStrErrorMsg = "";
                omStrErrorMsg.Format(_("Error in opening %s database file"),strFileName);
                if (!m_bAutoServerMode)
                {
                    strcpy_s(s_acTraceStr, 1024, T2A(omStrErrorMsg.GetBuffer(MAX_PATH)));
                    vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                }
            }
        }
        CATCH_ALL (pomE)
        {
            if(pomE != nullptr )
            {
                LPTSTR lpszError = "";
                // Get error
                pomE->GetErrorMessage( lpszError, defSIZE_OF_ERROR_BUFFER);
                if (!m_bAutoServerMode)
                {
                    strcpy_s(s_acTraceStr, 1024, T2A(lpszError));
                    vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                }
                pomE->Delete();
            }

            if (bIsFileOpen == TRUE)
            {
                o_File.Close();
            }

        }
        END_CATCH_ALL

    }

    if ( bReturnValue == FALSE )
    {
        // Delete occupied memory
        bDeAllocateMemory(strFileName);
        if(bIsMessageLengthExceeds == FALSE)
        {
            strFileName += _(" Is Invalid DBF File");
        }
        else
        {
            strFileName += _(" is not created for CAN. Since one of the message length exceeds 8 bytes.");
        }
        strcpy_s(s_acTraceStr, 1024, strFileName);
        vWriteTextToTrace();
    }
    else
    {
        vResetMsgMapContent();
        m_bIsDatabaseSaved = TRUE;
    }

    return (bReturnValue);
}

/**
* \brief         Replaces first occurance of a character in a char array with a new character
* \param[out]    Char array to be used
* \param[in]     character to be replaced
* \param[in]     new character used for replacement
* \return        S_OK for success, S_FALSE for failure
* \authors       Arunkumar Karri
* \date          03.01.2013 Created
*/
void vReplaceChar(char str[], char chOld, char chNew)
{
    char* ptrOccured = nullptr;
    ptrOccured = strchr(str, chOld);
    if ( ptrOccured != 0)
    {
        *ptrOccured = chNew;
    }
}

/******************************************************************************
  Function Name    :  bWriteIntoDatabaseFileFromDataStructure
  Input(s)         :  CString strFileName
  Output           :  BOOL
  Functionality    :  Dumps all the data structure information into the DB
                      file specified.
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  15.02.2002
  Modifications    :  03.03.2003 Rajesh Kumar
                      Added - Frame-Format field as per new DB format
                      04.03.2003 Rajesh Kumar
                      Modified - MIN-MAX values w.r.t change in sSIGNALS struct
******************************************************************************/
BOOL CMsgSignal::bWriteIntoDatabaseFileFromDataStructure( CString strFileName, eProtocol eProtocolName)
{
    BOOL bRetVal = FALSE;

    BOOL bIsFileOpen = FALSE;

    // For File I/O
    CStdioFile o_File;

    CString sFirstLine = "";

    TRY
    {
        // Open File
        bIsFileOpen =
        o_File.Open( strFileName, CFile::modeCreate|CFile::modeWrite|CFile::typeText );
        if(bIsFileOpen != FALSE )
        {
            // Write Header information
            o_File.WriteString( DATABASE_HEADER_NEW );

            // writeVersion info
            CString strBuffer = NEW_LINE;
            strBuffer += NEW_LINE;
            strBuffer += DATABASE_VERSION;
            strBuffer += SPACE;
            strBuffer += DATABASE_VERSION_NO;
            strBuffer += NEW_LINE;
            strBuffer += NEW_LINE;

            // Adding database Protocol in the version 1.3
            strBuffer += DATABASE_PROTOCOL;
            strBuffer += SPACE;

            if(eProtocolName == PROTOCOL_CAN)
            {
                strBuffer += DATABASE_PROTOCOL_CAN;
            }
            else if(eProtocolName == PROTOCOL_J1939)
            {
                strBuffer += DATABASE_PROTOCOL_J1939;
            }

            strBuffer += NEW_LINE;
            strBuffer += NEW_LINE;

            // Start Adding Application version in the version 1.3

            // Adding Application version information in the version 1.3
            CString omstrAppVersion = APPLICATION_VERSION_NO;

            CString omstrVersion;

            // Application version
            omstrVersion.Format("%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);

            omstrAppVersion.Replace(APPLICATION_VERSION_PLACE_HOLDER, omstrVersion);

            // Adding Application Version
            strBuffer += APPLICATION_VERSION;
            strBuffer += SPACE;
            strBuffer += omstrAppVersion;

            strBuffer += NEW_LINE;
            strBuffer += NEW_LINE;
            // End of Adding Application version in the version 1.3
            o_File.WriteString( strBuffer );


            // write Number of messages info
            strBuffer = "[";
            strBuffer += NUMBER_OF_MESSAGES;
            strBuffer += "]";
            strBuffer += " ";

            if ( m_psMessages != nullptr )
            {
                char buff[256];

                buff[255] = '\0';
                sprintf_s( buff, 255, "%d", m_unMessageCount );
                strBuffer += buff;
            }

            o_File.WriteString( strBuffer );

            for ( UINT nMsgIndex = 0; nMsgIndex < m_unMessageCount; nMsgIndex++ )
            {
                CString strTempBuf = NEW_LINE;

                strTempBuf += NEW_LINE;

                strTempBuf += MSG_START_ID;

                // start Message Details
                o_File.WriteString( strTempBuf );

                // Mesg Name
                CString sMsgDetails =
                    m_psMessages[nMsgIndex].m_omStrMessageName;

                sMsgDetails += ",";

                // Msg code
                char buff[10];

                buff[9] = '\0';
                sprintf_s( buff, "%d", m_psMessages[nMsgIndex].m_unMessageCode);
                sMsgDetails += buff;

                sMsgDetails += ",";

                // Msg Length
                buff[1] = '\0';
                sprintf_s( buff, "%d", m_psMessages[nMsgIndex].m_unMessageLength);
                sMsgDetails += buff;

                sMsgDetails += ",";

                // Number of Signals
                buff[2] = '\0';
                sprintf_s( buff, "%d", m_psMessages[nMsgIndex].m_unNumberOfSignals);
                sMsgDetails += buff;

                sMsgDetails += ",";

                // Message data format
                buff[2] = '\0';
                sprintf_s( buff, "%d", m_psMessages[nMsgIndex].m_nMsgDataFormat);
                sMsgDetails += buff;

                // rajesh: 03-03-2003: BEGIN: added FrameFormat
                sMsgDetails += (m_psMessages[nMsgIndex].m_bMessageFrameFormat == 0) ? ",S" : ",X";
                // rajesh: 03-03-2003: END
                //
                o_File.WriteString( sMsgDetails );

                if ( m_psMessages[nMsgIndex].m_psSignals != nullptr )
                {
                    sSIGNALS* pTempSg =
                        m_psMessages[nMsgIndex].m_psSignals;

                    while( pTempSg != nullptr &&
                            m_psMessages[nMsgIndex].m_unNumberOfSignals > 0)
                    {
                        strTempBuf = "";

                        strTempBuf = NEW_LINE;

                        strTempBuf += SG_START_ID;

                        // start Signal Details
                        o_File.WriteString( strTempBuf );

                        // Signal Name
                        CString sSgDetails = pTempSg->m_omStrSignalName;

                        sSgDetails += ",";

                        // Signal Length
                        char temp[25];

                        temp[9] = '\0';
                        sprintf_s( temp, "%d", pTempSg->m_unSignalLength );
                        sSgDetails += temp;

                        sSgDetails += ",";

                        // Signal at which byte
                        sprintf_s( buff, "%d", pTempSg->m_unStartByte );
                        sSgDetails += buff;

                        sSgDetails += ",";
                        sprintf_s( buff, "%d", pTempSg->m_byStartBit );
                        sSgDetails += buff;

                        sSgDetails += ",";

                        // Signal type
                        buff[0] = pTempSg->m_bySignalType;

                        sSgDetails += buff;

                        sSgDetails += ",";

                        // Signal max val
                        temp[19] = '\0';

                        if(pTempSg->m_bySignalType == CHAR_INT)
                        {
                            sprintf_s( temp, "%I64d", pTempSg->m_SignalMaxValue.n64Value);
                        }
                        else
                        {
                            sprintf_s( temp, "%I64d", pTempSg->m_SignalMaxValue.un64Value);
                        }

                        sSgDetails += temp;

                        sSgDetails += ",";

                        // Signal min val
                        if(pTempSg->m_bySignalType == CHAR_INT)
                        {
                            sprintf_s( temp, "%I64d", pTempSg->m_SignalMinValue.n64Value);
                        }
                        else
                        {
                            sprintf_s( temp, "%I64d", pTempSg->m_SignalMinValue.un64Value);
                        }
                        sSgDetails += temp;

                        sSgDetails += ",";

                        // Signal data format
                        sprintf_s( temp, "%d", pTempSg->m_eFormat);
                        sSgDetails += temp;

                        sSgDetails += ",";

                        // Signal offset value
                        sprintf_s( temp, "%f", pTempSg->m_fSignalOffset );

                        char szSep;
                        GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, &szSep, 1);

                        /* Make sure we use default dot(.) as decimal symbol for floating values */
                        vReplaceChar(temp, szSep,'.');

                        sSgDetails += temp;

                        sSgDetails += ",";

                        // Signal factor value
                        sprintf_s( temp, "%f", pTempSg->m_fSignalFactor );
                        /* Make sure we use default dot(.) as decimal symbol for floating values */
                        vReplaceChar(temp, szSep,'.');

                        sSgDetails += temp;

                        sSgDetails += ",";

                        // Signal unit
                        sSgDetails += pTempSg->m_omStrSignalUnit;
                        //added for validation
                        sSgDetails += ",";

                        // Dump signal details into the file
                        o_File.WriteString( sSgDetails );

                        CSignalDescVal* pTempIDVal = pTempSg->m_oSignalIDVal;

                        while ( pTempIDVal != nullptr )
                        {
                            strTempBuf = "";

                            strTempBuf = NEW_LINE;

                            strTempBuf += SG_DESC_START_ID;

                            // Start signal type info
                            o_File.WriteString( strTempBuf );

                            CString sSgTypeInfo = pTempIDVal->m_omStrSignalDescriptor;

                            sSgTypeInfo += ",";

                            char type[24];

                            type[19] = '\0';
                            //                            sprintf_s( type, "%I64d", pTempIDVal->m_n64SignalVal );
                            sprintf_s( type, "%I64d", pTempIDVal->m_DescValue.n64Value );
                            sSgTypeInfo += type;

                            o_File.WriteString( sSgTypeInfo );

                            pTempIDVal = pTempIDVal->m_pouNextSignalSignalDescVal;

                        }// while ( pTempIDVal != nullptr )

                        pTempSg = pTempSg->m_psNextSignalList;

                    }// while ( pTempSg != nullptr )

                }// if ( o_pgMsgArray[nMsgIndex].m_sMessage->m_psSignal != nullptr )

                strTempBuf = "";

                strTempBuf = NEW_LINE;

                strTempBuf += MSG_END_ID;

                o_File.WriteString( strTempBuf );

            }// for ( UINT nMsgIndex = 0

            bRetVal = TRUE;

            if ( bIsFileOpen == TRUE)
            {
                o_File.Close();
            }
        }
        else
        {
            CString omStrErrorMsg = "";
            omStrErrorMsg.Format(_("Error in opening %s database file"),strFileName);
            if (!m_bAutoServerMode)
            {
                strcpy_s(s_acTraceStr, 1024, T2A(omStrErrorMsg.GetBuffer(MAX_PATH)));
                vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
            }

        }

    }// TRY
    CATCH_ALL (pomE)
    {
        if ( bIsFileOpen == TRUE)
        {
            o_File.Close();
        }

        // Get error
        if(pomE != nullptr )
        {
            LPTSTR lpszError = "";
            pomE->GetErrorMessage( lpszError, defSIZE_OF_ERROR_BUFFER);
            if (!m_bAutoServerMode)
            {
                strcpy_s(s_acTraceStr, 1024, T2A(lpszError));
                vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
            }
            pomE->Delete();
        }

        bRetVal = FALSE;
    }
    END_CATCH_ALL

    if ( bRetVal == TRUE )
    {
        if (m_sDbParams.m_omDBPath == strFileName )
        {
            m_bIsDatabaseSaved = TRUE;
        }
#if 0
        CMainFrame* pMainFrm = (CMainFrame*)AfxGetApp()->m_pMainWnd;
        if(pMainFrm != nullptr )
        {
            // If it is active database and tx message dialog is open
            // update the message details list.
            if(m_bIsDatabaseActive == TRUE )
            {
                pMainFrm->vPostMsgToSendMsgDlg(m_sDbParams.m_eBus) ;
            }
        }
#endif
    }

    return bRetVal;
}
/******************************************************************************
  Function Name    :  unGetNumerOfMessages
  Input(s)         :
  Output           :  UINT
  Functionality    :  Returns number of messages in the DB.
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  15.02.2002
  Modifications    :
******************************************************************************/
UINT CMsgSignal::unGetNumerOfMessages()
{
    return (m_omMsgDetailsIDMap.GetCount());
}
/******************************************************************************
  Function Name    :  bAddMsg
  Input(s)         :
  Output           :  bool
  Functionality    :  Adds new message to the datastructure
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  15.02.2002
  Modifications    :
******************************************************************************/
BOOL CMsgSignal::bAddMsg()
{
    BOOL bReturnValue = TRUE;

    // Allocate memory for new message
    bReturnValue = bAllocateMemory( MD_ADD );

    if ( bReturnValue == TRUE)
    {
        m_bIsDatabaseSaved = FALSE;
    }

    return bReturnValue;
}
/******************************************************************************
  Function Name    :  bUpdateMsg
  Input(s)         :
  Output           :  eMSG_CHANGES_IN eNum,
                      CString omStrPreviousMsgName,
                      sMESSAGE *pMsgTobeUpdated
  Functionality    :  Updates message details to the datastructure
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  15.02.2002
  Modifications    :
******************************************************************************/
BOOL CMsgSignal::bUpdateMsg( eMSG_CHANGES_IN eNum,
                             CString omStrPreviousMsgName,
                             sMESSAGE* pMsgTobeUpdated )
{
    BOOL bReturnValue = TRUE;

    // Get message pointer corresponding
    // to "omStrPreviousMsgName" message name
    sMESSAGE* pCurrMsg =
        psGetMessagePointerInactive(omStrPreviousMsgName);

    if ( pCurrMsg != nullptr )
    {
        if ( eNum == MSG_NAME )
        {
            // Update message name
            pCurrMsg->m_omStrMessageName = pMsgTobeUpdated->m_omStrMessageName;
        }
        else if ( eNum == MSG_CODE )
        {
            // Update message code
            pCurrMsg->m_unMessageCode = pMsgTobeUpdated->m_unMessageCode;
        }
        else if ( eNum == MSG_LENGTH )
        {
            // Update message length
            pCurrMsg->m_unMessageLength = pMsgTobeUpdated->m_unMessageLength;
        }
        else if ( eNum == MSG_FRAME_FORMAT )
        {
            // Update message frame format
            pCurrMsg->m_bMessageFrameFormat = pMsgTobeUpdated->m_bMessageFrameFormat;
        }
        else if ( eNum == MSG_DATA_FORMAT )
        {
            // Update message data format
            pCurrMsg->m_nMsgDataFormat = pMsgTobeUpdated->m_nMsgDataFormat;
        }
    }
    else
    {
        bReturnValue = FALSE;
    }

    if ( bReturnValue == TRUE)
    {
        m_bIsDatabaseSaved = FALSE;
    }

    return bReturnValue;
}
/******************************************************************************
  Function Name    :  bDeleteMsg
  Input(s)         :
  Output           :  CString omStrMsgToBeDeleted
  Functionality    :  Deletes message from the datastructure
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  15.02.2002
  Modifications    :
******************************************************************************/
BOOL CMsgSignal::bDeleteMsg( CString omStrMsgToBeDeleted )
{
    BOOL bReturnValue = TRUE;

    if ( omStrMsgToBeDeleted.IsEmpty() )
    {
        bReturnValue = FALSE;
    }
    else
    {
        // Get the message index corresponding to the
        // message to be deleted
        int nMsgIndex = nGetMessageIndexFromMsgName( omStrMsgToBeDeleted );

        if ( nMsgIndex != -1 )
        {
            // overwrite the messages
            for ( UINT nIndex = (UINT)nMsgIndex; nIndex < m_unMessageCount - 1; nIndex++ )
            {
                m_psMessages[nIndex] = m_psMessages[nIndex + 1];
            }

            // decrement count
            m_unMessageCount--;

            // Delete memory associated with the message to be deleted
            bReturnValue = bAllocateMemory( MD_DELETE );
        }
        else
        {
            if (!m_bAutoServerMode)
            {
                strcpy_s(s_acTraceStr, 1024, T2A(_(MSG_MSG_NOT_FOUND)));
                vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
            }
            bReturnValue = FALSE;
        }
    }

    if ( bReturnValue == TRUE)
    {
        m_bIsDatabaseSaved = FALSE;
    }

    return bReturnValue;
}
/******************************************************************************
  Function Name    :  bIsDuplicateMessageName
  Input(s)         :  CString strMsgName
  Output           :  BOOL
  Functionality    :  Checks for duplicate mesage name
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  15.02.2002
  Modifications    :
******************************************************************************/
BOOL CMsgSignal::bIsDuplicateMessageName( int nMsgId, CString strMsgName)
{
    BOOL bReturnValue = FALSE;

    if ( nMsgId == -1 )
    {
        bReturnValue = FALSE;
    }
    else
    {
        bReturnValue = TRUE;
    }

    if (!strMsgName.IsEmpty() &&
            bReturnValue != FALSE)
    {
        for ( UINT unMsgCount = 0; unMsgCount < m_unMessageCount; unMsgCount++ )
        {
            // Avoid self check
            if ( m_psMessages[unMsgCount].m_unMessageCode != (UINT)nMsgId )
            {
                // Case sensitive comparision
                if ( m_psMessages[unMsgCount].
                        m_omStrMessageName.Compare(strMsgName ) == 0)
                {
                    bReturnValue = TRUE;
                }
                else
                {
                    bReturnValue = FALSE;
                }

                if (bReturnValue)
                {
                    break;
                }
            }
            else
            {
                bReturnValue = FALSE;
            }
        }
    }
    return (bReturnValue);
}
/******************************************************************************
  Function Name    :  bIsDuplicateSignalName
  Input(s)         :  CString strMsgName, CString strSgName
  Output           :  BOOL
  Functionality    :  Checks for duplicate signal name
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  15.02.2002
  Modifications    :
******************************************************************************/
BOOL CMsgSignal::bIsDuplicateSignalName( CString strMsgName, CString strSgName)
{
    BOOL bReturnValue = FALSE;

    if (!(strMsgName.IsEmpty() && strSgName.IsEmpty()))
    {
        for ( UINT unMsgCount = 0; unMsgCount < m_unMessageCount; unMsgCount++ )
        {
            if ( m_psMessages[unMsgCount].m_omStrMessageName == strMsgName )
            {
                if (m_psMessages[unMsgCount].m_unNumberOfSignals > 0 )
                {
                    sSIGNALS* p_Sg = m_psMessages[unMsgCount].m_psSignals;

                    while ( p_Sg != nullptr )
                    {
                        if ( p_Sg->m_omStrSignalName != strSgName )
                        {
                            p_Sg = p_Sg->m_psNextSignalList;
                        }
                        else
                        {
                            bReturnValue = TRUE;
                            break;
                        }
                    }// end while
                }
                else // No signals defined
                {
                    break;
                }

                // Duplicate found, skip rest of the message
                if ( bReturnValue == TRUE )
                {
                    break;
                }
            }// end if
        }// end for

    }// end if
    else
    {
        if (!m_bAutoServerMode)
        {
            strcpy_s(s_acTraceStr, 1024, _("Insufficient Data"));
            vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
        }
    }
    return (bReturnValue);
}
/******************************************************************************
  Function Name    :  bIsDuplicateMessageCode
  Input(s)         :  CString omStrMsgName, UINT unMsgCode
  Output           :  BOOL
  Functionality    :  Checks for duplicate message code
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  15.02.2002
  Modifications    :
******************************************************************************/
BOOL CMsgSignal::bIsDuplicateMessageCode( CString omStrMsgName, UINT unMsgCode)
{
    BOOL bReturnValue = FALSE;

    if  (unMsgCode >= 0)
    {
        for ( UINT unMsgCount = 0; unMsgCount < m_unMessageCount; unMsgCount++ )
        {
            // avoid self checking
            if ( m_psMessages[unMsgCount].
                    m_omStrMessageName.Compare( omStrMsgName) != 0 )
            {
                if ( m_psMessages[unMsgCount].m_unMessageCode == unMsgCode )
                {
                    bReturnValue = TRUE;
                    unMsgCount = m_unMessageCount;
                }
            }
        }
    }
    else
    {
        if (!m_bAutoServerMode)
        {
            strcpy_s(s_acTraceStr, 1024, _("Invalid message code"));
            vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
        }
    }
    return (bReturnValue);
}




/******************************************************************************
  Function Name    :  bIsDuplicateSignalStartBitValue
  Input(s)         :  CString omStrMsgName,
                      BYTE byMsgByte,
                      UINT unSignalLength,
                      UINT unStartBitValue
  Output           :  BOOL
  Functionality    :  Checks for duplicate strat bit value
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  15.02.2002
  Modifications    :  Raja N 0n 19.07.2005, Modified the logic of finding the
                      overlap
******************************************************************************/
BOOL CMsgSignal::bIsDuplicateSignalStartBitValue(CString omStrMsgName,
        UINT unByteNum,
        UINT unSignalLength,
        UINT unStartBitValue,
        BOOL bDataFormat)
{
    BOOL bReturnValue = FALSE;
    // Get message pointer corresponding the message "omStrMsgName"
    sMESSAGE* pMsg = psGetMessagePointerInactive(omStrMsgName);
    //Get the signal matrix, at the given byte
    if ( pMsg != nullptr )
    {
        BYTE abyMaskByte[DATA_LENGTH_MAX] = {0x0};
        bCalcBitMaskForSig(abyMaskByte, DATA_LENGTH_MAX, unByteNum, unStartBitValue, unSignalLength, (EFORMAT_DATA) bDataFormat);
        for (UINT i = 0; i < DATA_LENGTH_MAX; i++)
        {
            if ((abyMaskByte[i] & pMsg->m_bySignalMatrix[i]) != 0)
            {
                bReturnValue = TRUE;
                i = DATA_LENGTH_MAX; //break the loop
            }
        }
    }

    return bReturnValue;
}
/******************************************************************************
  Function Name    :  bAddSignalToMsg
  Input(s)         :  CString omStrMsgName
  Output           :  BOOL
  Functionality    :  Add new signal to the message specified
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  15.02.2002
  Modifications    :
******************************************************************************/
BOOL CMsgSignal::bAddSignalToMsg( CString omStrMsgName )
{
    BOOL bReturnValue = TRUE;

    if ( !omStrMsgName.IsEmpty() )
    {
        // Get message pointer corresponding the message "omStrMsgName"
        sMESSAGE* pMsg = psGetMessagePointerInactive(omStrMsgName);

        if ( pMsg != nullptr )
        {
            // Get signal pointer
            sSIGNALS* pSg = pMsg->m_psSignals;

            if ( pSg != nullptr )
            {
                // Get last signal pointer
                while ( pSg != nullptr )
                {
                    sSIGNALS* pSgTemp = pSg->m_psNextSignalList;

                    if ( pSg->m_psNextSignalList != nullptr )
                    {
                        pSg = pSgTemp;
                    }
                    else
                    {
                        // create new signal and append the data
                        sSIGNALS* pNewSignal = new sSIGNALS;

                        if ( pNewSignal != nullptr )
                        {
                            pNewSignal->m_oSignalIDVal = nullptr;

                            pNewSignal->m_psNextSignalList = nullptr;

                            // attach the new signal to the data structure
                            pSg->m_psNextSignalList = pNewSignal;

                            // Increment no of signals by 1
                            pMsg->m_unNumberOfSignals++;

                        }
                        else// create signal failed
                        {
                            if (!m_bAutoServerMode)
                            {
                                strcpy_s(s_acTraceStr, 1024,  T2A(_(MSG_MEMORY_CONSTRAINT)));
                                vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                            }
                            bReturnValue = FALSE;
                        }

                        break;
                    }
                }
            }
            else // First signal
            {
                pMsg->m_psSignals = new sSIGNALS;

                if ( pMsg->m_psSignals != nullptr )
                {
                    pMsg->m_psSignals->m_oSignalIDVal = nullptr;

                    pMsg->m_psSignals->m_psNextSignalList = nullptr;

                    // Increment no of signals by 1
                    pMsg->m_unNumberOfSignals++;

                }
                else // create signal failed
                {
                    if (!m_bAutoServerMode)
                    {
                        strcpy_s(s_acTraceStr, 1024,  T2A(_(MSG_MEMORY_CONSTRAINT)));
                        vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
                    }
                    bReturnValue = FALSE;
                }
            }
        }
        else // no message found
        {
            if (!m_bAutoServerMode)
            {
                strcpy_s(s_acTraceStr, 1024, T2A(_(MSG_MSG_NOT_FOUND)));
                vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
            }
            bReturnValue = FALSE;
        }
    }
    else // message name empty
    {
        bReturnValue = FALSE;
    }

    if ( bReturnValue == TRUE)
    {
        m_bIsDatabaseSaved = FALSE;
    }

    return bReturnValue;
}
/******************************************************************************
  Function Name    :  bDeleteSignalFromMsg
  Input(s)         :  int nIndexOfWhichItem,
                      CString omStrMsgName,
                      CString omStrSignalName
  Output           :  BOOL
  Functionality    :  Delete signal from message
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  17.02.2002
  Modifications    :
******************************************************************************/
BOOL CMsgSignal::bDeleteSignalFromMsg( int nIndexOfWhichItem,
                                       CString omStrMsgName,
                                       CString omStrSignalName)
{
    BOOL bReturnValue = FALSE;

    // Temporary list
    sSIGNALS* pCurr = nullptr; //
    sSIGNALS* pPrev = nullptr; // previous list

    if ( !omStrMsgName.IsEmpty() && !omStrSignalName.IsEmpty())
    {
        // Get message pointer corresponding the message "omStrMsgName"
        sMESSAGE* pMsg = psGetMessagePointerInactive(omStrMsgName);

        if ( pMsg != nullptr )
        {
            // Get signal pointer
            sSIGNALS* pSg = pMsg->m_psSignals;

            // Get the signal pointer to be deleted
            while ( pSg != nullptr )
            {
                // found
                if ( pSg->m_omStrSignalName == omStrSignalName )
                {
                    // RESET THE MATRIX VALUE COREESPODING TO THIS SIGNAL
                    // Update the matrix
                    BYTE abySigMask[DATA_LENGTH_MAX] = {0};
                    bCalcBitMaskForSig( abySigMask,
                                        DATA_LENGTH_MAX,
                                        pSg->m_unStartByte,
                                        pSg->m_byStartBit,
                                        pSg->m_unSignalLength,
                                        pSg->m_eFormat);
                    vUpdateSignalMatrix(abySigMask,
                                        pMsg->m_bySignalMatrix,
                                        DATA_LENGTH_MAX,
                                        RESET);

                    // Initialize
                    pPrev = pCurr = pMsg->m_psSignals;

                    // case 1: only one Signal exists
                    if ( !pCurr->m_psNextSignalList )
                    {
                        pCurr = pMsg->m_psSignals;

                        // Delete all Signal IDs and Vals associated with a signal
                        if ( pCurr->m_oSignalIDVal != nullptr )
                        {
                            CSignalDescVal* pTmp = new CSignalDescVal;

                            pTmp = pCurr->m_oSignalIDVal;

                            delete  pTmp;

                            pTmp = nullptr;
                        }
                        delete pCurr;

                        pCurr = nullptr;

                        pMsg->m_unNumberOfSignals = 0;

                        pCurr = pMsg->m_psSignals = nullptr;
                    }

                    // case 2 : deletion for any item,
                    // pPrev comes into the picture

                    // Seek to the edited Prm Values in the data structure
                    while ( pCurr != nullptr )
                    {
                        if ( pCurr->m_omStrSignalName == omStrSignalName ) // found
                        {
                            sSIGNALS* pDelete = nullptr;

                            // Get the pointer to be deleted
                            pDelete = pCurr;

                            // asked for deleting 2nd item onwards
                            if ( nIndexOfWhichItem )
                            {
                                pPrev->m_psNextSignalList = pDelete->m_psNextSignalList;
                            }
                            else // special case : only for deleting first item
                            {
                                pMsg->m_psSignals = pCurr->m_psNextSignalList;
                            }

                            // Delete all Sig ID's and Val if any
                            if ( pDelete->m_oSignalIDVal != nullptr )
                            {
                                delete pDelete->m_oSignalIDVal;

                                pDelete->m_oSignalIDVal = nullptr;
                            }

                            // Decrement the Signal count index
                            pMsg->m_unNumberOfSignals--;

                            delete pDelete;

                            pDelete = nullptr;

                            // job is done!
                            break;
                        }
                        // Not found, traverse
                        sSIGNALS* tmp = pCurr->m_psNextSignalList;

                        pPrev = pCurr;

                        if ( pCurr->m_psNextSignalList )
                        {
                            pCurr = tmp;
                        }
                    }// while ( pCurr != nullptr )

                    bReturnValue = TRUE;

                }// signal not found
                else
                {
                    pSg = pSg->m_psNextSignalList;
                }
                if ( bReturnValue == TRUE)
                {
                    break;
                }
            }// while ( pSg != nullptr )
        }
    }

    if ( bReturnValue == TRUE)
    {
        m_bIsDatabaseSaved = FALSE;
    }

    return bReturnValue;
}
/******************************************************************************
  Function Name    :  vUpdateSignalDetails
  Input(s)         :  CString omStrMsgName,
                      CString omStrSignalName,
                      sSIGNALS *pSgToBeUpdated
  Output           :  void
  Functionality    :  Update signal details to the data structure
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  17.02.2002
  Modifications    :  rajesh : 04.03.2003: BEGIN: changed w.r.t sSIGNALS struct change
******************************************************************************/
void CMsgSignal::vUpdateSignalDetails( CString omStrMsgName,
                                       CString omStrSignalName,
                                       sSIGNALS* pSgToBeUpdated)
{
    if ( !omStrMsgName.IsEmpty() || !omStrSignalName.IsEmpty())
    {
        // Get message pointer corresponding the message "omStrMsgName"
        sMESSAGE* pMsg = psGetMessagePointerInactive(omStrMsgName);
        if ( pMsg != nullptr )
        {
            // Get signal pointer
            sSIGNALS* pSg = pMsg->m_psSignals;
            // Get the signal pointer to be deleted
            while ( pSg != nullptr )
            {
                // found
                if ( pSg->m_omStrSignalName == omStrSignalName ||
                        pSg->m_omStrSignalName.IsEmpty())
                {
                    pSg->m_eFormat = pSgToBeUpdated->m_eFormat;
                    pSg->m_unStartByte = pSgToBeUpdated->m_unStartByte;

                    {
                        pSg->m_byStartBit = pSgToBeUpdated->m_byStartBit;
                    }
                    pSg->m_bySignalType =
                        pSgToBeUpdated->m_bySignalType;
                    pSg->m_fSignalFactor =
                        pSgToBeUpdated->m_fSignalFactor;

                    // rajesh : 04.03.2003: BEGIN: changed w.r.t sSIGNALS struct change
                    pSg->m_SignalMaxValue = pSgToBeUpdated->m_SignalMaxValue;
                    pSg->m_SignalMinValue = pSgToBeUpdated->m_SignalMinValue;
                    // rajesh: 04.03.2003: END

                    pSg->m_fSignalOffset =
                        pSgToBeUpdated->m_fSignalOffset;
                    pSg->m_omStrSignalName =
                        pSgToBeUpdated->m_omStrSignalName;
                    pSg->m_omStrSignalUnit =
                        pSgToBeUpdated->m_omStrSignalUnit;
                    pSg->m_unSignalLength =
                        pSgToBeUpdated->m_unSignalLength;

                    // SET THE MATRIX VALUE COREESPODING TO THIS SIGNAL
                    // Update the matrix
                    BYTE abySigMask[DATA_LENGTH_MAX] = {0};
                    bCalcBitMaskForSig( abySigMask,
                                        DATA_LENGTH_MAX,
                                        pSg->m_unStartByte,
                                        pSg->m_byStartBit,
                                        pSg->m_unSignalLength,
                                        pSg->m_eFormat);
                    vUpdateSignalMatrix(abySigMask,
                                        pMsg->m_bySignalMatrix,
                                        DATA_LENGTH_MAX,
                                        SET);
                    m_bIsDatabaseSaved = FALSE;
                    break;// ur done!
                }
                else // Get next signal
                {
                    pSg = pSg->m_psNextSignalList;
                }
            }
        }// if (pMsg != nullptr)

    }// message name is empty
    else
    {
        if (!m_bAutoServerMode)
        {
            strcpy_s(s_acTraceStr, 1024, _("Insufficient Data"));
            vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
        }
    }
}
/******************************************************************************
  Function Name    :  bAddSgDescVal
  Input(s)         :  CString omStrMsgName,
                      CString omStrSignalName
  Output           :  BOOL
  Functionality    :  Adds new signal desc and value toi the signal
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  18.02.2002
  Modifications    :  Anish , 02.02.07
                      Modified code to remove memory leak
******************************************************************************/
BOOL CMsgSignal::bAddSgDescVal( CString omStrMessageCode,
                                CString omStrSignalName )
{
    BOOL bReturnValue = TRUE;

    if ( !omStrMessageCode.IsEmpty() && !omStrSignalName.IsEmpty())
    {
        UINT unMsgID = strtol(omStrMessageCode,nullptr,16);
        // Get message pointer corresponding the message "omStrMsgName"
        sMESSAGE* pMsg = psGetMessagePointerInactive(unMsgID);

        if ( pMsg != nullptr )
        {
            // Get signal pointer
            sSIGNALS* pSg = pMsg->m_psSignals;

            // Get the signal pointer to be deleted
            while ( pSg != nullptr )
            {
                // found
                if ( pSg->m_omStrSignalName == omStrSignalName )
                {
                    CSignalDescVal* pCurr; /*= new CSignalDescVal;*/

                    pCurr = pSg->m_oSignalIDVal;

                    // if this is the first option
                    if (pCurr == nullptr)
                    {
                        // allocate memory to new ID and Val
                        CSignalDescVal* pSgNewIDVal = new CSignalDescVal;

                        pSgNewIDVal->m_pouNextSignalSignalDescVal= nullptr;

                        pSg->m_oSignalIDVal = pSgNewIDVal;

                        break;

                    }

                    // Seek to end of the Param Val list
                    while ( pCurr != nullptr )
                    {
                        CSignalDescVal* tmp = pCurr->m_pouNextSignalSignalDescVal;

                        if ( pCurr->m_pouNextSignalSignalDescVal )
                        {
                            pCurr = tmp;
                        }
                        else
                        {
                            CSignalDescVal* pNext = new CSignalDescVal;

                            // Update the data structure
                            pNext->m_pouNextSignalSignalDescVal = nullptr;

                            // attach the link
                            pCurr->m_pouNextSignalSignalDescVal = pNext;

                            break;
                        }// if ( pCurr->m_poNextSignalTypeList )

                    }// while ( pCurr != nullptr )

                    break;// ur done!
                }
                else // Get next signal
                {
                    pSg = pSg->m_psNextSignalList;
                }
            }
        }// if (pMsg != nullptr)

    }// message name is empty
    else
    {
        bReturnValue = FALSE;
        if (!m_bAutoServerMode)
        {
            strcpy_s(s_acTraceStr, 1024, _("Insufficient Data"));
            vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
        }
    }

    if ( bReturnValue == TRUE)
    {
        m_bIsDatabaseSaved = FALSE;
    }

    return bReturnValue;
}
/******************************************************************************
  Function Name    :  bUpdateSgDescVal
  Input(s)         :  CString omStrMsgName,
                      CString omStrSignalName,
                      CString omStrPrevDesc,
                      CSignalDescVal *pSgDescVal
  Output           :  BOOL
  Functionality    :  Updates signal desc and value to the signal
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  18.02.2002
  Modifications    :
******************************************************************************/
BOOL CMsgSignal::bUpdateSgDescVal( CString omStrMessageCode,
                                   CString omStrSignalName,
                                   CString omStrPrevDesc,
                                   CSignalDescVal* pSgDescVal)
{
    BOOL bReturnValue = TRUE;

    if ( !omStrMessageCode.IsEmpty() && !omStrSignalName.IsEmpty())
    {
        UINT unMsgID = strtol(omStrMessageCode,nullptr,16);
        // Get message pointer corresponding the message "omStrMsgName"
        sMESSAGE* pMsg = psGetMessagePointerInactive(unMsgID);

        if ( pMsg != nullptr )
        {
            // Get signal pointer
            sSIGNALS* pSg = pMsg->m_psSignals;

            // Get the signal pointer
            while ( pSg != nullptr )
            {
                // found
                if ( pSg->m_omStrSignalName == omStrSignalName )
                {
                    CSignalDescVal* pTempSgDescVal =
                        pSg->m_oSignalIDVal;

                    while ( pTempSgDescVal != nullptr )
                    {
                        if ( pTempSgDescVal->m_omStrSignalDescriptor ==
                                omStrPrevDesc)
                        {
                            // Update
                            if(pSg->m_bySignalType == CHAR_INT)
                            {
                                pTempSgDescVal->m_DescValue.n64Value =
                                    pSgDescVal->m_DescValue.n64Value;
                            }
                            else
                            {
                                pTempSgDescVal->m_DescValue.un64Value =
                                    pSgDescVal->m_DescValue.un64Value;
                            }

                            pTempSgDescVal->m_omStrSignalDescriptor =
                                pSgDescVal->m_omStrSignalDescriptor;

                            break;
                        }
                        else
                        {
                            pTempSgDescVal =
                                pTempSgDescVal->m_pouNextSignalSignalDescVal;
                        }
                    }
                    break;
                }
                else // Get next signal
                {
                    pSg = pSg->m_psNextSignalList;
                }
            }
        }// if (pMsg != nullptr)

    }// message name is empty
    else
    {
        bReturnValue = FALSE;
        if (!m_bAutoServerMode)
        {
            strcpy_s(s_acTraceStr, 1024, _("Insufficient Data"));
            vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
        }
    }

    if ( bReturnValue == TRUE)
    {
        m_bIsDatabaseSaved = FALSE;
    }

    return bReturnValue;
}
/******************************************************************************
  Function Name    :  bDeleteSgDescVal
  Input(s)         :  int nIndexOfItem,
                      CString omStrMsgName,
                      CString omStrSignalName,
                      CString omStrDesc
  Output           :  BOOL
  Functionality    :  Deletes signal desc and value from the signal
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  18.02.2002
  Modifications    :  Anish,02.02.07
                      Modified code to remove memory leak
******************************************************************************/
BOOL CMsgSignal::bDeleteSgDescVal( int nIndexOfItem,
                                   CString omStrMsgName,
                                   CString omStrSignalName,
                                   CString omStrDesc)
{
    BOOL bReturnValue = TRUE;

    if ( !omStrMsgName.IsEmpty() && !omStrSignalName.IsEmpty())
    {
        // Get message pointer corresponding the message "omStrMsgName"
        sMESSAGE* pMsg = psGetMessagePointerInactive(omStrMsgName);

        if ( pMsg != nullptr )
        {
            // Get signal pointer
            sSIGNALS* pSg = pMsg->m_psSignals;

            // Get the signal pointer to be deleted
            while ( pSg != nullptr )
            {
                // found
                if ( pSg->m_omStrSignalName == omStrSignalName )
                {
                    // Temporary list
                    CSignalDescVal* pCurr = nullptr,  *pPrev = nullptr;

                    // Iniatialize
                    pPrev = pCurr = pSg->m_oSignalIDVal;

                    // case 1: only one item exists
                    if ( pCurr->m_pouNextSignalSignalDescVal == nullptr )
                    {
                        pCurr = pSg->m_oSignalIDVal;

                        delete pCurr;

                        pCurr = pSg->m_oSignalIDVal = nullptr;

                        break;
                    }
                    // case 2 : deletion for any item,
                    // pPrev comes into the picture

                    CSignalDescVal* tmp;

                    if ( pCurr != nullptr )
                    {
                        tmp = pCurr->m_pouNextSignalSignalDescVal;
                    }

                    // Seek to the edited Prm Values in the data structure
                    while ( pCurr != nullptr )
                    {
                        if ( pCurr->m_omStrSignalDescriptor ==
                                omStrDesc) // found
                        {
                            CSignalDescVal* pDelete; /*= new CSignalDescVal;*/

                            // Get the pointer to be deleted
                            pDelete = pCurr;

                            // asked for deleting 2nd item onwards
                            if ( nIndexOfItem )
                            {
                                pPrev->m_pouNextSignalSignalDescVal =
                                    pDelete->m_pouNextSignalSignalDescVal;
                            }
                            else // special case : only for deleting first item
                            {
                                pSg->m_oSignalIDVal  = pCurr->m_pouNextSignalSignalDescVal;
                            }

                            delete pDelete;

                            // job is done!
                            break;
                        }
                        // Not found, traverse

                        tmp = pCurr->m_pouNextSignalSignalDescVal;

                        pPrev = pCurr;

                        if ( pCurr->m_pouNextSignalSignalDescVal )
                        {
                            pCurr = tmp;
                        }
                    }// while ( pCurr != nullptr )

                    break;
                }
                else // Get next signal
                {
                    pSg = pSg->m_psNextSignalList;
                }
            }
        }// if (pMsg != nullptr)

    }// message name is empty
    else
    {
        bReturnValue = FALSE;
        if (!m_bAutoServerMode)
        {
            strcpy_s(s_acTraceStr, 1024, _("Insufficient Data"));
            vWriteTextToTrace();//(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);
        }
    }

    if ( bReturnValue == TRUE)
    {
        m_bIsDatabaseSaved = FALSE;
    }

    return bReturnValue;
}
/******************************************************************************
  Function Name    :  vConvertStandardToExtendedFrameFormat
  Input(s)         :  int& nMsgCode
  Output           :  void
  Functionality    :  Converts Standard To Extended Frame Format
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  18.02.2002
  Modifications    :
******************************************************************************/
void CMsgSignal::vConvertStandardToExtendedFrameFormat(int& nMsgCode)
{
    if ( nMsgCode < MAX_LMT_FOR_STD_MSG_ID )
    {
        CString omStrMsgCode = "";

        omStrMsgCode.Format( "%d", nMsgCode);

        UINT nMaxCount = 9 - omStrMsgCode.GetLength();

        // Append zero's to the message code
        for (UINT nCount = 0; nCount < nMaxCount; nCount++ )
        {
            omStrMsgCode += "0";
        }

        nMsgCode = atoi( omStrMsgCode );
    }
}
/******************************************************************************
  Function Name    :  vConvertExtendedToStandardFrameFormat
  Input(s)         :  int& nMsgCode
  Output           :  void
  Functionality    :  Converts Extended To Standard Frame Format
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  19.02.2002
  Modifications    :
******************************************************************************/
void CMsgSignal::vConvertExtendedToStandardFrameFormat(int& nMsgCode)
{
    if ( nMsgCode >= MAX_LMT_FOR_STD_MSG_ID )
    {
        CString omStrMsgCode = "";

        omStrMsgCode.Format( "%d", nMsgCode);

        // if the first char is less than '2'
        // then extract 4 digits else extract 3
        char cChar = omStrMsgCode.GetAt(0);

        if ( cChar > '1' )
        {
            omStrMsgCode = omStrMsgCode.Left( 3 );
        }
        else
        {
            omStrMsgCode = omStrMsgCode.Left( 4 );
        }

        nMsgCode = atoi( omStrMsgCode );
    }
}
/******************************************************************************
  Function Name    :  psGetMessagePointer
  Input(s)         :  CString strMsgName
  Output           :  sMESSAGE*
  Functionality    :  Returns associted message pointer if found, otherwise
                      nullptr.
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  19.02.2002
  Modifications    :  Anish
                      13.12.2006,changes code for CMap
******************************************************************************/
sMESSAGE* CMsgSignal::psGetMessagePointer( CString strMsgName)
{
    m_omDBMapCritSec.Lock();
    POSITION pos = m_omMsgDetailsIDMap.GetStartPosition();
    sMESSAGE* psMsgStruct = nullptr;
    UINT unKey;
    while (pos != nullptr )
    {
        m_omMsgDetailsIDMap.GetNextAssoc( pos, unKey,psMsgStruct );
        if((nullptr != psMsgStruct) && (!(psMsgStruct->m_omStrMessageName.Compare(strMsgName))))
        {
            return psMsgStruct;
        }
    }
    m_omDBMapCritSec.Unlock();
    return nullptr;
}
/******************************************************************************
  Function Name    :  psGetMessagePointerInactive
  Input(s)         :  UINT unMsgID
  Output           :  sMESSAGE*
  Functionality    :  Returns associted message pointer if found, otherwise
                      nullptr.Called when dealing with inactive DB
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amitesh Bharti
  Date Created     :  08.03.2004
  Modifications    :  Anish
                      13.12.2006,changes code for CMap
******************************************************************************/
sMESSAGE* CMsgSignal::psGetMessagePointerInactive( UINT unMsgID)
{
    // validate message ID
    for (UINT unMsgCount = 0; unMsgCount < m_unMessageCount; unMsgCount++ )
    {
        if ( m_psMessages[unMsgCount].m_unMessageCode == unMsgID )
        {
            return &m_psMessages[unMsgCount];
        }
    }
    return nullptr;
}
/******************************************************************************
  Function Name    :  psGetMessagePointerInactive
  Input(s)         :  CString strMsgName
  Output           :  sMESSAGE*
  Functionality    :  Returns associted message pointer if found, otherwise
                      nullptr.Called if working with Inactive Database
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  19.02.2002
  Modifications    :  Anish
                      13.12.2006,changes code for CMap
******************************************************************************/
sMESSAGE* CMsgSignal::psGetMessagePointerInactive(CString strMsgName)
{
    // validate message name
    for (UINT unMsgCount = 0; unMsgCount < m_unMessageCount; unMsgCount++ )
    {
        if ( m_psMessages[unMsgCount].
                m_omStrMessageName.Compare(strMsgName) == 0 )
        {
            return &m_psMessages[unMsgCount];
        }
    }
    return nullptr;
}
/******************************************************************************
  Function Name    :  psGetMessagePointer
  Input(s)         :  UINT unMsgID
  Output           :  sMESSAGE*
  Functionality    :  Returns associted message pointer if found, otherwise
                      nullptr.
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amitesh Bharti
  Date Created     :  08.03.2004
  Modifications    :  Anish
                      13.12.2006,changes code for CMap
******************************************************************************/
sMESSAGE* CMsgSignal::psGetMessagePointer( UINT unMsgID)
{
    sMESSAGE* psMsgStruct = nullptr;
    m_omMsgDetailsIDMap.Lookup(unMsgID,psMsgStruct);
    return psMsgStruct;
}
/******************************************************************************
  Function Name    :  bGetDBAcitveFlagStatus
  Input(s)         :  -
  Output           :  BOOL
  Functionality    :  Returns the value of m_bIsDatabaseActive
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  19.02.2002
  Modifications    :
******************************************************************************/
BOOL CMsgSignal::bGetDBAcitveFlagStatus()
{
    return m_bIsDatabaseActive;
}
/******************************************************************************
  Function Name    :  vSetDBActiveFlagStatus
  Input(s)         :  -
  Output           :  -
  Functionality    :  Sets the value of m_bIsDatabaseActive
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  19.02.2002
  Modifications    :
******************************************************************************/
void CMsgSignal::vSetDBActiveFlagStatus(BOOL bValue)
{
    m_bIsDatabaseActive = bValue;
}
/******************************************************************************
  Function Name    :  omStrListGetMessageNames
  Input(s)         :  CStringList &omStrListMsgs
  Output           :  CStringList &omStrListMsgs
  Functionality    :  Fills the list with all message names
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Anish kumar
  Date Created     :  19.12.2006
  Modifications    :
******************************************************************************/
void CMsgSignal::omStrListGetMessageNames(CStringList& omStrListMsgs)
{
    // Clear the list
    omStrListMsgs.RemoveAll();
    m_omDBMapCritSec.Lock();
    // Clear the list
    int nTotalMsgCount = m_omMsgDetailsIDMap.GetCount();
    POSITION pos = m_omMsgDetailsIDMap.GetStartPosition( );
    sMESSAGE* psTempMsgStruct;
    UINT unMsgKey;
    for (int nMsgIndex = 0; nMsgIndex < nTotalMsgCount && pos != nullptr; nMsgIndex++ )
    {
        m_omMsgDetailsIDMap.GetNextAssoc(pos,unMsgKey,psTempMsgStruct);
        omStrListMsgs.AddHead( psTempMsgStruct->m_omStrMessageName);
    }
    m_omDBMapCritSec.Unlock();
}
/******************************************************************************
  Function Name    :  omStrListGetMessageNames
  Input(s)         :  CStringList &omStrListMsgs
  Output           :  CStringList &omStrListMsgs
  Functionality    :  Fills the list with all message names
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  19.02.2002
  Modifications    :
******************************************************************************/
void CMsgSignal::omStrListGetMessageNamesInactive(CStringList& omStrListMsgs)
{
    // Clear the list
    omStrListMsgs.RemoveAll();

    for (UINT unMsgIndex = 0; unMsgIndex < m_unMessageCount; unMsgIndex++ )
    {
        if (m_psMessages != nullptr )
        {
            omStrListMsgs.AddHead( m_psMessages[unMsgIndex].m_omStrMessageName );
        }
    }

}
/******************************************************************************
  Function Name    :  bGetModifiedFlag
  Input(s)         :
  Output           :  BOOL
  Functionality    :  Returns the value of m_bIsDatabaseSaved
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  20.02.2002
  Modifications    :
******************************************************************************/
BOOL CMsgSignal::bGetModifiedFlag()
{
    return (m_bIsDatabaseSaved);
}

/******************************************************************************
  Function Name    :  bGetModifiedFlag
  Input(s)         :  BOOL
  Output           :
  Functionality    :  sets the value of m_bIsDatabaseSaved
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Saravanan K S
  Date Created     :  04.04.2012
  Modifications    :
******************************************************************************/
void CMsgSignal::vSetModifiedFlag(BOOL bSaved)
{
    m_bIsDatabaseSaved = bSaved;
}

/******************************************************************************
  Function Name    :  vUpdateSignalMatrix
  Input(s)         :  BYTE *pByte,
                      BOOL bUpdateType,
                      BOOL bDataFormat,
                      sSIGNALS* pSg
  Output           :  -
  Functionality    :  Updates the signal matrix which will be used for
                      validation
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  20.02.2002
  Modifications    :  12.12.2002, Amarnath S
                      Overlapping Problem of signals of length more than
                      16 bits rectified.
  Modifications    :  17.06.2002, Amarnath S
                      Overlapping Problem of signals of length more than
                      32 bits rectified.
******************************************************************************/
void CMsgSignal::vUpdateSignalMatrix(const BYTE* pSrcByte,
                                     BYTE* pDestByte,
                                     UINT unArrayLen,
                                     BOOL bUpdateType)
{
    if (bUpdateType == RESET)//Remove this sig info from msg matrix
    {
        for (UINT i = 0; i < unArrayLen; i++)
        {
            pDestByte[i] &= ~pSrcByte[i];
        }
    }
    else // Update this sig info into msg matrix
    {
        for (UINT i = 0; i < unArrayLen; i++)
        {
            pDestByte[i] |= pSrcByte[i];
        }
    }
}

/******************************************************************************
  Function Name    :  bItemCanHaveSignalDesc
  Input(s)         :  CString omStrMsgName,
                      CString omStrSgName,
                      UINT nItemCount
  Output           :  BOOL
  Functionality    :  Checks whether a signal canm have new
                      signal description and value
                      validation
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  20.02.2002
  Modifications    :  Rajesh Kumar 04.03.2003: change w.r.t change in sSIGNALS
                                                - min/max values
  Modifications    :  Amitesh Bharti, 05.06.2003,replace datatype for
                      supporting 64bits to __int64
******************************************************************************/
BOOL CMsgSignal::bItemCanHaveSignalDesc(CString omStrMsgName,
                                        CString omStrSgName,
                                        unsigned __int64 un64ItemCount)
{
    BOOL bRetVal = FALSE;

    if ( !omStrMsgName.IsEmpty()    &&
            !omStrSgName.IsEmpty())
    {
        for ( UINT unMsgCount = 0; (unMsgCount < m_unMessageCount); unMsgCount++ )
        {
            if ( m_psMessages[unMsgCount].m_omStrMessageName == omStrMsgName )
            {
                if (m_psMessages[unMsgCount].m_unNumberOfSignals > 0 )
                {
                    sSIGNALS* p_Sg = m_psMessages[unMsgCount].m_psSignals;

                    while ( p_Sg != nullptr && bRetVal == FALSE )
                    {
                        if ( p_Sg->m_omStrSignalName != omStrSgName )
                        {
                            p_Sg = p_Sg->m_psNextSignalList;
                        }
                        else
                        {
                            //check for max no of descriptor should not exceed max no of signals
                            //i.e. (max - min)range of a signal

                            unsigned __int64 un64PossibleSgDescMinVal = 0;
                            unsigned __int64 un64PossibleSgDescMaxVal = 0;
                            if(p_Sg->m_bySignalType == CHAR_INT) // if signed type
                            {
                                un64PossibleSgDescMaxVal = p_Sg->m_SignalMaxValue.n64Value ;
                                un64PossibleSgDescMinVal = - p_Sg->m_SignalMinValue.n64Value;

                                __int64 n64ItemCount =  un64PossibleSgDescMaxVal - un64ItemCount;
                                if(n64ItemCount <= 0 ) //since it is signed signal
                                {
                                    n64ItemCount = n64ItemCount + un64PossibleSgDescMinVal + 1;
                                }
                                if ( n64ItemCount >0 )
                                {
                                    bRetVal = TRUE;
                                }
                            }
                            else // bool or unsigned int
                            {
                                un64PossibleSgDescMaxVal = p_Sg->m_SignalMaxValue.un64Value ;
                                un64PossibleSgDescMinVal = p_Sg->m_SignalMinValue.un64Value;

                                //max no of descriptor should not exceed max value of signal
                                /* Values possible : (max - min  >= Desc Count ) */
                                if ( un64PossibleSgDescMaxVal - un64PossibleSgDescMinVal >= un64ItemCount )
                                {
                                    bRetVal = TRUE;
                                }

                            }

                            p_Sg = nullptr;

                            unMsgCount = m_unMessageCount;
                        }
                    }// while ( p_Sg != nullptr )

                }// if (m_psMessages[unMsgCount].m_unNumberOfSignals > 0 )
            }// message not found

        }// for loop
    }// if

    return bRetVal;
}
/******************************************************************************
  Function Name    :  omStrListGetMessageIDs
  Input(s)         :  CStringList &omStrListMsgs
  Output           :  CStringList &omStrListMsgs
  Functionality    :  Fills the list with all message id's
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  05.04.2002
  Modifications    :
******************************************************************************/

void CMsgSignal::unListGetMessageIDs(UINT* omListId)
{
    m_omDBMapCritSec.Lock();
    // Clear the list
    int nTotalMsgCount = m_omMsgDetailsIDMap.GetCount();
    POSITION pos = m_omMsgDetailsIDMap.GetStartPosition( );
    sMESSAGE* psTempMsgStruct;
    UINT unMsgKey;
    for (int unMsgIndex = 0; unMsgIndex < nTotalMsgCount && pos != nullptr; unMsgIndex++ )
    {
        m_omMsgDetailsIDMap.GetNextAssoc(pos,unMsgKey,psTempMsgStruct);
        omListId[unMsgIndex] = psTempMsgStruct->m_unMessageCode;
    }
    m_omDBMapCritSec.Unlock();
}
/******************************************************************************
  Function Name    :  vGetSigNamesAndIndexTobeDeleted
  Input(s)         :  UINT unMessageLength
                      CString omStrMsgName
  Output           :  CStringArray& omStrList,
                      UINT unIndexes[]
  Functionality    :  Gets the names of the signals along with their
                      indexes whose byte index is more than
                      the given message length
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  10.12.2002
  Modifications    :
******************************************************************************/
void CMsgSignal::vGetSigNamesAndIndexTobeDeleted( UINT unMessageLength,
        CString omStrMsgName,
        CStringArray& omStrList,
        UINT unIndexes[])
{
    if ( omStrMsgName.IsEmpty() == FALSE )
    {
        // Clear the list
        omStrList.RemoveAll();
        // Get message pointer
        sMESSAGE* pMsg = psGetMessagePointerInactive( omStrMsgName );

        if ( pMsg != nullptr)
        {
            sSIGNALS* pSignal = pMsg->m_psSignals;

            // Count for Index
            USHORT usIndex = 0;
            // Signal Count
            USHORT usSgCount = 0;

            while ( pSignal != nullptr )
            {
                //get minimum message length for this signal in bits
                int nSignalMsgLen = (pSignal->m_unStartByte -1 ) * defBITS_IN_BYTE;
                nSignalMsgLen +=  pSignal->m_byStartBit + pSignal->m_unSignalLength;

                //get message length in bits
                int MsgMaxLength = unMessageLength * defBITS_IN_BYTE;

                //                if ( pSignal->m_unStartByte >
                //                    unMessageLength )

                if ( nSignalMsgLen > MsgMaxLength)
                {
                    // Add to the list
                    omStrList.Add( pSignal->m_omStrSignalName );
                    // Add the signal count to the index
                    unIndexes[usIndex] = usSgCount;
                    ++usIndex;
                }
                else
                {
                    ++usSgCount;
                }

                pSignal = pSignal->m_psNextSignalList;
            }
        }
    }
}
/******************************************************************************
Function Name   : bIsDuplicateValueDescription
Input(s)        : omStrMessageCode, omStrSgName, omStrSignalValue,
                  omStrDescription, nIndex, bMode
Output          : BOOL
Functionality   : Checks for duplicate signal description
Member of       : CMsgSignal
Friend of       :     -
Author(s)       : Amarnath Shastry
Date Created    : 11-12-2002
Modifications   : Amitesh Bharti, 20.05.2003, Correct to avoid self test and
                  problem related to it by introducing the new parameter nIndex
Modifications   : Amitesh Bharti, 05.06.2003,replace datatype for supporting
                  64bits to __int64
Modifications   : Anish, 11.12.2006,Change the parameter from message name to
                  message code
Modifications   : Ratnadip Choudhury, 19.3.2011, removed accessing of mainframe
                  for invoking string to integer conversion function. Using
                  utility library instead.
******************************************************************************/
BOOL CMsgSignal::bIsDuplicateValueDescription( CString omStrMessageCode,
        CString omStrSgName,
        CString omStrSignalValue,
        CString omStrDescription,
        INT nIndex,
        BOOL bMode)
{
    BOOL bReturnValue = FALSE;

    if ( !omStrMessageCode.IsEmpty() &&
            !omStrSgName.IsEmpty()      &&
            !omStrSignalValue.IsEmpty() &&
            !omStrDescription.IsEmpty())
    {
        UINT unMsgID = strtol(omStrMessageCode,nullptr,16);
        // Get message pointer
        sMESSAGE* pMsg = psGetMessagePointerInactive( unMsgID );

        BOOL bInLoop = TRUE;

        if ( pMsg != nullptr)
        {
            // Get signal pointer
            sSIGNALS* p_Sg = pMsg->m_psSignals;
            // Get description
            while ( p_Sg != nullptr &&
                    bInLoop == TRUE )
            {
                if ( p_Sg->m_omStrSignalName != omStrSgName )
                {
                    p_Sg = p_Sg->m_psNextSignalList;
                }
                else
                {
                    CSignalDescVal* pSgDesc =
                        p_Sg->m_oSignalIDVal;
                    INT nCount = 0;
                    // Get corrrect description
                    while ( pSgDesc != nullptr &&
                            bInLoop == TRUE  )
                    {
                        if ( bMode == MD_EDIT )
                        {
                            // Avoid self check in EDIT mode
                            // i.e. if the index of selected item and loop
                            // loop count is same it is same signal descriptor
                            if(nCount != nIndex )
                            {
                                if ( ( pSgDesc->m_omStrSignalDescriptor ==
                                        omStrDescription )
                                   )
                                {
                                    // Duplicate found
                                    bReturnValue = TRUE;
                                    // Loop condititon
                                    bInLoop = FALSE;
                                }
                            }

                        }
                        else
                        {
                            if ( pSgDesc->m_omStrSignalDescriptor ==
                                    omStrDescription)
                            {
                                // Duplicate found
                                bReturnValue = TRUE;
                                // Loop condititon
                                bInLoop = FALSE;
                            }
                        }
                        pSgDesc = pSgDesc->m_pouNextSignalSignalDescVal;
                        nCount++;
                    }

                    // Loop condititon
                    bInLoop = FALSE; // If no description is defined
                }
            }
        }
    }
    return ( bReturnValue );
}
/******************************************************************************
  Function Name    :  bIsDuplicateSignalValue
  Input(s)         :  CString omStrMessageName,
                      CString omStrSgName,
                      CString omStrSignalValue,
                      CString omStrDescription,
                      INT nIndex,
                      BOOL bMode
  Output           :  BOOL
  Functionality    :  Checks for duplicate signal value
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  11-12-2002
  Modifications    :  Amitesh Bharti,
                      20.05.2003, Correct to avoid self test and problem
                      related to it by introducing the new parameter nIndex
  Modifications    :  Amitesh Bharti, 05.06.2003,replace datatype for
                      supporting 64bits to __int64
  Modifications    :  Anish, 11.12.2006,Change the parameter from message name
                      to message code
******************************************************************************/
BOOL CMsgSignal::bIsDuplicateSignalValue( CString omStrMessageCode,
        CString omStrSgName,
        CString omStrSignalValue,
        CString omStrDescription,
        INT nIndex,
        BOOL bMode)
{
    BOOL bReturnValue = FALSE;

    if ( !omStrMessageCode.IsEmpty() &&
            !omStrSgName.IsEmpty()      &&
            !omStrSignalValue.IsEmpty() &&
            !omStrDescription.IsEmpty())
    {
        UINT unMsgID = strtol(omStrMessageCode,nullptr,16);
        // Get message pointer
        sMESSAGE* pMsg = psGetMessagePointerInactive( unMsgID );
        /*CMainFrame* pMainFrame =
            (CMainFrame*)AfxGetApp()->m_pMainWnd; */

        BOOL bInLoop = TRUE;

        if ( pMsg       != nullptr /*&&
             pMainFrame != nullptr*/)
        {
            //__int64  n64SgVal = /*pMainFrame->*/nConvertStringToInt( omStrSignalValue );

            // Get signal pointer
            sSIGNALS* p_Sg = pMsg->m_psSignals;
            // Get description
            while ( p_Sg != nullptr &&
                    bInLoop == TRUE )
            {
                if ( p_Sg->m_omStrSignalName != omStrSgName )
                {
                    p_Sg = p_Sg->m_psNextSignalList;
                }
                else
                {
                    __int64  n64SgVal = 0;
                    unsigned __int64  un64SgVal = 0;
                    if(p_Sg->m_bySignalType == CHAR_INT) // signed type
                    {
                        n64SgVal = _strtoi64( omStrSignalValue, nullptr, 10 );
                    }
                    else //unsigned / bool type
                    {
                        un64SgVal = _strtoui64( omStrSignalValue, nullptr, 10 );
                    }

                    CSignalDescVal* pSgDesc = p_Sg->m_oSignalIDVal;
                    INT nCount = 0;
                    // Get corrrect description
                    while ( pSgDesc != nullptr && bInLoop == TRUE )
                    {
                        if(p_Sg->m_bySignalType == CHAR_INT) // signed type
                        {
                            if ( pSgDesc->m_DescValue.n64Value == n64SgVal)
                            {
                                if ( bMode == MD_EDIT )
                                {
                                    // Avoid self check in EDIT mode
                                    if ( nCount != nIndex)
                                    {
                                        // Duplicate found
                                        bReturnValue = TRUE;
                                        // Loop condititon
                                        bInLoop = FALSE;
                                    }
                                }
                                else //add mode
                                {
                                    // Duplicate found
                                    bReturnValue = TRUE;
                                    // Loop condititon
                                    bInLoop = FALSE;
                                }
                            }
                        }
                        else //unsigned / bool type
                        {
                            if ( pSgDesc->m_DescValue.un64Value == un64SgVal)
                            {
                                if ( bMode == MD_EDIT )
                                {
                                    // Avoid self check in EDIT mode
                                    if ( nCount != nIndex)
                                    {
                                        // Duplicate found
                                        bReturnValue = TRUE;
                                        // Loop condititon
                                        bInLoop = FALSE;
                                    }
                                }
                                else
                                {
                                    // Duplicate found
                                    bReturnValue = TRUE;
                                    // Loop condititon
                                    bInLoop = FALSE;
                                }
                            }
                        }
                        pSgDesc = pSgDesc->m_pouNextSignalSignalDescVal;
                        nCount++;
                    }

                    // Loop condititon
                    bInLoop = FALSE; // If no description is defined
                }
            }
        }
    }
    return ( bReturnValue );
}
/******************************************************************************
  Function Name    :  bIsSignalValueOutofRange
  Input(s)         :  CString omStrMessageName,
                      CString omStrSgName,
                      CString omStrSignalValue
  Output           :  BOOL
  Functionality    :  Checks if signal value specified is out of range
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Amarnath Shastry
  Date Created     :  11-12-2002
  Modifications    :  Rajesh Kumar 04.03.2003: modification w.r.t change in
                                               sSIGNALS struct
  Modifications    :  Amitesh Bharti, 05.06.2003,replace datatype for
                      supporting 64bits to __int64
 .Modifications    :  Amitesh Bharti, 11.06.2003,
                                     Rework due to unit testing for CRH0005
 .Modifications    :  Amitesh Bharti, 17.06.2003,
                      Condition to check the out of range value corrected.
******************************************************************************/
BOOL CMsgSignal::bIsSignalValueOutofRange( CString omStrMessageCode,
        CString omStrSgName,
        CString omStrSignalValue)
{
    BOOL bReturnValue = FALSE;

    if ( !omStrMessageCode.IsEmpty() &&
            !omStrSgName.IsEmpty()      &&
            !omStrSignalValue.IsEmpty())
    {
        UINT unMsgID = strtol(omStrMessageCode,nullptr,16);
        // Get message pointer
        sMESSAGE* pMsg = psGetMessagePointerInactive( unMsgID );
        /*CMainFrame* pMainFrame =
            (CMainFrame*)AfxGetApp()->m_pMainWnd; */

        BOOL bInLoop = TRUE;

        if ( pMsg       != nullptr /*&&
             pMainFrame != nullptr*/)
        {
            // Get signal pointer
            sSIGNALS* p_Sg = pMsg->m_psSignals;
            // Get description
            while ( p_Sg != nullptr &&
                    bInLoop == TRUE )
            {
                if ( p_Sg->m_omStrSignalName != omStrSgName )
                {
                    p_Sg = p_Sg->m_psNextSignalList;
                }
                else
                {
                    // BEGIN :check validity of range based on signal type
                    if(p_Sg->m_bySignalType == CHAR_INT) // signed type
                    {
                        //                        __int64 n64SgVal = /*pMainFrame->*/nConvertStringToInt( omStrSignalValue );
                        __int64 n64SgVal = _strtoi64( omStrSignalValue, nullptr, 10 );
                        // now check validity range
                        if( (n64SgVal > p_Sg->m_SignalMaxValue.n64Value) ||
                                (n64SgVal < p_Sg->m_SignalMinValue.n64Value) )
                        {
                            bReturnValue = TRUE;
                        }
                    }
                    else
                    {
                        //                        __int64 un64SgVal = /*pMainFrame->*/nConvertStringToInt( omStrSignalValue );
                        __int64 un64SgVal = _strtoui64( omStrSignalValue, nullptr, 10 );
                        // now check validity range
                        if( ( un64SgVal > p_Sg->m_SignalMaxValue.un64Value ) ||
                                ( un64SgVal < p_Sg->m_SignalMinValue.un64Value )    )
                        {
                            bReturnValue = TRUE;
                        }
                    }
                    bInLoop = FALSE;
                }
            }
        }
    }
    return  bReturnValue;
}
/******************************************************************************
  Function Name    :  bSortSignalStartBitAscend

  Input(s)         :  UINT *punStartBit: pointer to array of start bits of
                                         signal
                      UINT unCount  : Total count of signal.
  Output           :  TRUE/FALSE : Return value
                      UINT *punStartBit : Array of sorted value of start bit
                                          of signals
  Functionality    :  This function will sort the start bits in assending order.
                      The sorted start bit is returned in punStartBit based on
                      the number of signal count passed as unCount.
  Member of        :  CMsgSignal
  Friend of        :      -

  Author(s)        :  Amitesh Bharti
  Date Created     :  12.03.2003
  Modifications    :  Amitesh Bharti, 22.04.2003, review comments incorporated.
******************************************************************************/
BOOL CMsgSignal::bSortSignalStartBitAscend(UINT* punSigStartBit, UINT unCount)
{
    BOOL bReturn = FALSE;

    if(unCount >0 && punSigStartBit != nullptr )
    {
        for(UINT i = 0 ; i< unCount ; i++ )
        {
            for(UINT j = i+1 ; j< unCount ; j++ )
            {
                if(punSigStartBit[i] > punSigStartBit[j] )
                {
                    UINT unTemp = punSigStartBit[i];
                    punSigStartBit[i] = punSigStartBit[j];
                    punSigStartBit[j] = unTemp;
                }
            }
        }
        bReturn = TRUE;
    }

    return 1;
}

/******************************************************************************
  Function Name    :  bFormSigNameAndLength

  Input(s)         :  UINT *punLength : pointer to array of length of signals
                      const UINT *punStartBit: array of start bits of signal
                      CStringArray &omStrArraySigName : reference to
                                                        CStringArray
                      const INT nIndex : message index
  Output           :  TRUE/FALSE : Return value
                      UINT *punLength : Array of computed value of length of
                                        signals
                      CStringArray &omStrArraySigName : signal name for bitwise
                                                        structure
  Functionality    :  This function will form the signal name and its length for
                      the definition of bitwise structure. The punStartBit and
                      nIndex are the input parameter. punLength and
                      omStrArraySigName are the input and output parameter.The
                      signal name and its length is computed based on the
                      message index passed as nIndex. Function returns TRUE
                      is the computation is successfull.
  Member of        :  CMsgSignal
  Friend of        :      -

  Author(s)        :  Amitesh Bharti
  Date Created     :  12.03.2003
  Modifications    :  Amitesh Bharti, 22.04.2003, review comments incorporated.
                      Raja N 12.02.2004
                      Modified to include data type along with signal name
******************************************************************************/
/*BOOL CMsgSignal::bFormSigNameAndLength(UINT* punLength,
                                       UINT* punStartBit,
                                       CStringArray& omStrArraySigName,
                                       const INT nIndex)
{
    BOOL bReturn    = FALSE;
    UINT unSigCount = unSigCount = m_psMessages[nIndex].m_unNumberOfSignals;;
    sSIGNALS *pSg = m_psMessages[nIndex].m_psSignals;
    UINT unStartBit = 0;
    UINT unUnused   = 0;
    UINT unCount    = 0;

    //rmdup(punStartBit, unSigCount);
    UINT *p;
    p = (UINT*)(std::unique(punStartBit, punStartBit+unSigCount));
    unSigCount = (p-punStartBit);

    if( pSg != nullptr && punLength != nullptr && punStartBit != nullptr )
    {
       // UINT unSigCount = m_psMessages[nIndex].m_unNumberOfSignals;
        UINT unStartBit = 0;
        UINT unUnused   = 0;
        UINT unCount    = 0;
        CString omFormatString;
        for(UINT i = 0 ; i <unSigCount ; i++ )
        {
            while(pSg != nullptr )
            {
                unStartBit  = pSg->m_unStartByte - 1;
                unStartBit  = unStartBit * defBITS_IN_BYTE ;
                unStartBit += pSg->m_byStartBit ;
                if(punStartBit[i]  == unStartBit )
                {
                    // if first signal is not defined from first bit in first
                    // byte
                    if(punStartBit[i] > 0  && i == 0)
                    {
                        // If more then first 32 bits are not having
                        // any signal defined, add two signal without
                        // any name one with size 32 bit and other
                        // with size of rest of bits.
                        if(punStartBit[i] > defBITS_IN_FOUR_BYTE )
                        {
                            omFormatString.Format(defUNION_FORMAT_STRING,
                                                  defUNSIGNED_INT,
                                                  "",
                                                  defUINT_LENGTH);
                            omStrArraySigName.Add(omFormatString);
                            // Start bit is zero index
                            punLength[i + unUnused] = defBITS_IN_FOUR_BYTE;
                            unUnused++;
                            unCount++;
                        }
                        // Start bit is zero index
                        punLength[i + unUnused] = punStartBit[i] ;
                        punLength[i + unUnused] -= unCount * defBITS_IN_FOUR_BYTE;

                        omFormatString.Format(defUNION_FORMAT_STRING,
                                              defUNSIGNED_INT,
                                              "",
                                              punLength[i + unUnused]);
                        omStrArraySigName.Add(omFormatString);
                        unUnused++;
                        unCount = 0;
                    }

                    // For signals > 32 bits
                    if( pSg->m_unSignalLength > defBITS_IN_FOUR_BYTE)
                    {
                        omFormatString.Format(defUNION_FORMAT_STRING,
                                              defINTEGER64,
                                              pSg->m_omStrSignalName,
                                              pSg->m_unSignalLength);
                    }
                    // For < 32 bits use signed int
                    else
                    {
                        // Check for the Sign of the signal
                        if( pSg->m_bySignalType == CHAR_INT )
                        {
                            omFormatString.Format(defUNION_FORMAT_STRING,
                                                  defSIGNED_INTEGER,
                                                  pSg->m_omStrSignalName,
                                                  pSg->m_unSignalLength);
                        }
                        else
                        {
                            omFormatString.Format(defUNION_FORMAT_STRING,
                                                  defUNSIGNED_INTEGER,
                                                  pSg->m_omStrSignalName,
                                                  pSg->m_unSignalLength);
                        }
                    }
                    omStrArraySigName.Add(omFormatString);
                    punLength[i + unUnused] = pSg->m_unSignalLength;
                    // check if there is some unsed in between two signal
                    // defined
                    if( ( (punStartBit[i] + punLength[i + unUnused] ) <
                            ( punStartBit[i + 1] ) )
                            && (unSigCount != i + 1 )
                      )
                    {
                        punLength[i + unUnused + 1]  = punStartBit[i + 1];
                        punLength[i + unUnused + 1] -= punStartBit[i] ;
                        punLength[i + unUnused + 1] -= punLength[i + unUnused];
                        UINT unLengthTemp = punLength[i + unUnused + 1];
                        if(unLengthTemp > defBITS_IN_FOUR_BYTE)
                        {
                            unUnused++;
                            punLength[i + unUnused]  = defBITS_IN_FOUR_BYTE;

                            omFormatString.Format(defUNION_FORMAT_STRING,
                                                  defUNSIGNED_INT,
                                                  "",
                                                  defUINT_LENGTH);
                            omStrArraySigName.Add(omFormatString);
                            unCount++;
                        }
                        unUnused++;
                        punLength[i + unUnused]  =
                            unLengthTemp  -  ( unCount * defBITS_IN_FOUR_BYTE );

                        omFormatString.Format(defUNION_FORMAT_STRING,
                                              defUNSIGNED_INT,
                                              "",
                                              punLength[i + unUnused]);
                        omStrArraySigName.Add(omFormatString);
                        unCount = 0 ;
                    }
                    else if (unSigCount == i + 1)// check if unsed bits at the end

                    {

                        INT nLastBitLength = 0;
                        nLastBitLength = static_cast<INT>
                                         ((m_psMessages[nIndex].m_unMessageLength) *
                                          defBITS_IN_BYTE ) ;
                        nLastBitLength -= static_cast<INT> (punStartBit[i]);
                        nLastBitLength -= static_cast<INT> (punLength[i + unUnused]);
                        // If there is unsed bits at the end add bit wise definition
                        // without giving any name.
                        if(nLastBitLength >0 )
                        {
                            // If last unsed signal is more then 32 bit
                            // add two signal with no name and first
                            // having length of
                            if(nLastBitLength> defBITS_IN_FOUR_BYTE)
                            {
                                unUnused++;
                                punLength[i + unUnused] = defBITS_IN_FOUR_BYTE;

                                omFormatString.Format(defUNION_FORMAT_STRING,
                                                      defUNSIGNED_INT,
                                                      "",
                                                      defUINT_LENGTH);
                                omStrArraySigName.Add(omFormatString);
                                unCount++;
                            }
                            unUnused++;
                            punLength[i + unUnused] =
                                static_cast<UINT>((nLastBitLength) -
                                                  (defBITS_IN_FOUR_BYTE * unCount ) );

                            omFormatString.Format(defUNION_FORMAT_STRING,
                                                  defUNSIGNED_INT,
                                                  "",
                                                  punLength[i + unUnused]);
                            omStrArraySigName.Add(omFormatString);

                            unCount = 0;
                        }
                    }
                }

                pSg = pSg->m_psNextSignalList;
            }
            pSg = m_psMessages[nIndex].m_psSignals;
        }
        bReturn = TRUE;
    }
    return bReturn;
}
*/
static sSIGNALS* psGetSigPtr(UINT unStartBitSrc, sSIGNALS* psSigRoot)
{
    sSIGNALS* psTemSig = psSigRoot;
    UINT unStartBit = 0;
    while (psTemSig != nullptr)
    {
        unStartBit  = psTemSig->m_unStartByte - 1;
        unStartBit  = unStartBit * defBITS_IN_BYTE ;
        unStartBit += psTemSig->m_byStartBit ;
        if (unStartBitSrc == unStartBit)
        {
            break;
        }
        psTemSig = psTemSig->m_psNextSignalList;
    }
    return psTemSig;
}
BOOL CMsgSignal::bFormSigNameAndLength(UINT* punLength,
                                       UINT* punStartBit,
                                       CStringArray& omStrArraySigName,
                                       const INT nIndex)
{
    BOOL bReturn    = FALSE;
    UINT unSigCount = unSigCount = m_psMessages[nIndex].m_unNumberOfSignals;;
    sSIGNALS* pSg = m_psMessages[nIndex].m_psSignals;
    UINT unStartBit = 0;
    UINT unUnused   = 0;
    UINT unCount    = 0;

    //rmdup(punStartBit, unSigCount);
    UINT* p = (UINT*)std::unique(punStartBit, punStartBit+unSigCount);
    unSigCount = (p-punStartBit);
    if( pSg != nullptr && punLength != nullptr && punStartBit != nullptr )
    {
        //unSigCount = m_psMessages[nIndex].m_unNumberOfSignals;
        CString omFormatString;
        for(UINT i = 0 ; i < unSigCount ; i++ )
        {
            sSIGNALS* pSg = psGetSigPtr(punStartBit[i],
                                        m_psMessages[nIndex].m_psSignals);
            if(pSg != nullptr)
            {
                // if first signal is not defined from first bit in first
                // byte
                if(punStartBit[i] > 0  && i == 0)
                {
                    // If more then first 32 bits are not having
                    // any signal defined, add two signal without
                    // any name one with size 32 bit and other
                    // with size of rest of bits.
                    INT nTempBitLen = static_cast<INT> (punStartBit[i]);
                    while (nTempBitLen > defBITS_IN_FOUR_BYTE )
                    {
                        omFormatString.Format(defUNION_FORMAT_STRING,
                                              defUNSIGNED_INT,
                                              "",
                                              defUINT_LENGTH);
                        omStrArraySigName.Add(omFormatString);
                        nTempBitLen -= defBITS_IN_FOUR_BYTE;
                    }


                    omFormatString.Format(defUNION_FORMAT_STRING,
                                          defUNSIGNED_INT,
                                          "",
                                          nTempBitLen);
                    omStrArraySigName.Add(omFormatString);
                }

                // For signals > 32 bits
                if( pSg->m_unSignalLength > defBITS_IN_FOUR_BYTE)
                {
                    omFormatString.Format(defUNION_FORMAT_STRING,
                                          defINTEGER64,
                                          pSg->m_omStrSignalName,
                                          pSg->m_unSignalLength);
                }
                // For < 32 bits use signed int
                else
                {
                    // Check for the Sign of the signal
                    if( pSg->m_bySignalType == CHAR_INT )
                    {
                        omFormatString.Format(defUNION_FORMAT_STRING,
                                              defSIGNED_INTEGER,
                                              pSg->m_omStrSignalName,
                                              pSg->m_unSignalLength);
                    }
                    else
                    {
                        omFormatString.Format(defUNION_FORMAT_STRING,
                                              defUNSIGNED_INTEGER,
                                              pSg->m_omStrSignalName,
                                              pSg->m_unSignalLength);
                    }
                }
                omStrArraySigName.Add(omFormatString);
                // check if there is some unsed in between two signal
                // defined

                if( ( (punStartBit[i] + pSg->m_unSignalLength) <
                        ( punStartBit[i + 1] ) )
                        && (unSigCount != i + 1 )
                  )
                {
                    INT nLengthTemp = static_cast<INT>(punStartBit[i + 1])
                                      - static_cast<INT>(punStartBit[i] + pSg->m_unSignalLength);
                    while (nLengthTemp > defBITS_IN_FOUR_BYTE)
                    {
                        omFormatString.Format(defUNION_FORMAT_STRING,
                                              defUNSIGNED_INT,
                                              "",
                                              defUINT_LENGTH);
                        omStrArraySigName.Add(omFormatString);
                        nLengthTemp -= defBITS_IN_FOUR_BYTE;
                    }
                    omFormatString.Format(defUNION_FORMAT_STRING,
                                          defUNSIGNED_INT,
                                          "",
                                          nLengthTemp);
                    omStrArraySigName.Add(omFormatString);
                }
                else if (i == (unSigCount -1))// if last signal, check if unsed bits at the end

                {

                    INT nLastBitLength = 0;
                    nLastBitLength = static_cast<INT>
                                     ((m_psMessages[nIndex].m_unMessageLength) *
                                      defBITS_IN_BYTE ) ;
                    nLastBitLength -= static_cast<INT> (punStartBit[i]);
                    nLastBitLength -= static_cast<INT> (pSg->m_unSignalLength);
                    // If there is unsed bits at the end add bit wise definition
                    // without giving any name.
                    if(nLastBitLength >0 )
                    {
                        // If last unsed signal is more then 32 bit
                        // add two signal with no name and first
                        // having length of
                        INT nTempLastBitLen = nLastBitLength;
                        while (nTempLastBitLen > defBITS_IN_FOUR_BYTE)
                        {
                            omFormatString.Format(defUNION_FORMAT_STRING,
                                                  defUNSIGNED_INT,
                                                  "",
                                                  defUINT_LENGTH);
                            omStrArraySigName.Add(omFormatString);
                            nTempLastBitLen -= defBITS_IN_FOUR_BYTE;
                        }
                        omFormatString.Format(defUNION_FORMAT_STRING,
                                              defUNSIGNED_INT,
                                              "",
                                              nTempLastBitLen);
                        omStrArraySigName.Add(omFormatString);
                    }
                }
            }
        }
        bReturn = TRUE;
    }
    return bReturn;
}

/******************************************************************************
  Function Name    :  bFormSigNameAndLengthJ1939

  Input(s)         :  UINT *punLength : pointer to array of length of signals
                      const UINT *punStartBit: array of start bits of signal
                      CStringArray &omStrArraySigName : reference to
                                                        CStringArray
                      const INT nIndex : message index
  Output           :  TRUE/FALSE : Return value
                      UINT *punLength : Array of computed value of length of
                                        signals
                      CStringArray &omStrArraySigName : signal name for bitwise
                                                        structure
  Functionality    :  This function will form the signal name and its length for
                      the definition of bitwise structure. The punStartBit and
                      nIndex are the input parameter. punLength and
                      omStrArraySigName are the input and output parameter.The
                      signal name and its length is computed based on the
                      message index passed as nIndex. Function returns TRUE
                      is the computation is successfull.
  Member of        :  CMsgSignal
  Friend of        :      -

  Author(s)        :  Amitesh Bharti
  Date Created     :  12.03.2003
  Modifications    :  Amitesh Bharti, 22.04.2003, review comments incorporated.
                      Raja N 12.02.2004
                      Modified to include data type along with signal name
******************************************************************************/
BOOL CMsgSignal::bFormSigNameAndLengthJ1939(const UINT* punStartBit,
        CStringArray& omStrArraySigName,
        const INT nIndex)
{
    BOOL bReturn    = FALSE;

    if(punStartBit != nullptr )
    {
        UINT unSigCount = m_psMessages[nIndex].m_unNumberOfSignals;
        CString omFormatString;
        for(UINT i = 0 ; i < unSigCount ; i++ )
        {
            sSIGNALS* pSg = psGetSigPtr(punStartBit[i],
                                        m_psMessages[nIndex].m_psSignals);
            if(pSg != nullptr)
            {
                // if first signal is not defined from first bit in first
                // byte
                if(punStartBit[i] > 0  && i == 0)
                {
                    // If more then first 32 bits are not having
                    // any signal defined, add two signal without
                    // any name one with size 32 bit and other
                    // with size of rest of bits.
                    INT nTempBitLen = static_cast<INT> (punStartBit[i]);
                    while (nTempBitLen > defBITS_IN_FOUR_BYTE )
                    {
                        omFormatString.Format(defUNION_FORMAT_STRING,
                                              defUNSIGNED_INT,
                                              "",
                                              defUINT_LENGTH);
                        omStrArraySigName.Add(omFormatString);
                        nTempBitLen -= defBITS_IN_FOUR_BYTE;
                    }


                    omFormatString.Format(defUNION_FORMAT_STRING,
                                          defUNSIGNED_INT,
                                          "",
                                          nTempBitLen);
                    omStrArraySigName.Add(omFormatString);
                }

                // For signals > 32 bits
                if( pSg->m_unSignalLength > defBITS_IN_FOUR_BYTE)
                {
                    omFormatString.Format(defUNION_FORMAT_STRING,
                                          defINTEGER64,
                                          pSg->m_omStrSignalName,
                                          pSg->m_unSignalLength);
                }
                // For < 32 bits use signed int
                else
                {
                    // Check for the Sign of the signal
                    if( pSg->m_bySignalType == CHAR_INT )
                    {
                        omFormatString.Format(defUNION_FORMAT_STRING,
                                              defSIGNED_INTEGER,
                                              pSg->m_omStrSignalName,
                                              pSg->m_unSignalLength);
                    }
                    else
                    {
                        omFormatString.Format(defUNION_FORMAT_STRING,
                                              defUNSIGNED_INTEGER,
                                              pSg->m_omStrSignalName,
                                              pSg->m_unSignalLength);
                    }
                }


                // Check if already the same signal is added in the list. Might be the scenario if more than one signal has same start bit index
                bool bSignalExists = false;
                if (omStrArraySigName.GetSize() > 0 )
                {
                    for (int nIndex = 0; nIndex < omStrArraySigName.GetSize(); nIndex++)
                    {
                        if (omStrArraySigName.GetAt(nIndex) == omFormatString)
                        {
                            bSignalExists = true;
                        }
                    }
                }

                // IF the signal is already added in the list, then ignore adding it again
                if (!bSignalExists)
                {
                    omStrArraySigName.Add(omFormatString);
                }

                // check if there is some unsed in between two signal
                // defined

                if( ( (punStartBit[i] + pSg->m_unSignalLength) <
                        ( punStartBit[i + 1] ) )
                        && (unSigCount != i + 1 )
                  )
                {
                    INT nLengthTemp = static_cast<INT>(punStartBit[i + 1])
                                      - static_cast<INT>(punStartBit[i] + pSg->m_unSignalLength);
                    while (nLengthTemp > defBITS_IN_FOUR_BYTE)
                    {
                        omFormatString.Format(defUNION_FORMAT_STRING,
                                              defUNSIGNED_INT,
                                              "",
                                              defUINT_LENGTH);
                        omStrArraySigName.Add(omFormatString);
                        nLengthTemp -= defBITS_IN_FOUR_BYTE;
                    }
                    omFormatString.Format(defUNION_FORMAT_STRING,
                                          defUNSIGNED_INT,
                                          "",
                                          nLengthTemp);
                    omStrArraySigName.Add(omFormatString);
                }
                else if (i == (unSigCount -1))// if last signal, check if unsed bits at the end

                {
                    INT nLastBitLength = 0;
                    nLastBitLength = static_cast<INT>
                                     ((m_psMessages[nIndex].m_unMessageLength) *
                                      defBITS_IN_BYTE ) ;
                    nLastBitLength -= static_cast<INT> (punStartBit[i]);
                    nLastBitLength -= static_cast<INT> (pSg->m_unSignalLength);
                    // If there is unsed bits at the end add bit wise definition
                    // without giving any name.
                    if(nLastBitLength >0 )
                    {
                        // If last unsed signal is more then 32 bit
                        // add two signal with no name and first
                        // having length of
                        INT nTempLastBitLen = nLastBitLength;
                        while (nTempLastBitLen > defBITS_IN_FOUR_BYTE)
                        {
                            omFormatString.Format(defUNION_FORMAT_STRING,
                                                  defUNSIGNED_INT,
                                                  "",
                                                  defUINT_LENGTH);
                            omStrArraySigName.Add(omFormatString);
                            nTempLastBitLen -= defBITS_IN_FOUR_BYTE;
                        }
                        omFormatString.Format(defUNION_FORMAT_STRING,
                                              defUNSIGNED_INT,
                                              "",
                                              nTempLastBitLen);
                        omStrArraySigName.Add(omFormatString);
                    }
                }
            }
        }
        bReturn = TRUE;
    }
    return bReturn;
}

/******************************************************************************
  Function Name    :  vResetMsgMapContent
  Input(s)         :  -
  Output           :  void
  Functionality    :  Reset the content of DB message map
  Member of        :  CMsgSignal
  Friend of        :      -
  Author(s)        :  Anish Kumar
  Date Created     :  11-12-2006
  Modifications    :
******************************************************************************/
void CMsgSignal::vResetMsgMapContent()
{
    //Remove all the elements and create fresh map
    m_omMsgDetailsIDMap.RemoveAll();
    if(m_psDatbaseStructList != nullptr)
    {
        sDBFileStruct* psTempDatbaseStructList = m_psDatbaseStructList;
        while(psTempDatbaseStructList != nullptr)
        {
            UINT unMessageCount = psTempDatbaseStructList->m_unMessageCount;
            sMESSAGE* psMessages = psTempDatbaseStructList->m_psMessages;
            for ( UINT nMsgIndex = 0; nMsgIndex < unMessageCount; nMsgIndex++ )
            {
                if ( &psMessages[nMsgIndex] != nullptr )
                {
                    m_omMsgDetailsIDMap.SetAt(psMessages[nMsgIndex].m_unMessageCode,
                                              &psMessages[nMsgIndex]);
                }
            }
            psTempDatbaseStructList = psTempDatbaseStructList->m_psNextDBFileStruct;
        }
    }
}
void CMsgSignal::vSetTraceWndObj(CUIThread* pUIThread)
{
    m_pUIThread = pUIThread;
}
void CMsgSignal::vWriteTextToTrace()
{
    //if (m_pUIThread != nullptr)
    {
        /*m_pUIThread->PostThreadMessage(WM_WRITE_TO_TRACE, 0, (LPARAM)s_acTraceStr);*/
        theApp.bWriteIntoTraceWnd(s_acTraceStr);
    }
}
BOOL CMsgSignal::bInsertBusSpecStructures(CStdioFile& omHeaderFile,
        CString& omStrcommandLine,
        CStringArray& omStrArraySigName,
        sMESSAGE* pMsg)
{
    CString omStrSigName     = "";
    //    INT nIndex           = 0;
    CString omStrdelimiter   = "";

    /* Add New lines */
    omHeaderFile.WriteString(NEW_LINE);
    omHeaderFile.WriteString(NEW_LINE);

    /* Include guards for each message added */
    omStrcommandLine.Format(defMSG_GUARD_CHECK,  pMsg->m_omStrMessageName );
    omHeaderFile.WriteString(omStrcommandLine);

    omStrcommandLine.Format(defMSG_GUARD_DEFINE, pMsg->m_omStrMessageName );
    omHeaderFile.WriteString(omStrcommandLine);

    switch (m_sDbParams.m_eBus)
    {
        case CAN:
        {
            omStrcommandLine = defUNION;
            omHeaderFile.WriteString(omStrcommandLine);

            omHeaderFile.WriteString(defDATA_BYTE_ARRAY);
            omHeaderFile.WriteString(defDATA_WORD_ARRAY);
            omHeaderFile.WriteString(defDATA_LONG_ARRAY);

            omStrcommandLine = defSTART_OF_STRUCT_DEF;

            // Write the Structure Header
            omHeaderFile.WriteString(omStrcommandLine);

            UINT unCountSig = omStrArraySigName.GetSize();

            for(UINT i = 0 ; i < unCountSig; i++ )
            {
                // Copy Structure Members
                omHeaderFile.WriteString(omStrArraySigName.GetAt(i));
            }
            // Remove all signal name
            omStrArraySigName.RemoveAll();

            omStrcommandLine.Format(defEND_OF_UNION_DEF,
                                    pMsg->m_omStrMessageName );

            omHeaderFile.WriteString(omStrcommandLine);

            omStrcommandLine = defTYPEDEF_STRUCT;
            omHeaderFile.WriteString(omStrcommandLine);

            omStrcommandLine = defDATATYPE_MSGID;
            omHeaderFile.WriteString(omStrcommandLine);

            omStrcommandLine = defDATATYPE_EXTD;
            omHeaderFile.WriteString(omStrcommandLine);

            omStrcommandLine = defDATATYPE_RTR;
            omHeaderFile.WriteString(omStrcommandLine);

            omStrcommandLine = defDATATYPE_DLC;
            omHeaderFile.WriteString(omStrcommandLine);

            //Adding the channel number to the message structures.
            omStrcommandLine = defDATATYPE_CHANNEL_NUM ;
            omHeaderFile.WriteString( omStrcommandLine );

            omStrcommandLine.Format(defDATATYPE_MSG_DATA,
                                    pMsg->m_omStrMessageName +
                                    defUNDERSCORE, defSIGNALMEMBER);

            omHeaderFile.WriteString(omStrcommandLine);

            //omHeaderFile.WriteString(defDATATYPE_CANFD);

            omHeaderFile.WriteString(defDATATYPE_TIMESTAMP);

            omStrcommandLine = defDATATYPE_FD_FLAG;
            omHeaderFile.WriteString(omStrcommandLine);

            omStrcommandLine.Format(defEND_OF_STRUCT_DEF,
                                    pMsg->m_omStrMessageName );
            omHeaderFile.WriteString(omStrcommandLine);
        }
        break;
        case J1939:
        {
            omStrcommandLine = defUNION;
            omHeaderFile.WriteString(omStrcommandLine);

            CString omDataBytes;
            omDataBytes.Format(defDATA_BYTE_ARRAY_J1939, pMsg->m_unMessageLength);
            omHeaderFile.WriteString(omDataBytes);

            omStrcommandLine = defSTART_OF_STRUCT_DEF;

            // Write the Structure Header
            omHeaderFile.WriteString(omStrcommandLine);

            UINT unCountSig = omStrArraySigName.GetSize();

            for(UINT i = 0 ; i < unCountSig; i++ )
            {
                // Copy Structure Members
                omHeaderFile.WriteString(omStrArraySigName.GetAt(i));
            }
            // Remove all signal name
            omStrArraySigName.RemoveAll();

            omStrcommandLine.Format(defEND_OF_UNION_DEF,
                                    pMsg->m_omStrMessageName );

            omHeaderFile.WriteString(omStrcommandLine);

            omStrcommandLine = defTYPEDEF_STRUCT;
            omHeaderFile.WriteString(omStrcommandLine);

            omStrcommandLine = defMSGPROPS_J1939;
            omHeaderFile.WriteString(omStrcommandLine);

            omStrcommandLine = defDLCJ1939;
            omHeaderFile.WriteString(omStrcommandLine);

            omStrcommandLine.Format(defDATATYPE_MSG_DATA,
                                    pMsg->m_omStrMessageName +
                                    defUNDERSCORE+"*", defSIGNALMEMBER);
            omHeaderFile.WriteString(omStrcommandLine);

            omStrcommandLine.Format(defEND_OF_STRUCT_DEF,
                                    pMsg->m_omStrMessageName );
            omHeaderFile.WriteString(omStrcommandLine);
        }
        break;
    }

    /* Endif for guards of each message */
    omStrcommandLine.Format(defMSG_GUARD_END,  pMsg->m_omStrMessageName );
    omHeaderFile.WriteString(omStrcommandLine);

    return TRUE;
}

void CMsgSignal::vGetSignalValidRange(CString omStrMessageCode,
                                      CString omStrSgName,
                                      CString&   omMinValue, CString& omMaxValue)
{
    BOOL bReturnValue = FALSE;

    if ( !omStrMessageCode.IsEmpty() && !omStrSgName.IsEmpty())
    {
        UINT unMsgID = strtol(omStrMessageCode,nullptr,16);
        // Get message pointer
        sMESSAGE* pMsg = psGetMessagePointerInactive( unMsgID );

        if ( pMsg != nullptr)
        {
            // Get signal pointer
            sSIGNALS* p_Sg = pMsg->m_psSignals;
            // Get description
            while ( p_Sg != nullptr)
            {
                if ( p_Sg->m_omStrSignalName == omStrSgName )
                {
                    if(p_Sg->m_bySignalType == CHAR_INT) // signed type
                    {
                        //store valid sinal ranges
                        omMinValue.Format(defFORMAT_INT64_DECIMAL,p_Sg->m_SignalMinValue.n64Value);
                        omMaxValue.Format(defFORMAT_INT64_DECIMAL,p_Sg->m_SignalMaxValue.n64Value);
                    }
                    else
                    {
                        omMinValue.Format(defFORMAT_UINT64_DEC,p_Sg->m_SignalMinValue.un64Value);
                        omMaxValue.Format(defFORMAT_UINT64_DEC,p_Sg->m_SignalMaxValue.un64Value);
                    }
                    break;
                }
                else
                {
                    p_Sg = p_Sg->m_psNextSignalList;
                }
            }
        }
    }
}

//CAPL_DB_NAME_CHANGE
BOOL CMsgSignal::bFillDbStructure(CMsgNameMsgCodeListDataBase& odMsgNameMsgCodeListDB)
{
    if(m_psDatbaseStructList != nullptr)
    {
        //get the database list and message structure
        sDBFileStruct* psTempDatbaseStructList = m_psDatbaseStructList;
        odMsgNameMsgCodeListDB.RemoveAll();
        //loop through the linked list to get all the DB name ans details
        while(psTempDatbaseStructList != nullptr)
        {
            SDB_NAME_MSG                    osDbNameMess;                   //DB name and details
            UINT    unMessageCount = psTempDatbaseStructList->m_unMessageCount;
            sMESSAGE* psMessages = psTempDatbaseStructList->m_psMessages;   //Message info

            for ( UINT nMsgIndex = 0; nMsgIndex < unMessageCount; nMsgIndex++ )
            {
                if ( &psMessages[nMsgIndex] != nullptr )
                {
                    SMSG_NAME_CODE sMsgNameCode;                        //structure to store mess info in output list
                    sMsgNameCode.m_omMsgName = psMessages[nMsgIndex].m_omStrMessageName;
                    sMsgNameCode.m_dwMsgCode = (DWORD)psMessages[nMsgIndex].m_unMessageCode;
                    sMsgNameCode.m_unMsgLen  = psMessages[nMsgIndex].m_unMessageLength;
                    sSIGNALS* psSignal = psMessages[nMsgIndex].m_psSignals;
                    while (psSignal != nullptr)                            //loop through signal pointer of message
                    {
                        //and get the details
                        sMsgNameCode.m_omSignalNames.AddTail(psSignal->m_omStrSignalName);
                        psSignal = psSignal->m_psNextSignalList;
                    }
                    osDbNameMess.m_oMsgNameMsgCodeList.AddTail(sMsgNameCode);
                }
            }
            osDbNameMess.m_omDbName = psTempDatbaseStructList->m_omStrDatabasePath;
            odMsgNameMsgCodeListDB.AddTail(osDbNameMess);                   //add structure to output list
            psTempDatbaseStructList = psTempDatbaseStructList->m_psNextDBFileStruct; //get nxt DB
        }
    }
    else        //if the last DB is removed then m_psDatbaseStructList will be nullptr, clear DB list as well
    {
        if(odMsgNameMsgCodeListDB.GetCount() != 0)
        {
            odMsgNameMsgCodeListDB.RemoveAll();
        }
    }
    return TRUE;
}
/**
* \brief         API to get the alias to CFrameMap list
* \req           RS_FLX_03 - Importing of Fibex file
* \param[in]     string containing the path for FIBEX File
* \return        returns S_OK if successful, else S_FALSE
* \authors       Arunkumar Karri
* \date          16.04.2013 Created
*/

CFrameMap&  CMsgSignal::GetFlexRayFrameMap()
{
    return m_ouFrameDataSet;
}

int CMsgSignal::nGetDeviceConfig(ABS_DEVICE_CONFIG& ouDeviceConfig)
{
    //assigning ECU spec configuration
    if(m_AbsFibexContainer.m_omProject.m_omID == "" )
    {
        return FALSE;
    }
    POSITION    pos = m_AbsFibexContainer.m_omElement.m_omECUList.GetHeadPosition();
    ABS_ECU& oECU = m_AbsFibexContainer.m_omElement.m_omECUList.GetAt(pos);
    ABS_CONTROLLER oCtrlr = oECU.m_odControllerList.GetAt(pos);
    ouDeviceConfig.m_ouFlxSpecCntlr = oCtrlr.m_sFlexraySpecControllerData;

    //assigning cluster configuration
    ABS_CLUSTER& oCluster = m_AbsFibexContainer.m_omElement.m_omClusterList.GetAt(pos);
    ouDeviceConfig.m_ouFlxClusterConfig = *oCluster.m_ouBusInfo.m_pouFlexRayCluster;
    return TRUE;
}

/**
 * @brief API to load the LDF file
 * @req TODO - Importing of LDF file
 * @param[in] strDBFile string containing the path for LDF File
 * @return returns FCLASS_SUCCESS if successful, VERSION_NOT_SUPPORTED for Invalid File, FCLASS_FAILURE for other failures
 */
HRESULT CMsgSignal::hLoadLdfFile(std::string strDBFile, std::list<Cluster>& ouClusterList, std::list<LinChannelParam>& ouLinParams)
{
    //Cluster ouCluster;
    CHANNEL_CONFIG ouCluster;
    int nRet = nParseLDFFile(strDBFile, ouCluster);
    if ( nRet == S_OK )
    {
        if ( ouCluster.m_ouLinParams.m_nBaudRate < 200 || ouCluster.m_ouLinParams.m_nBaudRate > 30000 )
        {
            return FCLASS_FAILURE;
        }
        else
        {
            if(strDBFile.empty() == false)
            {
                ouCluster.m_ouClusterInfo.bWriteDBHeader(strDBFile.c_str(), LIN);
            }
            ouClusterList.push_back(ouCluster.m_ouClusterInfo);
            ouLinParams.push_back(ouCluster.m_ouLinParams);
            return FCLASS_SUCCESS;
        }
    }
    return FCLASS_FAILURE;
}

/**
* \brief         API to load the FIBEX file and fill the respective CFrameMap list
* \req           RS_FLX_03 - Importing of Fibex file
* \param[in]     string containing the path for FIBEX File
* \return        returns FCLASS_SUCCESS if successful, VERSION_NOT_SUPPORTED for Invalid File,
                 FCLASS_FAILURE for other failures
* \authors       Arunkumar Karri
* \authors       Prathiba Bharathi
* \date          16.04.2013 Created
*/
HRESULT CMsgSignal::hLoadFibexDBFile(CString strFIBEXFile, std::list<Cluster>& ouClusterList)
{
    //return hSimulateFibexCluster(strFIBEXFile, ouClusterList);

    try
    {
        std::map<std::string, Cluster> lstCluster;

        CPARSER_FIBEX objFibexParser;

        if(strFIBEXFile.IsEmpty() == TRUE)
        {
            return S_FALSE;
        }

        int nReturnVal =  objFibexParser.LoadFibexFile_Generic(strFIBEXFile.GetBuffer(MAX_PATH), lstCluster);
        //
        objFibexParser.Initialize();
        if (nReturnVal == FCLASS_SUCCESS )
        {
            std::map<std::string, Cluster>::iterator itrClusterMap = lstCluster.begin();

            while(itrClusterMap != lstCluster.end())
            {
                ouClusterList.push_back( itrClusterMap->second );

                itrClusterMap++;
            }
            lstCluster.clear();
        }
        else
        {
            return nReturnVal;
        }
    }
    catch(...)
    {
        return FCLASS_FAILURE;
    }

    return FCLASS_SUCCESS;
}

HRESULT CMsgSignal::hSimulateFibexCluster(CString strFIBEXFile, std::list<Cluster>& ouClusterList)
{
    HRESULT hr = S_OK;
    if ( strFIBEXFile == "" )
    {
        return hr;
    }
    Cluster ouCluster;
    int nStartIndex = strFIBEXFile.ReverseFind('\\');
    CString strName ="Unknown";

    if ( nStartIndex >= 0);
    {
        int nEndIndex = strFIBEXFile.Find(".", nStartIndex);
        if ( nEndIndex >= nStartIndex+1)
        {
            strName = strFIBEXFile.Mid(nStartIndex+1, nEndIndex - nStartIndex-1 );
        }
    }

    hSimulateFillClusterInfo(strName, ouCluster);
    ouClusterList.clear();
    ouClusterList.push_back(ouCluster);


    return hr;
}

HRESULT CMsgSignal::hSimulateFillClusterInfo(CString /* strName */, Cluster& /* ouCluster */)
{
    HRESULT hr = S_OK;

    return hr;
}

HRESULT CMsgSignal::hSimulateFillClusterInfo(ABS_FLEXRAY_SPEC_CNTLR& ouController)
{
    HRESULT hr = S_OK;

    ouController.m_shMaxDynPayloadLgt           =  -1;
    ouController.m_shClusterDriftDamping            =  2;
    ouController.m_nDecodingCorr                    =  48;
    ouController.m_nListenTimeOut               =  401202;
    ouController.m_shMaxDrift                   =  601;
    ouController.m_shExternOffsetCorr           =  0;
    ouController.m_shExternRateCorr             =  0;
    ouController.m_shLatestTx                   =  249;
    ouController.m_nMicroPreCycle               =  200000;
    ouController.m_shOffsetCorrOut              =  127;
    ouController.m_shRateCorrOut                    =  601;
    ouController.m_shSamplePerMicrotick         =  -1;
    ouController.m_shDelayCompensationA         =  1;
    ouController.m_shDelayCompensationB         =  1;
    ouController.m_shWakeUpPattern              =  33;
    ouController.m_bAllowHaltDewToClock         =  1;
    ouController.m_shAllowPassiveToActive       =  1;
    ouController.m_shAcceptedStartUpRange       =  220;
    ouController.m_shMacroInitialOffsetA            =  3;
    ouController.m_shMacroInitialOffsetB            =  3;
    ouController.m_shMicroInitialOffsetA            =  6;
    ouController.m_shMicroInitialOffsetB            =  6;
    ouController.m_bSingleSlotEnable                =  0;
    ouController.m_fMicrotick                   =  -1;
    ouController.m_fMicroPerMacroNom                =  -1;
    ouController.m_nChannels                        =  CHANNEL_A | CHANNEL_B;
    ouController.m_nKeySlotId                   =  3;
    ouController.m_nWakeUpChannel               =  CHANNEL_A;


    return hr;
}
void CMsgSignal::vClearFIBEXContainers()
{
    m_AbsFibexContainer.m_omElement.DoCleanup();
    m_AbsFibexContainer.m_omProcInfo.DoCleanup();
    m_AbsFibexContainer.m_omProject.DoCleanup();
    m_AbsFibexContainer.m_omProtocol.DoCleanup();
    m_AbsFibexContainer.m_omRequirements.DoCleanup();

    m_ouFrameDataSet.RemoveAll(); // Clear all filled dataset
}

//void CMsgSignal::vGetSIgnalDetails( CSignalDefArray& ouSignalDefArray, AbsCSigInstanceList& odSignalInstList)
//{
//    POSITION pos = odSignalInstList.GetHeadPosition();
//    while (pos != nullptr)
//    {
//        CSignalDef ouTempSignalDef; // Temporary placeholder for Signal details
//        ABS_SIGNAL_INSTANCE& ouTempSigInst = odSignalInstList.GetNext(pos);
//
//        ouTempSignalDef.m_unStartbit = ouTempSigInst.m_uStartPos.m_sSigStartPos.m_ushBIT_POSITION;
//        ouTempSignalDef.m_bIsBigEndian = ouTempSigInst.m_uStartPos.m_sSigStartPos.m_bIS_HIGH_LOW_BYTE_ORDER;
//
//        POSITION pos1 = m_AbsFibexContainer.m_omElement.m_omSignalList.GetHeadPosition();
//        while (pos1 != nullptr)
//        {
//            ABS_SIGNAL& ouTempAbsSignal = m_AbsFibexContainer.m_omElement.m_omSignalList.GetNext(pos1);
//            if (ouTempSigInst.m_omSigRef == ouTempAbsSignal.m_omSignalID)
//            {
//                ouTempSignalDef.m_omNameSignal = ouTempAbsSignal.m_omSignalName;
//                POSITION pos2 = m_AbsFibexContainer.m_omProcInfo.m_omCodingList.GetHeadPosition();
//                while (pos2 != nullptr)
//                {
//                    ABS_CODING& ouTempAbsCoding = m_AbsFibexContainer.m_omProcInfo.m_omCodingList.GetNext(pos2);
//                    if (ouTempAbsSignal.m_omCoding == ouTempAbsCoding.m_omCodingID)
//                    {
//                        ouTempSignalDef.m_unLength = ouTempAbsCoding.m_sCodedType.m_sLength.m_unLength;
//                        POSITION pos3 = ouTempAbsCoding.m_odCompuMethodList.GetHeadPosition();
//                        while (pos3 != nullptr)
//                        {
//                            ABS_COMPU_METHOD& tempAbsCompuMethod = ouTempAbsCoding.m_odCompuMethodList.GetNext(pos3);
//                            POSITION pos4 = tempAbsCompuMethod.m_odInternalConstrs.GetHeadPosition();
//                            while (pos4 != nullptr)
//                            {
//                                SIG_INT_CONSTRAINT_EX tempConsts;
//                                ABS_CONSTRS tempInterConstr = tempAbsCompuMethod.m_odInternalConstrs.GetNext(pos4);
//                                tempConsts.m_eValid = eGetRangeValid(tempInterConstr.m_omValidity);
//                                tempConsts.m_sRange.m_dwLowerLimit = tempInterConstr.m_sLowerLimit.m_fValue;
//                                tempConsts.m_sRange.m_dwUpperLimit = tempInterConstr.m_sUpperLimit.m_fValue;
//                                ouTempSignalDef.m_ouSigConstrnt.Add(tempConsts);
//                            }
//
//                            pos4 = tempAbsCompuMethod.m_odPhysConstrs.GetHeadPosition();
//                            while (pos4 != nullptr)
//                            {
//                                SIG_INT_CONSTRAINT_EX tempConsts;
//                                ABS_CONSTRS tempPhylConstr = tempAbsCompuMethod.m_odInternalConstrs.GetNext(pos4);
//                                tempConsts.m_eValid = eGetRangeValid(tempPhylConstr.m_omValidity);
//                                tempConsts.m_sRange.m_dwLowerLimit = tempPhylConstr.m_sLowerLimit.m_fValue;
//                                tempConsts.m_sRange.m_dwUpperLimit = tempPhylConstr.m_sUpperLimit.m_fValue;
//                                ouTempSignalDef.m_ouSigConstrnt.Add(tempConsts);
//                            }
//
//                            CCompuMethodEx tempCompuMethod;
//                            tempCompuMethod.m_eCompuType = sCopyCompuType(tempAbsCompuMethod.m_omCategory);
//                            sCopyCompuMethod(tempCompuMethod, tempAbsCompuMethod.m_sCompu_Internal_To_Phys);
//                          //ouTempSignalDef.m_ouCompuMethods.SetSize(ouTempSignalDef.m_ouCompuMethods.GetSize() + 1);
//                            ouTempSignalDef.m_ouCompuMethods.Add(tempCompuMethod);
//
//
//                            pos4 = m_AbsFibexContainer.m_omProcInfo.m_ouUnitSpec.m_odUnitList.GetHeadPosition();
//                            while (pos4 != nullptr)
//                            {
//                                ABS_UNIT& tempAbsUnit = m_AbsFibexContainer.m_omProcInfo.m_ouUnitSpec.m_odUnitList.GetNext(pos4);
//                                if (tempAbsCompuMethod.m_omUnitRef == tempAbsUnit.m_omID)
//                                {
//                                    ouTempSignalDef.m_omUnit = tempAbsUnit.m_omDisplayName;
//                                    pos4 = nullptr;
//                                }
//                            }
//                        }
//                        pos2 = nullptr;
//                    }
//                }
//                pos1 = nullptr;
//            }
//        }
//        ouSignalDefArray.Add(ouTempSignalDef);
//    }
//}
//void CMsgSignal::sCopyCompuMethod(CCompuMethodEx& ouCompuMethodEx, ABS_COMPU_INTERNAL_TO_PHYS& ouCompuInToPhys)
//{
//    POSITION pos = ouCompuInToPhys.m_odCompuScaleList.GetHeadPosition();
//    COMPU_EXPRESSION_MSGSIG eCategory = ouCompuMethodEx.m_eCompuType;
//    COMPU_METHOD_EX& ouReturnCompuMethod = ouCompuMethodEx.m_uMethod;
//    switch (eCategory)
//    {
//        case IDENTICAL_ENUM:
//        {
//            while (pos != nullptr)
//            {
//                ABS_COMPU_SCALE& tempAbsCompuScale = ouCompuInToPhys.m_odCompuScaleList.GetNext(pos);
//                ouReturnCompuMethod.m_IdenticalCode.m_sRange.m_dwLowerLimit = tempAbsCompuScale.m_sLower.m_fValue;
//                ouReturnCompuMethod.m_IdenticalCode.m_sRange.m_dwLowerLimit = tempAbsCompuScale.m_sLower.m_fValue;
//                ouReturnCompuMethod.m_IdenticalCode.m_sRange.m_dwUpperLimit = tempAbsCompuScale.m_sUpper.m_fValue;
//            }
//            break;
//        }
//        case LINEAR_ENUM:
//        {
//            while (pos != nullptr)
//            {
//                ABS_COMPU_SCALE& tempAbsCompuScale = ouCompuInToPhys.m_odCompuScaleList.GetNext(pos);
//                ouReturnCompuMethod.m_LinearCode.m_sRange.m_dwLowerLimit = tempAbsCompuScale.m_sLower.m_fValue;
//                ouReturnCompuMethod.m_LinearCode.m_sRange.m_dwUpperLimit = tempAbsCompuScale.m_sUpper.m_fValue;
//                POSITION pos1 = tempAbsCompuScale.m_sCompuRationalCoeffs.m_faCompuDeno.GetHeadPosition();
//                if (pos1 != nullptr)
//                {
//                    ouReturnCompuMethod.m_LinearCode.m_dD0 = tempAbsCompuScale.m_sCompuRationalCoeffs.m_faCompuDeno.GetNext(pos1);
//                }
//                pos1 = tempAbsCompuScale.m_sCompuRationalCoeffs.m_faCompuNuma.GetHeadPosition();
//                if (pos1 != nullptr)
//                {
//                    ouReturnCompuMethod.m_LinearCode.m_dN0 = tempAbsCompuScale.m_sCompuRationalCoeffs.m_faCompuNuma.GetNext(pos1);
//                    float fCheck = tempAbsCompuScale.m_sCompuRationalCoeffs.m_faCompuNuma.GetAt(pos1);
//                    if (fCheck == 0.062525)
//                    {
//                        int z = 0;
//                    }
//                    ouReturnCompuMethod.m_LinearCode.m_dN1 = tempAbsCompuScale.m_sCompuRationalCoeffs.m_faCompuNuma.GetNext(pos1);
//                }
//            }
//            break;
//        }
//        case SCALE_LINEAR_ENUM:
//        {
//            while (pos != nullptr)
//            {
//                ABS_COMPU_SCALE& tempAbsCompuScale = ouCompuInToPhys.m_odCompuScaleList.GetNext(pos);
//
//              LINEAR_CODE_EX objLinearCode;
//
//              objLinearCode.m_sRange.m_dwLowerLimit = tempAbsCompuScale.m_sLower.m_fValue;
//                objLinearCode.m_sRange.m_dwUpperLimit = tempAbsCompuScale.m_sUpper.m_fValue;
//
//
//                POSITION pos1 = tempAbsCompuScale.m_sCompuRationalCoeffs.m_faCompuDeno.GetHeadPosition();
//                if (pos1 != nullptr)
//                {
//                   objLinearCode.m_dD0 = tempAbsCompuScale.m_sCompuRationalCoeffs.m_faCompuDeno.GetNext(pos1);
//                }
//
//                pos1 = tempAbsCompuScale.m_sCompuRationalCoeffs.m_faCompuNuma.GetHeadPosition();
//                if (pos1 != nullptr)
//                {
//                    objLinearCode.m_dN0 = tempAbsCompuScale.m_sCompuRationalCoeffs.m_faCompuNuma.GetNext(pos1);
//
//                   objLinearCode.m_dN1 = tempAbsCompuScale.m_sCompuRationalCoeffs.m_faCompuNuma.GetNext(pos1);
//                }
//
//              ouReturnCompuMethod.m_objScaleLinear.push_back(objLinearCode);
//
//                /*PSCALE_LINEAR_CODE_VAR_EX temp = ouReturnCompuMethod.m_pScaleLinear;
//                tempScaleLinear->m_pNextLinearType = temp;
//                ouReturnCompuMethod.m_pScaleLinear = tempScaleLinear;*/
//
//
//    //            PSCALE_LINEAR_CODE_VAR_EX tempScaleLinear = new SCALE_LINEAR_CODE_VAR_EX;
//    //            tempScaleLinear->m_pNextLinearType = nullptr;
//              ////LINEAR_CODE
//    //            tempScaleLinear->m_sLinearType.m_sRange.m_dwLowerLimit = tempAbsCompuScale.m_sLower.m_fValue;
//    //            tempScaleLinear->m_sLinearType.m_sRange.m_dwUpperLimit = tempAbsCompuScale.m_sUpper.m_fValue;
//
//
//    //            POSITION pos1 = tempAbsCompuScale.m_sCompuRationalCoeffs.m_faCompuDeno.GetHeadPosition();
//    //            if (pos1 != nullptr)
//    //            {
//    //                tempScaleLinear->m_sLinearType.m_dD0 = tempAbsCompuScale.m_sCompuRationalCoeffs.m_faCompuDeno.GetNext(pos1);
//    //            }
//
//    //            pos1 = tempAbsCompuScale.m_sCompuRationalCoeffs.m_faCompuNuma.GetHeadPosition();
//    //            if (pos1 != nullptr)
//    //            {
//    //                tempScaleLinear->m_sLinearType.m_dN0 = tempAbsCompuScale.m_sCompuRationalCoeffs.m_faCompuNuma.GetNext(pos1);
//
//    //                tempScaleLinear->m_sLinearType.m_dN1 = tempAbsCompuScale.m_sCompuRationalCoeffs.m_faCompuNuma.GetNext(pos1);
//    //            }
//    //            PSCALE_LINEAR_CODE_VAR_EX temp = ouReturnCompuMethod.m_pScaleLinear;
//    //            tempScaleLinear->m_pNextLinearType = temp;
//    //            ouReturnCompuMethod.m_pScaleLinear = tempScaleLinear;
//            }
//            break;
//        }
//        case TEXTTABLE_ENUM:
//        {
//            while (pos != nullptr)
//            {
//                ABS_COMPU_SCALE& tempAbsCompuScale = ouCompuInToPhys.m_odCompuScaleList.GetNext(pos);
//
//              TEXT_CODE_VAR_EX objTextCdeVar;
//
//              objTextCdeVar.m_sRange.m_dwLowerLimit = tempAbsCompuScale.m_sLower.m_fValue;
//              objTextCdeVar.m_sRange.m_dwUpperLimit = tempAbsCompuScale.m_sUpper.m_fValue;
//              strcpy(objTextCdeVar.m_aTextName,tempAbsCompuScale.m_sCompuConst.m_omVT.GetBuffer(MAX_PATH));
//
//              ouReturnCompuMethod.m_objTextCode.push_back(objTextCdeVar);
//               /* PTEXT_CODE_VAR_EX tempTextCode = new TEXT_CODE_VAR_EX;
//                tempTextCode->m_pNextTextCodeType = nullptr;
//
//                tempTextCode->m_sRange.m_dwLowerLimit = tempAbsCompuScale.m_sLower.m_fValue;
//                tempTextCode->m_sRange.m_dwUpperLimit = tempAbsCompuScale.m_sUpper.m_fValue;
//                strcpy(tempTextCode->m_aTextName,tempAbsCompuScale.m_sCompuConst.m_omVT.GetBuffer(MAX_PATH));
//
//                PTEXT_CODE_VAR_EX temp = ouReturnCompuMethod.m_pTextCode;
//                tempTextCode->m_pNextTextCodeType = temp;
//                ouReturnCompuMethod.m_pTextCode = tempTextCode;*/
//            }
//            break;
//        }
//        case TAB_NOINTP_ENUM:
//        {
//            while (pos != nullptr)
//            {
//                ABS_COMPU_SCALE& tempAbsCompuScale = ouCompuInToPhys.m_odCompuScaleList.GetNext(pos);
//
//              TAB_CODE_VAR_EX objTab;
//
//              objTab.m_sRange.m_dwLowerLimit = tempAbsCompuScale.m_sLower.m_fValue;
//              objTab.m_sRange.m_dwUpperLimit = tempAbsCompuScale.m_sUpper.m_fValue;
//                objTab.m_dPhysVal = tempAbsCompuScale.m_sCompuConst.m_fV;
//
//              ouReturnCompuMethod.m_objTabCode.push_back(objTab);
//
//                /*PTAB_CODE_VAR_EX tempTabCode = new TAB_CODE_VAR_EX;
//                tempTabCode->m_pNextTabCodeType = nullptr;
//
//                tempTabCode->m_sRange.m_dwLowerLimit = tempAbsCompuScale.m_sLower.m_fValue;
//                tempTabCode->m_sRange.m_dwUpperLimit = tempAbsCompuScale.m_sUpper.m_fValue;
//                tempTabCode->m_dPhysVal = tempAbsCompuScale.m_sCompuConst.m_fV;
//
//                PTAB_CODE_VAR_EX temp = ouReturnCompuMethod.m_pTabCode;
//                tempTabCode->m_pNextTabCodeType = temp;
//                ouReturnCompuMethod.m_pTabCode = tempTabCode;*/
//            }
//            break;
//        }
//        case FORMULA_ENUM:
//        {
//            while (pos != nullptr)
//            {
//                ABS_COMPU_SCALE& tempAbsCompuScale = ouCompuInToPhys.m_odCompuScaleList.GetNext(pos);
//                PFORMULA_CODE_VAR_EX tempFormula = new FORMULA_CODE_VAR_EX;
//                tempFormula->m_pFirstOperand = nullptr;
//
//                tempFormula->m_omFormula = tempAbsCompuScale.m_sCompuConst.m_omVT;
//                /*tempFormula->m_pFirstOperand = tempAbsCompuScale.m_sUpper.m_fValue;*/
//            }
//            break;
//        }
//    }
//}

COMPU_EXPRESSION_MSGSIG CMsgSignal::sCopyCompuType(CString ouCompuType)
{
    COMPU_EXPRESSION_MSGSIG sReturnCompuEx = IDENTICAL_ENUM;
    if (ouCompuType == "IDENTICAL")
    {
        sReturnCompuEx = IDENTICAL_ENUM;
    }
    else if (ouCompuType == "LINEAR")
    {
        sReturnCompuEx = LINEAR_ENUM;
    }
    else if (ouCompuType == "SCALE-LINEAR")
    {
        sReturnCompuEx = SCALE_LINEAR_ENUM;
    }
    else if (ouCompuType == "TEXTTABLE")
    {
        sReturnCompuEx = TEXTTABLE_ENUM;
    }
    else if (ouCompuType == "SCALE-LINEAR-TEXTTABLE")
    {
        sReturnCompuEx = SCALE_LINEAR_TEXTTABLE_ENUM;
    }
    else if (ouCompuType == "TAB-NOINTP")
    {
        sReturnCompuEx = TAB_NOINTP_ENUM;
    }
    else if (ouCompuType == "FORMULA")
    {
        sReturnCompuEx = FORMULA_ENUM;
    }

    return sReturnCompuEx;
}

RANGE_VALID CMsgSignal::eGetRangeValid(CString omValidity)
{
    RANGE_VALID eResult = VALID;

    if (omValidity == "NOT-VALID")
    {
        eResult = INVALID;
    }
    else if (omValidity == "NOT-AVAILABLE")
    {
        eResult = NOT_AVAILABLE;
    }
    else if (omValidity ==  "VALID")
    {
        eResult = VALID;
    }

    return eResult;
}
