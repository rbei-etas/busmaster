// MainPageDialog.cpp : implementation file
//

#include "CAN_SAINT_stdafx.h"
#include <afxtempl.h>  
#include "MainPageDialog.h"
#include "CAN_SAINT.h"

CHardwareListing* hl;
// MainPageDialog dialog
int isSwitched = 0;


MainPageDialog::MainPageDialog(CHardwareListing* h,CWnd* pParent )
	: CResizableDialog(MainPageDialog::IDD, pParent)
{
	hl = h;
}
void MainPageDialog::OnCancel()
{
	CResizableDialog::OnCancel();
}
MainPageDialog::~MainPageDialog()
{
}


BOOL MainPageDialog::OnInitDialog()
{
    CResizableDialog::OnInitDialog();
	combo5.LimitText(41);
	
	edittextBuffer = CString("");

	AutoMove(IDC_LIST1, 0, 0, 100, 100);
	AutoMove(IDC_STATIC4, 0, 100, 0, 0);
	AutoMove(IDC_COMBO_MESSAGE, 0, 100, 0, 0);
	AutoMove(IDC_BUTTON_SEND, 0, 100, 0, 0);
	AutoMove(IDC_BUTTON_LOAD_MESSAGE_FILE, 100, 100, 0, 0);
	AutoMove(IDC_STATIC, 100, 0, 0, 0);
	AutoMove(IDC_CHECK_CAN_2, 100, 0, 0, 0);
	AutoMove(IDC_STATIC5, 100, 0, 0, 0);
	AutoMove(IDC_STATIC6, 100, 0, 0, 0);
	AutoMove(IDC_COMBO_CAN_2_SPEED, 100, 0, 0, 0);
	AutoMove(IDC_COMBO_CAN_2_TYPE, 100, 0, 0, 0);
	hl->loadConfig();


	return 1;
}
void MainPageDialog::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, messageList);
	DDX_Control(pDX, IDC_COMBO_MESSAGE, combo5);
}


BEGIN_MESSAGE_MAP(MainPageDialog, CResizableDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, MainPageDialog::OnLbnSelchangeList1)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SEND, MainPageDialog::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_MESSAGE_FILE, MainPageDialog::OnBnClickedButtonLoadMessageFile)
	ON_CBN_SELCHANGE(IDC_COMBO_MESSAGE, MainPageDialog::OnCbnSelchangeComboMessage)
	ON_CBN_EDITUPDATE(IDC_COMBO_MESSAGE, MainPageDialog::OnCbnEditupdateComboMessage)
END_MESSAGE_MAP()

void MainPageDialog::getMessage(CString string){
	if(string.GetLength() > 100){
		return;
	}
	char stringtosend[100];
	int i = 0;
	while(string.GetAt(i) != '\"' && string.GetAt(i) != '\t' && string.GetAt(i) != '\r' ){
		stringtosend[i] = string.GetAt(i);
		i++;
	}
	stringtosend[i] = '\0';
	CString finalstr(stringtosend);
	finalstr.Replace(" ","");
	combo5.SetWindowTextA(finalstr);
	OnCbnEditupdateComboMessage();
}
void MainPageDialog::OnSize(UINT nType, int cx, int cy)
{

	CResizableDialog::OnSize(nType, cx, cy);
	MainPageDialog::BringWindowToTop();
}


CListBox* MainPageDialog::getMessageListBox(){
	return &messageList;
}

CButton* MainPageDialog::getCheck1(){
	 return (CButton*)GetDlgItem(IDC_CHECK_CAN_1);
}

CButton* MainPageDialog::getCheck2(){
	 return (CButton*)GetDlgItem(IDC_CHECK_CAN_2);
}



void MainPageDialog::OnLbnSelchangeList1()
{
	if (!messageList){
		AfxMessageBox("error");
		return;
	}
	int sel = messageList.GetCurSel();
	CString selectedString;
	int t;
	messageList. GetText(sel, selectedString);
	if((t = selectedString.GetLength()) == 1){

	}else if(selectedString.GetAt(0) != '\"'){
		getMessage(selectedString);
	}
}


void MainPageDialog::OnBnClickedButtonSend()
{
	combo5.GetWindowTextA(edittextBuffer);
	int temp = combo5.FindString(0,edittextBuffer);
	if(temp == -1){
		combo5.InsertString(0,edittextBuffer);
	}
	edittextBuffer.Replace(" ","");
	CAN_SendMessage(edittextBuffer);
}


void MainPageDialog::OnBnClickedButtonLoadMessageFile()
{
	hl->OnBnClickedButton1();
}


void MainPageDialog::OnCbnSelchangeComboMessage()
{
	isSwitched = 1;
	combo5.GetWindowTextA(edittextBuffer);
	curEntry = edittextBuffer;
	combo5.SetEditSel(edittextBuffer.GetLength(),edittextBuffer.GetLength());
}


void MainPageDialog::OnCbnEditupdateComboMessage()
{
	combo5.GetWindowTextA(edittextBuffer);
	if(curEntry.Compare(edittextBuffer) == 0){
		return;
	}
	if(isSwitched == 1){
		combo5.GetWindowTextA(edittextBuffer);
		curEntry = edittextBuffer;
		isSwitched = 0;
	}
	edittextBuffer.Replace(" ","");
	for(int i = 0; i < edittextBuffer.GetLength(); i++){
		TCHAR temp = edittextBuffer.GetAt(i);
		if(!(temp >= '0' && temp <= '9') && !(temp >= 'A' && temp <= 'F') && (temp != '\0')){

			combo5.DeleteString(400);
			combo5.InsertString(400,curEntry);
			combo5.SetWindowTextA(curEntry);
		}else if(i == edittextBuffer.GetLength()-1){
			curEntry = edittextBuffer;
		}
	}
	combo5.GetWindowTextA(edittextBuffer);
	edittextBuffer.Replace(" ","");
	for(int i = 0; i < ((edittextBuffer.GetLength()-1) / 2);i++){
		edittextBuffer.Insert((i*2) + i + 2," ");
	}
	edittextBuffer.TrimRight();
	curEntry = edittextBuffer;
	combo5.DeleteString(400);
	combo5.InsertString(400,edittextBuffer);
	combo5.SetWindowTextA(edittextBuffer);
	combo5.SetEditSel(edittextBuffer.GetLength(),edittextBuffer.GetLength());
}
