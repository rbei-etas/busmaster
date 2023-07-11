/**
 * \file      UDSMainWnd.h
 * \brief     Interface file for CUDSMainWnd class
 * \author    Sanchez Marin Maria Alejandra
 * Designer:    Bentea Radu Mihai
 * Interface file for CUDSMainWnd class
 */
#pragma once

#include "UDS_Resource.h"
#include "UDS_Protocol.h"
#include "Utility/RadixEdit.h"
#include "UDSWnd_Defines.h"
#include "DataTypes/UDS_DataTypes.h"
#include "MsgBufFSE.h"

/**  It contains the number of Data bytes that can be sent in the current message */
extern int numberOfTaken;

extern int initialByte;

/** It's used to control the index i according to if it's working on extended or normal addressing  */
extern int aux_Finterface;

/** Corresponds to the STMin received from the flowControlMessage */
extern int STMin;
extern int SSTMin;
extern int BSize;
extern int BSizE;
extern int SizeFC;
extern int P2_Time;
extern int P2_Time_Extended;
extern CString Bytes_to_Show;
extern CString StringToShow;

/** This variable is used to know if a message has been sent from the UDSMainWnd  */
extern bool FSending;

/** It Corresponds to the value put in the settingWnd if it's extended   */
extern int RespMsgID;

/** corresponds to the exact ID of the message received    */
extern int respID;

extern bool fMsgSize;   // If TRUE the msg should have 8 bytes always
extern bool FCRespReq;   // If TRUE TP will be ...0x3E 0x80 (No response required
extern int BytesShown_Line;
extern CString CurrentService;
extern UINT_PTR m_nTimer;
extern long Font_Color;
extern int Current_Channel;


class CUDSMainWnd : public CDialog
{
    DECLARE_DYNAMIC(CUDSMainWnd)
public:
    CUDSMainWnd(UINT SA, UINT TA , UDS_INTERFACE fInterface,UINT CanId, CWnd* pParent =NULL );   // standard constructor
    virtual ~CUDSMainWnd();

    /**
    This variable contains all the remaining bytes that has to be sent in a long request
    */
    CString DatatoSend;

    CRadixEdit m_omBytes;

    CComboBox m_omComboChannelUDS;

    CRadixEdit m_omSourceAddress;
    CRadixEdit m_omTargetAddress;
    CRadixEdit m_omCanID;
    CRadixEdit m_omEditDLC;
    UINT m_stringEditDLC;
    CRadixEdit  m_omEditMsgData;
    CString m_omMsgDataEdit;
    CString  m_abDatas;
    CButton m_omCheckTP;

    CString m_omDiagService;
    CRadixEdit m_omDataResponse;
    CButton m_omSendButton;

    UINT TotalLength;

    /** It has the first Byte of the consecutive messages 0x20 , 0x21, 0x22, 0x23  */
    int ConsecutiveFrame;

    static CUDSMainWnd* m_spodInstance;
    UINT SourceAddress;
    UINT TargetAddress;
    UDS_INTERFACE fInterface;

    /** This variable contains  the value of the CAN ID or Base Address that the user inserts into the Settings Wnd  */
    UINT CanID;

    /** This variable contains  the value of the complete CAN ID
    * The value of this variable depends of the type of addressing selected
    */
    int MsgID;

    CString StringCanID;

    /** Structure that contains the message to be sent    */
    mPSTXSELMSGDATA psTxCanMsgUds;

    int TotalChannel;

    CFont m_Font;

    // Declaraci�n de funciones
    (void)vInitializeUDSfFields();

    /**
    * This function is called to set in the UDS_Main_Window the value of the CanID editor Box
    * It takes into account the Interface that has been chosen and the values os SA and TA
    */
    (void)setValue();

    /**
    * This function is called by framework when user wants to Transmit a  message
    * it Filters the message so only the msg with a proper SA,TA and CANId can be sent
    */
    (void)CUDSMainWnd::OnBnClickedSendUD();

    /**
    * This function is called by the framework when the user has pressed the TesterPresent's
    * check box to start the timer to send the tester present message or to stops its sending
    */
    (void)CUDSMainWnd::OnBnClickedTesterPresent();

    /**
    * This function in called by the framework when the user modifies the Databytes section in the
    * Main Window. It's used to format the data inserted and to count the bytes and put the result
    * in the DLC editor box.
    */
    (void)CUDSMainWnd::OnEnChangeData();
    /**
    * This function in called by the framework when the user modifies the SA section in the
    * Main Window. It's used to set the the CAN identifier with the Source Address added
    */
    (void) CUDSMainWnd::OnEnChangeSA();

    /**
    * This function in called by the framework when the user modifies the SA section in the
    * Main Window.
    */
    (void) CUDSMainWnd::OnEnChangeTA();

    /**
    * This function is used to send any simple message
    */
    (void) CUDSMainWnd::SendSimpleDiagnosticMessage(void);

    /**
    \brief  This function sends the first Message of a long request
    * This function prepares the structure of the first frame of a long request and it sends it.
    \param omByteStr contains the bytes that has to be sent in the first frame of the long request
    \param abByteArr[] is the array where the data should be put to be sent
    \param psTxCanMsgUds is the general structure of the message
    \param              FInterface indicates the interface selected in the SettingsWnd
    */
    (int) CUDSMainWnd::SendFirstFrame( CString omByteStr, unsigned char abByteArr[],mPSTXSELMSGDATA psTxCanMsgUds, UDS_INTERFACE FInterface);

    /**
    \brief It send continuos frame of a long request
    * This function sends the continuos frames of a long request. It sends several continuos messages
    * until the BSize parameter has been reached.
    \param abByteArr[] is the array where the data should be put to be sent
    \param  psTxCanMsgUds is the general structure of the message
    \param  FInterface indicates the interface selected in the SettingsWnd
    */
    (void) CUDSMainWnd::SendContinuosFrames(unsigned char abByteArr[],mPSTXSELMSGDATA psTxCanMsgUds, UDS_INTERFACE FInterface);

    /**
    \brief It prepares the message to be sent
    * This function is called by OnBnClickedSendUDS when the user needs to transmit a message
    * It evaluates the message and divide it according to its lenght
    \param  omByteStr contains the bytes that has to be sent in the first frame of the long request
    \param  abByteArr[] is the array where the data should be put to be sent
    \param  psTxCanMsgUds is the general structure of the message
    \param  ByteArrLen is the length of the message to send
    */
    (void) CUDSMainWnd::PrepareDiagnosticMessage(CString omByteStr,mPSTXSELMSGDATA psTxCanMsgUds, unsigned char abByteArr[], UINT ByteArrLen /*, UDS_INTERFACE FInterface,UINT targetAddress*/ );

    /**
    *This function is called to send the flow control because it has been received a long response  */
    (void) CUDSMainWnd::PrepareFlowControl(void);


    CWnd* pomGetTxMsgViewPointers() const;

    void vSetDILInterfacePtr(void* ptrDILIntrf);
    void* pGetDILInterfacePtr();
    /**
    \brief It updates  in the UDS Main Window the channels available
    * This function is called from the Mainfrm when the user has changed the channel selection
    */
    void vUpdateChannelIDInfo();

    static CUDSMainWnd* s_podGetUdsMsgManager();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg LRESULT OnCommandHelp ( WPARAM wParam, LPARAM lParam );


    void CUDSMainWnd::SendSimpleDiagnosticMessagePanels(mPSTXSELMSGDATA pstx);



    /**
    \brief it indicates if the tool should wait for a response from the ECU
    * This funcition is called to evaluate if the sent message waits for
    * a response or not (No Positive Response Required)
    */
    CString initialEval(CString Data2Send );


    /**
    *This function is called to start the timer used to send the Tester Present
    */
    UINT StartTimer();


private:
    //CUdsMsgBlocksView* m_pomUDSMsgBlocksView;
    CWnd* pomGetBlocksViewPointer() const;

    /// To Get Parent Window Pointer
    CWnd* pomGetParentWindow() const;

    /**
    \brief This function is used to calulate the difference in time between two specified moments
    * This funcition is called to calculate the difference in time
    * between two consecutive frames and to send them each STmin ms.
    */
    void CUDSMainWnd::CalculateDiffTime(void);

    /**
    \brief This function is used by  CalculateDiffTime to get the current time
    */
    int  CUDSMainWnd::nCalculateCurrTime(BOOL bFromDIL);

    // Dialog Data
    enum { IDD = IDM_UDS };

protected:
    virtual BOOL OnInitDialog();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    /**
    \brief  This function transmita any simple message
    */
    static UINT OnSendSelectedMsg(LPVOID pParam);

    DECLARE_MESSAGE_MAP()
};
