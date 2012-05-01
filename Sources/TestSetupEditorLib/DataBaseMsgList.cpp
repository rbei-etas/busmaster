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
 * \file      DataBaseMsgList.cpp
 * \author    Venkatanarayana makam
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "TestSetupEditorLib_stdafx.h"
#include "DataBaseMsgList.h"


/******************************************************************************
Function Name  :  CDataBaseMsgList
Input(s)       :  -
Output         :  -
Functionality  :  Constructor
Member of      :  CDataBaseMsgList
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CDataBaseMsgList::CDataBaseMsgList(void)
{
    m_unMessageCount = 0;
    m_psMessages = NULL;
}

/******************************************************************************
Function Name  :  ~CDataBaseMsgList
Input(s)       :  -
Output         :  -
Functionality  :  Destructor
Member of      :  CDataBaseMsgList
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
CDataBaseMsgList::~CDataBaseMsgList(void)
{
    bFreeMessageMemory();
}

/******************************************************************************
Function Name  :  bFillDataStructureFromDatabaseFile
Input(s)       :  CString strFileName - DataBase File Path
Output         :  BOOL
Functionality  :
Member of      :  CDataBaseMsgList
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
BOOL CDataBaseMsgList::bFillDataStructureFromDatabaseFile( CString strFileName)
{
    BOOL bReturnValue   = TRUE;
    BOOL bIsFileOpen    = FALSE;
    m_unMessageCount = 0;
    CHAR s_acTraceStr[1024];

    // validate the file
    if ( bValidateDatabaseFile(strFileName) )
    {
        // For File I/O
        CStdioFile o_File;
        TRY
        {
            // Open File
            bIsFileOpen = o_File.Open(
                strFileName, CFile::modeRead|CFile::typeText );

            if(bIsFileOpen != FALSE )
            {
                float fDBVerNum = 0;
                float fCurrDBVer = 0;
                CString sFirstLine  = STR_EMPTY;
                CString omDBVerLine  = STR_EMPTY;
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
                    fDBVerNum = (FLOAT)atof(omDbVerNo.GetBuffer(MAX_PATH));
                    fCurrDBVer = (FLOAT)atof(DATABASE_VERSION_NO);

                    if (fDBVerNum < fCurrDBVer)
                    {
                        strcpy_s(s_acTraceStr, DATABASE_MISMATCH);
                        //TODO::    vWriteTextToTrace();
                        //m_bIsDatabaseSaved = FALSE;
                    }

                    if (fDBVerNum > fCurrDBVer)
                    {
                        strcpy_s(s_acTraceStr, "DATABASE_CORRUPT");
                        //TODO::    vWriteTextToTrace();
                        return FALSE;
                    }
                }

                /* Reading of database version number ends */
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

                    // create message signal structure using no of message
                    if(bFreeMessageMemory() == TRUE)
                    {
                        m_unMessageCount = atoi( omstrNoOfMsgs );
                        m_psMessages = new sMESSAGE[m_unMessageCount];
                    }
                    else
                    {
                        return FALSE;
                    }

                    // Number of messages updated here

                    if ( m_psMessages != NULL )
                    {
                        // Initialise all signal stuff to NULL to
                        // make sure application will not crash
                        for ( UINT nCount = 0; nCount < m_unMessageCount; nCount++ )
                        {
                            m_psMessages[nCount].m_psSignals = NULL;
                        }

                        UINT unMsgCount = 0;

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
                                for ( UINT nCount = 0; nCount < 8; nCount++ )
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
                                        strcpy_s(s_acTraceStr, "Database file corrupted");
                                        //TODO::    vWriteTextToTrace();
                                        bReturnValue = FALSE;
                                        break;
                                    }

                                    // Find Msg Code
                                    nIndex = sMsgDet.Find( ',' );

                                    if (  nIndex != -1 )
                                    {
                                        strTmp = sMsgDet.Left(nIndex);
                                        // Msg Code
                                        m_psMessages[unMsgCount].m_unMessageCode = (UINT)atoi((const char*) strTmp );
                                        int len = sMsgDet.GetLength() - nIndex - 1;
                                        sMsgDet = sMsgDet.Right(len);
                                    }
                                    else
                                    {
                                        strcpy_s(s_acTraceStr, "Database file corrupted");
                                        //TODO::    vWriteTextToTrace();
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
                                        sMsgDet = sMsgDet.Right(sMsgDet.GetLength() - nIndex - 1);
                                    }
                                    else
                                    {
                                        strcpy_s(s_acTraceStr, "Database file corrupted");
                                        //TODO::    vWriteTextToTrace();
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
                                        strcpy_s(s_acTraceStr, "Database file corrupted");
                                        //TODO::    vWriteTextToTrace();
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
                                        strcpy_s(s_acTraceStr, "Database file corrupted");
                                        bReturnValue = FALSE;
                                        break;
                                    }

                                    // check that extracted substring is not empty
                                    if(sMsgDet.IsEmpty())
                                    {
                                        strcpy_s(s_acTraceStr, "Database file corrupted");
                                        //TODO::    vWriteTextToTrace();
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
                                        strcpy_s(s_acTraceStr, "Database file corrupted");
                                        //TODO::    vWriteTextToTrace();
                                        bReturnValue = FALSE;
                                        break;
                                    }

                                    // rajesh: 03-03-2003: modification end

                                    if ( m_psMessages[unMsgCount].m_unNumberOfSignals > 0)
                                    {
                                        sSIGNALS* pNext =
                                            m_psMessages[unMsgCount].m_psSignals = NULL;
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

                                                if (sTempSg == NULL)
                                                {
                                                    strcpy_s(s_acTraceStr, MSG_MEMORY_CONSTRAINT);
                                                    //TODO::    vWriteTextToTrace();
                                                    bReturnValue = FALSE;
                                                    break;
                                                }
                                                else
                                                {
                                                    sTempSg->m_psNextSignalList = NULL;
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
                                                            strcpy_s(s_acTraceStr, MSG_MEMORY_CONSTRAINT);
                                                            //TODO::    vWriteTextToTrace();
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
                                                            strcpy_s(s_acTraceStr, MSG_MEMORY_CONSTRAINT);
                                                            //TODO::    vWriteTextToTrace();
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
                                                            strcpy_s(s_acTraceStr, MSG_MEMORY_CONSTRAINT);
                                                            //TODO::    vWriteTextToTrace();
                                                            bReturnValue = FALSE;
                                                            break;
                                                        }

                                                        // Find and get signal start bit
                                                        nIndex = sMsgDet.Find( ',' );

                                                        if (  nIndex != -1 )
                                                        {
                                                            CString sTmp = sMsgDet.Left(nIndex);
                                                            //Type cast no problem since we are usinf one character only
                                                            sTempSg->m_byStartBit = (BYTE)atoi( (const char*) sTmp );
                                                            sMsgDet = sMsgDet.Right(sMsgDet.GetLength() - nIndex - 1);
                                                        }
                                                        else
                                                        {
                                                            strcpy_s(s_acTraceStr, MSG_MEMORY_CONSTRAINT);
                                                            //TODO::    vWriteTextToTrace();
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
                                                            strcpy_s(s_acTraceStr, MSG_MEMORY_CONSTRAINT);
                                                            //TODO::    vWriteTextToTrace();
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
                                                            strcpy_s(s_acTraceStr, MSG_MEMORY_CONSTRAINT);
                                                            //TODO::    vWriteTextToTrace();
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
                                                            strcpy_s(s_acTraceStr, MSG_MEMORY_CONSTRAINT);
                                                            //TODO::    vWriteTextToTrace();
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
                                                            strcpy_s(s_acTraceStr, MSG_MEMORY_CONSTRAINT);
                                                            //TODO::    vWriteTextToTrace();
                                                            bReturnValue = FALSE;
                                                            break;
                                                        }

                                                        // Find and get signal offset value
                                                        nIndex = sMsgDet.Find( ',' );

                                                        if (  nIndex != -1 )
                                                        {
                                                            CString sTmp = sMsgDet.Left(nIndex);
                                                            sTempSg->m_fSignalOffset = (float)atof(sTmp);
                                                            sMsgDet = sMsgDet.Right(sMsgDet.GetLength() - nIndex - 1);
                                                        }
                                                        else
                                                        {
                                                            strcpy_s(s_acTraceStr, MSG_MEMORY_CONSTRAINT);
                                                            //TODO::    vWriteTextToTrace();
                                                            bReturnValue = FALSE;
                                                            break;
                                                        }

                                                        // Find and get signal factor value
                                                        nIndex = sMsgDet.Find( ',' );

                                                        if (  nIndex != -1 )
                                                        {
                                                            CString sTmp = sMsgDet.Left(nIndex);
                                                            sTempSg->m_fSignalFactor = (float)atof(sTmp);
                                                            sMsgDet = sMsgDet.Right(sMsgDet.GetLength() - nIndex - 1);
                                                        }
                                                        else
                                                        {
                                                            strcpy_s(s_acTraceStr, MSG_MEMORY_CONSTRAINT);
                                                            //TODO::    vWriteTextToTrace();
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
                                                            MessageBox( NULL,"Database file corrupted", NULL, MB_ICONINFORMATION|MB_OK );
                                                            bReturnValue = FALSE;
                                                            break;
                                                        }*/
                                                        // sTempSg->m_omStrSignalUnit = sMsgDet;
                                                        // Update the signal matrix
                                                        //venkat::abySigMask not referanced
                                                        //BYTE abySigMask[DATA_LENGTH_MAX] = {0};
                                                        /*bCalcBitMaskForSig(abySigMask,
                                                                           DATA_LENGTH_MAX,
                                                                           sTempSg->m_unStartByte,
                                                                           sTempSg->m_byStartBit,
                                                                           sTempSg->m_unSignalLength,
                                                                           sTempSg->m_eFormat);
                                                        vUpdateSignalMatrix(abySigMask,
                                                            m_psMessages[unMsgCount].m_bySignalMatrix,
                                                            DATA_LENGTH_MAX,
                                                            SET);*/
                                                        CSignalDescVal* pomSgDescValNext = sTempSg->m_oSignalIDVal = NULL;

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
                                                                    pomSgDescVal->m_n64SignalVal = _atoi64( (const char*) sFirstLine );
                                                                    pomSgDescVal->m_pouNextSignalSignalDescVal = NULL;
                                                                }// end if ( finding for comma)
                                                                else
                                                                {
                                                                    strcpy_s(s_acTraceStr, MSG_MEMORY_CONSTRAINT);
                                                                    //TODO::    vWriteTextToTrace();
                                                                    bReturnValue = FALSE;
                                                                    break;
                                                                }

                                                                // Attach the signal descand value to the data structure.

                                                                if ( pomSgDescValNext != NULL)
                                                                {
                                                                    while ( pomSgDescValNext->m_pouNextSignalSignalDescVal != NULL )
                                                                    {
                                                                        pomSgDescValNext = pomSgDescValNext->m_pouNextSignalSignalDescVal;
                                                                    }

                                                                    pomSgDescValNext->m_pouNextSignalSignalDescVal = pomSgDescVal;
                                                                }
                                                                else
                                                                {
                                                                    pomSgDescValNext = sTempSg->m_oSignalIDVal = pomSgDescVal;
                                                                    pomSgDescValNext->m_pouNextSignalSignalDescVal = NULL;
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
                                                    if ( pNext != NULL )
                                                    {
                                                        while( pNext->m_psNextSignalList != NULL )
                                                        {
                                                            pNext = pNext->m_psNextSignalList;
                                                        }

                                                        pNext->m_psNextSignalList = sTempSg;
                                                    }
                                                    else
                                                    {
                                                        pNext = m_psMessages[unMsgCount].m_psSignals = sTempSg;
                                                        pNext->m_psNextSignalList = NULL;
                                                    }

                                                    unNumberofSignalsRead++;
                                                }// if (!=NULL)
                                            }// if signal found
                                        }

                                        if (bReturnValue == FALSE)
                                        {
                                            break;
                                        }
                                    }
                                    else // No signals defined.
                                    {
                                        m_psMessages[unMsgCount].m_psSignals = NULL;
                                    }

                                    // next message index
                                    unMsgCount++;
                                }
                            }// message not found
                        }// while read message
                    }
                    else
                    {
                        strcpy_s(s_acTraceStr, MSG_MEMORY_CONSTRAINT);
                        //TODO::    vWriteTextToTrace();
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
                CString omStrErrorMsg = _T("");
                omStrErrorMsg.Format("Error in opening %s database file",strFileName);
                strcpy_s(s_acTraceStr, MSG_MEMORY_CONSTRAINT);
                //TODO::    vWriteTextToTrace();
            }
        }
        CATCH_ALL (pomE)
        {
            if(pomE != NULL )
            {
                LPTSTR lpszError = _T("");
                // Get error
                pomE->GetErrorMessage( lpszError, defSIZE_OF_ERROR_BUFFER);
                strcpy_s(s_acTraceStr, lpszError);
                //TODO::    vWriteTextToTrace();
                pomE->Delete();
            }

            if (bIsFileOpen == TRUE)
            {
                o_File.Close();
            }
        }
        END_CATCH_ALL
    }

    return (bReturnValue);
}

/******************************************************************************
Function Name  :  bValidateDatabaseFile
Input(s)       :  CString strFileName - DataBase File Path
Output         :  BOOL
Functionality  :
Member of      :  CDataBaseMsgList
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
BOOL CDataBaseMsgList::bValidateDatabaseFile(CString strFileName)
{
    BOOL bReturnValue = FALSE;
    CHAR s_acTraceStr[1024];

    if ( strFileName.IsEmpty() )
    {
        strcpy_s(s_acTraceStr, "Database filename is empty.");
        //vWriteTextToTrace();
    }
    else
    {
        // Verify for ".dbf" extn
        int nIndex = strFileName.ReverseFind(PERIOD);

        if ( nIndex == -1 ) // not found
        {
            return FALSE;
        }
        else // found
        {
            CString sFileExtn = strFileName.Right( (strFileName.GetLength() - nIndex) - 1 );
            sFileExtn.MakeUpper();

            if ( sFileExtn != DATABASE_EXTN)
            {
                strcpy_s(s_acTraceStr, "Invalid database filename");
                return FALSE;
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

                        CString sFirstLine = STR_EMPTY;

                        // Read file line
                        o_File.ReadString( sFirstLine );

                        // First line of the file
                        if ( sFirstLine == DATABASE_HEADER ||sFirstLine == DATABASE_HEADER_NEW)
                        {
                            bReturnValue = TRUE;
                        }

                        // close File
                        o_File.Close();
                    }
                    CATCH_ALL (pomE)
                    {
                        if(pomE != NULL )
                        {
                            LPTSTR lpszError = _T("");
                            // Get error
                            pomE->GetErrorMessage( lpszError, defSIZE_OF_ERROR_BUFFER);
                            strcpy_s(s_acTraceStr, "lpszError");
                            //TODO:: vWriteTextToTrace();
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
Function Name  :  bIsValidMessageID
Input(s)       :  UINT dwMsgId - Message id
Output         :  BOOL
Functionality  :
Member of      :  CDataBaseMsgList
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
BOOL CDataBaseMsgList::bIsValidMessageID(UINT dwMsgId)
{
    BOOL isValid = FALSE;

    for(UINT i = 0; i < m_unMessageCount; i++)
    {
        if(m_psMessages[i].m_unMessageCode == dwMsgId)
        {
            isValid = TRUE;
            break;
        }
    }

    return isValid;
}

/******************************************************************************
Function Name  :  nFillMessageList
Input(s)       :  CComboBox& omDbMsgCombo
                  BOOL bEmptyString
Output         :  INT
Functionality  :  Fills The Combo Box With Message List
Member of      :  CDataBaseMsgList
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
INT CDataBaseMsgList::nFillMessageList(CComboBox& omDbMsgCombo, BOOL bEmptyString)
{
    for (UINT i = 0; i < m_unMessageCount; i++)
    {
        omDbMsgCombo.AddString(m_psMessages[i].m_omStrMessageName);
    }

    if(bEmptyString == TRUE)
    {
        omDbMsgCombo.AddString(defDELETE_MSG_SYMBOL);
    }

    return m_unMessageCount;
}

/******************************************************************************
Function Name  :  nFillMessageList
Input(s)       :  CStringArray& omstrEntryList
                  BOOL bEmptyString
Output         :  INT
Functionality  :  Fills The Message List Into String Array
Member of      :  CDataBaseMsgList
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
INT CDataBaseMsgList::nFillMessageList(CStringArray& omstrEntryList, BOOL bEmptyString)
{
    for (UINT i = 0; i < m_unMessageCount; i++)
    {
        omstrEntryList.Add(m_psMessages[i].m_omStrMessageName);
    }

    if(bEmptyString == TRUE)
    {
        omstrEntryList.InsertAt(0, defDELETE_MSG_SYMBOL);
    }

    return m_unMessageCount;
}

/******************************************************************************
Function Name  :  bFreeMessageMemory
Input(s)       :
Output         :  BOOL
Functionality  :  Delete the Allocated Memory
Member of      :  CDataBaseMsgList
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
BOOL CDataBaseMsgList::bFreeMessageMemory(void)
{
    if(m_psMessages != NULL)
    {
        for(UINT i = 0; i<m_unMessageCount; i++)
        {
            tagSMSGENTRY::vClearSignalList(m_psMessages[i].m_psSignals);
        }

        delete []m_psMessages;
    }

    m_psMessages = NULL;
    m_unMessageCount = 0;
    return TRUE;
}

/******************************************************************************
Function Name  :  unGetMessageID
Input(s)       :  CString omstrMsgName - Message Name
Output         :  UINT
Functionality  :  Retrives the Message ID
Member of      :  CDataBaseMsgList
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
UINT CDataBaseMsgList::unGetMessageID(CString omstrMsgName)
{
    if( m_psMessages == NULL )
    {
        //For W4 Removal
        return (UINT)-1;
    }

    for(UINT i = 0; i < m_unMessageCount; i++)
    {
        if(m_psMessages[i].m_omStrMessageName == omstrMsgName)
        {
            return m_psMessages[i].m_unMessageCode;
        }
    }

    //For W4 Removal
    return (UINT)ERR_WRONG_ID;
}

/******************************************************************************
Function Name  :  nGetMessageName
Input(s)       :  UINT unMsgId - Message ID
                  CString& omstrMsgName  - message Name
Output         :  INT
Functionality  :  Returns the Message Name
Member of      :  CDataBaseMsgList
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
INT CDataBaseMsgList::nGetMessageName(UINT unMsgId, CString& omstrMsgName)
{
    omstrMsgName = "";

    if( m_psMessages == NULL )
    {
        return ERR_INVALID_DATABASE;
    }

    for(UINT i = 0; i < m_unMessageCount; i++)
    {
        if(m_psMessages[i].m_unMessageCode == unMsgId)
        {
            omstrMsgName = m_psMessages[i].m_omStrMessageName;
            return S_OK;
        }
    }

    return ERR_WRONG_ID;
}

/******************************************************************************
Function Name  :  nGetSignalList
Input(s)       :  CString omstrMsgName, sSIGNALS* psSignals
Output         :  INT
Functionality  :  Return the Signal Information
Member of      :  CDataBaseMsgList
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
INT CDataBaseMsgList::nGetSignalList(CString omstrMsgName, sSIGNALS* psSignals)
{
    if( m_psMessages == NULL )
    {
        return ERR_INVALID_DATABASE;
    }

    psSignals = NULL;

    for(UINT i = 0; i < m_unMessageCount; i++)
    {
        if(m_psMessages[i].m_omStrMessageName == omstrMsgName)
        {
            psSignals = m_psMessages[i].m_psSignals;
            return m_psMessages[i].m_unNumberOfSignals;
        }
    }

    return ERR_WRONG_ID;
}

/******************************************************************************
Function Name  :  nGetMessageInfo
Input(s)       :  CString omstrMsgName
                  sMESSAGE& sMsg
Output         :  INT
Functionality  :  Return the Message Info
Member of      :  CDataBaseMsgList
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
INT CDataBaseMsgList::nGetMessageInfo(CString omstrMsgName, sMESSAGE& sMsg)
{
    if( m_psMessages == NULL )
    {
        return ERR_INVALID_DATABASE;
    }

    for(UINT i = 0; i < m_unMessageCount; i++)
    {
        if(m_psMessages[i].m_omStrMessageName == omstrMsgName)
        {
            sMsg = m_psMessages[i];
            return S_OK;
        }
    }

    return ERR_WRONG_ID;
}

/******************************************************************************
Function Name  :  unGetMsg
Input(s)       :  UINT unMsgId
Output         :  sMESSAGE*
Functionality  :  Returns the message Information
Member of      :  CDataBaseMsgList
Friend of      :  -
Author(s)      :  Venkatanarayana Makam
Date Created   :  06/04/2011
Modifications  :
******************************************************************************/
sMESSAGE* CDataBaseMsgList::unGetMsg(UINT unMsgId)
{
    if( m_psMessages == NULL )
    {
        return NULL;
    }

    for(UINT i = 0; i < m_unMessageCount; i++)
    {
        if(m_psMessages[i].m_unMessageCode == unMsgId)
        {
            return &m_psMessages[i];
        }
    }

    return NULL;
}
