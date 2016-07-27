// GroupEditor.cpp : implementation file
//
#include "CAN_SAINT_stdafx.h"
#include <afxtempl.h>  
#include "HardwareListing.h"
#include "GroupEditor.h"
#include "CAN_SAINT.h"

CHardwareListing* hl3;
CListCtrl * lctrl;
int csel;
int ised;
int prop[4];
int size[4];
int listLen = 0;
// GroupEditor dialog

GroupEditor::GroupEditor(CHardwareListing* h,CWnd* pParent /*=NULL*/)
	: CResizableDialog(GroupEditor::IDD, pParent)
{
	hl3 = h;
}
void GroupEditor::OnCancel()
{
	CResizableDialog::OnCancel();
}
GroupEditor::~GroupEditor()
{

}



BOOL GroupEditor::OnInitDialog()
{
    CResizableDialog::OnInitDialog();

	AutoMove(IDC_LIST1, 0, 0, 100, 100);
	AutoMove(IDC_BUTTON_SAVE_FILE, 100, 0, 0, 0);

	AutoMove(IDC_BUTTON_ADD_LINE, 0, 100, 0, 0);
	AutoMove(IDC_BUTTON_REMOVE_LINE, 0, 100, 0, 0);
	AutoMove(IDC_BUTTON_SAVE_LINE, 0, 100, 0, 0);
	AutoMove(IDC_BUTTON_SEND, 100, 100, 0, 0);

	AutoMove(IDC_COMBO_ENABLED, 0, 100, 0, 0);
	AutoMove(IDC_EDIT1, 0, 100, 0, 0);
	AutoMove(IDC_EDIT15, 0, 100, 0, 0);
	AutoMove(IDC_EDIT4, 0, 100, 100, 0);

	comboEnabled.AddString("Enabled");
	comboEnabled.AddString("Disabled");
	comboEnabled.SetCurSel(1);

	listControl.SetExtendedStyle(listControl.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	listControl.ModifyStyle(LVS_SINGLESEL , 0);

	lctrl =  (CListCtrl *)GetDlgItem(IDC_LIST1);

	listControl.InsertColumn(0, _T("Enabled"), LVCFMT_LEFT, 80);
	listControl.InsertColumn(1, _T("Message"), LVCFMT_LEFT,264);
	listControl.InsertColumn(2, _T("Delay"), LVCFMT_LEFT, 111);
	listControl.InsertColumn(3, _T("Comment"), LVCFMT_LEFT, 155);
	saveSize(0,80);
	return TRUE;
}
void GroupEditor::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, editMessage);
	DDX_Control(pDX, IDC_EDIT15, editDelay);
	DDX_Control(pDX, IDC_EDIT4, editComment);
	DDX_Control(pDX, IDC_LIST1, listControl);
	DDX_Control(pDX, IDC_COMBO_ENABLED, comboEnabled);
}


BEGIN_MESSAGE_MAP(GroupEditor, CResizableDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, GroupEditor::OnLvnItemchangedList1)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, GroupEditor::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_FILE, GroupEditor::OnBnClickedButtonSaveFile)
	ON_BN_CLICKED(IDC_BUTTON_ADD_LINE, GroupEditor::OnBnClickedButtonAddLine)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_LINE, GroupEditor::OnBnClickedButtonRemoveLine)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_LINE, GroupEditor::OnBnClickedButtonSaveLine)
	ON_BN_CLICKED(IDC_BUTTON_SEND, GroupEditor::OnBnClickedButtonSend)
	ON_NOTIFY(HDN_ENDTRACK, 0, GroupEditor::OnHdnEndtrackList1)
	ON_NOTIFY(HDN_DIVIDERDBLCLICK, 0, GroupEditor::OnHdnDividerdblclickList1)
END_MESSAGE_MAP()
void GroupEditor::OnSize(UINT nType, int cx, int cy)
{
	CResizableDialog::OnSize(nType, cx, cy);
	if(listControl){
		fixSize();
	}
}

// GroupEditor message handlers
CListCtrl * GroupEditor::getListBox(){
	return lctrl;
}

void GroupEditor::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if(pNMLV->uNewState == 0){
		return;
	}
	ised = 0;
	csel = pNMLV->iItem;
	CString enabled = listControl.GetItemText(csel,0);
	CString message = listControl.GetItemText(csel,1);
	CString delay = listControl.GetItemText(csel,2);
	CString comment = listControl.GetItemText(csel,3);

	editMessage.SetWindowTextA(message);
	if(enabled.Compare("Enabled") == 0){
		comboEnabled.SetCurSel(1);
	}else{
		comboEnabled.SetCurSel(0);
	}
	editDelay.SetWindowTextA(delay);
	editComment.SetWindowTextA(comment);
	ised = 1;
	*pResult = 0;
}


void GroupEditor::OnBnClickedButtonLoad()
{
	hl3->OnBnLoadGrpFile();
}


void GroupEditor::OnBnClickedButtonSaveFile()
{
	
	 TCHAR szFilters[]= _T("Group Files (*.grp)\0*.grp\0");
	 CFileDialog fileDlg(FALSE, _T("grp"), _T("*.grp"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);
	 if(fileDlg.DoModal() == IDOK){
		CString pathName = fileDlg.GetPathName();
		CFile cfile;
		cfile.Open((pathName), CFile::modeCreate | CFile::modeReadWrite);
		CString strtowrite;
		CString buffer;


		for(int i = 0; i < listControl.GetItemCount(); i++){
			strtowrite += listControl.GetItemText(i,0) + " ";
			strtowrite += listControl.GetItemText(i,2) + " \t";
			strtowrite += listControl.GetItemText(i,1) + "\"";
			strtowrite += listControl.GetItemText(i,3) + "\n";
		}
		cfile.Write(strtowrite, strtowrite.GetLength());         
		cfile.Flush();
	}
}


void GroupEditor::OnBnClickedButtonAddLine()
{
	csel = -1;
	CString	buffer;
	editMessage.GetWindowTextA(buffer);
	if(checkValidity(buffer) == false){
		AfxMessageBox("Invalid Message");
		return;
	}
	int n = listControl.GetItemCount();
	int nIndex = listControl.InsertItem(n, _T("Disabled"));
	comboEnabled.GetWindowTextA(buffer);
	listControl.SetItemText(nIndex, 0, _T(buffer));

	editMessage.GetWindowTextA(buffer);
	listControl.SetItemText(nIndex, 1, _T(buffer));
	editDelay.GetWindowTextA(buffer);
	listControl.SetItemText(nIndex, 2, _T(buffer));
	editComment.GetWindowTextA(buffer);
	listControl.SetItemText(nIndex, 3, _T(buffer));
}


void GroupEditor::OnBnClickedButtonRemoveLine()
{
	if(csel != -1){
		listControl.DeleteItem(csel);
		ised = 0;
		editMessage.SetWindowTextA("");
		editDelay.SetWindowTextA("");
		editComment.SetWindowTextA("");
		comboEnabled.SetCurSel(0);
		csel = -1;
	}
}


void GroupEditor::OnBnClickedButtonSaveLine()
{
	if(csel != -1 && ised == 1){
		CString	buffer;
		editMessage.GetWindowTextA(buffer);
		if(checkValidity(buffer) == false){
			AfxMessageBox("Invalid Message");
			return;
		}
		CString message;
		comboEnabled.GetWindowTextA(message);
		listControl.SetItemText(csel,0,message);

		editMessage.GetWindowTextA(message);
		listControl.SetItemText(csel,1,message);

		editDelay.GetWindowTextA(message);
		listControl.SetItemText(csel,2,message);

		editComment.GetWindowTextA(message);
		listControl.SetItemText(csel,3,message);
	}
}


void GroupEditor::OnBnClickedButtonSend()
{
	CString message;
	editMessage.GetWindowTextA(message);
	if(checkValidity(message) == false){
		AfxMessageBox("Invalid Message");
		return;
	}
	CAN_SendMessage(message);
}
WINDOWPLACEMENT * pp = new WINDOWPLACEMENT();
void GroupEditor::fixSize(){
	listControl.GetWindowPlacement(pp);
	listLen = pp->rcNormalPosition.right - pp->rcNormalPosition.left;
	int x1,x2,x3,x4;
	x1 = (int)((double)prop[0]/10000*listLen);
	x2 = (int)((double)prop[1]/10000*listLen);
	x3 = (int)((double)prop[2]/10000*listLen);
	x4 = (int)((double)prop[3]/10000*listLen);
	listControl.SetColumnWidth(0,x1);
	listControl.SetColumnWidth(1,x2);
	listControl.SetColumnWidth(2,x3);
	listControl.SetColumnWidth(3,x4-3);
	int initx;
	CRect rect;
	comboEnabled.GetWindowRect(&rect);
	ScreenToClient(&rect); 
	rect.right = (int)((double)prop[0]/10000*listLen) + rect.left;
	comboEnabled.MoveWindow(&rect);
	initx = rect.left;

	editMessage.GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.left = initx+x1+5;
	rect.right = (int)((double)prop[1]/10000*listLen) + rect.left;
	editMessage.MoveWindow(&rect);

	editDelay.GetWindowRect(&rect);
	ScreenToClient(&rect); 
	rect.left = initx+x1+x2+5;
	rect.right = (int)((double)prop[2]/10000*listLen) + rect.left;
	editDelay.MoveWindow(&rect);

	editComment.GetWindowRect(&rect);
	ScreenToClient(&rect); 
	rect.left = initx+x1+x2+x3+5;
	rect.right = (int)((double)prop[3]/10000*listLen) + rect.left;
	editComment.MoveWindow(&rect);
}

int GroupEditor::getTotal(){
	int tot = 0;
	tot += listControl.GetColumnWidth(0);
	tot += listControl.GetColumnWidth(1);
	tot += listControl.GetColumnWidth(2);
	tot += listControl.GetColumnWidth(3);
	return tot;
}
void GroupEditor::saveSize(int index, int newVal){
	size[0] = listControl.GetColumnWidth(0);
	size[1] = listControl.GetColumnWidth(1);
	size[2] = listControl.GetColumnWidth(2);
	size[3] = listControl.GetColumnWidth(3);
	size[index] = newVal;
	prop[0] = (int)(size[0] / (double)(size[0]+size[1]+size[2]+size[3])*10000);
	prop[1] = (int)(size[1] / (double)(size[0]+size[1]+size[2]+size[3])*10000);
	prop[2] = (int)(size[2] / (double)(size[0]+size[1]+size[2]+size[3])*10000);
	prop[3] = (int)(size[3] / (double)(size[0]+size[1]+size[2]+size[3])*10000);
	int tot = prop[0] + prop[1] + prop[2] + prop[3];
	prop[3] += 10000-tot;
}

void GroupEditor::OnHdnEndtrackList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	int tot = getTotal();
	tot -= listControl.GetColumnWidth(phdr->iItem);
	tot += phdr->pitem->cxy;
	int diff = phdr->pitem->cxy - listControl.GetColumnWidth(phdr->iItem);
	if(phdr->iItem == 3){
		phdr->pitem->cxy = listControl.GetColumnWidth(phdr->iItem);
	}else if(diff > 0){
		if(listControl.GetColumnWidth(phdr->iItem+1)-diff<10){
			phdr->pitem->cxy = listControl.GetColumnWidth(phdr->iItem);
		}else{
			listControl.SetColumnWidth(phdr->iItem+1,listControl.GetColumnWidth(phdr->iItem+1)-diff);
		}
	}else{
		int resize = diff * -1;
		if(listControl.GetColumnWidth(phdr->iItem)-resize<10){
			phdr->pitem->cxy = listControl.GetColumnWidth(phdr->iItem);
		}else{
			listControl.SetColumnWidth(phdr->iItem+1,listControl.GetColumnWidth(phdr->iItem+1)+resize);
		}
	}
	saveSize(phdr->iItem,phdr->pitem->cxy);
	fixSize();
	*pResult = 0;
}
void GroupEditor::restoreSize(){
	listControl.SetColumnWidth(0,size[0]);
	listControl.SetColumnWidth(1,size[1]);
	listControl.SetColumnWidth(2,size[2]);
	listControl.SetColumnWidth(3,size[3]-3);
}

void GroupEditor::OnHdnDividerdblclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	restoreSize();
	*pResult = 0;
}
