/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  LogToExcel.cpp
  Description   :  
  $Log:   X:/Archive/Sources/Application/LogToExcel.cpv  $
   
      Rev 1.5   10 Jan 2012 17:27:40   CANMNTTM
   End of line check is added. This is to avoid the crash if the header is in the log is without asteriks.
   
      Rev 1.4   06 Sep 2011 19:55:24   CANMNTTM
   CANvas name is replaced with BUSMATER
   
      Rev 1.3   09 Jun 2011 18:03:48   CANMNTTM
    
   
      Rev 1.2   09 Jun 2011 11:28:06   CANMNTTM
    
   
      Rev 1.1   15 Apr 2011 20:01:00   CANMNTTM
   Added RBEI Copyright information.
  
  Author(s)     :  Pradeep Kadoor
  Date Created  :  15/04/2011
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
 ******************************************************************************/
#include "stdafx.h"
#include "LogToExcel.h"
#include "Defines.h"

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