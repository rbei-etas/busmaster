// CUDSSettingsWnd.cpp : implementation file
/**
 * \file      UDSSettingsWnd.cpp
 * \brief     Definition file for CUDSSettingsWnd class
 * \author    Sanchez Marin Maria Alejandra
 * Designer:    Bentea Radu Mihai
 *  Manager of the UDS_Protocol Porject
 */

#include "stdafx.h"
#include "UDSSettingsWnd.h"
#include "UDSMainWnd.h"
#include "UDS_Protocol.h"
#include "DataTypes/UDS_DataTypes.h"
#include "UDSWnd_Defines.h"


CUDS_Protocol* UdsProtocolPtr = NULL;
extern CUDSMainWnd* omMainWnd;
int SSTMin;



//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

typedef struct
{
    UDS_INTERFACE   m_eType;
    CString         m_omTypeName;
    BOOL            m_bShow;
} INTERFACE_COMB;

#define SIZE_INTERFACE_COMB    4
#define NUM_STAND_APPLIED 2

INTERFACE_COMB sg_asInterface[SIZE_INTERFACE_COMB] =
{
    {INTERFACE_NORMAL_11,          " 11bits Normal",          TRUE},
    {INTERFACE_EXTENDED_11,        " 11bits Extended",        TRUE},
    {INTERFACE_NORMAL_ISO_29,      " 29bits Normal ISO",      TRUE},
    {INTERFACE_NORMAL_J1939_29,    " 29bits Normal J1939",    TRUE},


};

INTERFACE_COMB sg_asSupportedInterface[SIZE_INTERFACE_COMB] =
{
    {INTERFACE_NORMAL_11,          " 11bits Normal",          TRUE},
    {INTERFACE_EXTENDED_11,        " 11bits Extended",        TRUE},
    {INTERFACE_NORMAL_ISO_29,      " 29bits Normal ISO",      TRUE},
    {INTERFACE_NORMAL_J1939_29,    " 29bits Normal J1939",    TRUE},
};



void vPopulateInterfaceComboBox(INTERFACE_COMB sg_asInterface[], int nSize,

                                CComboBox& omComboBox)
{
    omComboBox.ResetContent();
    for (int i = 0, nIncr = 0; i < nSize; i++)
    {
        if (sg_asInterface[i].m_bShow)
        {
            omComboBox.InsertString(nIncr++, sg_asInterface[i].m_omTypeName);
        }
    }
}
//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

typedef struct
{
    DIAGNOSTIC_STANDARD   Diagnostic_eType;
    CString         m_omStandard;
    BOOL            mo_bShow;
} STANDARD_COMB;

STANDARD_COMB sg_asDiagStandard[NUM_STAND_APPLIED] =
{
    {STANDARD_UDS,          "UDS(ISO 14229)",  TRUE},
    {STANDARD_KWP2000,      "KPW2000(ISO 14230)",        TRUE},
};

STANDARD_COMB sg_asSupportedDiagStandard[NUM_STAND_APPLIED] =
{
    {STANDARD_UDS,          "UDS(ISO 14229)",  TRUE},
    {STANDARD_KWP2000,      "KPW2000(ISO 14230)",        TRUE},
};

void vPopulateDiagnosticStandardComboBox(STANDARD_COMB sg_asInterface[], int nSize,
        CComboBox& omComboBox)
{
    omComboBox.ResetContent();
    for (int i = 0, nIncr = 0; i < nSize; i++)
    {
        if (sg_asInterface[i].mo_bShow)
        {
            omComboBox.InsertString(nIncr++, sg_asInterface[i].m_omStandard);
        }
    }
}

//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________


void vPopulateFCLength(CComboBox& omComboBox)
{
    omComboBox.ResetContent();
    omComboBox.InsertString(0,"3");
    omComboBox.InsertString(1,"8");   //We just have two options for the length of Flowcontrol
}
//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________


// CUDSSettingsWnd dialog

IMPLEMENT_DYNAMIC(CUDSSettingsWnd, CDialog)

CUDSSettingsWnd::CUDSSettingsWnd(CWnd* pParent /*=NULL*/,CUDS_Protocol* pEngine)
    : CDialog(CUDSSettingsWnd::IDD, pParent)
{
    UdsProtocolPtr = pEngine;
}

CUDSSettingsWnd::~CUDSSettingsWnd()
{
}

void CUDSSettingsWnd::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_STAND_DIAG, m_omStdDiag);

    DDX_Control(pDX, IDC_COMBO_INTERFACE, m_omInterface);
    DDX_Control(pDX, IDC_COMBO_FC_LENGTH, m_omFCLength);
    DDX_Control(pDX, IDC_SIZE_MSG, m_omCheckMsgDLC);
    DDX_Control(pDX, IDC_REQ_CAN_ID, m_omReqCanID);
    DDX_Text(pDX, IDC_REQ_CAN_ID, StringReqCanID );

    DDX_Control(pDX, IDC_RES_CAN_ID, m_omRespCanID);
    DDX_Text(pDX, IDC_RES_CAN_ID, StringRespCanID );

    DDX_Control(pDX, IDC_REQ_BASE_ADRRESS, m_omReqBaseAddress);
    DDX_Text(pDX, IDC_REQ_BASE_ADRRESS, StringReqBaseAddress );

    DDX_Control(pDX, IDC_RES_BASE_ADRRESS, m_omRespBaseAddress);
    DDX_Text(pDX, IDC_RES_BASE_ADRRESS, StringRespBaseAddress );

    DDX_Control(pDX, IDC_STmin, m_STMin);
    DDX_Text(pDX, IDC_STmin, Temp_SSTMin);

    DDX_Control(pDX, IDC_BSize, m_BSize);
    DDX_Text(pDX, IDC_BSize, Temp_BSize);

    DDX_Control(pDX, IDC_S3_Client, m_S3Client);
    DDX_Control(pDX, IDC_S3_Server, m_S3_Server);


    DDX_Control(pDX, IDC_P2_TimeOut, P2TimeOut);
    DDX_Control(pDX, IDC_P2_Extended, P2extended);

    DDX_Control(pDX,IDC_TP_NORESP_REQ, m_omTPNoRespReq);

}


BEGIN_MESSAGE_MAP(CUDSSettingsWnd, CDialog)

    ON_CBN_SELCHANGE(IDC_COMBO_INTERFACE, OnCbnSelchangeComboInterface)
    ON_CBN_SELCHANGE(IDC_COMBO_STAND_DIAG, OnCbnSelchangeStandardDiag)
    ON_BN_CLICKED(IDC_OKButton, OnBnOKPressed)
    ON_BN_CLICKED(ID_APPLY_Button, OnBnApplyPressed)
    ON_BN_CLICKED(ID_CANCEL, OnBnCancelPressed)
    ON_BN_CLICKED(IDC_DefaultValues, OnDefaultValuesPressed)
    ON_EN_UPDATE( IDC_STmin, OnEnChangeSTmin)
    ON_MESSAGE ( WM_COMMANDHELP, CUDSSettingsWnd::OnCommandHelp )
END_MESSAGE_MAP()

/**********************************************************************************************************
 Function Name  :   OnEnChangeBSize
 Input(s)       :   -
 Output         :   -
 Description    :   This function is called when the user press the OK button and it saves the new value
                    of the ISO TP parameter BSize
 Member of      :   CUDSSettingsWnd
 Author(s)      :   Sanchez Marin Maria Alejandra
**********************************************************************************************************/

void CUDSSettingsWnd::OnEnChangeBSize()
{
    UpdateData();
    BSize = m_BSize.lGetValue();
    if   ( !Temp_BSize.GetLength())          //in the case that the BSize Box is empty when OK botton has been pressed
    {
        Temp_BSize = "0";
        UpdateData(false);
    }
}

/**********************************************************************************************************
 Function Name  :   OnCbnSelchangeFCLength
 Input(s)       :   -
 Output         :   -
 Description    :   This function is called when the user press the OK button and it saves the new value
                    of the FCLength
 Member of      :   CUDSSettingsWnd
 Author(s)      :   Sanchez Marin Maria Alejandra
**********************************************************************************************************/

void CUDSSettingsWnd::OnCbnSelchangeFCLength()
{
    switch(m_omFCLength.GetCurSel() )
    {
        case 0:
        {
            SizeFC=3+aux_Finterface;
        }
        break;
        case 1:
        {
            SizeFC=8;
        }
        break;
        default:
            //do nothing
            break;
    }
}
//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

void CUDSSettingsWnd::OnEnChangeSTmin()
{
    UpdateData();
    if (Temp_SSTMin.GetLength())
    {
        int STmin_Value = _tcstol(Temp_SSTMin, L'\0', 16);
        if ( STmin_Value >= 0xFA)
        {
            m_STMin.vSetValue(0xF);
            return  ;
        }
        else if( ( STmin_Value&0x80) && ( STmin_Value<0xF1 ) )
        {
            m_STMin.vSetValue(0);
        }
    }
}
//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

void CUDSSettingsWnd::OnSaveSTmin()
{
    UpdateData();
    SSTMin = m_STMin.lGetValue();
    if   ( !Temp_SSTMin.GetLength())         //in the case that the STMin Box is empty when OK botton has been pressed
    {
        Temp_SSTMin = "0";
        UpdateData(false);
    }
}

//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

void CUDSSettingsWnd::OnEnChangeP2_TimeOut()
{
    UpdateData();
    P2_Time = P2TimeOut.lGetValue();
}
//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

void CUDSSettingsWnd::OnEnChangeP2_Extended()
{
    UpdateData();
    P2_Time_Extended =  P2extended.lGetValue();
}

/**********************************************************************************************************
 Function Name  :   OnEnChangeS3_Client
 Input(s)       :   -
 Output         :   -
 Description    :   This function is called when the user press the OK to get the value of the parameter
                    S3_Client inserted in the SettingsWnd.
 Member of      :   CUDSSettingsWnd

 Author(s)      :   Sanchez Marin Maria Alejandra
**********************************************************************************************************/
void CUDSSettingsWnd::OnEnChangeS3_Client()
{
    UpdateData();
    S3_Client = m_S3Client.lGetValue();
}

/**********************************************************************************************************
 Function Name  :   OnEnChangeS3_Client
 Input(s)       :   -
 Output         :   -
 Description    :   This function is called when the user press the OK to get the value of the parameter
                    S3_Client inserted in the SettingsWnd.
 Member of      :   CUDSSettingsWnd

 Author(s)      :   Sanchez Marin Maria Alejandra
**********************************************************************************************************/
void CUDSSettingsWnd::OnEnChangeS3_Server()
{
    UpdateData();
    S3_Server = m_S3_Server.lGetValue();
}
//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

BOOL CUDSSettingsWnd::OnInitDialog()
{
    CDialog::OnInitDialog();
    vInitializeUDSSettingsfFields();
    return TRUE;
}

/**********************************************************************************************************
 Function Name  :   vInitializeUDSSettingsfFields
 Input(s)       :   -
 Output         :   -
 Description    :   This function is called to initialize the settings Window
 Member of      :   CUDSSettingsWnd

 Author(s)      :   Sanchez Marin Maria Alejandra
**********************************************************************************************************/

void CUDSSettingsWnd::vInitializeUDSSettingsfFields()
{

    m_omReqCanID.vSetBase(BASE_HEXADECIMAL);
    m_omRespCanID.vSetBase(BASE_HEXADECIMAL);

    m_omReqCanID. LimitText(3);
    m_omRespCanID. LimitText(3);

    m_omReqCanID.vSetSigned(false);
    m_omRespCanID.vSetSigned(false);

    m_omReqBroadcAddress.vSetBase(BASE_HEXADECIMAL);
    m_omReqBroadcAddress.vSetSigned(false);
    m_omReqBroadcAddress.LimitText(3);

    m_omRespBroadcAddress.vSetBase(BASE_HEXADECIMAL);
    m_omRespBroadcAddress.vSetSigned(false);
    m_omRespBroadcAddress. LimitText(3);

    m_omReqBaseAddress.vSetBase(BASE_HEXADECIMAL);
    m_omReqBaseAddress.vSetSigned(false);
    m_omReqBaseAddress. LimitText(3);

    m_omRespBaseAddress.vSetBase(BASE_HEXADECIMAL);
    m_omRespBaseAddress.vSetSigned(false);
    m_omRespBaseAddress.LimitText(3);

    m_STMin.vSetBase(BASE_HEXADECIMAL);
    m_STMin.LimitText(2);
    m_STMin.vSetSigned(false);
    m_STMin.vSetValue(0);

    m_BSize.vSetBase(BASE_DECIMAL);
    m_BSize.vSetSigned(false);
    m_BSize.vSetValue(0);

    m_S3Client.vSetBase( BASE_DECIMAL);
    m_S3Client.vSetSigned(false);
    m_S3Client.vSetValue(2000);
    m_S3Client.LimitText(6);

    m_S3_Server.vSetBase(   BASE_DECIMAL);
    m_S3_Server.vSetSigned(false);
    m_S3_Server.vSetValue(5000);
    m_S3_Server.LimitText(6);

    P2TimeOut.vSetBase(BASE_DECIMAL);
    P2TimeOut.vSetSigned(false);
    P2TimeOut.vSetValue(250);
    P2TimeOut.LimitText(5);


    P2extended.vSetBase(BASE_DECIMAL);
    P2extended.vSetSigned(false);
    P2extended.vSetValue(2000);
    P2extended.LimitText(6);

    vPopulateInterfaceComboBox(sg_asInterface, SIZE_INTERFACE_COMB, m_omInterface);
    vPopulateDiagnosticStandardComboBox(sg_asDiagStandard,NUM_STAND_APPLIED, m_omStdDiag);    //Changed to modify only the Diagnostic Standard box
    vPopulateFCLength(m_omFCLength);
    m_omFCLength.SetCurSel(0);
    m_omStdDiag.SetCurSel(0);
    m_omInterface.SetCurSel(0);
    m_omStdDiag.SetCurSel(0);

    OnCbnSelchangeComboInterface();
    OnCbnSelchangeStandardDiag();
    Prev_InterfaceIndex = 0;
    OnBnOKPressed();
}


/*******************************************************************************************************************
 Function Name  :   OnCbnSelchangeComboInterface
 Input(s)       :   -
 Output         :   -
 Description    :   This function is called by the framework when the user selects
                     an option in the interface combo box.
 Member of      :   CUDSSettingsWnd

 Author(s)      :   Sanchez Marin Maria Alejandra
*******************************************************************************************************************/

void CUDSSettingsWnd::OnCbnSelchangeComboInterface()
{
    //Set From address field readonly
    //m_omFromEdit.SetReadOnly(TRUE);

    m_nInterfaceIndex = m_omInterface.GetCurSel();
    UdsProtocolPtr->fInterface = sg_asSupportedInterface[m_nInterfaceIndex].m_eType;  //Lo coloco acá para modificar cosas en el MainWnd
    switch (sg_asSupportedInterface[m_nInterfaceIndex].m_eType)
    {
        case INTERFACE_NORMAL_11:
        {
            m_omReqCanID.vSetValue(0x700);
            m_omRespCanID.vSetValue(0x700);
            StringRespCanID= "700";
            StringReqCanID= "700";
            m_omReqCanID. LimitText(3);
            m_omRespCanID. LimitText(3);

            m_omReqCanID.SetReadOnly(FALSE);
            m_omRespCanID.SetReadOnly(FALSE);

            m_omReqBaseAddress.SetReadOnly(TRUE);
            m_omRespBaseAddress.SetReadOnly(TRUE);

            m_omRespBroadcAddress.SetReadOnly(TRUE);
            m_omReqBroadcAddress.SetReadOnly(TRUE);

            StringReqBaseAddress = " ";
            StringRespBaseAddress = " ";

            m_omCheckMsgDLC.SetCheck(BST_CHECKED);
            m_omFCLength.SetCurSel(1);
        }
        break;
        case INTERFACE_EXTENDED_11:
        {
            m_omReqCanID.SetReadOnly(TRUE);
            m_omRespCanID.SetReadOnly(TRUE);

            m_omReqBaseAddress.vSetValue(0x600);
            m_omRespBaseAddress.vSetValue(0x600);

            StringReqBaseAddress = "600";
            StringRespBaseAddress = "600";

            m_omReqCanID. LimitText(3);
            m_omRespCanID. LimitText(3);

            m_omReqBaseAddress.SetReadOnly(FALSE);
            m_omRespBaseAddress.SetReadOnly(FALSE);

            m_omRespBroadcAddress.SetReadOnly(TRUE);
            m_omReqBroadcAddress.SetReadOnly(TRUE);

            StringRespCanID= " ";
            StringReqCanID= " ";

            m_omCheckMsgDLC.SetCheck(BST_UNCHECKED);
            m_omFCLength.SetCurSel(0);

        }
        break;
        case INTERFACE_NORMAL_ISO_29:
        {
            m_omReqCanID. LimitText(8);
            m_omRespCanID. LimitText(8);
            m_omReqCanID.vSetValue(0x1BC00000);
            m_omRespCanID.vSetValue(0x1BC00000);
            StringRespCanID= "1BC00000";
            StringReqCanID= "1BC00000";

            m_omReqCanID.SetReadOnly(FALSE);
            m_omRespCanID.SetReadOnly(FALSE);

            m_omReqBaseAddress.SetReadOnly(TRUE);
            m_omRespBaseAddress.SetReadOnly(TRUE);

            m_omRespBroadcAddress.SetReadOnly(TRUE);
            m_omReqBroadcAddress.SetReadOnly(TRUE);

            StringReqBaseAddress = " ";
            StringRespBaseAddress = " ";

            //m_omCheckMsgDLC.SetCheck(BST_CHECKED);
            //m_omFCLength.SetCurSel(1);
        }
        break;
        case INTERFACE_NORMAL_J1939_29:
        {
            m_omReqCanID. LimitText(8);
            m_omRespCanID. LimitText(8);
            m_omReqCanID.vSetValue(0x18DA0000);
            m_omRespCanID.vSetValue(0x18DA0000);
            StringRespCanID= "18DA0000";
            StringReqCanID= "18DA0000";

            m_omReqCanID.SetReadOnly(FALSE);
            m_omRespCanID.SetReadOnly(FALSE);

            m_omReqBaseAddress.SetReadOnly(TRUE);
            m_omRespBaseAddress.SetReadOnly(TRUE);

            m_omRespBroadcAddress.SetReadOnly(TRUE);
            m_omReqBroadcAddress.SetReadOnly(TRUE);

            StringReqBaseAddress = " ";
            StringRespBaseAddress = " ";

            //m_omCheckMsgDLC.SetCheck(BST_CHECKED);
            //m_omFCLength.SetCurSel(1);
        }
        break;

    }
    //OnChkbOnFlowC8();     // Para actalizar el tamaño del Flow Control
    UpdateData(false);
}
/*******************************************************************************************************************
/*******************************************************************************************************************/
void CUDSSettingsWnd::OnCbnSelchangeStandardDiag()
{

    m_nDiagnosticsIndex = m_omStdDiag.GetCurSel();
    if ( sg_asSupportedDiagStandard[m_omStdDiag.GetCurSel()].Diagnostic_eType == STANDARD_KWP2000 )
    {
        m_omTPNoRespReq.EnableWindow(FALSE);
    }
    else
    {
        m_omTPNoRespReq.EnableWindow(TRUE);
    }

}
/*******************************************************************************************************************
 Function Name  :   OnBnOKPressed
 Input(s)       :   -
 Output         :   -
 Description    :  This function is called by the framework when the user press the OK button in the
                   Settings Window to save all the changes made  previously.
 Member of      :  CUDSSettingsWnd

 Author(s)      :  Sanchez Marin Maria Alejandra
*******************************************************************************************************************/

void CUDSSettingsWnd::OnBnApplyPressed()
{
    //****************************************************************
    if(omMainWnd == NULL)
    {
        //DIL_UDS_ShowWnd();
    }
    //****************************************************************
    UpdateData();
    Prev_DiagnosticsIndex = m_nDiagnosticsIndex;
    Prev_InterfaceIndex = m_nInterfaceIndex;
    UdsProtocolPtr->fDiagnostics= sg_asSupportedDiagStandard[m_nDiagnosticsIndex].Diagnostic_eType;
    UdsProtocolPtr->fInterface = sg_asSupportedInterface[m_nInterfaceIndex].m_eType;
    switch (UdsProtocolPtr->fInterface)
    {
        case INTERFACE_NORMAL_11:                   //In this case I'm changing only the CANID
        {
            numberOfTaken=12;
            initialByte=0;
            aux_Finterface = 0;

            TempReq_CanID = m_omReqCanID.lGetValue();
            TempResp_CanID = m_omRespCanID.lGetValue();

            if ( StringReqCanID.GetLength() == 3  && TempReq_CanID >= 0x001 && TempReq_CanID <= 0x7FF)
            {
                ReqCanID = strtoul(StringReqCanID.Right(3), NULL, 16);  /// 3 in loc de 2 casian
                UdsProtocolPtr->SourceAddress = ReqCanID;
                UdsProtocolPtr->MsgID = strtoul(StringReqCanID.Right(3), NULL, 16);
            }
            else                   // Isn't a valid CAN ID
            {
                UdsProtocolPtr->SourceAddress = 0x00;
                // UdsProtocolPtr->MsgID = 0x000;   ########### Casian ###########
                UdsProtocolPtr->TargetAddress = 00;
            }

            if ( StringRespCanID.GetLength() == 3 && TempResp_CanID >= 0x001 && TempResp_CanID <= 0x7FF)
            {
                ReqCanID = strtoul(StringRespCanID.Right(3), NULL, 16); /// 3 in loc de 2 casian
                UdsProtocolPtr->TargetAddress = ReqCanID;
                respID = strtoul(StringRespCanID , NULL, 16);   // the StringRespCanID is updated only if the CanID is correct

                //############################ casian#################################//
                if(UdsProtocolPtr->SourceAddress == UdsProtocolPtr->TargetAddress)
                {
                    UdsProtocolPtr->MsgID = strtoul(StringReqCanID.Right(3), NULL, 16);

                    if(strtoul(StringReqCanID.Right(3), NULL, 16) == strtoul(StringRespCanID.Right(3), NULL, 16))
                    {
                        //omMainWnd->m_omSendButton.EnableWindow(FALSE);


                        UdsProtocolPtr->MsgID = 0x000;
                        UdsProtocolPtr->SourceAddress = 0x00;
                        UdsProtocolPtr->TargetAddress = 00;
                        //  UdsProtocolPtr->disableButton();
                    }
                    else
                    {
                        omMainWnd->m_omSendButton.EnableWindow(TRUE);
                    }
                    UdsProtocolPtr->MsgID = strtoul(StringReqCanID.Right(3), NULL, 16);

                }
                //############################ casian #################################//
            }
            else                            // Isn't a valid CAN ID
            {
                UdsProtocolPtr->SourceAddress = 0x00;
                //  UdsProtocolPtr->MsgID = 0x000;    ########### Casian ###########
                UdsProtocolPtr->TargetAddress = 00;
            }
        }
        break;
        case INTERFACE_EXTENDED_11:
        {
            numberOfTaken = 10;
            initialByte =1;
            aux_Finterface = 1;
            TempResp_BaseAddress = m_omRespBaseAddress.lGetValue();
            TempReq_BaseAddress = m_omReqBaseAddress.lGetValue();

            if ( StringReqBaseAddress.GetLength()== 3 && TempReq_BaseAddress>=0x100 && TempResp_BaseAddress<=0x700)
            {

                UdsProtocolPtr->MsgID = strtoul(StringReqBaseAddress.Left(3), NULL, 16);

                omMainWnd->m_omSourceAddress.vSetValue(0);  // Casian
                omMainWnd->m_omTargetAddress.vSetValue(0);  // Casian

            }
            else
            {
                UdsProtocolPtr->MsgID = 0x000;
            }

            if ( StringRespBaseAddress.GetLength()== 3 && TempResp_BaseAddress>=0x100 && TempReq_BaseAddress<=0x700)
            {
                RespMsgID= strtoul( StringRespBaseAddress, NULL, 16);
            }
            else
            {
                UdsProtocolPtr->MsgID = 0x000;
            }
        }

        break;
        case INTERFACE_NORMAL_ISO_29:                   //In this case I'm changing only the CANID
        {
            numberOfTaken=12;
            initialByte=0;
            aux_Finterface = 0;

            TempReq_CanID = m_omReqCanID.lGetValue();
            TempResp_CanID = m_omRespCanID.lGetValue();

            if ( StringReqCanID.GetLength() == 8  && TempReq_CanID <= MAX_VALUE_29BIT_ID)
            {
                ReqCanID = strtoul(StringReqCanID, NULL, 16);
                UdsProtocolPtr->SourceAddress = (ReqCanID & MASK_SA_ID_29Bits)>>11;
                UdsProtocolPtr->MsgID = strtoul(StringReqCanID.Right(8), NULL, 16);
                UdsProtocolPtr->TargetAddress = (ReqCanID & MASK_TA_ID_29Bits);
            }
            else                                     // Isn't a valid CAN ID
            {
                UdsProtocolPtr->SourceAddress = 0x00;
                UdsProtocolPtr->MsgID = 0x000;
                UdsProtocolPtr->TargetAddress = 00;
            }

            if ( StringRespCanID.GetLength() == 8 &&  TempResp_CanID <= MAX_VALUE_29BIT_ID)
            {
                //ReqCanID = strtoul(StringRespCanID.Right(2), NULL, 16);
                //UdsProtocolPtr->TargetAddress = ReqCanID;
                respID = strtoul(StringRespCanID , NULL, 16);                       // the StringRespCanID is updated only if the CanID is correct
            }
            else                            // Isn't a valid CAN ID
            {
                UdsProtocolPtr->SourceAddress = 0x00;
                UdsProtocolPtr->MsgID = 0x000;
                UdsProtocolPtr->TargetAddress = 00;
            }
        }
        break;
        case INTERFACE_NORMAL_J1939_29:         //In this case I'm changing only the CANID
        {
            numberOfTaken=12;
            initialByte=0;
            aux_Finterface = 0;

            TempReq_CanID = m_omReqCanID.lGetValue();
            TempResp_CanID = m_omRespCanID.lGetValue();

            if ( StringReqCanID.GetLength() == 8  && TempReq_CanID <= MAX_VALUE_29BIT_ID)
            {
                ReqCanID = strtoul(StringReqCanID, NULL, 16);
                UdsProtocolPtr->TargetAddress = (ReqCanID & MASK_TA_ID_29Bits_J1939)>>8;
                UdsProtocolPtr->MsgID = strtoul(StringReqCanID.Right(8), NULL, 16);
                UdsProtocolPtr->SourceAddress = (ReqCanID & 0xFF);
            }
            else                                 // Isn't a valid CAN ID
            {

                UdsProtocolPtr->SourceAddress = 0x00;
                UdsProtocolPtr->MsgID = 0x000;
                UdsProtocolPtr->TargetAddress = 00;
            }

            if ( StringRespCanID.GetLength() == 8 &&  TempResp_CanID <= MAX_VALUE_29BIT_ID)
            {

                respID = strtoul(StringRespCanID , NULL, 16);   // the StringRespCanID is updated only if the CanID is correct
            }
            else                            // Isn't a valid CAN ID
            {
                UdsProtocolPtr->SourceAddress = 0x00;
                UdsProtocolPtr->MsgID = 0x000;
                UdsProtocolPtr->TargetAddress = 00;
            }
        }
        break;


    }

    //To verify the legth of the simple messages
    fMsgSize = (m_omCheckMsgDLC.GetCheck() !=0) ;
    FCRespReq = (m_omTPNoRespReq.GetCheck() !=0);
    OnEnChangeP2_TimeOut();
    OnEnChangeP2_Extended();
    UdsProtocolPtr->UpdateFields();
    OnCbnSelchangeFCLength();
    OnSaveSTmin();
    OnEnChangeBSize();
    OnEnChangeS3_Client();
    OnEnChangeS3_Server();

}


void CUDSSettingsWnd::OnBnOKPressed()
{
    OnBnApplyPressed();
    this->ShowWindow(SW_HIDE);
}


LRESULT CUDSSettingsWnd::OnCommandHelp ( WPARAM wParam, LPARAM lParam )
{
    DWORD dwDirLen = 1024, dwRetVal = 0;
    char cCurrWkDirPath[1024] = { 0 }, cChmFilePath[2048] = { 0 };
    dwRetVal = GetModuleFileName ( NULL, cCurrWkDirPath, dwDirLen );
    PathRemoveFileSpec ( cCurrWkDirPath );
    CString strChmFilePath = "";
    strChmFilePath = PathCombine ( cChmFilePath, cCurrWkDirPath, "BUSMASTER.chm ::/topics/Diagnostics_Settings.html" );

    ::HtmlHelp ( nullptr, strChmFilePath, HH_DISPLAY_TOPIC, 0 );

    return S_OK;
}

/*******************************************************************************************************************
 Function Name  :   OnDefaultValuesPressed
 Input(s)       :   -
 Output         :   -
 Description    :  It sets in the settings Window the default values according to the standard
 Member of      :  CUDSSettingsWnd

 Author(s)      :  Sanchez Marin Maria Alejandra
*******************************************************************************************************************/

void CUDSSettingsWnd::OnDefaultValuesPressed()
{
    switch (sg_asSupportedInterface[m_nInterfaceIndex].m_eType)
    {
        case INTERFACE_NORMAL_11:
        {
            m_omReqCanID.vSetValue(0x700);
            m_omRespCanID.vSetValue(0x700);
            StringRespCanID= "700";
            StringReqCanID= "700";
            m_omCheckMsgDLC.SetCheck(BST_CHECKED);
            m_omFCLength.SetCurSel(1);
        }
        break;
        case INTERFACE_EXTENDED_11:
        {
            m_omReqBaseAddress.vSetValue(0x600);
            m_omRespBaseAddress.vSetValue(0x600);

            StringReqBaseAddress = "600";
            StringRespBaseAddress = "600";
            m_omCheckMsgDLC.SetCheck(BST_UNCHECKED);
            m_omFCLength.SetCurSel(0);

        }
        break;
        case INTERFACE_NORMAL_ISO_29:
        {
            m_omReqCanID.vSetValue(0x1BC00000);
            m_omRespCanID.vSetValue(0x1BC00000);
            StringRespCanID= "1BC00000";
            StringReqCanID= "1BC00000";
        }
        break;
        case INTERFACE_NORMAL_J1939_29:
        {
            m_omReqCanID.vSetValue(0x18DA0000);
            m_omRespCanID.vSetValue(0x18DA0000);
            StringRespCanID= "18DA0000";
            StringReqCanID= "18DA0000";
        }
        break;
    }
    m_STMin.vSetValue(0);
    m_BSize.vSetValue(0);
    m_S3Client.vSetValue(2000);
    m_S3_Server.vSetValue(5000);
    P2TimeOut.vSetValue(250);
    P2extended.vSetValue(2000);

    UpdateData(false);
}


/*******************************************************************************************************************
 Function Name  :   OnBnCancelPressed
 Input(s)       :   -
 Output         :   -
 Description    :  It closes the settings window without saving the changes
 Member of      :  CUDSSettingsWnd

 Author(s)      :  Sanchez Marin Maria Alejandra
*******************************************************************************************************************/

void CUDSSettingsWnd::OnBnCancelPressed()
{

    //UpdateData();
    m_omStdDiag.SetCurSel(Prev_DiagnosticsIndex);
    OnCbnSelchangeStandardDiag();
    UdsProtocolPtr->fInterface = sg_asSupportedInterface[Prev_InterfaceIndex].m_eType;  //Lo coloco acá para modificar cosas en el MainWnd
    m_omInterface.SetCurSel(Prev_InterfaceIndex);
    switch (UdsProtocolPtr->fInterface)
    {
        case INTERFACE_NORMAL_11:           //In this case I'm changing only the CANID
        {
            numberOfTaken=12;
            initialByte=0;
            aux_Finterface = 0;
            StringReqBaseAddress = " ";
            StringRespBaseAddress = " ";
            UpdateData(false);
            m_omReqCanID.vSetValue(TempReq_CanID);
            m_omRespCanID.vSetValue(TempResp_CanID);

            m_omReqCanID.SetReadOnly(FALSE);
            m_omRespCanID.SetReadOnly(FALSE);

            m_omReqBaseAddress.SetReadOnly(TRUE);
            m_omRespBaseAddress.SetReadOnly(TRUE);

            m_omCheckMsgDLC.SetCheck(BST_CHECKED);
            m_omFCLength.SetCurSel(1);
        }
        break;
        case INTERFACE_EXTENDED_11:
        {
            numberOfTaken = 10;
            initialByte =1;
            aux_Finterface = 1;
            StringRespCanID= " ";
            StringReqCanID= " ";
            UpdateData(false);
            m_omRespBaseAddress.vSetValue( TempResp_BaseAddress);
            m_omReqBaseAddress.vSetValue(TempReq_BaseAddress);
            m_omReqCanID.SetReadOnly(TRUE);
            m_omRespCanID.SetReadOnly(TRUE);

            m_omReqBaseAddress.SetReadOnly(FALSE);
            m_omRespBaseAddress.SetReadOnly(FALSE);

            m_omCheckMsgDLC.SetCheck(BST_UNCHECKED);
            m_omFCLength.SetCurSel(0);

        }
        case INTERFACE_NORMAL_ISO_29:           //In this case I'm changing only the CANID
        {
            numberOfTaken=12;
            initialByte=0;
            aux_Finterface = 0;
            StringReqBaseAddress = " ";
            StringRespBaseAddress = " ";
            UpdateData(false);
            m_omReqCanID.vSetValue(TempReq_CanID);
            m_omRespCanID.vSetValue(TempResp_CanID);

            m_omReqCanID.SetReadOnly(FALSE);
            m_omRespCanID.SetReadOnly(FALSE);

            m_omReqBaseAddress.SetReadOnly(TRUE);
            m_omRespBaseAddress.SetReadOnly(TRUE);

            m_omCheckMsgDLC.SetCheck(BST_CHECKED);
            m_omFCLength.SetCurSel(1);
        }
        case INTERFACE_NORMAL_J1939_29:         //In this case I'm changing only the CANID
        {
            numberOfTaken=12;
            initialByte=0;
            aux_Finterface = 0;
            StringReqBaseAddress = " ";
            StringRespBaseAddress = " ";
            UpdateData(false);
            m_omReqCanID.vSetValue(TempReq_CanID);
            m_omRespCanID.vSetValue(TempResp_CanID);

            m_omReqCanID.SetReadOnly(FALSE);
            m_omRespCanID.SetReadOnly(FALSE);

            m_omReqBaseAddress.SetReadOnly(TRUE);
            m_omRespBaseAddress.SetReadOnly(TRUE);

            m_omCheckMsgDLC.SetCheck(BST_CHECKED);
            m_omFCLength.SetCurSel(1);
        }
        break;
    }

    //To verify the legth of the simple messages

    if (fMsgSize)
    {
        m_omCheckMsgDLC.SetCheck( BST_CHECKED);
    }
    FCRespReq = (m_omTPNoRespReq.GetCheck()!=0);
    P2TimeOut.vSetValue( P2_Time);  // To save the previous P2Time Value
    P2extended.vSetValue(P2_Time_Extended);

    if(SizeFC==8)
    {
        m_omFCLength.SetCurSel(1);
    }
    else
    {
        m_omFCLength.SetCurSel(0);
    }
    m_STMin.vSetValue(SSTMin);
    m_BSize.vSetValue(BSize);
    m_S3Client.vSetValue(S3_Client);
    m_S3_Server.vSetValue(S3_Server);
    this->ShowWindow(SW_HIDE);

}
//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________
