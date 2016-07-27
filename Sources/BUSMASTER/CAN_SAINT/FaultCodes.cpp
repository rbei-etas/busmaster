// FaultCodes.cpp : implementation file
//

#include "CAN_SAINT_stdafx.h"
#include <afxtempl.h>  
#include "FaultCodes.h"
#include "HardwareListing.h"
#include <hash_map>
#include "CAN_SAINT.h"
CString mem;
CString clipboardMem;
CString clipboardHighlightMem;
CButton * buttonRead;
int sizlist[5];
int curNumOfFaults = 0;
CHardwareListing* hlisting;


FaultCodes::FaultCodes(CHardwareListing* h,CWnd* pParent /*=NULL*/)
	: CResizableDialog(FaultCodes::IDD, pParent)
{
	hlisting = h;
}

FaultCodes::~FaultCodes()
{
}

void FaultCodes::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listControl);
}

void FaultCodes::addNewFault(CString buffer){
	CString ID = buffer.Mid(hlisting->ID_Start*2,(hlisting->ID_End-hlisting->ID_Start+1)*2);
	CString DTC = buffer.Mid(6+(hlisting->DTC_Start*2),(hlisting->DTC_End-hlisting->DTC_Start+1)*2);
	CString Type = buffer.Mid(6+(hlisting->Type_Start*2),(hlisting->Type_End-hlisting->Type_Start+1)*2);
	CString Status = buffer.Mid(6+(hlisting->Status_End*2),(hlisting->Status_End-hlisting->Status_Start+1)*2);
	CString FaultName;
	unsigned int x = _Stoulx(Status, nullptr, 16,nullptr);
	unsigned int mask = _Stoulx(hlisting->Active_History_Mask, nullptr, 10,nullptr);

	int temp = x & mask;
	if(temp == 2){
		ID = "Active";
	}else{
		ID = "History";
	}


	DTC = CString ("0x") + DTC;
	Type = CString("0x") + Type;
	Status = CString("0x") + Status;


	
	std::string sDTC((LPCTSTR)DTC);
	int found = 0;
	for(int i = 0; i < CHardwareListing::getFault()[sDTC].size(); i++){
		std::string sTYPE(CHardwareListing::getFault()[sDTC].at(i).type);
		CString cTYPE(sTYPE.c_str());
		if(cTYPE.Compare(Type) == 0){
			found = 1;
			std::string sFAULT(CHardwareListing::getFault()[sDTC].at(i).faultCode);
			FaultName = sFAULT.c_str();
			break;
		}
	}
	if(DTC.Compare("0x0000") == 0){
		FaultName = "End of faults";
		ID = "";
		DTC = "";
		Type = "";
		Status = "";
	}else if(found == 0){
		FaultName = "Error: Fault Code not found.";
	}

	clipboardMem = clipboardMem + ID + "  " + FaultName;
	for(int i = 40- FaultName.GetLength();i > 0;i--){
		clipboardMem = clipboardMem + " ";
	}
	
	clipboardMem = clipboardMem + "  " +
		DTC + "  " + Type + "  "  + Status + "  " +
		 "\r\n";
	int nIndex = m_listControl.InsertItem(curNumOfFaults, _T(ID));
	m_listControl.SetItemText(nIndex, 1, _T(FaultName));
	m_listControl.SetItemText(nIndex, 2, _T(DTC));
	m_listControl.SetItemText(nIndex, 3, _T(Type));
	m_listControl.SetItemText(nIndex, 4, _T(Status));

	curNumOfFaults++;


}

void FaultCodes::OnCancel()
{
	CResizableDialog::OnCancel();
}
BOOL FaultCodes::OnInitDialog()
{
    CResizableDialog::OnInitDialog();
	AutoMove(IDC_LIST1, 0, 0, 100, 100);

	AutoMove(IDC_BUTTON_REQUEST, 0, 100, 0, 0);
	AutoMove(IDC_BUTTON_CLEAR, 0, 100, 0, 0);
	AutoMove(IDC_BUTTON_COPY, 100, 100, 0, 0);
	AutoMove(IDC_BUTTON_COPY_ALL, 100, 100, 0, 0);
	buttonRead = (CButton *)GetDlgItem(IDC_BUTTON_REQUEST);
	m_listControl.SetExtendedStyle(m_listControl.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );


	m_listControl.ModifyStyle(LVS_SINGLESEL , 0);

	m_listControl.InsertColumn(0, _T("Active/History"), LVCFMT_LEFT, 80);
	m_listControl.InsertColumn(1, _T("Fault Code"), LVCFMT_LEFT,300);
	m_listControl.InsertColumn(2, _T("DTC"), LVCFMT_LEFT, 70);
	m_listControl.InsertColumn(3, _T("Type"), LVCFMT_LEFT, 70);
	m_listControl.InsertColumn(4, _T("Status"), LVCFMT_LEFT, 110);
	savethesize(0,80);
	return TRUE;
}
void FaultCodes::OnSize(UINT nType, int cx, int cy)
{
	CResizableDialog::OnSize(nType, cx, cy);
	if(m_listControl){
		fixthesize();
	}
}
BEGIN_MESSAGE_MAP(FaultCodes, CResizableDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, FaultCodes::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_REQUEST, FaultCodes::OnBnClickedButtonRequest)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, FaultCodes::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_COPY, FaultCodes::OnBnClickedButtonCopy)
	ON_BN_CLICKED(IDC_BUTTON_COPY_ALL, FaultCodes::OnBnClickedButtonCopyAll)
	ON_NOTIFY(HDN_ENDTRACK, 0, FaultCodes::OnHdnEndtrackList1)
	ON_NOTIFY(HDN_DIVIDERDBLCLICK, 0, FaultCodes::OnHdnDividerdblclickList1)
END_MESSAGE_MAP()


// FaultCodes message handlers

void FaultCodes::finishedFault(CString mem){
	buttonRead->SetWindowTextA("Read Faults");
	m_listControl.DeleteAllItems();
	CString buff;
	clipboardMem = "";
	for(int i = 0; i < mem.GetLength();i++){

		if(mem.GetAt(i) == '\n'){
			addNewFault(buff);
			buff = "";
			i++;
		}
		buff += mem.GetAt(i);
	}
}
void FaultCodes::faultFailed(){
	buttonRead->SetWindowTextA("Read Faults");
}


void FaultCodes::fillBuffer()
{
	POSITION pos = m_listControl.GetFirstSelectedItemPosition();
	if (pos != NULL) {
		while (pos){
			int nItem = m_listControl.GetNextSelectedItem(pos);	
			CString ID = m_listControl.GetItemText(nItem,0);
			CString code = m_listControl.GetItemText(nItem,1);
			CString DTC = m_listControl.GetItemText(nItem,2);
			CString Type = m_listControl.GetItemText(nItem,3);
			CString Status = m_listControl.GetItemText(nItem,4);
			
			clipboardHighlightMem = clipboardHighlightMem + ID + "  " + code;
			for(int i = 40- code.GetLength();i > 0;i--){
				clipboardHighlightMem = clipboardHighlightMem + " ";
			}
	
			clipboardHighlightMem = clipboardHighlightMem + "  " +
				DTC + "  " + Type + "  "  + Status + "  " +
				 "\r\n";
	

		}
		clipboardHighlightMem = clipboardHighlightMem + "End of Faults";
	}
}

void FaultCodes::OnBnClickedButtonLoad()
{
	hlisting->OnBnClickedButton3();
}


void FaultCodes::OnBnClickedButtonRequest()
{
	if(hlisting->OnBnClickedButton2()){
		buttonRead->SetWindowTextA("Loading Faults...");
	}
}


void FaultCodes::OnBnClickedButtonClear()
{
	m_listControl.DeleteAllItems();
	int nIndex = m_listControl.InsertItem(0, _T(""));
	m_listControl.SetItemText(nIndex, 1, _T("Cleared Faults"));
	CAN_SendFaultCodeClear();
}


void FaultCodes::OnBnClickedButtonCopy()
{
	clipboardHighlightMem = "";
	fillBuffer();


	OpenClipboard();
	HGLOBAL clipbuffer;
	char * buffer;
	EmptyClipboard();
	clipbuffer = GlobalAlloc(GMEM_DDESHARE, clipboardHighlightMem.GetLength()+1);
	buffer = (char*)GlobalLock(clipbuffer);
	strcpy(buffer, LPCSTR(clipboardHighlightMem));
	GlobalUnlock(clipbuffer);
	SetClipboardData(CF_TEXT,clipbuffer);
	CloseClipboard();
	AfxMessageBox("Copied to clipboard");
}


void FaultCodes::OnBnClickedButtonCopyAll()
{
	OpenClipboard();
	HGLOBAL clipbuffer;
	char * buffer;
	EmptyClipboard();
	clipbuffer = GlobalAlloc(GMEM_DDESHARE, clipboardMem.GetLength()+1);
	buffer = (char*)GlobalLock(clipbuffer);
	strcpy(buffer, LPCSTR(clipboardMem));
	GlobalUnlock(clipbuffer);
	SetClipboardData(CF_TEXT,clipbuffer);
	CloseClipboard();
	AfxMessageBox("Copied to clipboard");
}

int FaultCodes::getTotSize(){
	int tot = 0;
	tot += m_listControl.GetColumnWidth(0);
	tot += m_listControl.GetColumnWidth(1);
	tot += m_listControl.GetColumnWidth(2);
	tot += m_listControl.GetColumnWidth(3);
	tot += m_listControl.GetColumnWidth(4);
	return tot;
}

int propor[5];
void FaultCodes::savethesize(int index, int newVal){
	sizlist[0] = m_listControl.GetColumnWidth(0);
	sizlist[1] = m_listControl.GetColumnWidth(1);
	sizlist[2] = m_listControl.GetColumnWidth(2);
	sizlist[3] = m_listControl.GetColumnWidth(3);
	sizlist[4] = m_listControl.GetColumnWidth(4);
	sizlist[index] = newVal;
	propor[0] = (int)(sizlist[0] / (double)(sizlist[0]+sizlist[1]+sizlist[2]+sizlist[3]+sizlist[4])*10000);
	propor[1] = (int)(sizlist[1] / (double)(sizlist[0]+sizlist[1]+sizlist[2]+sizlist[3]+sizlist[4])*10000);
	propor[2] = (int)(sizlist[2] / (double)(sizlist[0]+sizlist[1]+sizlist[2]+sizlist[3]+sizlist[4])*10000);
	propor[3] = (int)(sizlist[3] / (double)(sizlist[0]+sizlist[1]+sizlist[2]+sizlist[3]+sizlist[4])*10000);
	propor[4] = (int)(sizlist[4] / (double)(sizlist[0]+sizlist[1]+sizlist[2]+sizlist[3]+sizlist[4])*10000);
	int tot = propor[0] + propor[1] + propor[2] + propor[3] + propor[4];
	propor[4] += 10000-tot;
}
WINDOWPLACEMENT *wwp = new WINDOWPLACEMENT();
int listl;
void FaultCodes::fixthesize(){
	m_listControl.GetWindowPlacement(wwp);
	listl = wwp->rcNormalPosition.right - wwp->rcNormalPosition.left;
	int x1,x2,x3,x4,x5;
	x1 = (int)((double)propor[0]/10000*listl);
	x2 = (int)((double)propor[1]/10000*listl);
	x3 = (int)((double)propor[2]/10000*listl);
	x4 = (int)((double)propor[3]/10000*listl);
	x5 = (int)((double)propor[4]/10000*listl);
	m_listControl.SetColumnWidth(0,x1);
	m_listControl.SetColumnWidth(1,x2);
	m_listControl.SetColumnWidth(2,x3);
	m_listControl.SetColumnWidth(3,x4);
	m_listControl.SetColumnWidth(4,x5-3);
}
void FaultCodes::OnHdnEndtrackList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	int tot = getTotSize();
	tot -= m_listControl.GetColumnWidth(phdr->iItem);
	tot += phdr->pitem->cxy;
	int diff = phdr->pitem->cxy - m_listControl.GetColumnWidth(phdr->iItem);
	if(phdr->iItem == 4){
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
	savethesize(phdr->iItem,phdr->pitem->cxy);
	fixthesize();
	*pResult = 0;
}


void FaultCodes::OnHdnDividerdblclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	fixthesize();
	*pResult = 0;
}
