// CUDSSettingsWnd.cpp : implementation file
/**
 * \file      UDSSettingsWnd.cpp
 * \brief     Definition file for CUDSSettingsWnd class
 * \author    Sanchez Marin Maria Alejandra
 * Designer:    Bentea Radu Mihai
 *  Manager of the UDS_Protocol Porject
 */

//#include "stdafx.h"
//#include "MainPanel.h"
//#include "Node.h"
#include "MainPanel.h"
#include "PanelAbs.h"
//#include "UDSMainWnd.h"
//#include "UDS_Protocol.h"
//#include "DataTypes/UDS_DataTypes.h"
#include "UDSWnd_Defines.h"
#include    "stdafx.h"
#include    <Windows.h>
#include    <iostream>
#include    <process.h>
#include    <stdlib.h>

//CUDS_Protocol* UdsProtocolPtrPanel = NULL;
//extern CUDSMainWnd* omMainWnd;
CMainPanel* MPanelAbs;
mPSTXSELMSGDATA MessageAbs;


byte CrcTable8Abs[256] =
{
    0x00, 0x1d, 0x3a, 0x27, 0x74, 0x69, 0x4e, 0x53, 0xe8, 0xf5, 0xd2, 0xcf, 0x9c, 0x81, 0xa6, 0xbb,
    0xcd, 0xd0, 0xf7, 0xea, 0xb9, 0xa4, 0x83, 0x9e, 0x25, 0x38, 0x1f, 0x02, 0x51, 0x4c, 0x6b, 0x76,
    0x87, 0x9a, 0xbd, 0xa0, 0xf3, 0xee, 0xc9, 0xd4, 0x6f, 0x72, 0x55, 0x48, 0x1b, 0x06, 0x21, 0x3c,
    0x4a, 0x57, 0x70, 0x6d, 0x3e, 0x23, 0x04, 0x19, 0xa2, 0xbf, 0x98, 0x85, 0xd6, 0xcb, 0xec, 0xf1,
    0x13, 0x0e, 0x29, 0x34, 0x67, 0x7a, 0x5d, 0x40, 0xfb, 0xe6, 0xc1, 0xdc, 0x8f, 0x92, 0xb5, 0xa8,
    0xde, 0xc3, 0xe4, 0xf9, 0xaa, 0xb7, 0x90, 0x8d, 0x36, 0x2b, 0x0c, 0x11, 0x42, 0x5f, 0x78, 0x65,
    0x94, 0x89, 0xae, 0xb3, 0xe0, 0xfd, 0xda, 0xc7, 0x7c, 0x61, 0x46, 0x5b, 0x08, 0x15, 0x32, 0x2f,
    0x59, 0x44, 0x63, 0x7e, 0x2d, 0x30, 0x17, 0x0a, 0xb1, 0xac, 0x8b, 0x96, 0xc5, 0xd8, 0xff, 0xe2,
    0x26, 0x3b, 0x1c, 0x01, 0x52, 0x4f, 0x68, 0x75, 0xce, 0xd3, 0xf4, 0xe9, 0xba, 0xa7, 0x80, 0x9d,
    0xeb, 0xf6, 0xd1, 0xcc, 0x9f, 0x82, 0xa5, 0xb8, 0x03, 0x1e, 0x39, 0x24, 0x77, 0x6a, 0x4d, 0x50,
    0xa1, 0xbc, 0x9b, 0x86, 0xd5, 0xc8, 0xef, 0xf2, 0x49, 0x54, 0x73, 0x6e, 0x3d, 0x20, 0x07, 0x1a,
    0x6c, 0x71, 0x56, 0x4b, 0x18, 0x05, 0x22, 0x3f, 0x84, 0x99, 0xbe, 0xa3, 0xf0, 0xed, 0xca, 0xd7,
    0x35, 0x28, 0x0f, 0x12, 0x41, 0x5c, 0x7b, 0x66, 0xdd, 0xc0, 0xe7, 0xfa, 0xa9, 0xb4, 0x93, 0x8e,
    0xf8, 0xe5, 0xc2, 0xdf, 0x8c, 0x91, 0xb6, 0xab, 0x10, 0x0d, 0x2a, 0x37, 0x64, 0x79, 0x5e, 0x43,
    0xb2, 0xaf, 0x88, 0x95, 0xc6, 0xdb, 0xfc, 0xe1, 0x5a, 0x47, 0x60, 0x7d, 0x2e, 0x33, 0x14, 0x09,
    0x7f, 0x62, 0x45, 0x58, 0x0b, 0x16, 0x31, 0x2c, 0x97, 0x8a, 0xad, 0xb0, 0xe3, 0xfe, 0xd9, 0xc4
};
byte crcAbs;
byte SPEED_MSG_DATA_ABS[7];



/*unsigned int currentTime;
unsigned int initialTimeSpeed;
unsigned int initialTimeRel;
HANDLE timer;
BOOL flagEnvio;
*/

//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

// CUDSSettingsWnd dialog

IMPLEMENT_DYNAMIC(CPanelAbs, CDialog)

CPanelAbs::CPanelAbs(CWnd* pParent /*=NULL*/,CMainPanel* pMP)
    : CDialog(CPanelAbs::IDD, pParent)
{
    MPanelAbs=pMP;
}

CPanelAbs::~CPanelAbs()
{

}

void CPanelAbs::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_MILE_COU_FT_MOTBK_2010, editMileCouFtMotbk2010);
    DDX_Control(pDX, IDC_EDIT_MILE_COU_RR_MOTBK_2010, editMileCouRrMotbk2010);
    DDX_Control(pDX, IDC_EDIT_ALIV_V_MOTBK_2010, editAlivVMotbk2010);
    DDX_Control(pDX, IDC_EDIT_CRC_V_MOTBK_2010, editCrcVMotbk2010);
    DDX_Control(pDX, IDC_EDIT_MOD_VEH_ABS_ACV_MOTBK_2010, editModVehAbsAcvMotbk2010);
    DDX_Control(pDX, IDC_EDIT_MOD_VEH_ABS_CHOV_MOTBK_2010, editModVehAbsChovMotbk2010);
    DDX_Control(pDX, IDC_EDIT_ST_ILK_TLT_MOTBK_2010, editStIlkTltMotbk2010);
    DDX_Control(pDX, IDC_EDIT_ST_MOD_TLT_MOTBK_2010, editStModTltMotbk2010);
    DDX_Control(pDX, IDC_EDIT_ST_SEN_V_FT_MOTBK_2010, editStSenVFtMotbk2010);
    DDX_Control(pDX, IDC_EDIT_ST_SEN_V_RR_MOTBK_2010, editStSenVRrMotbk2010);
    DDX_Control(pDX, IDC_EDIT_ST_STASS_MOTBK_2010, editStStassMotbk2010);
    DDX_Control(pDX, IDC_EDIT_V_WHL_FT_MOTBK_2010, editVWhlFtMotbk2010);
    DDX_Control(pDX, IDC_EDIT_V_WHL_RR_MOTBK_2010, editVWhlRrMotbk2010);
    DDX_Control(pDX, IDC_EDIT_ST_ABS_MOTBK_2010, editStAbsMotbk2010);
    DDX_Control(pDX, IDC_EDIT_ST_BRK_FT_MOTBK_2010, editStBrkFtMotbk2010);
    DDX_Control(pDX, IDC_EDIT_ST_BRK_RR_MOTBK_2010, editStBrkRrMotbk2010);
    DDX_Control(pDX, IDC_CHECK_ALIV_V_MOTBK_2010, checkAlivVMotbk2010);
    DDX_Control(pDX, IDC_CHECK_CRC_V_MOTBK_2010, checkCrcVMotbk2010);
    DDX_Control(pDX, IDC_CHECK_ST_BRK_FT_MOTBK_2010, checkStBrkFtMotbk2010);
    DDX_Control(pDX, IDC_CHECK_ST_BRK_RR_MOTBK_2010, checkStBrkRrMotbk2010);

}


BEGIN_MESSAGE_MAP(CPanelAbs, CDialog)
    ON_BN_CLICKED(IDC_CHECK_ALIV_V_MOTBK_2010, OnCheckAlivVMotbk2010)
    ON_BN_CLICKED(IDC_CHECK_CRC_V_MOTBK_2010, OnCheckCrcVMotbk2010)
    ON_BN_CLICKED(IDC_CHECK_ST_BRK_FT_MOTBK_2010, OnCheckStBrkFtMotbk2010)
    ON_BN_CLICKED(IDC_CHECK_ST_BRK_RR_MOTBK_2010, OnCheckStBrkRrMotbk2010)

END_MESSAGE_MAP()

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAbs
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAbs::vInitializePanelFields()
{

    checkAlivVMotbk2010.SetCheck(BST_UNCHECKED);
    checkCrcVMotbk2010.SetCheck(BST_UNCHECKED);
    checkStBrkFtMotbk2010.SetCheck(BST_UNCHECKED);
    checkStBrkRrMotbk2010.SetCheck(BST_UNCHECKED);

    //float?
    editMileCouFtMotbk2010.vSetBase(BASE_DECIMAL);
    editMileCouFtMotbk2010.vSetSigned(false);
    editMileCouFtMotbk2010.vSetValue(0);

    //float?
    editMileCouRrMotbk2010.vSetBase(BASE_DECIMAL);
    editMileCouRrMotbk2010.vSetSigned(false);
    editMileCouRrMotbk2010.vSetValue(0);

    editCrcVMotbk2010.vSetBase(BASE_DECIMAL);
    editCrcVMotbk2010.vSetSigned(false);
    editCrcVMotbk2010.vSetValue(0);

    editAlivVMotbk2010.vSetBase(BASE_DECIMAL);
    editAlivVMotbk2010.vSetSigned(false);
    editAlivVMotbk2010.SetLimitText(2);
    editAlivVMotbk2010.vSetValue(0);

    editModVehAbsAcvMotbk2010.vSetBase(BASE_DECIMAL);
    editModVehAbsAcvMotbk2010.vSetSigned(false);
    editModVehAbsAcvMotbk2010.SetLimitText(1);
    editModVehAbsAcvMotbk2010.vSetValue(0);

    editModVehAbsChovMotbk2010.vSetBase(BASE_DECIMAL);
    editModVehAbsChovMotbk2010.vSetSigned(false);
    editModVehAbsChovMotbk2010.SetLimitText(1);
    editModVehAbsChovMotbk2010.vSetValue(0);

    editStIlkTltMotbk2010.vSetBase(BASE_DECIMAL);
    editStIlkTltMotbk2010.vSetSigned(false);
    editStIlkTltMotbk2010.SetLimitText(1);
    editStIlkTltMotbk2010.vSetValue(0);

    editStModTltMotbk2010.vSetBase(BASE_DECIMAL);
    editStModTltMotbk2010.vSetSigned(false);
    editStModTltMotbk2010.SetLimitText(1);
    editStModTltMotbk2010.vSetValue(0);

    editStSenVFtMotbk2010.vSetBase(BASE_DECIMAL);
    editStSenVFtMotbk2010.vSetSigned(false);
    editStSenVFtMotbk2010.SetLimitText(1);
    editStSenVFtMotbk2010.vSetValue(0);

    editStSenVRrMotbk2010.vSetBase(BASE_DECIMAL);
    editStSenVRrMotbk2010.vSetSigned(false);
    editStSenVRrMotbk2010.SetLimitText(1);
    editStSenVRrMotbk2010.vSetValue(0);

    editStStassMotbk2010.vSetBase(BASE_DECIMAL);
    editStStassMotbk2010.vSetSigned(false);
    editStStassMotbk2010.SetLimitText(1);
    editStStassMotbk2010.vSetValue(0);

    //float?
    editVWhlFtMotbk2010.vSetBase(BASE_DECIMAL);
    editVWhlFtMotbk2010.vSetSigned(false);
    editVWhlFtMotbk2010.vSetValue(0);

    //float?
    editVWhlRrMotbk2010.vSetBase(BASE_DECIMAL);
    editVWhlRrMotbk2010.vSetSigned(false);
    editVWhlRrMotbk2010.vSetValue(0);

    editStAbsMotbk2010.vSetBase(BASE_DECIMAL);
    editStAbsMotbk2010.vSetSigned(false);
    editStAbsMotbk2010.SetLimitText(1);
    editStAbsMotbk2010.vSetValue(0);

    editStBrkFtMotbk2010.vSetBase(BASE_DECIMAL);
    editStBrkFtMotbk2010.vSetSigned(false);
    editStBrkFtMotbk2010.SetLimitText(1);
    editStBrkFtMotbk2010.vSetValue(1);

    editStBrkRrMotbk2010.vSetBase(BASE_DECIMAL);
    editStBrkRrMotbk2010.vSetSigned(false);
    editStBrkRrMotbk2010.SetLimitText(1);
    editStBrkRrMotbk2010.vSetValue(1);

    flagAlivVMotbk2010 = false;
    flagCrcVMotbk2010 = false;
    flagStBrkFtMotbk2010 = false;
    flagStBrkRrMotbk2010 = false;

    valueMileCouFtMotbk2010 = 0;
    valueMileCouRrMotbk2010 = 0;
    valueCrcVMotbk2010 = 0;
    valueAlivVMotbk2010 = 0;
    valueModVehAbsAcvMotbk2010 = 0;
    valueModVehAbsChovMotbk2010 = 0;
    valueStIlkTltMotbk2010 = 0;
    valueStModTltMotbk2010 = 0;
    valueStSenVFtMotbk2010 = 0;
    valueStSenVRrMotbk2010 = 0;
    valueStStassMotbk2010 = 0;
    valueVWhlFtMotbk2010 = 0;
    valueVWhlRrMotbk2010 = 0;
    valueStAbsMotbk2010 = 0;
    valueStBrkFtMotbk2010 = 1;
    valueStBrkRrMotbk2010 = 1;

    MessageAbs = new mSTXSELMSGDATA;
    MessageAbs->m_unCount=1;
    MessageAbs->m_psTxMsg = new STCAN_MSG[1];
    MessageAbs->m_psTxMsg->m_ucChannel = 1;
    MessageAbs->m_psTxMsg->m_ucDataLen = 8;
    MessageAbs->m_psTxMsg->m_ucEXTENDED = false;
    //  MessageNode->m_psTxMsg->m_bCANFD = 0x656;
    MessageAbs->m_psTxMsg->m_unMsgID = 0x656;



    MessageAbs->m_psTxMsg->m_ucData[0] = 1;
    MessageAbs->m_psTxMsg->m_ucData[1] = 1;
    MessageAbs->m_psTxMsg->m_ucData[2] = 1;
    MessageAbs->m_psTxMsg->m_ucData[3] = 1;
    //UpdateData(FALSE);
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanel
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAbs::OnCheckAlivVMotbk2010()
{
    if (flagAlivVMotbk2010 == true)
    {
        flagAlivVMotbk2010 = false;
    }
    else
    {
        flagAlivVMotbk2010 = true;
    }
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanel
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAbs::OnCheckCrcVMotbk2010()
{
    if (flagCrcVMotbk2010 == true)
    {
        flagCrcVMotbk2010 = false;
    }
    else
    {
        flagCrcVMotbk2010 = true;
    }
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanel
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAbs::OnCheckStBrkFtMotbk2010()
{
    if (flagStBrkFtMotbk2010 == true)
    {
        flagStBrkFtMotbk2010 = false;
        valueStBrkFtMotbk2010 = 1;
        editStBrkFtMotbk2010.vSetValue(valueStBrkFtMotbk2010);
        UpdateData(false);
    }
    else
    {
        flagStBrkFtMotbk2010 = true;
        valueStBrkFtMotbk2010 = 2;
        editStBrkFtMotbk2010.vSetValue(valueStBrkFtMotbk2010);
        UpdateData(false);
    }
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanel
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAbs::OnCheckStBrkRrMotbk2010()
{
    if (flagStBrkRrMotbk2010 == true)
    {
        flagStBrkRrMotbk2010 = false;
        valueStBrkRrMotbk2010 = 1;
        editStBrkRrMotbk2010.vSetValue(valueStBrkRrMotbk2010);
        UpdateData(false);
    }
    else
    {
        flagStBrkRrMotbk2010 = true;
        valueStBrkRrMotbk2010 = 2;
        editStBrkRrMotbk2010.vSetValue(valueStBrkRrMotbk2010);
        UpdateData(false);
    }
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAbs
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
afx_msg  void CPanelAbs::OnCloseWnd()
{

}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAbs
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
BOOL CPanelAbs::OnInitDialog()
{
    CDialog::OnInitDialog();
    vInitializePanelFields();
    return TRUE;
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAbs
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAbs::SetPanelValues()
{
    editCrcVMotbk2010.vSetValue(valueCrcVMotbk2010);
    editAlivVMotbk2010.vSetValue(valueAlivVMotbk2010);
    editVWhlRrMotbk2010.vSetValue(valueVWhlRrMotbk2010);
    editVWhlFtMotbk2010.vSetValue(valueVWhlFtMotbk2010);
    editStBrkRrMotbk2010.vSetValue(valueStBrkRrMotbk2010);
    editStBrkFtMotbk2010.vSetValue(valueStBrkFtMotbk2010);
    editStSenVFtMotbk2010.vSetValue(valueStSenVFtMotbk2010);
    editStSenVRrMotbk2010.vSetValue(valueStSenVRrMotbk2010);
    editStAbsMotbk2010.vSetValue(valueStAbsMotbk2010);
    editModVehAbsAcvMotbk2010.vSetValue(valueModVehAbsAcvMotbk2010);
    editModVehAbsChovMotbk2010.vSetValue(valueModVehAbsChovMotbk2010);
    editStStassMotbk2010.vSetValue(valueStStassMotbk2010);
    editStIlkTltMotbk2010.vSetValue(valueStIlkTltMotbk2010);
    editStModTltMotbk2010.vSetValue(valueStModTltMotbk2010);
    editMileCouFtMotbk2010.vSetValue(valueMileCouFtMotbk2010);
    editMileCouRrMotbk2010.vSetValue(valueMileCouRrMotbk2010);
    UpdateData(false);
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanel
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAbs::PrepareData()
{
    UpdateData();
    valueMileCouFtMotbk2010 = (int)(editMileCouFtMotbk2010.fGetValue()/0.25);
    valueMileCouRrMotbk2010 = (int)(editMileCouRrMotbk2010.fGetValue()/0.25);
    valueCrcVMotbk2010 = editCrcVMotbk2010.lGetValue();
    valueAlivVMotbk2010 = editAlivVMotbk2010.lGetValue();
    valueModVehAbsAcvMotbk2010 = editModVehAbsAcvMotbk2010.lGetValue();
    valueModVehAbsChovMotbk2010 = editModVehAbsChovMotbk2010.lGetValue();
    valueStIlkTltMotbk2010 = editStIlkTltMotbk2010.lGetValue();
    valueStModTltMotbk2010 = editStModTltMotbk2010.lGetValue();
    valueStSenVFtMotbk2010 = editStSenVFtMotbk2010.lGetValue();
    valueStSenVRrMotbk2010 = editStSenVRrMotbk2010.lGetValue();
    valueStStassMotbk2010 = editStStassMotbk2010.lGetValue();
    valueVWhlFtMotbk2010 = (int) (editVWhlFtMotbk2010.fGetValue()/0.125);
    valueVWhlRrMotbk2010 = (int) (editVWhlRrMotbk2010.fGetValue()/0.125);
    valueStAbsMotbk2010 = editStAbsMotbk2010.lGetValue();
    valueStBrkFtMotbk2010 = editStBrkFtMotbk2010.lGetValue();
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAbs
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAbs::EvaluateMessageReceived(STCAN_MSG  Mensaje )
{

}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAbs
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAbs::OnTimerRel()
{

    UpdateAlive();
    PrepareData();

    initialTimeRel = currentTime;


    MessageAbs->m_psTxMsg->m_ucChannel = 1;
    MessageAbs->m_psTxMsg->m_ucDataLen = 8;
    //MessageNode->m_psTxMsg->m_ucEXTENDED = false;
    MessageAbs->m_psTxMsg->m_unMsgID = 656;
    MessageAbs->m_psTxMsg->m_ucData[0] = valueCrcVMotbk2010;
    MessageAbs->m_psTxMsg->m_ucData[1] = (0x0F & valueAlivVMotbk2010) + (0x00F & valueVWhlRrMotbk2010) * 16;
    MessageAbs->m_psTxMsg->m_ucData[2] = (0xFF0 & valueVWhlRrMotbk2010) / 16;
    MessageAbs->m_psTxMsg->m_ucData[3] = 0x0FF & valueVWhlFtMotbk2010;
    MessageAbs->m_psTxMsg->m_ucData[4] = valueStBrkFtMotbk2010 * 64 + valueStBrkRrMotbk2010 * 16 + (0xF00 & valueVWhlFtMotbk2010) / 256;
    MessageAbs->m_psTxMsg->m_ucData[5] = valueStAbsMotbk2010 * 16 + valueStSenVFtMotbk2010 * 4 + valueStSenVRrMotbk2010;
    MessageAbs->m_psTxMsg->m_ucData[6] = valueStStassMotbk2010 * 32 + valueModVehAbsChovMotbk2010 * 8 + valueModVehAbsAcvMotbk2010;
    MessageAbs->m_psTxMsg->m_ucData[7] = valueStModTltMotbk2010 * 4 + valueStIlkTltMotbk2010;

    MPanelAbs->SendPanelsMessage(MessageAbs);
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAbs
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAbs::OnTimerSpeed()
{

    initialTimeSpeed = currentTime;
    PrepareData();


    MessageAbs->m_psTxMsg->m_ucChannel = 1;
    MessageAbs->m_psTxMsg->m_ucDataLen = 8;
    //MessageNode->m_psTxMsg->m_ucEXTENDED = false;
    MessageAbs->m_psTxMsg->m_unMsgID = 688;
    MessageAbs->m_psTxMsg->m_ucData[0] = 0xFF;
    MessageAbs->m_psTxMsg->m_ucData[1] = 0x0F + (0x00F & valueMileCouRrMotbk2010) * 16;
    MessageAbs->m_psTxMsg->m_ucData[2] = (0xFF0 & valueMileCouRrMotbk2010) / 16;
    MessageAbs->m_psTxMsg->m_ucData[3] = 0x0FF & valueMileCouFtMotbk2010;
    MessageAbs->m_psTxMsg->m_ucData[4] = 0xF0 + valueMileCouFtMotbk2010 / 256;
    MessageAbs->m_psTxMsg->m_ucData[5] = 0xFF;
    MessageAbs->m_psTxMsg->m_ucData[6] = 0xFF;
    MessageAbs->m_psTxMsg->m_ucData[7] = 0xFF;

    MPanelAbs->SendPanelsMessage(MessageAbs);
}

//////////////////////////////////////////////////////////
void CPanelAbs::InitTimers(unsigned int curTime )
{
    initialTimeSpeed = curTime;
    initialTimeRel = curTime;

}

//////////////////////////////////////////////////////////
void CPanelAbs::TimersCheck(unsigned int curTime )
{
    currentTime = curTime;
    if((currentTime - initialTimeSpeed) > 100 - 1)
    {
        OnTimerSpeed();
        //b->MainP->SendPanelsMessage(b->MessageNode);
    }
    if((currentTime - initialTimeRel) > 10 - 1 )
    {
        OnTimerRel();
        //b->MainP->SendPanelsMessage(b->MessageNode);
    }
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanel
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAbs::UpdateAlive()
{
    if(flagAlivVMotbk2010)
    {
        if(valueAlivVMotbk2010 >= 14)
        {
            valueAlivVMotbk2010 = 0;
        }
        else
        {
            valueAlivVMotbk2010++;
        }
        editAlivVMotbk2010.vSetValue(valueAlivVMotbk2010);
        UpdateData(false);
    }

    if(flagCrcVMotbk2010)
    {
        int ApplicationId = 0x14;
        byte count;

        SPEED_MSG_DATA_ABS[0] = (0x0F & valueAlivVMotbk2010) + (0x00F & valueVWhlRrMotbk2010) * 16;
        SPEED_MSG_DATA_ABS[1] = (0xFF0 & valueVWhlRrMotbk2010) / 16;
        SPEED_MSG_DATA_ABS[2] = 0x0FF & valueVWhlFtMotbk2010;
        SPEED_MSG_DATA_ABS[3] = valueStBrkFtMotbk2010 * 64 + valueStBrkRrMotbk2010 * 16 + (0xF00 & valueVWhlFtMotbk2010) / 256;
        SPEED_MSG_DATA_ABS[4] = valueStAbsMotbk2010 * 16 + valueStSenVFtMotbk2010 * 4 + valueStSenVRrMotbk2010;
        SPEED_MSG_DATA_ABS[5] = valueStStassMotbk2010 * 32 + valueModVehAbsChovMotbk2010 * 8 + valueModVehAbsAcvMotbk2010;
        SPEED_MSG_DATA_ABS[6] = valueStModTltMotbk2010 * 4 + valueStIlkTltMotbk2010;

        crcAbs = 0;
        crcAbs = CrcTable8Abs[crcAbs ^ (byte)(ApplicationId & 0x00ff)];
        crcAbs = CrcTable8Abs[crcAbs ^ (byte)(ApplicationId >> 8)];
        for (count = 0; count < 7; count++)
        {
            crcAbs = CrcTable8Abs[crcAbs ^ SPEED_MSG_DATA_ABS[count]];
        }
        valueCrcVMotbk2010 = crcAbs;
        editCrcVMotbk2010.vSetValue(valueCrcVMotbk2010);
        UpdateData(false);
    }

}



