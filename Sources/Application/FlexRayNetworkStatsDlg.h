/**********************************************************************************
    Project         :   Frame
    FileName        :   NetworkStatsDlg.h
    Description     :   Definition of CFlexRayNetworkStatsDlg class
    Directory       :
    Version         :   V1.0
    Authors         :   Ravi D
    Date Created    :   17/10/2006
    Date Modified   :
    Modifications   :
**********************************************************************************/

#pragma once
#include "StdAfx.h"
#include "resource.h"
#include "MessageList.h"
#include "FlexRayNetworkStats.h"


// CFlexRayNetworkStatsDlg dialog

class CFlexRayNetworkStatsDlg : public CDialog
{
    DECLARE_DYNAMIC(CFlexRayNetworkStatsDlg)

private:
    CFlexRayNetworkStats* m_pouNetworkStat;
    HWND m_hParent;
    void vFillTxRxContent();

public:
    // standard constructor
    CFlexRayNetworkStatsDlg(CFlexRayNetworkStats* pNetworkStat, CWnd* pParent = nullptr);
    virtual ~CFlexRayNetworkStatsDlg();

    void GetConfigData(BYTE** ppvConfigData, UINT& unLength);
    void SetConfigData(BYTE* pvDataStream);

    // Dialog Data
    enum { IDD = IDD_DLG_NETWORK_STATISTICS };
    afx_msg LRESULT OnUpdateNetworkStat(WPARAM, LPARAM);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()

public:
    virtual BOOL OnInitDialog();
    CMessageList m_lstNetWorkStat;

    afx_msg void OnBnClickedOk();
    afx_msg void OnDestroy();
    afx_msg void OnSize(UINT nType, int cx, int cy);

    virtual BOOL PreTranslateMessage(MSG* pMsg);
    void vDisplayStats(SSUBFLEXRAYBUSSTATISTICS* psBusStats, int nStartIndex);
};
