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
 * \file      Utility_Replay.cpp
 * \author    Ratnadip Choudhury
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "Replay_stdafx.h"
#include "Utility_Replay.h"
#include "Application/HashDefines.h"


/******************************************************************************/
/*  Function Name    :  bGetMsgInfoFromMsgStr                                 */
/*  Input(s)         :  Constant CString Reference,pointer to UINT,pointer to */
/*                       UINT, and pointer to UCHAR                           */
/*  Output           :  TRUE or FALSE                                         */
/*  Functionality    :  This function is called when user wants to get DLC,   */
/*                      message ID and data from a string logged in a logfile */
/*                                                                            */
/*  Member of        :      -                                                 */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  04.04.2002                                            */
/*  Modifications By :  Amitesh Bharti                                        */
/*  Modifications on :  01.04.2003, Get the message information from the      */
/*                      current message into the message info structure.      */
/*  Modifications By :  Amitesh Bharti                                        */
/*  Modifications on :  08.03.2004, Changes due to message ID and name is     */
/*                      logged along side.                                    */
/*  Modifications on :  20.07.2005, Changed parameter to STCANDATA            */
/******************************************************************************/
BOOL bGetMsgInfoFromMsgStr( CONST CString& omSendMsgLine,
                            PSTCANDATA psCANData,
                            BOOL bHexON)
{
    CString omStrTemp       ="";
    CString omStrMsgID      ="";
    CString omStrDLC        ="";
    CString omStrData       ="";
    CString omStrMsgIDType  ="";
    CHAR* pcStopString      = NULL;
    BOOL nReturn            = FALSE;
    CByteArray omByteArrayDataTx;
    // Get the string before first white space charactor
    omStrTemp = omSendMsgLine.SpanExcluding("\t ");

    if(omStrTemp.IsEmpty()==0)
    {
        INT nIndex = omStrTemp.GetLength();

        if(nIndex>0)
        {
            // Remove the time stamp string
            omStrTemp = omSendMsgLine.Right(omSendMsgLine.GetLength() -
                                            nIndex -1);

            if(omStrTemp.IsEmpty() ==0)
            {
                omStrTemp.TrimLeft();
                omStrTemp.TrimRight();
                // Get the message ID after removing Tx/Rx string
                omStrMsgID = omStrTemp.SpanExcluding("\t ");

                if( omStrMsgID.Compare("Tx") == 0 )
                {
                    psCANData->m_ucDataType = TX_FLAG;
                }
                else
                {
                    psCANData->m_ucDataType = RX_FLAG;
                }

                nIndex     = omStrMsgID.GetLength();
                omStrTemp  = omStrTemp.Right(omStrTemp.GetLength() - nIndex - 1);
                omStrTemp.TrimLeft();
                // Channel ID
                omStrMsgID = omStrTemp.SpanExcluding("\t ");
                UCHAR ucChannel    =
                    (UCHAR) strtol( (LPCTSTR )omStrMsgID,&pcStopString ,10);
                nIndex     = omStrMsgID.GetLength();
                omStrTemp  = omStrTemp.Right(omStrTemp.GetLength() - nIndex -1);
                omStrTemp.TrimLeft();
                // Get the message with name
                omStrMsgID = omStrTemp.SpanExcluding("\t ");
                // Get the rest of the string.
                nIndex     = omStrMsgID.GetLength();
                omStrTemp  = omStrTemp.Right(omStrTemp.GetLength() - nIndex -1);
                omStrTemp.TrimLeft();
                // Get message ID string after removing any message name.
                omStrMsgID = omStrMsgID.SpanExcluding(defMSGID_NAME_DELIMITER);
                UINT unMsgID = 0;

                if( bHexON == TRUE)
                {
                    unMsgID    =
                        (UINT) strtol( (LPCTSTR )omStrMsgID,&pcStopString ,16);
                }
                else
                {
                    unMsgID    =
                        (UINT) strtol( (LPCTSTR )omStrMsgID,&pcStopString ,10);
                }

                // Get the message ID Type
                omStrMsgIDType = omStrTemp.SpanExcluding("\t ");

                // Message Id type is EXTENDED
                if(omStrMsgIDType.Find(defMSGID_EXTENDED) != -1)
                {
                    psCANData->m_uDataInfo.m_sCANMsg.m_ucEXTENDED = 1;
                }// Message Id type is STD
                else if(omStrMsgIDType.Find(defMSGID_STD)!= -1)
                {
                    psCANData->m_uDataInfo.m_sCANMsg.m_ucEXTENDED = 0;
                }

                // Message Id type is RTR
                if(omStrMsgIDType.Find(defMSGID_RTR)!= -1)
                {
                    psCANData->m_uDataInfo.m_sCANMsg.m_ucRTR = 1;
                }
                else
                {
                    psCANData->m_uDataInfo.m_sCANMsg.m_ucRTR = 0;
                }

                nIndex     = omStrMsgIDType.GetLength();
                omStrTemp  = omStrTemp.Right(omStrTemp.GetLength() - nIndex -1);
                omStrTemp.TrimLeft();
                // Get the DLC
                omStrDLC   = omStrTemp.SpanExcluding("\t ");
                nIndex     = omStrDLC.GetLength();
                UINT unDLC = (UINT) strtol((LPCTSTR)omStrDLC,&pcStopString ,16);
                omStrTemp  = omStrTemp.Right(omStrTemp.GetLength() - nIndex -1);
                omStrTemp.TrimLeft();
                // Get the data string
                omStrData  = omStrTemp;

                // Check if Message ID and DLC is valid.
                if(unMsgID>0 && unDLC<=8 && unDLC>0)
                {
                    nIndex = omStrData.GetLength();
                    vConvStrtoByteArray(&omByteArrayDataTx,
                                        omStrData.GetBuffer(nIndex),bHexON);
                    omStrData.ReleaseBuffer(nIndex);
                    INT nTotalData = (INT)omByteArrayDataTx.GetSize();

                    // Check if String to Byte array conversion
                    // has return a valid data
                    if(nTotalData<=8 )
                    {
                        for(INT i = 0; i<nTotalData; i++)
                        {
                            psCANData->m_uDataInfo.m_sCANMsg.m_ucData[i] =
                                omByteArrayDataTx.GetAt(i);
                        }

                        psCANData->m_uDataInfo.m_sCANMsg.m_unMsgID = unMsgID;
                        psCANData->m_uDataInfo.m_sCANMsg.m_ucDataLen   = (UCHAR)unDLC;
                        psCANData->m_uDataInfo.m_sCANMsg.m_ucChannel = ucChannel;
                        nReturn    = TRUE;
                    }
                }
            }
        }
    }

    return nReturn;
}

/******************************************************************************/
/*  Function Name    :  vConvStrtoByteArray                                   */
/*  Input(s)         :  CHAR* and Pointer to CByteArray                       */
/*  Output           :                                                        */
/*  Functionality    :  Function will convert a string to equivalent          */
/*                      byte array.                                           */
/*  Member of        :      -                                                 */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  04.04.2002                                            */
/*  Modifications By :  Amitesh Bharti                                        */
/*  Modifications on :  12.04.2002, Review comment incorporated               */
/*  Modifications By :  Krishnaswamy B.N                                      */
/*  Modifications on :  24.04.2002, Added a new parameter bHExON              */
/******************************************************************************/

void vConvStrtoByteArray(CByteArray* pomByteArrayBufTx,
                         CHAR* pctempBuf,
                         BOOL bHexON)
{
    char tCh = ' ';
    UINT unCount=0;
    UCHAR ucFirstCh = '\0';
    UCHAR ucSecondCh = '\0';
    UCHAR ucThirdCh = '\0';
    //Copy the modified string to a local Cstring
    //variable and remove all white spaces
    CString omStrTemp(pctempBuf);
    omStrTemp.Remove(tCh);
    INT nStrLength = omStrTemp.GetLength();

    //start converting them into a byte.
    while (nStrLength)
    {
        //get the first char
        ucFirstCh = omStrTemp.GetAt(unCount++);

        //if the value is greater than or equal to zero or
        //less than or equal to nine, get the value by
        //subtracting the ASCII value of '0'
        if (('0' <= ucFirstCh) && (ucFirstCh <= '9'))
        {
            ucFirstCh -='0';
        }
        else
        {
            //The character is between 'a' to 'f'. First
            //convert to lowercase and deduct 87 which
            //will give the correct value for the char
            //as the ASCII value of 'a' is 97.
            ucFirstCh = (UCHAR)tolower (ucFirstCh);
            ucFirstCh-= 87;
        }

        //if the value is greater than or equal to zero or
        //less than or equal to nine, get the value by
        //subtracting the ASCII value of '0'
        nStrLength --;

        if(nStrLength)
        {
            ucSecondCh = omStrTemp.GetAt(unCount++);

            if (('0' <= ucSecondCh) && (ucSecondCh <= '9'))
            {
                ucSecondCh -='0';
            }
            else
            {
                //The character is between 'a' to 'f'. First
                //convert to lowercase and deduct 87 which
                //will give the correct value for the char
                //as the ASCII value of 'a' is 97.
                ucSecondCh = (UCHAR)tolower (ucSecondCh);
                ucSecondCh-= 87;
            }

            //Now add the byte to the byte array
            if(bHexON == TRUE)
            {
                ucFirstCh <<= 4;
                ucFirstCh |= ucSecondCh;
                pomByteArrayBufTx->Add(ucFirstCh);
            }

            nStrLength --;
        }

        if(bHexON == FALSE )
        {
            //if the value is greater than or equal to zero or
            //less than or equal to nine, get the value by
            //subtracting the ASCII value of '0'
            //check if the mode is Decimal ,then get the third character
            if(nStrLength )
            {
                ucThirdCh = omStrTemp.GetAt(unCount++);

                if (('0' <= ucThirdCh) && (ucThirdCh <= '9'))
                {
                    ucThirdCh -='0';
                }

                ucFirstCh  = ucFirstCh * 100;
                ucFirstCh += ucSecondCh * 10;
                ucFirstCh = (UCHAR)(ucFirstCh + ucThirdCh);                 //ucFirstCh += ucThirdCh;
                nStrLength --;
                pomByteArrayBufTx->Add(ucFirstCh);
            }
        }
    }//while (nStrLength);

    return;
}


/******************************************************************************/
/*  Function Name    :  bIsModeMismatch                                       */
/*  Input(s)         :  ifstream omInReplayFile,BOOL bReplayHexON ............*/
/*                      BOOL wLogReplayTimeMode                               */
/*  Output           :  BOOL                                                  */
/*  Functionality    :  This function finds the change in mode                */
/*  Member of        :  CMsgReplayWnd                                         */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Krishnaswamy B.N                                      */
/*  Date Created     :  19.08.2003                                            */
/******************************************************************************/
BOOL bIsModeMismatch( ifstream& omInReplayFile,
                      BOOL bReplayHexON,
                      WORD wLogReplayTimeMode)
{
    BOOL bFlag = FALSE;
    BOOL bLine = TRUE;
    CHAR Line[500] = { NULL };
    CString omStrLine;
    BOOL bLogModeChecked = FALSE;
    BOOL bReplayMsgTypeChecked = FALSE;

    if(omInReplayFile != NULL)
    {
        while( bLine &&  ! omInReplayFile.eof())
        {
            omInReplayFile.getline( Line, sizeof(Line));
            omStrLine = Line;

            if( omStrLine.Find(HEX_MODE) == 0)
            {
                bLogModeChecked = TRUE;

                if(bReplayHexON != TRUE)
                {
                    bFlag = TRUE;
                }
            }
            else if (omStrLine.Find(DEC_MODE) == 0)
            {
                bLogModeChecked = TRUE;

                if(bReplayHexON != FALSE)
                {
                    bFlag = TRUE;
                }
            }

            if( omStrLine.Find(SYSTEM_MODE) == 0)
            {
                bReplayMsgTypeChecked = TRUE;

                if( wLogReplayTimeMode != eSYSTEM_MODE)
                {
                    bFlag = TRUE;
                    bLine = FALSE;
                }
            }
            else if( omStrLine.Find(ABSOLUTE_MODE) == 0)
            {
                bReplayMsgTypeChecked = TRUE;

                if( wLogReplayTimeMode != eABSOLUTE_MODE)
                {
                    bFlag = TRUE;
                    bLine = FALSE;
                }
            }
            else if( omStrLine.Find(RELATIVE_MODE) == 0)
            {
                bReplayMsgTypeChecked = TRUE;

                if( wLogReplayTimeMode != eRELATIVE_MODE)
                {
                    bFlag = TRUE;
                    bLine = FALSE;
                }
            }

            if(bLogModeChecked == TRUE && bReplayMsgTypeChecked == TRUE)
            {
                bLine = FALSE;
            }
        }
    }

    return bFlag;
}


/******************************************************************************/
/*  Function Name    :  unTimeDiffBetweenMsg                                  */
/*  Input(s)         :  CString& omStrNextMsg, CString& omStrCurrent          */
/*  Output           :  Time difference ( in milli second )                   */
/*  Functionality    :  To get the time difference between two consecutive    */
/*                      Messages in selected replay file.                     */
/*  Member of        :      -                                                 */
/*  Friend of        :      -                                                 */
/*  Author(s)        :  Amitesh Bharti                                        */
/*  Date Created     :  16.08.2002                                            */
/*  Modification By  :  Amitesh Bharti                                        */
/*  Modified On      :  20.08.2002, Review comment incorporated and changes   */
/*                                  after testing.                            */
/*  Modification By  :  Krishnaswamy B.N                                      */
/*  Modified On      :  03.06.2004, Relative time is computed                 */
/******************************************************************************/
UINT unTimeDiffBetweenMsg( CString& omStrNextMsg,
                           CString& omStrCurMsg,
                           WORD wLogReplyTimeMode)
{
    UINT unTimeDifference    = 0;
    CString omStrMsgCurTime  ="";
    CString omStrMsgNextTime  ="";
    CString omStrTemp        ="";
    DOUBLE dCurTime          = 0;
    DOUBLE dNextTime         = 0;
    // Multiplication factors for HR, MIN, SECOND, and MILLI SECOND
    DOUBLE adMultiFac[4]     = {60*60*1000,60*1000,1000,0.1};
    CHAR* pcStopString  = NULL;
    omStrMsgCurTime = omStrCurMsg.SpanExcluding("\t ");
    omStrMsgNextTime = omStrNextMsg.SpanExcluding("\t ");

    if(omStrMsgNextTime.IsEmpty()==FALSE && omStrMsgCurTime.IsEmpty()==FALSE)
    {
        INT  nIndex     = 0;
        INT  nLoopCount = 0;
        UINT unTemp     = 0;

        // Get the Next time in milli second
        while(omStrMsgNextTime.IsEmpty()==FALSE)
        {
            omStrTemp       = omStrMsgNextTime.SpanExcluding(":");
            nIndex          = omStrTemp.GetLength();
            unTemp          = (UINT) strtol( (LPCTSTR )omStrTemp,&pcStopString,
                                             10);
            dNextTime       += unTemp * adMultiFac[nLoopCount];
            nLoopCount++;
            omStrMsgNextTime =
                omStrMsgNextTime.Right(omStrMsgNextTime.GetLength() - nIndex -1);
        }

        // Reset the loop counter
        nLoopCount = 0;

        // Get the current time in milli second
        if( wLogReplyTimeMode != 2)
        {
            while(omStrMsgCurTime.IsEmpty()==FALSE)
            {
                omStrTemp       = omStrMsgCurTime.SpanExcluding(":");
                nIndex          = omStrTemp.GetLength();
                unTemp          =
                    (UINT) strtol( (LPCTSTR )omStrTemp,&pcStopString, 10);
                dCurTime        += unTemp * adMultiFac[nLoopCount];
                nLoopCount++;
                omStrMsgCurTime =
                    omStrMsgCurTime.Right(omStrMsgCurTime.GetLength() -
                                          nIndex -1);
            }
        }
    }

    if(wLogReplyTimeMode != 2)
    {
        if(dCurTime < dNextTime)
        {
            unTimeDifference = (UINT)(dNextTime - dCurTime + 0.5);
        }
    }
    else
    {
        if(dNextTime >0)
        {
            unTimeDifference = static_cast<UINT>(dNextTime) ;
        }
    }

    return unTimeDifference;
}
