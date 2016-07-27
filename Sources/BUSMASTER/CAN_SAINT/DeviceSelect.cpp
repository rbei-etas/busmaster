// DeviceSelect.cpp : implementation file
//
#include "CAN_SAINT_stdafx.h"
#include <afxtempl.h>  
#include "DeviceSelect.h"

// DeviceSelect dialog

CString deviceList;
CString selectedDevice;
DeviceSelect::DeviceSelect(CString list,CWnd* pParent /*=NULL*/)
	: CDialog(DeviceSelect::IDD, pParent)
{
	deviceList = list;
}
BOOL DeviceSelect::OnInitDialog()
{
	CDialog::OnInitDialog();

	listControl.SetExtendedStyle(listControl.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT  );
	listControl.ModifyStyle(LVS_SINGLESEL , 0);
	listControl.InsertColumn(0, _T("Device ID"), LVCFMT_LEFT, 100);
	listControl.InsertColumn(1, _T("Device Name"), LVCFMT_LEFT, 206);

	int pos;
	int count = 0;
	CString id;
	pos = deviceList.Find(",",0);
	CString device;
	while(pos >= 0)
	{
		device = deviceList.Left(pos);
		deviceList.Delete(0,pos + 1);
		pos = deviceList.Find(",",0);
		id = deviceList.Left(pos);
		deviceList.Delete(0,pos + 1);
		char * temp = (char *)malloc(100);
		itoa(count,temp,10);
		CString str(temp);
		int nIndex = listControl.InsertItem(count, str);
		listControl.SetItemText(nIndex, 1, _T(device));
		pos = deviceList.Find(",",0);
		count++;
	}
	listControl.SetFocus();
	listControl.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED ); 
	selectedDevice = device;
	return true;
}

DeviceSelect::~DeviceSelect()
{
}

CString DeviceSelect::getSelectedDevice(){
	return selectedDevice;
}
void DeviceSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, listControl);
}


BEGIN_MESSAGE_MAP(DeviceSelect, CDialog)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, DeviceSelect::OnLvnItemchangedList1)
END_MESSAGE_MAP()


// DeviceSelect message handlers


void DeviceSelect::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int csel = pNMLV->iItem;
	selectedDevice = listControl.GetItemText(csel,1);
	*pResult = 0;
}
