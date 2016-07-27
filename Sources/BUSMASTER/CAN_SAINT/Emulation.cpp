// Emulation.cpp : implementation file
//

#include "CAN_SAINT_stdafx.h"
#include <afxtempl.h>  
#include "HardwareListing.h"
#include "Emulation.h"
#include "Utility/Utility_Thread.h"
#include "CAN_SAINT.h"
CHardwareListing * hl1;
CListCtrl * listctrl1;
std::vector<UINT_PTR> arrayOfEmuTimers;
int sel = 0;
int edit = 0;
int sizelist[5];
int proplist[5];
int listLength1;
std::vector<GroupTransmit> Emulation::groupEmulations;
// Emulation dialog

Emulation::Emulation(CHardwareListing * hl, CWnd* pParent /*=NULL*/)
	: CResizableDialog(Emulation::IDD, pParent)
{
	transmit = 0;
	hl1 = hl;
}

Emulation::~Emulation()
{

}
void Emulation::OnCancel()
{
	CResizableDialog::OnCancel();
}
BOOL Emulation::OnInitDialog()
{
    CResizableDialog::OnInitDialog();


	AutoMove(IDC_LIST1, 0, 0, 100, 100);
	AutoMove(IDC_BUTTON5, 100, 0, 0, 0);
	AutoMove(IDC_BUTTON_SAVE_FILE, 100, 0, 0, 0);

	AutoMove(IDC_BUTTON_ADD_NEW_LINE, 0, 100, 0, 0);
	AutoMove(IDC_BUTTON_REMOVE_LINE, 0, 100, 0, 0);
	AutoMove(IDC_BUTTON_SAVE_LINE, 0, 100, 0, 0);
	AutoMove(IDC_BUTTON_START, 100, 100, 0, 0);

	AutoMove(IDC_COMBO_ENABLED, 0, 100, 0, 0);
	AutoMove(IDC_EDIT1, 0, 100, 0, 0);
	AutoMove(IDC_EDIT2, 0, 100, 0, 0);
	AutoMove(IDC_COMBO_TYPE, 0, 100, 0, 0);
	AutoMove(IDC_EDIT4, 0, 100, 100, 0);

	listControl.SetExtendedStyle(listControl.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	listControl.ModifyStyle(LVS_SINGLESEL , 0);
	listctrl1 = (CListCtrl *)GetDlgItem(IDC_LIST1);

	comboOptions.AddString("Transmit");
	comboOptions.AddString("Group");
	comboOptions.SetCurSel(1);

	comboEnabled.AddString("Enabled");
	comboEnabled.AddString("Disabled");
	comboEnabled.SetCurSel(1);

	listControl.InsertColumn(0, _T("Enabled"), LVCFMT_LEFT, 80);
	listControl.InsertColumn(1, _T("Rx Message"), LVCFMT_LEFT,184);
	listControl.InsertColumn(2, _T("Tx Message"), LVCFMT_LEFT, 185);
	listControl.InsertColumn(3, _T("Options"), LVCFMT_LEFT, 60);
	listControl.InsertColumn(4, _T("Comment"), LVCFMT_LEFT, 95);
	saveProperSizing(0,80);

	return true;
}
void Emulation::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, listControl);
	DDX_Control(pDX, IDC_EDIT1, editReceive);
	DDX_Control(pDX, IDC_EDIT2, editTransmit);
	DDX_Control(pDX, IDC_COMBO_TYPE, comboOptions);
	DDX_Control(pDX, IDC_EDIT4, editComments);
	DDX_Control(pDX, IDC_BUTTON_START, buttonEnable);
	DDX_Control(pDX, IDC_COMBO_ENABLED, comboEnabled);
}

void Emulation::OnSize(UINT nType, int cx, int cy)
{

	CResizableDialog::OnSize(nType, cx, cy);
	if(listControl){
		fixProperSizing();
	}
}
BEGIN_MESSAGE_MAP(Emulation, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, Emulation::OnLvnItemchangedList1)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, Emulation::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_FILE,Emulation::OnBnClickedButtonSaveFile)
	ON_BN_CLICKED(IDC_BUTTON_ADD_NEW_LINE, Emulation::OnBnClickedButtonAddNewLine)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_LINE, Emulation::OnBnClickedButtonRemoveLine)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_LINE, Emulation::OnBnClickedButtonSaveLine)
	ON_BN_CLICKED(IDC_BUTTON_START, Emulation::OnBnClickedButtonStart)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, Emulation::OnCbnSelchangeComboType)
	ON_NOTIFY(HDN_ENDTRACK, 0, Emulation::OnHdnEndtrackList1)
	ON_NOTIFY(HDN_DIVIDERDBLCLICK, 0, Emulation::OnHdnDividerdblclickList1)
END_MESSAGE_MAP()


// Emulation message handlers
CListCtrl * Emulation::getListBox(){
	return listctrl1;
}


void Emulation::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(transmit == 1){
		return;
	}
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if(pNMLV->uNewState == 0){
		return;
	}
	edit = 0;
	sel = pNMLV->iItem;
	CString enabled = listControl.GetItemText(sel,0);
	CString rxmessage = listControl.GetItemText(sel,1);
	CString txmessage = listControl.GetItemText(sel,2);
	CString options = listControl.GetItemText(sel,3);
	CString comment = listControl.GetItemText(sel,4);

	editTransmit.SetWindowTextA(txmessage);
	editReceive.SetWindowTextA(rxmessage);
	if(enabled.Compare("Enabled") == 0){
		comboEnabled.SetCurSel(1);
	}else{
		comboEnabled.SetCurSel(0);
	}
	editComments.SetWindowTextA(comment);
	if(options.Compare("Transmit") == 0){
		comboOptions.SetCurSel(1);
	}else{
		comboOptions.SetCurSel(0);
	}
	edit = 1;


	*pResult = 0;
}
void Emulation::startEmulation(){
	editTransmit.EnableWindow(false);
	editComments.EnableWindow(false);
	comboEnabled.EnableWindow(false);
	comboOptions.EnableWindow(false);
	editReceive.EnableWindow(false);
	GetDlgItem(IDC_BUTTON_ADD_NEW_LINE)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_REMOVE_LINE)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_SAVE_LINE)->EnableWindow(false);
	emulations.clear();
	groupEmulations.clear();

	edit = 0;
	editTransmit.SetWindowTextA("");
	editReceive.SetWindowTextA("");
	editComments.SetWindowTextA("");
	comboEnabled.SetCurSel(1);
	comboOptions.SetCurSel(1);

	for(int i = 0; i < listControl.GetItemCount(); i++){
		if(listControl.GetItemText(i,0).Compare("Enabled") == 0){
			CString receive = listControl.GetItemText(i,1);
			CString transmit = listControl.GetItemText(i,2);
			receive.Remove(' ');
			transmit.Remove(' ');
			if(listControl.GetItemText(i,3).Compare("Transmit") == 0){
				Message emu(receive,transmit);
				emulations.push_back(emu);
			}else{
				GroupTransmit gemu(transmit,receive);
				groupEmulations.push_back(gemu);
			}
		}
	}
}

void removeFromTimers(UINT_PTR val){
	for(int i = 0; i< arrayOfEmuTimers.size();i++){
		if(arrayOfEmuTimers.at(i) == val){
			arrayOfEmuTimers.erase(arrayOfEmuTimers.begin()+i);
		}
	}
}
void CALLBACK groupEmuCallback( HWND hWnd,UINT nMsg,UINT_PTR nIDEvent,DWORD dwTime){
	if(hl1->getEPageTransmit() == 0){
		removeFromTimers(nIDEvent);
		KillTimer(hWnd,nIDEvent);
		return;
	}
	int index = nIDEvent;
	CAN_SendMessage(Emulation::groupEmulations[index].messages[Emulation::groupEmulations[index].cur].data);
	Emulation::groupEmulations[index].cur++;
	removeFromTimers(nIDEvent);
	KillTimer(hWnd,nIDEvent);
	while(Emulation::groupEmulations[index].messages[Emulation::groupEmulations[index].cur].enabled == false){
		Emulation::groupEmulations[index].cur++;
	}
	if(Emulation::groupEmulations[index].cur == Emulation::groupEmulations[index].size){
		return;
	}else{
		removeFromTimers(nIDEvent);
		arrayOfEmuTimers.push_back(SetTimer (hWnd,nIDEvent, Emulation::groupEmulations[index].messages[Emulation::groupEmulations[index].cur].delay, &groupEmuCallback)); 
	}
}
void Emulation::handleGroupEmulation(HWND hWnd, int index){
	groupEmulations[index].cur = 0;
	removeFromTimers(index);
	while(Emulation::groupEmulations[index].messages[Emulation::groupEmulations[index].cur].enabled == false){
		Emulation::groupEmulations[index].cur++;
	}
	arrayOfEmuTimers.push_back(SetTimer (index, groupEmulations[index].messages[Emulation::groupEmulations[index].cur].delay, &groupEmuCallback)); 
}
void Emulation::handleEmulations(CString str){
	if(!transmit){
		return;
	}
	bool isExcluded = false;
	str.Remove(' ');
	for(int i = 0; i < 28-str.GetLength();i++){
		str += "X";
	}

	for(int i = 0; i < emulations.size();i++){
		for(int j = 0; j < emulations[i].receiveMessage.GetLength();j++){
			if(emulations[i].receiveMessage.GetAt(j) != str.GetAt(j)){
				if(emulations[i].receiveMessage.GetAt(j) != 'X' &&  str.GetAt(j) != 'X'){
					break;
				}
			}
			if(j == emulations[i].receiveMessage.GetLength()-1){
				CAN_SendMessage(emulations[i].transmitMessage);
				isExcluded = true;
			}
		}
	}
	for(int i = 0; i <  groupEmulations.size();i++){
		for(int j = 0; j < groupEmulations[i].receive.GetLength();j++){
			if(groupEmulations[i].receive.GetAt(j) != str.GetAt(j)){
				if(groupEmulations[i].receive.GetAt(j) != 'X' &&  str.GetAt(j) != 'X'){
					break;
				}
			}
			if(j == groupEmulations[i].receive.GetLength()-1){
				handleGroupEmulation(this->m_hWnd,i); 
				removeFromTimers(i);
				isExcluded = true;
			}
		}
	}
}

void Emulation::stopEmulation(){
	UINT_PTR temp;
	transmit = 0;
	editTransmit.EnableWindow(true);
	comboEnabled.EnableWindow(true);
	editReceive.EnableWindow(true);
	comboOptions.EnableWindow(true);
	editComments.EnableWindow(true);
	GetDlgItem(IDC_BUTTON_ADD_NEW_LINE)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_REMOVE_LINE)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_SAVE_LINE)->EnableWindow(true);

	CString enabled = listControl.GetItemText(sel,0);
	CString rxmessage = listControl.GetItemText(sel,1);
	CString txmessage = listControl.GetItemText(sel,2);
	CString options = listControl.GetItemText(sel,3);
	CString comment = listControl.GetItemText(sel,4);

	editTransmit.SetWindowTextA(txmessage);
	if(enabled.Compare("Disabled") == 0){
		comboEnabled.SetCurSel(0);
	}else{
		comboEnabled.SetCurSel(1);
	}
	editReceive.SetWindowTextA(rxmessage);
	editComments.SetWindowTextA(comment);
	if(options.Compare("Group") == 0){
		comboOptions.SetCurSel(0);
	}else{
		comboOptions.SetCurSel(1);
	}
	edit = 1;

	while(arrayOfEmuTimers.size() > 0){
		temp = arrayOfEmuTimers.back();
		arrayOfEmuTimers.pop_back();
		KillTimer(temp);
	}
}

void Emulation::OnBnClickedButtonLoad()
{
	transmit = 0;
	buttonEnable.SetWindowTextA("Start Emulation");
	stopEmulation();
	hl1->OnBnLoadEmuFile();
}


void Emulation::OnBnClickedButtonSaveFile()
{
	transmit = 0;
	buttonEnable.SetWindowTextA("Start Emulation");
	stopEmulation();


	 TCHAR szFilters[]= _T("Emulation Files (*.emu)\0*.emu\0");
	 CFileDialog fileDlg(FALSE, _T("emu"), _T("*.emu"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);
	 if(fileDlg.DoModal() == IDOK){
		CString pathName = fileDlg.GetPathName();
		CFile cfile;
		cfile.Open((pathName), CFile::modeCreate | CFile::modeReadWrite);
		CString strtowrite;
		CString buffer;


		for(int i = 0; i < listControl.GetItemCount(); i++){
			strtowrite += listControl.GetItemText(i,1) + ",";
			if(listControl.GetItemText(i,3).Compare("Transmit") == 0){
				strtowrite += "Transmit,";
			}else{
				strtowrite += "Group,";
			}
			strtowrite += listControl.GetItemText(i,2) + ",";
			if(listControl.GetItemText(i,0).Compare("Enabled") == 0){
				strtowrite += "True,";
			}else{
				strtowrite += "False,";
			}
			if(listControl.GetItemText(i,4).Compare("") == 0){
				strtowrite += "\n";
			}else{
				CString temp = listControl.GetItemText(i,4).Remove('\n');
				strtowrite += listControl.GetItemText(i,4) + "\n";
			}
		}
		cfile.Write(strtowrite, strtowrite.GetLength());         
		cfile.Flush();
	}
}


void Emulation::OnBnClickedButtonAddNewLine()
{
	if(transmit == 1){
		return;
	}
	sel = 0;
	edit = 1;
	if(comboOptions.GetCurSel()  == 1){
		CString	buffer;
		editTransmit.GetWindowTextA(buffer);
		if(checkValidity(buffer) == false){
			AfxMessageBox("Invalid Message");
			return;
		}
		editReceive.GetWindowTextA(buffer);
		if(checkWildcardValidity(buffer) == false){
			AfxMessageBox("Invalid Message");
			return;
		}
	}
	int n = listControl.GetItemCount();
	int nIndex = listControl.InsertItem(n, _T("Disabled"));
	CString	buffer;
	comboEnabled.GetWindowTextA(buffer);
	listControl.SetItemText(nIndex, 0, _T(buffer));

	editReceive.GetWindowTextA(buffer);
	listControl.SetItemText(nIndex, 1, _T(buffer));
	editTransmit.GetWindowTextA(buffer);
	listControl.SetItemText(nIndex, 2, _T(buffer));
	if(comboOptions.GetCurSel() == 0){
		listControl.SetItemText(nIndex, 3, _T("Group"));
	}else{
		listControl.SetItemText(nIndex, 3, _T("Transmit"));
	}
	editComments.GetWindowTextA(buffer);
	listControl.SetItemText(nIndex, 4, _T(buffer));
}


void Emulation::OnBnClickedButtonRemoveLine()
{
	if(transmit == 1){
		return;
	}
	if(sel != -1){
		listControl.DeleteItem(sel);
		edit = 0;
		editTransmit.SetWindowTextA("");
		comboEnabled.SetCurSel(1);
		editReceive.SetWindowTextA("");
		comboOptions.SetCurSel(1);
		editComments.SetWindowTextA("");
		sel = -1;
	}
}


void Emulation::OnBnClickedButtonSaveLine()
{
	if(transmit == 1){
		return;
	}
	if(sel != -1 && edit == 1){

		if(comboOptions.GetCurSel()  == 1){
			CString	buffer;
			editTransmit.GetWindowTextA(buffer);
			if(checkValidity(buffer) == false){
				AfxMessageBox("Invalid Message");
				return;
			}
			editReceive.GetWindowTextA(buffer);
			if(checkWildcardValidity(buffer) == false){
				AfxMessageBox("Invalid Message");
				return;
			}
		}


		CString message;
		comboEnabled.GetWindowTextA(message);
		listControl.SetItemText(sel,0,message);

		editReceive.GetWindowTextA(message);
		listControl.SetItemText(sel,1,message);
		editTransmit.GetWindowTextA(message);
		listControl.SetItemText(sel,2,message);

		if (comboOptions.GetCurSel() == 0){
			listControl.SetItemText(sel,3,"Group");
		}else{
			listControl.SetItemText(sel,3,"Transmit");
		}

		CString comment;
		editComments.GetWindowTextA(comment);
		listControl.SetItemText(sel,4,comment);
	}
}


void Emulation::OnBnClickedButtonStart()
{
	if(transmit == 0){
		transmit = 1;
		buttonEnable.SetWindowTextA("Stop Emulation");
		startEmulation();
	}else{
		transmit = 0;
		buttonEnable.SetWindowTextA("Start Emulation");
		stopEmulation();
	}
}


void Emulation::OnCbnSelchangeComboType()
{
	if (comboOptions.GetCurSel() == 0){
		hl1->OnChangeToGroup();
	}else{
		editTransmit.SetWindowTextA("");
	}
}
int Emulation::getTot(){
	int tot = 0;
	tot += listctrl1->GetColumnWidth(0);
	tot += listctrl1->GetColumnWidth(1);
	tot += listctrl1->GetColumnWidth(2);
	tot += listctrl1->GetColumnWidth(3);
	tot += listctrl1->GetColumnWidth(4);
	return tot;
}
WINDOWPLACEMENT *wpp = new WINDOWPLACEMENT();
void Emulation::fixProperSizing(){
	listControl.GetWindowPlacement(wpp);
	listLength1 = wpp->rcNormalPosition.right - wpp->rcNormalPosition.left;
	int x1,x2,x3,x4,x5;
	x1 = (int)((double)proplist[0]/10000*listLength1);
	x2 = (int)((double)proplist[1]/10000*listLength1);
	x3 = (int)((double)proplist[2]/10000*listLength1);
	x4 = (int)((double)proplist[3]/10000*listLength1);
	x5 = (int)((double)proplist[4]/10000*listLength1);
	listControl.SetColumnWidth(0,x1);
	listControl.SetColumnWidth(1,x2);
	listControl.SetColumnWidth(2,x3);
	listControl.SetColumnWidth(3,x4);
	listControl.SetColumnWidth(4,x5-6);
	int initx;
	CRect rect;
	comboEnabled.GetWindowRect(&rect);
	ScreenToClient(&rect); 
	rect.right = (int)((double)proplist[0]/10000*listLength1) + rect.left;
	comboEnabled.MoveWindow(&rect);
	initx = rect.left;

	editReceive.GetWindowRect(&rect);
	ScreenToClient(&rect); 
	rect.left = initx+x1+5;
	rect.right = (int)((double)proplist[1]/10000*listLength1) + rect.left;
	editReceive.MoveWindow(&rect);

	editTransmit.GetWindowRect(&rect);
	ScreenToClient(&rect); 
	rect.left = initx+x1+x2+5;
	rect.right = (int)((double)proplist[2]/10000*listLength1) + rect.left;
	editTransmit.MoveWindow(&rect);

	comboOptions.GetWindowRect(&rect);
	ScreenToClient(&rect);  
	rect.left = initx+x1+x2+x3+5;
	rect.right = (int)((double)proplist[3]/10000*listLength1) + rect.left;
	comboOptions.MoveWindow(&rect);

	editComments.GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.left = initx+x1+x2+x3+x4+5;
	rect.right = (int)((double)proplist[4]/10000*listLength1) + rect.left;
	editComments.MoveWindow(&rect);
}
void Emulation::saveProperSizing(int index, int newVal){
	sizelist[0] = listctrl1->GetColumnWidth(0);
	sizelist[1] = listctrl1->GetColumnWidth(1);
	sizelist[2] = listctrl1->GetColumnWidth(2);
	sizelist[3] = listctrl1->GetColumnWidth(3);
	sizelist[4] = listctrl1->GetColumnWidth(4);
	sizelist[index] = newVal;
	proplist[0] = (int)(sizelist[0] / (double)(sizelist[0]+sizelist[1]+sizelist[2]+sizelist[3]+sizelist[4])*10000);
	proplist[1] = (int)(sizelist[1] / (double)(sizelist[0]+sizelist[1]+sizelist[2]+sizelist[3]+sizelist[4])*10000);
	proplist[2] = (int)(sizelist[2] / (double)(sizelist[0]+sizelist[1]+sizelist[2]+sizelist[3]+sizelist[4])*10000);
	proplist[3] = (int)(sizelist[3] / (double)(sizelist[0]+sizelist[1]+sizelist[2]+sizelist[3]+sizelist[4])*10000);
	proplist[4] = (int)(sizelist[4] / (double)(sizelist[0]+sizelist[1]+sizelist[2]+sizelist[3]+sizelist[4])*10000);
	int tot = proplist[0] + proplist[1] + proplist[2] + proplist[3] +proplist[4];
	proplist[4] += 10000-tot;
}
void Emulation::OnHdnEndtrackList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	int tot = getTot();
	tot -= listctrl1->GetColumnWidth(phdr->iItem);
	tot += phdr->pitem->cxy;
	int diff = phdr->pitem->cxy - listctrl1->GetColumnWidth(phdr->iItem);
	if(phdr->iItem == 4){
		phdr->pitem->cxy = listctrl1->GetColumnWidth(phdr->iItem);
	}else if(diff > 0){
		if(listctrl1->GetColumnWidth(phdr->iItem+1)-diff<10){
			phdr->pitem->cxy = listctrl1->GetColumnWidth(phdr->iItem);
		}else{
			listctrl1->SetColumnWidth(phdr->iItem+1,listctrl1->GetColumnWidth(phdr->iItem+1)-diff);
		}
	}else{
		int resize = diff * -1;
		if(listctrl1->GetColumnWidth(phdr->iItem)-resize<10){
			phdr->pitem->cxy = listctrl1->GetColumnWidth(phdr->iItem);
		}else{
			listctrl1->SetColumnWidth(phdr->iItem+1,listctrl1->GetColumnWidth(phdr->iItem+1)+resize);
		}
	}
	saveProperSizing(phdr->iItem,phdr->pitem->cxy);
	fixProperSizing();
	*pResult = 0;
}


void Emulation::OnHdnDividerdblclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	fixProperSizing();
	*pResult = 0;
}
