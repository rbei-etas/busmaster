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
#include "Panel_DME.h"
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
CMainPanel* MPnel;
mPSTXSELMSGDATA Message_DME;





byte CrcTable8Dme[256] =
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
byte crcDt;
byte crcDisp;
byte SPEED_MSG_DATA_DT[7];
byte SPEED_MSG_DATA_DISP[7];






/*unsigned int currentTime;
unsigned int initialTimeSpeed;
unsigned int initialTimeRel;
HANDLE timer;
BOOL flagEnvio;
*/

//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

// CUDSSettingsWnd dialog

IMPLEMENT_DYNAMIC(CPanelDme, CDialog)

CPanelDme::CPanelDme(CWnd* pParent /*=NULL*/,CMainPanel* pMP)
    : CDialog(CPanelDme::IDD, pParent)
{
    MPnel=pMP;
}

CPanelDme::~CPanelDme()
{

}

void CPanelDme::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX,IDC_ST_KL_15_MOTBK_2010 ,editStKl15Motbk2010 );
    DDX_Control(pDX,IDC_IJV_FU_MOTBK_2010 ,editIjvFuMotbk2010);
    DDX_Control(pDX,IDC_TEMP_ENG_MOTBK_2010 ,editTempEngMotbk2010);
    DDX_Control(pDX,IDC_ST_EMOFF_MOTBK_2010 ,editStEmoffMotbk2010);
    DDX_Control(pDX,IDC_ST_KL_50_MOTBK_2010 ,editStKl50Motbk2010);
    DDX_Control(pDX,IDC_ST_SDST_MOTBK_2010 ,editStSdstMotbk2010);
    DDX_Control(pDX,IDC_RPM_ENG_MOTBK_2010 ,editRpmEngMotbk2010);
    DDX_Control(pDX,IDC_BNK_ANG_MOTBK_2010 ,editBnkAngMotbk2010);
    DDX_Control(pDX,IDC_ALIV_DISP_DRV_MOTBK_2010 ,editAlivDispDrvMotbk2010);
    DDX_Control(pDX,IDC_CRC_DISP_DRV_MOTBK_2010 ,editCrcDispDrvMotbk2010);
    DDX_Control(pDX,IDC_DISP_RDI_DRVG_MOTBK_2010 ,editDispRdiDrvgMotbk2010);
    DDX_Control(pDX,IDC_ALIV_DT_DRV_MOTBK_2010 ,editAlivDtDrvMotbk2010);
    DDX_Control(pDX,IDC_CRC_DT_DRV_MOTBK_2010 ,editCrcDtDrvMotbk2010);
    DDX_Control(pDX,IDC_ST_BRG_RR_MOTBK_2010 ,editStBrgRrMotbk2010);
    DDX_Control(pDX,IDC_DISP_MOD_VEH_ACV_MOTBK_2010 ,editDispModVehAcvMotbk2010);
    DDX_Control(pDX,IDC_KL_SHOW ,editKlShow);

    DDX_Control(pDX,IDC_BUTTON_ST_EMOFF_MOTBK_2010 ,buttonStEmoffMotbk2010);
    DDX_Control(pDX,IDC_BUTTON_ST_KL_50_MOTBK_2010 ,buttonStKl50Motbk2010 );
    DDX_Control(pDX,IDC_BUTTON_ST_SDST_MOTBK_2010 ,buttonStSdstMotbk2010 );
    DDX_Control(pDX,IDC_KL_CHANGER ,buttonKlChanger );
    DDX_Control(pDX,IDC_CHECK_ALIV_DISP_DRV_MOTBK_2010 ,checkAlivDispDrvMotbk2010 );
    DDX_Control(pDX,IDC_CHECK_CRC_DISP_DRV_MOTBK_2010 ,checkCrcDispDrvMotbk2010 );
    DDX_Control(pDX,IDC_CHECK_ALIV_DT_DRV_MOTBK_2010 ,checkAlivDtDrvMotbk2010 );
    DDX_Control(pDX,IDC_CHECK_CRC_DT_DRV_MOTBK_2010 ,checkCrcDtDrvMotbk2010 );

    DDX_Control(pDX,IDC_SLIDER_RPM ,sliderRpm);

}


BEGIN_MESSAGE_MAP(CPanelDme, CDialog)
    ON_WM_HSCROLL()
    ON_BN_CLICKED(IDC_BUTTON_ST_EMOFF_MOTBK_2010, OnButtonStEmoffMotbk2010)
    ON_BN_CLICKED(IDC_BUTTON_ST_KL_50_MOTBK_2010, OnButtonStKl50Motbk2010)
    ON_BN_CLICKED(IDC_BUTTON_ST_SDST_MOTBK_2010, OnButtonStSdstMotbk2010)
    ON_BN_CLICKED(IDC_KL_CHANGER, OnButtonKlChanger)

    ON_BN_CLICKED(IDC_CHECK_ALIV_DISP_DRV_MOTBK_2010, OnCheckAlivDispDrvMotbk2010)
    ON_BN_CLICKED(IDC_CHECK_CRC_DISP_DRV_MOTBK_2010, OnCheckCrcDispDrvMotbk2010)
    ON_BN_CLICKED(IDC_CHECK_ALIV_DT_DRV_MOTBK_2010, OnCheckAlivDtDrvMotbk2010)
    ON_BN_CLICKED(IDC_CHECK_CRC_DT_DRV_MOTBK_2010, OnCheckCrcDtDrvMotbk2010)
END_MESSAGE_MAP()

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelDme
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelDme::vInitializePanelFields()
{
    editStKl15Motbk2010.vSetBase(BASE_DECIMAL);
    editStKl15Motbk2010.vSetSigned(false);
    editStKl15Motbk2010.vSetValue(0);
    valueStKl15Motbk2010 = 0;

    editIjvFuMotbk2010.vSetBase(BASE_DECIMAL);
    editIjvFuMotbk2010.vSetSigned(false);
    editIjvFuMotbk2010.vSetValue(0);
    valueIjvFuMotbk2010 = 0;

    editTempEngMotbk2010.vSetBase(BASE_DECIMAL);
    editTempEngMotbk2010.vSetSigned(true);
    editTempEngMotbk2010.vSetValue(-25.00);
    valueTempEngMotbk2010 = 0;

    editStEmoffMotbk2010.vSetBase(BASE_DECIMAL);
    editStEmoffMotbk2010.vSetSigned(false);
    editStEmoffMotbk2010.vSetValue(2);
    valueStEmoffMotbk2010 = 2;

    editStKl50Motbk2010.vSetBase(BASE_DECIMAL);
    editStKl50Motbk2010.vSetSigned(false);
    editStKl50Motbk2010.vSetValue(0);
    valueStKl50Motbk2010 = 0;

    editStSdstMotbk2010.vSetBase(BASE_DECIMAL);
    editStSdstMotbk2010.vSetSigned(false);
    editStSdstMotbk2010.vSetValue(1);
    valueStSdstMotbk2010 = 1;

    editRpmEngMotbk2010.vSetBase(BASE_DECIMAL);
    editRpmEngMotbk2010.vSetSigned(false);
    editRpmEngMotbk2010.vSetValue(0);
    valueRpmEngMotbk2010 = 0;

    editBnkAngMotbk2010.vSetBase(BASE_DECIMAL);
    editBnkAngMotbk2010.vSetSigned(false);
    editBnkAngMotbk2010.vSetValue(0);
    valueBnkAngMotbk2010 = 0;

    editKlShow.vSetBase(BASE_DECIMAL);
    editKlShow.vSetSigned(false);
    editKlShow.vSetValue(0);
    valueKlShow = 0;

    editAlivDispDrvMotbk2010.vSetBase(BASE_DECIMAL);
    editAlivDispDrvMotbk2010.vSetSigned(false);
    editAlivDispDrvMotbk2010.vSetValue(0);
    valueAlivDispDrvMotbk2010 = 0;

    editCrcDispDrvMotbk2010.vSetBase(BASE_DECIMAL);
    editCrcDispDrvMotbk2010.vSetSigned(false);
    editCrcDispDrvMotbk2010.vSetValue(0);
    valueCrcDispDrvMotbk2010 = 0;

    editDispRdiDrvgMotbk2010.vSetBase(BASE_DECIMAL);
    editDispRdiDrvgMotbk2010.vSetSigned(false);
    editDispRdiDrvgMotbk2010.vSetValue(0);
    valueDispRdiDrvgMotbk2010 = 0;

    editAlivDtDrvMotbk2010.vSetBase(BASE_DECIMAL);
    editAlivDtDrvMotbk2010.vSetSigned(false);
    editAlivDtDrvMotbk2010.vSetValue(0);
    valueAlivDtDrvMotbk2010 = 0;

    editCrcDtDrvMotbk2010.vSetBase(BASE_DECIMAL);
    editCrcDtDrvMotbk2010.vSetSigned(false);
    editCrcDtDrvMotbk2010.vSetValue(0);
    valueCrcDtDrvMotbk2010 = 0;

    editStBrgRrMotbk2010.vSetBase(BASE_DECIMAL);
    editStBrgRrMotbk2010.vSetSigned(false);
    editStBrgRrMotbk2010.vSetValue(1);
    valueStBrgRrMotbk2010 = 1;

    editDispModVehAcvMotbk2010.vSetBase(BASE_DECIMAL);
    editDispModVehAcvMotbk2010.vSetSigned(false);
    editDispModVehAcvMotbk2010.vSetValue(0);
    valueDispModVehAcvMotbk2010 = 0;

    buttonStEmoffMotbk2010.SetCheck(0);
    flagStEmoffMotbk2010 = false;

    buttonKlChanger.SetCheck(0);
    valueKlChanger = 0;

    buttonStKl50Motbk2010.SetCheck(0);
    flagStKl50Motbk2010 = false;

    buttonStSdstMotbk2010.SetCheck(0);
    flagStSdstMotbk2010 = false;

    checkAlivDispDrvMotbk2010.SetCheck(0);
    flagAlivDispDrvMotbk2010 = false;

    checkCrcDispDrvMotbk2010.SetCheck(0);
    flagCrcDispDrvMotbk2010 = false;

    checkAlivDtDrvMotbk2010.SetCheck(0);
    flagAlivDtDrvMotbk2010 = false;

    checkCrcDtDrvMotbk2010.SetCheck(0);
    flagCrcDtDrvMotbk2010 = false;

    sliderRpm.SetRangeMax(20470);
    sliderRpm.SetRangeMin(0);
    sliderRpm.SetPos(0);
    valueSliderRpm = 0;



    Message_DME = new mSTXSELMSGDATA;
    Message_DME->m_unCount=1;
    Message_DME->m_psTxMsg = new STCAN_MSG[1];
    Message_DME->m_psTxMsg->m_ucChannel = 1;
    Message_DME->m_psTxMsg->m_ucDataLen = 8;
    Message_DME->m_psTxMsg->m_ucEXTENDED = false;
    //  MessageNode->m_psTxMsg->m_bCANFD = 0x656;
    Message_DME->m_psTxMsg->m_unMsgID = 0x656;



    Message_DME->m_psTxMsg->m_ucData[0] = 1;
    Message_DME->m_psTxMsg->m_ucData[1] = 1;
    Message_DME->m_psTxMsg->m_ucData[2] = 1;
    Message_DME->m_psTxMsg->m_ucData[3] = 1;
    //UpdateData(FALSE);


}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelDme
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
afx_msg  void CPanelDme::OnCloseWnd()
{

}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelDme
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
BOOL CPanelDme::OnInitDialog()
{
    CDialog::OnInitDialog();
    vInitializePanelFields();
    return TRUE;
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelDme
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelDme::SetPanelValues()
{
    UpdateData(false);
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelDme
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelDme::EvaluateMessageReceived(STCAN_MSG  Mensaje )
{

}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelDme::OnButtonStEmoffMotbk2010()
{

    if(editStEmoffMotbk2010.lGetValue()==1)
    {
        editStEmoffMotbk2010.vSetValue(2);
    }
    else
    {
        editStEmoffMotbk2010.vSetValue(1);
    }
    valueStEmoffMotbk2010 = editStEmoffMotbk2010.fGetValue();
    UpdateData();
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelDme::OnButtonStKl50Motbk2010()
{

    if(editStKl50Motbk2010.lGetValue()==0)
    {
        editStKl50Motbk2010.vSetValue(1);
    }
    else
    {
        editStKl50Motbk2010.vSetValue(0);
    }
    valueStKl50Motbk2010 = editStKl50Motbk2010.fGetValue();
    UpdateData();
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelDme::OnButtonStSdstMotbk2010()
{

    if(editStSdstMotbk2010.lGetValue()==0)
    {
        editStSdstMotbk2010.vSetValue(1);
    }
    else
    {
        editStSdstMotbk2010.vSetValue(0);
    }
    valueStSdstMotbk2010 = editStSdstMotbk2010.fGetValue();
    UpdateData();
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelDme::OnButtonKlChanger()
{

    if(editKlShow.lGetValue()==0)
    {
        editKlShow.vSetValue(1);
    }
    else
    {
        editKlShow.vSetValue(0);
    }
    valueKlShow = editKlShow.fGetValue();
    UpdateData();
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelDme::OnCheckAlivDispDrvMotbk2010()
{

    if (flagAlivDispDrvMotbk2010 == true)
    {
        flagAlivDispDrvMotbk2010 = false;
    }
    else
    {
        flagAlivDispDrvMotbk2010 = true;
    }
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelDme::OnCheckCrcDispDrvMotbk2010()
{

    if (flagCrcDispDrvMotbk2010== true)
    {
        flagCrcDispDrvMotbk2010 = false;
    }
    else
    {
        flagCrcDispDrvMotbk2010 = true;
    }
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelDme::OnCheckAlivDtDrvMotbk2010()
{

    if (flagAlivDtDrvMotbk2010 == true)
    {
        flagAlivDtDrvMotbk2010 = false;
    }
    else
    {
        flagAlivDtDrvMotbk2010 = true;
    }
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelDme::OnCheckCrcDtDrvMotbk2010()
{

    if (flagCrcDtDrvMotbk2010== true)
    {
        flagCrcDtDrvMotbk2010 = false;
    }
    else
    {
        flagCrcDtDrvMotbk2010 = true;
    }
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
/*void CPanelDme::OnEnChangeRpmEng(){

    valueRpmEngMotbk2010 = editRpmEngMotbk2010.lGetValue();
    valueSliderRpm = valueRpmEngMotbk2010;
    sliderRpm.SetPos(valueRpmEngMotbk2010);
}*/

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanel
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelDme::PrepareData()
{
    UpdateData();

    valueStKl15Motbk2010 = editStKl15Motbk2010.lGetValue();
    valueIjvFuMotbk2010 = editIjvFuMotbk2010.lGetValue();
    valueTempEngMotbk2010 = (int) ((editTempEngMotbk2010.fGetValue()+25)/0.75);
    valueStEmoffMotbk2010 = editStEmoffMotbk2010.lGetValue();
    valueStKl50Motbk2010 = editStKl50Motbk2010.lGetValue();
    valueStSdstMotbk2010 = editStSdstMotbk2010.lGetValue();
    valueRpmEngMotbk2010 = editRpmEngMotbk2010.lGetValue()/5;
    valueBnkAngMotbk2010 = (int) ((editBnkAngMotbk2010.fGetValue()+90)/0.0439453);
    valueKlShow = editKlShow.lGetValue();
    valueAlivDispDrvMotbk2010 = editAlivDispDrvMotbk2010.lGetValue();
    valueCrcDispDrvMotbk2010 = editCrcDispDrvMotbk2010.lGetValue();
    valueDispRdiDrvgMotbk2010 = editDispRdiDrvgMotbk2010.lGetValue();
    valueAlivDtDrvMotbk2010 = editAlivDtDrvMotbk2010.lGetValue();
    valueCrcDtDrvMotbk2010 = editCrcDtDrvMotbk2010.lGetValue();
    valueStBrgRrMotbk2010 = editStBrgRrMotbk2010.lGetValue();
    valueDispModVehAcvMotbk2010 = editDispModVehAcvMotbk2010.lGetValue();
    sliderRpm.SetPos(valueRpmEngMotbk2010*5);
    valueSliderRpm = sliderRpm.GetPos();
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelDme
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelDme::OnTimerEngDat2()
{
    initialTimeEngDat2 = currentTime;

    UpdateAliveDt();
    PrepareData();

    Message_DME->m_psTxMsg->m_ucChannel = 1;
    Message_DME->m_psTxMsg->m_ucDataLen = 8;
    //MessageNode->m_psTxMsg->m_ucEXTENDED = false;
    Message_DME->m_psTxMsg->m_unMsgID = 268;
    Message_DME->m_psTxMsg->m_ucData[0] = 0xFF;
    Message_DME->m_psTxMsg->m_ucData[1] = valueStSdstMotbk2010 * 64 + valueStKl50Motbk2010 * 16 + valueAlivDispDrvMotbk2010;
    Message_DME->m_psTxMsg->m_ucData[2] = (valueRpmEngMotbk2010) & 0x00FF;
    Message_DME->m_psTxMsg->m_ucData[3] = (((valueRpmEngMotbk2010) & 0x0F00)>>8) + (valueBnkAngMotbk2010 & 0x000F) * 16;
    Message_DME->m_psTxMsg->m_ucData[4] = (valueBnkAngMotbk2010 & 0x0FF0)>>4;
    Message_DME->m_psTxMsg->m_ucData[5] = 1 /*De moment me l'invento, es AngThva*/;
    Message_DME->m_psTxMsg->m_ucData[6] = 0x0F;
    Message_DME->m_psTxMsg->m_ucData[7] = 0x0F;

    MPnel->SendPanelsMessage(Message_DME);
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelDme
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelDme::OnTimerEngDat1()
{
    initialTimeEngDat1 = currentTime;

    //Pan->UpdateAlive();
    PrepareData();
    Message_DME->m_psTxMsg->m_ucChannel = 1;
    Message_DME->m_psTxMsg->m_ucDataLen = 8;
    //MessageNode->m_psTxMsg->m_ucEXTENDED = false;
    Message_DME->m_psTxMsg->m_unMsgID = 700;
    Message_DME->m_psTxMsg->m_ucData[0] = 0x00;
    Message_DME->m_psTxMsg->m_ucData[1] = 1 * 64/*No lo se StPOil*/ + 1 * 16 /**/;
    Message_DME->m_psTxMsg->m_ucData[2] = 0xFF;
    Message_DME->m_psTxMsg->m_ucData[3] = 0xFF;
    Message_DME->m_psTxMsg->m_ucData[4] = 0xFF;
    Message_DME->m_psTxMsg->m_ucData[5] = 0xFF;
    Message_DME->m_psTxMsg->m_ucData[6] = 0xFF;
    Message_DME->m_psTxMsg->m_ucData[7] = 0xFF;

    MPnel->SendPanelsMessage(Message_DME);
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelDme
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelDme::OnTimerModVeh()
{
    initialTimeModVeh = currentTime;

    //Pan->UpdateAlive();
    PrepareData();
    Message_DME->m_psTxMsg->m_ucChannel = 1;
    Message_DME->m_psTxMsg->m_ucDataLen = 8;
    //MessageNode->m_psTxMsg->m_ucEXTENDED = false;
    Message_DME->m_psTxMsg->m_unMsgID = 692;
    Message_DME->m_psTxMsg->m_ucData[0] = 0xFF/*CrcModVeh*/;
    Message_DME->m_psTxMsg->m_ucData[1] = 1/*AlivModVeh*/;
    Message_DME->m_psTxMsg->m_ucData[2] =   /*AlivModVeh*/ 1*64 + /*AlivModVeh*/ 1*8 + valueDispModVehAcvMotbk2010;
    Message_DME->m_psTxMsg->m_ucData[3] = /*/*DispModVehMnu*/1*64 + /*CtrModVehChas*/1*8 + /*CtrModVehAbs*/1;
    Message_DME->m_psTxMsg->m_ucData[4] = /*DispModVehMnu*/1;
    Message_DME->m_psTxMsg->m_ucData[5] = /*CctrTarvl*/1;
    Message_DME->m_psTxMsg->m_ucData[6] = /*StRpkRck*/1*16 + /*CctrTarvl[Msb]*/1;
    Message_DME->m_psTxMsg->m_ucData[7] = /*StIdvlFoldyn*/1*8 + /*StIdvlTrct*/1;

    MPnel->SendPanelsMessage(Message_DME);
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelDme
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelDme::OnTimerClmpStat()
{
    initialTimeClmpStat = currentTime;

    //Pan->UpdateAlive();
    //Pan->PrepareData();
    Message_DME->m_psTxMsg->m_ucChannel = 1;
    Message_DME->m_psTxMsg->m_ucDataLen = 8;
    //MessageNode->m_psTxMsg->m_ucEXTENDED = false;
    Message_DME->m_psTxMsg->m_unMsgID = 304;
    Message_DME->m_psTxMsg->m_ucData[0] = /*StKeyVld*/1*64 + valueStKl15Motbk2010*4;
    Message_DME->m_psTxMsg->m_ucData[1] = 0x00;
    Message_DME->m_psTxMsg->m_ucData[2] = 0x00;
    Message_DME->m_psTxMsg->m_ucData[3] = 0x00;
    Message_DME->m_psTxMsg->m_ucData[4] = 0x00;
    Message_DME->m_psTxMsg->m_ucData[5] = 0x00;
    Message_DME->m_psTxMsg->m_ucData[6] = 0x00;
    Message_DME->m_psTxMsg->m_ucData[7] = 0x00;

    MPnel->SendPanelsMessage(Message_DME);
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelDme
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelDme::OnTimerDtDrv()
{
    initialTimeDtDrv = currentTime;

    UpdateAliveDt();
    PrepareData();

    Message_DME->m_psTxMsg->m_ucChannel = 1;
    Message_DME->m_psTxMsg->m_ucDataLen = 8;
    //MessageNode->m_psTxMsg->m_ucEXTENDED = false;
    Message_DME->m_psTxMsg->m_unMsgID =267;
    Message_DME->m_psTxMsg->m_ucData[0] = valueCrcDtDrvMotbk2010;
    Message_DME->m_psTxMsg->m_ucData[1] = /*StChgrdi*/1*64 + /*CtrDcsw*/1*16 + valueAlivDtDrvMotbk2010;
    Message_DME->m_psTxMsg->m_ucData[2] = /*ULinkAe*/1;
    Message_DME->m_psTxMsg->m_ucData[3] = /*IAe[Lsb]*/1*16 + valueStBrgRrMotbk2010;
    Message_DME->m_psTxMsg->m_ucData[4] = /*IAe*/1;
    Message_DME->m_psTxMsg->m_ucData[5] = 0x00;
    Message_DME->m_psTxMsg->m_ucData[6] = 0x00;
    Message_DME->m_psTxMsg->m_ucData[7] = 0x00;

    MPnel->SendPanelsMessage(Message_DME);
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelDme
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelDme::OnTimerDispDrv()
{
    initialTimeDispDrv = currentTime;

    UpdateAliveDisp();
    PrepareData();

    Message_DME->m_psTxMsg->m_ucChannel = 1;
    Message_DME->m_psTxMsg->m_ucDataLen = 8;
    //MessageNode->m_psTxMsg->m_ucEXTENDED = false;
    Message_DME->m_psTxMsg->m_unMsgID = 678;
    Message_DME->m_psTxMsg->m_ucData[0] = valueCrcDispDrvMotbk2010;
    Message_DME->m_psTxMsg->m_ucData[1] = /*DispRevgr*/1*64 + valueDispRdiDrvgMotbk2010*16 + valueAlivDispDrvMotbk2010;
    Message_DME->m_psTxMsg->m_ucData[2] = /*DispRecupDomDv*/1;
    Message_DME->m_psTxMsg->m_ucData[3] = /*DispRecupDomSys*/1;
    Message_DME->m_psTxMsg->m_ucData[4] = /*DispRecupAclnImme*/1;
    Message_DME->m_psTxMsg->m_ucData[5] = /*DispAclnDomSys*/1;
    Message_DME->m_psTxMsg->m_ucData[6] = /*DispAclnDomDv*/1;
    Message_DME->m_psTxMsg->m_ucData[7] = /*DispErrProteDrv*/1*16 + /*DispErrDrv*/1*4 + /*DispErrRecup*/1;

    MPnel->SendPanelsMessage(Message_DME);
}

//////////////////////////////////////////////////////////
void CPanelDme::InitTimers(unsigned int curTime )
{
    initialTimeEngDat2= curTime;
    initialTimeEngDat1 = curTime;
    initialTimeModVeh = curTime;
    initialTimeClmpStat = curTime;
    initialTimeDtDrv = curTime;
    initialTimeDispDrv = curTime;

}

//////////////////////////////////////////////////////////
void CPanelDme::TimersCheck(unsigned int curTime )
{
    currentTime = curTime;
    if((currentTime - initialTimeEngDat2) > 10 - 1)
    {
        OnTimerEngDat2();
        //b->MainP->SendPanelsMessage(b->MessageNode);
    }
    if((currentTime - initialTimeEngDat1) > 100 - 1 )
    {
        OnTimerEngDat1();
        //b->MainP->SendPanelsMessage(b->MessageNode);
    }
    if((currentTime - initialTimeModVeh) > 100 - 1)
    {
        OnTimerModVeh();
        //b->MainP->SendPanelsMessage(b->MessageNode);
    }
    if((currentTime - initialTimeClmpStat) > 100 - 1 )
    {
        OnTimerClmpStat();
        //b->MainP->SendPanelsMessage(b->MessageNode);
    }
    if((currentTime - initialTimeDtDrv) > 10 - 1)
    {
        OnTimerDtDrv();
        //b->MainP->SendPanelsMessage(b->MessageNode);
    }
    if((currentTime - initialTimeDispDrv) > 100 - 1 )
    {
        OnTimerDispDrv();
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
void CPanelDme::UpdateAliveDt()
{
    if(flagAlivDtDrvMotbk2010)
    {
        if(valueAlivDtDrvMotbk2010 >= 14)
        {
            valueAlivDtDrvMotbk2010 = 0;
        }
        else
        {
            valueAlivDtDrvMotbk2010++;
        }
        editAlivDtDrvMotbk2010.vSetValue(valueAlivDtDrvMotbk2010);
        UpdateData(false);
    }

    if(flagCrcDtDrvMotbk2010)
    {
        int ApplicationId = 0x14;
        byte count;

        SPEED_MSG_DATA_DT[0] = /*StChgrdi*/1*64 + /*CtrDcsw*/1*16 + valueAlivDtDrvMotbk2010;
        SPEED_MSG_DATA_DT[1] = /*ULinkAe*/1;
        SPEED_MSG_DATA_DT[2] = /*IAe[Lsb]*/1*16 + valueStBrgRrMotbk2010;
        SPEED_MSG_DATA_DT[3] = /*IAe*/1;
        SPEED_MSG_DATA_DT[4] = 0x00;
        SPEED_MSG_DATA_DT[5] = 0x00;
        SPEED_MSG_DATA_DT[6] = 0x00;


        crcDt = 0;
        crcDt = CrcTable8Dme[crcDt ^ (byte)(ApplicationId & 0x00ff)];
        crcDt = CrcTable8Dme[crcDt ^ (byte)(ApplicationId >> 8)];
        for (count = 0; count < 7; count++)
        {
            crcDt = CrcTable8Dme[crcDt ^ SPEED_MSG_DATA_DT[count]];
        }
        valueCrcDtDrvMotbk2010= crcDt;
        editCrcDtDrvMotbk2010.vSetValue(valueCrcDtDrvMotbk2010);
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
void CPanelDme::UpdateAliveDisp()
{
    if(flagAlivDispDrvMotbk2010)
    {
        if(valueAlivDispDrvMotbk2010 >= 14)
        {
            valueAlivDispDrvMotbk2010 = 0;
        }
        else
        {
            valueAlivDispDrvMotbk2010++;
        }
        editAlivDispDrvMotbk2010.vSetValue(valueAlivDispDrvMotbk2010);
        UpdateData(false);
    }

    if(flagCrcDispDrvMotbk2010)
    {
        int ApplicationId = 0x14;
        byte count;

        SPEED_MSG_DATA_DISP[0] = /*DispRevgr*/1*64 + valueDispRdiDrvgMotbk2010*16 + valueAlivDispDrvMotbk2010;
        SPEED_MSG_DATA_DISP[1] = /*DispRecupDomDv*/1;
        SPEED_MSG_DATA_DISP[2] = /*DispRecupDomSys*/1;
        SPEED_MSG_DATA_DISP[3] = /*DispRecupAclnImme*/1;
        SPEED_MSG_DATA_DISP[4] = /*DispAclnDomSys*/1;
        SPEED_MSG_DATA_DISP[5] = /*DispAclnDomDv*/1;
        SPEED_MSG_DATA_DISP[6] = /*DispErrProteDrv*/1*16 + /*DispErrDrv*/1*4 + /*DispErrRecup*/1;

        crcDisp = 0;
        crcDisp = CrcTable8Dme[crcDisp ^ (byte)(ApplicationId & 0x00ff)];
        crcDisp = CrcTable8Dme[crcDisp ^ (byte)(ApplicationId >> 8)];
        for (count = 0; count < 7; count++)
        {
            crcDisp = CrcTable8Dme[crcDisp ^ SPEED_MSG_DATA_DISP[count]];
        }
        valueCrcDispDrvMotbk2010= crcDisp;
        editCrcDispDrvMotbk2010.vSetValue(valueCrcDispDrvMotbk2010);
        UpdateData(false);
    }

}

/////////////////////////////////////////////////////////
/*void CPanelDme::OnNMCustomdrawSliderRpm(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    @TN
        *pResult = 0;
    // TODO: Agregue aquí su código de controlador de notificación de control
    *pResult = 0;
}*/

////////////////////////////////////////////////////////
void CPanelDme::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    CSliderCtrl* pSlider = reinterpret_cast<CSliderCtrl*>(pScrollBar);

    // Check which slider sent the notification
    if (pSlider == &sliderRpm)
    {

        valueRpmEngMotbk2010 = pSlider->GetPos()/5;
        valueSliderRpm = valueRpmEngMotbk2010*5;
        editRpmEngMotbk2010.vSetValue(valueRpmEngMotbk2010*5);
        UpdateData();
    }

}