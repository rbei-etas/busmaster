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
* \file      HardwareListing.cpp
* \brief     Interface file for CHardwareListing class
* \author    Ratnadip Choudhury
* \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
*
* Interface file for CHardwareListing class
*/

// Standard include header
#include "CAN_SAINT_stdafx.h"
#include <afxtempl.h>  
#include "HardwareListing.h"
#include ".\hardwarelisting.h"
#include "Utility\MultiLanguageSupport.h"
#include "CAN_SAINT.h"
#include <ole2.h> // OLE2 Definitions
#include <afxdb.h>
#include <odbcinst.h>
#include <string>
#include <hash_map>
#include "Application\ConfigData.h"

#include "MainPageDialog.h"
#include "FaultCodes.h"
#include "PeriodicMessages.h"
#include "Filter.h"
#include "Emulation.h"
#include "GroupEditor.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


stdext::hash_map<std::string, std::vector<FaultCode>> faultcodes;
std::vector<CDialog> m_vPages;

CString filename;
CString dir;
CString curEntry = "";
MainPageDialog *mPage;
FaultCodes * fPage;
PeriodicMessages *pPage;
Filter * filterPage;
Emulation * ePage;
GroupEditor * gPage;
CCAN_SAINTApp * saintApp;
CHardwareListing::CHardwareListing( INTERFACE_HW* psIntrHw,
								   int nSize, int* pnSelList,CCAN_SAINTApp * saint, CWnd* pParent /*=nullptr*/)
								   : CResizableDialog(CHardwareListing::IDD, pParent)
{
	TCHAR pwd[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,pwd);
	dir = pwd;
	loadConfig();
	saintApp = saint;
}
CHardwareListing::CHardwareListing()
	: CResizableDialog(CHardwareListing::IDD, nullptr)
{
	
}
void CHardwareListing::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, tabCtrl);
	DDX_Control(pDX, IDC_LABEL, labelFile);
}


BEGIN_MESSAGE_MAP(CHardwareListing, CResizableDialog)
	ON_BN_CLICKED(IDC_BUTTON2, CHardwareListing::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, CHardwareListing::OnBnClickedButton3)
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, CHardwareListing::OnTcnSelchangeTab1)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, CHardwareListing::OnTcnSelchangingTab1)
END_MESSAGE_MAP()
int CHardwareListing::getEPageTransmit(){
	return ePage->transmit;
}
void CHardwareListing::loadConfig(){
	if(dir.Compare("") == 0){
		TCHAR pwd[MAX_PATH];
		GetCurrentDirectory(MAX_PATH,pwd);
		dir = pwd;
	}
	Fault_Code_Data_Byte_Start = 0;
	Fault_Code_Data_Byte_End = 0;
	ID_Start = 0;
	ID_End = 0;
	DTC_Start = 0;
	DTC_End = 0;
	Type_Start = 0;
	Type_End = 0;
	Status_Start = 0;
	Status_End = 0;


	char lineBuffer[1000];
	int i = 0;
	CFile	myFile;
	char * curChar;
	if ( myFile.Open(_T(dir + "\\saint_config.cfg"),CFile::modeReadWrite) ){
		ULONGLONG dwLength = myFile.GetLength();
		// make room for whole file, plus null
		BYTE *buffer = (BYTE *) malloc(dwLength + 1); 
		myFile.Read(buffer, dwLength); // read whole file
		*(buffer + dwLength) = '\0';   // add null
		curChar = (char *)buffer;
		CString str(curChar);
		int inBracket = 0;
		CString curVar;
		int commacount = 0;
		int prevIndex = 0;
		for(int i = 0; i < str.GetLength(); i++){
			if(str.GetAt(i) == '\n'){
				i++;
				prevIndex = i;
			}else if(str.GetAt(i) == '='){
				curVar = str.Mid(prevIndex,i-prevIndex);
				i+=2;
				prevIndex = i;
				inBracket = 1;
			}else if((str.GetAt(i) == ',' || str.GetAt(i) == '}') && inBracket == 1 ){
				if(curVar.Compare("CAN1_Speed") == 0){
					CAN1_Speed.push_back(str.Mid(prevIndex,i-prevIndex));
				}
				if(curVar.Compare("CAN1_Speed_Messages") == 0){
					CAN1_Speed_Messages.push_back(str.Mid(prevIndex,i-prevIndex));
				}
				if(curVar.Compare("CAN2_Speed") == 0){
					CAN2_Speed.push_back(str.Mid(prevIndex,i-prevIndex));
				}
				if(curVar.Compare("CAN2_Speed_Messages") == 0){
					CAN2_Speed_Messages.push_back(str.Mid(prevIndex,i-prevIndex));
				}
				if(curVar.Compare("CAN_Prepare_Message") == 0){
					CAN_Prepare_Message = str.Mid(prevIndex,i-prevIndex);
				}
				if(curVar.Compare("CAN_Init_Message") == 0){
					CAN_Init_Message = str.Mid(prevIndex,i-prevIndex);
				}
				if(curVar.Compare("CAN1_Set") == 0){
					CAN1_Set = str.Mid(prevIndex,i-prevIndex);
				}
				if(curVar.Compare("CAN1_Type") == 0){
					CAN1_Type.push_back(str.Mid(prevIndex,i-prevIndex));
				}
				if(curVar.Compare("CAN1_Type_Messages") == 0){
					CAN1_Type_Messages.push_back(str.Mid(prevIndex,i-prevIndex));
				}
				if(curVar.Compare("CAN1_Sleep_Messages") == 0){
					CAN1_Sleep_Messages.push_back(str.Mid(prevIndex,i-prevIndex));
				}
				if(curVar.Compare("CAN2_Set") == 0){
					CAN2_Set = str.Mid(prevIndex,i-prevIndex);
				}
				if(curVar.Compare("CAN2_Type") == 0){
					CAN2_Type.push_back(str.Mid(prevIndex,i-prevIndex));
				}
				if(curVar.Compare("CAN2_Type_Messages") == 0){
					CAN2_Type_Messages.push_back(str.Mid(prevIndex,i-prevIndex));
				}
				if(curVar.Compare("CAN2_Sleep_Messages") == 0){
					CAN2_Sleep_Messages.push_back(str.Mid(prevIndex,i-prevIndex));
				}
				if(curVar.Compare("Fault_Code_Requests") == 0){
					Fault_Code_Requests.push_back(str.Mid(prevIndex,i-prevIndex));
				}
				if(curVar.Compare("Fault_Code_Data_ID") == 0){
					Fault_Code_Data_ID = str.Mid(prevIndex,i-prevIndex);
				}
				if(curVar.Compare("Fault_Code_Data_End") == 0){
					Fault_Code_Data_End = str.Mid(prevIndex,i-prevIndex);
				}
				if(curVar.Compare("Fault_Code_Data_Bytes") == 0){
					if(str.GetAt(prevIndex+3) == '}'){
						Fault_Code_Data_Byte_Start = atoi(str.Mid(prevIndex,i-prevIndex));
						Fault_Code_Data_Byte_End = atoi(str.Mid(prevIndex+2,i-prevIndex+2));
						i++;
					}else{
						Fault_Code_Data_Byte_Start = atoi(str.Mid(prevIndex,i-prevIndex));
						Fault_Code_Data_Byte_End = atoi(str.Mid(prevIndex+2,i-prevIndex+3));
						i+=2;
					}
				}
				if(curVar.Compare("Clear_Fault_Message") == 0){
					Clear_Fault_Message = str.Mid(prevIndex,i-prevIndex);
				}
				if(curVar.Compare("ID") == 0){
					if(str.GetAt(prevIndex+3) == '}'){
						ID_Start = atoi(str.Mid(prevIndex,i-prevIndex));
						ID_End = atoi(str.Mid(prevIndex+2,i-prevIndex+2));
						i++;
					}else{
						ID_Start = atoi(str.Mid(prevIndex,i-prevIndex));
						ID_End = atoi(str.Mid(prevIndex+2,i-prevIndex+3));
						i+=2;
					}
				}
				if(curVar.Compare("DTC") == 0){
					if(str.GetAt(prevIndex+3) == '}'){
						DTC_Start = atoi(str.Mid(prevIndex,i-prevIndex));
						DTC_End = atoi(str.Mid(prevIndex+2,i-prevIndex+2));
						i++;
					}else{
						DTC_Start = atoi(str.Mid(prevIndex,i-prevIndex));
						DTC_End = atoi(str.Mid(prevIndex+2,i-prevIndex+3));
						i+=2;
					}
				}
				if(curVar.Compare("Type") == 0){
					if(str.GetAt(prevIndex+3) == '}'){
						char temp = str.GetAt(prevIndex);
						Type_Start = atoi(str.Mid(prevIndex,i-prevIndex));
						Type_End = atoi(str.Mid(prevIndex+2,i-prevIndex+2));
						i++;
					}else{
						Type_Start = atoi(str.Mid(prevIndex,i-prevIndex));
						Type_End = atoi(str.Mid(prevIndex+2,i-prevIndex+3));
						i+=2;
					}
				}
				if(curVar.Compare("Status") == 0){
					if(str.GetAt(prevIndex+3) == '}'){
						char temp = str.GetAt(prevIndex);
						Status_Start = atoi(str.Mid(prevIndex,i-prevIndex));
						Status_End = atoi(str.Mid(prevIndex+2,i-prevIndex+2));
						i++;
					}else{
						Status_Start = atoi(str.Mid(prevIndex,i-prevIndex));
						Status_End = atoi(str.Mid(prevIndex+2,i-prevIndex+3));
						i+=2;
					}
				}
				if(curVar.Compare("Active_History_Mask") == 0){
					Active_History_Mask = str.Mid(prevIndex,i-prevIndex);
				}
				if(curVar.Compare("Fault_Code_Excel_Page_Name") == 0){
					Fault_Code_Excel_Page_Name = str.Mid(prevIndex,i-prevIndex);
				}
				if(str.GetAt(i) == '}'){
					inBracket = 0;
				}
				i++;
				prevIndex = i;
			}

		}
	}
}
void CHardwareListing::readSettingsFile(){

	char lineBuffer[1000];
	int i = 0;
	CFile	myFile;
	char * curChar;
	if (myFile.Open(_T(dir + "\\saint.dat"), CFile::modeReadWrite)){
		ULONGLONG dwLength = myFile.GetLength();
		// make room for whole file, plus null
		BYTE *buffer = (BYTE *) malloc(dwLength + 1); 
		myFile.Read(buffer, dwLength); // read whole file
		*(buffer + dwLength) = '\0';   // add null
		curChar = (char *)buffer;
		CString str(curChar);
		int commacount = 0;
		int previndex = 0;
		for(int i = 0; i < str.GetLength(); i++){
			if(str.GetAt(i) == ','){
				if(commacount == 0){
					prevMessageFile = str.Mid(previndex,i-previndex);
				}else if(commacount == 1){
					prevExcelFile = str.Mid(previndex,i-previndex);
				}else if(commacount == 2){
					prevPeriodicFile = str.Mid(previndex,i-previndex);
				}else if(commacount == 3){
					prevFilterFile = str.Mid(previndex,i-previndex);
				}else if(commacount == 4){
					prevEmulationFile = str.Mid(previndex,i-previndex);
				}else if(commacount == 5){
					prevGroupFile = str.Mid(previndex,i-previndex);
				}
				commacount++;
				previndex = i+1;
				
			}
		}
		myFile.Close();
		CFile settingFile;
		if(prevMessageFile.Compare("NULL") != 0){
			if (settingFile.Open( _T(prevMessageFile),CFile::modeReadWrite ) )
			{
				ULONGLONG dwLength = settingFile.GetLength();
				// make room for whole file, plus null
				BYTE *buffer = (BYTE *) malloc(dwLength + 1); 
				settingFile.Read(buffer, dwLength); // read whole file
				*(buffer + dwLength) = '\0';   // add null
				curChar = (char*)buffer;
				while(*curChar != '\0'){
					if(*curChar == '\n'){
						if(i == 0){
							mPage->getMessageListBox()->AddString(" ");
						}else{
							lineBuffer[i] = '\0';
							i = 0;
							curChar++;
							CString line(lineBuffer);
							mPage->getMessageListBox()->AddString(line);
						}
					}else{
						lineBuffer[i] = *curChar;
						i++;
						curChar++;
					}
				}
				settingFile.Close();
			}
		}
		if(prevEmulationFile.Compare("NULL") != 0){
			if ( settingFile.Open( _T(prevEmulationFile),CFile::modeReadWrite ) )
			{

				ULONGLONG dwLength = settingFile.GetLength();
				BYTE *buffer = (BYTE *) malloc(dwLength + 1); 

				settingFile.Read(buffer, dwLength); // read whole file
				*(buffer + dwLength) = '\0';   // add null

				ePage->getListBox()->DeleteAllItems();
				curChar = (char*)buffer;
				int commaCount = 0;
				int nIndex;
				while(*curChar != '\0'){
					if(*curChar == '\n'){
						lineBuffer[i] = '\0';
						i = 0;
						curChar++;
						CString line(lineBuffer);
						line.Remove('\r');
						commaCount++;
						if(commaCount ==  5){
							ePage->getListBox()->SetItemText(nIndex, 4, _T(line));
						}else if(commaCount == 3){
							ePage->getListBox()->SetItemText(nIndex, 2, _T(line));
						}else if(commaCount == 4){
							if(line.Compare("True") == 0){
								ePage->getListBox()->SetItemText(nIndex, 0, _T("Enabled"));
							}else{
								ePage->getListBox()->SetItemText(nIndex, 0, _T("Disabled"));
							}
						}
						commaCount = 0;
					}else if(*curChar == ','){
						lineBuffer[i] = '\0';
						i = 0;
						curChar++;
						commaCount++;
						CString line(lineBuffer);
						if(commaCount == 1){
							nIndex = ePage->getListBox()->InsertItem(ePage->getListBox()->GetItemCount(), _T(line));	
							ePage->getListBox()->SetItemText(nIndex, 1, _T(line));
						}else if(commaCount == 2){
							ePage->getListBox()->SetItemText(nIndex, 3, _T(line));
						}else if(commaCount == 3){
							ePage->getListBox()->SetItemText(nIndex, 2, _T(line));
						}else if(commaCount == 4){
							if(line.Compare("True") == 0){
								ePage->getListBox()->SetItemText(nIndex, 0, _T("Enabled"));
							}else{
								ePage->getListBox()->SetItemText(nIndex, 0, _T("Disabled"));
							}
						}else if(commaCount == 5){
							ePage->getListBox()->SetItemText(nIndex, 4, _T(line));
						}
					}else{
						lineBuffer[i] = *curChar;
						i++;
						curChar++;
					}
				}
				lineBuffer[i] = '\0';
				i = 0;
				curChar++;
				CString line(lineBuffer);
				if(commaCount ==  5){
					pPage->getListBox()->SetItemText(nIndex, 4, _T(line));
				}else if(commaCount == 3){
					pPage->getListBox()->SetItemText(nIndex, 2, _T(line));
				}else if(commaCount == 4){
					if(line.Compare("True") == 0){
						pPage->getListBox()->SetItemText(nIndex, 0, _T("Enabled"));
					}else{
						pPage->getListBox()->SetItemText(nIndex, 0, _T("Disabled"));
					}
				}
				commaCount = 0;
				settingFile.Close();
			}
		}
		if(prevGroupFile.Compare("NULL") != 0){
			if (settingFile.Open( _T(prevGroupFile),CFile::modeReadWrite ) )
			{
				ULONGLONG dwLength = settingFile.GetLength();
				BYTE *buffer = (BYTE *) malloc(dwLength + 1); 

				settingFile.Read(buffer, dwLength); // read whole file
				*(buffer + dwLength) = '\0';   // add null

				gPage->getListBox()->DeleteAllItems();
				curChar = (char*)buffer;
				int spaceCount = 0;
				int nIndex = 0;
				bool old = false;
				while(*curChar != '\0'){
					if(*curChar == '\n'){
						lineBuffer[i] = '\0';
						i = 0;
						curChar++;
						CString line(lineBuffer);
						line.Remove('\r');
						gPage->getListBox()->SetItemText(nIndex, 3, _T(line));
						spaceCount = 0;
						old = false;
					}else if(*curChar == ' ' && old == false){
						lineBuffer[i] = '\0';
						i = 0;
						curChar++;
						spaceCount++;
						CString line(lineBuffer);
						if(spaceCount == 1){
							nIndex = gPage->getListBox()->InsertItem(gPage->getListBox()->GetItemCount(), _T(line));	
							line.Remove(' ');
							if(line.Compare("Enabled") == 0 || line.Compare("Disabled") == 0){
								gPage->getListBox()->SetItemText(nIndex, 0, _T(line));
							}else{
								old = true;
								gPage->getListBox()->SetItemText(nIndex, 0, _T("Enabled"));
								gPage->getListBox()->SetItemText(nIndex, 2, _T(line));
							}
						}else if(spaceCount == 2){
							gPage->getListBox()->SetItemText(nIndex, 2, _T(line));
							old = true;
						}
					}else if(*curChar == '"'){
						lineBuffer[i] = '\0';
						i = 0;
						curChar++;
						CString line(lineBuffer);
						line.Remove('\t');
						gPage->getListBox()->SetItemText(nIndex, 1, _T(line));
					}else{
						lineBuffer[i] = *curChar;
						i++;
						curChar++;
					}
				}
				i = 0;
				curChar++;
				CString line(lineBuffer);
				line.Remove('\r');
				gPage->getListBox()->SetItemText(nIndex, 3, _T(line));
				spaceCount = 0;
				settingFile.Close();
			}
		}
		if(prevFilterFile.Compare("NULL") != 0){
			if (settingFile.Open( _T(prevFilterFile),CFile::modeReadWrite ) )
			{
				ULONGLONG dwLength = settingFile.GetLength();
				BYTE *buffer = (BYTE *) malloc(dwLength + 1); 
				settingFile.Read(buffer, dwLength); // read whole file
				*(buffer + dwLength) = '\0';   // add null
				curChar = (char*)buffer;
				int commaCount = 0;
				int nIndex;
				while(*curChar != '\0'){
					if(*curChar == '\n'){
						lineBuffer[i] = '\0';
						i = 0;
						curChar++;
						CString line(lineBuffer);
						line.Remove('\r');
						if(commaCount == 2){
							if(line.Compare("True") == 0){
								filterPage->getListBox()->SetItemText(nIndex, 0, _T("Enabled"));
							}else{
								filterPage->getListBox()->SetItemText(nIndex, 0, _T("Disabled"));
							}
						}else if(commaCount == 3){
							filterPage->getListBox()->SetItemText(nIndex,3,_T(line));
						}
						commaCount = 0;
					}else if(*curChar == ','){
						lineBuffer[i] = '\0';
						i = 0;
						curChar++;
						commaCount++;
						CString line(lineBuffer);
						if(commaCount == 1){
							nIndex = filterPage->getListBox()->InsertItem(filterPage->getListBox()->GetItemCount(), _T(line));	
							filterPage->getListBox()->SetItemText(nIndex, 2, _T(line));
						}else if(commaCount == 2){
							filterPage->getListBox()->SetItemText(nIndex, 1, _T(line));
						}else if(commaCount == 3){
							if(line.Compare("True") == 0){
								filterPage->getListBox()->SetItemText(nIndex, 0, _T("Enabled"));
							}else{
								filterPage->getListBox()->SetItemText(nIndex, 0, _T("Disabled"));
							}
						}
					}else{
						lineBuffer[i] = *curChar;
						i++;
						curChar++;
					}
				}
				lineBuffer[i] = '\0';
				i = 0;
				curChar++;
				CString line(lineBuffer);
				line.Remove('\r');
				if(line.Compare("True") == 0){
					filterPage->getListBox()->SetItemText(nIndex, 0, _T("Enabled"));
				}else{
					filterPage->getListBox()->SetItemText(nIndex, 0, _T("Disabled"));
				}
				settingFile.Close();
			}
		}
		if(prevPeriodicFile.Compare("NULL") != 0){
			if (settingFile.Open( _T(prevPeriodicFile),CFile::modeReadWrite ) )
			{
				ULONGLONG dwLength = settingFile.GetLength();
				BYTE *buffer = (BYTE *) malloc(dwLength + 1); 

				settingFile.Read(buffer, dwLength); // read whole file
				*(buffer + dwLength) = '\0';   // add null

				pPage->getListBox()->DeleteAllItems();
				curChar = (char*)buffer;
				int commaCount = 0;
				int nIndex;
				while(*curChar != '\0'){
					if(*curChar == '\n'){
						lineBuffer[i] = '\0';
						i = 0;
						curChar++;
						CString line(lineBuffer);
						line.Remove('\r');
						if(commaCount ==  5){
							pPage->getListBox()->SetItemText(nIndex, 5, _T(line));
						}else if(commaCount == 3){
							if(line.Compare("True") == 0){
								pPage->getListBox()->SetItemText(nIndex, 0, _T("Enabled"));
							}else{
								pPage->getListBox()->SetItemText(nIndex, 0, _T("Disabled"));
							}
							pPage->getListBox()->SetItemText(nIndex,3,_T("0"));
						}else if(commaCount == 4){
							pPage->getListBox()->SetItemText(nIndex, 3, _T(line));
						}
						commaCount = 0;
					}else if(*curChar == ','){
						lineBuffer[i] = '\0';
						i = 0;
						curChar++;
						commaCount++;
						CString line(lineBuffer);
						if(commaCount == 1){
							nIndex = pPage->getListBox()->InsertItem(pPage->getListBox()->GetItemCount(), _T(line));	
							pPage->getListBox()->SetItemText(nIndex, 4, _T(line));
						}else if(commaCount == 2){
							pPage->getListBox()->SetItemText(nIndex, 1, _T(line));
						}else if(commaCount == 3){
							pPage->getListBox()->SetItemText(nIndex, 2, _T(line));
						}else if(commaCount == 4){
							if(line.Compare("True") == 0){
								pPage->getListBox()->SetItemText(nIndex, 0, _T("Enabled"));
							}else{
								pPage->getListBox()->SetItemText(nIndex, 0, _T("Disabled"));
							}
						}else if(commaCount == 5){
							pPage->getListBox()->SetItemText(nIndex, 3, _T(line));
						}
					}else{
						lineBuffer[i] = *curChar;
						i++;
						curChar++;
					}
				}
				lineBuffer[i] = '\0';
				i = 0;
				curChar++;
				CString line(lineBuffer);
				if(commaCount ==  5){
					pPage->getListBox()->SetItemText(nIndex, 5, _T(line));
				}else if(commaCount == 3){
					if(line.Compare("True") == 0){
						pPage->getListBox()->SetItemText(nIndex, 0, _T("Enabled"));
					}else{
						pPage->getListBox()->SetItemText(nIndex, 0, _T("Disabled"));
					}
					pPage->getListBox()->SetItemText(nIndex,3,_T("0"));
				}else if(commaCount == 4){
					pPage->getListBox()->SetItemText(nIndex, 3, _T(line));
				}
				commaCount = 0;
				settingFile.Close();
			}
		}
		if(prevExcelFile.Compare("NULL") != 0){
			CDatabase database;
			CString sSql;
			CString sItem1, sItem2, sItem3;
			CString sDriver;
			CString sDsn;
			CString sFile = prevExcelFile;  
			sDriver = GetExcelDriver();
			if( sDriver.IsEmpty() )
			{
				return;
			}
			sDsn.Format("ODBC;DRIVER={%s};DSN='';DBQ=%s",sDriver,sFile);
			TRY
			{
				database.Open(NULL,false,false,sDsn);

				CRecordset recset( &database );
				sSql = "select * from [" + Fault_Code_Excel_Page_Name + "$]";
				recset.Open(CRecordset::forwardOnly,sSql,CRecordset::readOnly);
				// Browse the result
				while( !recset.IsEOF() )
				{

					recset.GetFieldValue("Fault_Code",sItem1);
					std::string std1(sItem1, sItem1.GetLength());
					recset.GetFieldValue("DTC",sItem2);
					std::string std2(sItem2, sItem2.GetLength());
					recset.GetFieldValue("Type",sItem3);
					std::string std3(sItem3, sItem3.GetLength());
					FaultCode temp(std1,std2,std3);
					faultcodes[std2].push_back(temp);

					recset.MoveNext();
				}
				database.Close();
                             
			}
			CATCH(CDBException, e)
			{
				return;
			}
			END_CATCH;
		}

	}
}
void CHardwareListing::writeSettingsFile(){
	CFile cfile;
	cfile.Open(_T(dir + "\\saint.dat"), CFile::modeCreate | CFile::modeReadWrite);
	if(prevExcelFile == ""){
		prevExcelFile = "NULL";
	}
	if(prevPeriodicFile == ""){
		prevPeriodicFile = "NULL";
	}
	if(prevMessageFile == ""){
		prevMessageFile = "NULL";
	}
	if(prevFilterFile == ""){
		prevFilterFile = "NULL";
	}
	if(prevEmulationFile == ""){
		prevEmulationFile = "NULL";
	}
	if(prevGroupFile == ""){
		prevGroupFile = "NULL";
	}
	CString strtowrite(prevMessageFile + "," + prevExcelFile + "," + prevPeriodicFile + "," + prevFilterFile + "," + prevEmulationFile + "," + prevGroupFile + ",");
	cfile.Write(strtowrite, strtowrite.GetLength());         
	cfile.Flush();
}
void CHardwareListing::OnChangeToGroup(){
	CFileDialog *dlg = new CFileDialog(TRUE, "grp", NULL, NULL,"Group Files (*.grp)|*.grp");
	if(dlg->DoModal() == IDOK)
	{
		ePage->editTransmit.SetWindowTextA(strdup(dlg->GetPathName())); // return full path and filenam
	}else{
		return;
	}
	delete dlg;
	return;
}
bool CHardwareListing::passesFilter(CString str){
	if(!filterPage){
		return true;
	}
	if(!filterPage->transmitting){
		return true;
	}
	bool isExcluded = false;
	str.Remove(' ');
	for(int i = 0; i < 28-str.GetLength();i++){
		str += "X";
	}

	for(int i = 0; i < filterPage->amtOfExcludes;i++){
		for(int j = 0; j < filterPage->listOfExcludes[i].GetLength();j++){
			if(filterPage->listOfExcludes[i].GetAt(j) != str.GetAt(j)){
				if(filterPage->listOfExcludes[i].GetAt(j) != 'X' &&  str.GetAt(j) != 'X'){
					break;
				}
			}
			if(j == filterPage->listOfExcludes[i].GetLength()-1){
				isExcluded = true;
			}
		}
		if(isExcluded){
			break;
		}
	}
	if(isExcluded == false){
		return true;
	}else{ 
		for(int i = 0; i < filterPage->amtOfIncludes;i++){
			for(int j = 0; j < filterPage->listOfIncludes[i].GetLength();j++){
				if(filterPage->listOfIncludes[i].GetAt(j) != str.GetAt(j)){
					if(filterPage->listOfIncludes[i].GetAt(j) != 'X' &&  str.GetAt(j) != 'X'){
						break;
					}
				}
				if(j == filterPage->listOfExcludes[i].GetLength()-1){
					return true;
				}
			}
		}
	}
	return false;
}
void CHardwareListing::handleEmulations(CString str){
	if(ePage){
		ePage->handleEmulations(str);
	}
}
BOOL CHardwareListing::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

	
	mPage = new MainPageDialog(this,this);
	fPage = new FaultCodes(this);
	pPage = new PeriodicMessages(this);
	ePage = new Emulation(this);
	filterPage = new Filter(this);
	gPage = new GroupEditor(this);



	AutoMove(IDC_TAB1, 0, 0, 100, 100);
	AutoMove(IDC_LABEL, 0, 100, 0, 0);


	mPage->Create(MainPageDialog::IDD, &tabCtrl);
	fPage->Create(FaultCodes::IDD, &tabCtrl);
	pPage->Create(PeriodicMessages::IDD, &tabCtrl);
	filterPage->Create(Filter::IDD,&tabCtrl);
	ePage->Create(Emulation::IDD,&tabCtrl);
	gPage->Create(GroupEditor::IDD,&tabCtrl);

	mPage->ShowWindow(SW_SHOW);
	fPage->ShowWindow(SW_HIDE);
	pPage->ShowWindow(SW_HIDE);
	filterPage->ShowWindow(SW_HIDE);
	ePage->ShowWindow(SW_HIDE);
	gPage->ShowWindow(SW_HIDE);
	readSettingsFile();
	SetRectangle();
	repaintTabs();




	return TRUE;

}

void CHardwareListing::OnOK()
{
	CResizableDialog::OnOK();
}
void CHardwareListing::OnCancel()
{
	saintApp->unloadDriver();
	return;
}


void CHardwareListing::finishedFault(CString mem){
	fPage->finishedFault(mem);
}

stdext::hash_map<std::string, std::vector<FaultCode>> CHardwareListing::getFault(){
	return faultcodes;
}
void CHardwareListing::OnBnLoadGrpFile(){
	CFileDialog dlg(TRUE);
	dlg.m_ofn.nMaxFile = 511;
	dlg.m_ofn.lpstrFilter="Grp Files (*.grp)\0*.grp\0";
	dlg.m_ofn.lpstrTitle="Open Group File";


	if(dlg.DoModal() == IDOK)
	{
		filename = dlg.GetPathName(); // return full path and filename

	}else{
		return;
	}
	prevGroupFile = filename;
	
	writeSettingsFile();
	char lineBuffer[1000];
	int i = 0;
	CFile	myFile;
	char * curChar;

	char stringToSave[256];
	strcpy(stringToSave, (LPCTSTR)filename);
	if ( myFile.Open( _T(filename),CFile::modeReadWrite ) )
	{

		ULONGLONG dwLength = myFile.GetLength();
		BYTE *buffer = (BYTE *) malloc(dwLength + 1); 

		myFile.Read(buffer, dwLength); // read whole file
		*(buffer + dwLength) = '\0';   // add null

		gPage->getListBox()->DeleteAllItems();
		curChar = (char*)buffer;
		int spaceCount = 0;
		int nIndex = 0;
		bool old = false;
		while(*curChar != '\0'){
			if(*curChar == '\n'){
				lineBuffer[i] = '\0';
				i = 0;
				curChar++;
				CString line(lineBuffer);
				line.Remove('\r');
				gPage->getListBox()->SetItemText(nIndex, 3, _T(line));
				spaceCount = 0;
				old = false;
			}else if(*curChar == ' ' && old == false){
				lineBuffer[i] = '\0';
				i = 0;
				curChar++;
				spaceCount++;
				CString line(lineBuffer);
				if(spaceCount == 1){
					nIndex = gPage->getListBox()->InsertItem(gPage->getListBox()->GetItemCount(), _T(line));	
					line.Remove(' ');
					if(line.Compare("Enabled") == 0 || line.Compare("Disabled") == 0){
						gPage->getListBox()->SetItemText(nIndex, 0, _T(line));
					}else{
						old = true;
						gPage->getListBox()->SetItemText(nIndex, 0, _T("Enabled"));
						gPage->getListBox()->SetItemText(nIndex, 2, _T(line));
					}
				}else if(spaceCount == 2){
					gPage->getListBox()->SetItemText(nIndex, 2, _T(line));
					old = true;
				}
			}else if(*curChar == '"'){
				lineBuffer[i] = '\0';
				i = 0;
				curChar++;
				CString line(lineBuffer);
				line.Remove('\t');
				gPage->getListBox()->SetItemText(nIndex, 1, _T(line));
			}else{
				lineBuffer[i] = *curChar;
				i++;
				curChar++;
			}
		}
		i = 0;
		curChar++;
		CString line(lineBuffer);
		line.Remove('\r');
		gPage->getListBox()->SetItemText(nIndex, 3, _T(line));
		spaceCount = 0;
	}else{
		AfxMessageBox("File failed to open");
		return;
	}
	repaintTabs();
}
void CHardwareListing::OnBnLoadPerFile(){
	CFileDialog dlg(TRUE);
	dlg.m_ofn.nMaxFile = 511;
	dlg.m_ofn.lpstrFilter="Per Files (*.per)\0*.per\0";
	dlg.m_ofn.lpstrTitle="Open Periodic File";


	if(dlg.DoModal() == IDOK)
	{
		filename = dlg.GetPathName(); // return full path and filename

	}else{
		return;
	}
	prevPeriodicFile = filename;
	
	writeSettingsFile();
	char lineBuffer[1000];
	int i = 0;
	CFile	myFile;
	char * curChar;

	char stringToSave[256];
	strcpy(stringToSave, (LPCTSTR)filename);
	if ( myFile.Open( _T(filename),CFile::modeReadWrite ) )
	{

		ULONGLONG dwLength = myFile.GetLength();
		BYTE *buffer = (BYTE *) malloc(dwLength + 1); 

		myFile.Read(buffer, dwLength); // read whole file
		*(buffer + dwLength) = '\0';   // add null

		pPage->getListBox()->DeleteAllItems();
		curChar = (char*)buffer;
		int commaCount = 0;
		int nIndex;
		while(*curChar != '\0'){
			if(*curChar == '\n'){
				lineBuffer[i] = '\0';
				i = 0;
				curChar++;
				CString line(lineBuffer);
				line.Remove('\r');
				if(commaCount ==  5){
					pPage->getListBox()->SetItemText(nIndex, 5, _T(line));
				}else if(commaCount == 3){
					if(line.Compare("True") == 0){
						pPage->getListBox()->SetItemText(nIndex, 0, _T("Enabled"));
					}else{
						pPage->getListBox()->SetItemText(nIndex, 0, _T("Disabled"));
					}
					pPage->getListBox()->SetItemText(nIndex,3,_T("0"));
				}else if(commaCount == 4){
					pPage->getListBox()->SetItemText(nIndex, 3, _T(line));
				}
				commaCount = 0;
			}else if(*curChar == ','){
				lineBuffer[i] = '\0';
				i = 0;
				curChar++;
				commaCount++;
				CString line(lineBuffer);
				if(commaCount == 1){
					nIndex = pPage->getListBox()->InsertItem(pPage->getListBox()->GetItemCount(), _T(line));	
					pPage->getListBox()->SetItemText(nIndex, 4, _T(line));
				}else if(commaCount == 2){
					pPage->getListBox()->SetItemText(nIndex, 1, _T(line));
				}else if(commaCount == 3){
					pPage->getListBox()->SetItemText(nIndex, 2, _T(line));
				}else if(commaCount == 4){
					if(line.Compare("True") == 0){
						pPage->getListBox()->SetItemText(nIndex, 0, _T("Enabled"));
					}else{
						pPage->getListBox()->SetItemText(nIndex, 0, _T("Disabled"));
					}
				}else if(commaCount == 5){
					pPage->getListBox()->SetItemText(nIndex, 3, _T(line));
				}
			}else{
				lineBuffer[i] = *curChar;
				i++;
				curChar++;
			}
		}
		lineBuffer[i] = '\0';
		i = 0;
		curChar++;
		CString line(lineBuffer);
		if(commaCount ==  5){
			pPage->getListBox()->SetItemText(nIndex, 5, _T(line));
		}else if(commaCount == 3){
			if(line.Compare("True") == 0){
				pPage->getListBox()->SetItemText(nIndex, 0, _T("Enabled"));
			}else{
				pPage->getListBox()->SetItemText(nIndex, 0, _T("Disabled"));
			}
			pPage->getListBox()->SetItemText(nIndex,3,_T("0"));
		}else if(commaCount == 4){
			pPage->getListBox()->SetItemText(nIndex, 3, _T(line));
		}
		commaCount = 0;
	}else{
		AfxMessageBox("File failed to open");
		return;
	}
	repaintTabs();
}
void CHardwareListing::OnBnLoadEmuFile(){
	CFileDialog dlg(TRUE);
	dlg.m_ofn.nMaxFile = 511;
	dlg.m_ofn.lpstrFilter="Emu Files (*.per)\0*.emu\0";
	dlg.m_ofn.lpstrTitle="Open Emulation File";


	if(dlg.DoModal() == IDOK)
	{
		filename = dlg.GetPathName(); // return full path and filename

	}else{
		return;
	}
	prevEmulationFile = filename;
	
	writeSettingsFile();
	char lineBuffer[1000];
	int i = 0;
	CFile	myFile;
	char * curChar;

	char stringToSave[256];
	strcpy(stringToSave, (LPCTSTR)filename);
	if ( myFile.Open( _T(filename),CFile::modeReadWrite ) )
	{

		ULONGLONG dwLength = myFile.GetLength();
		BYTE *buffer = (BYTE *) malloc(dwLength + 1); 

		myFile.Read(buffer, dwLength); // read whole file
		*(buffer + dwLength) = '\0';   // add null

		ePage->getListBox()->DeleteAllItems();
		curChar = (char*)buffer;
		int commaCount = 0;
		int nIndex;
		while(*curChar != '\0'){
			if(*curChar == '\n'){
				lineBuffer[i] = '\0';
				i = 0;
				curChar++;
				CString line(lineBuffer);
				line.Remove('\r');
				commaCount++;
				if(commaCount ==  5){
					ePage->getListBox()->SetItemText(nIndex, 4, _T(line));
				}else if(commaCount == 3){
					ePage->getListBox()->SetItemText(nIndex, 2, _T(line));
				}else if(commaCount == 4){
					if(line.Compare("True") == 0){
						ePage->getListBox()->SetItemText(nIndex, 0, _T("Enabled"));
					}else{
						ePage->getListBox()->SetItemText(nIndex, 0, _T("Disabled"));
					}
				}
				commaCount = 0;
			}else if(*curChar == ','){
				lineBuffer[i] = '\0';
				i = 0;
				curChar++;
				commaCount++;
				CString line(lineBuffer);
				if(commaCount == 1){
					nIndex = ePage->getListBox()->InsertItem(ePage->getListBox()->GetItemCount(), _T(line));	
					ePage->getListBox()->SetItemText(nIndex, 1, _T(line));
				}else if(commaCount == 2){
					ePage->getListBox()->SetItemText(nIndex, 3, _T(line));
				}else if(commaCount == 3){
					ePage->getListBox()->SetItemText(nIndex, 2, _T(line));
				}else if(commaCount == 4){
					if(line.Compare("True") == 0){
						ePage->getListBox()->SetItemText(nIndex, 0, _T("Enabled"));
					}else{
						ePage->getListBox()->SetItemText(nIndex, 0, _T("Disabled"));
					}
				}else if(commaCount == 5){
					ePage->getListBox()->SetItemText(nIndex, 4, _T(line));
				}
			}else{
				lineBuffer[i] = *curChar;
				i++;
				curChar++;
			}
		}
		lineBuffer[i] = '\0';
		i = 0;
		curChar++;
		CString line(lineBuffer);
		if(commaCount ==  5){
			pPage->getListBox()->SetItemText(nIndex, 4, _T(line));
		}else if(commaCount == 3){
			pPage->getListBox()->SetItemText(nIndex, 2, _T(line));
		}else if(commaCount == 4){
			if(line.Compare("True") == 0){
				pPage->getListBox()->SetItemText(nIndex, 0, _T("Enabled"));
			}else{
				pPage->getListBox()->SetItemText(nIndex, 0, _T("Disabled"));
			}
		}
		commaCount = 0;
	}else{
		AfxMessageBox("File failed to open");
		return;
	}
	repaintTabs();
}



void CHardwareListing::OnBnLoadFltFile(){
	CFileDialog dlg(TRUE);
	dlg.m_ofn.nMaxFile = 511;
	dlg.m_ofn.lpstrFilter="Flt Files (*.flt)\0*.flt\0";
	dlg.m_ofn.lpstrTitle="Open Filter File";

	if(dlg.DoModal() == IDOK)
	{
		filename = dlg.GetPathName(); // return full path and filename

	}else{
		return;
	}
	prevFilterFile = filename;
	
	writeSettingsFile();
	char lineBuffer[1000];
	int i = 0;
	CFile	myFile;
	char * curChar;

	char stringToSave[256];
	strcpy(stringToSave, (LPCTSTR)filename);
	if(myFile.Open( _T(filename),CFile::modeReadWrite)){
		ULONGLONG dwLength = myFile.GetLength();
		BYTE *buffer = (BYTE *) malloc(dwLength + 1); 
		myFile.Read(buffer, dwLength); // read whole file
		*(buffer + dwLength) = '\0';   // add null
		curChar = (char*)buffer;
		filterPage->listControl.DeleteAllItems();
		int commaCount = 0;
		int nIndex;
		while(*curChar != '\0'){
			if(*curChar == '\n'){
				lineBuffer[i] = '\0';
				i = 0;
				curChar++;
				CString line(lineBuffer);
				line.Remove('\r');
				if(commaCount == 2){
					if(line.Compare("True") == 0){
						filterPage->getListBox()->SetItemText(nIndex, 0, _T("Enabled"));
					}else{
						filterPage->getListBox()->SetItemText(nIndex, 0, _T("Disabled"));
					}
				}else if(commaCount == 3){
					filterPage->getListBox()->SetItemText(nIndex,3,_T(line));
				}
				commaCount = 0;
			}else if(*curChar == ','){
				lineBuffer[i] = '\0';
				i = 0;
				curChar++;
				commaCount++;
				CString line(lineBuffer);
				if(commaCount == 1){
					nIndex = filterPage->getListBox()->InsertItem(filterPage->getListBox()->GetItemCount(), _T(line));	
					filterPage->getListBox()->SetItemText(nIndex, 2, _T(line));
				}else if(commaCount == 2){
					filterPage->getListBox()->SetItemText(nIndex, 1, _T(line));
				}else if(commaCount == 3){
					if(line.Compare("True") == 0){
						filterPage->getListBox()->SetItemText(nIndex, 0, _T("Enabled"));
					}else{
						filterPage->getListBox()->SetItemText(nIndex, 0, _T("Disabled"));
					}
				}
			}else{
				lineBuffer[i] = *curChar;
				i++;
				curChar++;
			}
		}
		lineBuffer[i] = '\0';
		i = 0;
		curChar++;
		CString line(lineBuffer);
		line.Remove('\r');
		if(line.Compare("True") == 0){
			filterPage->getListBox()->SetItemText(nIndex, 0, _T("Enabled"));
		}else{
			filterPage->getListBox()->SetItemText(nIndex, 0, _T("Disabled"));
		}
		myFile.Close();
	}else{
		AfxMessageBox("File failed to open");
		return;
	}
	repaintTabs();
}


void CHardwareListing::OnBnClickedButton1()
{
	CFileDialog dlg(TRUE);
	dlg.m_ofn.nMaxFile = 511;
	dlg.m_ofn.lpstrFilter="Msg Files (*.msg)\0*.msg\0";
	dlg.m_ofn.lpstrTitle="Open Message File";

	if(dlg.DoModal() == IDOK)
	{
		filename = dlg.GetPathName(); // return full path and filename

	}else{
		return;
	}
	prevMessageFile = filename;
	writeSettingsFile();
	char lineBuffer[1000];
	int i = 0;
	CFile	myFile;
	char * curChar;
	char stringToSave[256];
	strcpy(stringToSave, (LPCTSTR)filename);
	if ( myFile.Open( _T(filename),CFile::modeReadWrite ) )
	{
		mPage->getMessageListBox()->ResetContent();
		ULONGLONG dwLength = myFile.GetLength();
		BYTE *buffer = (BYTE *) malloc(dwLength + 1); 
		myFile.Read(buffer, dwLength); // read whole file
		*(buffer + dwLength) = '\0';   // add null

		curChar = (char*)buffer;
		while(*curChar != '\0'){
			if(*curChar == '\n'){
				if(i == 0){
					mPage->getMessageListBox()->AddString(" ");
				}else{
					lineBuffer[i] = '\0';
					i = 0;
					curChar++;
					CString line(lineBuffer);
					mPage->getMessageListBox()->AddString(line);
				}
			}else{
				lineBuffer[i] = *curChar;
				i++;
				curChar++;
			}
		}
	}else{
		AfxMessageBox("File failed to open");
		return;
	}
	repaintTabs();
}
void CHardwareListing::faultsFailed(){
	fPage->faultFailed();
}
bool CHardwareListing::OnBnClickedButton2()
{
	return CAN_SendFaultCodeRequest();
}


CString GetExcelDriver()
{
    char szBuf[2001];
    WORD cbBufMax = 2000;
    WORD cbBufOut;
    char *pszBuf = szBuf;
    CString sDriver;
   if(!SQLGetInstalledDrivers(szBuf,cbBufMax,& cbBufOut))
        return "";
    do {
        if( strstr( pszBuf, "Excel" ) != 0 ) {
            // Found !
            sDriver = CString( pszBuf );
            break;
        }
        pszBuf = strchr( pszBuf, '\0' ) + 1;
    }
    while( pszBuf[1] != '\0' );
    return sDriver;
}


void CHardwareListing::OnBnClickedButton3()
{
	CFileDialog dlg(TRUE);
	dlg.m_ofn.nMaxFile = 300;
	dlg.m_ofn.lpstrFilter="Excel Files (*.xls)\0*.xls\0";
	dlg.m_ofn.lpstrTitle="Open Excel File";

	if(dlg.DoModal() == IDOK)
	{
		filename = dlg.GetPathName(); // return full path and filename

		prevExcelFile = filename;
		writeSettingsFile();
	}else{
		return;
	}

	CDatabase database;
    CString sSql;
    CString sItem1, sItem2, sItem3;
    CString sDriver;
    CString sDsn;
    CString sFile = filename;      
    sDriver = GetExcelDriver();
    if( sDriver.IsEmpty() )
    {
        AfxMessageBox("No Excel ODBC driver found");
        return;
    }

    sDsn.Format("ODBC;DRIVER={%s};DSN='';DBQ=%s",sDriver,sFile);
    TRY
    {
        database.Open(NULL,false,false,sDsn);

        CRecordset recset( &database );
		sSql = "select * from [" + Fault_Code_Excel_Page_Name + "$]";
        recset.Open(CRecordset::forwardOnly,sSql,CRecordset::readOnly);
        // Browse the result
        while( !recset.IsEOF() )
		{

            recset.GetFieldValue("Fault_Code",sItem1);
			std::string std1(sItem1, sItem1.GetLength());
            recset.GetFieldValue("DTC",sItem2);
			std::string std2(sItem2, sItem2.GetLength());
			recset.GetFieldValue("Type",sItem3);
			std::string std3(sItem3, sItem3.GetLength());
			FaultCode temp(std1,std2,std3);
			faultcodes[std2].push_back(temp);

            recset.MoveNext();
        }
        database.Close();
                             
    }
    CATCH(CDBException, e)
    {
        AfxMessageBox("Database error: "+e->m_strError);
		return;
    }
    END_CATCH;
	 AfxMessageBox("Successfully opened excel sheet");
	 repaintTabs();
}
FaultCode::FaultCode(){

}

FaultCode::FaultCode(std::string ft,std::string d, std::string t){
	faultCode = ft;
	DTC = d;
	type = t;
}

BOOL CHardwareListing::DestroyWindow()
{
	return CDialog::DestroyWindow();
}


void CHardwareListing::OnSize(UINT nType, int cx, int cy)
{
	if(mPage){
		CResizableDialog::OnSize(nType, cx, cy);
		repaintTabs();
		CHardwareListing::BringWindowToTop();
	}
}
void CHardwareListing::SetRectangle()
{
	
	RECT r;
	TCITEM tci;

	GetClientRect(&r);

	tci.mask = TCIF_TEXT;
	tci.iImage = -1;

	tci.pszText = "Main";
	tabCtrl.InsertItem(0, &tci);

	tci.pszText = "Filters";
	tabCtrl.InsertItem(1, &tci);

	tci.pszText = "Group";
	tabCtrl.InsertItem(2, &tci);

	tci.pszText = "Periodic";
	tabCtrl.InsertItem(3, &tci);

	tci.pszText = "Emulation";
	tabCtrl.InsertItem(4, &tci);

	tci.pszText = "Fault Codes";
	tabCtrl.InsertItem(5, &tci);

	OnTcnSelchangeTab1(NULL, NULL);
}

void CHardwareListing::repaintTabs(){
	RECT rc;
	tabCtrl.GetItemRect(0,&rc);
	int nIndex = tabCtrl.GetCurSel();
	RECT temp;
	tabCtrl.GetClientRect(&temp);
	if(nIndex == 0){
		mPage->SetWindowPos( NULL, rc.left + 1, rc.bottom + 1, temp.right-temp.left-5, temp.bottom-temp.top-25,  SWP_NOZORDER | SWP_SHOWWINDOW);
		mPage->SetFocus();
		if(prevMessageFile.Compare("NULL") == 0){
			labelFile.SetWindowTextA("Message File - ");
		}else{
			labelFile.SetWindowTextA("Message File - " + prevMessageFile);
		}
	}else if(nIndex == 1){
		filterPage->SetWindowPos( NULL, rc.left + 1, rc.bottom + 1, temp.right-temp.left-5, temp.bottom-temp.top-25,  SWP_NOZORDER | SWP_SHOWWINDOW);
		filterPage->SetFocus();
		if(prevFilterFile.Compare("NULL") == 0){
			labelFile.SetWindowTextA("Filter File - " );
		}else{
			labelFile.SetWindowTextA("Filter File - " + prevFilterFile);
		}
	}else if(nIndex == 2){
		gPage->SetWindowPos( NULL, rc.left + 1, rc.bottom + 1, temp.right-temp.left-5, temp.bottom-temp.top-25,  SWP_NOZORDER | SWP_SHOWWINDOW);
		gPage->SetFocus();
		if(prevGroupFile.Compare("NULL") == 0){
			labelFile.SetWindowTextA("Group File - " );
		}else{
			labelFile.SetWindowTextA("Group File - " + prevGroupFile);
		}
	}else if(nIndex == 3){
		pPage->SetWindowPos( NULL, rc.left + 1, rc.bottom + 1, temp.right-temp.left-5, temp.bottom-temp.top-25,  SWP_NOZORDER | SWP_SHOWWINDOW);
		pPage->SetFocus();
		if(prevPeriodicFile.Compare("NULL") == 0){
			labelFile.SetWindowTextA("Periodic File - " );
		}else{
			labelFile.SetWindowTextA("Periodic File - " + prevPeriodicFile);
		}
	}else if(nIndex == 4){
		ePage->SetWindowPos( NULL, rc.left + 1, rc.bottom + 1, temp.right-temp.left-5, temp.bottom-temp.top-25,  SWP_NOZORDER | SWP_SHOWWINDOW);
		ePage->SetFocus();
		if(prevEmulationFile.Compare("NULL") == 0){
			labelFile.SetWindowTextA("Emulation File - " );
		}else{
			labelFile.SetWindowTextA("Emulation File - " + prevEmulationFile);
		}
	}else if(nIndex == 5){
		fPage->SetWindowPos( NULL, rc.left + 1, rc.bottom + 1, temp.right-temp.left-5, temp.bottom-temp.top-25,  SWP_NOZORDER | SWP_SHOWWINDOW);
		fPage->SetFocus();
		if(prevExcelFile.Compare("NULL") == 0){
			labelFile.SetWindowTextA("Excel File - ");
		}else{
			labelFile.SetWindowTextA("Excel File - " + prevExcelFile);
		}
	}
}


void CHardwareListing::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	repaintTabs();
}


void CHardwareListing::OnTcnSelchangingTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nIndex = tabCtrl.GetCurSel();

	if(nIndex == 0){
		mPage->ShowWindow(SW_HIDE);
	}else if(nIndex == 1){
		filterPage->ShowWindow(SW_HIDE);
	}else if(nIndex == 2){
		gPage->ShowWindow(SW_HIDE);
	}else if(nIndex == 3){
		pPage->ShowWindow(SW_HIDE);
	}else if(nIndex == 4){
		ePage->ShowWindow(SW_HIDE);
	}else if(nIndex == 5){
		fPage->ShowWindow(SW_HIDE);
	}
	SetMenu(NULL);
	*pResult = 0;
}