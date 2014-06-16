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
#include "PanelAndNode.h"
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
CMainPanel* MPanel;
mPSTXSELMSGDATA Message;

/*unsigned int currentTime;
unsigned int initialTimeSpeed;
unsigned int initialTimeRel;
HANDLE timer;
BOOL flagEnvio;
*/

//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

// CUDSSettingsWnd dialog

IMPLEMENT_DYNAMIC(CPanelAndNode, CDialog)

CPanelAndNode::CPanelAndNode(CWnd* pParent /*=NULL*/,CMainPanel* pMP)
    : CDialog(CPanelAndNode::IDD, pParent)
{
    MPanel=pMP;
}

CPanelAndNode::~CPanelAndNode()
{

}

void CPanelAndNode::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX,IDC_INDICATORS_PERIOD ,editIndicatorsPeriod );
    DDX_Control(pDX,IDC_IN_HORN_MOTBK_2010 ,editInHornMotbk2010);
    DDX_Control(pDX,IDC_IN_IDC_LH_MOTBK_2010 ,editInIdcLhMotbk2010);
    DDX_Control(pDX,IDC_IN_IDC_RH_MOTBK_2010 ,editInIdcRhMotbk2010);
    DDX_Control(pDX,IDC_IN_IDC_RSTG_MOTBK_2010 ,editInIdcRstgMotbk2010);
    DDX_Control(pDX,IDC_IN_MAB_MOTBK_2010 ,editInMabMotbk2010);
    DDX_Control(pDX,IDC_IN_PUBU_LH_MOTBK_2010 ,editInPubuLhMotbk2010);
    DDX_Control(pDX,IDC_IN_PUBU_RH_MOTBK_2010 ,editInPubuRhMotbk2010);
    DDX_Control(pDX,IDC_IN_ROSW_LH_DWN_MOTBK_2010 ,editInRoswLhDwnMotbk2010);
    DDX_Control(pDX,IDC_IN_ROSW_LH_UP_MOTBK_2010 ,editInRoswLhUpMotbk2010);
    DDX_Control(pDX,IDC_IN_ROSW_RH_DWN_MOTBK_2010 ,editInRoswRhDwnMotbk2010);
    DDX_Control(pDX,IDC_IN_ROSW_RH_UP_MOTBK_2010 ,editInRoswRhUpMotbk2010);
    DDX_Control(pDX,IDC_CTR_CHOV_ROSS_MOTBK_2010 ,editCtrChovRossMotbk2010);
    DDX_Control(pDX,IDC_CTR_HGHT_MOTBK_2010 ,editCtrHghtMotbk2010);
    DDX_Control(pDX,IDC_SU_DRL_AUTOMLY_MOTBK_2010 ,editSuDrlAutomlyMotbk2010);
    DDX_Control(pDX,IDC_CTR_SHTR_FS_MOTBK_2010 ,editCtrShtrFsMotbk2010);
    DDX_Control(pDX,IDC_BRIG_MOTBK_2010 ,editBrigMotbk2010);
    DDX_Control(pDX,IDC_MILE_TOT_MOTBK_2010 ,editMileTotMotbk2010);
    DDX_Control(pDX,IDC_T_SEC_COU_REL_MOTBK_2010 ,editTSecCouRelMotbk2010);

    DDX_Control(pDX,IDC_BUTTON_WINDSHIELD_UP ,buttonWindshieldUp );
    DDX_Control(pDX,IDC_BUTTON_WINDSHIELD_DOWN ,buttonWindshield );
    DDX_Control(pDX,IDC_BUTTON_INDICATORS ,buttonIndicators );
    DDX_Control(pDX,IDC_BUTTON_IN_HORN_MOTBK_2010 ,buttonInHornMotbk2010 );
    DDX_Control(pDX,IDC_BUTTON_IN_IDC_LH_MOTBK_2010 ,buttonInIdcLhMotbk2010 );
    DDX_Control(pDX,IDC_BUTTON_IN_IDC_RH_MOTBK_2010 ,buttonInIdcRhMotbk2010 );
    DDX_Control(pDX,IDC_BUTTON_IN_IDC_RSTG_MOTBK_2010 ,buttonInIdcRstgMotbk2010 );
    DDX_Control(pDX,IDC_BUTTON_IN_MAB_MOTBK_2010 ,buttonInMabMotbk2010 );
    DDX_Control(pDX,IDC_BUTTON_IN_PUBU_LH_MOTBK_2010 ,buttonInPubuLhMotbk2010 );
    DDX_Control(pDX,IDC_BUTTON_IN_PUBU_RH_MOTBK_2010 ,buttonInPubuRhMotbk2010 );
    DDX_Control(pDX,IDC_BUTTON_IN_ROSW_LH_DWN_MOTBK_2010 ,buttonInRoswLhDwnMotbk2010 );
    DDX_Control(pDX,IDC_BUTTON_IN_ROSW_LH_UP_MOTBK_2010 ,buttonInRoswLhUpMotbk2010 );

    DDX_Control(pDX,IDC_BUTTON_IN_ROSW_RH_DWN_MOTBK_2010 ,buttonInRoswRhDwnMotbk2010 );
    DDX_Control(pDX,IDC_BUTTON_IN_ROSW_RH_UP_MOTBK_2010 ,buttonInRoswRhUpMotbk2010 );
    //DDX_Control(pDX,IDC_WINDSHIELD_PROGRESS ,progressWindshieldProgress );

}


BEGIN_MESSAGE_MAP(CPanelAndNode, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_WINDSHIELD_UP, OnButtonWindshieldUp)
    ON_BN_CLICKED(IDC_BUTTON_WINDSHIELD_DOWN, OnButtonWindshieldDown)
    ON_BN_CLICKED(IDC_BUTTON_INDICATORS, OnButtonIndicators)
    ON_BN_CLICKED(IDC_BUTTON_IN_HORN_MOTBK_2010, OnButtonInHornMotbk2010)
    ON_BN_CLICKED(IDC_BUTTON_IN_IDC_LH_MOTBK_2010, OnButtonInIdcLhMotbk2010)
    ON_BN_CLICKED(IDC_BUTTON_IN_IDC_RH_MOTBK_2010, OnButtonInIdcRhMotbk2010)
    ON_BN_CLICKED(IDC_BUTTON_IN_IDC_RSTG_MOTBK_2010, OnButtonInIdcRstgMotbk2010)
    ON_BN_CLICKED(IDC_BUTTON_IN_MAB_MOTBK_2010, OnButtonInMabMotbk2010)
    ON_BN_CLICKED(IDC_BUTTON_IN_PUBU_LH_MOTBK_2010, OnButtonInPubuLhMotbk2010)
    ON_BN_CLICKED(IDC_BUTTON_IN_PUBU_RH_MOTBK_2010, OnButtonInPubuRhMotbk2010)
    ON_BN_CLICKED(IDC_BUTTON_IN_ROSW_LH_DWN_MOTBK_2010, OnButtonInRoswLhDwnMotbk2010)
    ON_BN_CLICKED(IDC_BUTTON_IN_ROSW_LH_UP_MOTBK_2010, OnButtonInRoswLhUpMotbk2010)
    ON_BN_CLICKED(IDC_BUTTON_IN_ROSW_RH_DWN_MOTBK_2010, OnButtonInRoswRhDwnMotbk2010)
    ON_BN_CLICKED(IDC_BUTTON_IN_ROSW_RH_UP_MOTBK_2010, OnButtonInRoswRhUpMotbk2010)
    //ON_BN_HILITE(IDC_BUTTON_IN_ROSW_RH_UP_MOTBK_2010, OnButtonInRoswLhUpMotbk2010 )
    ON_WM_TIMER()
    //ON_WM_LBUTTONDOWN

END_MESSAGE_MAP()

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAndNode::vInitializePanelFields()
{

    valueIndicatorsPeriod = 0;
    editIndicatorsPeriod.vSetBase(BASE_DECIMAL);
    editIndicatorsPeriod.vSetSigned(false);
    editIndicatorsPeriod.vSetValue(0);

    valueInHornMotbk2010 = 0;
    editInHornMotbk2010.vSetBase(BASE_DECIMAL);
    editInHornMotbk2010.vSetSigned(false);
    editInHornMotbk2010.vSetValue(0);

    valueInIdcLhMotbk2010 = 0;
    editInIdcLhMotbk2010.vSetBase(BASE_DECIMAL);
    editInIdcLhMotbk2010.vSetSigned(false);
    editInIdcLhMotbk2010.vSetValue(0);

    valueInIdcRhMotbk2010 = 0;
    editInIdcRhMotbk2010.vSetBase(BASE_DECIMAL);
    editInIdcRhMotbk2010.vSetSigned(false);
    editInIdcRhMotbk2010.vSetValue(0);

    valueInIdcRstgMotbk2010 = 0;
    editInIdcRstgMotbk2010.vSetBase(BASE_DECIMAL);
    editInIdcRstgMotbk2010.vSetSigned(false);
    editInIdcRstgMotbk2010.vSetValue(0);


    valueInMabMotbk2010 = 0;
    editInMabMotbk2010.vSetBase(BASE_DECIMAL);
    editInMabMotbk2010.vSetSigned(false);
    editInMabMotbk2010.vSetValue(0);

    valueInPubuLhMotbk2010 = 0;
    editInPubuLhMotbk2010.vSetBase(BASE_DECIMAL);
    editInPubuLhMotbk2010.vSetSigned(false);
    editInPubuLhMotbk2010.vSetValue(0);

    valueInPubuRhMotbk2010 = 0;
    editInPubuRhMotbk2010.vSetBase(BASE_DECIMAL);
    editInPubuRhMotbk2010.vSetSigned(false);
    editInPubuRhMotbk2010.vSetValue(0);

    valueInRoswLhDwnMotbk2010 = 0;
    editInRoswLhDwnMotbk2010.vSetBase(BASE_DECIMAL);
    editInRoswLhDwnMotbk2010.vSetSigned(false);
    editInRoswLhDwnMotbk2010.vSetValue(0);

    valueInRoswLhUpMotbk2010 = 0;
    editInRoswLhUpMotbk2010.vSetBase(BASE_DECIMAL);
    editInRoswLhUpMotbk2010.vSetSigned(false);
    editInRoswLhUpMotbk2010.vSetValue(0);

    valueInRoswRhDwnMotbk2010 = 0;
    editInRoswRhDwnMotbk2010.vSetBase(BASE_DECIMAL);
    editInRoswRhDwnMotbk2010.vSetSigned(false);
    editInRoswRhDwnMotbk2010.vSetValue(0);

    valueInRoswRhUpMotbk2010 = 0;
    editInRoswRhUpMotbk2010.vSetBase(BASE_DECIMAL);
    editInRoswRhUpMotbk2010.vSetSigned(false);
    editInRoswRhUpMotbk2010.vSetValue(0);
    editInRoswRhUpMotbk2010.SetForegroundWindow();

    valueCtrChovRossMotbk2010 = 0;
    editCtrChovRossMotbk2010.vSetBase(BASE_DECIMAL);
    editCtrChovRossMotbk2010.vSetSigned(false);
    editCtrChovRossMotbk2010.vSetValue(0);

    valueCtrHghtMotbk2010 = 0;
    editCtrHghtMotbk2010.vSetBase(BASE_DECIMAL);
    editCtrHghtMotbk2010.vSetSigned(false);
    editCtrHghtMotbk2010.vSetValue(0);

    valueSuDrlAutomlyMotbk2010 = 0;
    editSuDrlAutomlyMotbk2010.vSetBase(BASE_DECIMAL);
    editSuDrlAutomlyMotbk2010.vSetSigned(false);
    editSuDrlAutomlyMotbk2010.vSetValue(0);

    valueCtrShtrFsMotbk2010 = 0;
    editCtrShtrFsMotbk2010.vSetBase(BASE_DECIMAL);
    editCtrShtrFsMotbk2010.vSetSigned(false);
    editCtrShtrFsMotbk2010.vSetValue(0);

    valueBrigMotbk2010 = 0;
    editBrigMotbk2010.vSetBase(BASE_DECIMAL);
    editBrigMotbk2010.vSetSigned(false);
    editBrigMotbk2010.vSetValue(0);

    valueMileTotMotbk2010 = 0;
    editMileTotMotbk2010.vSetBase(BASE_DECIMAL);
    editMileTotMotbk2010.vSetSigned(false);
    editMileTotMotbk2010.vSetValue(0);

    valueTSecCouRelMotbk2010 = 0;
    editTSecCouRelMotbk2010.vSetBase(BASE_DECIMAL);
    editTSecCouRelMotbk2010.vSetSigned(false);
    editTSecCouRelMotbk2010.vSetValue(0);

    flagWindshieldUp = false;
    buttonWindshieldUp.SetCheck(0);

    flagWindshieldDown = false;
    buttonWindshield.SetCheck(0);

    flagIndicators = false;
    buttonIndicators.SetCheck(0);

    flagInHornMotbk2010 = false;
    buttonInHornMotbk2010.SetCheck(0);

    flagInIdcLhMotbk2010 = false;
    buttonInIdcLhMotbk2010.SetCheck(0);

    flagInIdcRhMotbk2010 = false;
    buttonInIdcRhMotbk2010.SetCheck(0);

    flagInIdcRstgMotbk2010 = false;
    buttonInIdcRstgMotbk2010.SetCheck(0);

    flagInMabMotbk2010 = false;
    buttonInMabMotbk2010.SetCheck(0);

    flagInPubuLhMotbk2010 = false;
    buttonInPubuLhMotbk2010.SetCheck(0);

    flagInPubuRhMotbk2010 = false;
    buttonInPubuRhMotbk2010.SetCheck(0);

    flagInRoswLhDwnMotbk2010 = false;
    buttonInRoswLhDwnMotbk2010.SetCheck(0);

    flagInRoswLhUpMotbk2010 = false;
    buttonInRoswLhUpMotbk2010.SetCheck(0);

    flagInRoswRhDwnMotbk2010 = false;
    buttonInRoswRhDwnMotbk2010.SetCheck(0);

    flagInRoswRhUpMotbk2010 = false;
    buttonInRoswRhUpMotbk2010.SetCheck(0);

    /*progressWindshieldProgress.SetRange(0,100);
    progressWindshieldProgress.SetStep(50);
    progressWindshieldProgress.SetPos(50);
    valueWindshieldProgress=50;
    */



    Message = new mSTXSELMSGDATA;
    Message->m_unCount=1;
    Message->m_psTxMsg = new STCAN_MSG[1];
    Message->m_psTxMsg->m_ucChannel = 1;
    Message->m_psTxMsg->m_ucDataLen = 8;
    Message->m_psTxMsg->m_ucEXTENDED = false;
    //  MessageNode->m_psTxMsg->m_bCANFD = 0x656;
    Message->m_psTxMsg->m_unMsgID = 0x656;



    Message->m_psTxMsg->m_ucData[0] = 1;
    Message->m_psTxMsg->m_ucData[1] = 1;
    Message->m_psTxMsg->m_ucData[2] = 1;
    Message->m_psTxMsg->m_ucData[3] = 1;
    //UpdateData(FALSE);
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
afx_msg  void CPanelAndNode::OnCloseWnd()
{

}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
BOOL CPanelAndNode::OnInitDialog()
{
    CDialog::OnInitDialog();
    vInitializePanelFields();
    return TRUE;
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAndNode::SetPanelValues()
{
    UpdateData(false);
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAndNode::EvaluateMessageReceived(STCAN_MSG  Mensaje )
{

}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAndNode::OnButtonWindshieldUp()
{
    editInRoswRhDwnMotbk2010.vSetValue(0);
    editInRoswRhUpMotbk2010.vSetValue(1);
    UpdateData();
    /*progressWindshieldProgress.SetPos(100);
    */

}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAndNode::OnButtonWindshieldDown()
{
    editInRoswRhDwnMotbk2010.vSetValue(1);
    editInRoswRhUpMotbk2010.vSetValue(0);
    UpdateData();
    /*progressWindshieldProgress.SetPos(0);
    */
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAndNode::OnButtonIndicators()
{

}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAndNode::OnButtonInHornMotbk2010()
{

    if(editInHornMotbk2010.lGetValue()==0)
    {
        editInHornMotbk2010.vSetValue(1);
    }
    else
    {
        editInHornMotbk2010.vSetValue(0);
    }
    valueInHornMotbk2010 = editInHornMotbk2010.fGetValue();
    UpdateData();
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAndNode::OnButtonInIdcLhMotbk2010()
{
    if(editInIdcLhMotbk2010.lGetValue()==0)
    {
        editInIdcLhMotbk2010.vSetValue(1);
    }
    else
    {
        editInIdcLhMotbk2010.vSetValue(0);
    }
    valueInIdcLhMotbk2010 = editInIdcLhMotbk2010.fGetValue();
    UpdateData();
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAndNode::OnButtonInIdcRhMotbk2010()
{
    if(editInIdcRhMotbk2010.lGetValue()==0)
    {
        editInIdcRhMotbk2010.vSetValue(1);
    }
    else
    {
        editInIdcRhMotbk2010.vSetValue(0);
    }
    valueInIdcRhMotbk2010 = editInIdcRhMotbk2010.fGetValue();
    UpdateData();
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAndNode::OnButtonInIdcRstgMotbk2010()
{
    if(editInIdcRstgMotbk2010.lGetValue()==0)
    {
        editInIdcRstgMotbk2010.vSetValue(1);
    }
    else
    {
        editInIdcRstgMotbk2010.vSetValue(0);
    }
    valueInIdcRstgMotbk2010 = editInIdcRstgMotbk2010.fGetValue();
    UpdateData();
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAndNode::OnButtonInMabMotbk2010()
{
    if(editInMabMotbk2010.lGetValue()==0)
    {
        editInMabMotbk2010.vSetValue(1);
    }
    else
    {
        editInMabMotbk2010.vSetValue(0);
    }
    valueInMabMotbk2010 = editInMabMotbk2010.fGetValue();
    UpdateData();
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAndNode::OnButtonInPubuLhMotbk2010()
{
    if(editInPubuLhMotbk2010.lGetValue()==0)
    {
        editInPubuLhMotbk2010.vSetValue(1);
    }
    else
    {
        editInPubuLhMotbk2010.vSetValue(0);
    }
    valueInPubuLhMotbk2010 = editInPubuLhMotbk2010.fGetValue();
    UpdateData();
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAndNode::OnButtonInPubuRhMotbk2010()
{
    if(editInPubuRhMotbk2010.lGetValue()==0)
    {
        editInPubuRhMotbk2010.vSetValue(1);
    }
    else
    {
        editInPubuRhMotbk2010.vSetValue(0);
    }
    valueInPubuRhMotbk2010 = editInPubuRhMotbk2010.fGetValue();
    UpdateData();
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAndNode::OnButtonInRoswLhDwnMotbk2010()
{
    if(editInRoswLhDwnMotbk2010.lGetValue()==0)
    {
        editInRoswLhDwnMotbk2010.vSetValue(1);
    }
    else
    {
        editInRoswLhDwnMotbk2010.vSetValue(0);
    }
    valueInRoswLhDwnMotbk2010 = editInRoswLhDwnMotbk2010.fGetValue();
    UpdateData();
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAndNode::OnButtonInRoswLhUpMotbk2010()
{
    if(editInRoswLhUpMotbk2010.lGetValue()==0)
    {
        editInRoswLhUpMotbk2010.vSetValue(1);
    }
    else
    {
        editInRoswLhUpMotbk2010.vSetValue(0);
    }
    valueInRoswLhUpMotbk2010 = editInRoswLhUpMotbk2010.fGetValue();
    UpdateData();
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAndNode::OnButtonInRoswRhDwnMotbk2010()
{
    if(editInRoswRhDwnMotbk2010.lGetValue()==0)
    {
        editInRoswRhDwnMotbk2010.vSetValue(1);
    }
    else
    {
        editInRoswRhDwnMotbk2010.vSetValue(0);
    }
    valueInRoswRhDwnMotbk2010 = editInRoswRhDwnMotbk2010.fGetValue();
    UpdateData();
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAndNode::OnButtonInRoswRhUpMotbk2010()
{
    if(editInRoswRhUpMotbk2010.lGetValue()==0)
    {
        editInRoswRhUpMotbk2010.vSetValue(1);
    }
    else
    {
        editInRoswRhUpMotbk2010.vSetValue(0);
    }
    valueInRoswRhUpMotbk2010 = editInRoswRhUpMotbk2010.fGetValue();
    UpdateData();
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAndNode::PrepareData()
{
    UpdateData();

    //editIndicatorsPeriod.vSetValue(buttonIndicators.GetState());
    valueInHornMotbk2010 = editInHornMotbk2010.lGetValue();
    valueInIdcLhMotbk2010 = editInIdcLhMotbk2010.lGetValue();
    valueInIdcRhMotbk2010 = editInIdcRhMotbk2010.lGetValue();
    valueInIdcRstgMotbk2010 = editInIdcRstgMotbk2010.lGetValue();
    valueInMabMotbk2010 = editInMabMotbk2010.lGetValue();
    valueInPubuLhMotbk2010 = editInPubuLhMotbk2010.lGetValue();
    valueInPubuRhMotbk2010 = editInPubuRhMotbk2010.lGetValue();
    valueInRoswLhDwnMotbk2010 = editInRoswLhDwnMotbk2010.lGetValue();
    valueInRoswLhUpMotbk2010 = editInRoswLhUpMotbk2010.lGetValue();
    valueInRoswRhDwnMotbk2010 = editInRoswRhDwnMotbk2010.lGetValue();
    valueInRoswRhUpMotbk2010 = editInRoswRhUpMotbk2010.lGetValue();
    valueBrigMotbk2010 = (int) (editBrigMotbk2010.fGetValue()/0.3937);
    valueMileTotMotbk2010 = editMileTotMotbk2010.lGetValue();
    valueTSecCouRelMotbk2010 = editTSecCouRelMotbk2010.lGetValue();
    valueCtrChovRossMotbk2010 = editCtrChovRossMotbk2010.lGetValue();
    valueCtrHghtMotbk2010 = editCtrHghtMotbk2010.lGetValue();
    valueSuDrlAutomlyMotbk2010 = editSuDrlAutomlyMotbk2010.lGetValue();
    valueCtrShtrFsMotbk2010 = editCtrShtrFsMotbk2010.lGetValue();

}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAndNode::OnTimerInSwcl()
{
    initialTimeInScl = currentTime;
    PrepareData();

    Message->m_psTxMsg->m_ucChannel = 1;
    Message->m_psTxMsg->m_ucDataLen = 8;
    //MessageNode->m_psTxMsg->m_ucEXTENDED = false;
    Message->m_psTxMsg->m_unMsgID = 672;
    Message->m_psTxMsg->m_ucData[0] =   0x00;
    Message->m_psTxMsg->m_ucData[1] =   ((BOOL)valueInHornMotbk2010) * 128 + ((BOOL)valueInIdcRstgMotbk2010) * 64 +
                                        ((BOOL)valueInIdcRhMotbk2010) * 32 + ((BOOL)valueInIdcLhMotbk2010) * 16 +
                                        ((BOOL) 1/*D'on trec InCctrON*/ ) * 8 + ((BOOL)valueInPubuRhMotbk2010) * 4 +
                                        ((BOOL)valueInRoswRhUpMotbk2010) * 2 + ((BOOL)valueInRoswRhDwnMotbk2010);

    Message->m_psTxMsg->m_ucData[2] =   ((BOOL)valueInPubuLhMotbk2010) * 64 + ((BOOL)valueInRoswLhUpMotbk2010) * 32 +
                                        ((BOOL)valueInRoswLhDwnMotbk2010) * 16 + ((BOOL) 1/*D'on trec InCctrAcl*/ ) * 8 +
                                        ((BOOL) 1/*D'on trec InCctrDcr*/ ) * 4 + ((BOOL)valueInMabMotbk2010) * 2;

    Message->m_psTxMsg->m_ucData[3] =   ((BOOL) 1/*D'on trec InMmcRh*/ ) * 2 + ((BOOL)1/*D'on trec InMmcLh*/ );

    Message->m_psTxMsg->m_ucData[4] = 0x08 /*D'on trec AlivOp*/ ;
    Message->m_psTxMsg->m_ucData[5] = 0xF0 /*D'on trec StMmcPo*/;
    Message->m_psTxMsg->m_ucData[6] = 0x80 /*D'on trec CommErr*/;
    Message->m_psTxMsg->m_ucData[7] = 0xFF /*D'on trec CrcOp*/;

    MPanel->SendPanelsMessage(Message);
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAndNode::OnTimerKmbi()
{
    initialTimeKombi = currentTime;
    PrepareData();


    Message->m_psTxMsg->m_ucChannel = 1;
    Message->m_psTxMsg->m_ucDataLen = 8;
    //MessageNode->m_psTxMsg->m_ucEXTENDED = false;
    Message->m_psTxMsg->m_unMsgID = 1023;
    Message->m_psTxMsg->m_ucData[0] = 0xFF;//De donde lo saco??
    Message->m_psTxMsg->m_ucData[1] = (UINT8) (valueBrigMotbk2010 & 0x00FF);
    Message->m_psTxMsg->m_ucData[2] = (UINT8) (valueTSecCouRelMotbk2010 & 0x0000FF);
    Message->m_psTxMsg->m_ucData[3] = (UINT8) ((valueTSecCouRelMotbk2010 & 0x00FF00)>>8);
    Message->m_psTxMsg->m_ucData[4] = (UINT8) ((valueTSecCouRelMotbk2010 & 0xFF0000)>>16);
    Message->m_psTxMsg->m_ucData[5] = (UINT8) (valueMileTotMotbk2010 & 0x0000FF);
    Message->m_psTxMsg->m_ucData[6] = (UINT8) ((valueMileTotMotbk2010 & 0x00FF00)>>8);
    Message->m_psTxMsg->m_ucData[7] = (UINT8) ((valueMileTotMotbk2010 & 0xFF0000)>>16);

    MPanel->SendPanelsMessage(Message);
}

/**********************************************************************************************************
 Function Name  :
 Description    :
 Member of      :   CPanelAndNode
 Author(s)      :   Marc Serra
 Date Created   :   11.06.2013
**********************************************************************************************************/
void CPanelAndNode::OnTimerOpVeh()
{
    initialTimeOpVeh= currentTime;
    PrepareData();


    Message->m_psTxMsg->m_ucChannel = 1;
    Message->m_psTxMsg->m_ucDataLen = 8;
    //MessageNode->m_psTxMsg->m_ucEXTENDED = false;
    Message->m_psTxMsg->m_unMsgID = 674;
    Message->m_psTxMsg->m_ucData[0] = 0xFF;
    Message->m_psTxMsg->m_ucData[1] = valueCtrChovRossMotbk2010 * 64 + 16/* d'on trec valuectrop*/;
    Message->m_psTxMsg->m_ucData[2] = 64/* d'on trec OpRbt4*/ + 16 /* d'on trec OpRbt3*/ + 4/* d'on trec OpRbt2*/ + 1 /* d'on trec OpRbt1*/;
    Message->m_psTxMsg->m_ucData[3] = 64/* d'on trec CtrAsc*/ + 8 /* d'on trec CtrDmpng*/ + 1/* d'on trec CtrSuspt*/;
    Message->m_psTxMsg->m_ucData[4] = 64/* d'on trec CtrDim*/ + 8 * valueCtrShtrFsMotbk2010 + valueCtrHghtMotbk2010;
    Message->m_psTxMsg->m_ucData[5] = 0xFF/* d'on trec SuDwaTar*/ ;
    Message->m_psTxMsg->m_ucData[6] = 64 /* d'on trec OpAudAdj*/ + 8 /* d'on trec LangAud*/ + valueSuDrlAutomlyMotbk2010;
    Message->m_psTxMsg->m_ucData[7] = 1/* d'on trec SuIChg*/;

    MPanel->SendPanelsMessage(Message);
}

//////////////////////////////////////////////////////////
void CPanelAndNode::InitTimers(unsigned int curTime )
{
    initialTimeInScl = curTime;
    initialTimeKombi = curTime;
    initialTimeOpVeh = curTime;

}

//////////////////////////////////////////////////////////
void CPanelAndNode::TimersCheck(unsigned int curTime )
{
    currentTime = curTime;
    if((currentTime - initialTimeKombi) > 100 - 1)
    {
        OnTimerKmbi();
        //b->MainP->SendPanelsMessage(b->MessageNode);
    }
    if((currentTime - initialTimeInScl) > 50 - 1 )
    {
        OnTimerInSwcl();
        //b->MainP->SendPanelsMessage(b->MessageNode);
    }
    if((currentTime - initialTimeOpVeh) > 100 - 1)
    {
        OnTimerOpVeh();
        //b->MainP->SendPanelsMessage(b->MessageNode);
    }
}





