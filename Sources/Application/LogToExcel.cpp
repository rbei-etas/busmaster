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
 * \file      LogToExcel.cpp
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#include "stdafx.h"
#include "LogToExcel.h"
#include "Common.h"
#include "HashDefines.h"

//**************************************************************************************************
// Function Name        : fnSetFields
// Input(s)             : 
// Output               : 
// Description          : Initializing the Column fields map table
// Author               : Sajith Kumar
// Creation Date        : 15/09/2006
// Modifications        :
//**************************************************************************************************
void CLogToExcel::fnSetFields()
{
    for(UINT unIndex = 0; unIndex < m_unNumOfFields; unIndex++)
	{
        pFieldMapTbl[m_pacFields[unIndex]] = unIndex;
	}
}

//**************************************************************************************************
// Function Name        : fnSetSelectedFields
// Input(s)             : CListBox *
// Output               : 
// Description          : set the fields which are selected by the user
// Author               : Sajith Kumar
// Creation Date        : 15/09/2006
// Modifications        :
//**************************************************************************************************
void CLogToExcel::fnSetSelectedFields(CListBox *omSelectedList)
{    
	char strFieldName[20];
	int n;
	DWORD unMaxIndex = omSelectedList->GetCount();

	//initialising the SelectedField array to -1 
    UINT unIndex;   //unIndex declared outside for loop
    for( unIndex = 0; unIndex < m_unNumOfFields; unIndex++)
		unSelectedField[unIndex] = (UINT)-1;

    //setting the appropriate value for each field	
    for(unIndex = 0; unIndex < unMaxIndex; unIndex++)
	{
		n = omSelectedList->GetTextLen(unIndex);
		memset( strFieldName,'\0',n );
        CString omFieldName;
		omSelectedList->GetText( unIndex,omFieldName);
        strcpy(strFieldName, omFieldName.GetBuffer(MAX_PATH));
		pMapValue = pFieldMapTbl.find(strFieldName);
		unSelectedField[pMapValue->second] = pMapValue->second;
	}
}

//**************************************************************************************************
// Function Name        : CLogToExcel
// Input(s)             : string strLogFileName, string strExcelFileName
// Output               : 
// Description          : opens the log file for reading and excel file for writing
// Author               : Sajith Kumar
// Creation Date        : 15/09/2006
// Modifications        :
//******************************************************************************************
CLogToExcel::CLogToExcel(string strLogFileName, string strExcelFileName, UINT unNoOfFields, CHAR_ARRAY_20* pacFields)
{
    m_pacFields = pacFields;
    m_pLogFile = NULL;
	m_pExcelFile = NULL;

	m_strLogVersion = STR_EMPTY;
	m_strLogFileName = strLogFileName;
	m_strExcelFileName = strExcelFileName;

	//open log file in read mode
	m_pLogFile = fopen(m_strLogFileName.c_str(), "r");
	if( m_pLogFile == NULL )
	{		
		//log file could not be opened
		MessageBox(NULL,EXPORTTOEXCEL_LOGFILEOPENERROR,APPLICATION_NAME,MB_OK); 
		m_bFilesOpened = FALSE;
		m_bIsValidFile = FALSE;
	}
	else
	{
		// Validate the log file
		if (!bValidateFile())
		{
			m_bFilesOpened = FALSE;
			m_bIsValidFile = FALSE;
			return;
		}
		else
		{
			m_bIsValidFile = TRUE;
		}

		//open Excel file in write 
		m_pExcelFile = fopen(m_strExcelFileName.c_str(),"w");
		if(m_pExcelFile == NULL)
		{
			//Excel file could not be opened
			MessageBox(NULL,EXPORTTOEXCEL_CSVFILEOPENERROR,APPLICATION_NAME,MB_OK); 
			m_bFilesOpened = FALSE;
			if (NULL != m_pLogFile)
			{
				fclose(m_pLogFile);
			}
			return;
		}
		else
		{
            m_unNumOfFields = unNoOfFields;
			m_bFilesOpened = TRUE;
			m_bIsValidFile = TRUE;
		}
	}
	//set selected fields
	fnSetFields();
}

CLogToExcel::~CLogToExcel()
{
}

//**************************************************************************************************
// Function Name        : unGetLine()
// Input(s)             : 
// Output               : unsigned int
// Description          : Retrieves each line from the file and stores it in m_strBuffer
// Author               : Sajith Kumar.V.R
// Creation Date        : 15/09/2006
// Modifications        :
//**************************************************************************************************

UINT CLogToExcel::unGetLine()
{
	UINT unBufferIndex = 0;  //index to traverse m_strBuffer string
	char ch;               // stores each character read from the file
	
	//reads log file till end of line is not reached
	while ( (ch = (char)getc(m_pLogFile)) != '\n' )
	{
		//m_strBuffer contains each line read from log file
		m_strBuffer.append(1,ch);
		if( ch == EOF )
		{ 
			m_strBuffer.append(1,'\0');
			return 1;
		}
		unBufferIndex++;
	}
	m_strBuffer.append(1,'\0');
	return 0;
}

//**************************************************************************************************
// Function Name        : vPrintFields()
// Input(s)             : 
// Output               : 
// Description          : Retrieves each field from m_strBuffer and writes it into Excel file.
// Author               : Sajith Kumar.V.R
// Creation Date        : 15/09/2006
// Modifications        :
//**************************************************************************************************
void CLogToExcel::vPrintFields()
{
	// ignore the headers in log file
   if( (m_strBuffer.empty() ) ||(m_strBuffer[0] == '*') ||(m_strBuffer[0] == ' ')  )
		return;

   // Check for the baudrate and database information
   if (!bIsValidMsgData(m_strBuffer.c_str(), m_bHexMode))
	   return;

	UINT unBufferIndex = 0;		//index to traverse m_strBuffer string
	UINT unFieldBufferIndex=0;	//index to traverse m_strFieldBuffer string
	UINT unFieldIndex = 0;		// field index
	bool bFieldIndexExists  = false;
	fprintf(m_pExcelFile,"<TR>");
    
	while( unFieldIndex != m_unNumOfFields && m_strBuffer[unBufferIndex] != '\0' )
	{
		unFieldBufferIndex = 0;
		m_strFieldBuffer.clear();
		if( unFieldIndex != m_unNumOfFields-1 )
		{
			//reading each field from m_strBuffer
			while( m_strBuffer[unBufferIndex] != ' ')
			{
				m_strFieldBuffer.append(1,m_strBuffer[unBufferIndex]);
				unFieldBufferIndex++;
				unBufferIndex++;
			}// while - m_strBuffer[unBufferIndex] != _T(" ")

			if( m_strBuffer.length() != unBufferIndex && m_strBuffer[unBufferIndex] == ' ')
				unBufferIndex++;
			if( m_strBuffer.length() != unBufferIndex && m_strBuffer[unBufferIndex] == ' ')
				unBufferIndex++;
		}// unFieldIndex != m_unNumOfFields-1
		else
		{
			// reading the data bytes field from m_strBuffer
			unFieldBufferIndex = 0;
			while((m_strBuffer[unBufferIndex] != '\0') && (m_strBuffer[unBufferIndex] != '\n'))
			{
				m_strFieldBuffer.append(1,m_strBuffer[unBufferIndex]);
				unFieldBufferIndex++;
				unBufferIndex++;
			}

		} // else 
		m_strFieldBuffer.append(1,'\0');


		//writing the field into excel file
		for(UINT nIndex = 0; nIndex < m_unNumOfFields; nIndex++)
		{
			if ( unSelectedField[nIndex] == unFieldIndex )
			{
				bFieldIndexExists = true;
				break;
			} // if unSelectedField[nIndex] == unFieldIndex
		}// for loop - unSelectedField

		if ( bFieldIndexExists )
		{
			fprintf(m_pExcelFile,"<TD>%s</TD>",m_strFieldBuffer.c_str());
			bFieldIndexExists = false;
		}
		else
			m_strFieldBuffer="";
	unFieldIndex++;
	}// While - field navigation
	m_strFieldBuffer.clear();
	fprintf(m_pExcelFile,"</TR>");

}

//**************************************************************************************************
// Function Name        : bConvert()
// Input(s)             : 
// Output               : BOOL
// Description          : Converts the .log file to .xls file
// Author               : Sajith Kumar.V.R
// Creation Date        : 15/09/2006
// Modifications        :
//**************************************************************************************************
BOOL CLogToExcel::bConvert()
{
	if(m_bFilesOpened)
	{
		fputs( "<HTML><HEAD></HEAD><BODY>",m_pExcelFile );
		fprintf( m_pExcelFile,"<FONT COLOR =\"GREEN\"><CENTER><B>BUSMASTER - Exported Log File Report</B></CENTER>" );
		fprintf( m_pExcelFile,"<BR><BR>");
		fprintf( m_pExcelFile," <TABLE BORDER COLOR=\"BLACK\"><TR> " );

		// write column headers
		for( UINT unIndex = 0; unIndex < m_unNumOfFields; unIndex++)
			if(unSelectedField[unIndex]!=-1)
				fprintf( m_pExcelFile,"<TD><FONT COLOR=\"Green\"><B> %s </B></TD>",m_pacFields[unIndex] );
		fprintf( m_pExcelFile,"</TR>" );
		// write each column
		while(!unGetLine())
		{
			vPrintFields();
			m_strBuffer.clear();
		}
		fputs( "</TABLE></BODY></HTML>",m_pExcelFile );
		//close file
		fclose(m_pExcelFile);
		fclose(m_pLogFile);
		return TRUE;
	}
	return FALSE;
}

BOOL CLogToExcel::bValidateFile()
{
	// Check for all the tags in the log file
	// Read the file line by line.
	BOOL bReturn		= TRUE;
    BOOL bModeFound		= FALSE;
    BOOL bFileEndFlag	= FALSE;
    BOOL bMsgModeFound	= FALSE;    
    BOOL bVersionFound	= FALSE;
	CString omStrLine	= STR_EMPTY;
	int nIndex = 0;
	
    while (( bMsgModeFound == FALSE  || bModeFound == FALSE || 
			 bVersionFound == FALSE ) && !unGetLine())
    {        
        omStrLine = m_strBuffer.c_str();
        omStrLine.TrimLeft();
        omStrLine.TrimRight();

        // Version check
        if( omStrLine.Find(defSTR_BUSMASTER_VERSION_STRING) != -1 )
        {
			nIndex = omStrLine.Replace("***", "");
			nIndex = omStrLine.Replace(defSTR_BUSMASTER_VERSION_STRING, "");
			omStrLine.TrimLeft();
			omStrLine.TrimRight();
			m_strLogVersion = omStrLine;
			if (!m_strLogVersion.empty())
				bVersionFound = TRUE;
        }
        
        // Display Mode check
        if( omStrLine.Find(HEX_MODE) == 0)
        {
            bMsgModeFound = TRUE;
			m_bHexMode = TRUE;
        }
        else if (omStrLine.Find(DEC_MODE) == 0)
        {
            bMsgModeFound = TRUE;
			m_bHexMode = FALSE;
        }

		// Time Mode check
        if( omStrLine.Find(SYSTEM_MODE) == 0)
        {            
            bModeFound = TRUE;
        }
        else if( omStrLine.Find(ABSOLUTE_MODE) == 0)
        {            
            bModeFound = TRUE;
        }
        else if( omStrLine.Find(RELATIVE_MODE) == 0)
        {            
            bModeFound = TRUE;
        }

		m_strBuffer.clear();
    }

	if( bReturn == TRUE &&
        ( bModeFound == FALSE || bMsgModeFound == FALSE ||
          bVersionFound == FALSE ) )
    {        
        bReturn = FALSE;
        
    }
	return bReturn;
}

BOOL CLogToExcel::bIsValidFile()
{
	return m_bIsValidFile;
}

BOOL CLogToExcel::bIsFilesOpened()
{
	return m_bFilesOpened;
}


BOOL CLogToExcel::bIsValidMsgData( CONST CString& omSendMsgLine, BOOL bHexON)
{
    CString omStrTemp       =_T("");
    CString omStrMsgID      =_T("");
    CString omStrDLC        =_T("");
    CString omStrData       =_T("");
    CString omStrMsgIDType  =_T("");
    CHAR* pcStopString      = NULL;
    BOOL nReturn            = FALSE;
	BOOL bDirectionTx		= TRUE;

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
					bDirectionTx = TRUE;
                }
                else
                {
					bDirectionTx = FALSE;
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
                }// Message Id type is STD
                else if(omStrMsgIDType.Find(defMSGID_STD)!= -1)
                {                    
                }

                // Message Id type is RTR
                if(omStrMsgIDType.Find(defMSGID_RTR)!= -1)
                {                    
                }
                else
                {                    
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
                    //nIndex = omStrData.GetLength();
                    //vConvStrtoByteArray(&omByteArrayDataTx,
                    //                    omStrData.GetBuffer(nIndex),bHexON);
                    //omStrData.ReleaseBuffer(nIndex);
                    //INT nTotalData = (INT)omByteArrayDataTx.GetSize();
                    //// Check if String to Byte array conversion
                    //// has return a valid data
                    //if(nTotalData<=8 )
                    {
                        nReturn = TRUE;
                    }
                }
            }
        }
    }
    return nReturn;
}