/******************************************************************************
  Project       :  Auto-SAT_Tools
  FileName      :  TxMsgWndJ1939.h
  Description   :  
  $Log:   X:/Archive/Sources/Application/TxMsgWndJ1939.h_v  $
 * 
 *    Rev 1.7   15 Apr 2011 20:02:20   CANMNTTM
 * Added RBEI Copyright information.
 * 
 *    Rev 1.6   23 Mar 2011 14:49:22   CANMNTTM
 * Support upto 32 channels
 * 
 *    Rev 1.5   13 Jan 2011 14:55:50   CANMNTTM
 * Implemented J1939 database specific functions.
 * 
 *    Rev 1.4   23 Dec 2010 16:43:26   CANMNTTM
 * Transmission stopped before disconnecting.
 * 
 *    Rev 1.3   22 Dec 2010 19:13:48   CANMNTTM
 * Added Cyclic transmission feature.
 * 
 *    Rev 1.2   15 Dec 2010 17:06:46   CANMNTTM
 * Added new function to set J1939 client parameter.
 * 
 *    Rev 1.1   13 Dec 2010 19:06:22   CANMNTTM
 * Removed absolute path inclusion
 * 
 *    Rev 1.0   13 Dec 2010 18:41:54   CANMNTTM
 *  

  Author(s)     :  Pradeep Kadoor
  Date Created  :  10/12/2010
  Modified By   :  
  Copyright (c) 2011, Robert Bosch Engineering and Business Solutions.  All rights reserved.
******************************************************************************/
#pragma once

#include "Utility/RadixEdit.h"
#include "Datatypes/MsgSignal_Datatypes.h"

enum ESTATE_TRANS
{
    TRANS_STOPPED = 0x0,
    TRANS_TO_BE_STOPPED,
    TRANS_STARTED
};
typedef struct tagMsgToBeSent
{
    UINT32 m_unPGN;
    EJ1939_MSG_TYPE m_eMsgType;
    UINT m_unChannel;
    BYTE m_bySrc;
    BYTE m_byDest;
    UINT m_unDLC;
    BYTE m_byPriority;
    BYTE m_abyData[MAX_DATA_LEN_J1939];
    UINT m_unTimerVal;
}SMSGTOBESENT;

// CTxMsgWndJ1939 dialog

class CTxMsgWndJ1939 : public CDialog
{
	DECLARE_DYNAMIC(CTxMsgWndJ1939)

public:
    CTxMsgWndJ1939(CWnd* pParent, SJ1939CLIENTPARAM& sClientParam);   // standard constructor
	virtual ~CTxMsgWndJ1939();

// Dialog Data
	enum { IDD = IDD_DLG_TX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedSend();
    afx_msg void OnBnClickedOk();
    afx_msg void OnEnChangeEditDlc();
    afx_msg void OnEnChangeEditMsg();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

public:
    CComboBox   m_omMsgTypeCombo;
    CRadixEdit  m_omDLCEdit;
    CRadixEdit  m_omMsgDataEdit;
    CString m_omMsgDataEditVal;
    UINT m_unDataLength;
    UINT m_unSelMsgHandler;
    int m_nMsgTypeIndex;
    SMSGTOBESENT m_sMsgToBeSent;
    CPARAM_THREADPROC m_sCyclicThread;
    BOOL m_bThreadStarted;
    CCriticalSection m_CS_CyclicTrans;
    CCriticalSection m_CS_ConfigData;
    
public:
    afx_msg void OnBnClickedRadioNm();
    afx_msg void OnBnClickedRadioTpf();
    void vSetJ1939ClientParam(SJ1939CLIENTPARAM& sClientParam);
    HRESULT SendSavedMessage(void);
    ESTATE_TRANS eGetTransState(void);
    void vSetTransState(ESTATE_TRANS eTransState);
    UINT unGetTimerVal(void);
    void vProcessTransmission(BOOL bStart);
    EJ1939_MSG_TYPE eGetCurrMsgType(void);
    void vSetDatabaseInfo(const SMSGENTRY* psMsgEntry);
    static void vUpdateDataStore(const SMSGENTRY* psMsgEntry);
    static void vClearDataStore(void);
private:
    static SMSGENTRY* m_psMsgRoot;

    SJ1939CLIENTPARAM m_sClientParams;
    BOOL m_bNM;
    ESTATE_TRANS m_eTransState;

    void vEnableNmFields(BOOL bEnable);
    void vEnableTpfFields(BOOL bEnable);
    void vInitializeNmFields(void);
    void vInitializeTpfFields(void);
    void vSetStatusBarText(const TCHAR* pacText);
    void vPopulatePGNComboBox(void);
public:
    // Control variable for ECU NAME
    CRadixEdit m_omEcuName;
    CRadixEdit m_omCurAddress;
    afx_msg void OnClose();
    afx_msg void OnBnClickedClaimAddress();
    afx_msg void OnBnClickedRqstAddress();
    afx_msg void OnBnClickedCmdAddress();
    afx_msg void OnCbnSelchangeComboMsgtype();
    CRadixEdit m_omFromEdit;
    CRadixEdit m_omPriorityEdit;
    CRadixEdit m_omTOEdit;
    //CRadixEdit m_omPGNEdit;
    CComboBox m_omComboChannel;
    afx_msg void OnBnClickedCheckCyclic();
    afx_msg LRESULT OnMessageConnect(WPARAM /*lParam*/, LPARAM /*wParam*/);
    CRadixEdit m_omMiliSecs;
    CButton m_omCheckCyclic;
    CComboBox m_omComboPGN;
    afx_msg void OnCbnSelchangeComboPgn();
};
