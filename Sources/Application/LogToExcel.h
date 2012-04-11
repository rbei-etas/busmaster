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
 * \file      LogToExcel.h
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */
#ifndef __LOGTOEXCEL_H
#define __LOGTOEXCEL_H


#define defNO_OF_FIELDS_CAN   7
#define defNO_OF_FIELDS_J1939 11
// Fileld names

typedef char CHAR_ARRAY_20[20];

__declspec( selectany ) char acFields_CAN[defNO_OF_FIELDS_CAN][20] = {  "Time",                                                                
                                                                "Direction",
                                                                "Channel",
                                                                "CAN ID",
                                                                "Type",
                                                                "DLC",
                                                                "Data Bytes"
                                                             };
__declspec( selectany ) char acFields_J1939[defNO_OF_FIELDS_J1939][20] = {  "Time",
                                                                "Channel",
                                                                "CAN ID",
                                                                "PGN",
                                                                "TYPE",
                                                                "Source",
                                                                "Destination", 
                                                                "Priority",
                                                                "Direction",
                                                                "DLC",
                                                                "Data Bytes"
                                                             };

#define defSTR_CSV_DIALOG_CAPTION    _T("Select Export File")
//#define defSTR_CSV_FORMAT_SPECIFIER  _T("Comma Separated Values format (*.xls)|*.xls||")
//#define defSTR_CSV_FORMAT            _T("xls")

#define defSTR_XLS_FORMAT_SPECIFIER  _T("Comma Separated Values format (*.xls)|*.xls||")
#define defSTR_XLS_FORMAT            _T("xls")

#define defSTR_LOG_DIALOG_CAPTION    _T("Select Log File")
#define defSTR_LOG_FORMAT_SPECIFIER  _T("*.log |*.log||")
#define defSTR_LOG_FORMAT            _T("log")
#define defSTR_FILE_NAME_EMPTY       _T("Please enter log file and export file names")
//#define defSTR_FILE_OPEN_ERROR       _T("Unable to open the file %s")

#define defSTR_CSV_HEADER            _T("FRAME Generated Report\n\n")

class CLogToExcel
{
public:
	CLogToExcel(string strLogFileName, string strExcelFileName, UINT unNoOfFields, CHAR_ARRAY_20* pacFields) ; // Open spreadsheet for reading and writing
	~CLogToExcel(); 
	BOOL bConvert();
	BOOL bValidateFile();
	BOOL bIsValidFile();
	BOOL bIsFilesOpened();
	void fnSetSelectedFields(CListBox *);
	void fnSetFields();

private:
    CHAR_ARRAY_20* m_pacFields;
	std::map<string, DWORD> pFieldMapTbl;
    std::map<string, DWORD> :: const_iterator pMapValue;
	string m_strLogFileName;
	string m_strExcelFileName;
	FILE *m_pLogFile;
	FILE *m_pExcelFile;
	string m_strBuffer;
	string m_strFieldBuffer;
    UINT unSelectedField[defNO_OF_FIELDS_J1939];
	UINT m_unNumOfFields;
	UINT unGetLine();
	void vPrintFields();
	BOOL bIsValidMsgData( CONST CString& omSendMsgLine, BOOL bHexON);
	BOOL m_bFilesOpened;
	BOOL m_bIsValidFile;
	BOOL m_bHexMode;
	std::string m_strLogVersion;
};

#endif;