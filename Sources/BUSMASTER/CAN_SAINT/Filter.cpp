// Filter.cpp : implementation file
//

#include "CAN_SAINT_stdafx.h"
#include <afxtempl.h>  
#include "HardwareListing.h"
#include "CAN_SAINT.h"
#include "Filter.h"

CHardwareListing* hlist;
CListCtrl * listctrl;
int proportions[4];
int uEdit = 0;
int cSel = -1;
int sizes[4];
int listLength = 0;

Filter::Filter(CHardwareListing* h,CWnd* pParent /*=NULL*/)
	: CResizableDialog(Filter::IDD, pParent)
{
	hlist = h;
	transmitting = 0;
	amtOfIncludes = 0;
	amtOfExcludes = 0;
}

Filter::~Filter()
{

}

void Filter::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, editMessageBox);
	DDX_Control(pDX, IDC_EDIT4, editCommentBox);
	DDX_Control(pDX, IDC_LIST1, listControl);
	DDX_Control(pDX, IDC_BUTTON10, buttonFiltering);
	DDX_Control(pDX, IDC_COMBO5, comboOptions);
	DDX_Control(pDX, IDC_COMBO_ENABLED, comboEnabled);
}


BEGIN_MESSAGE_MAP(Filter, CResizableDialog)
	
	ON_BN_CLICKED(IDC_BUTTON4, Filter::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_BUTTON5, Filter::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON6, Filter::OnBnClickedAddLine)
	ON_BN_CLICKED(IDC_BUTTON8, Filter::OnBnClickedRemoveLine)
	ON_BN_CLICKED(IDC_BUTTON9, Filter::OnBnClickedSaveLine)
	ON_WM_LBUTTONDBLCLK(IDC_LIST1,Filter::OnLButtonDblClk)
	ON_BN_CLICKED(IDC_BUTTON10, Filter::OnBnClickedStartFiltering)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, Filter::OnLvnItemchangedList1)
	ON_WM_SIZE()
	ON_NOTIFY(HDN_ENDTRACK, 0, Filter::OnHdnEndtrackList1)
	ON_NOTIFY(HDN_DIVIDERDBLCLICK, 0, Filter::OnHdnDividerdblclickList1)
END_MESSAGE_MAP()
WINDOWPLACEMENT *p = new WINDOWPLACEMENT();
void Filter::fixSizing(){
	listControl.GetWindowPlacement(p);
	listLength = p->rcNormalPosition.right - p->rcNormalPosition.left;
	int x1,x2,x3,x4;
	x1 = (int)((double)proportions[0]/10000*listLength);
	x2 = (int)((double)proportions[1]/10000*listLength);
	x3 = (int)((double)proportions[2]/10000*listLength);
	x4 = (int)((double)proportions[3]/10000*listLength);
	listControl.SetColumnWidth(0,x1);
	listControl.SetColumnWidth(1,x2);
	listControl.SetColumnWidth(2,x3);
	listControl.SetColumnWidth(3,x4-3);
	int initx;
	CRect rect;
	comboEnabled.GetWindowRect(&rect);
	ScreenToClient(&rect);  
	rect.right = (int)((double)proportions[0]/10000*listLength) + rect.left;
	comboEnabled.MoveWindow(&rect);
	initx = rect.left;

	editMessageBox.GetWindowRect(&rect);
	ScreenToClient(&rect); 
	rect.left = initx+x1+5;
	rect.right = (int)((double)proportions[1]/10000*listLength) + rect.left;
	editMessageBox.MoveWindow(&rect);

	comboOptions.GetWindowRect(&rect);
	ScreenToClient(&rect); 
	rect.left = initx+x1+x2+5;
	rect.right = (int)((double)proportions[2]/10000*listLength) + rect.left;
	comboOptions.MoveWindow(&rect);

	editCommentBox.GetWindowRect(&rect);
	ScreenToClient(&rect); 
	rect.left = initx+x1+x2+x3+5;
	rect.right = (int)((double)proportions[3]/10000*listLength) + rect.left;
	editCommentBox.MoveWindow(&rect);
}
void Filter::OnSize(UINT nType, int cx, int cy)
{
	CResizableDialog::OnSize(nType, cx, cy);
	if(listControl){
		fixSizing();
	}
}

// Filter message handlers
BOOL Filter::OnInitDialog()
{
    CResizableDialog::OnInitDialog();
	
	AutoMove(IDC_LIST1, 0, 0, 100, 100);
	AutoMove(IDC_BUTTON5, 100, 0, 0, 0);

	AutoMove(IDC_BUTTON6, 0, 100, 0, 0);
	AutoMove(IDC_BUTTON8, 0, 100, 0, 0);
	AutoMove(IDC_BUTTON9, 0, 100, 0, 0);
	AutoMove(IDC_BUTTON10, 100, 100, 0, 0);

	AutoMove(IDC_COMBO_ENABLED, 0, 100, 0, 0);
	AutoMove(IDC_EDIT1, 0, 100, 0, 0);
	AutoMove(IDC_COMBO5, 0, 100, 0, 0);
	AutoMove(IDC_EDIT4, 0, 100, 0, 0);


	listControl.SetExtendedStyle(listControl.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	listControl.ModifyStyle(LVS_SINGLESEL, 0);
	listctrl = (CListCtrl *)GetDlgItem(IDC_LIST1);

	comboOptions.AddString("Exclude");
	comboOptions.AddString("Include");
	comboOptions.SetCurSel(1);

	comboEnabled.AddString("Enabled");
	comboEnabled.AddString("Disabled");
	comboEnabled.SetCurSel(1);

	listControl.InsertColumn(0, _T("Enabled"), LVCFMT_LEFT, 80);
	listControl.InsertColumn(1, _T("Message"), LVCFMT_LEFT,264);
	listControl.InsertColumn(2, _T("Include/Exclude"), LVCFMT_LEFT, 111);
	listControl.InsertColumn(3, _T("Comment"), LVCFMT_LEFT, 140);
	savePropersize(0,80);

	return TRUE;
}
CListCtrl * Filter::getListBox(){
	return listctrl;;
}

void Filter::OnCancel()
{
	CResizableDialog::OnCancel();
}
void Filter::OnBnClickedLoad()
{
	transmitting = 0;
	buttonFiltering.SetWindowTextA("Start Filtering");
	stopFiltering();
	hlist->OnBnLoadFltFile();
}


void Filter::OnBnClickedSave()
{
	transmitting = 0;
	buttonFiltering.SetWindowTextA("Start Filtering");
	stopFiltering();


	 TCHAR szFilters[]= _T("Filter Files (*.flt)\0*.flt\0");
	 CFileDialog fileDlg(FALSE, _T("flt"), _T("*.flt"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);
	 if(fileDlg.DoModal() == IDOK){
		CString pathName = fileDlg.GetPathName();
		CFile cfile;
		cfile.Open((pathName), CFile::modeCreate | CFile::modeReadWrite);
		CString strtowrite;
		CString buffer;


		for(int i = 0; i < listControl.GetItemCount(); i++){
			if(listControl.GetItemText(i,2).Compare("Exclude") == 0){
				strtowrite += "Exclude,";
			}else{
				strtowrite += "Include,";
			}
			strtowrite += listControl.GetItemText(i,1) + ",";
			if(listControl.GetItemText(i,0).Compare("Enabled") == 0){
				strtowrite += "True";
			}else{
				strtowrite += "False";
			}
			strtowrite +="," + listControl.GetItemText(i,3);
			strtowrite += "\n";
		}
		cfile.Write(strtowrite, strtowrite.GetLength());         
		cfile.Flush();
	}
}

void Filter::OnBnClickedAddLine()
{
	if(transmitting == 1){
		return;
	}

	cSel = -1;
	CString	buffer;
	editMessageBox.GetWindowTextA(buffer);
	if(checkWildcardValidity(buffer) == false){
		AfxMessageBox("Invalid Message");
		return;
	}
	int n = listControl.GetItemCount();
	int nIndex = listControl.InsertItem(n, _T("Disabled"));
	comboEnabled.GetWindowTextA(buffer);
	listControl.SetItemText(nIndex, 0, _T(buffer));
	editMessageBox.GetWindowTextA(buffer);
	listControl.SetItemText(nIndex, 1, _T(buffer));
	if(comboOptions.GetCurSel() == 0){
		listControl.SetItemText(nIndex, 2, _T("Exclude"));
	}else{
		listControl.SetItemText(nIndex, 2, _T("Include"));
	}
	editCommentBox.GetWindowTextA(buffer);
	listControl.SetItemText(nIndex, 3, _T(buffer));

	transmitting = 0;
	buttonFiltering.SetWindowTextA("Start Filtering");
	stopFiltering();
}


void Filter::OnBnClickedRemoveLine()
{
	if(cSel != -1){
		listControl.DeleteItem(cSel);
		uEdit = 0;
		editMessageBox.SetWindowTextA("");
		editCommentBox.SetWindowTextA("");
		comboEnabled.SetCurSel(1);
		comboOptions.SetCurSel(1);
		cSel = -1;
		transmitting = 0;
		buttonFiltering.SetWindowTextA("Start Filtering");
		stopFiltering();
	}
}


void Filter::OnBnClickedSaveLine()
{
	if(transmitting == 1){
		return;
	}
	if(cSel != -1 && uEdit == 1){
		CString	buffer;
		editMessageBox.GetWindowTextA(buffer);
		if(checkWildcardValidity(buffer) == false){
			AfxMessageBox("Invalid Message");
			return;
		}
		CString message;
		comboEnabled.GetWindowTextA(message);
		listControl.SetItemText(cSel,0,message);
		editMessageBox.GetWindowTextA(message);
		listControl.SetItemText(cSel,1,message);

		editCommentBox.GetWindowTextA(message);
		listControl.SetItemText(cSel,3,message);

		if (comboOptions.GetCurSel() == 0){
			listControl.SetItemText(cSel,2,"Exclude");
		}else{
			listControl.SetItemText(cSel,2,"Include");
		}
	}
	
}


void Filter::OnBnClickedStartFiltering()
{
	if(transmitting == 0){
		transmitting = 1;
		buttonFiltering.SetWindowTextA("Stop Filtering");
		startFiltering();
	}else{
		transmitting = 0;
		buttonFiltering.SetWindowTextA("Start Filtering");
		stopFiltering();
	}
}
void Filter::stopFiltering(){
	transmitting = 0;
	editMessageBox.EnableWindow(true);
	editCommentBox.EnableWindow(true);
	comboEnabled.EnableWindow(true);
	comboOptions.EnableWindow(true);
	GetDlgItem(IDC_BUTTON6)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON8)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON9)->EnableWindow(true);
	CString enabled = listControl.GetItemText(cSel,0);
	CString message = listControl.GetItemText(cSel,1);
	CString options = listControl.GetItemText(cSel,2);
	CString comment = listControl.GetItemText(cSel,3);
	editMessageBox.SetWindowTextA(message);
	editCommentBox.SetWindowTextA(comment);
	if(enabled.Compare("Enabled") == 0){
		comboEnabled.SetCurSel(1);
	}else{
		comboEnabled.SetCurSel(0);
	}
	if(options.Compare("Transmit") == 0){
		comboOptions.SetCurSel(1);
	}else{
		comboOptions.SetCurSel(0);
	}
	uEdit = 1;
}
void Filter::startFiltering(){
	editMessageBox.EnableWindow(false);
	editCommentBox.EnableWindow(false);
	comboEnabled.EnableWindow(false);
	comboOptions.EnableWindow(false);
	GetDlgItem(IDC_BUTTON6)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON8)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON9)->EnableWindow(false);
	amtOfExcludes = 0;
	amtOfIncludes = 0;

	uEdit = 0;
	editMessageBox.SetWindowTextA("");
	editCommentBox.SetWindowTextA("");

	comboEnabled.SetCurSel(1);
	comboOptions.SetCurSel(1);

	for(int i = 0; i < listControl.GetItemCount(); i++){
		if(listControl.GetItemText(i,0).Compare("Enabled") == 0){
			if(listControl.GetItemText(i,2).Compare("Include") == 0){
				CString data = listControl.GetItemText(i,1);
				data.Remove(' ');
				listOfIncludes[amtOfIncludes] = data;
				amtOfIncludes++;
			}else{
				CString data = listControl.GetItemText(i,1);
				data.Remove(' ');
				listOfExcludes[amtOfExcludes] = data;
				amtOfExcludes++;
			}
		}
	}

}

void Filter::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(transmitting == 1){
		return;
	}
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if(pNMLV->uNewState == 0){
		return;
	}
	uEdit = 0;
	cSel = pNMLV->iItem;
	CString enabled = listControl.GetItemText(cSel,0);
	CString message = listControl.GetItemText(cSel,1);
	CString options = listControl.GetItemText(cSel,2);
	CString comment = listControl.GetItemText(cSel,3);
	editMessageBox.SetWindowTextA(message);
	editCommentBox.SetWindowTextA(comment);
	if(enabled.Compare("Enabled") == 0){
		comboEnabled.SetCurSel(1);
	}else{
		comboEnabled.SetCurSel(0);
	}
	if(options.Compare("Include") == 0){
		comboOptions.SetCurSel(1);
	}else{
		comboOptions.SetCurSel(0);
	}
	uEdit = 1;
	*pResult = 0;
}

int Filter::getTotalSize(){
	int tot = 0;
	tot += listctrl->GetColumnWidth(0);
	tot += listctrl->GetColumnWidth(1);
	tot += listctrl->GetColumnWidth(2);
	tot += listctrl->GetColumnWidth(3);
	return tot;
}
void Filter::savePropersize(int index, int newVal){
	sizes[0] = listctrl->GetColumnWidth(0);
	sizes[1] = listctrl->GetColumnWidth(1);
	sizes[2] = listctrl->GetColumnWidth(2);
	sizes[3] = listctrl->GetColumnWidth(3);
	sizes[index] = newVal;
	proportions[0] = (int)(sizes[0] / (double)(sizes[0]+sizes[1]+sizes[2]+sizes[3])*10000);
	proportions[1] = (int)(sizes[1] / (double)(sizes[0]+sizes[1]+sizes[2]+sizes[3])*10000);
	proportions[2] = (int)(sizes[2] / (double)(sizes[0]+sizes[1]+sizes[2]+sizes[3])*10000);
	proportions[3] = (int)(sizes[3] / (double)(sizes[0]+sizes[1]+sizes[2]+sizes[3])*10000);
	int tot = proportions[0] + proportions[1] + proportions[2] + proportions[3];
	proportions[3] += 10000-tot;
}
void Filter::restorePropersize(){
	listctrl->SetColumnWidth(0,sizes[0]);
	listctrl->SetColumnWidth(1,sizes[1]);
	listctrl->SetColumnWidth(2,sizes[2]);
	listctrl->SetColumnWidth(3,sizes[3]-3);
}

void Filter::OnHdnEndtrackList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	int tot = getTotalSize();
	tot -= listctrl->GetColumnWidth(phdr->iItem);
	tot += phdr->pitem->cxy;
	int diff = phdr->pitem->cxy - listctrl->GetColumnWidth(phdr->iItem);
	if(phdr->iItem == 3){
		phdr->pitem->cxy = listctrl->GetColumnWidth(phdr->iItem);
	}else if(diff > 0){
		if(listctrl->GetColumnWidth(phdr->iItem+1)-diff<10){
			phdr->pitem->cxy = listctrl->GetColumnWidth(phdr->iItem);
		}else{
			listctrl->SetColumnWidth(phdr->iItem+1,listctrl->GetColumnWidth(phdr->iItem+1)-diff);
		}
	}else{
		int resize = diff * -1;
		if(listctrl->GetColumnWidth(phdr->iItem)-resize<10){
			phdr->pitem->cxy = listctrl->GetColumnWidth(phdr->iItem);
		}else{
			listctrl->SetColumnWidth(phdr->iItem+1,listctrl->GetColumnWidth(phdr->iItem+1)+resize);
		}
	}
	savePropersize(phdr->iItem,phdr->pitem->cxy);
	fixSizing();
	*pResult = 0;
}



void Filter::OnHdnDividerdblclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	restorePropersize();
	*pResult = 0;
}

