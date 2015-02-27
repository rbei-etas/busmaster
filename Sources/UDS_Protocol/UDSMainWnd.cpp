/**
 * \file      UDSMainWnd.cpp
 * \brief     Definition file for CUDSMainWnd class
 * \author    Sanchez Marin Maria Alejandra
 * Designer:    Bentea Radu Mihai

 *  Manager of the Main Window of the UDS tool
 */

#include "stdafx.h"
#include "UDSMainWnd.h"
#include "UDSSettingsWnd.h"
#include "UDS_Resource.h"
#include "DIL_Interface/BaseDIL_CAN.h"
#include "DIL_Interface/DIL_Interface_extern.h"


CUDSMainWnd* CUDSMainWnd::m_spodInstance = NULL;

BOOL FWait_SendingFrame=FALSE;
/** This variable is used to indicate that the system should wait for a flow control */
bool FWaitFlow = FALSE;
bool FlagTP = FALSE;
int Counter_BSize;
BOOL g_bStopSelectedMsgTx = TRUE;
static CBaseDIL_CAN* g_pouDIL_CAN_Interface;
static DWORD g_dwClientID = 0;
int c_unPreviousTime = -1;
double c_dDiffTime = 0;
int S3_Client = 2000;
int S3_Server = 5000;
long Font_Color = RGB(69, 96,200) ;


// CUDSMainWnd dialog

IMPLEMENT_DYNAMIC(CUDSMainWnd, CDialog)

CUDSMainWnd::CUDSMainWnd(UINT SA, UINT TA , UDS_INTERFACE FInterface, UINT CanId,  CWnd* pParent /*=NULL*/)
    : CDialog(CUDSMainWnd::IDD, pParent)
{
    SourceAddress = SA;
    TargetAddress = TA;
    fInterface = FInterface;
    CanID  = CanId;
    P2_Time=250;
    BOOL FWaitFlow = FALSE;
    psTxCanMsgUds = NULL;
    m_stringEditDLC = 0;
    int ConsecutiveFrame = 0x21;
}

CUDSMainWnd::~CUDSMainWnd()
{
}

void CUDSMainWnd::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_SA, m_omSourceAddress);
    DDX_Control(pDX, IDC_EDIT_TA, m_omTargetAddress);
    DDX_Control(pDX, IDC_COMBO_CHANNEL, m_omComboChannelUDS);
    DDX_Control(pDX, IDC_EDIT_CANID, m_omCanID);
    DDX_Control(pDX, IDC_EDIT_DLCC, m_omEditDLC);
    DDX_Text(pDX, IDC_EDIT_DLCC, m_stringEditDLC );
    DDX_Control(pDX, IDC_EDIT_DATA, m_omEditMsgData);
    DDX_Text(pDX, IDC_EDIT_DATA, m_omMsgDataEdit);
    DDX_Text (pDX, IDC_RESPONSE_DATA, m_abDatas);
    DDX_Control(pDX, IDC_CHECK_TP, m_omCheckTP);
    DDX_Text(pDX, IDC_DIAG_SERVICE, m_omDiagService);
    DDX_Control(pDX, IDC_RESPONSE_DATA, m_omDataResponse);
    DDX_Control(pDX, IDC_NUMBER_OF_BYTES, m_omBytes);
    DDX_Control(pDX, IDC_SEND_UDS, m_omSendButton);

}

BEGIN_MESSAGE_MAP(CUDSMainWnd, CDialog)
    ON_BN_CLICKED(IDC_SEND_UDS, OnBnClickedSendUD)
    ON_BN_CLICKED(IDC_CHECK_TP, OnBnClickedTesterPresent)
    ON_EN_UPDATE(IDC_EDIT_DATA, OnEnChangeData)
    ON_EN_UPDATE(IDC_EDIT_SA, OnEnChangeSA)
    ON_EN_UPDATE(IDC_EDIT_TA, OnEnChangeTA)
    ON_WM_TIMER()
    ON_WM_CTLCOLOR()
    ON_WM_KEYDOWN()
    ON_WM_CHAR()

END_MESSAGE_MAP()


// CUDSMainWnd message handlers

//const int NO_OF_CHAR_IN_BYTE = 2;

/**********************************************************************************************************
 Function Name  :   StartTimer
  Input(s)      :   -
  Output        :   -
 Description    :   This function is called to start the timer used to send the Tester Present
 Member of      :   CUDSMainWnd

 Author(s)      :   Sanchez Marin Maria Alejandra
 Date Created   :   28.05.2013
**********************************************************************************************************/

UINT CUDSMainWnd::StartTimer(/*LPVOID alo*/)
{
    if (m_omCheckTP.GetCheck())
    {
        m_nTimer = SetTimer(ID_TIMER_TP, S3_Client, NULL);
    }
    return 0;
}

/**********************************************************************************************************
 Function Name  :   SendFirstFrame
 Input(s)       :   omByteStr contains the bytes that has to be sent in the first frame of the long request
                    abByteArr[] is the array where the data should be put to be sent
                    psTxCanMsgUds is the general structure of the message
                    FInterface indicates the interface selected in the SettingsWnd
 Output         :   -
 Description    :   This function sends the first Message of a long request
 Member of      :   CUDSMainWnd

 Author(s)      :   Sanchez Marin Maria Alejandra
 Date Created   :   28.05.2013
**********************************************************************************************************/

int CUDSMainWnd::SendFirstFrame(CString omByteStr, unsigned char abByteArr[], mPSTXSELMSGDATA psTxCanMsgUds, UDS_INTERFACE FInterface)
{
    bool FirstFrame= TRUE;
    CString Length;
    CString omTempByte;
    CString omByteStrTemp;
    //int numberofFrames =-1    ;
    int c_numberOfTaken= numberOfTaken;                             // It contains the number of Data bytes that can be sent in the current message

    Length.Format("%.3x\n",TotalLength);                        //Prepare the first 2 bytes of a first frame of a long request
    Length = "1" + Length;
    omTempByte = Length.Left(2);
    abByteArr[initialByte]= (BYTE)_tcstol(omTempByte, L'\0', 16);
    omTempByte = Length.Right(3);
    abByteArr[initialByte+1]= (BYTE)_tcstol(omTempByte,L'\0', 16);

    int i=7;
    omByteStrTemp = omByteStr.Left(c_numberOfTaken);                //I take the part of omByteStr that is going to be sent in the first Frame

    //while(FWait_SendingFrame){}                                       // Wait if something is being sent in this moment
    while (omByteStrTemp.GetLength())
    {
        omTempByte = omByteStrTemp.Right(NO_OF_CHAR_IN_BYTE);
        abByteArr[i--] = (BYTE)_tcstol(omTempByte, L'\0', 16);      // Fill the array to sent with the current data
        omByteStrTemp = omByteStrTemp.Left(omByteStrTemp.GetLength() - NO_OF_CHAR_IN_BYTE);
    }
    psTxCanMsgUds->m_psTxMsg->m_ucDataLen = 8;                  // The FirstFrame always has 8 butes

    SendSimpleDiagnosticMessage();                              //Send the message

    omByteStr = omByteStr.Right(((UINT)TotalLength*2)-c_numberOfTaken);          // The bytes that were sent are deleted
    DatatoSend =  omByteStr.Right(((UINT)TotalLength*2)-c_numberOfTaken);        // DatatoSend will contain the rest of the bytes that hasn't been sent yet.
    TotalLength = (((UINT)TotalLength*2)-c_numberOfTaken)/2;

    FirstFrame = FALSE;
    FWaitFlow =TRUE;                    // No Wait for the Flow Control comming from the ECU
    ConsecutiveFrame = 0x21;            //Initialize this -> because a long frame will be sent
    return 0;
}

/**********************************************************************************************************
 Function Name  :   SendContinuosFrames
  Input(s)      :   abByteArr[] is the array where the data should be put to be sent
                    psTxCanMsgUds is the general structure of the message
                    FInterface indicates the interface selected in the SettingsWnd
 Output         :   -
 Description    :   This function sends the continuos frames of a long request. It sends several messages
                    until the BSize parameter has been reached.
 Member of      :   CUDSMainWnd

 Author(s)      :   Sanchez Marin Maria Alejandra
 Date Created   :   28.05.2013
**********************************************************************************************************/

void CUDSMainWnd::SendContinuosFrames( unsigned char abByteArr[],mPSTXSELMSGDATA psTxCanMsgUds, UDS_INTERFACE FInterface)
{

    CString Length;
    CString omTempByte;
    CString omByteStrTemp;
    int numberofFrames =0;                      // It will indicate how many multiples frames have been sent
    int c_numberOfTaken = numberOfTaken+2;      // The consecutive Messages will contain one byte more that the FirstFrame

    int i = aux_Finterface + c_numberOfTaken/2;         // aux_Finterface it's used to indicate that i must be bigger if we're in extended Addressing
    if (TotalLength*2<c_numberOfTaken)          // It only enters here once, at the end when the last message of the multiple frames has to be sent when
    {
        i = TotalLength+aux_Finterface;         // the number of frames that has to be sent is less than c_numberOfTaken
    }
    while (DatatoSend.GetLength())                          // While there is remaining data that has to be sent
    {
        omByteStrTemp = DatatoSend.Left(c_numberOfTaken);   //I take the part of the message that is going to be sent in the current Frame
        //while(FWait_SendingFrame){}                           // Wait if something is being sent in this moment

        while (omByteStrTemp.GetLength())
        {
            omTempByte = omByteStrTemp.Right(NO_OF_CHAR_IN_BYTE);
            abByteArr[i--] = (BYTE)_tcstol(omTempByte, L'\0', 16);              // It fills the array
            omByteStrTemp = omByteStrTemp.Left(omByteStrTemp.GetLength() - NO_OF_CHAR_IN_BYTE);
        }
        psTxCanMsgUds->m_psTxMsg->m_ucDataLen = 8;                  // Consecutive Frames can always have 8 bytes
        abByteArr[initialByte]= ConsecutiveFrame;                   // Put the initial Byte of the consecutive frames in a long request
        SendSimpleDiagnosticMessage();                              // Send the current Message

        DatatoSend = DatatoSend.Right(((UINT)TotalLength*2)-c_numberOfTaken);        // DatatoSend will contain the rest of the bytes that hasn't been sent yet.
        TotalLength = (((UINT)TotalLength*2)-c_numberOfTaken)/2;
        ConsecutiveFrame++;
        if (ConsecutiveFrame == 0x30)
        {
            ConsecutiveFrame=0x20;    // Requirement from the ISO TP
        }
        numberofFrames++;

        if (numberofFrames == BSizE)        // It enters here when I've reached the quantity of Blocks settled by the ECU in the flow Control
        {
            FWaitFlow = TRUE;               // Now it has to wait for the Flow control again
            numberofFrames = 0;
            c_dDiffTime =0;
            return ;                        // Now it has to wait for another FlowControl
        }
        else
        {
            for(c_dDiffTime =0,c_unPreviousTime =-1 ; c_dDiffTime <=STMin; CalculateDiffTime()) {}      // Wait for the STMin Time settled by the ECU in the flow Control
        }
        c_unPreviousTime = -1;              //ReStart the variables for the timmings
        c_dDiffTime = 0;

        i = aux_Finterface + c_numberOfTaken/2;             // it must be a bigger number in the case of extended addressing-> aux_Finterface to control this.
        if (TotalLength*2<c_numberOfTaken)                  // It only enters here once, at the end when the last message of the multiple frames has to be sent when
        {
            i = TotalLength+aux_Finterface;                 // the number of frames that has to be sent is less than c_numberOfTaken
        }
    }
    m_omSendButton.EnableWindow(TRUE);      // It only enters here when it has sent all the msg
    // In the case that this function cannot be completed there is a timer as default to activate the SEND button
}

/**********************************************************************************************************
 Function Name  :   SendSimpleDiagnosticMessage
 Input(s)       :   -
 Output         :   -
 Description    :   This function is used to send any simple message
 Member of      :   CUDSMainWnd

 Author(s)      :   Sanchez Marin Maria Alejandra
 Date Created   :   28.05.2013
**********************************************************************************************************/

void CUDSMainWnd::SendSimpleDiagnosticMessage(void)
{

    g_bStopSelectedMsgTx = FALSE;
    // Get handle of thread and assign it to pulic data
    // member in app class. This will be used to terminate
    // the thread.
    if( psTxCanMsgUds->m_psTxMsg->m_unMsgID != 0 && (TargetAddress || SourceAddress)!=0 )
    {
        //CWinThread* pomThread = NULL ;
        //pomThread = AfxBeginThread( OnSendSelectedMsg, psTxCanMsgUds );
        //FWait_SendingFrame = TRUE;         // No one can send another message  until this procedure has ended
        int nReturn = g_pouDIL_CAN_Interface->DILC_SendMsg(g_dwClientID, psTxCanMsgUds->m_psTxMsg[0]);
    }
    StartTimer();
}
/**********************************************************************************************************
 Function Name  :   SendSimpleDiagnosticMessage
 Input(s)       :   -
 Output         :   -
 Description    :   This function is used to send any simple message
 Member of      :   CUDSMainWnd

 Author(s)      :   Sanchez Marin Maria Alejandra
 Date Created   :   28.05.2013
**********************************************************************************************************/

void CUDSMainWnd::SendSimpleDiagnosticMessagePanels(mPSTXSELMSGDATA pstx)
{

    g_bStopSelectedMsgTx = FALSE;
    // Get handle of thread and assign it to pulic data
    // member in app class. This will be used to terminate
    // the thread.

    //if(   psTxCanMsgUds->m_psTxMsg->m_unMsgID != 0 && (TargetAddress || SourceAddress)!=0 ) {
    //CWinThread* pomThread = NULL ;
    //pomThread = AfxBeginThread( OnSendSelectedMsg, psTxCanMsgUds );
    //FWait_SendingFrame = TRUE;         // No one can send another message  until this procedure has ended
    int nReturn = g_pouDIL_CAN_Interface->DILC_SendMsg(g_dwClientID, pstx->m_psTxMsg[0]);
    //}
    //StartTimer();
}

/**********************************************************************************************************
 Function Name  :   OnSendSelectedMsg
 Input(s)       :   -
 Output         :   -
 Description    :   This function is called to transmit any simple message
 Member of      :   CUDSMainWnd

 Author(s)      :   Sanchez Marin Maria Alejandra
 Date Created   :   28.05.2013
**********************************************************************************************************/
UINT CUDSMainWnd::OnSendSelectedMsg(LPVOID pParam)
{
    // s_omState.ResetEvent();
    mPSTXSELMSGDATA psTxCanMsgUds = static_cast <mPSTXSELMSGDATA> (pParam);
    int nReturn = g_pouDIL_CAN_Interface->DILC_SendMsg(g_dwClientID, psTxCanMsgUds->m_psTxMsg[0]);
    if (nReturn != S_OK)
    {
        // Message not sent.
    }
    FWait_SendingFrame = FALSE;  // Now that the msg was sent its structure can be modified
    //g_pouDIL_CAN_Interface->DILC_ManageMsgBuf(MSGBUF_CLEAR, g_dwClientID, &m_ouMCCanBufFSE);
    return 0;
}
/**********************************************************************************************************
 Function Name  :   PrepareFlowControl
 Input(s)       :   -
 Output         :   -
 Description    :   This function is called when it has been received a long response to send the flow control
 Member of      :   CUDSMainWnd

 Author(s)      :   Sanchez Marin Maria Alejandra
 Date Created   :   28.05.2013
**********************************************************************************************************/

void CUDSMainWnd::PrepareFlowControl()
{

    Counter_BSize = BSize;                              // BSize and SSTMin  depends of the value put in the settingsWnd.
    //FWaitLongRespBSize = Counter_BSize;                   // Not used
    psTxCanMsgUds->m_psTxMsg->m_ucDataLen= SizeFC;      //The DLC of the FCmessage depends of the value put in the settingsWnd.
    memset(psTxCanMsgUds->m_psTxMsg->m_ucData, 0, SizeFC);

    // ################## Casian #############################//
    switch(fInterface)
    {
        case INTERFACE_NORMAL_11 :
        {
            psTxCanMsgUds->m_psTxMsg->m_ucData[initialByte-1] = TargetAddress;
            psTxCanMsgUds->m_psTxMsg->m_ucData[initialByte] = 0x30;
            psTxCanMsgUds->m_psTxMsg->m_ucData[initialByte+1] = BSize;
            psTxCanMsgUds->m_psTxMsg->m_ucData[initialByte+2] = SSTMin; //////
            SendSimpleDiagnosticMessage();

        };
        break;

        case INTERFACE_EXTENDED_11 :
        {
            psTxCanMsgUds->m_psTxMsg->m_ucData[initialByte-1] = TargetAddress;
            psTxCanMsgUds->m_psTxMsg->m_ucData[initialByte] = 0x30;
            psTxCanMsgUds->m_psTxMsg->m_ucData[initialByte+1] = BSize;
            psTxCanMsgUds->m_psTxMsg->m_ucData[initialByte+2] = SSTMin;
            SendSimpleDiagnosticMessage();

        };
        break;

        case INTERFACE_NORMAL_ISO_29 :
        {
            psTxCanMsgUds->m_psTxMsg->m_ucData[initialByte-1] = TargetAddress;
            psTxCanMsgUds->m_psTxMsg->m_ucData[initialByte] = 0x30;
            psTxCanMsgUds->m_psTxMsg->m_ucData[initialByte+1] = BSize;
            psTxCanMsgUds->m_psTxMsg->m_ucData[initialByte+2] = SSTMin;
            SendSimpleDiagnosticMessage();

        };
        break;

        case INTERFACE_NORMAL_J1939_29 :
        {
            psTxCanMsgUds->m_psTxMsg->m_ucData[initialByte] = 0x30;
            psTxCanMsgUds->m_psTxMsg->m_ucData[initialByte+1] = BSize;
            psTxCanMsgUds->m_psTxMsg->m_ucData[initialByte+2] = SSTMin;

            SendSimpleDiagnosticMessage();

        };
        break;

    }
    // ################## Casian #############################//

}

/**********************************************************************************************************
 Function Name  :   PrepareDiagnosticMessage
  Input(s)      :   omByteStr contains the bytes that has to be sent in the first frame of the long request
                    abByteArr[] is the array where the data should be put to be sent
                    psTxCanMsgUds is the general structure of the message
                    ByteArrLen is the length of the message to send
 Output         :   -
 Description    :   This function is called by OnBnClickedSendUDS when the user needs to transmit a message
                    It evaluates the message and divide it according to its lenght
 Member of      :   CUDSMainWnd

 Author(s)      :   Sanchez Marin Maria Alejandra
 Date Created   :   28.05.2013
**********************************************************************************************************/

void CUDSMainWnd::PrepareDiagnosticMessage(CString omByteStr,mPSTXSELMSGDATA psTxCanMsgUds, unsigned char abByteArr[], UINT ByteArrLen)
{
    memset(abByteArr, 0, ByteArrLen);               // Initialize the array to zero
    omByteStr.Replace(" ","");
    int i_counter =0;
    UINT LengthStr = omByteStr.GetLength();
    UINT Result;
    if ( LengthStr%2 != 0)
    {
        CString LastString = "0" + omByteStr.Right(1);
        omByteStr= omByteStr.Left(omByteStr.GetLength() - 1);
        omByteStr = omByteStr+ LastString;

    }
    Result = LengthStr / 2 + LengthStr % 2;
    TotalLength = Result;
    m_omSendButton.EnableWindow(FALSE);  //I have to disable the sendButton everytime that I press SEND
    // If it's a long request I need it
    // If it's a long response I've to restart it  and receive all the bytes
    // If it's a simple response: the response could take more time or it could be a 0x78 resp

    m_nTimer = SetTimer(ID_TIMER_SEND_BUTTON,P2_Time , NULL);               // Time to wait to enable the send button again

    switch (fInterface)
    {
        case INTERFACE_NORMAL_11:
        {
            psTxCanMsgUds->m_psTxMsg->m_ucEXTENDED= FALSE;
            ByteArrLen = ByteArrLen-1;
            if (Result>7)                            // Long Request
            {
                int f =SendFirstFrame(omByteStr,abByteArr,psTxCanMsgUds,fInterface );

            }
            else                                     // Short request
            {
                i_counter = (int) Result;
                abByteArr[0] = Result;
                Result++;                            // Result must be increased to make the size array bigger
                CString omTempByte;
                while (omByteStr.GetLength())
                {
                    omTempByte = omByteStr.Right(NO_OF_CHAR_IN_BYTE);
                    abByteArr[i_counter--] = (BYTE)_tcstol(omTempByte, L'\0', 16);
                    omByteStr = omByteStr.Left(omByteStr.GetLength() - NO_OF_CHAR_IN_BYTE);
                }

                if(fMsgSize)
                {
                    psTxCanMsgUds->m_psTxMsg->m_ucDataLen = 8;
                }
                else
                {
                    psTxCanMsgUds->m_psTxMsg->m_ucDataLen = Result;
                }
                SendSimpleDiagnosticMessage();
            }
        }
        break;
        case INTERFACE_EXTENDED_11:
        {
            psTxCanMsgUds->m_psTxMsg->m_ucEXTENDED= FALSE;
            ByteArrLen = ByteArrLen-2;
            abByteArr[0] = TargetAddress;
            if (Result>6)                                                           // Long Request - The limit of a simple request for extended is 6 because the byte that contains the TA.
            {
                m_omSendButton.EnableWindow(FALSE);
                m_nTimer = SetTimer(ID_TIMER_SEND_BUTTON, P2_Time , NULL);          // Start Timer to wait to enable by default the send button again

                int f =SendFirstFrame(omByteStr,abByteArr,psTxCanMsgUds,fInterface);

            }
            else                                             // Short request
            {
                i_counter = Result+1;
                abByteArr[1] = Result;
                Result = Result+2;
                CString omTempByte;
                while (omByteStr.GetLength())               // Take all the bytes that will be parte of the simple message
                {
                    omTempByte = omByteStr.Right(NO_OF_CHAR_IN_BYTE);
                    abByteArr[i_counter--] = (BYTE)_tcstol(omTempByte, L'\0', 16);
                    omByteStr = omByteStr.Left(omByteStr.GetLength() - NO_OF_CHAR_IN_BYTE);
                }
                if(fMsgSize)
                {
                    psTxCanMsgUds->m_psTxMsg->m_ucDataLen = 8;
                }
                else
                {
                    psTxCanMsgUds->m_psTxMsg->m_ucDataLen = Result;
                }
                SendSimpleDiagnosticMessage();
            }
        }
        break;
        case INTERFACE_NORMAL_ISO_29:
        {
            psTxCanMsgUds->m_psTxMsg->m_ucEXTENDED= TRUE;
            ByteArrLen = ByteArrLen-1;
            if (Result>7)                            // Long Request
            {
                int f =SendFirstFrame(omByteStr,abByteArr,psTxCanMsgUds,fInterface );

            }
            else                                     // Short request
            {
                i_counter = (int) Result;
                abByteArr[0] = Result;
                Result++;                            // Result must be increased to make the size array bigger
                CString omTempByte;
                while (omByteStr.GetLength())
                {
                    omTempByte = omByteStr.Right(NO_OF_CHAR_IN_BYTE);
                    abByteArr[i_counter--] = (BYTE)_tcstol(omTempByte, L'\0', 16);
                    omByteStr = omByteStr.Left(omByteStr.GetLength() - NO_OF_CHAR_IN_BYTE);
                }
                if(fMsgSize)
                {
                    psTxCanMsgUds->m_psTxMsg->m_ucDataLen = 8;
                }
                else
                {
                    psTxCanMsgUds->m_psTxMsg->m_ucDataLen = Result;
                }
                SendSimpleDiagnosticMessage();
            }
        }
        break;
        case INTERFACE_NORMAL_J1939_29:
        {
            psTxCanMsgUds->m_psTxMsg->m_ucEXTENDED= TRUE;
            ByteArrLen = ByteArrLen-1;
            if (Result>7)                            // Long Request
            {
                int f =SendFirstFrame(omByteStr,abByteArr,psTxCanMsgUds,fInterface );

            }
            else                                     // Short request
            {
                i_counter = (int) Result;
                abByteArr[0] = Result;
                Result++;                            // Result must be increased to make the size array bigger
                CString omTempByte;
                while (omByteStr.GetLength())
                {
                    omTempByte = omByteStr.Right(NO_OF_CHAR_IN_BYTE);
                    abByteArr[i_counter--] = (BYTE)_tcstol(omTempByte, L'\0', 16);
                    omByteStr = omByteStr.Left(omByteStr.GetLength() - NO_OF_CHAR_IN_BYTE);
                }
                if(fMsgSize)
                {
                    psTxCanMsgUds->m_psTxMsg->m_ucDataLen = 8;
                }
                else
                {
                    psTxCanMsgUds->m_psTxMsg->m_ucDataLen = Result;
                }
                SendSimpleDiagnosticMessage();
            }
        }
        break;
    }
}

/**********************************************************************************************************
 Function Name  :   OnBnClickedSendUD
 Input(s)       :   -
 Output         :   -
 Description    :   This function is called by framework when user wants to Transmit a  message
                    It Filters the message so only the msg with a proper SA,TA and CANId can be sent
 Member of      :   CUDSMainWnd

 Author(s)      :   Sanchez Marin Maria Alejandra
 Date Created   :   28.05.2013
**********************************************************************************************************/

void CUDSMainWnd::OnBnClickedSendUD()
{
    KillTimer(ID_TIMER_TP);    //Added to kill the timer everyTime I've pressed the SEND button
    FSending = TRUE;        // This flag is used to know if a message has been sent from the UDSMainWnd
    Bytes_to_Show= ("\r\n   1 -> ");
    BytesShown_Line = 1;
    m_abDatas = " ";
    CurrentService = m_omMsgDataEdit.Left(NO_OF_CHAR_IN_BYTE); //Baiasu
    m_omDiagService = initialEval(m_omMsgDataEdit);
    m_omBytes.vSetValue(0);
    //CurrentService = m_omMsgDataEdit.Left(NO_OF_CHAR_IN_BYTE); //moved up by Adrian

    UpdateData(false);
    if (psTxCanMsgUds ==NULL)
    {
        psTxCanMsgUds  = new mSTXSELMSGDATA;
    }

    UpdateData();
    //setValue();
    BYTE byAddress = (BYTE)m_omSourceAddress.lGetValue();
    if(psTxCanMsgUds != NULL )
    {
        psTxCanMsgUds->m_unCount = 1;
        psTxCanMsgUds->m_psTxMsg = new STCAN_MSG[1];
        if(psTxCanMsgUds->m_psTxMsg != NULL )
        {
            psTxCanMsgUds->m_psTxMsg->m_ucRTR = FALSE;
            psTxCanMsgUds->m_psTxMsg->m_ucChannel = (UCHAR)m_omComboChannelUDS.GetCurSel()+1;
            Current_Channel = psTxCanMsgUds->m_psTxMsg->m_ucChannel;
            psTxCanMsgUds->m_psTxMsg->m_unMsgID = (int)m_omCanID.lGetValue();
            if( psTxCanMsgUds->m_psTxMsg->m_unMsgID != 0 && (TargetAddress || SourceAddress)!=0 && m_omMsgDataEdit.GetLength()!=0 )
            {
                PrepareDiagnosticMessage(m_omMsgDataEdit, psTxCanMsgUds, psTxCanMsgUds->m_psTxMsg->m_ucData, 8);     //This funtion evaluate if the message is a long or a short request and prepare the message
            }
            else
            {
                Font_Color = RGB(184,134,11 );
                m_omDiagService = "You have entered an invalid data";
                UpdateData(false);
                StartTimer();    // Start de timer for tester present
            }
        }
    }
}
/**********************************************************************************************************
Function Name  : OnTimer
Input(s)       : nIDEvent
Output         : -
Functionality  : Called during Timer events to send Tester Present ot enable the SEND button
Member of      : CUDSMainWnd
Author(s)      : Sanchez Marin Maria Alejandra
Date Created   : 28.05.2013
Modifications  :
/**********************************************************************************************************/

void  CUDSMainWnd::OnTimer(UINT_PTR nIDEvent)
{

    //Envía TesterPresent
    if(nIDEvent ==ID_TIMER_TP && psTxCanMsgUds->m_psTxMsg != NULL)      //Prepare the message
    {

        if(fInterface == INTERFACE_NORMAL_ISO_29 || fInterface ==INTERFACE_NORMAL_J1939_29)
        {
            psTxCanMsgUds->m_psTxMsg->m_ucEXTENDED= TRUE;               // Initial Config
        }
        else
        {
            psTxCanMsgUds->m_psTxMsg->m_ucEXTENDED= FALSE;              // Initial Config
        }
        psTxCanMsgUds->m_psTxMsg->m_ucRTR = FALSE;
        psTxCanMsgUds->m_psTxMsg->m_ucChannel = (UCHAR)m_omComboChannelUDS.GetCurSel()+1;
        psTxCanMsgUds->m_psTxMsg->m_unMsgID = (int)m_omCanID.lGetValue();
        memset(psTxCanMsgUds->m_psTxMsg->m_ucData, 0, 8);               // Initialize the bytes to 0
        if(fInterface == INTERFACE_EXTENDED_11 )
        {
            psTxCanMsgUds->m_psTxMsg->m_ucData[initialByte-1]= TargetAddress;
        }
        psTxCanMsgUds->m_psTxMsg->m_ucData[initialByte]= 0x02;          //The tester Present will always have 2 bytes 0x3E y 0x00
        psTxCanMsgUds->m_psTxMsg->m_ucData[initialByte+1]= 0x3E;

        if(fMsgSize)                    // how many bytes has the flow Control?
        {
            psTxCanMsgUds->m_psTxMsg->m_ucDataLen = 8;
        }
        else
        {
            psTxCanMsgUds->m_psTxMsg->m_ucDataLen = aux_Finterface + 3;
        }
        if(FCRespReq)
        {
            psTxCanMsgUds->m_psTxMsg->m_ucData[initialByte+2]= 0x80;    // if NoResponse Required is activated
        }
        SendSimpleDiagnosticMessage();
    }
    if ( nIDEvent == ID_TIMER_SEND_BUTTON)
    {
        m_omSendButton.EnableWindow(TRUE);
        KillTimer(ID_TIMER_SEND_BUTTON);
    }
}
/******************************************************************************
 Function Name  :   OnCtlColor

 Description    :   The framework calls this member function when a child
                    control is about to be drawn. Most controls send this
                    message to their parent (usually a dialog box) to prepare
                    the pDC for drawing the control using the correct colors.

 Input(s)       :   CDC* pDC       : Contains a pointer to the display context
                                     for the child window.
                    CWnd* pWnd     : Contains a pointer to the control asking
                                     for the color. May be temporary.
                    UINT nCtlColor : Contains one of the following values,
                                     specifying the type of control
 Output         :   HBRUSH:A handle to the brush that is to be used for painting
                    the control background.
 Functionality  :   The dialog is painted and the different control is draw with
                    a different colour.
 Member of      :   CUDSMainWnd

 Author(s)      :   Sanchez Marin Maria Alejandra.
 Modifications  :
******************************************************************************/
HBRUSH CUDSMainWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr =  CDialog::OnCtlColor( pDC,  pWnd,  nCtlColor);
    UINT nIDD = pWnd->GetDlgCtrlID();

    switch(nIDD)
    {
        case IDC_RESPONSE_DATA:
            //pDC->SetBkMode(OPAQUE );
            //pDC->SetTextColor(RGB(69, 96,200));
            pDC->SetBkColor(RGB(255,255,255));
            break;
        case IDC_DIAG_SERVICE:
            pDC->SetTextColor(Font_Color);
            pDC->SetBkColor(RGB(255,255,255));
            break;
        case IDC_NUMBER_OF_BYTES:
            //pDC->SetTextColor(RGB(69, 96,200));
            pDC->SetBkColor(RGB(255,255,255));
            break;
        default:
            break;
    }
    return hbr;

}

/**********************************************************************************************************
Function Name  : OnBnClickedTesterPresent
Input(s)       : CUDSMainWnd
Output         : -
Functionality  : This function is called by the framework when the user has pressed the TesterPresent's
                 check box to start the timer to send the tester present message or to stops its sending
Member of      : CUDSMainWnd
Author(s)      : Sanchez Marin Maria Alejandra
Date Created   : 28.03.2013
Modifications  :
/**********************************************************************************************************/
void CUDSMainWnd::OnBnClickedTesterPresent()
{
    if (m_omCheckTP.GetCheck())
    {
        CWinThread* pomThread2 = NULL ;
        StartTimer();
        if (psTxCanMsgUds ==NULL)
        {
            psTxCanMsgUds  = new mSTXSELMSGDATA;
        }
        if(psTxCanMsgUds != NULL )          // to verify if it has been created correctly
        {
            psTxCanMsgUds->m_unCount = 1;
            psTxCanMsgUds->m_psTxMsg = new STCAN_MSG[1];
        }
    }
    else
    {
        KillTimer(ID_TIMER_TP);
    }
}

//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

void CUDSMainWnd::OnEnChangeData()
{
    CEdit* e = (CEdit*)GetDlgItem(IDC_EDIT_DATA);

    UpdateData();
    CString omByteStr;
    omByteStr = m_omMsgDataEdit;
    omByteStr.Replace(" ","");
    omByteStr.Replace("\t","");

    UINT LengthStr = omByteStr.GetLength();
    m_omMsgDataEdit = "";
    while( omByteStr.GetLength()>2)
    {
        m_omMsgDataEdit = m_omMsgDataEdit + omByteStr.Left(2)+" ";
        omByteStr = omByteStr.Right(omByteStr.GetLength()-2);
    }
    m_omMsgDataEdit = m_omMsgDataEdit + omByteStr;

    UINT Data_Length = (LengthStr/2) + LengthStr%2;;
    m_stringEditDLC = (LengthStr/2) + LengthStr%2;

    BytesShown_Line = 1;
    m_abDatas = " ";
    m_omDiagService = " ";
    m_omBytes.vSetValue(0);
    UpdateData(false);

    //e->SetFocus();
    e->SetSel(0,-1); // select all text and move cursor at the end
    e->SetSel(-1);

}
//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

void CUDSMainWnd::OnEnChangeSA()
{

    UpdateData();
    SourceAddress = m_omSourceAddress.lGetValue();
    //MsgID = CanID + SourceAddress;
    m_omCanID.vSetValue(CanID + SourceAddress);
    setValue();

    // ######################## Casian ######################
    if( TargetAddress == 0 && SourceAddress == 0)
    {
        m_omCanID.vSetValue(0);
        m_omSendButton.EnableWindow(FALSE);
    }
    else
    {
        m_omSendButton.EnableWindow(TRUE);
    }
    //  ######################## Casian ######################

}
//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

void CUDSMainWnd::OnEnChangeTA()
{

    UpdateData();
    TargetAddress = m_omTargetAddress.lGetValue();
    if(fInterface == INTERFACE_EXTENDED_11 )
    {
        respID = RespMsgID + TargetAddress; // Casian
    }
    if(fInterface == INTERFACE_NORMAL_ISO_29 )
    {
        MsgID = (CanID&NEG_MASK_TA_ID_29Bits)+(TargetAddress & MASK_TA_ID_29Bits);
        CanID = MsgID;
        m_omCanID.vSetValue(MsgID);
    }
    else if( fInterface == INTERFACE_NORMAL_J1939_29)
    {
        MsgID = ((CanID & 0xFFFF00FF) + (TargetAddress<<8));
        CanID = MsgID;
        m_omCanID.vSetValue(MsgID);
        m_omSendButton.EnableWindow(TRUE); // ###################### Casian ###########################
    }

    // ########################## Casian ##########################
    if( TargetAddress == 0 && SourceAddress == 0)
    {
        m_omSendButton.EnableWindow(FALSE);
        m_omCanID.vSetValue(0);
    }
    else
    {
        m_omSendButton.EnableWindow(TRUE);
        setValue();
    }
    //########################## Casian ##########################

}
//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________
//This function is called to set in the UDS_Main_Window the value of the CanID editor Box

void CUDSMainWnd::setValue()
{

    switch (fInterface)
    {
        case INTERFACE_NORMAL_11:
        {
            MsgID = CanID;
            m_omCanID.vSetValue(MsgID);
        }
        break;
        case INTERFACE_EXTENDED_11:
        {
            MsgID = SourceAddress + CanID;
            m_omCanID.vSetValue(MsgID);

        }
        break;
        case INTERFACE_NORMAL_ISO_29:
        {
            if( SourceAddress>0x7FF)
            {
                m_omSourceAddress.vSetValue(000);    // The SA cannot be bigger than 0x7FF
            }
            MsgID = ((CanID & NEG_MASK_SA_ID_29Bits) + (SourceAddress<<11));
            CanID = MsgID;
            m_omCanID.vSetValue(MsgID);

        }
        break;
        case INTERFACE_NORMAL_J1939_29:
        {
            MsgID = ((CanID & 0xFFFFFF00) + (SourceAddress));
            CanID = MsgID;
            m_omCanID.vSetValue(MsgID);

        }
        break;

    }
}
//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

BOOL CUDSMainWnd::OnInitDialog()
{
    CDialog::OnInitDialog();

    /* Get CAN DIL interface */
    DIL_GetInterface(CAN, (void**)&g_pouDIL_CAN_Interface);
    g_pouDIL_CAN_Interface->DILC_RegisterClient(TRUE, g_dwClientID, _("CAN_MONITOR"));

    m_omCheckTP.SetCheck(BST_UNCHECKED);
    vInitializeUDSfFields();
    return TRUE;
}
//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

void CUDSMainWnd::vInitializeUDSfFields()
{

    m_omEditDLC.vSetBase(BASE_DECIMAL);
    m_omEditMsgData.vSetBase(BASE_HEXADECIMAL);

    m_omBytes.vSetBase(BASE_DECIMAL);
    m_omBytes.vSetSigned(false);

    m_omEditDLC.vSetSigned(false);
    m_omEditMsgData.vSetSigned(false);

    m_omSourceAddress.vSetBase(BASE_HEXADECIMAL);
    m_omSourceAddress.vSetSigned(true);
    m_omSourceAddress.vSetValue(SourceAddress);

    m_omTargetAddress.vSetBase(BASE_HEXADECIMAL);
    m_omTargetAddress.vSetSigned(true);
    m_omTargetAddress.vSetValue(TargetAddress);

    switch (fInterface)                       // To evaluate the status of the Textbox  SA y TA
    {
        case INTERFACE_NORMAL_11:
        {
            m_omSourceAddress.LimitText(2);
            m_omTargetAddress. LimitText(2);
            m_omSourceAddress.SetReadOnly(TRUE);
            m_omTargetAddress.SetReadOnly(TRUE);
            m_omCanID.vSetValue(CanID);
        }
        break;
        case INTERFACE_EXTENDED_11:
        {
            m_omSourceAddress.LimitText(2);
            m_omTargetAddress. LimitText(2);
            m_omSourceAddress.SetReadOnly(FALSE);
            m_omTargetAddress.SetReadOnly(FALSE);
            m_omCanID.vSetValue(CanID+SourceAddress);
        }
        break;
        case INTERFACE_NORMAL_ISO_29:
        {
            m_omSourceAddress.SetReadOnly(FALSE);
            m_omTargetAddress.SetReadOnly(FALSE);
            m_omSourceAddress.SetLimitText(3);
            m_omTargetAddress.SetLimitText(3);

            m_omCanID.vSetValue(CanID);
        }
        break;
        case INTERFACE_NORMAL_J1939_29:
        {
            m_omSourceAddress.SetReadOnly(FALSE);
            m_omTargetAddress.SetReadOnly(FALSE);
            m_omSourceAddress.SetLimitText(2);
            m_omTargetAddress.SetLimitText(2);

            m_omCanID.vSetValue(CanID);
        }
        break;

    }

    //########################## Casian ##########################
    if( TargetAddress == 0 && SourceAddress == 0)
    {
        m_omCanID.vSetValue(0);
        m_omSendButton.EnableWindow(FALSE);
    }
    //########################## Casian ##########################


    for (INT_PTR i = 0; i < TotalChannel; i++)
    {
        CString omChannel;
        omChannel.Format("%d", i + 1);
        m_omComboChannelUDS.InsertString(i, omChannel);
    }
    m_omComboChannelUDS.SetCurSel(0);
    vUpdateChannelIDInfo();
    m_omEditDLC.vSetValue(0);
    m_Font.CreatePointFont(110, "Courier");
    GetDlgItem(IDC_RESPONSE_DATA)->SetFont(&m_Font);
    m_Font.CreateFont(               14,                        // nHeight
                                     5,                         // nWidth
                                     0,                         // nEscapement
                                     0,                         // nOrientation
                                     FW_BOLD,                   // nWeight
                                     FALSE,                     // bItalic
                                     FALSE,                     // bUnderline
                                     0,                         // cStrikeOut
                                     ANSI_CHARSET,              // nCharSet
                                     OUT_DEFAULT_PRECIS,        // nOutPrecision
                                     CLIP_DEFAULT_PRECIS,       // nClipPrecision
                                     DEFAULT_QUALITY,           // nQuality
                                     DEFAULT_PITCH | FF_ROMAN,  // nPitchAndFamily
                                     "Arial");                  // Facename

    GetDlgItem(IDC_DIAG_SERVICE)->SetFont(&m_Font);
}

//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

void CUDSMainWnd::vSetDILInterfacePtr(void* ptrDILIntrf)
{

    g_pouDIL_CAN_Interface = (CBaseDIL_CAN*)ptrDILIntrf;
    g_pouDIL_CAN_Interface->DILC_RegisterClient(TRUE, g_dwClientID, _("CAN_MONITOR"));
    //g_pouDIL_CAN_Interface->DILC_ManageMsgBuf(MSGBUF_CLEAR, g_dwClientID, &m_ouMCCanBufFSE);
}

//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

void* CUDSMainWnd::pGetDILInterfacePtr()
{
    return (void*)g_pouDIL_CAN_Interface;
}

//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________
/** This function is called from the Mainframe when the user has changed the channel selection */

void CUDSMainWnd::vUpdateChannelIDInfo()
{
    m_omComboChannelUDS.ResetContent();
    LONG lParam = 0;
    if(((CBaseDIL_CAN*)CUDSMainWnd::pGetDILInterfacePtr()) != NULL)
    {
        if(((CBaseDIL_CAN*)CUDSMainWnd::pGetDILInterfacePtr())
                ->DILC_GetControllerParams(lParam, 0, NUMBER_HW) == S_OK)
        {
            UINT nHardware = (UINT)lParam;
            for( INT_PTR i = 0; i < lParam; i++)
            {
                CString omChannel;
                omChannel.Format("%d", i + 1);
                m_omComboChannelUDS.InsertString(i, omChannel);
            }
        }
    }
    m_omComboChannelUDS.SetCurSel(0);
    g_pouDIL_CAN_Interface->DILC_RegisterClient(TRUE, g_dwClientID, _("CAN_MONITOR"));
}

//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

CUDSMainWnd* CUDSMainWnd::s_podGetUdsMsgManager()
{
    if( m_spodInstance == NULL )
    {
        m_spodInstance = new CUDSMainWnd(0,0,INTERFACE_NORMAL_11,0);
        /*  m_spodInstance->objPrue =1;*/
        // Handling NULL condition is caller's duty
        if( m_spodInstance == NULL )
        {
            // Help debugging
            ASSERT( FALSE );
        }
    }
    // Return the pointer or NULL in case of failure
    return m_spodInstance;
}
//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

int CUDSMainWnd::nCalculateCurrTime(BOOL bFromDIL)      // Calculates the differential time in sec
{
    SYSTEMTIME CurrSysTimes;
    UINT64 c_TimeStamp;

    if (bFromDIL == FALSE)
    {
        GetLocalTime(&CurrSysTimes);
    }
    else
    {
        LARGE_INTEGER QueryTickCount;
        g_pouDIL_CAN_Interface->DILC_GetTimeModeMapping(CurrSysTimes, c_TimeStamp,QueryTickCount);
    }
    int nResult = (CurrSysTimes.wHour * 3600000 + CurrSysTimes.wMinute * 60000
                   + CurrSysTimes.wSecond) * 1000 + CurrSysTimes.wMilliseconds *1;
    return nResult;                  // Milliseconds
}

//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________

void CUDSMainWnd::CalculateDiffTime(void)
{
    if(c_unPreviousTime != -1 )
    {
        c_dDiffTime         = nCalculateCurrTime(FALSE) - c_unPreviousTime;
    }
    else
    {
        c_unPreviousTime = nCalculateCurrTime(FALSE) ;
    }
}
/**********************************************************************************************************
Function Name  : initialEval
Input(s)       : CUDSMainWnd
Output         : -
Functionality  : This funcition is called to evaluate if the sent message waits for
                 a response or nor (No Positive Response Required)
Member of      : CUDSMainWnd
Author(s)      : Sanchez Marin Maria Alejandra
Date Created   : 30.05.2013
Modifications  :
/**********************************************************************************************************/

CString CUDSMainWnd::initialEval(CString Data2Send )
{
    int CService;
    CService = strtol(CurrentService, NULL, 16);
    if ( CService==0x10 || CService==0x11 ||CService==0x28 ||CService==0x31 ||CService==0x3E ||CService==0x85 ||CService==0xA0)
    {
        CString SendingData = Data2Send;
        //SendingData.Replace(" ",""); //added by Alejandra - not working
        SendingData.Remove(' '); // added in adition to previos function not working
        SendingData = Data2Send.Right(Data2Send.GetLength() - NO_OF_CHAR_IN_BYTE);

        int pos;

        pos = SendingData.Find('8', 0);

        //CString SecondByte = SendingData.Left(1);         //added by Alejandra - not working
        if(pos == 1) //initial if(SecondByte == '8')
        {
            Font_Color = RGB(0,255,0 );
            return "     No Response Required";
        }
    }
    Font_Color = RGB(184,134,11 );
    return "     No Response Received";        // If it's the case of  No Positive Response Required return the default value
}

//________________________________________________________________________________________________________________________________________________________________
//________________________________________________________________________________________________________________________________________________________________


void CUDSMainWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if ( nChar == VK_DELETE )
    {
        OnChar(nChar, 0, 0);
    }
    else
    {
        CUDSMainWnd::OnKeyDown(nChar, nRepCnt, nFlags);
    }
}


BOOL CUDSMainWnd::PreTranslateMessage(MSG* pMsg)
{
    // Capture the space character and
    // do not process the same
    BOOL bSkip = FALSE;
    if ( pMsg->message == WM_CHAR )
    {
        //if ( pMsg->wParam == defEMPTY_CHAR)
        //{
        //    bSkip = TRUE;
        //}
    }
    if ( bSkip == FALSE)
    {
        //bSkip = CUDSMainWnd::PreTranslateMessage(pMsg);
    }

    return bSkip;
}