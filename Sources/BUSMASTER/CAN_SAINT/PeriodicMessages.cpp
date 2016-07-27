// PeriodicMessages.cpp : implementation file
//

#include "CAN_SAINT_stdafx.h"
#include <afxtempl.h>  
#include "PeriodicMessages.h"
#include "HardwareListing.h"
#include "Utility/Utility_Thread.h"
#include "CAN_SAINT.h"

// PeriodicMessages dialog
int curSel = -1;
int userEdit = 0;
int amtOfEnabledPeriodic = 0;
int proport[5];
int siz[5];
int listLeng = 0;

CListCtrl * listofmessages;
std::vector<UINT_PTR> arrayofTimers;
int isTransmitting = 0;
CHardwareListing * hardwarelisting;
std::vector<GroupTransmit> groups;
std::vector<int> cycleCounts;
std::vector<int> origCycleCounts;
PeriodicMessages::PeriodicMessages(CHardwareListing* h,CWnd* pParent /*=NULL*/)
	: CResizableDialog(PeriodicMessages::IDD, pParent)
{
	hardwarelisting = h;
}

PeriodicMessages::~PeriodicMessages()
{
}

void PeriodicMessages::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listControl);
	DDX_Control(pDX, IDC_EDIT1, editMessage);
	DDX_Control(pDX, IDC_EDIT2, editPeriod);
	DDX_Control(pDX, IDC_EDIT4, editComments);
	DDX_Control(pDX, IDC_BUTTON_START, buttonEnable);
	DDX_Control(pDX, IDC_EDIT15, editCycles);
	DDX_Control(pDX, IDC_COMBO5, comboOptions);
	DDX_Control(pDX, IDC_COMBO_ENABLED, comboEnabled);
}
void PeriodicMessages::OnCancel()
{
	CResizableDialog::OnCancel();
}

WINDOWPLACEMENT *wp = new WINDOWPLACEMENT();
void PeriodicMessages::fixSiz(){
	m_listControl.GetWindowPlacement(wp);
	listLeng = wp->rcNormalPosition.right - wp->rcNormalPosition.left;
	int x1,x2,x3,x4,x5,x6;
	x1 = (int)((double)proport[0]/10000*listLeng);
	x2 = (int)((double)proport[1]/10000*listLeng);
	x3 = (int)((double)proport[2]/10000*listLeng);
	x4 = (int)((double)proport[3]/10000*listLeng);
	x5 = (int)((double)proport[4]/10000*listLeng);
	x6 = (int)((double)proport[5]/10000*listLeng);
	m_listControl.SetColumnWidth(0,x1);
	m_listControl.SetColumnWidth(1,x2);
	m_listControl.SetColumnWidth(2,x3);
	m_listControl.SetColumnWidth(3,x4);
	m_listControl.SetColumnWidth(4,x5);
	m_listControl.SetColumnWidth(5,x6-8);
	int initx;
	CRect rect;
	comboEnabled.GetWindowRect(&rect);
	ScreenToClient(&rect); 
	rect.right = (int)((double)proport[0]/10000*listLeng) + rect.left;
	comboEnabled.MoveWindow(&rect);
	initx = rect.left;

	editMessage.GetWindowRect(&rect);
	ScreenToClient(&rect); 
	rect.left = initx+x1+5;
	rect.right = (int)((double)proport[1]/10000*listLeng) + rect.left;
	editMessage.MoveWindow(&rect);

	editPeriod.GetWindowRect(&rect);
	ScreenToClient(&rect);  
	rect.left = initx+x1+x2+5;
	rect.right = (int)((double)proport[2]/10000*listLeng) + rect.left;
	editPeriod.MoveWindow(&rect);

	editCycles.GetWindowRect(&rect);
	ScreenToClient(&rect); 
	rect.left = initx+x1+x2+x3+5;
	rect.right = (int)((double)proport[3]/10000*listLeng) + rect.left;
	editCycles.MoveWindow(&rect);

	comboOptions.GetWindowRect(&rect);
	ScreenToClient(&rect); 
	rect.left = initx+x1+x2+x3+x4+5;
	rect.right = (int)((double)proport[4]/10000*listLeng) + rect.left;
	comboOptions.MoveWindow(&rect);

	editComments.GetWindowRect(&rect);
	ScreenToClient(&rect);  
	rect.left = initx+x1+x2+x3+x4+x5+5;
	rect.right = (int)((double)proport[5]/10000*listLeng) + rect.left;
	editComments.MoveWindow(&rect);
}
void PeriodicMessages::OnSize(UINT nType, int cx, int cy)
{

	CResizableDialog::OnSize(nType, cx, cy);
	if(m_listControl){
		fixSiz();
	}
}
BOOL PeriodicMessages::OnInitDialog()
{
    CResizableDialog::OnInitDialog();

	AutoMove(IDC_LIST1, 0, 0, 100, 100);
	AutoMove(IDC_BUTTON_SAVE_FILE, 100, 0, 0, 0);

	AutoMove(IDC_BUTTON_ADD_LINE, 0, 100, 0, 0);
	AutoMove(IDC_BUTTON_REMOVE, 0, 100, 0, 0);
	AutoMove(IDC_BUTTON_SAVE_LINE, 0, 100, 0, 0);
	AutoMove(IDC_BUTTON_START, 100, 100, 0, 0);

	AutoMove(IDC_COMBO_ENABLED, 0, 100, 0, 0);
	AutoMove(IDC_EDIT1, 0, 100, 0, 0);
	AutoMove(IDC_EDIT2, 0, 100, 0, 0);
	AutoMove(IDC_EDIT15, 0, 100, 0, 0);
	AutoMove(IDC_COMBO5, 0, 100, 0, 0);
	AutoMove(IDC_EDIT4, 0, 100, 100, 0);

	listofmessages = (CListCtrl *)GetDlgItem(IDC_LIST1);
	m_listControl.SetExtendedStyle( m_listControl.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	comboEnabled.AddString("Enabled");
	comboEnabled.AddString("Disabled");
	comboEnabled.SetCurSel(1);


	comboOptions.AddString("Transmit");
	comboOptions.AddString("Group");
	comboOptions.SetCurSel(1);
	m_listControl.InsertColumn(0, _T("Enabled"), LVCFMT_LEFT, 80);
	m_listControl.InsertColumn(1, _T("Tx Message"), LVCFMT_LEFT,235);
	m_listControl.InsertColumn(2, _T("Period (ms)"), LVCFMT_LEFT, 87);
	m_listControl.InsertColumn(3, _T("Cycles"), LVCFMT_LEFT, 60);
	m_listControl.InsertColumn(4, _T("Options"), LVCFMT_LEFT, 60);
	m_listControl.InsertColumn(5, _T("Comment"), LVCFMT_LEFT, 90);
	saveSiz(0,80);


	return TRUE;
}

BEGIN_MESSAGE_MAP(PeriodicMessages, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, PeriodicMessages::OnLvnItemchangedList1)
	ON_CBN_SELCHANGE(IDC_COMBO5, PeriodicMessages::OnCbnSelchangeCombo5)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, PeriodicMessages::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_FILE, PeriodicMessages::OnBnClickedButtonSaveFile)
	ON_BN_CLICKED(IDC_BUTTON_ADD_LINE, PeriodicMessages::OnBnClickedButtonAddLine)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, PeriodicMessages::OnBnClickedButtonRemove)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_LINE, PeriodicMessages::OnBnClickedButtonSaveLine)
	ON_BN_CLICKED(IDC_BUTTON_START, PeriodicMessages::OnBnClickedButtonStart)
	ON_NOTIFY(HDN_DIVIDERDBLCLICK, 0, PeriodicMessages::OnHdnDividerdblclickList1)
	ON_NOTIFY(HDN_ENDTRACK, 0, PeriodicMessages::OnHdnEndtrackList1)
END_MESSAGE_MAP()


// PeriodicMessages message handlers

void fixCycleArray(int num){
	for(int i = cycleCounts.size(); i < num+1;i++){
		cycleCounts.push_back(-1);
	}
}
void fixOrigCycleArray(int num){
	for(int i = origCycleCounts.size(); i < num+1;i++){
		origCycleCounts.push_back(-1);
	}
}
void fixGroupArray(int num){
	for(int i = groups.size(); i < num+1;i++){
		groups.push_back(GroupTransmit());
	}
}

void PeriodicMessages::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(isTransmitting == 1){
		return;
	}
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if(pNMLV->uNewState == 0){
		return;
	}
	userEdit = 0;
	curSel = pNMLV->iItem;
	CString enabled = m_listControl.GetItemText(curSel,0);
	CString message = m_listControl.GetItemText(curSel,1);
	CString period = m_listControl.GetItemText(curSel,2);
	CString cycles = m_listControl.GetItemText(curSel,3);
	CString options = m_listControl.GetItemText(curSel,4);
	CString comment = m_listControl.GetItemText(curSel,5);

	editMessage.SetWindowTextA(message);
	if(enabled.Compare("Enabled") == 0){
		comboEnabled.SetCurSel(1);
	}else{
		comboEnabled.SetCurSel(0);
	}
	editPeriod.SetWindowTextA(period);
	editComments.SetWindowTextA(comment);
	if(options.Compare("Transmit") == 0){
		comboOptions.SetCurSel(1);
	}else{
		comboOptions.SetCurSel(0);
	}
	editCycles.SetWindowTextA(cycles);
	userEdit = 1;
	*pResult = 0;
}

void CALLBACK transmitCallback( HWND hWnd,UINT nMsg,UINT_PTR nIDEvent,DWORD dwTime){
	if(isTransmitting == 0){
		KillTimer(hWnd,nIDEvent);
		return;
	}
	CString message = listofmessages->GetItemText(nIDEvent,1);
	CString options = listofmessages->GetItemText(nIDEvent,4);
	cycleCounts[nIDEvent]--;

	if(cycleCounts[nIDEvent] == 0){
		char temper[10];
		itoa(origCycleCounts[nIDEvent],temper,10);
		CString str(temper);
		itoa((origCycleCounts[nIDEvent]-cycleCounts[nIDEvent]),temper,10);
		CString count(temper);
		CString temp = str + "(";
		temp += count + ")";
		listofmessages->SetItemText(nIDEvent,3,temp);
		removeFromList(nIDEvent);
		KillTimer(hWnd,nIDEvent);
		return;
	}
	char temper[10];
	itoa(origCycleCounts[nIDEvent],temper,10);
	CString str(temper);
	itoa((origCycleCounts[nIDEvent]-cycleCounts[nIDEvent]),temper,10);
	CString count(temper);
	CString temp = str + "(";
	temp += count + ")";
	listofmessages->SetItemText(nIDEvent,3,temp);

	if(options.Compare("Group")== 0){
		handleGroupMessage(hWnd,nIDEvent);
	}else{
		CAN_SendMessage(message);
	}

}
void CALLBACK groupCallback( HWND hWnd,UINT nMsg,UINT_PTR nIDEvent,DWORD dwTime){
	if(isTransmitting == 0){
		KillTimer(hWnd,nIDEvent);
		return;
	}
	int index = nIDEvent - 5000;
	OutputDebugString("groupCall\n");
	CAN_SendMessage(groups[index].messages[groups[index].cur].data);
	groups[index].cur++;
	removeFromList(nIDEvent);
	KillTimer(hWnd,nIDEvent);
	while(groups[index].messages[groups[index].cur].enabled == false){
		groups[index].cur++;
	}
	if(groups[index].cur == groups[index].size){
		return;
	}else{
		removeFromList(nIDEvent);

		arrayofTimers.push_back(SetTimer (hWnd,nIDEvent, groups[index].messages[groups[index].cur].delay, &groupCallback)); 
	}
}
void handleGroupMessage(HWND hWnd, int index){
	groups[index].cur = 0;
	removeFromList(index+5000);
	while(groups[index].messages[groups[index].cur].enabled == false){
		groups[index].cur++;
	}
	arrayofTimers.push_back(SetTimer (hWnd,index+5000, groups[index].messages[groups[index].cur].delay, &groupCallback)); 
}

void PeriodicMessages::startPeriodicTransmission(){
	editMessage.EnableWindow(false);
	comboEnabled.EnableWindow(false);
	editPeriod.EnableWindow(false);
	comboOptions.EnableWindow(false);
	editComments.EnableWindow(false);
	editCycles.EnableWindow(false);
	GetDlgItem(IDC_BUTTON_ADD_LINE)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_REMOVE)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_SAVE_LINE)->EnableWindow(false);

	for(int i = 0; i < listofmessages->GetItemCount();i++){
		fixOrigCycleArray(i);
		origCycleCounts[i] = atoi(listofmessages->GetItemText(i,3));
		fixCycleArray(i);
		cycleCounts[i] = 0;
	}

	userEdit = 0;
	editMessage.SetWindowTextA("");
	comboEnabled.SetCurSel(1);
	editPeriod.SetWindowTextA("");
	comboOptions.SetCurSel(1);
	editComments.SetWindowTextA("");
	editCycles.SetWindowTextA("");
	if(CAN_SendMessage("test") == false){
		isTransmitting = 0;
		buttonEnable.SetWindowTextA("Start Transmission");
		stopPeriodicTransmission();
		return;
	}
	int amt = m_listControl.GetItemCount();
	amtOfEnabledPeriodic = 0;

	for(int i = 0; i  < amt;i++){
		CString enabled = m_listControl.GetItemText(i,0);
		CString period = m_listControl.GetItemText(i,2);
		if(enabled.Compare("Enabled") == 0 && period.Compare("")){
			char temper[10];
			itoa(origCycleCounts[i],temper,10);
			CString str(temper);
			itoa((origCycleCounts[i]-cycleCounts[i]),temper,10);
			CString count(temper);
			CString temp = str + "(0)";
			listofmessages->SetItemText(i,3,temp);

			amtOfEnabledPeriodic++;
			CString message = m_listControl.GetItemText(i,1);
			CString period = m_listControl.GetItemText(i,2);
			CString cycles = m_listControl.GetItemText(i,3);
			CString transmit = m_listControl.GetItemText(i,4);
			fixCycleArray(i);
			cycleCounts[i] = atoi(cycles);
			if(transmit.Compare("Group") == 0){
				if(CAN_SendMessage("test") == false){
					isTransmitting = 0;
					buttonEnable.SetWindowTextA("Start Transmission");
					stopPeriodicTransmission();
					return;
				}
				GroupTransmit gt(message,"");
				fixGroupArray(i);
				groups[i] = gt;
				int periodInt = atoi(period);
				handleGroupMessage(this->m_hWnd,i); 
				removeFromList(i);
				arrayofTimers.push_back(SetTimer (i, periodInt, &transmitCallback)); 
			}else{
				if(CAN_SendMessage(message) == false){
					isTransmitting = 0;
					buttonEnable.SetWindowTextA("Start Transmission");
					stopPeriodicTransmission();
					return;
				}
				int periodInt = atoi(period);
				removeFromList(i);
				arrayofTimers.push_back(SetTimer (i, periodInt, &transmitCallback)); 
			}
		}
	}
}
void removeFromList(UINT_PTR val){
	for(int i = 0; i< arrayofTimers.size();i++){
		if(arrayofTimers.at(i) == val){
			arrayofTimers.erase( arrayofTimers.begin()+i);
		}
	}
}
CListCtrl * PeriodicMessages::getListBox(){
	return listofmessages;
}

void PeriodicMessages::stopPeriodicTransmission(){
	UINT_PTR temp;
	isTransmitting = 0;
	editMessage.EnableWindow(true);
	comboEnabled.EnableWindow(true);
	editPeriod.EnableWindow(true);
	comboOptions.EnableWindow(true);
	editComments.EnableWindow(true);
	editCycles.EnableWindow(true);
	GetDlgItem(IDC_BUTTON_ADD_LINE)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_REMOVE)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_SAVE_LINE)->EnableWindow(true);



	for(int i = 0; i < listofmessages->GetItemCount();i++){
		if(origCycleCounts.size() < i+1){
			break;
		}
		char temp[10];
		itoa(origCycleCounts[i],temp,10);
		listofmessages->SetItemText(i,3,temp);
	}
	while(arrayofTimers.size() > 0){
		temp = arrayofTimers.back();
		arrayofTimers.pop_back();
		KillTimer(temp);
	}
	CString enabled = m_listControl.GetItemText(curSel,0);
	CString message = m_listControl.GetItemText(curSel,1);
	CString period = m_listControl.GetItemText(curSel,2);
	CString cycles = m_listControl.GetItemText(curSel,3);
	CString options = m_listControl.GetItemText(curSel,4);
	CString comment = m_listControl.GetItemText(curSel,5);

	editMessage.SetWindowTextA(message);
	if(enabled.Compare("Disabled") == 0){
		comboEnabled.SetCurSel(0);
	}else{
		comboEnabled.SetCurSel(1);
	}
	editPeriod.SetWindowTextA(period);
	editComments.SetWindowTextA(comment);
	if(options.Compare("Group") == 0){
		comboOptions.SetCurSel(0);
	}else{
		comboOptions.SetCurSel(1);
	}
	editCycles.SetWindowTextA(cycles);
	userEdit = 1;
}


CString filename1;
CFileDialog dlg1(TRUE);
void PeriodicMessages::OnCbnSelchangeCombo5()
{
	if (comboOptions.GetCurSel() == 0){
		dlg1.m_ofn.nMaxFile = 511;
		dlg1.m_ofn.lpstrFilter="Group Files (*.grp)\0*.grp\0";
		dlg1.m_ofn.lpstrTitle="Open Group File";
		if(dlg1.DoModal() == IDOK)
		{
			filename1 = dlg1.GetPathName(); // return full path and filenam
		}else{
			return;
		}
		editMessage.SetWindowTextA(filename1);
	}else{
		editMessage.SetWindowTextA("");
	}

}


void PeriodicMessages::OnBnClickedButtonLoad()
{
	isTransmitting = 0;
	buttonEnable.SetWindowTextA("Start Transmission");
	stopPeriodicTransmission();
	hardwarelisting->OnBnLoadPerFile();
}


void PeriodicMessages::OnBnClickedButtonSaveFile()
{
	isTransmitting = 0;
	buttonEnable.SetWindowTextA("Start Transmission");
	stopPeriodicTransmission();


	 TCHAR szFilters[]= _T("Period Files (*.per)\0*.per\0");
	 CFileDialog fileDlg(FALSE, _T("per"), _T("*.per"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);
	 if(fileDlg.DoModal() == IDOK){
		CString pathName = fileDlg.GetPathName();
		CFile cfile;
		cfile.Open((pathName), CFile::modeCreate | CFile::modeReadWrite);
		CString strtowrite;
		CString buffer;


		for(int i = 0; i < listofmessages->GetItemCount(); i++){

			if(listofmessages->GetItemText(i,4).Compare("Transmit") == 0){
				strtowrite += "Transmit,";
			}else{
				strtowrite += "Group,";
			}
			strtowrite += listofmessages->GetItemText(i,1) + ",";
			if(listofmessages->GetItemText(i,2).Compare("") == 0){
				strtowrite +=  "0,";
			}else{
				strtowrite +=   listofmessages->GetItemText(i,2) + ",";
			}
			if(listofmessages->GetItemText(i,0).Compare("Enabled") == 0){
				strtowrite += "True,";
			}else{
				strtowrite += "False,";
			}
			if(listofmessages->GetItemText(i,3).Compare("") == 0){
				strtowrite += "0,";
			}else{
				strtowrite += listofmessages->GetItemText(i,3) + "," ;
			}
			if(listofmessages->GetItemText(i,5).Compare("") == 0){
				strtowrite += "\n";
			}else{
				CString temp = listofmessages->GetItemText(i,5).Remove('\n');
				strtowrite += listofmessages->GetItemText(i,5) + "\n";
			}
		}
		cfile.Write(strtowrite, strtowrite.GetLength());         
		cfile.Flush();
	}
}


void PeriodicMessages::OnBnClickedButtonAddLine()
{
	if(isTransmitting == 1){
		return;
	}
	curSel = 0;
	if(comboOptions.GetCurSel()  == 1){
		CString	buffer;
		editMessage.GetWindowTextA(buffer);
		if(checkValidity(buffer) == false){
			AfxMessageBox("Invalid Message");
			return;
		}
	}
	int n = m_listControl.GetItemCount();
	int nIndex = m_listControl.InsertItem(n, _T("Disabled"));
	CString	buffer;
	comboEnabled.GetWindowTextA(buffer);
	m_listControl.SetItemText(nIndex, 0, _T(buffer));

	editMessage.GetWindowTextA(buffer);
	m_listControl.SetItemText(nIndex, 1, _T(buffer));
	editPeriod.GetWindowTextA(buffer);
	m_listControl.SetItemText(nIndex, 2, _T(buffer));
	editCycles.GetWindowTextA(buffer);
	m_listControl.SetItemText(nIndex, 3, _T(buffer));
	if(comboOptions.GetCurSel() == 0){
		m_listControl.SetItemText(nIndex, 4, _T("Group"));
	}else{
		m_listControl.SetItemText(nIndex, 4, _T("Transmit"));
	}
	editComments.GetWindowTextA(buffer);
	m_listControl.SetItemText(nIndex, 5, _T(buffer));
}


void PeriodicMessages::OnBnClickedButtonRemove()
{
	if(curSel != -1){
		m_listControl.DeleteItem(curSel);
		userEdit = 0;
		editMessage.SetWindowTextA("");
		comboEnabled.SetCurSel(1);
		editPeriod.SetWindowTextA("");
		comboOptions.SetCurSel(1);
		editComments.SetWindowTextA("");
		editCycles.SetWindowTextA("");
		curSel = -1;
		isTransmitting = 0;
		buttonEnable.SetWindowTextA("Start Transmission");
		stopPeriodicTransmission();
	}
}


void PeriodicMessages::OnBnClickedButtonSaveLine()
{
	if(isTransmitting == 1){
		return;
	}
	if(curSel != -1 && userEdit == 1){

		if(comboOptions.GetCurSel()  == 1){
			CString	buffer;
			editMessage.GetWindowTextA(buffer);
			if(checkValidity(buffer) == false){
				AfxMessageBox("Invalid Message");
				return;
			}
		}


		CString cycles;
		editCycles.GetWindowTextA(cycles);
		if(cycles.Compare("") == 0){
			cycles = "0";
		}
		m_listControl.SetItemText(curSel,3,cycles);

		if(comboEnabled.GetCurSel() == 0){
			m_listControl.SetItemText(curSel,0,"Disabled");
		}else{
			m_listControl.SetItemText(curSel,0,"Enabled");
		}
		CString message;
		editMessage.GetWindowTextA(message);
		m_listControl.SetItemText(curSel,1,message);
		CString period;
		editPeriod.GetWindowTextA(period);
		if(period.Compare("") == 0){
			period = "0";
		}
		m_listControl.SetItemText(curSel,2,period);

		if (comboOptions.GetCurSel() == 0){
			m_listControl.SetItemText(curSel,4,"Group");
		}else{
			m_listControl.SetItemText(curSel,4,"Transmit");
		}

		CString comment;
		editComments.GetWindowTextA(comment);
		m_listControl.SetItemText(curSel,5,comment);
	}
	
}


void PeriodicMessages::OnBnClickedButtonStart()
{
	if(isTransmitting == 0){
		isTransmitting = 1;
		buttonEnable.SetWindowTextA("Stop Transmission");
		startPeriodicTransmission();
	}else{
		isTransmitting = 0;
		buttonEnable.SetWindowTextA("Start Transmission");
		stopPeriodicTransmission();
	}
}


int PeriodicMessages::getSiz(){
	int tot = 0;
	tot += m_listControl.GetColumnWidth(0);
	tot += m_listControl.GetColumnWidth(1);
	tot += m_listControl.GetColumnWidth(2);
	tot += m_listControl.GetColumnWidth(3);
	tot += m_listControl.GetColumnWidth(4);
	tot += m_listControl.GetColumnWidth(5);
	return tot;
}
void PeriodicMessages::restoreSiz(){
	m_listControl.SetColumnWidth(0,siz[0]);
	m_listControl.SetColumnWidth(1,siz[1]);
	m_listControl.SetColumnWidth(2,siz[2]);
	m_listControl.SetColumnWidth(3,siz[3]);
	m_listControl.SetColumnWidth(4,siz[4]);
	m_listControl.SetColumnWidth(5,siz[5]-6);
}
void PeriodicMessages::OnHdnDividerdblclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	fixSiz();
	*pResult = 0;
}
void PeriodicMessages::saveSiz(int index, int newVal){
	siz[0] = m_listControl.GetColumnWidth(0);
	siz[1] = m_listControl.GetColumnWidth(1);
	siz[2] = m_listControl.GetColumnWidth(2);
	siz[3] = m_listControl.GetColumnWidth(3);
	siz[4] = m_listControl.GetColumnWidth(4);
	siz[5] = m_listControl.GetColumnWidth(5);
	siz[index] = newVal;
	proport[0] = (int)(siz[0] / (double)(siz[0]+siz[1]+siz[2]+siz[3]+siz[4]+siz[5])*10000);
	proport[1] = (int)(siz[1] / (double)(siz[0]+siz[1]+siz[2]+siz[3]+siz[4]+siz[5])*10000);
	proport[2] = (int)(siz[2] / (double)(siz[0]+siz[1]+siz[2]+siz[3]+siz[4]+siz[5])*10000);
	proport[3] = (int)(siz[3] / (double)(siz[0]+siz[1]+siz[2]+siz[3]+siz[4]+siz[5])*10000);
	proport[4] = (int)(siz[4] / (double)(siz[0]+siz[1]+siz[2]+siz[3]+siz[4]+siz[5])*10000);
	proport[5] = (int)(siz[5] / (double)(siz[0]+siz[1]+siz[2]+siz[3]+siz[4]+siz[5])*10000);
	int tot = proport[0] + proport[1] + proport[2] + proport[3]+ proport[4]+ proport[5];
	proport[5] += 10000-tot;
}

void PeriodicMessages::OnHdnEndtrackList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	int tot = getSiz();
	tot -= m_listControl.GetColumnWidth(phdr->iItem);
	tot += phdr->pitem->cxy;
	int diff = phdr->pitem->cxy - m_listControl.GetColumnWidth(phdr->iItem);
	if(phdr->iItem == 5){
		phdr->pitem->cxy = m_listControl.GetColumnWidth(phdr->iItem);
	}else if(diff > 0){
		if(m_listControl.GetColumnWidth(phdr->iItem+1)-diff<10){
			phdr->pitem->cxy = m_listControl.GetColumnWidth(phdr->iItem);
		}else{
			m_listControl.SetColumnWidth(phdr->iItem+1,m_listControl.GetColumnWidth(phdr->iItem+1)-diff);
		}
	}else{
		int resize = diff * -1;
		if(m_listControl.GetColumnWidth(phdr->iItem)-resize<10){
			phdr->pitem->cxy = m_listControl.GetColumnWidth(phdr->iItem);
		}else{
			m_listControl.SetColumnWidth(phdr->iItem+1,m_listControl.GetColumnWidth(phdr->iItem+1)+resize);
		}
	}
	saveSiz(phdr->iItem,phdr->pitem->cxy);
	fixSiz();
	*pResult = 0;
}
