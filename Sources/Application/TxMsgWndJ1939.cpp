/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  TxMsgWndJ1939.cpp
  Description   :  
  $Log:   X:/Archive/Sources/Application/TxMsgWndJ1939.cpv  $
   
  Author(s)     :  Pradeep Kadoor
  Date Created  :  10/12/2010
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/

#include "stdafx.h"
#include "Application/Resource.h"
#include "HashDefines.h"
#include "Utility/Utility_Thread.h"
#include "DataTypes/J1939_Datatypes.h"
#include "DataTypes/MsgBufAll_Datatypes.h"
#include "DIL_Interface/Dil_Interface_extern.h"
#include "DIL_Interface/BaseDIL_CAN.h"
#include "DIL_Interface/BaseDIL_J1939.h"
#include "Application/InterfaceGetter.h"
#include "TxMsgWndJ1939.h"
#include ".\txmsgwndj1939.h"

HANDLE sg_hMsgSent = NULL;
HANDLE sg_hMsgStopped = NULL;
typedef struct 
{
    EJ1939_MSG_TYPE m_eType;
    CString         m_omTypeName;
    BOOL            m_bShow;
} SCONFIGDATA_COMB;

#define SIZE_TYPE_COMB    3

SCONFIGDATA_COMB sg_asMsgType[SIZE_TYPE_COMB] =
{
    {MSG_TYPE_REQUEST,          _T("Request PGN"),    TRUE},
    {MSG_TYPE_DATA,             _T("Data"),                 TRUE},
    {MSG_TYPE_BROADCAST,        _T("Broadcast"),            TRUE},
};

/*************************************************************************
    Function Name    : unGetMsgIDFromName
    Input(s)         : Message name with ID attached in it MsgName[0xMsgID]
    Output           :  
    Functionality    : 
    Member of        : 
    Author(s)        :  Anish kumar
    Date Created     :  
**************************************************************************/
static UINT unGetMsgIDFromName(CString omMsgName)
{
	CString omStrMsgID;
	UINT unMsgID = (UINT)-1;
	CHAR* pcStopStr = NULL;
	int nIndex = omMsgName.ReverseFind(defMSGID_EXTENDED);
	if(nIndex != -1)
	{
		int nLength = omMsgName.GetLength(); 
		omStrMsgID = omMsgName.Mid(nIndex+1,nLength-1);
		unMsgID = strtol((LPCTSTR )omStrMsgID,&pcStopStr,16);
	}
	return unMsgID;
}

void vPopulateMsgTypeComboBox(SCONFIGDATA_COMB asConfigData[], int nSize, 
                       CComboBox& omComboBox)
{
    for (int i = 0; i < nSize; i++)
    {
        if (asConfigData[i].m_bShow)
        {
            omComboBox.InsertString(i, asConfigData[i].m_omTypeName);
        }
    }
}
void CTxMsgWndJ1939::vPopulatePGNComboBox(void)
{
    m_CS_ConfigData.Lock();
    CSize omSize(0,0);
    
    m_omComboPGN.ResetContent();
    const SMSGENTRY* psTemp = m_psMsgRoot;
    while (psTemp != NULL)
    {
        if (psTemp->m_psMsg != NULL)
        {
            CString omMsgIdWithName;
			omMsgIdWithName.Format(defSTR_MSG_ID_IN_HEX,
                                    psTemp->m_psMsg->m_unMessageCode);
            omMsgIdWithName += psTemp->m_psMsg->m_omStrMessageName;
			int nIndex = m_omComboPGN.AddString(omMsgIdWithName);
            CSize omTextSize = m_omComboPGN.GetDC()->GetTextExtent(omMsgIdWithName);
            if (omTextSize.cx > omSize.cx)
            {
                omSize = omTextSize;
            }
            m_omComboPGN.SetItemData(nIndex, psTemp->m_psMsg->m_unMessageCode);
        }
        psTemp = psTemp->m_psNext;
    }
    m_omComboPGN.SetHorizontalExtent(omSize.cx);
    m_omComboPGN.SetCurSel(0);

    m_CS_ConfigData.Unlock();
}

const int NO_OF_CHAR_IN_BYTE = 2;

UINT CString_2_ByteArray(CString omByteStr, BYTE abByteArr[], UINT ByteArrLen)
{
    memset(abByteArr, 0, ByteArrLen);
	omByteStr.TrimLeft(); // Get rid of whitespace if there is any
	omByteStr.TrimRight(); // Get rid of whitespace if there is any

    UINT LengthStr = omByteStr.GetLength();

    if ((2 * ByteArrLen) < LengthStr) // If storage capacity of of the target
    {                                 // byte buffer is lesser than the string,
        omByteStr = omByteStr.Left(2 * ByteArrLen); // then chop off the extra
    }                                   // characters from right side of string

    CString omTempByte;
    // If number of characters is an odd number, then number of bytes needed
    UINT Result = LengthStr / 2 + LengthStr % 2; // is set accordingly
    int i = (int) Result - 1;
    while (omByteStr.GetLength())
    {
        omTempByte = omByteStr.Right(NO_OF_CHAR_IN_BYTE);
        abByteArr[i--] = (BYTE)_tcstol(omTempByte, L'\0', 16);
        omByteStr = omByteStr.Left(omByteStr.GetLength() - NO_OF_CHAR_IN_BYTE);
    }

    return Result;
}
/* This function is a call back function will be called after 
sending it completely. */
static void CallBackMsgSent(UINT32 /*unPGN*/, BYTE /*bySrc*/, 
                                BYTE /*byDest*/, BOOL /*bSuccess*/)
{
    /* Make it thread safe. TBD*/
    SetEvent(sg_hMsgSent);
}
/* Thread function used for sending messages cyclically */
DWORD WINAPI Cyclic_Transmission_Thread(LPVOID pVoid)
{
    CPARAM_THREADPROC* pThreadParam = (CPARAM_THREADPROC *) pVoid;
    if (pThreadParam == NULL)
    {
        return (DWORD)-1;
    }
    CTxMsgWndJ1939* pTxMsgWndJ1939 = static_cast<CTxMsgWndJ1939*> (pThreadParam->m_pBuffer);
    if (pTxMsgWndJ1939 == NULL)
    {
        return (DWORD)-1;
    }
    bool bLoopON = true;
    DWORD dwMiliSecs = INFINITE;
    while (bLoopON) // Continue so long as the loop is ON.
    {
        
        WaitForSingleObject(pThreadParam->m_hActionEvent,
                            dwMiliSecs);
        dwMiliSecs = pTxMsgWndJ1939->unGetTimerVal();
        ESTATE_TRANS eTransState = pTxMsgWndJ1939->eGetTransState();
        if ((eTransState == TRANS_TO_BE_STOPPED)
            || (eTransState == TRANS_STOPPED))
        {
			if (NULL != pTxMsgWndJ1939 && IsWindow(pTxMsgWndJ1939->m_hWnd))
			{
				pTxMsgWndJ1939->vProcessTransmission(FALSE);
				dwMiliSecs = INFINITE;
				ResetEvent(pThreadParam->m_hActionEvent);
			}
        }
        else if (eTransState == TRANS_STARTED)
        {
            switch (pThreadParam->m_unActionCode)
            {
                case INVOKE_FUNCTION:
                {
                    if (pTxMsgWndJ1939->SendSavedMessage() == S_OK)
                    {
                        if (pTxMsgWndJ1939->m_sMsgToBeSent.m_unDLC > 8)
                        {
                            WaitForSingleObject(sg_hMsgSent, INFINITE);
                        }
                    }
                }
                break;
                case EXIT_THREAD:
                {
                    bLoopON = false;
                }
                break;
                case IDLE: break;
                default: break;
            }
        }
    }
    SetEvent(pThreadParam->hGetExitNotifyEvent()); // Signal the owner that the thread
    Sleep(0);                         // is going to terminate the infinite loop. 
    return 0;
}

// CTxMsgWndJ1939 dialog

IMPLEMENT_DYNAMIC(CTxMsgWndJ1939, CDialog)
CTxMsgWndJ1939::CTxMsgWndJ1939(CWnd* pParent /*=NULL*/, SJ1939CLIENTPARAM& sClientParam)
	: CDialog(CTxMsgWndJ1939::IDD, pParent)
    , m_omMsgDataEditVal(_T(""))
    , m_unDataLength(0)
    , m_nMsgTypeIndex(0)
    , m_sClientParams(sClientParam)
    , m_bNM(TRUE)
{
    m_eTransState = TRANS_STOPPED;
    m_sCyclicThread.m_hActionEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_bThreadStarted = FALSE;
    sg_hMsgSent = CreateEvent(NULL, FALSE, FALSE, NULL);
    sg_hMsgStopped = CreateEvent(NULL, FALSE, FALSE, NULL);
    GetIJ1939DIL()->DILIJ_SetCallBckFuncPtr(m_sClientParams.m_dwClientId, CLBCK_FN_LDATA_CONF, (void*)CallBackMsgSent);
    GetIJ1939DIL()->DILIJ_SetCallBckFuncPtr(m_sClientParams.m_dwClientId, CLBCK_FN_BC_LDATA_CONF, (void*)CallBackMsgSent);
}

CTxMsgWndJ1939::~CTxMsgWndJ1939()
{
    m_sCyclicThread.bTerminateThread();

    if (m_sCyclicThread.m_hActionEvent != NULL)
    {
        CloseHandle(m_sCyclicThread.m_hActionEvent);
        m_sCyclicThread.m_hActionEvent = NULL;
    }
    if (sg_hMsgSent != NULL)
    {
        CloseHandle(sg_hMsgSent);
        sg_hMsgSent = NULL;
    }
    if (sg_hMsgStopped != NULL)
    {
        CloseHandle(sg_hMsgStopped);
        sg_hMsgStopped = NULL;
    }
}

void CTxMsgWndJ1939::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_MSGTYPE, m_omMsgTypeCombo);
    DDX_Control(pDX, IDC_EDIT_DLC, m_omDLCEdit);
    DDX_Control(pDX, IDC_EDIT_DATA, m_omMsgDataEdit);
    DDX_Text(pDX, IDC_EDIT_DATA, m_omMsgDataEditVal);
    DDX_Text(pDX, IDC_EDIT_DLC, m_unDataLength);
    DDX_CBIndex(pDX, IDC_COMBO_MSGTYPE, m_nMsgTypeIndex);
    DDX_Control(pDX, IDC_EDIT_ECU_NAME, m_omEcuName);
    DDX_Control(pDX, IDC_EDIT_CA, m_omCurAddress);
    DDX_Control(pDX, IDC_EDIT_FROM, m_omFromEdit);
    DDX_Control(pDX, IDC_EDIT_PRIORITY, m_omPriorityEdit);
    DDX_Control(pDX, IDC_EDIT_TO, m_omTOEdit);
    DDX_Control(pDX, IDC_COMBO_CHANNEL, m_omComboChannel);
    DDX_Control(pDX, IDC_EDIT_MILI_SEC, m_omMiliSecs);
    DDX_Control(pDX, IDC_CHECK_CYCLIC, m_omCheckCyclic);
    DDX_Control(pDX, IDC_COMBO_PGN, m_omComboPGN);
}


BEGIN_MESSAGE_MAP(CTxMsgWndJ1939, CDialog)
    ON_BN_CLICKED(IDC_SEND, OnBnClickedSend)
    ON_BN_CLICKED(IDC_OK, OnBnClickedOk)
    ON_EN_CHANGE(IDC_EDIT_DLC, OnEnChangeEditDlc)
    ON_EN_CHANGE(IDC_EDIT_DATA, OnEnChangeEditMsg)
    ON_WM_SHOWWINDOW()
    ON_BN_CLICKED(IDC_RADIO_NM, OnBnClickedRadioNm)
    ON_BN_CLICKED(IDC_RADIO_TPF, OnBnClickedRadioTpf)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_CLAIM_ADDRESS, OnBnClickedClaimAddress)
    ON_BN_CLICKED(IDC_RQST_ADDRESS, OnBnClickedRqstAddress)
    ON_BN_CLICKED(IDC_CMD_ADDRESS, OnBnClickedCmdAddress)
    ON_CBN_SELCHANGE(IDC_COMBO_MSGTYPE, OnCbnSelchangeComboMsgtype)
    ON_BN_CLICKED(IDC_CHECK_CYCLIC, OnBnClickedCheckCyclic)
    ON_MESSAGE(WM_CONNECT_CHANGE, OnMessageConnect)
    ON_CBN_SELCHANGE(IDC_COMBO_PGN, OnCbnSelchangeComboPgn)
END_MESSAGE_MAP()


// CTxMsgWndJ1939 message handlers

BOOL CTxMsgWndJ1939::OnInitDialog()
{
    CDialog::OnInitDialog();
    // TODO:  Add extra initialization here

   
    m_omCheckCyclic.SetCheck(BST_UNCHECKED);
    m_omMiliSecs.vSetBase(BASE_DECIMAL);
    m_omMiliSecs.vSetSigned(false);
    m_omMiliSecs.vSetValue(100);
    m_omMiliSecs.EnableWindow(FALSE);

    vInitializeTpfFields();

    
    vInitializeNmFields();

    vEnableTpfFields(FALSE);
    vEnableNmFields(TRUE);

    m_omCheckCyclic.EnableWindow(FALSE);
    m_omMiliSecs.EnableWindow(FALSE);
    return TRUE;
}
static BYTE byExtractAddress(CString& omText)
{
    BYTE byAddress = ADDRESS_NULL;
	CString omTemp;
	TCHAR* pcStopStr = NULL;
    int nIndex = omText.Find(defMSGID_EXTENDED);
    int nCloseBraceIndex = omText.Find(defMSG_NAME_END_CHAR);
	if((nIndex != -1) && (nCloseBraceIndex != -1))
	{
		omTemp = omText.Mid(nIndex + 1, nCloseBraceIndex - (nIndex + 1));
		byAddress = (BYTE)_tcstol((LPCTSTR )omTemp,&pcStopStr,16);
	}
	return byAddress;
}
void CTxMsgWndJ1939::OnBnClickedSend()
{
    UpdateData();
    LPARAM lParam = 0;
    GetICANDIL()->DILC_GetControllerParams(lParam, 0, NUMBER_HW);
    UINT unChannel = (UINT) lParam;
    if (m_bNM == TRUE)
    {
        BYTE byAddress = (BYTE)m_omCurAddress.lGetValue();
        BYTE byEcuName = (BYTE)m_omEcuName.lGetValue();
        CButton* pButton = (CButton*)GetDlgItem(IDC_CLAIM_ADDRESS);
        if (pButton->GetCheck() == BST_CHECKED)
        {   
            for (UINT i = 0; i < unChannel; i++)
            {
                GetIJ1939DIL()->DILIJ_NM_ClaimAddress(m_sClientParams.m_dwClientId, 
                                                      i + 1, byAddress);
            }
        }
        pButton = (CButton*)GetDlgItem(IDC_RQST_ADDRESS);
        if (pButton->GetCheck() == BST_CHECKED)
        {
            for (UINT i = 0; i < unChannel; i++)
            {
                GetIJ1939DIL()->DILIJ_NM_RequestAddress(m_sClientParams.m_dwClientId, i + 1, DEFAULT_PRIORITY, byAddress, byEcuName);
            }
        }
        pButton = (CButton*)GetDlgItem(IDC_CMD_ADDRESS);
        if (pButton->GetCheck() == BST_CHECKED)
        {
            UINT64 unECUNAME = m_omEcuName.lGetValue();
            for (UINT i = 0; i < unChannel; i++)
            {
                GetIJ1939DIL()->DILIJ_NM_CommandAddress(m_sClientParams.m_dwClientId, 
                                                        i + 1, unECUNAME, byAddress, 
                                                        DEFAULT_PRIORITY, byAddress,
                                                        ADDRESS_ALL);
            }
        }
    }
    else
    {
        if ((eGetTransState() == TRANS_STOPPED) && GetIJ1939DIL()->DILIJ_bIsOnline())
        {
            m_sMsgToBeSent.m_unChannel = 1;
            CString omChannel;
            int nIndex = m_omComboChannel.GetCurSel();
            if (nIndex != -1)
            {
                m_omComboChannel.GetLBText(m_omComboChannel.GetCurSel(), omChannel);
                m_sMsgToBeSent.m_unChannel = _ttoi(omChannel);
            }        
            //Data bytes
            if (m_omMsgDataEditVal.GetLength() > (INT)(2 * m_unDataLength))
            {
                m_omMsgDataEditVal = m_omMsgDataEditVal.Left(2 * m_unDataLength);
            }
            //DLC
            m_sMsgToBeSent.m_unDLC = CString_2_ByteArray(m_omMsgDataEditVal, m_sMsgToBeSent.m_abyData, MAX_DATA_LEN_J1939);
            //Msg type
            m_sMsgToBeSent.m_eMsgType = eGetCurrMsgType();
            //Destination address
            m_sMsgToBeSent.m_byDest = (BYTE)m_omTOEdit.lGetValue();
            //Source address
            m_sMsgToBeSent.m_bySrc = (BYTE)m_omFromEdit.lGetValue();
            //Priority
            m_sMsgToBeSent.m_byPriority = (BYTE)m_omPriorityEdit.lGetValue();
            //PGN value
            CString omPGN;
            nIndex = m_omComboPGN.GetCurSel();
            if (nIndex != -1)
            {
                m_omComboPGN.GetLBText(m_omComboPGN.GetCurSel(), omPGN);
            }
            else
            {
                m_omComboPGN.GetWindowText(omPGN);
                omPGN = _T("0x") + omPGN;
            }
            m_sMsgToBeSent.m_unPGN = unGetMsgIDFromName(omPGN);
            if (m_sMsgToBeSent.m_unPGN > MAX_LMT_FOR_PGN)
            {
                CString omPGN;
                omPGN.Format(_T("Invalid PGN value: %x, Please enter a value between 0 and 0x%x"),
                             m_sMsgToBeSent.m_unPGN,  MAX_LMT_FOR_PGN);  
                vSetStatusBarText(omPGN);
                return;
            }
            
            //Now send the message
            //Check for cyclic transmission
            if (m_omCheckCyclic.GetCheck() == BST_CHECKED)
            {
                m_sMsgToBeSent.m_unTimerVal = (UINT)m_omMiliSecs.lGetValue();
                vProcessTransmission(TRUE);
                if (m_bThreadStarted == FALSE)
                {
                    m_sCyclicThread.m_pBuffer = this;
                    m_sCyclicThread.m_unActionCode = INVOKE_FUNCTION;
                    m_bThreadStarted = m_sCyclicThread.bStartThread(Cyclic_Transmission_Thread);
                    SetEvent(m_sCyclicThread.m_hActionEvent);
                }
                else
                {
                    m_sCyclicThread.m_pBuffer = this;
                    m_sCyclicThread.m_unActionCode = INVOKE_FUNCTION;
                    SetEvent(m_sCyclicThread.m_hActionEvent);
                }
            }
            else
            {
                vSetStatusBarText(_T("")); //Clear status bar.
                SendSavedMessage();
            }
        }
        else if (eGetTransState() == TRANS_STARTED)
        {
            vSetTransState(TRANS_TO_BE_STOPPED);
            SetEvent(sg_hMsgSent);
            SetEvent(m_sCyclicThread.m_hActionEvent);
        }
    }
}

HRESULT CTxMsgWndJ1939::SendSavedMessage(void)
{
    HRESULT hResult = S_FALSE;
    if ((m_sMsgToBeSent.m_eMsgType == MSG_TYPE_DATA) 
        || (m_sMsgToBeSent.m_eMsgType == MSG_TYPE_BROADCAST))
    {
        hResult = GetIJ1939DIL()->DILIJ_SendJ1939Msg(m_sClientParams.m_dwClientId,
                                            m_sMsgToBeSent.m_unChannel, 
                                            m_sMsgToBeSent.m_eMsgType, 
                                            m_sMsgToBeSent.m_unPGN, 
                                            m_sMsgToBeSent.m_abyData,
                                            m_sMsgToBeSent.m_unDLC, 
                                            m_sMsgToBeSent.m_byPriority, 
                                            m_sMsgToBeSent.m_bySrc,
                                            m_sMsgToBeSent.m_byDest);
    }
    if (m_sMsgToBeSent.m_eMsgType == MSG_TYPE_REQUEST)
    {
        hResult = GetIJ1939DIL()->DILIJ_RequestPGN(m_sClientParams.m_dwClientId,
                                            m_sMsgToBeSent.m_unChannel,
                                            m_sMsgToBeSent.m_unPGN, 
                                            m_sMsgToBeSent.m_byPriority,
                                            m_sMsgToBeSent.m_bySrc,
                                            m_sMsgToBeSent.m_byDest);
    }
    return hResult;
}

void CTxMsgWndJ1939::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    ShowWindow(SW_HIDE);
}

void CTxMsgWndJ1939::OnEnChangeEditDlc()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
}

void CTxMsgWndJ1939::OnEnChangeEditMsg()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
}

void CTxMsgWndJ1939::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDialog::OnShowWindow(bShow, nStatus);
}

EJ1939_MSG_TYPE CTxMsgWndJ1939::eGetCurrMsgType(void)
{
    return sg_asMsgType[m_omMsgTypeCombo.GetCurSel()].m_eType;
}
void CTxMsgWndJ1939::OnBnClickedRadioNm()
{
    m_bNM = TRUE;
    vEnableNmFields(TRUE);
    vEnableTpfFields(FALSE);
    m_omCheckCyclic.SetCheck(BST_UNCHECKED);
    m_omCheckCyclic.EnableWindow(FALSE);
    m_omMiliSecs.EnableWindow(FALSE);
}

void CTxMsgWndJ1939::OnBnClickedRadioTpf()
{
    m_bNM = FALSE;
    vEnableNmFields(FALSE);
    vEnableTpfFields(TRUE);
    m_omCheckCyclic.EnableWindow(TRUE);
    OnBnClickedCheckCyclic();    
    //Update according to the Msg type.
    OnCbnSelchangeComboMsgtype();
}
void CTxMsgWndJ1939::vEnableNmFields(BOOL bEnable)
{
    /* Enable all the radio buttons */
    GetDlgItem(IDC_CLAIM_ADDRESS)->EnableWindow(bEnable);
    GetDlgItem(IDC_RQST_ADDRESS)->EnableWindow(bEnable);
    GetDlgItem(IDC_CMD_ADDRESS)->EnableWindow(bEnable);

    GetDlgItem(IDC_COMBO_NODE)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDIT_ECU_NAME)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDIT_CA)->EnableWindow(bEnable);
}
void CTxMsgWndJ1939::vEnableTpfFields(BOOL bEnable)
{
    m_omComboPGN.EnableWindow(bEnable);
    GetDlgItem(IDC_COMBO_MSGTYPE)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDIT_FROM)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDIT_TO)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDIT_DLC)->EnableWindow(bEnable);
    GetDlgItem(IDC_EDIT_DATA)->EnableWindow(bEnable);
    m_omComboChannel.EnableWindow(bEnable);
}
void CTxMsgWndJ1939::vInitializeTpfFields(void)
{
    //Set the properties first
    m_omDLCEdit.vSetBase(BASE_DECIMAL);
    m_omMsgDataEdit.vSetBase(BASE_HEXADECIMAL);

    m_omDLCEdit.vSetSigned(false);
    m_omMsgDataEdit.vSetSigned(false);

    m_omPriorityEdit.vSetBase(BASE_HEXADECIMAL);
    m_omPriorityEdit.vSetSigned(true);
    m_omPriorityEdit.LimitText(1);

    m_omFromEdit.vSetBase(BASE_HEXADECIMAL);
    m_omFromEdit.vSetSigned(true);
    m_omFromEdit.LimitText(2);

    m_omTOEdit.vSetBase(BASE_HEXADECIMAL);
    m_omTOEdit.vSetSigned(true);
    m_omTOEdit.LimitText(2);

    //Initialize with default values
    LPARAM lParam;
    GetICANDIL()->DILC_GetControllerParams(lParam, 0, NUMBER_HW);
    for (INT_PTR i = 0; i < lParam; i++)
    {
        CString omChannel;
        omChannel.Format("%d", i + 1);
        m_omComboChannel.InsertString(i, omChannel);
    }
    m_omComboChannel.SetCurSel(0);
    //Populate msg type combo box
    vPopulateMsgTypeComboBox(sg_asMsgType, SIZE_TYPE_COMB, m_omMsgTypeCombo);
    m_omMsgTypeCombo.SetCurSel(0);
    //Populate PGN combo box
    if ((m_psMsgRoot != NULL) && (m_psMsgRoot->m_psMsg != NULL))
    {
        // If atleast one database message is present
        m_CS_ConfigData.Lock();
        vPopulatePGNComboBox();
        m_omMsgTypeCombo.SetCurSel(0);      
    }
    else
    {
        m_omComboPGN.SetWindowText(_T("0"));
    }   
    m_omPriorityEdit.vSetValue(DEFAULT_PRIORITY);
    m_omFromEdit.vSetValue(m_sClientParams.m_byAddress);
    m_omTOEdit.vSetValue(ADDRESS_ALL);
    m_omDLCEdit.vSetValue(0x3);
    m_omMsgDataEdit.vSetValue(0);

    UpdateData(TRUE);
    
}
void CTxMsgWndJ1939::vInitializeNmFields(void)
{ 
    //Set the properties of the field first
    m_omCurAddress.vSetBase(BASE_HEXADECIMAL);
    m_omCurAddress.vSetSigned(false);
    m_omCurAddress.LimitText(2);

    m_omEcuName.vSetBase(BASE_HEXADECIMAL);
    m_omEcuName.vSetSigned(false);
    m_omEcuName.LimitText(16);

    m_omDLCEdit.LimitText(3);

    m_bNM = TRUE;
    CButton* pButton = (CButton*) GetDlgItem(IDC_RADIO_NM);
    pButton->SetCheck(BST_CHECKED);
    //Populate the values
    
    pButton = (CButton*)GetDlgItem(IDC_RQST_ADDRESS);
    pButton->SetCheck(BST_UNCHECKED);
    pButton = (CButton*)GetDlgItem(IDC_CMD_ADDRESS);
    pButton->SetCheck(BST_UNCHECKED);
    pButton = (CButton*)GetDlgItem(IDC_CLAIM_ADDRESS);
    pButton->SetCheck(BST_CHECKED);

    CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_NODE);
    
    int nIndex = pComboBox->AddString(m_sClientParams.m_acName);
    pComboBox->SetItemData(nIndex, m_sClientParams.m_byAddress);
    pComboBox->SetCurSel(nIndex);
    m_omEcuName.vSetValue(m_sClientParams.m_unEcuName);
    m_omCurAddress.vSetValue(m_sClientParams.m_byAddress);

    UpdateData(TRUE);
}
void CTxMsgWndJ1939::OnClose()
{
    ShowWindow(SW_HIDE);
}

void CTxMsgWndJ1939::OnBnClickedClaimAddress()
{
    CWnd* pWnd = (CWnd*)GetDlgItem(IDC_STATIC_CA);
    pWnd->SetWindowText(_T("Address 0x"));
    m_omCurAddress.SetReadOnly(FALSE);
    pWnd = (CWnd*)GetDlgItem(IDC_STATIC_ECU_NAME);
    pWnd->SetWindowText(_T("ECU NAME 0x"));
    m_omEcuName.vSetValue(m_sClientParams.m_unEcuName);
    m_omEcuName.SetReadOnly(TRUE);
}

void CTxMsgWndJ1939::OnBnClickedRqstAddress()
{
    CWnd* pWnd = (CWnd*)GetDlgItem(IDC_STATIC_CA);
    pWnd->SetWindowText(_T("Address 0x"));
    m_omCurAddress.SetReadOnly(TRUE);
    pWnd = (CWnd*)GetDlgItem(IDC_STATIC_ECU_NAME);
    pWnd->SetWindowText(_T("Destination 0x"));
    m_omEcuName.SetReadOnly(FALSE);
    m_omEcuName.vSetValue(ADDRESS_ALL);
}

void CTxMsgWndJ1939::OnBnClickedCmdAddress()
{
    CWnd* pWnd = (CWnd*)GetDlgItem(IDC_STATIC_CA);
    pWnd->SetWindowText(_T("Address 0x"));
    m_omCurAddress.SetReadOnly(FALSE);
    pWnd = (CWnd*)GetDlgItem(IDC_STATIC_ECU_NAME);
    pWnd->SetWindowText(_T("ECU NAME 0x"));
    m_omEcuName.vSetValue(m_sClientParams.m_unEcuName);
    m_omEcuName.SetReadOnly(FALSE);
}

void CTxMsgWndJ1939::OnCbnSelchangeComboMsgtype()
{    
    //Set From address field readonly
    //m_omFromEdit.SetReadOnly(TRUE);
    m_nMsgTypeIndex = m_omMsgTypeCombo.GetCurSel();
    switch (sg_asMsgType[m_nMsgTypeIndex].m_eType)
    {
        case MSG_TYPE_REQUEST:
        {
            m_omDLCEdit.vSetValue(0x3);
            m_omMsgDataEdit.vSetValue(0);
            m_omMsgDataEdit.SetWindowText(_T(""));

            m_omDLCEdit.SetReadOnly(TRUE);
            m_omMsgDataEdit.SetReadOnly(TRUE);
        }
        break;
        case MSG_TYPE_DATA:
        {
            m_omDLCEdit.SetReadOnly(FALSE);
            m_omMsgDataEdit.SetReadOnly(FALSE);
        }
        break;
        case MSG_TYPE_BROADCAST:
        {
            m_omDLCEdit.SetReadOnly(FALSE);
            m_omMsgDataEdit.SetReadOnly(FALSE);
        }
        break;
    }
}
void CTxMsgWndJ1939::vSetJ1939ClientParam(SJ1939CLIENTPARAM& sClientParam)
{
    m_sClientParams = sClientParam;
}

void CTxMsgWndJ1939::OnBnClickedCheckCyclic()
{
    if (m_omCheckCyclic.GetCheck() == BST_CHECKED)
    {
        m_omMiliSecs.EnableWindow(TRUE);
    }
    else
    {
        m_omMiliSecs.EnableWindow(FALSE);
    }

}

ESTATE_TRANS CTxMsgWndJ1939::eGetTransState(void)
{
    return m_eTransState;
}
void CTxMsgWndJ1939::vSetStatusBarText(const TCHAR* pacText)
{
    CWnd* pWnd = GetDlgItem(IDC_STATUSBAR);
    pWnd->SetWindowText(pacText);
}
void CTxMsgWndJ1939::vSetTransState(ESTATE_TRANS eTransState)
{
    m_eTransState = eTransState;
    switch (m_eTransState)
    {
        case TRANS_STARTED: 
        {
            vSetStatusBarText(_T("Transmission started..."));
        }
        break;
        case TRANS_TO_BE_STOPPED:
        {
            vSetStatusBarText(_T("Stopping Transmission..."));
        }
        break;
        case TRANS_STOPPED:
        {
            vSetStatusBarText(_T("Transmission stopped..."));
        }
        break;
    }
}

UINT CTxMsgWndJ1939::unGetTimerVal(void)
{
    UINT unTimerVal = 0;
    m_CS_CyclicTrans.Lock();
    unTimerVal = m_sMsgToBeSent.m_unTimerVal;
    m_CS_CyclicTrans.Unlock();
    return unTimerVal;
}

void CTxMsgWndJ1939::vProcessTransmission(BOOL bStart)
{
    m_CS_CyclicTrans.Lock();
    
    
    CString omWndText = bStart ? _T("Stop") : _T("Transmit");
    CButton* pButton = (CButton*)GetDlgItem(IDC_SEND);
    pButton->SetWindowText(omWndText);
    if (bStart == TRUE)
    {
        GetDlgItem(IDC_RADIO_TPF)->EnableWindow(FALSE);
        GetDlgItem(IDC_RADIO_NM)->EnableWindow(FALSE);
        vEnableTpfFields(FALSE);
        vEnableTpfFields(FALSE);
        m_omCheckCyclic.EnableWindow(FALSE);
        m_omMiliSecs.EnableWindow(FALSE);
        vSetTransState(TRANS_STARTED);
    }
    else
    {
        //First set the event sg_hMsgStopped if it is waiting
        SetEvent(sg_hMsgStopped);
        GetDlgItem(IDC_RADIO_TPF)->EnableWindow(TRUE);
        GetDlgItem(IDC_RADIO_NM)->EnableWindow(TRUE);
        pButton = (CButton*)GetDlgItem(IDC_RADIO_TPF);
        if (pButton->GetCheck() == BST_CHECKED)
        {
            vEnableTpfFields(TRUE);
            vEnableNmFields(FALSE);
            m_omCheckCyclic.EnableWindow(TRUE);
            if (m_omCheckCyclic.GetCheck() == BST_CHECKED)
            {
                m_omMiliSecs.EnableWindow(TRUE);
            }
        }
        else
        {
            vEnableTpfFields(FALSE);
            vEnableNmFields(TRUE);
            m_omCheckCyclic.EnableWindow(FALSE);
            m_omMiliSecs.EnableWindow(FALSE);
        }
        vSetTransState(TRANS_STOPPED);
    }

    m_CS_CyclicTrans.Unlock();

}

LRESULT CTxMsgWndJ1939::OnMessageConnect(WPARAM wParam, LPARAM lParam)
{
    BOOL bConnect = (BOOL)wParam;
    BOOL bOnline = (BOOL)lParam;
    ResetEvent(sg_hMsgStopped); //For safety
    if (bOnline == TRUE)
    {
        GetIJ1939DIL()->DILIJ_SetCallBckFuncPtr(m_sClientParams.m_dwClientId, CLBCK_FN_LDATA_CONF, (void*)CallBackMsgSent);
        GetIJ1939DIL()->DILIJ_SetCallBckFuncPtr(m_sClientParams.m_dwClientId, CLBCK_FN_BC_LDATA_CONF, (void*)CallBackMsgSent);
    }
    if (eGetTransState() == TRANS_STARTED)
    {
        vSetTransState(TRANS_TO_BE_STOPPED);
        SetEvent(sg_hMsgSent);
    }
    if (eGetTransState() == TRANS_TO_BE_STOPPED)
    {
        WaitForSingleObject(sg_hMsgStopped, m_sMsgToBeSent.m_unTimerVal * 2);
    }
    GetDlgItem(IDC_SEND)->EnableWindow(bConnect);
    return 0;
}
void CTxMsgWndJ1939::vSetDatabaseInfo(const SMSGENTRY* psMsgEntry)
{
    m_CS_ConfigData.Lock();

    SMSGENTRY::vClearMsgList(m_psMsgRoot);
    const SMSGENTRY* psTemp = psMsgEntry;
    while (psTemp != NULL)
    {
        SMSGENTRY::bUpdateMsgList(m_psMsgRoot, psTemp->m_psMsg);
        psTemp = psTemp->m_psNext;
    }
    vPopulatePGNComboBox();

    m_CS_ConfigData.Unlock();
}
void CTxMsgWndJ1939::vUpdateDataStore(const SMSGENTRY* psMsgEntry)
{
    const SMSGENTRY* psTemp = psMsgEntry;
    while (psTemp != NULL)
    {
        SMSGENTRY::bUpdateMsgList(m_psMsgRoot, psTemp->m_psMsg);
        psTemp = psTemp->m_psNext;
    }
}
void CTxMsgWndJ1939::vClearDataStore(void)
{
    SMSGENTRY::vClearMsgList(m_psMsgRoot);
}
void CTxMsgWndJ1939::OnCbnSelchangeComboPgn()
{
    //PGN value
    CString omPGN;
    INT nIndex = m_omComboPGN.GetCurSel();
    if (nIndex != -1)
    {
        m_omComboPGN.GetLBText(nIndex, omPGN);
        UINT unPGN = unGetMsgIDFromName(omPGN);
        sMESSAGE* psMsg = NULL;
        if (SMSGENTRY::bGetMsgPtrFromMsgId(m_psMsgRoot, unPGN, psMsg) == TRUE)
        {
            if (psMsg != NULL)
            {
                m_omDLCEdit.vSetValue(psMsg->m_unMessageLength);
                m_omTOEdit.vSetValue((BYTE)unPGN);
                for (UINT i = 0; i < psMsg->m_unMessageLength; i++)
                {
                    m_omMsgDataEditVal += _T("00");
                }
                CString omText;
                m_omMsgDataEdit.GetWindowText(omText);
                if (omText.IsEmpty() == TRUE)
                {
                    m_omMsgDataEdit.vSetValue(0);
                    m_omMsgDataEdit.SetWindowText(m_omMsgDataEditVal);
                    
                }
            }
        }
        UpdateData(TRUE);
    }
}
