/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      TxMsgWndJ1939.h
 * \author    Pradeep Kadoor
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 */

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
