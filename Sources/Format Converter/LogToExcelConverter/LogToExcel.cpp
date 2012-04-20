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
 * \brief     Definition of CLogToExcel class
 * \authors   Pradeep Kadoor, Tobias Lorenz
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Definition of the CLogToExcel class.
 */

/* Project includes */
#include "Defines.h"
#include "LogToExcel.h"

/**
 * \brief Initializing the Column fields map table
 *
 * Initializing the Column fields map table
 */
void CLogToExcel::fnSetFields()
{
    for(UINT unIndex = 0; unIndex < m_unNumOfFields; unIndex++)
	{
        pFieldMapTbl[m_pacFields[unIndex]] = unIndex;
	}
}

/**
 * \brief set the fields which are selected by the user
 *
 * set the fields which are selected by the user
 */
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

/**
 * \brief opens the log file for reading and excel file for writing
 *
 * opens the log file for reading and excel file for writing
 */
CLogToExcel::CLogToExcel(string strLogFileName, string strExcelFileName, UINT unNoOfFields, CHAR_ARRAY_20* pacFields)
{
    m_pacFields = pacFields;
    m_pLogFile = NULL;
	m_pExcelFile = NULL;

	m_strLogFileName = strLogFileName;
	m_strExcelFileName = strExcelFileName;

	//open log file in read mode
	m_pLogFile = fopen(m_strLogFileName.c_str(), "r");
	if( m_pLogFile == NULL )
	{		
		//log file could not be opened
		MessageBox(NULL,EXPORTTOEXCEL_LOGFILEOPENERROR,APPLICATION_NAME,MB_OK); 
		m_bFilesOpened = FALSE;
	}
	else
	{
		//open Excel file in write 
		m_pExcelFile = fopen(m_strExcelFileName.c_str(),"w");
		if(m_pExcelFile == NULL)
		{
			//Excel file could not be opened
			MessageBox(NULL,EXPORTTOEXCEL_CSVFILEOPENERROR,APPLICATION_NAME,MB_OK); 
			m_bFilesOpened = FALSE;
		}
		else
		{
            m_unNumOfFields = unNoOfFields;
			m_bFilesOpened = TRUE;
		}
	}
	//set selected fields
	fnSetFields();
}

CLogToExcel::~CLogToExcel()
{
}

/**
 * \brief Retrieves each line from the file and stores it in m_strBuffer
 *
 * Retrieves each line from the file and stores it in m_strBuffer
 */
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

/**
 * \brief Retrieves each field from m_strBuffer and writes it into Excel file.
 *
 * Retrieves each field from m_strBuffer and writes it into Excel file.
 */
void CLogToExcel::vPrintFields()
{
		// ignore the headers in log file
   if( (m_strBuffer.empty() ) ||(m_strBuffer[0] == '*') ||(m_strBuffer[0] == ' ')  )
		return;
   /*if(m_strBuffer[0] != '<')
	   return;*/

	UINT unBufferIndex = 0;		//index to traverse m_strBuffer string
	UINT unFieldBufferIndex=0;	//index to traverse m_strFieldBuffer string
	UINT unFieldIndex = 0;		// field index
	bool bFieldIndexExists  = false;
	fprintf(m_pExcelFile,"<TR>");
	int nFieldCurPosition = 0;

	while( unFieldIndex != m_unNumOfFields && m_strBuffer[unBufferIndex] != '\0' )
	{
		unFieldBufferIndex = 0;
		m_strFieldBuffer.clear();
		if( unFieldIndex != m_unNumOfFields-1 )
		{
			//reading each field from m_strBuffer
			while( m_strBuffer[unBufferIndex] != ' ' && m_strBuffer[unBufferIndex] != '\0')
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
			nFieldCurPosition = unFieldIndex;

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

/**
 * \brief Converts the .log file to .xls file
 *
 * Converts the .log file to .xls file
 */
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
