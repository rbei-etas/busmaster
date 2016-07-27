// CANConfig.cpp : implementation file
//

#include "CAN_SAINT_stdafx.h"
#include <afxtempl.h>  
#include "HardwareListing.h"
#include "CANConfig.h"
#include "CAN_SAINT.h"
// CCANConfig dialog
int can1_speed;
int can2_speed;
int can1_type;
int can2_type;
int can1_enabled;
int can2_enabled;

int can1EnabledBox;
int can2EnabledBox;

CComboBox *can1SpeedBox;
CComboBox *can2SpeedBox;
CComboBox *can1TypeBox;
CComboBox *can2TypeBox;
CHardwareListing *hll;

CCANConfig::CCANConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CCANConfig::IDD, pParent)
{
	hll = new CHardwareListing();
}
CCANConfig::~CCANConfig()
{
}

void CCANConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_CAN_1, checkCAN1);
	DDX_Control(pDX, IDC_CHECK_CAN_2, checkCAN2);
}


BEGIN_MESSAGE_MAP(CCANConfig, CDialog)
	ON_BN_CLICKED(IDOK, CCANConfig::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CCANConfig::OnInitDialog(){
	hll->loadConfig();
	CDialog::OnInitDialog();
	can1SpeedBox = (CComboBox *)GetDlgItem(IDC_COMBO_CAN_1_SPEED);
	can2SpeedBox = (CComboBox *)GetDlgItem(IDC_COMBO_CAN_2_SPEED);
	can1TypeBox = (CComboBox *)GetDlgItem(IDC_COMBO_CAN_1_TYPE);
	can2TypeBox = (CComboBox *)GetDlgItem(IDC_COMBO_CAN_2_TYPE);

	for(int i = 0; i < hll->CAN1_Speed.size();i++){
		can1SpeedBox->AddString(hll->CAN1_Speed[i]);
	}
	can1SpeedBox->SetCurSel(0);
	for(int i = 0; i < hll->CAN2_Speed.size();i++){
		can2SpeedBox->AddString(hll->CAN2_Speed[i]);
	}
	can2SpeedBox->SetCurSel(0);

	for(int i = 0; i < hll->CAN1_Type.size();i++){
		can1TypeBox->AddString(hll->CAN1_Type[i]);
	}
	can1TypeBox->SetCurSel(0);

	for(int i = 0; i < hll->CAN2_Type.size();i++){
		can2TypeBox->AddString(hll->CAN2_Type[i]);
	}
	can2TypeBox->SetCurSel(0);
	return 1;
}
// CCANConfig message handlers

void updateCAN1(){
	CAN_SendMessage(hll->CAN_Init_Message); 
	if(can1_enabled == 1){
		CAN_SendMessage(hll->CAN_Prepare_Message); 
		CAN_SendMessage(hll->CAN1_Set); 
		CAN_SendMessage(hll->CAN1_Type_Messages[can1_type]); 
		CAN_SendMessage(hll->CAN1_Speed_Messages[can1_speed]); 
	}else{
		CAN_SendMessage(hll->CAN_Prepare_Message);
		for(int i = 0; i <  hll->CAN1_Sleep_Messages.size();i++){
			CAN_SendMessage(hll->CAN1_Sleep_Messages[i]);
		}
	}
}


void updateCAN2(){
	CAN_SendMessage(hll->CAN_Init_Message);
	if(can2_enabled == 1){
		CAN_SendMessage(hll->CAN_Prepare_Message); 
		CAN_SendMessage(hll->CAN2_Set);
		CAN_SendMessage(hll->CAN2_Type_Messages[can2_type]); 
		CAN_SendMessage(hll->CAN2_Speed_Messages[can2_speed]);
	}else{
		CAN_SendMessage(hll->CAN_Prepare_Message); 
		for(int i = 0; i <  hll->CAN2_Sleep_Messages.size();i++){
			CAN_SendMessage(hll->CAN2_Sleep_Messages[i]);
		}
	}
}

void CAN_ConfigureSaint(int whichCAN, int can1enabled,int can1speed, int can1type, int can2enabled, int can2speed, int can2type){
	if(whichCAN == 1){
		can1_enabled = can1enabled;
		can1_speed = can1speed;
		can1_type = can1type;
		updateCAN1();
	}else if(whichCAN == 2){
		can2_enabled = can2enabled;
		can2_speed = can2speed;
		can2_type = can2type;
		updateCAN2();
	}
}


void CCANConfig::OnBnClickedOk()
{
	CAN_SendMessage("connect");
	int can1speed = can1SpeedBox->GetCurSel();
	int can1type = can1TypeBox->GetCurSel();

	if(checkCAN1.GetCheck() == BST_CHECKED ){
		can1EnabledBox = 1;
	}else{
		can1EnabledBox = 0;
	}

	CAN_ConfigureSaint(1,can1EnabledBox,can1speed,can1type,0,0,0);

	int can2speed = can2SpeedBox->GetCurSel();
	int can2type = can2TypeBox->GetCurSel();

	if(checkCAN2.GetCheck() == BST_CHECKED ){
		can2EnabledBox = 1;
	}else{
		can2EnabledBox = 0;
	}

	CAN_ConfigureSaint(2,0,0,0,can2EnabledBox,can2speed,can2type);

	CAN_SendMessage("disconnect");
	CDialog::OnOK();
}
